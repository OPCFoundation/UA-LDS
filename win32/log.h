/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: OPC UA Local Discovery Server
**
** Author: Gerhard Gappmeier <gerhard.gappmeier@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
******************************************************************************/
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
