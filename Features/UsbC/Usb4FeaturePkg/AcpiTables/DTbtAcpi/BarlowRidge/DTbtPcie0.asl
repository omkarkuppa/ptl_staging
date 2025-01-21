/** @file
  ACPI methods of dTBT PCIE port for PCIE tunneling of CIO port 0

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

#include "DTbtSwcmWrapper.h"

External (DTBT_US_PORT.DTID, IntObj)
External (\DTRT, FieldUnitObj)

If (LAnd (CondRefOf (DTBT_US_PORT.DTID), CondRefOf (\DTRT))) {
  //
  // YB observed on root device router after USB4 router hot plug several seconds if _S0W/_PS0/_PS3 are removed
  //
  Method (_S0W, 0, Serialized) {
    If (LEqual (\DTRT, 0)) {
      ADBG (Concatenate("[dTBT] DS PCIE 0 _S0W returns 3 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
      Return (3)
    } Else {
      ADBG (Concatenate("[dTBT] DS PCIE 0 _S0W returns 4 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
      Return (4)
    }
  }

  Method (_PS0,0,Serialized) {
    ADBG (Concatenate ("[dTBT] DS PCIE 0 _PS0 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
  }

  Method (_PS3,0,Serialized) {
    ADBG (Concatenate ("[dTBT] DS PCIE 0 _PS3 - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
  }

  //
  // _DSD package for reporting power management relationship between PCIe DS port and dTBT NHI device
  //
  If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID)) {
    Method (_DSD, 0) {
      ADBG (Concatenate ("[dTBT] DS PCIE 0 _DSD - dTBT ", ToDecimalString (DTBT_US_PORT.DTID)))
      If (LEqual (DTBT_US_PORT.DTID, 0)) {
        Return (
          Package () {
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package() {
              Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI0_DEV},
              Package (2) {"usb4-port-number", 0}
            }
          }
        ) // Return ()
      } Else {
        Return (
          Package () {
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package() {
              Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI1_DEV},
              Package (2) {"usb4-port-number", 0}
            }
          }
        ) // Return ()
      }
    } // Method (_DSD, 0)
  } // If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID))
} Else {
  ADBG ("[dTBT] DTBT_US_PORT.DTID or DTRT NVS not present in DS PCIE 0")
}
