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
STATIC
VOID
GetBspInfo (
  IN      BSPM_ELEMENT    *Bspm,
  IN      UINT32          Index,
  IN OUT  UINT32          *BspBaseAddress,
  IN OUT  UINT32          *BspSize
  )
{
  REGION_SEGMENT    *BspSegment;

  BspSegment = ((REGION_SEGMENT *) (Bspm + 1)) + Index;
  *BspBaseAddress = BspSegment->Base;
  *BspSize        = BspSegment->Size;
}

/**
  Load FSP-M and verify FSP-M via FBM.

  @param[in]       Bspm               BPM/Bspm structure for FSP Element
  @param[in]       Fbm                FSP Boot Manifest structure

  @retval EFI_INVALID_PARAMETER       One or more parameters are invalid.
  @retval EFI_DEVICE_ERROR            System error in verifying FSPM.
  @retval EFI_SUCCESS                 Load and verify FSP-M successful.
  @retval Others                      Unexpected error in FSP-M Load and verify.

**/
EFI_STATUS
LoadFspm (
  IN      BSPM_ELEMENT                   *Bspm,
  IN      FSP_BOOT_MANIFEST_STRUCTURE    *Fbm
  )
{
  EFI_STATUS            Status;
  UINT8                 AvailableMemoryBuffer[HASH_CTX_LEN_MAX];

  if (Bspm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mFspVerifyApiWrapper.VerifyFspmApiWrapper == NULL) {
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "FSP verifies FSPM\n"));
  Status = mFspVerifyApiWrapper.VerifyFspmApiWrapper (Bspm, Fbm, AvailableMemoryBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FSP-M verification fails...\n"));
    VerifyCrtmStatusAndDisableTxtCmos (Bspm);
    return EFI_UNSUPPORTED;
  } else {
    DEBUG ((DEBUG_INFO, "FSP-M verification pass...\n"));
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
  Load BSP-PreMem and verify BSP-PreMem.

  @param[in]       Bspm               BPM/BSPM structure for FSP Element

  @retval EFI_INVALID_PARAMETER       One or more parameters are invalid.
  @retval EFI_DEVICE_ERROR            System error in verifying BSP.
  @retval EFI_SUCCESS                 Load and verify BSP-PreMem successful.
  @retval Others                      Unexpected error in BSP-PreMem Load and verify.

**/
EFI_STATUS
LoadBspPreMem (
  IN      BSPM_ELEMENT           *Bspm
  )
{
  EFI_STATUS            Status;
  UINT8                 AvailableMemoryBuffer[HASH_CTX_LEN_MAX];

  if (Bspm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mFspVerifyApiWrapper.VerifyBspApiWrapper == NULL) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "FSP verifies BSP\n"));
  Status = mFspVerifyApiWrapper.VerifyBspApiWrapper (Bspm, AvailableMemoryBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "BSP verification fails...\n"));
    VerifyCrtmStatusAndDisableTxtCmos (Bspm);
    return EFI_UNSUPPORTED;
  } else  {
    DEBUG ((DEBUG_INFO, "BSP verification pass...\n"));
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
  FlashRegionList->FlashRegionList[0].TargetNemBufferType = CacheWriteBack;
}

/**
  Initialize NEM for FSP-M and BSP Pre-Mem.
  This function allocates NEM space for FSP-M before decompression and sets up
  the necessary NEM configuration for subsequent component loading.

  @param[in]    BsssBaseAddress    Base address of BSSS structure
  @param[in]    TopOfCar           Top of Cache-as-RAM address

  @retval  EFI_SUCCESS             NEM allocation successful.
  @retval  EFI_OUT_OF_RESOURCES    Insufficient NEM resources available.
  @retval  EFI_INVALID_PARAMETER   Invalid parameters provided.
  @retval  EFI_NOT_FOUND           Required structures not found.

**/
EFI_STATUS
EFIAPI
InitializeNemForPreMemComponents (
  IN UINTN BsssBaseAddress,
  IN UINTN TopOfCar
  )
{
  EFI_STATUS                     Status;
  BSPM_ELEMENT                   *Bspm;
  IBB_ELEMENT                    *BpmIbb;
  UINT32                         DataStackBase;
  UINT32                         DataStackSize;
  FLASH_REGION_LIST              FlashRegionList;
  FLASH_REGION_SEGMENT           FlashRegionFspm;
  UINT32                         BspBaseAddress;
  UINT32                         BspSize;
  FLASH_REGION_SEGMENT           FlashRegionBspPreMem;
  UINT32                         Index;

  //
  // Locate BSPM structure from the provided BSSS base address
  //
  Bspm = (BSPM_ELEMENT *) BsssBaseAddress;
  if (Bspm == NULL) {
    DEBUG ((DEBUG_ERROR, "Invalid BSSS, NEM allocation failed\n"));
    return EFI_INVALID_PARAMETER;
  }

  BpmIbb = LocateBpmIbbElement (); // Locate Bpm IBB Element
  if (BpmIbb == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Check boot component bit - if not set, no NEM allocation needed here
  //
  if (!((BpmIbb->Flags) & IBB_FLAG_BOOT_COMPONENT_BIT)) {
    return EFI_SUCCESS;
  }

  //
  // Get aligned data stack base and size
  //
  DataStackAlign ((UINT32) TopOfCar, &DataStackBase, &DataStackSize);

  //
  // Initialize the flash region list with FSP-O and FSP-T regions
  //
  InitializeFlashRegionList (&FlashRegionList);

  //
  // Allocate NEM for FSP-M
  //
  FlashRegionFspm.FlashRegionBase     = Bspm->FspmBaseAddress;
  FlashRegionFspm.FlashRegionSize     = FixedPcdGet32 (PcdFlashFvFspmSize);
  FlashRegionFspm.TargetNemBufferType = CacheWriteBack;

  //
  // Allocate NEM for FSPM.
  // When NEM has no enough size or MTRRs has no enough pairs, system should halt directly.
  //
  Status = AllocateNemForFlashRegion (
             &FlashRegionList,
             &FlashRegionFspm,
             DataStackBase,
             DataStackSize
             );
  if (Status == EFI_OUT_OF_RESOURCES) {
    DEBUG ((DEBUG_ERROR, "Insufficient NEM resources for FSP-M allocation\n"));
    CpuDeadLoop ();
  }

  //
  // Allocate NEM for BSP PreMem regions if they exist
  //
  if (Bspm->BspSegmentCount != 0) {
    for (Index = 0; Index < Bspm->BspSegmentCount; Index++) {
      GetBspInfo (Bspm, Index, &BspBaseAddress, &BspSize);

      FlashRegionBspPreMem.FlashRegionBase     = BspBaseAddress;
      FlashRegionBspPreMem.FlashRegionSize     = BspSize;
      FlashRegionBspPreMem.TargetNemBufferType = CacheWriteBack;

      //
      // Allocate NEM for BspPreMem.
      // When NEM has no enough size or MTRRs has no enough pairs, system should halt directly.
      //
      Status = AllocateNemForFlashRegion (
                 &FlashRegionList,
                 &FlashRegionBspPreMem,
                 DataStackBase,
                 DataStackSize
                 );
      if (Status == EFI_OUT_OF_RESOURCES) {
        DEBUG ((DEBUG_ERROR, "Insufficient NEM resources for BSP PreMem allocation (Index: %d)\n", Index));
        CpuDeadLoop ();
      }
    }
  }

  return EFI_SUCCESS;
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
  // Load FSP-M (NEM allocation already done by InitializeNemForPreMemComponents)
  //
  if ((Bspm->FspmLoadingPolicy & BIT0) == LOADING_FSPM) {
    Status = LoadFspm (Bspm, Fbm);
    if (Status == EFI_SUCCESS) {
      ExtendFspmRegion (&FspMeasurementInfo, Fbm, TpmActivePcrBanks);
    }
  }

  //
  // Load BspPreMem regions (NEM allocation already done by InitializeNemForPreMemComponents)
  //
  if (Bspm->BspSegmentCount != 0) {
    Status = LoadBspPreMem (Bspm);
    if (Status == EFI_SUCCESS) {
      ExtendBspRegion (&FspMeasurementInfo, Bspm, TpmActivePcrBanks);
    }
  }

  FspBuildMeasurementInfo (FspMeasurementInfo);
  return;
}
