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
#include <errno.h>
#include <sys/stat.h>
/* local includes */
#include "../config.h"
#include "service.h"
#include "platform.h"

#ifdef HAVE_VISUAL_LEAK_DETECTOR
# include <vld.h>
#endif

/** This initializes the platform.
 * On Windows we need to call WSAStartup to make the network code working.
 */
int ualds_platform_initialize()
{
#ifdef HAVE_OPCUA_STACK
    /* Oh, how do I love this Microsoft crap  ...
     * In a world without walls and fences, who needs windows and gates?
     */
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* this is an early error, no trace available yet,
         * so lets print to to stderr.
         */
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif /* HAVE_OPCUA_STACK */
    return 0;
}

/** Should be self-explanatory ...*/
void ualds_platform_cleanup()
{
#ifdef HAVE_OPCUA_STACK
    WSACleanup();
#endif /* HAVE_OPCUA_STACK */
}

/** Dropping Administrator privileges permanently.
 * This is called when starting as Administrator.
 */
int ualds_platform_drop_privileges()
{
    /* Is there a way to drop Administrator privileges on Windows?
     * When running as service we can configure a dedicated user,
     * if started as Admin we could terminate with EXIT_FAILURE.
     */
    return 0;
}

/** On Windows we don't have a defined config file location.
 * We find the config file relative to the executable file.
 */
void ualds_platform_getconfigfile_path(char *szFilePath, size_t len)
{
    char *pszAppData = getenv("ALLUSERSPROFILE");

    if (pszAppData)
    {
        strlcpy(szFilePath, pszAppData, len);
        strlcat(szFilePath, "\\OPC Foundation\\UA\\Discovery\\ualds.ini", len);
    }
    else
    {
        /* fallback to working directory */
        pszAppData = "ualds.ini";
    }
}

/** Windows specific function for retrieving the path the current running executable file .*/
void ualds_platform_getapplicationpath(char *szFilePath, size_t len)
{
    GetModuleFileNameA(NULL, szFilePath, len);
}

#ifdef HAVE_OPCUA_STACK
/** Get fully qualified hostname.
 * gethostname() can return the short hostname, so we use gethostbyname()
 * to get FQDN. This functions works exactly the same on Windows and Linux.
 * Only the system headers are different, that's why we are wrapping this
 * in UALDS platform function.
 */
int ualds_platform_getfqhostname(char *szHostname, int len)
{
    struct hostent *pEnt = 0;
    int ret = gethostname(szHostname, len);

    if (ret != 0) return ret;

    pEnt = gethostbyname(szHostname);
    if (pEnt == 0) return -1;

    strncpy(szHostname, pEnt->h_name, len);
    szHostname[len - 1] = 0;

    return 0;
}
#endif /* HAVE_OPCUA_STACK */

/** Returns one if the given file exists, zero otherwise. */
int ualds_platform_fileexists(const char *szFile)
{
    FILE *f = fopen(szFile, "r");
    if (f)
    {
        fclose(f);
    }
    else
    {
        if (errno == ENOENT) return 0;
    }

    return 1;
}

/** Windows specific function for converting an error code from GetLastError()
 * to a string.
 */
void ualds_platform_errorstring(DWORD err, TCHAR *szMessage, size_t len)
{
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        szMessage, len, NULL);
}

int ualds_platform_rm(char *szFilePath)
{
    return unlink(szFilePath);
}

int ualds_platform_mkdir(char *szFilePath, uint8_t mode)
{
    BOOL bCreated;

    UALDS_UNUSED(mode); /* not used on windows */

    /* TODO */
    bCreated = CreateDirectoryA(szFilePath, NULL);
    if (bCreated == FALSE) return -1;
    return 0;
}

int ualds_platform_mkpath(char *szFilePath, uint8_t mode)
{
    char *path = strdup(szFilePath);
    int ret = 0;
    char *szFind = path;
    char *szFindNext = szFind;

    UALDS_UNUSED(mode); /* not used on windows */

    /* create parent directories */
    while ((ret == 0 || errno == EEXIST) && (szFindNext = strchr(szFind, '/')) != 0)
    {
        if (szFindNext != szFind)
        {
            *szFindNext = 0;   /* replace '/' with \0 */
            /* TODO: ret = mkdir(path, mode); */
            *szFindNext = '/'; /* restore '/' */
        }
        szFind = szFindNext + 1;
    }
    /* create last dir */
    if (ret == 0 || errno == EEXIST)
    {
        /* TODO: */
        //ret = mkdir(path, mode);
    }

    free(path);

    return ret;
}

UALDS_FILE* ualds_platform_fopen(const char *path, const char *mode)
{
    return fopen(path, mode);
}

int ualds_platform_fclose(UALDS_FILE *fp)
{
    return fclose(fp);
}

size_t ualds_platform_fread(void *ptr, size_t size, size_t nmemb, UALDS_FILE *fp)
{
    return fread(ptr, size, nmemb, fp);
}

size_t ualds_platform_fwrite(const void *ptr, size_t size, size_t nmemb, UALDS_FILE *fp)
{
    return fwrite(ptr, size, nmemb, fp);
}

/** These function returns information about a file.
 * No permissions are required on the file itself, execute (search) permission
 * is required on all of the directories in path that lead to the file.
 * @see ualds_platform_fstat
 */
int ualds_platform_stat(const char *path, struct ualds_stat *buf)
{
    int ret;
    struct stat winbuf;

    memset(buf, 0, sizeof(struct ualds_stat));
    if ((ret = stat(path, &winbuf)) == 0)
    {
        buf->st_dev = winbuf.st_dev;
        buf->st_ino = winbuf.st_ino;
        buf->st_mode = winbuf.st_mode;
        buf->st_nlink = winbuf.st_nlink;
        buf->st_uid = winbuf.st_uid;
        buf->st_gid = winbuf.st_gid;
        buf->st_rdev = winbuf.st_rdev;
        buf->st_size = winbuf.st_size;
        buf->st_atime = winbuf.st_atime;
        buf->st_mtime = winbuf.st_mtime;
        buf->st_ctime = winbuf.st_ctime;
    }

    return ret;
}

/** These function returns information about a file.
 * This function use an open file handle.
 * @see ualds_platform_stat
 */
int ualds_platform_fstat(UALDS_FILE *fp, struct ualds_stat *buf)
{
    int ret;
    struct stat winbuf;

    memset(buf, 0, sizeof(struct ualds_stat));
    if ((ret = fstat(fileno(fp), &winbuf)) == 0)
    {
        buf->st_dev = winbuf.st_dev;
        buf->st_ino = winbuf.st_ino;
        buf->st_mode = winbuf.st_mode;
        buf->st_nlink = winbuf.st_nlink;
        buf->st_uid = winbuf.st_uid;
        buf->st_gid = winbuf.st_gid;
        buf->st_rdev = winbuf.st_rdev;
        buf->st_size = winbuf.st_size;
        buf->st_atime = winbuf.st_atime;
        buf->st_mtime = winbuf.st_mtime;
        buf->st_ctime = winbuf.st_ctime;
    }

    return ret;
}

int ualds_platform_scandir(const char *dirp, struct ualds_dirent ***namelist,
    int (*filter)(const struct ualds_dirent *),
    int (*compar)(const struct ualds_dirent **, const struct ualds_dirent **))
{
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    struct ualds_dirent entry;
    struct ualds_dirent **results = 0;
    void *tmp;
    int numResults = 0;
    char szPath[PATH_MAX];

    strlcpy(szPath, dirp, PATH_MAX);
    strlcat(szPath, "/*", PATH_MAX);

    hFind = FindFirstFileA(szPath, &data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            /* prepare entry */
            strlcpy(entry.d_name, data.cFileName, PATH_MAX);
            entry.d_type = 0;
            entry.d_fileno = 0;
            if (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) entry.d_type |= UALDS_DT_REG;
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) entry.d_type |= UALDS_DT_DIR;
            /* call filter function */
            if (filter(&entry) == 0) continue;
            /* copy result */
            /* Note: results is an array of pointers, where each element points to an
             * directory entry allocated using malloc. */
            tmp = realloc(results, sizeof(void*) * (numResults+1));
            if (tmp)
            {
                /* realloc succeeded */
                results = tmp;
                results[numResults] = malloc(sizeof(struct ualds_dirent));
                if (results[numResults])
                {
                    memcpy(results[numResults], &entry, sizeof(struct ualds_dirent));
                }
                numResults++;
            }
            else
            {
                /* realloc failed, return what we have so far */
                break;
            }
        }
        while (FindNextFileA(hFind, &data) != FALSE);

        FindClose(hFind);
    }

    if (compar)
    {
        /* sort entries if a compar function was specified. */
        qsort(results, numResults, sizeof(struct ualds_dirent*), compar);
    }

    *namelist = results;
    return numResults;
}
