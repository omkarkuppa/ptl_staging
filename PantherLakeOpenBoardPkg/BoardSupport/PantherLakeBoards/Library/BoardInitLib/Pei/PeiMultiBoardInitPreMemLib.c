/**@file
  Panther Lake U RVP Multi-Board Initialization Pre-Memory library

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
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>
#include <Library/PeiServicesLib.h>

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


BOARD_DETECT_FUNC  mPtlBoardDetectFunc = {
  PtlMultiBoardDetect
};

BOARD_PRE_MEM_INIT_FUNC  mPtlBoardPreMemInitFunc = {
  PtlBoardDebugInit,
  NULL, //BoardBootModeDetect,
  PtlBoardInitBeforeMemoryInit,
  NULL, // BoardInitAfterMemoryInit
  NULL, // BoardInitBeforeTempRamExit
  NULL, // BoardInitAfterTempRamExit
};

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
  UINTN                  Instance;
  EFI_PEI_FV_HANDLE      VolumeHandle;
  EFI_PEI_FILE_HANDLE    FileHandle;

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

EFI_STATUS
EFIAPI
PtlMultiBoardDetect (
  VOID
  )
{
  UINTN                           VpdBaseAddress;

  DEBUG ((DEBUG_INFO, " In PtlMultiBoardDetect \n"));

  PtlBoardDetect ();

  VpdBaseAddress = (UINTN) PcdGet64 (PcdVpdBaseAddress64);
  DEBUG ((EFI_D_INFO, "VpdFfsAddress: %x\n", VpdBaseAddress));
  if (VpdBaseAddress == 0) {
    VpdBaseAddress= (UINTN) GetVpdFfsAddress();
    PcdSet64S (PcdVpdBaseAddress64,VpdBaseAddress);
    DEBUG ((EFI_D_INFO, "VpdFfsAddress updated: %x\n", VpdBaseAddress));
  }

  if (LibPcdGetSku () == SkuIdPtlSimics) {
    RegisterBoardPreMemInit (&mPtlBoardPreMemInitFunc);
  } else {
    DEBUG ((DEBUG_WARN,"Not a Valid PantherLake Simics Board\n"));
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiPtlMultiBoardInitPreMemLibConstructor (
  VOID
  )
{
  return RegisterBoardDetect (&mPtlBoardDetectFunc);
}
