/** @file
  Implementation file for TSE Protocol functionality

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

#include <Uefi.h>
#include <PiDxe.h>
#include <IndustryStandard/Pci.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/TseDataHob.h>
#include <Library/TseInfoLib.h>
#include <Library/VmdInfoLib.h>

#include <Defines/HostBridgeDefines.h>
#include <Register/TseIocceRegs.h>
#include <Register/TseIocRegs.h>
#include <Protocol/Tse.h>

#include "TseDxe.h"

EFI_HANDLE               mImageHandle;
TSE_DRIVER_CONTEXT       *mTseDriverContextPtr = NULL;
TSE_DRIVER_CONTEXT       mTseDriverContext;

EFI_EVENT                mLockBdfEndOfDxeEvent = NULL;
EFI_EVENT                mClearTseTableExitBootServicesEvent  = NULL;

/**
   Clears TSE DRAM table and issues cache invalidatation

   @param[in] Event                The event that triggered this notification function
   @param[in] ParentImageHandle    Pointer to the notification functions context
**/
VOID
EFIAPI
LockBdfsEndOfDxeCallback (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  mTseDriverContextPtr->TseApi.SetBDF = LockedBDF;
  DEBUG((DEBUG_INFO, "[TseProtocol] Locked SetBDF API.\n"));
  gBS->CloseEvent (Event);
  return;
}

/**
   This callback function will handle clearing the contents of the
   UEFI crypto configuration table, clearing the TSE DRAM table line entries
   and invalidating the TSE cache.

   @param[in] Event                The event that triggered this notification function
   @param[in] ParentImageHandle    Pointer to the notification functions context
**/
VOID
EFIAPI
TseExitBootServicesCallback (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  //
  // Zero out the UEFI crypto configuration table contents
  //
  ZeroMem (mTseDriverContextPtr->CryptoConfigurationTable, sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);
  DEBUG((DEBUG_INFO, "[TseProtocol] Cleared UEFI crypto configuration table.\n"));
  //
  // Clear TSE DRAM table and invalidate cache
  //
  ClearTseDramTable ();
  DEBUG((DEBUG_INFO, "[TseProtocol] Cleared TSE DRAM Table.\n"));
  gBS->CloseEvent (Event);
  return;
}

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
  )
{
  return EFI_ACCESS_DENIED;
}

/**
   The SetBDF() configures the IOC BDF registers that allow hardware to forward
   NVMe traffic to the IOCCE engine.

   The IOC BDF registers must be configured by the firmware before issuing a
   NVMe command that utilizes the IOCCE engine.

   The firmware must detect Bus Device Function of the NVMe controllers that
   have drives connected.

   This API must perform input validation such that only NVMe connected BDF
   is allowed to be programed in the IOC BDF register.  In addition, this
   API must be locked at EndOfDxe UEFI event to prevent rouge drivers from
   accessing the IOC BDF registers through UEFI driver.

   @param[in] Bus                  Bus of the end-point to be programmed.
   @param[in] Device               Device of the end-point to be programmed.
   @param[in] Function             Function of the end-point to be programmed.

   @retval EFI_SUCCESS             if configuration of qualifier and enable registers succeded.
   @retval EFI_NOT_READY           if TSE driver context is NULL
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
   @retval EFI_OUT_OF_RESOURCES    if no additional registers are available to program the given end-point device.
**/
EFI_STATUS
EFIAPI
SetBDF (
  IN CONST UINT8 Bus,
  IN CONST UINT8 Device,
  IN CONST UINT8 Function
  )
{
  UINTN PciBase = 0;
  UINT8 BaseCode = 0;
  UINT8 SubClassCode = 0;

  UINT8 RegisteredDevices = 0;
  UINT8 DeviceIdx = 0;

  UINT16 NewBDF = 0;
  UINT16 ConfiguredBDF = 0;
  UINT64 MchBar = 0;

  TSE_IOC_TSE_BDF_CMP0_STRUCT IocTseBdfCmp0;
  TSE_IOC_TSE_BDF_CMP1_STRUCT IocTseBdfCmp1;
  TSE_IOC_TSE_BDF_EN_STRUCT   IocTseBdfEn;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (mTseDriverContextPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "SetBDF() - Driver context is NULL!\n"));
    return EFI_NOT_READY;
  }

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));
  RegisteredDevices = mTseDriverContextPtr->EngineInfo.RegisteredDevices;

  //
  // Check endpoint device type is NVME drive
  //

  if ((IsVmdEnabled() == TRUE) && (IsVmdBus(Bus) == TRUE)) {
    DEBUG ((DEBUG_ERROR, "Specified BDF is a VMD NVMe device\n"));
  } else {
    PciBase = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

    SubClassCode = PciSegmentRead8 (PciBase + PCI_CLASSCODE_OFFSET + 1);
    BaseCode     = PciSegmentRead8 (PciBase + PCI_CLASSCODE_OFFSET + 2);

    if ((BaseCode != PCI_CLASS_MASS_STORAGE) || (SubClassCode != PCI_CLASS_MASS_STORAGE_NVM)) {
      DEBUG ((DEBUG_WARN, "Specified BDF is not TSE compatible! B:0x%x, D:0x%x, F:0x%x, SubClassCode: 0x%x, BaseCode: 0x%x\n", Bus, Device, Function, SubClassCode, BaseCode));
      EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
      return EFI_INVALID_PARAMETER;
    }
  }

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  ZeroMem (&IocTseBdfEn,   sizeof (TSE_IOC_TSE_BDF_EN_STRUCT));
  ZeroMem (&IocTseBdfCmp0, sizeof (TSE_IOC_TSE_BDF_CMP0_STRUCT));
  ZeroMem (&IocTseBdfCmp1, sizeof (TSE_IOC_TSE_BDF_CMP1_STRUCT));

  DEBUG ((DEBUG_INFO, "(MchBar) 0x%x + (R_IOC_TSE_BDF_EN_BASE) 0x%x\n", MchBar, R_IOC_TSE_BDF_EN_BASE));
  IocTseBdfEn.Data32 = MmioRead32 (MchBar + R_IOC_TSE_BDF_EN_BASE);
  DEBUG ((DEBUG_INFO, "Before programming programming IocTseBdfEn  : 0x%08x\n",    IocTseBdfEn.Data32));
  DEBUG ((DEBUG_INFO, "(MchBar) 0x%x + (R_IOC_TSE_BDF_CMP0_BASE) 0x%x\n", MchBar, R_IOC_TSE_BDF_CMP0_BASE));
  IocTseBdfCmp0.Data = MmioRead64 (MchBar + R_IOC_TSE_BDF_CMP0_BASE);
  DEBUG ((DEBUG_INFO, "Before programming IocTseBdfCmp0: 0x%016llx\n", IocTseBdfCmp0.Data));

  if (RegisteredDevices >= 4) {
    DEBUG ((DEBUG_INFO, "(MchBar) 0x%x + (R_IOC_TSE_BDF_CMP1_BASE) 0x%llx\n", MchBar, R_IOC_TSE_BDF_CMP1_BASE));
    IocTseBdfCmp1.Data = MmioRead64 (MchBar + R_IOC_TSE_BDF_CMP1_BASE);
    DEBUG ((DEBUG_INFO, "Before programming IocTseBdfCmp1: 0x%016llx\n", IocTseBdfCmp1.Data));
  }

  NewBDF = UINT16_BDF (Bus, Device, Function);

  //
  // Verify the BDF had not been registered before
  //
  for (DeviceIdx = 0; DeviceIdx < RegisteredDevices; DeviceIdx++) {
    switch (DeviceIdx) {
    case 0:
      ConfiguredBDF = (UINT16) IocTseBdfCmp0.Bits.BDF0;
      break;
    case 1:
      ConfiguredBDF = (UINT16) IocTseBdfCmp0.Bits.BDF1;
      break;
    case 2:
      ConfiguredBDF = (UINT16) IocTseBdfCmp0.Bits.BDF2;
      break;
    case 3:
      ConfiguredBDF = (UINT16) IocTseBdfCmp0.Bits.BDF3;
      break;
    case 4:
      ConfiguredBDF = (UINT16) IocTseBdfCmp1.Bits.BDF4;
      break;
    case 5:
      ConfiguredBDF = (UINT16) IocTseBdfCmp1.Bits.BDF5;
      break;
    case 6:
      ConfiguredBDF = (UINT16) IocTseBdfCmp1.Bits.BDF6;
      break;
    case 7:
      ConfiguredBDF = (UINT16) IocTseBdfCmp1.Bits.BDF7;
      break;
    default:
      ConfiguredBDF = 0;
      break;
    }

    if (ConfiguredBDF == 0) {
      continue;
    }

    if (ConfiguredBDF == NewBDF) {
      DEBUG ((DEBUG_WARN, "BDF had already been registered! B:0x%x, D:0x%x, F:0x%x\n", Bus, Device, Function));
      EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
      return EFI_ALREADY_STARTED;
    }
  }

  //
  // Register new BDF
  //
  switch (RegisteredDevices) {
  case 0:
    IocTseBdfCmp0.Bits.BDF0 = NewBDF;
    IocTseBdfEn.Bits.Enable0 = 1;
    break;
  case 1:
    IocTseBdfCmp0.Bits.BDF1 = NewBDF;
    IocTseBdfEn.Bits.Enable1 = 1;
    break;
  case 2:
    IocTseBdfCmp0.Bits.BDF2 = NewBDF;
    IocTseBdfEn.Bits.Enable2 = 1;
    break;
  case 3:
    IocTseBdfCmp0.Bits.BDF3 = NewBDF;
    IocTseBdfEn.Bits.Enable3 = 1;
    break;
  case 4:
    IocTseBdfCmp1.Bits.BDF4 = NewBDF;
    IocTseBdfEn.Bits.Enable4 = 1;
    break;
  case 5:
    IocTseBdfCmp1.Bits.BDF5 = NewBDF;
    IocTseBdfEn.Bits.Enable5 = 1;
    break;
  case 6:
    IocTseBdfCmp1.Bits.BDF6 = NewBDF;
    IocTseBdfEn.Bits.Enable6 = 1;
    break;
  case 7:
    IocTseBdfCmp1.Bits.BDF7 = NewBDF;
    IocTseBdfEn.Bits.Enable7 = 1;
    break;
  default:
    EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
    return EFI_OUT_OF_RESOURCES;
    break;
  }

  MmioWrite32 (MchBar + R_IOC_TSE_BDF_EN_BASE, IocTseBdfEn.Data32);

  if ((RegisteredDevices) < 4) {
    MmioWrite64 (MchBar + R_IOC_TSE_BDF_CMP0_BASE, IocTseBdfCmp0.Data);
  } else {
    MmioWrite64 (MchBar + R_IOC_TSE_BDF_CMP1_BASE, IocTseBdfCmp1.Data);
  }
  DEBUG ((DEBUG_INFO, "Registered B:0x%x, D:0x%x, F:0x%x at index %d\n", Bus, Device, Function, RegisteredDevices));

  IocTseBdfEn.Data32 = MmioRead32 (MchBar + R_IOC_TSE_BDF_EN_BASE);
  IocTseBdfCmp0.Data = MmioRead64 (MchBar + R_IOC_TSE_BDF_CMP0_BASE);
  IocTseBdfCmp1.Data = MmioRead64 (MchBar + R_IOC_TSE_BDF_CMP1_BASE);
  DEBUG ((DEBUG_INFO, "After programming IocTseBdfEn  : 0x%08x\n",    IocTseBdfEn.Data32));
  DEBUG ((DEBUG_INFO, "After programming IocTseBdfCmp0: 0x%016llx\n", IocTseBdfCmp0.Data));
  DEBUG ((DEBUG_INFO, "After programming IocTseBdfCmp1: 0x%016llx\n", IocTseBdfCmp1.Data));

  mTseDriverContextPtr->EngineInfo.RegisteredDevices++;
  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));

  return EFI_SUCCESS;
}

/**
   SetBlockIoMedia() configures the Block IO Media instance into the TSE protocol context.

   This function must be called to set the context of a TSE read or write operation.
   This function can also be called to clean up the context after having issues the read or write operation.

   The configured Block IO Media instance is part of the TSE IO context and any BlockIoCrypto IO
   operation calling SubmitTseTableEntries() will consume this context as part of the TSE
   Table configuration.

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] BlockIoCryptoProtocol Instance of the Block IO Crypto Protocol to use in TSE IO operation.

   @retval EFI_SUCCESS             if configuration of the BlockIoMedia instance succeded
**/
EFI_STATUS
EFIAPI
SetBlockIoMedia (
  IN EFI_BLOCK_IO_MEDIA *BlockIoMedia
  )
{
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));
  mTseDriverContextPtr->TseIoContext.BlockIoMedia = BlockIoMedia;
  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));

  return EFI_SUCCESS;
}

/**
   SetCryptoKey() configures the Crypto Key Index and Tweak value into the TSE protocol context.

   This function must be called to set the context of a TSE read or write operation.
   This function can also be called to clean up the context after having issues the read or write operation.

   This Key and Tweak values are consumed after calling to SubmitTseTableEntries().

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] Index                Input key index associated with UEFI configuration table entry
   @param[in] CryptoIvInput        Input tweak value to use in the IO operation

   @retval EFI_SUCCESS             if crypto configuration into IO context succeded.
**/
EFI_STATUS
EFIAPI
SetCryptoConfig (
  IN UINT64  *Index,
  IN VOID   *CryptoIvInput
  )
{
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));
  mTseDriverContextPtr->TseIoContext.CryptoKeyIndex = Index;
  mTseDriverContextPtr->TseIoContext.CryptoIvInput = CryptoIvInput;
  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
  return EFI_SUCCESS;
}

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
  )
{
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  UINT64 TweakValue = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));

  TweakValue = mTseDriverContextPtr->TseIoContext.CryptoIvInput->CryptoBlockNumber;
  TweakValue += TweakIncrement;
  //
  // Check for overflow condition before updating TseIoContext
  //
  if (TweakValue > mTseDriverContextPtr->TseIoContext.CryptoIvInput->CryptoBlockNumber) {
    mTseDriverContextPtr->TseIoContext.CryptoIvInput->CryptoBlockNumber = TweakValue;
  } else {
    DEBUG ((DEBUG_INFO, "[TseProtocol] CryptoBlockNumber increment overflow error!\n"));
    Status = EFI_INVALID_PARAMETER;
  }
  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
  return Status;
}

/**
   This function retrieves the value of CryptoBlockNumber from the configured CryptoIvInput
   associated with the TSE IO Context and returns it as the TweakValue.

   The function performs a sanity check on the CryptoIvInput and it's contents.
   Only if the CryptoIvInput is configured and the CryptoBlockByte value is valid,
   it will write the CryptoBlockNumber value into the TweakValue.

   If any of the tweak value checks fail, the function returns with EFI_UNSUPPORTED.

   @param[in] TweakValue  Value to be programmed as the starting LBA (BASE_LBA) field of the
   TSE Table upon passing the CryptoBlockByteSize input validation check.

   @retval   EFI_SUCCESS If the tweak value was successfully retrieved from the TSE IO Context.
   @retval   EFI_UNSUPPORTED If the CryptoIvInput configuration was not available or invalid.

**/
EFI_STATUS
GetCryptoBlockNumber (
  UINT64 * TweakValue
  )
{
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  //
  // Verify that the CryptoIvInput has been configured in the TSE IO Context
  //
  if (mTseDriverContextPtr->TseIoContext.CryptoIvInput == NULL) {
    DEBUG ((DEBUG_ERROR, "CryptoIvInput is NULL!\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Verify that the CryptoIvInput configuration matches the media's block size
  //
  if (mTseDriverContextPtr->TseIoContext.CryptoIvInput->CryptoBlockByteSize != mTseDriverContextPtr->TseIoContext.BlockIoMedia->BlockSize) {
    DEBUG ((DEBUG_ERROR, "CryptoIvInput Blocksize != BlockIoMedia blocksize!\n"));
    return EFI_UNSUPPORTED;
  }

  *TweakValue = mTseDriverContextPtr->TseIoContext.CryptoIvInput->CryptoBlockNumber;
  return EFI_SUCCESS;
}


/**
   This function scans the UEFI Crypto Configuration table for the TSE key index associated
   with the TSE IO Context

   @retval   CryptoKey  Pointer to the TSE key index in the UEFI Crypto configuration table
                        if a match is found. NULL if no key with the associated index was found.
**/
UINT8 *
GetCryptoKeyFromUefiConfigTable (
  VOID
  )
{
  UINT8    *CryptoKeyPtr;
  UINT64   ConfiguredKeyIndex;
  UINT64   ConfigurationMaxEntries;
  UINT64   TableEntryIdx;
  EFI_GUID NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  CryptoKeyPtr = NULL;
  //
  // Retrieve ConfiguredKeyIndex from TSE IO Context
  //
  ConfiguredKeyIndex = *(mTseDriverContextPtr->TseIoContext.CryptoKeyIndex);

  //
  // Scan UEFI Table Configuration entries with
  // a KeyOwnerGuid different to NIL_GUID, which
  // match the configured index in the IO context.
  //
  ConfigurationMaxEntries = mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount;

  for (TableEntryIdx = 0; TableEntryIdx < ConfigurationMaxEntries; TableEntryIdx++) {
    if (!CompareGuid (&NilGuid, &(mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].KeyOwnerGuid)) &&
        (ConfiguredKeyIndex == mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].Index)) {
      DEBUG ((DEBUG_INFO, "TableEntryIdx [%d] Index: %d found!\n", TableEntryIdx, mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].Index));
      //
      // If matching index is found in the UEFI Crypto Configuration table, retrieve key.
      // Note: In TSE the CryptoKey stored in the UEFI Crypto Configuration is not an actual
      // AES-XTS 256B key, but a 4-BIT index to the key obtained through PCONFIG.
      //
      CryptoKeyPtr = (UINT8 *) (mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].CryptoKey);
      return CryptoKeyPtr;
    }
  }

  DEBUG ((DEBUG_ERROR, "ERROR: Index %d not found!\n", mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].Index));
  return NULL;
}

/**
   This function checks that the TSE IO context has been defined prior to issuing an IO operation

   @retval TRUE      if the TSE IO Context is valid
   @retval FALSE     if the TSE IO Context is not valid
**/
BOOLEAN
IsTseIoContextValid (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));
  if ((mTseDriverContextPtr->TseIoContext.BlockIoMedia != NULL) &&
      (mTseDriverContextPtr->TseIoContext.CryptoKeyIndex != NULL)) {
    return TRUE;
  }
  return FALSE;
}

/**
   This function will invalidate the entire IOCCE cache
   and clear all the TSE DRAM table contents.
**/
VOID
ClearTseDramTable (
  VOID
  )
{
  UINT64        MchBar = 0;
  TSE_IOCCE_TSE_CACHE_INVALIDATION_STRUCT IocceTseCacheInvalidationStruct;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  //
  // Invalidate cache for current entries
  //
  ZeroMem (&IocceTseCacheInvalidationStruct, sizeof (TSE_IOCCE_TSE_CACHE_INVALIDATION_STRUCT));
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  IocceTseCacheInvalidationStruct.Data = MmioRead32 (MchBar + R_IOCCE_TSE_BASE + B_IOCCE_TSE_CACHE_INVALIDATION_MCHBAR);
  IocceTseCacheInvalidationStruct.Data = TSE_FLUSH_ENTIRE_IOCCE_CACHE_BIT;
  MmioWrite32 ((MchBar + R_IOCCE_TSE_BASE + B_IOCCE_TSE_CACHE_INVALIDATION_MCHBAR), IocceTseCacheInvalidationStruct.Data);

  //
  // Read back register to enforce HW cache invalidation barrier
  //
  IocceTseCacheInvalidationStruct.Data = MmioRead32 (MchBar + R_IOCCE_TSE_BASE + B_IOCCE_TSE_CACHE_INVALIDATION_MCHBAR);

  //
  // Clear current table entries
  //
  ZeroMem (mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress, TSE_TABLE_SIZE);
  mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex = TSE_TABLE_FIRST_IDX;
}

/**
   This function dumps the requested number of TSE Lines starting at a given index

   @param[in]      TseLineStartIndex   Index associated with the starting TSE line
   @param[in]      TseLineNo      Number of lines to dump
**/
VOID
DumpTseLines (
  IN UINT16  TseLineStartIndex,
  IN UINT16  TseLineNo
  )
{
  TSE_LINE *TseLinePtr;
  UINT16   TseLineIndex;
  UINT16 GppaIndex;
  UINT8  ByteIndex;
  UINT8  *BytePtr;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));
  CONST UINT8 MaxGppaIndex = TSE_GPPA_ENTRIES_PER_LINE - 1;

  for (TseLineIndex = 0; TseLineIndex < TseLineNo; TseLineIndex++) {
    if ((TseLineStartIndex + TseLineIndex) >= TSE_TABLE_TOTAL_ENTRIES) {
      DEBUG ((DEBUG_INFO, "  Line index: %d is out of bounds! Aborting dump...\n", TseLineStartIndex + TseLineIndex));
      return;
    }
    TseLinePtr = &(mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress[TseLineStartIndex + TseLineIndex]);

    DEBUG ((DEBUG_INFO, "  Header @   0x%016llx\n", (UINTN *) TseLinePtr));
    DEBUG ((DEBUG_INFO, "  Header     0x%016llx %04x\n", *(UINT64 *)&(TseLinePtr->Header.Data[0]), TseLinePtr->Header.Data[8]));

    DEBUG ((DEBUG_INFO, "  OffsetType 0x%x\n", TseLinePtr->Header.Bits.OffsetType));
    DEBUG ((DEBUG_INFO, "  BlkSize    0x%x\n", TseLinePtr->Header.Bits.BlkSize));
    DEBUG ((DEBUG_INFO, "  KeyIdx     0x%x\n", TseLinePtr->Header.Bits.KeyIdx));
    DEBUG ((DEBUG_INFO, "  Offset     0x%x\n", TseLinePtr->Header.Bits.Offset));
    DEBUG ((DEBUG_INFO, "  BLBA/Tweak 0x%016llx\n", TseLinePtr->Header.Bits.Tweak));

    for (GppaIndex = 0; GppaIndex < TSE_GPPA_ENTRIES_PER_LINE; GppaIndex++) {
      //
      // Print TSE line GPPA entries from MSB to LSB
      //
      BytePtr = (UINT8 *) (TseLinePtr->GPPA + (MaxGppaIndex - GppaIndex));
      DEBUG ((DEBUG_INFO, "   GPPA[%02d]: 0x", GppaIndex));
      for (ByteIndex = 0; ByteIndex < sizeof (GPPA_ENTRY); ByteIndex++) {
        DEBUG ((DEBUG_INFO, "%02x", *(BytePtr + (sizeof (GPPA_ENTRY) - 1) - ByteIndex)));
      }
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }
}

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
)
{
  TSE_LINE *TseLinePtr;
  UINT64 TruncatedPrp;
  UINT64 PatchedPrp;
  CONST UINT8 MaxGppaIndex = TSE_GPPA_ENTRIES_PER_LINE - 1;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (GppaIndex >= TSE_GPPA_ENTRIES_PER_LINE) {
    DEBUG ((DEBUG_ERROR, "  Invalid GppaIndex! %d\n", GppaIndex));
    ASSERT (GppaIndex < TSE_GPPA_ENTRIES_PER_LINE);
    return EFI_INVALID_PARAMETER;
  }

  TseLinePtr = &(mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress[TseLineIndex]);

  DEBUG ((DEBUG_INFO, "  TseLinePtr: 0x%016llx\n", TseLinePtr));
  TruncatedPrp = RShiftU64 (*Prp, 12);
  DEBUG ((DEBUG_INFO, "  Truncated PRP 0x%09llx\n", TruncatedPrp));

  //
  // Inject truncated PRP into TSE Line GPPA entry
  // NOTE: GPPAs are injected from MSB to LSB inside the TSE line.
  //
  TseLinePtr->GPPA[MaxGppaIndex - GppaIndex].Data[0] = (UINT8) (TruncatedPrp & 0xFF);
  TseLinePtr->GPPA[MaxGppaIndex - GppaIndex].Data[1] = (UINT8) (RShiftU64 (TruncatedPrp, 8) & 0xFF);
  TseLinePtr->GPPA[MaxGppaIndex - GppaIndex].Data[2] = (UINT8) (RShiftU64 (TruncatedPrp, 16) & 0xFF);
  TseLinePtr->GPPA[MaxGppaIndex - GppaIndex].Data[3] = (UINT8) (RShiftU64 (TruncatedPrp, 24) & 0xFF);
  TseLinePtr->GPPA[MaxGppaIndex - GppaIndex].Data[4] = (UINT8) (RShiftU64 (TruncatedPrp, 32) & 0xFF);

  //
  // Patch TseLine index into PRP
  //
  DEBUG ((DEBUG_INFO, "  Prp @ 0x%016llx\n", Prp));
  DEBUG ((DEBUG_INFO, "  Original Page Address 0x%016llx\n", *Prp));
  PatchedPrp = (LShiftU64 (TseLineIndex, 52)) | *Prp;
  *Prp = PatchedPrp;
  DEBUG ((DEBUG_INFO, "  Patched Page Address 0x%016llx\n", *Prp));

  return EFI_SUCCESS;
}

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
  )
{
  TSE_LINE *TseLinePtr;
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  TseLinePtr = &(mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress[TseLineIndex]);
  DEBUG ((DEBUG_INFO, "  Current TseLineIndex: 0x%x\n", mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex));
  DEBUG ((DEBUG_INFO, "  TseLinePtr: 0x%016llx\n", TseLinePtr));

  TseLinePtr->Header.Bits.OffsetType = OffsetType;     // Offset type: 1 = Page offset, 0 = LBA offset
  TseLinePtr->Header.Bits.BlkSize    = BlockSize;      // BlockSize: 0 = 4KB, 1 = 512B
  TseLinePtr->Header.Bits.KeyIdx     = KeyIdx;         // Input cryptokey from UEFI configuration
  TseLinePtr->Header.Bits.Offset     = Offset;         // Offset: Page or Sector
  TseLinePtr->Header.Bits.Tweak      = Tweak;          // Custom tweak input

  DEBUG ((DEBUG_INFO, "  OffsetType 0x%x\n", TseLinePtr->Header.Bits.OffsetType));
  DEBUG ((DEBUG_INFO, "  BlkSize    0x%x\n", TseLinePtr->Header.Bits.BlkSize));
  DEBUG ((DEBUG_INFO, "  KeyIdx     0x%x\n", TseLinePtr->Header.Bits.KeyIdx));
  DEBUG ((DEBUG_INFO, "  Offset     0x%x\n", TseLinePtr->Header.Bits.Offset));
  DEBUG ((DEBUG_INFO, "  BLBA/Tweak 0x%016llx\n", TseLinePtr->Header.Bits.Tweak));

  if (TseLinePtr->Header.Bits.Tweak != Tweak) {
    DEBUG ((DEBUG_ERROR, "  ERROR: Truncated Tweak value!\n"));
    Status = EFI_INVALID_PARAMETER;
  }
  if (TseLinePtr->Header.Bits.Offset != Offset) {
    DEBUG ((DEBUG_ERROR, "  ERROR: Truncated Offset value!\n"));
    Status = EFI_INVALID_PARAMETER;
  }
  if (TseLinePtr->Header.Bits.KeyIdx != KeyIdx) {
    DEBUG ((DEBUG_ERROR, "  ERROR: Truncated KeyIdx value!\n"));
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**
   The SubmitTseTableEntries() configures the PRP List and the TSE DRAM Table entries.
   This API is called before NVMe command is issued.

   Please refer to TSE Table Configuration section for more details regards to how this
   API parameters are configured into the PRP List and the TSE DRAM Table.

   @param[in] Prp                  Pointer to base address of PRP lists in memory
   @param[in] Lba                  Starting LBA of the associated NVMe command
   @param[in] Bytes                Transfer length in bytes of the associated NVMe command

   @retval EFI_SUCCESS             if configuration of qualifier and enable registers succeded.
   @retval EFI_NOT_READY           if TSE driver context is NULL
   @retval EFI_INVALID_PARAMETER   if input parameters are invalid.
**/
EFI_STATUS
EFIAPI
SubmitTseTableEntries (
  IN  UINT64          *Prp,
  IN  UINT64          Lba,
  IN  UINT32          Bytes
  )
{
  EFI_STATUS            Status;
  EFI_BLOCK_IO_MEDIA    *Media;
  UINT32                BlockSize;
  UINT8                 BlockSizeType;
  UINT32                BlockOffset;
  UINT32                LbaOffset;
  UINT32                PageOffset;
  UINT64                Pages;
  UINT64                BytesWithOffset;

  UINT8                 *CryptoKeyPtr;

  BOOLEAN               IsPageAligned;
  BOOLEAN               IsSectorAligned;
  BOOLEAN               SecondTseLineWithOffset;

  VOID                  **PrpListHost;
  UINTN                 PrpListNo;
  UINT64                PrpListRemainder;

  UINTN                 PrpListIndex;
  UINT64                PrpListBase;
  UINTN                 PrpEntryIndex;
  UINTN                 PrpEntryNo;
  TSE_PRP_LINE_TYPE     TsePrpLineType;

  UINT8                 TseLineGppaIndex;
  UINTN                 TseLineNo;
  UINT64                TseLineRemainder;
  UINT16                TseLineRemainingEntries;
  UINT16                *CurrentTseLineIndexPtr;
  UINT64                TweakValue;
  UINT64                LbasPerLine;
  UINT64                LbasInOffset;
  UINT64                LbasInOffsetRemainder;
  UINT8                 TseLineOffset;
  UINT8                 TseLineOffsetType;

  CryptoKeyPtr = NULL;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (mTseDriverContextPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Driver context is NULL!\n"));
    return EFI_NOT_READY;
  }

  if (IsTseIoContextValid () == FALSE) {
    DEBUG ((DEBUG_ERROR, "TSE IO context is not configured!\n"));
    return EFI_NOT_READY;
  }

  //
  // Get Crypto key (TSE Key index) from TseIoContext
  //
  CryptoKeyPtr = GetCryptoKeyFromUefiConfigTable ();
  if (CryptoKeyPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Unable to retrieve Crypto Key for IO operation!\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Sanity check hard requirement for TSE IO: 16B alignment
  //
  if ((Prp[0] & (TSE_IO_16_BYTE_ALIGNMENT - 1)) != 0) {
    DEBUG ((DEBUG_ERROR, " ERROR: IO is not 16B aligned!"));
    ASSERT ((Prp[0] & (TSE_IO_16_BYTE_ALIGNMENT - 1)) != 0);
    return EFI_UNSUPPORTED;
  }

  //
  // Retrieve Crypto IV tweak value from TseIoContext
  //
  Status = GetCryptoBlockNumber (&TweakValue);

  //
  // If tweak value was not available or invalid, return error without performing IO.
  //
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, " TweakValue:   0x%016llx\n", TweakValue));

  DEBUG ((DEBUG_INFO, " TsePrp[0]:    0x%x\n", Prp[0]));
  DEBUG ((DEBUG_INFO, " Starting LBA: 0x%x\n", Lba));
  DEBUG ((DEBUG_INFO, " Bytes:        0x%x\n", Bytes));

  //
  // Get page offset from PRP[0]
  //
  PageOffset = Prp[0] & (EFI_PAGE_SIZE - 1);
  IsPageAligned = PageOffset == 0 ? TRUE : FALSE;
  DEBUG ((DEBUG_INFO, " PageSize: 0x%x, PageOffset: 0x%x\n", EFI_PAGE_SIZE, PageOffset));
  BytesWithOffset = (UINT64) PageOffset + (UINT64) Bytes;

  //
  // Get media block size and block offset
  //
  Media = mTseDriverContextPtr->TseIoContext.BlockIoMedia;
  BlockSize = Media->BlockSize;
  BlockSizeType = BlockSize < SIZE_4KB ? TSE_LINE_BLOCK_SIZE_512B : TSE_LINE_BLOCK_SIZE_4KB;

  //
  // Get block offset from PRP[0]
  //
  BlockOffset = Prp[0] & (BlockSize - 1);
  IsSectorAligned = BlockOffset == 0 ? TRUE : FALSE;

  //
  // Calculate the number of LBAs skipped in Page offset
  //
  LbasInOffset = (UINTN) DivU64x64Remainder (
                           (UINT64) PageOffset,
                           (UINT64) BlockSize,
                           &LbasInOffsetRemainder);

  // Increase skipped LBAs by one if remainder is not 0.
  LbasInOffset += LbasInOffsetRemainder == 0 ? 0 : 1;

  LbaOffset = (BlockSize - BlockOffset)  >> 4 ;

  DEBUG ((DEBUG_INFO, " Media->BlockSize: 0x%x, BlockOffset: 0x%x, LbaOffset: 0x%x, LbasInOffset: 0x%x\n", BlockSize, BlockOffset, LbaOffset, LbasInOffset));

  //
  // TseLineOffset is always the Page Offset for the first TSE line entry
  // Convert offset value for TSE lines into 16B granularity.
  //
  TseLineOffset = (UINT8) (PageOffset >> 4);

  //
  // Calculate the number of LBAs per TSE Line
  //
  LbasPerLine = (UINTN) DivU64x64Remainder (
                          (UINT64) EFI_PAGE_SIZE * TSE_GPPA_ENTRIES_PER_LINE,
                          (UINT64) BlockSize,
                          NULL);

  //
  // Calculate the number of PRP entries in a memory page.
  //
  Pages = EFI_SIZE_TO_PAGES (BytesWithOffset);
  DEBUG ((DEBUG_INFO, " Pages: 0x%x\n", Pages));

  TseLineNo = (UINTN) DivU64x64Remainder ((UINT64) Pages, (UINT64) TSE_GPPA_ENTRIES_PER_LINE, &TseLineRemainder);
  DEBUG ((DEBUG_INFO, " TseLineNo: 0x%x, TseLineRemainder: 0x%x\n", TseLineNo, TseLineRemainder));

  //
  // Initialize flag to page aligned case
  //
  SecondTseLineWithOffset = FALSE;
  TseLineOffsetType = (UINT8) TSE_LINE_PAGE_OFFSET_TYPE;

  //
  // Determine TSE Line type
  //
  if ((IsPageAligned == TRUE) &&
      (TseLineNo == 0)) {
    TsePrpLineType = TSE_SINGLE_LINE_PAGE_ALIGNED;
  } else if ((IsPageAligned == FALSE) &&
             (TseLineNo == 0) &&
             (TseLineRemainder < (TSE_GPPA_ENTRIES_PER_LINE - 1))) {
    TsePrpLineType = TSE_SINGLE_LINE_PAGE_UNALIGNED;
  } else if ((IsPageAligned == TRUE) &&
             (TseLineNo > 0)) {
    TsePrpLineType = TSE_MULTIPLE_LINE_PAGE_ALIGNED;
  } else if ((IsPageAligned == FALSE) &&
             (IsSectorAligned == TRUE) &&
             (TseLineNo > 0)) {
    TsePrpLineType = TSE_MULTIPLE_LINE_SECTOR_ALIGNED;
    //
    // This flag ensures that TseLineOffset is cleared
    // in all subsequent TSE lines.
    //
    SecondTseLineWithOffset = TRUE;
  } else {
    TsePrpLineType = TSE_MULTIPLE_LINE_PAGE_IO_SPLIT;
    //
    // This flag ensures that TseLineOffset is updated with the LBA offset
    // and it is used in all subsequent TSE lines.
    //
    SecondTseLineWithOffset = TRUE;
  }

  DEBUG ((DEBUG_INFO, "  TSE Line Type: 0x%x\n", TsePrpLineType));

  //
  // Calculate total PrpList number.
  //
  PrpEntryNo = EFI_PAGE_SIZE / sizeof (UINT64);
  PrpListNo = (UINTN) DivU64x64Remainder ((UINT64) Pages - 1, (UINT64) PrpEntryNo - 1, &PrpListRemainder);

  if (PrpListNo == 0) {
    PrpListNo = 1;
  } else if ((PrpListRemainder != 0) && (PrpListRemainder != 1)) {
    PrpListNo += 1;
  } else if (PrpListRemainder == 1) {
    PrpListRemainder = PrpEntryNo;
  } else if (PrpListRemainder == 0) {
    PrpListRemainder = PrpEntryNo - 1;
  }

  DEBUG ((DEBUG_INFO, " PrpListNo [0x%x]\n", PrpListNo));
  DEBUG ((DEBUG_INFO, " PrpListRemainder [0x%x]\n", PrpListRemainder));

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));

  //
  // Calculate remaining free TSE lines in DRAM Table
  //
  TseLineRemainingEntries = TSE_TABLE_USABLE_ENTRIES - (mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex) + 1;
  DEBUG ((DEBUG_INFO, "Required TSE Lines: %d, Remaining TSE Lines: %d\n", (TseLineNo + 1), TseLineRemainingEntries));

  //
  // Check if we need to clear the TSE DRAM to free additional TSE lines
  //
  if (TseLineRemainingEntries < (TseLineNo + 1)) {
    ClearTseDramTable ();
    TseLineRemainingEntries = TSE_TABLE_USABLE_ENTRIES;
  }

  //
  // Confirm enough free TSE lines are available for completing IO request
  //
  if (TseLineRemainingEntries < (TseLineNo + 1)) {
    DEBUG ((DEBUG_ERROR, "ERROR: Insufficient TSE entries to perform requested IO!\n"));
    EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
    ASSERT (TseLineRemainingEntries < (TseLineNo + 1));
    return EFI_OUT_OF_RESOURCES;
  }

  CurrentTseLineIndexPtr = &(mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex);
  DEBUG ((DEBUG_INFO, " TSE DRAM Table Address: 0x%016llx\n", (mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress)));
  TseLineGppaIndex = 0;

  //
  // Process PRP[0] contents
  // First TSE Line in IO
  //
  Status = PopulateTseLineHeader (
             *CurrentTseLineIndexPtr,
             TweakValue,
             TseLineOffset,
             BlockSizeType,
             *CryptoKeyPtr,
             TseLineOffsetType
             );

  if (EFI_ERROR (Status)) {
    EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Inject PRP[0] entry into GPPA
  DEBUG ((DEBUG_INFO, " TseLineGppaIndex:    0x%x\n", TseLineGppaIndex));
  Status = InjectGppaPatchPrp (
             *CurrentTseLineIndexPtr,
             TseLineGppaIndex,
             &(Prp[0])
             );

  if (EFI_ERROR (Status)) {
    EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
    return EFI_ABORTED;
  }

  TseLineGppaIndex++;

  //
  // Process PRP[1] contents
  // Parsing cases:
  //
  if (BytesWithOffset > (EFI_PAGE_SIZE * 2)) {
    //
    // a) PRP[1] is a list
    //
    DEBUG ((DEBUG_INFO, " List TsePrp[1]: 0x%016llx\n", Prp[1]));

    PrpListHost = (VOID **) &(Prp[1]);

    DEBUG ((DEBUG_INFO, " Parsing PRP list...\n"));
    DEBUG ((DEBUG_INFO, " PrpListHost @ 0x%016llx = 0x%016llx\n", PrpListHost, *PrpListHost));

    //
    // Parse first N-1 PRP lists
    // Remainder entries are parsed separately
    //
    if (PrpListNo > 1) {
      DEBUG ((DEBUG_INFO, " PrpListNo: 0x%x, PrpEntryNo: 0x%x\n", PrpListNo, PrpEntryNo));
    }

    for (PrpListIndex = 0; PrpListIndex < PrpListNo - 1; ++PrpListIndex) {
      DEBUG ((DEBUG_INFO, " PrpListIndex [0x%x]\n", PrpListIndex));
      PrpListBase = *(UINT64 *) PrpListHost + PrpListIndex * EFI_PAGE_SIZE;
      DEBUG ((DEBUG_INFO, " PrpListBase [0x%016llx]\n", PrpListBase));

      for (PrpEntryIndex = 0; PrpEntryIndex < PrpEntryNo; ++PrpEntryIndex) {
        if (PrpEntryIndex != PrpEntryNo - 1) {
          DEBUG ((DEBUG_INFO, " PrpEntryIndex [0x%x] @ 0x%llx: 0x%016llx\n", PrpEntryIndex, ((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex), *((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex)));
          //
          // If TSE line GPPA entries are full, move to new TSE Line.
          //
          if (TseLineGppaIndex >= TSE_GPPA_ENTRIES_PER_LINE) {

            //
            // Dump current line
            //
            DEBUG ((DEBUG_INFO, " Current TseLineIndex: 0x%x\n", *CurrentTseLineIndexPtr));
            DumpTseLines (*CurrentTseLineIndexPtr, 1);

            //
            // Configure new line
            //
            if (SecondTseLineWithOffset == TRUE) {
              //
              // Disable flag for subsequent cycles
              //
              SecondTseLineWithOffset = FALSE;

              //
              // When more than one TSE line is used, the first line skips as many
              // LBAs as the offset contains.
              //
              TweakValue += LbasPerLine - LbasInOffset;

              if (IsSectorAligned == TRUE) {
                //
                // Clear TSE Page offset for the reset of the TSE lines
                //
                TseLineOffset = (UINT8) 0;
              } else {
                //
                // For sector unaligned entries, TseLineOffset becomes the LBA offset
                // Convert offset value for TSE lines into 16B granularity.
                //
                // Note: All remaining entries will continue to use the LBA offset.
                //
                TseLineOffsetType = (UINT8) TSE_LINE_LBA_OFFSET_TYPE;
                TseLineOffset = (UINT8) LbaOffset;
              }
            } else {
              //
              // For all other lines, Tweak value always increments by LbasPerLine
              //
              TweakValue += LbasPerLine;
            }

            //
            // Reset Gppa Index for new line.
            //
            TseLineGppaIndex = 0;

            //
            // Populate new TSE line
            //
            *CurrentTseLineIndexPtr = *CurrentTseLineIndexPtr + 1;
            Status = PopulateTseLineHeader (
                       *CurrentTseLineIndexPtr,
                       TweakValue,
                       TseLineOffset,
                       BlockSizeType,
                       *CryptoKeyPtr,
                       TseLineOffsetType
                       );

            if (EFI_ERROR (Status)) {
              EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
              ASSERT_EFI_ERROR (Status);
              return Status;
            }
          }

          DEBUG ((DEBUG_INFO, " TseLineGppaIndex:    0x%x\n", TseLineGppaIndex));
          Status = InjectGppaPatchPrp (
                     *CurrentTseLineIndexPtr,
                     TseLineGppaIndex,
                     ((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex)
                     );

          if (EFI_ERROR (Status)) {
            EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
            return EFI_ABORTED;
          }

          TseLineGppaIndex++;
        } else {
          //
          // New PRP list, Not a PRP entry.
          //
          DEBUG ((DEBUG_INFO, " New PrpList [0x%x] @ 0x%llx: 0x%016llx\n", PrpEntryIndex, ((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex), *((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex)));
        }
      }
    }

    //
    // Last PRP list
    // Parsing remainder entries
    //
    for (PrpEntryIndex = 0; PrpEntryIndex < PrpListRemainder; ++PrpEntryIndex) {
      DEBUG ((DEBUG_INFO, " PrpListIndex [0x%x]\n", PrpListIndex));
      PrpListBase = *(UINT64 *)PrpListHost + PrpListIndex * EFI_PAGE_SIZE;
      DEBUG ((DEBUG_INFO, " PrpListBase [0x%016llx]\n", PrpListBase));
      for (PrpEntryIndex = 0; PrpEntryIndex < PrpListRemainder; ++PrpEntryIndex) {
        DEBUG ((DEBUG_INFO, " PrpEntryIndex [0x%x] @ 0x%016llx: 0x%016llx\n", PrpEntryIndex, ((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex), *((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex)));
        //
        // If TSE line GPPA entries are full, move to new TSE Line.
        //
        if (TseLineGppaIndex >= TSE_GPPA_ENTRIES_PER_LINE) {

          //
          // Dump current line
          //
          DEBUG ((DEBUG_INFO, " Current TseLineIndex: 0x%x\n", *CurrentTseLineIndexPtr));
          DumpTseLines (*CurrentTseLineIndexPtr, 1);

          //
          // Configure new line
          //
          if (SecondTseLineWithOffset == TRUE) {
            //
            // Disable flag for subsequent cycles
            //
            SecondTseLineWithOffset = FALSE;

            //
            // When more than one TSE line is used, the first line skips as many
            // LBAs as the offset contains.
            //
            TweakValue += LbasPerLine - LbasInOffset;

            if (IsSectorAligned == TRUE) {
              //
              // Clear TSE Page offset for the reset of the TSE lines
              //
              TseLineOffset = (UINT8) 0;
            } else {
              //
              // For sector unaligned entries, TseLineOffset becomes the LBA offset
              // Convert offset value for TSE lines into 16B granularity.
              //
              // Note: All remaining entries will continue to use the LBA offset.
              //
              TseLineOffsetType = (UINT8) TSE_LINE_LBA_OFFSET_TYPE;
              TseLineOffset = (UINT8) LbaOffset;
            }
          } else {
            //
            // For all other lines, Tweak value always increments by LbasPerLine
            //
            TweakValue += LbasPerLine;
          }

          //
          // Reset Gppa Index for new line.
          //
          TseLineGppaIndex = 0;

          //
          // Populate new TSE line
          //
          *CurrentTseLineIndexPtr = *CurrentTseLineIndexPtr + 1;
          Status = PopulateTseLineHeader (
                     *CurrentTseLineIndexPtr,
                     TweakValue,
                     TseLineOffset,
                     BlockSizeType,
                     *CryptoKeyPtr,
                     TseLineOffsetType
                     );

          if (EFI_ERROR (Status)) {
            EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
            ASSERT_EFI_ERROR (Status);
            return Status;
          }
        }

        DEBUG ((DEBUG_INFO, " TseLineGppaIndex:    0x%x\n", TseLineGppaIndex));
        Status = InjectGppaPatchPrp (
                   *CurrentTseLineIndexPtr,
                   TseLineGppaIndex,
                   ((UINT64 *) (UINTN) PrpListBase + PrpEntryIndex)
                   );

        if (EFI_ERROR (Status)) {
          EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
          return EFI_ABORTED;
        }

        TseLineGppaIndex++;
      }
    }
  } else {
    if (BytesWithOffset > (EFI_PAGE_SIZE)) {
      //
      // b) Two individual entries, PRP[0], PRP[1]
      //
      DEBUG ((DEBUG_INFO, " Two PRP entrys, PRP[1] is an additional entry\n"));
      DEBUG ((DEBUG_INFO, " TseLineGppaIndex:    0x%x\n", TseLineGppaIndex));
      Status = InjectGppaPatchPrp (
                 *CurrentTseLineIndexPtr,
                 TseLineGppaIndex,
                 &(Prp[1])
                 );

      if (EFI_ERROR (Status)) {
        EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
        return EFI_ABORTED;
      }
    } else {
      //
      // c) Single PRP[0] entry, PRP[1] is empty
      //
      DEBUG ((DEBUG_INFO, " Single PRP entry. Nothing to do.\n"));
    }
  }

  //
  // Dump current TSE line
  //
  DEBUG ((DEBUG_INFO, " Current TseLineIndex: 0x%x\n", *CurrentTseLineIndexPtr));
  DumpTseLines (*CurrentTseLineIndexPtr, 1);

  //
  // Complete current TSE line by moving pointer to next available TSE line
  //
  mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex++;
  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));
  return Status;
}

/**
   The GetCapabilities() provides the capabilities of the Crypto Engine.

   If there are multiple Crypto algorithms supported by the Crypto Engine than
   this API will account for those various instances of the algorithm in the
   CryptoCapability[] structure.  Below figure depicts the high-level flow of
   this API.

   @param[out] Capabilities      Pointer to the EFI_BLOCK_IO_CRYPTO_CAPABILITIES structure.

   @retval EFI_SUCCESS           Capabilities were copied successfully into output parameter.
   @retval EFI_NOT_READY         Capabilities is NULL.
   @retval EFI_NOT_READY         if TSE driver context is NULL
   @retval EFI_OUT_OF_RESOURCES  Unable to allocate memory for requested capabilities.
**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN OUT EFI_BLOCK_IO_CRYPTO_CAPABILITIES *Capabilities
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (mTseDriverContextPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Driver context is NULL!\n"));
    return EFI_NOT_READY;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITIES), (VOID **) Capabilities);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (Capabilities, &(mTseDriverContextPtr->EngineInfo.CryptoCap), sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITIES));

  return EFI_SUCCESS;
}

/**
   Obtains the first free entry available in the input UEFI CryptoConfigurationTable.

   @param[out] FreeEntryIndex     Pointer to a UINT64 with resulting FreeEntryIndex.
   @param[in]  ConfigurationCount Entry size of the Configuration Table.
   @param[in]  ConfigurationTable Pointer to the input Configuration Table.

   @retval EFI_SUCCESS             if free index is available in the configuration table and
                                   FreeEntryIndex now contains the resulting index.
   @retval EFI_OUT_OF_RESOURCES    if there are not enough available configuration entries.

**/
EFI_STATUS
GetAvailableEntryIndex (
  OUT UINT64                                           *FreeEntryIndex,
  IN  UINT64                                           ConfigurationCount,
  IN  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ConfigurationTable
  )
{
  UINT64     TableEntryIdx;
  BOOLEAN    FoundFreeEntryIndex = FALSE;
  EFI_GUID   NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  for (TableEntryIdx = 0; TableEntryIdx < ConfigurationCount; TableEntryIdx++) {
    if (CompareGuid (&(ConfigurationTable[TableEntryIdx].KeyOwnerGuid), &NilGuid)) {
      FoundFreeEntryIndex = TRUE;
      break;
    }
  }

  if (FoundFreeEntryIndex == FALSE) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, " Free entry found table index @ %d.\n", TableEntryIdx));
  *FreeEntryIndex = TableEntryIdx;
  return EFI_SUCCESS;
}

/**
   Release entries from CryptoConfigurationTable instance.

   @param[in]       RequestedChangesCount       Number of entries being configured.
   @param[in]       RequestedConfiguration      Pointer to a table with the requested changes.
   @param[in][out]  ModifiedConfigurationCount  Pointer to number of entries in table being configured.
   @param[in][out]  ModifiedConfigurationTable  Pointer to a table being configured

   @retval EFI_SUCCESS             if modifications occurred without exceptions
   @retval EFI_INVALID_PARAMETER   if ConfigurationTable is NULL.
   @retval EFI_INVALID_PARAMETER   if ConfigurationCount is 0.
   @retval EFI_OUT_OF_RESOURCES    if there are not enough available configuration entries to submit request.
**/
EFI_STATUS
EFIAPI
ProcessReleasedUefiTableEntries (
  IN     UINT64                                           RequestedChangesCount,
  IN     EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *RequestedConfigurationEntries,
  IN OUT UINT64                                           *ModifiedConfigurationCount,
  IN OUT EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ModifiedConfigurationTable
  )
{
  UINT64     CapCount;
  UINT64     CapIdx;
  BOOLEAN    CapSupported;

  UINT64     InputEntryIdx;
  UINT64     TableEntryIdx;

  UINT64     ConfigurationMaxEntries;
  EFI_GUID   NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  ConfigurationMaxEntries = mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount;
  CapCount = mTseDriverContextPtr->EngineInfo.CryptoCap.CapabilityCount;

  //
  // If entries being set are already in use by another owner, the call should fail
  // and none of the entries should be modified.
  //
  DEBUG ((DEBUG_INFO, "Sanitizing input entries...\n"));
  for (InputEntryIdx = 0; InputEntryIdx < RequestedChangesCount; InputEntryIdx++) {
    DEBUG ((DEBUG_INFO, "InputEntryIdx: %d\n", InputEntryIdx));
    for (TableEntryIdx = 0; TableEntryIdx < ConfigurationMaxEntries; TableEntryIdx++) {
      DEBUG ((DEBUG_INFO, "TableEntryIdx: %d\n", TableEntryIdx));

      //
      // Skip released entries
      //
      if (CompareGuid (&(ModifiedConfigurationTable[TableEntryIdx].KeyOwnerGuid), &NilGuid)) {
        continue;
      }

      //
      // If input Index had been allocated and input KeyOwnerGuid is NilGuid, release key
      //
      if ((RequestedConfigurationEntries[InputEntryIdx].Index == ModifiedConfigurationTable[TableEntryIdx].Index) &&
          (CompareGuid (&(RequestedConfigurationEntries[InputEntryIdx].KeyOwnerGuid), &NilGuid))) {
        //
        // Release entry with specified index
        //
        ModifiedConfigurationTable[TableEntryIdx].Index = 0;
        CopyGuid (&(ModifiedConfigurationTable[TableEntryIdx].KeyOwnerGuid), &NilGuid);
        ZeroMem (&(ModifiedConfigurationTable[TableEntryIdx].Capability), sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));
        ModifiedConfigurationTable[TableEntryIdx].CryptoKey = NULL;
        DEBUG ((DEBUG_INFO, "Released entry with index (%d)!\n", (RequestedConfigurationEntries[InputEntryIdx].Index)));

        if (*ModifiedConfigurationCount > 0) {
          *ModifiedConfigurationCount = *ModifiedConfigurationCount - 1;
        }
        continue;
      }

      //
      // Scan and compare every new entry's index against current entries' index.
      //
      if ((RequestedConfigurationEntries[InputEntryIdx].Index != EFI_BLOCK_IO_CRYPTO_INDEX_ANY) &&
          (RequestedConfigurationEntries[InputEntryIdx].Index == ModifiedConfigurationTable[TableEntryIdx].Index)) {
        DEBUG ((DEBUG_ERROR, "SubmitConfigurationTable () - Index (%d) has already been allocated.\n", RequestedConfigurationEntries[InputEntryIdx].Index));
        return EFI_INVALID_PARAMETER;
      }
      //
      // Scan and check that the requested capability is supported by the engine.
      //
      CapSupported = FALSE;
      for (CapIdx = 0; CapIdx < CapCount; CapIdx++) {
        if (CompareGuid (&(RequestedConfigurationEntries[InputEntryIdx].Capability.Algorithm), &(mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].Algorithm)) &&
            (RequestedConfigurationEntries[InputEntryIdx].Capability.KeySize == mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].KeySize) &&
            ((RequestedConfigurationEntries[InputEntryIdx].Capability.CryptoBlockSizeBitMask & mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].CryptoBlockSizeBitMask) != 0)) {
          CapSupported = TRUE;
          break;
        }
      }

      //
      // If any entry is unsupported by the engine, abort operation.
      //
      if (CapSupported == FALSE) {
        DEBUG ((DEBUG_ERROR, "Unsupported capability for entry with index (%d)!\n", RequestedConfigurationEntries[InputEntryIdx].Index));
        return EFI_UNSUPPORTED;
      }
    }
  }
  return EFI_SUCCESS;
}

/**
   Add Indexed entries from CryptoConfigurationTable instance.

   @param[in]       RequestedChangesCount       Number of entries being configured.
   @param[in]       RequestedConfiguration      Pointer to a table with the requested changes.
   @param[in][out]  ModifiedConfigurationCount  Pointer to number of entries in table being configured.
   @param[in][out]  ModifiedConfigurationTable  Pointer to a table being configured
   @param[in][out]  NewEntriesCount             Pointer to number of new entries registered.
   @param[in][out]  NewEntries                  Pointer to a table containing the new entries successfully registered.
   @param[in][out]  DeferredEntries             Pointer to flag signaling if the registration of unindexed entries was requested.

   @retval EFI_SUCCESS             if modifications occurred without exceptions
   @retval EFI_INVALID_PARAMETER   if RequestedConfigurationEntries is NULL.
   @retval EFI_INVALID_PARAMETER   if RequestedChangesCount is 0.
   @retval EFI_UNSUPPORTED         if any entry is unsupported by the engine, abort operation.
   @retval EFI_OUT_OF_RESOURCES    if there are not enough available configuration entries to submit request.
**/
EFI_STATUS
EFIAPI
ProcessIndexedUefiTableEntries (
  IN     UINT64                                           RequestedChangesCount,
  IN     EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *RequestedConfigurationEntries,
  IN OUT UINT64                                           *ModifiedConfigurationCount,
  IN OUT EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ModifiedConfigurationTable,
  IN OUT UINT64                                           *NewEntriesCount,
  OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *NewEntries,
  OUT    BOOLEAN                                          *DeferredEntries
  )
{
  EFI_STATUS Status;

  UINT64     InputEntryIdx;
  UINT64     ResultEntryIdx;

  UINT64     FreeTableEntryIndex;
  UINT64     ConfigurationMaxEntries;

  EFI_GUID   NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  ConfigurationMaxEntries = mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount;
  ResultEntryIdx = *NewEntriesCount;

  //
  // Submit indexed entries.
  //
  DEBUG ((DEBUG_INFO, "Parsing indexed entries...\n"));
  for (InputEntryIdx = 0; InputEntryIdx < RequestedChangesCount; InputEntryIdx++) {
    //
    // If index is not specified, flag and defer index allocation
    //
    if (RequestedConfigurationEntries[InputEntryIdx].Index == EFI_BLOCK_IO_CRYPTO_INDEX_ANY) {
      *DeferredEntries = TRUE;
      continue;
    }

    //
    // Skip entries with NilGuid
    //
    if (CompareGuid (&(RequestedConfigurationEntries[InputEntryIdx].KeyOwnerGuid), &NilGuid)) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "InputEntryIdx: %d\n", InputEntryIdx));

    //
    // Locate free index in the table to store new entry
    //
    Status = GetAvailableEntryIndex (&FreeTableEntryIndex, ConfigurationMaxEntries, ModifiedConfigurationTable);
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Allocate request with specified index
    //
    ModifiedConfigurationTable[FreeTableEntryIndex].Index = RequestedConfigurationEntries[InputEntryIdx].Index ;
    CopyGuid (
      &(ModifiedConfigurationTable[FreeTableEntryIndex].KeyOwnerGuid),
      &(RequestedConfigurationEntries[InputEntryIdx].KeyOwnerGuid));

    CopyMem (
      &(ModifiedConfigurationTable[FreeTableEntryIndex].Capability),
      &(RequestedConfigurationEntries[InputEntryIdx].Capability),
      sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));

    ModifiedConfigurationTable[FreeTableEntryIndex].CryptoKey = RequestedConfigurationEntries[InputEntryIdx].CryptoKey;

    DEBUG ((DEBUG_INFO, "Added entry with index (%d)!\n", ModifiedConfigurationTable[FreeTableEntryIndex].Index));

    //
    // Add entry to resulting table
    //
    if (NewEntries != NULL) {
      NewEntries[ResultEntryIdx].Index = ModifiedConfigurationTable[FreeTableEntryIndex].Index;
      CopyGuid (
        &(NewEntries[ResultEntryIdx].KeyOwnerGuid),
        &(ModifiedConfigurationTable[FreeTableEntryIndex].KeyOwnerGuid));

      CopyMem (
        &(NewEntries[ResultEntryIdx].Capability),
        &(ModifiedConfigurationTable[FreeTableEntryIndex].Capability),
        sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));
      *NewEntriesCount = ++ResultEntryIdx;
    }
    *ModifiedConfigurationCount = *ModifiedConfigurationCount + 1;
  }

  return EFI_SUCCESS;
}

/**
   Add Non-indexed entries from CryptoConfigurationTable instance.

   @param[in]       RequestedChangesCount       Number of entries being configured.
   @param[in]       RequestedConfiguration      Pointer to a table with the requested changes.
   @param[in][out]  ModifiedConfigurationCount  Pointer to number of entries in table being configured.
   @param[in][out]  ModifiedConfigurationTable  Pointer to a table being configured
   @param[in][out]  NewEntriesCount             Pointer to number of new entries registered.
   @param[in][out]  NewEntries                  Pointer to a table containing the new entries successfully registered.

   @retval EFI_SUCCESS             if modifications occurred without exceptions
   @retval EFI_INVALID_PARAMETER   if RequestedConfigurationEntries is NULL.
   @retval EFI_INVALID_PARAMETER   if RequestedChangesCount is 0.
   @retval EFI_UNSUPPORTED         if any entry is unsupported by the engine, abort operation.
   @retval EFI_OUT_OF_RESOURCES    if there are not enough available configuration entries to submit request.
**/
EFI_STATUS
EFIAPI
ProcessNonIndexedUefiTableEntries (
  IN     UINT64                                           RequestedChangesCount,
  IN     EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *RequestedConfigurationEntries,
  IN OUT UINT64                                           *ModifiedConfigurationCount,
  IN OUT EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ModifiedConfigurationTable,
  IN OUT UINT64                                           *NewEntriesCount,
  IN OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *NewEntries
  )
{
  EFI_STATUS Status;
  UINT64     InputEntryIdx;
  UINT64     ResultEntryIdx;
  UINT64     TableEntryIdx;
  UINT64     FreeTableEntryIndex;

  UINT64     CandidateIndexValue;
  BOOLEAN    CandidateIndexValueFound;

  UINT64     ConfigurationMaxEntries;
  EFI_GUID   NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  ConfigurationMaxEntries = mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount;
  ResultEntryIdx = *NewEntriesCount;

  for (InputEntryIdx = 0; InputEntryIdx < RequestedChangesCount; InputEntryIdx++) {
    //
    // Skip entries with indexes
    //
    if (RequestedConfigurationEntries[InputEntryIdx].Index != EFI_BLOCK_IO_CRYPTO_INDEX_ANY) {
      continue;
    }

    //
    // Skip entries with NilGuid
    //
    if (CompareGuid (&(RequestedConfigurationEntries[InputEntryIdx].KeyOwnerGuid), &NilGuid)) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "InputEntryIdx: %d\n", InputEntryIdx));
    //
    // Search for a valid available index value.
    //
    CandidateIndexValue = 0;
    do {
      CandidateIndexValueFound = TRUE;
      //
      // Check if candidate has been previously allocated
      //
      for (TableEntryIdx = 0; TableEntryIdx < ConfigurationMaxEntries; TableEntryIdx++) {
        if (ModifiedConfigurationTable[TableEntryIdx].Index == CandidateIndexValue) {
          // If conflict is found, move to next candidate index
          CandidateIndexValueFound = FALSE;
          CandidateIndexValue++;
          break;
        }
      }
    } while ((CandidateIndexValueFound == FALSE) && (CandidateIndexValue < MAX_UINT64));

    if (CandidateIndexValueFound == FALSE) {
      DEBUG ((DEBUG_ERROR, "CandidateIndex not found!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Locate free entry in the table to store new entry
    //
    Status = GetAvailableEntryIndex (&FreeTableEntryIndex, ConfigurationMaxEntries, ModifiedConfigurationTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Can't locate enough free entries in UEFI configuration table!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Allocate request with candidate index
    //
    ModifiedConfigurationTable[FreeTableEntryIndex].Index = CandidateIndexValue;

    CopyGuid (
              &(ModifiedConfigurationTable[FreeTableEntryIndex].KeyOwnerGuid),
              &(RequestedConfigurationEntries[InputEntryIdx].KeyOwnerGuid));

    CopyMem (
             &(ModifiedConfigurationTable[FreeTableEntryIndex].Capability),
             &(RequestedConfigurationEntries[InputEntryIdx].Capability),
             sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));

    ModifiedConfigurationTable[FreeTableEntryIndex].CryptoKey = RequestedConfigurationEntries[InputEntryIdx].CryptoKey;

    DEBUG ((DEBUG_INFO, "Added entry with index (%d)!\n", ModifiedConfigurationTable[FreeTableEntryIndex].Index));

    //
    // Add entry to resulting table
    //
    if (NewEntries != NULL) {
      NewEntries[ResultEntryIdx].Index = ModifiedConfigurationTable[FreeTableEntryIndex].Index;
      CopyGuid (
        &(NewEntries[ResultEntryIdx].KeyOwnerGuid),
        &(ModifiedConfigurationTable[FreeTableEntryIndex].KeyOwnerGuid));

      CopyMem (
        &(NewEntries[ResultEntryIdx].Capability),
        &(ModifiedConfigurationTable[FreeTableEntryIndex].Capability),
        sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));

      *NewEntriesCount = ++ResultEntryIdx;
    }
    *ModifiedConfigurationCount = *ModifiedConfigurationCount + 1;
  }

  return EFI_SUCCESS;
}

/**
   Configures the provided entries into the UEFI CryptoConfigurationTable.

   This call will modify the UEFI Crypto Configuration Table in an atomic manner.
   Only when all the requested modifications can be applied successfully will the
   changes to configuration be submitted.

   If any of the requested modifications fail, the configuration is left unmodified
   and the function returns with an error status.

   @param[in]  RequestedChangesCount  Number of entries being configured with this call.
   @param[in]  RequestedConfiguration Entries  Pointer to a table used to populate the configuration table.
   @param[out] ResultingTable         Optional pointer to a table that receives the newly configured
   entries.

   @retval EFI_SUCCESS             if all requested modifications were successfully submitted.
   @retval EFI_NOT_READY           if TSE driver context is NULL
   @retval EFI_INVALID_PARAMETER   if RequestedConfigurationEntries is NULL.
   @retval EFI_INVALID_PARAMETER   if RequestedChangesCount is 0.
   @retval EFI_UNSUPPORTED         if any entry is unsupported by the engine.
   @retval EFI_OUT_OF_RESOURCES    if there are not enough available configuration entries to submit request.
**/
EFI_STATUS
EFIAPI
SubmitUefiTableConfigurationEntries (
  IN  UINT64                                           RequestedChangesCount,
  IN  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *RequestedConfigurationEntries,
  OUT EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ResultingTable OPTIONAL
  )
{
  UINT64                                           ModifiedConfigurationCount;
  EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY    *ModifiedConfigurationTable = NULL;

  UINT64                                           NewEntriesCount;
  EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *NewEntries = NULL;

  BOOLEAN    DeferredEntries;
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (mTseDriverContextPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Driver context is NULL!\n"));
    return EFI_NOT_READY;
  }

  //
  // Allocate temporal configuration table
  //
  ModifiedConfigurationTable = AllocateZeroPool (sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);

  if (ModifiedConfigurationTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate temporal response table
  //
  NewEntriesCount = 0;
  NewEntries = AllocateZeroPool (sizeof (EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);

  if (NewEntries == NULL) {
    FreePool (ModifiedConfigurationTable);
    return EFI_OUT_OF_RESOURCES;
  }

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));

  //
  // Copy current table contents into temporal table
  //
  CopyMem (
    ModifiedConfigurationTable,
    mTseDriverContextPtr->CryptoConfigurationTable,
    sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);

  ModifiedConfigurationCount = mTseDriverContextPtr->CryptoConfigurationTableEntryCount;

  //
  // Attempt to perform requested configuration change atomically.
  //
  // Failure to configure any of the entries will result in aborting
  // the configuration change.
  //
  do {
    //
    // Step 1: Process all entries to be released.
    //
    Status = ProcessReleasedUefiTableEntries (
               RequestedChangesCount,
               RequestedConfigurationEntries,
               &ModifiedConfigurationCount,
               ModifiedConfigurationTable);

    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Step 2: Process all new entries with a defined index.
    //
    DeferredEntries = FALSE;
    Status = ProcessIndexedUefiTableEntries (
               RequestedChangesCount,
               RequestedConfigurationEntries,
               &ModifiedConfigurationCount,
               ModifiedConfigurationTable,
               &NewEntriesCount,
               NewEntries,
               &DeferredEntries);

    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Step 3: Process all new entries without a defined index.
    //
    if (DeferredEntries == TRUE) {
      Status = ProcessNonIndexedUefiTableEntries (
                 RequestedChangesCount,
                 RequestedConfigurationEntries,
                 &ModifiedConfigurationCount,
                 ModifiedConfigurationTable,
                 &NewEntriesCount,
                 NewEntries);
    }
    break;
  } while (TRUE);

  if (!EFI_ERROR (Status)) {
    //
    // Replace configuration with modified table.
    //
    mTseDriverContextPtr->CryptoConfigurationTableEntryCount = ModifiedConfigurationCount;

    CopyMem (
      mTseDriverContextPtr->CryptoConfigurationTable,
      ModifiedConfigurationTable,
      sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);

    DEBUG ((DEBUG_INFO, "Final Used Configuration Entries Count: %d\n", mTseDriverContextPtr->CryptoConfigurationTableEntryCount));

    //
    // Replace resulting table response
    //
    CopyMem (
      ResultingTable,
      NewEntries,
      sizeof (EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY) * NewEntriesCount);

    DEBUG ((DEBUG_INFO, "Modified Entries Count: %d\n", NewEntriesCount));
  } else {
    DEBUG ((DEBUG_ERROR, "Error processing requested changes. UEFI Crypto Configuration table was not modified.\n"));
  }

  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));

  FreePool (ModifiedConfigurationTable);
  FreePool (NewEntries);

  return Status;
}

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

  @retval EFI_SUCCESS             If UEFI table configuration was successfully returned
  @retval EFI_NOT_READY           if TSE driver context is NULL
  @retval EFI_INVALID_PARAMETER   Configuration table is NULL.
  @retval EFI_INVALID_PARAMETER   StartIndex is out of bounds.

**/
EFI_STATUS
EFIAPI
GetUefiTableConfigurationEntries (
  IN     UINT64                                              StartIndex,
  IN     UINT64                                              ConfigurationCount,
  IN     EFI_GUID                                            *KeyOwnerGuid OPTIONAL,
  IN OUT    EFI_BLOCK_IO_CRYPTO_RESPONSE_CONFIGURATION_ENTRY *ConfigurationTable
  )
{
  UINT64     TableEntryIdx;
  UINT64     CopiedEntries;
  UINT64     ConfigurationMaxEntries;
  EFI_GUID   NilGuid = NIL_GUID;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (mTseDriverContextPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Driver context is NULL!\n"));
    return EFI_NOT_READY;
  }

  CopiedEntries = 0;
  TableEntryIdx = 0;
  ConfigurationMaxEntries = mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount;

  if ((StartIndex >= ConfigurationMaxEntries) ||
      (ConfigurationCount == 0) ||
      (ConfigurationTable == NULL)) {
    DEBUG ((DEBUG_ERROR, "Invalid parameters! StartIndex: %d, ConfigurationCount: %d, ConfigurationTable: 0x%08x\n", StartIndex, ConfigurationCount, ConfigurationTable));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Scan current CryptoConfigurationTable for populated entries.
  // Starts scan at StartIndex and iterates on all current allocated entries (ConfigurationMaxEntries) or
  // until CopiedEntriesConfigurationCount number of elements have been copied to returning response table.
  //

  EfiAcquireLock (&(mTseDriverContextPtr->TseDriverContextLock));

  //
  // Copy current configuration table entries into response table entries
  //
  for (TableEntryIdx = 0;
       (TableEntryIdx < ConfigurationMaxEntries) && (CopiedEntries < ConfigurationCount) && (TableEntryIdx < ConfigurationMaxEntries);
       TableEntryIdx++) {
    //
    // Filter by KeyOwnerGuid when parameter is not NULL.
    //
    if (!CompareGuid (KeyOwnerGuid, &NilGuid) &&
        !CompareGuid (KeyOwnerGuid, &(mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].KeyOwnerGuid))){
      continue;
    }

    ConfigurationTable[TableEntryIdx].Index = mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].Index;

    CopyGuid (
      (&ConfigurationTable[TableEntryIdx].KeyOwnerGuid),
      &(mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].KeyOwnerGuid));

    CopyMem (
      &(ConfigurationTable[TableEntryIdx].Capability),
      &(mTseDriverContextPtr->CryptoConfigurationTable[TableEntryIdx].Capability),
      sizeof (EFI_BLOCK_IO_CRYPTO_CAPABILITY));

    CopiedEntries++;
  }

  EfiReleaseLock (&(mTseDriverContextPtr->TseDriverContextLock));

  return EFI_SUCCESS;
}

/**
  <b>TSE DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The TSE DXE driver provides a standard way for NVMe drivers to access the BlockIoCrypto interface.

  - @pre
    - TSE enabled
    - IOC and IOCCE IPs initialized
    - This TSE driver installs the TSE Protocol, so it needs to execute before any consumer of the protocol.

  - @result
    The TSE DXE module produces @link Include/Protocol/Tse.h TSE_PROTOCOL @endlink.

  @param[in] ImageHandle          Image handle for this driver image
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             TSE DXE driver initialization completed successfully.
  @retval EFI_UNSUPPORTED         if TSE is disabled or not supported.
  @retval EFI_NOT_FOUND           if TSE data HOB was not found.
  @retval EFI_NOT_STARTED         if an error prevented the TSE protocol from being installed.
  @retval others                  Error conditions
**/
EFI_STATUS
EFIAPI
TseDxeEntryPoint (
  IN  EFI_HANDLE       ImageHandle,
  IN  EFI_SYSTEM_TABLE *SystemTable
  )
{
  TSE_DATA_HOB       *TseDataHobPtr = NULL;
  EFI_STATUS         Status;
  UINT32             AlgMask;
  UINT8              CapCount;
  UINT8              CapIdx;

  EFI_EVENT           EndOfDxeEvent;
  EFI_EVENT           ExitBootServicesEvent;

  Status = EFI_SUCCESS;

  AlgMask = 0;
  CapCount = 0;
  CapIdx = 0;

  DEBUG ((DEBUG_INFO, "[TseProtocol] %a()\n", __FUNCTION__));

  if (IsTseSupported () == FALSE) {
    DEBUG ((DEBUG_WARN, "TSE is not supported. TSE protocol will not be installed.\n"));
    return EFI_UNSUPPORTED;
  }

  TseDataHobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);
  if (TseDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "[TseProtocol]: ERROR: TseDataHob not found!\n"));
    ASSERT (TseDataHobPtr != NULL);
    return EFI_NOT_FOUND;
  }

  if (TseDataHobPtr->TseEnabled == FALSE) {
    DEBUG ((DEBUG_ERROR, "[TseProtocol]: TSE is disabled. Skipping TSE protocol initialization...\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Register callback to lock BDF API
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  LockBdfsEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register a End of DXE Service for locking the SetBDF API, Status: %r\n", Status));
    return EFI_NOT_STARTED;
  }

  //
  // Register callback to clear UEFI configuration table data,
  // clear up the TSE DRAM line entries and invalidate TSE cache
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  TseExitBootServicesCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServicesEvent
                  );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register a Exit Boot Service for clearing TSE DRAM Table, Status: %r\n", Status));
    return EFI_NOT_STARTED;
  }

  mTseDriverContextPtr = &mTseDriverContext;
  EfiInitializeLock (&(mTseDriverContextPtr->TseDriverContextLock), TPL_NOTIFY);

  //
  // Populate Engine capabilities information:

  //
  // Check engine's algorithm support and capabilities
  //
  AlgMask = (UINT32) GetTseSupportedAlgorithms ();
  //
  // Calculate capability count from algorithm mask bit count
  //
  CapCount = BitFieldCountOnes32 (AlgMask, 0, 15);

  //
  // Single TSE engine support:
  //
  mTseDriverContextPtr->EngineInfo.TseEngineId     = 0;
  mTseDriverContextPtr->EngineInfo.AlgCount        = CapCount;
  mTseDriverContextPtr->EngineInfo.IoAlign         = TSE_IO_16_BYTE_ALIGNMENT;

  mTseDriverContextPtr->EngineInfo.CryptoCap.Supported       = TRUE;
  mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount        = GetTseMaxKeys ();
  mTseDriverContextPtr->EngineInfo.CryptoCap.CapabilityCount = CapCount;

  mTseDriverContextPtr->EngineInfo.TseTableProperties.TableAddress = (TSE_LINE *) (UINTN) ((TseDataHobPtr->TseDramMemBase) << 20);      // HOB Value in MB;
  mTseDriverContextPtr->EngineInfo.TseTableProperties.CurrentIndex = TSE_TABLE_FIRST_IDX;

  //
  // Check AES-XTS 256 Support
  //
  if (IsAesXts256Supported () == TRUE) {
    DEBUG ((DEBUG_INFO, "CryptoCap[%d]: 0x%x\n", CapIdx, mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx]));
    mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].Algorithm = gTseBlockIoCryptoAlgoAesXtsGuid;
    mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].KeySize = TSE_ALG_KEY_SIZE_AES_XTS_256;
    mTseDriverContextPtr->EngineInfo.CryptoCap.Capabilities[CapIdx].CryptoBlockSizeBitMask = TSE_LBA_512B_SUPPORT | TSE_LBA_4KB_SUPPORT;
    CapIdx++;
  }

  //
  // Initialize TSE Protocol API function implementation
  //
  mTseDriverContextPtr->TseApi.SetBDF                               = SetBDF;
  mTseDriverContextPtr->TseApi.SetBlockIoMedia                      = SetBlockIoMedia;
  mTseDriverContextPtr->TseApi.SetCryptoConfig                      = SetCryptoConfig;
  mTseDriverContextPtr->TseApi.CryptoBlockIncrement                 = CryptoBlockIncrement;
  mTseDriverContextPtr->TseApi.SubmitTseTableEntries                = SubmitTseTableEntries;
  mTseDriverContextPtr->TseApi.GetCapabilities                      = GetCapabilities;
  mTseDriverContextPtr->TseApi.SubmitUefiTableConfigurationEntries  = SubmitUefiTableConfigurationEntries;
  mTseDriverContextPtr->TseApi.GetUefiTableConfigurationEntries     = GetUefiTableConfigurationEntries;

  //
  // Populated EFI_BLOCK_IO_CRYPTO_CAPABILITIES need to have been allocated together with
  // the context creation.
  //
  DEBUG ((DEBUG_INFO, "AlgCount: %d, CapIdx: %d\n", CapCount, CapIdx));
  ASSERT (CapIdx <= CapCount);

  //
  // Allocate Block IO Crypto configuration table with as many entries as key's supported by the TSE engine.
  //
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount, (VOID **) &(mTseDriverContextPtr->CryptoConfigurationTable));
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate UEFI Block IO Crypto configuration table, Status: %r\n", Status));
    return Status;
  }

  ZeroMem (mTseDriverContextPtr->CryptoConfigurationTable, sizeof (EFI_BLOCK_IO_CRYPTO_CONFIGURATION_TABLE_ENTRY) * mTseDriverContextPtr->EngineInfo.CryptoCap.KeyCount);

  //
  // Initialize number of allocated configuration entries.
  //
  mTseDriverContextPtr->CryptoConfigurationTableEntryCount = 0;

  //
  // Initialize TSE DRAM table entries.
  //
  ClearTseDramTable ();

  //
  //  Install the driver protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mImageHandle,
                  &gTseProtocolGuid,
                  &mTseDriverContextPtr->TseApi,
                  NULL
                  );

  return EFI_SUCCESS;
}
