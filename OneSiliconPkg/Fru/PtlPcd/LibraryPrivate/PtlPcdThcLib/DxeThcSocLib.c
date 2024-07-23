/** @file
  DXE Thc SoC library.

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

#include <Base.h>
#include <Protocol/ThcResetProtocol.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2AccessLib.h>
#include <GpioV2Services.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Signals.h>

/**
  This function controls output level of THC SPI reset pin.

  @param[in] This         Pointer to the THC_RESET_PROTOCOL instance.
  @param[in] InstanceId   SPI instance id
  @param[in] Value        Output value
                            0: OutputLow, 1: OutputHigh

  @retval     Status
**/
EFI_STATUS
EFIAPI
ThcResetSetOutputValue (
  IN THC_RESET_PROTOCOL  *This,
  IN UINT8               InstanceId,
  IN UINT32              Value
  )
{
  EFI_STATUS       Status;
  GPIOV2_SERVICES  *GpioServices;
  GPIOV2_PAD_STATE GpioPadState;
  GPIOV2_PAD       GpioPad;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioGetThcSpiResetPin (GpioServices, InstanceId, &GpioPad);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GpioPadState = Value ? GpioV2StateHigh : GpioV2StateLow;
  Status = GpioServices->SetTx (GpioServices, GpioPad, GpioPadState);

  return Status;
}
