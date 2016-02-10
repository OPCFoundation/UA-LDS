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
#ifndef __SERVICE_H__
#define __SERVICE_H__

/* system includes */
#include <windows.h>

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
int ServiceRegister();
int ServiceUnregister();
int ServiceStart();
int ServiceStop();
int ServiceStatus();

#endif /* __SERVICE_H__ */