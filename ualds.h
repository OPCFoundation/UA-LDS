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

void ualds_endpoint_initialize(ualds_endpoint *pEndpoint);
void ualds_endpoint_clear(ualds_endpoint *pEndpoint);

int ualds_serve();
void ualds_shutdown();
void ualds_reload();
void ualds_expirationcheck();

const ualds_endpoint* ualds_endpoints(OpcUa_UInt32 *pNumEndpoints);
const char* ualds_serveruri();
const char* ualds_producturi();
const char* ualds_applicationname(const char *szLocale);

/*============================================================================
 * Helper Macro For Building Response Header
 *===========================================================================*/
#define UALDS_BUILDRESPONSEHEADER                                                   \
    pResponse->ResponseHeader.Timestamp     = OpcUa_DateTime_UtcNow();              \
    pResponse->ResponseHeader.RequestHandle = pRequest->RequestHeader.RequestHandle;\
    pResponse->ResponseHeader.ServiceResult = uStatus;\
    uStatus                                 = OpcUa_Good

#endif /* __UALDS_H__ */
