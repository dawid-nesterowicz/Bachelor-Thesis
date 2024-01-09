//*******************************************************************
//  UTILS.CPP
//  Module: WMI Instance provider sample code
//  Purpose: General purpose utilities.  
// (c) Microsoft. All rights reserved.
//
//*******************************************************************

#include "malprovider.h"
#include <objbase.h>


/********************************************************************
 *
 * Name: GetCurrentImpersonationLevel
 * Description: Get COM impersonation level of caller.
 *
 *******************************************************************/

DWORD GetCurrentImpersonationLevel()
{
    DWORD t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
    HANDLE t_ThreadToken = NULL;
    BOOL t_Status = OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &t_ThreadToken);

    if (t_Status)
    {
        SECURITY_IMPERSONATION_LEVEL t_Level = SecurityAnonymous;
        DWORD t_Returned = 0;

        t_Status = GetTokenInformation(t_ThreadToken,
            TokenImpersonationLevel,
            &t_Level,
            sizeof(SECURITY_IMPERSONATION_LEVEL),
            &t_Returned);

        CloseHandle(t_ThreadToken);

        if (t_Status == FALSE)
        {
            t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
        }
        else
        {
            switch (t_Level)
            {
            case SecurityAnonymous:
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
            }
            break;

            case SecurityIdentification:
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_IDENTIFY;
            }
            break;

            case SecurityImpersonation:
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
            }
            break;

            case SecurityDelegation:
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_DELEGATE;
            }
            break;

            default:
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
            }
            break;
            }
        }
    }
    else
    {
        ULONG t_LastError = GetLastError();

        if (t_LastError == ERROR_NO_IMPERSONATION_TOKEN ||
            t_LastError == ERROR_NO_TOKEN)
        {
            t_ImpersonationLevel = RPC_C_IMP_LEVEL_DELEGATE;
        }
        else
        {
            if (t_LastError == ERROR_CANT_OPEN_ANONYMOUS)
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
            }
            else
            {
                t_ImpersonationLevel = RPC_C_IMP_LEVEL_ANONYMOUS;
            }
        }
    }

    return t_ImpersonationLevel;
}

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        //printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        //printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        //printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}