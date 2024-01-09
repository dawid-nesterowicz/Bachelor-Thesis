//*******************************************************************
//
//  CLASSFAC.CPP
//  Module: WMI Instance provider sample code
//  Purpose: Contains the class factory.  This creates objects when
//           connections are requested.
// Copyright (C) Microsoft. All Rights Reserved.
//
//*******************************************************************

//#include "pch.h"
#include "malprovider.h"
#include <objbase.h>


//*******************************************************************
// Constructor Parameters:
//  None
//*******************************************************************

WmiPerfClassFactory::WmiPerfClassFactory()
{
    m_cRef = 0L;
    return;
}

WmiPerfClassFactory::~WmiPerfClassFactory()
{
    return;
}

//*******************************************************************
//
// IUnknown::QueryInterface
// IUnknown::AddRef
// IUnknown::Release
//
// Purpose: Standard Ole routines needed for all interfaces
//
//*******************************************************************

STDMETHODIMP WmiPerfClassFactory::QueryInterface(REFIID riid, PPVOID ppv)
{
    *ppv = NULL;

    if (IID_IUnknown == riid || IID_IClassFactory == riid)
        *ppv = this;

    if (NULL != *ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) WmiPerfClassFactory::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) WmiPerfClassFactory::Release(void)
{
    ULONG nNewCount = InterlockedDecrement((long*)&m_cRef);
    if (0L == nNewCount)
        delete this;

    return nNewCount;
}

//*******************************************************************
//
// IClassFactory::CreateInstance
//
// Purpose: Instantiates a Locator object
// and returns an interface pointer.
//
// Parameters:
//  pUnkOuter       LPUNKNOWN to the controlling IUnknown if 
//                  being used in an aggregation.
//  riid            REFIID identifies the interface the caller
//                  desires to have for the new object.
//  ppvObj          PPVOID in which to store the desired
//                  interface pointer for the new object.
//
// Return Value:
//  HRESULT         NOERROR if successful, otherwise E_NOINTERFACE
//                  if you cannot support the requested interface.
//*******************************************************************

STDMETHODIMP WmiPerfClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, PPVOID ppvObj)
{
    WmiPerfClass* pObj;
    HRESULT hr;

    *ppvObj = NULL;

    // This object does not support aggregation.
    if (NULL != pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    // Create the locator object.
    pObj = new WmiPerfClass();
    if (NULL == pObj)
        return E_OUTOFMEMORY;

    hr = pObj->QueryInterface(riid, ppvObj);

    //Kill the object if initial creation or Init failed.
    if (FAILED(hr))
        delete pObj;

    return hr;
}

//*******************************************************************
//
// IClassFactory::LockServer
//
// Purpose:
//  Increments or decrements the lock count of the DLL.  If the
//  lock count goes to zero and there are no objects, the DLL
//  is allowed to unload.  See DllCanUnloadNow.
//
// Parameters:
//  fLock           BOOL specifying whether to increment or
//                  decrement the lock count.
//
// Return Value:
//  HRESULT         NOERROR always.
//*******************************************************************

STDMETHODIMP WmiPerfClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement(&g_cLock);
    else
        InterlockedDecrement(&g_cLock);
    return NOERROR;
}