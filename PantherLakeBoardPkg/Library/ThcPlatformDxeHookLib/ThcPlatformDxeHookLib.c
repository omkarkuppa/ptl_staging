/** @file
  ThcPlatformDxe constructor to initialize the PCDs consumed in ThcPlatformDxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <ThcPlatformDxe.h>
#include <ThcBoardData.h>
#include <Library/PchInfoLib.h>
#include <Library/PcdPeiThcSocInitLib.h>
#include <Library/PcdInfoLib.h>

/**
  Return THC Device ID for HID over I2C support

  @param[in] ThcIndex            THC0/THC1 0 or 1
  @retval new Device ID
**/
STATIC
UINT16
PtlPcdSocHidOverI2cDeviceId (
  IN UINT8          ThcIndex
  )
{
  switch (ThcIndex) {
    case 0:
      if (PtlIsPcdP ()) {
        return PTL_PCD_P_I2C_DEVICE_ID_1;
      } else if (IsWclPcd ()) {
        return WCL_PCD_I2C_DEVICE_ID_1;
      } else {
        return PTL_PCD_H_I2C_DEVICE_ID_1;
      }
    case 1:
      if (PtlIsPcdP ()) {
        return PTL_PCD_P_I2C_DEVICE_ID_2;
      } else if (IsWclPcd ()) {
        return WCL_PCD_I2C_DEVICE_ID_2;
      } else {
        return PTL_PCD_H_I2C_DEVICE_ID_2;
      }
    default:
      return 0;
  }
}

/**
  The constructor function to initialize the Thc PCD's consumed in ThcPlatformDxe driver.
  This function creates list of THC Controller device id list, and updates it to Structure PCD
  Structure PCD is consumed by ThcPlatformDxe driver for supported check

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
                           It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
ThcPlatformDxeLibConstructor (
  VOID
  )
{
  UINT8                   Index;
  EFI_STATUS              Status;
  UINTN                   Size;
  THC_HID_OVER_I2C_DEVID  ThcContreollerDevId;

  DEBUG ((DEBUG_INFO, "[%a] Entry\n", __FUNCTION__));

  ThcContreollerDevId.Count = GetPchMaxThcControllersNum ();

  for (Index = 0; Index < ThcContreollerDevId.Count; Index++) {
    ThcContreollerDevId.ThcHidOverI2cDevId[Index].DevId = PtlPcdSocHidOverI2cDeviceId (Index);
    ThcContreollerDevId.ThcHidOverI2cDevId[Index].Index = Index;
  }

  Size = (UINTN) sizeof (THC_HID_OVER_I2C_DEVID);
  //
  // Update the PCD with Device ID of all Serial I2C controller
  //
  Status = PcdSetPtrS (PcdThcQuickI2cDevId, &Size, &ThcContreollerDevId);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: PcdSetPtrS of PcdThcContreollerDevId Status = %r  Size = %x\n", __FUNCTION__, Status, Size));
    // continue
  }

  return EFI_SUCCESS;
}
