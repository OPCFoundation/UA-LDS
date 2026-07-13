/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
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

#include "dnssd_ipc.h"

#if defined(_WIN32)

char *win32_strerror(int inErrorCode)
{
    static char buffer[1024];
    DWORD n;
    memset(buffer, 0, sizeof(buffer));
    n = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD) inErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buffer,
        sizeof(buffer),
        NULL);
    if (n > 0)
    {
        // Remove any trailing CR's or LF's since some messages have them.
        while ((n > 0) && isspace(((unsigned char *) buffer)[n - 1]))
            buffer[--n] = '\0';
    }
    return buffer;
}

#endif

void put_uint32(const uint32_t l, char **ptr)
{
    (*ptr)[0] = (char)((l >> 24) &  0xFF);
    (*ptr)[1] = (char)((l >> 16) &  0xFF);
    (*ptr)[2] = (char)((l >>  8) &  0xFF);
    (*ptr)[3] = (char)((l      ) &  0xFF);
    *ptr += sizeof(uint32_t);
}

uint32_t get_uint32(const char **ptr, const char *end)
{
    if (!*ptr || *ptr + sizeof(uint32_t) > end)
    {
        *ptr = NULL;
        return(0);
    }
    else
    {
        uint8_t *p = (uint8_t*) *ptr;
        *ptr += sizeof(uint32_t);
        return((uint32_t) ((uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3]));
    }
}

void put_uint16(uint16_t s, char **ptr)
{
    (*ptr)[0] = (char)((s >>  8) &  0xFF);
    (*ptr)[1] = (char)((s      ) &  0xFF);
    *ptr += sizeof(uint16_t);
}

uint16_t get_uint16(const char **ptr, const char *end)
{
    if (!*ptr || *ptr + sizeof(uint16_t) > end)
    {
        *ptr = NULL;
        return(0);
    }
    else
    {
        uint8_t *p = (uint8_t*) *ptr;
        *ptr += sizeof(uint16_t);
        return((uint16_t) ((uint16_t)p[0] << 8 | p[1]));
    }
}

int put_string(const char *str, char **ptr)
{
    if (!str) str = "";
    strcpy(*ptr, str);
    *ptr += strlen(str) + 1;
    return 0;
}

int get_string(const char **ptr, const char *const end, char *buffer, int buflen)
{
    if (!*ptr)
    {
        *buffer = 0;
        return(-1);
    }
    else
    {
        char *lim = buffer + buflen;    // Calculate limit
        while (*ptr < end && buffer < lim)
        {
            char c = *buffer++ = *(*ptr)++;
            if (c == 0) return(0);      // Success
        }
        if (buffer == lim) buffer--;
        *buffer = 0;                    // Failed, so terminate string,
        *ptr = NULL;                    // clear pointer,
        return(-1);                     // and return failure indication
    }
}

void put_rdata(const int rdlen, const unsigned char *rdata, char **ptr)
{
    memcpy(*ptr, rdata, rdlen);
    *ptr += rdlen;
}

const char *get_rdata(const char **ptr, const char *end, int rdlen)
{
    if (!*ptr || *ptr + rdlen > end)
    {
        *ptr = NULL;
        return(0);
    }
    else
    {
        const char *rd = *ptr;
        *ptr += rdlen;
        return rd;
    }
}

void ConvertHeaderBytes(ipc_msg_hdr *hdr)
{
    hdr->version   = htonl(hdr->version);
    hdr->datalen   = htonl(hdr->datalen);
    hdr->ipc_flags = htonl(hdr->ipc_flags);
    hdr->op        = htonl(hdr->op );
    hdr->reg_index = htonl(hdr->reg_index);
}
