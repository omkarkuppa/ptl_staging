/** @file
  Variable Storage Selector PEIM

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

#include <Library/BaseMemoryLib.h>
#include <Library/BootMediaLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/VariableStorageSelectorLib.h>
#include <Ppi/VariableStorageSelectorPpi.h>

#include <Guid/CseVariableStoragePpiInstanceGuid.h>
#include <Guid/FvbVariableStoragePpiInstanceGuid.h>

//
// NOTE: Dynamic PCDs cannot be consumed in the Variable Storage
//       Selector PPI due to lock conflicts with the variable driver.
//

/**
  Gets the variable storage instance ID for the variable storage instance
  that is used to store a given variable.

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
PeiGetVariableStorageId (
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
      if (!PcdGetBool (PcdEnableFvbVariableStorage) && !PcdGetBool (PcdNvVariableEmulationMode)) {
        DEBUG ((DEBUG_ERROR, "FVB variable storage is disabled with variable dependencies.\n"));
        Status = EFI_NOT_FOUND;
      } else {
        CopyMem (VariableStorageId, &gFvbVariableStoragePpiInstanceGuid, sizeof (EFI_GUID));
      }
    } else if (VariableStorageType == VariableStorageCse) {
      if (!PcdGetBool (PcdEnableCseVariableStorage) && !PcdGetBool (PcdNvVariableEmulationMode)) {
        DEBUG ((DEBUG_ERROR, "CSE variable storage is disabled with variable dependencies.\n"));
        Status = EFI_NOT_FOUND;
      } else {
        CopyMem (VariableStorageId, &gCseVariableStoragePpiInstanceGuid, sizeof (EFI_GUID));
      }
    } else {
      Status = EFI_NOT_FOUND;
    }
  }

  return Status;
}

EDKII_VARIABLE_STORAGE_SELECTOR_PPI mCseVariableStorageSelectorPpi = {
  PeiGetVariableStorageId
};

EFI_PEI_PPI_DESCRIPTOR mPpiListVariable = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiVariableStorageSelectorPpiGuid,
  &mCseVariableStorageSelectorPpi
};

/**
  Installs the Variable Storage Selector PPI.

  The Variable Storage Selector PPI is consumed by the Variable PEIM. It controls
  which specific Variable Storage PPI is used to store a variable.

  @param      None

  @retval     EFI_SUCCESS  Variable Storage Selector services were sucessfully installed / reinstalled.
  @retval     EFI_ERROR    Variable Storage Selector services were not installed / reinstalled.
**/
EFI_STATUS
EFIAPI
PeimInstallVariableStorageSelector (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_PEI_PPI_DESCRIPTOR                *PeiPpiDescriptor;
  EDKII_VARIABLE_STORAGE_SELECTOR_PPI   *VariableStorageSelectorPpi;
  EFI_STATUS                            Status;

  PeiServicesRegisterForShadow (FileHandle);

  Status = PeiServicesLocatePpi (
             &gEdkiiVariableStorageSelectorPpiGuid,
             0,
             &PeiPpiDescriptor,
             (VOID **) &VariableStorageSelectorPpi
             );

  if (Status == EFI_SUCCESS) {
    //
    // Reinstall the Variable Storage Selector PPI
    //
    Status = PeiServicesReInstallPpi (
               PeiPpiDescriptor,
               &mPpiListVariable
               );
  } else if (Status == EFI_NOT_FOUND) {
    //
    // Install the Variable Storage Selector PPI
    //
    Status = PeiServicesInstallPpi (&mPpiListVariable);
    ASSERT_EFI_ERROR (Status);

    if (PcdGetBool (PcdNvVariableEmulationMode)) {
      DEBUG ((DEBUG_INFO, "UEFI variable emulation mode is enabled. Disabling other mechanisms.\n"));
      PcdSetBoolS (PcdEnableFvbVariableStorage, FALSE);
      PcdSetBoolS (PcdEnableCseVariableStorage, FALSE);
    } else if (!BootMediaIsKnown ()) {
      DEBUG ((DEBUG_ERROR, "The boot media is unknown. UEFI variables cannot be saved properly!\n"));
      ASSERT (BootMediaIsKnown ());
      CpuDeadLoop ();
    } else if (BootMediaIsSpi ()) {
      if (PcdGetBool (PcdEnableFvbVariableStorage)) {
        PcdSetBoolS (PcdEnableCseVariableStorage, FALSE);
      } else {
        DEBUG ((DEBUG_WARN, "Warning: PcdEnableFvbVariableStorage is FALSE on SPI boot. Enabling emulation mode.\n"));

        PcdSetBoolS (PcdEnableCseVariableStorage, FALSE);
        PcdSetBoolS (PcdNvVariableEmulationMode, TRUE);
      }
    } else {
      DEBUG ((DEBUG_ERROR, "The boot media is unknown. UEFI variables cannot be saved properly!\n"));
      CpuDeadLoop ();
    }
  }
  ASSERT_EFI_ERROR (Status);

  return Status;
}
