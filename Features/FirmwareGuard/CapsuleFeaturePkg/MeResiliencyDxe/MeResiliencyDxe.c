/** @file
  ME Resiliency init Dxe driver.

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/FirmwareManagement.h>
#include <Protocol/PlatformSpecificResetFilter.h>
#include <Register/HeciRegs.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeInfoLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/TopSwapSupportLib.h>

/**
  MeResiliencyCallBack

  ME Resiliency Feature to determine the CSE Image health and then
  initiate capsule based recovery.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
MeResiliencyCallBack (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                                     Status;
  UINT64                                         HeciBaseAddress;
  HECI_FWS_REGISTER                              MeFwSts;
  HECI_GS_SHDW_REGISTER                          MeFwSts2;
  PCH_RESET_DATA                                 ResetData;
  UINTN                                          ResetDataSize;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS                PreviousProgress;

  DEBUG ((DEBUG_INFO, "MeResiliencyCallBack Start\n"));

  Status  = EFI_SUCCESS;

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  MeFwSts.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
  MeFwSts2.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);

  if ((MeFwSts.ul == 0xFFFFFFFF) || (MeFwSts2.ul == 0xFFFFFFFF)) {
    DEBUG ((DEBUG_INFO, "ME is disabled, read status from MeFwHob\n"));
    MeFwSts.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
    MeFwSts2.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_2);
  }

  DEBUG ((DEBUG_INFO, "MeFwSts = 0x%x!\n", MeFwSts.ul));
  DEBUG ((DEBUG_INFO, "MeFwSts2 = 0x%x!\n", MeFwSts2.ul));

  if ((MeFwSts.r.CurrentState == ME_STATE_RECOVERY) ||
      (MeFwSts.r.FtBupLdFlr == 1) ||
      (MeFwSts2.r.FwUpdIpu == 1)) {
    DEBUG ((DEBUG_INFO, "ME region is corrupted, initiate ME recovery\n"));

    IsPreviousUpdateUnfinished (&PreviousProgress);
    if (IsMeRecoveryCapsuleExistOnStorage () &&
        ((PreviousProgress.Component == NotStarted) || (PreviousProgress.Component == UpdatingResiliency))) {
      //
      // Set Update Progress variable for ME resiliency in the next boot.
      //
      SetUpdateProgress (UpdatingMeResiliency, 0);

      //
      // Issue a global reset to initiate capsule based recovery.
      //
      ResetDataSize = sizeof (PCH_RESET_DATA);
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

      DEBUG ((DEBUG_INFO, "Issue global reset for capsule based recovery\n"));
      if ((PreviousProgress.Component == UpdatingResiliency) || IsTopSwapEnabled ()) {
        DEBUG ((DEBUG_INFO, "Defer global reset to Resiliency callback\n"));
      } else {
        gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, ResetDataSize, &ResetData);
      }
    } else {
      DEBUG ((DEBUG_ERROR, "ME Recovery capsule is not found on storage - skip recovery.\n"));
    }
  } else {
    //
    // Sync ME Recovery Capsules.
    //
    Status = SyncMeRecoveryCapsules ();
    DEBUG ((DEBUG_INFO, "SyncMeRecoveryCapsules - %r\n", Status));
  }

  DEBUG((DEBUG_INFO, "MeResiliencyCallBack End\n"));
  return;
}

/**
  Initialize Resiliency Support for ME.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS    Resiliency Support is initialized successfully
  @retval EFI_NOT_FOUND  Resiliency Support is not initialized successfully
**/

EFI_STATUS
EFIAPI
MeResiliencyDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_EVENT                 BeforeEndOfDxeEvent;

  Status                    = EFI_SUCCESS;
  BeforeEndOfDxeEvent       = NULL;

  DEBUG ((DEBUG_INFO, "MeResiliencyDxeEntryPoint Start\n"));

  DEBUG ((DEBUG_INFO, "Register an EndOfDxe Callback Function for ME Resiliency Health Check\n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeResiliencyCallBack,
                  NULL,
                  &gPlatformBeforeEndOfDxeEventGroupGuid,
                  &BeforeEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register an EndOfDxe CallBack function for ME Resiliency, Status: %d\n", Status));
  }

  DEBUG ((DEBUG_INFO, "MeResiliencyDxeEntryPoint End\n"));
  return Status;
}
