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
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* default install prefix */
#ifndef PREFIX
# define PREFIX "/opt/opcfoundation"
#endif

/* platform checks */
/* #undef HAVE_STRLCAT */
/* #undef HAVE_STRLCPY */

/* stringify macros */
#define xstr(s) str(s)
#define str(s) #s

/* define version information */
#define UALDS_CONF_VERSION_MAJOR 1
#define UALDS_CONF_VERSION_MINOR 03
#define UALDS_CONF_VERSION_PATCH 340
#define UALDS_CONF_VERSION_BUILD 0
#define UALDS_VERSION_SUFFIX " Release"
#define UALDS_CONF_VERSION_STRING xstr(UALDS_CONF_VERSION_MAJOR) "." \
    xstr(UALDS_CONF_VERSION_MINOR) "." xstr(UALDS_CONF_VERSION_PATCH) "." \
    xstr(UALDS_CONF_VERSION_BUILD) UALDS_VERSION_SUFFIX

/* maximum length of URIs and URLs. */
#define UALDS_CONF_MAX_URI_LENGTH 256
/* maximum length of a settings key */
#define UALDS_CONF_MAX_KEY_LENGTH 50

/* Windows specific section */
#ifdef _WIN32
/* Service Configuration */
# define UALDS_CONF_SERVICE_NAME        TEXT("UALDS")
# define UALDS_CONF_SERVICE_DISPLAYNAME TEXT("OPC UA Local Discovery Server")
# define UALDS_CONF_SERVICE_DESCRIPTION TEXT("The Local Discovery Server allows UA clients to discover UA servers running on the local machine.");
/* Windows certificate store */
# if OPCUA_SUPPORT_PKI_WIN32
#  define UALDS_CONF_WIN32_STORE_LOCATION OPCUA_P_PKI_WIN32_STORE_MACHINE
#  define UALDS_CONF_WIN32_STORE_NAME "UA Applications"
# endif /* OPCUA_SUPPORT_PKI_WIN32 */
#endif

/* Linux specific section */
/* pid file: default=/var/run/ualds.pid */
#define UALDS_CONF_PID_FILE          "/var/run/ualds/ualds.pid"
#define UALDS_CONF_LOG_TARGET        UALDS_LOG_FILE
#define UALDS_CONF_LOG_LEVEL         UALDS_LOG_DEBUG
#define UALDS_CONF_SYSLOG_IDENT      "ualds"
#define UALDS_CONF_SYSLOG_FACILITY   LOG_LOCAL0

// Bonjour Service Names
# define APPLE_BONJOUR_SERVICE_NAME       TEXT("Bonjour Service")
# define OPCF_BONJOUR_SERVICE_NAME        TEXT("OPCF Bonjour Service")

#endif /* __CONFIG_H__ */
