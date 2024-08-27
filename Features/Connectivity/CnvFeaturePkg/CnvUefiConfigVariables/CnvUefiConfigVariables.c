/** @file
  The driver is to create and publish the Connectivity UEFI variables.

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

@par Specification
**/

#include <CnvUefiConfigVariables.h>
#include <CnvVfrSetupMenuHii.h>
#include "CnvGuidLockIndicator.h"

GLOBAL_REMOVE_IF_UNREFERENCED EFI_CNV_UEFI_GUID_LOCK_STATUS_INDICATOR_PROTOCOL  mCnvGuidLockStatus = {
  CNV_UEFI_GUID_LOCK_REVISION,
  GetGuidLockStatusIndicator
};

/**
  This function handles PlatformInit task at the end of DXE
  @param[in]  Event
  @param[in]  *Context
**/
VOID
EFIAPI
CnvUefiEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  UINTN       HandleCount;
  UINTN       Index;
  EFI_HANDLE  *ImageHandle;

  Status = EFI_SUCCESS;

  gBS->LocateHandleBuffer (
         ByProtocol,
         &gCnvUefiGuidLockStatusProtocolGuid,
         NULL,
         &HandleCount,
         &ImageHandle
         );

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    ImageHandle[Index],
                    &gCnvUefiGuidLockStatusProtocolGuid,
                    &mCnvGuidLockStatus,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  }

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  ///
  /// Free any allocated buffers
  ///
  FreePool (ImageHandle);
}

/**
  The function get the GUID lock status
  @param[out]  GuidLockStatusIndicator
**/
UINT8
EFIAPI
GetGuidLockStatusIndicator (
  )
{
  return PcdGet8 (PcdCnvGuidLockStatus);
}

/**
  Update/Create Connectivity UEFI variable

  @param[in] CnvVarSize      Connectivity variable size
  @param[in] VariableName    Pointer to variable name
  @param[in] VendorGuid      Pointer to variable vendor guid
  @param[in] NewData         Pointer to variable data
  @param[in] LockVariable    Variable to be kept locked/unlocked
  @param[in] CheckVarUpdate  Check if variable needs to be updated

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
UpdateVariables (
  IN UINTN     CnvVarSize,
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN VOID      *NewData,
  IN BOOLEAN   LockVariable,
  IN BOOLEAN   CheckVarUpdate
  )
{
  EDKII_VARIABLE_POLICY_PROTOCOL  *VariablePolicy;
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  VOID                            *OldData;
  BOOLEAN                         NeedUpdate;
  UINT8                           CnvGuidLockStatus;

  OldData           = NULL;
  NeedUpdate        = FALSE;
  Status            = EFI_SUCCESS;
  CnvGuidLockStatus = 0;

  VarSize = CnvVarSize;
  OldData = AllocateZeroPool (VarSize);
  if (OldData == NULL) {
    ASSERT (OldData);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get Variable to know whether variable is already created or not
  //
  Status = gRT->GetVariable (
                  VariableName,
                  VendorGuid,
                  NULL,
                  &VarSize,
                  OldData
                  );

  if (Status == EFI_NOT_FOUND) {
    Status     = EFI_SUCCESS;
    NeedUpdate = TRUE;
  }

  //
  // Update the variable if the variable has not been created yet or has been changed.
  //
  if (!EFI_ERROR (Status)) {
    if ((NeedUpdate == FALSE) && (CheckVarUpdate == TRUE)) {
      if (CompareMem (OldData, NewData, VarSize)) {
        NeedUpdate = TRUE;
      }
    }

    if (NeedUpdate) {
      Status = gRT->SetVariable (
                      VariableName,
                      VendorGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      VarSize,
                      NewData
                      );
    }
  }

  FreePool (OldData);

  if (EFI_ERROR (Status)) {
    ASSERT (!EFI_ERROR (Status));
    return Status;
  }

  CnvGuidLockStatus = PcdGet8 (PcdCnvGuidLockStatus);

  if (CnvGuidLockStatus == 1) {
    LockVariable = TRUE;
    VariableName = NULL;
  }

  // Feature variable to be kept unlocked in BIOS, to enable updating via tool (Ex. XmlCli).
  if (!LockVariable) {
    return EFI_SUCCESS;
  }

  //
  // Mark the read-only variables if the Variable Lock protocol exists (Lock CNV variables)
  //
  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID **)&VariablePolicy);
  DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
  if (!EFI_ERROR (Status)) {
    Status = RegisterBasicVariablePolicy (
                                          VariablePolicy,
               VendorGuid,
               VariableName,
               VARIABLE_POLICY_NO_MIN_SIZE,
               VARIABLE_POLICY_NO_MAX_SIZE,
               VARIABLE_POLICY_NO_MUST_ATTR,
               VARIABLE_POLICY_NO_CANT_ATTR,
               VARIABLE_POLICY_TYPE_LOCK_NOW
               );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Entry point of the CNV UEFI Variables driver.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval EFI_ABORTED        The driver encountered an error and could not create/update
                             the CNV UEFI Variables completely.

**/
EFI_STATUS
EFIAPI
CnvUefiConfigVariables (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE                         Handle;
  EFI_STATUS                         Status;
  CNV_UEFI_VARIABLE_CONFIG_PROTOCOL  *CnvUefiVariableConfig;
  UEFI_CNV_CONFIG                    *UefiCnvConfig;
  EFI_EVENT                          EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "CnvUefiConfigVariables entry\n"));

  Handle        = NULL;
  UefiCnvConfig = NULL;

  Status = gBS->LocateProtocol (&gCnvUefiVariableConfigProtocolGuid, NULL, (VOID **)&CnvUefiVariableConfig);

  if (EFI_ERROR (Status) || (CnvUefiVariableConfig == NULL)) {
    //
    // Create UEFI_CNV_CONFIG with default settings
    //
    UefiCnvConfig = AllocateZeroPool (sizeof (UEFI_CNV_CONFIG));
    if (UefiCnvConfig != NULL) {
      CnvConfigInit (UefiCnvConfig);
    } else {
      goto Done;
    }
  } else {
    ASSERT (CnvUefiVariableConfig != NULL);
    UefiCnvConfig = &CnvUefiVariableConfig->UefiCnvConfig;
  }

  Status = InstallCnvUefiVariables (UefiCnvConfig);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  //
  // Install the CNV UEFI Variables Protocol on a new handle
  // to signify the CNV UEFI Variables are ready.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gCnvUefiVariablesProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gCnvUefiGuidLockStatusProtocolGuid,
                  &mCnvGuidLockStatus,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  CnvUefiEndOfDxe,
                  &ImageHandle,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

Done:
  if ((UefiCnvConfig != NULL) && (CnvUefiVariableConfig == NULL)) {
    FreePool (UefiCnvConfig);
  }

  DEBUG ((DEBUG_ERROR, "CnvUefiConfigVariables status: %r\n", Status));
  ASSERT (!EFI_ERROR (Status));
  return Status;
}
