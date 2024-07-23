/** @file
  HECI common library.

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

@par Specification
**/

#include <PiPei.h>
#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MeUtilsLib.h>

/**
  Return HECI number based on Pci function

  @param[in]      PciFunction     Pci function to be converted to HECI number
  @param[in,out]  HeciNumber      Pointer to returned Heci number
                                  0         is returned for non communication devices (SOL and IDEr)
                                  1..4      is returned for Pci function of specific HECI device
                                  MAX_UINTN is returned for non-existent Pci function

  @retval EFI_SUCCESS             Returned valid HECI number
  @retval EFI_INVALID_PARAMETER   Invalid input parameters
**/
EFI_STATUS
GetHeciNumber (
  IN     UINTN        PciFunction,
  IN OUT UINTN        *HeciNumber
  )
{
  CONST UINT8  HeciFn2No[] = { 1, 2, 0, 0, 3, 4 };

  if (HeciNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (PciFunction >= sizeof (HeciFn2No) / sizeof (HeciFn2No[0])) {
    *HeciNumber = MAX_UINTN;
    return EFI_INVALID_PARAMETER;
  }

  *HeciNumber = HeciFn2No[PciFunction];

  return EFI_SUCCESS;
}

/**
  Allocate memory for HeciControl

  @param[in]  AllocationSize      Amount memory to allocate

  @retval Allocated memory pointer, NULL on failure
**/
VOID *
HeciAllocateMemory (
  IN UINTN   AllocationSize
  )
{
  VOID                      *ReturnAllocatedMemory;
  EFI_SMM_SYSTEM_TABLE2     *Smst2;
  EFI_STATUS                Status;

  MeIsInSmm (&Smst2);
  ReturnAllocatedMemory = NULL;
  if (Smst2 != NULL) {
    Status = Smst2->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      AllocationSize,
                      (VOID **) &ReturnAllocatedMemory
                      );
    if (!EFI_ERROR (Status)) {
      ZeroMem (ReturnAllocatedMemory, AllocationSize);
    }
  } else {
    ReturnAllocatedMemory = AllocateZeroPool (AllocationSize);
  }

  return ReturnAllocatedMemory;
}

/**
  Free memory for HeciControl

  @param[in]      Memory   Memory pointer to free
**/
VOID
HeciFreeMemory (
  IN VOID   *Memory
  )
{
  EFI_SMM_SYSTEM_TABLE2     *Smst2;

  MeIsInSmm (&Smst2);
  if (Smst2 != NULL) {
    Smst2->SmmFreePool (Memory);
  } else {
    FreePool (Memory);
  }
}
