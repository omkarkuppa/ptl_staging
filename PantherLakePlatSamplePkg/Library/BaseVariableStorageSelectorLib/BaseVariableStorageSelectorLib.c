/** @file
  Base Variable Storage Selector Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Library/PcdLib.h>
#include <Library/BootMediaLib.h>
#include <Library/DebugLib.h>
#include <Library/VariableStorageSelectorLib.h>

#include "BaseVariableStorageSelectorLib.h"

//
// NOTE: Dynamic PCDs cannot be consumed in this library due to
//       lock conflicts with the variable driver.
//

/**
  Gets the variable storage instance type for the variable storage instance
  that is used to store a given variable

  @param[in]  VariableName        A pointer to a null-terminated string that is
                                  the variable's name.
  @param[in]  VariableGuid        A pointer to an EFI_GUID that is the variable's
                                  GUID. The combination of VariableGuid and
                                  VariableName must be unique.
  @param[out] VariableStorageType The type for the variable storage instance that
                                  stores a given variable

  @retval EFI_SUCCESS           Variable storage instance type was retrieved
**/
EFI_STATUS
EFIAPI
GetVariableStorageType (
  IN  CONST  CHAR16                 *VariableName,
  IN  CONST  EFI_GUID               *VendorGuid,
  OUT        VARIABLE_STORAGE_TYPE  *VariableStorageType
  )
{
  if (!BootMediaIsKnown ()) {
    DEBUG ((DEBUG_ERROR, "The boot media unknown! Variable storage is unknown!\n"));
    ASSERT (FALSE);
    CpuDeadLoop ();
  } else if (BootMediaIsSpi ()) {
    *VariableStorageType = VariableStorageFvb;
  } else {
    *VariableStorageType = VariableStorageCse;
  }

  return EFI_SUCCESS;
}
