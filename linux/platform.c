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
#include <stdlib.h>

/* local includes */
#include "../config.h"
#include "platform.h"
#include "log.h"


int ualds_platform_drop_privileges()
{
    if (getuid() == 0)
    {
        /* process is running as root, drop privileges */
        if (setgid(getgid()) != 0)
        {
            fprintf(stderr, "setgid: Unable to drop group privileges: %s", strerror(errno));
            return -1;
        }
        if (setuid(getuid()) != 0)
        {
            fprintf(stderr, "setuid: Unable to drop user privileges: %s", strerror(errno));
            return -1;
        }
    }
    return 0;
}

void ualds_platform_getconfigfile_path(char *szFilePath, size_t len)
{
    getcwd(szFilePath, len);
    strlcat(szFilePath, "/ualds.conf", len);

    if (!ualds_platform_fileexists(szFilePath))
    {
        szFilePath[0] = 0;
    }
}

int ualds_platform_getfqhostname(char *szHostname, int len)
{
    struct hostent *pEnt = 0;
    int ret = gethostname(szHostname, len);

    if (ret != 0) return ret;

    return ualds_platform_gethostbyname(szHostname, szHostname, len);
}

int ualds_platform_gethostbyname(const char* host, char* szHostname, int len)
{
    struct hostent *pEnt = 0;

    pEnt = gethostbyname(szHostname);
    if (pEnt == 0) return -1;

    strncpy(szHostname, pEnt->h_name, len);
    szHostname[len - 1] = 0;

    return 0;
}

/** Linux specific function for onvert IP4 to hostname .*/
int ualds_platform_convertIP4ToHostname(char* host, int len)
{
    struct hostent *hp;
    struct in_addr addr = { 0 };
    addr.s_addr = inet_addr(host);
    if (addr.s_addr == INADDR_NONE) {
        return -1;
    }
    else
    {
        hp = gethostbyaddr((char *)&addr, 4, AF_INET);
        if (hp == 0) return -1;

        strncpy(host, hp->h_name, len);
        host[len - 1] = 0;
    }
    
    return 0;
}

int ualds_platform_convertHostnameToIP4(const char* host, char* ip)
{
    struct addrinfo* result;
    struct addrinfo* res;
    int ret_error;

    /* resolve the domain name into a list of addresses */
    ret_error = getaddrinfo(host, NULL, NULL, &result);
    if (ret_error != 0) {
        return ret_error;
    }

    /* loop over all returned results and do inverse lookup */
    for (res = result; res != NULL; res = res->ai_next) {
        char hostname[1024];
        int error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, 1024, NULL, 0, 0);
        if (error != 0) {
            //fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
            ret_error = error;
            continue;
        }

        if (res->ai_family == AF_INET)
        {
            struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in *) res->ai_addr;
            char* IP4 = inet_ntoa(sockaddr_ipv4->sin_addr);
            strncpy(ip, IP4, strlen(IP4));
            ret_error = 0;
            break;
        }
    }

    freeaddrinfo(result);

    return ret_error;
}

int ualds_platform_fileexists(const char *szFile)
{
    return 0 == access(szFile, 0);
}

int ualds_platform_mkpath(char *szFilePath)
{
    char *path = strdup(szFilePath);
    int ret = 0;
    char *szFind = path;
    char *szFindNext = szFind;
    int mode = UALDS_S_IRWXU | UALDS_S_IRWXG | UALDS_S_IRWXO;

    /* create parent directories */
    while ((ret == 0 || errno == EEXIST) && (szFindNext = strchr(szFind, '/')) != 0)
    {
        if (szFindNext != szFind)
        {
            *szFindNext = 0;   /* replace '/' with \0 */
            ret = mkdir(path, mode);
            if (ret < 0 && errno != EEXIST)
            {
                ualds_log(UALDS_LOG_WARNING, "Failed making %s error %s\n", path, strerror(errno));
            }
            *szFindNext = '/'; /* restore '/' */
        }
        szFind = szFindNext + 1;
    }

    /* create last dir */
    if (ret == 0 || errno == EEXIST)
    {
        ret = mkdir(path, mode);
        if (ret < 0 && errno != EEXIST)
        {
            ualds_log(UALDS_LOG_WARNING, "Failed making %s error %s\n", path, strerror(errno));
        }
    }

    if (ret < 0 && errno == EEXIST)
    {
        ret = 0;
    }

    free(path);
    return ret;
}
