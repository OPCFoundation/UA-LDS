/* Copyright (c) 1996-2023, OPC Foundation. All rights reserved.

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

#include "utils.h"
//#include <opcua_statuscodes.h>
//#include <opcua_errorhandling.h>

/* uastack includes */
#include <opcua_timer.h>
#include <opcua_socket.h>
#include <opcua_list.h>
#include <opcua_memory.h>
#include <opcua_endpoint.h>
#include <opcua_core.h>
/* local platform includes */
#include <platform.h>
#include <log.h>
#include <opcua_p_socket.h>

#include "tlds-alpha-by-domain.h"

typedef struct _TLDStruct
{
    char tlds[2000][100];
    int nrTlds;
} TLDStruct;

static TLDStruct tld_struct;

OpcUa_StatusCode ualds_parse_url(char *szUrl, char **szScheme, char **szHostname, uint16_t *port, char **szPath)
{
    char *szTmp = szUrl;

    *szScheme = OpcUa_Null;
    *szHostname = OpcUa_Null;
    *port = 4840;
    *szPath = OpcUa_Null;

    *szScheme = szTmp;
    while (*szTmp != 0 && *szTmp != ':') { szTmp++; }
    if (*szTmp == 0) { return OpcUa_BadInvalidArgument; }
    *szTmp = 0;

    szTmp++;
    if (*szTmp != '/') { return OpcUa_BadInvalidArgument; }
    szTmp++;
    if (*szTmp != '/') { return OpcUa_BadInvalidArgument; }
    szTmp++;

    *szHostname = szTmp;
    if (*szTmp == '[')
    {
        while (*szTmp != 0 && *szTmp != ']') { szTmp++; }
    }
    while (*szTmp != 0 && *szTmp != ':' && *szTmp != '/') { szTmp++; }
    if (*szTmp == 0) { return OpcUa_Good; }

    if (*szTmp == ':')
    {
        char *szPort = OpcUa_Null;
        *szTmp = 0;
        szPort = ++szTmp;
        *port = (uint16_t)strtol(szPort, OpcUa_Null, 10);
        while (*szTmp != 0 && *szTmp != '/') { szTmp++; }
    }

    if (*szTmp == '/')
    {
        *szPath = szTmp;
    }

    // if szHostname is between square brackets, remove them (it is used for IPv6).
    int szHostnameLen = strlen(*szHostname);
    if (szHostnameLen >= 2 && (*szHostname)[0] == '[' && (*szHostname)[szHostnameLen-1] == ']')
    {
    	(*szHostname)[szHostnameLen-1] = 0;
    	(*szHostname) += 1;
    }

    return OpcUa_Good;
}

int is_Host_IP4Address(const char* host)
{
    int a1, b1, c1, d1;

    if (sscanf(host, "%d.%d.%d.%d", &a1, &b1, &c1, &d1) != 4)
    {
        return -1;
    }

    if (a1 < 0 || a1 > 255)
    {
        return -1;
    }

    if (b1 < 0 || b1 > 255)
    {
        return -1;
    }

    if (c1 < 0 || c1 > 255)
    {
        return -1;
    }

    if (d1 < 0 || d1 > 255)
    {
        return -1;
    }
    return 0;
}

int isTLD(char* domain)
{
    int i = 0;
    for (i = 0; i < tld_struct.nrTlds; ++i)
    {
        if (ualds_platform_strcpy_insensitive(domain, tld_struct.tlds[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

void loadKnownTLD()
{
    // load known Top Level Domains (TLD) 
    tld_struct.nrTlds = 0;

    char * pch;
    pch = strtok(tld_str, " ");
    while (pch != NULL)
    {
        strcpy(tld_struct.tlds[tld_struct.nrTlds], ".");
        strcat(tld_struct.tlds[tld_struct.nrTlds], pch);
        tld_struct.nrTlds++;
        pch = strtok(NULL, " ");
    }
}
