/** @file
  A library for managing the UEFI Variable default data HOB in PEI.

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

#ifndef _UEFI_VARIABLE_DEFAULT_HOB_LIB_H_
#define _UEFI_VARIABLE_DEFAULT_HOB_LIB_H_

#include <PiPei.h>
#include <Uefi.h>
#include <Guid/VariableFormat.h>

/**
  Find variable from default variable HOB.

  @param[in]  VariableName      A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.
  @param[out] AuthFlag          Pointer to output Authenticated variable flag.

  @return Pointer to variable header, NULL if not found.

**/
AUTHENTICATED_VARIABLE_HEADER *
EFIAPI
FindVariableFromHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  OUT BOOLEAN                   *AuthFlag
  );

/**
  Get variable from default variable HOB.

  @param[in]      VariableName  A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in]      VendorGuid    A unique identifier for the vendor.
  @param[out]     Attributes    If not NULL, a pointer to the memory location to return the
                                attributes bitmask for the variable.
  @param[in, out] DataSize      On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[out]     Data          The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER DataSize is NULL.
  @retval EFI_INVALID_PARAMETER The DataSize is not too small and Data is NULL.

**/
EFI_STATUS
EFIAPI
GetVariableFromHob (
  IN     CHAR16                 *VariableName,
  IN     EFI_GUID               *VendorGuid,
  OUT    UINT32                 *Attributes, OPTIONAL
  IN OUT UINTN                  *DataSize,
  OUT    VOID                   *Data
  );

/**
  Set variable to default variable HOB.

  @param[in] VariableName       A Null-terminated string that is the name of the vendor's
                                variable.
  @param[in] VendorGuid         A unique identifier for the vendor.
  @param[in] Attributes         If not NULL, a pointer to the memory location to set the
                                attributes bitmask for the variable.
  @param[in] DataSize           On input, the size in bytes of the return Data buffer.
                                On output the size of data returned in Data.
  @param[in] Data               The buffer to return the contents of the variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER Attributes is not NULL, but attributes value is 0.
  @retval EFI_INVALID_PARAMETER DataSize is not equal to the variable data size.
  @retval EFI_INVALID_PARAMETER The DataSize is equal to the variable data size, but Data is NULL.

**/
EFI_STATUS
EFIAPI
SetVariableToHob (
  IN CHAR16                     *VariableName,
  IN EFI_GUID                   *VendorGuid,
  IN UINT32                     *Attributes, OPTIONAL
  IN UINTN                      DataSize,
  IN VOID                       *Data
  );

/**
  This function finds the matched default data and create GUID hob for it.

  @param[in] DefaultId         - Specifies the type of defaults to retrieve.

  @return EFI_SUCCESS          - The matched default data is found.
  @return EFI_NOT_FOUND        - The matched default data is not found.
  @return EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

**/
EFI_STATUS
EFIAPI
CreateDefaultVariableHob (
  IN UINT16  DefaultId
  );

#endif
