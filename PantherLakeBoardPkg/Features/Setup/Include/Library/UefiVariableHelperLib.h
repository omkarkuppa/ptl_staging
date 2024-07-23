/** @file
  Implementation of helper functions for interacting with UEFI variables.

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

#ifndef _UEFI_VARIABLE_HELPER_LIB_H_
#define _UEFI_VARIABLE_HELPER_LIB_H_

#include <Uefi.h>
#include <Guid/VariableFormat.h>

/**
  Gets the pointer to the first variable header in given variable store area.

  @param VarStoreHeader  Pointer to the Variable Store Header.

  @return Pointer to the first variable header

**/
AUTHENTICATED_VARIABLE_HEADER *
GetStartPointer (
  IN VARIABLE_STORE_HEADER       *VarStoreHeader
  );

/**
  This code gets the pointer to the last variable memory pointer byte.

  @param  VarStoreHeader  Pointer to the Variable Store Header.

  @return AUTHENTICATED_VARIABLE_HEADER* pointer to last unavailable Variable Header.

**/
AUTHENTICATED_VARIABLE_HEADER *
GetEndPointer (
  IN VARIABLE_STORE_HEADER       *VarStoreHeader
  );

/**
  This code checks if variable header is valid or not.

  @param  Variable  Pointer to the Variable Header.

  @retval TRUE      Variable header is valid.
  @retval FALSE     Variable header is not valid.

**/
BOOLEAN
IsValidVariableHeader (
  IN  AUTHENTICATED_VARIABLE_HEADER   *Variable
  );

/**
  This code gets the size of name of variable.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
UINTN
NameSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  );

/**
  This code gets the size of data of variable.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return Size of variable in bytes in type UINTN.

**/
UINTN
DataSizeOfVariable (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  );

/**
  This code gets the size of variable header.

  @param AuthFlag   Authenticated variable flag.

  @return Size of variable header in bytes in type UINTN.

**/
UINTN
GetVariableHeaderSize (
  IN  BOOLEAN       AuthFlag
  );

/**
  This code gets the pointer to the variable name.

  @param   Variable  Pointer to the Variable Header.
  @param   AuthFlag  Authenticated variable flag.

  @return  A CHAR16* pointer to Variable Name.

**/
CHAR16 *
GetVariableNamePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN BOOLEAN                            AuthFlag
  );

/**
  This code gets the pointer to the variable guid.

  @param Variable   Pointer to the Variable Header.
  @param AuthFlag   Authenticated variable flag.

  @return A EFI_GUID* pointer to Vendor Guid.

**/
EFI_GUID *
GetVendorGuidPtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *AuthVariable,
  IN  BOOLEAN                           AuthFlag
  );

/**
  This code gets the pointer to the variable data.

  @param   Variable  Pointer to the Variable Header.
  @param   AuthFlag  Authenticated variable flag.

  @return  A UINT8* pointer to Variable Data.

**/
UINT8 *
GetVariableDataPtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN  BOOLEAN                           AuthFlag
  );

/**
  This code gets the pointer to the next variable header.

  @param  Variable  Pointer to the Variable Header.
  @param  AuthFlag  Authenticated variable flag.

  @return  A AUTHENTICATED_VARIABLE_HEADER* pointer to next variable header.

**/
AUTHENTICATED_VARIABLE_HEADER *
GetNextVariablePtr (
  IN  AUTHENTICATED_VARIABLE_HEADER     *Variable,
  IN  BOOLEAN                           AuthFlag
  );

/**
  Prints UEFI variable data.

  @param  Data8     A pointer to a buffer of UEFI variable data.
  @param  DataSize  The size of UEFI variable data in the buffer.

**/
VOID
PrintVariableData (
  IN UINT8   *Data8,
  IN UINTN   DataSize
  );

#endif
