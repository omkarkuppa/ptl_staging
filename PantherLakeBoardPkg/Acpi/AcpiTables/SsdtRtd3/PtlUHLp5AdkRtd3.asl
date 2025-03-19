/** @file
  Intel PTL ADK RTD3 code

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Include/AcpiDebug.h>

DefinitionBlock (
  "Rtd3.aml",
  "SSDT",
  2,
  "Rtd3",
  "PtlUHAdk",
  0x1000
  )
{
  Store ("[PTL UH LP5 ADK RTD3 SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[PTL UH LP5 ADK RTD3 SSDT][AcpiTableEntry]")

  Include ("PcieRvpRtd3Common.asl")

  //
  // PCIe Root Port Start
  //

  //
  // Gen5 M.2 SSD (RP09)
  // SD2P, 32, // [M2Ssd2PowerEnableGpio          ] Pch M.2 SSD Power Enable Gpio pin
  // SDP1, 8,  // [M2Ssd2PowerEnableGpioPolarity  ] Pch M.2 SSD Power Enable Gpio pin polarity
  // SD2R, 32, // [M2Ssd2RstGpio                  ] Pch M.2 SSD Reset Gpio pin
  // SDR1, 8,  // [M2Ssd2RstGpioPolarity          ] Pch M.2 SSD Reset Gpio pin polarity
  //

  PCIE_RP_SCOPE_BEGIN(\_SB.PC00.RP09)
  PCIE_RP_SCOPE_BODY(SD2P,SDP1,SD2R,SDR1,RW09,1)
  Include ("PcieRpGenericPcieDeviceRtd3.asl")
  PCIE_SSD_EP_SCOPE_BEGIN(\_SB.PC00.RP09)
  Include ("PcieRpSsdStorageRtd3Hook.asl")
  PCIE_SSD_EP_SCOPE_END
  PCIE_RP_SCOPE_END

  //
  // PCIe Root Port End
  //

  //
  // VMD - START
  //
#if (FixedPcdGetBool (PcdVmdEnable) == 1)
  Include ("Rtd3Vmd.asl")
#endif
  //
  // VMD - END
  //

  // GPE handler for 2-tier GPIO Start
  Scope (\_GPE) {

    //
    // For wlan port there is no WAKG variable so checking for RW04 variable
    //
    If (CondRefOf (RW04)) {
      If (LNotEqual(RW04, 0)) {
        Method (_L4C) {
          ADBG("_L4C RP04 SLOT")
          \_SB.SHPO(RW04, 1) // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
          Notify(\_SB.PC00.RP04, 0x02)   // Device wake
        }
      }
    }

    //
    // On Rework the GEN5 M.2 SSD converted to x4 slot
    //

    If (CondRefOf (\_SB.PC00.RP09.WAKG)) {
      If (LNotEqual(\_SB.PC00.RP09.WAKG, 0)) {
        Method (_L39) {
          ADBG("_L39 RP09 SLOT")
          \_SB.SHPO(\_SB.PC00.RP09.WAKG, 1) // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
          Notify(\_SB.PC00.RP09, 0x02)   // Device wake
        }
      }
    }
  }
  // GPE handler for 2-tier GPIO End

  ADBG ("[PTL UH LP5 ADK RTD3 SSDT][AcpiTableExit]")
  Store ("[PTL UH LP5 ADK RTD3 SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // End SSDT
