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

#include <opcua.h>
#include <opcua_encoder.h>
#include <opcua_decoder.h>

/*============================================================================
 * OpcUa_Decoder_Close
 *===========================================================================*/
OpcUa_Void OpcUa_Decoder_Close(
    struct _OpcUa_Decoder* a_pDecoder,
    OpcUa_Handle* a_phDecodeContext)
{
    if (a_pDecoder != OpcUa_Null && a_pDecoder->Delete != OpcUa_Null)
    {
        a_pDecoder->Close(a_pDecoder, a_phDecodeContext);
    }
}

/*============================================================================
 * OpcUa_Decoder_Delete
 *===========================================================================*/
OpcUa_Void OpcUa_Decoder_Delete(
    struct _OpcUa_Decoder** a_ppDecoder)
{
    if (a_ppDecoder != OpcUa_Null && *a_ppDecoder != OpcUa_Null && (*a_ppDecoder)->Delete != OpcUa_Null)
    {
        (*a_ppDecoder)->Delete(a_ppDecoder);
    }
}
