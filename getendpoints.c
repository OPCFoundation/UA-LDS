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

/* system includes */
#include <string.h>
/* uastack includes */
#include <opcua_serverstub.h>
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_core.h>
#include <opcua_datetime.h>
/* local includes */
#include "config.h"
#include "ualds.h"
#include "settings.h"
/* local platform includes */
#include <platform.h>
#include <log.h>

/** TODO: remove this hack */
extern OpcUa_ByteString g_server_certificate;

/** GetEndpoints Service implementation.
* @param hEndpoint OPC UA Endpoint handle.
* @param hContext  Service context.
* @param ppRequest Pointer to decoded service request structure.
* @param pRequestType The service type.
*
* @return OpcUa_StatusCode
*/
OpcUa_StatusCode ualds_getendpoints(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_Void          **ppRequest,
    OpcUa_EncodeableType *pRequestType)
{
    OpcUa_GetEndpointsRequest  *pRequest;
    OpcUa_GetEndpointsResponse *pResponse;
    OpcUa_EncodeableType       *pResponseType = 0;
    OpcUa_UInt32                numEndpoints, numMessageModes;
    const ualds_endpoint       *pEP = ualds_endpoints(&numEndpoints);
    OpcUa_StatusCode            uStatus = OpcUa_Good;
    char                        szHostname[50];
    char                        szTmpUrl[UALDS_CONF_MAX_URI_LENGTH];
    int                         numDiscoveryUrls = 0;
    int                         iDiscoveryUrl;
    OpcUa_UInt32 i, j, k, index;
    OpcUa_UInt16 mode;

    UALDS_UNUSED(pRequestType);

    OpcUa_ReturnErrorIfArgumentNull(ppRequest);
    pRequest = *ppRequest;

    ualds_platform_getfqhostname(szHostname, 50);

    uStatus = OpcUa_Endpoint_BeginSendResponse(
        hEndpoint,
        hContext,
        (OpcUa_Void**)&pResponse,
        &pResponseType);
    OpcUa_ReturnErrorIfBad(uStatus);

    if ( pResponse )
    {
        pResponse->NoOfEndpoints = 0;
        /* counting: we need one endpoint description for each mesage mode in each security policy in each real endpoint.
         * sounds stupid, but that's how it is defiend in the spec.
         */
        for (i=0; i<numEndpoints; i++)
        {
            /* filter URL */
            /*if (strcmp(pEP[i].szUrl, OpcUa_String_GetRawString(&pRequest->EndpointUrl)) != 0)
            {
                ualds_log(UALDS_LOG_DEBUG, "Filtering endpoint url '%s', client requestet '%s'",
                          pEP[i].szUrl,
                          OpcUa_String_GetRawString(&pRequest->EndpointUrl));
                continue;
            }*/

            /* iterate of all security policies */
            for (j=0; j<pEP[i].nNoOfSecurityPolicies; j++)
            {
                /* count number of message modes */
                numMessageModes = 0;
                if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_NONE) != 0) numMessageModes++;
                if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGN) != 0) numMessageModes++;
                if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGNANDENCRYPT) != 0) numMessageModes++;
                pResponse->NoOfEndpoints += numMessageModes;
            }
        }
        /* now allocate the array and fill it */
        pResponse->Endpoints = OpcUa_Alloc(sizeof(OpcUa_EndpointDescription) * pResponse->NoOfEndpoints);
        if (pResponse->Endpoints)
        {
            index = 0;
            for (i=0; i<numEndpoints; i++)
            {
                /* filter URL */
                /*if (strcmp(pEP[i].szUrl, OpcUa_String_GetRawString(&pRequest->EndpointUrl)) != 0) continue;*/

                for (j=0; j<pEP[i].nNoOfSecurityPolicies; j++)
                {
                    /* count number of message modes */
                    numMessageModes = 0;
                    if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_NONE) != 0) numMessageModes++;
                    if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGN) != 0) numMessageModes++;
                    if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGNANDENCRYPT) != 0) numMessageModes++;

                    for (k=0, mode=1; k<3; k++, mode<<=1)
                    {
                        char szApplicationUri[UALDS_CONF_MAX_URI_LENGTH];

                        if ((pEP[i].pSecurityPolicies[j].uMessageSecurityModes & mode) == 0) continue;

                        OpcUa_EndpointDescription_Initialize(&pResponse->Endpoints[index]);
                        /* fill endpoint url */
                        OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].EndpointUrl, (const OpcUa_StringA)pEP[i].szUrl);
                        /* fill application description */
                        OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].Server.ApplicationName.Locale, "en-US");
                        OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].Server.ApplicationName.Text, (const OpcUa_StringA)ualds_applicationname("en-US"));
                        strlcpy(szApplicationUri, ualds_serveruri(), UALDS_CONF_MAX_URI_LENGTH);
                        replace_string(szApplicationUri, sizeof(szApplicationUri), "[gethostname]", szHostname);
                        OpcUa_String_AttachCopy(&pResponse->Endpoints[index].Server.ApplicationUri, szApplicationUri);
                        OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].Server.ProductUri, (const OpcUa_StringA)ualds_producturi());
                        pResponse->Endpoints[index].Server.ApplicationType = OpcUa_ApplicationType_DiscoveryServer;

						OpcUa_Mutex_Lock(g_mutex);

                        ualds_settings_begingroup(ualds_serveruri());
                        ualds_settings_beginreadarray("DiscoveryUrls", &numDiscoveryUrls);
                        if (numDiscoveryUrls > 0)
                        {
                            pResponse->Endpoints[index].Server.NoOfDiscoveryUrls = numDiscoveryUrls;
                            pResponse->Endpoints[index].Server.DiscoveryUrls = OpcUa_Alloc(sizeof(OpcUa_String) * numDiscoveryUrls);
                            if (pResponse->Endpoints[index].Server.DiscoveryUrls)
                            {
                                for (iDiscoveryUrl=0; iDiscoveryUrl<numDiscoveryUrls; iDiscoveryUrl++)
                                {
                                    ualds_settings_setarrayindex(iDiscoveryUrl);
                                    ualds_settings_readstring("Url", szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);
                                    replace_string(szTmpUrl, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);
                                    OpcUa_String_Initialize(&pResponse->Endpoints[index].Server.DiscoveryUrls[iDiscoveryUrl]);
                                    OpcUa_String_AttachCopy(&pResponse->Endpoints[index].Server.DiscoveryUrls[iDiscoveryUrl], szTmpUrl);
                                }
                            }
                        }
                        ualds_settings_endarray();
                        ualds_settings_endgroup();

						OpcUa_Mutex_Unlock(g_mutex);

                        /* set security policy */
                        OpcUa_String_StrnCpy(&pResponse->Endpoints[index].SecurityPolicyUri, &pEP[i].pSecurityPolicies[j].sSecurityPolicy, OPCUA_STRING_LENDONTCARE);
                        /* tranport profile uri */
                        OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].TransportProfileUri, "http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");
                        /* fill server certificate */
                        pResponse->Endpoints[index].ServerCertificate.Length = g_server_certificate.Length;
                        if (g_server_certificate.Length >= 0)
                        {
                            pResponse->Endpoints[index].ServerCertificate.Data = OpcUa_Memory_Alloc(g_server_certificate.Length);
                            memcpy(pResponse->Endpoints[index].ServerCertificate.Data, g_server_certificate.Data, g_server_certificate.Length);
                        }
                        /* set security level */
                        if (strcmp(OpcUa_String_GetRawString(&pEP[i].pSecurityPolicies[j].sSecurityPolicy), OpcUa_SecurityPolicy_Basic256) == 0)
                        {
                            pResponse->Endpoints[index].SecurityLevel = 3;
                        }
                        else if (strcmp(OpcUa_String_GetRawString(&pEP[i].pSecurityPolicies[j].sSecurityPolicy), OpcUa_SecurityPolicy_Basic128Rsa15) == 0)
                        {
                            pResponse->Endpoints[index].SecurityLevel = 2;
                        }
                        else if (strcmp(OpcUa_String_GetRawString(&pEP[i].pSecurityPolicies[j].sSecurityPolicy), OpcUa_SecurityPolicy_None) == 0)
                        {
                            pResponse->Endpoints[index].SecurityLevel = 1;
                        }
                        else
                        {
                            pResponse->Endpoints[index].SecurityLevel = 0; /* don't use this */
                            ualds_log(UALDS_LOG_ERR, "Configured unsupported security policy: %s", OpcUa_String_GetRawString(&pEP[i].pSecurityPolicies[j].sSecurityPolicy));
                        }
                        /* set security mode */
                        switch (pEP[i].pSecurityPolicies[j].uMessageSecurityModes & mode)
                        {
                        case OPCUA_ENDPOINT_MESSAGESECURITYMODE_NONE:
                            pResponse->Endpoints[index].SecurityMode = OpcUa_MessageSecurityMode_None;
                            break;
                        case OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGN:
                            pResponse->Endpoints[index].SecurityMode = OpcUa_MessageSecurityMode_Sign;
                            break;
                        case OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGNANDENCRYPT:
                            pResponse->Endpoints[index].SecurityMode = OpcUa_MessageSecurityMode_SignAndEncrypt;
                            break;
                        default:
                            break;
                        }
                        /* set user tokens */
                        pResponse->Endpoints[index].NoOfUserIdentityTokens = 1;
                        pResponse->Endpoints[index].UserIdentityTokens = OpcUa_Alloc(sizeof(OpcUa_UserTokenPolicy));
                        if (pResponse->Endpoints[index].UserIdentityTokens)
                        {
                            OpcUa_UserTokenPolicy_Initialize(&pResponse->Endpoints[index].UserIdentityTokens[0]);
                            pResponse->Endpoints[index].UserIdentityTokens[0].TokenType = OpcUa_UserTokenType_Anonymous;
                            OpcUa_String_AttachReadOnly(&pResponse->Endpoints[index].UserIdentityTokens[0].PolicyId, "0");
                        }
                        index++;
                    }
                }
            }
        }
        else
        {
            uStatus = OpcUa_BadOutOfMemory;
        }

        UALDS_BUILDRESPONSEHEADER;

        /* Send response */
        OpcUa_Endpoint_EndSendResponse(
            hEndpoint,
            &hContext,
            uStatus,
            pResponse,
            pResponseType);

        /* free response */
        OpcUa_GetEndpointsResponse_Clear(pResponse);
        OpcUa_Free(pResponse);

        /* we have send a response, either with good or bad status, this does not matter.
         * we must return Good now, otherwise the stack will send an error message.
         */
        uStatus = OpcUa_Good;
    }

    return uStatus;
}

/** @} */

