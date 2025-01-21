/** @file
  The implementation about Me ReadyToBoot common functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/FspErrorInfoLib.h>
#include <Library/HeciInitLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/MeInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeInitLib.h>
#include <Library/MeChipsetLib.h>
#include <Register/HeciRegs.h>
#include <Protocol/Wdt.h>
#include <Protocol/AmtReadyToBoot.h>
#include <Protocol/MeDisplayMsgProtocol.h>
#include <PchResetPlatformSpecific.h>
#include "MeInit.h"

/**
  Checks for ME FW Status errors.
**/
VOID
CheckFwStatus (
  VOID
  )
{
  HECI_FWS_REGISTER        MeFirmwareStatus;
  EFI_STATUS               Status;
  ME_DISPLAY_MSG_PROTOCOL  *DisplayMsgProtocol;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    MeFirmwareStatus.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
  }

  Status = gBS->LocateProtocol (&gMeDisplayMsgProtocolGuid, NULL, (VOID**) &DisplayMsgProtocol);
  if (EFI_ERROR (Status)) {
    DisplayMsgProtocol = NULL;
  }

  switch (MeFirmwareStatus.r.CurrentState) {

    case ME_STATE_INVALID_CPU:
      if (DisplayMsgProtocol != NULL) {
        DisplayMsgProtocol->DisplayMeMsg (MsgIllegalCpuPluggedIn);
      }
      CpuDeadLoop ();
      break;

    case ME_STATE_DISABLE_WAIT:
      if (DisplayMsgProtocol != NULL) {
        DisplayMsgProtocol->DisplayMeMsg (MsgPlatDisableWait);
      }
      break;

    default:
      break;
  }
}

/**
  Dummy return for Me signal event use

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context
**/
VOID
EFIAPI
MeEmptyEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  gBS->CloseEvent (Event);
}


/**
  Performs End of Post related actions.

  @retval TRUE    Error has occurred, need to disable HECI1
  @retval FALSE   HECI1 interface needs to remain enabled
**/
BOOLEAN
SendEndOfPost (
  VOID
  )
{
  EFI_STATUS               Status;
  UINT32                   RequestedActions;
  WDT_PROTOCOL             *WdtProtocol;
  PCH_RESET_DATA           ResetData;
  ME_DISPLAY_MSG_PROTOCOL  *DisplayMsgProtocol;

  Status = gBS->LocateProtocol (&gMeDisplayMsgProtocolGuid, NULL, (VOID**) &DisplayMsgProtocol);
  if (EFI_ERROR (Status)) {
    DisplayMsgProtocol = NULL;
  }

  RequestedActions = HeciEopStatusSuccess;
  Status = MeEndOfPostEvent (&RequestedActions);
  MeSetEndOfPostDone ();
  if (EFI_ERROR (Status)) {
    if (DisplayMsgProtocol != NULL) {
      DisplayMsgProtocol->DisplayMeMsg (MsgEopError);
    }
    SendFspErrorInfoStatusCode (gMeFspErrorTypeCallerId, gMeFspErrorTypeEop, Status);
    return TRUE;
  } else if (RequestedActions == HeciEopPerformGlobalReset) {
    Status = gBS->LocateProtocol (&gWdtProtocolGuid, NULL, (VOID **) &WdtProtocol);
    if (!EFI_ERROR (Status)) {
      WdtProtocol->AllowKnownReset ();
    }
    DEBUG ((DEBUG_INFO, "%a - Global Reset requested by FW EOP ACK %r\n", __FUNCTION__, Status));
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
  }
  return FALSE;
}

/**
  To perform Me ReadyToBoot event function.
**/
VOID
MeReadyToBoot (
  VOID
  )
{
  ME_BIOS_BOOT_PATH               MeBiosPath;
  BOOLEAN                         MeDisable;
  EFI_STATUS                      Status;
  EFI_EVENT                       MePlatformReadyToBootEvent;
  EFI_HANDLE                      *Handles;
  UINTN                           Index;
  UINTN                           Count;
  AMT_READY_TO_BOOT_PROTOCOL      *AmtReadyToBoot;
  ME_HANDLE                       *MeHandle;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ///
  /// We will trigger all events in order
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gAmtReadyToBootProtocolGuid,
                  NULL,
                  &Count,
                  &Handles
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < Count; Index++) {
      Status = gBS->HandleProtocol (Handles[Index], &gAmtReadyToBootProtocolGuid, (VOID **) &AmtReadyToBoot);
      if (EFI_ERROR (Status)) {
        continue;
      }
      AmtReadyToBoot->Signal ();
    }
    gBS->FreePool (Handles);
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeEmptyEvent,
                  NULL,
                  &gMePlatformReadyToBootGuid,
                  &MePlatformReadyToBootEvent
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (MePlatformReadyToBootEvent);
  }

  CheckFwStatus ();

  MeDisable = FALSE;
  MeBiosPath = CheckMeBootPath ();
  switch (MeBiosPath) {
    case SecoverJmprPath:
    case SecoverMeiMsgPath:
    case SwTempDisablePath:
    case DebugModePath:
      ///
      /// If ME is running in those ME-BIOS paths - hide HECI1 prior to boot
      ///
      MeDisable = TRUE;
      break;
    case NormalPath:
    case RecoveryPath:
    case ErrorPath:
      ///
      /// Send EoP message for paths where HECI1 remains enabled - this is crucial for platform security
      ///
      if (MeEndOfPostEnabled ()) {
        MeDisable = SendEndOfPost ();
      }
      break;
    default:
      ///
      /// In other ME-BIOS paths HECI1 has already been disabled in PEI Post Mem
      ///
      break;
  }

  ///
  /// PMIR Configuration
  ///
  LockConfig ();

  if (((MeBiosPath == SwTempDisablePath) || (MeBiosPath == SecoverMeiMsgPath)) &&
      MeIsHeci1ForceEnabled ()) {
    MeDisable = FALSE;
  }

  if (MeDisable) {
    SetD0I3Bit (HECI1);
  }

  Status = GetMeHandle (&MeHandle);
  if (!EFI_ERROR (Status)) {
    PmcDisableMeDevices (MeHandle->PmcPwrmBase, MeDisable);
  }
}
