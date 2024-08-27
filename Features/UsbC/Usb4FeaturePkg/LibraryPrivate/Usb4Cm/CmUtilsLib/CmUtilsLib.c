/** @file
  Implements Connection Manager utility functions

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

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PostCodeLib.h>
#include <Library/CmUtilsLib.h>

/**
  Allocates the memory.

  @param[in] MemSize - The size in bytes of the allocation request.

  @retval    Pointer to the allocated memory, NULL in case of failure.

**/
VOID *
CmAllocateZeroMem (
  IN UINTN  MemSize
  )
{
  return AllocateZeroPool (MemSize);
}

/**
  Deallocates previously allocated memory using CmAllocateZeroMem interface.

  @param[in] Mem - Pointer to the memory.
**/
VOID
CmFreeMem (
  IN VOID  *Mem
)
{
  if (Mem != NULL) {
    FreePool (Mem);
  }
}

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
  )
{
  EFI_STATUS              Status;
  PHYSICAL_ADDRESS        BaseAddr;

  if ((ImageHandle == (EFI_HANDLE) 0) || (MmioAddr == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  BaseAddr = *MmioAddr;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  MMIO_ALIGNMENT_1MB, // 2^20: 1M Alignment
                  MmioLength,
                  &BaseAddr,
                  ImageHandle,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CmAllocMmio: AllocateMemorySpace failure, Status=%r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
  } else {
    *MmioAddr = BaseAddr;
  }

Exit:
  return Status;
}

/**
  Free the allocated MMIO resources

  @param[in] MmioAddr   - MMIO base address to be freed
  @param[in] MmioLength - MMIO length to be freed
**/
VOID
CmFreeMmio (
  IN PHYSICAL_ADDRESS     MmioAddr,
  IN UINT64               MmioLength
  )
{
  EFI_STATUS    Status;

  Status = gDS->FreeMemorySpace (MmioAddr, MmioLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CmFreeMmio: FreeMemorySpace Status=%r\n", Status));
  }
}

/**
  Swaps the byte endianness within each DWORD

  @param[in]  Src    - Data to swap
  @param[in]  Length - Length of the data to swap in DW
  @param[out] Dst    - Swapped data
**/
VOID
CmSwapEndianness (
  IN  UINT32   *Src,
  IN  UINT32   Length,
  OUT UINT32   *Dst
  )
{
  UINT32 Index;
  for (Index = 0; Index < Length; Index++) {
    *(Dst + Index) = SwapBytes32 (*(Src + Index));
  }
}

/**
  Output post code with optional delay

  @param[in] Code  - Post code to be displayed
  @param[in] Delay - Delay in us to be applied after post code output
**/
VOID
CmPostCode (
  IN UINT32  Code,
  IN UINT32  Delay
  )
{
  PostCode (Code);

  if (Delay != 0) {
    gBS->Stall (Delay);
  }
}