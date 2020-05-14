/* Copyright (c) 1996-2020, OPC Foundation. All rights reserved.

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

#include <sys/types.h>
#include <string.h>

/** Size limited concatenate string function.
 * This function guaranties that \c dst will be zero terminated,
 * as long as \c dst is at least one byte long.
 * Note that strlcpy works on true 'C' strings, which means
 * both \c src and \c dst must be zero terminated.
 * If \c len <= strlen(dst) strlcat cannot write any characters,
 * strlen(dst) will be returned.
 * @param dst destination buffer
 * @param src source buffer
 * @param len length of destination
 * @return total length of the created string in \c dst
 */
size_t strlcat(char *dst, const char *src, size_t len)
{
    size_t pos = strlen(dst);
    if (len <= pos) return pos; /* sanity check */
    len--; /* reserve space for null terminator */

    while (pos < len && *src)
    {
        dst[pos++] = *src;
        src++;
    }
    dst[pos] = 0;
    return pos;
}



