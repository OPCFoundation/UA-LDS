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
/* local platform includes */
#include <platform.h>
/* locale includes */
#include "../config.h"
#include "../ualds.h"
#include "service.h"
#include "log.h"

BOOL CtrlHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
        ualds_log(UALDS_LOG_NOTICE, "Received CTRL_C_EVENT. Terminating now.");
        ualds_shutdown();
        return TRUE;
    case CTRL_CLOSE_EVENT:
        ualds_log(UALDS_LOG_NOTICE, "Received CTRL_CLOSE_EVENT. Terminating now.");
        ualds_shutdown();
        return TRUE;
    case CTRL_BREAK_EVENT:
        ualds_log(UALDS_LOG_NOTICE, "Received CTRL_BREAK_EVENT. Terminating now.");
        ualds_shutdown();
        return TRUE;
    case CTRL_SHUTDOWN_EVENT:
        ualds_log(UALDS_LOG_NOTICE, "Received CTRL_SHUTDOWN_EVENT. Terminating now.");
        ualds_shutdown();
        return TRUE;
    default:
        return FALSE;
    }
}

static void install_signal_handlers()
{
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
    {
        ualds_log(UALDS_LOG_INFO, "Installed Control Handler.");
    }
    else
    {
        ualds_log(UALDS_LOG_ERR, "Installing Control Handler failed.");
    }
}

/** Starts the windows service and returns. */
int daemonize()
{
    BOOL bRet;
    SERVICE_TABLE_ENTRY dispatchTable[] =
    {
        { UALDS_CONF_SERVICE_NAME, ServiceMain },
        { NULL, NULL }
    };

    bRet = StartServiceCtrlDispatcher(dispatchTable);
    if (bRet)
    {
        fprintf(stdout, "Successfully started service.");
    }
    else
    {
        TCHAR szMessage[100];
        DWORD err = GetLastError();
        ualds_platform_errorstring(err, szMessage, sizeof(szMessage));
        _ftprintf(stderr, _T("Error: Starting service '%s' failed with error(%i): %s.\n"), UALDS_CONF_SERVICE_NAME, err, szMessage);
    }

    return bRet == TRUE ? 0 : 1;
}

/** just runs the application without daemonizing. */
int run()
{
    install_signal_handlers();
    return ualds_serve();
}

