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

#ifndef UTILS
#define UTILS

#include <opcua_proxystub.h>
#include <platform.h>
#include <opcua_p_types.h>

OpcUa_StatusCode ualds_parse_url(char *szUrl, char **szScheme, char **szHostname, uint16_t *port, char **szPath);
int is_Host_IP4Address(const char* host);
int isTLD(char* domain);
void loadKnownTLD();

#endif /* UTILS */