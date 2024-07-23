/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include "AhciPciHcPei.h"


/**
  Check whether the controller is a Ahci host controller.

  @param[in] PciHcBase    PCI address of a specified controller.

  @retval TRUE     The specified controller is an NVM Express host controller.
  @retval FALSE    The specified controller is not an NVM Express host controller.

**/
BOOLEAN
IsPciAhciHc (
  IN UINTN    PciHcBase
  )
{
  UINT8    SubClass;
  UINT8    BaseClass;

  SubClass  = PciRead8 (PciHcBase + PCI_CLASSCODE_OFFSET + 1);
  BaseClass = PciRead8 (PciHcBase + PCI_CLASSCODE_OFFSET + 2);

  if ((BaseClass != PCI_CLASS_MASS_STORAGE) ||
      (SubClass != PCI_CLASS_MASS_STORAGE_SATADPA)) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if the PCI device valid.

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               There is a bridge on that bus.
  @retval FALSE              There is a no bridge on that bus.

**/
BOOLEAN
IsValidPci(
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  )
{
  UINT64 PciAddr;
  PciAddr = PCI_SEGMENT_LIB_ADDRESS ( 0, Bus, Dev, Fun, 0 );
  if (PciRead16 ((UINTN) PciAddr + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }
  return TRUE;
}

/**
  Check if there is a PCI bridge on the Bus.

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               There is a PCI bridge on that bus.
  @retval FALSE              There is a no PCI bridge on that bus.

**/
BOOLEAN
IsPciBridge(
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  )
{
  UINT64 PciAddr;
  PciAddr = PCI_SEGMENT_LIB_ADDRESS ( 0, Bus, Dev, Fun, 0 );

  if ((PciRead8 ((UINTN) PciAddr + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) != HEADER_TYPE_PCI_TO_PCI_BRIDGE) {
    return FALSE;
  }
  return TRUE;
}


/**
  Check if the PCI bridge support multi function

  @param[in]  Bus            Bus Number.
  @param[in]  Dev            Device Number.
  @param[in]  Fun            Function Number.

  @retval TRUE               It support multi function.
  @retval FALSE              It dose not support multi function.

**/
BOOLEAN
IsPciMultiFunc (
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Fun
  )
{
  UINT64 PciAddr;
  PciAddr = PCI_SEGMENT_LIB_ADDRESS ( 0, Bus, Dev, Fun, 0 );

  if (PciRead8 ((UINTN) PciAddr + PCI_HEADER_TYPE_OFFSET) & HEADER_TYPE_MULTI_FUNCTION) {
    return TRUE;
  }
  return FALSE;
}
