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

/**
 * \addtogroup zeroconf Zeroconf Module
 * @{
 *
 */

/**
 * \file
 * Zeroconf implementation file.
 * \author Hannes Mezger <hannes.mezger@ascolab.com>
 */

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

/* Globals for zeroconf registration */


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

/* callback function for socket events coming out of the uastack */
OpcUa_StatusCode ualds_zeroconf_socketEventCallback(OpcUa_Socket   hSocket,
                                                    OpcUa_UInt32   uintSocketEvent,
                                                    OpcUa_Void*    pUserData,
                                                    OpcUa_UInt16   usPortNumber,
                                                    OpcUa_Boolean  bIsSSL)
{
    DNSServiceErrorType     retDnssd;
    ualds_registerContext  *pRegisterContext = (ualds_registerContext*)pUserData;

    UALDS_UNUSED(hSocket);
    UALDS_UNUSED(usPortNumber);
    UALDS_UNUSED(bIsSSL);

    /* we only need to handle READ events */
    switch (uintSocketEvent)
    {
    case OPCUA_SOCKET_READ_EVENT:
        {
            retDnssd = DNSServiceProcessResult(pRegisterContext->sdRef);
            if (retDnssd != kDNSServiceErr_NoError)
            {
                ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_socketEventCallback: DNSServiceProcessResult returned error %i", retDnssd);
                pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;
                OpcUa_Socket_Close(pRegisterContext->uaSocket);
                DNSServiceRefDeallocate(pRegisterContext->sdRef);
                pRegisterContext->uaSocket = OpcUa_Null;
                pRegisterContext->sdRef = OpcUa_Null;
            }
            break;
        }
    case OPCUA_SOCKET_EXCEPT_EVENT:
        ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_socketEventCallback: received OPCUA_SOCKET_EXCEPT_EVENT");
        break;
    case OPCUA_SOCKET_TIMEOUT_EVENT:
        ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_socketEventCallback: received OPCUA_SOCKET_TIMEOUT_EVENT");
        break;
    default:
        ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_socketEventCallback: received event %u", uintSocketEvent);
        break;
    }

    return OpcUa_Good;
}

OpcUa_StatusCode ualds_parse_url(char *szUrl, char **szScheme, char **szHostname, uint16_t *port, char **szPath)
{
    char *szTmp = szUrl;

    *szScheme = OpcUa_Null;
    *szHostname = OpcUa_Null;
    *port = 4840;
    *szPath = OpcUa_Null;

    *szScheme = szTmp;
    while (*szTmp != 0 && *szTmp != ':') {szTmp++;}
    if (*szTmp == 0) {return OpcUa_BadInvalidArgument;}
    *szTmp = 0;

    szTmp++;
    if (*szTmp != '/') {return OpcUa_BadInvalidArgument;}
    szTmp++;
    if (*szTmp != '/') {return OpcUa_BadInvalidArgument;}
    szTmp++;

    *szHostname = szTmp;
    if (*szTmp == '[')
    {
        while (*szTmp != 0 && *szTmp != ']') {szTmp++;}
    }
    while (*szTmp != 0 && *szTmp != ':' && *szTmp != '/') {szTmp++;}
    if (*szTmp == 0) {return OpcUa_Good;}

    if (*szTmp == ':')
    {
        char *szPort = OpcUa_Null;
        *szTmp = 0;
        szPort = ++szTmp;
        *port = (uint16_t)strtol(szPort, OpcUa_Null, 10);
        while (*szTmp != 0 && *szTmp != '/') {szTmp++;}
    }

    if (*szTmp == '/')
    {
        *szPath = szTmp;
    }

    return OpcUa_Good;
}

OpcUa_StatusCode ualds_zeroconf_getServerInfo(const char *szServerUri,
                                              char *szMDNSServerName,
                                              unsigned int uMDNSServerNameLength,
                                              char *szServiceName,
                                              unsigned int uServiceNameLength,
                                              uint16_t *port,
                                              TXTRecordRef *txtRecord)
{
    int   numURLs = 0, numCapabilities = 0;
    char  szURL[UALDS_CONF_MAX_URI_LENGTH] = {0};
    char *szScheme = OpcUa_Null;
    char *szHostname = OpcUa_Null;
    char *szPath = OpcUa_Null;

    OpcUa_ReturnErrorIfArgumentNull(szServerUri);
    OpcUa_ReturnErrorIfArgumentNull(szMDNSServerName);
    OpcUa_ReturnErrorIfArgumentNull(szServiceName);
    OpcUa_ReturnErrorIfTrue(uMDNSServerNameLength < UALDS_CONF_MAX_URI_LENGTH, OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfTrue(uServiceNameLength < UALDS_CONF_MAX_URI_LENGTH, OpcUa_BadInvalidArgument);
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
    if (numURLs > 0)
    {
        ualds_settings_setarrayindex(0);
        ualds_settings_readstring("Url", szURL, UALDS_CONF_MAX_URI_LENGTH);
    }
    ualds_settings_endarray();
    if (szURL == OpcUa_Null)
    {
        return OpcUa_BadInternalError;
    }

    if (OpcUa_IsGood(ualds_parse_url(szURL, &szScheme, &szHostname, port, &szPath)))
    {
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
            TXTRecordSetValue(txtRecord, "caps", 2, "NO");
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

    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        char szMDNSServerName[UALDS_CONF_MAX_URI_LENGTH];
        char szServiceName[UALDS_CONF_MAX_URI_LENGTH];
        uint16_t port = 0;
        TXTRecordRef txtRecord;

        if (pRegisterContext->registrationStatus != RegistrationStatus_Unregistered)
        {
            bOwnRegistration = OpcUa_False;
            pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
            continue;
        }

        /* get registration information */
        uStatus = ualds_zeroconf_getServerInfo(pRegisterContext->szServerUri,
                                               szMDNSServerName,
                                               UALDS_CONF_MAX_URI_LENGTH,
                                               szServiceName,
                                               UALDS_CONF_MAX_URI_LENGTH,
                                               &port,
                                               &txtRecord);
        if (OpcUa_IsBad(uStatus))
        {
            uStatus = OpcUa_Good;
            bOwnRegistration = OpcUa_False;
            pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
            continue;
        }

        /* replace [gethostname] in own server name */
        if (bOwnRegistration != OpcUa_False)
        {
            char szHostname[UALDS_CONF_MAX_URI_LENGTH] = {0};
            ualds_platform_getfqhostname(szHostname, sizeof(szHostname));
            replace_string(szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);
            bOwnRegistration = OpcUa_False;
        }

        /* register the server at the zeroconf service */
        ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_registerInternal: Call DNSServiceRegister");
        retDnssd = DNSServiceRegister(&pRegisterContext->sdRef,
                                      0,
                                      0,
                                      szMDNSServerName,
                                      szServiceName,
                                      OpcUa_Null,
                                      OpcUa_Null,
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
            int fd = DNSServiceRefSockFD(pRegisterContext->sdRef);

            /*uStatus = OpcUa_SocketManager_AddSocket(OpcUa_Null,
                                                    (OpcUa_RawSocket)fd,
                                                    OpcUa_True,
                                                    OpcUa_Null,
                                                    OpcUa_Null,
                                                    OpcUa_Null,
                                                    ualds_zeroconf_socketEventCallback,
                                                    pRegisterContext,
                                                    &pRegisterContext->uaSocket);
													
			
            if (OpcUa_IsNotGood(uStatus))
            {
                ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_registerInternal: OpcUa_SocketManager_AddSocket returned error %08x", uStatus);
                DNSServiceRefDeallocate(pRegisterContext->sdRef);
                pRegisterContext->sdRef = 0;
            }
            else*/

			fd_set         readFDs;
			struct timeval tv;

			int tmp_flag = 1;
			//while (tmp_flag)
			{
				FD_ZERO(&readFDs);
				FD_SET(fd, &readFDs);

				//tv.tv_sec = 1000000;
				tv.tv_sec = 1;
				tv.tv_usec = 0;

				int status = select(fd + 1, &readFDs, NULL, NULL, &tv);

				if (status == -1)
				{
					fprintf(stderr, "status == -1\n");
					break;
				}
				else
				if (status == 0)
				{
					fprintf(stderr, "status == 0\n");
				}
				else
				if (FD_ISSET(fd, &readFDs))
				{
					int error = DNSServiceProcessResult(pRegisterContext->sdRef);

					if (error != kDNSServiceErr_NoError)
					{
						fprintf(stderr, "DNSServiceProcessResult: error == %d\n", error);
						break;
					}
				}
			}

            {
                pRegisterContext->registrationStatus = RegistrationStatus_Registering;
            }
        }

        pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }

	OpcUa_Mutex_Unlock(g_mutex);

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

	OpcUa_Mutex_Lock(g_mutex);

    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        if (pRegisterContext->registrationStatus == RegistrationStatus_Registered)
        {
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

    OpcUa_List_Clear(&g_lstServers);

	OpcUa_Mutex_Unlock(g_mutex);

    return OpcUa_Good;
}

void ualds_zeroconf_init_servers()
{
    int i;
    int     numServers = 0;

    OpcUa_List_Initialize(&g_lstServers);
    ualds_settings_begingroup("RegisteredServers");
    ualds_settings_beginreadarray("Servers", &numServers);
    for (i = 0; i < numServers; i++)
    {
        ualds_registerContext *pRegisterContext = OpcUa_Alloc(sizeof(ualds_registerContext));
        OpcUa_MemSet(pRegisterContext, 0, sizeof(ualds_registerContext));
        pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;

        ualds_settings_setarrayindex(i);
        ualds_settings_readstring("ServerUri", pRegisterContext->szServerUri, UALDS_CONF_MAX_URI_LENGTH);
        OpcUa_List_AddElementToEnd(&g_lstServers, pRegisterContext);
    }
    ualds_settings_endarray();
    ualds_settings_endgroup();
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
    ualds_registerContext *pRegisterContext = OpcUa_Alloc(sizeof(ualds_registerContext));
    OpcUa_MemSet(pRegisterContext, 0, sizeof(ualds_registerContext));
    pRegisterContext->registrationStatus = RegistrationStatus_Unregistered;

    strlcpy(pRegisterContext->szServerUri, szServerUri, UALDS_CONF_MAX_URI_LENGTH);
    OpcUa_List_AddElementToEnd(&g_lstServers, pRegisterContext);

    /* call ualds_zeroconf_registerInternal manually to force registration */
    ualds_zeroconf_registerInternal(OpcUa_Null, OpcUa_Null, 0);
}

void ualds_zeroconf_removeRegistration(const char *szServerUri)
{
    ualds_registerContext *pRegisterContext = OpcUa_Null;

    OpcUa_List_ResetCurrent(&g_lstServers);
    pRegisterContext = (ualds_registerContext*)OpcUa_List_GetCurrentElement(&g_lstServers);

    while (pRegisterContext)
    {
        if (strcmp(pRegisterContext->szServerUri, szServerUri) == 0)
        {
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
            break;
        }
        pRegisterContext = (ualds_registerContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }
}

/**
 * @}
 */

