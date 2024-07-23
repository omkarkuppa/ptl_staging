/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include <Include/AcpiDebug.h>

DefinitionBlock (
  "zpodd.aml",
  "SSDT",
  2,
  "AcpiRe",
  "zpodd",
  0x1000
  )
{
  Store ("[zpodd zpodd SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[zpodd zpodd SSDT][AcpiTableEntry]")

  External (\_SB.PC00.SAT0.PRT1, DeviceObj)
  External (\_SB.PC00.SAT0.PRT3, DeviceObj)
  External (RCG0, IntObj)                     // RTD3 Config Setting (BIT0:ZPODD,BIT1:USB Camera Port4, BIT2/3:SATA Port3)
  External (\ZPOD)

  // GPIO methods
  External (\_SB.GGIV, MethodObj)
  External (\_SB.SGOV, MethodObj)
  External (\_SB.GGII, MethodObj)
  External (\_SB.SGII, MethodObj)

  If (LAnd (And (RCG0,1), 1)) {
    Scope (\_SB.PC00.SAT0.PRT1) {
      //
      // _DSM Device Specific Method supporting SATA ZPODD function
      //
      // Arg0: UUID Unique function identifier
      // Arg1: Integer Revision Level
      // Arg2: Integer Function Index
      // Arg3: Package Parameters
      //
      Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
        If (LEqual (Arg0, ToUUID ("bdfaef30-aebb-11de-8a39-0800200c9a66"))) {
          //
          // Switch by function index
          //
          Switch (ToInteger (Arg2)) {
            //
            // Standard query - A bitmask of functions supported
            // Supports function 0-3
            //
            Case (0) {
              Switch (ToInteger (Arg1)) {
                Case (1) {
                  Switch (ToInteger (ZPOD)) {
                    Case ( Package () {1,2,3}) {
                      Return (Buffer () {0xF})
                    }
                    Default {
                      Return (Buffer () {0})
                    }
                  }
                }
                Default {
                  Return (Buffer () {0})
                }
              }
            }

            //
            // Enable ZPODD feature
            //
            Case (1) {
              //
              // Enable Power ON/OFF
              //
              Return (1)
            }

            //
            // Power Off Device
            //
            Case (2) {
              Switch (ToInteger (ZPOD)) {
                Case ( Package () {1}) {
                }

                Case ( Package () {2,3}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to low to power off device.
                  //
                }

              }
              Return (1)
            }

            //
            // Power ON Device
            //
            Case (3) {
              Switch (ToInteger (ZPOD)) {
                Case ( Package () {1}) {

                }

                Case ( Package () {2}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to high to power on device,
                  // only when ODD is detected
                  //
                }

                Case ( Package () {3}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to high to power on device,
                  // only when ODD is detected
                  //
                }

              }
              Sleep (10)
              Return (1)
            }

            Default {
              Return (0)
            }
          }
        } Else {
          Return (Buffer () {0})
        }
      }  // _DSM ()
    } // \_SB.PC00.SAT0.PRT1

    Scope (\_SB.PC00.SAT0.PRT3) {
      //
      // _DSM Device Specific Method supporting SATA ZPODD function
      //
      // Arg0: UUID Unique function identifier
      // Arg1: Integer Revision Level
      // Arg2: Integer Function Index
      // Arg3: Package Parameters
      //
      Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj}) {
        If (LEqual (Arg0, ToUUID ("bdfaef30-aebb-11de-8a39-0800200c9a66"))) {
          //
          // Switch by function index
          //
          Switch (ToInteger (Arg2)) {
            //
            // Standard query - A bitmask of functions supported
            // Supports function 0-3
            //
            Case (0) {
              Switch (ToInteger (Arg1)) {
                Case (1) {
                  Switch (ToInteger (ZPOD)) {
                    Case ( Package () {1,2,3}) {
                      Return (Buffer () {0xF})
                    }

                    Default {
                      Return (Buffer () {0})
                    }
                  }
                }
                Default {
                  Return (Buffer () {0})
                }
              }
            }

            //
            // Enable ZPODD feature
            //
            Case (1) {
              //
              // Enable Power ON/OFF
              //
              Return (1)
            }

            //
            // Power Off Device
            //
            Case (2) {
              Switch (ToInteger (ZPOD)) {
                Case ( Package () {1}) {
                }

                Case ( Package () {2,3}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to low to power off device.
                  //
                }

              }
              Return (1)
            }

            //
            // Power ON Device
            //
            Case (3) {
              Switch (ToInteger (ZPOD)) {
                Case ( Package () {1}) {
                }

                Case ( Package () {2}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to high to power on device,
                  // only when ODD is detected
                  //
                }

                Case ( Package () {3}) {
                  //
                  // Drive GPP_G_1_FAN_TACH_1 to high to power on device,
                  // only when ODD is detected
                  //
                }

              }
              Sleep (10)
              Return (1)
            }

            Default {
              Return (0)
            }
          }
        } Else {
          Return (Buffer () {0})
        }
      }  // _DSM ()
    } // \_SB.PC00.SAT0.PRT3
  }
  ADBG ("[zpodd zpodd SSDT][AcpiTableExit]")
  Store ("[zpodd zpodd SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
}

