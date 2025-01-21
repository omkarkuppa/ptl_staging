/** @file
  Initializes PCH Primary To Sideband Bridge (P2SB) Device in PEI

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <P2SbController.h>

/**
  Internal function performing allocation HOB for memory resource

  @param[in] P2SbController      P2SB controller
  @param[in] HpetMmio            HPET MMIO
**/
VOID
P2sbHpetBuildMemResHob (
  IN  P2SB_CONTROLLER     *P2SbController,
  IN  UINTN               HpetMmio
  )
{
  //
  // Build the resource descriptor HOB for HPET address resource.
  // HPET only claims 0x400 in size, but the minimal size to reserve memory
  // is one page 0x1000.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    HpetMmio,
    0x1000
    );
  BuildMemoryAllocationHob (
    HpetMmio,
    0x1000,
    EfiMemoryMappedIO
    );
}
