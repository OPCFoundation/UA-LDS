/* Copyright (c) 1996-2017, OPC Foundation. All rights reserved.

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

#ifndef __UALDS_H__
#define __UALDS_H__

/** UA LDS Endpoint configuration structure.
 * Unlike the OpcUa_EndpointDescription this is a technical configuration,
 * which defines endpoint = url = IP/port combination, which is required
 * for creating the sockets.
 * OpcUa_EndpointDescription defines one endpoint to be the combination
 * of one url + one security configuration + one messagemode. This requires
 * to create individual endpoints for each permutation of url and security configurations.
 * (complete nonesense!).
 */
struct _ualds_endpoint
{
    char                                        szUrl[UALDS_CONF_MAX_URI_LENGTH];
    OpcUa_UInt32                                nNoOfSecurityPolicies;
    OpcUa_Endpoint_SecurityPolicyConfiguration *pSecurityPolicies;
    OpcUa_Endpoint                              hEndpoint; /**< handle of open endpoint */
};
typedef struct _ualds_endpoint ualds_endpoint;

typedef struct _multicastSocketCallbackStruct
{
    OpcUa_Void*	 sdRef;
    OpcUa_Void*  context;
} MulticastSocketCallbackStruct;

extern OpcUa_Mutex g_mutex;
extern int g_bEnableZeroconf;

void ualds_endpoint_initialize(ualds_endpoint *pEndpoint);
void ualds_endpoint_clear(ualds_endpoint *pEndpoint);

int ualds_server();
void ualds_shutdown();
void ualds_reload();
void ualds_expirationcheck();

const ualds_endpoint* ualds_endpoints(OpcUa_UInt32 *pNumEndpoints);
const char* ualds_serveruri();
const char* ualds_producturi();
const char* ualds_applicationname(const char *szLocale);

int ualds_settings_cleanup();

/*============================================================================
 * Helper Macro For Building Response Header
 *===========================================================================*/
#define UALDS_BUILDRESPONSEHEADER                                                   \
    pResponse->ResponseHeader.Timestamp     = OpcUa_DateTime_UtcNow();              \
    pResponse->ResponseHeader.RequestHandle = pRequest->RequestHeader.RequestHandle;\
    pResponse->ResponseHeader.ServiceResult = uStatus;\
    uStatus                                 = OpcUa_Good

#endif /* __UALDS_H__ */
