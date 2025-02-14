/** @file
  Intel PTL BEP RTD3 code

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

@par Specification
**/

#include <Include/AcpiDebug.h>

DefinitionBlock (
  "Rtd3.aml",
  "SSDT",
  2,
  "Rtd3",
  "PtlH_Bep",
  0x1000
  )
{
  Store ("[PTL UH LP5 BEP RTD3 SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[PTL UH LP5 BEP RTD3 SSDT][AcpiTableEntry]")

  Include ("PcieRvpRtd3Common.asl")

  External (\_SB.PC00.RP02.PXSX.PNVM, MethodObj)
  External (\_SB.PC00.RP02.PXSX.PAHC, MethodObj)

  //
  // SD card controller  ( RP02)
  // P1PG, 32, // [PcieSlot1PowerEnableGpio ] SD card Power Enable Gpio pin
  // P1EP, 8,  // [PcieSlot1PowerEnableGpioPolarity ] SD card Power Enable Gpio pin polarity
  // P1RG, 32, // [PcieSlot1RstGpio  ] SD card Rest Gpio pin
  // P1RP, 8,  // [PcieSlot1RstGpioPolarity] SD card Rest Gpio pin polarity
  // RW02      // Wake Pin
  //

  //
  // SD card controller
  //
  If (LEqual (P1RN ,0x2)) {
    PCIE_RP_SCOPE_BEGIN(\_SB.PC00.RP02)
    PCIE_RP_SCOPE_BODY(P1PG,P1EP,P1RG,P1RP,RW02,3)
    Include ("PcieRpGenericPcieDeviceRtd3.asl")
    PCIE_RP_SCOPE_END
  }

  //
  // Gen5 M.2 SSD (RP09)
  // SD2P, 32, // [M2Ssd2PowerEnableGpio          ] Pch M.2 SSD Power Enable Gpio pin
  // SDP1, 8,  // [M2Ssd2PowerEnableGpioPolarity  ] Pch M.2 SSD Power Enable Gpio pin polarity
  // SD2R, 32, // [M2Ssd2RstGpio                  ] Pch M.2 SSD Reset Gpio pin
  // SDR1, 8,  // [M2Ssd2RstGpioPolarity          ] Pch M.2 SSD Reset Gpio pin polarity
  //

  PCIE_RP_SCOPE_BEGIN(\_SB.PC00.RP09)
  PCIE_RP_SCOPE_BODY(SD2P,SDP1,SD2R,SDR1,RW09,4)
  Include ("PcieRpGenericPcieDeviceRtd3.asl")
  PCIE_SSD_EP_SCOPE_BEGIN(\_SB.PC00.RP09)
  Include ("PcieRpSsdStorageRtd3Hook.asl")
  PCIE_SSD_EP_SCOPE_END
  PCIE_RP_SCOPE_END

  //
  // PCIe Root Port End
  //

  // GPE handler for 2-tier GPIO Start
  Scope (\_GPE) {
    //
    // AL6F to handle the 2-tier GPIO GPE event
    //
    Method (AL6F) {
      ADBG ("AL6F Start!")
      // SD card controller Wake
      PCIE_RP_L6F_WAKE(\_SB.PC00.RP02)
      ADBG ("AL6F End!")
    }
  }
  // GPE handler for 2-tier GPIO End

  ADBG ("[PTL UH LP5 BEP RTD3 SSDT][AcpiTableExit]")
  Store ("[PTL UH LP5 BEP RTD3 SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // End SSDT
