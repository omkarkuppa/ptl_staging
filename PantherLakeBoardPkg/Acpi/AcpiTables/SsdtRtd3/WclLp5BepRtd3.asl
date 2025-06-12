/** @file
  ACPI RTD3 SSDT table for WCL Lp5 Bep

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
    "Rtd3.aml",
    "SSDT",
    2,
    "Rtd3",
    "WclN_Bep",
    0x1000
    )
{
//
// Board configuration WCL N Lp5 Bep
// __________________________________________________________________________________________
// | RP Number | config | End Point                                | ClkSrc# | Lane Reversal |
// __________________________________________________________________________________________|
// |   RP01    |   x4   | SSD (M.2)                                |   3     |  No           |
// __________________________________________________________________________________________|
//
  Store ("[Wcl Lp5 Bep RTD3 SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  External (\_SB.PC00.RP01.PXSX, DeviceObj)
  External (\_SB.PC00.RP01.PXSX.PNVM, MethodObj)
  External (\_SB.PC00.RP01.PXSX.PAHC, MethodObj)
  ADBG ("[Wcl Lp5 Bep RTD3 SSDT][AcpiTableEntry]")

  Include ("PcieRvpRtd3Common.asl")

  //
  // M.2 Gen4 SSD Key-M (NIST) (x4 RP01)
  // SSDP, 32, // [PchM2SsdPowerEnableGpio            ] Pch M.2 SSD Power Enable Gpio pin
  // SDPP, 8,  // [PchM2SsdPowerEnableGpioPolarity    ] Pch M.2 SSD Power Enable Gpio pin polarity
  // SSDR, 32, // [PchM2SsdRstGpio                    ] Pch M.2 SSD Reset Gpio pin
  // SDRP, 8,  // [PchM2SsdRstGpioPolarity            ] Pch M.2 SSD Reset Gpio pin polarity
  //

  PCIE_RP_SCOPE_BEGIN (\_SB.PC00.RP01)
  PCIE_RP_SCOPE_BODY (SSDP,SDPP,SSDR,SDRP,RW01,3)
  Include ("PcieRpGenericPcieDeviceRtd3.asl")
  PCIE_SSD_EP_SCOPE_BEGIN (\_SB.PC00.RP01)
  Include ("PcieRpSsdStorageRtd3Hook.asl")
  PCIE_SSD_EP_SCOPE_END
  PCIE_RP_SCOPE_END

  ADBG ("[Wcl Lp5 Bep RTD3 SSDT][AcpiTableExit]")
  Store ("[Wcl Lp5 Bep RTD3 SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // End SSDT
