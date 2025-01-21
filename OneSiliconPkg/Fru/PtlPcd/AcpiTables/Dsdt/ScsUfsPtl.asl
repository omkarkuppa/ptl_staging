/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

External(UF0E)
#define PCI_UFS0_ADR                0x00170000

Scope(\_SB.PC00) {

  If (LEqual (UF0E, 1)) {
    //
    // SCS UFS0 (PCI Mode)
    //
    Device(PUF0) {
      Name(_ADR, PCI_UFS0_ADR)
      Name (_DDN, "Intel(R) UFS Controller")
    }
  } //UF0E
}
