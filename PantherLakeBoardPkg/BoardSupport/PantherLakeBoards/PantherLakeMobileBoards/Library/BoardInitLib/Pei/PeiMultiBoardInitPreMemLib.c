/** @file
  PEI Multi-Board Initialization in Pre-Memory PEI Library

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

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BoardConfigLib.h>
#include <Library/EcMiscLib.h>
#include <PlatformBoardId.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Ppi/Spi.h>
#include <BiosGuardConfig.h>
#include <TcssDataHob.h>
#include <Guid/MigratedFvInfo.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <SetupVariable.h>
#include <Library/HobLib.h>

EFI_STATUS
EFIAPI
PtlBoardDetect (
  VOID
  );

EFI_STATUS
EFIAPI
PtlMultiBoardDetect (
  VOID
  );

EFI_BOOT_MODE
EFIAPI
PtlBoardBootModeDetect (
  VOID
  );

EFI_STATUS
EFIAPI
PtlBoardDebugInit (
  VOID
  );

EFI_STATUS
EFIAPI
PtlBoardInitBeforeMemoryInit (
  VOID
  );

EFI_STATUS
EFIAPI
PtlBoardInitAfterMemoryInit (
  VOID
  );

EFI_STATUS
EFIAPI
ResetMemoryAddress (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );


static EFI_PEI_NOTIFY_DESCRIPTOR  mMigrationMemoryList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMemoryDiscoveredPpiGuid,
    ResetMemoryAddress
  }
};

STATIC EFI_PEI_PPI_DESCRIPTOR mBoardDetectedPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardDetectedPpiGuid,
  NULL
};

BOARD_DETECT_FUNC  mPtlBoardDetectFunc = {
  PtlMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mPtlBoardPreMemInitFunc = {
  PtlBoardDebugInit,
  PtlBoardBootModeDetect,
  PtlBoardInitBeforeMemoryInit,
  PtlBoardInitAfterMemoryInit,
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

typedef struct {
  EFI_HOB_GUID_TYPE                Header;
  UINT32                           TotalCnt;
  UINT32                           CacheCnt;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariablePpi;
  SETUP_DATA                       SetupData;
  UINT32                           SetupAttributes;
  BOARD_INFO_SETUP                 BoardInfoSetup;
  UINT32                           BoardInfoAttributes;
  SA_SETUP                         SaSetup;
  UINT32                           SaAttributes;
  ME_SETUP                         MeSetup;
  UINT32                           MeAttributes;
  CPU_SETUP                        CpuSetup;
  UINT32                           CpuAttributes;
  PCH_SETUP                        PchSetup;
  UINT32                           PchAttributes;
} SETUP_DATA_HOB;

/**
  Migrate the pointer between flash address to memory address
  Leverage the original HOB created by Core.
  @param[in, out] FlashPointer Pointer to flash address will result page fault
**/
STATIC
VOID
MigrateFlashPointer (
  IN OUT UINTN  *FlashPointer
  )
{
  VOID                    *Hob;
  EDKII_MIGRATED_FV_INFO  *MigratedFvInfo;

  Hob  = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);

  while (Hob != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);

    if ((*FlashPointer > MigratedFvInfo->FvOrgBase) &&
        (*FlashPointer < (MigratedFvInfo->FvOrgBase + MigratedFvInfo->FvLength - 1))) {
      // Migrate the pointer
      if (MigratedFvInfo->FvOrgBase >= MigratedFvInfo->FvNewBase) {
        *FlashPointer += MigratedFvInfo->FvNewBase - MigratedFvInfo->FvOrgBase;
      } else {
        *FlashPointer -= MigratedFvInfo->FvOrgBase - MigratedFvInfo->FvNewBase;
      }

      // break after migrate successfully
      break;
    }

    Hob = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, GET_NEXT_HOB (Hob));
  }
}

/**
  Migrate the pointer between flash address to memory address
  Leverage the original HOB created by Core.
  @param[in, out] FlashPointer Pointer to flash address will result page fault
**/
STATIC
VOID
MigrateBiosGuardFlashPointer (
  IN OUT EFI_PHYSICAL_ADDRESS  *BiosGuardFlashPointer
  )
{
  VOID                    *Hob;
  EDKII_MIGRATED_FV_INFO  *MigratedFvInfo;

  Hob  = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);

  while (Hob != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);

    if ((*BiosGuardFlashPointer > MigratedFvInfo->FvOrgBase) &&
        (*BiosGuardFlashPointer < (MigratedFvInfo->FvOrgBase + MigratedFvInfo->FvLength - 1))) {
      // Migrate the pointer
      if (MigratedFvInfo->FvOrgBase >= MigratedFvInfo->FvNewBase) {
        *BiosGuardFlashPointer += MigratedFvInfo->FvNewBase - MigratedFvInfo->FvOrgBase;
      } else {
        *BiosGuardFlashPointer -= MigratedFvInfo->FvOrgBase - MigratedFvInfo->FvNewBase;
      }

      // break after migrate successfully
      break;
    }

    Hob = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, GET_NEXT_HOB (Hob));
  }
}

/**
  TOC/TOU migration issues are maintained by this function for BoardPkg.
  Find the memory address or pointer then call MigrateFlashPointer() for processing.
  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  PPI.
**/
EFI_STATUS
EFIAPI
ResetMemoryAddress (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  UINTN                          VpdBaseAddress;
  SETUP_DATA_HOB                 *SetupDataHob;
  EFI_STATUS                     Status;
  PCH_SPI_PPI                    *SpiPpi;
  BIOSGUARD_HOB                  *BiosGuardHobPtr;
  TCSS_DATA_SSID_HOB             *TcssSsidHob;

  SpiPpi = NULL;

  DEBUG ((DEBUG_INFO, "MultiBoard ResetMemoryAddress - Start\n"));

  //
  // Reset VpdBaseAddress
  //
  DEBUG ((DEBUG_INFO, "Reset VpdBaseAddress\n"));
  VpdBaseAddress = (UINTN)PcdGet64 (PcdVpdBaseAddress64);
  MigrateFlashPointer (&VpdBaseAddress);
  PcdSet64S (PcdVpdBaseAddress64, VpdBaseAddress);

  //
  // Reset PCD value
  //
  DEBUG ((DEBUG_INFO, "Reset PCD value\n"));
  PcdSet64S (PcdUsb2PhyTuningTable, (UINT64) PcdGetPtr (VpdPcdUsb2PhyTuningTable));

  //
  // Reset VariablePpi
  //
  DEBUG ((DEBUG_INFO, "Reset VariablePpi\n"));
  SetupDataHob = (SETUP_DATA_HOB *)(VOID *)GetFirstGuidHob (&gSetupDataHobGuid);
  if (SetupDataHob != NULL) {
    MigrateFlashPointer ((UINTN *)&SetupDataHob->VariablePpi);
  }

  //
  // Reset SpiPpi->GetRegionAddress
  //
  DEBUG ((DEBUG_INFO, "Reset SpiPpi->GetRegionAddress\n"));
  Status = PeiServicesLocatePpi (&gPchSpiPpiGuid, 0, NULL, (VOID **) &SpiPpi);
  if (!EFI_ERROR (Status)) {
    MigrateFlashPointer ((UINTN *) &SpiPpi->GetRegionAddress);
  }

  //
  // Reset BiosGuardHobPtr->BiosGuardModulePtr
  //
  DEBUG ((DEBUG_INFO, "Reset BiosGuardHobPtr->BiosGuardModulePtr\n"));
  BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (BiosGuardHobPtr != NULL) {
    MigrateBiosGuardFlashPointer (&BiosGuardHobPtr->BiosGuardModulePtr);
  }

  //
  // Reset TcssSsidHob->TcssSsidTable
  //
  DEBUG ((DEBUG_INFO, "Reset TcssSsidHob->TcssSsidTable\n"));
  TcssSsidHob = GetFirstGuidHob (&gTcssSsidHobGuid);
  if (TcssSsidHob != NULL) {
    MigrateFlashPointer ((UINTN *)&TcssSsidHob->TcssSsidTable);

  }

  DEBUG ((DEBUG_INFO, "ResetMemoryAddress - End\n"));

  return EFI_SUCCESS;
}

/**
  Get Vpd binary address

  Parse through each FV for VPD FFS file and return the address

  @retval Address on VPD FFS detection else returns 0

**/
UINTN
EFIAPI
GetVpdFfsAddress (
  )
{
  EFI_STATUS            Status;
  VOID                  *Address;
  UINTN                 Instance;
  EFI_PEI_FV_HANDLE     VolumeHandle;
  EFI_PEI_FILE_HANDLE   FileHandle;

  Address = NULL;

  VolumeHandle = NULL;
  Instance = 0;
  while (TRUE) {
    //
    // Traverse all firmware volume instances.
    //
    Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (&gVpdFfsGuid, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Search RAW section.
      //
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, &Address);
      if (!EFI_ERROR (Status)) {
        return (UINTN)Address;
      }
    }

    //
    // Search the next volume.
    //
    Instance++;
  }

  DEBUG ((EFI_D_ERROR, " PEI get VPD address: %r\n", EFI_NOT_FOUND));
  return 0;
}

VOID
EFIAPI
BoardConfigGpioInit (
  VOID
  )
{
  GPIOV2_INIT_CONFIG  *VpdPcdGpioTable;
  EFI_STATUS          Status;

  VpdPcdGpioTable = NULL;


  //
  // Coin-less mode SPI-NOR clear
  //
  VpdPcdGpioTable = PcdGetPtr (VpdPcdCoinlessSpiNorClearGpio);
  PcdSet32S (PcdCoinlessSpiNorClearGpio, VpdPcdGpioTable->GpioPad);
  if ((VpdPcdGpioTable->GpioPad) != 0) {
    Status = GpioV2ConfigurePad (PcdGet32(PcdCoinlessSpiNorClearGpio), &VpdPcdGpioTable->GpioConfig);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Configure COINLESS_SPI_NOR_CLEAR Pad failed.\n"));
    }
  }

}

EFI_STATUS
EFIAPI
PtlMultiBoardDetect (
  VOID
  )
{
  UINTN VpdBaseAddress;

  DEBUG ((DEBUG_INFO, " In PtlMultiBoardDetect \n"));

  PtlBoardDetect ();

  VpdBaseAddress = (UINTN) PcdGet64 (PcdVpdBaseAddress64);
  DEBUG ((EFI_D_INFO, "VpdFfsAddress: %x\n", VpdBaseAddress));
  if (VpdBaseAddress == 0) {
    VpdBaseAddress= (UINTN) GetVpdFfsAddress();
    PcdSet64S (PcdVpdBaseAddress64,VpdBaseAddress);
    DEBUG ((EFI_D_INFO, "VpdFfsAddress updated: %x\n", VpdBaseAddress));
  }

  PcdSet8S (PcdSkuType, PcdGet8 (VpdPcdSkuType));
  DEBUG ((DEBUG_INFO, "SKU Type: %x , %x \n", PcdGet8 (PcdSkuType), PcdGet8 (VpdPcdSkuType)));

  PeiServicesInstallPpi (&mBoardDetectedPpi);

  BoardConfigGpioInit ();

  RegisterBoardPreMemInit (&mPtlBoardPreMemInitFunc);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiPtlMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  EFI_STATUS         Status;

  DEBUG ((DEBUG_INFO, "PeiPtlMultiBoardInitPreMemLibConstructor() Start\n"));

  if (PcdGetBool (PcdMigrateTemporaryRamFirmwareVolumes)) {
    Status = PeiServicesNotifyPpi (mMigrationMemoryList);
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "PeiPtlMultiBoardInitPreMemLibConstructor() End\n"));
  return RegisterBoardDetect (&mPtlBoardDetectFunc);
}
