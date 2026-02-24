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

#include "buildversion.h"

/* define version information */
#define UALDS_CONF_VERSION_MAJOR 1
#define UALDS_CONF_VERSION_MINOR 04
#define UALDS_CONF_VERSION_PATCH XXX
#define UALDS_CONF_VERSION_BUILD YYY
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
