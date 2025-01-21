/** @file

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

#ifndef _HECI_COMMON_LIB_H_
#define _HECI_COMMON_LIB_H_

#include <Library/SmmServicesTableLib.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/HeciTransportProtocol.h>

/**
  Allocate memory for Heci (PEI/DXE/SMM)

  @param[in]  AllocationSize      Size of the memory to allocate

  @retval   Allocated memory pointer, NULL on failure
**/
VOID *
HeciAllocateMemory (
  IN UINTN   AllocationSize
  );

/**
  Free memory used by Heci (PEI/DXE/SMM)

  @param[in]  Memory              Pointer to the buffer to free
**/
VOID
HeciFreeMemory (
  IN VOID   *Memory
  );

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
  );

/**
  Get instance index of given PPI for given GUID.

  @param[in] InputPpi         PPI instance pointer to look for
  @param[in] InputPpiGuid     PPI Guid to search for

  @retval PPI instance index if PPI instance found
  @retval MAX_UINT32 if PPI instance not found
**/
UINT32
GetPpiInstanceIndex (
  IN VOID       *InputPpi,
  IN EFI_GUID   *InputPpiPpiGuid
  );

/**
  Get next instance of given PPI for given GUID.
  If NULL passed as an InputPpi, returns first PPI instance.

  @param[in] InputPpi         PPI instance pointer to look for next instance
  @param[in] InputPpiGuid     PPI Guid to search for

  @retval Pointer to next PPI instance
  @retval NULL if there's no more PPI instances
**/
VOID *
GetNextPpi (
  IN VOID       *InputPpi,
  IN EFI_GUID   *InputPpiGuid
  );

/**
  Get first HeciAccess PPI/Protocol pointer (PEI/DXE/SMM)

  @retval Pointer to first HeciAccess protocol
  @retval NULL pointer indicates error
**/
HECI_ACCESS *
GetFirstHeciAccess (
  VOID
  );

/**
  Get first HeciTransport PPI/Protocol pointer (PEI/DXE/SMM)

  @retval Pointer to first HeciTransport protocol
  @retval NULL pointer indicates error
**/
HECI_TRANSPORT *
GetFirstHeciTransport (
  VOID
  );

/**
  Function installs DXE or SMM protocol depending on mode we are running in

  @param[in] HeciGuidProtocol       Pointer to Dxe/Smm protocol guid to install
  @param[in] HeciProtocol           Pointer to HECI protocol to install

  @retval EFI_SUCCESS               Protocol installed successfully
  @retval EFI_INVALID_PARAMETER     Wrong parameters passed
**/
EFI_STATUS
InstallHeciProtocol (
  EFI_GUID    *HeciGuidProtocol,
  VOID        *HeciProtocol
  );

#endif // _HECI_COMMON_LIB_H_
