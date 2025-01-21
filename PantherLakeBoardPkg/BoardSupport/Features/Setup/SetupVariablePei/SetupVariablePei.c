/** @file
  Implementation of Setup variable initialization in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SetupInitLib.h>
#include <Library/UefiVariableDefaultHobLib.h>
#include <Library/SetupDataCacheLib.h>
#include <Library/SetupDebugConfigLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>

EFI_STATUS
EFIAPI
SetupVariableCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mSetupVariableNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  SetupVariableCallback
};

/**
  Callback caches variables.
**/
EFI_STATUS
EFIAPI
SetupVariableCacheCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  PPI to inidicate setup variables are ready.
**/
static EFI_PEI_PPI_DESCRIPTOR mSetupVariablesReadyPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSetupVariablesReadyPpiGuid,
  NULL
};

/**
  Notify caches setup variables on variable readiness.
**/
static EFI_PEI_NOTIFY_DESCRIPTOR mSetupVariableCacheNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSetupVariablesReadyPpiGuid,
  SetupVariableCacheCallback
};

/**
  Callback on gEfiPeiMasterBootModePpiGuidiGuid so Setup Variables are initialized.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The MasterBootMode PPI.  Not used.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_NOT_FOUND        - The matched default data is not found on CreateDefaultVariableHob
  @retval EFI_OUT_OF_RESOURCES - No enough resource to create HOB on CreateDefaultVariableHob
  @retval Others               - An error occurred locating the UEFI variables.
**/
EFI_STATUS
EFIAPI
SetupVariableCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                      Status;
  EFI_BOOT_MODE                   BootMode;
  BOOLEAN                         CreateUefiVariableDefaultHob;

  DEBUG ((DEBUG_INFO, "Preparing to initialize the Setup UEFI variable...\n"));

  CreateUefiVariableDefaultHob = FALSE;
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    if ((BootMode == BOOT_IN_RECOVERY_MODE || BootMode == BOOT_WITH_DEFAULT_SETTINGS)) {
      CreateUefiVariableDefaultHob = TRUE;
    }
  }

  // @todo: Add CMOS corruption check (checksum is bad) and clear diagnostic status
  //Hint: Need ResetBoardDefaultVariableHob () in PlatformNvRamHookLibCmos.c

  if (CreateUefiVariableDefaultHob) {
    DEBUG((DEBUG_INFO, "Enable Hii StructurePcd as setup default source.\n"));
    PcdSet16S(PcdSetNvStoreDefaultId, 0);
    // Update revision ID for the Setup variable structures
    Status = UpdateAllSetupVariableRevisions ();
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }
  }
  PeiServicesInstallPpi (&mSetupVariablesReadyPpi);

  DEBUG ((DEBUG_INFO, "Dumping Setup structures:\n"));
  Status = PrintSetupStructures ();
  if (!EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Setup structures successfully found.\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "Setup structures not found at end of Setup initialization.\n"));
  }
  return Status;
}

/**
  Callback caches variables.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gSetupVariablesReadyPpiGuid PPI. Not used.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval Others               - An error occurred locating the UEFI variables.
**/
EFI_STATUS
EFIAPI
SetupVariableCacheCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS     Status;
  DEBUG ((DEBUG_INFO, "Begin Caching Setup Data.\n"));
  Status = SetupDataCacheInit ();
  Status = SetupDebugConfigInit ();
  DEBUG ((DEBUG_INFO, "End Caching Setup Data Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval EFI_SUCCESS     The function completed successfully.

**/
EFI_STATUS
EFIAPI
SetupVariableEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  PeiServicesNotifyPpi(&mSetupVariableNotifyList);

  PeiServicesNotifyPpi(&mSetupVariableCacheNotifyList);

  return EFI_SUCCESS;
}
