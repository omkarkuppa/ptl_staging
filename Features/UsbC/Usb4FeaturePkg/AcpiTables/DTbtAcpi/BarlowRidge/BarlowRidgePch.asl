/** @file
  ACPI implementation of BarlowRidge on PCH

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

//
// Macro used to generate RP ACPI name with associated number
// DTBT_RP_NAME_NUM should be defined before include this file
// For RP number < 10, 0 may be inserted before RP number in RP ACPI name
// For example, RP01.
// If 0 should not be inserted in ACPI name, then PCH_RP_PREFIX_NO_ZERO should be defined.
//
#define GET_NAME(N)       N
#define RP_NAME_NUM       GET_NAME(DTBT_RP_NAME_NUM)

//
// Macros for Root port path
//
#if DTBT_RP_NAME_NUM < 10
#define DTBT_ROOT_PORT    \_SB.PCH_RP_PC.PCH_RP_NAME_1(RP_NAME_NUM)
#else
#define DTBT_ROOT_PORT    \_SB.PCH_RP_PC.PCH_RP_NAME(RP_NAME_NUM)
#endif

#define DTBT_US_PORT      DTBT_ROOT_PORT.PCH_RP_UPORT
#define DTBT_RP_INDEX     DTBT_RP_NAME_NUM

External (DTBT_US_PORT, DeviceObj)
External (DTBT_US_PORT.DTID, IntObj)

//
// Identify dTBT index
//
Scope (DTBT_US_PORT) {
  Name (DTID, 0)  // dTBT ID used within AFP

  If (LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, DTBT_RP_NAME_NUM))) {
    ADBG (Concatenate ("[dTBT] Include BarlowRidge as dTBT 0 at PCH ", TOSTR (\DTBT_ROOT_PORT)))
  }
  If (LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, DTBT_RP_NAME_NUM))) {
    Store (1, DTID)
    ADBG (Concatenate ("[dTBT] Include BarlowRidge as dTBT 1 at PCH ", TOSTR (\DTBT_ROOT_PORT)))
  }
}

Include("BarlowRidge.asl")

//
// Undef the defined macro when they are not required
// These macros are defined per DTBT_RP_NAME_NUM value
// They will be re-defined for different DTBT_RP_NAME_NUM value
//
#undef GET_NAME
#undef RP_NAME_NUM
#undef DTBT_ROOT_PORT
#undef DTBT_US_PORT
#undef DTBT_RP_INDEX