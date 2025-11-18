/** @file
  The Dxe drvier is used to update crashlog variable with the data from HOB

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

//
// Statements that include other header files
//
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>

#include <CrashLogVariable.h>


/**
  Entry point of the CrashLog variable update driver.

  @param[in] ImageHandle  EFI_HANDLE: A handle for the image that is initializing this driver
  @param[in] SystemTable  EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval  EFI_SUCCESS:           Driver initialized successfully
           EFI_LOAD_ERROR:        Failed to Initialize or to Load the driver
           EFI_OUT_OF_RESOURCES:  Could not allocate needed resources
**/
EFI_STATUS
EFIAPI
CrashLogVariableUpdate (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS         Status;
  EFI_HOB_GUID_TYPE  *GuidHob;
  VOID               *VariableHob;
  VOID               *Variable;
  UINTN              VarSize;
  UINT32             VarAttr;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  //
  // Get Variable hob and set to CRASHLOG_VARIABLE
  //
  GuidHob = GetFirstGuidHob (&gCrashLogVariableGuid);
  if (GuidHob != NULL) {
    VariableHob = (CRASHLOG_VARIABLE *) GET_GUID_HOB_DATA (GuidHob);

    VarSize = sizeof (CRASHLOG_VARIABLE);
    Variable = AllocateZeroPool (VarSize);
    if (Variable == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate resource for %S\n", CRASHLOG_VARIABLE_NAME));
      return EFI_OUT_OF_RESOURCES;
    }
    Status = gRT->GetVariable (
                    CRASHLOG_VARIABLE_NAME,
                    &gCrashLogVariableGuid,
                    &VarAttr,
                    &VarSize,
                    Variable
                    );
    if (!EFI_ERROR (Status) &&
         CompareMem (Variable, VariableHob, sizeof (CRASHLOG_VARIABLE))) {
      CopyMem (Variable, VariableHob, sizeof (CRASHLOG_VARIABLE));
      Status = gRT->SetVariable(
                      CRASHLOG_VARIABLE_NAME,
                      &gCrashLogVariableGuid,
                      VarAttr,
                      VarSize,
                      Variable
                      );
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "update Crashlog variable\n"));
      }
    }
    FreePool (Variable);
  }
  GuidHob = NULL;
  //
  // Get crash records efi page count hob and set to CRASHLOG_RECORD_PAGES
  //
  GuidHob = GetFirstGuidHob (&gCrashLogRecordPagesGuid);
  if (GuidHob != NULL) {
    VariableHob = (CRASHLOG_RECORD_PAGES *) GET_GUID_HOB_DATA (GuidHob);

    VarSize = sizeof (CRASHLOG_RECORD_PAGES);
    Variable = AllocateZeroPool (VarSize);
    if (Variable == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate resource for %S\n", CRASHLOG_RECORDPAGES_NAME));
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (Variable, VariableHob, sizeof (CRASHLOG_RECORD_PAGES));
    Status = gRT->SetVariable(
                    CRASHLOG_RECORDPAGES_NAME,
                    &gCrashLogRecordPagesGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    VarSize,
                    Variable
                    );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Update crashlog record pages variable.\n"));
    }
    FreePool (Variable);
  }

  GuidHob = NULL;
  //
  // Get control Variable hob and set to CRASHLOG_CONTROL_VARIABLE
  //
  GuidHob = GetFirstGuidHob (&gCrashLogCtlVariableGuid);
  if (GuidHob != NULL) {
    VariableHob = GET_GUID_HOB_DATA (GuidHob);

    VarSize = sizeof (CRASHLOG_CONTROL_VARIABLE);
    Variable = AllocateZeroPool (VarSize);
    if (Variable == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate resource for %S\n", CRASHLOG_CONTROL_VARIABLE_NAME));
      return EFI_OUT_OF_RESOURCES;
    }

    Status = gRT->GetVariable (
                    CRASHLOG_CONTROL_VARIABLE_NAME,
                    &gCrashLogCtlVariableGuid,
                    &VarAttr,
                    &VarSize,
                    Variable
                    );
    if (!EFI_ERROR (Status) &&
         CompareMem (Variable, VariableHob, sizeof (CRASHLOG_CONTROL_VARIABLE))) {
      CopyMem (Variable, VariableHob, sizeof (CRASHLOG_CONTROL_VARIABLE));
      Status = gRT->SetVariable(
                      CRASHLOG_CONTROL_VARIABLE_NAME,
                      &gCrashLogCtlVariableGuid,
                      VarAttr,
                      VarSize,
                      Variable
                  );
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "update Crashlog control variable\n"));
      }
    }
    FreePool (Variable);
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}