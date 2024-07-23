/** @file
  HidI2cPlatformSupportLib provide the interface to initialize touch panel hardware in platform-specific way

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _HID_I2C_PLATFORM_SUPPORT_LIB_H_
#define _HID_I2C_PLATFORM_SUPPORT_LIB_H_

#include <Uefi.h>

//
// There's no decision yet if the 3 functions below should be a lib or protocol
//

EFI_STATUS EFIAPI HidI2cInitialize ( IN EFI_HANDLE Handle);
BOOLEAN EFIAPI HidI2cCheckInterrupt ( IN EFI_HANDLE Handle);
UINT16 EFIAPI HidI2cGetHidRegisterAddress ( IN EFI_HANDLE Handle);

typedef
UINT16
(EFIAPI *HID_I2C_GET_HID_REGISTER_ADDRESS)(
  IN EFI_HANDLE Device
);
typedef
BOOLEAN
(EFIAPI *HID_I2C_CHECK_INTERRUPT)(
  IN EFI_HANDLE Device
);
typedef
EFI_STATUS
(EFIAPI *HID_I2C_INITIALIZE)(
  IN EFI_HANDLE Device
);

typedef struct {
  HID_I2C_GET_HID_REGISTER_ADDRESS   GetHidRegisterAddress;
  HID_I2C_CHECK_INTERRUPT            CheckInterrupt;
  HID_I2C_INITIALIZE                 Initialize;
} HID_I2C_PLATFORM_SUPPORT;

#endif  // _HID_I2C_PLATFORM_SUPPORT_LIB_H_
