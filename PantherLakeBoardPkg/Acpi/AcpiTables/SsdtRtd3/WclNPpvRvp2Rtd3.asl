/** @file
  ACPI RTD3 SSDT table for WCL N Lp5 T3 Rvp2

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
    "WclNPpv2",
    0x1000
    )
{
//
// Board configuration WCL N Lp5 T3 PPV Rvp2 CONFIG1
// __________________________________________________________________________________________
// | RP Number | config | End Point                                | ClkSrc# | Lane Reversal |
// __________________________________________________________________________________________|
// |   RP01    |   x4   | CEM Slot 2                               |   4     |  No           |
// |   RP05    |   x1   | CEM Slot 1                               |   2     |  No           |
// |   RP06    |   x1   | Gbe / CEM Slot1 / WWAN (M.2)             |   5     |  No           |
// __________________________________________________________________________________________|
//
  Store ("[Wcl N Ppv Rvp2 RTD3 SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE

  ADBG ("[Wcl N Ppv Rvp2 RTD3 SSDT][AcpiTableEntry]")

  Include ("PcieRvpRtd3Common.asl")


  // PCIe Slot 2 ( RP01)
  // P2RN, 8,      // [PcieSlot2RpNumber       ] Pcie Slot 2 Root Port Number
  // P2PG, 32,     // [PcieSlot2PowerEnableGpio           ] Pcie Slot 2 Power Enable Gpio pin
  // P2EP, 8,      // [PcieSlot2PowerEnableGpioPolarity   ] Pcie Slot 2 Power Enable Gpio pin polarity
  // P2RG, 32,     // [PcieSlot2RstGpio                   ] Pcie Slot 2 Rest Gpio pin
  // P2RP, 8,      // [PcieSlot2RstGpioPolarity           ] Pcie Slot 2 Rest Gpio pin polarity
  // RW01         // Wake Pin

  //
  // Pcie slot 2
  //
  If (LEqual (P2RN ,0x1)) {
    PCIE_RP_SCOPE_BEGIN (\_SB.PC00.RP01)
    PCIE_RP_SCOPE_BODY (P2PG,P2EP,P2RG,P2RP,RW01, 4)
    Include ("PcieRpGenericPcieDeviceRtd3.asl")
    PCIE_RP_SCOPE_END
  }


  //
  // PCIe Slot 1 ( RP05)
  // P1PG, 32, // [PcieSlot1PowerEnableGpio ] Pcie Slot 1 Power Enable Gpio pin
  // P1EP, 8,  // [PcieSlot1PowerEnableGpioPolarity ] Pcie Slot 1 Power Enable Gpio pin polarity
  // P1RG, 32, // [PcieSlot1RstGpio  ] Pcie Slot 1 Rest Gpio pin
  // P1RP, 8,  // [PcieSlot1RstGpioPolarity] Pcie Slot 1 Rest Gpio pin polarity
  // RW05      // Wake Pin
  //

  //
  // Pcie slot 1
  //
  If (LEqual (P1RN ,0x5)) {
    PCIE_RP_SCOPE_BEGIN (\_SB.PC00.RP05)
    PCIE_RP_SCOPE_BODY (P1PG,P1EP,P1RG,P1RP,RW05, 2)
    Include ("PcieRpGenericPcieDeviceRtd3.asl")
    PCIE_RP_SCOPE_END
  }


  Scope (\_GPE) {

    Method (_L14) {  //GPIOV2_WCL_PCD_XXGPP_F_20 RP01
      ADBG("_L14 RP01 SLOT")
      \_SB.SHPO(RW01, 1) // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
      Notify(\_SB.PC00.RP01, 0x02)   // Device wake
      ADBG("_L14 RP01 Slot End")
    }

    //
    // AL6F to handle the 2-tier GPIO GPE event
    //
    Method (AL6F) {
      ADBG ("AL6F Start!")
      //
      // PCIe Slot 1 (x1) Wake
      //
      PCIE_RP_L6F_WAKE(\_SB.PC00.RP05)

      ADBG ("AL6F End!")
    }
  }

  ADBG ("[Wcl N Ppv Rvp2 RTD3 SSDT][AcpiTableExit]")
  Store ("[Wcl N Ppv Rvp2 RTD3 SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // End SSDT
