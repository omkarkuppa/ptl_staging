/** @file
  This file contains functions for creating TPM Event Log

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

@par Specification
**/

#include <PiPei.h>

#include <IndustryStandard/UefiTcgPlatform.h>
#include <Guid/TcgEventHob.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/HobLib.h>
#include <Library/BootGuardLib.h>
#include <PchResetPlatformSpecific.h>

/**
   Handle Tpm2Startup (State) failures reported by BIOS.

   The function checks for the TpmStartupFailureOnS3 flag if TRUE it will
   request the system to perform a cold reset.
**/
EFI_STATUS
EFIAPI
HandleTpmStartupFailureOnS3FromBios (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDesc,
  IN  VOID                          *InvokePpi
  )
{
  DEBUG ((DEBUG_INFO, "HandleTpmStartupFailureOnS3FromBios Entry\n"));

  VOID                            *TcgEventLog;
#if (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)
  BOOT_GUARD_INFO                 BootGuardInfo;

  //
  // Check for TPM Startup error during BootGuard ACM execution
  //
  if (IsBootGuardSupported () == TRUE) {
      GetBootGuardInfo (&BootGuardInfo);
      if (BootGuardInfo.TpmStartupFailureOnS3 == TRUE) {
          DEBUG ((EFI_D_ERROR, "ERROR: Tpm2Startup (State) failure during S3 reported by Boot Guard ACM, requesting cold reset... \n"));
          (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
          CpuDeadLoop ();
          return EFI_DEVICE_ERROR;
      }
  }
#endif // (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)


  //
  // Try to get TcgEventLog in S3 to see if S3 error is reported.
  //
  TcgEventLog = GetFirstGuidHob(&gTcgEventEntryHobGuid);
  if (TcgEventLog == NULL) {
    TcgEventLog = GetFirstGuidHob(&gTcgEvent2EntryHobGuid);
  }

  if (TcgEventLog == NULL) {
    //
    // no S3 error reported by BIOS
    //
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_ERROR, "ERROR: Tpm2Startup (State) failure during S3, requesting cold reset... \n"));
  (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
  CpuDeadLoop ();

  return EFI_DEVICE_ERROR;
}

EFI_PEI_NOTIFY_DESCRIPTOR mHandleTpmStartupFailureOnS3FromBiosNotifyList = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTpmInitializationDonePpiGuid,
  HandleTpmStartupFailureOnS3FromBios
};

/**
   The library constructor will register a callback to handle TPM Startup errors.

   @param  FileHandle   The handle of FFS header the loaded driver.
   @param  PeiServices  The pointer to the PEI services.

   @retval EFI_SUCCESS  The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PeiTpmS3ErrorHandlerConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "PeiTpmS3ErrorHandlerConstructor Entry\n"));
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
    DEBUG ((DEBUG_INFO, "Register TPM S3 failure handler.\n"));
    PeiServicesNotifyPpi (&mHandleTpmStartupFailureOnS3FromBiosNotifyList);
  }

  return EFI_SUCCESS;
}
