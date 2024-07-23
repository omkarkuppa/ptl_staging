/** @file
  Variable Storage Selector SMM Module

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

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/VariableStorageSelectorLib.h>
#include <Protocol/VariableStorageSelectorProtocol.h>

#include <Guid/CseVariableStorageProtocolInstanceGuid.h>
#include <Guid/FvbVariableStorageProtocolInstanceGuid.h>

//
// NOTE: Dynamic PCDs cannot be consumed in the CSE Variable Storage
//       Selector Protocol due to lock conflicts with the variable driver.
//

//
// Handle for the VariableStorageSelectorProtocol
//
STATIC EFI_HANDLE  mHandle;
STATIC BOOLEAN     mEnableFvbVariableStorage;
STATIC BOOLEAN     mEnableCseVariableStorage;

/**
  Gets the variable storage instance ID for the variable storage instance
  that is used to store a given variable

  @param[in]  VariableName      A pointer to a null-terminated string that is
                                the variable's name.
  @param[in]  VariableGuid      A pointer to an EFI_GUID that is the variable's
                                GUID. The combination of VariableGuid and
                                VariableName must be unique.
  @param[out] VariableStorageId The ID for the variable storage instance that
                                stores a given variable

  @retval EFI_SUCCESS           Variable storage instance id was retrieved
**/
EFI_STATUS
EFIAPI
GetVariableStorageId (
  IN  CONST  CHAR16       *VariableName,
  IN  CONST  EFI_GUID     *VendorGuid,
  OUT        EFI_GUID     *VariableStorageId
  )
{
  EFI_STATUS              Status;
  VARIABLE_STORAGE_TYPE   VariableStorageType;

  Status = GetVariableStorageType (VariableName, VendorGuid, &VariableStorageType);
  if (!EFI_ERROR (Status)) {
    if (VariableStorageType == VariableStorageFvb) {
      if (!mEnableFvbVariableStorage) {
        DEBUG ((DEBUG_ERROR, "FVB variable storage is disabled with variable dependencies.\n"));
        ASSERT (mEnableFvbVariableStorage == TRUE);
        Status = EFI_NOT_FOUND;
      } else {
        CopyMem (VariableStorageId, &gFvbVariableStorageProtocolInstanceGuid, sizeof (EFI_GUID));
      }
    } else if (VariableStorageType == VariableStorageCse) {
      if (!mEnableCseVariableStorage) {
        DEBUG ((DEBUG_ERROR, "CSE variable storage is disabled with variable dependencies.\n"));
        ASSERT (mEnableCseVariableStorage == TRUE);
        Status = EFI_NOT_FOUND;
      } else {
        CopyMem (VariableStorageId, &gCseVariableStorageProtocolInstanceGuid, sizeof (EFI_GUID));
      }
    } else {
      Status = EFI_NOT_FOUND;
    }
  }

  return Status;
}

EDKII_VARIABLE_STORAGE_SELECTOR_PROTOCOL mVariableStorageSelectorProtocol = {
  GetVariableStorageId
};

/**
  Installs Variable Storage Selector services.

  The Variable Storage Selector Protocol is consumed by the Variable SMM driver. It
  controls which specific Variable Storage Protocol is used to store a variable.

  @param      None

  @retval     EFI_SUCCESS  Variable Storage Selector services were sucessfully installed / reinstalled.
  @retval     EFI_ERROR    Variable Storage Selector services were not installed / reinstalled.
**/
EFI_STATUS
EFIAPI
VariableStorageSelectorSmmInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  mEnableCseVariableStorage = PcdGetBool (PcdEnableCseVariableStorage);
  mEnableFvbVariableStorage = PcdGetBool (PcdEnableFvbVariableStorage);

  return gSmst->SmmInstallProtocolInterface (
                &mHandle,
                &gEdkiiSmmVariableStorageSelectorProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mVariableStorageSelectorProtocol
                );
}