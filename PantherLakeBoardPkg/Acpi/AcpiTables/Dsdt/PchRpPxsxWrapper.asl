/** @file
  Intel ACPI Reference Code for Pch Rootport Endpoint Wrapper

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

Include ("Gmio.asl")                  // Provided by CnvFeaturePkg

Scope (\_SB.PC00.RP01.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP01
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}

Scope (\_SB.PC00.RP02.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP02
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}

Scope (\_SB.PC00.RP03.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP03
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}

Scope (\_SB.PC00.RP04.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP04
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}

Scope (\_SB.PC00.RP05.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP05
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}

Scope (\_SB.PC00.RP06.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP06
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
}
