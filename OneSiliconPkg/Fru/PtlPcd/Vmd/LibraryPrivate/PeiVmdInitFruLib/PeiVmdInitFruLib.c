/** @file
  Fru file for VMD feature initialization.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <PcieRegs.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiVmdInitLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/PcieSipRegs.h>
#include <Library/VmdInfoLib.h>
#include <Register/VmdRegs.h>
#include <Library/PeiVmdInitFruLib.h>

#define VMD_DEVICE_ID        (0xB06F)

/**
  This function enumerate all downstream bridge.

  @param[in] BusNum  - Primary bus number of current bridge.

  @retval BusNum: return current bus number if current bus is an endpoint device.
  @retval SubBus: return subordinate bus number if current bus is a bridge.
**/
UINT8
VmdEnumerateDownstream (
  IN UINT8  BusNum
  )
{
  UINT64  DeviceBaseAddress;
  UINT8   DevNum;
  UINT16  Buffer16;
  UINT8   SubBus;
  UINT8   SecBus;

  SubBus  = 0;

  SecBus  = BusNum;

  for (DevNum = 0; DevNum < 32; DevNum++) {
    ///
    /// Read Vendor ID to check if device exists
    /// if no device exists, then check next device
    ///
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, BusNum, DevNum, 0, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
      continue;
    }

    Buffer16 = PciSegmentRead16 (DeviceBaseAddress + R_PCI_SCC_OFFSET);
    ///
    /// Check for PCI/PCI Bridge Device Base Class 6 with subclass 4
    ///
    if (Buffer16 == 0x0604) {
      SecBus++;
      PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, BusNum);
      PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SecBus);
      ///
      /// Assign temporary subordinate bus number so that device behind this bridge can be seen
      ///
      PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xFF);

      ///
      /// A config write is required in order for the device to re-capture the Bus number,
      /// according to PCI Express Base Specification, 2.2.6.2
      /// Write to a read-only register VendorID to not cause any side effects.
      ///
      PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SecBus, 0, 0, PCI_VENDOR_ID_OFFSET), 0);

      ///
      /// Enumerate bus behind this bridge by calling this function recursively
      ///
      SubBus = VmdEnumerateDownstream (SecBus);
      ///
      /// Update the correct subordinate bus number
      ///
      PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBus);
      SecBus = SubBus;
    }
  }

  if (SubBus == 0) {
    return BusNum;
  } else {
    return SubBus;
  }
}


/**
This function detects the mass storage devices attached to PEG Slots and PCH PCIe Root ports and the Sata controller.

@param[in]      *VmdInfoHob     pointer to VMD info Hob to store the information of detected devices.

@retval EFI_SUCCESS            Successfully initialized Pre-Mem configurations.
**/

EFI_STATUS
VmdDetectPcieStorageDevices (
    VMD_INFO_HOB               *VmdInfoHob
)
{
  UINTN       RpBus;
  UINTN       RpDev;
  UINTN       RpFunc;
  UINT64      RpDeviceBaseAddress;
  UINT64      EpDeviceBaseAddress;
  UINT8       Bus;
  UINT8       Dev;
  UINT8       Func;
  UINT8       MaxFunction;
  UINT8       RpMaxFunction;
  UINT8       NumOfDevices;
  UINT8       HeaderType;
  UINT16      DevId;
  UINT8       SubBusNum;
  UINT16      Buffer16;
  UINTN       NonStorageRpBus;
  UINTN       NonStorageRpDev;
  UINTN       NonStorageRpFunc;

  NumOfDevices         = 0;
  RpBus                = 0;
  NonStorageRpBus      = 0;
  NonStorageRpDev      = 0;
  NonStorageRpFunc     = 0;
  DevId                = 0;

  DEBUG ((DEBUG_INFO, "VMD: Discovering devices on the PCIe root ports \n"));
  // Discovering devices on root bus 0, for the PCie device numbers 6 and 28
  for (RpDev = 6; RpDev <= 28; RpDev++) {
    if (RpDev == 7) {
      RpDev = 28;  // Next PCIe root port device will be Dev 28, so skip enumeration from 7 to 27.
    }

    RpMaxFunction = 7;
      for (RpFunc = 0; RpFunc <= RpMaxFunction; RpFunc++) {
      ///
      /// Check if root port exists
      ///
      RpDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, RpBus, RpDev, RpFunc, 0);
      if (PciSegmentRead16 (RpDeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
        continue;
      }
      DEBUG ((DEBUG_VERBOSE, "VMD: Checking RP B/D/F %d/%d/%d with DID 0x%x\n", RpBus, RpDev, RpFunc, PciSegmentRead16 (RpDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)));
      ///
      /// Temporarily program the secondary and subordinate bus numbers (1, 0xFF) so that devices behind the bridge can be seen
      ///
      Bus = (UINT8) RpBus + 1;
      PciSegmentWrite8 (RpDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, Bus);
      PciSegmentWrite8 (RpDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xFF);
      ///
      /// A config write is required in order for the device to re-capture the Bus number,
      /// according to PCI Express Base Specification, 2.2.6.2
      /// Write to a read-only register VendorID to not cause any side effects.
      ///
      PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, Bus, 0, 0, PCI_VENDOR_ID_OFFSET), 0);
      SubBusNum                       = VmdEnumerateDownstream (Bus);
      ///
      /// Update the actual subordinate bus number
      ///
      PciSegmentWrite8 (RpDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBusNum);
      for (Bus = (UINT8) (RpBus+1); Bus <= SubBusNum; Bus++) {
        for (Dev = 0; Dev < 32; Dev++) {
          ///
          /// Read Vendor ID to check if device exists
          /// if no device exists, then check next device
          ///
          EpDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (0, Bus, Dev, 0, 0);
          if (PciSegmentRead16 (EpDeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
            continue;
          }
          ///
          /// Check for a multifunction device
          ///
          HeaderType = PciSegmentRead8 (EpDeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
          if ((HeaderType & HEADER_TYPE_MULTI_FUNCTION) != 0) {
            MaxFunction = 7;
          } else {
            MaxFunction = 0;
          }
          for (Func = 0; Func <= MaxFunction; Func++) {
            EpDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (0, Bus, Dev, Func, 0);
            if (PciSegmentRead16 (EpDeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
              continue;
            }
            // Check if the device is mass storage device
            if (PciSegmentRead16 (EpDeviceBaseAddress + R_PCI_SCC_OFFSET) == 0x0108) {
              DevId = (PciSegmentRead16 (EpDeviceBaseAddress + PCI_DEVICE_ID_OFFSET));
              if (NumOfDevices && (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpBus == RpBus) && (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpDev == RpDev) &&
                  (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpFunc == RpFunc)) {
                DEBUG ((DEBUG_INFO, "VMD: Another storage device found under root port B/D/F  %d/%d/%d \n", RpBus, RpDev, RpFunc));
                DEBUG ((DEBUG_INFO, "EpDevId 0x%x \n", DevId));
                continue;
              }
              if ((NonStorageRpBus == RpBus) && (NonStorageRpDev == RpDev) && (NonStorageRpFunc == RpFunc)) {
                DEBUG ((DEBUG_INFO, "VMD: Another Non storage device was found under the same root port B/D/F  %d/%d/%d \n", RpBus, RpDev, RpFunc));
                DEBUG ((DEBUG_INFO, "Hence skipping this RP entry from possible VMD mapped root ports\n"));
                continue;
              }
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].DeviceDetected = 1;
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].DevId   = DevId;
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpBus   = (UINT8) RpBus;
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpDev   = (UINT8) RpDev;
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpFunc  = (UINT8) RpFunc;
              VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpIndex = (UINT8)((PciSegmentRead32 (RpDeviceBaseAddress + R_PCIE_CFG_LCAP) & 0xFF000000) >> 24);
              ++NumOfDevices;
              DEBUG ((DEBUG_INFO, "VMD: Storage device found \t" ));
              DEBUG ((DEBUG_INFO, "on B/D/F  %d/%d/%d \t", Bus, Dev, Func));
              DEBUG ((DEBUG_INFO, "under RP B/D/F  %d/%d/%d \t", RpBus, RpDev, RpFunc));
              DEBUG ((DEBUG_INFO, "EpDevId 0x%x \t Rp Index %d\n", DevId, VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpIndex));
            } else {
              DEBUG ((DEBUG_INFO, "VMD: Non Storage device found \t" ));
              DEBUG ((DEBUG_INFO, "on B/D/F  %d/%d/%d \t", Bus, Dev, Func));
              DEBUG ((DEBUG_INFO, "under RP B/D/F  %d/%d/%d \t", RpBus, RpDev, RpFunc));
              DEBUG ((DEBUG_INFO, "EpDevId 0x%x \n", DevId));
              NonStorageRpBus  = RpBus;
              NonStorageRpDev  = RpDev;
              NonStorageRpFunc = RpFunc;
              if (NumOfDevices && (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpBus == RpBus) &&
                  (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpDev == RpDev) &&
                  (VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices-1].RpFunc == RpFunc)) {
                DEBUG ((DEBUG_INFO, "VMD: Another Non storage device found under the same root port B/D/F  %d/%d/%d \n", RpBus, RpDev, RpFunc));
                DEBUG ((DEBUG_INFO, "Hence skipping this RP entry from possible VMD mapped root ports\n"));
                --NumOfDevices;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].DeviceDetected = 0;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].DevId   = 0;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpBus   = 0;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpDev   = 0;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpFunc  = 0;
                VmdInfoHob->VmdPortInfo.PortInfo[NumOfDevices].RpIndex = 0;
                DEBUG ((DEBUG_INFO, "Cleared last entry, Updated NumOfDevices is %d \n", NumOfDevices));
              }
            }
          }
        }
      }
      ///
      /// Clear bus number on all the bridges that we have opened so far.
      /// We have to do it in the reverse Bus number order.
      ///
      for (Bus = SubBusNum; Bus >= (UINT8) (RpBus+1); Bus--) {
        for (Dev = 0; Dev < 32; Dev++) {
          ///
          /// Read Vendor ID to check if device exists
          /// if no device exists, then check next device
          ///
          EpDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (0, Bus, Dev, 0, 0);
          if (PciSegmentRead16 (EpDeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
            continue;
          }
          Buffer16 = PciSegmentRead16 (EpDeviceBaseAddress + R_PCI_SCC_OFFSET);
          ///
          /// Clear Bus Number for PCI/PCI Bridge Device
          ///
          if (Buffer16 == 0x0604) {
            PciSegmentWrite32 (EpDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
          }
        }
      }
      ///
      /// Clear bus numbers so that PCIe slots are hidden
      ///
      PciSegmentWrite32 (RpDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
    }
  } // End of Dev for loop
  return EFI_SUCCESS;
}

/**
  This function detects whether Root Bus1 is supported and if so, programs the additional set of registers for Root Bus 1.
  @param[in]      *VmdInfoHob  pointer to VMD info Hob to read the information of detected devices.
  @param[in, out] *VmConfig    pointer to VmConfig register structure.
**/

VOID
VmdInitFruApi (
  IN      VMD_INFO_HOB *VmdInfoHob,
  IN OUT  VMCONFIG_IOC_VMD_STRUCT *VmConfig
  )
{
  UINT16                                 VmdDid;
  VmdDid = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM,0) + PCI_DEVICE_ID_OFFSET);
  DEBUG((DEBUG_INFO, "VMD DID value is 0x%x\n", VmdDid));
  if (VmdDid != VMD_DEVICE_ID) {
    DEBUG ((DEBUG_INFO, "VMD: Programming DID Assign\n"));
    VmConfig->Bits.did_assign = VMD_DEVICE_ID;
  }

  return;
}
