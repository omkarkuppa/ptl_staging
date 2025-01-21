/** @file
  This is the driver that locates the MemoryConfigurationData HOB, if it
  exists, and saves the data to NVRAM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "SaveMemoryConfig.h"

GLOBAL_REMOVE_IF_UNREFERENCED CHAR16    mMemoryConfigVariable[] = L"MemoryConfig";

/**
  This is the standard EFI driver point that detects whether there is a
  MemoryConfigurationData HOB and, if so, saves its data to NVRAM.

  @param[in] ImageHandle    Handle for the image of this driver
  @param[in] SystemTable    Pointer to the EFI System Table

  @retval    EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SaveMemoryConfigEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_HOB_GUID_TYPE                   *GuidHob;
  VOID                                *HobData;
  VOID                                *VariableData;
  UINTN                               DataSize;
  UINTN                               BufferSize;
  EDKII_VARIABLE_POLICY_PROTOCOL      *VariablePolicy = NULL;

  DataSize        = 0;
  BufferSize      = 0;
  VariableData    = NULL;
  GuidHob         = NULL;
  HobData         = NULL;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  // Firstly check version2 FspNvsHob.
  //
  GuidHob = GetFirstGuidHob (&gFspNonVolatileStorageHob2Guid);
  if (GuidHob != NULL) {
    HobData = (VOID *) (UINTN) ((FSP_NON_VOLATILE_STORAGE_HOB2 *) (UINTN) GuidHob)->NvsDataPtr;
    DataSize = (UINTN) ((FSP_NON_VOLATILE_STORAGE_HOB2 *) (UINTN) GuidHob)->NvsDataLength;
  } else {
    //
    // Fall back to version1 FspNvsHob
    //
    GuidHob = GetFirstGuidHob (&gFspNonVolatileStorageHobGuid);
    if (GuidHob != NULL) {
      HobData  = GET_GUID_HOB_DATA (GuidHob);
      DataSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
    }
  }

  if (HobData != NULL) {
    DEBUG ((DEBUG_INFO, "FspNvsHob.NvsDataLength:%d\n", DataSize));
    DEBUG ((DEBUG_INFO, "FspNvsHob.NvsDataPtr   : 0x%x\n", HobData));
    if (DataSize > 0) {
      //
      // Use the HOB to save Memory Configuration Data
      //
      BufferSize = DataSize;
      VariableData = AllocatePool (BufferSize);
      if (VariableData == NULL) {
        return EFI_UNSUPPORTED;
      }
      Status = gRT->GetVariable (
                      mMemoryConfigVariable,
                      &gMemoryConfigVariableGuid,
                      NULL,
                      &BufferSize,
                      VariableData
                      );

      if (Status == EFI_BUFFER_TOO_SMALL) {
        FreePool (VariableData);
        VariableData = AllocatePool (BufferSize);
        if (VariableData == NULL) {
          return EFI_UNSUPPORTED;
        }

        Status = gRT->GetVariable (
                        mMemoryConfigVariable,
                        &gMemoryConfigVariableGuid,
                        NULL,
                        &BufferSize,
                        VariableData
                        );
      }

      if ((EFI_ERROR (Status)) || (BufferSize != DataSize) || (0 != CompareMem (HobData, VariableData, DataSize))) {
        Status = gRT->SetVariable (
                        mMemoryConfigVariable,
                        &gMemoryConfigVariableGuid,
                        (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                        DataSize,
                        HobData
                        );
        ASSERT_EFI_ERROR (Status);

        DEBUG ((DEBUG_INFO, "Restored Size is 0x%x\n", DataSize));
      }

      //
      // Mark MemoryConfig to read-only if the Variable Lock protocol exists
      //
      Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID **)&VariablePolicy);
      DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
      if (!EFI_ERROR (Status)) {
        Status = RegisterBasicVariablePolicy (VariablePolicy,
                   &gMemoryConfigVariableGuid,
                   L"MemoryConfig",
                   VARIABLE_POLICY_NO_MIN_SIZE,
                   VARIABLE_POLICY_NO_MAX_SIZE,
                   VARIABLE_POLICY_NO_MUST_ATTR,
                   VARIABLE_POLICY_NO_CANT_ATTR,
                   VARIABLE_POLICY_TYPE_LOCK_NOW
                   );
        ASSERT_EFI_ERROR (Status);
      }

      //
      // Set the DISB bit
      // after S3 Data is saved to NVRAM.
      //
      PmcSetDramInitScratchpad ();

      FreePool (VariableData);
    } else {
      DEBUG ((DEBUG_INFO, "Memory save size is %d\n", DataSize));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Memory S3 Data HOB was not found\n"));
  }

  //
  // This driver does not produce any protocol services, so always unload it.
  //
  return EFI_UNSUPPORTED;
}
