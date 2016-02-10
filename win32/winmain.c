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

#include <windows.h>
#include "../config.h"
#include "platform.h"

extern int main(int argc, char* argv[]);

/* On Windows we need this special main entry function when compiling
 * with subsystem:windows.
 * This function is only a wrapper and will call the standard C main().
 */
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    UALDS_UNUSED(hInstance);
    UALDS_UNUSED(hPrevInstance);
    UALDS_UNUSED(lpCmdLine);
    UALDS_UNUSED(nCmdShow);
    /* Call standard C main() function.
     * Luckily MS as defined the original parameters as undocumented feature:
     * __argc and __argv
     */
    return main(__argc, __argv);
}


