/** @file
  Integrated XDCI PTL PCD controller ACPI methods

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

@par Specification Reference:
**/

External (XDCE)
External (\ADBG, MethodObj)

Device (XDCI)
{

  Name (_ADR, 0x00140001)  // _ADR: Address
  Name (PPDS, 0)
  Store (\PPDS, PPDS)

  // XDCI Wake Support
  Method (_PRW, 0) {
    If (CondRefOf (GP1E)) {
      If (LNotEqual (GP1E, 0)) {
        Return (GPRW (GPE1_XDCI_PME_B0, 4))
      }
    }
    Return (GPRW (0x6D, 4)) // can wakeup from S4 state
  }

  Include ("Xdci.asl")

  If (CondRefOf (XDCE)) {
    If (LEqual (XDCE, 1)) {
      OperationRegion (GENR, SystemMemory, Add (XDBA (), 0x10F81C), 0x4)  //AON MMIO - 10F81C: APBFC_U3PMU_CFG5
      Field (GENR, WordAcc, NoLock, Preserve)
      {
            ,   2,
        CPME,   1,    //bit2 core_pme_en
        U3EN,   1,    //bit3 u3_pme_en
        U2EN,   1     //bit4 u2_pme_en
      }
      Method (_PS3, 0, NotSerialized)
      {
        Store (1, CPME)
        Store (1, U2EN)
        Store (1, U3EN) 
        ADBG ("XDC _PS3")
      }

      Method (_PS0, 0, NotSerialized)
      {
        Store (0, CPME)
        Store (0, U2EN)
        Store (0, U3EN)
        If (LEqual (DVID, 0xFFFF)) {
          Return ()
        }
        ADBG ("XDC _PS0")
      }

      Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
      {
        Return (0)
      }
    }
  }
}
