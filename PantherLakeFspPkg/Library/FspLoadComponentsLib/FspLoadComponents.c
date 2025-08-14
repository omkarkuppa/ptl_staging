/** @file
  Fsp loading FSP-M and BspPreMem

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/NemConfigLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/FspFbmSupportLib.h>
#include <Library/FspVerificationLib.h>
#include <Library/BootGuardLib.h>
#include <Library/FspMeasurementLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FspLoadComponentsLib.h>

volatile FSP_VERIFY_API_WRAPPER  mFspVerifyApiWrapper = {
  FSP_VERIFY_API_WRAPPER_STRUCTURE_ID,
  NULL,  // To be patched post FSP build
  NULL,  // To be patched post FSP build
  NULL,  // To be patched post FSP build
  NULL   // To be patched post FSP build
};

/**
  Get BSP Segment Base Address and Size infomation from BSPM_ELEMENT

  @param[in]       Bspm               BPM/Bspm structure for FSP Element
  @param[in]       Index              Index of BSP Segment
  @param[in, out]  BspBaseAddress     BSP Base Address value
  @param[in, out]  BspSize            BSP Size value

**/
VOID
GetBspInfo (
  IN      BSPM_ELEMENT    *Bspm,
  IN      UINT32          Index,
  IN OUT  UINT32          *BspBaseAddress,
  IN OUT  UINT32          *BspSize
  )
{
  REGION_SEGMENT    *BspSegment;

  ASSERT (Bspm != NULL);

  BspSegment = ((REGION_SEGMENT *) (Bspm + 1)) + Index;

  *BspBaseAddress = BspSegment->Base;
  *BspSize        = BspSegment->Size;
}

/**
  Load FSP-M onto Write Back NEM and verify FSP-M via FBM.

  @param[in]       Bspm               BPM/Bspm structure for FSP Element
  @param[in]       Fbm                FSP Boot Manifest structure
  @param[in, out]  FlashRegionsOnNem  The flash regions already loaded to NEM. This region list will be updated
                                      if FSPM is successfully loaded.

  @retval EFI_INVALID_PARAMETER       One or more parameters are invalid.
  @retval EFI_DEVICE_ERROR            System error in verifying FSPM.
  @retval EFI_SUCCESS                 Load and verify FSP-M successful.
  @retval Others                      Unexpected error in FSP-M Load and verify.

**/
EFI_STATUS
LoadFspm (
  IN      BSPM_ELEMENT                   *Bspm,
  IN      FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN OUT  FLASH_REGION_LIST              *FlashRegionsOnNem,
  IN      UINT32                         DataStackBase,
  IN      UINT32                         DataStackSize
  )
{
  EFI_STATUS            Status;
  UINT8                 AvailableMemoryBuffer[HASH_CTX_LEN_MAX];
  FLASH_REGION_SEGMENT  FlashRegionFspm;

  if (Bspm == NULL || FlashRegionsOnNem == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FlashRegionFspm.FlashRegionBase     = Bspm->FspmBaseAddress;
  FlashRegionFspm.FlashRegionSize     = FixedPcdGet32 (PcdFlashFvFspmSize);
  FlashRegionFspm.TargetNemBufferType = CacheWriteBack;
  DEBUG ((DEBUG_INFO, "FSP loads new region Base: %x Size: %x\n", Bspm->FspmBaseAddress, FixedPcdGet32 (PcdFlashFvFspmSize)));

  //
  // Allocate NEM for FSPM.
  // When NEM has no enough size or MTRRs has no enough pairs, system should halt directly.
  //
  Status = AllocateNemForFlashRegion (
             FlashRegionsOnNem,
             &FlashRegionFspm,
             DataStackBase,
             DataStackSize
             );
  if (Status == EFI_OUT_OF_RESOURCES) {
    CpuDeadLoop ();
  }

  if (mFspVerifyApiWrapper.VerifyFspmApiWrapper == NULL) {
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "FSP verifies FSPM region ...\n"));
  Status = mFspVerifyApiWrapper.VerifyFspmApiWrapper (Bspm, Fbm, AvailableMemoryBuffer);
  if (EFI_ERROR (Status)) {
    VerifyCrtmStatusAndDisableTxtCmos ();
    return EFI_UNSUPPORTED;
  }

  if (!(Bspm->FspmLoadingPolicy & FSPM_COMPRESSED)) {
    //
    // Pei Core rebase is not needed for FSP-M compression as we
    // are not using FSP Pei Core when FSP-M is compressed.
    //
    RebasePeiCoreFfs ((UINTN) Bspm->FspmBaseAddress);
  }

  return EFI_SUCCESS;
}

/**
  Load BSP-PreMem onto Write Protected NEM and verify BSP-PreMem.

  @param[in]       Bspm               BPM/BSPM structure for FSP Element
  @param[in, out]  FlashRegionsOnNem  The flash regions already loaded to NEM. This region list will be updated
                                      if BspPreMem is successfully loaded.

  @retval EFI_INVALID_PARAMETER       One or more parameters are invalid.
  @retval EFI_DEVICE_ERROR            System error in verifying BSP.
  @retval EFI_SUCCESS                 Load and verify BSP-PreMem successful.
  @retval Others                      Unexpected error in BSP-PreMem Load and verify.

**/
EFI_STATUS
LoadBspPreMem (
  IN      BSPM_ELEMENT           *Bspm,
  IN OUT  FLASH_REGION_LIST      *FlashRegionsOnNem,
  IN      UINT32                 DataStackBase,
  IN      UINT32                 DataStackSize
  )
{
  EFI_STATUS            Status;
  UINT8                 AvailableMemoryBuffer[HASH_CTX_LEN_MAX];
  UINT32                BspBaseAddress;
  UINT32                BspSize;
  FLASH_REGION_SEGMENT  FlashRegionBspPreMem;
  UINT32                Index;

  if (Bspm == NULL || FlashRegionsOnNem == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < Bspm->BspSegmentCount; Index ++) {

    GetBspInfo (Bspm, Index, &BspBaseAddress, &BspSize);

    FlashRegionBspPreMem.FlashRegionBase     = BspBaseAddress;
    FlashRegionBspPreMem.FlashRegionSize     = BspSize;
    FlashRegionBspPreMem.TargetNemBufferType = CacheWriteProtected;

    DEBUG ((DEBUG_INFO, "FSP loads new region Base: %x Size: %x\n", BspBaseAddress, BspSize));

    //
    // Allocate NEM forBspPreMem.
    // When NEM has no enough size or MTRRs has no enough pairs, system should halt directly.
    //
    Status = AllocateNemForFlashRegion (
               FlashRegionsOnNem,
               &FlashRegionBspPreMem,
               DataStackBase,
               DataStackSize
               );
    if (Status == EFI_OUT_OF_RESOURCES) {
      CpuDeadLoop ();
    }
  }

  if (mFspVerifyApiWrapper.VerifyBspApiWrapper == NULL) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "FSP verifies Bsp region ...\n"));
  Status = mFspVerifyApiWrapper.VerifyBspApiWrapper (Bspm, AvailableMemoryBuffer);
  if (EFI_ERROR (Status)) {
    VerifyCrtmStatusAndDisableTxtCmos ();
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Align DataStack base and size to WayCacheSize.

  @param[in]   DataStackTop   The top address of data stack.
  @param[out]  DataStackBase  Aligned base address of data stack.
  @param[out]  DataStackSize  Aligned size of data stack.
**/
VOID
DataStackAlign (
  IN     UINT32    DataStackTop,
  OUT    UINT32    *DataStackBase,
  OUT    UINT32    *DataStackSize
  )
{
  UINT32    EffCacheSize;
  UINT32    EffWayCacheSize;

  EffCacheSize    = 0;
  EffWayCacheSize = 0;
  GetEffCacheSize (&EffCacheSize, &EffWayCacheSize);

  if (EffWayCacheSize == 0) {
    DEBUG ((DEBUG_ERROR, "Invalid Way Cache Size\n"));
    return;
  }

  *DataStackBase = FixedPcdGet32 (PcdTemporaryRamBase);
  if (*DataStackBase % EffWayCacheSize != 0) {
    *DataStackBase = *DataStackBase - *DataStackBase % EffWayCacheSize;
  }

  if (DataStackTop % EffWayCacheSize != 0) {
    *DataStackSize = (DataStackTop - DataStackTop % EffWayCacheSize + EffWayCacheSize) - *DataStackBase;
  } else {
    *DataStackSize = DataStackTop - *DataStackBase;
  }
  DEBUG ((DEBUG_INFO, "Aligned DataStackBase: %x DataStackSize: %x\n", *DataStackBase, *DataStackSize));
}

/**
  Initialize flash region list already be loaded to NEM. FSP-O and FSP-T regions will be
  added to this list, since they must already be loaded by ACM or previous Bios code.

  @param[in, out]  FlashRegionList  Flash region list to be initialized.
**/
VOID
InitializeFlashRegionList (
  IN OUT    FLASH_REGION_LIST    *FlashRegionList
  )
{
  UINT32    FspoSize;
  UINT32    FsptSize;
  UINT32    TopIbbSize;

  FspoSize      = FixedPcdGet32 (PcdFlashFvFspoSize);
  FsptSize      = FixedPcdGet32 (PcdFlashFvFsptSize);
  TopIbbSize    = FspoSize + FsptSize;
  FlashRegionList->Count = 1;
  FlashRegionList->FlashRegionList[0].FlashRegionBase     = (UINT32) (BASE_4GB - TopIbbSize);
  FlashRegionList->FlashRegionList[0].FlashRegionSize     = TopIbbSize;
  FlashRegionList->FlashRegionList[0].TargetNemBufferType = CacheWriteProtected;
}

/**
  Fsp Load Components. When FSP Signing is supported, FBM should be verified as valid
  by ACM only in ACM verified boot flow, Bios should perform below tasks:
  1. Load and verify FSP-M as optional, and halt the system if any error occurs.
  2. Load and verify BSP-PreMem, and halt the system if any error occurs.

**/
VOID
EFIAPI
FspLoadComponents (
  IN UINTN TopOfCar
  )
{
  EFI_STATUS                     Status;
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  BSPM_ELEMENT                   *Bspm;
  UINT32                         DataStackBase;
  UINT32                         DataStackSize;
  FLASH_REGION_LIST              FlashRegionList;
  FSP_BUILD_MEASUREMENT_INFO     FspMeasurementInfo;
  UINT32                         TpmActivePcrBanks;

  Bspm = LocateBspm ();
  if (Bspm == NULL) {
    return;
  }

  Fbm = LocateFbm ();

  //
  // Check if signing is supported
  //
  if (!(IsSigningSupported (Fbm))) {
    //
    // Rebase PeiCore inside FSP-M FV for signed FSP
    //
    if (!(Bspm->FspmLoadingPolicy & FSPM_COMPRESSED)) {
      RebasePeiCoreFfs ((UINTN) Bspm->FspmBaseAddress);
    }
    return;
  }

  FspMeasurementInfo.Data = FSP_MEASUREMENT_INFO_DEFAULT;
  InitializeTpmAndGetActivePcrs (&FspMeasurementInfo, Bspm, &TpmActivePcrBanks);

  if ((FspMeasurementInfo.Bits.IbbStatus != EFI_SUCCESS) &&
      (FspMeasurementInfo.Bits.FspVersionStatus == EFI_SUCCESS)) {
    //
    // Measure FSP version and FSP-OT for BTG4 when it was not measured by ACM
    //
    ExtendFspVersion (&FspMeasurementInfo, Fbm, TpmActivePcrBanks);
    ExtendFspotRegion (&FspMeasurementInfo, Fbm, TpmActivePcrBanks);
    FspExtendIbbRegionNear4G (TpmActivePcrBanks);
  }

  //
  // Get aligned data stack base and size
  //
  DataStackAlign ((UINT32) TopOfCar, &DataStackBase, &DataStackSize);

  //
  // Before loading FSP-M or BspPreMem to NEM, initialize the flash region list
  //
  InitializeFlashRegionList (&FlashRegionList);

  //
  // Load FSP-M
  //
  if ((Bspm->FspmLoadingPolicy & BIT0) == LOADING_FSPM) {
    Status = LoadFspm (Bspm, Fbm, &FlashRegionList, DataStackBase, DataStackSize);
    if (Status == EFI_SUCCESS) {
      ExtendFspmRegion (&FspMeasurementInfo, Fbm, TpmActivePcrBanks);
    }
  }

  //
  // Load BspPreMem regions
  //
  if (Bspm->BspSegmentCount != 0) {
    Status = LoadBspPreMem (Bspm, &FlashRegionList, DataStackBase, DataStackSize);
    if (Status == EFI_SUCCESS) {
      ExtendBspRegion (&FspMeasurementInfo, Bspm, TpmActivePcrBanks);
    }
  }

  FspBuildMeasurementInfo (FspMeasurementInfo);
  return;
}
