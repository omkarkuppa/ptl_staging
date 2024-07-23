/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

//
// Serial Io Block
//
#include <PcdSerialIoAcpiDefines.h>
Scope (\_SB.PC00) {
  Include ("SerialIoCommon.asl")
  Include ("SerialIoI2cCommon.asl")
  Include ("Fru/PtlPcd/AcpiTables/Dsdt/SerialIoI2cPtlPcd.asl")
  Include ("Fru/PtlPcd/AcpiTables/Dsdt/SerialIoI3cPtlPcd.asl")
  Include ("SerialIoSpiCommon.asl")
  Include ("Fru/PtlPcd/AcpiTables/Dsdt/SerialIoSpiPtlPcd.asl")
  Include ("SerialIoUartCommon.asl")
}
Include ("SerialIoUartCommonHidden.asl")
Include ("Fru/PtlPcd/AcpiTables/Dsdt/SerialIoUartPtlPcd.asl")
Include ("Fru/PtlPcd/AcpiTables/Dsdt/PtlPcdCnvi.asl")
Include ("Fru/PtlPcd/AcpiTables/Dsdt/ScsUfsPtl.asl")
Include ("Fru/PtlPcd/AcpiTables/Dsdt/IsClkPtlPcd.asl")

Include ("Fru/PtlPcd/AcpiTables/Dsdt/GpioPtlPcd.asl")

//
// Touch Host Controllers definition
//
Include ("ThcPcd.asl")

Include ("Fru/PtlPcd/AcpiTables/Dsdt/PtlPcdP2Sb.asl")

//
// Legacy support which redirects to PTL SoC P2SB
//
Scope (\) {
  Include ("PcrLib.asl")
}
