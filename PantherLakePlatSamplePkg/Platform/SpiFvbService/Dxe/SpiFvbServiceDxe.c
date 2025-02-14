/** @file
  Common driver source for several Serial Flash devices
  which are compliant with the Intel(R) Serial Flash Interface Compatibility Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include "SpiFvbServiceCommon.h"
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/CpuPlatformLib.h>
#include <BootStateLib.h>

EFI_EVENT  mSpiFvbServiceAddressChangeEvent = NULL;

FV_INFO mPlatformFullBiosFvBaseAddress[] = {
  {FixedPcdGet32(PcdFlashNvStorageVariableBase), FixedPcdGet32(PcdFlashNvStorageVariableSize)},
  {FixedPcdGet32(PcdFlashFvMicrocodeBase), FixedPcdGet32(PcdFlashFvMicrocodeSize)},
  {FixedPcdGet32(PcdFlashFvAdvancedBase), FixedPcdGet32(PcdFlashFvAdvancedSize)},
  {FixedPcdGet32(PcdFlashFvSecurityBase), FixedPcdGet32(PcdFlashFvSecuritySize)},
  {FixedPcdGet32(PcdFlashFvOsBootBase), FixedPcdGet32(PcdFlashFvOsBootSize)},
  {FixedPcdGet32(PcdFlashFvUefiBootBase), FixedPcdGet32(PcdFlashFvUefiBootSize)},
  {FixedPcdGet32(PcdFlashFvPostMemoryBase), FixedPcdGet32(PcdFlashFvPostMemorySize)},
  {FixedPcdGet32(PcdFlashFvPreMemoryBase), FixedPcdGet32(PcdFlashFvPreMemorySize)}
#if FixedPcdGetBool (PcdExtendedBiosRegionSupport) == 1
  ,{FixedPcdGet32(PcdFlashFvExtendedPostMemoryBase), FixedPcdGet32(PcdFlashFvExtendedPostMemorySize)},
  {FixedPcdGet32(PcdFlashFvExtendedAdvancedBase), FixedPcdGet32(PcdFlashFvExtendedAdvancedSize)}
#endif
};

UINTN mPlatformFullBiosFvBaseAddressCount = ARRAY_SIZE (mPlatformFullBiosFvBaseAddress);

FV_INFO mPlatformUpdateInProgressFvBaseAddress[] = {
  {FixedPcdGet32(PcdFlashNvStorageVariableBase), FixedPcdGet32(PcdFlashNvStorageVariableSize)}
};

UINTN mPlatformUpdateInProgressFvBaseAddressCount = ARRAY_SIZE (mPlatformUpdateInProgressFvBaseAddress);

/**
  Notification function of EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It convers pointer to new virtual address.

  @param[in]  Event        Event whose notification function is being invoked.
  @param[in]  Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
SpiFvbServiceAddressChangeEvent (
  IN EFI_EVENT             Event,
  IN VOID                  *Context
  )
{
  UINT32 Index;
  EFI_FVB_INSTANCE   *FvbInstance;

  FvbInstance = mFvbModuleGlobal.FvbInstance;

  for (Index = 0; Index < mFvbModuleGlobal.NumFv; Index++) {
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvBase);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->DevicePath);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.GetBlockSize);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.GetPhysicalAddress);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.GetAttributes);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.SetAttributes);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.Read);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.Write);
    EfiConvertPointer (0x0, (VOID **) &FvbInstance->FvbProtocol.EraseBlocks);
    FvbInstance = (EFI_FVB_INSTANCE *) ((UINTN) ((UINT8 *) FvbInstance) +
                  FvbInstance->FvHeader.HeaderLength +
                  (sizeof (EFI_FVB_INSTANCE) - sizeof (EFI_FIRMWARE_VOLUME_HEADER)));
  }

  EfiConvertPointer (0x0, (VOID **) &(mFvbModuleGlobal.FvbInstance));
}

/**
  The function installs EFI_FIRMWARE_VOLUME_BLOCK protocol
  for each FV in the system.

  @param[in]  FvbInstance   The pointer to a FW volume instance structure,
                            which contains the information about one FV.

  @retval     VOID

**/
VOID
InstallFvbProtocol (
  IN  EFI_FVB_INSTANCE               *FvbInstance
  )
{
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  EFI_STATUS                            Status;
  EFI_HANDLE                            FvbHandle;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL    *OldFvbInterface;

  ASSERT (FvbInstance != NULL);
  if (FvbInstance == NULL) {
    return;
  }

  CopyMem (&FvbInstance->FvbProtocol, &mFvbProtocolTemplate, sizeof (EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL));
  FvHeader = &FvbInstance->FvHeader;
  if (FvHeader == NULL) {
    return;
  }

  //
  // Set up the devicepath
  //
  DEBUG ((DEBUG_INFO, "FwBlockService.c: Setting up DevicePath for 0x%lx:\n", FvbInstance->FvBase));
  if (FvHeader->ExtHeaderOffset == 0) {
    //
    // FV does not contains extension header, then produce MEMMAP_DEVICE_PATH
    //
    FvbInstance->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_MEMMAP_DEVICE_PATH), &mFvMemmapDevicePathTemplate);
    if (FvbInstance->DevicePath == NULL) {
      DEBUG ((DEBUG_INFO, "SpiFvbServiceDxe.c: Memory allocation for MEMMAP_DEVICE_PATH failed\n"));
      return;
    }
    ((FV_MEMMAP_DEVICE_PATH *) FvbInstance->DevicePath)->MemMapDevPath.StartingAddress = FvbInstance->FvBase;
    ((FV_MEMMAP_DEVICE_PATH *) FvbInstance->DevicePath)->MemMapDevPath.EndingAddress   = FvbInstance->FvBase + FvHeader->FvLength - 1;
  } else {
    FvbInstance->DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AllocateRuntimeCopyPool (sizeof (FV_PIWG_DEVICE_PATH), &mFvPIWGDevicePathTemplate);
    if (FvbInstance->DevicePath == NULL) {
      DEBUG ((DEBUG_INFO, "SpiFvbServiceDxe.c: Memory allocation for FV_PIWG_DEVICE_PATH failed\n"));
      return;
    }
    CopyGuid (
      &((FV_PIWG_DEVICE_PATH *)FvbInstance->DevicePath)->FvDevPath.FvName,
      (GUID *)(UINTN)(FvbInstance->FvBase + FvHeader->ExtHeaderOffset)
      );
  }

  //
  // Find a handle with a matching device path that has supports FW Block protocol
  //
  Status = gBS->LocateDevicePath (&gEfiFirmwareVolumeBlockProtocolGuid, &FvbInstance->DevicePath, &FvbHandle);
  if (EFI_ERROR (Status) ) {
    //
    // LocateDevicePath fails so install a new interface and device path
    //
    DEBUG ((DEBUG_INFO, "FwBlockService.c: LocateDevicePath failed, install new interface 0x%lx:\n", FvbInstance->FvBase));
    FvbHandle = NULL;
    Status =  gBS->InstallMultipleProtocolInterfaces (
                     &FvbHandle,
                     &gEfiFirmwareVolumeBlockProtocolGuid,
                     &FvbInstance->FvbProtocol,
                     &gEfiDevicePathProtocolGuid,
                     FvbInstance->DevicePath,
                     NULL
                     );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "FwBlockService.c: IMPI FirmwareVolBlockProt, DevPath 0x%lx: %r\n", FvbInstance->FvBase, Status));

  } else if (IsDevicePathEnd (FvbInstance->DevicePath)) {
    //
    // Device already exists, so reinstall the FVB protocol
    //
    DEBUG ((DEBUG_INFO, "FwBlockService.c: LocateDevicePath succeeded, reinstall interface 0x%lx:\n", FvbInstance->FvBase));
    Status = gBS->HandleProtocol (
                    FvbHandle,
                    &gEfiFirmwareVolumeBlockProtocolGuid,
                    (VOID **) &OldFvbInterface
                    );
    ASSERT_EFI_ERROR (Status);

    Status =  gBS->ReinstallProtocolInterface (
                     FvbHandle,
                     &gEfiFirmwareVolumeBlockProtocolGuid,
                     OldFvbInterface,
                     &FvbInstance->FvbProtocol
                     );
    ASSERT_EFI_ERROR (Status);

  } else {
    //
    // There was a FVB protocol on an End Device Path node
    //
    ASSERT (FALSE);
  }

}

/**

  The function does the necessary initialization work for
  Firmware Volume Block Driver.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       This funtion always return EFI_SUCCESS.
                                It will ASSERT on errors.

**/
EFI_STATUS
EFIAPI
FvbInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_FVB_INSTANCE                      *FvbInstance;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  EFI_FV_BLOCK_MAP_ENTRY                *PtrBlockMapEntry;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  UINTN                                 Idx;
  UINT32                                MaxLbaSize;
  FV_INFO                               *PlatformFvInfo;
  UINT32                                BytesWritten;
  UINTN                                 BytesErased;
  UINTN                                 PlatformFvInfoCount;

  if (!IsSimicsEnvironment ()) {
    if (IsBiosGuardEnabled ()) {
      PlatformFvInfo      = mPlatformBiosGuardEnabledFvBaseAddress;
      PlatformFvInfoCount = mPlatformBiosGuardEnabledFvBaseAddressCount;
    } else {
      PlatformFvInfo = mPlatformFvBaseAddress;
      PlatformFvInfoCount = mPlatformFvBaseAddressCount;
    }
  } else {
    //
    // Launch all FVs every boot under Simics to make PCR is consistent
    //
    PlatformFvInfo = mPlatformDefaultBaseAddress;
    PlatformFvInfoCount = mPlatformDefaultBaseAddressCount;
  }

  if ((GetBootModeHob() == BOOT_ON_FLASH_UPDATE) || (GetBootModeHob() == BOOT_IN_RECOVERY_MODE)) {
    PlatformFvInfo        = mPlatformFullBiosFvBaseAddress;
    PlatformFvInfoCount   = mPlatformFullBiosFvBaseAddressCount;
  }

  if ((GetBootModeHob() == BOOT_ON_FLASH_UPDATE) && (GetFirstGuidHob (&gSysFwUpdateProgressGuid) != NULL)) {
    PlatformFvInfo        = mPlatformUpdateInProgressFvBaseAddress;
    PlatformFvInfoCount   = mPlatformUpdateInProgressFvBaseAddressCount;
  }

  //
  // Check BootState variable, NULL means it's the first boot after reflashing
  //
  if (!IsBootStatePresent()) {
    DEBUG ((DEBUG_INFO, "This is the first boot \n"));
    if (IsBiosGuardEnabled ()) {
      PlatformFvInfo      = mPlatformBiosGuardEnabledDefaultBaseAddress;
      PlatformFvInfoCount = mPlatformBiosGuardEnabledDefaultBaseAddressCount;
    } else {
      PlatformFvInfo      = mPlatformDefaultBaseAddress;
      PlatformFvInfoCount = mPlatformDefaultBaseAddressCount;
    }
  }

  //
  // Make sure all FVB are valid and/or fix if possible
  //
  for (Idx = 0; Idx < PlatformFvInfoCount; Idx++) {
    BaseAddress = PlatformFvInfo[Idx].FvBase;
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

    if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
      BytesWritten = 0;
      BytesErased = 0;
      DEBUG ((DEBUG_ERROR, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
      Status = GetFvbInfo (BaseAddress, &FvHeader);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ERROR - Can't recovery FV header at 0x%x.  GetFvbInfo Status %r\n", BaseAddress, Status));
        continue;
      }
      DEBUG ((DEBUG_INFO, "Rewriting FV header at 0x%X with static data\n", BaseAddress));

      //
      // Spi erase
      //
      BytesErased = (UINTN) FvHeader->BlockMap->Length;
      Status = SpiFlashBlockErase ( (UINTN) BaseAddress, &BytesErased);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ERROR - SpiFlashBlockErase Error  %r\n", Status));
        continue;
      }
      if (BytesErased != FvHeader->BlockMap->Length) {
        DEBUG ((DEBUG_ERROR, "ERROR - BytesErased != FvHeader->BlockMap->Length\n"));
        DEBUG ((DEBUG_ERROR, " BytesErased = 0x%X\n Length = 0x%X\n", BytesErased, FvHeader->BlockMap->Length));
        continue;
      }
      BytesWritten = FvHeader->HeaderLength;
      Status = SpiFlashWrite (BaseAddress, &BytesWritten, (UINT8*) FvHeader);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ERROR - SpiFlashWrite Error  %r\n", Status));
        continue;
      }
      if (BytesWritten != FvHeader->HeaderLength) {
        DEBUG ((DEBUG_ERROR, "ERROR - BytesWritten != HeaderLength\n"));
        DEBUG ((DEBUG_ERROR, " BytesWritten = 0x%X\n HeaderLength = 0x%X\n", BytesWritten, FvHeader->HeaderLength));
        continue;
      }
      Status = SpiFlashLock ();
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ERROR - SpiFlashLock Error  %r\n", Status));
        continue;
      }
      DEBUG ((DEBUG_INFO, "FV Header @ 0x%X restored with static data\n", BaseAddress));
      //
      // Clear cache for this range.
      //
      WriteBackInvalidateDataCacheRange ( (VOID *) (UINTN) BaseAddress, FvHeader->BlockMap->Length);
    }
  }

  //
  // Calculate the total size for all firmware volume block instances
  //
  BufferSize = 0;
  for (Idx = 0; Idx < PlatformFvInfoCount; Idx++) {
    BaseAddress = PlatformFvInfo[Idx].FvBase;
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

    if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
      DEBUG ((DEBUG_ERROR, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
      continue;
    }

    BufferSize += (FvHeader->HeaderLength +
                  sizeof (EFI_FVB_INSTANCE) -
                  sizeof (EFI_FIRMWARE_VOLUME_HEADER)
                  );
  }

  mFvbModuleGlobal.FvbInstance =  (EFI_FVB_INSTANCE *) AllocateRuntimeZeroPool (BufferSize);
  if (mFvbModuleGlobal.FvbInstance == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  MaxLbaSize      = 0;
  FvbInstance     = mFvbModuleGlobal.FvbInstance;
  mFvbModuleGlobal.NumFv   = 0;

  for (Idx = 0; Idx < PlatformFvInfoCount; Idx++) {
    BaseAddress = PlatformFvInfo[Idx].FvBase;
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) BaseAddress;

    if (!IsFvHeaderValid (BaseAddress, FvHeader)) {
      DEBUG ((DEBUG_ERROR, "ERROR - The FV in 0x%x is invalid!\n", FvHeader));
      continue;
    }

    FvbInstance->Signature = FVB_INSTANCE_SIGNATURE;
    CopyMem (&(FvbInstance->FvHeader), FvHeader, FvHeader->HeaderLength);

    FvHeader = &(FvbInstance->FvHeader);
    FvbInstance->FvBase = (UINTN)BaseAddress;

    //
    // Process the block map for each FV
    //
    FvbInstance->NumOfBlocks   = 0;
    for (PtrBlockMapEntry = FvHeader->BlockMap;
         PtrBlockMapEntry->NumBlocks != 0;
         PtrBlockMapEntry++) {
      //
      // Get the maximum size of a block.
      //
      if (MaxLbaSize < PtrBlockMapEntry->Length) {
        MaxLbaSize  = PtrBlockMapEntry->Length;
      }
      FvbInstance->NumOfBlocks += PtrBlockMapEntry->NumBlocks;
    }

    //
    // Add a FVB Protocol Instance
    //
    InstallFvbProtocol (FvbInstance);
    mFvbModuleGlobal.NumFv++;

    //
    // Move on to the next FvbInstance
    //
    FvbInstance = (EFI_FVB_INSTANCE *) ((UINTN)((UINT8 *)FvbInstance) +
                                          FvHeader->HeaderLength +
                                          (sizeof (EFI_FVB_INSTANCE) - sizeof (EFI_FIRMWARE_VOLUME_HEADER)));

  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  SpiFvbServiceAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mSpiFvbServiceAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
