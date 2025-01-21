/** @file
  MeFwDowngrade driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DxeMeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeInfoLib.h>
#include <Library/MeInitLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/MeDisplayMsgProtocol.h>
#include <Register/HeciRegs.h>
#include <PchResetPlatformSpecific.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8    mHeciHmrfpoLockResult;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8    mHeciHmrfpoEnableResult;

/**
  Tell User that we failed to lock/unlock the flash - Do this after we have graphics initialized

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context
**/
VOID
EFIAPI
MeFwDowngradeMsgEvent (
  IN EFI_EVENT                    Event,
  IN VOID                         *Context
  )
{
  EFI_STATUS                      Status;
  ME_DISPLAY_MSG_PROTOCOL         *DisplayMsgProtocol;

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gMeDisplayMsgProtocolGuid, NULL, (VOID**) &DisplayMsgProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (mHeciHmrfpoLockResult != HMRFPO_LOCK_SUCCESS) {
    DisplayMsgProtocol->DisplayMeMsg (MsgHmrfpoLockFailure);
  }

  if (mHeciHmrfpoEnableResult != HMRFPO_ENABLE_SUCCESS) {
    DisplayMsgProtocol->DisplayMeMsg (MsgHmrfpoUnLockFailure);
  }

  return;
}

/**
  Send the HMRFPO_DISABLE MEI message.

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context
**/
VOID
EFIAPI
MeHmrfpoDisableEvent (
  IN EFI_EVENT                       Event,
  IN VOID                            *Context
  )
{
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "(B3) Me FW Downgrade - Send the HMRFPO_DISABLE MEI message\n"));

  Status = HeciHmrfpoDisable ();
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Me FW Downgrade Disable Msg Received Successfully\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "Me FW Downgrade Disable Msg ACK not Received\n"));
  }

  gBS->CloseEvent (Event);
}

/**
  Entry point for the MeFwDowngrade driver

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             The driver initializes correctly.
  @retval Others                  Status code returned by CreateEventEx.
**/
EFI_STATUS
EFIAPI
MeFwDowngradeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_EVENT         MeFwDowngradeEvent;
  UINT32            MeMode;
  UINT32            MeStatus;
  HECI_FWS_REGISTER MeFirmwareStatus;
  PCH_RESET_DATA    ResetData;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MeBiosGetMeStatus (&MeStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }


  ///
  /// (B1) Check if ME is in normal mode.
  ///
  if (MeMode == ME_MODE_NORMAL) {
    ///
    /// (A2) Intel Me image re-flash is requested?
    ///
    if (MeFwDowngradeSupported ()) {
      ///
      /// (A4)(A5) The BIOS sends the HMRFPO ENABLE MEI message and wait for response.
      ///
      mHeciHmrfpoEnableResult  = HMRFPO_ENABLE_UNKNOWN_FAILURE;
      Status                   = HeciHmrfpoEnable (&mHeciHmrfpoEnableResult);
      if ((Status == EFI_SUCCESS) && (mHeciHmrfpoEnableResult == HMRFPO_ENABLE_SUCCESS)) {
        ///
        /// (A6) The BIOS sends the GLOBAL RESET MEI message
        ///
        DEBUG ((DEBUG_INFO, "Me FW Downgrade !!- Step A6\n"));
        ZeroMem (&ResetData, sizeof (ResetData));
        CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
        StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

        gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
        CpuDeadLoop ();
      }
      ///
      /// (A8) Error Handling, HeciHmrfpoEnable include error handling.
      ///
      DEBUG ((
        DEBUG_ERROR,
        "Me FW Downgrade Error !!- Step A8, the Status is %r, The result is %x\n",
        Status,
        mHeciHmrfpoEnableResult
        ));
    } else {
      ///
      /// (A7) The BIOS sends the HMRFPO Lock MEI message and continues the normal boot
      ///
      mHeciHmrfpoLockResult = HMRFPO_LOCK_SUCCESS;

      ///
      /// The ME firmware will ignore the HMRFPO LOCK command if ME is in ME manufacturing mode
      ///
      if ((MeFirmwareStatus.r.SpiProtectionMode == 0) &&
          ((ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_IN_RECOVERY_MODE) || (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY))) {

        DEBUG ((DEBUG_INFO, "(A7) Me FW Downgrade - The BIOS sends the HMRFPO Lock MEI message and continues the normal boot\n"));

        Status = HeciHmrfpoLock (&mHeciHmrfpoLockResult);
        if (EFI_ERROR (Status)) {
          mHeciHmrfpoLockResult = HMRFPO_LOCK_FAILURE;
        }
      }
    }

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    MeFwDowngradeMsgEvent,
                    (VOID *) &ImageHandle,
                    &gMePlatformReadyToBootGuid,
                    &MeFwDowngradeEvent
                    );

  }
  ///
  /// (B3) Create an event that will call the HMRFPO_DISABLE
  ///
  if ((MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_HECI_MSG) &&
      (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY)) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    MeHmrfpoDisableEvent,
                    (VOID *) &ImageHandle,
                    &gMePlatformReadyToBootGuid,
                    &MeFwDowngradeEvent
                    );
  }

  return Status;
}
