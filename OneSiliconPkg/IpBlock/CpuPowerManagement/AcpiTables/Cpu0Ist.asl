/** @file
  Intel Processor Power Management ACPI Code.

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

#include "CpuPowerMgmt.h"

DefinitionBlock (
  "CPU0IST.aml",
  "SSDT",
  0x02,
  "PmRef",
  "Cpu0Ist",
  0x3000
  )
{
  External (\_SB.OSCP, IntObj)
  External (\TCNT, FieldUnitObj)
  External (\_SB.CPPC, FieldUnitObj)
  External (\_SB.PR00, DeviceObj)

  Scope (\_SB.PR00)
  {
    Method (_PPC, 0)
    {
      Return (\_SB.CPPC) // return max P-state available
    }

    //
    // NOTE:  For CMP systems; this table is not loaded unless
    //        the required driver support is present.
    //        So, we do not check for those cases here.
    //
    // CFGD[0]  = EIST Capable/Enabled
    // PDCx[0]  = OS Capable of Hardware P-State control
    //
    Name (_PCT, Package ()  // Native Mode
    {
        ResourceTemplate () {Register (FfixedHW, 0, 0, 0)},
        ResourceTemplate () {Register (FfixedHW, 0, 0, 0)}
    })

    //
    // Returns a list of supported processor performance states.
    //
    // \_SB.OSCP[10] = Platform-Wide OS Capable for no limit 16 P-states
    //
    Method (_PSS,0)
    {
      If (And (\_SB.OSCP,0x0400))
      {
        Return (TPSS)
      }
      Else
      {
        Return (LPSS)
      }
    }

    //
    // LPSS is for OS to support limit 16 P-states
    //
    Name (LPSS,Package ()
    {
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000}
    })

    //
    // TPSS is for ACPI 6.0 and beyond support 255 states for no limit 16 P-states.
    // Here has limitation that namespace size only could up to 99, over 99 will cause CpuSsdt load fail and P-state no cycling issue.
    //
    Name (TPSS,Package ()
    {
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
      Package () {0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000}
    })
  } // End of Scope (\_SB.PR00)
} // End of Definition Block

