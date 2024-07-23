/** @file
  Header file for USB4 control fields

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

#ifndef _USB4_CONTROL_H_
#define _USB4_CONTROL_H_

#include <Uefi.h>

//
// USB4 control fields
//
// bit 0 - USB Tunneling
// bit 1 - DisplayPort Tunneling
// bit 2 - PCIe Tunneling
// bit 3 - Inter-Domain USB4 Internet Protocol
// bit 31:4 - Reserved
//
#define USB4_CONTROL_USB3            0x00000001
#define USB4_CONTROL_DP              0x00000002
#define USB4_CONTROL_PCIE            0x00000004
#define USB4_CONTROL_INTER_DOMAIN    0x00000008

#define DEFAULT_USB4_CONTROLS        0x0000000F

typedef union _USB4_CONTROL {
  struct {
    UINT32 Usb3        : 1;
    UINT32 Dp          : 1;
    UINT32 Pcie        : 1;
    UINT32 InterDomain : 1;
  } Fields;
  UINT32 Value;
} USB4_CONTROL, *PUSB4_CONTROL;

#endif
