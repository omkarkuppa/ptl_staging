/** @file

    Source code file for the Base MMIO Init library

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


#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/HobLib.h>
#include <Library/MmioInitLib.h>

typedef struct {
  EFI_PHYSICAL_ADDRESS    BaseAddress;
  UINT64                  Length;
} MEMORY_MAP;

GLOBAL_REMOVE_IF_UNREFERENCED MEMORY_MAP MmioMap [] = {
  {FixedPcdGet64 (PcdLocalApicAddress), FixedPcdGet32 (PcdLocalApicMmioSize)},
  {FixedPcdGet64 (PcdMchBaseAddress), FixedPcdGet32 (PcdMchMmioSize)},
  {FixedPcdGet64 (PcdDmiBaseAddress), FixedPcdGet32 (PcdDmiMmioSize)},
  {FixedPcdGet64 (PcdEpBaseAddress), FixedPcdGet32 (PcdEpMmioSize)}
};

/**
  Initialize MMIO ranges

  @retval EFI_SUCCESS    The function completed successfully
**/
EFI_STATUS
MmioInit (
  VOID
  )
{
  UINTN                         Index;
  MEMORY_MAP                    PcieMmioMap;

  ///
  /// Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios.
  ///
  PcieMmioMap.BaseAddress = FixedPcdGet64 (PcdPciExpressBaseAddress);
  PcieMmioMap.Length = PcdGet32 (PcdPciExpressRegionLength);

  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    PcieMmioMap.BaseAddress,
    PcieMmioMap.Length
    );
  BuildMemoryAllocationHob (
    PcieMmioMap.BaseAddress,
    PcieMmioMap.Length,
    EfiMemoryMappedIO
    );
  for (Index = 0; Index < sizeof (MmioMap) / (sizeof (MEMORY_MAP)); Index++) {
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
      (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length
      );
    BuildMemoryAllocationHob (
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length,
      EfiMemoryMappedIO
      );
  }

  return EFI_SUCCESS;
}
