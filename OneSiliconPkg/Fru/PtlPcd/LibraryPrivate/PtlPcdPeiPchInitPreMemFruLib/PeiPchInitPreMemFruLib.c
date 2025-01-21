/** @file
  PEIM to disable the PLL

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PmcPrivateLib.h>
#include <Library/HobLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Defines/PchReservedResources.h>

/**
  Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios,
  the resource range should be preserved in ACPI as well.
**/
VOID
PchPreservedMmioResource (
  VOID
  )
{
  //
  // This function builds a HOB that describes a chunk of system memory.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PCH_PRESERVED_BASE_ADDRESS,
    PCH_PRESERVED_MMIO_SIZE
    );
  //
  // This function builds a HOB for the memory allocation.
  //
  BuildMemoryAllocationHob (
    PCH_PRESERVED_BASE_ADDRESS,
    PCH_PRESERVED_MMIO_SIZE,
    EfiMemoryMappedIO
    );
  //
  // Reserve additional MMIO for both P2SB instances
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PCH_PCR_BASE_ADDRESS,
    PCH_PCR_MMIO_SIZE
    );
  BuildMemoryAllocationHob (
    PCH_PCR_BASE_ADDRESS,
    PCH_PCR_MMIO_SIZE,
    EfiMemoryMappedIO
    );
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PCH_PCR_SECOND_BASE_ADDRESS,
    PCH_PCR_MMIO_SIZE
  );
  BuildMemoryAllocationHob (
    PCH_PCR_SECOND_BASE_ADDRESS,
    PCH_PCR_MMIO_SIZE,
    EfiMemoryMappedIO
    );
}