/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsIMacAppHelper.idl
 */

#ifndef __gen_nsIMacAppHelper_h__
#define __gen_nsIMacAppHelper_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsILocalFile; /* forward declaration */


/* starting interface:    nsIMacAppHelper */
#define NS_IMACAPPHELPER_IID_STR "8d3f5a9d-118d-4548-a137-cf7718679069"

#define NS_IMACAPPHELPER_IID \
  {0x8d3f5a9d, 0x118d, 0x4548, \
    { 0xa1, 0x37, 0xcf, 0x77, 0x18, 0x67, 0x90, 0x69 }}

class NS_NO_VTABLE NS_SCRIPTABLE nsIMacAppHelper : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IMACAPPHELPER_IID)

  /* void openFile (in nsILocalFile aFile); */
  NS_SCRIPTABLE NS_IMETHOD OpenFile(nsILocalFile *aFile) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(nsIMacAppHelper, NS_IMACAPPHELPER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMACAPPHELPER \
  NS_SCRIPTABLE NS_IMETHOD OpenFile(nsILocalFile *aFile); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMACAPPHELPER(_to) \
  NS_SCRIPTABLE NS_IMETHOD OpenFile(nsILocalFile *aFile) { return _to OpenFile(aFile); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMACAPPHELPER(_to) \
  NS_SCRIPTABLE NS_IMETHOD OpenFile(nsILocalFile *aFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->OpenFile(aFile); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsMacAppHelper : public nsIMacAppHelper
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMACAPPHELPER

  nsMacAppHelper();

private:
  ~nsMacAppHelper();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsMacAppHelper, nsIMacAppHelper)

nsMacAppHelper::nsMacAppHelper()
{
  /* member initializers and constructor code */
}

nsMacAppHelper::~nsMacAppHelper()
{
  /* destructor code */
}

/* void openFile (in nsILocalFile aFile); */
NS_IMETHODIMP nsMacAppHelper::OpenFile(nsILocalFile *aFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIMacAppHelper_h__ */
