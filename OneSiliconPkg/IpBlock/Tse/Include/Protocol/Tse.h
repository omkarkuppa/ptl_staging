/** @file
  Total Storage Encryption protocol

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
#ifndef _TSE_PROTOCOL_H_
#define _TSE_PROTOCOL_H_

#include <Protocol/BlockIoCrypto.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gTseProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _TSE_PROTOCOL TSE_PROTOCOL;

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
typedef
EFI_STATUS
(EFIAPI *TSE_SET_BDF) (
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
typedef
EFI_STATUS
(EFIAPI *TSE_SET_BLOCK_IO_MEDIA) (
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
typedef
EFI_STATUS
(EFIAPI *TSE_SET_CRYPTO_CONFIG) (
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
typedef
EFI_STATUS
(EFIAPI *TSE_CRYPTO_BLOCK_INCREMENT) (
  IN UINT64 TweakIncrement
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
typedef
EFI_STATUS
(EFIAPI *TSE_SUBMIT_TSE_TABLE_ENTRIES) (
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
typedef
EFI_STATUS
(EFIAPI *TSE_GET_CAPABILITIES) (
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
   @retval EFI_INVALID_PARAMETER   ConfigurationTable is NULL.
   @retval EFI_INVALID_PARAMETER   ConfigurationCount is 0.
   @retval EFI_OUT_OF_RESOURCES    Could not find the requested number of available entries in the
   configuration table.
**/
typedef
EFI_STATUS
(EFIAPI *TSE_SUBMIT_UEFI_TABLE_CONFIGURATION_ENTRIES) (
  IN     UINT64                                           ConfigurationCount,
  IN     EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ConfigurationTable,
  OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable OPTIONAL
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
typedef
EFI_STATUS
(EFIAPI *TSE_GET_UEFI_TABLE_CONFIGURATION_ENTRIES) (
  IN     UINT64                                           StartIndex,
  IN     UINT64                                           ConfigurationCount,
  IN     EFI_GUID                                         *KeyOwnerGuid OPTIONAL,
  OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY    *ConfigurationTable
  );

/**
  The protocols allow a platform module to perform configuration and
  operations of the Total Storage Encryption IP. The TSE protocol implements the Intel
  Total Storage Encryption API for the DXE environment.
**/
struct _TSE_PROTOCOL {
  TSE_SET_BDF SetBDF;
  TSE_SET_BLOCK_IO_MEDIA SetBlockIoMedia;
  TSE_SET_CRYPTO_CONFIG SetCryptoConfig;
  TSE_CRYPTO_BLOCK_INCREMENT CryptoBlockIncrement;
  TSE_SUBMIT_TSE_TABLE_ENTRIES SubmitTseTableEntries;
  TSE_GET_CAPABILITIES GetCapabilities;
  TSE_SUBMIT_UEFI_TABLE_CONFIGURATION_ENTRIES SubmitUefiTableConfigurationEntries;
  TSE_GET_UEFI_TABLE_CONFIGURATION_ENTRIES GetUefiTableConfigurationEntries;
};

#endif // _TSE_PROTOCOL_H_
