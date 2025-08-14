/** @file
  Header file of available functions in general USB Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _USB_LIB_H_
#define _USB_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  Disables requested ports through Port Disable Override register programming.
  This is extended function that allows for defining how many lanes are available
  since PDO constructed on per lane basis.

  @param[in]  XhciMmioBase        xHCI Memory BAR0 address
  @param[in]  Usb2DisabledPorts   Disabled ports bitmap with a bit for each USB2 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb3DisabledPorts   Disabled ports bitmap with a bit for each USB3 port
                                  i.e. BIT0 is Port 0, BIT1 is Port 1 etc
  @param[in]  Usb2LanesCount      Number of USB2 lanes
  @param[in]  Usb3LanesCount      Number of USB3 lanes

  @retval EFI_SUCCESS             Programming ended successfully and no errors occured
          EFI_ACCESS_DENIED       Port Disable Override register was locked and write
                                  didn't go through. Platform may require restart to unlock.
**/
EFI_STATUS
UsbDisablePortsEx (
  IN  UINTN   XhciMmioBase,
  IN  UINT32  Usb2DisabledPorts,
  IN  UINT32  Usb3DisabledPorts,
  IN  UINT32  Usb2LanesCount,
  IN  UINT32  Usb3LanesCount
  );

#endif // _USB_LIB_H
