/** @file
  The PEIM implements External Gfxcard Detect Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <PcieRegs.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiExtGfxCardDetectLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/ConfigBlockLib.h>
#include <Register/PchRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Defines/HostBridgeDefines.h>
#include <IGpuDataHob.h>
#include <Library/MsrFruLib.h>
#include <Register/HostBridgeRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <ImrConfigHob.h>

#define SA_PEG_DEV_NUM     0x01

/**
  This function enumerate all downstream bridge.

  @param[in] BusNum  - Primary bus number of current bridge.

  @retval BusNum: return current bus number if current bus is an enpoint device.
  @retval SubBus: return subordinate bus number if current bus is a bridge.
**/
UINT8
EnumerateDownstream (
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
      SubBus = EnumerateDownstream (SecBus);
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
  Find the MMIO size that a given PCI device requires

  @param[in] BusNum     - Bus number of the device
  @param[in] DevNum     - device Number of the device
  @param[in] FunNum     - Function number of the device
  @param[out] MmioLength - MMIO Length in bytes
**/
VOID
FindPciDeviceMmioLength (
  IN UINT32  BusNum,
  IN UINT32  DevNum,
  IN UINT32  FunNum,
  OUT UINT32 *MmioLength
  )
{
  UINT32  CurrentMmioLength;
  UINT32  SavedBAR;
  UINT16  SavedCmd;
  UINT32  i;
  UINT8   ClassCode;
  UINT64  DeviceBaseAddress;
  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi;
  HOST_BRIDGE_PREMEM_CONFIG  *HostBridgePreMemConfig;
  EFI_STATUS                 Status;

  *MmioLength = 0;

  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR(Status);

  HostBridgePreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, BusNum, DevNum, FunNum, 0);
  ClassCode         = PciSegmentRead8 (DeviceBaseAddress + R_PCI_BCC_OFFSET);
  if (ClassCode == PCI_CLASS_BRIDGE) {
    return;
  }

  //
  // Save and disable PCI command before change BAR
  //
  SavedCmd = PciSegmentRead16 (DeviceBaseAddress + PCI_COMMAND_OFFSET);
  PciSegmentWrite16 (DeviceBaseAddress + PCI_COMMAND_OFFSET, 0);

  for (i = R_BASE_ADDRESS_OFFSET_0; i <= R_BASE_ADDRESS_OFFSET_5; i += 4) {
    SavedBAR = PciSegmentRead32 (DeviceBaseAddress + i);
    ///
    /// Skip next index if BAR is 64bit address when enable above 4G support.
    ///
    if ((HostBridgePreMemConfig != NULL) && (HostBridgePreMemConfig->EnableAbove4GBMmio == 1)) {
      if ((SavedBAR & (BIT1 + BIT2)) == 0x4) {
        i += 4;
        continue;
      }
    }

    ///
    /// Check BAR is read-only or not
    ///
    PciSegmentAnd32 (DeviceBaseAddress + i, (UINT32) 0);
    PciSegmentOr32  (DeviceBaseAddress + i, (UINT32) MAX_UINT32);
    if (SavedBAR == PciSegmentRead32 (DeviceBaseAddress + i)) {
      ///
      /// Restore BAR as original value
      ///
      PciSegmentWrite32 (DeviceBaseAddress + i, SavedBAR);
      continue;
    }
    ///
    /// If BAR is not memory map, skip it
    ///
    if ((SavedBAR & BIT0) != 0) {
      ///
      /// Restore BAR as original value
      ///
      PciSegmentWrite32 (DeviceBaseAddress + i, SavedBAR);
      continue;
    }
    ///
    /// Calculate the MMIO length through BAR
    ///
    CurrentMmioLength = ~(PciSegmentRead32 (DeviceBaseAddress + i) &~0xF) + 1;
    *MmioLength += CurrentMmioLength;

    ///
    /// Restore BAR as original value
    ///
    PciSegmentWrite32 (DeviceBaseAddress + i, SavedBAR);
  }
  //
  // Restore Command as original value
  //
  PciSegmentWrite16 (DeviceBaseAddress + PCI_COMMAND_OFFSET, SavedCmd);
}

/**
  CheckOffboardPcieVga: Check if off board PCIe graphics Card is present

  @param[in, out] PchPcieMmioLength       - Total PCIe MMIO length on all PCH root ports
  @param[in, out] PrimaryDisplay          - Primary Display - default is IGD
  @param[in]      ResizableBarSupport     - Resizable BAR is support
**/
VOID
CheckOffboardPcieVga (
  IN OUT   UINT32                       *PchPcieMmioLength,
  IN OUT   DISPLAY_DEVICE               *PrimaryDisplay,
  IN       UINT8                        ResizableBarSupport
  )
{
  EFI_STATUS                         Status;
  UINTN                              RpDev;
  UINTN                              RpFunc;
  UINTN                              RpIndex;
  UINT64                             DeviceBaseAddress;
  UINT32                             PcieBusNum;
  UINT8                              Bus;
  UINT8                              Dev;
  UINT8                              Func;
  UINT8                              MaxFunction;
  UINT8                              SubBusNum;
  UINT8                              HeaderType;
  UINT16                             Buffer16;
  BOOLEAN                            CardDetect;
  UINT32                             MmioLength;
  IMR_CONFIG_HOB                     *ImrConfigHob;
  VOID                               *HobPtr;
  IGPU_DATA_HOB                      *IGpuDataHob;

  MmioLength = 0;
  IGpuDataHob = NULL;
  //
  // Get the HOB for Gfx Data
  //
  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "IGPU Data Hob Not Found\n"));
    ASSERT (FALSE);
    return;
  }

  ///
  /// Initialize Secondary and Subordinate bus number for first Pcie root port
  ///
  PcieBusNum  = 0x00010100;

  SubBusNum   = 0;

  CardDetect  = FALSE;

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    ///
    /// Check if root port exists
    ///
    Status = GetPchPcieRpDevFun (RpIndex, &RpDev, &RpFunc);
    ASSERT_EFI_ERROR (Status);

    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDev, (UINT32) RpFunc, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
      continue;
    }

    PciSegmentWrite32 (DeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, PcieBusNum);
    Bus = PciSegmentRead8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);

    ///
    /// Assign temporary subordinate bus number so that device this bridge can be seen
    ///
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xFF);

    ///
    /// A config write is required in order for the device to re-capture the Bus number,
    /// according to PCI Express Base Specification, 2.2.6.2
    /// Write to a read-only register VendorID to not cause any side effects.
    ///
    PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, 0, 0, PCI_VENDOR_ID_OFFSET), 0);

    SubBusNum                       = EnumerateDownstream (Bus);

    ///
    /// Update the actual subordinate bus number
    ///
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBusNum);
    PcieBusNum = (SubBusNum + 1) << 8;
  }

  for (Bus = 1; Bus <= SubBusNum; Bus++) {
    for (Dev = 0; Dev < 32; Dev++) {
      ///
      /// Read Vendor ID to check if device exists
      /// if no device exists, then check next device
      ///
      DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, 0, 0);
      if (PciSegmentRead16 (DeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
        continue;
      }
      ///
      /// Check for a multifunction device
      ///
      HeaderType = PciSegmentRead8 (DeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
      if ((HeaderType & HEADER_TYPE_MULTI_FUNCTION) != 0) {
        MaxFunction = 7;
      } else {
        MaxFunction = 0;
      }

      for (Func = 0; Func <= MaxFunction; Func++) {
        if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, Func, PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
          continue;
        }

        FindPciDeviceMmioLength (Bus, Dev, Func, &MmioLength);
        *PchPcieMmioLength += MmioLength;

        ///
        /// Video cards can have Base Class 0 with Sub-class 1
        /// or Base Class 3.
        ///

        HobPtr = GetFirstGuidHob (&gImrConfigGuid);
        if (HobPtr == NULL) {
          DEBUG ((DEBUG_ERROR, "No IMR_CONFIG HOB\n"));
          return;
        }
        ImrConfigHob = (IMR_CONFIG_HOB*) GET_GUID_HOB_DATA (HobPtr);

        ImrConfigHob->ResizableBarSupport = FALSE;
        if (ResizableBarSupport == 1) {
          ImrConfigHob->ResizableBarSupport = TRUE;
        } else if (((PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, Func, PCI_VENDOR_ID_OFFSET)) == 0x8086) || (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, Func, PCI_VENDOR_ID_OFFSET)) == 0x1002)) && (ResizableBarSupport != 0)) {
          ImrConfigHob->ResizableBarSupport = TRUE;
        }

        if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, Func, R_PCI_SCC_OFFSET)) == 0x0300) {
          if (CardDetect != TRUE) {
            *PrimaryDisplay = DISPLAY_PCI;
            IGpuDataHob->PrimaryDisplayDetection = DISPLAY_PCI;
            DEBUG ((DEBUG_INFO, "PCH PCIe Graphics Card enabled.\n"));
            CardDetect = TRUE;
          }
        }
      }
    }
  }
  ///
  /// Clear bus number on all the bridges that we have opened so far.
  /// We have to do it in the reverse Bus number order.
  ///
  for (Bus = SubBusNum; Bus >= 1; Bus--) {
    for (Dev = 0; Dev < 32; Dev++) {
      ///
      /// Read Vendor ID to check if device exists
      /// if no device exists, then check next device
      ///
      DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, Bus, Dev, 0, 0);
      if (PciSegmentRead16 (DeviceBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
        continue;
      }

      Buffer16 = PciSegmentRead16 (DeviceBaseAddress + R_PCI_SCC_OFFSET);
      ///
      /// Clear Bus Number for PCI/PCI Bridge Device
      ///
      if (Buffer16 == 0x0604) {
        PciSegmentWrite32 (DeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
      }
    }
  }

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    ///
    /// Clear bus numbers so that PCIe slots are hidden
    ///
    Status = GetPchPcieRpDevFun (RpIndex, &RpDev, &RpFunc);
    ASSERT_EFI_ERROR (Status);

    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDev, (UINT32) RpFunc, 0);
    PciSegmentWrite32 (DeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
  }
}
