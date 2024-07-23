/**@file
  A library instance for PEI/DXE phase GpioV2WrapperLib

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

@par Specification
**/
#include <Uefi/UefiBaseType.h>
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/GpioV2AccessLib.h>

#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioV2PcdPins.h>
#include <Register/GpioAcpiDefines.h>

EFI_STATUS
GetGpioV2ServicesFromPad (
  IN  GPIOV2_PAD                GpioPad,
  OUT GPIOV2_SERVICES           **GpioServices
  )
{
  EFI_STATUS              Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, GpioServices);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    DEBUG((DEBUG_ERROR, "Failed to find Gpio Services for GpioPad 0x%x\n", GpioPad));
  }

  return Status;
}
/**
  This procedure will configure all GPIO pads given in PadInitConfigTable
  All pads in PadInitConfigTable should belong to the same ChipsetId.

  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
GpioV2ConfigurePads (
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  if ((GpioPadsConfigTable == NULL) || (GpioPadsConfigTableSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // All pads in PadInitConfigTable should belong to the same ChipsetId.
  //
  Status = GetGpioV2ServicesFromPad (GpioPadsConfigTable[0].GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->ConfigurePads (GpioServices, GpioPadsConfigTable, GpioPadsConfigTableSize);

}
