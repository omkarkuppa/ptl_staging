/** @file
  Header file for USB2 PHY programming library.

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

#ifndef _USB2_PHY_LIB_H_
#define _USB2_PHY_LIB_H_

#include <UsbHandle.h>

/**
  Tune the USB 2.0 high-speed signals quality of USB2PHY version 4.4

  @param[in]  UsbHandle                USB Handle
  @param[in]  SbMsgAccess              P2SB Register Access to USB2
**/
VOID
Usb2Phy4p4Programming (
  IN  USB_HANDLE                      *UsbHandle,
  IN  REGISTER_ACCESS                 *SbMsgAccess
  );

/**
  Tune the USB 2.0 high-speed signals quality.

  @param[in]  UsbHandle                USB Handle
  @param[in]  SbMsgAccess              P2SB Register Access to USB2
**/
VOID
Usb2PhyProgramming (
  IN  USB_HANDLE                      *UsbHandle,
  IN  REGISTER_ACCESS                 *SbMsgAccess
  );

#endif // _USB2_PHY_LIB_H_
