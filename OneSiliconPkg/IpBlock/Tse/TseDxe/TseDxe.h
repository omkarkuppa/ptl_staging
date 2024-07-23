/** @file
  Header contains definitions of TSE protocol implementation functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _TSE_DXE_H_
#define _TSE_DXE_H_

#define UINT16_BDF(Bus, Device, Function)                   \
  ((((Function) & 0x07)) | (((Device) & 0x1f) << 5) | (((Bus) & 0xff) << 8))

#define PCI_CLASS_MASS_STORAGE_NVM  0x08                // mass storage sub-class non-volatile memory.

#define NIL_GUID \
  { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

//
// TSE Table line size
//
#define TSE_TABLE_SIZE       SIZE_256KB
#define TSE_TABLE_LINE_SIZE  64   // Each TSE Line is 64 Bytes
#define TSE_TABLE_TOTAL_ENTRIES   (TSE_TABLE_SIZE / TSE_TABLE_LINE_SIZE)
//
// First and last entries in the table are reserved.
//
#define TSE_TABLE_USABLE_ENTRIES  TSE_TABLE_TOTAL_ENTRIES - 2

//
// Table boundaries
//
#define TSE_TABLE_FIRST_IDX 1                         // First usable entry starts at index 1, index 0 is reserved
#define TSE_TABLE_LAST_IDX  TSE_TABLE_USABLE_ENTRIES  // Last usable entry's index

#define TSE_LINE_BLOCK_SIZE_4KB  0x0
#define TSE_LINE_BLOCK_SIZE_512B 0x1

#define TSE_LINE_LBA_OFFSET_TYPE  0x0
#define TSE_LINE_PAGE_OFFSET_TYPE 0x1

#define TSE_GPPA_ENTRIES_PER_LINE 11
#define TSE_IO_16_BYTE_ALIGNMENT  0x10 // 16 Byte alignment

#define TSE_ALG_KEY_SIZE_AES_XTS_256 BIT8
#define TSE_LBA_512B_SUPPORT BIT9
#define TSE_LBA_4KB_SUPPORT  BIT12

#define TSE_FLUSH_ENTIRE_IOCCE_CACHE_BIT BIT31

#define TSE_DRIVER_CONTEXT_SIGNATURE  SIGNATURE_32 ('T','D','C','D')
#define TSE_DRIVER_CONTEXT_REVISION   0x0001

#pragma pack(push,1)
typedef union {
  struct {
    UINT64 Tweak      : 44; // [43:0]
    UINT64 Offset     : 8;  // [51:44]
    UINT64 KeyIdx     : 4;  // [55:52]
    UINT64 BlkSize    : 2;  // [57:56]
    UINT64 OffsetType : 1;  // [58]
    UINT64 rsvd_0     : 5;  // [63:59]
    UINT8  rsvd_1     : 8;  // [71:64]
  } Bits;
  UINT8 Data[9];
} TSE_LINE_HEADER;

//
// Definition for Guest Physical Page Address (GPPA) 5B
//
typedef union {
  UINT8    Data[5];
} GPPA_ENTRY;

//
// TSE_LINE
//
typedef struct _TSE_LINE {
  GPPA_ENTRY GPPA[TSE_GPPA_ENTRIES_PER_LINE];
  TSE_LINE_HEADER Header;
} TSE_LINE;
#pragma pack(pop)

//
// TSE_TABLE_PROPERTIES
//
typedef struct {
  //
  // Address pointing to the TSE DRAM table.
  //
  TSE_LINE                         *TableAddress;
  //
  // Index pointing to the first available entry index in the TSE DRAM table.
  //
  UINT16                          CurrentIndex;
} TSE_TABLE_PROPERTIES;


typedef enum {
  TSE_SINGLE_LINE_PAGE_ALIGNED       = 0x0,
  TSE_SINGLE_LINE_PAGE_UNALIGNED     = 0x1,
  TSE_MULTIPLE_LINE_PAGE_ALIGNED     = 0x2,
  TSE_MULTIPLE_LINE_SECTOR_ALIGNED   = 0x3,
  TSE_MULTIPLE_LINE_PAGE_IO_SPLIT    = 0x4,
  TSE_PRP_TYPE_MAX_VALUE
} TSE_PRP_LINE_TYPE;

typedef struct {
  //
  // Pointer to BlockIoMedia in which we will issue the IO operation
  //
  EFI_BLOCK_IO_MEDIA *BlockIoMedia;
  //
  // Pointer to the configured Crypto Key Index for IO operation
  //
  UINT64 *CryptoKeyIndex;
  //
  // Pointer to configured CryptoIvInput custom tweak value for the IO operation
  //
  EFI_BLOCK_IO_CRYPTO_IV_INPUT_AES_XTS *CryptoIvInput;
} TSE_IO_CONTEXT;

typedef struct {
  ///
  /// The TSE engine ID.
  ///
  UINT8                            TseEngineId;
  ///
  /// Supported algorithm count
  ///
  UINT8                            AlgCount;
  ///
  /// Registered NVMe Devices
  ///
  UINT8                            RegisteredDevices;
  //
  // State of the engine's TSE DRAM table
  //
  TSE_TABLE_PROPERTIES             TseTableProperties;
  ///
  /// Supplies the TSE Engine alignment requirement for any buffer to read or write block(s).
  ///
  /// IoAlign must be a power of 2, and the requirement is that the start address of a buffer must
  /// be evenly divisible by IoAlign with no remainder.
  ///
  UINT32                           IoAlign;
  ///
  /// The TSE engine capabilities.
  ///
  EFI_BLOCK_IO_CRYPTO_CAPABILITIES CryptoCap;
} TSE_ENGINE_INFO;

typedef struct {
  UINT32    Signature;
  UINT16    Length;
  UINT16    Revision;
} TSE_DRIVER_CONTEXT_HEADER;

typedef struct {
  //
  // Context header
  //
  TSE_DRIVER_CONTEXT_HEADER Header;
  //
  // TSE Engine context
  //
  TSE_ENGINE_INFO EngineInfo;
  //
  // TSE Protocol
  //
  TSE_PROTOCOL TseApi;
  //
  // Driver's Block IO Crypto Configuration Table.
  //
  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY  *CryptoConfigurationTable;
  //
  // Configuration table allocated entries
  //
  UINT64 CryptoConfigurationTableEntryCount;
  //
  // TSE IO transaction context
  //
  TSE_IO_CONTEXT TseIoContext;
  //
  // Driver context lock
  //
  // This lock is used as a semaphore when modifying the context of TSE protocol instance.
  // Functions that can be called from the external API will acquire and release the lock.
  //
  EFI_LOCK TseDriverContextLock;
} TSE_DRIVER_CONTEXT;

/**
   This function is meant to replace the SetBDF function pointer after
   EndOfDxe event, to prevent rogue drivers from registering additional
   BDFs into the IOC registers.

   @param[in] Bus                  Ignored Bus of the end-point.
   @param[in] Device               Ignored Device of the end-point.
   @param[in] Function             Ignored function of the end-point.

   @retval EFI_ACCESS_DENIED       Always returns this error.
**/
EFI_STATUS
EFIAPI
LockedBDF (
  IN CONST UINT8 Bus,
  IN CONST UINT8 Device,
  IN CONST UINT8 Function
  );

/**
   The SetBDF() configures the IOC BDF registers that allow hardware to forward
   NVMe traffic to the IOCCE engine.

   The IOC BDF registers must be configured by the firmware before issuing a
   NVMe command that utilizes the IOCCE engine.

   The firmware must detect (Bus, Device, Function) of the NVMe controllers that
   have drives connected.

   This API must perform input validation such that only NVMe connected BDF
   is allowed to be programed in the IOC BDF register.  In addition, this
   API must be locked at EndOfDxe UEFI event to prevent rouge drivers from
   accessing the IOC BDF registers through UEFI driver.

   @param[in] Bus                  Bus of the end-point to be programmed.
   @param[in] Device               Device of the end-point to be programmed.
   @param[in] Function             Function of the end-point to be programmed.

   @retval EFI_SUCCESS             configuration of the BDF succeded.
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
   @retval EFI_OUT_OF_RESOURCES    if no additional registers are available to program the given end-point device.
   @retval Others                  error condition prevented the BDF from being registered.
**/
EFI_STATUS
EFIAPI
SetBDF (
  IN CONST UINT8 Bus,
  IN CONST UINT8 Device,
  IN CONST UINT8 Function
  );

/**
   SetBlockIoMedia() configures the Block IO Media instance into the TSE protocol context.

   The configured Block IO Media instance is part of the TSE IO context and any BlockIoCrypto IO
   operation calling SubmitTseTableEntries() will consume this context as part of the TSE
   Table configuration.

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] BlockIoCryptoProtocol Instance of the Block IO Crypto Protocol to use in TSE IO operation.

   @retval EFI_SUCCESS             if configuration of the BlockIoMedia instance succeded
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
**/
EFI_STATUS
EFIAPI
SetBlockIoMedia (
  IN EFI_BLOCK_IO_MEDIA *BlockIoMedia
  );

/**
   SetCryptoKey() configures the Crypto Key Index and Tweak value into the TSE protocol context.

   This Key and Tweak values are consumed after calling to SubmitTseTableEntries().

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] Index                Input key index associated with UEFI configuration table entry
   @param[in] CryptoIvInput        Input tweak value to use in the IO operation

   @retval EFI_SUCCESS             if crypto configuration into IO context succeded.
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
**/
EFI_STATUS
EFIAPI
SetCryptoConfig (
  IN UINT64 *Index,
  IN VOID   *CryptoIvInput
  );

/**
   CryptoBlockIncrement() reconfigures the Tweak value into the TSE protocol context.

   This TweakIncrement value updates the currently configured TSE contextCryptoIvInput value consumed after calling to SubmitTseTableEntries().

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] TweakIncrement       Input tweak increment.

   @retval EFI_SUCCESS             if crypto configuration into IO context succeded.
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
**/
EFI_STATUS
EFIAPI
CryptoBlockIncrement (
  IN UINT64 TweakIncrement
  );

/**
   This function scans the UEFI Crypto Configuration table for the TSE key index associated
   with the TSE IO Context

   @retval   CryptoKey  Pointer to the TSE key index in the UEFI Crypto configuration table
                        if a match is found. NULL if no key with the associated index was found.
**/
UINT8  *
GetCryptoKeyFromUefiConfigTable (
  VOID
  );

/**
   This function checks that the TSE IO context has been defined prior to issuing an IO operation

   @retval TRUE      if the TSE IO Context is valid
   @retval FALSE     if the TSE IO Context is not valid
**/
BOOLEAN
IsTseIoContextValid (
  VOID
  );

/**
   This function will clear all the TSE DRAM table contents.
**/
VOID
ClearTseDramTable (
  VOID
  );

/**
   This function dumps the requested number of TSE Lines starting at a given index

   @param[in]      TseLineIndex   Index associated with the starting TSE line
   @param[in]      TseLineNo      Number of lines to dump
**/
VOID
DumpTseLines (
  IN UINT16  TseLineIndex,
  IN UINT16  TseLineNo
  );

/**
   Injects PRP into GPPA and Patches PRP with TSE Line Index.

   This helper function receives a TseLine index to be written into,
   the line's GPPA index into which we want to inject a PRP address
   and the PRP address to be injected and patched.

   The result will have the truncated PRP (12 lower bits are truncated) and written
   into the specific GPPA. Also, the PRP will be patched to contain the TSE index
   written into the upper 12 bits of the PRP.

   @param[in]      TseLineIndex   Index associated with the TSE line
   @param[in][out] GppaIndex      Index to the assigned GPPA within the TSE line
   @param[in][out] Prp            PRP to be injected and patched.

   @retval EFI_SUCCESS            if the TSE line header was populated without errors.
   @retval EFI_INVALID_PARAMETER  if any of the TSE line header input data was truncated.
**/
EFI_STATUS
InjectGppaPatchPrp (
  IN UINT16      TseLineIndex,
  IN UINT64      GppaIndex,
  IN OUT UINT64  *Prp
  );

/**
   Populates the TSE Line Header with the provided values

   This helper function will populate a TSE line header with the provided values

   @param[in] TseLineIndex   Index associated with the TSE line to be populated
   @param[in] Tweak          TSE line's tweak value
   @param[in] Offset         TSE line's page or LBA offset value
   @param[in] BlockSize      TSE line's block Size value
   @param[in] KeyIdx         TSE line's crypto key index value
   @param[in] OffsetType     TSE line's offset type

   @retval EFI_SUCCESS            if the TSE line header was populated without errors.
   @retval EFI_INVALID_PARAMETER  if any of the TSE line header input data was truncated.
**/
EFI_STATUS
PopulateTseLineHeader (
  IN UINT16 TseLineIndex,
  IN UINT64 Tweak,
  IN UINT8 Offset,
  IN UINT8 BlockSize,
  IN UINT8 KeyIdx,
  IN UINT8 OffsetType
  );

/**
   The SubmitTseTableEntries() configures the PRP List and the TSE DRAM Table entries.
   This API is called before NVMe command is issued.

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] Prp                  Pointer to PRP1 and PRP2 entries in NVMe command
   @param[in] Lba                  Starting LBA of the associated NVMe command
   @param[in] Bytes                Transfer length in bytes of the associated NVMe command

   @retval EFI_SUCCESS             if configuration of qualifier and enable registers succeded.
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
   @retval EFI_OUT_OF_RESOURCES    if no additional registers are available to program the given end-point device.
**/
EFI_STATUS
EFIAPI
SubmitTseTableEntries (
  IN  UINT64          *Prp,
  IN  UINT64          Lba,
  IN  UINT32          Bytes
  );

/**
   The GetCapabilities() provides the capabilities of the Crypto Engine.

   If there are multiple Crypto algorithms supported by the Crypto Engine than
   this API will account for those various instances of the algorithm in the
   CryptoCapability[] structure.  Below figure depicts the high-level flow of
   this API.

   @param[out] Capabilities      Pointer to the EFI_BLOCK_IO_CRYPTO_CAPABILITIES structure.

   @retval EFI_NO_RESPONSE       No response was received from the ICI.
   @retval EFI_DEVICE_ERROR      An error occurred when attempting to access the ICI.
   @retval EFI_INVALID_PARAMETER This is NULL.
   @retval EFI_INVALID_PARAMETER Capabilities is NULL.
**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN OUT EFI_BLOCK_IO_CRYPTO_CAPABILITIES *Capabilities
  );

/**
   Configures the provided entries into the UEFI CryptoConfigurationTable.

   @param[in]  ConfigurationCount  Number of entries being configured with this call.
   @param[in]  ConfigurationTable  Pointer to a table used to populate the configuration table.
   @param[out] ResultingTable      Optional pointer to a table that receives the newly configured
   entries.

   @retval EFI_SUCCESS             The ICI is ready for use.
   @retval EFI_NO_RESPONSE         No response was received from the ICI.
   @retval EFI_DEVICE_ERROR        An error occurred when attempting to access the ICI.
   @retval EFI_INVALID_PARAMETER   ConfigurationTable is NULL
   @retval EFI_INVALID_PARAMETER   ConfigurationCount is 0.
   @retval EFI_OUT_OF_RESOURCES    Could not find the requested number of available entries in the
   configuration table.
**/
EFI_STATUS
EFIAPI
SubmitUefiTableConfigurationEntries (
  IN  UINT64                                           ConfigurationCount,
  IN  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ConfigurationTable,
  OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable OPTIONAL
  );

/**
   Retrieves a copy of the current Block IO Crypto configuration table for
   the crypto engine.

  @param[in]  StartIndex          Configuration table index at which to start the configuration
                                  query.
  @param[in]  ConfigurationCount  Number of entries to return in the response table.
  @param[in]  KeyOwnerGuid        Optional parameter to filter response down to entries with a
                                  given owner. A pointer to the Nil value can be used to return
                                  available entries. Set to NULL when no owner filtering is required.
  @param[out] ConfigurationTable  Table of configured configuration table entries (with no CryptoKey
                                  returned): configuration table index, KeyOwnerGuid, Capability.
                                  Should have sufficient space to store up to ConfigurationCount
                                  entries.

  @retval EFI_SUCCESS             The ICI is ready for use.
  @retval EFI_NO_RESPONSE         No response was received from the ICI.
  @retval EFI_DEVICE_ERROR        An error occurred when attempting to access the ICI.
  @retval EFI_INVALID_PARAMETER   Configuration table is NULL.
  @retval EFI_INVALID_PARAMETER   StartIndex is out of bounds.
**/
EFI_STATUS
EFIAPI
GetUefiConfigurationTable (
  IN     UINT64                                           StartIndex,
  IN     UINT64                                           ConfigurationCount,
  IN     EFI_GUID                                         *KeyOwnerGuid OPTIONAL,
  OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ConfigurationTable
  );

#endif // _TSE_DXE_H_
