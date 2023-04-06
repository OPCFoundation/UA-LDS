/* Copyright (c) 1996-2022, OPC Foundation. All rights reserved.

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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>          
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <inttypes.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>  
#include <netinet/tcp.h> 
#include <arpa/inet.h> 
#include <poll.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <err.h>

#define UALDS_DT_DIR 4
#define UALDS_DT_REG 8
#define UALDS_DT_LINK 10

#define UALDS_UNUSED(xParameter) (void)(xParameter)

#define UALDS_S_IRWXU S_IRWXU
#define UALDS_S_IRWXG S_IRWXG
#define UALDS_S_IRWXO S_IRWXO

# define PRITT "u"
# define TTINTTYPE unsigned int

#define ualds_platform_initialize() 1
#define ualds_platform_cleanup() (void)0
int ualds_platform_gethostbyname(const char* host, char* fqdn, int len);
int ualds_platform_getfqhostname(char *szHostname, int len);
int ualds_platform_fileexists(const char *szFile);
int ualds_platform_drop_privileges();
void ualds_platform_getconfigfile_path(char *szFilePath, size_t len);

void getDefaultCertificateFolder(char *szFolderPath, size_t len);
void getDefaultLogFilePath(char *szFilePath, size_t len);

enum ip_format
{
	ip_format_unknown 	= -1,
	ip_format_host 		= 0,
	ip_format_v4		= 1,
	ip_format_v6		= 2
};

enum ip_format ualds_platform_get_ip_format(const char* host);
int ualds_platform_convert_ipv4_to_hostname(char* host, int len);
int ualds_platform_convert_ipv6_to_hostname(char* host, int len);
int ualds_platform_convert_hostname_to_ipv4(const char* host, char* ip);

#define UALDS_FILE FILE
#define ualds_platform_rename rename
#define ualds_platform_rm rmdir
#define ualds_platform_mkdir mkdir
int ualds_platform_mkpath(char *szFilePath);
void ualds_getOldLogFilename(const char *szLogFileName, char *szOldFileName, size_t bufSize, int maxRotateCount);
#define ualds_platform_fopen fopen
#define ualds_platform_fclose fclose
#define ualds_platform_fread fread
#define ualds_platform_fwrite fwrite
#define ualds_platform_fprintf fprintf
#define ualds_stat stat
#define ualds_platform_stat stat
#define ualds_platform_fstat fstat
#define ualds_dirent dirent
#define ualds_platform_scandir scandir
#define ualds_platform_errorstring strerror_r
#define ualds_platform_getcwd getcwd
#define ualds_platform_getapplicationpath(p, l)
#define ualds_platform_sleep sleep

int ualds_platform_strcpy_insensitive(const char *s1, const char *s2);

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t len);
#endif
#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t len);
#endif

#endif /* __PLATFORM_H__ */
