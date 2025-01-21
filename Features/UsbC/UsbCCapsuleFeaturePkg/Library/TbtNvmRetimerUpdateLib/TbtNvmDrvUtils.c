/** @file
  Implements TbtNvmDrv common utilities.

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

@par Specification Reference:

**/
#include <Library/TbtNvmDrvUtils.h>

/**
  Allocates the memory

  @param[in]    AllocationSize The size in bytes of the allocation request
  @retval       Pointer to the allocated memory, NULL in case of failure

**/
VOID*
TbtNvmDrvAllocateMem (
  IN UINTN  AllocationSize
  )
{
  return AllocateZeroPool (AllocationSize);
}

/**
  Deallocates previously allocated memory using TbtNvmDrvAllocateMem interface

  @param[in]    Mem            Pointer to the memory

**/
VOID
TbtNvmDrvDeAllocateMem (
  IN VOID*  Mem
  )
{
  FreePool (Mem);
}

/**
  Swaps the endianess of the given data

  @param[in]  Src    Data to swap
  @param[in]  Length Length of the data to swap in DW
  @param[out] Dst    Swapped data

**/
VOID
TbtNvmDrvSwapEndianess (
  IN  UINT32 *Src,
  IN  UINT32 Length,
  OUT UINT32 *Dst
  )
{
  UINT32 Index;

  for (Index = 0; Index < Length; Index++) {
    *(Dst + Index) = SwapBytes32 (*(Src + Index));
  }
}