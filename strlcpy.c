/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: Portable BSD style ANSI C string functions.
**
** Author: Gerhard Gappmeier <gerhard.gappmeier@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
******************************************************************************/

#include <sys/types.h>
#include <string.h>

/** Size limited copy string function.
 * This function guaranties that \c dst will be zero terminated,
 * as long as \c dst is at least one byte long.
 * Note that strlcpy works on true 'C' strings, which means
 * both \c src and \c dst must be zero terminated.
 * @param dst destination buffer
 * @param src source buffer
 * @param len length of destination
 * @return total length of the created string in \c dst
 */
size_t strlcpy(char *dst, const char *src, size_t len)
{
    size_t pos = 0;
    if (len < 1) return 0; /* sanity check */
    len--; /* reserve space for null terminator */

    while (pos < len && *src)
    {
        dst[pos++] = *src;
        src++;
    }
    dst[pos] = 0;
    return pos;
}


