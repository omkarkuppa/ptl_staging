/** @file
  Integrated XHCI PTL SOC controller ACPI methods

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

Device (XHCI) {

  Name (_ADR, 0x00140000)
  Name (DUWS, 0)
  Name (PU2C, 0)
  Name (PU3C, 0)
  Name (AHWA, 1) // Advertise HSII workaround to OS driver

  Store (\DUWS, DUWS)
  Store (\PU2C, PU2C)
  Store (\PU3C, PU3C)

  Method (_PRW, 0) {
    If (CondRefOf (DUWS)) {
      If (CondRefOf (GP1E)){
        If (LNotEqual (GP1E, 0)) {
          Return (GPRW (GPE1_XHCI_PME_B0, DUWS))
        }
      }
      Return (GPRW (0x6D, DUWS)) // can wakeup from deepest USB Sleep Wake State
    } Else {
      If (CondRefOf (GP1E)){
        If (LNotEqual (GP1E, 0)) {
          Return (GPRW (GPE1_XHCI_PME_B0, 4))
        }
      }
      Return (GPRW (0x6D, 4)) // can wakeup from S4 state
    }
  }

  Include ("PchXhci.asl")
}
