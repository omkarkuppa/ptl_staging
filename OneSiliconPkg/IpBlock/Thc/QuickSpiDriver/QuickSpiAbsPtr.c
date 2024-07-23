/** @file
  Touch Host Controller Absolute Pointer API

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

#include "QuickSpiPrivate.h"

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
  DEBUG ((DEBUG_INFO, "QuickSpi reset attempted\n"));
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
  QUICK_SPI_DEV     *QuickSpiDev;

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  QuickSpiDev  = QUICK_SPI_CONTEXT_FROM_ABSPTR_PROTOCOL (This);

  if (QuickSpiDev->AbsPtrDataAvailable) {
    State->CurrentX      = (QuickSpiDev->Report.HigherXByte << 8) + QuickSpiDev->Report.LowerXByte;
    State->CurrentY      = (QuickSpiDev->Report.HigherYByte << 8) + QuickSpiDev->Report.LowerYByte;
    State->CurrentZ      = 0;
    State->ActiveButtons = QuickSpiDev->Report.TouchSts;
    THC_LOCAL_DEBUG(L"AbsPtr State | X:%d | Y:%d | ActiveButton: %d \n", State->CurrentX, State->CurrentY, State->ActiveButtons)
    QuickSpiDev->AbsPtrDataAvailable = FALSE;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_READY;
  }
}

/**
  Installs AbsolutePointerProtocol
  Assigns TouchPanel Guid for Virtual Keyboard support

  @param[in]  QuickSpiDev            Context of QuickSpi device

  @retval EFI_SUCCESS           Absolute Pointer Protocol Installed
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
QuickSpiInstallAbsPtr (
  IN QUICK_SPI_DEV                *QuickSpiDev
  )
{
  EFI_STATUS   Status;

  QuickSpiDev->AbsPtrMode.AbsoluteMinX = 0;
  QuickSpiDev->AbsPtrMode.AbsoluteMinY = 0;
  QuickSpiDev->AbsPtrMode.AbsoluteMinZ = 0;
  QuickSpiDev->AbsPtrMode.AbsoluteMaxX = 0x7FFF;
  QuickSpiDev->AbsPtrMode.AbsoluteMaxY = 0x7FFF;
  QuickSpiDev->AbsPtrMode.AbsoluteMaxZ = 0;
  QuickSpiDev->AbsPtrMode.Attributes   = 0;

  QuickSpiDev->AbsPtrProtocol.Reset    = AbsPtrReset;
  QuickSpiDev->AbsPtrProtocol.GetState = AbsPtrGetState;
  QuickSpiDev->AbsPtrProtocol.Mode     = &(QuickSpiDev->AbsPtrMode);

  Status = gBS->CreateEvent (EVT_NOTIFY_WAIT, TPL_NOTIFY, AbsPtrWaitForInput, QuickSpiDev, &((QuickSpiDev->AbsPtrProtocol).WaitForInput));
  THC_LOCAL_DEBUG(L"QuickSpiInstallAbsPtr CreateEvent WaitForInput  Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiInstallAbsPtr error, cannot create CreateEvent AbsPtrWaitForInput Status: %r\n", Status));
    return Status;
  }

  //
  // Install Absolute Pointer Protocol and TouchPanelGuid onto ImageHandle
  //
  if (QuickSpiDev->InputReportTable.TouchPanel == 1) {
    // Installing with TouchPanel guid
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &QuickSpiDev->ControllerHandle,
                    &gEfiAbsolutePointerProtocolGuid,
                    &QuickSpiDev->AbsPtrProtocol,
                    &gEdkiiTouchPanelGuid,
                    NULL,
                    NULL
                    );
  }

  if (QuickSpiDev->InputReportTable.TouchPad == 1) {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &QuickSpiDev->ControllerHandle,
                    &gEfiAbsolutePointerProtocolGuid,
                    &QuickSpiDev->AbsPtrProtocol,
                    NULL
                    );
  }

  THC_LOCAL_DEBUG(L"QuickSpiInstallAbsPtr Install AbsPtrProtocol  Status: %r\n", Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickSpiInstallAbsPtr ERROR - QuickSpi failed to install AbsolutePointerProtocol, Status: %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Closes and uninstalls Absolute Pointer Protocol

  @param[in]  QuickSpiDev            Context of QuickSpi device

**/
VOID
EFIAPI
QuickSpiStopAbsPtr (
  IN QUICK_SPI_DEV                *QuickSpiDev
  )
{
  THC_LOCAL_DEBUG(L"QuickSpiStopAbsPtr \n");
  QuickSpiDev->AbsPtrDataAvailable = FALSE;
  //
  // Done with the events
  //
  if (QuickSpiDev->PollingEvent != NULL) {
    gBS->CloseEvent (QuickSpiDev->PollingEvent);
  }
  if (QuickSpiDev->AbsPtrProtocol.WaitForInput != NULL) {
    gBS->CloseEvent (QuickSpiDev->AbsPtrProtocol.WaitForInput);
  }
  //
  //  Close/Uninstall Abs Ptr Protocol
  //
  gBS->CloseProtocol (
         QuickSpiDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &QuickSpiDev->AbsPtrProtocol,
         QuickSpiDev->ControllerHandle
         );
  gBS->UninstallMultipleProtocolInterfaces (
         QuickSpiDev->ControllerHandle,
         &gEfiAbsolutePointerProtocolGuid,
         &QuickSpiDev->AbsPtrProtocol,
         NULL
         );
}
