 /** @file
  This file contains the interface to invoke Capsule PPI functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef _CAPSULE_PPI_PROCESS_LIB_H_
#define _CAPSULE_PPI_PROCESS_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

//
// System main memory map range information
//
typedef struct {
  EFI_PHYSICAL_ADDRESS    PhysicalAddress;
  UINT64                  RangeLength;
} PEI_DDR_MAIN_MEMORY_MAP_RANGE;

/**
  This function tries to coalesce capsule image buffer before calling
  PeiServicesInstallPeiMemory() when BootMode is BOOT_ON_FLASH_UPDATE mode.

  @param[in]  MemoryMap             A pointer to the array of main memory ranges.
  @param[in]  NumRanges             Number of main memory ranges.
  @param[in]  PeiMemoryBaseAddress  The base address of PEI memory to be installed.
  @param[in]  PeiMemoryLength       The length of of PEI memory to be installed.

**/
VOID
EFIAPI
CoalesceCapsulesIfNeeded (
  IN PEI_DDR_MAIN_MEMORY_MAP_RANGE  *MemoryMap,
  IN UINT8                          NumRanges,
  IN EFI_PHYSICAL_ADDRESS           PeiMemoryBaseAddress,
  IN UINT64                         PeiMemoryLength
  );

#endif // _CAPSULE_PPI_PROCESS_LIB_H_
