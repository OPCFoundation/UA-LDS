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

#ifndef OpcUa_Uadp_Networkmessage_H_
#define OpcUa_Uadp_Networkmessage_H_ 1

#if OPCUA_HAVE_PUBSUB

OPCUA_BEGIN_EXTERN_C

#define OPCUA_UADP_VERSION 1

typedef enum _OpcUa_UadpNetworkMessageFlags
{
    OpcUa_UadpNetworkMessageFlags_GroupHeader         = 0x000020, /*!< UADP Flags     Bit 5: GroupHeader enabled */
    OpcUa_UadpNetworkMessageFlags_PayloadHeader       = 0x000040, /*!< UADP Flags     Bit 6: PayloadHeader enabled */
    OpcUa_UadpNetworkMessageFlags_DataSetClassId      = 0x000800, /*!< ExtendedFlags1 Bit 3: DataSetClassId enabled */
    OpcUa_UadpNetworkMessageFlags_Security            = 0x001000, /*!< ExtendedFlags1 Bit 4: Security enabled */
    OpcUa_UadpNetworkMessageFlags_Timestamp           = 0x002000, /*!< ExtendedFlags1 Bit 5: Timestamp enabled */
    OpcUa_UadpNetworkMessageFlags_PicoSeconds         = 0x004000, /*!< ExtendedFlags1 Bit 6: PicoSeconds enabled */
    OpcUa_UadpNetworkMessageFlags_Chunk               = 0x010000, /*!< ExtendedFlags2 Bit 0: NetworkMessage with chunk payload */
    OpcUa_UadpNetworkMessageFlags_PromotedFields      = 0x020000, /*!< ExtendedFlags2 Bit 1: PromotedFields enabled */
    OpcUa_UadpNetworkMessageFlags_DiscoveryRequest    = 0x040000, /*!< ExtendedFlags2 Bit 2-4: 001 NetworkMessage with dicovery request payload */
    OpcUa_UadpNetworkMessageFlags_DiscoveryResponse   = 0x080000  /*!< ExtendedFlags2 Bit 2-4: 010 NetworkMessage with dicovery response payload */
}
OpcUa_UadpNetworkMessageFlags;


typedef struct _OpcUa_UadpNetworkMessageHeader
{
    OpcUa_UInt32             NetworkMessageFlags; /** A Bit mask, whose single flags are defined by OpcUa_PubSubNetworkMessageFlags. */
    OpcUa_Variant            PublisherId;         /** The Id of the Publisher that sent the data. Valid DataTypes are UInteger, Guid and String.
                                                    * The Subscribers can skip NetworkMessages from Publishers it does not expect NetworkMessages from. */
    OpcUa_Guid               DataSetClassId;      /** A DataSetClass can be used to describe that the structure of the DataSets follow a certain template.
                                                    * When this ID is set, it describes that all DataSets of this NetworkMessage use the same template. */
    /* Extended header fields */
    OpcUa_DateTime           Timestamp;           /** The time the Data was collected. */
    OpcUa_UInt16             PicoSeconds;         /** Specifies the number of 10 picoseconds (1,0 e-11 seconds) intervals which shall be added to the Timestamp. */
    OpcUa_UInt16             NoOfPromotedFields;  /** Number of used fields of the DataSet contained in the DataSetMessage. */
    OpcUa_UInt16*            PromotedFields;      /** The index field values of the DataSet.  This refers to the KeyOrEventMessage array.  */
} OpcUa_UadpNetworkMessageHeader;


typedef struct _OpcUa_UadpGroupHeader
{
    OpcUa_Byte          GroupFlags;             /** Bit 0: WriterGroupId enabled, Bit 1: GroupVersion enabled, Bit 2: NetworkMessageNumber enabled, Bit 3: SequenceNumber enabled */
    OpcUa_UInt16        WriterGroupId;          /** The ID of the PubSub WriterGroup, which contains the DataSets */
    OpcUa_UInt32        GroupVersion;           /** Version of the header and payload layout configuration of the NetworkMessages sent for the group. */
    OpcUa_UInt16        NetworkMessageNumber;   /** Unique number of a NetworkMessage across the combination of PublisherId and WriterGroupId within one PublishingInterval. */
    OpcUa_UInt16        SequenceNumber;         /** Sequence number for the NetworkMessage */
} OpcUa_UadpGroupHeader;


typedef struct _OpcUa_UadpSecurityHeader
{
    OpcUa_Byte          SecurityFlags;      /** Bit 0: NetworkMessage Signed, Bit 1: NetworkMessage Encrypted, Bit 2: SecurityFooter enabled, Bit 3: Force key reset */
    OpcUa_UInt32        SecurityTokenId;    /** The ID of the security token that identifies the security key in a SecurityGroup */
    OpcUa_ByteString    MessageNonce;       /** A number used exactly once for a given security key. For a given security key a unique nonce shall be generated for every NetworkMessage. */
    OpcUa_UInt16        SecurityFooterSize; /** The size of the SecurityFooter.  The security footer size shall be omitted if bit 2 of the SecurityFlags is false. */
} OpcUa_UadpSecurityHeader;


/** This structure is used to indicate configuration changes in the information published for a DataSet.
*
* Both version members of this structure represent the time when the configuration was changed in seconds since Jan. 1st, 2000 00:00:00 (UTC). */
typedef struct _OpcUa_ConfigurationVersion
{
    OpcUa_UInt32 MajorVersion; /** The major version of the DataSet. A change in the major version means that existing fields were changed, reordered or removed. */
    OpcUa_UInt32 MinorVersion; /** The minor version of the DataSet. A change in the minor version means that additional fields were appended to the message, all fields of the previous version remain unchanged. */
} OpcUa_ConfigurationVersion;


/** Defines the encoding bits in the GroupHeader */
typedef enum _OpcUa_UadpGroupHeader_EncodingBits
{
    OpcUa_UadpGroupHeader_EncodingBit_WriterGroupId           = 0x01, /*!< Bit 0: WriterGroupId enabled */
    OpcUa_UadpGroupHeader_EncodingBit_GroupVersion            = 0x02, /*!< Bit 1: GroupVersion enabled */
    OpcUa_UadpGroupHeader_EncodingBit_NetworkMessageNumber    = 0x04, /*!< Bit 2: NetworkMessageNumber enabled */
    OpcUa_UadpGroupHeader_EncodingBit_SequenceNumber          = 0x08  /*!< Bit 3: SequenceNumber enabled */
}
OpcUa_UadpGroupHeader_EncodingBits;


/** Defines the security flags in the SecurityHeader */
typedef enum _OpcUa_UadpSecurityHeader_SecurityFlags
{
    OpcUa_UadpSecurityHeader_SecurityFlags_Signed             = 0x01, /*!< Bit 0: NetworkMessage Signed */
    OpcUa_UadpSecurityHeader_SecurityFlags_Encrypted          = 0x02, /*!< Bit 1: GroupVersion enabled */
    OpcUa_UadpSecurityHeader_SecurityFlags_FooterEnabled      = 0x04, /*!< Bit 2: SecurityFooter enabled */
    OpcUa_UadpSecurityHeader_SecurityFlags_ForceKeyReset      = 0x08  /*!< Bit 3: Force key reset */
}
OpcUa_UadpSecurityHeader_SecurityFlags;


/** The type of message. */
typedef enum _OpcUa_DataSetMessageType
{
    OpcUa_DataSetMessageType_DataKeyFrame                     = 0x0, /*!< 0000 Data Key Frame. */
    OpcUa_DataSetMessageType_DataDeltaFrame                   = 0x1, /*!< 0001 Data Delta Frame. */
    OpcUa_DataSetMessageType_Event                            = 0x2, /*!< 0010 Event. */
    OpcUa_DataSetMessageType_KeepAlive                        = 0x3  /*!< 0011 Keep Alive. */
}
OpcUa_DataSetMessageType;


/** Defines the header content and encoding of a DataSetMessage */
typedef enum _OpcUa_UadpDataMessage_FieldEncoding
{
    OpcUa_UadpDataMessage_FieldEncoding_Variant     = 0,    /*!< 00 The DataSetFields are encoded as Variant
                                                            * The Variant can contain a StatusCode instead of the expected DataType if the status of the field is Bad.
                                                            * The Variant can contain a DataValue if the status of the field is Uncertain. */
    OpcUa_UadpDataMessage_FieldEncoding_RawData     = 1,    /*!< 01 RawData Field Encoding
                                                            * The DataSet fields are encoded in the DataTypes specified in the DataSetMetaData for the DataSet.
                                                            * The encoding is handled like a Structure DataType where the DataSet fields are handled like Structure fields and fields with Structure DataType are handled like nested structures.
                                                            * All restrictions for the encoding of Structure DataTypes also apply to the RawData Field Encoding. */
    OpcUa_UadpDataMessage_FieldEncoding_DataValue   = 2     /*!< 10 DataValue Field Encoding
                                                            * The DataSet fields are encoded as DataValue. This option is set if the DataSet is configured to send more than the Value. */
}
OpcUa_UadpDataMessage_FieldEncoding;


/** Defines the encoding bits in a DataSetMessage header */
typedef enum _OpcUa_UadpDataMessage_EncodingFlags
{
    /* DataSetFlags1 */
    OpcUa_UadpDataMessage_FlagsEncodingBit_IsValid       = 0x01,  /*!< Bit 0: DataSetMessage is valid. */
    /* Bits 1-2 are for the FieldEncoding */
    OpcUa_UadpDataMessage_FlagsEncodingBit_SequenceNumber= 0x08,  /*!< Bit 3: DataSetMessageSequenceNumber enabled. */
    OpcUa_UadpDataMessage_FlagsEncodingBit_Status        = 0x10,  /*!< Bit 4: Status included in the DataSetMessage header. */
    OpcUa_UadpDataMessage_FlagsEncodingBit_MajorVersion  = 0x20,  /*!< Bit 5: MajorVersion included in the DataSetMessage header. */
    OpcUa_UadpDataMessage_FlagsEncodingBit_MinorVersion  = 0x40,  /*!< Bit 6: MinorVersion included in the DataSetMessage header. */
    /* DataSetFlags2 */
    /* Bit 0-3 are for the MessageType */
    OpcUa_UadpDataMessage_FlagsEncodingBit_Timestamp     = 0x1000,  /*!< Bit 4: Timestamp included in the DataSetMessage header. */
    OpcUa_UadpDataMessage_FlagsEncodingBit_PicoSeconds   = 0x2000   /*!< Bit 5: PicoSeconds included in the DataSetMessage header. */
}
OpcUa_UadpDataMessage_EncodingFlags;


typedef struct _OpcUa_UadpDataSetMessageHeader
{
    OpcUa_UInt16                        DataSetWriterId;               /** DataSetWriterId for Payload Header. */
    OpcUa_UInt16                        DataSetEncodingFlags;          /** Encoding Flags for DataSetMessage Header. */
    OpcUa_DataSetMessageType            DataSetMessageType;            /** The type of message. */
    OpcUa_UadpDataMessage_FieldEncoding FieldEncoding;                 /** Defines how the fields of the message is encoded. */
    OpcUa_DateTime                      Timestamp;                     /** The time the Data was collected. */
    OpcUa_UInt16                        PicoSeconds;                   /** Specifies the number of 10 picoseconds (1,0 e-11 seconds) intervals which shall be added to the Timestamp. */
    OpcUa_UInt16                        DataSetMessageSequenceNumber;  /** A monotonically increasing sequence number assigned by the publisher to each DataSetMessage sent. */
    OpcUa_StatusCode                    StatusCode;                    /** The high order 16 bits of a StatusCode representing the numeric value of the code. */
    OpcUa_ConfigurationVersion          Version;                       /** The mayor and minor version of the data set */
} OpcUa_UadpDataSetMessageHeader;


typedef struct _OpcUa_UadpKeyOrEventMessageBody
{
    OpcUa_UInt16                        NoOfDataSetFields;             /** Number of fields of the DataSet contained in the DataSetMessage. */
    OpcUa_DataValue**                   DataSetFields;                 /** The field values of the DataSet. */
} OpcUa_UadpKeyOrEventMessageBody;


typedef struct _OpcUa_UadpDeltaFrameMessageBody
{
    OpcUa_UInt16                        NoOfDeltaFields;               /** Number of used fields of the DataSet contained in the DataSetMessage. */
    OpcUa_UInt16*                       DeltaFields;                   /** The index field values of the DataSet.  This refers to the KeyOrEventMessage array.  */
} OpcUa_UadpDeltaFrameMessageBody;


typedef struct _OpcUa_UadpDataSetMessage
{
    OpcUa_UadpDataSetMessageHeader      Header;                        /** The header describing the content of the body. */
    OpcUa_UadpKeyOrEventMessageBody     KeyOrEventMessage;             /** The content of the DataSetMessage, described by the Header. */
    OpcUa_UadpDeltaFrameMessageBody     DeltaFrameMessage;             /** The content of the Data Delta Frame Message. */
} OpcUa_UadpDataSetMessage;


typedef struct _OpcUa_UadpChunkData
{
    OpcUa_UInt16                        MessageSequenceNumber;         /** Current DataSetMessageSequenceNumber. */
    OpcUa_UInt32                        ChunkOffset;                   /** Chunk is complete when ChunkOffset == TotalSize. */
    OpcUa_UInt32                        TotalSize;                     /** Total Size of DataSetMessage.  Initialize to 0. */
    OpcUa_ByteString                    ChunkBuffer;                   /** Allocated by caller for maximum possible DataSetMessage. */
    OpcUa_ByteString                    ShadowBits;                    /** For duplicate detection. Need 1/8 the size of ChunkBuffer. */
} OpcUa_UadpChunkData;


typedef struct _OpcUa_UadpNetworkMessage
{
    OpcUa_UadpNetworkMessageHeader      NetworkMessageHeader;          /** Defines the content of the UadpNetworkMessage */
    OpcUa_UadpGroupHeader               GroupHeader;                   /** Describes the content of the WriterGroup */
    OpcUa_UadpSecurityHeader            SecurityHeader;                /** Describes the security configuration to use */
    OpcUa_Byte                          MessageCount;                  /** Number of DataSetMessages contained in the NetworkMessage. */
    OpcUa_UadpDataSetMessage*           Payload;                       /** The payload of the NetworkMessage. */
    OpcUa_UadpChunkData                 ChunkData;                     /** Used for Chunking */
} OpcUa_UadpNetworkMessage;


/**
  @brief Converts a UADP Network Message (Header part only) to Binary.
         This part is usually unencrypted.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer start on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessageHeader_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

/**
  @brief Converts a UADP Network Message (Payload part only) to Binary.
         This part is usually encrypted.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessageBody_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

/**
  @brief Converts a UADP Network Message to Binary.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessage_WriteBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

/**
  @brief Converts a UADP Network Message (Header part only) from Binary.
         This part is usually unencrypted.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer start on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessageHeader_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

/**
  @brief Converts a UADP Network Message (Payload part only) from Binary.
         This part is usually encrypted.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer start on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessageBody_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

/**
  @brief Converts a UADP Network Message from Binary.

  @param a_pValue     [in/out] The Network Message metadata.
  @param a_pBuffer    [in/out] Points to the buffer start on input, to end of message on output.
  @param a_pSize      [in/out] Number of bytes available buffer space.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_UadpMessage_ReadBinary(OpcUa_UadpNetworkMessage* a_pValue, OpcUa_Byte **a_pBuffer, OpcUa_UInt32 *a_pSize);

#endif /* OPCUA_HAVE_PUBSUB */

#endif /* OpcUa_Uadp_Networkmessage_H_ */
