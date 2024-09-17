 /** @file
  Intel RVP RTD3 common code

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

// Board configuration PTL rvp1  Default topology Rvp1
// ____________________________________________________________________________________________
// | RP Number | config | End Point                                 | ClkSrc# | Lane Reversal |
// ____________________________________________________________________________________________
// |   RP01    |   x1   | GBE LAN                                   |   3     |  No           |
// |   RP02    |   x1   | M.2 WWAN                                  |   5     |  No           |
// |   RP03    |   x1   | x1 PCIe Slot                              |   2     |  No           |
// |   RP04    |   x1   | M.2 WLAN                                  |   4     |  No           |
// |   RP05    |   x4   | Gen4 M.2 SSD                              |   6     |  No           |
// |   RP09    |   x4   | Gen5 M.2 SSD/ X4 pcieslot(Rework))        |   1     |  No           |
// |   RP11    |   x8   | x8 PCIe Slot                              |   0     |  No           |
// ____________________________________________________________________________________________
//
// Board configuration PTL  Default topology Rvp3
// ____________________________________________________________________________________________
// | RP Number | config | End Point                                 | ClkSrc# | Lane Reversal |
// ____________________________________________________________________________________________
// |   RP01    |   x1   | GBE LAN                                   |   3     |  Yes          |
// |   RP02    |   x1   | M.2 WWAN                                  |   5     |  Yes          |
// |   RP03    |   x1   | x2 PCIe Slot                              |   2     |  Yes          |
// |   RP05    |   x4   | Gen4 M.2 SSD                              |   6     |  No           |
// |   RP09    |   x4   | Gen5 M.2 SSD                              |   1     |  No           |
// |   RP11    |   x8   | x8 PCIe Slot (X4 pcie slot rework)        |   0     |  No           |
// |   rp12    |   X4   | X4 pcie slot                              |   7     |  No           |
// ____________________________________________________________________________________________
//

// Board configuration PTL  Default topology Rvp4
// ____________________________________________________________________________________________
// | RP Number | config | End Point                                 | ClkSrc# | Lane Reversal |
// ____________________________________________________________________________________________
// |   RP01    |   x1   | GBE LAN                                   |   3     |  Yes          |
// |   RP02    |   x1   | M.2 WWAN                                  |   5     |  Yes          |
// |   RP03    |   x1   | x2 PCIe Slot                              |   2     |  Yes          |
// |   RP05    |   x4   | Gen4 M.2 SSD                              |   6     |  No           |
// |   RP09    |   x4   | Gen5 M.2 SSD                              |   1     |  No           |
// |   RP11    |   x8   | x8 PCIe Slot                              |   0     |  Yes          |
// ____________________________________________________________________________________________
//
External (RW04)
Include ("Rtd3Common.asl")

//
// WWAN Pins External Reference
//
External (WRTO)
External (WBRS)
External (PBRS)
External (PRST)
External (WPRP)
External (WFCP)
External (PFCP)
External (RW02)
External (WCLK)
External (WWRP)


//
// PCIe Slot 1 (x1 RP03)
//
External (P1RN)
External (P1PG)
External (P1EP)
External (P1RG)
External (P1RP)
External (RW03)
External (RW01)

//
// Gen4 M.2 SSD (x4 RP05)
//
External (SSDP)
External (SDPP)
External (SSDR)
External (SDRP)
External (RW05)
External (\_SB.PC00.RP05.PXSX, DeviceObj)
External (\_SB.PC00.RP05.PXSX.PNVM, MethodObj)
External (\_SB.PC00.RP05.PXSX.PAHC, MethodObj)

//
// Gen5 M.2 SSD (RP09)
//
External (SD2P)
External (SDP1)
External (SD2R)
External (SDR1)
External (RW09)
External (\_SB.PC00.RP09.PXSX, DeviceObj)
External (\_SB.PC00.RP09.PXSX.PNVM, MethodObj)
External (\_SB.PC00.RP09.PXSX.PAHC, MethodObj)

//
// PCIe Slot 2 (x8/x4 RP11)
//
External (P2PG)
External (P2EP)
External (P2RG)
External (P2RP)
External (RW11)
External (RW02)
External (P2RN)

//
// PCIe Slot 2 (x8 RP12)
//
External (P3PG)
External (P3EP)
External (P3RG)
External (P3RP)
External (RW12)
External (P3RN)

#define PCIE_RP_SCOPE_BEGIN(Device_Object) \
  If (CondRefOf (Device_Object)) { \
    Scope (Device_Object) {

#define PCIE_RP_SCOPE_BODY(SlotPowerPin,SlotPowerPolarity,SlotResetPin,SlotResetPolarity,SlotWakePin,Slotclock) \
  Name (RSTG, Package () {0, 0})\
  Store (SlotResetPin, Index (RSTG, 0)) \
  Store (SlotResetPolarity, Index (RSTG, 1)) \
  Name (PWRG, Package () {0, 0}) \
  Store (SlotPowerPin, Index (PWRG, 0)) \
  Store (SlotPowerPolarity, Index (PWRG, 1)) \
  Name (WAKG, 0) \
  Store (SlotWakePin, WAKG) \
  Name (SCLK,0) \
  Store (Slotclock,SCLK) \
  Name (WAKP, 0) \

#define WWAN_RP_SCOPE_BODY(SlotPowerPin,SlotPowerPolarity,SlotResetPin,SlotResetPolarity,SlotWakePin,Slotclock,SlotBResetPin,SlotBResetPolarity) \
  PCIE_RP_SCOPE_BODY(SlotPowerPin,SlotPowerPolarity,SlotResetPin,SlotResetPolarity,SlotWakePin,Slotclock) \
  Name (BRST, Package () {0, 0}) \
  Store (SlotBResetPin, Index (BRST, 0)) \
  Store (SlotBResetPolarity, Index (BRST, 1))

#define PCIE_RP_SCOPE_END } \
  }

#define PCIE_SSD_EP_SCOPE_BEGIN(Device_Object) \
 If (LOr(Device_Object.PXSX.PNVM(),Device_Object.PXSX.PAHC())) { \
  Scope (Device_Object.PXSX) {

#define PCIE_SSD_EP_SCOPE_END } \
 }

#define PCIE_RP_L6F_WAKE(PCIE_SLOT_ROOT_PORT) \
  If (CondRefOf(PCIE_SLOT_ROOT_PORT.WAKG)) { \
    If (LNotEqual(PCIE_SLOT_ROOT_PORT.WAKG, 0)) { \
      If (\_SB.ISME (PCIE_SLOT_ROOT_PORT.WAKG)) { \
        ADBG (Concatenate (" Wake event  ", PCIE_SLOT_ROOT_PORT.WAKG)) \
        \_SB.SHPO (PCIE_SLOT_ROOT_PORT.WAKG, 1) \
        Notify (PCIE_SLOT_ROOT_PORT, 0x02) \
        \_SB.CAGS (PCIE_SLOT_ROOT_PORT.WAKG) \
      } \
    } \
  }
