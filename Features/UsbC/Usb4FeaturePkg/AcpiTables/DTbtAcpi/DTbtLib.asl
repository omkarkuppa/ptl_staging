/** @file
 Thunderbolt ACPI methods

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

#include <TbtMailBoxCmdDefinition.h>
#include <DTbtDefinition.h>

#define CM_MASK_DTBT_0              0x10
#define CM_MASK_DTBT_1              0x20

//
// External reference in OneSiliconPkg
//
External (\_SB.PC00.GPCB, MethodObj)
External (\_SB.PC00.PC2M, MethodObj)
External (\PBSB, FieldUnitObj) // PCH Bus Number

//
// Legacy implementation uses DSCE to indicate FW CM or SW CM running
// 0 = SW CM Disabled (FW CM), 1 = SW CM Enabled
// Now DSCE present can be used to indicate SW CM is running.
// Assign fixed value 1 for backward compatible.
Name (DSCE, 1)

//
// Description: Function to return PCH RP ACPI Address
// Input: Arg0 -> RP number in ACPI name from dTBT NVS
// Return: PCH RP# ACPI Address (Device and Function)
//
Method (PRPA, 1, Serialized)
{
#if FixedPcdGet32 (PcdPchDTbtRpSupport) == 0
  ADBG (Concatenate ("[dTBT] Unsupported PCH RP for querying _ADR - RP ", ToDecimalString (Arg0)))
#else
  Switch (ToInteger (Arg0)) {
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (0) {
      External (PCH_RP_PATH_1 (0)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (0)._ADR)) {
        Return (PCH_RP_PATH_1 (0)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (1) {
      External (PCH_RP_PATH_1 (1)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (1)._ADR)) {
        Return (PCH_RP_PATH_1 (1)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (2) {
      External (PCH_RP_PATH_1 (2)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (2)._ADR)) {
        Return (PCH_RP_PATH_1 (2)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (3) {
      External (PCH_RP_PATH_1 (3)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (3)._ADR)) {
        Return (PCH_RP_PATH_1 (3)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (4) {
      External (PCH_RP_PATH_1 (4)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (4)._ADR)) {
        Return (PCH_RP_PATH_1 (4)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (5) {
      External (PCH_RP_PATH_1 (5)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (5)._ADR)) {
        Return (PCH_RP_PATH_1 (5)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (6) {
      External (PCH_RP_PATH_1 (6)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (6)._ADR)) {
        Return (PCH_RP_PATH_1 (6)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (7) {
      External (PCH_RP_PATH_1 (7)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (7)._ADR)) {
        Return (PCH_RP_PATH_1 (7)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (8) {
      External (PCH_RP_PATH_1 (8)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (8)._ADR)) {
        Return (PCH_RP_PATH_1 (8)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (9) {
      External (PCH_RP_PATH_1 (9)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH_1 (9)._ADR)) {
        Return (PCH_RP_PATH_1 (9)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (10) {
      External (PCH_RP_PATH (10)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (10)._ADR)) {
        Return (PCH_RP_PATH (10)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (11) {
      External (PCH_RP_PATH (11)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (11)._ADR)) {
        Return (PCH_RP_PATH (11)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (12) {
      External (PCH_RP_PATH (12)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (12)._ADR)) {
        Return (PCH_RP_PATH (12)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (13) {
      External (PCH_RP_PATH (13)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (13)._ADR)) {
        Return (PCH_RP_PATH (13)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (14) {
      External (PCH_RP_PATH (14)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (14)._ADR)) {
        Return (PCH_RP_PATH (14)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (15) {
      External (PCH_RP_PATH (15)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (15)._ADR)) {
        Return (PCH_RP_PATH (15)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (16) {
      External (PCH_RP_PATH (16)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (16)._ADR)) {
        Return (PCH_RP_PATH (16)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (17) {
      External (PCH_RP_PATH (17)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (17)._ADR)) {
        Return (PCH_RP_PATH (17)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (18) {
      External (PCH_RP_PATH (18)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (18)._ADR)) {
        Return (PCH_RP_PATH (18)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (19) {
      External (PCH_RP_PATH (19)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (19)._ADR)) {
        Return (PCH_RP_PATH (19)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (20) {
      External (PCH_RP_PATH (20)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (20)._ADR)) {
        Return (PCH_RP_PATH (20)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (21) {
      External (PCH_RP_PATH (21)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (21)._ADR)) {
        Return (PCH_RP_PATH (21)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (22) {
      External (PCH_RP_PATH (22)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (22)._ADR)) {
        Return (PCH_RP_PATH (22)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (23) {
      External (PCH_RP_PATH (23)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (23)._ADR)) {
        Return (PCH_RP_PATH (23)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (24) {
      External (PCH_RP_PATH (24)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (24)._ADR)) {
        Return (PCH_RP_PATH (24)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (25) {
      External (PCH_RP_PATH (25)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (25)._ADR)) {
        Return (PCH_RP_PATH (25)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (26) {
      External (PCH_RP_PATH (26)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (26)._ADR)) {
        Return (PCH_RP_PATH (26)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (27) {
      External (PCH_RP_PATH (27)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (27)._ADR)) {
        Return (PCH_RP_PATH (27)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (28) {
      External (PCH_RP_PATH (28)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (28)._ADR)) {
        Return (PCH_RP_PATH (28)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (29) {
      External (PCH_RP_PATH (29)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (29)._ADR)) {
        Return (PCH_RP_PATH (29)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (30) {
      External (PCH_RP_PATH (30)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (30)._ADR)) {
        Return (PCH_RP_PATH (30)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
    Case (31) {
      External (PCH_RP_PATH (31)._ADR, MethodObj)
      If (CondRefOf (PCH_RP_PATH (31)._ADR)) {
        Return (PCH_RP_PATH (31)._ADR ())
      }
    }
#endif
    Default {
      ADBG (Concatenate ("[dTBT] Unsupported PCH RP for querying _ADR - RP ", ToDecimalString (Arg0)))
    }
  } // Switch
#endif
  Return (0x00FF00FF)
}

//
// Description: Function to return IOE RP ACPI Address
// Input: Arg0 -> RP number in ACPI name from dTBT NVS
// Return: IOE RP# ACPI Address (Device and Function)
//
Method (IRPA, 1, Serialized)
{
#if FixedPcdGet32 (PcdIoeDTbtRpSupport) == 0
  ADBG (Concatenate ("[dTBT] Unsupported IOE RP for querying _ADR - RP ", ToDecimalString (Arg0)))
#else
  Switch (ToInteger (Arg0)) {
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (0) {
      External (IOE_RP_PATH_1 (0)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (0)._ADR)) {
        Return (IOE_RP_PATH_1 (0)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (1) {
      External (IOE_RP_PATH_1 (1)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (1)._ADR)) {
        Return (IOE_RP_PATH_1 (1)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (2) {
      External (IOE_RP_PATH_1 (2)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (2)._ADR)) {
        Return (IOE_RP_PATH_1 (2)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (3) {
      External (IOE_RP_PATH_1 (3)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (3)._ADR)) {
        Return (IOE_RP_PATH_1 (3)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (4) {
      External (IOE_RP_PATH_1 (4)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (4)._ADR)) {
        Return (IOE_RP_PATH_1 (4)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (5) {
      External (IOE_RP_PATH_1 (5)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (5)._ADR)) {
        Return (IOE_RP_PATH_1 (5)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (6) {
      External (IOE_RP_PATH_1 (6)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (6)._ADR)) {
        Return (IOE_RP_PATH_1 (6)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (7) {
      External (IOE_RP_PATH_1 (7)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (7)._ADR)) {
        Return (IOE_RP_PATH_1 (7)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (8) {
      External (IOE_RP_PATH_1 (8)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (8)._ADR)) {
        Return (IOE_RP_PATH_1 (8)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (9) {
      External (IOE_RP_PATH_1 (9)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH_1 (9)._ADR)) {
        Return (IOE_RP_PATH_1 (9)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (10) {
      External (IOE_RP_PATH (10)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (10)._ADR)) {
        Return (IOE_RP_PATH (10)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (11) {
      External (IOE_RP_PATH (11)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (11)._ADR)) {
        Return (IOE_RP_PATH (11)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (12) {
      External (IOE_RP_PATH (12)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (12)._ADR)) {
        Return (IOE_RP_PATH (12)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (13) {
      External (IOE_RP_PATH (13)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (13)._ADR)) {
        Return (IOE_RP_PATH (13)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (14) {
      External (IOE_RP_PATH (14)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (14)._ADR)) {
        Return (IOE_RP_PATH (14)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (15) {
      External (IOE_RP_PATH (15)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (15)._ADR)) {
        Return (IOE_RP_PATH (15)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (16) {
      External (IOE_RP_PATH (16)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (16)._ADR)) {
        Return (IOE_RP_PATH (16)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (17) {
      External (IOE_RP_PATH (17)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (17)._ADR)) {
        Return (IOE_RP_PATH (17)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (18) {
      External (IOE_RP_PATH (18)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (18)._ADR)) {
        Return (IOE_RP_PATH (18)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (19) {
      External (IOE_RP_PATH (19)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (19)._ADR)) {
        Return (IOE_RP_PATH (19)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (20) {
      External (IOE_RP_PATH (20)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (20)._ADR)) {
        Return (IOE_RP_PATH (20)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (21) {
      External (IOE_RP_PATH (21)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (21)._ADR)) {
        Return (IOE_RP_PATH (21)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (22) {
      External (IOE_RP_PATH (22)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (22)._ADR)) {
        Return (IOE_RP_PATH (22)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (23) {
      External (IOE_RP_PATH (23)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (23)._ADR)) {
        Return (IOE_RP_PATH (23)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (24) {
      External (IOE_RP_PATH (24)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (24)._ADR)) {
        Return (IOE_RP_PATH (24)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (25) {
      External (IOE_RP_PATH (25)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (25)._ADR)) {
        Return (IOE_RP_PATH (25)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (26) {
      External (IOE_RP_PATH (26)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (26)._ADR)) {
        Return (IOE_RP_PATH (26)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (27) {
      External (IOE_RP_PATH (27)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (27)._ADR)) {
        Return (IOE_RP_PATH (27)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (28) {
      External (IOE_RP_PATH (28)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (28)._ADR)) {
        Return (IOE_RP_PATH (28)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (29) {
      External (IOE_RP_PATH (29)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (29)._ADR)) {
        Return (IOE_RP_PATH (29)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (30) {
      External (IOE_RP_PATH (30)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (30)._ADR)) {
        Return (IOE_RP_PATH (30)._ADR ())
      }
    }
#endif
#if (FixedPcdGet32(PcdIoeDTbtRpSupport)) != 0
    Case (31) {
      External (IOE_RP_PATH (31)._ADR, MethodObj)
      If (CondRefOf (IOE_RP_PATH (31)._ADR)) {
        Return (IOE_RP_PATH (31)._ADR ())
      }
    }
#endif
    Default {
      ADBG (Concatenate ("[dTBT] Unsupported IOE RP for querying _ADR - RP ", ToDecimalString (Arg0)))
    }
  } // Switch
#endif
  Return (0x00FF00FF)
}

//
// Name: GPRA
// Description: Function to return PCIE RP ACPI Address
// Input: Arg0 -> RP number in ACPI name from dTBT NVS
// Input: Arg1 -> RP type from dTBT NVS
// Return: TBT RP# ACPI Address (Device and Function)
//
Method (GPRA, 2, Serialized)
{
  If (LEqual (Arg1, PCIE_RP_TYPE_PCH)) {
    Return (PRPA (Arg0))
  } ElseIf (LEqual (Arg1, PCIE_RP_TYPE_CPU)) {
    Return (IRPA (Arg0))
  }
  ADBG (Concatenate ("[dTBT] GPRA: Unsupported RP Type for querying _ADR - Type ", ToDecimalString (Arg1)))
  Return (0x00FF00FF)
} // End of Method (GPRA,2)

//
// Name: MMRP
// Description: Function to return the Pci base address of TBT rootport
// Input: Arg0 -> Tbt Root Port value from Tbt NVS
// Input: Arg1 -> Tbt port type value from Tbt NVS
//
Method(MMRP, 2, Serialized)
{
  Store (GPRA (Arg0, Arg1), Local1)    // RP _ADR
  If (CondRefOf (\_SB.PC00.PC2M)) {
    //
    // PC2M returns the system address by providing PCI device BDF
    // PC2M arg0 = PCI _ADR (Dev# in high word | Func# in low word)
    // PC2M arg1 = PCI bus number
    //
    Store (0, Local2)       // Set default PCI bus number to 0
    If (LEqual (Arg1, PCIE_RP_TYPE_PCH)) {
      If (CondRefOf (\PBSB)) {
        Store (\PBSB, Local2)  // Update PCH PCI bus number based on PBSB
      } Else {
        ADBG ("[dTBT] PBSB IS NOT FOUND!!")
      }
    }
    Store (\_SB.PC00.PC2M (Local1, Local2), Local0)
    ADBG (Concatenate ("[dTBT] RP _ADR = ", ToHexString (Local1)))
    ADBG (Concatenate ("[dTBT] RP ECAM address = ", ToHexString (Local0)))
  } Else {
    ADBG ("[dTBT] PC2M method is not present for base address calculation")
    Return (0)
  }
  Return (Local0)
} // End of Method (MMRP)

//
// Name: MMTB
// Description: Function to return the Pci base address of TBT Up stream port
// Input: Arg0 -> Tbt Root Port value from Tbt NVS
// Input: Arg1 -> Tbt port type value from Tbt NVS
//
Method(MMTB, 2, Serialized)
{
  //
  // Get RP base address
  //
  Store (GPRA (Arg0, Arg1), Local1)    // RP _ADR
  ADBG (Concatenate ("[dTBT] DTBT RP _ADR = ", ToHexString (Local1)))

  If (CondRefOf (\_SB.PC00.PC2M)) {
    //
    // Get the system address of DTBT RP
    //
    // PC2M returns the system address by giving PCI device BDF
    // PC2M arg0 = PCI _ADR (Dev# in high word | Func# in low word)
    // PC2M arg1 = PCI bus number
    //
    Store (0, Local2)       // Set default PCI bus number to 0
    If (LEqual (Arg1, PCIE_RP_TYPE_PCH)) {
      If (CondRefOf (\PBSB)) {
        Store (\PBSB, Local2)  // Update PCH PCI bus number based on PBSB
      } Else {
        ADBG ("[dTBT] PBSB IS NOT FOUND!!")
      }
    }
    Store (\_SB.PC00.PC2M (Local1, Local2), Local0)
    ADBG (Concatenate ("[dTBT] RP ECAM address = ", ToHexString (Local0)))
  } Else {
    ADBG ("[dTBT] PC2M method is not present for base address calculation")
    Return (0)
  }

  //
  // Get the bus number of DTBT Upstream Port from OpRegion SBUS field
  //
  OperationRegion (MMMM, SystemMemory, Local0, 0x1A)
  Field (MMMM, AnyAcc, NoLock, Preserve)
  {
    Offset(0x19),
    SBUS, 8
  }

  //
  // Get the system address of DTBT Upstream Port
  // DTBT Upstream Port's _ADR = 0, bus number = SBUS
  //
  Store (\_SB.PC00.PC2M (0, SBUS), Local0)
  ADBG (Concatenate ("[dTBT] Upstream Port ECAM address = ", ToHexString (Local0)))

  Return(Local0)
} // End of Method(MMTB, 1, Serialized)

//
// Name: DTMB
// Description: Send mailbox command to discrete USB4 host router
// Arguments:
// Arg0 - Mailbox command to be sent
// Arg1 - Mailbox argument
// Arg2 - Root port number of discrete USB4 host router
// Arg3 - Root port type of discrete USB4 host router
// Return: 0 = Success. 0xFF = Failure
// Local0 - The PCI base address of upstream port for sending mailbox
// Local1 - Command value set to P2TB register
// Local2 - Command done timeout counter
// Local3 - Command clear timeout counter
//
Method (DTMB, 4, Serialized)
{
  ADBG (Concatenate ("[dTBT] DTMB - MB command  = ", ToHexString (Arg0)))
  ADBG (Concatenate ("[dTBT] DTMB - MB argument = ", ToHexString (Arg1)))
  ADBG (Concatenate ("[dTBT] DTMB - RP number   = ", ToHexString (Arg2)))
  ADBG (Concatenate ("[dTBT] DTMB - RP type     = ", ToHexString (Arg3)))
  Store (MMTB (Arg2, Arg3), Local0)
  ADBG (Concatenate ("[dTBT] HR ECAM address = ", ToHexString (Local0)))
  OperationRegion (DTOP, SystemMemory, Local0, 0x550)
  Field (DTOP, DWordAcc, NoLock, Preserve) {
    DIVI, 32,
    CMDR, 32,
    Offset (0x84),
    TBPS, 2, // PowerState of TBT
    Offset (0x548),
    TB2P, 32,
    P2TB, 32
  }

  ADBG (Concatenate ("[dTBT] TBT HR VDID ", ToHexString (DIVI)))
  If (LNotEqual (DIVI, 0xFFFFFFFF)) {
    ADBG (Concatenate ("[dTBT]   TBT HR Power State ", ToHexString (TBPS)))
    ADBG (Concatenate ("[dTBT]   TBT HR TBT2PCIE(0x548) ", ToHexString (TB2P)))
    ADBG (Concatenate ("[dTBT]   TBT HR PCIE2TBT(0x54C) ", ToHexString (P2TB)))
  } Else {
    ADBG ("[dTBT] TBT is off, abort sending mailbox command")
    Return (0xFF)
  }

  //
  // Send mailbox command via P2TB
  // Bit 0    - Valid
  // Bit 7:1  - Mailbox command
  // Bit 31:8 - Command argument
  //
  Or (Arg0, PCIE2TBT_VLD_B, Local1)
  Or (Local1, ShiftLeft (Arg1, 8), Local1)
  Store (Local1, P2TB)
  //
  // Wait for command done bit set
  //
  Store (SET_CM_TIMEOUT_IN_MS, Local2)
  While (LGreater (Local2, 0)) {
    If (And (TB2P, TBT2PCIE_DON_R)) {
      ADBG (Concatenate ("[dTBT] TB2P cmd done bit is set, wait count = ", ToHexString (Subtract (SET_CM_TIMEOUT_IN_MS, Local2))))
      Break
    }
    Decrement (Local2)
    Sleep (1)
  }
  If (LEqual (Local2, 0)) {
    ADBG (Concatenate ("[dTBT] Command Timeout, timeout value = ", SET_CM_TIMEOUT_IN_MS))
  }

  //
  // Clear command
  //
  Store (0, P2TB)
  //
  // Wait for command done bit clear
  //
  Store (SET_CM_TIMEOUT_IN_MS, Local3)
  While (LGreater (Local3, 0)) {
    If (LEqual (And (TB2P, TBT2PCIE_DON_R), 0)) {
      ADBG (Concatenate ("[dTBT] TB2P cmd done bit is cleared, wait count = ", ToHexString (Subtract (SET_CM_TIMEOUT_IN_MS, Local3))))
      Break
    }
    Decrement (Local3)
    Sleep (1)
  }
  If (LEqual (Local3, 0)) {
    ADBG (Concatenate ("[dTBT] Command clear timeout, timeout value = ", SET_CM_TIMEOUT_IN_MS))
  }

  //
  // Return cmd sent success if command done bit is set for mailbox command sent
  //
  If (LGreater (Local2, 0)) {
    Return (0)
  } Else {
    Return (0xFF)
  }
}

//
// Name: DSCL
// Description: Send Set Class Code command to the discrete host router
// Arg0 - DTbt host router index
// Arg1 - Set Class Code command argument
// Return: None
//
Method (DSCL, 2, Serialized)
{
  ADBG (Concatenate ("[dTBT] Send Set Class Code command - dTBT", ToDecimalString (Arg0)))
  ADBG (Concatenate ("[dTBT] Set Class Code command argument - ", ToHexString (Arg1)))

  If (LEqual (Arg0, 0)) {
    Store (MMTB (RPS0, RPT0), Local0)
  } ElseIf (LEqual (Arg0, 1)) {
    Store (MMTB (RPS1, RPT1), Local0)
  }
  Add (Local0, 0x00200000, Local0)  // advance 2 bus to locate NHI device
  ADBG (Concatenate ("[dTBT] DTBT NHI address = ", ToHexString (Local0)))
  OperationRegion (CLOP, SystemMemory, Local0, 0x0c)
  Field (CLOP, DWordAcc, NoLock, Preserve) {
    DIVI, 32,  // Device ID & Vendor ID
    CMDR, 32,  // Status & Command
    CLRV, 32   // Class Code & Revision ID
  }
  ADBG (Concatenate ("[dTBT] DIVI - ", ToHexString (DIVI)))
  ADBG (Concatenate ("[dTBT] CLRV - ", ToHexString (CLRV)))
  If (LEqual (Arg0, 0)) {
    DTMB (PCIE2TBT_SET_CLASS_CODE, Arg1, RPS0, RPT0)
  } ElseIf (LEqual (Arg0, 1)) {
    DTMB (PCIE2TBT_SET_CLASS_CODE, Arg1, RPS1, RPT1)
  }
  ADBG (Concatenate ("[dTBT] DIVI - ", ToHexString (DIVI)))
  ADBG (Concatenate ("[dTBT] CLRV - ", ToHexString (CLRV)))
}

//
// Name: DUCL
// Description: Send Set Class Code command to the discrete host router if it supports USB4 CM Version 2.
//              Command argument is determined by USB4 Class Code setup option and OS Supported USB4 Version.
// Arg0 - OS Supported USB4 Version. (0 - USB4 Ver1, 1 - USB4 Ver2)
// Return: None
// Local0: Argument value for Set Class Code command.
//
Method (DUCL, 1, Serialized)
{
  ADBG (Concatenate ("[dTBT] DUCL - OSPM USB4 supported field = ", ToHexString (Arg0)))

  If (LGreater (Arg0, 1)) {
    //
    // 0x0F is also observed in boot sequence of current OS implementation.
    // OS Supported USB4 Version values other than 1 will be identified as USB4 Ver1.
    //
    ADBG ("[dTBT] DUCL - Invalid OSPM USB4 supported field!")
  }

  If (CondRefOf (U4CL)) {
    ADBG (Concatenate ("[dTBT] DUCL - U4CL = ", ToHexString (U4CL)))
    If (LGreater (U4CL, 2)) {
      ADBG ("[dTBT] DUCL - Invalid USB4 class code option!")
      Return ()
    }
  } Else {
    ADBG ("[dTBT] DUCL - U4CL Is not present!")
    Return ()
  }

  //
  // Set argument value for Set Class Code command
  // (1) USB4 class code option (U4CL) is 0 (Auto):
  //     Command Argument = 2 if OSPM USB4 supported field == 1 (OS support USB4 Ver2, Inbox HRD driver loaded)
  //     Otherwise, Command Argument = 1 (OS doesn't support USB4 Ver2, Intel USB4 Ver2 driver loaded)
  // (2) USB4 class code option (U4CL) is 1 (Intel USB4 Ver2):
  //     Command Argument = 1
  // (3) USB4 class code option (U4CL) is 2 (Inbox HRD):
  //     Command Argument = 2
  //
  Store (1, Local0)
  If (LEqual (U4CL, 0)) {
    If (LEqual (Arg0, 1)) {
      Store (2, Local0)
    }
  } ElseIf (LEqual (U4CL, 2)) {
    Store (2, Local0)
  }

  //
  // Send Set Class Code command to dTBT 0 if dTBT 0 is enabled
  //
  If (LEqual (RPN0, 1)) {
    ADBG (Concatenate ("[dTBT] Set dTBT0 Class Code, Arg = ", ToHexString (Local0)))
    DSCL (0, Local0)
  }
  //
  // Send Set Class code command to dTBT 1 if dTBT 1 is enabled
  //
  If (LEqual (RPN1, 1)) {
    ADBG (Concatenate ("[dTBT] Set dTBT1 Class Code, Arg = ", ToHexString (Local0)))
    DSCL (1, Local0)
  }
}
