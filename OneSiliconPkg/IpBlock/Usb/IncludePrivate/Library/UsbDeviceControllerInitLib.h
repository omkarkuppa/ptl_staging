/** @file
  Header file for USB Device Controller initialization library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _USB_DEVICE_CONTROLLER_INIT_LIB_H_
#define _USB_DEVICE_CONTROLLER_INIT_LIB_H_

#include <UsbHandle.h>

/**
  Common entry point for PCH and CPU xDCI controller

  @param[in]  UsbHandle               USB Handle

  @retval     EFI_SUCCESS             Configuration ended successfully
              EFI_INVALID_PARAMETER   UsbHandle was NULL
              EFI_UNSUPPORTED         USB controller was not found
**/
EFI_STATUS
XdciConfigure (
  IN  USB_HANDLE      *UsbHandle
  );

/**
  Used for programming interrupt pin and IRQ number in xDCI controller

  @param[in]  UsbHandle               USB Handle
  @param[in]  InterruptPin            Interrupt pin
  @param[in]  Irq                     IRQ number

  @return     EFI_STATUS              Programmed interrupt in xDCI controller
**/
EFI_STATUS
XdciConfigureInterrupt (
  IN  USB_HANDLE      *UsbHandle,
  IN  UINT8           InterruptPin,
  IN  UINT8           Irq
  );

/**
  Prints USB Device Controller configuration

  @param[in]  UsbHandle               USB_HANDLE structure
**/
VOID
PrintUsbDeviceControllerInformation (
  IN  USB_HANDLE      *UsbHandle
  );

#endif // _USB_DEVICE_CONTROLLER_INIT_LIB_H_
