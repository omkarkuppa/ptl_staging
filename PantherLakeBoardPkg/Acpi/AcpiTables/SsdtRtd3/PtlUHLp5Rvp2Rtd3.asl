/** @file
  Intel PTL RVP2 RTD3 code

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
  "Ptl_Rvp2",
  0x1000
  )
{
  Store ("[PTL UH LP5 RVP2 RTD3 SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[PTL UH LP5 RVP2 RTD3 SSDT][AcpiTableEntry]")

  Include ("PcieRvpRtd3Common.asl")

  #define WWAN_PCIE_ROOT_PORT \_SB.PC00.RP02
  External (WWAN_PCIE_ROOT_PORT.PXSX, DeviceObj)
  External (WWAN_PCIE_ROOT_PORT.PXSX.WWST, MethodObj)
  External (WWAN_PCIE_ROOT_PORT.LASX)
  External (\_SB.PC00.RP02.PXSX.PNVM, MethodObj)
  External (\_SB.PC00.RP02.PXSX.PAHC, MethodObj)
  //
  // PCIe Root Port Start
  //
  //
  // M.2 Wwan RP02
  // WRTO, 8,      // [WwanRtd3Option                   ] WWAN RTD3 options
  // WFCP, 32,     // [WwanFullCardPowerOffGpio         ] WWAN Full card power off gpio pin
  // PFCP, 8,      // [WwanFullCardPowerOffGpioPolarity ] WWAN Full card power off gpio pin polarity
  // WBRS, 32,     // [WwanBbrstGpio                    ] WWAN BBRST Gpio pin
  // PBRS, 8,      // [WwanBbrstGpioPolarity            ] WWAN BBRST Gpio pin polarity
  // PRST, 32,     // [WwanPerstGpio                    ] WWAN PERST Gpio pin
  // WPRP, 8,      // [WwanPerstGpioPolarity            ] WWAN PERST Gpio polarity
  // WCLK, 8,      // [WwanSourceClock                  ] WWAN Source Clock
  // WWRP, 8,      // [WwanRootPortNumber               ] WWAN Root Port Nmuber

  //
  // PCIE RTD3 RP 02 - PCIe M.2 CONNECTOR WWAN
  //
  //
  If (LEqual (WWRP,0x2)) {
    If (LNotEqual (WRTO, 0)) {
      PCIE_RP_SCOPE_BEGIN(WWAN_PCIE_ROOT_PORT)
      WWAN_RP_SCOPE_BODY(WFCP,PFCP,PRST,WPRP,RW02,5,WBRS,PBRS)
      Include ("Rtd3PcieWwan.asl")
      PCIE_RP_SCOPE_END
    }
  }
  //
  // PCIe Slot  ( RP03 or RP04)
  // P1PG, 32, // [PcieSlot1PowerEnableGpio ] Pcie Slot 1 Power Enable Gpio pin
  // P1EP, 8,  // [PcieSlot1PowerEnableGpioPolarity ] Pcie Slot 1 Power Enable Gpio pin polarity
  // P1RG, 32, // [PcieSlot1RstGpio  ] Pcie Slot 1 Rest Gpio pin
  // P1RP, 8,  // [PcieSlot1RstGpioPolarity] Pcie Slot 1 Rest Gpio pin polarity
  // RW03      // Wake Pin
  //

  //
  // Pcie slot 1
  //
  If (LEqual (P1RN ,0x3)) {
    PCIE_RP_SCOPE_BEGIN(\_SB.PC00.RP03)
    PCIE_RP_SCOPE_BODY(P1PG,P1EP,P1RG,P1RP,RW03,2)
    Include ("PcieRpGenericPcieDeviceRtd3.asl")
    PCIE_RP_SCOPE_END
  }

  //
  // PCIe x4 Barlow Ridge (RP05)
  // P3PG, 32, // [PcieSlot3PowerEnableGpio ] Pcie Slot 3 Power Enable Gpio pin
  // P3EP, 8,  // [PcieSlot3PowerEnableGpioPolarity ] Pcie Slot 3 Power Enable Gpio pin polarity
  // P3RG, 32, // [PcieSlot3RstGpio  ] Pcie Slot 3 Rest Gpio pin
  // P3RP, 8,  // [PcieSlot3RstGpioPolarity] Pcie Slot 3 Rest Gpio pin polarity
  // RW05      // Wake Pin
  //

  #define DTBT_BR_PCIE_ROOT_PORT \_SB.PC00.RP05
  PCIE_RP_SCOPE_BEGIN(DTBT_BR_PCIE_ROOT_PORT)
  ADBG (Concatenate ("BR PWR EN GPIO = ", ToHexString (P3PG)))
  ADBG (Concatenate ("BR RST GPIO = ", ToHexString (P3RG)))
  ADBG (Concatenate ("BR Wake GPIO = ", ToHexString (RW05)))
  Name (TUID, 0) // TBT RP Number 0 for RP05
  PCIE_RP_SCOPE_BODY(P3PG,P3PG,P3RG,P3RP,RW05,6)
  Include ("Rtd3PcieTbt.asl")
  PCIE_RP_SCOPE_END

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
  // PCIe Slot 2 (x8 RP11)
  // P2PG, 32, // [PcieSlot2PowerEnableGpio           ] Pcie Slot 2 Power Enable Gpio pin
  // P2EP, 8,  // [PcieSlot2PowerEnableGpioPolarity   ] Pcie Slot 2 Power Enable Gpio pin polarity
  // P2RG, 32, // [PcieSlot2RstGpio                   ] Pcie Slot 2 Rest Gpio pin
  // P2RP, 8,  // [PcieSlot2RstGpioPolarity           ] Pcie Slot 2 Rest Gpio pin polarity

  If (LEqual (P2RN ,0xB)) {
    PCIE_RP_SCOPE_BEGIN(\_SB.PC00.RP11)
    PCIE_RP_SCOPE_BODY(P2PG,P2EP,P2RG,P2RP,RW11,0)
    Include ("PcieRpGenericPcieDeviceRtd3.asl")
    PCIE_RP_SCOPE_END
  }

  //
  // PCIe Root Port End
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

    If (CondRefOf (\_SB.PC00.RP11.WAKG)) {
      If (LNotEqual(\_SB.PC00.RP11.WAKG, 0)) {
        Method (_L38) {
          ADBG("_L38 RP11 SLOT")
          \_SB.SHPO (\_SB.PC00.RP11.WAKG, 1) // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
          Notify (\_SB.PC00.RP11, 0x02)   // Device wake
        }
      }
    }

    //
    // AL6F to handle the 2-tier GPIO GPE event
    //
    Method (AL6F) {
      ADBG ("AL6F Start!")
      // WWAN wake
      PCIE_RP_L6F_WAKE(WWAN_PCIE_ROOT_PORT)
      // PCIe Slot 1 (x1) Wake
      PCIE_RP_L6F_WAKE(\_SB.PC00.RP03)
      // PCIe x4 Barlow Ridge
      PCIE_RP_L6F_WAKE(DTBT_BR_PCIE_ROOT_PORT)
      ADBG ("AL6F End!")
    }
  }
  // GPE handler for 2-tier GPIO End

  ADBG ("[PTL UH LP5 RVP2 RTD3 SSDT][AcpiTableExit]")
  Store ("[PTL UH LP5 RVP2 RTD3 SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // End SSDT
