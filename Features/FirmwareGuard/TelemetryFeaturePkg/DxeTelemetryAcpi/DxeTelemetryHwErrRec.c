/** @file
  Dxe Telemetry HwErrRec Lib implementation.

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
#include "DxeTelemetryAcpi.h"
#include "DxeTelemetryHwErrRec.h"

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                  mIsFirstCall                 = TRUE;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                    *mMotherBoardHealthRecBuffer = NULL;

/**

  Clear all the Telemetry HwErrRec entries on flash.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.

**/
EFI_STATUS
EFIAPI
ClearAllTelemetryHwErrRec (
  VOID
  )
{
  UINT32                Index;
  UINT32                OffsetOfList;
  CHAR16                VarName[TELEMETRY_HW_ERR_REC_VAR_NAME_LEN];
  EFI_STATUS            Status;
  UINT8                 *VariableData;
  EFI_GUID              SectionType;
  BOOLEAN               IsDriverHealth;

  Index          = 0;
  OffsetOfList   = 0;
  Status         = EFI_SUCCESS;
  VariableData   = NULL;
  IsDriverHealth = FALSE;

  DEBUG ((DEBUG_INFO, "ClearAllTelemetryHwErrRec entered\n"));

  for (Index = 0; Index <= MAX_UINT16; Index++) {
    UnicodeSPrint (VarName, sizeof(VarName), L"%s%04X", TELEMETRY_HW_ERR_REC_VAR_NAME, Index);
    DEBUG ((DEBUG_INFO,"Variablesearch = %s\n",VarName));

    Status = GetVariable2 (VarName, &gEfiHardwareErrorVariableGuid, (VOID **) &VariableData, NULL);
    if (Status == EFI_NOT_FOUND) {
      //
      // There is no this HwErrRec Variable
      //
      break;
    } else if (EFI_ERROR (Status) || (VariableData == NULL)) {
      DEBUG ((DEBUG_ERROR, "Failed to get HwErrRecVariable data.  Status = %r\n", Status));
      return Status;
    }
    CopyGuid (&SectionType, &((EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD *) VariableData)->SectionDescriptor.SectionType);
    //
    // Clear variable when the variable SectionType is belong to mDriverHealthSupportList.
    //
    for (OffsetOfList = 0; mDriverHealthSupportList[OffsetOfList] != NULL; OffsetOfList++ ) {
      if (CompareGuid (&SectionType, mDriverHealthSupportList[OffsetOfList])) {
        IsDriverHealth = TRUE;
        break;
      }
    }
    if (IsDriverHealth == TRUE) {
      Status = gRT->SetVariable (
                      VarName,
                      &gEfiHardwareErrorVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE |
                      EFI_VARIABLE_BOOTSERVICE_ACCESS |
                      EFI_VARIABLE_RUNTIME_ACCESS |
                      EFI_VARIABLE_HARDWARE_ERROR_RECORD,
                      0,
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, " Clear HwErrRec has an issue...\n"));
        break;
      }
    }
  }
  if (Status == EFI_NOT_FOUND) {
    Status = EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "ClearAllTelemetryHwErrRec exit...\n"));
  return Status;
}

/**

  This routine accepts the pointer to a UINT16 number. It will iterate through each HwErrRecXXXX and stops
  after 0xFFFF iterations or spotted a slot that returns EFI_NOT_FOUND.

  @param[out]  NextVariable      The pointer to output result holder

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_INVALID_PARAMETER  NextVariable is NULL.
  @retval EFI_NOT_READY          gRT or gRt->GetVariable fuction is NULL.
  @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.

**/
EFI_STATUS
TelemetryFindNextAvailableSlot (
  OUT UINT16      *NextVariable
  )
{
  EFI_STATUS        Status;
  UINT32            Index;
  UINTN             VariableSize;
  CHAR16            VarName[TELEMETRY_HW_ERR_REC_VAR_NAME_LEN];

  Status       = EFI_SUCCESS;
  Index        = 0;
  VariableSize = 0;

  if (NextVariable == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  if ((gRT == NULL) || (gRT->GetVariable == NULL)) {
    Status = EFI_NOT_READY;
    goto Cleanup;
  }

  for (Index = 0; Index <= MAX_UINT16; Index++) {
    VariableSize = 0;
    UnicodeSPrint (VarName, sizeof(VarName), L"%s%04X", TELEMETRY_HW_ERR_REC_VAR_NAME, Index);
    Status = gRT->GetVariable (
                    VarName,
                    &gEfiHardwareErrorVariableGuid,
                    NULL,
                    &VariableSize,
                    NULL
                    );
    if (Status == EFI_NOT_FOUND) {
      break;
    }
  }
  //
  // Translate result corresponds to this specific function
  //
  if (Status == EFI_NOT_FOUND) {
    *NextVariable = (UINT16)Index;
    Status = EFI_SUCCESS;
  } else if (Status == EFI_BUFFER_TOO_SMALL) {
    Status = EFI_OUT_OF_RESOURCES;
  }

Cleanup:
  return Status;
}

/**
  Returns the SMBIOS table with the specified type.

  @param[in]        Type            The type of the SMBIOS table which is searched.
  @param[out]       Record          If the functions finishes with success, then it will point the found SMBIOS Record.

  @retval EFI_SUCCESS               Record was found.
  @retval EFI_INVALID_PARAMETER     Table pointer is NULL.
  @retval EFI_NOT_FOUND             Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN     SMBIOS_TYPE              Type,
     OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  EFI_SMBIOS_PROTOCOL        *Smbios;
  EFI_SMBIOS_TABLE_HEADER    *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  Init HwErr Header data.

  @param[in out]   HwErrRecData    Point to the guid buffer of EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_INVALID_PARAMETER  HwErrRecData is NULL.
**/
EFI_STATUS
InitHwErrHeader(
  IN OUT EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD   *HwErrRecData
  )
{
  EFI_STATUS                Status;
  EFI_TIME                  Time;
  EFI_GUID                  Uuid;
  EFI_GUID                  ZeroGuid;
  UINT8                     Thousand;
  UINT8                     Hundred;
  SMBIOS_TABLE_TYPE1        *SmbiosTableType1Data;

  Status = EFI_SUCCESS;

  if (HwErrRecData == NULL) {
    DEBUG ((DEBUG_ERROR, "InitHwErrHeader : Invalid Parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem (HwErrRecData, sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD));
  ZeroMem (&Time, sizeof (EFI_TIME));
  ZeroMem (&Uuid, sizeof (EFI_GUID));
  ZeroMem (&ZeroGuid, sizeof (EFI_GUID));

  //
  //  SignatureStart is CPER
  //
  HwErrRecData->Header.SignatureStart = ERROR_RECORD_HEADER_SIGNATURE_START;
  HwErrRecData->Header.Revision       = ERROR_RECORD_HEADER_REVISION;
  HwErrRecData->Header.SignatureEnd   = ERROR_RECORD_HEADER_SIGNATURE_END;
  HwErrRecData->Header.SectionCount   = ERROR_RECORD_HEADER_SIGNATURE_COUNT;
  HwErrRecData->Header.ErrorSeverity  = ERROR_RECORD_HEADER_ERRORSEVERITY;
  HwErrRecData->Header.ValidationBits = ERROR_RECORD_HEADER_VALIDATIONBITS;
  //
  // Get time
  //
  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status) || (Time.Year < 1970)) {
    DEBUG ((DEBUG_ERROR, "Get Time failed.\n"));
  }
  HwErrRecData->Header.TimeStamp.Seconds = Time.Second;
  HwErrRecData->Header.TimeStamp.Minutes = Time.Minute;
  HwErrRecData->Header.TimeStamp.Hours   = Time.Hour;
  HwErrRecData->Header.TimeStamp.Flag    = 0;
  HwErrRecData->Header.TimeStamp.Day     = Time.Day;
  HwErrRecData->Header.TimeStamp.Month   = Time.Month;
  HwErrRecData->Header.TimeStamp.Year    = (UINT8) (Time.Year % 100);
  Thousand                               = (UINT8) ((Time.Year / 1000) %10);
  Hundred                                = (UINT8) ((Time.Year / 100) %10);
  HwErrRecData->Header.TimeStamp.Century = Thousand * 10 + Hundred + 1;

  //
  // Get UUID from the smbios table
  //
  Status = GetSmbiosTable (SMBIOS_TYPE_SYSTEM_INFORMATION, (EFI_SMBIOS_TABLE_HEADER **)&SmbiosTableType1Data);
  if (Status == EFI_SUCCESS) {
    CopyMem (&Uuid, &SmbiosTableType1Data->Uuid, sizeof (EFI_GUID));
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to read system GUID from the smbios table! Status = %r\n", Status));
    ZeroMem (&Uuid, sizeof (EFI_GUID));
  }

  CopyGuid (&HwErrRecData->Header.PlatformID, &Uuid);
  CopyGuid (&HwErrRecData->Header.CreatorID, &Uuid);
  CopyGuid (&HwErrRecData->Header.PartitionID, &ZeroGuid);
  HwErrRecData->Header.NotificationType = gEfiEventNotificationTypeBootGuid;
  HwErrRecData->Header.Flags            = ERROR_RECORD_HEADER_FLAGS;

  HwErrRecData->SectionDescriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  HwErrRecData->SectionDescriptor.Revision      = ERROR_RECORD_SECTION_DESCRIPTOR_REVISION;
  HwErrRecData->SectionDescriptor.SecValidMask  = ERROR_RECORD_SECTION_DESCRIPTOR_SECVALIDMASK;
  HwErrRecData->SectionDescriptor.SectionFlags  = ERROR_RECORD_SECTION_DESCRIPTOR_SECTIONFLAGS;
  HwErrRecData->SectionDescriptor.Severity      = ERROR_RECORD_SECTION_DESCRIPTOR_SEVERITY;

  return EFI_SUCCESS;
}

/**

  This function set DriverHealth HwErrRec Variable.
  If the current variable size is greater than the MaximumVariableSize
  current HwErrRec Variable will not be created.

  @param[in]      AcpiPhatTablePointer  A pointer to the EFI_ACPI_DESCRIPTION_HEADER instance.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
SetDriverHealthHwErrRecVariable (
  IN     EFI_ACPI_DESCRIPTION_HEADER     *AcpiPhatTablePointer
  )
{
  EFI_ACPI_6_5_PHAT_RECORD   *AcpiTelemetryHeader;
  UINTN                                       BufferSize;
  EFI_GUID                                    ComponentID;
  EFI_STATUS                                  Status;
  UINT8                                       *PhatPointer;
  UINT8                                       *HealthRecord;
  UINTN                                       TotalRecordLength;
  UINTN                                       TelemetryDataLength;
  EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD      HwErrRecData;
  UINT8                                       *DriverHealthRecData;
  UINT32                                      DevicePathOffset;
  CHAR16                                      VariableName[TELEMETRY_HW_ERR_REC_VAR_NAME_LEN];
  UINT16                                      NextVariable;
  UINT64                                      MaximumVariableStorageSize;
  UINT64                                      RemainingVariableStorageSize;
  UINT64                                      MaximumVariableSize;
  EFI_TELEMETRY_HEALTH_RECORD                 SectionRecord;

  BufferSize          = 0;
  TotalRecordLength   = 0;
  DriverHealthRecData = NULL;
  AcpiTelemetryHeader = NULL;

  if (AcpiPhatTablePointer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  ZeroMem (&SectionRecord, sizeof (EFI_TELEMETRY_HEALTH_RECORD));

  Status = InitHwErrHeader (&HwErrRecData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to initialize the data of HwErrRec header. Status = %r\n", Status));
    goto Done;
  }

  if (AcpiPhatTablePointer->Length > sizeof (EFI_ACPI_DESCRIPTION_HEADER)) {
    TelemetryDataLength  = AcpiPhatTablePointer->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER);
    if (TelemetryDataLength < sizeof (EFI_ACPI_6_5_PHAT_RECORD)) {
      DEBUG ((DEBUG_ERROR, "PHAT table is not existed or corrupted \n"));
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PHAT table is not existed or corrupted \n"));
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  PhatPointer = (UINT8 *) AcpiPhatTablePointer + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  for (TotalRecordLength = 0; TotalRecordLength < TelemetryDataLength - sizeof (EFI_ACPI_6_5_PHAT_RECORD);
      PhatPointer += AcpiTelemetryHeader->RecordLength, TotalRecordLength += AcpiTelemetryHeader->RecordLength) {
    AcpiTelemetryHeader = (EFI_ACPI_6_5_PHAT_RECORD *) PhatPointer;

    if (AcpiTelemetryHeader->RecordLength <= sizeof (EFI_ACPI_6_5_PHAT_RECORD)) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    //
    // Check Telemetry data RecordType.
    //
    if (AcpiTelemetryHeader->PlatformHealthAssessmentRecordType != FirmwareHealthDataRecord) {
      continue;
    }
    //
    // found driver health records
    //
    HealthRecord = (UINT8 *) AcpiTelemetryHeader;
    //
    // skip EFI_ACPI_6_5_PHAT_RECORD, Reserved and AmHealthy
    //
    CopyGuid (&ComponentID, &((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->DeviceSignature);
    //
    // Pre-allocate Buffer for MotherBoard Record When the function is first call.
    //
    if ((mIsFirstCall == TRUE) && ((CompareGuid (&ComponentID, &gAdapterInfoMotherBoardHealthGuid)))) {
       mMotherBoardHealthRecBuffer = AllocateZeroPool (sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD) +
                                                      ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->DeviceSpecificDataOffset);
      if (mMotherBoardHealthRecBuffer == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to process SetDriverHealthHwErrRecVariable due to lack of resources!\n"));
        Status = EFI_OUT_OF_RESOURCES;
        goto Done;
      }
      continue;
    }
    //
    // check if it's Unhealthy Data records.
    //
    if (((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->AmHealthy != TelemetryUnhealthy) {
      continue;
    }
    //
    // Get the the maximum size of an individual EFI variable in current system
    //
    Status = gRT->QueryVariableInfo (
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_HARDWARE_ERROR_RECORD,
                    &MaximumVariableStorageSize,
                    &RemainingVariableStorageSize,
                    &MaximumVariableSize
                    );
    if (Status != EFI_SUCCESS) {
      goto Done;
    }

    DevicePathOffset = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->DeviceSpecificDataOffset;
    CopyGuid (&HwErrRecData.SectionDescriptor.SectionType, &ComponentID);
    HwErrRecData.SectionDescriptor.SectionLength = DevicePathOffset;
    BufferSize = sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD) + DevicePathOffset;
    if (BufferSize > MaximumVariableSize) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }
    HwErrRecData.Header.RecordLength = (UINT32) BufferSize;
    //
    // Skip other devices when the function is not called for the first time.
    //
    if (mIsFirstCall == FALSE) {
      if (CompareGuid (&ComponentID, &gAdapterInfoMotherBoardHealthGuid)) {
        //
        // DriverHealthRecData Point to pre-allocated buffer.
        //
        DriverHealthRecData = mMotherBoardHealthRecBuffer;
      } else {
        continue;
      }
    } else {
      DriverHealthRecData = AllocateZeroPool (BufferSize);
    }
    if (DriverHealthRecData == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to process SetDriverHealthHwErrRecVariable due to lack of resources!\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Done;
    }

    //    telemetry HwErrCper record header
    //    ------------------------------------------------------------|
    //   | field                          | Byte Length | Byte offset |
    //   |------------------------------------------------------------|
    //   | Record Length                  | 2           | 0           |
    //   |------------------------------------------------------------|
    //   | Revision                       | 1           | 2           |
    //   |------------------------------------------------------------|
    //   | Reserved                       | 4           | 3           |
    //   |------------------------------------------------------------|
    //   | AmHealthy                      | 1           | 7           |
    //   -------------------------------------------------------------|
    //   | DeviceSignature                | 16          | 8           |
    //   -------------------------------------------------------------|
    //   | DeviceSpecificDataOffset       | 4           | 24          |
    //   -------------------------------------------------------------|
    //
    SectionRecord.RecordLength             = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->RecordLength;
    SectionRecord.Revision                 = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->Revision;
    SectionRecord.Reserved                 = 0;
    SectionRecord.AmHealthy                = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->AmHealthy;
    SectionRecord.DeviceSignature          = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->DeviceSignature;
    SectionRecord.DeviceSpecificDataOffset = ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) AcpiTelemetryHeader)->DeviceSpecificDataOffset;
    //
    // Find next HwErrRec Variable name.
    //
    Status = TelemetryFindNextAvailableSlot (&NextVariable);
    if (Status != EFI_SUCCESS) {
      goto Done;
    }
    UnicodeSPrint (VariableName, sizeof(VariableName), L"%s%04X", TELEMETRY_HW_ERR_REC_VAR_NAME, NextVariable );
    HwErrRecData.Header.RecordID = NextVariable;
    //
    // Copy telemetry HwErr record data to the buffer.
    //
    CopyMem (DriverHealthRecData, (UINT8 *) &HwErrRecData, sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD));
    //
    // Copy telemetry HwErrCper record header to the buffer.
    //
    CopyMem (DriverHealthRecData + sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD),
            (UINT8 *) &SectionRecord, sizeof (EFI_TELEMETRY_HEALTH_RECORD));
    //
    // Copy device path to the buffer.
    //
    CopyMem (DriverHealthRecData + sizeof (EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD) + sizeof (EFI_TELEMETRY_HEALTH_RECORD),
            HealthRecord + sizeof (EFI_TELEMETRY_HEALTH_RECORD), DevicePathOffset - sizeof (EFI_TELEMETRY_HEALTH_RECORD)); // lijie error
    //
    // Set DriverHealth HwErrRec Variable.
    //
    Status = gRT->SetVariable (
                    VariableName,
                    &gEfiHardwareErrorVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_HARDWARE_ERROR_RECORD,
                    BufferSize,
                    DriverHealthRecData
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set the DriverHealthRecData Health record into a HwErrRec#### variable.  Status = %r\n", Status));
      goto Done;
    }

    if (DriverHealthRecData != NULL) {
      FreePool (DriverHealthRecData);
      DriverHealthRecData = NULL;
    }
  }
Done:
  if (DriverHealthRecData != NULL) {
    FreePool (DriverHealthRecData);
  }
  mIsFirstCall = FALSE;

  return Status;
}
