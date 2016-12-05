/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: OPC UA Local Discovery Server
**
** Author: Hannes Mezger <hannes.mezger@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** This file is based on uaserver_settings_filebackend.c from the
** ANSI C based OPC UA Server SDK / Toolkit from Unified Automation GmbH.
**
******************************************************************************/
#ifndef __ZEROCONF_H__
#define __ZEROCONF_H__

#include <opcua_proxystub.h>

OpcUa_StatusCode ualds_zeroconf_start_registration();
void ualds_zeroconf_stop_registration();

void ualds_zeroconf_addRegistration(const char *szServerUri);
void ualds_zeroconf_removeRegistration(const char *szServerUri);

#endif /* __ZEROCONF_H__ */
