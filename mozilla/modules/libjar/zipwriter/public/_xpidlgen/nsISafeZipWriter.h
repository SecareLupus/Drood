/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsISafeZipWriter.idl
 */

#ifndef __gen_nsISafeZipWriter_h__
#define __gen_nsISafeZipWriter_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISafeZipWriter */
#define NS_ISAFEZIPWRITER_IID_STR "fd96fd2e-8612-4c93-bcc7-22d3bcef4fae"

#define NS_ISAFEZIPWRITER_IID \
  {0xfd96fd2e, 0x8612, 0x4c93, \
    { 0xbc, 0xc7, 0x22, 0xd3, 0xbc, 0xef, 0x4f, 0xae }}

/**
 * nsISafeZipWriter
 *
 * An interface for a safe zip archiver that can be used from script.
 */
class NS_NO_VTABLE NS_SCRIPTABLE nsISafeZipWriter : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_ISAFEZIPWRITER_IID)

  /**
   * Closes the zip file and cause the original target to be overwritten.
   *
   * @throws NS_ERROR_NOT_INITIALIZED if no zip file has been opened
   * @throws NS_ERROR_IN_PROGRESS if another operation is currently in progress
   * @throws <other-error> on failure to complete the zip file
   */
  /* void finish (); */
  NS_SCRIPTABLE NS_IMETHOD Finish(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsISafeZipWriter, NS_ISAFEZIPWRITER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISAFEZIPWRITER \
  NS_SCRIPTABLE NS_IMETHOD Finish(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISAFEZIPWRITER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Finish(void) { return _to Finish(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISAFEZIPWRITER(_to) \
  NS_SCRIPTABLE NS_IMETHOD Finish(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Finish(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSafeZipWriter : public nsISafeZipWriter
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISAFEZIPWRITER

  nsSafeZipWriter();

private:
  ~nsSafeZipWriter();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSafeZipWriter, nsISafeZipWriter)

nsSafeZipWriter::nsSafeZipWriter()
{
  /* member initializers and constructor code */
}

nsSafeZipWriter::~nsSafeZipWriter()
{
  /* destructor code */
}

/* void finish (); */
NS_IMETHODIMP nsSafeZipWriter::Finish()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISafeZipWriter_h__ */
