/* Copyright (c) 1996-2019, OPC Foundation. All rights reserved.

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
    return ualds_server();
}

