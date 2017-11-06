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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <sys/types.h>
#include <windows.h>

/* add some missing C99 defines for MS compiler*/
#define INT8_C(x)  x##i8
#define UINT8_C(x) x##ui8
#define INT16_C(x)  x##i16
#define UINT16_C(x) x##ui16
#define INT32_C(x)  x##i32
#define UINT32_C(x) x##ui32
#define INT64_C(x)  x##i64
#define UINT64_C(x) x##ui64
#define uint8_t unsigned char
#define int8_t  char
#define uint16_t unsigned short
#define int16_t  short
#define uint32_t unsigned int
#define int32_t  int
#define uint64_t unsigned __int64
#define int64_t  __int64
#if defined(_WIN64)
# define ssize_t int64_t
#else
# define ssize_t int32_t
#endif

#define mode_t unsigned short
#define uid_t short
#define gid_t short
#define uid_t short
#define nlink_t short

#define PATH_MAX MAX_PATH

/** Encapsulation of POSIX struct stat to make it portable.
 * This struct is very different on some systems, or don't even exist.
 * @see ualds_platform_fstat
 * @see ualds_platform_stat
 */
struct ualds_stat
{
    _dev_t     st_dev;     /* ID of device containing file */
    _ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    _dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    //blksize_t st_blksize; /* blocksize for file system I/O */
    //blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};

#define UALDS_DT_DIR 4
#define UALDS_DT_REG 8
#define UALDS_DT_LINK 10

struct ualds_dirent
{
    char d_name[PATH_MAX];
    _ino_t d_fileno;
    unsigned char d_type;
};

#define UALDS_S_IRWXU 0x1
#define UALDS_S_IRWXG 0x2
#define UALDS_S_IRWXO 0x4

#define UALDS_UNUSED(xParameter) (void)(xParameter)

/* Detecting the size of time_t on Windows is really complicated.
 * With VS2005 MS introduced a breaking change by making time_t
 * 64bit long also on 32bit systems. By defining _USE_32BIT_TIME_T
 * this can be switched back to 32bit, also on 64bit systems.
 * Which means:
 * 1.) on 32bit: If _USE_32BIT_TIME_T is NOT set: time_t is 32bit for _MSC_VER<1400,
 *    64bit for new compilers.
 * 2.) on 64bit: If _USE_32BIT_TIME_T is NOT set: time_t is always 64bit
 * 3.) if _USE_32BIT_TIME_T is set, time_t is 32bit
 */

#if defined(_WIN64)
/* 64bit build for x86 */
# if defined(_USE_32BIT_TIME_T)
/* time_t is 32bit */
# define PRITT "u"
# define TTINTTYPE unsigned long
# else
/* time_t is 64bit */
# define PRITT "uI64"
# define TTINTTYPE unsigned __int64
# endif
#elif defined(_WIN32)
/* 32bit build for x86 */
# if defined(_USE_32BIT_TIME_T) || _MSC_VER<1400
/* time_t is 32bit */
# define PRITT "u"
# define TTINTTYPE unsigned int
# else
/* time_t is 64bit */
# define PRITT "llu"
# define TTINTTYPE unsigned __int64
# endif
#else
# error("This architecture is currently not supported")
#endif

#define snprintf _snprintf

int ualds_platform_initialize();
void ualds_platform_cleanup();
int ualds_platform_gethostbyname(const char* host, char* fqdn, int len);
int ualds_platform_getfqhostname(char *szHostname, int len);
int ualds_platform_fileexists(const char *szFile);
int ualds_platform_drop_privileges();
void ualds_platform_getconfigfile_path(char *szFilePath, size_t len);

void getDefaultCertificateFolder(char *szFolderPath, size_t len);
void getDefaultLogFilePath(char *szFilePath, size_t len);

int ualds_platform_convertIP4ToHostname(char* host, int len);
int ualds_platform_convertHostnameToIP4(const char* host, char* ip);

#define UALDS_FILE FILE
int ualds_platform_rename(const char* from, const char* to);
int ualds_platform_rm(char *szFilePath);
int ualds_platform_mkdir(char *szFilePath, int mode);
int ualds_platform_mkpath(char *szFilePath);
void ualds_getOldLogFilename(const char *szLogFileName, char *szOldFileName, size_t bufSize, int maxRotateCount);
UALDS_FILE* ualds_platform_fopen(const char *path, const char *mode);
int ualds_platform_fclose(UALDS_FILE *fp);
size_t ualds_platform_fread(void *ptr, size_t size, size_t nmemb, UALDS_FILE *fp);
size_t ualds_platform_fwrite(const void *ptr, size_t size, size_t nmemb, UALDS_FILE *fp);
// int ualds_platform_fprintf(UALDS_FILE *fp, const char* fmt, ...);
#define ualds_platform_fprintf fprintf
int ualds_platform_stat(const char *path, struct ualds_stat *buf);
int ualds_platform_fstat(UALDS_FILE *fp, struct ualds_stat *buf);
int ualds_platform_scandir(const char *dirp, struct ualds_dirent ***namelist,
    int (*filter)(const struct ualds_dirent *),
    int (*compar)(const struct ualds_dirent **, const struct ualds_dirent **));

void ualds_platform_errorstring(DWORD err, TCHAR *szMessage, size_t len);
void ualds_platform_getapplicationpath(char *szFilePath, size_t len);
void ualds_platform_getcwd(char *szFilePath, size_t len);
void ualds_platform_sleep(int seconds);

int ualds_platform_strcpy_insensitive(const char *s1, const char *s2);

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t len);
#endif
#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t len);
#endif


#endif /* __PLATFORM_H__ */
