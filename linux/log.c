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

/* system includes */
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#ifdef HAVE_OPCUA_STACK
/* uastack includes */
#include <opcua_platformdefs.h>
#include <opcua_string.h>
#endif /* HAVE_OPCUA_STACK */
/* local platform includes */
#include <platform.h>
#include <syslog.h>
/* local includes */
#include "../config.h"
#include "../settings.h"
#include "log.h"

/** internal logger state */
static int       g_max_size = 100*1024*1024; // 100 MB default value, equivalent in bytes
static int       g_logger_state = 0; /* 0=closed, 1=open */
static LogTarget g_target = UALDS_LOG_SYSLOG;
static LogLevel  g_level = UALDS_LOG_EMERG;
static FILE     *g_f = 0;
static char     szLogfile[PATH_MAX];

int ualds_openlog(LogTarget target, LogLevel level)
{
    int ret = 0;
    char szLogfileSize[10];
    int success;

    if (g_logger_state != 0) return 1;

    /* store configuration */
    g_target = target;
    g_level = level;

    switch (target)
    {
    case UALDS_LOG_SYSLOG:
        openlog("LDS", LOG_PERROR | LOG_NDELAY, LOG_USER);
        g_f = NULL;
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
            // convert int to string
            sprintf(szLogfileSize, "%d", g_max_size / 1024 / 1024);

            // add it to global settings, so that it can be flushed to file, at a later point.
            ualds_settings_addcomment("#Maximum logfile size in MB. This is only required for LogSystem=file.");
            ualds_settings_addemptyline();
            ualds_settings_writestring("LogFileSize", szLogfileSize);
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

void ualds_log(LogLevel level, const char *format, ...)
{
    char *szTimeStamp;
    time_t now;
    va_list ap;
    long currentPos;
    char szLogfile_backup[PATH_MAX];
    time_t rawtime;
    struct tm * timeinfo;
    char time_str[80];

    if (g_logger_state == 0 || level > g_level) return;

    va_start(ap, format);

    switch (g_target)
    {
    case UALDS_LOG_SYSLOG:
        switch(level)
        {
            case UALDS_LOG_EMERG: level = LOG_EMERG; break;
            case UALDS_LOG_ALERT: level = LOG_ALERT; break;
            case UALDS_LOG_CRIT: level = LOG_CRIT; break;
            case UALDS_LOG_ERR: level = LOG_ERR; break;
            case UALDS_LOG_WARNING: level = LOG_WARNING; break;
            case UALDS_LOG_NOTICE: level = LOG_NOTICE; break;
            case UALDS_LOG_INFO: level = LOG_INFO; break;
            default:
            case UALDS_LOG_DEBUG: level = LOG_DEBUG; break;
        }
        vsyslog(level, format, ap);
        break;
    case UALDS_LOG_STDERR:
    case UALDS_LOG_FILE:
        now = time(0);
        szTimeStamp = ctime(&now); /* standard time format: "Tue Nov 27 12:22:42" */
        szTimeStamp[19] = 0; /* remove linebreak from timestamp */
        fprintf(g_f, "%s [%i]: ", szTimeStamp, getpid());
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

                strlcpy(szLogfile_backup, szLogfile, PATH_MAX);

                // get current time in string format
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(time_str, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);

                strlcat(szLogfile_backup, "_", PATH_MAX);
                strlcat(szLogfile_backup, time_str, PATH_MAX);
                strlcat(szLogfile_backup, ".log", PATH_MAX);

                // rename file
                ualds_platform_rename(szLogfile, szLogfile_backup);

                // open log file
                ualds_openlog(tmp_target, tmp_level);
            }
        }
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
        closelog();
        break;
    case UALDS_LOG_STDERR:
        break;
    case UALDS_LOG_FILE:
        fclose(g_f);
        break;
    }

    g_logger_state = 0;
}
