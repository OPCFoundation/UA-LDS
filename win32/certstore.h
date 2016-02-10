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
******************************************************************************/
#ifndef __CERTSTORE_H__
#define __CERTSTORE_H__

#include <opcua_proxystub.h>

OpcUa_StatusCode ualds_verify_cert_win32(OpcUa_ByteString* pbsClientCertificate);

#endif /* __CERTSTORE_H__ */
