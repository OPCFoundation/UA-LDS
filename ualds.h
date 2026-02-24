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

int ualds_server(void);
void ualds_shutdown(void);
void ualds_reload(void);
void ualds_expirationcheck(void);

const ualds_endpoint* ualds_endpoints(OpcUa_UInt32 *pNumEndpoints);
const char* ualds_serveruri(void);
const char* ualds_producturi(void);
const char* ualds_applicationname(const char *szLocale);

// param flush 
//          != 0 => flush to disk
//          == 0 => don't flush to disk
int ualds_settings_cleanup(int flush);

/*============================================================================
 * Helper Macro For Building Response Header
 *===========================================================================*/
#define UALDS_BUILDRESPONSEHEADER                                                   \
    pResponse->ResponseHeader.Timestamp     = OpcUa_DateTime_UtcNow();              \
    pResponse->ResponseHeader.RequestHandle = pRequest->RequestHeader.RequestHandle;\
    pResponse->ResponseHeader.ServiceResult = uStatus;\
    uStatus                                 = OpcUa_Good

#endif /* __UALDS_H__ */
