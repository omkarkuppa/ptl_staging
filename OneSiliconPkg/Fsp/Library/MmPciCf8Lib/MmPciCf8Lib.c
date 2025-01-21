/** @file
  This file contains routines that get PCI Express Address through I/O Ports CF8 and CFC

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include <Library/IoLib.h>

#define PCIEX_BAR_REG          0x80000060     // B0:D0:F0:R60
#define PCIEX_BAR_EN_MASK      0x1
#define PCIEX_BAR_LENGTH_MASK  0x6
#define PCIEX_BAR_ADDR_MASK    0x7FFC000000

/**
  This procedure will get PCIE address

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number

  @retval PCIE address
**/
UINTN
MmPciBase (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  )
{
  UINT64 PciExpressBaseValue = 0;
  BOOLEAN  InterruptState;

  ASSERT ((Bus <= 0xFF) && (Device <= 0x1F) && (Function <= 0x7));
  //
  //  Get PciExpressBaseAddress from PCIEX_BAR_REG (B0:D0:F0:R60)
  //
  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32(0xCF8, PCIEX_BAR_REG);
  PciExpressBaseValue = IoRead32(0xCFC);
  SetInterruptState (InterruptState);
  ASSERT ((PciExpressBaseValue & PCIEX_BAR_EN_MASK) == 1);

  return ((UINTN) (PciExpressBaseValue & PCIEX_BAR_ADDR_MASK) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) (Function << 12));
}
