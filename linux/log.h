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
void ualds_closelog(void);

#endif /* __LOG_H__ */
