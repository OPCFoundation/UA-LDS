/* Copyright (c) 1996-2016, OPC Foundation. All rights reserved.

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
#define UALDS_CONF_VERSION_PATCH 341
#define UALDS_CONF_VERSION_BUILD 1
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
