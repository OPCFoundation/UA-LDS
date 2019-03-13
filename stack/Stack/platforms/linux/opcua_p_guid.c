/* ========================================================================
 * Copyright (c) 2005-2018 The OPC Foundation, Inc. All rights reserved.
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

#include <stdlib.h>
#include <time.h>

/* UA platform definitions */
#include <opcua_p_internal.h>

#if OPCUA_REQUIRE_OPENSSL
#include <openssl/rand.h>
#endif

/* own headers */
#include <opcua_guid.h>
#include <opcua_p_guid.h>
/*============================================================================
 * CreateGuid
 *===========================================================================*/
/**
* CreateGuid generates a global unique identifier.
*/
OpcUa_Guid* OpcUa_P_Guid_Create(OpcUa_Guid* guid)
{
#if OPCUA_REQUIRE_OPENSSL
    if(RAND_bytes((unsigned char*)guid, sizeof(OpcUa_Guid)) <= 0)
    {
        return OpcUa_Null;
    }
#else
    unsigned int *data = (unsigned int*)guid;
    int chunks = 16 / sizeof(unsigned int);
    static const int intbits = sizeof(int)*8;
    static int randbits = 0;
    if(!randbits)
    {
        int max = RAND_MAX;
        do { ++randbits; } while ((max=max>>1));
        srand(time(NULL));
        rand(); /* Skip first */
    }

    while(chunks--)
    {
        unsigned int randNumber = 0;
        int filled;
        for (filled = 0; filled < intbits; filled += randbits)
            randNumber |= (unsigned int)rand()<<filled;
        memcpy(data+chunks, &randNumber, sizeof(randNumber));
    }
#endif /* OPCUA_REQUIRE_OPENSSL */

    guid->Data4[0] = (guid->Data4[0] & 0x3F) | 0x80; /* UV_DCE */
    guid->Data3 = (guid->Data3 & 0x0FFF) | 0x4000;   /* UV_Random */

    return guid;
}
