/* ========================================================================
 * Copyright (c) 2005-2021 The OPC Foundation, Inc. All rights reserved.
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

/* base */
#include <opcua_platformdefs.h>
#include <opcua_statuscodes.h>
#include <opcua_errorhandling.h>
#include <opcua_trace.h>

/* core */
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_guid.h>

/* stack */
#include <opcua_encoder.h>
#include <opcua_decoder.h>

/* types */
#include <opcua_identifiers.h>

/* self */
#include <opcua_types.h>

#ifndef OPCUA_EXCLUDE_IdType
/*============================================================================
 * OpcUa_IdType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_IdType_EnumeratedValues[] =
{
    { "Numeric", 0 },
    { "String", 1 },
    { "Guid", 2 },
    { "Opaque", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_IdType_EnumeratedType =
{
    "IdType",
    g_OpcUa_IdType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_NodeClass
/*============================================================================
 * OpcUa_NodeClass_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_NodeClass_EnumeratedValues[] =
{
    { "Unspecified", 0 },
    { "Object", 1 },
    { "Variable", 2 },
    { "Method", 4 },
    { "ObjectType", 8 },
    { "VariableType", 16 },
    { "ReferenceType", 32 },
    { "DataType", 64 },
    { "View", 128 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_NodeClass_EnumeratedType =
{
    "NodeClass",
    g_OpcUa_NodeClass_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_PermissionType
/*============================================================================
 * OpcUa_PermissionType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_PermissionType_EnumeratedValues[] =
{
    { "None", 0 },
    { "Browse", 1 },
    { "ReadRolePermissions", 2 },
    { "WriteAttribute", 4 },
    { "WriteRolePermissions", 8 },
    { "WriteHistorizing", 16 },
    { "Read", 32 },
    { "Write", 64 },
    { "ReadHistory", 128 },
    { "InsertHistory", 256 },
    { "ModifyHistory", 512 },
    { "DeleteHistory", 1024 },
    { "ReceiveEvents", 2048 },
    { "Call", 4096 },
    { "AddReference", 8192 },
    { "RemoveReference", 16384 },
    { "DeleteNode", 32768 },
    { "AddNode", 65536 },
    { "All", 131071 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_PermissionType_EnumeratedType =
{
    "PermissionType",
    g_OpcUa_PermissionType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_AccessLevelType
/*============================================================================
 * OpcUa_AccessLevelType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_AccessLevelType_EnumeratedValues[] =
{
    { "None", 0 },
    { "CurrentRead", 1 },
    { "CurrentWrite", 2 },
    { "HistoryRead", 4 },
    { "HistoryWrite", 16 },
    { "StatusWrite", 32 },
    { "TimestampWrite", 64 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_AccessLevelType_EnumeratedType =
{
    "AccessLevelType",
    g_OpcUa_AccessLevelType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_AccessLevelExType
/*============================================================================
 * OpcUa_AccessLevelExType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_AccessLevelExType_EnumeratedValues[] =
{
    { "None", 0 },
    { "CurrentRead", 1 },
    { "CurrentWrite", 2 },
    { "HistoryRead", 4 },
    { "HistoryWrite", 16 },
    { "StatusWrite", 32 },
    { "TimestampWrite", 64 },
    { "NonatomicRead", 65536 },
    { "NonatomicWrite", 131072 },
    { "WriteFullArrayOnly", 262144 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_AccessLevelExType_EnumeratedType =
{
    "AccessLevelExType",
    g_OpcUa_AccessLevelExType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_EventNotifierType
/*============================================================================
 * OpcUa_EventNotifierType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_EventNotifierType_EnumeratedValues[] =
{
    { "None", 0 },
    { "SubscribeToEvents", 1 },
    { "HistoryRead", 4 },
    { "HistoryWrite", 8 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_EventNotifierType_EnumeratedType =
{
    "EventNotifierType",
    g_OpcUa_EventNotifierType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_AccessRestrictionType
/*============================================================================
 * OpcUa_AccessRestrictionType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_AccessRestrictionType_EnumeratedValues[] =
{
    { "None", 0 },
    { "SigningRequired", 1 },
    { "EncryptionRequired", 2 },
    { "SessionRequired ", 4 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_AccessRestrictionType_EnumeratedType =
{
    "AccessRestrictionType",
    g_OpcUa_AccessRestrictionType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_RolePermissionType
/*============================================================================
 * OpcUa_RolePermissionType_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RolePermissionType_Initialize(OpcUa_RolePermissionType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, RoleId);
        OpcUa_Field_Initialize(UInt32, Permissions);
    }
}

/*============================================================================
 * OpcUa_RolePermissionType_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RolePermissionType_Clear(OpcUa_RolePermissionType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, RoleId);
        OpcUa_Field_Clear(UInt32, Permissions);
    }
}

/*============================================================================
 * OpcUa_RolePermissionType_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RolePermissionType_GetSize(OpcUa_RolePermissionType* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RolePermissionType_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, RoleId);
    OpcUa_Field_GetSize(UInt32, Permissions);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RolePermissionType_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RolePermissionType_Encode(OpcUa_RolePermissionType* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RolePermissionType_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, RoleId);
    OpcUa_Field_Write(UInt32, Permissions);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RolePermissionType_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RolePermissionType_Decode(OpcUa_RolePermissionType* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RolePermissionType_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RolePermissionType_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, RoleId);
    OpcUa_Field_Read(UInt32, Permissions);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RolePermissionType_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RolePermissionType_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RolePermissionType_EncodeableType =
{
    "RolePermissionType",
    OpcUaId_RolePermissionType,
    OpcUaId_RolePermissionType_Encoding_DefaultBinary,
    OpcUaId_RolePermissionType_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RolePermissionType),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RolePermissionType_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RolePermissionType_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RolePermissionType_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RolePermissionType_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RolePermissionType_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_StructureType
/*============================================================================
 * OpcUa_StructureType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_StructureType_EnumeratedValues[] =
{
    { "Structure", 0 },
    { "StructureWithOptionalFields", 1 },
    { "Union", 2 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_StructureType_EnumeratedType =
{
    "StructureType",
    g_OpcUa_StructureType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_StructureField
/*============================================================================
 * OpcUa_StructureField_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_StructureField_Initialize(OpcUa_StructureField* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, Name);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(UInt32, MaxStringLength);
        OpcUa_Field_Initialize(Boolean, IsOptional);
    }
}

/*============================================================================
 * OpcUa_StructureField_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_StructureField_Clear(OpcUa_StructureField* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, Name);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(UInt32, MaxStringLength);
        OpcUa_Field_Clear(Boolean, IsOptional);
    }
}

/*============================================================================
 * OpcUa_StructureField_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureField_GetSize(OpcUa_StructureField* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureField_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, Name);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(UInt32, MaxStringLength);
    OpcUa_Field_GetSize(Boolean, IsOptional);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureField_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureField_Encode(OpcUa_StructureField* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureField_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, Name);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(UInt32, MaxStringLength);
    OpcUa_Field_Write(Boolean, IsOptional);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureField_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureField_Decode(OpcUa_StructureField* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureField_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_StructureField_Initialize(a_pValue);

    OpcUa_Field_Read(String, Name);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(UInt32, MaxStringLength);
    OpcUa_Field_Read(Boolean, IsOptional);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_StructureField_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureField_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_StructureField_EncodeableType =
{
    "StructureField",
    OpcUaId_StructureField,
    OpcUaId_StructureField_Encoding_DefaultBinary,
    OpcUaId_StructureField_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_StructureField),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_StructureField_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_StructureField_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_StructureField_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_StructureField_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_StructureField_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_StructureDefinition
/*============================================================================
 * OpcUa_StructureDefinition_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_StructureDefinition_Initialize(OpcUa_StructureDefinition* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, DefaultEncodingId);
        OpcUa_Field_Initialize(NodeId, BaseDataType);
        OpcUa_Field_InitializeEnumerated(OpcUa_StructureType, StructureType);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_StructureField, Fields);
    }
}

/*============================================================================
 * OpcUa_StructureDefinition_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_StructureDefinition_Clear(OpcUa_StructureDefinition* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, DefaultEncodingId);
        OpcUa_Field_Clear(NodeId, BaseDataType);
        OpcUa_Field_ClearEnumerated(OpcUa_StructureType, StructureType);
        OpcUa_Field_ClearEncodeableArray(OpcUa_StructureField, Fields);
    }
}

/*============================================================================
 * OpcUa_StructureDefinition_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureDefinition_GetSize(OpcUa_StructureDefinition* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureDefinition_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, DefaultEncodingId);
    OpcUa_Field_GetSize(NodeId, BaseDataType);
    OpcUa_Field_GetSizeEnumerated(OpcUa_StructureType, StructureType);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_StructureField, Fields);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureDefinition_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureDefinition_Encode(OpcUa_StructureDefinition* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureDefinition_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, DefaultEncodingId);
    OpcUa_Field_Write(NodeId, BaseDataType);
    OpcUa_Field_WriteEnumerated(OpcUa_StructureType, StructureType);
    OpcUa_Field_WriteEncodeableArray(OpcUa_StructureField, Fields);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureDefinition_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_StructureDefinition_Decode(OpcUa_StructureDefinition* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "StructureDefinition_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_StructureDefinition_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, DefaultEncodingId);
    OpcUa_Field_Read(NodeId, BaseDataType);
    OpcUa_Field_ReadEnumerated(OpcUa_StructureType, StructureType);
    OpcUa_Field_ReadEncodeableArray(OpcUa_StructureField, Fields);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_StructureDefinition_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_StructureDefinition_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_StructureDefinition_EncodeableType =
{
    "StructureDefinition",
    OpcUaId_StructureDefinition,
    OpcUaId_StructureDefinition_Encoding_DefaultBinary,
    OpcUaId_StructureDefinition_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_StructureDefinition),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_StructureDefinition_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_StructureDefinition_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_StructureDefinition_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_StructureDefinition_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_StructureDefinition_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EnumDefinition
/*============================================================================
 * OpcUa_EnumDefinition_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EnumDefinition_Initialize(OpcUa_EnumDefinition* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeableArray(OpcUa_EnumField, Fields);
    }
}

/*============================================================================
 * OpcUa_EnumDefinition_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EnumDefinition_Clear(OpcUa_EnumDefinition* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeableArray(OpcUa_EnumField, Fields);
    }
}

/*============================================================================
 * OpcUa_EnumDefinition_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumDefinition_GetSize(OpcUa_EnumDefinition* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumDefinition_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeableArray(OpcUa_EnumField, Fields);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumDefinition_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumDefinition_Encode(OpcUa_EnumDefinition* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumDefinition_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeableArray(OpcUa_EnumField, Fields);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumDefinition_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumDefinition_Decode(OpcUa_EnumDefinition* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumDefinition_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EnumDefinition_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeableArray(OpcUa_EnumField, Fields);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EnumDefinition_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumDefinition_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EnumDefinition_EncodeableType =
{
    "EnumDefinition",
    OpcUaId_EnumDefinition,
    OpcUaId_EnumDefinition_Encoding_DefaultBinary,
    OpcUaId_EnumDefinition_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EnumDefinition),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EnumDefinition_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EnumDefinition_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EnumDefinition_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EnumDefinition_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EnumDefinition_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_Node
/*============================================================================
 * OpcUa_Node_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_Node_Initialize(OpcUa_Node* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_Node_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_Node_Clear(OpcUa_Node* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_Node_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Node_GetSize(OpcUa_Node* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Node_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Node_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Node_Encode(OpcUa_Node* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Node_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Node_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Node_Decode(OpcUa_Node* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Node_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_Node_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_Node_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Node_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_Node_EncodeableType =
{
    "Node",
    OpcUaId_Node,
    OpcUaId_Node_Encoding_DefaultBinary,
    OpcUaId_Node_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_Node),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_Node_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_Node_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_Node_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_Node_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_Node_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_InstanceNode
/*============================================================================
 * OpcUa_InstanceNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_InstanceNode_Initialize(OpcUa_InstanceNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_InstanceNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_InstanceNode_Clear(OpcUa_InstanceNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_InstanceNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_InstanceNode_GetSize(OpcUa_InstanceNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "InstanceNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_InstanceNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_InstanceNode_Encode(OpcUa_InstanceNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "InstanceNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_InstanceNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_InstanceNode_Decode(OpcUa_InstanceNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "InstanceNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_InstanceNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_InstanceNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_InstanceNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_InstanceNode_EncodeableType =
{
    "InstanceNode",
    OpcUaId_InstanceNode,
    OpcUaId_InstanceNode_Encoding_DefaultBinary,
    OpcUaId_InstanceNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_InstanceNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_InstanceNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_InstanceNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_InstanceNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_InstanceNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_InstanceNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_TypeNode
/*============================================================================
 * OpcUa_TypeNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_TypeNode_Initialize(OpcUa_TypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_TypeNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_TypeNode_Clear(OpcUa_TypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
    }
}

/*============================================================================
 * OpcUa_TypeNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TypeNode_GetSize(OpcUa_TypeNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TypeNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TypeNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TypeNode_Encode(OpcUa_TypeNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TypeNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TypeNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TypeNode_Decode(OpcUa_TypeNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TypeNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_TypeNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_TypeNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TypeNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_TypeNode_EncodeableType =
{
    "TypeNode",
    OpcUaId_TypeNode,
    OpcUaId_TypeNode_Encoding_DefaultBinary,
    OpcUaId_TypeNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_TypeNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_TypeNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_TypeNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_TypeNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_TypeNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_TypeNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ObjectNode
/*============================================================================
 * OpcUa_ObjectNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectNode_Initialize(OpcUa_ObjectNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ObjectNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectNode_Clear(OpcUa_ObjectNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ObjectNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectNode_GetSize(OpcUa_ObjectNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Byte, EventNotifier);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectNode_Encode(OpcUa_ObjectNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectNode_Decode(OpcUa_ObjectNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ObjectNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ObjectNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ObjectNode_EncodeableType =
{
    "ObjectNode",
    OpcUaId_ObjectNode,
    OpcUaId_ObjectNode_Encoding_DefaultBinary,
    OpcUaId_ObjectNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ObjectNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ObjectNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ObjectNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ObjectNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ObjectNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ObjectNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ObjectTypeNode
/*============================================================================
 * OpcUa_ObjectTypeNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectTypeNode_Initialize(OpcUa_ObjectTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_ObjectTypeNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectTypeNode_Clear(OpcUa_ObjectTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_ObjectTypeNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeNode_GetSize(OpcUa_ObjectTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Boolean, IsAbstract);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeNode_Encode(OpcUa_ObjectTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeNode_Decode(OpcUa_ObjectTypeNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ObjectTypeNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ObjectTypeNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ObjectTypeNode_EncodeableType =
{
    "ObjectTypeNode",
    OpcUaId_ObjectTypeNode,
    OpcUaId_ObjectTypeNode_Encoding_DefaultBinary,
    OpcUaId_ObjectTypeNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ObjectTypeNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ObjectTypeNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ObjectTypeNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ObjectTypeNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ObjectTypeNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ObjectTypeNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_VariableNode
/*============================================================================
 * OpcUa_VariableNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_VariableNode_Initialize(OpcUa_VariableNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Variant, Value);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(Byte, AccessLevel);
        OpcUa_Field_Initialize(Byte, UserAccessLevel);
        OpcUa_Field_Initialize(Double, MinimumSamplingInterval);
        OpcUa_Field_Initialize(Boolean, Historizing);
        OpcUa_Field_Initialize(UInt32, AccessLevelEx);
    }
}

/*============================================================================
 * OpcUa_VariableNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_VariableNode_Clear(OpcUa_VariableNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Variant, Value);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(Byte, AccessLevel);
        OpcUa_Field_Clear(Byte, UserAccessLevel);
        OpcUa_Field_Clear(Double, MinimumSamplingInterval);
        OpcUa_Field_Clear(Boolean, Historizing);
        OpcUa_Field_Clear(UInt32, AccessLevelEx);
    }
}

/*============================================================================
 * OpcUa_VariableNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableNode_GetSize(OpcUa_VariableNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Variant, Value);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(Byte, AccessLevel);
    OpcUa_Field_GetSize(Byte, UserAccessLevel);
    OpcUa_Field_GetSize(Double, MinimumSamplingInterval);
    OpcUa_Field_GetSize(Boolean, Historizing);
    OpcUa_Field_GetSize(UInt32, AccessLevelEx);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableNode_Encode(OpcUa_VariableNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Variant, Value);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(Byte, AccessLevel);
    OpcUa_Field_Write(Byte, UserAccessLevel);
    OpcUa_Field_Write(Double, MinimumSamplingInterval);
    OpcUa_Field_Write(Boolean, Historizing);
    OpcUa_Field_Write(UInt32, AccessLevelEx);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableNode_Decode(OpcUa_VariableNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_VariableNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Variant, Value);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(Byte, AccessLevel);
    OpcUa_Field_Read(Byte, UserAccessLevel);
    OpcUa_Field_Read(Double, MinimumSamplingInterval);
    OpcUa_Field_Read(Boolean, Historizing);
    OpcUa_Field_Read(UInt32, AccessLevelEx);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_VariableNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_VariableNode_EncodeableType =
{
    "VariableNode",
    OpcUaId_VariableNode,
    OpcUaId_VariableNode_Encoding_DefaultBinary,
    OpcUaId_VariableNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_VariableNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_VariableNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_VariableNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_VariableNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_VariableNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_VariableNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_VariableTypeNode
/*============================================================================
 * OpcUa_VariableTypeNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_VariableTypeNode_Initialize(OpcUa_VariableTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Variant, Value);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_VariableTypeNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_VariableTypeNode_Clear(OpcUa_VariableTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Variant, Value);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_VariableTypeNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeNode_GetSize(OpcUa_VariableTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Variant, Value);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(Boolean, IsAbstract);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeNode_Encode(OpcUa_VariableTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Variant, Value);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeNode_Decode(OpcUa_VariableTypeNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_VariableTypeNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Variant, Value);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_VariableTypeNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_VariableTypeNode_EncodeableType =
{
    "VariableTypeNode",
    OpcUaId_VariableTypeNode,
    OpcUaId_VariableTypeNode_Encoding_DefaultBinary,
    OpcUaId_VariableTypeNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_VariableTypeNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_VariableTypeNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_VariableTypeNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_VariableTypeNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_VariableTypeNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_VariableTypeNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReferenceTypeNode
/*============================================================================
 * OpcUa_ReferenceTypeNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceTypeNode_Initialize(OpcUa_ReferenceTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
        OpcUa_Field_Initialize(Boolean, Symmetric);
        OpcUa_Field_Initialize(LocalizedText, InverseName);
    }
}

/*============================================================================
 * OpcUa_ReferenceTypeNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceTypeNode_Clear(OpcUa_ReferenceTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Boolean, IsAbstract);
        OpcUa_Field_Clear(Boolean, Symmetric);
        OpcUa_Field_Clear(LocalizedText, InverseName);
    }
}

/*============================================================================
 * OpcUa_ReferenceTypeNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeNode_GetSize(OpcUa_ReferenceTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Boolean, IsAbstract);
    OpcUa_Field_GetSize(Boolean, Symmetric);
    OpcUa_Field_GetSize(LocalizedText, InverseName);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeNode_Encode(OpcUa_ReferenceTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Boolean, IsAbstract);
    OpcUa_Field_Write(Boolean, Symmetric);
    OpcUa_Field_Write(LocalizedText, InverseName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeNode_Decode(OpcUa_ReferenceTypeNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReferenceTypeNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Boolean, IsAbstract);
    OpcUa_Field_Read(Boolean, Symmetric);
    OpcUa_Field_Read(LocalizedText, InverseName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReferenceTypeNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReferenceTypeNode_EncodeableType =
{
    "ReferenceTypeNode",
    OpcUaId_ReferenceTypeNode,
    OpcUaId_ReferenceTypeNode_Encoding_DefaultBinary,
    OpcUaId_ReferenceTypeNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReferenceTypeNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReferenceTypeNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReferenceTypeNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReferenceTypeNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReferenceTypeNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReferenceTypeNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_MethodNode
/*============================================================================
 * OpcUa_MethodNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_MethodNode_Initialize(OpcUa_MethodNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Boolean, Executable);
        OpcUa_Field_Initialize(Boolean, UserExecutable);
    }
}

/*============================================================================
 * OpcUa_MethodNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_MethodNode_Clear(OpcUa_MethodNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Boolean, Executable);
        OpcUa_Field_Clear(Boolean, UserExecutable);
    }
}

/*============================================================================
 * OpcUa_MethodNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodNode_GetSize(OpcUa_MethodNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Boolean, Executable);
    OpcUa_Field_GetSize(Boolean, UserExecutable);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodNode_Encode(OpcUa_MethodNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Boolean, Executable);
    OpcUa_Field_Write(Boolean, UserExecutable);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodNode_Decode(OpcUa_MethodNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_MethodNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Boolean, Executable);
    OpcUa_Field_Read(Boolean, UserExecutable);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_MethodNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_MethodNode_EncodeableType =
{
    "MethodNode",
    OpcUaId_MethodNode,
    OpcUaId_MethodNode_Encoding_DefaultBinary,
    OpcUaId_MethodNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_MethodNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_MethodNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_MethodNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_MethodNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_MethodNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_MethodNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ViewNode
/*============================================================================
 * OpcUa_ViewNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ViewNode_Initialize(OpcUa_ViewNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Boolean, ContainsNoLoops);
        OpcUa_Field_Initialize(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ViewNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ViewNode_Clear(OpcUa_ViewNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Boolean, ContainsNoLoops);
        OpcUa_Field_Clear(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ViewNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewNode_GetSize(OpcUa_ViewNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Boolean, ContainsNoLoops);
    OpcUa_Field_GetSize(Byte, EventNotifier);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewNode_Encode(OpcUa_ViewNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Boolean, ContainsNoLoops);
    OpcUa_Field_Write(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewNode_Decode(OpcUa_ViewNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ViewNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Boolean, ContainsNoLoops);
    OpcUa_Field_Read(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ViewNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ViewNode_EncodeableType =
{
    "ViewNode",
    OpcUaId_ViewNode,
    OpcUaId_ViewNode_Encoding_DefaultBinary,
    OpcUaId_ViewNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ViewNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ViewNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ViewNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ViewNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ViewNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ViewNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_DataTypeNode
/*============================================================================
 * OpcUa_DataTypeNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_DataTypeNode_Initialize(OpcUa_DataTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_InitializeEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Initialize(QualifiedName, BrowseName);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Initialize(UInt16, AccessRestrictions);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
        OpcUa_Field_Initialize(ExtensionObject, DataTypeDefinition);
    }
}

/*============================================================================
 * OpcUa_DataTypeNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_DataTypeNode_Clear(OpcUa_DataTypeNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_ClearEnumerated(OpcUa_NodeClass, NodeClass);
        OpcUa_Field_Clear(QualifiedName, BrowseName);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
        OpcUa_Field_Clear(UInt16, AccessRestrictions);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ReferenceNode, References);
        OpcUa_Field_Clear(Boolean, IsAbstract);
        OpcUa_Field_Clear(ExtensionObject, DataTypeDefinition);
    }
}

/*============================================================================
 * OpcUa_DataTypeNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeNode_GetSize(OpcUa_DataTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_GetSize(QualifiedName, BrowseName);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_GetSize(UInt16, AccessRestrictions);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_GetSize(Boolean, IsAbstract);
    OpcUa_Field_GetSize(ExtensionObject, DataTypeDefinition);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeNode_Encode(OpcUa_DataTypeNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_WriteEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Write(QualifiedName, BrowseName);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Write(UInt16, AccessRestrictions);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Write(Boolean, IsAbstract);
    OpcUa_Field_Write(ExtensionObject, DataTypeDefinition);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeNode_Decode(OpcUa_DataTypeNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_DataTypeNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_ReadEnumerated(OpcUa_NodeClass, NodeClass);
    OpcUa_Field_Read(QualifiedName, BrowseName);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, RolePermissions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_RolePermissionType, UserRolePermissions);
    OpcUa_Field_Read(UInt16, AccessRestrictions);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ReferenceNode, References);
    OpcUa_Field_Read(Boolean, IsAbstract);
    OpcUa_Field_Read(ExtensionObject, DataTypeDefinition);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_DataTypeNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_DataTypeNode_EncodeableType =
{
    "DataTypeNode",
    OpcUaId_DataTypeNode,
    OpcUaId_DataTypeNode_Encoding_DefaultBinary,
    OpcUaId_DataTypeNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_DataTypeNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_DataTypeNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_DataTypeNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_DataTypeNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_DataTypeNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_DataTypeNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReferenceNode
/*============================================================================
 * OpcUa_ReferenceNode_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceNode_Initialize(OpcUa_ReferenceNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, ReferenceTypeId);
        OpcUa_Field_Initialize(Boolean, IsInverse);
        OpcUa_Field_Initialize(ExpandedNodeId, TargetId);
    }
}

/*============================================================================
 * OpcUa_ReferenceNode_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceNode_Clear(OpcUa_ReferenceNode* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, ReferenceTypeId);
        OpcUa_Field_Clear(Boolean, IsInverse);
        OpcUa_Field_Clear(ExpandedNodeId, TargetId);
    }
}

/*============================================================================
 * OpcUa_ReferenceNode_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceNode_GetSize(OpcUa_ReferenceNode* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceNode_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, ReferenceTypeId);
    OpcUa_Field_GetSize(Boolean, IsInverse);
    OpcUa_Field_GetSize(ExpandedNodeId, TargetId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceNode_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceNode_Encode(OpcUa_ReferenceNode* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceNode_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, ReferenceTypeId);
    OpcUa_Field_Write(Boolean, IsInverse);
    OpcUa_Field_Write(ExpandedNodeId, TargetId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceNode_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceNode_Decode(OpcUa_ReferenceNode* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceNode_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReferenceNode_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, ReferenceTypeId);
    OpcUa_Field_Read(Boolean, IsInverse);
    OpcUa_Field_Read(ExpandedNodeId, TargetId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReferenceNode_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceNode_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReferenceNode_EncodeableType =
{
    "ReferenceNode",
    OpcUaId_ReferenceNode,
    OpcUaId_ReferenceNode_Encoding_DefaultBinary,
    OpcUaId_ReferenceNode_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReferenceNode),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReferenceNode_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReferenceNode_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReferenceNode_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReferenceNode_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReferenceNode_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_Argument
/*============================================================================
 * OpcUa_Argument_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_Argument_Initialize(OpcUa_Argument* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, Name);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(LocalizedText, Description);
    }
}

/*============================================================================
 * OpcUa_Argument_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_Argument_Clear(OpcUa_Argument* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, Name);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(LocalizedText, Description);
    }
}

/*============================================================================
 * OpcUa_Argument_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Argument_GetSize(OpcUa_Argument* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Argument_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, Name);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(LocalizedText, Description);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Argument_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Argument_Encode(OpcUa_Argument* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Argument_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, Name);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(LocalizedText, Description);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Argument_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Argument_Decode(OpcUa_Argument* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "Argument_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_Argument_Initialize(a_pValue);

    OpcUa_Field_Read(String, Name);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(LocalizedText, Description);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_Argument_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_Argument_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_Argument_EncodeableType =
{
    "Argument",
    OpcUaId_Argument,
    OpcUaId_Argument_Encoding_DefaultBinary,
    OpcUaId_Argument_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_Argument),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_Argument_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_Argument_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_Argument_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_Argument_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_Argument_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EnumValueType
/*============================================================================
 * OpcUa_EnumValueType_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EnumValueType_Initialize(OpcUa_EnumValueType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Int64, Value);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
    }
}

/*============================================================================
 * OpcUa_EnumValueType_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EnumValueType_Clear(OpcUa_EnumValueType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Int64, Value);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
    }
}

/*============================================================================
 * OpcUa_EnumValueType_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumValueType_GetSize(OpcUa_EnumValueType* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumValueType_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Int64, Value);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumValueType_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumValueType_Encode(OpcUa_EnumValueType* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumValueType_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Int64, Value);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumValueType_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumValueType_Decode(OpcUa_EnumValueType* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumValueType_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EnumValueType_Initialize(a_pValue);

    OpcUa_Field_Read(Int64, Value);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EnumValueType_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumValueType_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EnumValueType_EncodeableType =
{
    "EnumValueType",
    OpcUaId_EnumValueType,
    OpcUaId_EnumValueType_Encoding_DefaultBinary,
    OpcUaId_EnumValueType_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EnumValueType),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EnumValueType_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EnumValueType_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EnumValueType_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EnumValueType_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EnumValueType_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EnumField
/*============================================================================
 * OpcUa_EnumField_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EnumField_Initialize(OpcUa_EnumField* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Int64, Value);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(String, Name);
    }
}

/*============================================================================
 * OpcUa_EnumField_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EnumField_Clear(OpcUa_EnumField* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Int64, Value);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(String, Name);
    }
}

/*============================================================================
 * OpcUa_EnumField_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumField_GetSize(OpcUa_EnumField* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumField_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Int64, Value);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(String, Name);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumField_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumField_Encode(OpcUa_EnumField* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumField_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Int64, Value);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(String, Name);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumField_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumField_Decode(OpcUa_EnumField* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumField_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EnumField_Initialize(a_pValue);

    OpcUa_Field_Read(Int64, Value);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(String, Name);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EnumField_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumField_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EnumField_EncodeableType =
{
    "EnumField",
    OpcUaId_EnumField,
    OpcUaId_EnumField_Encoding_DefaultBinary,
    OpcUaId_EnumField_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EnumField),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EnumField_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EnumField_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EnumField_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EnumField_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EnumField_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_OptionSet
/*============================================================================
 * OpcUa_OptionSet_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_OptionSet_Initialize(OpcUa_OptionSet* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(ByteString, Value);
        OpcUa_Field_Initialize(ByteString, ValidBits);
    }
}

/*============================================================================
 * OpcUa_OptionSet_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_OptionSet_Clear(OpcUa_OptionSet* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(ByteString, Value);
        OpcUa_Field_Clear(ByteString, ValidBits);
    }
}

/*============================================================================
 * OpcUa_OptionSet_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OptionSet_GetSize(OpcUa_OptionSet* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OptionSet_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(ByteString, Value);
    OpcUa_Field_GetSize(ByteString, ValidBits);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OptionSet_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OptionSet_Encode(OpcUa_OptionSet* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OptionSet_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(ByteString, Value);
    OpcUa_Field_Write(ByteString, ValidBits);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OptionSet_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OptionSet_Decode(OpcUa_OptionSet* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OptionSet_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_OptionSet_Initialize(a_pValue);

    OpcUa_Field_Read(ByteString, Value);
    OpcUa_Field_Read(ByteString, ValidBits);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_OptionSet_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OptionSet_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_OptionSet_EncodeableType =
{
    "OptionSet",
    OpcUaId_OptionSet,
    OpcUaId_OptionSet_Encoding_DefaultBinary,
    OpcUaId_OptionSet_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_OptionSet),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_OptionSet_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_OptionSet_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_OptionSet_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_OptionSet_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_OptionSet_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_TimeZoneDataType
/*============================================================================
 * OpcUa_TimeZoneDataType_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_TimeZoneDataType_Initialize(OpcUa_TimeZoneDataType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Int16, Offset);
        OpcUa_Field_Initialize(Boolean, DaylightSavingInOffset);
    }
}

/*============================================================================
 * OpcUa_TimeZoneDataType_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_TimeZoneDataType_Clear(OpcUa_TimeZoneDataType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Int16, Offset);
        OpcUa_Field_Clear(Boolean, DaylightSavingInOffset);
    }
}

/*============================================================================
 * OpcUa_TimeZoneDataType_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TimeZoneDataType_GetSize(OpcUa_TimeZoneDataType* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TimeZoneDataType_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Int16, Offset);
    OpcUa_Field_GetSize(Boolean, DaylightSavingInOffset);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TimeZoneDataType_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TimeZoneDataType_Encode(OpcUa_TimeZoneDataType* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TimeZoneDataType_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Int16, Offset);
    OpcUa_Field_Write(Boolean, DaylightSavingInOffset);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TimeZoneDataType_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_TimeZoneDataType_Decode(OpcUa_TimeZoneDataType* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "TimeZoneDataType_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_TimeZoneDataType_Initialize(a_pValue);

    OpcUa_Field_Read(Int16, Offset);
    OpcUa_Field_Read(Boolean, DaylightSavingInOffset);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_TimeZoneDataType_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_TimeZoneDataType_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_TimeZoneDataType_EncodeableType =
{
    "TimeZoneDataType",
    OpcUaId_TimeZoneDataType,
    OpcUaId_TimeZoneDataType_Encoding_DefaultBinary,
    OpcUaId_TimeZoneDataType_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_TimeZoneDataType),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_TimeZoneDataType_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_TimeZoneDataType_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_TimeZoneDataType_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_TimeZoneDataType_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_TimeZoneDataType_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ApplicationType
/*============================================================================
 * OpcUa_ApplicationType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_ApplicationType_EnumeratedValues[] =
{
    { "Server", 0 },
    { "Client", 1 },
    { "ClientAndServer", 2 },
    { "DiscoveryServer", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_ApplicationType_EnumeratedType =
{
    "ApplicationType",
    g_OpcUa_ApplicationType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ApplicationDescription
/*============================================================================
 * OpcUa_ApplicationDescription_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ApplicationDescription_Initialize(OpcUa_ApplicationDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, ApplicationUri);
        OpcUa_Field_Initialize(String, ProductUri);
        OpcUa_Field_Initialize(LocalizedText, ApplicationName);
        OpcUa_Field_InitializeEnumerated(OpcUa_ApplicationType, ApplicationType);
        OpcUa_Field_Initialize(String, GatewayServerUri);
        OpcUa_Field_Initialize(String, DiscoveryProfileUri);
        OpcUa_Field_InitializeArray(String, DiscoveryUrls);
    }
}

/*============================================================================
 * OpcUa_ApplicationDescription_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ApplicationDescription_Clear(OpcUa_ApplicationDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, ApplicationUri);
        OpcUa_Field_Clear(String, ProductUri);
        OpcUa_Field_Clear(LocalizedText, ApplicationName);
        OpcUa_Field_ClearEnumerated(OpcUa_ApplicationType, ApplicationType);
        OpcUa_Field_Clear(String, GatewayServerUri);
        OpcUa_Field_Clear(String, DiscoveryProfileUri);
        OpcUa_Field_ClearArray(String, DiscoveryUrls);
    }
}

/*============================================================================
 * OpcUa_ApplicationDescription_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_GetSize(OpcUa_ApplicationDescription* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, ApplicationUri);
    OpcUa_Field_GetSize(String, ProductUri);
    OpcUa_Field_GetSize(LocalizedText, ApplicationName);
    OpcUa_Field_GetSizeEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_GetSize(String, GatewayServerUri);
    OpcUa_Field_GetSize(String, DiscoveryProfileUri);
    OpcUa_Field_GetSizeArray(String, DiscoveryUrls);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_Encode(OpcUa_ApplicationDescription* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, ApplicationUri);
    OpcUa_Field_Write(String, ProductUri);
    OpcUa_Field_Write(LocalizedText, ApplicationName);
    OpcUa_Field_WriteEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_Write(String, GatewayServerUri);
    OpcUa_Field_Write(String, DiscoveryProfileUri);
    OpcUa_Field_WriteArray(String, DiscoveryUrls);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_Decode(OpcUa_ApplicationDescription* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ApplicationDescription_Initialize(a_pValue);

    OpcUa_Field_Read(String, ApplicationUri);
    OpcUa_Field_Read(String, ProductUri);
    OpcUa_Field_Read(LocalizedText, ApplicationName);
    OpcUa_Field_ReadEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_Read(String, GatewayServerUri);
    OpcUa_Field_Read(String, DiscoveryProfileUri);
    OpcUa_Field_ReadArray(String, DiscoveryUrls);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ApplicationDescription_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ApplicationDescription_EncodeableType =
{
    "ApplicationDescription",
    OpcUaId_ApplicationDescription,
    OpcUaId_ApplicationDescription_Encoding_DefaultBinary,
    OpcUaId_ApplicationDescription_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ApplicationDescription),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ApplicationDescription_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ApplicationDescription_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ApplicationDescription_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ApplicationDescription_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ApplicationDescription_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RequestHeader
/*============================================================================
 * OpcUa_RequestHeader_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RequestHeader_Initialize(OpcUa_RequestHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, AuthenticationToken);
        OpcUa_Field_Initialize(DateTime, Timestamp);
        OpcUa_Field_Initialize(UInt32, RequestHandle);
        OpcUa_Field_Initialize(UInt32, ReturnDiagnostics);
        OpcUa_Field_Initialize(String, AuditEntryId);
        OpcUa_Field_Initialize(UInt32, TimeoutHint);
        OpcUa_Field_Initialize(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_RequestHeader_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RequestHeader_Clear(OpcUa_RequestHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, AuthenticationToken);
        OpcUa_Field_Clear(DateTime, Timestamp);
        OpcUa_Field_Clear(UInt32, RequestHandle);
        OpcUa_Field_Clear(UInt32, ReturnDiagnostics);
        OpcUa_Field_Clear(String, AuditEntryId);
        OpcUa_Field_Clear(UInt32, TimeoutHint);
        OpcUa_Field_Clear(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_RequestHeader_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_GetSize(OpcUa_RequestHeader* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, AuthenticationToken);
    OpcUa_Field_GetSize(DateTime, Timestamp);
    OpcUa_Field_GetSize(UInt32, RequestHandle);
    OpcUa_Field_GetSize(UInt32, ReturnDiagnostics);
    OpcUa_Field_GetSize(String, AuditEntryId);
    OpcUa_Field_GetSize(UInt32, TimeoutHint);
    OpcUa_Field_GetSize(ExtensionObject, AdditionalHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_Encode(OpcUa_RequestHeader* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, AuthenticationToken);
    OpcUa_Field_Write(DateTime, Timestamp);
    OpcUa_Field_Write(UInt32, RequestHandle);
    OpcUa_Field_Write(UInt32, ReturnDiagnostics);
    OpcUa_Field_Write(String, AuditEntryId);
    OpcUa_Field_Write(UInt32, TimeoutHint);
    OpcUa_Field_Write(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_Decode(OpcUa_RequestHeader* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RequestHeader_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, AuthenticationToken);
    OpcUa_Field_Read(DateTime, Timestamp);
    OpcUa_Field_Read(UInt32, RequestHandle);
    OpcUa_Field_Read(UInt32, ReturnDiagnostics);
    OpcUa_Field_Read(String, AuditEntryId);
    OpcUa_Field_Read(UInt32, TimeoutHint);
    OpcUa_Field_Read(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RequestHeader_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RequestHeader_EncodeableType =
{
    "RequestHeader",
    OpcUaId_RequestHeader,
    OpcUaId_RequestHeader_Encoding_DefaultBinary,
    OpcUaId_RequestHeader_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RequestHeader),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RequestHeader_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RequestHeader_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RequestHeader_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RequestHeader_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RequestHeader_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ResponseHeader
/*============================================================================
 * OpcUa_ResponseHeader_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ResponseHeader_Initialize(OpcUa_ResponseHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(DateTime, Timestamp);
        OpcUa_Field_Initialize(UInt32, RequestHandle);
        OpcUa_Field_Initialize(StatusCode, ServiceResult);
        OpcUa_Field_Initialize(DiagnosticInfo, ServiceDiagnostics);
        OpcUa_Field_InitializeArray(String, StringTable);
        OpcUa_Field_Initialize(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_ResponseHeader_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ResponseHeader_Clear(OpcUa_ResponseHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(DateTime, Timestamp);
        OpcUa_Field_Clear(UInt32, RequestHandle);
        OpcUa_Field_Clear(StatusCode, ServiceResult);
        OpcUa_Field_Clear(DiagnosticInfo, ServiceDiagnostics);
        OpcUa_Field_ClearArray(String, StringTable);
        OpcUa_Field_Clear(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_ResponseHeader_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_GetSize(OpcUa_ResponseHeader* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(DateTime, Timestamp);
    OpcUa_Field_GetSize(UInt32, RequestHandle);
    OpcUa_Field_GetSize(StatusCode, ServiceResult);
    OpcUa_Field_GetSize(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_GetSizeArray(String, StringTable);
    OpcUa_Field_GetSize(ExtensionObject, AdditionalHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_Encode(OpcUa_ResponseHeader* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(DateTime, Timestamp);
    OpcUa_Field_Write(UInt32, RequestHandle);
    OpcUa_Field_Write(StatusCode, ServiceResult);
    OpcUa_Field_Write(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_WriteArray(String, StringTable);
    OpcUa_Field_Write(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_Decode(OpcUa_ResponseHeader* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ResponseHeader_Initialize(a_pValue);

    OpcUa_Field_Read(DateTime, Timestamp);
    OpcUa_Field_Read(UInt32, RequestHandle);
    OpcUa_Field_Read(StatusCode, ServiceResult);
    OpcUa_Field_Read(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_ReadArray(String, StringTable);
    OpcUa_Field_Read(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ResponseHeader_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ResponseHeader_EncodeableType =
{
    "ResponseHeader",
    OpcUaId_ResponseHeader,
    OpcUaId_ResponseHeader_Encoding_DefaultBinary,
    OpcUaId_ResponseHeader_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ResponseHeader),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ResponseHeader_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ResponseHeader_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ResponseHeader_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ResponseHeader_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ResponseHeader_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ServiceFault
/*============================================================================
 * OpcUa_ServiceFault_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ServiceFault_Initialize(OpcUa_ServiceFault* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_ServiceFault_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ServiceFault_Clear(OpcUa_ServiceFault* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_ServiceFault_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_GetSize(OpcUa_ServiceFault* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_Encode(OpcUa_ServiceFault* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_Decode(OpcUa_ServiceFault* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ServiceFault_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ServiceFault_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ServiceFault_EncodeableType =
{
    "ServiceFault",
    OpcUaId_ServiceFault,
    OpcUaId_ServiceFault_Encoding_DefaultBinary,
    OpcUaId_ServiceFault_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ServiceFault),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ServiceFault_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ServiceFault_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ServiceFault_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ServiceFault_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ServiceFault_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_SessionlessInvokeRequestType
/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_SessionlessInvokeRequestType_Initialize(OpcUa_SessionlessInvokeRequestType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeArray(UInt32, UrisVersion);
        OpcUa_Field_InitializeArray(String, NamespaceUris);
        OpcUa_Field_InitializeArray(String, ServerUris);
        OpcUa_Field_InitializeArray(String, LocaleIds);
        OpcUa_Field_Initialize(UInt32, ServiceId);
    }
}

/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_SessionlessInvokeRequestType_Clear(OpcUa_SessionlessInvokeRequestType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearArray(UInt32, UrisVersion);
        OpcUa_Field_ClearArray(String, NamespaceUris);
        OpcUa_Field_ClearArray(String, ServerUris);
        OpcUa_Field_ClearArray(String, LocaleIds);
        OpcUa_Field_Clear(UInt32, ServiceId);
    }
}

/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeRequestType_GetSize(OpcUa_SessionlessInvokeRequestType* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeRequestType_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeArray(UInt32, UrisVersion);
    OpcUa_Field_GetSizeArray(String, NamespaceUris);
    OpcUa_Field_GetSizeArray(String, ServerUris);
    OpcUa_Field_GetSizeArray(String, LocaleIds);
    OpcUa_Field_GetSize(UInt32, ServiceId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeRequestType_Encode(OpcUa_SessionlessInvokeRequestType* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeRequestType_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteArray(UInt32, UrisVersion);
    OpcUa_Field_WriteArray(String, NamespaceUris);
    OpcUa_Field_WriteArray(String, ServerUris);
    OpcUa_Field_WriteArray(String, LocaleIds);
    OpcUa_Field_Write(UInt32, ServiceId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeRequestType_Decode(OpcUa_SessionlessInvokeRequestType* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeRequestType_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_SessionlessInvokeRequestType_Initialize(a_pValue);

    OpcUa_Field_ReadArray(UInt32, UrisVersion);
    OpcUa_Field_ReadArray(String, NamespaceUris);
    OpcUa_Field_ReadArray(String, ServerUris);
    OpcUa_Field_ReadArray(String, LocaleIds);
    OpcUa_Field_Read(UInt32, ServiceId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_SessionlessInvokeRequestType_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeRequestType_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_SessionlessInvokeRequestType_EncodeableType =
{
    "SessionlessInvokeRequestType",
    OpcUaId_SessionlessInvokeRequestType,
    OpcUaId_SessionlessInvokeRequestType_Encoding_DefaultBinary,
    OpcUaId_SessionlessInvokeRequestType_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_SessionlessInvokeRequestType),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_SessionlessInvokeRequestType_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_SessionlessInvokeRequestType_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_SessionlessInvokeRequestType_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_SessionlessInvokeRequestType_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_SessionlessInvokeRequestType_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_SessionlessInvokeResponseType
/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_SessionlessInvokeResponseType_Initialize(OpcUa_SessionlessInvokeResponseType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeArray(String, NamespaceUris);
        OpcUa_Field_InitializeArray(String, ServerUris);
        OpcUa_Field_Initialize(UInt32, ServiceId);
    }
}

/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_SessionlessInvokeResponseType_Clear(OpcUa_SessionlessInvokeResponseType* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearArray(String, NamespaceUris);
        OpcUa_Field_ClearArray(String, ServerUris);
        OpcUa_Field_Clear(UInt32, ServiceId);
    }
}

/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeResponseType_GetSize(OpcUa_SessionlessInvokeResponseType* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeResponseType_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeArray(String, NamespaceUris);
    OpcUa_Field_GetSizeArray(String, ServerUris);
    OpcUa_Field_GetSize(UInt32, ServiceId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeResponseType_Encode(OpcUa_SessionlessInvokeResponseType* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeResponseType_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteArray(String, NamespaceUris);
    OpcUa_Field_WriteArray(String, ServerUris);
    OpcUa_Field_Write(UInt32, ServiceId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SessionlessInvokeResponseType_Decode(OpcUa_SessionlessInvokeResponseType* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SessionlessInvokeResponseType_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_SessionlessInvokeResponseType_Initialize(a_pValue);

    OpcUa_Field_ReadArray(String, NamespaceUris);
    OpcUa_Field_ReadArray(String, ServerUris);
    OpcUa_Field_Read(UInt32, ServiceId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_SessionlessInvokeResponseType_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SessionlessInvokeResponseType_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_SessionlessInvokeResponseType_EncodeableType =
{
    "SessionlessInvokeResponseType",
    OpcUaId_SessionlessInvokeResponseType,
    OpcUaId_SessionlessInvokeResponseType_Encoding_DefaultBinary,
    OpcUaId_SessionlessInvokeResponseType_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_SessionlessInvokeResponseType),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_SessionlessInvokeResponseType_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_SessionlessInvokeResponseType_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_SessionlessInvokeResponseType_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_SessionlessInvokeResponseType_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_SessionlessInvokeResponseType_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServers
#ifndef OPCUA_EXCLUDE_FindServersRequest
/*============================================================================
 * OpcUa_FindServersRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersRequest_Initialize(OpcUa_FindServersRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeArray(String, LocaleIds);
        OpcUa_Field_InitializeArray(String, ServerUris);
    }
}

/*============================================================================
 * OpcUa_FindServersRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersRequest_Clear(OpcUa_FindServersRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearArray(String, LocaleIds);
        OpcUa_Field_ClearArray(String, ServerUris);
    }
}

/*============================================================================
 * OpcUa_FindServersRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_GetSize(OpcUa_FindServersRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeArray(String, LocaleIds);
    OpcUa_Field_GetSizeArray(String, ServerUris);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_Encode(OpcUa_FindServersRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteArray(String, LocaleIds);
    OpcUa_Field_WriteArray(String, ServerUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_Decode(OpcUa_FindServersRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadArray(String, LocaleIds);
    OpcUa_Field_ReadArray(String, ServerUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersRequest_EncodeableType =
{
    "FindServersRequest",
    OpcUaId_FindServersRequest,
    OpcUaId_FindServersRequest_Encoding_DefaultBinary,
    OpcUaId_FindServersRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersResponse
/*============================================================================
 * OpcUa_FindServersResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersResponse_Initialize(OpcUa_FindServersResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ApplicationDescription, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersResponse_Clear(OpcUa_FindServersResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ApplicationDescription, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_GetSize(OpcUa_FindServersResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ApplicationDescription, Servers);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_Encode(OpcUa_FindServersResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ApplicationDescription, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_Decode(OpcUa_FindServersResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ApplicationDescription, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersResponse_EncodeableType =
{
    "FindServersResponse",
    OpcUaId_FindServersResponse,
    OpcUaId_FindServersResponse_Encoding_DefaultBinary,
    OpcUaId_FindServersResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_ServerOnNetwork
/*============================================================================
 * OpcUa_ServerOnNetwork_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ServerOnNetwork_Initialize(OpcUa_ServerOnNetwork* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, RecordId);
        OpcUa_Field_Initialize(String, ServerName);
        OpcUa_Field_Initialize(String, DiscoveryUrl);
        OpcUa_Field_InitializeArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ServerOnNetwork_Clear(OpcUa_ServerOnNetwork* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, RecordId);
        OpcUa_Field_Clear(String, ServerName);
        OpcUa_Field_Clear(String, DiscoveryUrl);
        OpcUa_Field_ClearArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_ServerOnNetwork_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_GetSize(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, RecordId);
    OpcUa_Field_GetSize(String, ServerName);
    OpcUa_Field_GetSize(String, DiscoveryUrl);
    OpcUa_Field_GetSizeArray(String, ServerCapabilities);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_Encode(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, RecordId);
    OpcUa_Field_Write(String, ServerName);
    OpcUa_Field_Write(String, DiscoveryUrl);
    OpcUa_Field_WriteArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_Decode(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ServerOnNetwork_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, RecordId);
    OpcUa_Field_Read(String, ServerName);
    OpcUa_Field_Read(String, DiscoveryUrl);
    OpcUa_Field_ReadArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ServerOnNetwork_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ServerOnNetwork_EncodeableType =
{
    "ServerOnNetwork",
    OpcUaId_ServerOnNetwork,
    OpcUaId_ServerOnNetwork_Encoding_DefaultBinary,
    OpcUaId_ServerOnNetwork_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ServerOnNetwork),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ServerOnNetwork_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ServerOnNetwork_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ServerOnNetwork_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ServerOnNetwork_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ServerOnNetwork_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetwork
#ifndef OPCUA_EXCLUDE_FindServersOnNetworkRequest
/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkRequest_Initialize(OpcUa_FindServersOnNetworkRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(UInt32, StartingRecordId);
        OpcUa_Field_Initialize(UInt32, MaxRecordsToReturn);
        OpcUa_Field_InitializeArray(String, ServerCapabilityFilter);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkRequest_Clear(OpcUa_FindServersOnNetworkRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(UInt32, StartingRecordId);
        OpcUa_Field_Clear(UInt32, MaxRecordsToReturn);
        OpcUa_Field_ClearArray(String, ServerCapabilityFilter);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_GetSize(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(UInt32, StartingRecordId);
    OpcUa_Field_GetSize(UInt32, MaxRecordsToReturn);
    OpcUa_Field_GetSizeArray(String, ServerCapabilityFilter);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Encode(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(UInt32, StartingRecordId);
    OpcUa_Field_Write(UInt32, MaxRecordsToReturn);
    OpcUa_Field_WriteArray(String, ServerCapabilityFilter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Decode(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersOnNetworkRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(UInt32, StartingRecordId);
    OpcUa_Field_Read(UInt32, MaxRecordsToReturn);
    OpcUa_Field_ReadArray(String, ServerCapabilityFilter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersOnNetworkRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkRequest_EncodeableType =
{
    "FindServersOnNetworkRequest",
    OpcUaId_FindServersOnNetworkRequest,
    OpcUaId_FindServersOnNetworkRequest_Encoding_DefaultBinary,
    OpcUaId_FindServersOnNetworkRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersOnNetworkRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersOnNetworkRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersOnNetworkRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersOnNetworkRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersOnNetworkRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersOnNetworkRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetworkResponse
/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkResponse_Initialize(OpcUa_FindServersOnNetworkResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Initialize(DateTime, LastCounterResetTime);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ServerOnNetwork, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkResponse_Clear(OpcUa_FindServersOnNetworkResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Clear(DateTime, LastCounterResetTime);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ServerOnNetwork, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_GetSize(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSize(DateTime, LastCounterResetTime);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Encode(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Write(DateTime, LastCounterResetTime);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Decode(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersOnNetworkResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Read(DateTime, LastCounterResetTime);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersOnNetworkResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkResponse_EncodeableType =
{
    "FindServersOnNetworkResponse",
    OpcUaId_FindServersOnNetworkResponse,
    OpcUaId_FindServersOnNetworkResponse_Encoding_DefaultBinary,
    OpcUaId_FindServersOnNetworkResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersOnNetworkResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersOnNetworkResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersOnNetworkResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersOnNetworkResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersOnNetworkResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersOnNetworkResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_MessageSecurityMode
/*============================================================================
 * OpcUa_MessageSecurityMode_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_MessageSecurityMode_EnumeratedValues[] =
{
    { "Invalid", 0 },
    { "None", 1 },
    { "Sign", 2 },
    { "SignAndEncrypt", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_MessageSecurityMode_EnumeratedType =
{
    "MessageSecurityMode",
    g_OpcUa_MessageSecurityMode_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_UserTokenType
/*============================================================================
 * OpcUa_UserTokenType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_UserTokenType_EnumeratedValues[] =
{
    { "Anonymous", 0 },
    { "UserName", 1 },
    { "Certificate", 2 },
    { "IssuedToken", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_UserTokenType_EnumeratedType =
{
    "UserTokenType",
    g_OpcUa_UserTokenType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_UserTokenPolicy
/*============================================================================
 * OpcUa_UserTokenPolicy_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_UserTokenPolicy_Initialize(OpcUa_UserTokenPolicy* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
        OpcUa_Field_InitializeEnumerated(OpcUa_UserTokenType, TokenType);
        OpcUa_Field_Initialize(String, IssuedTokenType);
        OpcUa_Field_Initialize(String, IssuerEndpointUrl);
        OpcUa_Field_Initialize(String, SecurityPolicyUri);
    }
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_UserTokenPolicy_Clear(OpcUa_UserTokenPolicy* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
        OpcUa_Field_ClearEnumerated(OpcUa_UserTokenType, TokenType);
        OpcUa_Field_Clear(String, IssuedTokenType);
        OpcUa_Field_Clear(String, IssuerEndpointUrl);
        OpcUa_Field_Clear(String, SecurityPolicyUri);
    }
}

/*============================================================================
 * OpcUa_UserTokenPolicy_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_GetSize(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_GetSize(String, IssuedTokenType);
    OpcUa_Field_GetSize(String, IssuerEndpointUrl);
    OpcUa_Field_GetSize(String, SecurityPolicyUri);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_Encode(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);
    OpcUa_Field_WriteEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_Write(String, IssuedTokenType);
    OpcUa_Field_Write(String, IssuerEndpointUrl);
    OpcUa_Field_Write(String, SecurityPolicyUri);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_Decode(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_UserTokenPolicy_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);
    OpcUa_Field_ReadEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_Read(String, IssuedTokenType);
    OpcUa_Field_Read(String, IssuerEndpointUrl);
    OpcUa_Field_Read(String, SecurityPolicyUri);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UserTokenPolicy_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_UserTokenPolicy_EncodeableType =
{
    "UserTokenPolicy",
    OpcUaId_UserTokenPolicy,
    OpcUaId_UserTokenPolicy_Encoding_DefaultBinary,
    OpcUaId_UserTokenPolicy_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_UserTokenPolicy),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_UserTokenPolicy_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_UserTokenPolicy_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_UserTokenPolicy_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_UserTokenPolicy_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_UserTokenPolicy_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EndpointDescription
/*============================================================================
 * OpcUa_EndpointDescription_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointDescription_Initialize(OpcUa_EndpointDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeEncodeable(OpcUa_ApplicationDescription, Server);
        OpcUa_Field_Initialize(ByteString, ServerCertificate);
        OpcUa_Field_InitializeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Initialize(String, SecurityPolicyUri);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
        OpcUa_Field_Initialize(String, TransportProfileUri);
        OpcUa_Field_Initialize(Byte, SecurityLevel);
    }
}

/*============================================================================
 * OpcUa_EndpointDescription_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointDescription_Clear(OpcUa_EndpointDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearEncodeable(OpcUa_ApplicationDescription, Server);
        OpcUa_Field_Clear(ByteString, ServerCertificate);
        OpcUa_Field_ClearEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Clear(String, SecurityPolicyUri);
        OpcUa_Field_ClearEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
        OpcUa_Field_Clear(String, TransportProfileUri);
        OpcUa_Field_Clear(Byte, SecurityLevel);
    }
}

/*============================================================================
 * OpcUa_EndpointDescription_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_GetSize(OpcUa_EndpointDescription* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_GetSize(ByteString, ServerCertificate);
    OpcUa_Field_GetSizeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_GetSize(String, SecurityPolicyUri);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_GetSize(String, TransportProfileUri);
    OpcUa_Field_GetSize(Byte, SecurityLevel);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_Encode(OpcUa_EndpointDescription* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_Write(ByteString, ServerCertificate);
    OpcUa_Field_WriteEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Write(String, SecurityPolicyUri);
    OpcUa_Field_WriteEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_Write(String, TransportProfileUri);
    OpcUa_Field_Write(Byte, SecurityLevel);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_Decode(OpcUa_EndpointDescription* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EndpointDescription_Initialize(a_pValue);

    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_Read(ByteString, ServerCertificate);
    OpcUa_Field_ReadEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Read(String, SecurityPolicyUri);
    OpcUa_Field_ReadEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_Read(String, TransportProfileUri);
    OpcUa_Field_Read(Byte, SecurityLevel);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EndpointDescription_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EndpointDescription_EncodeableType =
{
    "EndpointDescription",
    OpcUaId_EndpointDescription,
    OpcUaId_EndpointDescription_Encoding_DefaultBinary,
    OpcUaId_EndpointDescription_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EndpointDescription),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EndpointDescription_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EndpointDescription_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EndpointDescription_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EndpointDescription_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EndpointDescription_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GetEndpoints
#ifndef OPCUA_EXCLUDE_GetEndpointsRequest
/*============================================================================
 * OpcUa_GetEndpointsRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsRequest_Initialize(OpcUa_GetEndpointsRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeArray(String, LocaleIds);
        OpcUa_Field_InitializeArray(String, ProfileUris);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsRequest_Clear(OpcUa_GetEndpointsRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearArray(String, LocaleIds);
        OpcUa_Field_ClearArray(String, ProfileUris);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_GetSize(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeArray(String, LocaleIds);
    OpcUa_Field_GetSizeArray(String, ProfileUris);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_Encode(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteArray(String, LocaleIds);
    OpcUa_Field_WriteArray(String, ProfileUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_Decode(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GetEndpointsRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadArray(String, LocaleIds);
    OpcUa_Field_ReadArray(String, ProfileUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GetEndpointsRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GetEndpointsRequest_EncodeableType =
{
    "GetEndpointsRequest",
    OpcUaId_GetEndpointsRequest,
    OpcUaId_GetEndpointsRequest_Encoding_DefaultBinary,
    OpcUaId_GetEndpointsRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GetEndpointsRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GetEndpointsRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GetEndpointsRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GetEndpointsRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GetEndpointsRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GetEndpointsRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GetEndpointsResponse
/*============================================================================
 * OpcUa_GetEndpointsResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsResponse_Initialize(OpcUa_GetEndpointsResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_EndpointDescription, Endpoints);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsResponse_Clear(OpcUa_GetEndpointsResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearEncodeableArray(OpcUa_EndpointDescription, Endpoints);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_GetSize(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_Encode(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_Decode(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GetEndpointsResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GetEndpointsResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GetEndpointsResponse_EncodeableType =
{
    "GetEndpointsResponse",
    OpcUaId_GetEndpointsResponse,
    OpcUaId_GetEndpointsResponse_Encoding_DefaultBinary,
    OpcUaId_GetEndpointsResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GetEndpointsResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GetEndpointsResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GetEndpointsResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GetEndpointsResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GetEndpointsResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GetEndpointsResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_RegisteredServer
/*============================================================================
 * OpcUa_RegisteredServer_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisteredServer_Initialize(OpcUa_RegisteredServer* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, ServerUri);
        OpcUa_Field_Initialize(String, ProductUri);
        OpcUa_Field_InitializeArray(LocalizedText, ServerNames);
        OpcUa_Field_InitializeEnumerated(OpcUa_ApplicationType, ServerType);
        OpcUa_Field_Initialize(String, GatewayServerUri);
        OpcUa_Field_InitializeArray(String, DiscoveryUrls);
        OpcUa_Field_Initialize(String, SemaphoreFilePath);
        OpcUa_Field_Initialize(Boolean, IsOnline);
    }
}

/*============================================================================
 * OpcUa_RegisteredServer_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisteredServer_Clear(OpcUa_RegisteredServer* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, ServerUri);
        OpcUa_Field_Clear(String, ProductUri);
        OpcUa_Field_ClearArray(LocalizedText, ServerNames);
        OpcUa_Field_ClearEnumerated(OpcUa_ApplicationType, ServerType);
        OpcUa_Field_Clear(String, GatewayServerUri);
        OpcUa_Field_ClearArray(String, DiscoveryUrls);
        OpcUa_Field_Clear(String, SemaphoreFilePath);
        OpcUa_Field_Clear(Boolean, IsOnline);
    }
}

/*============================================================================
 * OpcUa_RegisteredServer_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_GetSize(OpcUa_RegisteredServer* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, ServerUri);
    OpcUa_Field_GetSize(String, ProductUri);
    OpcUa_Field_GetSizeArray(LocalizedText, ServerNames);
    OpcUa_Field_GetSizeEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_GetSize(String, GatewayServerUri);
    OpcUa_Field_GetSizeArray(String, DiscoveryUrls);
    OpcUa_Field_GetSize(String, SemaphoreFilePath);
    OpcUa_Field_GetSize(Boolean, IsOnline);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_Encode(OpcUa_RegisteredServer* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, ServerUri);
    OpcUa_Field_Write(String, ProductUri);
    OpcUa_Field_WriteArray(LocalizedText, ServerNames);
    OpcUa_Field_WriteEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_Write(String, GatewayServerUri);
    OpcUa_Field_WriteArray(String, DiscoveryUrls);
    OpcUa_Field_Write(String, SemaphoreFilePath);
    OpcUa_Field_Write(Boolean, IsOnline);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_Decode(OpcUa_RegisteredServer* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisteredServer_Initialize(a_pValue);

    OpcUa_Field_Read(String, ServerUri);
    OpcUa_Field_Read(String, ProductUri);
    OpcUa_Field_ReadArray(LocalizedText, ServerNames);
    OpcUa_Field_ReadEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_Read(String, GatewayServerUri);
    OpcUa_Field_ReadArray(String, DiscoveryUrls);
    OpcUa_Field_Read(String, SemaphoreFilePath);
    OpcUa_Field_Read(Boolean, IsOnline);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisteredServer_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisteredServer_EncodeableType =
{
    "RegisteredServer",
    OpcUaId_RegisteredServer,
    OpcUaId_RegisteredServer_Encoding_DefaultBinary,
    OpcUaId_RegisteredServer_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisteredServer),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisteredServer_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisteredServer_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisteredServer_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisteredServer_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisteredServer_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer
#ifndef OPCUA_EXCLUDE_RegisterServerRequest
/*============================================================================
 * OpcUa_RegisterServerRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerRequest_Initialize(OpcUa_RegisterServerRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_InitializeEncodeable(OpcUa_RegisteredServer, Server);
    }
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerRequest_Clear(OpcUa_RegisterServerRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_ClearEncodeable(OpcUa_RegisteredServer, Server);
    }
}

/*============================================================================
 * OpcUa_RegisterServerRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_GetSize(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RegisteredServer, Server);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_Encode(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_WriteEncodeable(OpcUa_RegisteredServer, Server);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_Decode(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServerRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_ReadEncodeable(OpcUa_RegisteredServer, Server);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServerRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServerRequest_EncodeableType =
{
    "RegisterServerRequest",
    OpcUaId_RegisterServerRequest,
    OpcUaId_RegisterServerRequest_Encoding_DefaultBinary,
    OpcUaId_RegisterServerRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServerRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServerRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServerRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServerRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServerRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServerRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServerResponse
/*============================================================================
 * OpcUa_RegisterServerResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerResponse_Initialize(OpcUa_RegisterServerResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerResponse_Clear(OpcUa_RegisterServerResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_RegisterServerResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_GetSize(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_Encode(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_Decode(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServerResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServerResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServerResponse_EncodeableType =
{
    "RegisterServerResponse",
    OpcUaId_RegisterServerResponse,
    OpcUaId_RegisterServerResponse_Encoding_DefaultBinary,
    OpcUaId_RegisterServerResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServerResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServerResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServerResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServerResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServerResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServerResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_MdnsDiscoveryConfiguration
/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Initialize(OpcUa_MdnsDiscoveryConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, MdnsServerName);
        OpcUa_Field_InitializeArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Clear(OpcUa_MdnsDiscoveryConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, MdnsServerName);
        OpcUa_Field_ClearArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_GetSize(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, MdnsServerName);
    OpcUa_Field_GetSizeArray(String, ServerCapabilities);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Encode(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, MdnsServerName);
    OpcUa_Field_WriteArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Decode(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_MdnsDiscoveryConfiguration_Initialize(a_pValue);

    OpcUa_Field_Read(String, MdnsServerName);
    OpcUa_Field_ReadArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_MdnsDiscoveryConfiguration_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_MdnsDiscoveryConfiguration_EncodeableType =
{
    "MdnsDiscoveryConfiguration",
    OpcUaId_MdnsDiscoveryConfiguration,
    OpcUaId_MdnsDiscoveryConfiguration_Encoding_DefaultBinary,
    OpcUaId_MdnsDiscoveryConfiguration_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_MdnsDiscoveryConfiguration),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_MdnsDiscoveryConfiguration_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_MdnsDiscoveryConfiguration_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_MdnsDiscoveryConfiguration_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_MdnsDiscoveryConfiguration_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_MdnsDiscoveryConfiguration_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2
#ifndef OPCUA_EXCLUDE_RegisterServer2Request
/*============================================================================
 * OpcUa_RegisterServer2Request_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Request_Initialize(OpcUa_RegisterServer2Request* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_InitializeEncodeable(OpcUa_RegisteredServer, Server);
        OpcUa_Field_InitializeArray(ExtensionObject, DiscoveryConfiguration);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Request_Clear(OpcUa_RegisterServer2Request* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_ClearEncodeable(OpcUa_RegisteredServer, Server);
        OpcUa_Field_ClearArray(ExtensionObject, DiscoveryConfiguration);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Request_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_GetSize(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_GetSizeArray(ExtensionObject, DiscoveryConfiguration);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_Encode(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_WriteEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_WriteArray(ExtensionObject, DiscoveryConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_Decode(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServer2Request_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_ReadEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_ReadArray(ExtensionObject, DiscoveryConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServer2Request_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServer2Request_EncodeableType =
{
    "RegisterServer2Request",
    OpcUaId_RegisterServer2Request,
    OpcUaId_RegisterServer2Request_Encoding_DefaultBinary,
    OpcUaId_RegisterServer2Request_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServer2Request),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServer2Request_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServer2Request_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServer2Request_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServer2Request_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServer2Request_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2Response
/*============================================================================
 * OpcUa_RegisterServer2Response_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Response_Initialize(OpcUa_RegisterServer2Response* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeArray(StatusCode, ConfigurationResults);
        OpcUa_Field_InitializeArray(DiagnosticInfo, DiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Response_Clear(OpcUa_RegisterServer2Response* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearArray(StatusCode, ConfigurationResults);
        OpcUa_Field_ClearArray(DiagnosticInfo, DiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Response_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_GetSize(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeArray(StatusCode, ConfigurationResults);
    OpcUa_Field_GetSizeArray(DiagnosticInfo, DiagnosticInfos);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_Encode(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteArray(StatusCode, ConfigurationResults);
    OpcUa_Field_WriteArray(DiagnosticInfo, DiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_Decode(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServer2Response_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadArray(StatusCode, ConfigurationResults);
    OpcUa_Field_ReadArray(DiagnosticInfo, DiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServer2Response_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServer2Response_EncodeableType =
{
    "RegisterServer2Response",
    OpcUaId_RegisterServer2Response,
    OpcUaId_RegisterServer2Response_Encoding_DefaultBinary,
    OpcUaId_RegisterServer2Response_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServer2Response),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServer2Response_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServer2Response_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServer2Response_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServer2Response_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServer2Response_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_SecurityTokenRequestType
/*============================================================================
 * OpcUa_SecurityTokenRequestType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_SecurityTokenRequestType_EnumeratedValues[] =
{
    { "Issue", 0 },
    { "Renew", 1 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_SecurityTokenRequestType_EnumeratedType =
{
    "SecurityTokenRequestType",
    g_OpcUa_SecurityTokenRequestType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ChannelSecurityToken
/*============================================================================
 * OpcUa_ChannelSecurityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ChannelSecurityToken_Initialize(OpcUa_ChannelSecurityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, ChannelId);
        OpcUa_Field_Initialize(UInt32, TokenId);
        OpcUa_Field_Initialize(DateTime, CreatedAt);
        OpcUa_Field_Initialize(UInt32, RevisedLifetime);
    }
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ChannelSecurityToken_Clear(OpcUa_ChannelSecurityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, ChannelId);
        OpcUa_Field_Clear(UInt32, TokenId);
        OpcUa_Field_Clear(DateTime, CreatedAt);
        OpcUa_Field_Clear(UInt32, RevisedLifetime);
    }
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_GetSize(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, ChannelId);
    OpcUa_Field_GetSize(UInt32, TokenId);
    OpcUa_Field_GetSize(DateTime, CreatedAt);
    OpcUa_Field_GetSize(UInt32, RevisedLifetime);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_Encode(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, ChannelId);
    OpcUa_Field_Write(UInt32, TokenId);
    OpcUa_Field_Write(DateTime, CreatedAt);
    OpcUa_Field_Write(UInt32, RevisedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_Decode(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ChannelSecurityToken_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, ChannelId);
    OpcUa_Field_Read(UInt32, TokenId);
    OpcUa_Field_Read(DateTime, CreatedAt);
    OpcUa_Field_Read(UInt32, RevisedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ChannelSecurityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ChannelSecurityToken_EncodeableType =
{
    "ChannelSecurityToken",
    OpcUaId_ChannelSecurityToken,
    OpcUaId_ChannelSecurityToken_Encoding_DefaultBinary,
    OpcUaId_ChannelSecurityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ChannelSecurityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ChannelSecurityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ChannelSecurityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ChannelSecurityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ChannelSecurityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ChannelSecurityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannel
#ifndef OPCUA_EXCLUDE_OpenSecureChannelRequest
/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelRequest_Initialize(OpcUa_OpenSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(UInt32, ClientProtocolVersion);
        OpcUa_Field_InitializeEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
        OpcUa_Field_InitializeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Initialize(ByteString, ClientNonce);
        OpcUa_Field_Initialize(UInt32, RequestedLifetime);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelRequest_Clear(OpcUa_OpenSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(UInt32, ClientProtocolVersion);
        OpcUa_Field_ClearEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
        OpcUa_Field_ClearEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Clear(ByteString, ClientNonce);
        OpcUa_Field_Clear(UInt32, RequestedLifetime);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_GetSize(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(UInt32, ClientProtocolVersion);
    OpcUa_Field_GetSizeEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_GetSizeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_GetSize(ByteString, ClientNonce);
    OpcUa_Field_GetSize(UInt32, RequestedLifetime);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Encode(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(UInt32, ClientProtocolVersion);
    OpcUa_Field_WriteEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_WriteEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Write(ByteString, ClientNonce);
    OpcUa_Field_Write(UInt32, RequestedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Decode(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_OpenSecureChannelRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(UInt32, ClientProtocolVersion);
    OpcUa_Field_ReadEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_ReadEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Read(ByteString, ClientNonce);
    OpcUa_Field_Read(UInt32, RequestedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_OpenSecureChannelRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelRequest_EncodeableType =
{
    "OpenSecureChannelRequest",
    OpcUaId_OpenSecureChannelRequest,
    OpcUaId_OpenSecureChannelRequest_Encoding_DefaultBinary,
    OpcUaId_OpenSecureChannelRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_OpenSecureChannelRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_OpenSecureChannelRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_OpenSecureChannelRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_OpenSecureChannelRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_OpenSecureChannelRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_OpenSecureChannelRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannelResponse
/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelResponse_Initialize(OpcUa_OpenSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Initialize(UInt32, ServerProtocolVersion);
        OpcUa_Field_InitializeEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
        OpcUa_Field_Initialize(ByteString, ServerNonce);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelResponse_Clear(OpcUa_OpenSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Clear(UInt32, ServerProtocolVersion);
        OpcUa_Field_ClearEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
        OpcUa_Field_Clear(ByteString, ServerNonce);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_GetSize(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSize(UInt32, ServerProtocolVersion);
    OpcUa_Field_GetSizeEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_GetSize(ByteString, ServerNonce);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Encode(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Write(UInt32, ServerProtocolVersion);
    OpcUa_Field_WriteEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_Write(ByteString, ServerNonce);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Decode(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_OpenSecureChannelResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Read(UInt32, ServerProtocolVersion);
    OpcUa_Field_ReadEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_Read(ByteString, ServerNonce);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_OpenSecureChannelResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelResponse_EncodeableType =
{
    "OpenSecureChannelResponse",
    OpcUaId_OpenSecureChannelResponse,
    OpcUaId_OpenSecureChannelResponse_Encoding_DefaultBinary,
    OpcUaId_OpenSecureChannelResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_OpenSecureChannelResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_OpenSecureChannelResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_OpenSecureChannelResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_OpenSecureChannelResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_OpenSecureChannelResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_OpenSecureChannelResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannel
#ifndef OPCUA_EXCLUDE_CloseSecureChannelRequest
/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelRequest_Initialize(OpcUa_CloseSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelRequest_Clear(OpcUa_CloseSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_GetSize(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Encode(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Decode(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_CloseSecureChannelRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_CloseSecureChannelRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelRequest_EncodeableType =
{
    "CloseSecureChannelRequest",
    OpcUaId_CloseSecureChannelRequest,
    OpcUaId_CloseSecureChannelRequest_Encoding_DefaultBinary,
    OpcUaId_CloseSecureChannelRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_CloseSecureChannelRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_CloseSecureChannelRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_CloseSecureChannelRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_CloseSecureChannelRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_CloseSecureChannelRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_CloseSecureChannelRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannelResponse
/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelResponse_Initialize(OpcUa_CloseSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelResponse_Clear(OpcUa_CloseSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_GetSize(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Encode(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Decode(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_CloseSecureChannelResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_CloseSecureChannelResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelResponse_EncodeableType =
{
    "CloseSecureChannelResponse",
    OpcUaId_CloseSecureChannelResponse,
    OpcUaId_CloseSecureChannelResponse_Encoding_DefaultBinary,
    OpcUaId_CloseSecureChannelResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_CloseSecureChannelResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_CloseSecureChannelResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_CloseSecureChannelResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_CloseSecureChannelResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_CloseSecureChannelResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_CloseSecureChannelResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_SignedSoftwareCertificate
/*============================================================================
 * OpcUa_SignedSoftwareCertificate_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_SignedSoftwareCertificate_Initialize(OpcUa_SignedSoftwareCertificate* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(ByteString, CertificateData);
        OpcUa_Field_Initialize(ByteString, Signature);
    }
}

/*============================================================================
 * OpcUa_SignedSoftwareCertificate_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_SignedSoftwareCertificate_Clear(OpcUa_SignedSoftwareCertificate* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(ByteString, CertificateData);
        OpcUa_Field_Clear(ByteString, Signature);
    }
}

/*============================================================================
 * OpcUa_SignedSoftwareCertificate_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignedSoftwareCertificate_GetSize(OpcUa_SignedSoftwareCertificate* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignedSoftwareCertificate_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(ByteString, CertificateData);
    OpcUa_Field_GetSize(ByteString, Signature);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignedSoftwareCertificate_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignedSoftwareCertificate_Encode(OpcUa_SignedSoftwareCertificate* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignedSoftwareCertificate_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(ByteString, CertificateData);
    OpcUa_Field_Write(ByteString, Signature);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignedSoftwareCertificate_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignedSoftwareCertificate_Decode(OpcUa_SignedSoftwareCertificate* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignedSoftwareCertificate_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_SignedSoftwareCertificate_Initialize(a_pValue);

    OpcUa_Field_Read(ByteString, CertificateData);
    OpcUa_Field_Read(ByteString, Signature);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_SignedSoftwareCertificate_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignedSoftwareCertificate_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_SignedSoftwareCertificate_EncodeableType =
{
    "SignedSoftwareCertificate",
    OpcUaId_SignedSoftwareCertificate,
    OpcUaId_SignedSoftwareCertificate_Encoding_DefaultBinary,
    OpcUaId_SignedSoftwareCertificate_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_SignedSoftwareCertificate),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_SignedSoftwareCertificate_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_SignedSoftwareCertificate_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_SignedSoftwareCertificate_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_SignedSoftwareCertificate_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_SignedSoftwareCertificate_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_SignatureData
/*============================================================================
 * OpcUa_SignatureData_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_SignatureData_Initialize(OpcUa_SignatureData* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, Algorithm);
        OpcUa_Field_Initialize(ByteString, Signature);
    }
}

/*============================================================================
 * OpcUa_SignatureData_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_SignatureData_Clear(OpcUa_SignatureData* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, Algorithm);
        OpcUa_Field_Clear(ByteString, Signature);
    }
}

/*============================================================================
 * OpcUa_SignatureData_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignatureData_GetSize(OpcUa_SignatureData* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignatureData_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, Algorithm);
    OpcUa_Field_GetSize(ByteString, Signature);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignatureData_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignatureData_Encode(OpcUa_SignatureData* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignatureData_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, Algorithm);
    OpcUa_Field_Write(ByteString, Signature);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignatureData_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SignatureData_Decode(OpcUa_SignatureData* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SignatureData_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_SignatureData_Initialize(a_pValue);

    OpcUa_Field_Read(String, Algorithm);
    OpcUa_Field_Read(ByteString, Signature);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_SignatureData_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SignatureData_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_SignatureData_EncodeableType =
{
    "SignatureData",
    OpcUaId_SignatureData,
    OpcUaId_SignatureData_Encoding_DefaultBinary,
    OpcUaId_SignatureData_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_SignatureData),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_SignatureData_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_SignatureData_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_SignatureData_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_SignatureData_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_SignatureData_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_UserIdentityToken
/*============================================================================
 * OpcUa_UserIdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_UserIdentityToken_Initialize(OpcUa_UserIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_UserIdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_UserIdentityToken_Clear(OpcUa_UserIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_UserIdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_GetSize(OpcUa_UserIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_Encode(OpcUa_UserIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_Decode(OpcUa_UserIdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_UserIdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UserIdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_UserIdentityToken_EncodeableType =
{
    "UserIdentityToken",
    OpcUaId_UserIdentityToken,
    OpcUaId_UserIdentityToken_Encoding_DefaultBinary,
    OpcUaId_UserIdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_UserIdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_UserIdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_UserIdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_UserIdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_UserIdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_UserIdentityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_AnonymousIdentityToken
/*============================================================================
 * OpcUa_AnonymousIdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_AnonymousIdentityToken_Initialize(OpcUa_AnonymousIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_AnonymousIdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_AnonymousIdentityToken_Clear(OpcUa_AnonymousIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_AnonymousIdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AnonymousIdentityToken_GetSize(OpcUa_AnonymousIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AnonymousIdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AnonymousIdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AnonymousIdentityToken_Encode(OpcUa_AnonymousIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AnonymousIdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AnonymousIdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AnonymousIdentityToken_Decode(OpcUa_AnonymousIdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AnonymousIdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_AnonymousIdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_AnonymousIdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AnonymousIdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_AnonymousIdentityToken_EncodeableType =
{
    "AnonymousIdentityToken",
    OpcUaId_AnonymousIdentityToken,
    OpcUaId_AnonymousIdentityToken_Encoding_DefaultBinary,
    OpcUaId_AnonymousIdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_AnonymousIdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_AnonymousIdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_AnonymousIdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_AnonymousIdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_AnonymousIdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_AnonymousIdentityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_UserNameIdentityToken
/*============================================================================
 * OpcUa_UserNameIdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_UserNameIdentityToken_Initialize(OpcUa_UserNameIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
        OpcUa_Field_Initialize(String, UserName);
        OpcUa_Field_Initialize(ByteString, Password);
        OpcUa_Field_Initialize(String, EncryptionAlgorithm);
    }
}

/*============================================================================
 * OpcUa_UserNameIdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_UserNameIdentityToken_Clear(OpcUa_UserNameIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
        OpcUa_Field_Clear(String, UserName);
        OpcUa_Field_Clear(ByteString, Password);
        OpcUa_Field_Clear(String, EncryptionAlgorithm);
    }
}

/*============================================================================
 * OpcUa_UserNameIdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserNameIdentityToken_GetSize(OpcUa_UserNameIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserNameIdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);
    OpcUa_Field_GetSize(String, UserName);
    OpcUa_Field_GetSize(ByteString, Password);
    OpcUa_Field_GetSize(String, EncryptionAlgorithm);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserNameIdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserNameIdentityToken_Encode(OpcUa_UserNameIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserNameIdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);
    OpcUa_Field_Write(String, UserName);
    OpcUa_Field_Write(ByteString, Password);
    OpcUa_Field_Write(String, EncryptionAlgorithm);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserNameIdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserNameIdentityToken_Decode(OpcUa_UserNameIdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserNameIdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_UserNameIdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);
    OpcUa_Field_Read(String, UserName);
    OpcUa_Field_Read(ByteString, Password);
    OpcUa_Field_Read(String, EncryptionAlgorithm);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UserNameIdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserNameIdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_UserNameIdentityToken_EncodeableType =
{
    "UserNameIdentityToken",
    OpcUaId_UserNameIdentityToken,
    OpcUaId_UserNameIdentityToken_Encoding_DefaultBinary,
    OpcUaId_UserNameIdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_UserNameIdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_UserNameIdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_UserNameIdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_UserNameIdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_UserNameIdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_UserNameIdentityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_X509IdentityToken
/*============================================================================
 * OpcUa_X509IdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_X509IdentityToken_Initialize(OpcUa_X509IdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
        OpcUa_Field_Initialize(ByteString, CertificateData);
    }
}

/*============================================================================
 * OpcUa_X509IdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_X509IdentityToken_Clear(OpcUa_X509IdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
        OpcUa_Field_Clear(ByteString, CertificateData);
    }
}

/*============================================================================
 * OpcUa_X509IdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_X509IdentityToken_GetSize(OpcUa_X509IdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "X509IdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);
    OpcUa_Field_GetSize(ByteString, CertificateData);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_X509IdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_X509IdentityToken_Encode(OpcUa_X509IdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "X509IdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);
    OpcUa_Field_Write(ByteString, CertificateData);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_X509IdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_X509IdentityToken_Decode(OpcUa_X509IdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "X509IdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_X509IdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);
    OpcUa_Field_Read(ByteString, CertificateData);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_X509IdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_X509IdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_X509IdentityToken_EncodeableType =
{
    "X509IdentityToken",
    OpcUaId_X509IdentityToken,
    OpcUaId_X509IdentityToken_Encoding_DefaultBinary,
    OpcUaId_X509IdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_X509IdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_X509IdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_X509IdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_X509IdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_X509IdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_X509IdentityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_IssuedIdentityToken
/*============================================================================
 * OpcUa_IssuedIdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_IssuedIdentityToken_Initialize(OpcUa_IssuedIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
        OpcUa_Field_Initialize(ByteString, TokenData);
        OpcUa_Field_Initialize(String, EncryptionAlgorithm);
    }
}

/*============================================================================
 * OpcUa_IssuedIdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_IssuedIdentityToken_Clear(OpcUa_IssuedIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
        OpcUa_Field_Clear(ByteString, TokenData);
        OpcUa_Field_Clear(String, EncryptionAlgorithm);
    }
}

/*============================================================================
 * OpcUa_IssuedIdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_IssuedIdentityToken_GetSize(OpcUa_IssuedIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "IssuedIdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);
    OpcUa_Field_GetSize(ByteString, TokenData);
    OpcUa_Field_GetSize(String, EncryptionAlgorithm);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_IssuedIdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_IssuedIdentityToken_Encode(OpcUa_IssuedIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "IssuedIdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);
    OpcUa_Field_Write(ByteString, TokenData);
    OpcUa_Field_Write(String, EncryptionAlgorithm);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_IssuedIdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_IssuedIdentityToken_Decode(OpcUa_IssuedIdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "IssuedIdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_IssuedIdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);
    OpcUa_Field_Read(ByteString, TokenData);
    OpcUa_Field_Read(String, EncryptionAlgorithm);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_IssuedIdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_IssuedIdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_IssuedIdentityToken_EncodeableType =
{
    "IssuedIdentityToken",
    OpcUaId_IssuedIdentityToken,
    OpcUaId_IssuedIdentityToken_Encoding_DefaultBinary,
    OpcUaId_IssuedIdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_IssuedIdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_IssuedIdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_IssuedIdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_IssuedIdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_IssuedIdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_IssuedIdentityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_NodeAttributesMask
/*============================================================================
 * OpcUa_NodeAttributesMask_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_NodeAttributesMask_EnumeratedValues[] =
{
    { "None", 0 },
    { "AccessLevel", 1 },
    { "ArrayDimensions", 2 },
    { "BrowseName", 4 },
    { "ContainsNoLoops", 8 },
    { "DataType", 16 },
    { "Description", 32 },
    { "DisplayName", 64 },
    { "EventNotifier", 128 },
    { "Executable", 256 },
    { "Historizing", 512 },
    { "InverseName", 1024 },
    { "IsAbstract", 2048 },
    { "MinimumSamplingInterval", 4096 },
    { "NodeClass", 8192 },
    { "NodeId", 16384 },
    { "Symmetric", 32768 },
    { "UserAccessLevel", 65536 },
    { "UserExecutable", 131072 },
    { "UserWriteMask", 262144 },
    { "ValueRank", 524288 },
    { "WriteMask", 1048576 },
    { "Value", 2097152 },
    { "DataTypeDefinition", 4194304 },
    { "RolePermissions", 8388608 },
    { "AccessRestrictions", 16777216 },
    { "All", 33554431 },
    { "BaseNode", 26501220 },
    { "Object", 26501348 },
    { "ObjectType", 26503268 },
    { "Variable", 26571383 },
    { "VariableType", 28600438 },
    { "Method", 26632548 },
    { "ReferenceType", 26537060 },
    { "View", 26501356 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_NodeAttributesMask_EnumeratedType =
{
    "NodeAttributesMask",
    g_OpcUa_NodeAttributesMask_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_NodeAttributes
/*============================================================================
 * OpcUa_NodeAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_NodeAttributes_Initialize(OpcUa_NodeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
    }
}

/*============================================================================
 * OpcUa_NodeAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_NodeAttributes_Clear(OpcUa_NodeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
    }
}

/*============================================================================
 * OpcUa_NodeAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeAttributes_GetSize(OpcUa_NodeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeAttributes_Encode(OpcUa_NodeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeAttributes_Decode(OpcUa_NodeAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_NodeAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_NodeAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_NodeAttributes_EncodeableType =
{
    "NodeAttributes",
    OpcUaId_NodeAttributes,
    OpcUaId_NodeAttributes_Encoding_DefaultBinary,
    OpcUaId_NodeAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_NodeAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_NodeAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_NodeAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_NodeAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_NodeAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_NodeAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ObjectAttributes
/*============================================================================
 * OpcUa_ObjectAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectAttributes_Initialize(OpcUa_ObjectAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ObjectAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectAttributes_Clear(OpcUa_ObjectAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ObjectAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectAttributes_GetSize(OpcUa_ObjectAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Byte, EventNotifier);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectAttributes_Encode(OpcUa_ObjectAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectAttributes_Decode(OpcUa_ObjectAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ObjectAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ObjectAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ObjectAttributes_EncodeableType =
{
    "ObjectAttributes",
    OpcUaId_ObjectAttributes,
    OpcUaId_ObjectAttributes_Encoding_DefaultBinary,
    OpcUaId_ObjectAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ObjectAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ObjectAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ObjectAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ObjectAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ObjectAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ObjectAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_VariableAttributes
/*============================================================================
 * OpcUa_VariableAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_VariableAttributes_Initialize(OpcUa_VariableAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Variant, Value);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(Byte, AccessLevel);
        OpcUa_Field_Initialize(Byte, UserAccessLevel);
        OpcUa_Field_Initialize(Double, MinimumSamplingInterval);
        OpcUa_Field_Initialize(Boolean, Historizing);
    }
}

/*============================================================================
 * OpcUa_VariableAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_VariableAttributes_Clear(OpcUa_VariableAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Variant, Value);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(Byte, AccessLevel);
        OpcUa_Field_Clear(Byte, UserAccessLevel);
        OpcUa_Field_Clear(Double, MinimumSamplingInterval);
        OpcUa_Field_Clear(Boolean, Historizing);
    }
}

/*============================================================================
 * OpcUa_VariableAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableAttributes_GetSize(OpcUa_VariableAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Variant, Value);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(Byte, AccessLevel);
    OpcUa_Field_GetSize(Byte, UserAccessLevel);
    OpcUa_Field_GetSize(Double, MinimumSamplingInterval);
    OpcUa_Field_GetSize(Boolean, Historizing);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableAttributes_Encode(OpcUa_VariableAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Variant, Value);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(Byte, AccessLevel);
    OpcUa_Field_Write(Byte, UserAccessLevel);
    OpcUa_Field_Write(Double, MinimumSamplingInterval);
    OpcUa_Field_Write(Boolean, Historizing);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableAttributes_Decode(OpcUa_VariableAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_VariableAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Variant, Value);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(Byte, AccessLevel);
    OpcUa_Field_Read(Byte, UserAccessLevel);
    OpcUa_Field_Read(Double, MinimumSamplingInterval);
    OpcUa_Field_Read(Boolean, Historizing);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_VariableAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_VariableAttributes_EncodeableType =
{
    "VariableAttributes",
    OpcUaId_VariableAttributes,
    OpcUaId_VariableAttributes_Encoding_DefaultBinary,
    OpcUaId_VariableAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_VariableAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_VariableAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_VariableAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_VariableAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_VariableAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_VariableAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_MethodAttributes
/*============================================================================
 * OpcUa_MethodAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_MethodAttributes_Initialize(OpcUa_MethodAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Boolean, Executable);
        OpcUa_Field_Initialize(Boolean, UserExecutable);
    }
}

/*============================================================================
 * OpcUa_MethodAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_MethodAttributes_Clear(OpcUa_MethodAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Boolean, Executable);
        OpcUa_Field_Clear(Boolean, UserExecutable);
    }
}

/*============================================================================
 * OpcUa_MethodAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodAttributes_GetSize(OpcUa_MethodAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Boolean, Executable);
    OpcUa_Field_GetSize(Boolean, UserExecutable);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodAttributes_Encode(OpcUa_MethodAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Boolean, Executable);
    OpcUa_Field_Write(Boolean, UserExecutable);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MethodAttributes_Decode(OpcUa_MethodAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MethodAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_MethodAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Boolean, Executable);
    OpcUa_Field_Read(Boolean, UserExecutable);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_MethodAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MethodAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_MethodAttributes_EncodeableType =
{
    "MethodAttributes",
    OpcUaId_MethodAttributes,
    OpcUaId_MethodAttributes_Encoding_DefaultBinary,
    OpcUaId_MethodAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_MethodAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_MethodAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_MethodAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_MethodAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_MethodAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_MethodAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ObjectTypeAttributes
/*============================================================================
 * OpcUa_ObjectTypeAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectTypeAttributes_Initialize(OpcUa_ObjectTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_ObjectTypeAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ObjectTypeAttributes_Clear(OpcUa_ObjectTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_ObjectTypeAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeAttributes_GetSize(OpcUa_ObjectTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Boolean, IsAbstract);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeAttributes_Encode(OpcUa_ObjectTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ObjectTypeAttributes_Decode(OpcUa_ObjectTypeAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ObjectTypeAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ObjectTypeAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ObjectTypeAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ObjectTypeAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ObjectTypeAttributes_EncodeableType =
{
    "ObjectTypeAttributes",
    OpcUaId_ObjectTypeAttributes,
    OpcUaId_ObjectTypeAttributes_Encoding_DefaultBinary,
    OpcUaId_ObjectTypeAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ObjectTypeAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ObjectTypeAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ObjectTypeAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ObjectTypeAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ObjectTypeAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ObjectTypeAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_VariableTypeAttributes
/*============================================================================
 * OpcUa_VariableTypeAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_VariableTypeAttributes_Initialize(OpcUa_VariableTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Variant, Value);
        OpcUa_Field_Initialize(NodeId, DataType);
        OpcUa_Field_Initialize(Int32, ValueRank);
        OpcUa_Field_InitializeArray(UInt32, ArrayDimensions);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_VariableTypeAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_VariableTypeAttributes_Clear(OpcUa_VariableTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Variant, Value);
        OpcUa_Field_Clear(NodeId, DataType);
        OpcUa_Field_Clear(Int32, ValueRank);
        OpcUa_Field_ClearArray(UInt32, ArrayDimensions);
        OpcUa_Field_Clear(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_VariableTypeAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeAttributes_GetSize(OpcUa_VariableTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Variant, Value);
    OpcUa_Field_GetSize(NodeId, DataType);
    OpcUa_Field_GetSize(Int32, ValueRank);
    OpcUa_Field_GetSizeArray(UInt32, ArrayDimensions);
    OpcUa_Field_GetSize(Boolean, IsAbstract);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeAttributes_Encode(OpcUa_VariableTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Variant, Value);
    OpcUa_Field_Write(NodeId, DataType);
    OpcUa_Field_Write(Int32, ValueRank);
    OpcUa_Field_WriteArray(UInt32, ArrayDimensions);
    OpcUa_Field_Write(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_VariableTypeAttributes_Decode(OpcUa_VariableTypeAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "VariableTypeAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_VariableTypeAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Variant, Value);
    OpcUa_Field_Read(NodeId, DataType);
    OpcUa_Field_Read(Int32, ValueRank);
    OpcUa_Field_ReadArray(UInt32, ArrayDimensions);
    OpcUa_Field_Read(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_VariableTypeAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_VariableTypeAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_VariableTypeAttributes_EncodeableType =
{
    "VariableTypeAttributes",
    OpcUaId_VariableTypeAttributes,
    OpcUaId_VariableTypeAttributes_Encoding_DefaultBinary,
    OpcUaId_VariableTypeAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_VariableTypeAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_VariableTypeAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_VariableTypeAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_VariableTypeAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_VariableTypeAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_VariableTypeAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReferenceTypeAttributes
/*============================================================================
 * OpcUa_ReferenceTypeAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceTypeAttributes_Initialize(OpcUa_ReferenceTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
        OpcUa_Field_Initialize(Boolean, Symmetric);
        OpcUa_Field_Initialize(LocalizedText, InverseName);
    }
}

/*============================================================================
 * OpcUa_ReferenceTypeAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReferenceTypeAttributes_Clear(OpcUa_ReferenceTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Boolean, IsAbstract);
        OpcUa_Field_Clear(Boolean, Symmetric);
        OpcUa_Field_Clear(LocalizedText, InverseName);
    }
}

/*============================================================================
 * OpcUa_ReferenceTypeAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeAttributes_GetSize(OpcUa_ReferenceTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Boolean, IsAbstract);
    OpcUa_Field_GetSize(Boolean, Symmetric);
    OpcUa_Field_GetSize(LocalizedText, InverseName);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeAttributes_Encode(OpcUa_ReferenceTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Boolean, IsAbstract);
    OpcUa_Field_Write(Boolean, Symmetric);
    OpcUa_Field_Write(LocalizedText, InverseName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReferenceTypeAttributes_Decode(OpcUa_ReferenceTypeAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReferenceTypeAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReferenceTypeAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Boolean, IsAbstract);
    OpcUa_Field_Read(Boolean, Symmetric);
    OpcUa_Field_Read(LocalizedText, InverseName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReferenceTypeAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReferenceTypeAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReferenceTypeAttributes_EncodeableType =
{
    "ReferenceTypeAttributes",
    OpcUaId_ReferenceTypeAttributes,
    OpcUaId_ReferenceTypeAttributes_Encoding_DefaultBinary,
    OpcUaId_ReferenceTypeAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReferenceTypeAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReferenceTypeAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReferenceTypeAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReferenceTypeAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReferenceTypeAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReferenceTypeAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_DataTypeAttributes
/*============================================================================
 * OpcUa_DataTypeAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_DataTypeAttributes_Initialize(OpcUa_DataTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_DataTypeAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_DataTypeAttributes_Clear(OpcUa_DataTypeAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Boolean, IsAbstract);
    }
}

/*============================================================================
 * OpcUa_DataTypeAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeAttributes_GetSize(OpcUa_DataTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Boolean, IsAbstract);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeAttributes_Encode(OpcUa_DataTypeAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_DataTypeAttributes_Decode(OpcUa_DataTypeAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "DataTypeAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_DataTypeAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Boolean, IsAbstract);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_DataTypeAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_DataTypeAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_DataTypeAttributes_EncodeableType =
{
    "DataTypeAttributes",
    OpcUaId_DataTypeAttributes,
    OpcUaId_DataTypeAttributes_Encoding_DefaultBinary,
    OpcUaId_DataTypeAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_DataTypeAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_DataTypeAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_DataTypeAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_DataTypeAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_DataTypeAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_DataTypeAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ViewAttributes
/*============================================================================
 * OpcUa_ViewAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ViewAttributes_Initialize(OpcUa_ViewAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_Initialize(Boolean, ContainsNoLoops);
        OpcUa_Field_Initialize(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ViewAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ViewAttributes_Clear(OpcUa_ViewAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_Clear(Boolean, ContainsNoLoops);
        OpcUa_Field_Clear(Byte, EventNotifier);
    }
}

/*============================================================================
 * OpcUa_ViewAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewAttributes_GetSize(OpcUa_ViewAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSize(Boolean, ContainsNoLoops);
    OpcUa_Field_GetSize(Byte, EventNotifier);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewAttributes_Encode(OpcUa_ViewAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_Write(Boolean, ContainsNoLoops);
    OpcUa_Field_Write(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewAttributes_Decode(OpcUa_ViewAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ViewAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_Read(Boolean, ContainsNoLoops);
    OpcUa_Field_Read(Byte, EventNotifier);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ViewAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ViewAttributes_EncodeableType =
{
    "ViewAttributes",
    OpcUaId_ViewAttributes,
    OpcUaId_ViewAttributes_Encoding_DefaultBinary,
    OpcUaId_ViewAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ViewAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ViewAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ViewAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ViewAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ViewAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ViewAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GenericAttributeValue
/*============================================================================
 * OpcUa_GenericAttributeValue_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GenericAttributeValue_Initialize(OpcUa_GenericAttributeValue* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, AttributeId);
        OpcUa_Field_Initialize(Variant, Value);
    }
}

/*============================================================================
 * OpcUa_GenericAttributeValue_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GenericAttributeValue_Clear(OpcUa_GenericAttributeValue* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, AttributeId);
        OpcUa_Field_Clear(Variant, Value);
    }
}

/*============================================================================
 * OpcUa_GenericAttributeValue_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributeValue_GetSize(OpcUa_GenericAttributeValue* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributeValue_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, AttributeId);
    OpcUa_Field_GetSize(Variant, Value);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributeValue_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributeValue_Encode(OpcUa_GenericAttributeValue* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributeValue_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, AttributeId);
    OpcUa_Field_Write(Variant, Value);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributeValue_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributeValue_Decode(OpcUa_GenericAttributeValue* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributeValue_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GenericAttributeValue_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, AttributeId);
    OpcUa_Field_Read(Variant, Value);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GenericAttributeValue_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributeValue_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GenericAttributeValue_EncodeableType =
{
    "GenericAttributeValue",
    OpcUaId_GenericAttributeValue,
    OpcUaId_GenericAttributeValue_Encoding_DefaultBinary,
    OpcUaId_GenericAttributeValue_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GenericAttributeValue),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GenericAttributeValue_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GenericAttributeValue_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GenericAttributeValue_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GenericAttributeValue_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GenericAttributeValue_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GenericAttributes
/*============================================================================
 * OpcUa_GenericAttributes_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GenericAttributes_Initialize(OpcUa_GenericAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, SpecifiedAttributes);
        OpcUa_Field_Initialize(LocalizedText, DisplayName);
        OpcUa_Field_Initialize(LocalizedText, Description);
        OpcUa_Field_Initialize(UInt32, WriteMask);
        OpcUa_Field_Initialize(UInt32, UserWriteMask);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_GenericAttributeValue, AttributeValues);
    }
}

/*============================================================================
 * OpcUa_GenericAttributes_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GenericAttributes_Clear(OpcUa_GenericAttributes* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, SpecifiedAttributes);
        OpcUa_Field_Clear(LocalizedText, DisplayName);
        OpcUa_Field_Clear(LocalizedText, Description);
        OpcUa_Field_Clear(UInt32, WriteMask);
        OpcUa_Field_Clear(UInt32, UserWriteMask);
        OpcUa_Field_ClearEncodeableArray(OpcUa_GenericAttributeValue, AttributeValues);
    }
}

/*============================================================================
 * OpcUa_GenericAttributes_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributes_GetSize(OpcUa_GenericAttributes* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributes_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, SpecifiedAttributes);
    OpcUa_Field_GetSize(LocalizedText, DisplayName);
    OpcUa_Field_GetSize(LocalizedText, Description);
    OpcUa_Field_GetSize(UInt32, WriteMask);
    OpcUa_Field_GetSize(UInt32, UserWriteMask);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_GenericAttributeValue, AttributeValues);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributes_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributes_Encode(OpcUa_GenericAttributes* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributes_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, SpecifiedAttributes);
    OpcUa_Field_Write(LocalizedText, DisplayName);
    OpcUa_Field_Write(LocalizedText, Description);
    OpcUa_Field_Write(UInt32, WriteMask);
    OpcUa_Field_Write(UInt32, UserWriteMask);
    OpcUa_Field_WriteEncodeableArray(OpcUa_GenericAttributeValue, AttributeValues);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributes_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GenericAttributes_Decode(OpcUa_GenericAttributes* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GenericAttributes_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GenericAttributes_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, SpecifiedAttributes);
    OpcUa_Field_Read(LocalizedText, DisplayName);
    OpcUa_Field_Read(LocalizedText, Description);
    OpcUa_Field_Read(UInt32, WriteMask);
    OpcUa_Field_Read(UInt32, UserWriteMask);
    OpcUa_Field_ReadEncodeableArray(OpcUa_GenericAttributeValue, AttributeValues);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GenericAttributes_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GenericAttributes_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GenericAttributes_EncodeableType =
{
    "GenericAttributes",
    OpcUaId_GenericAttributes,
    OpcUaId_GenericAttributes_Encoding_DefaultBinary,
    OpcUaId_GenericAttributes_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GenericAttributes),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GenericAttributes_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GenericAttributes_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GenericAttributes_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GenericAttributes_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GenericAttributes_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_AttributeWriteMask
/*============================================================================
 * OpcUa_AttributeWriteMask_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_AttributeWriteMask_EnumeratedValues[] =
{
    { "None", 0 },
    { "AccessLevel", 1 },
    { "ArrayDimensions", 2 },
    { "BrowseName", 4 },
    { "ContainsNoLoops", 8 },
    { "DataType", 16 },
    { "Description", 32 },
    { "DisplayName", 64 },
    { "EventNotifier", 128 },
    { "Executable", 256 },
    { "Historizing", 512 },
    { "InverseName", 1024 },
    { "IsAbstract", 2048 },
    { "MinimumSamplingInterval", 4096 },
    { "NodeClass", 8192 },
    { "NodeId", 16384 },
    { "Symmetric", 32768 },
    { "UserAccessLevel", 65536 },
    { "UserExecutable", 131072 },
    { "UserWriteMask", 262144 },
    { "ValueRank", 524288 },
    { "WriteMask", 1048576 },
    { "ValueForVariableType", 2097152 },
    { "DataTypeDefinition", 4194304 },
    { "RolePermissions", 8388608 },
    { "AccessRestrictions", 16777216 },
    { "AccessLevelEx", 33554432 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_AttributeWriteMask_EnumeratedType =
{
    "AttributeWriteMask",
    g_OpcUa_AttributeWriteMask_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ViewDescription
/*============================================================================
 * OpcUa_ViewDescription_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ViewDescription_Initialize(OpcUa_ViewDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, ViewId);
        OpcUa_Field_Initialize(DateTime, Timestamp);
        OpcUa_Field_Initialize(UInt32, ViewVersion);
    }
}

/*============================================================================
 * OpcUa_ViewDescription_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ViewDescription_Clear(OpcUa_ViewDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, ViewId);
        OpcUa_Field_Clear(DateTime, Timestamp);
        OpcUa_Field_Clear(UInt32, ViewVersion);
    }
}

/*============================================================================
 * OpcUa_ViewDescription_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewDescription_GetSize(OpcUa_ViewDescription* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewDescription_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, ViewId);
    OpcUa_Field_GetSize(DateTime, Timestamp);
    OpcUa_Field_GetSize(UInt32, ViewVersion);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewDescription_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewDescription_Encode(OpcUa_ViewDescription* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewDescription_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, ViewId);
    OpcUa_Field_Write(DateTime, Timestamp);
    OpcUa_Field_Write(UInt32, ViewVersion);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewDescription_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ViewDescription_Decode(OpcUa_ViewDescription* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ViewDescription_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ViewDescription_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, ViewId);
    OpcUa_Field_Read(DateTime, Timestamp);
    OpcUa_Field_Read(UInt32, ViewVersion);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ViewDescription_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ViewDescription_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ViewDescription_EncodeableType =
{
    "ViewDescription",
    OpcUaId_ViewDescription,
    OpcUaId_ViewDescription_Encoding_DefaultBinary,
    OpcUaId_ViewDescription_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ViewDescription),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ViewDescription_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ViewDescription_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ViewDescription_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ViewDescription_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ViewDescription_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RelativePathElement
/*============================================================================
 * OpcUa_RelativePathElement_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RelativePathElement_Initialize(OpcUa_RelativePathElement* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, ReferenceTypeId);
        OpcUa_Field_Initialize(Boolean, IsInverse);
        OpcUa_Field_Initialize(Boolean, IncludeSubtypes);
        OpcUa_Field_Initialize(QualifiedName, TargetName);
    }
}

/*============================================================================
 * OpcUa_RelativePathElement_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RelativePathElement_Clear(OpcUa_RelativePathElement* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, ReferenceTypeId);
        OpcUa_Field_Clear(Boolean, IsInverse);
        OpcUa_Field_Clear(Boolean, IncludeSubtypes);
        OpcUa_Field_Clear(QualifiedName, TargetName);
    }
}

/*============================================================================
 * OpcUa_RelativePathElement_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePathElement_GetSize(OpcUa_RelativePathElement* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePathElement_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, ReferenceTypeId);
    OpcUa_Field_GetSize(Boolean, IsInverse);
    OpcUa_Field_GetSize(Boolean, IncludeSubtypes);
    OpcUa_Field_GetSize(QualifiedName, TargetName);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePathElement_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePathElement_Encode(OpcUa_RelativePathElement* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePathElement_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, ReferenceTypeId);
    OpcUa_Field_Write(Boolean, IsInverse);
    OpcUa_Field_Write(Boolean, IncludeSubtypes);
    OpcUa_Field_Write(QualifiedName, TargetName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePathElement_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePathElement_Decode(OpcUa_RelativePathElement* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePathElement_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RelativePathElement_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, ReferenceTypeId);
    OpcUa_Field_Read(Boolean, IsInverse);
    OpcUa_Field_Read(Boolean, IncludeSubtypes);
    OpcUa_Field_Read(QualifiedName, TargetName);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RelativePathElement_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePathElement_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RelativePathElement_EncodeableType =
{
    "RelativePathElement",
    OpcUaId_RelativePathElement,
    OpcUaId_RelativePathElement_Encoding_DefaultBinary,
    OpcUaId_RelativePathElement_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RelativePathElement),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RelativePathElement_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RelativePathElement_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RelativePathElement_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RelativePathElement_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RelativePathElement_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RelativePath
/*============================================================================
 * OpcUa_RelativePath_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RelativePath_Initialize(OpcUa_RelativePath* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeableArray(OpcUa_RelativePathElement, Elements);
    }
}

/*============================================================================
 * OpcUa_RelativePath_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RelativePath_Clear(OpcUa_RelativePath* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeableArray(OpcUa_RelativePathElement, Elements);
    }
}

/*============================================================================
 * OpcUa_RelativePath_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePath_GetSize(OpcUa_RelativePath* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePath_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeableArray(OpcUa_RelativePathElement, Elements);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePath_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePath_Encode(OpcUa_RelativePath* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePath_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeableArray(OpcUa_RelativePathElement, Elements);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePath_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RelativePath_Decode(OpcUa_RelativePath* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RelativePath_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RelativePath_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeableArray(OpcUa_RelativePathElement, Elements);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RelativePath_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RelativePath_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RelativePath_EncodeableType =
{
    "RelativePath",
    OpcUaId_RelativePath,
    OpcUaId_RelativePath_Encoding_DefaultBinary,
    OpcUaId_RelativePath_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RelativePath),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RelativePath_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RelativePath_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RelativePath_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RelativePath_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RelativePath_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_BrowsePath
/*============================================================================
 * OpcUa_BrowsePath_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_BrowsePath_Initialize(OpcUa_BrowsePath* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, StartingNode);
        OpcUa_Field_InitializeEncodeable(OpcUa_RelativePath, RelativePath);
    }
}

/*============================================================================
 * OpcUa_BrowsePath_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_BrowsePath_Clear(OpcUa_BrowsePath* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, StartingNode);
        OpcUa_Field_ClearEncodeable(OpcUa_RelativePath, RelativePath);
    }
}

/*============================================================================
 * OpcUa_BrowsePath_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePath_GetSize(OpcUa_BrowsePath* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePath_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, StartingNode);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RelativePath, RelativePath);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePath_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePath_Encode(OpcUa_BrowsePath* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePath_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, StartingNode);
    OpcUa_Field_WriteEncodeable(OpcUa_RelativePath, RelativePath);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePath_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePath_Decode(OpcUa_BrowsePath* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePath_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_BrowsePath_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, StartingNode);
    OpcUa_Field_ReadEncodeable(OpcUa_RelativePath, RelativePath);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_BrowsePath_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePath_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_BrowsePath_EncodeableType =
{
    "BrowsePath",
    OpcUaId_BrowsePath,
    OpcUaId_BrowsePath_Encoding_DefaultBinary,
    OpcUaId_BrowsePath_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_BrowsePath),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_BrowsePath_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_BrowsePath_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_BrowsePath_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_BrowsePath_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_BrowsePath_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_BrowsePathTarget
/*============================================================================
 * OpcUa_BrowsePathTarget_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_BrowsePathTarget_Initialize(OpcUa_BrowsePathTarget* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(ExpandedNodeId, TargetId);
        OpcUa_Field_Initialize(UInt32, RemainingPathIndex);
    }
}

/*============================================================================
 * OpcUa_BrowsePathTarget_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_BrowsePathTarget_Clear(OpcUa_BrowsePathTarget* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(ExpandedNodeId, TargetId);
        OpcUa_Field_Clear(UInt32, RemainingPathIndex);
    }
}

/*============================================================================
 * OpcUa_BrowsePathTarget_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePathTarget_GetSize(OpcUa_BrowsePathTarget* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePathTarget_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(ExpandedNodeId, TargetId);
    OpcUa_Field_GetSize(UInt32, RemainingPathIndex);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePathTarget_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePathTarget_Encode(OpcUa_BrowsePathTarget* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePathTarget_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(ExpandedNodeId, TargetId);
    OpcUa_Field_Write(UInt32, RemainingPathIndex);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePathTarget_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_BrowsePathTarget_Decode(OpcUa_BrowsePathTarget* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "BrowsePathTarget_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_BrowsePathTarget_Initialize(a_pValue);

    OpcUa_Field_Read(ExpandedNodeId, TargetId);
    OpcUa_Field_Read(UInt32, RemainingPathIndex);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_BrowsePathTarget_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_BrowsePathTarget_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_BrowsePathTarget_EncodeableType =
{
    "BrowsePathTarget",
    OpcUaId_BrowsePathTarget,
    OpcUaId_BrowsePathTarget_Encoding_DefaultBinary,
    OpcUaId_BrowsePathTarget_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_BrowsePathTarget),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_BrowsePathTarget_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_BrowsePathTarget_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_BrowsePathTarget_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_BrowsePathTarget_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_BrowsePathTarget_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EndpointConfiguration
/*============================================================================
 * OpcUa_EndpointConfiguration_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointConfiguration_Initialize(OpcUa_EndpointConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Int32, OperationTimeout);
        OpcUa_Field_Initialize(Boolean, UseBinaryEncoding);
        OpcUa_Field_Initialize(Int32, MaxStringLength);
        OpcUa_Field_Initialize(Int32, MaxByteStringLength);
        OpcUa_Field_Initialize(Int32, MaxArrayLength);
        OpcUa_Field_Initialize(Int32, MaxMessageSize);
        OpcUa_Field_Initialize(Int32, MaxBufferSize);
        OpcUa_Field_Initialize(Int32, ChannelLifetime);
        OpcUa_Field_Initialize(Int32, SecurityTokenLifetime);
    }
}

/*============================================================================
 * OpcUa_EndpointConfiguration_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointConfiguration_Clear(OpcUa_EndpointConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Int32, OperationTimeout);
        OpcUa_Field_Clear(Boolean, UseBinaryEncoding);
        OpcUa_Field_Clear(Int32, MaxStringLength);
        OpcUa_Field_Clear(Int32, MaxByteStringLength);
        OpcUa_Field_Clear(Int32, MaxArrayLength);
        OpcUa_Field_Clear(Int32, MaxMessageSize);
        OpcUa_Field_Clear(Int32, MaxBufferSize);
        OpcUa_Field_Clear(Int32, ChannelLifetime);
        OpcUa_Field_Clear(Int32, SecurityTokenLifetime);
    }
}

/*============================================================================
 * OpcUa_EndpointConfiguration_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointConfiguration_GetSize(OpcUa_EndpointConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointConfiguration_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Int32, OperationTimeout);
    OpcUa_Field_GetSize(Boolean, UseBinaryEncoding);
    OpcUa_Field_GetSize(Int32, MaxStringLength);
    OpcUa_Field_GetSize(Int32, MaxByteStringLength);
    OpcUa_Field_GetSize(Int32, MaxArrayLength);
    OpcUa_Field_GetSize(Int32, MaxMessageSize);
    OpcUa_Field_GetSize(Int32, MaxBufferSize);
    OpcUa_Field_GetSize(Int32, ChannelLifetime);
    OpcUa_Field_GetSize(Int32, SecurityTokenLifetime);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointConfiguration_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointConfiguration_Encode(OpcUa_EndpointConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointConfiguration_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Int32, OperationTimeout);
    OpcUa_Field_Write(Boolean, UseBinaryEncoding);
    OpcUa_Field_Write(Int32, MaxStringLength);
    OpcUa_Field_Write(Int32, MaxByteStringLength);
    OpcUa_Field_Write(Int32, MaxArrayLength);
    OpcUa_Field_Write(Int32, MaxMessageSize);
    OpcUa_Field_Write(Int32, MaxBufferSize);
    OpcUa_Field_Write(Int32, ChannelLifetime);
    OpcUa_Field_Write(Int32, SecurityTokenLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointConfiguration_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointConfiguration_Decode(OpcUa_EndpointConfiguration* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointConfiguration_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EndpointConfiguration_Initialize(a_pValue);

    OpcUa_Field_Read(Int32, OperationTimeout);
    OpcUa_Field_Read(Boolean, UseBinaryEncoding);
    OpcUa_Field_Read(Int32, MaxStringLength);
    OpcUa_Field_Read(Int32, MaxByteStringLength);
    OpcUa_Field_Read(Int32, MaxArrayLength);
    OpcUa_Field_Read(Int32, MaxMessageSize);
    OpcUa_Field_Read(Int32, MaxBufferSize);
    OpcUa_Field_Read(Int32, ChannelLifetime);
    OpcUa_Field_Read(Int32, SecurityTokenLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EndpointConfiguration_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointConfiguration_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EndpointConfiguration_EncodeableType =
{
    "EndpointConfiguration",
    OpcUaId_EndpointConfiguration,
    OpcUaId_EndpointConfiguration_Encoding_DefaultBinary,
    OpcUaId_EndpointConfiguration_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EndpointConfiguration),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EndpointConfiguration_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EndpointConfiguration_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EndpointConfiguration_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EndpointConfiguration_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EndpointConfiguration_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FilterOperator
/*============================================================================
 * OpcUa_FilterOperator_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_FilterOperator_EnumeratedValues[] =
{
    { "Equals", 0 },
    { "IsNull", 1 },
    { "GreaterThan", 2 },
    { "LessThan", 3 },
    { "GreaterThanOrEqual", 4 },
    { "LessThanOrEqual", 5 },
    { "Like", 6 },
    { "Not", 7 },
    { "Between", 8 },
    { "InList", 9 },
    { "And", 10 },
    { "Or", 11 },
    { "Cast", 12 },
    { "InView", 13 },
    { "OfType", 14 },
    { "RelatedTo", 15 },
    { "BitwiseAnd", 16 },
    { "BitwiseOr", 17 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_FilterOperator_EnumeratedType =
{
    "FilterOperator",
    g_OpcUa_FilterOperator_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_NodeReference
/*============================================================================
 * OpcUa_NodeReference_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_NodeReference_Initialize(OpcUa_NodeReference* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_Initialize(NodeId, ReferenceTypeId);
        OpcUa_Field_Initialize(Boolean, IsForward);
        OpcUa_Field_InitializeArray(NodeId, ReferencedNodeIds);
    }
}

/*============================================================================
 * OpcUa_NodeReference_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_NodeReference_Clear(OpcUa_NodeReference* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_Clear(NodeId, ReferenceTypeId);
        OpcUa_Field_Clear(Boolean, IsForward);
        OpcUa_Field_ClearArray(NodeId, ReferencedNodeIds);
    }
}

/*============================================================================
 * OpcUa_NodeReference_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeReference_GetSize(OpcUa_NodeReference* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeReference_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSize(NodeId, ReferenceTypeId);
    OpcUa_Field_GetSize(Boolean, IsForward);
    OpcUa_Field_GetSizeArray(NodeId, ReferencedNodeIds);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeReference_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeReference_Encode(OpcUa_NodeReference* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeReference_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_Write(NodeId, ReferenceTypeId);
    OpcUa_Field_Write(Boolean, IsForward);
    OpcUa_Field_WriteArray(NodeId, ReferencedNodeIds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeReference_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_NodeReference_Decode(OpcUa_NodeReference* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "NodeReference_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_NodeReference_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_Read(NodeId, ReferenceTypeId);
    OpcUa_Field_Read(Boolean, IsForward);
    OpcUa_Field_ReadArray(NodeId, ReferencedNodeIds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_NodeReference_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_NodeReference_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_NodeReference_EncodeableType =
{
    "NodeReference",
    OpcUaId_NodeReference,
    OpcUaId_NodeReference_Encoding_DefaultBinary,
    OpcUaId_NodeReference_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_NodeReference),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_NodeReference_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_NodeReference_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_NodeReference_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_NodeReference_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_NodeReference_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ContentFilterElement
/*============================================================================
 * OpcUa_ContentFilterElement_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterElement_Initialize(OpcUa_ContentFilterElement* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEnumerated(OpcUa_FilterOperator, FilterOperator);
        OpcUa_Field_InitializeArray(ExtensionObject, FilterOperands);
    }
}

/*============================================================================
 * OpcUa_ContentFilterElement_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterElement_Clear(OpcUa_ContentFilterElement* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEnumerated(OpcUa_FilterOperator, FilterOperator);
        OpcUa_Field_ClearArray(ExtensionObject, FilterOperands);
    }
}

/*============================================================================
 * OpcUa_ContentFilterElement_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElement_GetSize(OpcUa_ContentFilterElement* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElement_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEnumerated(OpcUa_FilterOperator, FilterOperator);
    OpcUa_Field_GetSizeArray(ExtensionObject, FilterOperands);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElement_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElement_Encode(OpcUa_ContentFilterElement* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElement_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEnumerated(OpcUa_FilterOperator, FilterOperator);
    OpcUa_Field_WriteArray(ExtensionObject, FilterOperands);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElement_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElement_Decode(OpcUa_ContentFilterElement* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElement_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ContentFilterElement_Initialize(a_pValue);

    OpcUa_Field_ReadEnumerated(OpcUa_FilterOperator, FilterOperator);
    OpcUa_Field_ReadArray(ExtensionObject, FilterOperands);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ContentFilterElement_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElement_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ContentFilterElement_EncodeableType =
{
    "ContentFilterElement",
    OpcUaId_ContentFilterElement,
    OpcUaId_ContentFilterElement_Encoding_DefaultBinary,
    OpcUaId_ContentFilterElement_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ContentFilterElement),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ContentFilterElement_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ContentFilterElement_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ContentFilterElement_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ContentFilterElement_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ContentFilterElement_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ContentFilter
/*============================================================================
 * OpcUa_ContentFilter_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilter_Initialize(OpcUa_ContentFilter* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ContentFilterElement, Elements);
    }
}

/*============================================================================
 * OpcUa_ContentFilter_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilter_Clear(OpcUa_ContentFilter* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeableArray(OpcUa_ContentFilterElement, Elements);
    }
}

/*============================================================================
 * OpcUa_ContentFilter_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilter_GetSize(OpcUa_ContentFilter* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilter_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ContentFilterElement, Elements);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilter_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilter_Encode(OpcUa_ContentFilter* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilter_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeableArray(OpcUa_ContentFilterElement, Elements);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilter_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilter_Decode(OpcUa_ContentFilter* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilter_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ContentFilter_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeableArray(OpcUa_ContentFilterElement, Elements);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ContentFilter_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilter_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ContentFilter_EncodeableType =
{
    "ContentFilter",
    OpcUaId_ContentFilter,
    OpcUaId_ContentFilter_Encoding_DefaultBinary,
    OpcUaId_ContentFilter_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ContentFilter),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ContentFilter_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ContentFilter_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ContentFilter_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ContentFilter_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ContentFilter_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ElementOperand
/*============================================================================
 * OpcUa_ElementOperand_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ElementOperand_Initialize(OpcUa_ElementOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, Index);
    }
}

/*============================================================================
 * OpcUa_ElementOperand_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ElementOperand_Clear(OpcUa_ElementOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, Index);
    }
}

/*============================================================================
 * OpcUa_ElementOperand_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ElementOperand_GetSize(OpcUa_ElementOperand* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ElementOperand_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, Index);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ElementOperand_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ElementOperand_Encode(OpcUa_ElementOperand* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ElementOperand_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, Index);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ElementOperand_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ElementOperand_Decode(OpcUa_ElementOperand* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ElementOperand_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ElementOperand_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, Index);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ElementOperand_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ElementOperand_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ElementOperand_EncodeableType =
{
    "ElementOperand",
    OpcUaId_ElementOperand,
    OpcUaId_ElementOperand_Encoding_DefaultBinary,
    OpcUaId_ElementOperand_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ElementOperand),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ElementOperand_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ElementOperand_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ElementOperand_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ElementOperand_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ElementOperand_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_LiteralOperand
/*============================================================================
 * OpcUa_LiteralOperand_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_LiteralOperand_Initialize(OpcUa_LiteralOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Variant, Value);
    }
}

/*============================================================================
 * OpcUa_LiteralOperand_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_LiteralOperand_Clear(OpcUa_LiteralOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Variant, Value);
    }
}

/*============================================================================
 * OpcUa_LiteralOperand_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_LiteralOperand_GetSize(OpcUa_LiteralOperand* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "LiteralOperand_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Variant, Value);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_LiteralOperand_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_LiteralOperand_Encode(OpcUa_LiteralOperand* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "LiteralOperand_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Variant, Value);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_LiteralOperand_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_LiteralOperand_Decode(OpcUa_LiteralOperand* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "LiteralOperand_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_LiteralOperand_Initialize(a_pValue);

    OpcUa_Field_Read(Variant, Value);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_LiteralOperand_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_LiteralOperand_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_LiteralOperand_EncodeableType =
{
    "LiteralOperand",
    OpcUaId_LiteralOperand,
    OpcUaId_LiteralOperand_Encoding_DefaultBinary,
    OpcUaId_LiteralOperand_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_LiteralOperand),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_LiteralOperand_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_LiteralOperand_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_LiteralOperand_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_LiteralOperand_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_LiteralOperand_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_AttributeOperand
/*============================================================================
 * OpcUa_AttributeOperand_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_AttributeOperand_Initialize(OpcUa_AttributeOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_Initialize(String, Alias);
        OpcUa_Field_InitializeEncodeable(OpcUa_RelativePath, BrowsePath);
        OpcUa_Field_Initialize(UInt32, AttributeId);
        OpcUa_Field_Initialize(String, IndexRange);
    }
}

/*============================================================================
 * OpcUa_AttributeOperand_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_AttributeOperand_Clear(OpcUa_AttributeOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_Clear(String, Alias);
        OpcUa_Field_ClearEncodeable(OpcUa_RelativePath, BrowsePath);
        OpcUa_Field_Clear(UInt32, AttributeId);
        OpcUa_Field_Clear(String, IndexRange);
    }
}

/*============================================================================
 * OpcUa_AttributeOperand_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AttributeOperand_GetSize(OpcUa_AttributeOperand* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AttributeOperand_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSize(String, Alias);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RelativePath, BrowsePath);
    OpcUa_Field_GetSize(UInt32, AttributeId);
    OpcUa_Field_GetSize(String, IndexRange);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AttributeOperand_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AttributeOperand_Encode(OpcUa_AttributeOperand* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AttributeOperand_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_Write(String, Alias);
    OpcUa_Field_WriteEncodeable(OpcUa_RelativePath, BrowsePath);
    OpcUa_Field_Write(UInt32, AttributeId);
    OpcUa_Field_Write(String, IndexRange);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AttributeOperand_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AttributeOperand_Decode(OpcUa_AttributeOperand* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AttributeOperand_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_AttributeOperand_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_Read(String, Alias);
    OpcUa_Field_ReadEncodeable(OpcUa_RelativePath, BrowsePath);
    OpcUa_Field_Read(UInt32, AttributeId);
    OpcUa_Field_Read(String, IndexRange);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_AttributeOperand_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AttributeOperand_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_AttributeOperand_EncodeableType =
{
    "AttributeOperand",
    OpcUaId_AttributeOperand,
    OpcUaId_AttributeOperand_Encoding_DefaultBinary,
    OpcUaId_AttributeOperand_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_AttributeOperand),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_AttributeOperand_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_AttributeOperand_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_AttributeOperand_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_AttributeOperand_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_AttributeOperand_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_SimpleAttributeOperand
/*============================================================================
 * OpcUa_SimpleAttributeOperand_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_SimpleAttributeOperand_Initialize(OpcUa_SimpleAttributeOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, TypeDefinitionId);
        OpcUa_Field_InitializeArray(QualifiedName, BrowsePath);
        OpcUa_Field_Initialize(UInt32, AttributeId);
        OpcUa_Field_Initialize(String, IndexRange);
    }
}

/*============================================================================
 * OpcUa_SimpleAttributeOperand_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_SimpleAttributeOperand_Clear(OpcUa_SimpleAttributeOperand* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, TypeDefinitionId);
        OpcUa_Field_ClearArray(QualifiedName, BrowsePath);
        OpcUa_Field_Clear(UInt32, AttributeId);
        OpcUa_Field_Clear(String, IndexRange);
    }
}

/*============================================================================
 * OpcUa_SimpleAttributeOperand_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SimpleAttributeOperand_GetSize(OpcUa_SimpleAttributeOperand* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SimpleAttributeOperand_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, TypeDefinitionId);
    OpcUa_Field_GetSizeArray(QualifiedName, BrowsePath);
    OpcUa_Field_GetSize(UInt32, AttributeId);
    OpcUa_Field_GetSize(String, IndexRange);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SimpleAttributeOperand_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SimpleAttributeOperand_Encode(OpcUa_SimpleAttributeOperand* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SimpleAttributeOperand_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, TypeDefinitionId);
    OpcUa_Field_WriteArray(QualifiedName, BrowsePath);
    OpcUa_Field_Write(UInt32, AttributeId);
    OpcUa_Field_Write(String, IndexRange);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SimpleAttributeOperand_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_SimpleAttributeOperand_Decode(OpcUa_SimpleAttributeOperand* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "SimpleAttributeOperand_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_SimpleAttributeOperand_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, TypeDefinitionId);
    OpcUa_Field_ReadArray(QualifiedName, BrowsePath);
    OpcUa_Field_Read(UInt32, AttributeId);
    OpcUa_Field_Read(String, IndexRange);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_SimpleAttributeOperand_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_SimpleAttributeOperand_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_SimpleAttributeOperand_EncodeableType =
{
    "SimpleAttributeOperand",
    OpcUaId_SimpleAttributeOperand,
    OpcUaId_SimpleAttributeOperand_Encoding_DefaultBinary,
    OpcUaId_SimpleAttributeOperand_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_SimpleAttributeOperand),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_SimpleAttributeOperand_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_SimpleAttributeOperand_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_SimpleAttributeOperand_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_SimpleAttributeOperand_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_SimpleAttributeOperand_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ContentFilterElementResult
/*============================================================================
 * OpcUa_ContentFilterElementResult_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterElementResult_Initialize(OpcUa_ContentFilterElementResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(StatusCode, StatusCode);
        OpcUa_Field_InitializeArray(StatusCode, OperandStatusCodes);
        OpcUa_Field_InitializeArray(DiagnosticInfo, OperandDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ContentFilterElementResult_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterElementResult_Clear(OpcUa_ContentFilterElementResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(StatusCode, StatusCode);
        OpcUa_Field_ClearArray(StatusCode, OperandStatusCodes);
        OpcUa_Field_ClearArray(DiagnosticInfo, OperandDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ContentFilterElementResult_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElementResult_GetSize(OpcUa_ContentFilterElementResult* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElementResult_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(StatusCode, StatusCode);
    OpcUa_Field_GetSizeArray(StatusCode, OperandStatusCodes);
    OpcUa_Field_GetSizeArray(DiagnosticInfo, OperandDiagnosticInfos);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElementResult_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElementResult_Encode(OpcUa_ContentFilterElementResult* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElementResult_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(StatusCode, StatusCode);
    OpcUa_Field_WriteArray(StatusCode, OperandStatusCodes);
    OpcUa_Field_WriteArray(DiagnosticInfo, OperandDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElementResult_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterElementResult_Decode(OpcUa_ContentFilterElementResult* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterElementResult_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ContentFilterElementResult_Initialize(a_pValue);

    OpcUa_Field_Read(StatusCode, StatusCode);
    OpcUa_Field_ReadArray(StatusCode, OperandStatusCodes);
    OpcUa_Field_ReadArray(DiagnosticInfo, OperandDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ContentFilterElementResult_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterElementResult_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ContentFilterElementResult_EncodeableType =
{
    "ContentFilterElementResult",
    OpcUaId_ContentFilterElementResult,
    OpcUaId_ContentFilterElementResult_Encoding_DefaultBinary,
    OpcUaId_ContentFilterElementResult_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ContentFilterElementResult),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ContentFilterElementResult_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ContentFilterElementResult_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ContentFilterElementResult_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ContentFilterElementResult_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ContentFilterElementResult_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ContentFilterResult
/*============================================================================
 * OpcUa_ContentFilterResult_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterResult_Initialize(OpcUa_ContentFilterResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ContentFilterElementResult, ElementResults);
        OpcUa_Field_InitializeArray(DiagnosticInfo, ElementDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ContentFilterResult_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ContentFilterResult_Clear(OpcUa_ContentFilterResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeableArray(OpcUa_ContentFilterElementResult, ElementResults);
        OpcUa_Field_ClearArray(DiagnosticInfo, ElementDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ContentFilterResult_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterResult_GetSize(OpcUa_ContentFilterResult* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterResult_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ContentFilterElementResult, ElementResults);
    OpcUa_Field_GetSizeArray(DiagnosticInfo, ElementDiagnosticInfos);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterResult_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterResult_Encode(OpcUa_ContentFilterResult* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterResult_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeableArray(OpcUa_ContentFilterElementResult, ElementResults);
    OpcUa_Field_WriteArray(DiagnosticInfo, ElementDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterResult_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ContentFilterResult_Decode(OpcUa_ContentFilterResult* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ContentFilterResult_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ContentFilterResult_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeableArray(OpcUa_ContentFilterElementResult, ElementResults);
    OpcUa_Field_ReadArray(DiagnosticInfo, ElementDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ContentFilterResult_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ContentFilterResult_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ContentFilterResult_EncodeableType =
{
    "ContentFilterResult",
    OpcUaId_ContentFilterResult,
    OpcUaId_ContentFilterResult_Encoding_DefaultBinary,
    OpcUaId_ContentFilterResult_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ContentFilterResult),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ContentFilterResult_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ContentFilterResult_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ContentFilterResult_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ContentFilterResult_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ContentFilterResult_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ParsingResult
/*============================================================================
 * OpcUa_ParsingResult_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ParsingResult_Initialize(OpcUa_ParsingResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(StatusCode, StatusCode);
        OpcUa_Field_InitializeArray(StatusCode, DataStatusCodes);
        OpcUa_Field_InitializeArray(DiagnosticInfo, DataDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ParsingResult_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ParsingResult_Clear(OpcUa_ParsingResult* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(StatusCode, StatusCode);
        OpcUa_Field_ClearArray(StatusCode, DataStatusCodes);
        OpcUa_Field_ClearArray(DiagnosticInfo, DataDiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_ParsingResult_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ParsingResult_GetSize(OpcUa_ParsingResult* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ParsingResult_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(StatusCode, StatusCode);
    OpcUa_Field_GetSizeArray(StatusCode, DataStatusCodes);
    OpcUa_Field_GetSizeArray(DiagnosticInfo, DataDiagnosticInfos);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ParsingResult_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ParsingResult_Encode(OpcUa_ParsingResult* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ParsingResult_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(StatusCode, StatusCode);
    OpcUa_Field_WriteArray(StatusCode, DataStatusCodes);
    OpcUa_Field_WriteArray(DiagnosticInfo, DataDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ParsingResult_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ParsingResult_Decode(OpcUa_ParsingResult* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ParsingResult_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ParsingResult_Initialize(a_pValue);

    OpcUa_Field_Read(StatusCode, StatusCode);
    OpcUa_Field_ReadArray(StatusCode, DataStatusCodes);
    OpcUa_Field_ReadArray(DiagnosticInfo, DataDiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ParsingResult_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ParsingResult_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ParsingResult_EncodeableType =
{
    "ParsingResult",
    OpcUaId_ParsingResult,
    OpcUaId_ParsingResult_Encoding_DefaultBinary,
    OpcUaId_ParsingResult_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ParsingResult),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ParsingResult_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ParsingResult_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ParsingResult_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ParsingResult_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ParsingResult_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_TimestampsToReturn
/*============================================================================
 * OpcUa_TimestampsToReturn_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_TimestampsToReturn_EnumeratedValues[] =
{
    { "Source", 0 },
    { "Server", 1 },
    { "Both", 2 },
    { "Neither", 3 },
    { "Invalid", 4 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_TimestampsToReturn_EnumeratedType =
{
    "TimestampsToReturn",
    g_OpcUa_TimestampsToReturn_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ReadValueId
/*============================================================================
 * OpcUa_ReadValueId_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReadValueId_Initialize(OpcUa_ReadValueId* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, NodeId);
        OpcUa_Field_Initialize(UInt32, AttributeId);
        OpcUa_Field_Initialize(String, IndexRange);
        OpcUa_Field_Initialize(QualifiedName, DataEncoding);
    }
}

/*============================================================================
 * OpcUa_ReadValueId_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReadValueId_Clear(OpcUa_ReadValueId* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, NodeId);
        OpcUa_Field_Clear(UInt32, AttributeId);
        OpcUa_Field_Clear(String, IndexRange);
        OpcUa_Field_Clear(QualifiedName, DataEncoding);
    }
}

/*============================================================================
 * OpcUa_ReadValueId_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadValueId_GetSize(OpcUa_ReadValueId* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadValueId_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, NodeId);
    OpcUa_Field_GetSize(UInt32, AttributeId);
    OpcUa_Field_GetSize(String, IndexRange);
    OpcUa_Field_GetSize(QualifiedName, DataEncoding);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadValueId_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadValueId_Encode(OpcUa_ReadValueId* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadValueId_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, NodeId);
    OpcUa_Field_Write(UInt32, AttributeId);
    OpcUa_Field_Write(String, IndexRange);
    OpcUa_Field_Write(QualifiedName, DataEncoding);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadValueId_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadValueId_Decode(OpcUa_ReadValueId* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadValueId_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReadValueId_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, NodeId);
    OpcUa_Field_Read(UInt32, AttributeId);
    OpcUa_Field_Read(String, IndexRange);
    OpcUa_Field_Read(QualifiedName, DataEncoding);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReadValueId_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadValueId_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReadValueId_EncodeableType =
{
    "ReadValueId",
    OpcUaId_ReadValueId,
    OpcUaId_ReadValueId_Encoding_DefaultBinary,
    OpcUaId_ReadValueId_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReadValueId),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReadValueId_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReadValueId_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReadValueId_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReadValueId_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReadValueId_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReadEventDetails
/*============================================================================
 * OpcUa_ReadEventDetails_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReadEventDetails_Initialize(OpcUa_ReadEventDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, NumValuesPerNode);
        OpcUa_Field_Initialize(DateTime, StartTime);
        OpcUa_Field_Initialize(DateTime, EndTime);
        OpcUa_Field_InitializeEncodeable(OpcUa_EventFilter, Filter);
    }
}

/*============================================================================
 * OpcUa_ReadEventDetails_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReadEventDetails_Clear(OpcUa_ReadEventDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, NumValuesPerNode);
        OpcUa_Field_Clear(DateTime, StartTime);
        OpcUa_Field_Clear(DateTime, EndTime);
        OpcUa_Field_ClearEncodeable(OpcUa_EventFilter, Filter);
    }
}

/*============================================================================
 * OpcUa_ReadEventDetails_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadEventDetails_GetSize(OpcUa_ReadEventDetails* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadEventDetails_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, NumValuesPerNode);
    OpcUa_Field_GetSize(DateTime, StartTime);
    OpcUa_Field_GetSize(DateTime, EndTime);
    OpcUa_Field_GetSizeEncodeable(OpcUa_EventFilter, Filter);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadEventDetails_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadEventDetails_Encode(OpcUa_ReadEventDetails* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadEventDetails_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, NumValuesPerNode);
    OpcUa_Field_Write(DateTime, StartTime);
    OpcUa_Field_Write(DateTime, EndTime);
    OpcUa_Field_WriteEncodeable(OpcUa_EventFilter, Filter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadEventDetails_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadEventDetails_Decode(OpcUa_ReadEventDetails* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadEventDetails_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReadEventDetails_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, NumValuesPerNode);
    OpcUa_Field_Read(DateTime, StartTime);
    OpcUa_Field_Read(DateTime, EndTime);
    OpcUa_Field_ReadEncodeable(OpcUa_EventFilter, Filter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReadEventDetails_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadEventDetails_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReadEventDetails_EncodeableType =
{
    "ReadEventDetails",
    OpcUaId_ReadEventDetails,
    OpcUaId_ReadEventDetails_Encoding_DefaultBinary,
    OpcUaId_ReadEventDetails_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReadEventDetails),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReadEventDetails_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReadEventDetails_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReadEventDetails_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReadEventDetails_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReadEventDetails_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReadRawModifiedDetails
/*============================================================================
 * OpcUa_ReadRawModifiedDetails_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReadRawModifiedDetails_Initialize(OpcUa_ReadRawModifiedDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Boolean, IsReadModified);
        OpcUa_Field_Initialize(DateTime, StartTime);
        OpcUa_Field_Initialize(DateTime, EndTime);
        OpcUa_Field_Initialize(UInt32, NumValuesPerNode);
        OpcUa_Field_Initialize(Boolean, ReturnBounds);
    }
}

/*============================================================================
 * OpcUa_ReadRawModifiedDetails_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReadRawModifiedDetails_Clear(OpcUa_ReadRawModifiedDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Boolean, IsReadModified);
        OpcUa_Field_Clear(DateTime, StartTime);
        OpcUa_Field_Clear(DateTime, EndTime);
        OpcUa_Field_Clear(UInt32, NumValuesPerNode);
        OpcUa_Field_Clear(Boolean, ReturnBounds);
    }
}

/*============================================================================
 * OpcUa_ReadRawModifiedDetails_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadRawModifiedDetails_GetSize(OpcUa_ReadRawModifiedDetails* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadRawModifiedDetails_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Boolean, IsReadModified);
    OpcUa_Field_GetSize(DateTime, StartTime);
    OpcUa_Field_GetSize(DateTime, EndTime);
    OpcUa_Field_GetSize(UInt32, NumValuesPerNode);
    OpcUa_Field_GetSize(Boolean, ReturnBounds);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadRawModifiedDetails_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadRawModifiedDetails_Encode(OpcUa_ReadRawModifiedDetails* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadRawModifiedDetails_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Boolean, IsReadModified);
    OpcUa_Field_Write(DateTime, StartTime);
    OpcUa_Field_Write(DateTime, EndTime);
    OpcUa_Field_Write(UInt32, NumValuesPerNode);
    OpcUa_Field_Write(Boolean, ReturnBounds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadRawModifiedDetails_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadRawModifiedDetails_Decode(OpcUa_ReadRawModifiedDetails* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadRawModifiedDetails_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReadRawModifiedDetails_Initialize(a_pValue);

    OpcUa_Field_Read(Boolean, IsReadModified);
    OpcUa_Field_Read(DateTime, StartTime);
    OpcUa_Field_Read(DateTime, EndTime);
    OpcUa_Field_Read(UInt32, NumValuesPerNode);
    OpcUa_Field_Read(Boolean, ReturnBounds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReadRawModifiedDetails_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadRawModifiedDetails_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReadRawModifiedDetails_EncodeableType =
{
    "ReadRawModifiedDetails",
    OpcUaId_ReadRawModifiedDetails,
    OpcUaId_ReadRawModifiedDetails_Encoding_DefaultBinary,
    OpcUaId_ReadRawModifiedDetails_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReadRawModifiedDetails),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReadRawModifiedDetails_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReadRawModifiedDetails_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReadRawModifiedDetails_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReadRawModifiedDetails_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReadRawModifiedDetails_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReadProcessedDetails
/*============================================================================
 * OpcUa_ReadProcessedDetails_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReadProcessedDetails_Initialize(OpcUa_ReadProcessedDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(DateTime, StartTime);
        OpcUa_Field_Initialize(DateTime, EndTime);
        OpcUa_Field_Initialize(Double, ProcessingInterval);
        OpcUa_Field_InitializeArray(NodeId, AggregateType);
        OpcUa_Field_InitializeEncodeable(OpcUa_AggregateConfiguration, AggregateConfiguration);
    }
}

/*============================================================================
 * OpcUa_ReadProcessedDetails_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReadProcessedDetails_Clear(OpcUa_ReadProcessedDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(DateTime, StartTime);
        OpcUa_Field_Clear(DateTime, EndTime);
        OpcUa_Field_Clear(Double, ProcessingInterval);
        OpcUa_Field_ClearArray(NodeId, AggregateType);
        OpcUa_Field_ClearEncodeable(OpcUa_AggregateConfiguration, AggregateConfiguration);
    }
}

/*============================================================================
 * OpcUa_ReadProcessedDetails_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadProcessedDetails_GetSize(OpcUa_ReadProcessedDetails* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadProcessedDetails_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(DateTime, StartTime);
    OpcUa_Field_GetSize(DateTime, EndTime);
    OpcUa_Field_GetSize(Double, ProcessingInterval);
    OpcUa_Field_GetSizeArray(NodeId, AggregateType);
    OpcUa_Field_GetSizeEncodeable(OpcUa_AggregateConfiguration, AggregateConfiguration);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadProcessedDetails_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadProcessedDetails_Encode(OpcUa_ReadProcessedDetails* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadProcessedDetails_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(DateTime, StartTime);
    OpcUa_Field_Write(DateTime, EndTime);
    OpcUa_Field_Write(Double, ProcessingInterval);
    OpcUa_Field_WriteArray(NodeId, AggregateType);
    OpcUa_Field_WriteEncodeable(OpcUa_AggregateConfiguration, AggregateConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadProcessedDetails_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadProcessedDetails_Decode(OpcUa_ReadProcessedDetails* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadProcessedDetails_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReadProcessedDetails_Initialize(a_pValue);

    OpcUa_Field_Read(DateTime, StartTime);
    OpcUa_Field_Read(DateTime, EndTime);
    OpcUa_Field_Read(Double, ProcessingInterval);
    OpcUa_Field_ReadArray(NodeId, AggregateType);
    OpcUa_Field_ReadEncodeable(OpcUa_AggregateConfiguration, AggregateConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReadProcessedDetails_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadProcessedDetails_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReadProcessedDetails_EncodeableType =
{
    "ReadProcessedDetails",
    OpcUaId_ReadProcessedDetails,
    OpcUaId_ReadProcessedDetails_Encoding_DefaultBinary,
    OpcUaId_ReadProcessedDetails_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReadProcessedDetails),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReadProcessedDetails_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReadProcessedDetails_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReadProcessedDetails_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReadProcessedDetails_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReadProcessedDetails_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ReadAtTimeDetails
/*============================================================================
 * OpcUa_ReadAtTimeDetails_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ReadAtTimeDetails_Initialize(OpcUa_ReadAtTimeDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeArray(DateTime, ReqTimes);
        OpcUa_Field_Initialize(Boolean, UseSimpleBounds);
    }
}

/*============================================================================
 * OpcUa_ReadAtTimeDetails_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ReadAtTimeDetails_Clear(OpcUa_ReadAtTimeDetails* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearArray(DateTime, ReqTimes);
        OpcUa_Field_Clear(Boolean, UseSimpleBounds);
    }
}

/*============================================================================
 * OpcUa_ReadAtTimeDetails_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadAtTimeDetails_GetSize(OpcUa_ReadAtTimeDetails* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadAtTimeDetails_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeArray(DateTime, ReqTimes);
    OpcUa_Field_GetSize(Boolean, UseSimpleBounds);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadAtTimeDetails_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadAtTimeDetails_Encode(OpcUa_ReadAtTimeDetails* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadAtTimeDetails_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteArray(DateTime, ReqTimes);
    OpcUa_Field_Write(Boolean, UseSimpleBounds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadAtTimeDetails_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ReadAtTimeDetails_Decode(OpcUa_ReadAtTimeDetails* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ReadAtTimeDetails_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ReadAtTimeDetails_Initialize(a_pValue);

    OpcUa_Field_ReadArray(DateTime, ReqTimes);
    OpcUa_Field_Read(Boolean, UseSimpleBounds);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ReadAtTimeDetails_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ReadAtTimeDetails_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ReadAtTimeDetails_EncodeableType =
{
    "ReadAtTimeDetails",
    OpcUaId_ReadAtTimeDetails,
    OpcUaId_ReadAtTimeDetails_Encoding_DefaultBinary,
    OpcUaId_ReadAtTimeDetails_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ReadAtTimeDetails),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ReadAtTimeDetails_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ReadAtTimeDetails_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ReadAtTimeDetails_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ReadAtTimeDetails_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ReadAtTimeDetails_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_PerformUpdateType
/*============================================================================
 * OpcUa_PerformUpdateType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_PerformUpdateType_EnumeratedValues[] =
{
    { "Insert", 1 },
    { "Replace", 2 },
    { "Update", 3 },
    { "Remove", 4 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_PerformUpdateType_EnumeratedType =
{
    "PerformUpdateType",
    g_OpcUa_PerformUpdateType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_EventFilter
/*============================================================================
 * OpcUa_EventFilter_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EventFilter_Initialize(OpcUa_EventFilter* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeableArray(OpcUa_SimpleAttributeOperand, SelectClauses);
        OpcUa_Field_InitializeEncodeable(OpcUa_ContentFilter, WhereClause);
    }
}

/*============================================================================
 * OpcUa_EventFilter_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EventFilter_Clear(OpcUa_EventFilter* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeableArray(OpcUa_SimpleAttributeOperand, SelectClauses);
        OpcUa_Field_ClearEncodeable(OpcUa_ContentFilter, WhereClause);
    }
}

/*============================================================================
 * OpcUa_EventFilter_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EventFilter_GetSize(OpcUa_EventFilter* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EventFilter_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeableArray(OpcUa_SimpleAttributeOperand, SelectClauses);
    OpcUa_Field_GetSizeEncodeable(OpcUa_ContentFilter, WhereClause);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EventFilter_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EventFilter_Encode(OpcUa_EventFilter* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EventFilter_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeableArray(OpcUa_SimpleAttributeOperand, SelectClauses);
    OpcUa_Field_WriteEncodeable(OpcUa_ContentFilter, WhereClause);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EventFilter_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EventFilter_Decode(OpcUa_EventFilter* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EventFilter_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EventFilter_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeableArray(OpcUa_SimpleAttributeOperand, SelectClauses);
    OpcUa_Field_ReadEncodeable(OpcUa_ContentFilter, WhereClause);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EventFilter_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EventFilter_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EventFilter_EncodeableType =
{
    "EventFilter",
    OpcUaId_EventFilter,
    OpcUaId_EventFilter_Encoding_DefaultBinary,
    OpcUaId_EventFilter_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EventFilter),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EventFilter_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EventFilter_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EventFilter_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EventFilter_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EventFilter_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_AggregateConfiguration
/*============================================================================
 * OpcUa_AggregateConfiguration_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_AggregateConfiguration_Initialize(OpcUa_AggregateConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(Boolean, UseServerCapabilitiesDefaults);
        OpcUa_Field_Initialize(Boolean, TreatUncertainAsBad);
        OpcUa_Field_Initialize(Byte, PercentDataBad);
        OpcUa_Field_Initialize(Byte, PercentDataGood);
        OpcUa_Field_Initialize(Boolean, UseSlopedExtrapolation);
    }
}

/*============================================================================
 * OpcUa_AggregateConfiguration_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_AggregateConfiguration_Clear(OpcUa_AggregateConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(Boolean, UseServerCapabilitiesDefaults);
        OpcUa_Field_Clear(Boolean, TreatUncertainAsBad);
        OpcUa_Field_Clear(Byte, PercentDataBad);
        OpcUa_Field_Clear(Byte, PercentDataGood);
        OpcUa_Field_Clear(Boolean, UseSlopedExtrapolation);
    }
}

/*============================================================================
 * OpcUa_AggregateConfiguration_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AggregateConfiguration_GetSize(OpcUa_AggregateConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AggregateConfiguration_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(Boolean, UseServerCapabilitiesDefaults);
    OpcUa_Field_GetSize(Boolean, TreatUncertainAsBad);
    OpcUa_Field_GetSize(Byte, PercentDataBad);
    OpcUa_Field_GetSize(Byte, PercentDataGood);
    OpcUa_Field_GetSize(Boolean, UseSlopedExtrapolation);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AggregateConfiguration_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AggregateConfiguration_Encode(OpcUa_AggregateConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AggregateConfiguration_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(Boolean, UseServerCapabilitiesDefaults);
    OpcUa_Field_Write(Boolean, TreatUncertainAsBad);
    OpcUa_Field_Write(Byte, PercentDataBad);
    OpcUa_Field_Write(Byte, PercentDataGood);
    OpcUa_Field_Write(Boolean, UseSlopedExtrapolation);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AggregateConfiguration_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_AggregateConfiguration_Decode(OpcUa_AggregateConfiguration* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "AggregateConfiguration_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_AggregateConfiguration_Initialize(a_pValue);

    OpcUa_Field_Read(Boolean, UseServerCapabilitiesDefaults);
    OpcUa_Field_Read(Boolean, TreatUncertainAsBad);
    OpcUa_Field_Read(Byte, PercentDataBad);
    OpcUa_Field_Read(Byte, PercentDataGood);
    OpcUa_Field_Read(Boolean, UseSlopedExtrapolation);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_AggregateConfiguration_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_AggregateConfiguration_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_AggregateConfiguration_EncodeableType =
{
    "AggregateConfiguration",
    OpcUaId_AggregateConfiguration,
    OpcUaId_AggregateConfiguration_Encoding_DefaultBinary,
    OpcUaId_AggregateConfiguration_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_AggregateConfiguration),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_AggregateConfiguration_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_AggregateConfiguration_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_AggregateConfiguration_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_AggregateConfiguration_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_AggregateConfiguration_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ExceptionDeviationFormat
/*============================================================================
 * OpcUa_ExceptionDeviationFormat_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_ExceptionDeviationFormat_EnumeratedValues[] =
{
    { "AbsoluteValue", 0 },
    { "PercentOfValue", 1 },
    { "PercentOfRange", 2 },
    { "PercentOfEURange", 3 },
    { "Unknown", 4 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_ExceptionDeviationFormat_EnumeratedType =
{
    "ExceptionDeviationFormat",
    g_OpcUa_ExceptionDeviationFormat_EnumeratedValues
};
#endif

/*============================================================================
 * Table of known types.
 *===========================================================================*/
static OpcUa_EncodeableType* g_OpcUa_KnownEncodeableTypes[] =
{
    #ifndef OPCUA_EXCLUDE_RolePermissionType
    &OpcUa_RolePermissionType_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_StructureField
    &OpcUa_StructureField_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_StructureDefinition
    &OpcUa_StructureDefinition_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EnumDefinition
    &OpcUa_EnumDefinition_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_Node
    &OpcUa_Node_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_InstanceNode
    &OpcUa_InstanceNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_TypeNode
    &OpcUa_TypeNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ObjectNode
    &OpcUa_ObjectNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ObjectTypeNode
    &OpcUa_ObjectTypeNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_VariableNode
    &OpcUa_VariableNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_VariableTypeNode
    &OpcUa_VariableTypeNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReferenceTypeNode
    &OpcUa_ReferenceTypeNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_MethodNode
    &OpcUa_MethodNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ViewNode
    &OpcUa_ViewNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_DataTypeNode
    &OpcUa_DataTypeNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReferenceNode
    &OpcUa_ReferenceNode_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_Argument
    &OpcUa_Argument_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EnumValueType
    &OpcUa_EnumValueType_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EnumField
    &OpcUa_EnumField_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_OptionSet
    &OpcUa_OptionSet_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_TimeZoneDataType
    &OpcUa_TimeZoneDataType_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ApplicationDescription
    &OpcUa_ApplicationDescription_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RequestHeader
    &OpcUa_RequestHeader_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ResponseHeader
    &OpcUa_ResponseHeader_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ServiceFault
    &OpcUa_ServiceFault_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_SessionlessInvokeRequestType
    &OpcUa_SessionlessInvokeRequestType_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_SessionlessInvokeResponseType
    &OpcUa_SessionlessInvokeResponseType_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_FindServers
    &OpcUa_FindServersRequest_EncodeableType,
    &OpcUa_FindServersResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ServerOnNetwork
    &OpcUa_ServerOnNetwork_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_FindServersOnNetwork
    &OpcUa_FindServersOnNetworkRequest_EncodeableType,
    &OpcUa_FindServersOnNetworkResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_UserTokenPolicy
    &OpcUa_UserTokenPolicy_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EndpointDescription
    &OpcUa_EndpointDescription_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_GetEndpoints
    &OpcUa_GetEndpointsRequest_EncodeableType,
    &OpcUa_GetEndpointsResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisteredServer
    &OpcUa_RegisteredServer_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer
    &OpcUa_RegisterServerRequest_EncodeableType,
    &OpcUa_RegisterServerResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_MdnsDiscoveryConfiguration
    &OpcUa_MdnsDiscoveryConfiguration_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer2
    &OpcUa_RegisterServer2Request_EncodeableType,
    &OpcUa_RegisterServer2Response_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ChannelSecurityToken
    &OpcUa_ChannelSecurityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_OpenSecureChannel
    &OpcUa_OpenSecureChannelRequest_EncodeableType,
    &OpcUa_OpenSecureChannelResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_CloseSecureChannel
    &OpcUa_CloseSecureChannelRequest_EncodeableType,
    &OpcUa_CloseSecureChannelResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_SignedSoftwareCertificate
    &OpcUa_SignedSoftwareCertificate_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_SignatureData
    &OpcUa_SignatureData_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_UserIdentityToken
    &OpcUa_UserIdentityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_AnonymousIdentityToken
    &OpcUa_AnonymousIdentityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_UserNameIdentityToken
    &OpcUa_UserNameIdentityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_X509IdentityToken
    &OpcUa_X509IdentityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_IssuedIdentityToken
    &OpcUa_IssuedIdentityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_NodeAttributes
    &OpcUa_NodeAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ObjectAttributes
    &OpcUa_ObjectAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_VariableAttributes
    &OpcUa_VariableAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_MethodAttributes
    &OpcUa_MethodAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ObjectTypeAttributes
    &OpcUa_ObjectTypeAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_VariableTypeAttributes
    &OpcUa_VariableTypeAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReferenceTypeAttributes
    &OpcUa_ReferenceTypeAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_DataTypeAttributes
    &OpcUa_DataTypeAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ViewAttributes
    &OpcUa_ViewAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_GenericAttributeValue
    &OpcUa_GenericAttributeValue_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_GenericAttributes
    &OpcUa_GenericAttributes_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ViewDescription
    &OpcUa_ViewDescription_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RelativePathElement
    &OpcUa_RelativePathElement_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RelativePath
    &OpcUa_RelativePath_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_BrowsePath
    &OpcUa_BrowsePath_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_BrowsePathTarget
    &OpcUa_BrowsePathTarget_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EndpointConfiguration
    &OpcUa_EndpointConfiguration_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_NodeReference
    &OpcUa_NodeReference_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ContentFilterElement
    &OpcUa_ContentFilterElement_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ContentFilter
    &OpcUa_ContentFilter_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ElementOperand
    &OpcUa_ElementOperand_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_LiteralOperand
    &OpcUa_LiteralOperand_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_AttributeOperand
    &OpcUa_AttributeOperand_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_SimpleAttributeOperand
    &OpcUa_SimpleAttributeOperand_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ContentFilterElementResult
    &OpcUa_ContentFilterElementResult_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ContentFilterResult
    &OpcUa_ContentFilterResult_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ParsingResult
    &OpcUa_ParsingResult_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReadValueId
    &OpcUa_ReadValueId_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReadEventDetails
    &OpcUa_ReadEventDetails_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReadRawModifiedDetails
    &OpcUa_ReadRawModifiedDetails_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReadProcessedDetails
    &OpcUa_ReadProcessedDetails_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ReadAtTimeDetails
    &OpcUa_ReadAtTimeDetails_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EventFilter
    &OpcUa_EventFilter_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_AggregateConfiguration
    &OpcUa_AggregateConfiguration_EncodeableType,
    #endif
    OpcUa_Null
};

OpcUa_EncodeableType** OpcUa_KnownEncodeableTypes = g_OpcUa_KnownEncodeableTypes;
/* This is the last line of an autogenerated file. */
