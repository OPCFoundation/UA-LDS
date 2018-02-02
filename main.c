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
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
/* uastack includes */
#include <opcua_serverstub.h>
/* local includes */
#include "config.h"
#include "ualds.h"
#include "settings.h"
/* local platform includes */
#include <platform.h>
#include <daemon.h>
#include <log.h>
#ifdef _WIN32
#include <crtdbg.h>
#endif /* _WIN32 */

void version()
{
    fprintf(stdout, "OPC UA Local Discovery Server %s\n", UALDS_CONF_VERSION_STRING);
    fprintf(stdout, "Copyright (c) 1996-2016, OPC Foundation. All rights reserved.n");
    fprintf(stdout, "Build date: %s\n", __DATE__);
}

void usage(const char *szAppName)
{
    fprintf(stderr, "Usage: %s [-h] [-c configfile] [-d] [-v] ", szAppName);
#ifdef HAVE_SERVICE_REGISTER
    fprintf(stderr, "[-i <account>] [-p <password>] ");
#endif
#ifdef HAVE_SERVICE_UNREGISTER
    fprintf(stderr, "[-u] ");
#endif
#if defined(HAVE_SERVICE_START) || defined(HAVE_SERVICE_STOP) || defined(HAVE_SERVICE_STATUS)
    fprintf(stderr,"<command>\n");
#endif
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -d: Debug mode. The server sends verbose output to standard error, "
            "and does not go into background.\n");
    fprintf(stderr, "  -D: No detach. When this options is specified, ualds will not detach "
            "and does not become a daemon (or service on Windows).\n");
    fprintf(stderr, "  -v: Prints version information.\n");
#ifdef HAVE_SERVICE_REGISTER
    fprintf(stderr, "  -i: Installs the UA LDS as service. As account you can either specify SYSTEM,\n"
            "      or any other user account. Use option -p to specify a password for the account.\n");
    fprintf(stderr, "  -p: Specifies password for service account.\n");
#endif
#ifdef HAVE_SERVICE_UNREGISTER
    fprintf(stderr, "  -u: Uninstalls the UA LDS service.\n");
#endif
    fprintf(stderr, "  -h: Prints the synopsis and a short description of the possible options.\n");
#if defined(HAVE_SERVICE_START) || defined(HAVE_SERVICE_STOP) || defined(HAVE_SERVICE_STATUS)
    fprintf(stderr, "Commands:\n");
#ifdef HAVE_SERVICE_START
    fprintf(stderr, "  start: Starts the service.\n");
#endif
#ifdef HAVE_SERVICE_STOP
    fprintf(stderr, "  stop: Stops the service.\n");
#endif
#ifdef HAVE_SERVICE_STATUS
    fprintf(stderr, "  status: Prints status information about service.\n");
#endif
#endif
}

int main(int argc, char* argv[])
{
    int ret = EXIT_SUCCESS;
    int opt;
    int debug = 0, nodetach = 0;
    int install = 0;
    int uninstall = 0;
    int start = 0;
    int stop = 0;
    int status = 0;
    char szConfigFile[PATH_MAX] = {0};
    char szValue[20];
    char szUser[50];
    char szPass[50];
    LogTarget logtarget = UALDS_CONF_LOG_TARGET;
    LogLevel loglevel = UALDS_CONF_LOG_LEVEL;

#if defined(_WIN32) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11656); // Comment or un-comment on need basis
#endif
    
    /* parse commandline arguments */
    while ((opt = getopt(argc, argv, "dDvhc:i:p:u")) != -1) {
        switch (opt) {
        case 'd':
            debug = 1;
            nodetach = 1;
            break;
        case 'D':
            nodetach = 1;
            break;
        case 'v':
            version();
            exit(EXIT_SUCCESS);
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case 'c':
            strlcpy(szConfigFile, optarg, PATH_MAX);
            break;
        case 'i':
            install = 1;
            strlcpy(szUser, optarg, sizeof(szUser));
            break;
        case 'p':
            strlcpy(szPass, optarg, sizeof(szPass));
            break;
        case 'u':
            uninstall = 1;
            break;
        default: /* '?' */
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    /* parse commands */
    if (optind < argc) {
        if (strcmp(argv[optind], "start") == 0) {
            start = 1;
        } else if (strcmp(argv[optind], "stop") == 0) {
            stop = 1;
        } else if (strcmp(argv[optind], "status") == 0) {
            status = 1;
        }
    }

#if _DEBUG
    debug = 1;
    nodetach = 1;
    /*ualds_platform_sleep(60);*/ /* give some time for attaching with debugger */
#endif

    ualds_platform_initialize();
    if (szConfigFile[0] == 0) {
        /* if not config file was given as commandline argument use the default path */
        ualds_platform_getconfigfile_path(szConfigFile, sizeof(szConfigFile));
    }

    if (debug == 1) {
        /* in debug mode we trace to stderr */
        logtarget = UALDS_LOG_STDERR;
        /* with debug log level */
        loglevel = UALDS_LOG_DEBUG;
        /* for configuration errors we log to stderr, because we don't have
         * the log settings until ualds_settings_open has been called.
         */
        ualds_openlog(logtarget, loglevel);
        ret = ualds_settings_open(szConfigFile);
        if (ret != 0) {
            fprintf(stderr, "Failed to open config file '%s'.\n", szConfigFile);
            ualds_platform_cleanup();
            exit(EXIT_FAILURE);
        }
        ualds_closelog();
    } else {
        ret = ualds_settings_open(szConfigFile);
        if (ret != 0) {
            fprintf(stderr, "Failed to open config file '%s'.\n", szConfigFile);
            ualds_platform_cleanup();
            exit(EXIT_FAILURE);
        }
        /* read log settings */
        ualds_settings_begingroup("Log");
        if (ualds_settings_readstring("LogSystem", szValue, sizeof(szValue)) == 0) {
            if (strcmp(szValue, "syslog") == 0) {
                logtarget = UALDS_LOG_SYSLOG;
            } else if (strcmp(szValue, "file") == 0) {
                logtarget = UALDS_LOG_FILE;
            } else if (strcmp(szValue, "stderr") == 0) {
                logtarget = UALDS_LOG_STDERR;
            }
        }
        if (ualds_settings_readstring("LogLevel", szValue, sizeof(szValue)) == 0) {
            if (strcmp(szValue, "emerg") == 0) {
                loglevel = UALDS_LOG_EMERG;
            } else if (strcmp(szValue, "alert") == 0) {
                loglevel = UALDS_LOG_ALERT;
            } else if (strcmp(szValue, "crit") == 0) {
                loglevel = UALDS_LOG_CRIT;
            } else if (strcmp(szValue, "error") == 0) {
                loglevel = UALDS_LOG_ERR;
            } else if (strcmp(szValue, "warn") == 0) {
                loglevel = UALDS_LOG_WARNING;
            } else if (strcmp(szValue, "info") == 0) {
                loglevel = UALDS_LOG_INFO;
            } else if (strcmp(szValue, "debug") == 0) {
                loglevel = UALDS_LOG_DEBUG;
            }
        }
        ualds_settings_endgroup();
    }

    /* initialize logger with correct settings */
    ualds_openlog(logtarget, loglevel);

    /* check if we should never write the configuration file */
    ualds_settings_begingroup("General");
    int icfgReadOnly;
    if (ualds_settings_readint("ReadOnlyCfg", &icfgReadOnly) == 0) {
        ualds_settings_setReadOnly(icfgReadOnly);
    }
    ualds_settings_endgroup();

    if (install == 1) {
#ifdef HAVE_SERVICE_REGISTER
		/* force an unregister first	*/
		ret = ServiceUnregister();
        ualds_platform_sleep(1);
		/* no matter if the server was unregistered, go ahead and attempt a forced register */
        ret = ServiceRegister(szUser, szPass);
#else
        fprintf(stderr, "Service install option is not supported on this platform.\n");
        ret = EXIT_FAILURE;
#endif
    } else if (uninstall == 1) {
#ifdef HAVE_SERVICE_UNREGISTER
        ret = ServiceUnregister();
#else
        fprintf(stderr, "Service uninstall option is not supported on this platform.\n");
        ret = EXIT_FAILURE;
#endif
    } else if (start == 1) {
#ifdef HAVE_SERVICE_START
        ret = ServiceStart();
#else
        fprintf(stderr, "Service start command is not supported on this platform.\n");
        ret = EXIT_FAILURE;
#endif
    } else if (stop == 1) {
#ifdef HAVE_SERVICE_STOP
        ret = ServiceStop();
#else
        fprintf(stderr, "Service stop command is not supported on this platform.\n");
        ret = EXIT_FAILURE;
#endif
    } else if (status == 1) {
#ifdef HAVE_SERVICE_STATUS
        ret = ServiceStatus();
#else
        fprintf(stderr, "Service status command is not supported on this platform.\n");
        ret = EXIT_FAILURE;
#endif
    } 
    
    if (!install && !uninstall && !start && !stop && !status) {
        /* drop privileges when started as root or with SETUID */
        ret = ualds_platform_drop_privileges();
        if (ret != 0) {
            ualds_log(UALDS_LOG_ERR, "Dropping privileges failed.");
            ret = EXIT_FAILURE;
        }
        else if (nodetach == 0) {
            /* daemonize / start service */
            ret = daemonize();
        } else {
            /* no detach, run as console application */
            ret = run();
        }
        ualds_log(UALDS_LOG_NOTICE, "Terminating with exitcode=%i", ret);
    }

    ualds_closelog();
    ualds_platform_cleanup();

    return ret;
}


