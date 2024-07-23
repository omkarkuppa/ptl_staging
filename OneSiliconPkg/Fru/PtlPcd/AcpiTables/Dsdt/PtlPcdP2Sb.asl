/** @file

  PTL SoC P2SB devices description.

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

#include "Defines/PchReservedResources.h"

Scope (\_SB.PC00) {
  Device (SBSC) {
    Name (_HID,EISAID ("PNP0C02")) // motherboard resource
    Name (_UID,"SOCP2SB")
    Name (_STA,0x8) // device present and decodes its resources, but not to be displayed in OSPM
    Name (SBRG, 0)
    Store (\SBRG, SBRG)
    Include ("PcrLib.asl")
  }

  Device (SBIE) {
    Name (_HID,EISAID ("PNP0C02")) // motherboard resource
    Name (_UID,"SECP2SB")
    Name (_STA,0x8) // device present and decodes its resources, but not to be displayed in OSPM
    Name (SBRG, PCH_PCR_SECOND_BASE_ADDRESS)
    Include ("PcrLib.asl")
  }
}
