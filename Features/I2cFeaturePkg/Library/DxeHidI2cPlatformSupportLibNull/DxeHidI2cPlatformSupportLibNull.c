/**@file
  HID-I2C platform support Null library

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
  return FALSE;
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
  return 0xFFFF;
}

