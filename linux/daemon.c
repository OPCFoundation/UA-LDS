/* Copyright (c) 1996-2021, OPC Foundation. All rights reserved.

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

/* uastack includes */
#include <opcua_serverstub.h>
/* local platform includes */
#include <platform.h>
/* locale includes */
#include "../config.h"
#include "../ualds.h"
#include "log.h"

void signal_handler(int sig)
{
    ualds_log(UALDS_LOG_NOTICE, "Received signal %d. Terminating now.", sig);
    ualds_shutdown();
}

/** Starts the windows service and returns. */
int daemonize()
{
    daemon(0, 0);
    return ualds_server();
}

/** just runs the application without daemonizing. */
int run()
{
    signal(SIGINT, signal_handler);
    return ualds_server();
}

