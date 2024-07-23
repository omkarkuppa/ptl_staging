/** @file
  This file contains routines that get PCI Express Address

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

/**
  This procedure will get PCIE address

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number

  @retval PCIE address
**/
UINTN
EFIAPI
MmPciBase (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  )
{
  ASSERT ((Bus <= 0xFF) && (Device <= 0x1F) && (Function <= 0x7));

#if FixedPcdGetBool(PcdFspBinaryEnable) == 1
  return ((UINTN) PcdGet64 (PcdSiPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) (Function << 12));
#else
  return ((UINTN) PcdGet64 (PcdPciExpressBaseAddress)   + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) (Function << 12));
#endif
}
