/* Copyright (c) 1996-2018, OPC Foundation. All rights reserved.

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
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <tchar.h>
#ifdef HAVE_OPCUA_STACK
/* uastack includes */
#include <opcua_platformdefs.h>
#include <opcua_string.h>
#endif /* HAVE_OPCUA_STACK */
/* local platform includes */
#include <platform.h>
/* local includes */
#include "../config.h"
#include "../settings.h"
#include "log.h"

/** internal logger state */
static int       g_max_size = 100*1024*1024; // 100 MB default value, equivalent in bytes
static int       g_max_rotate_count = 0; // 0=Do not use log-rotate (use standard logfilenames with date/time); >0 Use the max. number of logfiles
static int       g_logger_state = 0; /* 0=closed, 1=open */
static LogTarget g_target = UALDS_LOG_STDERR;
static LogLevel  g_level = UALDS_LOG_EMERG;
static FILE     *g_f = 0;
static char     szLogfile[PATH_MAX];

int ualds_openlog(LogTarget target, LogLevel level)
{
    int ret = 0;
    char szLogfileSize[10];
    char szLogRotateCount[10];
    int success;

    if (g_logger_state != 0) return 1;

    /* store configuration */
    g_target = target;
    g_level = level;

    switch (target)
    {
    case UALDS_LOG_SYSLOG:
        ret = -1; /* this is not supported on windows */
        break;
    case UALDS_LOG_STDERR:
        g_f = stderr;
        break;
    case UALDS_LOG_FILE:
        ualds_settings_begingroup("Log");
        ualds_settings_readstring("LogFile", szLogfile, sizeof(szLogfile));
        success = ualds_settings_readstring("LogFileSize", szLogfileSize, sizeof(szLogfileSize));
        if (success == 0)
        {
           // convert string to int
            int tmpLogSize = atoi(szLogfileSize);
            if (tmpLogSize > 0)
            {
                g_max_size = tmpLogSize *1024 *1024;
            }
        }
        else
        {
            // convert int ot string
            _itoa(g_max_size / 1024 / 1024, szLogfileSize, 10);

            // add it to global settings, so that it can be flushed to file, at a later point.
            ualds_settings_addcomment("#Maximum logfile size in MB. This is only required for LogSystem=file.");
            ualds_settings_addemptyline();
            ualds_settings_writestring("LogFileSize", szLogfileSize);
            ualds_settings_addemptyline();
        }
        
        success = ualds_settings_readstring("LogRotateCount", szLogRotateCount, sizeof(szLogRotateCount));
        if (success == 0)
        {
            // convert string to int
            int tmpLogRotateCount = atoi(szLogRotateCount);
            if (tmpLogRotateCount > 0)
            {
                g_max_rotate_count = min(tmpLogRotateCount, 9999);
            }
        }
        else
        {
            // add it to global settings, so that it can be flushed to file, at a later point.
            ualds_settings_addcomment("#Maximum number of logfiles. This is optional for LogSystem=file. Default is '0' (no restriction in logfiles)");
            ualds_settings_addemptyline();
            // write 0 as default value
            ualds_settings_writestring("LogRotateCount", "0");
            ualds_settings_addemptyline();
        }

        ualds_settings_endgroup();

        g_f = fopen(szLogfile, "a");
        if (g_f == 0) ret = -1;
        break;
    }

    if (ret == 0)
    {
        g_logger_state = 1;
    }

    return ret;
}

#ifdef _DEBUG
void OutputDebugStringVarArgs(const char *format, va_list args)
{
    char szBuf[512];
    _vsnprintf(szBuf, 512, format, args);
    OutputDebugStringA(szBuf);
    OutputDebugStringA("\n");
}
#endif

void ualds_log(LogLevel level, const char *format, ...)
{
    char *szTimeStamp;
    time_t now;
    va_list ap;
    long currentPos;
    char szLogfile_backup[PATH_MAX];

    if (g_logger_state == 0 || level > g_level) return;

    va_start(ap, format);

    switch (g_target)
    {
    case UALDS_LOG_SYSLOG:
        /* this is not supported on windows */
        break;
    case UALDS_LOG_STDERR:
    case UALDS_LOG_FILE:
        now = time(0);
        szTimeStamp = ctime(&now); /* standard time format: "Tue Nov 27 12:22:42" */
        szTimeStamp[19] = 0; /* remove linebreak from timestamp */
        fprintf(g_f, "%s [%i]: ", szTimeStamp, _getpid());
        vfprintf(g_f, format, ap);
        fprintf(g_f, "\n");
        fflush(g_f);

        currentPos = ftell(g_f);
        if (currentPos != -1)
        {
            // Log file size is limited
            if (currentPos >= g_max_size)
            {
                // backup File parameters
                LogTarget tmp_target = g_target;
                LogLevel tmp_level = g_level;

                // close log file
                ualds_closelog();

                // get old logfile name
                ualds_getOldLogFilename(szLogfile, szLogfile_backup, sizeof(szLogfile_backup), g_max_rotate_count);

                // rename file
                ualds_platform_rename(szLogfile, szLogfile_backup);

                // open log file
                ualds_openlog(tmp_target, tmp_level);
            }
        }

#ifdef _DEBUG
        OutputDebugStringVarArgs(format, ap);
#endif
        break;
    }

    va_end(ap);
}

void ualds_closelog()
{
    if (g_logger_state == 0) return;

    switch (g_target)
    {
    case UALDS_LOG_SYSLOG:
        /* this is not supported on windows */
        break;
    case UALDS_LOG_FILE:
        fclose(g_f);
        break;
    }

    g_logger_state = 0;
}
