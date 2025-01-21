/** @file
  Definitions for XmlCliApi

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:

**/

#ifndef XML_CLI_API_H
#define XML_CLI_API_H


#define PAGE_ALIGNMENT_64_BIT           (0xFFFFFFFFFFFFF000)
#define PAGE_ALIGNMENT_32_BIT           (0xFFFFF000)
#define ALIGNMENT_64_KB_32_BIT          (0xFFFF0000)

#define XML_DATA_ONLY                   (0)
#define KNOBS_DATA_BIN                  (9)


#define NVAR_SIGNATURE                  (0x5241564E24)  // $NVAR
#define NVRO_SIGNATURE                  (0x4F52564E24)  // $NVRO

#define MAX_CLI_APIs_SUPPORTED          (0x20)          //32

#define CLI_SGN_RESPONSE_READY          (0xCAFECAFE)
#define CLI_SGN_REQUEST_READY           (0xC001C001)
#define CLI_SGN_RESPONSE_GET            (0xCAFE0001)

#define CLI_GEN2_SGN_RESPONSE_READY     (0xD055CAFE)
#define CLI_GEN2_SGN_REQUEST_READY      (0xD055C001)
#define CLI_GEN2_SGN_RESPONSE_GET       (0xD0550001)

// Knob Header Revision
#define KNOB_HEADER_REVISION_3          (0x3)  // Added support to indicate current knob is part of DEPEX or not
#define KNOB_HEADER_REVISION_4          (0x4)  // BitWise support was added from revision 4 onwards


// Command side effects
#define CLI_CMD_NO_SIDE_EFFECT          (0)
#define CLI_CMD_RESET_REQUIRED          (1)
#define CLI_CMD_RESTART_REQUIRED        (2)
#define CLI_CMD_SIDE_EFFECT_RESERVED    (3)

#define APPEND_BIOS_KNOBS_OPCODE        (0x0048)
#define RESTORE_MODIFY_KNOBS_OPCODE     (0x0049)
#define READ_BIOS_KNOBS_OPCODE          (0x004A)
#define LOAD_DEFAULT_KNOBS_OPCODE       (0x004B)
#define UPDATE_BIOS_KNOBS_OPCODE        (0x0050)
#define GET_SET_VARIABLE_OPCODE         (0x9E5E)

#define GET_VAR                         (0x00)
#define SET_VAR                         (0x01)
#define GET_MOD_SET_VAR                 (0x02)
#define GET_DEF_CUR_VAR                 (0x03)
#define GET_ALL_VARS                    (0x9A)

#define EXIT_OPCODE                     (0xFFFF)

#define SW_XML_CLI_ENTRY                (0xF6)
#define SW_SMI_PORT                     (0xB2)

#pragma pack(push, 1)

typedef
EFI_STATUS
(*DXE_CLI_ENTRY) (
  VOID  *XmlCliCom
  );

typedef
EFI_STATUS
(*CLI_CMD_HANDLER_FUNC) (
  VOID  *Buffer
  );

typedef struct {
  UINT16                CommandId;
  CLI_CMD_HANDLER_FUNC  CmdHandler;
  VOID                  *Buffer;
} REG_CLI_COMMAND_ENTRY;

typedef
VOID
(*DXE_REG_CLI_API) (
  UINT16                CommandId,
  CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  VOID                  *Buffer
  );

typedef
VOID
(*SMM_REG_CLI_API) (
  UINT16                CommandId,
  CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  VOID                  *Buffer
  );

typedef
EFI_STATUS
(*TIANO_COMPRESS) (
  IN UINT8       *SrcBuffer,
  IN UINT32      SrcSize,
  IN UINT8       *DstBuffer,
  IN OUT UINT32  *DstSize
  );

typedef
EFI_STATUS
(*TIANO_DECOMPRESS) (
  IN VOID      *SrcBuffer,
  IN UINT32    SrcSize,
  IN OUT VOID  *DstBuffer,
  OUT UINT32   *DstSize
  );

typedef
UINT16
(*DXE_GET_SET_API) (
  IN VOID      *XmlCliCom,
  IN UINT8     Operation,
  IN OUT VOID  *MyKnobList,
  IN UINT16    MyKnobCount
  );


// BIOS Repository Data structure
typedef struct _XML_CLI_API
{
  DXE_CLI_ENTRY     DxeCliEntry;
  DXE_REG_CLI_API   CliDxeRegisterApi;
  SMM_REG_CLI_API   CliSmmRegisterApi;
  TIANO_COMPRESS    TianoCompress;
  TIANO_DECOMPRESS  TianoDecompress;
  DXE_GET_SET_API   CliGetSetMyVariable;
// Please do not touch or change the above order of the entries in this Struct.
} XML_CLI_API;

typedef union {
  UINT16  RawAccess;
  struct {
    UINT16  WrongParameter      :1;
    UINT16  CannotExecute       :1;
    UINT16  CommandSideEffects  :4;
    UINT16  Reserved            :10;
  } Fields;
} CLI_BUFFER_FLAGS;

typedef struct {
  UINT32            Signature;
  UINT16            CommandId;
  CLI_BUFFER_FLAGS  Flags;
  UINT32            Status;
  UINT32            ParametersSize;
  UINT8             Parameters[0];
} CLI_BUFFER;

typedef struct {
  UINT8   VarStoreIndex;
  UINT16  KnobOffset;
  UINT8   KnobSize;
//  Below structure member are dynamically populated based on KnobSize
//  UINT8   KnobValue[KnobSize];
} CLI_PROCESS_BIOS_KNOBS_RQST_PARAM;

typedef struct {
  UINT32  KnobXmlEntryPtr;
  UINT16  KnobXmlEntrySize;
  UINT8   VarStoreIndex;
  UINT16  KnobOffset;
  UINT8   KnobSize;
//  Below structure member are dynamically populated based on KnobSize
//  UINT8   KnobDefValue[KnobSize];
//  UINT8   KnobCurrValue[KnobSize];
} CLI_PROCESS_BIOS_KNOBS_RSP_PARAM;

typedef struct {
  UINT8   VarStoreIndex;
  UINT16  KnobOffset;
  UINT8   KnobSize;
  UINT64  KnobValue;
} CLI_UPDATE_BIOS_KNOBS_RQST_PARAM;

typedef struct {
  UINT32    RawAccess;
  struct {
    UINT32  SetupModified      :1;
    UINT32  Reserved           :31;
  } Status;
} CLI_UPDATE_BIOS_KNOBS_RSP_PARAM;

typedef struct {
  EFI_GUID  NvarGuid;
  UINT32    NvarAttribute;
  UINT32    NvarSize;
  UINT32    NvarRetStatus;
  UINT8     Operation;
  CHAR8     NvarName[1];
//  Below structure member are dynamically populated based on NvarSize
//  UINT8    NvarDataBuff[NvarSize];
} CLI_GET_SET_VARIABLE_PARAM;

typedef struct {
  UINT16  Offset;
  UINT8   Size;
//  Below structure member are dynamically populated based on Size
//  UINT8    KnobValue[Size];
} NVAR_DATA_BUFF;

typedef struct _KNOB_BIN_HDR {
  struct{
    UINT64  Signature   : 40;
    UINT64  VarId       : 8;
    UINT64  KnobCount   : 16;
  } Hdr0;
  struct{
    UINT64  DupKnobBufOff  : 24;
    UINT64  NvarPktSize    : 24;
    UINT64  NvarNameOffset : 8;
    UINT64  HdrRevision    : 8;
  } Hdr1;
  UINT16    NvarSize;
  EFI_GUID  NvarGuid;
  CHAR8     NvarName[44];  // Variable size Name, we can reduce/increase the size going forward.
  UINT16    BitKnobCount;  // to support bit wise Knobs
} KNOB_BIN_HDR;

typedef struct _KNOB_LIST {
  CHAR8   *KnobName;
  UINT16  KnobOffset;
  UINT8   KnobSize;
  UINT8   VarId;
  UINTN   KnobValue;
  UINTN   DefKnobValue;
} KNOB_LIST;

/**
  Entry point method to validate request/response buffer
  and perform operation for given valid operation code as per
  validated request buffer.

  @param[in] XmlCliCom  XmlCli Common Structure
                        Defined as VOID so that anyone can use this as
                        API without consuming while XML_CLI_COMMON structure

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER  Null pointer passed as parameter
  @retval  EFI_NO_RESPONSE        Signature in Structure is still not in READY state
  @retval  EXIT_OPCODE            Specified Operation code in Request buffer match to EXIT_OPCODE
**/
EFI_STATUS
CliEntryPoint (
  IN VOID  *XmlCliCom
  );

/**
  Registers the Cli API
  to expose the Api this function has to be used
  usage:
    CliDxeRegisterApi(MEM_READ_OPCODE, CliMemRead, XmlCliProtocol)

  @param[in] CommandId      Command Id for the API
  @param[in] ClicmdHandler  Handler function
  @param[in] Buffer         Pointer to Protocol Structure

  @retval  VOID
**/
VOID
CliDxeRegisterApi (
  IN UINT16                CommandId,
  IN CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  IN VOID                  *Buffer
  );

/**
  Registers Register Cli Api Protocol to Smm

  @param[in] CommandId      Command Id
  @param[in] ClicmdHandler  Method to Invoke Valid Cli Command to call
  @param[in] Buffer         Input Buffer

  @retval  VOID
**/
VOID
CliSmmRegisterApi (
  IN UINT16                CommandId,
  IN CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  IN VOID                  *Buffer
  );

/**
  Standard Tiano Compress Method from edk2
**/
EFI_STATUS
TianoCompress (
  IN      UINT8   *SrcBuffer,
  IN      UINT32  SrcSize,
  IN      UINT8   *DstBuffer,
  IN OUT  UINT32  *DstSize
  );

/**
  Tiano Decompress utility
**/
EFI_STATUS
TianoDecompress (
  IN  VOID      *SrcBuffer,
  IN  UINT32    SrcSize,
  IN  OUT VOID  *DstBuffer,
  OUT UINT32    *DstSize
  );

/**
  Get Offset and size of the knob(setup variable)  from Bios Knobs Data bin.

  @param[in]     BiosKnobsDataAddr  Address of Bios knobs Data bin.
  @param[in]     BiosKnobsDataSize  Size of Bios knobs Data bin.
  @param[in]     VarName            Name of Nvar.
  @param[in]     VarGuid            Nvar Guid.
  @param[in,out] MyKnobList         List of knob for which Offset and size to be find.
                                      Size and offset will be part of the structure as output.
  @param[in]     MyKnobCount        Total number of knob for which Offset and size to be find.
  @param[in,out] NvarSize           Size of the setup variable

  @retval  UINT16
**/
UINT16
CliGetSetMyVariable (
  IN VOID      *XmlCliCom,
  IN UINT8     Operation,
  IN OUT VOID  *MyKnobList,
  IN UINT16    MyKnobCount
  );

#pragma pack(pop)
#endif //XML_CLI_API
