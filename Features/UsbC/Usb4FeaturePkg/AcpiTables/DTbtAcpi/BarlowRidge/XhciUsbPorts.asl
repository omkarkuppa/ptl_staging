/** @file
  USB port mapping on Discrete Thunderbolt (TBT) AIC

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

External (\ADBG, MethodObj)
External (\_SB.TPLD, MethodObj)
External (\_SB.TUPC, MethodObj)
External (\_SB.H1UP, MethodObj)
External (\_SB.H2UP, MethodObj)
External (\_SB.J1UP, MethodObj)
External (\_SB.J2UP, MethodObj)
External (\_SB.H1PL, MethodObj)
External (\_SB.H2PL, MethodObj)
External (\_SB.J1PL, MethodObj)
External (\_SB.J2PL, MethodObj)

  Device(HS01)
  {
    Name(_ADR, 0x01)
  } // End of Device(HS01)

  Device(HS02)
  {
    Name(_ADR, 0x02)
  } // End of Device(HS02)

  Device(SS01)
  {
    Name(_ADR, 0x03)
    //
    // _DSD package for reporting power management relationship between SS port and dTBT NHI device
    //
    If (CondRefOf (DTBT_US_PORT.DTID)) {
      ADBG (Concatenate ("[dTBT] DS XHCI SS01 - dTBT ", ToHexString (DTBT_US_PORT.DTID)))
      If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID)) {
        Method (_DSD, 0) {
          ADBG (Concatenate("[dTBT] DS XHCI SS01 _DSD - dTBT ", ToHexString (DTBT_US_PORT.DTID)))
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            Return (
              Package () {
                ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
                Package() {
                  Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI0_DEV},
                  Package (2) {"usb4-port-number", 0}
                }
              }
            ) // End of Return ()
          } Else {
            Return (
              Package () {
                ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
                Package() {
                  Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI1_DEV},
                  Package (2) {"usb4-port-number", 0}
                }
              }
            ) // End of Return ()
          }
        } // End of _DSD ()

        Method (_UPC, 0, Serialized) {
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            If (CondRefOf (\_SB.H1UP)) {
              Return (\_SB.H1UP ())
            }
            Else {
              Return (Package (4) { 0x0, 0xFF, 0x0, 0x0 })
            }
          } Else {
            If (CondRefOf (\_SB.J1UP)) {
              Return (\_SB.J1UP ())
            }
            Else {
              Return (Package (4) { 0x0, 0xFF, 0x0, 0x0 })
            }
          }
        }
        Method (_PLD, 0, Serialized) {
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            If (CondRefOf (\_SB.H1PL)) {
              Return (\_SB.H1PL ())
            }
            Else {
              Return (Package (1) { Buffer (0x14) { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }})
            }
          } Else {
            If (CondRefOf (\_SB.J1PL)) {
              Return (\_SB.J1PL ())
            }
            Else {
              Return (Package (1) { Buffer (0x14) { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }})
            }
          }
        }
      } // If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID))
    } Else {
      ADBG ("[dTBT] DTBT_US_PORT.DTID is not present in XhciUsbPorts!")
    }
  } // End of Device(SS01)

  Device(SS02)
  {
    Name(_ADR, 0x04)
    //
    // _DSD package for reporting power management relationship between SS port and dTBT NHI device
    //
    If (CondRefOf (DTBT_US_PORT.DTID)) {
      ADBG (Concatenate ("[dTBT] DS XHCI SS02 - dTBT ", ToHexString (DTBT_US_PORT.DTID)))
      If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID)) {
        Method (_DSD, 0) {
          ADBG (Concatenate("[dTBT] DS XHCI SS02 _DSD - dTBT ", ToHexString (DTBT_US_PORT.DTID)))
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            Return (
              Package () {
                ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
                Package() {
                  Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI0_DEV},
                  Package (2) {"usb4-port-number", 1}
                }
              }
            ) // End of Return ()
          } Else {
            Return (
              Package () {
                ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
                Package() {
                  Package (2) {"usb4-host-interface", DTBT_US_PORT.TBDH.NHI1_DEV},
                  Package (2) {"usb4-port-number", 1}
                }
              }
            ) // End of Return ()
          }
        } // End of _DSD ()

        Method (_UPC, 0, Serialized) {
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            If (CondRefOf (\_SB.H2UP)) {
              Return (\_SB.H2UP ())
            }
            Else {
              Return (Package (4) { 0x0, 0xFF, 0x0, 0x0 })
            }
          } Else {
            If (CondRefOf (\_SB.J2UP)) {
              Return (\_SB.J2UP ())
            }
            Else {
              Return (Package (4) { 0x0, 0xFF, 0x0, 0x0 })
            }
          }
        }
        Method (_PLD, 0, Serialized) {
          If (LEqual (DTBT_US_PORT.DTID, 0)) {
            If (CondRefOf (\_SB.H2PL)) {
              Return (\_SB.H2PL ())
            }
            Else {
              Return (Package (1) { Buffer (0x14) { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }})
            }
          } Else {
            If (CondRefOf (\_SB.J2PL)) {
              Return (\_SB.J2PL ())
            }
            Else {
              Return (Package (1) { Buffer (0x14) { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }})
            }
          }
        }
      } // If (LLessEqual (DTBT_US_PORT.DTID, MAX_TBT_DTID))
    } Else {
      ADBG ("[dTBT] DTBT_US_PORT.DTID is not present in XhciUsbPorts!")
    }
  } // End of Device(SS02)
