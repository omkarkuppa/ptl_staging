/** @file
  Dxe Telemetry Lib implementation.

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
#include "DxeTelemetryFviLib.h"

//
// Buffer for Adapter Information
//
EFI_AIP_TELEMETRY_VERSION_RECORD *mFviAipBuffer = NULL;
EFI_HANDLE                       mFviHandle     = NULL;
EFI_EVENT                        mAcpiEvent     = NULL;
EFI_GUID                         *mPlatformFviReadyProtocolGuid = NULL;

/**
  Get a list of supported information types for this instance of the protocol.

  This function returns a list of InformationType GUIDs that are supported on an
  adapter with this instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned
  in InfoTypesBuffer, and the number of GUID pointers in InfoTypesBuffer is returned in
  InfoTypesBufferCount.

  @param[in]  This                  A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] InfoTypesBuffer       A pointer to the array of InformationType GUIDs that are supported
                                    by This.
  @param[out] InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.

  @retval EFI_SUCCESS               The list of information type GUIDs that are supported on this adapter was
                                    returned in InfoTypesBuffer. The number of information type GUIDs was
                                    returned in InfoTypesBufferCount.
  @retval EFI_INVALID_PARAMETER     This is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBuffer is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBufferCount is NULL.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
EFI_STATUS
EFIAPI
FviAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  if ((This == NULL) || (InfoTypesBuffer == NULL) || (InfoTypesBufferCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InfoTypesBuffer = AllocateCopyPool (sizeof (gTelemetryVersionRecord2Guid), &gTelemetryVersionRecord2Guid);
  if (*InfoTypesBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

/**
  Returns the current state information for the adapter.

  This function returns information of type InformationType from the adapter.
  If an adapter does not support the requested informational type, then
  EFI_UNSUPPORTED is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[out] InformationBlock       The service returns a pointer to the buffer with the InformationBlock
                                     structure which contains details about the data specific to InformationType.
  @param[out] InformationBlockSize   The driver returns the size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The InformationType information was retrieved.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_OUT_OF_RESOURCES       The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlockSize is NULL.

**/
EFI_STATUS
EFIAPI
FviAipGetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL) || (InformationBlockSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gTelemetryVersionRecord2Guid)) {
    return EFI_UNSUPPORTED;
  }

  *InformationBlockSize = sizeof(EFI_AIP_TELEMETRY_VERSION_RECORD) + sizeof(EFI_ACPI_6_5_PHAT_VERSION_ELEMENT) * mFviAipBuffer->RecordCount;
  *InformationBlock = (EFI_AIP_TELEMETRY_VERSION_RECORD *) AllocateZeroPool (*InformationBlockSize);
  if (*InformationBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (*InformationBlock, mFviAipBuffer, *InformationBlockSize);

  return EFI_SUCCESS;

}

/**
  Sets state information for an adapter.

  This function sends information of type InformationType for an adapter.
  If an adapter does not support the requested information type, then EFI_UNSUPPORTED
  is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[in]  InformationBlock       A pointer to the InformationBlock structure which contains details
                                     about the data specific to InformationType.
  @param[in]  InformationBlockSize   The size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The information was received and interpreted successfully.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED        The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().

**/
EFI_STATUS
EFIAPI
FviAipSetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gTelemetryVersionRecord2Guid)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Process the full partition table data and add individual PHAT entries.

  @param[in]     PartitionData    Pointer to the array of partition data from HECI
  @param[in]     NumOfModules     Number of partitions in the data
  @param[in]     ProducerId       Producer ID for the entries
  @param[in,out] RecordCount      Current record count, updated with new entries

  @retval EFI_SUCCESS           Partition table processed successfully
  @retval EFI_INVALID_PARAMETER Invalid input parameters
  @retval EFI_NOT_FOUND         No valid partitions found
**/
STATIC
EFI_STATUS
EFIAPI
ProcessFviPartitionTable (
  IN     FLASH_PARTITION_DATA  *PartitionData,
  IN     UINT32                NumOfModules,
  IN     UINT32                ProducerId,
  IN OUT UINT64                *RecordCount
  )
{
  STATIC FVI_PARTITION_MAPPING  mPartitionMappings[] = FVI_PARTITION_MAPPING_TABLE;
  UINT32                        ProcessedEntries;
  UINT32                        i;
  UINT32                        j;
  UINT64                        StartingRecordCount;
  UINT32                        CurrentPartitionId;
  UINT64                        RecordIndex;

  RecordIndex         = 0;
  ProcessedEntries    = 0;

  if ((PartitionData == NULL) || (RecordCount == NULL) || (NumOfModules == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  StartingRecordCount = *RecordCount;
  ZeroMem (&mFviAipBuffer->Record[StartingRecordCount], FVI_PARTITION_MAPPING_COUNT * sizeof(EFI_ACPI_6_5_PHAT_VERSION_ELEMENT));

  for (i = 0; i < NumOfModules && ProcessedEntries < FVI_PARTITION_MAPPING_COUNT; i++) {
    CurrentPartitionId = PartitionData[i].PartitionId;

    for (j = 0; j < FVI_PARTITION_MAPPING_COUNT; j++) {
      if (mPartitionMappings[j].PartitionId == CurrentPartitionId) {
        RecordIndex = StartingRecordCount + j;
        CopyGuid (&mFviAipBuffer->Record[RecordIndex].ComponentId, mPartitionMappings[j].ComponentGuid);

        // Format version from FLASH_PARTITION_DATA
        mFviAipBuffer->Record[RecordIndex].VersionValue =
          (LShiftU64 ((UINT64) PartitionData[i].Version.Major,  VERSION_MAJOR_VERSION_SHIFT) | \
           LShiftU64 ((UINT64) PartitionData[i].Version.Minor,  VERSION_MINOR_VERSION_SHIFT) | \
           LShiftU64 ((UINT64) PartitionData[i].Version.Hotfix, VERSION_REVISION_SHIFT)      | \
                     ((UINT16) PartitionData[i].Version.Build));

        mFviAipBuffer->Record[RecordIndex].ProducerId = ProducerId;
        ProcessedEntries++;

        DEBUG ((DEBUG_VERBOSE, "[%a] Found partition 0x%08X (%c%c%c%c) at HECI index %u, mapped to PHAT entry %llu\n",
                __FUNCTION__, CurrentPartitionId,
                (CHAR8) (CurrentPartitionId & 0xFF), (CHAR8)((CurrentPartitionId >> 8) & 0xFF),
                (CHAR8) ((CurrentPartitionId >> 16) & 0xFF), (CHAR8)((CurrentPartitionId >> 24) & 0xFF),
                i, RecordIndex));
        break;
      }
    }
  }

  // Update record count to include all allocated slots (found + zero entries)
  *RecordCount = StartingRecordCount + FVI_PARTITION_MAPPING_COUNT;
  return EFI_SUCCESS;
}

/**
  Create an Fvi AIP record when the Fvi AIP record is empty,
  otherwise Append an entry after it.

  @param[in] ComponentId      ComponentId for the Fvi entry.
  @param[in] Version          Version for Fvi entry.
  @param[in] ProducerId       ProducerId for Fvi entry, e.g. 'INTC'.

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
**/
EFI_STATUS
EFIAPI
AppendTelemetryFviBlock (
  IN  EFI_GUID  ComponentId,
  IN  UINT64    Version,
  IN  UINT32    ProducerId
  )
{
  UINTN                 OldBufferSize;
  UINTN                 NewBufferSize;
  UINT64                RecordCount;
  EFI_STATUS            Status;
  UINT32                NumOfModules;
  UINT32                EntriesToAdd;
  VOID                  *GenericPartitionData;
  FLASH_PARTITION_DATA  *PartitionData;

  NumOfModules = 0;
  EntriesToAdd = 1;

  //
  // Get the FW Version data
  //
  if (CompareGuid (&ComponentId, &gFviFullPartitionTableComponentId)) {
    Status = GetFullFviPartitionTable (&GenericPartitionData, &NumOfModules);
    if (EFI_ERROR (Status) || NumOfModules == 0) {
      DEBUG ((DEBUG_WARN, "[%a] No partition data received from HECI call\n", __FUNCTION__));
      return EFI_ABORTED;
    }
    PartitionData = (FLASH_PARTITION_DATA *) GenericPartitionData;
    EntriesToAdd = FVI_PARTITION_MAPPING_COUNT;
  } else if (CompareGuid (&ComponentId, &gMeFirmwareVersionComponentId)) {
    Status = TelemetryFirmwareVersionUpdate (ComponentId, &Version);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[%a] Fail to update Fvi data: %r\n", __FUNCTION__, Status));
    }
  }

  //
  // Update the FVI AIP buffer to hold new data
  //
  if (mFviAipBuffer == NULL) {
    RecordCount = 0;
    NewBufferSize = sizeof (EFI_AIP_TELEMETRY_VERSION_RECORD) + sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT) * EntriesToAdd;
    mFviAipBuffer = (EFI_AIP_TELEMETRY_VERSION_RECORD *) AllocateZeroPool (NewBufferSize);
    if (mFviAipBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to allocate new buffer\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }
    mFviAipBuffer->Revision = 1;
  } else {
    RecordCount = mFviAipBuffer->RecordCount;
    OldBufferSize = sizeof (EFI_AIP_TELEMETRY_VERSION_RECORD) + sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT) * RecordCount;
    NewBufferSize = sizeof (EFI_AIP_TELEMETRY_VERSION_RECORD) + sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT) * (RecordCount + EntriesToAdd);
    mFviAipBuffer = (EFI_AIP_TELEMETRY_VERSION_RECORD *) ReallocatePool (OldBufferSize, NewBufferSize, (VOID *) mFviAipBuffer);
    if (mFviAipBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to reallocate buffer\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }
  }

  //
  // Handle the Full partition table data parsing if applicable
  //
  if (CompareGuid (&ComponentId, &gFviFullPartitionTableComponentId)) {
    if (NumOfModules == 0) {
      DEBUG ((DEBUG_WARN, "[%a] No partition data received\n", __FUNCTION__));
      return EFI_ABORTED;
    }
    //
    // Process the full partition table using dedicated handler
    //
    Status = ProcessFviPartitionTable (PartitionData, NumOfModules, ProducerId, &RecordCount);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to process FVI partition table: %r\n", __FUNCTION__, Status));
      if (PartitionData != NULL) {
        FreePool (PartitionData);
        PartitionData = NULL;
      }
      return Status;
    }

    //
    // RecordCount is already updated by ProcessFviPartitionTable to include all allocated slots
    //
    mFviAipBuffer->RecordCount = RecordCount;

    if (PartitionData != NULL) {
      FreePool (PartitionData);
      PartitionData = NULL;
    }
  } else {
    CopyGuid (&mFviAipBuffer->Record[RecordCount].ComponentId, &ComponentId);
    mFviAipBuffer->Record[RecordCount].VersionValue = Version;
    mFviAipBuffer->Record[RecordCount].ProducerId   = ProducerId;
    mFviAipBuffer->RecordCount = RecordCount + 1;
  }

  // Signal ACPI table rebuild
  gBS->SignalEvent (mAcpiEvent);

  return EFI_SUCCESS;
}

/**
  Get Platform Firmware Version Information.

  @param[in] Event    The Event this notify function registered to.
  @param[in] Context  Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
InitializeFviAipCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                       Status;
  VOID                             *Interface;
  EFI_ADAPTER_INFORMATION_PROTOCOL *AdapterInfo;

  AdapterInfo = NULL;

  Status = gBS->LocateProtocol (mPlatformFviReadyProtocolGuid, NULL, &Interface);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Locate protocol (%g) failed = %r.\n", __FUNCTION__, mPlatformFviReadyProtocolGuid, Status));
    return;
  }

  gBS->CloseEvent (Event);

  AdapterInfo = AllocateZeroPool (sizeof (EFI_ADAPTER_INFORMATION_PROTOCOL));
  if (AdapterInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_OUT_OF_RESOURCES to Allocate EFI_ADAPTER_INFORMATION_PROTOCOL. \n"));
    return;
  }

  AdapterInfo->GetInformation    = FviAipGetInfo;
  AdapterInfo->SetInformation    = FviAipSetInfo;
  AdapterInfo->GetSupportedTypes = FviAipGetSupportedTypes;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mFviHandle,
                  &gEfiAdapterInformationProtocolGuid,
                  AdapterInfo,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Platform Fvi: faild to install AIP protocol, %r.\n", Status));
  }

  return;
}

/**
  Notification function of ACPI Table change.
  This is a notification function registered on ACPI Table change event.

  @param[in]  Event        Event whose notification function is being invoked.
  @param[in]  Context      Pointer to the notification function's context.
**/
VOID
EFIAPI
AcpiUpdateNotificationFunc (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  return;
}

/**
  This is driver entry point to register the notification event.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
DxeTelemetryFviLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_EVENT          GetPlatformFviEvent;
  EFI_STATUS         Status;
  VOID               *Registration;

  GetPlatformFviEvent = NULL;

  if (PcdGetSize (PcdPlatformFviReadyProtocolGuid) == sizeof (EFI_GUID)) {
    mPlatformFviReadyProtocolGuid = (EFI_GUID *) PcdGetPtr (PcdPlatformFviReadyProtocolGuid);
  }

  if (mPlatformFviReadyProtocolGuid != NULL) {

    GetPlatformFviEvent = EfiCreateProtocolNotifyEvent (
                            mPlatformFviReadyProtocolGuid,
                            TPL_CALLBACK,
                            InitializeFviAipCallback,
                            NULL,
                            &Registration
                            );

  }

  if (GetPlatformFviEvent == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Create callback for protocol notify (%g) failed.\n", __FUNCTION__, mPlatformFviReadyProtocolGuid));
  }

  // Register PHAT rebuild event
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiUpdateNotificationFunc,
                  NULL,
                  &gPlatformAcpiUpdateEventGuid,
                  &mAcpiEvent
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  This is driver unload entry point to uninstall AIP.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
DxeTelemetryFviLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                       Status;
  EFI_ADAPTER_INFORMATION_PROTOCOL *AdapterInfo;

  DEBUG ((DEBUG_INFO, "DxeTelemetryFviLibDestructor is called!\n"));

  Status = gBS->HandleProtocol (
                  mFviHandle,
                  &gEfiAdapterInformationProtocolGuid,
                  (VOID **) &AdapterInfo
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->UninstallProtocolInterface (
                    mFviHandle,
                    &gEfiAdapterInformationProtocolGuid,
                    AdapterInfo
                    );
  }

  // Signal PHAT table rebuild and cleanup
  gBS->SignalEvent (mAcpiEvent);
  gBS->CloseEvent (mAcpiEvent);

  if (AdapterInfo != NULL) {
    FreePool (AdapterInfo);
  }

  if (mFviAipBuffer != NULL) {
    FreePool (mFviAipBuffer);
  }

  return EFI_SUCCESS;
}
