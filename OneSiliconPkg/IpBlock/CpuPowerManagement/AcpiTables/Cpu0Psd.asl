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
  "CPU0PSD.aml",
  "SSDT",
  0x02,
  "PmRef",
  "Cpu0Psd",
  0x3000
  )
{
  External (\PF00, IntObj)
  External (\TCNT, FieldUnitObj)
  External (\_SB.CFGD, FieldUnitObj)
  External (\_SB.PR00, DeviceObj)
  External (HTRO)
  External (MDID)
  External (SCM0)
  External (SCM1)
  External (BCNT)
  External (DI00)
  External (CM00)


  Scope (\_SB.PR00)
  {
    //
    // Get _PSD
    // Arg0 = CPU Index
    // Arg1 = ModuleID
    // Arg2 = CoreCountInModule
    //
    Method (GPSD, 3)
    {
      Name (HPSD,Package () // HW_ALL
      {
        Package () { 5,     // NumEntries. Current Value is 5.
                    0,      // Revision. Current Value is 0.
                    0,      // Domain.
                    0xFE,   // Coordination type 0xFE = HW_ALL
                    0x80    // Number of processors.
        }
      })

      Name (SPSD,Package () // SW_ALL
      {
        Package () { 5,     // NumEntries. Current Value is 5.
                    0,      // Revision. Current Value is 0.
                    0,      // Domain.
                    0xFC,   // Coordination type 0xFC = SW_ALL
                    0x80    // Number of processors.
        }
      })

      If (LAnd (And (\_SB.CFGD, PPM_TURBO_BOOST_MAX), And (\_SB.CFGD, PPM_HWP)))
      {
        Store (Arg0, Index (DerefOf (Index (HPSD, 0)),2)) // Domain
        Store (1, Index (DerefOf (Index (HPSD, 0)),4))    // Number of processors belonging to the domain.
      } Else {
        //
        // For Hetero Cores, Each module have to be in its seperate domain
        //
        If (HTRO) {
          //
          // Number of processors belonging to the domain.
          //
          Store (Arg2, Index (DerefOf (Index (HPSD, 0)), 4))
          Store (Arg2, Index (DerefOf (Index (SPSD, 0)), 4))

          Store (Arg1, Index (DerefOf (Index (HPSD, 0)), 2)) // Domain
          Store (Arg1, Index (DerefOf (Index (SPSD, 0)), 2)) // Domain
        } Else {
          Store (TCNT, Index (DerefOf (Index (HPSD, 0)), 4))
          Store (TCNT, Index (DerefOf (Index (SPSD, 0)), 4))
        }
      }
      If (And (PF00,0x0800))
      {
        Return (HPSD)
      }
      Return (SPSD)
    }

    Method (_PSD,0)
    {
      Return (GPSD (0, DI00, CM00))
    }
  }
} // End of Definition Block

