/** @file
  Helper functions for SCS controllers initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include "PeiScsInitInternal.h"
#include <IndustryStandard/Pci30.h>

/**
  Programs passed MmioBase address into BAR register.

  @param[in] PciBaseAddress  Address of the PCI config space
  @param[in] MmioBase        Address to be used to access MMIO space
**/
VOID
ScsControllerEnableMmio (
  IN UINT64  PciBaseAddress,
  IN UINT64  MmioBase
  )
{
  //
  // Disable MSE
  //
  PciSegmentAnd16 (PciBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  //
  // Write temp address to base address
  //
  PciSegmentWrite32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) MmioBase);
  if (PciSegmentRead32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (MmioBase, 32));
  }
  //
  // Enable MSE
  //
  PciSegmentOr16 (PciBaseAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  This function clears BAR address.

  @param[in] PciBaseAddress  Address of PCI config space
**/
VOID
ScsControllerDisableMmio (
  IN UINT64  PciBaseAddress
  )
{
  //
  // Disable MSE
  //
  PciSegmentAnd16 (PciBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  //
  // Clear lower base address
  //
  PciSegmentWrite32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, 0);
  if (PciSegmentRead32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (PciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  }
}

/**
  This function puts SCS controller into D3 state

  @param[in] PciBaseAddress  Address of the PCI config space
**/
VOID
ScsControllerPutToD3 (
  IN UINT64  PciBaseAddress
  )
{
  PciSegmentOr32 (PciBaseAddress + R_SCS_CFG_PCS, B_SCS_CFG_PCS_PS_D3HOT);

  PciSegmentRead32 (PciBaseAddress + R_SCS_CFG_PCS);
}

