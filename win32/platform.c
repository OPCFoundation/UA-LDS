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
#include <direct.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

/* local includes */
#include "../config.h"
#include "service.h"
#include "platform.h"
#include "log.h"

#ifdef HAVE_VISUAL_LEAK_DETECTOR
# include <vld.h>
#endif

/** This initializes the platform.
 * On Windows we need to call WSAStartup to make the network code working.
 */
int ualds_platform_initialize()
{
#ifdef HAVE_OPCUA_STACK
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
    
    if (!pszAppData || !ualds_platform_fileexists(szFilePath))
    {
        /* fallback to working directory */
        strlcpy(szFilePath, "ualds.ini", len);
    }

    if (!ualds_platform_fileexists(szFilePath))
    {
        /* Results in lds exiting */
        szFilePath[0] = 0;
    }
}

/** Windows specific function for onvert IP4 to hostname .*/
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

/** Windows specific function for retrieving the path the current running executable file .*/
void ualds_platform_getapplicationpath(char *szFilePath, size_t len)
{
    GetModuleFileNameA(NULL, szFilePath, len);
}

/** Windows specific function for retrieving the path the current running executable file .*/
void ualds_platform_getcwd(char *szFilePath, size_t len)
{
    GetCurrentDirectoryA((DWORD)len, szFilePath);
}

/** Rename file by moving it */
int ualds_platform_rename(const char* from, const char* to)
{
    return MoveFileA(from, to) ? 0 : -1;
}

/** Sleep */
void ualds_platform_sleep(int seconds)
{
    Sleep(seconds * 1000);
}

/** Get fully qualified hostname. */
int ualds_platform_getfqhostname(char *szHostname, int len)
{
    struct hostent *pEnt = 0;
    int ret = gethostname(szHostname, len);

    if (ret != 0) return ret;

    return ualds_platform_gethostbyname(szHostname, szHostname, len);
}

/** Get fully qualified name by using gethostbyname. */
int ualds_platform_gethostbyname(const char* host, char* szHostname, int len)
{
    struct hostent *pEnt = 0;

    pEnt = gethostbyname(szHostname);
    if (pEnt == 0) return -1;

    strncpy(szHostname, pEnt->h_name, len);
    szHostname[len - 1] = 0;

    return 0;
}

/** Returns one if the given file exists, zero otherwise. */
int ualds_platform_fileexists(const char *szFile)
{
    return GetFileAttributesA(szFile) != INVALID_FILE_ATTRIBUTES;
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
    return DeleteFileA(szFilePath) ? 0 : -1;
}

int ualds_platform_mkdir(char *szFilePath, int mode)
{
	/* Not supported on windows - return failure to caller */
    return -1;
}

int ualds_platform_mkpath(char *szFilePath)
{
    char *path = strdup(szFilePath);
    int ret = 0;
    char *szFind = path;
    char *szFindNext = szFind;

    /* be sure to not have a normal backspace */
    char *szRepl;
    szRepl = path;
    while (*szRepl)
    {
        if (*szRepl == '/') *szRepl = '\\';
        szRepl++;
    }

    /* create parent directories */
    while ((ret == 0 || errno == EEXIST) && ((szFindNext = strchr(szFind, '\\')) != NULL))
    {
        if (szFindNext != szFind)
        {
            *szFindNext = 0;   /* replace '/' with \0 */

            {
                char *colon_char = strrchr(path, ':');
                if (strcmp(colon_char, ":") == 0) // skip driver names
                {
                    *szFindNext = '\\'; /* restore '\\' */
                    szFind = szFindNext + 1;
                    continue;
                }
            }
 
            ret = _mkdir(path);
            if (ret < 0 && errno != EEXIST)
            {
                ualds_log(UALDS_LOG_WARNING, "Failed making %s error %s\n", path, strerror(errno));
            }

            *szFindNext = '\\'; /* restore '\\' */
        }
        szFind = szFindNext + 1;
    }

    /* create last dir */
    if (ret == 0 || errno == EEXIST)
    {
        ret = _mkdir(path);
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

void ualds_getOldLogFilename(const char *szLogFileName, char *szOldFileName, size_t bufSize, int maxRotateCount)
{
    time_t rawtime;
    struct tm * timeinfo;
    char time_str[80];

    if (maxRotateCount <= 0)
    {
        // get current time in string format
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_str, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);

        strcpy(szOldFileName, szLogFileName);
        strcat(szOldFileName, "_");
        strcat(szOldFileName, time_str);
        strcat(szOldFileName, ".log");
    }
    else
    {
        // get oldest logfile name or new file (if not yet all old logfiles created)

        // Find the oldest file
        char szPath[PATH_MAX];
        FILETIME oldestFileTime;
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &oldestFileTime);
        int oldestFileNumber = 1;
        char szNumber[30];
        for (int i = 1; i <= maxRotateCount; i++)
        {
            strcpy(szPath, szLogFileName);
            strcat(szPath, ".");
            sprintf(szNumber, "%4.4d", i);
            strcat(szPath, szNumber);
            strcat(szPath, ".log");
            HANDLE hFind;
            WIN32_FIND_DATAA data;
            hFind = FindFirstFileA(szPath, &data);
            if (hFind != INVALID_HANDLE_VALUE)
            {
                if (CompareFileTime(&data.ftLastWriteTime, &oldestFileTime) < 0) {
                    oldestFileTime = data.ftLastWriteTime;
                    oldestFileNumber = i;
                }
                FindClose(hFind);
            }
            else
            {
                // We found a free file, so use it ;)
                oldestFileNumber = i;
                break;
            }
        }
        strcpy(szOldFileName, szLogFileName);
        strcat(szOldFileName, ".");
        sprintf(szNumber, "%4.4d", oldestFileNumber);
        strcat(szOldFileName, szNumber);
        strcat(szOldFileName, ".log");

        // Be sure the file does not exists
        SetFileAttributesA(szOldFileName, FILE_ATTRIBUTE_NORMAL);
        DeleteFileA(szOldFileName);
    }
}

int ualds_platform_strcpy_insensitive(const char *s1, const char *s2)
{
    return stricmp(s1, s2);
}
