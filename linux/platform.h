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
int ualds_platform_drop_privileges(void);
void ualds_platform_getconfigfile_path(char *szFilePath, size_t len);

void getDefaultCertificateFolder(char *szFolderPath, size_t len);
void getDefaultLogFilePath(char *szFilePath, size_t len);

int ualds_platform_is_ip_format(const char* host);
int ualds_platform_convert_ip_to_hostname(char *host, int len);
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
