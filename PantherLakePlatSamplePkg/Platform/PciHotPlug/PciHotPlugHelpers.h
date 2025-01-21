/** @file

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

#ifndef _PCI_HOT_PLUG_HELPERS_H_
#define _PCI_HOT_PLUG_HELPERS_H_

#include "PciHotPlug.h"
#include <Library/PciSegmentLib.h>
#include <Library/PcieHelperLib.h>


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
  );

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
  );

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
  );

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
  );

/**
  Check if P2pb device is the last one in the same layer bus.

  @param[in]      HpcPciAddress        The address of the Hot Plug Controller function on the PCI bus.

  @retval TRUE    This P2pb device is the last one.
  @retval FALSE   This P2pb device is not the last one.
**/
BOOLEAN
IsLastP2pbDevice (
  IN      UINT64 HpcPciAddress
  );
#endif
