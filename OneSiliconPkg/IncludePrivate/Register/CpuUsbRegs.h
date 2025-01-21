/** @file
  Register names for TCSS USB devices

  <b>Conventions</b>:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

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

@par Specification Reference:
**/
#ifndef _CPU_USB_REGS_H_
#define _CPU_USB_REGS_H_

//
// TCSS USB3 xHCI related definitions
//
#define XHCI_NORTH_BUS_NUM             0
#define XHCI_NORTH_DEV_NUM             13
#define XHCI_NORTH_FUNC_NUM            0

#endif
