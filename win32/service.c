/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: OPC UA Local Discovery Server
**
** Author: Gerhard Gappmeier <gerhard.gappmeier@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
******************************************************************************/

/* system includes */
#include <tchar.h>
/* uastack includes */
#include <opcua_serverstub.h>
/* local includes */
#include "../config.h"
#include "../ualds.h"
#include "platform.h"

static SERVICE_STATUS_HANDLE g_svcStatusHandle;
static SERVICE_STATUS        g_svcStatus;

static BOOL ServiceSetStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    g_svcStatus.dwCurrentState  = dwCurrentState;
    g_svcStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_svcStatus.dwWaitHint      = dwWaitHint;

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

    g_svcStatus.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
    g_svcStatus.dwServiceSpecificExitCode = 0;
    g_svcStatus.dwCheckPoint              = 0;
    g_svcStatus.dwWaitHint                = 0;
    g_svcStatus.dwWin32ExitCode           = 0;

    bRet = ServiceSetStatus(SERVICE_START_PENDING, NO_ERROR, 0);
    bRet = ServiceSetStatus(SERVICE_RUNNING, NO_ERROR, 0);

    exitcode = ualds_serve();

    bRet = ServiceSetStatus(SERVICE_STOPPED, exitcode, 0);
}

int ServiceRegister(const char *szUser, const char *szPass)
{
    int ret = 0;
    SC_HANDLE hSCM = NULL;
    SC_HANDLE hService = NULL;

    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"), UALDS_CONF_SERVICE_NAME);
        return 1;
    }

    /* test if service already exists */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_QUERY_CONFIG);

    if (hService == NULL)
    {
        TCHAR szApplicationFilePath[MAX_PATH] = TEXT("");

        GetModuleFileName(NULL, szApplicationFilePath, MAX_PATH);

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
                szApplicationFilePath,
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
                szApplicationFilePath,
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
                szApplicationFilePath,
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
        TCHAR szMessage[256] = {0};
        DWORD err = GetLastError();
        ualds_platform_errorstring(err, szMessage, sizeof(szMessage)/sizeof(TCHAR));
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

    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"), UALDS_CONF_SERVICE_NAME);
        return 1;
    }

    /* test if service already exists */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_STOP | DELETE);

    if (hService != NULL)
    {
        SERVICE_STATUS status;
        ZeroMemory(&status, sizeof(status));
        bRet = ControlService(hService, SERVICE_CONTROL_STOP, &status);
        if (bRet == FALSE)
        {
            err = GetLastError();
            ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
            fprintf(stderr, "Error: Stopping service '%s' failed with error(%i): %s.\n", UALDS_CONF_SERVICE_NAME, err, szMessage);
        }

        /* Mark service for deletion. It will b deleted when all handles are closed. */
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
        _ftprintf(stderr, _T("Error: A service with the name '%s' does not exist.\n"), UALDS_CONF_SERVICE_NAME);
        ret = 1;
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
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"), UALDS_CONF_SERVICE_NAME);
        return 1;
    }

    /* get service handle */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_START);
    if (hService != NULL)
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
        _ftprintf(stderr, _T("Error: Could not open Service Control Manager. (Missing Admin privileges)\n"), UALDS_CONF_SERVICE_NAME);
        return 1;
    }

    /* get service handle */
    hService = OpenService(hSCM, UALDS_CONF_SERVICE_NAME, SERVICE_STOP);
    if (hService != NULL)
    {
        SERVICE_STATUS stat;
        bRet = ControlService(hService, SERVICE_CONTROL_STOP, &stat);
        if (bRet)
        {
            printf("Service stopped successfully.\n");
        }
        else
        {
            err = GetLastError();
            ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
            _ftprintf(stderr, _T("Error: ControlService failed with error(%i): %s.\n"), err, szMessage);
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

