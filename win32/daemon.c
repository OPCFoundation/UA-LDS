/* ========================================================================
* Copyright (c) 2005-2026 The OPC Foundation, Inc. All rights reserved.
*
* OPC Foundation MIT License 1.00
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* The complete license agreement can be found here:
* http://opcfoundation.org/License/MIT/1.00/
* ======================================================================*/

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

