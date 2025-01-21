/** @file
  Definitions for XmlCliCommon.h

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

#ifndef XML_CLI_COMMON_H
#define XML_CLI_COMMON_H

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <SharedMailbox.h>
#include <XmlCliApi.h>
#include <XmlCliSetup.h>


#define XML_CLI_VERSION          "5.0.5"

#define KNOB_TYPE_CUTOFF         (0xD0)
#define MAX_NVARS_SUPPORTED      (0x60)  // 96
#define MAX_KNOB_SIZE_SUPPORTED  (0x100) // 256
#define XML_OS_INT               (0x05)
#define XML_OS_EXT               (0x0E)
#define MAX_VARSTORE_NAME        (64)

#define VARIABLE_OFFSET(TYPE,Field)  ((UINTN)(&(((TYPE *) 0)->Field)))

#ifndef EFI_DEADLOOP
  #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif


#pragma pack(push, 1)

typedef struct _CLI_RUNTIME {
  UINT8    CommandSubType;
  UINT8    ResetRequired;
  UINT8    FlashSts;
  BOOLEAN  UpdateKnobsEnabled;
  BOOLEAN  Reserved;  // Deprecated InSmm variable
} CLI_RUNTIME;

typedef struct _DUP_KNOB_DETAILS {
  UINT8              *DupKnobName;
  UINT8              *DupKnobExp;
  UINT8              *PrevKnobExp;
  EFI_IFR_OP_HEADER  *PrevIfrOpCodeHdr;
} DUP_KNOB_DETAILS;

typedef struct _KNOB_INFO {
  UINT8  KnobProcessed : 6;
  UINT8  PartOfDepex   : 1;
  UINT8  BitWise       : 1;
} KNOB_INFO;

typedef struct _VARSTORE_TABLE {
  // Varstore constant info
  CHAR8             Name[64];
  EFI_GUID          Guid;
  CHAR8             DefName[64];
  EFI_GUID          DefGuid;
  EFI_VARSTORE_ID   VarStoreId;  // Varstore info read from HII package
  UINT16            Size;
  UINT16            BitKnobCount;
  UINT32            KnobXmlEntryBase;
  VOID              *Data;
  VOID              *DefData;
  UINT8             **KnobName;
  UINT8             **ExpressionHash;
  UINT8             *BinKnobType;
  UINT8             *BinKnobSize;
  UINT16            *BinKnobOffset;
  KNOB_INFO         *KnobInfo;        // this indicates knob is BitWise [BIT07], Part of depex expression [BIT06], Knob is processed [BIT05:00]
  DUP_KNOB_DETAILS  *DupKnobDetails;
  BOOLEAN           FoundInPackage;   // flag that marks if this varstore is found in specific HII package
} VARSTORE_TABLE;

typedef struct _VARSTORE_LIST {
  // Varstore constant info
  CHAR8     StructName[64];
  CHAR8     Name[64];
  EFI_GUID  Guid;
  CHAR8     DefName[64];
  EFI_GUID  DefGuid;
} VARSTORE_LIST;

typedef struct _BIT_KNOB_INDEX_LOOKUP {
  UINT8   BitField;       // BitSize[7:3] BitOffset[2:0]
  UINT16  ByteOffset;
} BIT_KNOB_INDEX_LOOKUP;

typedef struct _KNOB_ENTRY_TBL_BITWISE {
  UINT64  StartIndex       : 16;
  UINT64  EndIndex         : 16;
  UINT64  BitWiseKnobCount : 4;      // Every unique offset can hold upto 8 bitwise knobs at max
  UINT64  Reserved         : 19;
  UINT64  IsBitWise        : 1;      // Assuming the knob size would not exceed value (256-1)
  UINT64  KnobSize         : 8;
} KNOB_ENTRY_TBL_BITWISE;

typedef struct _KNOB_ENTRY_TBL {
  UINT64  KnobXmlEntryOfst : 25;     // Offset of Knob XML Entry from GBT XML base address
  UINT64  KnobValMapOffset : 18;     // offset to the given knobs value map list
  UINT64  XmlEntrySize     : 12;     // assuming that knob entry size would not exceed (4 KB -1)
  UINT64  IsBitWise        : 1;      // Mark if the knob entry in table is bitwise or not
  UINT64  KnobSize         : 8;      // assuming the knob size would not exceed value (256-1)
} KNOB_ENTRY_TBL;

typedef struct _KNOB_PATCH_TBL {
  UINT8  KnobXmlEntryOfst[3];   // Offset of Knob XML Entry from GBT XML base address
  struct{
    UINT16  RelOffset : 12;     // assuming that knob entry size would not exceed (4 KB -1)
    UINT16  Reserved  : 4;      // Reserved for future use
  } CurrentVal;
    UINT8   KnobSize;
//  UINT8   Value[];            // Knob Value, the size of this variable will depend on KnobSize
} KNOB_PATCH_TBL;

typedef
UINTN
(*GEN_KNOBS_SECTION) (
  VOID  *XmlCliCom,
  VOID  *NewXmlWriterContext
  );

typedef
UINT32
(*DXE_XML_COMPRESS) (
  VOID    *XmlCliCom,
  UINT32  XmlSize
  );

// BIOS Repository Data structure
typedef struct _XML_CLI_BASE {
  XML_CLI_API       XmlCliApi;        // Preserving Order
  VOID              *DxeCliApiProto;  // Preserving Order
  VOID              *SmmCliApiProto;  // Preserving Order
  BOOLEAN           CliGen2Enable;
  BOOLEAN           XmlSkipSavingDefVar;
  UINT8             XmlType;
  CHAR8             BiosVersion[100];
  CHAR8             BuildTimeStamp[64];
  UINT16            EnableXmlCliLite;   // this is to know if XmlCliLite is enabled
  UINT32            GbtXmlAddress;
  UINT32            DramSharedMBAddr;
  UINT32            CurrGetSetVarBuffer;
  UINT32            XmlCliInterfaceMemSize;
  UINT32            CliBuffSize;
  UINT32            CliRequestBufferAddress;
  UINT32            CliResponseBufferAddress;
  UINT32            SetupKnobsAddress;
  UINT32            BiosKnobsDataAddr;
  UINT32            BiosKnobsDataSize;
  UINT32            PcieAddr;
  CLI_RUNTIME       CliRT;
  DXE_XML_COMPRESS  DxeXmlCompress;
  VOID              *Smst;
  VOID              *SmmVariable;
  // Please dont touch or change or delete the above entry in this Struct.
} XML_CLI_BASE;

typedef struct _XML_CLI_COMMON {
  // XML_CLI_BASE      XmlCliBase;
  XML_CLI_API       XmlCliApi;        // Preserving Order
  VOID              *DxeCliApiProto;  // Preserving Order
  VOID              *SmmCliApiProto;  // Preserving Order
  BOOLEAN           CliGen2Enable;
  BOOLEAN           XmlSkipSavingDefVar;
  UINT8             XmlType;
  CHAR8             BiosVersion[100];
  CHAR8             BuildTimeStamp[64];
  UINT16            EnableXmlCliLite;   // this is to know if XmlCliLite is enabled
  UINT32            GbtXmlAddress;
  UINT32            DramSharedMBAddr;
  UINT32            CurrGetSetVarBuffer;
  UINT32            XmlCliInterfaceMemSize;
  UINT32            CliBuffSize;
  UINT32            CliRequestBufferAddress;
  UINT32            CliResponseBufferAddress;
  UINT32            SetupKnobsAddress;
  UINT32            BiosKnobsDataAddr;
  UINT32            BiosKnobsDataSize;
  UINT32            PcieAddr;
  CLI_RUNTIME       CliRT;
  DXE_XML_COMPRESS  DxeXmlCompress;
  VOID              *Smst;
  VOID              *SmmVariable;
  UINT8             DummyPleaseDontRemove;  // remove this variable when structure is splitted
  // Please don't touch or change or delete the above entry in this Struct.
  BOOLEAN            XmlCliStructInitialized;
  BOOLEAN            PcdDfxAdvDebugJumper;
  UINT8              PublishSetupPgPtr;
  UINT8              SkipXmlComprs;
  UINT8              NumberOfVarstores;
  UINT8              NvroCount;
  UINT8              MergeDuplicateKnobs;
  UINT32             SetupKnobsSize;
  UINT32             KnobXmlEntryAddr;
  UINT32             KnobValMapAddr;
  UINT32             XmlPatchBaseAddr;
  UINT32             XmlCliDebugLogBuff;
  UINT32             FreeGbtMemAddr;
  UINT32             FreeGbtMemSize;
  UINT32             KnobsSignature;
  GEN_KNOBS_SECTION  XmlGenBiosKnobsSection;
  EFI_HANDLE         XmlCliProtocolHandle;
  VARSTORE_TABLE     VarstoreTable[MAX_NVARS_SUPPORTED];
} XML_CLI_COMMON;

/**
  Helper function to write Dram Mailbox Address to CMOS
  As the mailbox address is reserved area and may change,
  hence saving in CMOS for the tool to read

  @param[in] DramMbAddress  Address of Dram Mailbox

  @retval  VOID
**/
VOID
WriteDramMbAddr2Cmos (
  IN UINT32  DramMbAddress
  );

/**
  Reads/Writes Mailbox

  @param[in]  Offset          Mailbox offset
  @param[in]  Size            Size in bytes
  @param[out] Buffer          Output buffer
  @param[in]  MailboxAddress  Device Private Data and should not exceed to PCIe Address
  @param[in]  PcieAddr        PCIe Address
  @param[in]  OpRdOrWr        Operation type

  @retval  EFI_SUCCESS   Operation Successful
  @retval  !EFI_SUCCESS  Failure
**/
VOID ReadWriteMB (
  IN  UINT32  Offset,
  IN  UINT32  Size,
  OUT VOID    *Buffer,
  IN  UINT32  MailboxAddress,
  IN  UINT32  PcieAddr,
  IN  UINT8   OpRdOrWr
  );

/**
  Reads/Writes HIF Mailbox

  @param[in]  Offset          Mailbox offset
  @param[in]  Size            Size in bytes
  @param[out] Buffer          Output buffer
  @param[in]  MailboxAddress  Device Private Data
  @param[in]  OpRdOrWr        Operation type

  @retval  EFI_SUCCESS   Operation Successful
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
ReadWriteHIF (
  IN  UINT32  Offset,
  IN  UINT32  Size,
  OUT VOID    *Buffer,
  IN  UINT32  MailboxAddress,
  IN  UINT8   OpRdOrWr
  );

/**
  Helper function to Convert Hex array to ascii values.
  Takes input as pointer hex values and start offset to
  convert to ascii string

  @param[in,out] AsciiStr   Pointer to Ascii String to be stored
  @param[in]     InHexVal   Pointer to the hex values
  @param[in]     VarLength  Number of hex values to be converted to ascii string

  @retval  VOID
**/
VOID
ConvHexArray2asciiVal (
  IN OUT CHAR8  *AsciiStr,
  IN UINT8      *InHexVal,
  IN UINTN      VarLength
  );

#pragma pack(pop)

extern  EFI_GUID  gXmlCliCommonGuid;
extern  EFI_GUID  gDramSharedMailBoxGuid;
extern  EFI_GUID  gBiosKnobsDataBinGuid;

#endif // XML_CLI_COMMON
