/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Stuart Parmenter <stuart@mozilla.com>
 *   Federico Mena-Quintero <federico@novell.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsJPEGDecoder.h"

#include "imgIContainerObserver.h"

#include "nsIComponentManager.h"
#include "nsIInputStream.h"

#include "nspr.h"
#include "nsCRT.h"
#include "ImageLogging.h"
#include "nsIImage.h"
#include "nsIInterfaceRequestorUtils.h"
#include "gfxColor.h"

#include "jerror.h"

#include "gfxPlatform.h"

extern "C" {
#include "iccjpeg.h"

/* Colorspace conversion (copied from jpegint.h) */
struct jpeg_color_deconverter {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, color_convert, (j_decompress_ptr cinfo,
				JSAMPIMAGE input_buf, JDIMENSION input_row,
				JSAMPARRAY output_buf, int num_rows));
};

METHODDEF(void)
ycc_rgb_convert_argb (j_decompress_ptr cinfo,
                 JSAMPIMAGE input_buf, JDIMENSION input_row,
                 JSAMPARRAY output_buf, int num_rows);
}

NS_IMPL_ISUPPORTS1(nsJPEGDecoder, imgIDecoder)

#if defined(PR_LOGGING)
PRLogModuleInfo *gJPEGlog = PR_NewLogModule("JPEGDecoder");
static PRLogModuleInfo *gJPEGDecoderAccountingLog = PR_NewLogModule("JPEGDecoderAccounting");
#else
#define gJPEGlog
#define gJPEGDecoderAccountingLog
#endif


METHODDEF(void) init_source (j_decompress_ptr jd);
METHODDEF(boolean) fill_input_buffer (j_decompress_ptr jd);
METHODDEF(void) skip_input_data (j_decompress_ptr jd, long num_bytes);
METHODDEF(void) term_source (j_decompress_ptr jd);
METHODDEF(void) my_error_exit (j_common_ptr cinfo);

static void cmyk_convert_rgb(JSAMPROW row, JDIMENSION width);

/* Normal JFIF markers can't have more bytes than this. */
#define MAX_JPEG_MARKER_LENGTH  (((PRUint32)1 << 16) - 1)


nsJPEGDecoder::nsJPEGDecoder()
{
  mState = JPEG_HEADER;
  mReading = PR_TRUE;
  mError = NS_OK;

  mBytesToSkip = 0;
  memset(&mInfo, 0, sizeof(jpeg_decompress_struct));
  memset(&mSourceMgr, 0, sizeof(mSourceMgr));
  mInfo.client_data = (void*)this;

  mSegment = nsnull;
  mSegmentLen = 0;

  mBackBuffer = nsnull;
  mBackBufferLen = mBackBufferSize = mBackBufferUnreadLen = 0;

  mInProfile = nsnull;
  mTransform = nsnull;

  PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
         ("nsJPEGDecoder::nsJPEGDecoder: Creating JPEG decoder %p",
          this));
}

nsJPEGDecoder::~nsJPEGDecoder()
{
  PR_FREEIF(mBackBuffer);
  if (mTransform)
    cmsDeleteTransform(mTransform);
  if (mInProfile)
    cmsCloseProfile(mInProfile);

  PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
         ("nsJPEGDecoder::~nsJPEGDecoder: Destroying JPEG decoder %p",
          this));
}


/** imgIDecoder methods **/

/* void init (in imgILoad aLoad); */
NS_IMETHODIMP nsJPEGDecoder::Init(imgILoad *aLoad)
{
  mImageLoad = aLoad;
  mObserver = do_QueryInterface(aLoad);

  /* We set up the normal JPEG error routines, then override error_exit. */
  mInfo.err = jpeg_std_error(&mErr.pub);
  /*   mInfo.err = jpeg_std_error(&mErr.pub); */
  mErr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(mErr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    return NS_ERROR_FAILURE;
  }

  /* Step 1: allocate and initialize JPEG decompression object */
  jpeg_create_decompress(&mInfo);
  /* Set the source manager */
  mInfo.src = &mSourceMgr;

  /* Step 2: specify data source (eg, a file) */

  /* Setup callback functions. */
  mSourceMgr.init_source = init_source;
  mSourceMgr.fill_input_buffer = fill_input_buffer;
  mSourceMgr.skip_input_data = skip_input_data;
  mSourceMgr.resync_to_restart = jpeg_resync_to_restart;
  mSourceMgr.term_source = term_source;

  /* Record app markers for ICC data */
  for (PRUint32 m = 0; m < 16; m++)
    jpeg_save_markers(&mInfo, JPEG_APP0 + m, 0xFFFF);



  /* Check if the request already has an image container.
   * this is the case when multipart/x-mixed-replace is being downloaded
   * if we already have one and it has the same width and height, reuse it.
   * This is also the case when an existing container is reloading itself from
   * us.
   *
   * If we have a mismatch in width/height for the container later on we will
   * generate an error.
   */
  mImageLoad->GetImage(getter_AddRefs(mImage));

  if (!mImage) {
    mImage = do_CreateInstance("@mozilla.org/image/container;1");
    if (!mImage)
      return NS_ERROR_OUT_OF_MEMORY;
      
    mImageLoad->SetImage(mImage);

    // Don't discard if we're multipart, and assume we are for safety.
    PRBool multipart = PR_TRUE;
    if (NS_SUCCEEDED(mImageLoad->GetIsMultiPartChannel(&multipart)) && !multipart) {
      nsresult result = mImage->SetDiscardable("image/jpeg");
      if (NS_FAILED(result)) {
        mState = JPEG_ERROR;
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               (" (could not set image container to discardable)"));
        return result;
      }
    }
  }

  return NS_OK;
}


/* void close (); */
NS_IMETHODIMP nsJPEGDecoder::Close()
{
  PR_LOG(gJPEGlog, PR_LOG_DEBUG,
         ("[this=%p] nsJPEGDecoder::Close\n", this));

  /* Step 8: Release JPEG decompression object */
  mInfo.src = nsnull;

  jpeg_destroy_decompress(&mInfo);

  if (mState != JPEG_DONE && mState != JPEG_SINK_NON_JPEG_TRAILER) {
    NS_WARNING("Never finished decoding the JPEG.");
    /* Tell imgLoader that image decoding has failed */
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

/* void flush (); */
NS_IMETHODIMP nsJPEGDecoder::Flush()
{
  LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::Flush");

  PRUint32 ret;
  if (mState != JPEG_DONE && mState != JPEG_SINK_NON_JPEG_TRAILER && mState != JPEG_ERROR)
    return this->ProcessData(nsnull, 0, &ret);

  return NS_OK;
}

static NS_METHOD ReadDataOut(nsIInputStream* in,
                             void* closure,
                             const char* fromRawSegment,
                             PRUint32 toOffset,
                             PRUint32 count,
                             PRUint32 *writeCount)
{
  nsJPEGDecoder *decoder = static_cast<nsJPEGDecoder*>(closure);
  nsresult rv = decoder->ProcessData(fromRawSegment, count, writeCount);
  if (NS_FAILED(rv)) {
    /* Tell imgLoader that image decoding has failed */
    decoder->mError = rv;
    *writeCount = 0;
  }

  return NS_OK;
}


/* unsigned long writeFrom (in nsIInputStream inStr, in unsigned long count); */
NS_IMETHODIMP nsJPEGDecoder::WriteFrom(nsIInputStream *inStr, PRUint32 count, PRUint32 *writeCount)
{
  NS_ENSURE_ARG_POINTER(inStr);
  NS_ENSURE_ARG_POINTER(writeCount);

  /* necko doesn't propagate the errors from ReadDataOut */
  nsresult rv = inStr->ReadSegments(ReadDataOut, this, count, writeCount);
  if (NS_FAILED(mError)) {
    /* Tell imgLoader that image decoding has failed */
    rv = NS_ERROR_FAILURE;
  }

  return rv;
}

//******************************************************************************
nsresult nsJPEGDecoder::ProcessData(const char *data, PRUint32 count, PRUint32 *writeCount)
{
  LOG_SCOPE_WITH_PARAM(gJPEGlog, "nsJPEGDecoder::ProcessData", "count", count);

  mSegment = (const JOCTET *)data;
  mSegmentLen = count;
  *writeCount = count;

  if (data && count) {
    nsresult result = mImage->AddRestoreData((char *) data, count);

    if (NS_FAILED(result)) {
      mState = JPEG_ERROR;
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (could not add restore data)"));
      return result;
    }

    PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
           ("        added %u bytes to restore data",
            count));
  }
  // else no input stream.. Flush() ?

  /* Return here if there is a fatal error. */
  nsresult error_code;
  if ((error_code = setjmp(mErr.setjmp_buffer)) != 0) {
    mState = JPEG_SINK_NON_JPEG_TRAILER;
    if (error_code == NS_ERROR_FAILURE) {
      /* Error due to corrupt stream - return NS_OK so that libpr0n
         doesn't throw away a partial image load */
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (setjmp returned NS_ERROR_FAILURE)"));
      return NS_OK;
    } else {
      /* Error due to reasons external to the stream (probably out of
         memory) - let libpr0n attempt to clean up, even though
         mozilla is seconds away from falling flat on its face. */
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (setjmp returned an error)"));
      return error_code;
    }
  }

  PR_LOG(gJPEGlog, PR_LOG_DEBUG,
         ("[this=%p] nsJPEGDecoder::ProcessData -- processing JPEG data\n", this));

  switch (mState) {
  case JPEG_HEADER:
  {
    LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::ProcessData -- entering JPEG_HEADER case");

    /* Step 3: read file parameters with jpeg_read_header() */
    if (jpeg_read_header(&mInfo, TRUE) == JPEG_SUSPENDED) {
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (JPEG_SUSPENDED)"));
      return NS_OK; /* I/O suspension */
    }

    JOCTET  *profile;
    PRUint32 profileLength;

    if (gfxPlatform::IsCMSEnabled() &&
        read_icc_profile(&mInfo, &profile, &profileLength) &&
        (mInProfile = cmsOpenProfileFromMem(profile, profileLength)) != NULL) {
      free(profile);

      PRUint32 profileSpace = cmsGetColorSpace(mInProfile);
      PRBool mismatch = PR_FALSE;

#ifdef DEBUG_tor
      fprintf(stderr, "JPEG profileSpace: 0x%08X\n", profileSpace);
#endif
      switch (mInfo.jpeg_color_space) {
      case JCS_GRAYSCALE:
        if (profileSpace == icSigRgbData)
          mInfo.out_color_space = JCS_RGB;
        else if (profileSpace != icSigGrayData)
          mismatch = PR_TRUE;
        break;
      case JCS_RGB:
        if (profileSpace != icSigRgbData)
          mismatch =  PR_TRUE;
        break;
      case JCS_YCbCr:
        if (profileSpace == icSigRgbData)
          mInfo.out_color_space = JCS_RGB;
        else if (profileSpace != icSigYCbCrData)
          mismatch = PR_TRUE;
        break;
      case JCS_CMYK:
      case JCS_YCCK:
        if (profileSpace == icSigCmykData)
          mInfo.out_color_space = JCS_CMYK;
        else
          mismatch = PR_TRUE;
        break;
      default:
        mState = JPEG_ERROR;
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               ("} (unknown colorpsace (1))"));
        return NS_ERROR_UNEXPECTED;
      }

      if (!mismatch) {
        PRUint32 type;
        switch (mInfo.out_color_space) {
        case JCS_GRAYSCALE:
          type = COLORSPACE_SH(PT_GRAY)  | CHANNELS_SH(1) | BYTES_SH(1);
          break;
        case JCS_RGB:
          type = COLORSPACE_SH(PT_RGB)   | CHANNELS_SH(3) | BYTES_SH(1);
          break;
        case JCS_YCbCr:
          type = COLORSPACE_SH(PT_YCbCr) | CHANNELS_SH(3) | BYTES_SH(1);
          break;
        case JCS_CMYK:
          type = COLORSPACE_SH(PT_CMYK)  | CHANNELS_SH(4) | BYTES_SH(1);
          break;
        default:
          mState = JPEG_ERROR;
          PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
                 ("} (unknown colorpsace (2))"));
          return NS_ERROR_UNEXPECTED;
        }

        /* Adobe Photoshop writes YCCK/CMYK files with inverted data */
        if (mInfo.out_color_space == JCS_CMYK)
          type |= FLAVOR_SH(mInfo.saw_Adobe_marker ? 1 : 0);

        if (gfxPlatform::GetCMSOutputProfile())
          mTransform = cmsCreateTransform(mInProfile,
                                          type,
                                          gfxPlatform::GetCMSOutputProfile(),
                                          TYPE_RGB_8,
                                          cmsTakeRenderingIntent(mInProfile),
                                          0);
      } else {
#ifdef DEBUG_tor
        fprintf(stderr, "ICM profile colorspace mismatch\n");
#endif
      }
    }

    if (!mTransform) {
      switch (mInfo.jpeg_color_space) {
      case JCS_GRAYSCALE:
      case JCS_RGB:
      case JCS_YCbCr:
        mInfo.out_color_space = JCS_RGB;
        break;
      case JCS_CMYK:
      case JCS_YCCK:
        /* libjpeg can convert from YCCK to CMYK, but not to RGB */
        mInfo.out_color_space = JCS_CMYK;
        break;
      default:
        mState = JPEG_ERROR;
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               ("} (unknown colorpsace (3))"));
        return NS_ERROR_UNEXPECTED;
        break;
      }
    }

    /*
     * Don't allocate a giant and superfluous memory buffer
     * when the image is a sequential JPEG.
     */
    mInfo.buffered_image = jpeg_has_multiple_scans(&mInfo);

    /* Used to set up image size so arrays can be allocated */
    jpeg_calc_output_dimensions(&mInfo);

    mObserver->OnStartDecode(nsnull);

    /* verify that the width and height of the image are the same as
     * the container we're about to put things in to.
     * XXX it might not matter maybe we should just resize the image.
     */
    PRInt32 width, height;
    mImage->GetWidth(&width);
    mImage->GetHeight(&height);
    if (width == 0 && height == 0) {
      mImage->Init(mInfo.image_width, mInfo.image_height, mObserver);
    } else if ((width != (PRInt32)mInfo.image_width) || (height != (PRInt32)mInfo.image_height)) {
      mState = JPEG_ERROR;
      return NS_ERROR_UNEXPECTED;
    }

    mImage->Init(mInfo.image_width, mInfo.image_height, mObserver);

    mObserver->OnStartContainer(nsnull, mImage);

    mImage->GetFrameAt(0, getter_AddRefs(mFrame));

    PRBool createNewFrame = PR_TRUE;

    if (mFrame) {
      PRInt32 width, height;
      mFrame->GetWidth(&width);
      mFrame->GetHeight(&height);

      if ((width == (PRInt32)mInfo.image_width) &&
          (height == (PRInt32)mInfo.image_height)) {
        createNewFrame = PR_FALSE;
      } else {
        mImage->Clear();
      }
    }

    if (createNewFrame) {
      mFrame = do_CreateInstance("@mozilla.org/gfx/image/frame;2");
      if (!mFrame) {
        mState = JPEG_ERROR;
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               ("} (could not create image frame)"));
        return NS_ERROR_OUT_OF_MEMORY;
      }

      gfx_format format = gfxIFormats::RGB;
#if defined(XP_WIN) || defined(XP_OS2) || defined(XP_BEOS)
      format = gfxIFormats::BGR;
#endif

      if (NS_FAILED(mFrame->Init(0, 0, mInfo.image_width, mInfo.image_height, format, 24))) {
        mState = JPEG_ERROR;
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               ("} (could not initialize image frame)"));
        return NS_ERROR_OUT_OF_MEMORY;
      }

      mImage->AppendFrame(mFrame);

      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("        JPEGDecoderAccounting: nsJPEGDecoder::ProcessData -- created image frame with %ux%u pixels",
              mInfo.image_width, mInfo.image_height));
    }

    mObserver->OnStartFrame(nsnull, mFrame);
    mState = JPEG_START_DECOMPRESS;
  }

  case JPEG_START_DECOMPRESS:
  {
    LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::ProcessData -- entering JPEG_START_DECOMPRESS case");
    /* Step 4: set parameters for decompression */

    /* FIXME -- Should reset dct_method and dither mode
     * for final pass of progressive JPEG
     */
    mInfo.dct_method =  JDCT_ISLOW;
    mInfo.dither_mode = JDITHER_FS;
    mInfo.do_fancy_upsampling = TRUE;
    mInfo.enable_2pass_quant = FALSE;
    mInfo.do_block_smoothing = TRUE;

    /* Step 5: Start decompressor */
    if (jpeg_start_decompress(&mInfo) == FALSE) {
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (I/O suspension after jpeg_start_decompress())"));
      return NS_OK; /* I/O suspension */
    }

    /* Force to use our YCbCr to Packed RGB converter when possible */
    if (!mTransform && !gfxPlatform::IsCMSEnabled() &&
        mInfo.jpeg_color_space == JCS_YCbCr && mInfo.out_color_space == JCS_RGB) {
      /* Special case for the most common case: transform from YCbCr direct into packed ARGB */
      mInfo.out_color_components = 4; /* Packed ARGB pixels are always 4 bytes...*/
      mInfo.cconvert->color_convert = ycc_rgb_convert_argb;
    }

    /* If this is a progressive JPEG ... */
    mState = mInfo.buffered_image ? JPEG_DECOMPRESS_PROGRESSIVE : JPEG_DECOMPRESS_SEQUENTIAL;
  }

  case JPEG_DECOMPRESS_SEQUENTIAL:
  {
    if (mState == JPEG_DECOMPRESS_SEQUENTIAL)
    {
      LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::ProcessData -- JPEG_DECOMPRESS_SEQUENTIAL case");
      
      if (!OutputScanlines()) {
        PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
               ("} (I/O suspension after OutputScanlines() - SEQUENTIAL)"));
        return NS_OK; /* I/O suspension */
      }
      
      /* If we've completed image output ... */
      NS_ASSERTION(mInfo.output_scanline == mInfo.output_height, "We didn't process all of the data!");
      mState = JPEG_DONE;
    }
  }

  case JPEG_DECOMPRESS_PROGRESSIVE:
  {
    if (mState == JPEG_DECOMPRESS_PROGRESSIVE)
    {
      LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::ProcessData -- JPEG_DECOMPRESS_PROGRESSIVE case");

      int status;
      do {
        status = jpeg_consume_input(&mInfo);
      } while ((status != JPEG_SUSPENDED) &&
               (status != JPEG_REACHED_EOI));

      for (;;) {
        if (mInfo.output_scanline == 0) {
          int scan = mInfo.input_scan_number;

          /* if we haven't displayed anything yet (output_scan_number==0)
             and we have enough data for a complete scan, force output
             of the last full scan */
          if ((mInfo.output_scan_number == 0) &&
              (scan > 1) &&
              (status != JPEG_REACHED_EOI))
            scan--;

          if (!jpeg_start_output(&mInfo, scan)) {
            PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
                   ("} (I/O suspension after jpeg_start_output() - PROGRESSIVE)"));
            return NS_OK; /* I/O suspension */
          }
        }

        if (mInfo.output_scanline == 0xffffff)
          mInfo.output_scanline = 0;

        if (!OutputScanlines()) {
          if (mInfo.output_scanline == 0) {
            /* didn't manage to read any lines - flag so we don't call
               jpeg_start_output() multiple times for the same scan */
            mInfo.output_scanline = 0xffffff;
          }
          PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
                 ("} (I/O suspension after OutputScanlines() - PROGRESSIVE)"));
          return NS_OK; /* I/O suspension */
        }

        if (mInfo.output_scanline == mInfo.output_height)
        {
          if (!jpeg_finish_output(&mInfo)) {
            PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
                   ("} (I/O suspension after jpeg_finish_output() - PROGRESSIVE)"));
            return NS_OK; /* I/O suspension */
          }

          if (jpeg_input_complete(&mInfo) &&
              (mInfo.input_scan_number == mInfo.output_scan_number))
            break;

          mInfo.output_scanline = 0;
        }
      }

      mState = JPEG_DONE;
    }
  }

  case JPEG_DONE:
  {
    nsresult result;

    LOG_SCOPE(gJPEGlog, "nsJPEGDecoder::ProcessData -- entering JPEG_DONE case");

    /* Step 7: Finish decompression */

    if (jpeg_finish_decompress(&mInfo) == FALSE) {
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (I/O suspension after jpeg_finish_decompress() - DONE)"));
      return NS_OK; /* I/O suspension */
    }

    mState = JPEG_SINK_NON_JPEG_TRAILER;

    result = mImage->RestoreDataDone();
    if (NS_FAILED (result)) {
      mState = JPEG_ERROR;
      PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
             ("} (could not mark image container with RestoreDataDone)"));
      return result;
    }

    /* we're done dude */
    break;
  }
  case JPEG_SINK_NON_JPEG_TRAILER:
    PR_LOG(gJPEGlog, PR_LOG_DEBUG,
           ("[this=%p] nsJPEGDecoder::ProcessData -- entering JPEG_SINK_NON_JPEG_TRAILER case\n", this));

    break;

  case JPEG_ERROR:
    PR_LOG(gJPEGlog, PR_LOG_DEBUG,
           ("[this=%p] nsJPEGDecoder::ProcessData -- entering JPEG_ERROR case\n", this));

    break;
  }

  PR_LOG(gJPEGDecoderAccountingLog, PR_LOG_DEBUG,
         ("} (end of function)"));
  return NS_OK;
}


PRBool
nsJPEGDecoder::OutputScanlines()
{
  const PRUint32 top = mInfo.output_scanline;
  PRBool rv = PR_TRUE;

  mFrame->LockImageData();
  
  // we're thebes. we can write stuff directly to the data
  PRUint8 *imageData;
  PRUint32 imageDataLength;
  mFrame->GetImageData(&imageData, &imageDataLength);

  while ((mInfo.output_scanline < mInfo.output_height)) {
      /* Use the Cairo image buffer as scanline buffer */
      PRUint32 *imageRow = ((PRUint32*)imageData) +
                           (mInfo.output_scanline * mInfo.output_width);

      if (mInfo.cconvert->color_convert == ycc_rgb_convert_argb) {
        /* Special case: scanline will be directly converted into packed ARGB */
        if (jpeg_read_scanlines(&mInfo, (JSAMPARRAY)&imageRow, 1) != 1) {
          rv = PR_FALSE; /* suspend */
          break;
        }
        continue; /* all done for this row! */
      }

      JSAMPROW sampleRow = (JSAMPROW)imageRow;
      if (mInfo.output_components == 3) {
        /* Put the pixels at end of row to enable in-place expansion */
        sampleRow += mInfo.output_width;
      }

      /* Request one scanline.  Returns 0 or 1 scanlines. */    
      if (jpeg_read_scanlines(&mInfo, &sampleRow, 1) != 1) {
        rv = PR_FALSE; /* suspend */
        break;
      }

      if (mTransform) {
        JSAMPROW source = sampleRow;
        if (mInfo.out_color_space == JCS_GRAYSCALE) {
          /* Convert from the 1byte grey pixels at begin of row 
             to the 3byte RGB byte pixels at 'end' of row */
          sampleRow += mInfo.output_width;
        }
        cmsDoTransform(mTransform, source, sampleRow, mInfo.output_width);
        /* Move 3byte RGB data to end of row */
        if (mInfo.out_color_space == JCS_CMYK) {
          memmove(sampleRow + mInfo.output_width,
                  sampleRow,
                  3 * mInfo.output_width);
          sampleRow += mInfo.output_width;
        }
      } else {
        if (mInfo.out_color_space == JCS_CMYK) {
          /* Convert from CMYK to RGB */
          /* We cannot convert directly to Cairo, as the CMSRGBTransform may wants to do a RGB transform... */
          /* Would be better to have platform CMSenabled transformation from CMYK to (A)RGB... */
          cmyk_convert_rgb((JSAMPROW)imageRow, mInfo.output_width);
          sampleRow += mInfo.output_width;
        }
        if (gfxPlatform::IsCMSEnabled()) {
          /* No embedded ICC profile - treat as sRGB */
          cmsHTRANSFORM transform = gfxPlatform::GetCMSRGBTransform();
          if (transform) {
            cmsDoTransform(transform, sampleRow, sampleRow, mInfo.output_width);
          }
        }
      }

      // counter for while() loops below
      PRUint32 idx = mInfo.output_width;

      // copy as bytes until source pointer is 32-bit-aligned
      for (; (NS_PTR_TO_UINT32(sampleRow) & 0x3) && idx; --idx) {
        *imageRow++ = GFX_PACKED_PIXEL(0xFF, sampleRow[0], sampleRow[1], sampleRow[2]);
        sampleRow += 3;
      }

      // copy pixels in blocks of 4
      while (idx >= 4) {
        GFX_BLOCK_RGB_TO_FRGB(sampleRow, imageRow);
        idx       -=  4;
        sampleRow += 12;
        imageRow  +=  4;
      }

      // copy remaining pixel(s)
      while (idx--) {
        // 32-bit read of final pixel will exceed buffer, so read bytes
        *imageRow++ = GFX_PACKED_PIXEL(0xFF, sampleRow[0], sampleRow[1], sampleRow[2]);
        sampleRow += 3;
      }
  }

  if (top != mInfo.output_scanline) {
      nsIntRect r(0, top, mInfo.output_width, mInfo.output_scanline-top);
      nsCOMPtr<nsIImage> img(do_GetInterface(mFrame));
      img->ImageUpdated(nsnull, nsImageUpdateFlags_kBitsChanged, &r);
      mObserver->OnDataAvailable(nsnull, mFrame, &r);
  }
  
  mFrame->UnlockImageData();
  
  return rv;
}


/* Override the standard error method in the IJG JPEG decoder code. */
METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  decoder_error_mgr *err = (decoder_error_mgr *) cinfo->err;

  /* Convert error to a browser error code */
  nsresult error_code = err->pub.msg_code == JERR_OUT_OF_MEMORY
                      ? NS_ERROR_OUT_OF_MEMORY
                      : NS_ERROR_FAILURE;

#ifdef DEBUG
  char buffer[JMSG_LENGTH_MAX];

  /* Create the message */
  (*err->pub.format_message) (cinfo, buffer);

  fprintf(stderr, "JPEG decoding error:\n%s\n", buffer);
#endif

  /* Return control to the setjmp point. */
  longjmp(err->setjmp_buffer, error_code);
}

/******************************************************************************/
/*-----------------------------------------------------------------------------
 * This is the callback routine from the IJG JPEG library used to supply new
 * data to the decompressor when its input buffer is exhausted.  It juggles
 * multiple buffers in an attempt to avoid unnecessary copying of input data.
 *
 * (A simpler scheme is possible: It's much easier to use only a single
 * buffer; when fill_input_buffer() is called, move any unconsumed data
 * (beyond the current pointer/count) down to the beginning of this buffer and
 * then load new data into the remaining buffer space.  This approach requires
 * a little more data copying but is far easier to get right.)
 *
 * At any one time, the JPEG decompressor is either reading from the necko
 * input buffer, which is volatile across top-level calls to the IJG library,
 * or the "backtrack" buffer.  The backtrack buffer contains the remaining
 * unconsumed data from the necko buffer after parsing was suspended due
 * to insufficient data in some previous call to the IJG library.
 *
 * When suspending, the decompressor will back up to a convenient restart
 * point (typically the start of the current MCU). The variables
 * next_input_byte & bytes_in_buffer indicate where the restart point will be
 * if the current call returns FALSE.  Data beyond this point must be
 * rescanned after resumption, so it must be preserved in case the decompressor
 * decides to backtrack.
 *
 * Returns:
 *  TRUE if additional data is available, FALSE if no data present and
 *   the JPEG library should therefore suspend processing of input stream
 *---------------------------------------------------------------------------*/

/******************************************************************************/
/* data source manager method                                                 */
/******************************************************************************/


/******************************************************************************/
/* data source manager method 
        Initialize source.  This is called by jpeg_read_header() before any
        data is actually read.  May leave
        bytes_in_buffer set to 0 (in which case a fill_input_buffer() call
        will occur immediately).
*/
METHODDEF(void)
init_source (j_decompress_ptr jd)
{
}

/******************************************************************************/
/* data source manager method
        Skip num_bytes worth of data.  The buffer pointer and count should
        be advanced over num_bytes input bytes, refilling the buffer as
        needed.  This is used to skip over a potentially large amount of
        uninteresting data (such as an APPn marker).  In some applications
        it may be possible to optimize away the reading of the skipped data,
        but it's not clear that being smart is worth much trouble; large
        skips are uncommon.  bytes_in_buffer may be zero on return.
        A zero or negative skip count should be treated as a no-op.
*/
METHODDEF(void)
skip_input_data (j_decompress_ptr jd, long num_bytes)
{
  struct jpeg_source_mgr *src = jd->src;
  nsJPEGDecoder *decoder = (nsJPEGDecoder *)(jd->client_data);

  if (num_bytes > (long)src->bytes_in_buffer) {
    /*
     * Can't skip it all right now until we get more data from
     * network stream. Set things up so that fill_input_buffer
     * will skip remaining amount.
     */
    decoder->mBytesToSkip = (size_t)num_bytes - src->bytes_in_buffer;
    src->next_input_byte += src->bytes_in_buffer;
    src->bytes_in_buffer = 0;

  } else {
    /* Simple case. Just advance buffer pointer */

    src->bytes_in_buffer -= (size_t)num_bytes;
    src->next_input_byte += num_bytes;
  }
}


/******************************************************************************/
/* data source manager method
        This is called whenever bytes_in_buffer has reached zero and more
        data is wanted.  In typical applications, it should read fresh data
        into the buffer (ignoring the current state of next_input_byte and
        bytes_in_buffer), reset the pointer & count to the start of the
        buffer, and return TRUE indicating that the buffer has been reloaded.
        It is not necessary to fill the buffer entirely, only to obtain at
        least one more byte.  bytes_in_buffer MUST be set to a positive value
        if TRUE is returned.  A FALSE return should only be used when I/O
        suspension is desired.
*/
METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr jd)
{
  struct jpeg_source_mgr *src = jd->src;
  nsJPEGDecoder *decoder = (nsJPEGDecoder *)(jd->client_data);

  if (decoder->mReading) {
    const JOCTET *new_buffer = decoder->mSegment;
    PRUint32 new_buflen = decoder->mSegmentLen;
  
    if (!new_buffer || new_buflen == 0)
      return PR_FALSE; /* suspend */

    decoder->mSegmentLen = 0;

    if (decoder->mBytesToSkip) {
      if (decoder->mBytesToSkip < new_buflen) {
        /* All done skipping bytes; Return what's left. */
        new_buffer += decoder->mBytesToSkip;
        new_buflen -= decoder->mBytesToSkip;
        decoder->mBytesToSkip = 0;
      } else {
        /* Still need to skip some more data in the future */
        decoder->mBytesToSkip -= (size_t)new_buflen;
        return PR_FALSE; /* suspend */
      }
    }

      decoder->mBackBufferUnreadLen = src->bytes_in_buffer;

    src->next_input_byte = new_buffer;
    src->bytes_in_buffer = (size_t)new_buflen;
    decoder->mReading = PR_FALSE;

    return PR_TRUE;
  }

  if (src->next_input_byte != decoder->mSegment) {
    /* Backtrack data has been permanently consumed. */
    decoder->mBackBufferUnreadLen = 0;
    decoder->mBackBufferLen = 0;
  }

  /* Save remainder of netlib buffer in backtrack buffer */
  const PRUint32 new_backtrack_buflen = src->bytes_in_buffer + decoder->mBackBufferLen;
 
  /* Make sure backtrack buffer is big enough to hold new data. */
  if (decoder->mBackBufferSize < new_backtrack_buflen) {
    /* Check for malformed MARKER segment lengths, before allocating space for it */
    if (new_backtrack_buflen > MAX_JPEG_MARKER_LENGTH) {
      my_error_exit((j_common_ptr)(&decoder->mInfo));
    }

    /* Round up to multiple of 256 bytes. */
    const size_t roundup_buflen = ((new_backtrack_buflen + 255) >> 8) << 8;
    JOCTET *buf = (JOCTET *)PR_REALLOC(decoder->mBackBuffer, roundup_buflen);
    /* Check for OOM */
    if (!buf) {
      decoder->mInfo.err->msg_code = JERR_OUT_OF_MEMORY;
      my_error_exit((j_common_ptr)(&decoder->mInfo));
    }
    decoder->mBackBuffer = buf;
    decoder->mBackBufferSize = roundup_buflen;
  }

  /* Copy remainder of netlib segment into backtrack buffer. */
  memmove(decoder->mBackBuffer + decoder->mBackBufferLen,
          src->next_input_byte,
          src->bytes_in_buffer);

  /* Point to start of data to be rescanned. */
  src->next_input_byte = decoder->mBackBuffer + decoder->mBackBufferLen - decoder->mBackBufferUnreadLen;
  src->bytes_in_buffer += decoder->mBackBufferUnreadLen;
  decoder->mBackBufferLen = (size_t)new_backtrack_buflen;
  decoder->mReading = PR_TRUE;

  return PR_FALSE;
}

/******************************************************************************/
/* data source manager method */
/*
 * Terminate source --- called by jpeg_finish_decompress() after all
 * data has been read to clean up JPEG source manager. NOT called by 
 * jpeg_abort() or jpeg_destroy().
 */
METHODDEF(void)
term_source (j_decompress_ptr jd)
{
  nsJPEGDecoder *decoder = (nsJPEGDecoder *)(jd->client_data);

  if (decoder->mObserver) {
    decoder->mObserver->OnStopFrame(nsnull, decoder->mFrame);
    decoder->mObserver->OnStopContainer(nsnull, decoder->mImage);
    decoder->mObserver->OnStopDecode(nsnull, NS_OK, nsnull);
  }

  PRBool isMutable = PR_FALSE;
  if (decoder->mImageLoad) 
      decoder->mImageLoad->GetIsMultiPartChannel(&isMutable);
  decoder->mFrame->SetMutable(isMutable);
}


/**************** YCbCr -> Cairo's RGB24/ARGB32 conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *      R = Y                + 1.40200 * Cr
 *      G = Y - 0.34414 * Cb - 0.71414 * Cr
 *      B = Y + 1.77200 * Cb
 * where Cb and Cr represent the incoming values less CENTERJSAMPLE.
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 * Notice that Y, being an integral input, does not contribute any fraction
 * so it need not participate in the rounding.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times Cb and Cr for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The Cr=>R and Cb=>B values can be rounded to integers in advance; the
 * values for the G calculation are left scaled up, since we must add them
 * together before rounding.
 */

#define SCALEBITS       16      /* speediest right-shift on some machines */

/* Use static tables for color processing. */
/* Four tables, each 256 entries of 4 bytes totals 4K which is not bad... */

const int Cr_r_tab[(MAXJSAMPLE+1) * sizeof(int)] ={
  0xffffff4d, 0xffffff4e, 0xffffff4f, 0xffffff51, 0xffffff52, 0xffffff54,
  0xffffff55, 0xffffff56, 0xffffff58, 0xffffff59, 0xffffff5b, 0xffffff5c,
  0xffffff5d, 0xffffff5f, 0xffffff60, 0xffffff62, 0xffffff63, 0xffffff64,
  0xffffff66, 0xffffff67, 0xffffff69, 0xffffff6a, 0xffffff6b, 0xffffff6d,
  0xffffff6e, 0xffffff70, 0xffffff71, 0xffffff72, 0xffffff74, 0xffffff75,
  0xffffff77, 0xffffff78, 0xffffff79, 0xffffff7b, 0xffffff7c, 0xffffff7e,
  0xffffff7f, 0xffffff80, 0xffffff82, 0xffffff83, 0xffffff85, 0xffffff86,
  0xffffff87, 0xffffff89, 0xffffff8a, 0xffffff8c, 0xffffff8d, 0xffffff8e,
  0xffffff90, 0xffffff91, 0xffffff93, 0xffffff94, 0xffffff95, 0xffffff97,
  0xffffff98, 0xffffff9a, 0xffffff9b, 0xffffff9c, 0xffffff9e, 0xffffff9f,
  0xffffffa1, 0xffffffa2, 0xffffffa3, 0xffffffa5, 0xffffffa6, 0xffffffa8,
  0xffffffa9, 0xffffffaa, 0xffffffac, 0xffffffad, 0xffffffaf, 0xffffffb0,
  0xffffffb1, 0xffffffb3, 0xffffffb4, 0xffffffb6, 0xffffffb7, 0xffffffb8,
  0xffffffba, 0xffffffbb, 0xffffffbd, 0xffffffbe, 0xffffffc0, 0xffffffc1,
  0xffffffc2, 0xffffffc4, 0xffffffc5, 0xffffffc7, 0xffffffc8, 0xffffffc9,
  0xffffffcb, 0xffffffcc, 0xffffffce, 0xffffffcf, 0xffffffd0, 0xffffffd2,
  0xffffffd3, 0xffffffd5, 0xffffffd6, 0xffffffd7, 0xffffffd9, 0xffffffda,
  0xffffffdc, 0xffffffdd, 0xffffffde, 0xffffffe0, 0xffffffe1, 0xffffffe3,
  0xffffffe4, 0xffffffe5, 0xffffffe7, 0xffffffe8, 0xffffffea, 0xffffffeb,
  0xffffffec, 0xffffffee, 0xffffffef, 0xfffffff1, 0xfffffff2, 0xfffffff3,
  0xfffffff5, 0xfffffff6, 0xfffffff8, 0xfffffff9, 0xfffffffa, 0xfffffffc,
  0xfffffffd, 0xffffffff,       0x00,       0x01,       0x03,       0x04,
        0x06,       0x07,       0x08,       0x0a,       0x0b,       0x0d,
        0x0e,       0x0f,       0x11,       0x12,       0x14,       0x15,
        0x16,       0x18,       0x19,       0x1b,       0x1c,       0x1d,
        0x1f,       0x20,       0x22,       0x23,       0x24,       0x26,
        0x27,       0x29,       0x2a,       0x2b,       0x2d,       0x2e,
        0x30,       0x31,       0x32,       0x34,       0x35,       0x37,
        0x38,       0x39,       0x3b,       0x3c,       0x3e,       0x3f,
        0x40,       0x42,       0x43,       0x45,       0x46,       0x48,
        0x49,       0x4a,       0x4c,       0x4d,       0x4f,       0x50,
        0x51,       0x53,       0x54,       0x56,       0x57,       0x58,
        0x5a,       0x5b,       0x5d,       0x5e,       0x5f,       0x61,
        0x62,       0x64,       0x65,       0x66,       0x68,       0x69,
        0x6b,       0x6c,       0x6d,       0x6f,       0x70,       0x72,
        0x73,       0x74,       0x76,       0x77,       0x79,       0x7a,
        0x7b,       0x7d,       0x7e,       0x80,       0x81,       0x82,
        0x84,       0x85,       0x87,       0x88,       0x89,       0x8b,
        0x8c,       0x8e,       0x8f,       0x90,       0x92,       0x93,
        0x95,       0x96,       0x97,       0x99,       0x9a,       0x9c,
        0x9d,       0x9e,       0xa0,       0xa1,       0xa3,       0xa4,
        0xa5,       0xa7,       0xa8,       0xaa,       0xab,       0xac,
        0xae,       0xaf,       0xb1,       0xb2
  };

const int Cb_b_tab[(MAXJSAMPLE+1) * sizeof(int)] ={
  0xffffff1d, 0xffffff1f, 0xffffff21, 0xffffff22, 0xffffff24, 0xffffff26,
  0xffffff28, 0xffffff2a, 0xffffff2b, 0xffffff2d, 0xffffff2f, 0xffffff31,
  0xffffff32, 0xffffff34, 0xffffff36, 0xffffff38, 0xffffff3a, 0xffffff3b,
  0xffffff3d, 0xffffff3f, 0xffffff41, 0xffffff42, 0xffffff44, 0xffffff46,
  0xffffff48, 0xffffff49, 0xffffff4b, 0xffffff4d, 0xffffff4f, 0xffffff51,
  0xffffff52, 0xffffff54, 0xffffff56, 0xffffff58, 0xffffff59, 0xffffff5b,
  0xffffff5d, 0xffffff5f, 0xffffff61, 0xffffff62, 0xffffff64, 0xffffff66,
  0xffffff68, 0xffffff69, 0xffffff6b, 0xffffff6d, 0xffffff6f, 0xffffff70,
  0xffffff72, 0xffffff74, 0xffffff76, 0xffffff78, 0xffffff79, 0xffffff7b,
  0xffffff7d, 0xffffff7f, 0xffffff80, 0xffffff82, 0xffffff84, 0xffffff86,
  0xffffff88, 0xffffff89, 0xffffff8b, 0xffffff8d, 0xffffff8f, 0xffffff90,
  0xffffff92, 0xffffff94, 0xffffff96, 0xffffff97, 0xffffff99, 0xffffff9b,
  0xffffff9d, 0xffffff9f, 0xffffffa0, 0xffffffa2, 0xffffffa4, 0xffffffa6,
  0xffffffa7, 0xffffffa9, 0xffffffab, 0xffffffad, 0xffffffae, 0xffffffb0,
  0xffffffb2, 0xffffffb4, 0xffffffb6, 0xffffffb7, 0xffffffb9, 0xffffffbb,
  0xffffffbd, 0xffffffbe, 0xffffffc0, 0xffffffc2, 0xffffffc4, 0xffffffc6,
  0xffffffc7, 0xffffffc9, 0xffffffcb, 0xffffffcd, 0xffffffce, 0xffffffd0,
  0xffffffd2, 0xffffffd4, 0xffffffd5, 0xffffffd7, 0xffffffd9, 0xffffffdb,
  0xffffffdd, 0xffffffde, 0xffffffe0, 0xffffffe2, 0xffffffe4, 0xffffffe5,
  0xffffffe7, 0xffffffe9, 0xffffffeb, 0xffffffed, 0xffffffee, 0xfffffff0,
  0xfffffff2, 0xfffffff4, 0xfffffff5, 0xfffffff7, 0xfffffff9, 0xfffffffb,
  0xfffffffc, 0xfffffffe,       0x00,       0x02,       0x04,       0x05,
        0x07,       0x09,       0x0b,       0x0c,       0x0e,       0x10,
        0x12,       0x13,       0x15,       0x17,       0x19,       0x1b,
        0x1c,       0x1e,       0x20,       0x22,       0x23,       0x25,
        0x27,       0x29,       0x2b,       0x2c,       0x2e,       0x30,
        0x32,       0x33,       0x35,       0x37,       0x39,       0x3a,
        0x3c,       0x3e,       0x40,       0x42,       0x43,       0x45,
        0x47,       0x49,       0x4a,       0x4c,       0x4e,       0x50,
        0x52,       0x53,       0x55,       0x57,       0x59,       0x5a,
        0x5c,       0x5e,       0x60,       0x61,       0x63,       0x65,
        0x67,       0x69,       0x6a,       0x6c,       0x6e,       0x70,
        0x71,       0x73,       0x75,       0x77,       0x78,       0x7a,
        0x7c,       0x7e,       0x80,       0x81,       0x83,       0x85,
        0x87,       0x88,       0x8a,       0x8c,       0x8e,       0x90,
        0x91,       0x93,       0x95,       0x97,       0x98,       0x9a,
        0x9c,       0x9e,       0x9f,       0xa1,       0xa3,       0xa5,
        0xa7,       0xa8,       0xaa,       0xac,       0xae,       0xaf,
        0xb1,       0xb3,       0xb5,       0xb7,       0xb8,       0xba,
        0xbc,       0xbe,       0xbf,       0xc1,       0xc3,       0xc5,
        0xc6,       0xc8,       0xca,       0xcc,       0xce,       0xcf,
        0xd1,       0xd3,       0xd5,       0xd6,       0xd8,       0xda,
        0xdc,       0xde,       0xdf,       0xe1
  };

const int Cr_g_tab[(MAXJSAMPLE+1) * sizeof(int)] ={
    0x5b6900,   0x5ab22e,   0x59fb5c,   0x59448a,   0x588db8,   0x57d6e6,
    0x572014,   0x566942,   0x55b270,   0x54fb9e,   0x5444cc,   0x538dfa,
    0x52d728,   0x522056,   0x516984,   0x50b2b2,   0x4ffbe0,   0x4f450e,
    0x4e8e3c,   0x4dd76a,   0x4d2098,   0x4c69c6,   0x4bb2f4,   0x4afc22,
    0x4a4550,   0x498e7e,   0x48d7ac,   0x4820da,   0x476a08,   0x46b336,
    0x45fc64,   0x454592,   0x448ec0,   0x43d7ee,   0x43211c,   0x426a4a,
    0x41b378,   0x40fca6,   0x4045d4,   0x3f8f02,   0x3ed830,   0x3e215e,
    0x3d6a8c,   0x3cb3ba,   0x3bfce8,   0x3b4616,   0x3a8f44,   0x39d872,
    0x3921a0,   0x386ace,   0x37b3fc,   0x36fd2a,   0x364658,   0x358f86,
    0x34d8b4,   0x3421e2,   0x336b10,   0x32b43e,   0x31fd6c,   0x31469a,
    0x308fc8,   0x2fd8f6,   0x2f2224,   0x2e6b52,   0x2db480,   0x2cfdae,
    0x2c46dc,   0x2b900a,   0x2ad938,   0x2a2266,   0x296b94,   0x28b4c2,
    0x27fdf0,   0x27471e,   0x26904c,   0x25d97a,   0x2522a8,   0x246bd6,
    0x23b504,   0x22fe32,   0x224760,   0x21908e,   0x20d9bc,   0x2022ea,
    0x1f6c18,   0x1eb546,   0x1dfe74,   0x1d47a2,   0x1c90d0,   0x1bd9fe,
    0x1b232c,   0x1a6c5a,   0x19b588,   0x18feb6,   0x1847e4,   0x179112,
    0x16da40,   0x16236e,   0x156c9c,   0x14b5ca,   0x13fef8,   0x134826,
    0x129154,   0x11da82,   0x1123b0,   0x106cde,    0xfb60c,    0xeff3a,
     0xe4868,    0xd9196,    0xcdac4,    0xc23f2,    0xb6d20,    0xab64e,
     0x9ff7c,    0x948aa,    0x891d8,    0x7db06,    0x72434,    0x66d62,
     0x5b690,    0x4ffbe,    0x448ec,    0x3921a,    0x2db48,    0x22476,
     0x16da4,     0xb6d2,        0x0, 0xffff492e, 0xfffe925c, 0xfffddb8a,
  0xfffd24b8, 0xfffc6de6, 0xfffbb714, 0xfffb0042, 0xfffa4970, 0xfff9929e,
  0xfff8dbcc, 0xfff824fa, 0xfff76e28, 0xfff6b756, 0xfff60084, 0xfff549b2,
  0xfff492e0, 0xfff3dc0e, 0xfff3253c, 0xfff26e6a, 0xfff1b798, 0xfff100c6,
  0xfff049f4, 0xffef9322, 0xffeedc50, 0xffee257e, 0xffed6eac, 0xffecb7da,
  0xffec0108, 0xffeb4a36, 0xffea9364, 0xffe9dc92, 0xffe925c0, 0xffe86eee,
  0xffe7b81c, 0xffe7014a, 0xffe64a78, 0xffe593a6, 0xffe4dcd4, 0xffe42602,
  0xffe36f30, 0xffe2b85e, 0xffe2018c, 0xffe14aba, 0xffe093e8, 0xffdfdd16,
  0xffdf2644, 0xffde6f72, 0xffddb8a0, 0xffdd01ce, 0xffdc4afc, 0xffdb942a,
  0xffdadd58, 0xffda2686, 0xffd96fb4, 0xffd8b8e2, 0xffd80210, 0xffd74b3e,
  0xffd6946c, 0xffd5dd9a, 0xffd526c8, 0xffd46ff6, 0xffd3b924, 0xffd30252,
  0xffd24b80, 0xffd194ae, 0xffd0dddc, 0xffd0270a, 0xffcf7038, 0xffceb966,
  0xffce0294, 0xffcd4bc2, 0xffcc94f0, 0xffcbde1e, 0xffcb274c, 0xffca707a,
  0xffc9b9a8, 0xffc902d6, 0xffc84c04, 0xffc79532, 0xffc6de60, 0xffc6278e,
  0xffc570bc, 0xffc4b9ea, 0xffc40318, 0xffc34c46, 0xffc29574, 0xffc1dea2,
  0xffc127d0, 0xffc070fe, 0xffbfba2c, 0xffbf035a, 0xffbe4c88, 0xffbd95b6,
  0xffbcdee4, 0xffbc2812, 0xffbb7140, 0xffbaba6e, 0xffba039c, 0xffb94cca,
  0xffb895f8, 0xffb7df26, 0xffb72854, 0xffb67182, 0xffb5bab0, 0xffb503de,
  0xffb44d0c, 0xffb3963a, 0xffb2df68, 0xffb22896, 0xffb171c4, 0xffb0baf2,
  0xffb00420, 0xffaf4d4e, 0xffae967c, 0xffaddfaa, 0xffad28d8, 0xffac7206,
  0xffabbb34, 0xffab0462, 0xffaa4d90, 0xffa996be, 0xffa8dfec, 0xffa8291a,
  0xffa77248, 0xffa6bb76, 0xffa604a4, 0xffa54dd2
 };

const int Cb_g_tab[(MAXJSAMPLE+1) * sizeof(int)] ={
    0x2c8d00,   0x2c34e6,   0x2bdccc,   0x2b84b2,   0x2b2c98,   0x2ad47e,
    0x2a7c64,   0x2a244a,   0x29cc30,   0x297416,   0x291bfc,   0x28c3e2,
    0x286bc8,   0x2813ae,   0x27bb94,   0x27637a,   0x270b60,   0x26b346,
    0x265b2c,   0x260312,   0x25aaf8,   0x2552de,   0x24fac4,   0x24a2aa,
    0x244a90,   0x23f276,   0x239a5c,   0x234242,   0x22ea28,   0x22920e,
    0x2239f4,   0x21e1da,   0x2189c0,   0x2131a6,   0x20d98c,   0x208172,
    0x202958,   0x1fd13e,   0x1f7924,   0x1f210a,   0x1ec8f0,   0x1e70d6,
    0x1e18bc,   0x1dc0a2,   0x1d6888,   0x1d106e,   0x1cb854,   0x1c603a,
    0x1c0820,   0x1bb006,   0x1b57ec,   0x1affd2,   0x1aa7b8,   0x1a4f9e,
    0x19f784,   0x199f6a,   0x194750,   0x18ef36,   0x18971c,   0x183f02,
    0x17e6e8,   0x178ece,   0x1736b4,   0x16de9a,   0x168680,   0x162e66,
    0x15d64c,   0x157e32,   0x152618,   0x14cdfe,   0x1475e4,   0x141dca,
    0x13c5b0,   0x136d96,   0x13157c,   0x12bd62,   0x126548,   0x120d2e,
    0x11b514,   0x115cfa,   0x1104e0,   0x10acc6,   0x1054ac,    0xffc92,
     0xfa478,    0xf4c5e,    0xef444,    0xe9c2a,    0xe4410,    0xdebf6,
     0xd93dc,    0xd3bc2,    0xce3a8,    0xc8b8e,    0xc3374,    0xbdb5a,
     0xb8340,    0xb2b26,    0xad30c,    0xa7af2,    0xa22d8,    0x9cabe,
     0x972a4,    0x91a8a,    0x8c270,    0x86a56,    0x8123c,    0x7ba22,
     0x76208,    0x709ee,    0x6b1d4,    0x659ba,    0x601a0,    0x5a986,
     0x5516c,    0x4f952,    0x4a138,    0x4491e,    0x3f104,    0x398ea,
     0x340d0,    0x2e8b6,    0x2909c,    0x23882,    0x1e068,    0x1884e,
     0x13034,     0xd81a,     0x8000,     0x27e6, 0xffffcfcc, 0xffff77b2,
  0xffff1f98, 0xfffec77e, 0xfffe6f64, 0xfffe174a, 0xfffdbf30, 0xfffd6716,
  0xfffd0efc, 0xfffcb6e2, 0xfffc5ec8, 0xfffc06ae, 0xfffbae94, 0xfffb567a,
  0xfffafe60, 0xfffaa646, 0xfffa4e2c, 0xfff9f612, 0xfff99df8, 0xfff945de,
  0xfff8edc4, 0xfff895aa, 0xfff83d90, 0xfff7e576, 0xfff78d5c, 0xfff73542,
  0xfff6dd28, 0xfff6850e, 0xfff62cf4, 0xfff5d4da, 0xfff57cc0, 0xfff524a6,
  0xfff4cc8c, 0xfff47472, 0xfff41c58, 0xfff3c43e, 0xfff36c24, 0xfff3140a,
  0xfff2bbf0, 0xfff263d6, 0xfff20bbc, 0xfff1b3a2, 0xfff15b88, 0xfff1036e,
  0xfff0ab54, 0xfff0533a, 0xffeffb20, 0xffefa306, 0xffef4aec, 0xffeef2d2,
  0xffee9ab8, 0xffee429e, 0xffedea84, 0xffed926a, 0xffed3a50, 0xffece236,
  0xffec8a1c, 0xffec3202, 0xffebd9e8, 0xffeb81ce, 0xffeb29b4, 0xffead19a,
  0xffea7980, 0xffea2166, 0xffe9c94c, 0xffe97132, 0xffe91918, 0xffe8c0fe,
  0xffe868e4, 0xffe810ca, 0xffe7b8b0, 0xffe76096, 0xffe7087c, 0xffe6b062,
  0xffe65848, 0xffe6002e, 0xffe5a814, 0xffe54ffa, 0xffe4f7e0, 0xffe49fc6,
  0xffe447ac, 0xffe3ef92, 0xffe39778, 0xffe33f5e, 0xffe2e744, 0xffe28f2a,
  0xffe23710, 0xffe1def6, 0xffe186dc, 0xffe12ec2, 0xffe0d6a8, 0xffe07e8e,
  0xffe02674, 0xffdfce5a, 0xffdf7640, 0xffdf1e26, 0xffdec60c, 0xffde6df2,
  0xffde15d8, 0xffddbdbe, 0xffdd65a4, 0xffdd0d8a, 0xffdcb570, 0xffdc5d56,
  0xffdc053c, 0xffdbad22, 0xffdb5508, 0xffdafcee, 0xffdaa4d4, 0xffda4cba,
  0xffd9f4a0, 0xffd99c86, 0xffd9446c, 0xffd8ec52, 0xffd89438, 0xffd83c1e,
  0xffd7e404, 0xffd78bea, 0xffd733d0, 0xffd6dbb6, 0xffd6839c, 0xffd62b82,
  0xffd5d368, 0xffd57b4e, 0xffd52334, 0xffd4cb1a
 };


/* We assume that right shift corresponds to signed division by 2 with
 * rounding towards minus infinity.  This is correct for typical "arithmetic
 * shift" instructions that shift in copies of the sign bit.  But some
 * C compilers implement >> with an unsigned shift.  For these machines you
 * must define RIGHT_SHIFT_IS_UNSIGNED.
 * RIGHT_SHIFT provides a proper signed right shift of an INT32 quantity.
 * It is only applied with constant shift counts.  SHIFT_TEMPS must be
 * included in the variables of any routine using RIGHT_SHIFT.
 */

#ifdef RIGHT_SHIFT_IS_UNSIGNED
#define SHIFT_TEMPS	INT32 shift_temp;
#define RIGHT_SHIFT(x,shft)  \
	((shift_temp = (x)) < 0 ? \
	 (shift_temp >> (shft)) | ((~((INT32) 0)) << (32-(shft))) : \
	 (shift_temp >> (shft)))
#else
#define SHIFT_TEMPS
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#endif


METHODDEF(void)
ycc_rgb_convert_argb (j_decompress_ptr cinfo,
                 JSAMPIMAGE input_buf, JDIMENSION input_row,
                 JSAMPARRAY output_buf, int num_rows)
{
  JDIMENSION num_cols = cinfo->output_width;
  JSAMPLE * range_limit = cinfo->sample_range_limit;

  SHIFT_TEMPS

  /* This is used if we don't have SSE2 */

  while (--num_rows >= 0) {
    JSAMPROW inptr0 = input_buf[0][input_row];
    JSAMPROW inptr1 = input_buf[1][input_row];
    JSAMPROW inptr2 = input_buf[2][input_row];
    input_row++;
    PRUint32 *outptr = (PRUint32 *) *output_buf++;
    for (JDIMENSION col = 0; col < num_cols; col++) {
      int y  = GETJSAMPLE(inptr0[col]);
      int cb = GETJSAMPLE(inptr1[col]);
      int cr = GETJSAMPLE(inptr2[col]);
      JSAMPLE * range_limit_y = range_limit + y;
      /* Range-limiting is essential due to noise introduced by DCT losses. */
      outptr[col] = 0xFF000000 |
                    ( range_limit_y[Cr_r_tab[cr]] << 16 ) |
                    ( range_limit_y[((int) RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS))] << 8 ) |
                    ( range_limit_y[Cb_b_tab[cb]] );
    }
  }
}


/**************** Inverted CMYK -> RGB conversion **************/
/*
 * Input is (Inverted) CMYK stored as 4 bytes per pixel.
 * Output is RGB stored as 3 bytes per pixel.
 * @param row Points to row buffer containing the CMYK bytes for each pixel in the row.
 * @param width Number of pixels in the row.
 */
static void cmyk_convert_rgb(JSAMPROW row, JDIMENSION width)
{
  /* Work from end to front to shrink from 4 bytes per pixel to 3 */
  JSAMPROW in = row + width*4;
  JSAMPROW out = in;

  for (PRUint32 i = width; i > 0; i--) {
    in -= 4;
    out -= 3;

    // Source is 'Inverted CMYK', output is RGB.
    // See: http://www.easyrgb.com/math.php?MATH=M12#text12
    // Or:  http://www.ilkeratalay.com/colorspacesfaq.php#rgb

    // From CMYK to CMY
    // C = ( C * ( 1 - K ) + K )
    // M = ( M * ( 1 - K ) + K )
    // Y = ( Y * ( 1 - K ) + K )

    // From Inverted CMYK to CMY is thus:
    // C = ( (1-iC) * (1 - (1-iK)) + (1-iK) ) => 1 - iC*iK
    // Same for M and Y

    // Convert from CMY (0..1) to RGB (0..1)
    // R = 1 - C => 1 - (1 - iC*iK) => iC*iK
    // G = 1 - M => 1 - (1 - iM*iK) => iM*iK
    // B = 1 - Y => 1 - (1 - iY*iK) => iY*iK
  
    // Convert from Inverted CMYK (0..255) to RGB (0..255)
    const PRUint32 iC = in[0];
    const PRUint32 iM = in[1];
    const PRUint32 iY = in[2];
    const PRUint32 iK = in[3];
    out[0] = iC*iK/255;   // Red
    out[1] = iM*iK/255;   // Green
    out[2] = iY*iK/255;   // Blue
  }
}
