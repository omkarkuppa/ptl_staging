/** @file
  Header file for PCI Express helpers base library

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
#ifndef _PCIE_HELPER_LIB_H_
#define _PCIE_HELPER_LIB_H_

#include <PcieRegs.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci.h>

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] DeviceBase           device's base address
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieBaseFindCapId (
  IN UINT64  DeviceBase,
  IN UINT8   CapId
  );

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Segment              Pci Segment Number
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] DeviceBase           device base address
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found, this includes situation where device doesn't exist
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieBaseFindExtendedCapId (
  IN UINT64  DeviceBase,
  IN UINT16  CapId
  );

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Rreporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Segment              Pci Segment Number
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  );

/*
  Checks device's Slot Clock Configuration

  @param[in] Base            device's base address
  @param[in] PcieCapOffset   devices Pci express capability list register offset

  @retval TRUE when device device uses slot clock, FALSE otherwise
*/
BOOLEAN
GetScc (
  UINT64    Base,
  UINT8     PcieCapOffset
  );

/*
  Sets Common Clock Configuration bit for given device.
  @param[in] PcieCapOffset   devices Pci express capability list register offset
  @param[in] Base            device's base address
*/
VOID
EnableCcc (
  UINT64    Base,
  UINT8     PcieCapOffset
  );

/**
  Checks the negotiated PCIe link speed.

  @param[in]  Base    device's base address
  @param[out] LnkSts  pointer to the register value with link speed mask applied. Valid values are:
                        0001b: 2.5 Gb/s PCI Express Link
                        0010b: 5.0 Gb/s PCI Express Link
                        0011b: 8.0 Gb/s PCI Express Link
                        Other: Link speed NOT found, this includes situation where device doesn't exist

  @retval TRUE   valid link width
          FALSE  link width NOT found, this includes situation where device doesn't exist
**/
BOOLEAN
GetPcieLinkSpeed (
  IN  UINT64           Base,
  OUT UINT16           *LnkSts
  );

/**
  Checks the negotiated PCIe link width.

  @param[in]  Base    device's base address
  @param[out] LnkSts  pointer to the register value with link width mask applied. Valid values are:
                        00 0001b: x1
                        00 0010b: X2
                        00 0100b: x4
                        00 1000b: X8
                        00 1100b: X12 NOT supported
                        01 0000b: X16
                        10 0000b: X32 NOT supported
                        Other     Link width NOT found, this includes situation where device doesn't exist

  @retval TRUE   valid link width
          FALSE  link width NOT found, this includes situation where device doesn't exist
**/
BOOLEAN
GetPcieLinkWidth (
  IN  UINT64           Base,
  OUT UINT16           *LnkSts
  );

/*
  Retrains link behind given device.
  It only makes sense to call it for downstream ports.
  If called for upstream port nothing will happen, it won't enter infinite loop.

  @param[in] Base                 device's base address
  @param[in] PcieCapOffset        devices Pci express capability list register offset
  @param[boolean] WaitUnitlDone   when TRUE, function waits until link has retrained
*/
VOID
RetrainLink (
  UINT64  Base,
  UINT8   PcieCapOffset,
  BOOLEAN WaitUntilDone
  );

/*
  Checks if device at given address exists

  @param[in] Base            device's base address

  @retval TRUE when device exists; FALSE otherwise
*/
BOOLEAN
IsDevicePresent (
  UINT64 Base
  );

/*
  Checks if device is a multifunction device

  @param[in] Base            device's base address

  @retval TRUE if multifunction; FALSE otherwise
*/
BOOLEAN
IsMultifunctionDevice (
  UINT64 Base
  );

/**
  This function assigns Subordinate Bus Number and Secondary Bus Number.

  @param[in] RpBase      Root Port PCI base address
  @param[in] Segment     PCIe Root port PCI Segment
  @param[in] TempPciBus  Temporary PCI bus number
  @retval UINT64         Assigned sub bus base address
**/
UINT64
PcieRpSetSubordSecondBus (
  IN UINT64 RpBase,
  IN UINT8  Segment,
  IN UINT8  TempPciBus
  );

/**
  Clear Subordinate Bus Number and Secondary Bus Number.

  @param[in] RpBase     Root Port PCI base address
**/
VOID
PcieRpClearSubordSecondBus (
  IN UINT64 RpBase
  );
#endif // _PCIE_HELPER_LIB_H_
