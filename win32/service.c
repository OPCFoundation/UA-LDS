/* Copyright (c) 1996-2020, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual-license scenario:
- RCL: for OPC Foundation members in good-standing
- GPL V2: everybody else

RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* system includes */
#include <tchar.h>
/* uastack includes */
#include <opcua_serverstub.h>
/* local includes */
#include "../config.h"
#include "../ualds.h"
#include "platform.h"
#include "log.h"

static SERVICE_STATUS_HANDLE g_svcStatusHandle;
static SERVICE_STATUS        g_svcStatus;

// used for Bonjour Service start-up
BOOL StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager);
BOOL WaitForPendingStateToFinish(SC_HANDLE schService, int pending_state, LPSERVICE_STATUS_PROCESS status);

static BOOL ServiceSetStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    g_svcStatus.dwCurrentState = dwCurrentState;
    g_svcStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_svcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
    {
        g_svcStatus.dwControlsAccepted = 0;
    }
    else
    {
        g_svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    }

    if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
    {
        g_svcStatus.dwCheckPoint = 0;
    }
    else
    {
        g_svcStatus.dwCheckPoint++;
    }

    /* report service status to SCM */
    return SetServiceStatus(g_svcStatusHandle, &g_svcStatus);
}

static void WINAPI ServiceControlHandler(DWORD dwCtrl)
{
    if (dwCtrl == SERVICE_CONTROL_STOP || dwCtrl == SERVICE_CONTROL_SHUTDOWN)
    {
        ualds_shutdown();
    }
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    int exitcode = 0;
    BOOL bRet;
    DWORD err;

    UALDS_UNUSED(dwArgc);
    UALDS_UNUSED(lpszArgv);

    g_svcStatusHandle = RegisterServiceCtrlHandler(UALDS_CONF_SERVICE_NAME, ServiceControlHandler);
    err = GetLastError();

    g_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_svcStatus.dwServiceSpecificExitCode = 0;
    g_svcStatus.dwCheckPoint = 0;
    g_svcStatus.dwWaitHint = 0;
    g_svcStatus.dwWin32ExitCode = 0;

    bRet = ServiceSetStatus(SERVICE_START_PENDING, NO_ERROR, 0);
    bRet = ServiceSetStatus(SERVICE_RUNNING, NO_ERROR, 0);

    exitcode = ualds_server();

    bRet = ServiceSetStatus(SERVICE_STOPPED, exitcode, 0);
}

BOOL StartBonjourService()
{
    BOOL success = TRUE;
    DWORD lastErrorCode = 0;
    DWORD dwBytesNeeded;
    TCHAR szMessage[256];
    SC_HANDLE appleBonjourServiceHandle;
    SERVICE_STATUS_PROCESS _status;
    LPSERVICE_STATUS_PROCESS status = &_status;
    DWORD bytesNeeded;
    BOOL queryOpcfBonjourServiceRetCode;
    BOOL queryApplefBonjourServiceRetCode;
    BOOL opcfBonjourStartServiceRetCode;
    BOOL controlServiceRetCode;
    BOOL waitSuccess;
    BOOL waitSuccess1;
    BOOL waitSuccess2;

    SC_HANDLE serviceDbHandle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (serviceDbHandle == NULL)
    {
        lastErrorCode = GetLastError();
        ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"));
        ualds_log(UALDS_LOG_ERR, "Error: Could not open Service Control Manager. (Missing Admin privileges). ErrorCode: %S", szMessage);
        return FALSE;
    }

    // verify if Apple-Bounjour is registered
    appleBonjourServiceHandle = OpenService(serviceDbHandle, APPLE_BONJOUR_SERVICE_NAME, SERVICE_QUERY_STATUS | 
                                            SERVICE_CHANGE_CONFIG | SERVICE_START | SERVICE_STOP | SERVICE_ENUMERATE_DEPENDENTS);

    if (appleBonjourServiceHandle == NULL)
    {
        // Apple-Bounjour is not regitered

        // verify if OPCF-Bonjour is registered
        SC_HANDLE opcfBonjourServiceHandle = OpenService(serviceDbHandle, OPCF_BONJOUR_SERVICE_NAME, SERVICE_QUERY_STATUS |
                                                SERVICE_CHANGE_CONFIG | SERVICE_START | SERVICE_STOP | SERVICE_ENUMERATE_DEPENDENTS);

        if (opcfBonjourServiceHandle == NULL)
        {
            lastErrorCode = GetLastError();
            ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: OpenService '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: OpenService '%S' failed with error(%i): %S", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            CloseServiceHandle(serviceDbHandle);
            return FALSE;
        }

        // verify if OPCF-Bonjour is running
        queryOpcfBonjourServiceRetCode = QueryServiceStatusEx(opcfBonjourServiceHandle, SC_STATUS_PROCESS_INFO,
            (LPBYTE)status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
        if (queryOpcfBonjourServiceRetCode == FALSE)
        {
            lastErrorCode = GetLastError();
            ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            CloseServiceHandle(opcfBonjourServiceHandle);
            CloseServiceHandle(serviceDbHandle);
            return FALSE;
        }

        // wait util the PENDING state has finished
        waitSuccess1 = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_START_PENDING, status);
        waitSuccess2 = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_STOP_PENDING, status);

        if (waitSuccess1 == FALSE || waitSuccess2 == FALSE)
        {
            CloseServiceHandle(opcfBonjourServiceHandle);
            CloseServiceHandle(serviceDbHandle);
            return FALSE;
        }

        // start the opcf-bonjour
        if (status->dwCurrentState == SERVICE_STOPPED)
        {
            // make sure that the service Startup-Type is Manual, not Disabled. A disabled service can not be started.
            //if (status->dwServiceType == SERVICE_DISABLED)
            {
                if (!ChangeServiceConfig(
                opcfBonjourServiceHandle, // handle of service
                SERVICE_NO_CHANGE, // service type: no change
                SERVICE_DEMAND_START, // dwStartType, change service startup type to manual
                SERVICE_NO_CHANGE, // error control: no change
                NULL, // binary path: no change
                NULL, // load order group: no change
                NULL, // tag ID: no change
                NULL, // dependencies: no change
                NULL, // account name: no change
                NULL, // password: no change
                NULL)) // display name: no change
                {
                    lastErrorCode = GetLastError();
                    ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                    _ftprintf(stderr, _T("Error: ChangeServiceConfig '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                    ualds_log(UALDS_LOG_ERR, "Error: ChangeServiceConfig '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                    CloseServiceHandle(opcfBonjourServiceHandle);
                    CloseServiceHandle(serviceDbHandle);
                    return FALSE;
                }

            }

            // start service.
            opcfBonjourStartServiceRetCode = StartService(opcfBonjourServiceHandle, 0, NULL);
            if (opcfBonjourStartServiceRetCode == FALSE)
            {
                lastErrorCode = GetLastError();
                ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: StartService '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: StartService '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }

            // Check the status
            if (!QueryServiceStatusEx(
                opcfBonjourServiceHandle,       // handle to service 
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE)status,                // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded))                // if buffer too small
            {
                lastErrorCode = GetLastError();
                ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }

            // Wait until the service is no longer start pending. 
            waitSuccess = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_START_PENDING, status);
            if (waitSuccess == FALSE)
            {
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }
        }

        CloseServiceHandle(opcfBonjourServiceHandle);
    }
    else
    {
        // try to stop opcf-bonfour, if it is running.

        // verify if OPCF-Bonjour is registered
        SC_HANDLE opcfBonjourServiceHandle = OpenService(serviceDbHandle, OPCF_BONJOUR_SERVICE_NAME, SERVICE_QUERY_STATUS |
                                                SERVICE_CHANGE_CONFIG | SERVICE_START | SERVICE_STOP | SERVICE_ENUMERATE_DEPENDENTS);

        if (opcfBonjourServiceHandle != NULL)
        {
            // verify if OPCF-Bonjour is running
            queryOpcfBonjourServiceRetCode = QueryServiceStatusEx(opcfBonjourServiceHandle, SC_STATUS_PROCESS_INFO,
                (LPBYTE)status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
            if (queryOpcfBonjourServiceRetCode == FALSE)
            {
                lastErrorCode = GetLastError();
                ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }

            // wait util the PENDING state has finished
            waitSuccess1 = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_START_PENDING, status);
            waitSuccess2 = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_STOP_PENDING, status);

            if (waitSuccess1 == FALSE || waitSuccess2 == FALSE)
            {
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(appleBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }

            if (status->dwCurrentState == SERVICE_RUNNING)
            {
                // if the service is running, dependencies must be stopped first.
                // the Bounjour Service does not have dependencies to it, but if this changes in the future, this case is covered. 
                StopDependentServices(opcfBonjourServiceHandle, serviceDbHandle);

                // stop the opcf-bonjour
                controlServiceRetCode = ControlService(opcfBonjourServiceHandle, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)status);
                if (controlServiceRetCode == 0)
                {
                    lastErrorCode = GetLastError();
                    ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                    _ftprintf(stderr, _T("Error: SERVICE_CONTROL_STOP '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                    ualds_log(UALDS_LOG_ERR, "Error: SERVICE_CONTROL_STOP '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                    CloseServiceHandle(opcfBonjourServiceHandle);
                    CloseServiceHandle(appleBonjourServiceHandle);
                    CloseServiceHandle(serviceDbHandle);
                    return FALSE;
                }

                // wait for the service to stop.
                waitSuccess = WaitForPendingStateToFinish(opcfBonjourServiceHandle, SERVICE_STOP_PENDING, status);
                if (waitSuccess == FALSE)
                {
                    CloseServiceHandle(opcfBonjourServiceHandle);
                    CloseServiceHandle(appleBonjourServiceHandle);
                    CloseServiceHandle(serviceDbHandle);
                    return FALSE;
                }

                // set the OPCF-Bonjour to disabled state
                if (status->dwServiceType != SERVICE_DISABLED)
                {
                    if (!ChangeServiceConfig(
                        opcfBonjourServiceHandle, // handle of service
                        SERVICE_NO_CHANGE, // service type: no change
                        SERVICE_DISABLED, // dwStartType, change service startup type to disabled
                        SERVICE_NO_CHANGE, // error control: no change
                        NULL, // binary path: no change
                        NULL, // load order group: no change
                        NULL, // tag ID: no change
                        NULL, // dependencies: no change
                        NULL, // account name: no change
                        NULL, // password: no change
                        NULL)) // display name: no change
                    {
                        lastErrorCode = GetLastError();
                        ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                        _ftprintf(stderr, _T("Error: ChangeServiceConfig '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                        ualds_log(UALDS_LOG_ERR, "Error: ChangeServiceConfig '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                        CloseServiceHandle(opcfBonjourServiceHandle);
                        CloseServiceHandle(appleBonjourServiceHandle);
                        CloseServiceHandle(serviceDbHandle);
                        return FALSE;
                    }
                } // end if (status.dwServiceType != SERVICE_DISABLED)
            } // end if (status.dwCurrentState == SERVICE_RUNNING) opcf-bonjour

            CloseServiceHandle(opcfBonjourServiceHandle);
        }

        // verify if Apple-Bonjour is running
        queryApplefBonjourServiceRetCode = QueryServiceStatusEx(appleBonjourServiceHandle, SC_STATUS_PROCESS_INFO,
            (LPBYTE)status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
        if (queryApplefBonjourServiceRetCode == FALSE)
        {
            lastErrorCode = GetLastError();
            ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), APPLE_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", APPLE_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
            CloseServiceHandle(opcfBonjourServiceHandle);
            CloseServiceHandle(appleBonjourServiceHandle);
            CloseServiceHandle(serviceDbHandle);
            return FALSE;
        }

        // wait util the PENDING state has finished
        waitSuccess1 = WaitForPendingStateToFinish(appleBonjourServiceHandle, SERVICE_START_PENDING, status);
        waitSuccess2 = WaitForPendingStateToFinish(appleBonjourServiceHandle, SERVICE_STOP_PENDING, status);
        if (waitSuccess1 == FALSE || waitSuccess2 == FALSE)
        {
            CloseServiceHandle(opcfBonjourServiceHandle);
            CloseServiceHandle(appleBonjourServiceHandle);
            CloseServiceHandle(serviceDbHandle);
            return FALSE;
        }

        if (status->dwCurrentState == SERVICE_STOPPED)
        {
            BOOL startServiceRetCode = StartService(appleBonjourServiceHandle, 0, NULL);
            if (startServiceRetCode == FALSE)
            {
                lastErrorCode = GetLastError();
                ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: StartService '%s' failed with error(%i): %s.\n"), APPLE_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: StartService '%S' failed with error(%i): %S.", APPLE_BONJOUR_SERVICE_NAME, lastErrorCode, szMessage);
                CloseServiceHandle(opcfBonjourServiceHandle);
                CloseServiceHandle(appleBonjourServiceHandle);
                CloseServiceHandle(serviceDbHandle);
                return FALSE;
            }

            // wait for the service to start.
            waitSuccess = WaitForPendingStateToFinish(appleBonjourServiceHandle, SERVICE_START_PENDING, status);
            if (waitSuccess == FALSE)
            {
                // nothing to do.
            }
        } // end if (status.dwCurrentState == SERVICE_STOPPED)
    } // end else apple-bonjour exists

    CloseServiceHandle(appleBonjourServiceHandle);
    CloseServiceHandle(serviceDbHandle);

    return success;
}

BOOL WaitForPendingStateToFinish(SC_HANDLE schService, int pending_state, LPSERVICE_STATUS_PROCESS status)
{
    DWORD dwWaitTime;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    BOOL success = TRUE;
    DWORD dwOldCheckPoint;

    // Save the tick count and initial checkpoint.
    dwStartTime = GetTickCount();
    dwOldCheckPoint = status->dwCheckPoint;

    while (status->dwCurrentState == pending_state)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 

        dwWaitTime = status->dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status again. 
        if (!QueryServiceStatusEx(
            schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // info level
            (LPBYTE)status,                 // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))                // if buffer too small
        {
            DWORD lastErrorCode = GetLastError();
            TCHAR szMessage[256];
            ualds_platform_errorstring(lastErrorCode, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx failed with error(%i): %s.\n"), lastErrorCode, szMessage);
            success = FALSE;
            break;
        }

        if (status->dwCurrentState != pending_state)
        {
            break;
        }

        if (status->dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.
            dwStartTime = GetTickCount();
            dwOldCheckPoint = status->dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTime > status->dwWaitHint)
            {
                // No progress made within the wait hint.
                success = FALSE;
                break;
            }
        }
    }

    return success;
}

BOOL StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager)
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if (EnumDependentServices(schService, SERVICE_ACTIVE,
        lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
        // If the Enum call succeeds, then there are no dependent
        // services, so do nothing.
        return TRUE;
    }
    else
    {
        if (GetLastError() != ERROR_MORE_DATA)
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

        if (!lpDependencies)
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if (!EnumDependentServices(schService, SERVICE_ACTIVE,
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount))
                return FALSE;

            for (i = 0; i < dwCount; i++)
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService(schSCManager,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!hDepService)
                    return FALSE;

                __try {
                    // Send a stop code.
                    if (!ControlService(hDepService,
                        SERVICE_CONTROL_STOP,
                        (LPSERVICE_STATUS)&ssp))
                        return FALSE;

                    // Wait for the service to stop.
                    while (ssp.dwCurrentState != SERVICE_STOPPED)
                    {
                        Sleep(ssp.dwWaitHint);
                        if (!QueryServiceStatusEx(
                            hDepService,
                            SC_STATUS_PROCESS_INFO,
                            (LPBYTE)&ssp,
                            sizeof(SERVICE_STATUS_PROCESS),
                            &dwBytesNeeded))
                            return FALSE;

                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                            break;

                        if (GetTickCount() - dwStartTime > dwTimeout)
                            return FALSE;
                    }
                }
                __finally
                {
                    // Always release the service handle.
                    CloseServiceHandle(hDepService);
                }
            }
        }
        __finally
        {
            // Always free the enumeration buffer.
            HeapFree(GetProcessHeap(), 0, lpDependencies);
        }
    }
    return TRUE;
}

int ServiceRegister(const char *szUser, const char *szPass)
{
    int ret = 0;
    SC_HANDLE hSCM = NULL;
    SC_HANDLE hService = NULL;

    _ftprintf(stderr, _T("Trying to register a service with the name '%s'.\n"), UALDS_CONF_SERVICE_NAME);

    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"));
        return 1;
    }

    /* test if service already exists */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_QUERY_CONFIG);

    if (hService == NULL)
    {
        TCHAR szApplicationFilePath[MAX_PATH] = TEXT("");
        TCHAR szApplicationFilePathWithQuotes[MAX_PATH + 10] = TEXT("\"");

        GetModuleFileName(NULL, szApplicationFilePath, MAX_PATH);

        _tcscat(szApplicationFilePathWithQuotes, szApplicationFilePath);
        _tcscat(szApplicationFilePathWithQuotes, TEXT("\""));

        if (szUser == 0 || strcmp(szUser, "SYSTEM") == 0)
        {
            hService = CreateService(
                hSCM,
                UALDS_CONF_SERVICE_NAME,
                UALDS_CONF_SERVICE_DISPLAYNAME,
                SERVICE_ALL_ACCESS,
                SERVICE_WIN32_OWN_PROCESS,
                SERVICE_AUTO_START,
                SERVICE_ERROR_NORMAL,
                szApplicationFilePathWithQuotes,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);
        }
        else
        {
#ifdef  UNICODE
            wchar_t wszUser[50];
            wchar_t wszPass[50];

            MultiByteToWideChar(CP_OEMCP, 0, szUser, -1, wszUser, 50);
            MultiByteToWideChar(CP_OEMCP, 0, szPass, -1, wszPass, 50);

            hService = CreateService(
                hSCM,
                UALDS_CONF_SERVICE_NAME,
                UALDS_CONF_SERVICE_DISPLAYNAME,
                SERVICE_ALL_ACCESS,
                SERVICE_WIN32_OWN_PROCESS,
                SERVICE_AUTO_START,
                SERVICE_ERROR_NORMAL,
                szApplicationFilePathWithQuotes,
                NULL,
                NULL,
                NULL,
                wszUser,
                wszPass);
#else
            hService = CreateService(
                hSCM,
                UALDS_CONF_SERVICE_NAME,
                UALDS_CONF_SERVICE_DISPLAYNAME,
                SERVICE_ALL_ACCESS,
                SERVICE_WIN32_OWN_PROCESS,
                SERVICE_AUTO_START,
                SERVICE_ERROR_NORMAL,
                szApplicationFilePathWithQuotes,
                NULL,
                NULL,
                NULL,
                szUser,
                szPass);
#endif
        }

        if (hService != NULL)
        {
            SERVICE_DESCRIPTION SD;

            /* set the service description */
            SD.lpDescription = UALDS_CONF_SERVICE_DESCRIPTION;
            ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &SD);

            _tprintf(_T("Successfully registered service '%s'.\n"), UALDS_CONF_SERVICE_NAME);
        }
    }
    else
    {
        _ftprintf(stderr, _T("Error: A service with the name '%s' does already exist.\n"), UALDS_CONF_SERVICE_NAME);
        ret = 1;
    }

    if (hService != NULL)
    {
        CloseServiceHandle(hService);
    }
    else
    {
        TCHAR szMessage[256] = { 0 };
        DWORD err = GetLastError();
        ualds_platform_errorstring(err, szMessage, sizeof(szMessage) / sizeof(TCHAR));
        _ftprintf(stderr, _T("Error: Registering the service '%s' failed with error(%i): %s.\n"), UALDS_CONF_SERVICE_NAME, err, szMessage);
        ret = 1;
    }
    CloseServiceHandle(hSCM);

    return ret;
}

int ServiceUnregister()
{
    int ret = 0;
    SC_HANDLE hSCM = NULL;
    SC_HANDLE hService = NULL;
    BOOL bRet;
    DWORD err = 0;
    TCHAR szMessage[100];

    _ftprintf(stderr, _T("Trying to unregister a service with the name '%s'.\n"), UALDS_CONF_SERVICE_NAME);

    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"));
        return 1;
    }

    /* test if service already exists */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_QUERY_STATUS | SERVICE_STOP | DELETE);

    if (hService != NULL)
    {
        // verify if LDS is running
        SERVICE_STATUS_PROCESS _status;
        DWORD bytesNeeded;
        BOOL queryLDSServiceRetCode = QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
            (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
        if (queryLDSServiceRetCode == FALSE)
        {
            err = GetLastError();
            ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCM);
            return FALSE;
        }

        if (_status.dwCurrentState != SERVICE_STOPPED)
        {
            // Stop the LDS
            SERVICE_STATUS status;
            ZeroMemory(&status, sizeof(status));
            bRet = ControlService(hService, SERVICE_CONTROL_STOP, &status);
            if (bRet == FALSE)
            {
                err = GetLastError();
                ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
                fprintf(stderr, "Error: Stopping service '%s' failed with error(%i): %s.\n", UALDS_CONF_SERVICE_NAME, err, szMessage);
            }

            // verify if LDS is stopped
            SERVICE_STATUS_PROCESS _status;
            DWORD bytesNeeded;
            BOOL queryLDSServiceRetCode = QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
            if (queryLDSServiceRetCode == FALSE)
            {
                err = GetLastError();
                ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
                CloseServiceHandle(hService);
                CloseServiceHandle(hSCM);
                return FALSE;
            }

            // Wait for LDS to stop.
            DWORD dwStartTime = GetTickCount();
            DWORD dwTimeout = 30000; // 30-second time-out
            while (_status.dwCurrentState != SERVICE_STOPPED)
            {
                DWORD sleep_millis = _status.dwWaitHint;
                if (sleep_millis == 0)
                {
                    sleep_millis = 1000;
                }
                Sleep(sleep_millis);
                if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                    (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded))
                {
                    printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
                    break;
                }

                if (_status.dwCurrentState == SERVICE_STOPPED)
                    break;

                if (GetTickCount() - dwStartTime > dwTimeout)
                {
                    printf("Wait timed out\n");
                    break;
                }
            }
            printf("Service stopped successfully\n");
        }

        /* Mark service for deletion. It will be deleted when all handles are closed. */
        bRet = DeleteService(hService);
        if (bRet == TRUE)
        {
            _tprintf(_T("Successfully unregistered service '%s'.\n"), UALDS_CONF_SERVICE_NAME);
        }
        else
        {
            err = GetLastError();
            _ftprintf(stderr, _T("Error: Unregistering service '%s' failed with error(%i): %s.\n"), UALDS_CONF_SERVICE_NAME, err, szMessage);
            ret = 1;
        }

        CloseServiceHandle(hService);
    }
    else
    {
        _ftprintf(stderr, _T("Warning: A service with the name '%s' does not exist.\n"), UALDS_CONF_SERVICE_NAME);
        ret = 0;
    }
    CloseServiceHandle(hSCM);

    return ret;
}

int ServiceStart()
{
    int ret = 0;
    SC_HANDLE hSCM = NULL;
    SC_HANDLE hService = NULL;
    BOOL bRet;
    DWORD err;
    TCHAR szMessage[256];

    hSCM = OpenSCManager(NULL, NULL, SERVICE_START);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"));
        return 1;
    }

    /* get service handle */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_START | SERVICE_QUERY_STATUS);
    if (hService != NULL)
    {
        // verify if LDS is running
        SERVICE_STATUS_PROCESS _status;
        DWORD bytesNeeded;
        BOOL queryLDSServiceRetCode = QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
            (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
        if (queryLDSServiceRetCode == FALSE)
        {
            err = GetLastError();
            ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCM);
            return FALSE;
        }

        if (_status.dwCurrentState != SERVICE_RUNNING)
        {
            bRet = StartService(hService, 0, NULL);
            if (bRet)
            {
                printf("Service started successfully.\n");
            }
            else
            {
                err = GetLastError();
                ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: StartService failed with error(%i): %s.\n"), err, szMessage);
            }
        }

        CloseServiceHandle(hService);
    }
    else
    {
        err = GetLastError();
        ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
        _ftprintf(stderr, _T("Error: OpenService '%s' failed with error(%i): %s.\n"), UALDS_CONF_SERVICE_NAME, err, szMessage);
        ret = 1;
    }

    CloseServiceHandle(hSCM);

    return ret;
}

int ServiceStop()
{
    int ret = 0;
    SC_HANDLE hSCM = NULL;
    SC_HANDLE hService = NULL;
    BOOL bRet;
    DWORD err;
    TCHAR szMessage[256];

    hSCM = OpenSCManager(NULL, NULL, SERVICE_STOP);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"));
        return 1;
    }

    /* get service handle */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (hService != NULL)
    {
        // verify if LDS is stopped
        SERVICE_STATUS_PROCESS _status;
        DWORD bytesNeeded;
        BOOL queryLDSServiceRetCode = QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
            (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
        if (queryLDSServiceRetCode == FALSE)
        {
            err = GetLastError();
            ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCM);
            return FALSE;
        }

        if (_status.dwCurrentState != SERVICE_STOPPED)
        {
            SERVICE_STATUS stat;
            bRet = ControlService(hService, SERVICE_CONTROL_STOP, &stat);
            if (bRet == FALSE)
            {
                err = GetLastError();
                ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
                fprintf(stderr, "Error: Stopping service '%s' failed with error(%i): %s.\n", UALDS_CONF_SERVICE_NAME, err, szMessage);
            }

            // verify if LDS is stopped
            SERVICE_STATUS_PROCESS _status;
            DWORD bytesNeeded;
            BOOL queryLDSServiceRetCode = QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
            if (queryLDSServiceRetCode == FALSE)
            {
                err = GetLastError();
                ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
                _ftprintf(stderr, _T("Error: QueryServiceStatusEx '%s' failed with error(%i): %s.\n"), OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
                ualds_log(UALDS_LOG_ERR, "Error: QueryServiceStatusEx '%S' failed with error(%i): %S.", OPCF_BONJOUR_SERVICE_NAME, err, szMessage);
                CloseServiceHandle(hService);
                CloseServiceHandle(hSCM);
                return FALSE;
            }

            // Wait for LDS to stop.
            DWORD dwStartTime = GetTickCount();
            DWORD dwTimeout = 30000; // 30-second time-out
            while (_status.dwCurrentState != SERVICE_STOPPED)
            {
                DWORD sleep_millis = _status.dwWaitHint;
                if (sleep_millis == 0)
                {
                    sleep_millis = 1000;
                }
                Sleep(sleep_millis);
                if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                    (LPBYTE)&_status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded))
                {
                    printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
                    break;
                }

                if (_status.dwCurrentState == SERVICE_STOPPED)
                    break;

                if (GetTickCount() - dwStartTime > dwTimeout)
                {
                    printf("Wait timed out\n");
                    break;
                }
            }
            printf("Service stopped successfully\n");
        }

        CloseServiceHandle(hService);
    }
    else
    {
        err = GetLastError();
        ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
        _ftprintf(stderr, _T("Error: OpenService '%s' failed with error(%i): %s.\n"), UALDS_CONF_SERVICE_NAME, err, szMessage);
        ret = 1;
    }

    CloseServiceHandle(hSCM);

    return ret;
}

int ServiceStatus()
{
    int ret = 0;

    return ret;
}

