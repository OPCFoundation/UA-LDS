/* Copyright (c) 1996-2018, OPC Foundation. All rights reserved.

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

/* core */
#include <opcua.h>

#if OPCUA_HAVE_PUBSUB

#include <opcua_memorystream.h>
#include <opcua_binaryencoder.h>
#include <opcua_p_binary.h>
#include <opcua_uadp_networkmessage.h>

/*============================================================================
 ** @brief Global table of known types.
 *===========================================================================*/
extern OpcUa_EncodeableTypeTable OpcUa_ProxyStub_g_EncodeableTypes;

/*============================================================================
 ** @brief Global table of supported namespaces.
 *===========================================================================*/
extern OpcUa_StringTable OpcUa_ProxyStub_g_NamespaceUris;

static OpcUa_StatusCode OpcUa_UadpNetworkMessageHeader_WriteBinary(OpcUa_UadpNetworkMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpGroupHeader_WriteBinary(OpcUa_UadpGroupHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpPayloadHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpExtendedNetworkMessageHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpSecurityHeader_WriteBinary(OpcUa_UadpSecurityHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetMessageHeader_WriteBinary(OpcUa_UadpDataSetMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetField_WriteBinary(OpcUa_DataValue* a_pValue, OpcUa_UadpDataMessage_FieldEncoding a_fieldEncoding, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetMessage_WriteBinary(OpcUa_UadpDataSetMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpChunkData_WriteBinary(OpcUa_UadpChunkData* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

#define OpcUa_UadpEncode_Direct(xType, pValue) \
{ \
    OpcUa_##xType##_Wire oValue; \
    if(*a_pSize < sizeof(oValue)) \
    { \
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError); \
    } \
    uStatus = OpcUa_##xType##_P_NativeToWire(&oValue, pValue); \
    OpcUa_GotoErrorIfBad(uStatus); \
    memcpy(*a_pBuffer, &oValue, sizeof(oValue)); \
    *a_pBuffer += sizeof(oValue); \
    *a_pSize -= sizeof(oValue); \
}

#define OpcUa_UadpDeclare_Context \
    OpcUa_MessageContext cContext; \
    OpcUa_Encoder* pEncoder = OpcUa_Null; \
    OpcUa_Handle hEncoderContext = OpcUa_Null; \
    OpcUa_OutputStream* pOutputStream = OpcUa_Null

#define OpcUa_UadpInit_Context \
    OpcUa_MessageContext_Initialize(&cContext); \
    cContext.KnownTypes = &OpcUa_ProxyStub_g_EncodeableTypes; \
    cContext.NamespaceUris = &OpcUa_ProxyStub_g_NamespaceUris; \
    cContext.AlwaysCheckLengths = OpcUa_False

#define OpcUa_UadpClear_Context \
    OpcUa_Encoder_Close(pEncoder, &hEncoderContext); \
    OpcUa_Encoder_Delete(&pEncoder); \
    OpcUa_Stream_Delete(&pOutputStream); \
    OpcUa_MessageContext_Clear(&cContext)

#define OpcUa_UadpEncode_Simple(xType, pValue) \
{ \
    OpcUa_Byte* pBuffer; \
    OpcUa_UInt32 uBufferSize; \
    uStatus = OpcUa_MemoryStream_CreateWriteable(256, *a_pSize + 256, &pOutputStream); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_BinaryEncoder_Create(&pEncoder); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pEncoder->Open(pEncoder, pOutputStream, &cContext, &hEncoderContext); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pEncoder->Write##xType((OpcUa_Encoder*)hEncoderContext, OpcUa_Null, pValue, OpcUa_Null); \
    OpcUa_GotoErrorIfBad(uStatus); \
    OpcUa_Stream_Close(pOutputStream); \
    uStatus = OpcUa_MemoryStream_GetBuffer(pOutputStream, &pBuffer, &uBufferSize); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_Stream_GetPosition(pOutputStream, &uBufferSize); \
    OpcUa_GotoErrorIfBad(uStatus); \
    if(*a_pSize < uBufferSize) \
    { \
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError); \
    } \
    memcpy(*a_pBuffer, pBuffer, uBufferSize); \
    *a_pBuffer += uBufferSize; \
    *a_pSize -= uBufferSize; \
    OpcUa_Encoder_Close(pEncoder, &hEncoderContext); \
    OpcUa_Encoder_Delete(&pEncoder); \
    OpcUa_Stream_Delete(&pOutputStream); \
}

#define OpcUa_UadpEncode_VariantUnion(pValue) \
{ \
    OpcUa_Byte* pBuffer; \
    OpcUa_UInt32 uBufferSize; \
    uStatus = OpcUa_MemoryStream_CreateWriteable(256, *a_pSize + 256, &pOutputStream); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_BinaryEncoder_Create(&pEncoder); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pEncoder->Open(pEncoder, pOutputStream, &cContext, &hEncoderContext); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pEncoder->WriteVariant((OpcUa_Encoder*)hEncoderContext, OpcUa_Null, pValue, OpcUa_Null); \
    OpcUa_GotoErrorIfBad(uStatus); \
    OpcUa_Stream_Close(pOutputStream); \
    uStatus = OpcUa_MemoryStream_GetBuffer(pOutputStream, &pBuffer, &uBufferSize); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_Stream_GetPosition(pOutputStream, &uBufferSize); \
    OpcUa_GotoErrorIfBad(uStatus); \
    ++ pBuffer; \
    -- uBufferSize; \
    if(*a_pSize < uBufferSize) \
    { \
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError); \
    } \
    memcpy(*a_pBuffer, pBuffer, uBufferSize); \
    *a_pBuffer += uBufferSize; \
    *a_pSize -= uBufferSize; \
    OpcUa_Encoder_Close(pEncoder, &hEncoderContext); \
    OpcUa_Encoder_Delete(&pEncoder); \
    OpcUa_Stream_Delete(&pOutputStream); \
}


static
OpcUa_StatusCode OpcUa_UadpNetworkMessageHeader_WriteBinary(OpcUa_UadpNetworkMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte uadpVersionFlags;
    OpcUa_Byte extendedFlags1;
    OpcUa_Byte extendedFlags2;
    OpcUa_UadpDeclare_Context;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpNetworkMessageHeader_WriteBinary");

    OpcUa_UadpInit_Context;

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);


    /* assign the configured part of the flag bytes */
    uadpVersionFlags = (OpcUa_Byte)a_pValue->NetworkMessageFlags & 0x60; /* Take Byte 0 only */
    /* Bit 0-3: UADP Version, Bit 5: GroupHeader enabled, Bit 6: PayloadHeader enabled */
    uadpVersionFlags |= OPCUA_UADP_VERSION & 0x0F;

    extendedFlags1 = (OpcUa_Byte)((a_pValue->NetworkMessageFlags & 0x7800) >> 8); /* Take Byte 1 only */
    /* Bit 3: DataSetClassId enabled, Bit 4: Security enabled, Bit 5: Timestamp enabled, Bit 6: PicoSeconds enabled */
    extendedFlags2 = (OpcUa_Byte)((a_pValue->NetworkMessageFlags & 0x1F0000) >> 16); /* Take Byte 2 only */
    /* Bit 0: Chunk message, Bit 1: PromotedFields enabled */
    /* ExtendedFlags2 Bit 2-4 NetworkMessageType: 000: DataSetMessage, 001: Discovery Request, 010: Discovery Response */

    /* calculate the remaining bits */
    /* ExtendedFlags1 Bit 0-2 PublisherIdType: */
    if(a_pValue->PublisherId.ArrayType != OpcUa_VariantArrayType_Scalar)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }
    switch(a_pValue->PublisherId.Datatype)
    {
    case OpcUaType_Byte:
        /* 0x0 */
        uadpVersionFlags |= 0x10; /* PublisherId enabled */
        break;
    case OpcUaType_UInt16:
        extendedFlags1 |= 0x1;
        uadpVersionFlags |= 0x10; /* PublisherId enabled */
        break;
    case OpcUaType_UInt32:
        extendedFlags1 |= 0x2;
        uadpVersionFlags |= 0x10; /* PublisherId enabled */
        break;
    case OpcUaType_UInt64:
        extendedFlags1 |= 0x3;
        uadpVersionFlags |= 0x10; /* PublisherId enabled */
        break;
    case OpcUaType_String:
        extendedFlags1 |= 0x4;
        uadpVersionFlags |= 0x10; /* PublisherId enabled */
        break;
    case OpcUaType_Null:
        /* keep PublisherId disabled */
        break;
    default:
        /* unsupported data type */
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
        break;
    }

    /* ExtendedFlags1 Bit 7 ExtendedFlags2 enabled: */
    if(extendedFlags2)
    {
        extendedFlags1 |= 0x80;
    }

    /* UADPFlags Bit 7 ExtendedFlags1 enabled */
    if(extendedFlags1)
    {
        uadpVersionFlags |= 0x80;
    }

    OpcUa_UadpEncode_Direct(Byte, &uadpVersionFlags);
    if(extendedFlags1)
    {
        OpcUa_UadpEncode_Direct(Byte, &extendedFlags1);
    }
    if(extendedFlags2)
    {
        OpcUa_UadpEncode_Direct(Byte, &extendedFlags2);
    }

    /* cast away the const pointer */
    switch(a_pValue->PublisherId.Datatype)
    {
    case OpcUaType_Byte:
        OpcUa_UadpEncode_Direct(Byte, &a_pValue->PublisherId.Value.Byte);
        break;
    case OpcUaType_UInt16:
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->PublisherId.Value.UInt16);
        break;
    case OpcUaType_UInt32:
        OpcUa_UadpEncode_Direct(UInt32, &a_pValue->PublisherId.Value.UInt32);
        break;
    case OpcUaType_UInt64:
        OpcUa_UadpEncode_Direct(UInt64, &a_pValue->PublisherId.Value.UInt64);
        break;
    case OpcUaType_String:
        OpcUa_UadpEncode_Simple(String, &a_pValue->PublisherId.Value.String);
        break;
    case OpcUaType_Null:
        break;
    }

    if(a_pValue->NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_DataSetClassId)
    {
        OpcUa_UadpEncode_Simple(Guid, &a_pValue->DataSetClassId);
    }

    OpcUa_UadpClear_Context;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UadpClear_Context;

    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpGroupHeader_WriteBinary(OpcUa_UadpGroupHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpGroupHeader_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpEncode_Direct(Byte, &a_pValue->GroupFlags);

    if(a_pValue->GroupFlags & OpcUa_UadpGroupHeader_EncodingBit_WriterGroupId)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->WriterGroupId);
    }
    if(a_pValue->GroupFlags & OpcUa_UadpGroupHeader_EncodingBit_GroupVersion)
    {
        OpcUa_UadpEncode_Direct(UInt32, &a_pValue->GroupVersion);
    }
    if(a_pValue->GroupFlags & OpcUa_UadpGroupHeader_EncodingBit_NetworkMessageNumber)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->NetworkMessageNumber);
    }
    if(a_pValue->GroupFlags & OpcUa_UadpGroupHeader_EncodingBit_SequenceNumber)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->SequenceNumber);
    }
    a_pValue->SequenceNumber++;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpPayloadHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpPayloadHeader_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Chunk)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->Payload[0].Header.DataSetWriterId);
    }
    else
    {
        OpcUa_UadpEncode_Direct(Byte, &a_pValue->MessageCount);
        for(ii = 0; ii < a_pValue->MessageCount; ii++)
        {
            OpcUa_UadpEncode_Direct(UInt16, &a_pValue->Payload[ii].Header.DataSetWriterId);
        }
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpExtendedNetworkMessageHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpExtendedNetworkMessageHeader_WriteBinary")

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Timestamp)
    {
        /* convert structure to UInt64 */
        OpcUa_UInt64 timeValue;
        timeValue = (OpcUa_UInt32)a_pValue->NetworkMessageHeader.Timestamp.dwHighDateTime;
        timeValue <<= 32;
        timeValue += (OpcUa_UInt32)a_pValue->NetworkMessageHeader.Timestamp.dwLowDateTime;
        OpcUa_UadpEncode_Direct(UInt64, &timeValue);
    }
    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PicoSeconds)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->NetworkMessageHeader.PicoSeconds);
    }
    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PromotedFields)
    {
        OpcUa_Byte *pSizeBuffer;
        OpcUa_UInt16_Wire totalSize16_Wire;
        OpcUa_UInt16 totalSize16;

        if(*a_pSize < sizeof(OpcUa_UInt16_Wire))
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
        }

        pSizeBuffer = *a_pBuffer;
        *a_pBuffer += sizeof(OpcUa_UInt16_Wire);
        *a_pSize   -= sizeof(OpcUa_UInt16_Wire);

        for(ii = 0; ii < a_pValue->NetworkMessageHeader.NoOfPromotedFields; ii++)
        {
            if(a_pValue->NetworkMessageHeader.PromotedFields[ii] >= a_pValue->Payload[0].KeyOrEventMessage.NoOfDataSetFields)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
            }
            uStatus = OpcUa_UadpDataSetField_WriteBinary(a_pValue->Payload[0].KeyOrEventMessage.DataSetFields[a_pValue->NetworkMessageHeader.PromotedFields[ii]],
                                                         a_pValue->Payload[0].Header.FieldEncoding, a_pBuffer, a_pSize);
            OpcUa_GotoErrorIfBad(uStatus);
        }

        totalSize16 = (OpcUa_UInt16)(*a_pBuffer - pSizeBuffer) - sizeof(OpcUa_UInt16_Wire);
        uStatus = OpcUa_UInt16_P_NativeToWire(&totalSize16_Wire, &totalSize16);
        OpcUa_GotoErrorIfBad(uStatus);
        memcpy(pSizeBuffer, &totalSize16_Wire, sizeof(OpcUa_UInt16_Wire));
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpSecurityHeader_WriteBinary(OpcUa_UadpSecurityHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte nonceLength;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpSecurityHeader_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpEncode_Direct(Byte, &a_pValue->SecurityFlags);
    OpcUa_UadpEncode_Direct(UInt32, &a_pValue->SecurityTokenId);
    /* The nonce is stored as ByteString, but the length has to be encoded as Byte */
    if(a_pValue->MessageNonce.Length < 0 || a_pValue->MessageNonce.Length > 0xFF)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }

    nonceLength = (OpcUa_Byte)a_pValue->MessageNonce.Length;
    OpcUa_UadpEncode_Direct(Byte, &nonceLength);
    if(*a_pSize < nonceLength)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }
    memcpy(*a_pBuffer, a_pValue->MessageNonce.Data, nonceLength);
    *a_pBuffer += nonceLength;
    *a_pSize -= nonceLength;

    if(a_pValue->SecurityFlags & OpcUa_UadpSecurityHeader_SecurityFlags_FooterEnabled)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->SecurityFooterSize);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetMessageHeader_WriteBinary(OpcUa_UadpDataSetMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte dataSetFlags1;
    OpcUa_Byte dataSetFlags2;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetMessageHeader_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    /* Flags 1 */
    /* Bit 0 DataSetMessage is valid */
    /* Bit 1-2 Field Encoding */
    /* Bit 3 SequenceNumber enabled */
    /* Bit 4 Status enabled */
    /* Bit 5 Major Version */
    /* Bit 6 Minor Version */
    dataSetFlags1 = (OpcUa_Byte)a_pValue->DataSetEncodingFlags & 0x79;
    dataSetFlags1 |= (OpcUa_Byte)a_pValue->FieldEncoding << 1;

    /* Flags 2 */
    /* Bits 0-3 MessageType */
    /* Bit 4 Timestamp */
    /* Bit 5 PicoSeconds */
    dataSetFlags2 = (OpcUa_Byte)((a_pValue->DataSetEncodingFlags & 0x3000) >> 8);
    dataSetFlags2 |= a_pValue->DataSetMessageType;

    if(dataSetFlags2)
    {
        dataSetFlags1 |= 0x80;
    }

    OpcUa_UadpEncode_Direct(Byte, &dataSetFlags1);

    if(dataSetFlags2)
    {
        OpcUa_UadpEncode_Direct(Byte, &dataSetFlags2);
    }

    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_SequenceNumber)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->DataSetMessageSequenceNumber);
    }
    if(a_pValue->DataSetMessageType != OpcUa_DataSetMessageType_KeepAlive)
    {
        a_pValue->DataSetMessageSequenceNumber++;
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_Timestamp)
    {
        /* convert structure to UInt64 */
        OpcUa_UInt64 timeValue;
        timeValue = (OpcUa_UInt32)a_pValue->Timestamp.dwHighDateTime;
        timeValue <<= 32;
        timeValue += (OpcUa_UInt32)a_pValue->Timestamp.dwLowDateTime;
        OpcUa_UadpEncode_Direct(UInt64, &timeValue);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_PicoSeconds)
    {
        OpcUa_UadpEncode_Direct(UInt16, &a_pValue->PicoSeconds);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_Status)
    {
        /* Write only the 2 high order Bytes */
        OpcUa_UInt16 statusCode = (a_pValue->StatusCode >> 16);
        OpcUa_UadpEncode_Direct(UInt16, &statusCode);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_MajorVersion)
    {
        OpcUa_UadpEncode_Direct(UInt32, &a_pValue->Version.MajorVersion);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_MinorVersion)
    {
        OpcUa_UadpEncode_Direct(UInt32, &a_pValue->Version.MinorVersion);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetField_WriteBinary(OpcUa_DataValue* a_pValue, OpcUa_UadpDataMessage_FieldEncoding a_fieldEncoding, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_UadpDeclare_Context;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetField_WriteBinary");

    OpcUa_UadpInit_Context;

    OpcUa_GotoErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    switch(a_fieldEncoding)
    {
    case OpcUa_UadpDataMessage_FieldEncoding_Variant:
        if(OpcUa_IsBad(a_pValue->StatusCode))
        {
            /* The Variant can contain a StatusCode instead of the expected DataType if the status of the field is Bad */
            OpcUa_Byte encodingByte = OpcUaType_StatusCode;
            OpcUa_UadpEncode_Direct(Byte, &encodingByte);
            OpcUa_UadpEncode_Direct(UInt32, &a_pValue->StatusCode);
        }
        else
        {
            OpcUa_UadpEncode_Simple(Variant, &a_pValue->Value);
        }
        break;
    case OpcUa_UadpDataMessage_FieldEncoding_RawData:
        switch(a_pValue->Value.Datatype | a_pValue->Value.ArrayType << 6)
        {
        case OpcUaType_Null:
            break;
        case OpcUaType_Boolean:
            OpcUa_UadpEncode_Direct(Boolean, &a_pValue->Value.Value.Boolean);
            break;
        case OpcUaType_SByte:
            OpcUa_UadpEncode_Direct(SByte, &a_pValue->Value.Value.SByte);
            break;
        case OpcUaType_Byte:
            OpcUa_UadpEncode_Direct(Byte, &a_pValue->Value.Value.Byte);
            break;
        case OpcUaType_Int16:
            OpcUa_UadpEncode_Direct(Int16, &a_pValue->Value.Value.Int16);
            break;
        case OpcUaType_UInt16:
            OpcUa_UadpEncode_Direct(UInt16, &a_pValue->Value.Value.UInt16);
            break;
        case OpcUaType_Int32:
            OpcUa_UadpEncode_Direct(Int32, &a_pValue->Value.Value.Int32);
            break;
        case OpcUaType_UInt32:
            OpcUa_UadpEncode_Direct(UInt32, &a_pValue->Value.Value.UInt32);
            break;
        case OpcUaType_Int64:
            OpcUa_UadpEncode_Direct(Int64, &a_pValue->Value.Value.Int64);
            break;
        case OpcUaType_UInt64:
            OpcUa_UadpEncode_Direct(UInt64, &a_pValue->Value.Value.UInt64);
            break;
        case OpcUaType_Float:
            OpcUa_UadpEncode_Direct(Float, &a_pValue->Value.Value.Float);
            break;
        case OpcUaType_Double:
            OpcUa_UadpEncode_Direct(Double, &a_pValue->Value.Value.Double);
            break;
        case OpcUaType_StatusCode:
            OpcUa_UadpEncode_Direct(UInt32, &a_pValue->Value.Value.StatusCode);
            break;
        default:
            OpcUa_UadpEncode_VariantUnion(&a_pValue->Value);
            break;
        }
        break;
    case OpcUa_UadpDataMessage_FieldEncoding_DataValue:
        OpcUa_UadpEncode_Simple(DataValue, a_pValue);
        break;
    default:
        /* This should have been rejected earlier */
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }

    OpcUa_UadpClear_Context;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UadpClear_Context;

    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetMessage_WriteBinary(OpcUa_UadpDataSetMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetMessage_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpDataSetMessageHeader_WriteBinary(&a_pValue->Header, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    switch(a_pValue->Header.DataSetMessageType)
    {
        case OpcUa_DataSetMessageType_DataKeyFrame:
        case OpcUa_DataSetMessageType_Event:
            /* Number of fields of the DataSet contained in the DataSetMessage.  The FieldCount shall be omitted if RawData field encoding is set in the EncodingFlags. */
            if(a_pValue->Header.FieldEncoding != OpcUa_UadpDataMessage_FieldEncoding_RawData)
            {
                OpcUa_UadpEncode_Direct(UInt16, &a_pValue->KeyOrEventMessage.NoOfDataSetFields);
            }
            for(ii = 0; ii < a_pValue->KeyOrEventMessage.NoOfDataSetFields; ii++)
            {
                uStatus = OpcUa_UadpDataSetField_WriteBinary(a_pValue->KeyOrEventMessage.DataSetFields[ii], a_pValue->Header.FieldEncoding, a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);
            }
            break;
        case OpcUa_DataSetMessageType_DataDeltaFrame:
            OpcUa_UadpEncode_Direct(UInt16, &a_pValue->DeltaFrameMessage.NoOfDeltaFields);
            for(ii = 0; ii < a_pValue->DeltaFrameMessage.NoOfDeltaFields; ii++)
            {
                if(a_pValue->DeltaFrameMessage.DeltaFields[ii] >= a_pValue->KeyOrEventMessage.NoOfDataSetFields)
                {
                    OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
                }
                OpcUa_UadpEncode_Direct(UInt16, &a_pValue->DeltaFrameMessage.DeltaFields[ii]);
                uStatus = OpcUa_UadpDataSetField_WriteBinary(a_pValue->KeyOrEventMessage.DataSetFields[a_pValue->DeltaFrameMessage.DeltaFields[ii]],
                                                             a_pValue->Header.FieldEncoding, a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);
            }
            a_pValue->DeltaFrameMessage.NoOfDeltaFields = 0;
            break;
        case OpcUa_DataSetMessageType_KeepAlive:
            /* Nothing to do, the KeepAliveMessage has only the header */
            break;
        default:
            OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpChunkData_WriteBinary(OpcUa_UadpChunkData* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_UInt32 uSize;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpChunkData_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    if(a_pValue->ChunkOffset >= a_pValue->TotalSize)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }

    OpcUa_UadpEncode_Direct(UInt16, &a_pValue->MessageSequenceNumber);
    OpcUa_UadpEncode_Direct(UInt32, &a_pValue->ChunkOffset);
    OpcUa_UadpEncode_Direct(UInt32, &a_pValue->TotalSize);

    uSize = a_pValue->TotalSize - a_pValue->ChunkOffset;

    if(*a_pSize < sizeof(OpcUa_UInt32))
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }
    if(uSize > *a_pSize - sizeof(OpcUa_UInt32))
    {
        uSize = *a_pSize - sizeof(OpcUa_UInt32);
    }
    if(uSize == 0)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }

    OpcUa_UadpEncode_Direct(UInt32, &uSize);
    memcpy(*a_pBuffer, a_pValue->ChunkBuffer.Data + a_pValue->ChunkOffset, uSize);
    a_pValue->ChunkOffset += uSize;
    *a_pBuffer += uSize;
    *a_pSize -= uSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

OPCUA_EXPORT
OpcUa_StatusCode OpcUa_UadpMessageHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessageHeader_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpNetworkMessageHeader_WriteBinary(&a_pValue->NetworkMessageHeader, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_GroupHeader)
    {
        uStatus = OpcUa_UadpGroupHeader_WriteBinary(&a_pValue->GroupHeader, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PayloadHeader)
    {
        uStatus = OpcUa_UadpPayloadHeader_WriteBinary(a_pValue, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    uStatus = OpcUa_UadpExtendedNetworkMessageHeader_WriteBinary(a_pValue, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Security)
    {
        uStatus = OpcUa_UadpSecurityHeader_WriteBinary(&a_pValue->SecurityHeader, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

OPCUA_EXPORT
OpcUa_StatusCode OpcUa_UadpMessageBody_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessageBody_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Chunk)
    {
        if(a_pValue->ChunkData.ChunkOffset >= a_pValue->ChunkData.TotalSize && a_pValue->Payload != OpcUa_Null)
        {
            OpcUa_Byte *pDataBuffer = a_pValue->ChunkData.ChunkBuffer.Data;
            OpcUa_UInt32 uDataSize = a_pValue->ChunkData.ChunkBuffer.Length;

            a_pValue->ChunkData.MessageSequenceNumber = a_pValue->Payload->Header.DataSetMessageSequenceNumber;
            a_pValue->ChunkData.ChunkOffset = 0;
            a_pValue->ChunkData.TotalSize = 0;

            uStatus = OpcUa_UadpDataSetMessage_WriteBinary(a_pValue->Payload, &pDataBuffer, &uDataSize);
            OpcUa_GotoErrorIfBad(uStatus);

            a_pValue->ChunkData.TotalSize = (OpcUa_UInt32)(pDataBuffer - a_pValue->ChunkData.ChunkBuffer.Data);
        }

        uStatus = OpcUa_UadpChunkData_WriteBinary(&a_pValue->ChunkData, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {

        /* Before the messages, there is an array of message lengths. This field shall be omitted if count is one or if bit 6 of the UADPFlags is false. */
        if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PayloadHeader && a_pValue->MessageCount != 1)
        {
            OpcUa_Byte *pSizesBuffer;

            if(*a_pSize < a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
            }

            pSizesBuffer = *a_pBuffer;
            *a_pBuffer += a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire);
            *a_pSize   -= a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire);

            for(ii = 0; ii < a_pValue->MessageCount; ii++)
            {
                OpcUa_Byte *pMessage;
                OpcUa_UInt16_Wire size16_Wire;
                OpcUa_UInt16 size16;

                pMessage = *a_pBuffer;
                uStatus = OpcUa_UadpDataSetMessage_WriteBinary(&a_pValue->Payload[ii], a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);

                size16 = (OpcUa_UInt16)(*a_pBuffer - pMessage);
                uStatus = OpcUa_UInt16_P_NativeToWire(&size16_Wire, &size16);
                OpcUa_GotoErrorIfBad(uStatus);
                memcpy(pSizesBuffer, &size16_Wire, sizeof(OpcUa_UInt16_Wire));
                pSizesBuffer += sizeof(OpcUa_UInt16_Wire);
            }
        }
        else
        {
            for(ii = 0; ii < a_pValue->MessageCount; ii++)
            {
                uStatus = OpcUa_UadpDataSetMessage_WriteBinary(&a_pValue->Payload[ii], a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);
            }
        }
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

OPCUA_EXPORT
OpcUa_StatusCode OpcUa_UadpMessage_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessage_WriteBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpMessageHeader_WriteBinary(a_pValue, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_UadpMessageBody_WriteBinary(a_pValue, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

#endif /* OPCUA_HAVE_PUBSUB */

/*============================================================================
 * End Of File
 *===========================================================================*/
