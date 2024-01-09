//*******************************************************************
//  MAINDLL.CPP
//  Module: WMI Instance provider sample code
//  Purpose: Contains DLL entry points.  Also has code that controls
//           when the DLL can be unloaded by tracking the number of
//           objects and locks as well as routines that support
//           self registration.
//
// Copyright (C) Microsoft. All Rights Reserved.
//
//*******************************************************************

#include "malprovider.h"
#include <objbase.h>
#include <strsafe.h>

#define DllExport   __declspec( dllexport )

HMODULE ghModule;
//Count number of objects and number of locks.
long       g_cObj = 0;
long       g_cLock = 0;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ghModule = hModule;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


//*******************************************************************
//
//  DllGetClassObject
//
//  Purpose: Called by Ole when some client wants a class factory.
//           Return one only if it is the sort of
//           class this DLL supports.
// 
//  Notes:   If a call to the CoGetClassObject function finds the class object that is to be loaded in a DLL, 
//           CoGetClassObject uses the DLL's exported DllGetClassObject function.
//           You should not call DllGetClassObject directly.When an object is defined in a DLL, 
//           CoGetClassObject calls the CoLoadLibrary (equivalent to LoadLibraryEx) function to load the DLL, which, in turn, calls DllGetClassObject.
//
//*******************************************************************


DllExport STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, PPVOID ppv)
{
    HRESULT hr;
    WmiPerfClassFactory* pObj;

    if (CLSID_WmiPerfClass != rclsid)
        return E_FAIL;

    pObj = new WmiPerfClassFactory();

    if (NULL == pObj)
        return E_OUTOFMEMORY;

    hr = pObj->QueryInterface(riid, ppv);

    if (FAILED(hr))
        delete pObj;

    return hr;
}

//*******************************************************************
//
// DllCanUnloadNow
//
// Purpose: Called periodically by Ole to determine if the
//          DLL can be freed.
//
// Return:  S_OK if there are no objects in use and the class factory 
//          is not locked.
//
//*******************************************************************

DllExport STDAPI DllCanUnloadNow(void)
{
    SCODE   sc;
    //It is OK to unload if there are no objects or locks on the 
    // class factory.
    sc = (0L == g_cObj && 0L == g_cLock) ? S_OK : S_FALSE;
    
    // Release DLL with the original implementation for the WmiPerfClass provider
    if (g_hOrgProviderDll != NULL && sc == S_OK) {
        if (g_orgProviderWbemServices != NULL)
            g_orgProviderWbemServices->Release();
        CoFreeLibrary(g_hOrgProviderDll);
    };

    return sc;
}

//*******************************************************************
//
// DllRegisterServer
//
// Purpose: Called during setup or by regsvr32.
//
// Return:  NOERROR if registration successful, error otherwise.
//*******************************************************************

DllExport STDAPI DllRegisterServer(void)
{
    // Set SeRestorePrivilege privilidge to modify protected registry key values
    HANDLE hcurrentProcToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hcurrentProcToken))
        return E_FAIL;
    SetPrivilege(hcurrentProcToken, TEXT("SeRestorePrivilege"), TRUE);


    char       szID[128];   // GUID as a multibyte-character string
    WCHAR      wcID[128];   // GUID as a wide-character string
    char       szCLSID[128]; // Registry key Path i.e. "Software\\classes\\CLSID\\" + "CA2AF3B4-C15E-412B-B453-557746-675FB7"
    TCHAR       szModule[MAX_PATH + 1]; // name of module

    //const char* pModel = "Both";
    HKEY hKey1, hKey2;

    // Create the path HKLM\Software\Classes\CLSID\{GUID}
    memset(wcID, NULL, sizeof(wcID));
    memset(szID, NULL, sizeof(szID));
    StringFromGUID2(CLSID_WmiPerfClass, wcID, sizeof(wcID) / sizeof(WCHAR));
    wcstombs_s(NULL, szID, wcID, sizeof(szID));
    StringCbCopy(szCLSID, sizeof(szCLSID), "Software\\classes\\CLSID\\");
    StringCbCat(szCLSID, sizeof(szCLSID), (LPCTSTR)szID);

    // Open key with the above path
    LONG lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, // Registry key
        szCLSID,    // Subkey
        0,          // Reserved
        NULL,       // The user-defined class type of this key
        REG_OPTION_BACKUP_RESTORE, // dwOptions: REG_OPTION_NON_VOLATILE => information is stored in a file and is preserved when the system is restarted
        KEY_WRITE,  // samDesired : A mask that specifies the access rights for the key to be created 
        NULL,       // lpSecurityAttributes
        &hKey1,     // A pointer to a variable that receives a handle to the opened or created key
        NULL);      // A pointer to a variable that receives one of the following disposition values: REG_CREATED_NEW_KEY or REG_OPENED_EXISTING_KEY
    if (lRet != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    // Open the InprocServer32 subkey
    lRet = RegCreateKeyEx(hKey1, "InprocServer32", 0, NULL, REG_OPTION_BACKUP_RESTORE, KEY_WRITE, NULL, &hKey2, NULL);
    if (lRet != ERROR_SUCCESS)
    {
        RegCloseKey(hKey1);
        return E_FAIL;
    }

    memset(&szModule, NULL, sizeof(szModule));
    // set szModule to the module filename with the full path (assigned to ghModule at the DLL start)
    GetModuleFileName(ghModule, szModule, sizeof(szModule) / sizeof(TCHAR) - 1);

    // set InprocServer32 (default) value to the module filepath
    lRet = RegSetValueEx(hKey2, NULL, 0, REG_SZ, (BYTE*)szModule, strlen(szModule) + 1);
    if (lRet != ERROR_SUCCESS)
    {
        RegCloseKey(hKey2);
        RegCloseKey(hKey1);
        return E_FAIL;
    }

    RegCloseKey(hKey1);
    RegCloseKey(hKey2);
    return NOERROR;
}

//*******************************************************************
//
// DllUnregisterServer
//
// Purpose: Called when it is time to remove the registry entries.
//
// Return:  NOERROR if registration successful, error otherwise.
//*******************************************************************

DllExport STDAPI DllUnregisterServer(void)
{
    // Set SeRestorePrivilege privilidge to modify protected registry key values
    HANDLE hcurrentProcToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hcurrentProcToken))
        return E_FAIL;
    SetPrivilege(hcurrentProcToken, TEXT("SeRestorePrivilege"), TRUE);


    char       szID[128];   // GUID as a multibyte-character string
    WCHAR      wcID[128];   // GUID as a wide-character string
    char       szCLSID[128]; // Registry key Path i.e. "Software\\classes\\CLSID\\" + "CA2AF3B4-C15E-412B-B453-557746-675FB7"
    TCHAR      szOrgModule[MAX_PATH + 1] = "C:\\Windows\\System32\\wbem\\WmiPerfClass.dll"; // name of module

    //const char* pModel = "Both";
    HKEY hKey1, hKey2;

    // Create the path HKLM\Software\Classes\CLSID\{GUID}
    memset(wcID, NULL, sizeof(wcID));
    memset(szID, NULL, sizeof(szID));
    StringFromGUID2(CLSID_WmiPerfClass, wcID, sizeof(wcID) / sizeof(WCHAR));
    wcstombs_s(NULL, szID, wcID, sizeof(szID));
    StringCbCopy(szCLSID, sizeof(szCLSID), "Software\\classes\\CLSID\\");
    StringCbCat(szCLSID, sizeof(szCLSID), (LPCTSTR)szID);

    // Open key with the above path
    LONG lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, // Registry key
        szCLSID,    // Subkey
        0,          // Reserved
        NULL,       // The user-defined class type of this key
        REG_OPTION_BACKUP_RESTORE, // dwOptions: REG_OPTION_NON_VOLATILE => information is stored in a file and is preserved when the system is restarted
        KEY_WRITE,  // samDesired : A mask that specifies the access rights for the key to be created 
        NULL,       // lpSecurityAttributes
        &hKey1,     // A pointer to a variable that receives a handle to the opened or created key
        NULL);      // A pointer to a variable that receives one of the following disposition values: REG_CREATED_NEW_KEY or REG_OPENED_EXISTING_KEY
    if (lRet != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    // Open the InprocServer32 subkey
    lRet = RegCreateKeyEx(hKey1, "InprocServer32", 0, NULL, REG_OPTION_BACKUP_RESTORE, KEY_WRITE, NULL, &hKey2, NULL);
    if (lRet != ERROR_SUCCESS)
    {
        RegCloseKey(hKey1);
        return E_FAIL;
    }

    // set InprocServer32 (default) value to the module filepath
    lRet = RegSetValueEx(hKey2, NULL, 0, REG_SZ, (BYTE*)szOrgModule, strlen(szOrgModule) + 1);
    if (lRet != ERROR_SUCCESS)
    {
        RegCloseKey(hKey2);
        RegCloseKey(hKey1);
        return E_FAIL;
    }

    RegCloseKey(hKey1);
    RegCloseKey(hKey2);
    return NOERROR;
}


