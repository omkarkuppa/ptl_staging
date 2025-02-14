/** @file
  Touch Host Controller Absolute Pointer API

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

@par Specification Reference:
**/

#include "QuickI2cPrivate.h"
#include "QuickI2cDriver.h"

/**
  SimplePointer protocol's WaitForInput (called when a consumer of SimplePtr protocol waits for new data)

  @param[in]  Event              Pointer to event that triggered this callback
  @param[in]  Context            Device context
**/
VOID
EFIAPI
SimplePtrWaitForInput (
  IN EFI_EVENT Event,
  IN VOID*     Context
)
{

}

/**
  Reset touch panel
  Function required by SimplePtr protocol.

  @param[in]  This
  @param[in]  ExtendedVerification

  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SimplePtrReset (
  IN EFI_SIMPLE_POINTER_PROTOCOL* This,
  IN BOOLEAN                      ExtendedVerification
)
{
  QUICK_I2C_DEV     *QuickI2cDev;

  QuickI2cDev  = QUICK_I2C_CONTEXT_FROM_SIMPLEPTR_PROTOCOL (This);

  ZeroMem (
    &(QuickI2cDev->SimplePointerData),
    sizeof (EFI_SIMPLE_POINTER_STATE)
    );

  ZeroMem (
    &(QuickI2cDev->MouseReport),
    sizeof (MOUSE_SINGLE_FINGER_REPORT)
    );

  QuickI2cDev->SimplePtrDataAvailable = FALSE;
  return EFI_SUCCESS;
}

/**
  If not-yet-consumed touch data is available, GetState() returns it and marks it unavailable
  If such data is not available, error will be returned. User should wait until WaitForInput event gets signalled

  @param[in]              This
  @param[in/out]          State

  @retval EFI_SUCCESS     Data consumed
  @retval EFI_NOT_READY   Data not available
**/
EFI_STATUS
EFIAPI
SimplePtrGetState (
  IN      EFI_SIMPLE_POINTER_PROTOCOL* This,
  IN OUT  EFI_SIMPLE_POINTER_STATE*    State
)
{
  QUICK_I2C_DEV* QuickI2cDev;

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_SIMPLEPTR_PROTOCOL (This);

  if (!QuickI2cDev->SimplePtrDataAvailable) {
    return EFI_NOT_READY;
  }

  State->RelativeMovementX = QuickI2cDev->MouseReport.X;
  State->RelativeMovementY = QuickI2cDev->MouseReport.Y;
  State->RelativeMovementZ = 0;
  State->LeftButton        = QuickI2cDev->MouseReport.TouchSts;
  State->RightButton       = 0;

  THC_LOCAL_DEBUG(L"SimplePtr State | X:%d | Y:%d| B:%d \n", State->RelativeMovementX, State->RelativeMovementY, State->LeftButton)

  // Clear previous move state
  QuickI2cDev->SimplePtrDataAvailable = FALSE;
  ZeroMem (
    &(QuickI2cDev->MouseReport),
    sizeof (MOUSE_SINGLE_FINGER_REPORT)
    );

  return EFI_SUCCESS;
}


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
  QUICK_I2C_DEV     *QuickI2cDev;

  QuickI2cDev  = QUICK_I2C_CONTEXT_FROM_ABSPTR_PROTOCOL (This);

  ZeroMem (
    &(QuickI2cDev->AbsPtrTouchData),
    sizeof (EFI_ABSOLUTE_POINTER_STATE)
    );

  ZeroMem (
    &(QuickI2cDev->Report),
    sizeof (SINGLE_FINGER_REPORT)
    );

  QuickI2cDev->AbsPtrDataAvailable = FALSE;
  return EFI_SUCCESS;
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
  QUICK_I2C_DEV     *QuickI2cDev;

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  QuickI2cDev  = QUICK_I2C_CONTEXT_FROM_ABSPTR_PROTOCOL (This);

  if (QuickI2cDev->AbsPtrDataAvailable) {
    State->CurrentX      = (QuickI2cDev->Report.HigherXByte << 8) + QuickI2cDev->Report.LowerXByte;
    State->CurrentY      = (QuickI2cDev->Report.HigherYByte << 8) + QuickI2cDev->Report.LowerYByte;
    State->CurrentZ      = 0;
    State->ActiveButtons = QuickI2cDev->Report.TouchSts;
    THC_LOCAL_DEBUG (L"AbsPtr State | X:%d | Y:%d | ActiveButton: %d \n", State->CurrentX, State->CurrentY, State->ActiveButtons)
    QuickI2cDev->AbsPtrDataAvailable = FALSE;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_READY;
  }
}

/**
  Installs AbsolutePointerProtocol
  Assigns TouchPanel Guid for Virtual Keyboard support

  @param[in]  QuickI2cDev            Context of QuickI2c device

  @retval EFI_SUCCESS           Absolute Pointer Protocol Installed
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
QuickI2cInstallAbsPtr (
  IN QUICK_I2C_DEV                *QuickI2cDev
  )
{
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "TouchPanel: %d, TouchPad: %d, Mouse: %d\n",
          QuickI2cDev->InputReportTable.TouchPanel,
          QuickI2cDev->InputReportTable.TouchPad,
          QuickI2cDev->InputReportTable.Mouse)
        );

  // Install Absolute Pointer Protocol and TouchPanelGuid onto ImageHandle
  //
  if ((QuickI2cDev->InputReportTable.TouchPanel == 1) || (QuickI2cDev->InputReportTable.TouchPad == 1)) {
    QuickI2cDev->AbsPtrMode.AbsoluteMinX = 0;
    QuickI2cDev->AbsPtrMode.AbsoluteMinY = 0;
    QuickI2cDev->AbsPtrMode.AbsoluteMinZ = 0;
    QuickI2cDev->AbsPtrMode.AbsoluteMaxX = 0x7FFF;
    QuickI2cDev->AbsPtrMode.AbsoluteMaxY = 0x7FFF;
    QuickI2cDev->AbsPtrMode.AbsoluteMaxZ = 0;
    QuickI2cDev->AbsPtrMode.Attributes   = 0;
    QuickI2cDev->AbsPtrProtocol.Reset    = AbsPtrReset;
    QuickI2cDev->AbsPtrProtocol.GetState = AbsPtrGetState;
    QuickI2cDev->AbsPtrProtocol.Mode     = &(QuickI2cDev->AbsPtrMode);

    Status = gBS->CreateEvent (
                    EVT_NOTIFY_WAIT,
                    TPL_NOTIFY,
                    AbsPtrWaitForInput,
                    QuickI2cDev,
                    &((QuickI2cDev->AbsPtrProtocol).WaitForInput)
                    );
    THC_LOCAL_DEBUG(L"QuickI2cInstallAbsPtr CreateEvent WaitForInput  Status: %r\n", Status)
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_WARN, "QuickI2cInstallAbsPtr error, cannot create CreateEvent AbsPtrWaitForInput Status: %r\n", Status));
      return Status;
    }

    //
    // Install Absolute Pointer Protocol and TouchPanelGuid onto ImageHandle
    //
    if (QuickI2cDev->InputReportTable.TouchPanel == 1) {
      // Installing with TouchPanel guid
      THC_LOCAL_DEBUG (L"QuickI2cInstallAbsPtr install protocol for TouchPanel\n")
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &QuickI2cDev->ControllerHandle,
                      &gEfiAbsolutePointerProtocolGuid,
                      &QuickI2cDev->AbsPtrProtocol,
                      &gEdkiiTouchPanelGuid,
                      NULL,
                      NULL
                      );
    }

    if (QuickI2cDev->InputReportTable.TouchPad == 1) {
      THC_LOCAL_DEBUG (L"QuickI2cInstallAbsPtr install protocol for TouchPad\n")
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &QuickI2cDev->ControllerHandle,
                      &gEfiAbsolutePointerProtocolGuid,
                      &QuickI2cDev->AbsPtrProtocol,
                      NULL
                      );
    }
  }

  //
  // Install Simple Pointer Protocol onto ImageHandle
  //
  if (QuickI2cDev->InputReportTable.Mouse == 1) {
    QuickI2cDev->SimplePointerMode.LeftButton  = FALSE;
    QuickI2cDev->SimplePointerMode.RightButton = FALSE;
    QuickI2cDev->SimplePointerMode.ResolutionX = 8;
    QuickI2cDev->SimplePointerMode.ResolutionY = 8;
    QuickI2cDev->SimplePointerMode.ResolutionZ = 0;
    QuickI2cDev->SimplePtrProtocol.Reset       = SimplePtrReset;
    QuickI2cDev->SimplePtrProtocol.GetState    = SimplePtrGetState;
    QuickI2cDev->SimplePtrProtocol.Mode        = &(QuickI2cDev->SimplePointerMode);

    Status = gBS->CreateEvent (
                    EVT_NOTIFY_WAIT,
                    TPL_NOTIFY,
                    SimplePtrWaitForInput,
                    QuickI2cDev,
                    &((QuickI2cDev->SimplePtrProtocol).WaitForInput)
                    );
    THC_LOCAL_DEBUG(L"QuickI2cInstallSimplePtr CreateEvent WaitForInput  Status: %r\n", Status)
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_WARN, "QuickI2cInstallSimplePtr error, cannot create CreateEvent SimplePtrWaitForInput Status: %r\n", Status));
      return Status;
    }

    THC_LOCAL_DEBUG (L"QuickI2cInstallSimplePtr install protocol for TouchPad\n")
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &QuickI2cDev->ControllerHandle,
                    &gEfiSimplePointerProtocolGuid,
                    &QuickI2cDev->SimplePtrProtocol,
                    NULL
                    );
    THC_LOCAL_DEBUG(L"QuickI2cInstallAbs Install SimplePtrProtocol  Status: %r\n", Status);
  }

  return Status;
}

/**
  Closes and uninstalls Absolute Pointer Protocol

  @param[in]  QuickI2cDev            Context of QuickI2c device

**/
VOID
EFIAPI
QuickI2cStopAbsPtr (
  IN QUICK_I2C_DEV                *QuickI2cDev
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cStopAbsPtr \n");
  QuickI2cDev->AbsPtrDataAvailable = FALSE;
  QuickI2cDev->SimplePtrDataAvailable = FALSE;

  //
  // Done with the events
  //
  if (QuickI2cDev->PollingEvent != NULL) {
    gBS->CloseEvent (QuickI2cDev->PollingEvent);
  }

  //
  //  Close/Uninstall Abs Ptr Protocol
  //
  if (QuickI2cDev->AbsPtrProtocol.WaitForInput != NULL) {
    gBS->CloseEvent (QuickI2cDev->AbsPtrProtocol.WaitForInput);
    gBS->UninstallMultipleProtocolInterfaces (
           QuickI2cDev->ControllerHandle,
           &gEfiAbsolutePointerProtocolGuid,
           &QuickI2cDev->AbsPtrProtocol,
           NULL
        );
  }

  //
  //  Close/Uninstall Simple Ptr Protocol
  //
  if (QuickI2cDev->SimplePtrProtocol.WaitForInput != NULL) {
    gBS->CloseEvent(QuickI2cDev->SimplePtrProtocol.WaitForInput);
    gBS->UninstallMultipleProtocolInterfaces (
          QuickI2cDev->ControllerHandle,
          &gEfiSimplePointerProtocolGuid,
          &QuickI2cDev->SimplePtrProtocol,
          NULL
        );
  }
}
