/**@file
  Integrated GbE PTL SOC controller ACPI methods

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

If (LNotEqual (GBES, 0)) {
  Device (GLAN) {
    Name (_ADR, 0x001F0006)

    Name (_S0W, 3) // Device can wake itself from D3 in S0

    Method (_PRW, 0) {
      Return (GPRW (GPE1_GBE_PME_B0, 4)) // can wakeup from S4 state
    }

    Method (_PS3, 0, NotSerialized)
    {
      ADBG("GBE _PS3")
    }

    Method (_PS0, 0, NotSerialized)
    {
      ADBG("GBE _PS0")
    }

    Include ("Gbe.asl")
  }
}
