/** @file
  ACPI RTD3 Hook SSDT Library for Generic Pcie Rp with End Point as M.2 SSD/Storage Device.

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

//
// This code should be included in the scope of the PCIe root port that is connected to the M.2 slot
//
  External (PAHC, MethodObj) // Checks if the connected device is AHCI drive
  External (PNVM, MethodObj) // Checks if the connected device is NVME drive

  //
  // The deepest D-state supported by this device in the S0 system sleeping state where the device can wake itself,
  // "4" represents D3cold.
  //
  Method (_S0W, 0x0, NotSerialized)
  {
    ADBG_PRINT_PCIE_RP_INFO_AFTER ("_S0W For Child Storage Device Of ")
    If (CondRefOf (^^RD3C)) {
      If (LEqual (^^RD3C, 0x02)) {
        ADBG_PRINT_PCIE_RP_INFO_AFTER ("_S0W - D3 Cold Enable For Child Device of ")
        Return (0x4)
      } ElseIf (LEqual (^^RD3C, 0x00)) {
        ADBG_PRINT_PCIE_RP_INFO_AFTER ("_S0W - D3 Disable For Child Device of ")
        Return (0x0)
      }
    }

    ADBG_PRINT_PCIE_RP_INFO_AFTER ("_S0W - D3 Hot Enable For Child Device of ")
    Return (0x3)
  }

  //
  // PCIe D3Cold support hacks ACPI specification in order to work around bug in Windows PCIe driver.
  // For storage we want to return power resources from root port's _PR0 so that we won't get unexpected D3Cold entry.
  //
  Method (_PR0, Zero, NotSerialized, 0, PkgObj){
    Return (^^_PR0 ())
  }
  Method (_PR3, Zero, NotSerialized, 0, PkgObj){
    Return (^^_PR3 ())
  }

  Method (_PS0, 0, Serialized){
    ADBG_PRINT_PCIE_RP_INFO_AFTER ("Storage Child Device _PS0 for ")
  }

  Method (_PS3, 0, Serialized){
    ADBG_PRINT_PCIE_RP_INFO_AFTER ("Storage Child Device _PS3 for ")
  }
