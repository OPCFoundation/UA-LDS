/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Project: OPC UA ANSI C SDK
**
** Description: Windows getopt() implementation
**
******************************************************************************/

/* Windows has not getopt() so we implement our own simple version.
 * Unlike real getopt() implementation this version can only parse short option names.
 */
#include <stdio.h>
#include <string.h>

char           *optarg  = NULL; /* Global argument pointer. */
int             optind  = 0;    /* Global argv index.       */
static char    *scan    = NULL; /* Private scan pointer.    */

int getopt(int argc, char *argv[], char *optstring)
{
    char  c       = '\0';
    char *place   = NULL;

    optarg = NULL;

    if (scan == NULL || *scan == '\0')
    {
        if (optind == 0)
        {
            optind++;
        }

        if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
        {
            return EOF;
        }

        if (strcmp(argv[optind], "--") == 0)
        {
            optind++;
            return EOF;
        }

        scan = argv[optind] + 1;
        optind++;
    }

    c = *scan++;
    place = strchr(optstring, c);

    if (place == NULL || c == ':')
    {
        fprintf(stderr, "%s: unknown option -%c\n", argv[0], c);
        return '?' ;
    }

    place++;

    if (*place == ':')
    {
        if (*scan != '\0')
        {
            optarg = scan;
            scan = NULL;
        }
        else if (optind < argc)
        {
            optarg = argv[optind];
            optind++;
        }
        else
        {
            fprintf(stderr, "%s: -%c argument missing\n", argv[0], c);
            return '?';
        }
    }

    return c;
}

