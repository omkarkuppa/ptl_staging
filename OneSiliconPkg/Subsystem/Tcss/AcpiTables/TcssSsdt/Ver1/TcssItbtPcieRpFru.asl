/** @file
  This file contains the TCSS PCIe Root Port configuration

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

Scope (TRP0) {
  #define GPE0_EN GPE_TC_PCI_EXP
  #define GPE1_EN GPE1_TC_PCI0_PCI_EXP
  Include ("TcssPrwWrapper.asl")

  Device (PXSX)
  {
    Name (_ADR, 0x00000000)
    Include ("TcssPrwWrapper.asl")
  }

  #undef GPE1_EN
  #undef GPE0_EN
}

Scope (TRP1) {
  #define GPE0_EN GPE_TC_PCI_EXP
  #define GPE1_EN GPE1_TC_PCI1_PCI_EXP
  Include ("TcssPrwWrapper.asl")

  Device (PXSX)
  {
    Name (_ADR, 0x00000000)
    Include ("TcssPrwWrapper.asl")
  }

  #undef GPE1_EN
  #undef GPE0_EN
}

Scope (TRP2) {
  #define GPE0_EN GPE_TC_PCI_EXP
  #define GPE1_EN GPE1_TC_PCI2_PCI_EXP
  Include ("TcssPrwWrapper.asl")

  Device (PXSX)
  {
    Name (_ADR, 0x00000000)
    Include ("TcssPrwWrapper.asl")
  }

  #undef GPE1_EN
  #undef GPE0_EN
}

Scope (TRP3) {
  #define GPE0_EN GPE_TC_PCI_EXP
  #define GPE1_EN GPE1_TC_PCI3_PCI_EXP
  Include ("TcssPrwWrapper.asl")

  Device (PXSX)
  {
    Name (_ADR, 0x00000000)
    Include ("TcssPrwWrapper.asl")
  }

  #undef GPE1_EN
  #undef GPE0_EN
}