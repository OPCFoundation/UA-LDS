/* Copyright (c) 1996-2019, OPC Foundation. All rights reserved.

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

#ifndef __LOG_H__
#define __LOG_H__

enum _LogTarget
{
    UALDS_LOG_SYSLOG = 0, /**< uses system log for logging */
    UALDS_LOG_STDERR,     /**< prints log messages to stderr (Makes only sense with -d option) */
    UALDS_LOG_FILE        /**< logs into a custom file */
};
typedef enum _LogTarget LogTarget;

enum _LogLevel
{
    UALDS_LOG_EMERG = 0, /**< system is unusable */
    UALDS_LOG_ALERT,     /**< action must be taken immediately */
    UALDS_LOG_CRIT,      /**< critical conditions */
    UALDS_LOG_ERR,       /**< error conditions */
    UALDS_LOG_WARNING,   /**< warnings conditions */
    UALDS_LOG_NOTICE,    /**< normal, but significant, condition */
    UALDS_LOG_INFO,      /**< informational message */
    UALDS_LOG_DEBUG      /** debug-level message */
};
typedef enum _LogLevel LogLevel;

int ualds_openlog(LogTarget target, LogLevel level);
void ualds_log(LogLevel level, const char *format, ...);
void ualds_closelog();

#endif /* __LOG_H__ */
