/* Copyright (c) 1996-2019, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual - license scenario :
- RCL: for OPC Foundation members in good - standing
- GPL V2: everybody else

RCL license terms accompanied with this source code.See http ://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code.See http ://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <stdlib.h>
#include "zeroconf.h"

/* DNS-SD includes */
#include <dns_sd.h>
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

/* local includes */
#include "config.h"
#include "settings.h"
#include "ualds.h"
#include "utils.h"

/* Globals for zeroconf registration */

static OpcUa_List g_registerServersSocketList;

typedef enum _ualds_RegistrationStatus
{
    RegistrationStatus_Unregistered,
    RegistrationStatus_Registering,
    RegistrationStatus_Registered
} ualds_RegistrationStatus;

typedef struct _ualds_registerContext
{
    DNSServiceRef               sdRef;
    OpcUa_Socket                uaSocket;
    ualds_RegistrationStatus    registrationStatus;
    char                        szServerUri[UALDS_CONF_MAX_URI_LENGTH];
    int                         discoveryUrlIndex;
} ualds_registerContext;

static OpcUa_Timer          g_hRegistrationTimer = OpcUa_Null;
/* list of ualds_registerContext representing servers announced via zeroconf */
static OpcUa_List           g_lstServers;

void DNSSD_API ualds_DNSServiceRegisterReply(DNSServiceRef sdRef,
                                             DNSServiceFlags flags,
                                             DNSServiceErrorType errorCode,
                                             const char *name,
                                             const char *regtype,
                                             const char *domain,
                                             void *context)
{
    ualds_registerContext *pRegisterContext = (ualds_registerContext*)context;

    UALDS_UNUSED(sdRef);

    ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceRegisterReply: server registered:");
    ualds_log(UALDS_LOG_DEBUG, "                               flags %u", flags);
    ualds_log(UALDS_LOG_DEBUG, "                               error %i", errorCode);
    ualds_log(UALDS_LOG_DEBUG, "                               name %s", name);
    ualds_log(UALDS_LOG_DEBUG, "                               regtype %s", regtype);
    ualds_log(UALDS_LOG_DEBUG, "                               domain %s", domain);

    if (errorCode != kDNSServiceErr_NoError)
    {
        ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceRegisterReply returned error %i, retrying registration", errorCode);
        pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;
        OpcUa_Socket_Close(pRegisterContext->uaSocket);
        DNSServiceRefDeallocate(pRegisterContext->sdRef);
        pRegisterContext->uaSocket = OpcUa_Null;
        pRegisterContext->sdRef = OpcUa_Null;
    }
    else
    {
        ualds_log(UALDS_LOG_INFO, "Announcing server %s via Zeroconf succeeded", pRegisterContext->szServerUri);
        pRegisterContext->registrationStatus = RegistrationStatus_Registered;
    }
}

void ualds_zeroconf_socketEventCallback(int* shutdown)
{
    fd_set readFDs;
    struct timeval tv;
    int fd, status;
    ualds_registerContext* context;
    MulticastSocketCallbackStruct* socketCallbackStruct;

    if (*shutdown)
    {
        return;
    }

    /* Need to synchronize access to shared fd/sdref between this main thread
      and registration thread - the g_lstServers list is convinient for that. */
    OpcUa_List_Enter(&g_lstServers);
    OpcUa_List_Enter(&g_registerServersSocketList);
    OpcUa_List_ResetCurrent(&g_registerServersSocketList);

    socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_registerServersSocketList);
    while (socketCallbackStruct)
    {
        context = (ualds_registerContext*)socketCallbackStruct->context;
        if (*shutdown || context == OpcUa_Null || context->sdRef == OpcUa_Null)
        {
            /* previously a registration failed or was stopped - remove socket from poll list */
            OpcUa_Free(socketCallbackStruct);
            OpcUa_List_DeleteCurrentElement(&g_registerServersSocketList);
            socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_registerServersSocketList);
            continue;
        }

        fd = DNSServiceRefSockFD(socketCallbackStruct->sdRef);
        FD_ZERO(&readFDs);
        FD_SET(fd, &readFDs);

        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        status = select(fd + 1, &readFDs, NULL, NULL, &tv);
        if (status > 0)
        {
            if (FD_ISSET(fd, &readFDs))
            {
                int error = DNSServiceProcessResult(socketCallbackStruct->sdRef);
                if (error != kDNSServiceErr_NoError)
                {
                    ualds_log(UALDS_LOG_ERR, "DNSServiceProcessResult: error == %d\n", error);
                }
            }
        }
        socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetNextElement(&g_registerServersSocketList);
    }

    OpcUa_List_Leave(&g_registerServersSocketList);
    OpcUa_List_Leave(&g_lstServers);
}

OpcUa_StatusCode ualds_zeroconf_getServerInfo(const char *szServerUri,
                                              int discoveryUrlIndex,
                                              char *szMDNSServerName,
                                              unsigned int uMDNSServerNameLength,
                                              char *szServiceName,
                                              unsigned int uServiceNameLength,
                                              char *szHostName,
                                              unsigned int uHostNameLength,
                                              uint16_t *port,
                                              TXTRecordRef *txtRecord)
{
    int   numURLs = 0, numCapabilities = 0;
    char  szURL[UALDS_CONF_MAX_URI_LENGTH] = {0};
    char *szScheme = OpcUa_Null;
    char *tmpHostname = OpcUa_Null;
    char *szPath = OpcUa_Null;

    OpcUa_ReturnErrorIfArgumentNull(szServerUri);
    OpcUa_ReturnErrorIfArgumentNull(szMDNSServerName);
    OpcUa_ReturnErrorIfArgumentNull(szServiceName);
    OpcUa_ReturnErrorIfArgumentNull(szHostName);
    OpcUa_ReturnErrorIfTrue(uMDNSServerNameLength < UALDS_CONF_MAX_URI_LENGTH, OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfTrue(uServiceNameLength < UALDS_CONF_MAX_URI_LENGTH, OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfTrue(uHostNameLength < UALDS_CONF_MAX_URI_LENGTH, OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfArgumentNull(port);
    OpcUa_ReturnErrorIfArgumentNull(txtRecord);

    ualds_settings_begingroup(szServerUri);

    /* get server name */
    szMDNSServerName[0] = 0;
    ualds_settings_readstring("MdnsServerName", szMDNSServerName, uMDNSServerNameLength);
    if (szMDNSServerName[0] == 0)
    {
        int numNames = 0, i = 0;
        ualds_settings_beginreadarray("ServerNames", &numNames);
        for (i = 0; i < numNames; i++)
        {
            ualds_settings_setarrayindex(i);
            ualds_settings_readstring("Text", szMDNSServerName, uMDNSServerNameLength);
            if (szMDNSServerName[0] == 0)
            {
                break;
            }
        }
        ualds_settings_endarray();
    }
    if (szMDNSServerName[0] == 0)
    {
        return OpcUa_BadInternalError;
    }

    /* split discovery URL */
    ualds_settings_beginreadarray("DiscoveryUrls", &numURLs);
    if ((numURLs > 0) && (discoveryUrlIndex < numURLs))
    {
        ualds_settings_setarrayindex(discoveryUrlIndex);
        ualds_settings_readstring("Url", szURL, UALDS_CONF_MAX_URI_LENGTH);
    }
    ualds_settings_endarray();
    if (szURL == OpcUa_Null)
    {
        return OpcUa_BadInternalError;
    }

    if (OpcUa_IsGood(ualds_parse_url(szURL, &szScheme, &tmpHostname, port, &szPath)))
    {
        /* set host name */
        strlcpy(szHostName, tmpHostname, uHostNameLength);

        /* set scheme */
        if (OpcUa_StrCmpA(szScheme, "opc.tcp") == 0)
        {
            strlcpy(szServiceName, "_opcua-tcp._tcp", uServiceNameLength);
        }
        else if (OpcUa_StrCmpA(szScheme, "https") == 0)
        {
            strlcpy(szServiceName, "_opcua-tls._tcp", uServiceNameLength);
        }
        else if (OpcUa_StrCmpA(szScheme, "http") == 0)
        {
            strlcpy(szServiceName, "_opcua-http._tcp", uServiceNameLength);
        }

        /* create TXT record */
        TXTRecordCreate(txtRecord, 0, OpcUa_Null);
        if (szPath != OpcUa_Null)
        {
            TXTRecordSetValue(txtRecord, "path", (uint8_t)strlen(szPath), szPath);
        }
        else {
           TXTRecordSetValue(txtRecord, "path", 0, ""); 
        }

        ualds_settings_beginreadarray("ServerCapabilities", &numCapabilities);
        if (numCapabilities > 0)
        {
            char szCapabilities[UALDS_CONF_MAX_URI_LENGTH] = {0};
            int i = 0;
            for (i = 0; i < numCapabilities; i++)
            {
                char szCapability[UALDS_CONF_MAX_URI_LENGTH] = {0};
                ualds_settings_setarrayindex(i);
                ualds_settings_readstring("Capability", szCapability, UALDS_CONF_MAX_URI_LENGTH);
                strlcat(szCapabilities, szCapability, UALDS_CONF_MAX_URI_LENGTH);
                if (i < numCapabilities-1)
                {
                    strlcat(szCapabilities, ",", UALDS_CONF_MAX_URI_LENGTH);
                }
            }
            TXTRecordSetValue(txtRecord, "caps", (uint8_t)strlen(szCapabilities), szCapabilities);
        }
        else
        {
            if (*port == 4840)
            {
                TXTRecordSetValue(txtRecord, "caps", 3, "LDS");
            }
            else
            {
                TXTRecordSetValue(txtRecord, "caps", 2, "NA");
            }
        }
        ualds_settings_endarray();
    }
    else
    {
        return OpcUa_BadInternalError;
    }

    ualds_settings_endgroup();

    return OpcUa_Good;
}

OpcUa_StatusCode OPCUA_DLLCALL ualds_zeroconf_registerInternal(OpcUa_Void*  pvCallbackData,
                                                               OpcUa_Timer  hTimer,
                                                               OpcUa_UInt32 msecElapsed)
{
    OpcUa_StatusCode        uStatus = OpcUa_Good;
    DNSServiceErrorType     retDnssd;
    ualds_registerContext  *pRegisterContext = OpcUa_Null;
    OpcUa_Boolean           bOwnRegistration = OpcUa_True;

    UALDS_UNUSED(pvCallbackData);
    UALDS_UNUSED(hTimer);
    UALDS_UNUSED(msecElapsed);

    OpcUa_Mutex_Lock(g_mutex);
    ualds_expirationcheck();
    OpcUa_Mutex_Unlock(g_mutex);

    OpcUa_List_Enter(&g_lstServers);
    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        char szMDNSServerName[UALDS_CONF_MAX_URI_LENGTH];
        char szServiceName[UALDS_CONF_MAX_URI_LENGTH];
        char szHostName[UALDS_CONF_MAX_URI_LENGTH];
        char* domain, *registeredHostName;

        uint16_t port = 0;
        TXTRecordRef txtRecord;

        if (pRegisterContext->registrationStatus != RegistrationStatus_Unregistered)
        {
            bOwnRegistration = OpcUa_False;
            pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
            continue;
        }

        /* get registration information */
        //OpcUa_Mutex_Lock(g_mutex);
        uStatus = ualds_zeroconf_getServerInfo(pRegisterContext->szServerUri,
                                               pRegisterContext->discoveryUrlIndex,
                                               szMDNSServerName,
                                               UALDS_CONF_MAX_URI_LENGTH,
                                               szServiceName,
                                               UALDS_CONF_MAX_URI_LENGTH,
                                               szHostName,
                                               UALDS_CONF_MAX_URI_LENGTH,
                                               &port,
                                               &txtRecord);
        //OpcUa_Mutex_Unlock(g_mutex);

        if (OpcUa_IsBad(uStatus))
        {
            uStatus = OpcUa_Good;
            bOwnRegistration = OpcUa_False;
            pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
            continue;
        }

        if (bOwnRegistration != OpcUa_False)
        {
            /* replace [gethostname] in own server name */
            char hostname[UALDS_CONF_MAX_URI_LENGTH] = {0};
            ualds_platform_getfqhostname(hostname, sizeof(hostname));
            replace_string(szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", hostname);
            replace_string(szHostName, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", hostname);
            bOwnRegistration = OpcUa_False;
        }
        else
        {
            /* If IP4 => convert it to hostname */
            int isIP4 = is_Host_IP4Address(szHostName);
            if (isIP4 == 0)
            {
                ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_registerInternal: Found IP4: '%s'. Converting it to hostname.",
                    szHostName);
                int convertSuccess = ualds_platform_convertIP4ToHostname(szHostName, UALDS_CONF_MAX_URI_LENGTH);
                if (convertSuccess != 0)
                {
                    ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_registerInternal: Convert IP4 to hostname failed for '%s'",
                        szHostName);
                    return uStatus;
                }
            }
        }

        /* Make sure we pass hostname.local. or a FQDN as registered host or else register or resolve will fail */
        /* Check whether there is a domain label in the name */
        domain = strrchr(szHostName, '.');

        if (domain)
        {
            if (strcmp(domain, ".") == 0)
            {
                // hostname.subdomain.domain.
                *domain = '\0';
                domain = strrchr(szHostName, '.');
            }

            if (domain)
            {
                int tldCheck = isTLD(domain);
                if (tldCheck != 0)
                {
                    /* Not a Top Level Domain */

                    /*
                    Strip the name down to just the node name and make it link local.
                    */
                    domain = strchr(szHostName, '.');
                    *domain = '\0';
                    domain = NULL; /* This will make below add .local to the node name left in szHostName */
                }
            }
        }

        if (!domain)
        {
            /* If no domain labels in hostname, make link local by adding .local domain */
            strlcat(szHostName, ".local.", UALDS_CONF_MAX_URI_LENGTH);
        }

        registeredHostName = szHostName;

        /* register the server at the zeroconf service */
        retDnssd = DNSServiceRegister(&pRegisterContext->sdRef,
                                      0,
                                      0,
                                      szMDNSServerName,
                                      szServiceName,
                                      OpcUa_Null,
                                      registeredHostName,
                                      htons(port),
                                      TXTRecordGetLength(&txtRecord),
                                      TXTRecordGetBytesPtr(&txtRecord),
                                      ualds_DNSServiceRegisterReply,
                                      pRegisterContext);

        TXTRecordDeallocate(&txtRecord);

        if (retDnssd != kDNSServiceErr_NoError)
        {
            ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_registerInternal: DNSServiceRegister returned error %i", retDnssd);
            bOwnRegistration = OpcUa_False;
            pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
            continue;
        }
        else
        {
            pRegisterContext->registrationStatus = RegistrationStatus_Registering;
            if (pRegisterContext->sdRef)
            {
                MulticastSocketCallbackStruct* socketCallbackStruct = OpcUa_Alloc(sizeof(MulticastSocketCallbackStruct));
                socketCallbackStruct->sdRef = pRegisterContext->sdRef;
                socketCallbackStruct->context = pRegisterContext;
                OpcUa_List_Enter(&g_registerServersSocketList);
                uStatus = OpcUa_List_AddElementToEnd(&g_registerServersSocketList, socketCallbackStruct);
                OpcUa_List_Leave(&g_registerServersSocketList);
            }
        }

        pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }

    OpcUa_List_Leave(&g_lstServers);

    return uStatus;
}

OpcUa_StatusCode OPCUA_DLLCALL ualds_zeroconf_unregisterInternal(OpcUa_Void*  pvCallbackData,
                                                                 OpcUa_Timer  hTimer,
                                                                 OpcUa_UInt32 msecElapsed)
{
    ualds_registerContext  *pRegisterContext = OpcUa_Null;

    UALDS_UNUSED(pvCallbackData);
    UALDS_UNUSED(hTimer);
    UALDS_UNUSED(msecElapsed);

    OpcUa_List_Enter(&g_lstServers);
    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        if (pRegisterContext->registrationStatus == RegistrationStatus_Registered)
        {
            {
                // remove element from g_registerServersSocketList
                OpcUa_List_Enter(&g_registerServersSocketList);
                OpcUa_List_ResetCurrent(&g_registerServersSocketList);
                MulticastSocketCallbackStruct* socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_registerServersSocketList);
                while (socketCallbackStruct)
                {
                    DNSServiceRef* serviceRef = (DNSServiceRef*)socketCallbackStruct->sdRef;
                    if ((serviceRef == OpcUa_Null) || (socketCallbackStruct->context == pRegisterContext))
                    {
                        OpcUa_Free(socketCallbackStruct);
                        OpcUa_List_DeleteCurrentElement(&g_registerServersSocketList);
                        break;
                    }
                    socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetNextElement(&g_registerServersSocketList);
                }

                OpcUa_List_Leave(&g_registerServersSocketList);
            }

            /* release ref for unregistering */
            ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_unregisterInternal: Call DNSServiceRefDeallocate to unregister server");
            OpcUa_Socket_Close(pRegisterContext->uaSocket);
            DNSServiceRefDeallocate(pRegisterContext->sdRef);
            pRegisterContext->uaSocket = OpcUa_Null;
            pRegisterContext->sdRef = OpcUa_Null;

            pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;
        }
        OpcUa_Free(pRegisterContext);
        pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }
    OpcUa_List_Leave(&g_lstServers);
    OpcUa_List_Clear(&g_lstServers);

    OpcUa_List_Clear(&g_registerServersSocketList);

    return OpcUa_Good;
}

void ualds_zeroconf_init_servers()
{
    int i;
    int     numServers = 0;
    OpcUa_UInt32 index;

    OpcUa_List_Initialize(&g_lstServers);

    ualds_settings_begingroup("RegisteredServers");
    ualds_settings_beginreadarray("Servers", &numServers);

    OpcUa_String* serverUri = OpcUa_Alloc(sizeof(OpcUa_String)* numServers);
    for (i = 0; i < numServers; i++)
    {
        char szServerUriRaw[UALDS_CONF_MAX_URI_LENGTH];
        ualds_settings_setarrayindex(i);
        ualds_settings_readstring("ServerUri", szServerUriRaw, UALDS_CONF_MAX_URI_LENGTH);

        OpcUa_String_Initialize(&serverUri[i]);
        OpcUa_String_AttachCopy(&serverUri[i], szServerUriRaw);
    }
    ualds_settings_endarray();
    ualds_settings_endgroup();
    
    for (i = 0; i < numServers; i++)
    {
        OpcUa_StringA rawServerUri = OpcUa_String_GetRawString(&serverUri[i]);
        OpcUa_UInt32 numURLs = 0;

        ualds_settings_begingroup(rawServerUri);
        ualds_settings_beginreadarray("DiscoveryUrls", &numURLs);
        ualds_settings_endarray();
        ualds_settings_endgroup();

        for (index = 0; index < numURLs; ++index)
        {
            ualds_registerContext *pRegisterContext = OpcUa_Alloc(sizeof(ualds_registerContext));
            OpcUa_MemSet(pRegisterContext, 0, sizeof(ualds_registerContext));
            pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;

            strlcpy(pRegisterContext->szServerUri, rawServerUri, UALDS_CONF_MAX_URI_LENGTH);
            pRegisterContext->discoveryUrlIndex = index;
            OpcUa_List_AddElementToEnd(&g_lstServers, pRegisterContext);
        }
    }

    for (i = 0; i < numServers; i++)
    {
        OpcUa_String_Clear(&serverUri[i]);
    }
    OpcUa_Free(serverUri);
    
    OpcUa_List_Initialize(&g_registerServersSocketList);
}

OpcUa_StatusCode ualds_zeroconf_start_registration()
{
    OpcUa_StatusCode ret = OpcUa_BadNothingToDo;

    if (g_hRegistrationTimer == OpcUa_Null)
    {
        int registrationInterval = 10;

        OpcUa_Mutex_Lock(g_mutex);

        /* call ualds_zeroconf_registerInternal manually on startup */
        ualds_expirationcheck();
        ualds_zeroconf_init_servers();

        OpcUa_Mutex_Unlock(g_mutex);

        ualds_zeroconf_registerInternal(OpcUa_Null, OpcUa_Null, 0);

        OpcUa_Mutex_Lock(g_mutex);

        /* get RegistrationInterval setting */
        ualds_settings_begingroup("Zeroconf");
        ualds_settings_readint("RegistrationInterval", &registrationInterval);
        if (registrationInterval < 5)
        {
            registrationInterval = 5;
        }
        ualds_settings_endgroup();

        OpcUa_Mutex_Unlock(g_mutex);

        /* create timer for regular checking of registration */
        ualds_log(UALDS_LOG_INFO, "Create Zeroconf registration timer with interval %i", registrationInterval);
        ret = OpcUa_Timer_Create(&g_hRegistrationTimer,
                                 registrationInterval * 1000,
                                 ualds_zeroconf_registerInternal,
                                 ualds_zeroconf_unregisterInternal,
                                 OpcUa_Null);
    }

    return ret;
}

void ualds_zeroconf_stop_registration()
{
    if (g_hRegistrationTimer != OpcUa_Null)
    {
        /* delete registration timer, this automatically calls the unregister function */
        ualds_log(UALDS_LOG_INFO, "Delete Zeroconf registration timer");
        OpcUa_Timer_Delete(&g_hRegistrationTimer);
        g_hRegistrationTimer = OpcUa_Null;
    }
}

void ualds_zeroconf_addRegistration(const char *szServerUri)
{
    int numURLs = 0;
    int index;

    ualds_settings_begingroup(szServerUri);
    ualds_settings_beginreadarray("DiscoveryUrls", &numURLs);
    ualds_settings_endarray();
    ualds_settings_endgroup();

    for (index = 0; index < numURLs; ++index)
    {
        ualds_registerContext *pRegisterContext = OpcUa_Alloc(sizeof(ualds_registerContext));
        OpcUa_MemSet(pRegisterContext, 0, sizeof(ualds_registerContext));
        pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;

        strlcpy(pRegisterContext->szServerUri, szServerUri, UALDS_CONF_MAX_URI_LENGTH);
        pRegisterContext->discoveryUrlIndex = index;
        OpcUa_List_AddElementToEnd(&g_lstServers, pRegisterContext);
    }

    /* call ualds_zeroconf_registerInternal manually to force registration */
    ualds_zeroconf_registerInternal(OpcUa_Null, OpcUa_Null, 0);
}

void ualds_zeroconf_removeRegistration(const char *szServerUri)
{
    ualds_registerContext *pRegisterContext = OpcUa_Null;

    OpcUa_List_Enter(&g_lstServers);
    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        if (strcmp(pRegisterContext->szServerUri, szServerUri) == 0)
        {
            {
                // remove element from g_registerServersSocketList
                OpcUa_List_Enter(&g_registerServersSocketList);
                OpcUa_List_ResetCurrent(&g_registerServersSocketList);
                MulticastSocketCallbackStruct* socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_registerServersSocketList);
                while (socketCallbackStruct)
                {
                    DNSServiceRef* serviceRef = (DNSServiceRef*)socketCallbackStruct->sdRef;
                    if ((serviceRef == OpcUa_Null) || (socketCallbackStruct->context == pRegisterContext))
                    {
                        OpcUa_Free(socketCallbackStruct);
                        OpcUa_List_DeleteCurrentElement(&g_registerServersSocketList);
                        break;
                    }

                    socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetNextElement(&g_registerServersSocketList);
                }

                OpcUa_List_Leave(&g_registerServersSocketList);
            }

            if (pRegisterContext->registrationStatus != RegistrationStatus_Unregistered)
            {
                /* release ref for unregistering */
                ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_removeRegistration: Call DNSServiceRefDeallocate to unregister server");
                OpcUa_Socket_Close(pRegisterContext->uaSocket);
                DNSServiceRefDeallocate(pRegisterContext->sdRef);
                pRegisterContext->uaSocket = OpcUa_Null;
                pRegisterContext->sdRef = OpcUa_Null;
            }

            OpcUa_List_DeleteCurrentElement(&g_lstServers);
            OpcUa_Free(pRegisterContext);
        }
        pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }
    OpcUa_List_Leave(&g_lstServers);
}