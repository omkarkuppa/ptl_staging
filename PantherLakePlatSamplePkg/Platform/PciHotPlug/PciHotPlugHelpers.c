/** @file
  Pci Hotplug Helpers Driver : This file will perform specific PCI-EXPRESS
  Devics resource configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include "PciHotPlugHelpers.h"

extern P2PB_RESOURCE_RECORD       mP2pbResourceRecord[MAX_P2PB_OF_TBT_DAISY_CHAIN];
extern NUMBER_OF_HOTPLUG_BRIDGE   mNumOfHotPlugBridge[MAX_P2PB_OF_TBT_DAISY_CHAIN];

/**
  Get the Parent Pci Address

  @param[in] Segment              Segment Number
  @param[in] CurrentRpBus         P2P Bridge Bus Number

  @retval ParentP2PBridgeBase     Parent Pci Address
**/
UINT64
GetParentPciAddress (
  IN  UINT8  Segment,
  IN  UINT64 CurrentRpBus
  )
{
  UINT16  ParentRpBus;
  UINT8   ParentRpDev;
  UINT8   ParentRpFunc;
  UINT64  ParentP2PBridgeBase;
  UINT8   BaseClass;
  UINT8   SubClass;
  UINT8   CapHeaderOffset;
  PCI_REG_PCIE_CAPABILITY  PcieCap;

  ParentRpBus  = 0;
  ParentRpDev  = 0;
  ParentRpFunc = 0;

  for (ParentRpBus = 0; ParentRpBus <= PCI_MAX_BUS; ++ParentRpBus) {
    for (ParentRpDev = 0; ParentRpDev <= PCI_MAX_DEVICE; ++ParentRpDev) {
      for (ParentRpFunc = 0; ParentRpFunc <= PCI_MAX_FUNC; ++ParentRpFunc) {
        ParentP2PBridgeBase = PCI_SEGMENT_LIB_ADDRESS (Segment, ParentRpBus, ParentRpDev, ParentRpFunc, 0);
        if (PciSegmentRead16 (ParentP2PBridgeBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          if (ParentRpFunc == 0) {
            //
            // IF Fun is zero, stop enumerating other functions of the particular bridge
            //
            break;
          }
          continue;
        }
        BaseClass = PciSegmentRead8 (ParentP2PBridgeBase + PCI_CLASSCODE_OFFSET + 2);
        if (BaseClass == PCI_CLASS_BRIDGE) {
          SubClass = PciSegmentRead8 (ParentP2PBridgeBase + PCI_CLASSCODE_OFFSET + 1);
          if (SubClass == PCI_CLASS_BRIDGE_P2P) {
            CapHeaderOffset = PcieFindCapId (Segment, (UINT8) ParentRpBus, ParentRpDev, ParentRpFunc, EFI_PCI_CAPABILITY_ID_PCIEXP);
            PciSegmentReadBuffer (ParentP2PBridgeBase + CapHeaderOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, Capability), sizeof (UINT16), &PcieCap);
            if (PcieCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT) {
              continue;
            }
            if (PciSegmentRead8 (ParentP2PBridgeBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET) == CurrentRpBus) {
              if (PcieCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT) {
                //
                // If the parent is a UPSTREAM_PORT, then it must inherit all resource from grand parent
                // And since UPSTREAM_PORT is not a HotPlug P2PB, so check the resoruce from grand parent.
                //
                ParentP2PBridgeBase = GetParentPciAddress (Segment, ParentRpBus);
                return ParentP2PBridgeBase;
              }
              goto Exit;
            }
          }
        }
      }
    }
  }

  Exit:
  DEBUG ((DEBUG_INFO, "Parent P2PB is Segment 0x%x, Bus 0x%x, Device 0x%x, Function 0x%x \n", Segment, ParentRpBus, ParentRpDev, ParentRpFunc));
  DEBUG ((DEBUG_INFO, "PcieCap.Bits.DevicePortType = 0x%x \n", PcieCap.Bits.DevicePortType));
  ParentP2PBridgeBase = (((UINT64) Segment << 32) | ((UINT64) ParentRpBus << 24) | ((UINT64) ParentRpDev << 16) | ((UINT64) ParentRpFunc << 8));
  return ParentP2PBridgeBase;
}

/**
  Get the Parent Resource

  @param[in]  HpcPciAddress       The address of the Hot Plug Controller function on the PCI bus.
  @param[out] ParentBusNum        Parent Reserved Bus resource Number.
  @param[out] ParentPcieMegaMem   Parent Reserved Alignment of MEM resource.
  @param[out] ParentPciePMegaMem  Parent Reserved Alignment of PMEM resource.
  @param[out] ParentPcieKiloIo    Parent Reserved Pcie Kilo Io Number.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
GetParentResourceSize (
  IN  UINT64 HpcPciAddress,
  OUT UINT8  *ParentBusNum,
  OUT UINT16 *ParentPcieMegaMem,
  OUT UINT32 *ParentPciePMegaMem,
  OUT UINT8  *ParentPcieKiloIo
  )
{
  UINT16  Index;
  UINTN   RpBus;
  UINTN   RpDev;
  UINTN   RpFunc;

  RpBus  = (UINTN) ((HpcPciAddress >> 24) & 0xFF);
  RpDev  = (UINTN) ((HpcPciAddress >> 16) & 0xFF);
  RpFunc = (UINTN) ((HpcPciAddress >> 8) & 0xFF);

  DEBUG ((DEBUG_INFO, "[GetParentResourceSize] Segment 0x%x, Bus 0x%x, Device 0x%x, Function 0x%x \n", ((HpcPciAddress >> 32) & 0x01), RpBus, RpDev, RpFunc));

  for (Index = 0; Index < MAX_P2PB_OF_TBT_DAISY_CHAIN; Index++) {
    if (mP2pbResourceRecord[Index].HpcPciAddress == 0) {
      continue;
    }
    if ((UINT64)HpcPciAddress == (UINT64)mP2pbResourceRecord[Index].HpcPciAddress) {
        *ParentBusNum       = mP2pbResourceRecord[Index].RsvdExtraBusNum;
        *ParentPcieMegaMem  = mP2pbResourceRecord[Index].RsvdPcieMegaMem;
        *ParentPciePMegaMem = mP2pbResourceRecord[Index].RsvdPciePMegaMem;
        *ParentPcieKiloIo   = mP2pbResourceRecord[Index].RsvdPcieKiloIo;
        break;
    }
  }
  return EFI_SUCCESS;
}

/**
  Check the HotPlug Capable of P2P bridge at the same Bus

  @param[in]  Segment             Segment Number
  @param[in]  CurrentRpBus        P2P Bridge Bus Number.
  @param[out] *StrucIndex         Indxe number of NUMBER_OF_HOTPLUG_BRIDGE struct.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid Bus number.
  @retval EFI_ABORTED             No matched P2P Bridge, exit without recorded.
**/
EFI_STATUS
GetCurrentBusP2pbHotPlug (
  IN  UINT8  Segment,
  IN  UINT8  CurrentRpBus,
  OUT UINT8  *StrucIndex
  )
{
  UINT8   CurrentRpDev;
  UINT8   CurrentRpFunc;
  UINT64  CurrentP2PBridgeBase;
  UINT8   BaseClass;
  UINT8   SubClass;
  UINT8   CapHeaderOffset;
  UINTN   Index;
  PCI_REG_PCIE_SLOT_CAPABILITY  PcieSlotCap;

  DEBUG ((DEBUG_INFO, "[GetCurrentBusP2pbHotPlug] Segment 0x%x, Bus 0x%x\n", Segment, CurrentRpBus));
  if ((CurrentRpBus == 0) | (CurrentRpBus > 0xff)) {
    DEBUG ((DEBUG_INFO, "Invalid Bus Number\n"));
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index <= PCI_MAX_BUS; Index++) {
    if (mNumOfHotPlugBridge[Index].P2pbBusNum == 0) {
      for (CurrentRpDev = 0; CurrentRpDev <= PCI_MAX_DEVICE; ++CurrentRpDev) {
        for (CurrentRpFunc = 0; CurrentRpFunc <= PCI_MAX_FUNC; ++CurrentRpFunc) {
          CurrentP2PBridgeBase = PCI_SEGMENT_LIB_ADDRESS (Segment, CurrentRpBus, CurrentRpDev, CurrentRpFunc, 0);
          if (PciSegmentRead16 (CurrentP2PBridgeBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
            if (CurrentRpFunc == 0) {
              //
              // IF Fun is zero, stop enumerating other functions of the particular bridge
              //
              break;
            }
            continue;
          }

          BaseClass = PciSegmentRead8 (CurrentP2PBridgeBase + PCI_CLASSCODE_OFFSET + 2);
          if (BaseClass == PCI_CLASS_BRIDGE) {
            SubClass = PciSegmentRead8 (CurrentP2PBridgeBase + PCI_CLASSCODE_OFFSET + 1);
            if (SubClass == PCI_CLASS_BRIDGE_P2P) {
              mNumOfHotPlugBridge[Index].P2pbSegNum = Segment;
              mNumOfHotPlugBridge[Index].P2pbBusNum = CurrentRpBus;
              // Check HotPlugCapable from PCI_CAPABILITY
              CapHeaderOffset = PcieFindCapId (Segment, (UINT8)CurrentRpBus, CurrentRpDev, CurrentRpFunc, EFI_PCI_CAPABILITY_ID_PCIEXP);
              PciSegmentReadBuffer (CurrentP2PBridgeBase + CapHeaderOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, SlotCapability), sizeof (UINT16), &PcieSlotCap);
              if (PcieSlotCap.Bits.HotPlugCapable) {
                mNumOfHotPlugBridge[Index].NumOfP2pbHotPlug += 1;
                mNumOfHotPlugBridge[Index].P2pbLastDevNum = CurrentRpDev; // Record the last device number in the same layer of Bus
              } else {
                mNumOfHotPlugBridge[Index].NumOfP2pbNonHotPlug += 1;
              }
              *StrucIndex = (UINT8) Index;
            }
          }
        }
      }
      DEBUG ((DEBUG_INFO, "Data structure Index: 0x%x, Number of P2PB with Hot-Plug: %x, Number of P2PB without Hot-Plug: %x\n", *StrucIndex, mNumOfHotPlugBridge[*StrucIndex].NumOfP2pbHotPlug, mNumOfHotPlugBridge[*StrucIndex].NumOfP2pbNonHotPlug));
      return EFI_SUCCESS;
    } else if ((mNumOfHotPlugBridge[Index].P2pbBusNum == CurrentRpBus) && (mNumOfHotPlugBridge[Index].P2pbSegNum == Segment)) {
      *StrucIndex = (UINT8) Index;
      DEBUG ((DEBUG_INFO, "The number of HotPlug Bridge of Bus already recorded in data structure Index: 0x%x\n", *StrucIndex));
      return EFI_SUCCESS;
    }
  }
  return EFI_ABORTED;
}

/**
  Calculate the number of Bus consumed by Root port, Upstream port and Downstream port.

  @param[in]      ParentPciAddress      The address of the Parent device.
  @param[in]      CurrentPciAddress     The address of the Current device.
  @param[in, out] *ConsumedBusNumber    Consumed Bus number.

  @retval         None
**/
VOID
CalculateBusConsumptionBeforeAssign (
  IN      UINT64 ParentPciAddress,
  IN      UINT64 CurrentPciAddress,
  IN OUT  UINT8  *ConsumedBusNumber
  )
{
  UINTN   UpBus;
  UINTN   DpBus;
  UINT8   Segment;
  UINTN   ParentRpBus;
  UINTN   ParentRpDev;
  UINTN   ParentRpFunc;
  UINT64  ParentP2PBridgeBase;

  UpBus = 0;
  DpBus = 0;
  Segment       = (UINT8) ((ParentPciAddress >> 32) & 0x01);
  ParentRpBus   = (UINTN) ((ParentPciAddress >> 24) & 0xFF);
  ParentRpDev   = (UINTN) ((ParentPciAddress >> 16) & 0xFF);
  ParentRpFunc  = (UINTN) ((ParentPciAddress >> 8) & 0xFF);

  ParentP2PBridgeBase = PCI_SEGMENT_LIB_ADDRESS (Segment, ParentRpBus, ParentRpDev, ParentRpFunc, 0);
  UpBus = PciSegmentRead8 (ParentP2PBridgeBase + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  DpBus = (UINTN) ((CurrentPciAddress >> 24) & 0xFF);
  DEBUG ((DEBUG_INFO, "Upstream Port Bus 0x%x and Downstream Port Bus 0x%x consume 2 bus\n", UpBus, DpBus));
  *ConsumedBusNumber = *ConsumedBusNumber + 2;
}

/**
  Check if P2pb device is the last one in the same layer bus.

  @param[in]      HpcPciAddress        The address of the Hot Plug Controller function on the PCI bus.

  @retval TRUE    This P2pb device is the last one.
  @retval FALSE   This P2pb device is not the last one.
**/
BOOLEAN
IsLastP2pbDevice (
  IN      UINT64 HpcPciAddress
  )
{
  UINT8   RpSeg;
  UINTN   RpBus;
  UINTN   RpDev;
  UINTN   Index;

  RpSeg = (UINT8)((HpcPciAddress >> 32) & 0x01);
  RpBus = (UINTN)((HpcPciAddress >> 24) & 0xFF);
  RpDev = (UINTN)((HpcPciAddress >> 16) & 0xFF);

  for (Index = 0; Index <= PCI_MAX_BUS; Index++) {
    if (mNumOfHotPlugBridge[Index].P2pbBusNum != 0) {
      if ((mNumOfHotPlugBridge[Index].P2pbSegNum == RpSeg) && (mNumOfHotPlugBridge[Index].P2pbBusNum == RpBus) && (mNumOfHotPlugBridge[Index].P2pbLastDevNum == RpDev)) {
        return TRUE;
      }
    } else {
      // if P2pbBusNum = 0, the record has ended.
      return FALSE;
    }
  }
  return FALSE;
}