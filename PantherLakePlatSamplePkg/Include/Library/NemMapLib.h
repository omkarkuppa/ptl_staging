/** @file
  Header file for the NEM Map Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _NEM_MAP_LIB_H_
#define _NEM_MAP_LIB_H_

#include <Uefi.h>

/**
  Determines the Intel Boot Block (IBB) base address and size.

  @param[out] IbbBase         The IBB base address.
  @param[out] IbbSize         The IBB size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetIbbBaseAndSize (
  OUT UINTN     *IbbBase, OPTIONAL
  OUT UINT32    *IbbSize  OPTIONAL
  );

/**
  Determines the Non-Eviction Mode (NEM) region base address and size.

  @param[out] NemBase         The NEM region base address.
  @param[out] NemSize         The NEM region size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetNemBaseAndSize (
  OUT UINTN       *NemBase, OPTIONAL
  OUT UINT32      *NemSize  OPTIONAL
  );

/**
  Determines the Temporary RAM (T-RAM) base address and size.

  @param[out] TempRamBase     The T-RAM base address.
  @param[out] TempRamSize     The T-RAM size.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found.
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetTempRamBaseAndSize (
  OUT UINTN       *TempRamBase, OPTIONAL
  OUT UINT32      *TempRamSize  OPTIONAL
  );

/**
  Determines the Firmware Interface Table (FIT) base address.

  @param[out]   FitBase           The FIT base address.

  @retval       EFI_SUCCESS       The operation completed successfully.
  @retval       EFI_NOT_FOUND     The gNemMapStructureHobGuid was not found
  @retval       EFI_NOT_FOUND     The FIT table could not be found (or is no longer available)
  @retval       EFI_DEVICE_ERROR  The GUID in the NEM map structure is invalid.
**/
EFI_STATUS
EFIAPI
GetFitBase (
  OUT UINTN       *FitBase
  );

/**
  Copies FIT data from a pre-existing FIT location to a FIT HOB.

  Future FIT API requests will return information from the FIT HOB after this API is invoked. The FIT entries
  may be trimmed to BIOS-relevant entries to reduce memory consumption. This allows the total HOB size to be less
  than the maximum HOB size and reduces memory usage if this API is used in pre-memory.

  @param[in]  FitSourceBase         The base address of a pre-existing FIT.

  @retval     EFI_SUCCESS           The FIT HOB was produced successfully.
  @retval     EFI_INVALID_PARAMETER The parameter given is invalid.
  @retval     EFI_NOT_FOUND         The source FIT table could not be found.
**/
EFI_STATUS
EFIAPI
ProduceFitHob (
  IN UINTN FitSourceBase
  );

#endif
