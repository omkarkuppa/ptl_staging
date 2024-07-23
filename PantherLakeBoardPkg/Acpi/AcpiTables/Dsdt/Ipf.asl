 /** @file
  Intel ACPI Reference Code for Intel Platform Framework

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#define IETM_PARTICIPANT_HID           "INTC10D4"
#define LEDGER_ISLAND_PARTICIPANT_HID  "INTC10D5"


Scope (\_SB) {
  Device (IETM) {
    //
    // Intel Platform Framework Device
    //
    Name (_UID, "IETM")
    Name (_HID, IETM_PARTICIPANT_HID)

    Method (_STA) {
      If (LEqual (IPFE,1)) {
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    //
    // LHID (Ledger Island HID)
    //
    // This method returns the HID value of the Ledger Island GPD1 device
    Method (LHID) {
      Return (LEDGER_ISLAND_PARTICIPANT_HID)
    }
  }
}