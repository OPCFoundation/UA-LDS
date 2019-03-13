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
#include <opcua_binaryencoderinternal.h>
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

static OpcUa_StatusCode OpcUa_UadpNetworkMessageHeader_ReadBinary(OpcUa_UadpNetworkMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpGroupHeader_ReadBinary(OpcUa_UadpGroupHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpPayloadHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpExtendedNetworkMessageHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpSecurityHeader_ReadBinary(OpcUa_UadpSecurityHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetMessageHeader_ReadBinary(OpcUa_UadpDataSetMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetField_ReadBinary(OpcUa_DataValue* a_pValue, OpcUa_UadpDataMessage_FieldEncoding a_fieldEncoding, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpDataSetMessage_ReadBinary(OpcUa_UadpDataSetMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);
static OpcUa_StatusCode OpcUa_UadpChunkData_ReadBinary(OpcUa_UadpChunkData* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

#define OpcUa_UadpDecode_Direct(xType, pValue) \
{ \
    OpcUa_##xType##_Wire oValue; \
    if(*a_pSize < sizeof(oValue)) \
    { \
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError); \
    } \
    memcpy(&oValue, *a_pBuffer, sizeof(oValue)); \
    *a_pBuffer += sizeof(oValue); \
    *a_pSize -= sizeof(oValue); \
    uStatus = OpcUa_##xType##_P_WireToNative(pValue, &oValue); \
    OpcUa_GotoErrorIfBad(uStatus); \
}

#define OpcUa_UadpDeclare_Context \
    OpcUa_MessageContext cContext; \
    OpcUa_Decoder* pDecoder = OpcUa_Null; \
    OpcUa_Handle hDecoderContext = OpcUa_Null; \
    OpcUa_InputStream* pInputStream = OpcUa_Null

#define OpcUa_UadpInit_Context \
    OpcUa_MessageContext_Initialize(&cContext); \
    cContext.KnownTypes = &OpcUa_ProxyStub_g_EncodeableTypes; \
    cContext.NamespaceUris = &OpcUa_ProxyStub_g_NamespaceUris; \
    cContext.AlwaysCheckLengths = OpcUa_False

#define OpcUa_UadpClear_Context \
    OpcUa_Decoder_Close(pDecoder, &hDecoderContext); \
    OpcUa_Decoder_Delete(&pDecoder); \
    OpcUa_Stream_Delete(&pInputStream); \
    OpcUa_MessageContext_Clear(&cContext)

#define OpcUa_UadpDecode_Simple(xType, pValue) \
{ \
    OpcUa_UInt32 uPosition; \
    uStatus = OpcUa_MemoryStream_CreateReadable(*a_pBuffer, *a_pSize, &pInputStream); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_BinaryDecoder_Create(&pDecoder); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pDecoder->Open(pDecoder, pInputStream, &cContext, &hDecoderContext); \
    OpcUa_GotoErrorIfBad(uStatus); \
    OpcUa_##xType##_Clear(pValue); \
    uStatus = pDecoder->Read##xType((OpcUa_Decoder*)hDecoderContext, OpcUa_Null, pValue); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_Stream_GetPosition(pInputStream, &uPosition); \
    OpcUa_GotoErrorIfBad(uStatus); \
    *a_pBuffer += uPosition; \
    *a_pSize -= uPosition; \
    OpcUa_Decoder_Close(pDecoder, &hDecoderContext); \
    OpcUa_Decoder_Delete(&pDecoder); \
    OpcUa_Stream_Delete(&pInputStream); \
}

#define OpcUa_UadpDecode_VariantUnion(pValue) \
{ \
    OpcUa_UInt32 uPosition; \
    OpcUa_Byte uSavedByte; \
    OpcUa_Byte uEncodingByte; \
    uEncodingByte = (pValue)->Datatype; \
    if((pValue)->ArrayType != OpcUa_VariantArrayType_Scalar) \
    { \
        uEncodingByte |= OpcUa_Variant_ArrayMask; \
        if((pValue)->ArrayType == OpcUa_VariantArrayType_Matrix) \
        { \
            uEncodingByte |= OpcUa_Variant_ArrayDimensionsMask; \
        } \
    } \
    -- *a_pBuffer; \
    ++ *a_pSize; \
    uStatus = OpcUa_MemoryStream_CreateReadable(*a_pBuffer, *a_pSize, &pInputStream); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = OpcUa_BinaryDecoder_Create(&pDecoder); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uStatus = pDecoder->Open(pDecoder, pInputStream, &cContext, &hDecoderContext); \
    OpcUa_GotoErrorIfBad(uStatus); \
    uSavedByte = **a_pBuffer; \
    **a_pBuffer = uEncodingByte; \
    OpcUa_Variant_Clear(pValue); \
    uStatus = pDecoder->ReadVariant((OpcUa_Decoder*)hDecoderContext, OpcUa_Null, pValue); \
    **a_pBuffer = uSavedByte; \
    if(OpcUa_IsBad(uStatus)) \
    { \
        (pValue)->Datatype = uEncodingByte & OpcUa_Variant_TypeMask; \
        switch(uEncodingByte & (OpcUa_Variant_ArrayMask | OpcUa_Variant_ArrayDimensionsMask)) \
        { \
            case 0: \
                (pValue)->ArrayType = OpcUa_VariantArrayType_Scalar; \
                break; \
            case OpcUa_Variant_ArrayMask: \
                (pValue)->ArrayType = OpcUa_VariantArrayType_Array; \
                break; \
            case OpcUa_Variant_ArrayMask | OpcUa_Variant_ArrayDimensionsMask: \
                (pValue)->ArrayType = OpcUa_VariantArrayType_Matrix; \
                break; \
        } \
        OpcUa_GotoError; \
    } \
    uStatus = OpcUa_Stream_GetPosition(pInputStream, &uPosition); \
    OpcUa_GotoErrorIfBad(uStatus); \
    *a_pBuffer += uPosition; \
    *a_pSize -= uPosition; \
    OpcUa_Decoder_Close(pDecoder, &hDecoderContext); \
    OpcUa_Decoder_Delete(&pDecoder); \
    OpcUa_Stream_Delete(&pInputStream); \
}


static
OpcUa_StatusCode OpcUa_UadpNetworkMessageHeader_ReadBinary(OpcUa_UadpNetworkMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte uadpVersionFlags;
    OpcUa_Byte extendedFlags1 = 0;
    OpcUa_Byte extendedFlags2 = 0;
    OpcUa_UadpDeclare_Context;
    OpcUa_String str;

    /* Bit 5: DataSet array enabled  *//* Bit 6: DataSetWriterId enabled*/
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpNetworkMessageHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpInit_Context;
    OpcUa_String_Initialize(&str);

    /* assign the configured part of the flag bytes */
    OpcUa_UadpDecode_Direct(Byte, &uadpVersionFlags);
    if(uadpVersionFlags & 0x80)
    {
        OpcUa_UadpDecode_Direct(Byte, &extendedFlags1);
    }
    if(extendedFlags1 & 0x80)
    {
        OpcUa_UadpDecode_Direct(Byte, &extendedFlags2);
    }


    if((uadpVersionFlags & 0x0F) != OPCUA_UADP_VERSION)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    if((uadpVersionFlags & 0x60) != (a_pValue->NetworkMessageFlags & 0x60))
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    if((extendedFlags1 & 0x78) != (a_pValue->NetworkMessageFlags & 0x7800) >> 8)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    if((extendedFlags2 & 0x1F) != (a_pValue->NetworkMessageFlags & 0x1F0000) >> 16)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    if(a_pValue->PublisherId.ArrayType != OpcUa_VariantArrayType_Scalar)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadEncodingError);
    }
    switch(a_pValue->PublisherId.Datatype)
    {
    case OpcUaType_Byte:
        if((uadpVersionFlags & 0x10) && (extendedFlags1 & 0x07) == 0x0)
        {
            OpcUa_Byte byte;

            OpcUa_UadpDecode_Direct(Byte, &byte);
            if(byte != a_pValue->PublisherId.Value.Byte)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
        else
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    case OpcUaType_UInt16:
        if((uadpVersionFlags & 0x10) && (extendedFlags1 & 0x07) == 0x1)
        {
            OpcUa_UInt16 uint16;

            OpcUa_UadpDecode_Direct(UInt16, &uint16);
            if(uint16 != a_pValue->PublisherId.Value.UInt16)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
        else
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    case OpcUaType_UInt32:
        if((uadpVersionFlags & 0x10) && (extendedFlags1 & 0x07) == 0x2)
        {
            OpcUa_UInt32 uint32;

            OpcUa_UadpDecode_Direct(UInt32, &uint32);
            if(uint32 != a_pValue->PublisherId.Value.UInt32)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
        else
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    case OpcUaType_UInt64:
        if((uadpVersionFlags & 0x10) && (extendedFlags1 & 0x07) == 0x3)
        {
            OpcUa_UInt64 uint64;

            OpcUa_UadpDecode_Direct(UInt64, &uint64);
            if(uint64 != a_pValue->PublisherId.Value.UInt64)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
        else
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    case OpcUaType_String:
        if((uadpVersionFlags & 0x10) && (extendedFlags1 & 0x07) == 0x4)
        {
            OpcUa_UadpDecode_Simple(String, &str);
            if(OpcUa_String_StrnCmp(&str, &a_pValue->PublisherId.Value.String, OPCUA_STRING_LENDONTCARE, OpcUa_False) != 0)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
        else
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    case OpcUaType_Null:
        if(uadpVersionFlags & 0x10)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        break;
    default:
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        break;
    }

    OpcUa_String_Clear(&str);
    OpcUa_UadpClear_Context;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_String_Clear(&str);
    OpcUa_UadpClear_Context;

    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpGroupHeader_ReadBinary(OpcUa_UadpGroupHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte    groupFlags;
    OpcUa_UInt16  writerGroupId;
    OpcUa_UInt32  groupVersion;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpGroupHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpDecode_Direct(Byte, &groupFlags);

    if(groupFlags != a_pValue->GroupFlags)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    if(groupFlags & OpcUa_UadpGroupHeader_EncodingBit_WriterGroupId)
    {
        OpcUa_UadpDecode_Direct(UInt16, &writerGroupId);
        if(writerGroupId != a_pValue->WriterGroupId)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
    }
    if(groupFlags & OpcUa_UadpGroupHeader_EncodingBit_GroupVersion)
    {
        OpcUa_UadpDecode_Direct(UInt32, &groupVersion);
        if(groupVersion != a_pValue->GroupVersion)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
    }
    if(groupFlags & OpcUa_UadpGroupHeader_EncodingBit_NetworkMessageNumber)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->NetworkMessageNumber);
    }
    if(groupFlags & OpcUa_UadpGroupHeader_EncodingBit_SequenceNumber)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->SequenceNumber);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpPayloadHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32  ii;
    OpcUa_Byte   messageCount;
    OpcUa_UInt16 dataSetWriterId;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpPayloadHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Chunk)
    {
        OpcUa_UadpDecode_Direct(UInt16, &dataSetWriterId);
        if(dataSetWriterId != a_pValue->Payload[0].Header.DataSetWriterId)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
    }
    else
    {
        OpcUa_UadpDecode_Direct(Byte, &messageCount);
        if(messageCount != a_pValue->MessageCount)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        for(ii = 0; ii < a_pValue->MessageCount; ii++)
        {
            OpcUa_UadpDecode_Direct(UInt16, &dataSetWriterId);
            if(dataSetWriterId != a_pValue->Payload[ii].Header.DataSetWriterId)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
        }
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpExtendedNetworkMessageHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpExtendedNetworkMessageHeader_ReadBinary")

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Timestamp)
    {
        /* convert UInt64 to structure */
        OpcUa_UInt64 timeValue;
        OpcUa_UadpDecode_Direct(UInt64, &timeValue);
        a_pValue->NetworkMessageHeader.Timestamp.dwHighDateTime = (OpcUa_UInt32)(timeValue >> 32);
        a_pValue->NetworkMessageHeader.Timestamp.dwLowDateTime  = (OpcUa_UInt32)(timeValue & 0x00000000FFFFFFFF);
    }
    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PicoSeconds)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->NetworkMessageHeader.PicoSeconds);
    }
    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PromotedFields)
    {
        OpcUa_UInt16 totalSize16;
        OpcUa_UInt32 totalSize;
        OpcUa_Byte *pPromotedFields;

        OpcUa_UadpDecode_Direct(UInt16, &totalSize16);
        if(*a_pSize < totalSize16)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
        totalSize = totalSize16;
        pPromotedFields = *a_pBuffer;
        for(ii = 0; ii < a_pValue->NetworkMessageHeader.NoOfPromotedFields; ii++)
        {
            if(a_pValue->NetworkMessageHeader.PromotedFields[ii] >= a_pValue->Payload[0].KeyOrEventMessage.NoOfDataSetFields)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
            uStatus = OpcUa_UadpDataSetField_ReadBinary(a_pValue->Payload[0].KeyOrEventMessage.DataSetFields[a_pValue->NetworkMessageHeader.PromotedFields[ii]],
                                                        a_pValue->Payload[0].Header.FieldEncoding, &pPromotedFields, &totalSize);
            OpcUa_GotoErrorIfBad(uStatus);
        }
        *a_pBuffer += totalSize16;
        *a_pSize -= totalSize16;
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpSecurityHeader_ReadBinary(OpcUa_UadpSecurityHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte securityFlags;
    OpcUa_Byte nonceLength;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpSecurityHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpDecode_Direct(Byte, &securityFlags);
    if(securityFlags != a_pValue->SecurityFlags)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    OpcUa_UadpDecode_Direct(UInt32, &a_pValue->SecurityTokenId);
    /* The nonce is stored as ByteString, but the length has to be encoded as Byte */
    OpcUa_UadpDecode_Direct(Byte, &nonceLength);
    if(nonceLength != a_pValue->MessageNonce.Length)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    if(*a_pSize < nonceLength)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    memcpy(a_pValue->MessageNonce.Data, *a_pBuffer, nonceLength);
    *a_pBuffer += nonceLength;
    *a_pSize -= nonceLength;

    if(a_pValue->SecurityFlags & OpcUa_UadpSecurityHeader_SecurityFlags_FooterEnabled)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->SecurityFooterSize);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetMessageHeader_ReadBinary(OpcUa_UadpDataSetMessageHeader* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Byte dataSetFlags1;
    OpcUa_Byte dataSetFlags2 = 0;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetMessageHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    /* Flags 1 */
    OpcUa_UadpDecode_Direct(Byte, &dataSetFlags1);
    /* Bit 0 DataSetMessage is valid */
    /* Bit 1-2 Field Encoding */
    /* Bit 3 SequenceNumber enabled */
    /* Bit 4 Status enabled */
    /* Bit 5 Major Version */
    /* Bit 6 Minor Version */
    if((dataSetFlags1 & 0x79) != ((OpcUa_Byte)a_pValue->DataSetEncodingFlags & 0x79))
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    if((dataSetFlags1 & 0x06) != ((OpcUa_Byte)a_pValue->FieldEncoding << 1))
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    /* Flags 2 */
    if(dataSetFlags1 & 0x80)
    {
        OpcUa_UadpDecode_Direct(Byte, &dataSetFlags2);
    }
    /* Bits 0-3 MessageType */
    /* Bit 4 Timestamp */
    /* Bit 5 PicoSeconds */
    if((dataSetFlags2 & 0x30) != (OpcUa_Byte)((a_pValue->DataSetEncodingFlags & 0x3000) >> 8))
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }
    a_pValue->DataSetMessageType = (OpcUa_DataSetMessageType)(dataSetFlags2 & 0x0F);

    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_SequenceNumber)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->DataSetMessageSequenceNumber);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_Timestamp)
    {
        /* convert UInt64 to structure */
        OpcUa_UInt64 timeValue;
        OpcUa_UadpDecode_Direct(UInt64, &timeValue);
        a_pValue->Timestamp.dwHighDateTime = (OpcUa_UInt32)(timeValue >> 32);
        a_pValue->Timestamp.dwLowDateTime  = (OpcUa_UInt32)(timeValue & 0x00000000FFFFFFFF);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_PicoSeconds)
    {
        OpcUa_UadpDecode_Direct(UInt16, &a_pValue->PicoSeconds);
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_Status)
    {
        /* Read only the 2 high order Bytes */
        OpcUa_UInt16 statusCode;
        OpcUa_UadpDecode_Direct(UInt16, &statusCode);
        a_pValue->StatusCode = (OpcUa_UInt32)statusCode << 16;
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_MajorVersion)
    {
        OpcUa_UInt32 majorVersion;
        OpcUa_UadpDecode_Direct(UInt32, &majorVersion);
        if(majorVersion != a_pValue->Version.MajorVersion)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
    }
    if(a_pValue->DataSetEncodingFlags & OpcUa_UadpDataMessage_FlagsEncodingBit_MinorVersion)
    {
        OpcUa_UInt32 minorVersion;
        OpcUa_UadpDecode_Direct(UInt32, &minorVersion);
        if(minorVersion != a_pValue->Version.MinorVersion)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
        }
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetField_ReadBinary(OpcUa_DataValue* a_pValue, OpcUa_UadpDataMessage_FieldEncoding a_fieldEncoding, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_UadpDeclare_Context;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetField_ReadBinary");

    OpcUa_UadpInit_Context;

    OpcUa_GotoErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    switch(a_fieldEncoding)
    {
    case OpcUa_UadpDataMessage_FieldEncoding_Variant:
        /* The Variant can contain a StatusCode instead of the expected DataType if the status of the field is Bad */
        if(*a_pSize >= 5 && **a_pBuffer == OpcUaType_StatusCode && (*a_pBuffer)[4] & 0x80)
        {
            OpcUa_Byte encodingByte;
            OpcUa_UadpDecode_Direct(Byte, &encodingByte);
            OpcUa_UadpDecode_Direct(UInt32, &a_pValue->StatusCode);
        }
        else
        {
            OpcUa_UadpDecode_Simple(Variant, &a_pValue->Value);
            a_pValue->StatusCode = OpcUa_Good;
        }
        break;
    case OpcUa_UadpDataMessage_FieldEncoding_RawData:
        switch(a_pValue->Value.Datatype | a_pValue->Value.ArrayType << 6)
        {
        case OpcUaType_Null:
            break;
        case OpcUaType_Boolean:
            OpcUa_UadpDecode_Direct(Boolean, &a_pValue->Value.Value.Boolean);
            break;
        case OpcUaType_SByte:
            OpcUa_UadpDecode_Direct(SByte, &a_pValue->Value.Value.SByte);
            break;
        case OpcUaType_Byte:
            OpcUa_UadpDecode_Direct(Byte, &a_pValue->Value.Value.Byte);
            break;
        case OpcUaType_Int16:
            OpcUa_UadpDecode_Direct(Int16, &a_pValue->Value.Value.Int16);
            break;
        case OpcUaType_UInt16:
            OpcUa_UadpDecode_Direct(UInt16, &a_pValue->Value.Value.UInt16);
            break;
        case OpcUaType_Int32:
            OpcUa_UadpDecode_Direct(Int32, &a_pValue->Value.Value.Int32);
            break;
        case OpcUaType_UInt32:
            OpcUa_UadpDecode_Direct(UInt32, &a_pValue->Value.Value.UInt32);
            break;
        case OpcUaType_Int64:
            OpcUa_UadpDecode_Direct(Int64, &a_pValue->Value.Value.Int64);
            break;
        case OpcUaType_UInt64:
            OpcUa_UadpDecode_Direct(UInt64, &a_pValue->Value.Value.UInt64);
            break;
        case OpcUaType_Float:
            OpcUa_UadpDecode_Direct(Float, &a_pValue->Value.Value.Float);
            break;
        case OpcUaType_Double:
            OpcUa_UadpDecode_Direct(Double, &a_pValue->Value.Value.Double);
            break;
        case OpcUaType_StatusCode:
            OpcUa_UadpDecode_Direct(UInt32, &a_pValue->Value.Value.StatusCode);
            break;
        default:
            OpcUa_UadpDecode_VariantUnion(&a_pValue->Value);
            break;
        }
        break;
    case OpcUa_UadpDataMessage_FieldEncoding_DataValue:
        OpcUa_UadpDecode_Simple(DataValue, a_pValue);
        break;
    default:
        /* This should have been rejected earlier */
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    OpcUa_UadpClear_Context;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UadpClear_Context;

    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpDataSetMessage_ReadBinary(OpcUa_UadpDataSetMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpDataSetMessage_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpDataSetMessageHeader_ReadBinary(&a_pValue->Header, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    switch(a_pValue->Header.DataSetMessageType)
    {
        case OpcUa_DataSetMessageType_DataKeyFrame:
        case OpcUa_DataSetMessageType_Event:
            /* Number of fields of the DataSet contained in the DataSetMessage.  The FieldCount shall be omitted if RawData field encoding is set in the EncodingFlags. */
            if(a_pValue->Header.FieldEncoding != OpcUa_UadpDataMessage_FieldEncoding_RawData)
            {
                OpcUa_UInt16 noOfDataSetFields;
                OpcUa_UadpDecode_Direct(UInt16, &noOfDataSetFields);
                if(noOfDataSetFields != a_pValue->KeyOrEventMessage.NoOfDataSetFields)
                {
                    OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
                }
            }
            for(ii = 0; ii < a_pValue->KeyOrEventMessage.NoOfDataSetFields; ii++)
            {
                uStatus = OpcUa_UadpDataSetField_ReadBinary(a_pValue->KeyOrEventMessage.DataSetFields[ii], a_pValue->Header.FieldEncoding, a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);
            }
            break;
        case OpcUa_DataSetMessageType_DataDeltaFrame:
            if(a_pValue->DeltaFrameMessage.DeltaFields == OpcUa_Null)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
            OpcUa_UadpDecode_Direct(UInt16, &a_pValue->DeltaFrameMessage.NoOfDeltaFields);
            if(a_pValue->DeltaFrameMessage.NoOfDeltaFields > a_pValue->KeyOrEventMessage.NoOfDataSetFields)
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }
            for(ii = 0; ii < a_pValue->DeltaFrameMessage.NoOfDeltaFields; ii++)
            {
                OpcUa_UadpDecode_Direct(UInt16, &a_pValue->DeltaFrameMessage.DeltaFields[ii]);
                if(a_pValue->DeltaFrameMessage.DeltaFields[ii] >= a_pValue->KeyOrEventMessage.NoOfDataSetFields)
                {
                    OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
                }
                uStatus = OpcUa_UadpDataSetField_ReadBinary(a_pValue->KeyOrEventMessage.DataSetFields[a_pValue->DeltaFrameMessage.DeltaFields[ii]], a_pValue->Header.FieldEncoding, a_pBuffer, a_pSize);
                OpcUa_GotoErrorIfBad(uStatus);
            }
            break;
        case OpcUa_DataSetMessageType_KeepAlive:
            /* Nothing to do, the KeepAliveMessage has only the header */
            break;
        default:
            OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

static
OpcUa_StatusCode OpcUa_UadpChunkData_ReadBinary(OpcUa_UadpChunkData* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_UInt16 messageSequenceNumber;
    OpcUa_UInt32 chunkOffset;
    OpcUa_UInt32 totalSize;
    OpcUa_UInt32 uSize;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpChunkData_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    OpcUa_UadpDecode_Direct(UInt16, &messageSequenceNumber);
    OpcUa_UadpDecode_Direct(UInt32, &chunkOffset);
    OpcUa_UadpDecode_Direct(UInt32, &totalSize);
    OpcUa_UadpDecode_Direct(UInt32, &uSize);

    if(totalSize == 0 || chunkOffset >= totalSize
       || uSize > *a_pSize || totalSize > (OpcUa_UInt32)a_pValue->ChunkBuffer.Length
       || uSize > totalSize || uSize > totalSize - chunkOffset)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    if(messageSequenceNumber != a_pValue->MessageSequenceNumber || totalSize != a_pValue->TotalSize)
    {
        a_pValue->MessageSequenceNumber = messageSequenceNumber;
        a_pValue->TotalSize = totalSize;
        a_pValue->ChunkOffset = 0;
        if(a_pValue->ShadowBits.Data != OpcUa_Null)
        {
            memset(a_pValue->ShadowBits.Data, 0, (totalSize + 7) / 8);
        }
    }

    if(a_pValue->ChunkOffset > a_pValue->TotalSize)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    if(a_pValue->ShadowBits.Data != OpcUa_Null)
    {
        if(a_pValue->ShadowBits.Data[chunkOffset / 8] & (1 << (chunkOffset % 8)))
        {
            /* ignore duplicate network message */
            *a_pBuffer += uSize;
            *a_pSize -= uSize;
            OpcUa_ReturnStatusCode;
        }
        a_pValue->ShadowBits.Data[chunkOffset / 8] |= 1 << (chunkOffset % 8);
    }

    memcpy(a_pValue->ChunkBuffer.Data + chunkOffset, *a_pBuffer, uSize);
    *a_pBuffer += uSize;
    *a_pSize -= uSize;
    a_pValue->ChunkOffset += uSize;

    if(a_pValue->ChunkOffset > a_pValue->TotalSize)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

OPCUA_EXPORT
OpcUa_StatusCode OpcUa_UadpMessageHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessageHeader_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpNetworkMessageHeader_ReadBinary(&a_pValue->NetworkMessageHeader, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_GroupHeader)
    {
        uStatus = OpcUa_UadpGroupHeader_ReadBinary(&a_pValue->GroupHeader, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PayloadHeader)
    {
        uStatus = OpcUa_UadpPayloadHeader_ReadBinary(a_pValue, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    uStatus = OpcUa_UadpExtendedNetworkMessageHeader_ReadBinary(a_pValue, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Security)
    {
        uStatus = OpcUa_UadpSecurityHeader_ReadBinary(&a_pValue->SecurityHeader, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;
    /* nothing to do */
    OpcUa_FinishErrorHandling;
}

OPCUA_EXPORT
OpcUa_StatusCode OpcUa_UadpMessageBody_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_Int32 ii;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessageBody_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_Chunk)
    {
        uStatus = OpcUa_UadpChunkData_ReadBinary(&a_pValue->ChunkData, a_pBuffer, a_pSize);
        OpcUa_GotoErrorIfBad(uStatus);

        if(a_pValue->ChunkData.ChunkOffset == a_pValue->ChunkData.TotalSize && a_pValue->Payload != OpcUa_Null)
        {
            OpcUa_Byte *pDataBuffer = a_pValue->ChunkData.ChunkBuffer.Data;
            OpcUa_UInt32 nDataSize = a_pValue->ChunkData.TotalSize;

            a_pValue->ChunkData.ChunkOffset = 0;
            a_pValue->ChunkData.TotalSize = 0;

            uStatus = OpcUa_UadpDataSetMessage_ReadBinary(a_pValue->Payload, &pDataBuffer, &nDataSize);
            OpcUa_GotoErrorIfBad(uStatus);
        }
    }
    else
    {
        /* Before the messages, there is an array of message lengths. This field shall be omitted if count is one or if bit 6 of the UADPFlags is false. */
        if(a_pValue->NetworkMessageHeader.NetworkMessageFlags & OpcUa_UadpNetworkMessageFlags_PayloadHeader && a_pValue->MessageCount != 1)
        {
            OpcUa_Byte *pSizesBuffer;

            if(*a_pSize < a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
            }

            pSizesBuffer = *a_pBuffer;
            *a_pBuffer += a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire);
            *a_pSize -= a_pValue->MessageCount * sizeof(OpcUa_UInt16_Wire);

            for(ii = 0; ii < a_pValue->MessageCount; ii++)
            {
                OpcUa_UInt16_Wire size16_Wire;
                OpcUa_UInt16 size16;
                OpcUa_Byte* pMessage;
                OpcUa_UInt32 uSize;

                memcpy(&size16_Wire, pSizesBuffer, sizeof(OpcUa_UInt16_Wire));
                pSizesBuffer += sizeof(OpcUa_UInt16_Wire);
                uStatus = OpcUa_UInt16_P_WireToNative(&size16, &size16_Wire);
                OpcUa_GotoErrorIfBad(uStatus);
                if(size16 > *a_pSize)
                {
                     OpcUa_GotoErrorWithStatus(OpcUa_BadDecodingError);
                }

                pMessage = *a_pBuffer;
                uSize = size16;
                uStatus = OpcUa_UadpDataSetMessage_ReadBinary(&a_pValue->Payload[ii], &pMessage, &uSize);
                OpcUa_GotoErrorIfBad(uStatus);
                *a_pBuffer += size16;
                *a_pSize -= size16;
            }
        }
        else
        {
            for(ii = 0; ii < a_pValue->MessageCount; ii++)
            {
                uStatus = OpcUa_UadpDataSetMessage_ReadBinary(&a_pValue->Payload[ii], a_pBuffer, a_pSize);
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
OpcUa_StatusCode OpcUa_UadpMessage_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize)
{
    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_UadpMessage_ReadBinary");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(*a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    uStatus = OpcUa_UadpMessageHeader_ReadBinary(a_pValue, a_pBuffer, a_pSize);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_UadpMessageBody_ReadBinary(a_pValue, a_pBuffer, a_pSize);
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
