/** @file
  Declares Connection Manager common utility functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _CM_UTILS_LIB_H_
#define _CM_UTILS_LIB_H_

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>

#define MMIO_ALIGNMENT_1MB                20

#define CmZeroMem(Dest, Length)           ZeroMem (Dest, Length)
#define CmFillMem(Dest, Length, Fill)     SetMem (Dest, Length, Fill)
#define CmCopyMem(Dest, Src, Length)      CopyMem (Dest, Src, Length)

//
// Define the debug output level of verbose messages.
// Set CM_VERBOSE to DEBUG_INFO if CM verbose messages are required for debugging.
// CM_DEBUG is used to debug timing related issue that only DEBUG_ERROR level messages are displayed.
//
#define CM_VERBOSE          DEBUG_VERBOSE
#define TX_VERBOSE          DEBUG_VERBOSE
#define HOTPLUG_VERBOSE     DEBUG_VERBOSE
#define CAP_VERBOSE         DEBUG_INFO
#define PATH_VERBOSE        DEBUG_INFO
#define CM_DEBUG            DEBUG_ERROR

#define sizeofdw(a)   (sizeof (a) / sizeof (UINT32))

/**
  Allocates the memory and zero the content

  @param[in] MemSize - The size in bytes of the allocation request
  @retval    Pointer to the allocated memory, NULL in case of failure

**/
VOID *
CmAllocateZeroMem (
  IN UINTN  MemSize
  );

/**
  Deallocates previously allocated memory using TbtCmAllocateZeroMem interface

  @param[in] Mem - Pointer to the memory
**/
VOID
CmFreeMem (
  IN VOID  *Mem
  );

/**
  Swaps the byte endianness within each DWORD

  @param[in]  Src    - Data to swap
  @param[in]  Length - Length of the data to swap in DW
  @param[out] Dst    - Swapped data
**/
VOID
CmSwapEndianness (
  IN UINT32     *Src,
  IN UINT32     Length,
  OUT UINT32    *Dst
  );

/**
  Allocate MMIO space with 1MB alignment

  @param[in]  ImageHandle - The image handle consume the allocated space
  @param[in]  MmioLength  - Length of request MMIO range
  @param[out] MmioAddr    - Base address of the allocated MMIO range

  @retval EFI_SUCCESS           - Allocate MMIO successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resources.
**/
EFI_STATUS
CmAllocMmio (
  IN  EFI_HANDLE          ImageHandle,
  IN  UINT64              MmioLength,
  OUT PHYSICAL_ADDRESS    *MmioAddr
  );

/**
  Free the allocated MMIO resources

  @param[in] MmioAddr   - MMIO base address to be freed
  @param[in] MmioLength - MMIO length to be freed
**/
VOID
CmFreeMmio (
  IN PHYSICAL_ADDRESS     MmioAddr,
  IN UINT64               MmioLength
  );

/**
  Output post code with optional delay

  @param[in] Code  - Post code to be displayed
  @param[in] Delay - Delay in us to be applied after post code output
**/
VOID
CmPostCode (
  IN UINT32  Code,
  IN UINT32  Delay
  );
#endif
