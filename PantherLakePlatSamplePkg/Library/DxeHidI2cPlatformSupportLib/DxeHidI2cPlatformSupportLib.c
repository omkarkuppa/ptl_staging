/**@file
  HID-I2C platform support library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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


#include <Base.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/BaseMemoryLib.h>
#include <SetupVariable.h>

/**
  Initializes selected device

  @param[in]  Handle               Handle of the device

  @retval TRUE      interrupt is active
  @retval FALSE     no interrupt

**/
EFI_STATUS
EFIAPI
HidI2cInitialize (
  IN EFI_HANDLE Handle
  )
{
  //
  // This is the place to add power management or reset sequence if needed
  //
  return EFI_SUCCESS;
}

/**
  Checks if device indicated by Handle has active interrupt

  @param[in]  Handle               Handle of the device whose interrupt is being checked

  @retval TRUE      interrupt is active
  @retval FALSE     no interrupt

**/
BOOLEAN
EFIAPI
HidI2cCheckInterrupt (
  IN EFI_HANDLE Handle
  )
{
  EFI_STATUS         Status;
  GPIOV2_PAD_STATE   PadState;
  static GPIOV2_PAD  sInterruptPad = 0;

  if (sInterruptPad == 0) {
    sInterruptPad = (GPIOV2_PAD) PcdGet32 (PcdTouchpanel1IrqGpio);
  }

  // Handle case when Pcd is not set
  if (sInterruptPad == 0) {
    sInterruptPad = (GPIOV2_PAD) PcdGet32 (PcdTouchpanelIrqGpio);
    if (sInterruptPad == 0) {
      return FALSE;
    }
  }

  //
  // Our platforms support only a single HID device, so no need to determine
  // device based on Handle parameter
  //

  Status = GpioV2GetRx (sInterruptPad, &PadState);
  if (Status != EFI_SUCCESS || PadState != GpioV2StateLow) {
    return FALSE;
  }
  return TRUE;
}


/**
  Returns address of HID Descriptor Register of selected device
  device is identified by Handle. This implementation ignores handle,
  because it's for platforms that have only a single HID device

  @param[in]  Handle               Handle of the device whose HID address is needed.

  @return                          HID descriptor register's address

**/
UINT16
EFIAPI
HidI2cGetHidRegisterAddress (
  IN EFI_HANDLE Handle
  )
{
  SETUP_DATA              SetupData;
  EFI_STATUS              Status;
  UINTN                   Size;

  Size = sizeof (SETUP_DATA);
  ZeroMem (&SetupData, sizeof (SETUP_DATA));
  Status = gRT->GetVariable (L"Setup", &gSetupVariableGuid, NULL, &Size, &SetupData);
  if (EFI_ERROR(Status)) {
    return 0xFFFF;
  }
  switch(SetupData.PchI2cTouchPanelType) {
    case 1: return 0;
    case 2: return 0;
    case 3: return 1;
    case 4: return 1;
    case 5: return 1;
    case 6: return 1;
    case 7: return SetupData.PchI2cTouchPanelHidAddress;
    case 8: return 1;
    default: return 0xFFFF;
  }
}


