/**@file
  isCLK ASL code.

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

#include "Register/PchRegsClk.h"

//
// Variables and definitions required by this file
// PID - variable holding isCLK sideband PID
// P2SB_DEV - define holding path to sideband device this isCLK can be accessed from.
//

//
// PCIe(100MHz) clock disable
// Arg0 - clock index
//
Method (CLKD, 1) {
  P2SB_DEV.PCRA (PID, R_ISCLK_PCR_BIOS_BUFFEN_H, Not (ShiftLeft (1, Arg0)))
}

//
// PCIe(100MHz) clock enable
// Arg0 - clock index
//
Method (CLKE, 1) {
  P2SB_DEV.PCRO (PID, R_ISCLK_PCR_BIOS_BUFFEN_H, (ShiftLeft (1, Arg0)))
}