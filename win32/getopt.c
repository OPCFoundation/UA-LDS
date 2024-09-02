/* Copyright (c) 1996-2024, OPC Foundation. All rights reserved.

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

