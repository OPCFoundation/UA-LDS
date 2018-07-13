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

/* system includes */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
/* uastack includes */
#include <opcua_serverstub.h>
/* local includes */
#include "config.h"
#include "ualds.h"
#include "settings.h"
/* local platform includes */
#include <platform.h>
#include <daemon.h>
#include <log.h>
#ifdef _WIN32
#include <crtdbg.h>
#endif /* _WIN32 */


extern void ualds_settings_dump(char* pText);
extern void ualds_settings_clear();


static void createDefaultSettings_General()
{
    // General
    int retCode = ualds_settings_begingroup("General");

    // General/ServerUri
    retCode = ualds_settings_addcomment("# The OPCUA application uri used by the LDS.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("ServerUri", "urn:[gethostname]:UALocalDiscoveryServer");
    retCode = ualds_settings_addemptyline();

    // General/ExpirationMaxAge
    retCode = ualds_settings_addcomment("# maximum age of registered server entries before they expire in seconds. (Default=600)");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("ExpirationMaxAge", 600);
    retCode = ualds_settings_addemptyline();

    retCode = ualds_settings_addcomment("# AllowLocalRegistration: (default=no) this will disable certificate trust checks for connections comming from");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# localhost. Note that this is a security risk. Only enable this feature if you are aware of the full implications.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("#AllowLocalRegistration = yes");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addemptyline();

    retCode = ualds_settings_addcomment("# Endpoint configuration");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# Number of available endpoints");
    retCode = ualds_settings_addemptyline();

    // General/Endpoints
    retCode = ualds_settings_beginwritearray("Endpoints", 1);
    retCode = ualds_settings_setarrayindex(0);

    // General/Url
    retCode = ualds_settings_writestring("Url", "opc.tcp://[gethostname]:4840");

    // General/SecurityPolicies
    retCode = ualds_settings_writestring("SecurityPolicies", "SecurityPolicy_None, SecurityPolicy_Basic128Rsa15, SecurityPolicy_Basic256");
    retCode = ualds_settings_endarray();

    retCode = ualds_settings_addemptyline();

    // General/ReadOnlyCfg
    retCode = ualds_settings_addcomment("# Defines if this configuration file is never written by the LDS");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# ReadOnly (default) > 0; Write = 0.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("ReadOnlyCfg", 1);
    retCode = ualds_settings_addemptyline();

    // General
    retCode = ualds_settings_endgroup();
}


static void createDefaultSettings_CertificateInfo()
{
    // CertificateInfo
    int retCode = ualds_settings_begingroup("CertificateInfo");
    retCode = ualds_settings_addcomment("# Certificate information for creating self-signed certificates on startup");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# This is only used if no certificate exists in the configured path.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# On machine name change, the own certificate should be re-created by uninstalling/reinstalling LDS OR by deleting the actual certificate and restarting the service.");
    retCode = ualds_settings_addemptyline();

    // CertificateInfo/CommonName
    retCode = ualds_settings_writestring("CommonName", "UA Local Discovery Server");
    // CertificateInfo/Organization
    retCode = ualds_settings_writestring("Organization", "OPC Foundation");
    // CertificateInfo/OrganizationUnit
    retCode = ualds_settings_writestring("OrganizationUnit", "");
    // CertificateInfo/Locality
    retCode = ualds_settings_writestring("Locality", "16101 N. 82nd Street, Scottsdale");
    // CertificateInfo/State
    retCode = ualds_settings_writestring("State", "Arizona");
    // CertificateInfo/Country
    retCode = ualds_settings_writestring("Country", "US");

    retCode = ualds_settings_addemptyline();

    // CertificateInfo
    retCode = ualds_settings_endgroup();

}

static void createDefaultSettings_RegisteredServers()
{
    // RegisteredServers
    int retCode = ualds_settings_begingroup("RegisteredServers");

    retCode = ualds_settings_addcomment("# This section contains all registered server entries. The first entry is always the LDS itself.\n");
    retCode = ualds_settings_addcomment("# Servers/size is the number of entries.\n");
    retCode = ualds_settings_addcomment("# Servers/0/ServerUri is the first entry.\n");
    retCode = ualds_settings_addcomment("# Servers/1/ServerUri is the second entry and so on.\n");
    retCode = ualds_settings_addcomment("# For each ServerUri there must exist a section with the same name\n");
    retCode = ualds_settings_addcomment("# which contains the detailed information of the server entry.\n");

    //retCode = ualds_settings_beginwritearray("Servers", 1);
    //retCode = ualds_settings_endarray();

    // RegisteredServers/Servers
    retCode = ualds_settings_beginwritearray("Servers", 1);
    // RegisteredServers/[0]
    retCode = ualds_settings_setarrayindex(0);
    // RegisteredServers/[0]ServerUri
    retCode = ualds_settings_writestring("ServerUri", "urn:[gethostname]:UALocalDiscoveryServer");
    // RegisteredServers/[0]
    retCode = ualds_settings_endarray();

    retCode = ualds_settings_writestring("TEST", "1");

    retCode = ualds_settings_addemptyline();

    // RegisteredServers
    retCode = ualds_settings_endgroup();
}

// ##############################################


void settings_test_removegroup1()
{
    ualds_settings_clear();
    char *pszText = calloc(1 * 1024 * 1024, 1);

    createDefaultSettings_General();
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 828);

    ualds_settings_removegroup("General");
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 0);

    createDefaultSettings_General();
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 828);
}

void settings_test_removegroup2()
{
    ualds_settings_clear();
    char *pszText = calloc(1 * 1024 * 1024, 1);

    createDefaultSettings_General();  // 828
    createDefaultSettings_CertificateInfo();  // 491
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1319);

    ualds_settings_removegroup("General");
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 491);

    createDefaultSettings_General();
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1319);
}

void settings_test_removegroup3()
{
    ualds_settings_clear();
    char *pszText = calloc(1 * 1024 * 1024, 1);

    createDefaultSettings_General();  // 828
    createDefaultSettings_CertificateInfo();  // 491
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1319);

    ualds_settings_removegroup("CertificateInfo");
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 828);

    createDefaultSettings_CertificateInfo();
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1319);
}

void settings_test_removearray1()
{
    ualds_settings_clear();
    char *pszText = calloc(1 * 1024 * 1024, 1);

    createDefaultSettings_General();  // 828
    createDefaultSettings_RegisteredServers();  // 473
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1301);

    int retCode = ualds_settings_begingroup("RegisteredServers");
    ualds_settings_removearray("Servers");
    ualds_settings_endarray();

    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1220);
}

void settings_test_removearray2()
{
    ualds_settings_clear();
    char *pszText = calloc(1 * 1024 * 1024, 1);

    createDefaultSettings_General();  // 828
    createDefaultSettings_RegisteredServers();  // 473
    createDefaultSettings_CertificateInfo();  // 491
    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1792);

    int retCode = ualds_settings_begingroup("RegisteredServers");
    ualds_settings_removearray("Servers");
    ualds_settings_endarray();

    pszText[0] = 0;
    ualds_settings_dump(pszText);
    _ASSERTE(strlen(pszText) == 1711);
}

void settings_doTests()
{
    settings_test_removegroup1();
    settings_test_removegroup2();
    settings_test_removegroup3();

    settings_test_removearray1();
    settings_test_removearray2();
}

