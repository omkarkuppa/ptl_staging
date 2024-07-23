/** @file
  Intel ACPI Sample Code for Discrete Connectivity Modules

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

@par Specification
**/

External (\_SB.SGOV, MethodObj)
External (\CNMT, MutexObj)
External (BRMT, IntObj)
External (FDEL, IntObj)
External (PDEL, IntObj)
External (VDEL, IntObj)
External (RDLY, IntObj)

//
// CNVd backup and retore register while PLDR.
//
External (MSNL, FieldUnitObj)
External (L1C1, FieldUnitObj)
External (L1C2, FieldUnitObj)
External (PTMR, FieldUnitObj)

Name (RSTY, 0)
Name (FLRC, 0) // Function Level Reset
Name (BOFC, 0)
Name (DPRS, 0) // Discrete PLDR Status

PowerResource (WRST, 5, 0)
{
  // Define the PowerResource for PCIe slot
  // PowerResource expects to have _STA, _ON and _OFF Method per ACPI Spec. Not having one of them will cause BSOD

  // Method: Dummy _STA () to comply with ACPI Spec
  Method (_STA)
  {
    ADBG ("Wifi PR _STA")
    Return (1)
  }

  // Method: Dummy _ON () to comply with ACPI Spec
  Method (_ON, 0)
  {
    ADBG ("Wifi PR _ON")
  }

  // Method: Dummy _OFF () to comply with ACPI Spec
  Method (_OFF, 0)
  {
    ADBG ("Wifi PR _OFF")
  }

  // Method: _RST ()
  Method (_RST, 0, NotSerialized)
  {
    If (LEqual (RSTY, 1)) {
      ADBG ("Wifi Product RST")

      If (CondRefOf (\CNMT)) {
        Store (Acquire (\CNMT, 1000), Local0)
      }
      If (LEqual (Local0, Zero)) {                          // Check for Mutex acquired
        If (LEqual (FLRC, 1)) {
          Store (DCTR, Local0)
          Or (Local0, 0x8000, Local0)
          Store (Local0, DCTR)
          If (CondRefOf (FDEL)) {
            Sleep (FDEL)
          }
        }
        Store (1, DPRS)                                     // Core PLDR Completed successfully
        If (LEqual (BOFC, 1)) {                             // Is BT Disabled?
          If (LAnd (CondRefOf (BRMT), LEqual (BRMT, 0))) {  // Is BT RFKILL
            \_SB.BTRK (0)                           // Assert WDISABLE_2#
          } Else {                                          // BT VSEQ DISABLE Interface
            If (LNotEqual (WVHO, 0)) {
              Store (0, BTIE)                               // Clear VSEC WNIC CSR Bit#1
            }
          }
          If (CondRefOf (RDLY)) {
            Sleep (RDLY)
          }
        }
        Store (1, PDRC)                                     // Set VSEC WNIC CSR Bit#8
        If (CondRefOf (VDEL)) {
          Sleep (VDEL)
        }
        If (CondRefOf (\_SB.SGOV)) {
          \_SB.SGOV (WLRT, 0)                               // Assert M.2 Perst#
        }
        If (CondRefOf (PDEL)) {
          Sleep (PDEL)
        }
        If (CondRefOf (\_SB.SGOV)) {
          \_SB.SGOV (WLRT, 1)                               // Deassert WiFi Perst#
        }
        If (LEqual (BOFC, 1)) {                             // Is BT Disabled?
          If (LAnd (CondRefOf (BRMT), LEqual (BRMT, 0))) {  // Is BT RFKILL
            \_SB.BTRK (1)                           // De-Assert WDISABLE_2#
          } Else {
            If (LNotEqual (WVHO, 0)) {
              Store (1, BTIE)                               // Set VSEC WNIC CSR Bit#1
            }
          }
          If (CondRefOf (RDLY)) {
            Sleep (RDLY)
          }
        }
        Store (2, DPRS)                                     // Core PLDR Completed successfully

        If (CondRefOf (\CNMT)) {
          Release (\CNMT)                                   // Unlock Mutex
        }
      }
    } Else {
      ADBG ("Wifi Core RST")
      If (And (DCAP, 0x10000000)) {
        Store (DCTR, Local0)
        Or (Local0, 0x8000, Local0)
        Store (Local0, DCTR)
      }
      Store (1, DPRS) // Core PLDR Completed successfully
    }
    //
    // Restore CNVd PCI Cfg Reg after PLDR
    //
    Sleep (500)
    If (CondRefOf (MSNL)) {
      Store (DR01, MSNL)
      ADBG (Concatenate ("Restore MSNL ", ToDecimalString (MSNL)))
    }
    If (CondRefOf (L1C1)) {
      Store (DR02, L1C1)
      ADBG (Concatenate ("Restore L1C1 ", ToDecimalString (L1C1)))
    }
    If (CondRefOf (L1C2)) {
      Store (DR03, L1C2)
      ADBG (Concatenate ("Restore L1C1 ", ToDecimalString (L1C2)))
    }
    If (CondRefOf (PTMR)) {
      Store (DR04, PTMR)
      ADBG (Concatenate ("Restore L1C1 ", ToDecimalString (PTMR)))
    }
    Sleep (500)
  } // End _RST
} // End WRST

Method (_PRR) {
  Return (Package () {WRST})
}