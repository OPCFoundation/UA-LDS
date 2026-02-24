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
#include <opcua_stream.h>


/*============================================================================
 * OpcUa_Stream_Read
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_Read(
    OpcUa_InputStream*             istrm,
    OpcUa_Byte*                    buffer,
    OpcUa_UInt32*                  count)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);
    OpcUa_ReturnErrorIfArgumentNull(istrm);
    OpcUa_ReturnErrorIfArgumentNull(istrm->Read);

    return istrm->Read(istrm, buffer, count);
}

/*============================================================================
 * OpcUa_Stream_Write
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_Write(
    OpcUa_OutputStream* ostrm,
    OpcUa_Byte*         buffer,
    OpcUa_UInt32        count)
{
    /*OpcUa_StatusCode uStatus = OpcUa_Good;*/
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);

    OpcUa_ReturnErrorIfArgumentNull(ostrm);
    OpcUa_ReturnErrorIfArgumentNull(ostrm->Write);

    return ostrm->Write(ostrm, buffer, count);
}

/*============================================================================
 * OpcUa_Stream_Flush
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_Flush(
    OpcUa_OutputStream* ostrm,
    OpcUa_Boolean       lastCall)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);
    OpcUa_ReturnErrorIfArgumentNull(ostrm);
    OpcUa_ReturnErrorIfArgumentNull(ostrm->Flush);

    return ostrm->Flush(ostrm, lastCall);
}

/*============================================================================
 * OpcUa_Stream_Close
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_Close(
    OpcUa_Stream* strm)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(strm->Close);

    return strm->Close(strm);
}

/*============================================================================
 * OpcUa_Stream_GetChunkLength
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_GetChunkLength(
    struct _OpcUa_Stream* strm,
    OpcUa_UInt32*         length)
{
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(length);
    OpcUa_ReturnErrorIfArgumentNull(strm->GetChunkLength);

    return strm->GetChunkLength(strm, length);
}

/*============================================================================
 * OpcUa_Stream_AttachBuffer
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_AttachBuffer(
    struct _OpcUa_Stream*   strm,
    OpcUa_Buffer*           buffer)
{
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(buffer);
    OpcUa_ReturnErrorIfArgumentNull(strm->AttachBuffer);

    return strm->AttachBuffer(strm, buffer);
}


/*============================================================================
 * OpcUa_Stream_DetachBuffer
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_Stream_DetachBuffer(
    struct _OpcUa_Stream*   strm,
    OpcUa_Buffer*           buffer)
{
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(buffer);
    OpcUa_ReturnErrorIfArgumentNull(strm->DetachBuffer);

    return strm->DetachBuffer(strm, buffer);
}

/*============================================================================
 * OpcUa_Stream_Delete
 *===========================================================================*/
OpcUa_Void OpcUa_Stream_Delete(
    struct _OpcUa_Stream** strm)
{
    if (strm != OpcUa_Null && *strm != OpcUa_Null)
    {
        (*strm)->Delete(strm);
    }
}

/*============================================================================
 * OpcUa_Stream_GetPosition
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_GetPosition(
    struct _OpcUa_Stream* strm,
    OpcUa_UInt32*         position)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(strm->GetPosition);

    return strm->GetPosition(strm, position);
}

/*============================================================================
 * OpcUa_Stream_SetPosition
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Stream_SetPosition(
    struct _OpcUa_Stream* strm,
    OpcUa_UInt32          position)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Stream);
    OpcUa_ReturnErrorIfArgumentNull(strm);
    OpcUa_ReturnErrorIfArgumentNull(strm->SetPosition);

    return strm->SetPosition(strm, position);
}
