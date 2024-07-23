/** @file
  Touch Host Controller Absolute Pointer API

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

#include "ThcPrivate.h"

/**
  AbsPtr protocol's WaitForInput (called when a consumer of AbsPtr protocol waits for new data)

  @param[in]  Event              Pointer to event that triggered this callback
  @param[in]  Context            Device context
**/
VOID
EFIAPI
AbsPtrWaitForInput (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{

}

/**
  Reset touch panel
  Function required by AbsPtr protocol.

  @param[in]  This
  @param[in]  ExtendedVerification

  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
AbsPtrReset (
  IN EFI_ABSOLUTE_POINTER_PROTOCOL *This,
  IN BOOLEAN                       ExtendedVerification
  )
{
  DEBUG ((DEBUG_INFO, "Thc reset attempted\n"));
  return EFI_UNSUPPORTED;
}

/**
  If not-yet-consumed touch data is available, GetState() returns it and marks it unavailable
  If such data is not available, error will be returned. User should wait until WaitForInput event gets signalled

  @param[in]      This
  @param[in/out]  State

  @retval EFI_SUCCESS     Data consumed
  @retval EFI_NOT_READY   Data not available
**/
EFI_STATUS
EFIAPI
AbsPtrGetState (
  IN      EFI_ABSOLUTE_POINTER_PROTOCOL  *This,
  IN OUT  EFI_ABSOLUTE_POINTER_STATE     *State
  )
{
  THC_DEV     *ThcDev;

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ThcDev  = THC_CONTEXT_FROM_ABSPTR_PROTOCOL (This);

  if (ThcDev->AbsPtrDataAvailable) {
    State->CurrentX      = (ThcDev->Report.HigherXByte << 8) + ThcDev->Report.LowerXByte;
    State->CurrentY      = (ThcDev->Report.HigherYByte << 8) + ThcDev->Report.LowerYByte;
    State->CurrentZ      = 0;
    State->ActiveButtons = ThcDev->Report.TouchSts;
    THC_LOCAL_DEBUG(L"AbsPtr State | X:%d | Y:%d | ActiveButton: %d \n", State->CurrentX, State->CurrentY, State->ActiveButtons)
    ThcDev->AbsPtrDataAvailable = FALSE;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_READY;
  }
}

/**
  Installs AbsolutePointerProtocol
  Assigns TouchPanel Guid for Virtual Keyboard support

  @param[in]  ThcDev            Context of Thc device

  @retval EFI_SUCCESS           Absolute Pointer Protocol Installed
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
ThcInstallAbsPtr (
  IN THC_DEV                *ThcDev
  )
{
  EFI_STATUS   Status;

  ThcDev->AbsPtrMode.AbsoluteMinX = 0;
  ThcDev->AbsPtrMode.AbsoluteMinY = 0;
  ThcDev->AbsPtrMode.AbsoluteMinZ = 0;
  ThcDev->AbsPtrMode.AbsoluteMaxX = 0x7FFF;
  ThcDev->AbsPtrMode.AbsoluteMaxY = 0x7FFF;
  ThcDev->AbsPtrMode.AbsoluteMaxZ = 0;
  ThcDev->AbsPtrMode.Attributes   = 0;

  ThcDev->AbsPtrProtocol.Reset    = AbsPtrReset;
  ThcDev->AbsPtrProtocol.GetState = AbsPtrGetState;
  ThcDev->AbsPtrProtocol.Mode     = &(ThcDev->AbsPtrMode);

  Status = gBS->CreateEvent (EVT_NOTIFY_WAIT, TPL_NOTIFY, AbsPtrWaitForInput, ThcDev, &((ThcDev->AbsPtrProtocol).WaitForInput));
  THC_LOCAL_DEBUG(L"ThcInstallAbsPtr CreateEvent WaitForInput  Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcInstallAbsPtr error, cannot create CreateEvent AbsPtrWaitForInput Status: %r\n", Status));
    return Status;
  }


  //
  // Install Absolute Pointer Protocol and TouchPanelGuid onto ImageHandle
  //
  if (ThcDev->InputReportTable.TouchPanel == 1) {
    // Installing with TouchPanel guid
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &ThcDev->ControllerHandle,
                    &gEfiAbsolutePointerProtocolGuid,
                    &ThcDev->AbsPtrProtocol,
                    &gEdkiiTouchPanelGuid,
                    NULL,
                    NULL
                    );
  }

  if (ThcDev->InputReportTable.TouchPad == 1) {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &ThcDev->ControllerHandle,
                    &gEfiAbsolutePointerProtocolGuid,
                    &ThcDev->AbsPtrProtocol,
                    NULL
                    );
  }

  THC_LOCAL_DEBUG(L"ThcInstallAbsPtr Install AbsPtrProtocol  Status: %r\n", Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ThcInstallAbsPtr ERROR - Thc failed to install AbsolutePointerProtocol, Status: %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Closes and uninstalls Absolute Pointer Protocol

  @param[in]  ThcDev            Context of Thc device

**/
VOID
EFIAPI
ThcStopAbsPtr (
  IN THC_DEV                *ThcDev
  )
{
  THC_LOCAL_DEBUG(L"ThcStopAbsPtr \n");
  ThcDev->AbsPtrDataAvailable = FALSE;
  //
  // Done with the events
  //
  if (ThcDev->PollingEvent != NULL) {
    gBS->CloseEvent (ThcDev->PollingEvent);
  }
  if (ThcDev->AbsPtrProtocol.WaitForInput != NULL) {
    gBS->CloseEvent (ThcDev->AbsPtrProtocol.WaitForInput);
  }
  //
  //  Close/Uninstall Abs Ptr Protocol
  //
  gBS->CloseProtocol (
         ThcDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &ThcDev->AbsPtrProtocol,
         ThcDev->ControllerHandle
         );
  gBS->UninstallMultipleProtocolInterfaces (
         ThcDev->ControllerHandle,
         &gEfiAbsolutePointerProtocolGuid,
         &ThcDev->AbsPtrProtocol,
         NULL
         );
}
