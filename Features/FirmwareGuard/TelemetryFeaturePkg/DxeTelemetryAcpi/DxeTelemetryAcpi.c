/** @file
  This file implements firmware version information.

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

EFI_RSC_HANDLER_PROTOCOL                               *mRscHandlerProtocol = NULL;

UINT8   AcpiComPonentHeadSize = OFFSET_OF (EFI_COMPONENT_VERSION_DATA, Records);
UINT16  AcpiFviRecordSize     = sizeof (EFI_ACPI_6_5_PHAT_VERSION_ELEMENT);

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  *mDriverHealthSupportList[] = {
  &gAdapterInfoMotherBoardHealthGuid,
  &gAdapterInfoCsmeGuid,
  &gAdapterInfoMrcGuid,
  &gAdapterInfoNvmeHealthGuid,
  NULL
};

/**
  This function calculates and updates an UINT8 checksum.

  @param[in]  Buffer          Pointer to buffer to checksum
  @param[in]  Size            Number of bytes to checksum

**/
VOID
AcpiPlatformChecksumTelemetry (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF(EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
}

/**
  Collect driver health information by CommponentID(Guid).

  @param[in]       ComponentID          The unique GUID associated with this device.
  @param[out]      AcpiDataBlock        Point to the buffer of driver health Acpi record
  @param[out]      AcpiDataSize         length of driver health Acpi record

  @retval EFI_SUCCESS                   All driver health info was collected successfully
  @retval EFI_NOT_FOUND                 No handles match the search or no record was found.
  @retval EFI_OUT_OF_RESOURCES          There is not enough memory resource.

**/
EFI_STATUS
DriverHealthAipDataCollection (
  IN EFI_GUID                 *ComponentID,
  OUT UINT8                   **AcpiDataBlock,
  OUT UINTN                   *AcpiDataSize
  )
{
  EFI_STATUS                                           Status;
  EFI_ADAPTER_INFORMATION_PROTOCOL                     *Aip;
  UINTN                                                HandleCount;
  EFI_HANDLE                                           *HandleBuffer;
  EFI_GUID                                             *InfoTypesBuffer;
  UINTN                                                InfoTypeBufferCount;
  UINTN                                                Index;
  UINTN                                                TypeIndex;
  BOOLEAN                                              Supported;
  UINT8                                                *DataBlock;
  UINTN                                                BlockSize;
  EFI_AIP_HEALTH_HEADER                                *AipPtr;
  EFI_DEVICE_PATH_PROTOCOL                             *DevicePath;
  UINTN                                                SizeOfDevicePath;
  UINTN                                                DevSpecificDataOffset;
  EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE      *AcpiDataBlockHeader;
  UINTN                                                AcpiDataSizeTemp;
  CHAR16                                               *DevicePathText;
  UINT8                                                *AcpiDataBlockHead;

  InfoTypesBuffer       = NULL;
  Status                = EFI_SUCCESS;
  AipPtr                = NULL;
  DataBlock             = NULL;
  BlockSize             = 0;
  DevSpecificDataOffset = 0;
  DevicePathText        = NULL;
  SizeOfDevicePath      = 0;
  *AcpiDataBlock        = NULL;
  *AcpiDataSize         = 0;
  AcpiDataSizeTemp      = 0;
  AcpiDataBlockHead     = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiAdapterInformationProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // gEfiAdapterInformationProtocolGuid protocol not installed yet!
    //
    DEBUG ((DEBUG_ERROR, "gEfiAdapterInformationProtocolGuid locate failed, status = [%x] \n", Status));
    return Status;
  }
  if (HandleBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, " HandleBuffer is null \n"));
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < HandleCount ; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiAdapterInformationProtocolGuid, (VOID **) &Aip);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HandleProtocol gEfiAdapterInformationProtocolGuid failed. \n"));
      goto Exit;
    } else {
      InfoTypeBufferCount = 0;
      if (InfoTypesBuffer != NULL) {
        FreePool (InfoTypesBuffer);
        InfoTypesBuffer = NULL;
      }
      Status = Aip->GetSupportedTypes (Aip, &InfoTypesBuffer, &InfoTypeBufferCount);
      if (EFI_ERROR (Status) || (InfoTypesBuffer == NULL)) {
        continue;
      }

      //
      // Check whether the AIP instance has Fvi information block.
      //
      Supported = FALSE;
      for (TypeIndex = 0; TypeIndex < InfoTypeBufferCount; TypeIndex++) {
        if (CompareGuid (&InfoTypesBuffer[TypeIndex], ComponentID)) {
          Supported = TRUE;
          break;
        }
      }

      if (!Supported) {
        continue;
      }

      if ((DataBlock != NULL) && (BlockSize != 0)) {
        FreePool (DataBlock);
        DataBlock = NULL;
      }
      Status = Aip->GetInformation (Aip, ComponentID, (VOID **) &DataBlock, &BlockSize);
      DEBUG ((DEBUG_ERROR, "Aip->GetInformation, Status=[%x], data length=[%x]. \n", Status, BlockSize));
      if (EFI_ERROR (Status) || (DataBlock == NULL) || (BlockSize < sizeof (EFI_AIP_HEALTH_HEADER))) {
        DEBUG ((DEBUG_ERROR, "It is failed to built driver Health. \n"));
        continue;
      }

      AipPtr = (EFI_AIP_HEALTH_HEADER *) DataBlock;

      //
      // To handle driver health device path
      //
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiDevicePathProtocolGuid,
                      (VOID **) &DevicePath
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Can't get DevicePath, Status = [%x]. \n", Status));
        break;
      } else {
        DevicePathText = ConvertDeviceNodeToText (DevicePath, FALSE, FALSE);
        if (DevicePathText == NULL) {
          DEBUG ((DEBUG_ERROR, "Invalid DevicePath node.\n"));
          break;
        }
      }
      SizeOfDevicePath = StrSize (DevicePathText);
      DevSpecificDataOffset = sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE) + SizeOfDevicePath;

      //
      // To save the previous total size
      //
      AcpiDataSizeTemp = *AcpiDataSize;
      //
      // Get the total length (just include one default capsule instance for Mother board health)
      //
      if ((MAX_UINTN - DevSpecificDataOffset) < BlockSize - sizeof (EFI_AIP_HEALTH_HEADER)) {
        DEBUG ((DEBUG_ERROR, "Warning: found integer overflow.\n"));
        continue;
      }
      *AcpiDataSize += DevSpecificDataOffset + (BlockSize - sizeof (EFI_AIP_HEALTH_HEADER));

      if (*AcpiDataBlock == NULL) {
         //
         // Try to find the 1th health record
         //
        *AcpiDataBlock = AllocateZeroPool (*AcpiDataSize);
      } else {
        *AcpiDataBlock = ReallocatePool (AcpiDataSizeTemp, *AcpiDataSize, *AcpiDataBlock);
      }

      if (*AcpiDataBlock == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
       }

      AcpiDataBlockHead   = *AcpiDataBlock;
      *AcpiDataBlock      += AcpiDataSizeTemp;
      AcpiDataBlockHeader = (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) (*AcpiDataBlock);

      AcpiDataBlockHeader->PlatformRecordType = FirmwareHealthDataRecord;
      AcpiDataBlockHeader->RecordLength       = (UINT16) (*AcpiDataSize - AcpiDataSizeTemp);
      AcpiDataBlockHeader->Revision           = TELEMETRY_ACPI_HEADER_REVISION;
      AcpiDataBlockHeader->Reserved           = 0;
      AcpiDataBlockHeader->AmHealthy          = AipPtr->Healthy;
      CopyGuid (&AcpiDataBlockHeader->DeviceSignature, &AipPtr->Signature);
      AcpiDataBlockHeader->DeviceSpecificDataOffset = (UINT32) DevSpecificDataOffset;

      if (SizeOfDevicePath > 0) {
        //
        // Add device path data to ACPI table
        //
        CopyMem (
          *AcpiDataBlock + sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE),
          DevicePathText,
          SizeOfDevicePath
          );
      }

      //
      // Attach driver health special data
      //
      CopyMem (
        *AcpiDataBlock + DevSpecificDataOffset,
        DataBlock + sizeof (EFI_AIP_HEALTH_HEADER),
        BlockSize - sizeof (EFI_AIP_HEALTH_HEADER)
        );

      *AcpiDataBlock = AcpiDataBlockHead;
    }
  }

Exit:
  if (InfoTypesBuffer != NULL) {
    FreePool (InfoTypesBuffer);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  if ((DataBlock != NULL) && (BlockSize != 0)) {
    FreePool (DataBlock);
  }

  if ((*AcpiDataBlock == NULL) || (*AcpiDataSize == 0)) {
    return EFI_NOT_FOUND;
  }

  return Status;
}

/**
  Retrieve all available Image info by Fmp protocol, and attach them
  to the tail of Mother board health record.

  @param[out]      MotherBoardData      Point to the buffer of EFI_LAST_UPDATE_DATA
                                        in mother board health record
  @param[out]      MotherBoardSize      totoal length of all EFI_LAST_UPDATE_DATA data

  @retval          EFI_SUCCESS          Fmp info was collected successfully
  @retval          EFI_OUT_OF_RESOURCES There is not enough memory resource.

**/
EFI_STATUS
AttachAllFmpInfoToEnd (
  OUT UINT8    **MotherBoardData,
  OUT UINTN    *MotherBoardSize
  )
{
  EFI_STATUS                                    Status;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL              *Fmp;
  EFI_HANDLE                                    *HandleBuffer;
  UINTN                                         NumberOfHandles;
  UINTN                                         Index;
  UINTN                                         Index2;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                 *FmpImageInfoBuf;
  UINTN                                         ImageInfoSize;
  UINT32                                        FmpImageInfoDescriptorVer;
  UINT8                                         FmpImageInfoCount;
  UINTN                                         DescriptorSize;
  UINT32                                        PackageVersion;
  CHAR16                                        *PackageVersionName;
  UINTN                                         RecordCount;
  UINT8                                         *ImageInfoHead;
  UINT64                                        *LastUpdateTimeValue;
  UINT8                                         *CapsuleUpdateValue;
  UINT16                                        *CapsuleUpdateLastValue;
  EFI_TIME                                      CapsuleLastUdpateTime;
  EFI_GUID                                      CapsuleUpdateGuid;
  UINTN                                         NumOfCapsuleUpdate;
  CHAR16                                        CapsuleUpdate[16];
  EFI_STATUS                                    CapsuleStatus;

  RecordCount            = 0;
  ImageInfoHead          = NULL;
  FmpImageInfoBuf        = NULL;
  LastUpdateTimeValue    = NULL;
  CapsuleUpdateLastValue = NULL;
  CapsuleUpdateValue     = NULL;
  CapsuleStatus          = EFI_SUCCESS;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareManagementProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FMP protocol is EFI_NOT_FOUND\n"));
    return Status;
  }

  *MotherBoardData = AllocateZeroPool (sizeof (EFI_LAST_UPDATE_DATA) * NumberOfHandles);
  if (*MotherBoardData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }
  ImageInfoHead = *MotherBoardData;

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareManagementProtocolGuid,
                    (VOID **) &Fmp
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    ImageInfoSize = 0;
    Status = Fmp->GetImageInfo (
                    Fmp,
                    &ImageInfoSize,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                    );
    if (Status != EFI_BUFFER_TOO_SMALL) {
      continue;
    }

    FmpImageInfoBuf = AllocateZeroPool (ImageInfoSize);
    if (FmpImageInfoBuf == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto EXIT;
    }

    PackageVersionName = NULL;
    Status = Fmp->GetImageInfo (
                    Fmp,
                    &ImageInfoSize,               // ImageInfoSize
                    FmpImageInfoBuf,              // ImageInfo
                    &FmpImageInfoDescriptorVer,   // DescriptorVersion
                    &FmpImageInfoCount,           // DescriptorCount
                    &DescriptorSize,              // DescriptorSize
                    &PackageVersion,              // PackageVersion
                    &PackageVersionName           // PackageVersionName
                    );

    //
    // If FMP GetInformation interface failed, skip this resource
    //
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FMP (%d) ImageInfo - %r\n", Index, Status));
      FreePool (FmpImageInfoBuf);
      continue;
    }
    //
    //Check CapsuleLast
    //
    Status = GetVariable2 (CAPSULELAST, &gEfiCapsuleReportGuid, (VOID **) &CapsuleUpdateLastValue, NULL);
    if (!EFI_ERROR (Status)) {
      NumOfCapsuleUpdate = (UINT16) StrHexToUintn (&CapsuleUpdateLastValue[sizeof ("Capsule") - 1]);
      for (Index2 = 0; Index2 <= NumOfCapsuleUpdate; Index2++) {
        UnicodeSPrint (CapsuleUpdate, sizeof (CapsuleUpdate), L"%s%04X", CAPSULE_VAR_NAME, Index2);
        Status = GetVariable2 (CapsuleUpdate, &gEfiCapsuleReportGuid, (VOID **) &CapsuleUpdateValue, NULL);
        if (!EFI_ERROR (Status)) {
          CopyMem (&CapsuleStatus, CapsuleUpdateValue + OFFSET_OF (EFI_CAPSULE_RESULT_VARIABLE_HEADER,
                  CapsuleStatus), sizeof (EFI_STATUS));
          if (EFI_ERROR (CapsuleStatus)) {
            continue;
          }
          CopyGuid (&CapsuleUpdateGuid,
                   (EFI_GUID *) (CapsuleUpdateValue +
                   sizeof (EFI_CAPSULE_RESULT_VARIABLE_HEADER) +
                   OFFSET_OF (EFI_CAPSULE_RESULT_VARIABLE_FMP, UpdateImageTypeId)));
          if (CompareGuid (&FmpImageInfoBuf->ImageTypeId, &CapsuleUpdateGuid)) {
            CopyMem (&CapsuleLastUdpateTime,
                    CapsuleUpdateValue +
                    OFFSET_OF (EFI_CAPSULE_RESULT_VARIABLE_HEADER, CapsuleProcessed), sizeof (EFI_TIME));
            Status = gRT->SetVariable (
                            VARNAME_LASTUPDATETIME,
                            &FmpImageInfoBuf->ImageTypeId,
                            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                            sizeof (EFI_TIME),
                            &CapsuleLastUdpateTime
                            );
            if (EFI_ERROR(Status)) {
              DEBUG ((EFI_D_ERROR, "SetVariable fail, Status  = %r\n",Status));
            }
          }
          if (CapsuleUpdateValue != NULL) {
            FreePool (CapsuleUpdateValue);
          }
        } else {
          continue;
        }
      }
      if (CapsuleUpdateLastValue != NULL) {
        FreePool (CapsuleUpdateLastValue);
      }
    }

    //
    // combine all LastUdpateTime and EsrtID here
    //
    Status = GetVariable2 (VARNAME_LASTUPDATETIME, &FmpImageInfoBuf->ImageTypeId, (VOID **) &LastUpdateTimeValue, NULL);
    if (!EFI_ERROR (Status) && (LastUpdateTimeValue != NULL)) {
      CopyMem (ImageInfoHead, LastUpdateTimeValue, sizeof (EFI_TIME));
    } else {
      ZeroMem (ImageInfoHead, sizeof (EFI_TIME));
      Status = EFI_SUCCESS;
    }
    ImageInfoHead = ImageInfoHead + sizeof (EFI_TIME);

    CopyMem (ImageInfoHead, &FmpImageInfoBuf->ImageTypeId, sizeof (EFI_GUID));
    ImageInfoHead = ImageInfoHead + sizeof (EFI_GUID);

    RecordCount++;
    if (PackageVersionName != NULL) {
      FreePool (PackageVersionName);
    }

    if (LastUpdateTimeValue != NULL) {
      FreePool (LastUpdateTimeValue);
    }

    FreePool (FmpImageInfoBuf);
  }

  *MotherBoardSize = sizeof (EFI_LAST_UPDATE_DATA) * RecordCount;

EXIT:
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  return Status;
}

/**

  This function scan ACPI table entry point by its signature.

  @param[in] Xsdt      XSDT table entry point.
  @param[in] Signature ACPI table signature.

  @return ACPI table entry point.

**/
VOID *
ScanTableInXSDT (
  IN EFI_ACPI_DESCRIPTION_HEADER    *Xsdt,
  IN UINT32                         Signature
  )
{
  UINTN                          Index;
  UINT32                         EntryCount;
  UINT64                         EntryPtr;
  UINTN                          BasePtr;
  EFI_ACPI_DESCRIPTION_HEADER    *Table;

  if (Xsdt == NULL) {
    return NULL;
  }

  EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (UINT64);

  BasePtr = (UINTN)(Xsdt + 1);
  for (Index = 0; Index < EntryCount; Index ++) {
    CopyMem (&EntryPtr, (VOID *) (BasePtr + Index * sizeof (UINT64)), sizeof (UINT64));
    Table = (EFI_ACPI_DESCRIPTION_HEADER *) ((UINTN) (EntryPtr));
    if (Table->Signature == Signature) {
      return Table;
    }
  }

  return NULL;
}

/**

  This function scan PHAT table which was created in READY_TO_BOOT, and update it when EXIT_BOOT_SERVICES
  Even is triggered.

  @param[in] AcpiPhatTablePointer               A pointer to the EFI_ACPI_DESCRIPTION_HEADER instance.
  @param[in] AcpiBasicBootPerformancePointer    A pointer to the BOOT_PERFORMANCE_TABLE instance.
  @param[in] TelemetryVariable                  A pointer to the PLATFORM_TELEMETRY_VARIABLE instance.
**/
VOID
SearchAndUpdatePhatTable (
  IN EFI_ACPI_DESCRIPTION_HEADER        *AcpiPhatTablePointer,
  IN BOOT_PERFORMANCE_TABLE             *AcpiBasicBootPerformancePointer,
  IN MOTHER_BOARD_HEALTH_VARIABLE       *TelemetryVariable
   )
{
  UINT8                                            *PhatPointer;
  EFI_ACPI_6_5_PHAT_RECORD                         *TelemetryRecordHeader;
  UINT8                                            *HealthRecord;
  UINTN                                            TotalRecordLength;
  UINTN                                            TelemetryDataLength;
  EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE  *MBRecordHead;
  EFI_ACPI_MOTHERBOARD_HEALTH_DATA                 *MBRecordData;

  TotalRecordLength    = 0;
  HealthRecord         = NULL;
  MBRecordHead         = NULL;
  MBRecordData         = NULL;
  if (AcpiPhatTablePointer->Length > sizeof (EFI_ACPI_DESCRIPTION_HEADER)) {
    TelemetryDataLength  = AcpiPhatTablePointer->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  } else {
    DEBUG ((DEBUG_ERROR, "PHAT table is not existed or corrupted \n"));
    return;
  }
  //
  // PHAT entry.
  //
  PhatPointer = (UINT8 *) AcpiPhatTablePointer + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  for (TotalRecordLength = 0; TotalRecordLength < TelemetryDataLength - sizeof (EFI_ACPI_6_5_PHAT_RECORD); ) {
    TelemetryRecordHeader = (EFI_ACPI_6_5_PHAT_RECORD *) PhatPointer;
    if ((TelemetryRecordHeader->PlatformHealthAssessmentRecordType == FirmwareHealthDataRecord)
      && (TelemetryRecordHeader->RecordLength > sizeof (EFI_ACPI_6_5_PHAT_RECORD))) {
      //
      // found driver health records
      //
      HealthRecord = (UINT8 *) TelemetryRecordHeader;
      MBRecordHead = (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) HealthRecord;
      if (CompareGuid (&MBRecordHead->DeviceSignature, &gAdapterInfoMotherBoardHealthGuid)) {
        MBRecordData = (EFI_ACPI_MOTHERBOARD_HEALTH_DATA *)(HealthRecord + MBRecordHead->DeviceSpecificDataOffset);
        MBRecordData->MeanBootTime             = TelemetryVariable->MeanBootTime;
        MBRecordData->CurrentBootTime          = AcpiBasicBootPerformancePointer->BasicBoot.OsLoaderStartImageStart;
        MBRecordData->NumberOfBootsSinceUpdate = TelemetryVariable->NumberOfBootsSinceUpdate;
        //
        // Current Boot time over 50%(default) means NotHealth
        //
        if (PcdGet8 (PcdBootTimeOverPercentage) != 0) {
          if ((DivU64x32 (MAX_UINT64, (UINT32) PcdGet8 (PcdBootTimeOverPercentage)) < TelemetryVariable->MeanBootTime)) {
            DEBUG((DEBUG_ERROR, "MeanBootTime Overflow, Return Unhealthy.\n"));
            ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) TelemetryRecordHeader)->AmHealthy = TelemetryUnhealthy;
          } else if (AcpiBasicBootPerformancePointer->BasicBoot.OsLoaderStartImageStart >
                    (TelemetryVariable->MeanBootTime + DivU64x32 (MultU64x32 (TelemetryVariable->MeanBootTime, (UINT32) PcdGet8 (PcdBootTimeOverPercentage)), 100))) {
            ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) TelemetryRecordHeader)->AmHealthy = TelemetryUnhealthy;
          }
        } else {
          if (AcpiBasicBootPerformancePointer->BasicBoot.OsLoaderStartImageStart > TelemetryVariable->MeanBootTime) {
            ((EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE *) TelemetryRecordHeader)->AmHealthy = TelemetryUnhealthy;
          }
        }

        AcpiPlatformChecksumTelemetry (
          (UINT8 *) AcpiPhatTablePointer,
          AcpiPhatTablePointer->Length
          );

        break;
      }
    }

    if (TelemetryRecordHeader->RecordLength > sizeof (EFI_ACPI_6_5_PHAT_RECORD)) {
      PhatPointer += TelemetryRecordHeader->RecordLength;
      //
      // check record border.
      //
      TotalRecordLength += TelemetryRecordHeader->RecordLength;
    } else {
      break;
    }
  }
}

/**
  Update the final record to Mother board variable.

  @attention This function may receive untrusted input.
  Data and AcpiBasicBootPerformancePointer is external input, so this function will validate its data structure within this buffer before use.

  @param[in]  Data                             The contents for the variable.
  @param[in]  AcpiBasicBootPerformancePointer  This structure contains BasicBoot performance record.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  The input was NULL or calculating the result overflow of MeanBootTime.
**/
EFI_STATUS
SetMotherBoardVariable (
  IN  VOID                         *Data,
  IN  BOOT_PERFORMANCE_TABLE       *AcpiBasicBootPerformancePointer
  )
{
  EFI_STATUS                       Status;
  MOTHER_BOARD_HEALTH_VARIABLE     *TelemetryVariable;

  if ((Data == NULL) || (AcpiBasicBootPerformancePointer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  TelemetryVariable   = (MOTHER_BOARD_HEALTH_VARIABLE *) Data;
  if (TelemetryVariable->NumberOfBootsSinceUpdate != 0) {
    //
    // Multiplication overflow check.
    //
    if (DivU64x32 (MAX_UINT64,(UINT32) TelemetryVariable->NumberOfBootsSinceUpdate) > TelemetryVariable->MeanBootTime ) {
      //
      // Addition overflow check.
      //
      if ((MAX_UINT64 - AcpiBasicBootPerformancePointer->BasicBoot.OsLoaderStartImageStart) <
         (TelemetryVariable->MeanBootTime * TelemetryVariable->NumberOfBootsSinceUpdate)) {
        DEBUG((DEBUG_ERROR, "Addition MeanBootTime Overflow.\n"));
        return EFI_INVALID_PARAMETER;
      }
    } else {
      DEBUG((DEBUG_ERROR, "Multiplication MeanBootTime Overflow.\n"));
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Search driver health from Acpi table and update them.
  //
  TelemetryVariable->MeanBootTime = DivU64x32 (TelemetryVariable->MeanBootTime * TelemetryVariable->NumberOfBootsSinceUpdate
                                              + AcpiBasicBootPerformancePointer->BasicBoot.OsLoaderStartImageStart,
                                              (UINT32)(TelemetryVariable->NumberOfBootsSinceUpdate + 1));
  TelemetryVariable->NumberOfBootsSinceUpdate += 1;

  Status = gRT->SetVariable (
                  MOTHERBOARD_HEALTH_DATA_NAME,
                  &gMotherBoardHealthVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (MOTHER_BOARD_HEALTH_VARIABLE),
                  TelemetryVariable
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set the MOTHERBOARD_HEALTH_DATA_NAME record into a variable.  Status = %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
/**
  Report status code listener of PHAT. This is used to collect performance data
  for OsLoaderStartImageStart in PHAT.

  @param[in]  CodeType            Indicates the type of status code being reported.
  @param[in]  Value               Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is used to
                                  classify the entity as well as an operation.
  @param[in]  Instance            The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in]  CallerId            This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different rules to
                                  different callers.
  @param[in]  Data                This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.

**/
EFI_STATUS
EFIAPI
PhatStatusCodeListenerDxe (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  )
{
  EFI_ACPI_DESCRIPTION_HEADER     *AcpiPhatTablePointer;
  FIRMWARE_PERFORMANCE_TABLE      *AcpiFpdtTablePointer;
  BOOT_PERFORMANCE_TABLE          *AcpiBasicBootPerformancePointer;
  EFI_STATUS                      Status;
  MOTHER_BOARD_HEALTH_VARIABLE    TelemetryVariable;
  UINTN                           TelemetryBufferSize;

  AcpiPhatTablePointer            = NULL;
  AcpiFpdtTablePointer            = NULL;
  AcpiBasicBootPerformancePointer = NULL;
  TelemetryBufferSize             = 0;

  //
  // Check whether status code is what we are interested in.
  //
  if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_PROGRESS_CODE) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&TelemetryVariable, sizeof (MOTHER_BOARD_HEALTH_VARIABLE));
  Status = EFI_SUCCESS;
  if (Value == PcdGet32 (PcdProgressCodeOsLoaderLoad)) {
    //
    // Progress code for OS Loader LoadImage.
    //
    if (AcpiPhatTablePointer == NULL) {
      return Status;
    }

  } else if (Value == (EFI_SOFTWARE_EFI_BOOT_SERVICE | EFI_SW_BS_PC_EXIT_BOOT_SERVICES)) {
    AcpiPhatTablePointer = (EFI_ACPI_DESCRIPTION_HEADER *)EfiLocateFirstAcpiTable (
                                                            EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_SIGNATURE
                                                            );
    if (AcpiPhatTablePointer == NULL) {
      goto Done;
    }
    //
    // Update OS Loader StartImage Start for UEFI boot.
    //
    AcpiFpdtTablePointer = (FIRMWARE_PERFORMANCE_TABLE *)EfiLocateFirstAcpiTable (
                                                           EFI_ACPI_6_5_FIRMWARE_PERFORMANCE_DATA_TABLE_SIGNATURE
                                                           );
    if (AcpiFpdtTablePointer == NULL) {
      goto Done;
    }
    AcpiBasicBootPerformancePointer = (BOOT_PERFORMANCE_TABLE *) (UINTN)AcpiFpdtTablePointer->BootPointerRecord.BootPerformanceTablePointer;

    TelemetryBufferSize = sizeof (MOTHER_BOARD_HEALTH_VARIABLE);
    Status = gRT->GetVariable (
                    MOTHERBOARD_HEALTH_DATA_NAME,
                    &gMotherBoardHealthVariableGuid,
                    NULL,
                    &TelemetryBufferSize,
                    &TelemetryVariable
                    );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Failed to get the MotherBoardHealth variable Status = %r\n", Status));
      goto Done;
    }
    //
    // Update the final record to Mother board variable.
    //
    Status = SetMotherBoardVariable (&TelemetryVariable, AcpiBasicBootPerformancePointer);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Failed to set the MotherBoardHealth record into a variable.  Status = %r\n", Status));
      goto Done;
    }
    //
    // Update the final record to PHAT table
    //
    SearchAndUpdatePhatTable (AcpiPhatTablePointer, AcpiBasicBootPerformancePointer, &TelemetryVariable);
Done:
    //
    // Set HwErrRec variable.
    //
    Status = SetDriverHealthHwErrRecVariable (AcpiPhatTablePointer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set the Health record into a HwErrRec#### variable.  Status = %r\n", Status));
    }

    //
    // Unregister boot time report status code listener.
    //
    mRscHandlerProtocol->Unregister (PhatStatusCodeListenerDxe);

  } else {
    //
    // Ignore else progress code.
    //
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Convert Telemetry Aip data to Telemetry ACPI data.

  @param[in]  AipRecord       A pointer to the Fvi record buffer,does not contain header information.
                              May be NULL in order to determine the size buffer needed.
  @param[out] AcpiAipRecord   A pointer to the Fvi record buffer.
  @param[in]  LengthOfFvi     A pointer to the Length of Fvi record.

  @retval EFI_SUCCESS               The function completed successfully.
  @retval EFI_INVALID_PARAMETER     LengthOfFvi is NULL.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
EFI_STATUS
ConvertFviDataFromAipToAcpi (
  IN        UINT8  *AipRecord,
  OUT       UINT8  **AcpiAipRecord,
  IN        UINT16 *LengthOfFvi
  )
{
  if ((LengthOfFvi == NULL) || (AcpiAipRecord == NULL)) {
    DEBUG ((DEBUG_ERROR, "ConvertFviDataFromAipToAcpi : Invalid Parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }
  *AcpiAipRecord  = NULL;

  if (AipRecord == NULL) {
    DEBUG ((DEBUG_INFO, "Get length of Fvi successfully and then return.\n"));
    return EFI_SUCCESS;
  }

  *AcpiAipRecord  = AllocateZeroPool (*LengthOfFvi);
  if (*AcpiAipRecord == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_OUT_OF_RESOURCES : AcpiAipBuffHead allocate failed.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (*AcpiAipRecord, AipRecord, *LengthOfFvi);

  return EFI_SUCCESS;
}

/**
  Get Aip data from Hobs, these records won't be updated in DXE phase.

  @param[in, out] InformationBlock        A pointer to the InformationBlock structure which contains details.
                                          May be NULL in order to determine the size buffer needed.
  @param[out]     InformationBlockSize    The size of the InformationBlock in bytes.
**/
VOID
GetAipHobRecords (
  IN OUT UINT8   **InformationBlock,
  OUT    UINT16  *InformationBlockSize
  )
{
  EFI_PEI_HOB_POINTERS                 Hob;
  UINT8                                *RecordPtr;
  UINT8                                *FviRecord;
  UINT8                                *AcpiAipRecord;
  UINT16                               RecordSize;
  UINT8                                HobHeadLength;
  UINT8                                *HobBuffTemp;
  TELEMETRY_FIRMWARE_VERSION_INFO2_HOB  *FviHob;
  EFI_STATUS                           Status;

  FviHob        = NULL;
  HobBuffTemp   = NULL;
  AcpiAipRecord = NULL;
  HobHeadLength = OFFSET_OF (TELEMETRY_FIRMWARE_VERSION_INFO2_HOB, Records);

  if (InformationBlockSize == NULL) {
    DEBUG ((DEBUG_ERROR, "GetAipHobRecords : Invalid Parameter.\n"));
    return;
  }
  *InformationBlockSize = 0;
  if (InformationBlock != NULL){
    HobBuffTemp = *InformationBlock;
  }

  //
  // Get Fvi HOB
  //
  for (Hob.Raw = GetHobList (); !END_OF_HOB_LIST (Hob); Hob.Raw = GET_NEXT_HOB (Hob)) {
    if ((GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_GUID_EXTENSION) && (CompareGuid (&Hob.Guid->Name, &gTelemetryVersionHobGuid))) {
      FviHob = (TELEMETRY_FIRMWARE_VERSION_INFO2_HOB *) GET_GUID_HOB_DATA (Hob.Raw);

      RecordPtr = (UINT8 *) FviHob->Records;
      if (FviHob->Header.HobLength > HobHeadLength) {
        RecordSize = FviHob->Header.HobLength - HobHeadLength;
        //
        // Convert Telemetry Aip data to Telemetry ACPI data or get Length of Fvi.
        //
        if (InformationBlock != NULL) {
          //
          // Record need 16-bit alignment in ConvertFviDataFromAipToAcpi process.
          //
          FviRecord = AllocatePool (RecordSize * sizeof(UINT8));
          if (FviRecord == NULL) {
            DEBUG ((DEBUG_ERROR, "EFI_OUT_OF_RESOURCES : AcpiAipBuffHead allocate failed.\n"));
            *InformationBlockSize = 0;
            return;
          }
          CopyMem (FviRecord, RecordPtr, RecordSize);

          Status = ConvertFviDataFromAipToAcpi (FviRecord, &AcpiAipRecord, &RecordSize);
          if (Status == EFI_OUT_OF_RESOURCES) {
            DEBUG ((DEBUG_ERROR, "Failed to process ConvertFviDataFromAipToAcpi due to lack of resources!\n"));
            FreePool (FviRecord);
            *InformationBlockSize = 0;
            return;
          }
          FreePool (FviRecord);
        }
      } else {
        continue;
      }
      if (InformationBlock != NULL) {
        CopyMem (HobBuffTemp, AcpiAipRecord, RecordSize);
        HobBuffTemp += RecordSize;
        FreePool (AcpiAipRecord);
      }

      *InformationBlockSize += (UINT16) RecordSize;
    }
  }
}

/**
   Get Aip data from Fvi drivers and library.

  @param[in, out]     TempAipBuffHeader    A pointer to the buffer which follows up with hob record.
                                           May be NULL in order to determine the size buffer needed.
  @param[in, out]     TotalLength          The size of telemetry Aip data in bytes.

  @retval EFI_SUCCESS               The function completed successfully.
  @retval EFI_INVALID_PARAMETER     HandleBuffer is NULL.
  @retval EFI_INVALID_PARAMETER     Aip is NULL.
  @retval EFI_UNSUPPORTED           The InformationType is not known.
  @retval EFI_DEVICE_ERROR          The device reported an error.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
EFI_STATUS
CollectFviDataFromAip (
  IN OUT UINT8   **TempAipBuffHeader,
  IN OUT UINT16  *TotalLength
  )
{
  EFI_STATUS                        Status;
  EFI_ADAPTER_INFORMATION_PROTOCOL  *Aip;
  UINTN                             HandleCount;
  EFI_HANDLE                        *HandleBuffer;
  EFI_GUID                          *InfoTypesBuffer;
  UINTN                             InfoTypeBufferCount;
  UINTN                             TypeIndex;
  BOOLEAN                           Supported;
  VOID                              *DataBlock;
  UINTN                             BufferSize;
  UINT8                             *FviRecordHeader;
  UINTN                             Index;
  UINT8                             *AcpiAipRecord;
  UINT16                            AcpiFviSize;

  FviRecordHeader  = NULL;
  DataBlock        = NULL;
  AcpiAipRecord    = NULL;
  BufferSize       = 0;
  AcpiFviSize      = 0;
  if (TotalLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *TotalLength = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiAdapterInformationProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gEfiAdapterInformationProtocolGuid locate failed, status = [%x] \n", Status));
    //
    // gEfiAdapterInformationProtocolGuid protocol not installed yet.
    //
    return Status;
  }

  for (Index = 0; Index < HandleCount ; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiAdapterInformationProtocolGuid, (VOID **) &Aip);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HandleProtocol gEfiAdapterInformationProtocolGuid failed. \n"));
    } else {
      InfoTypesBuffer     = NULL;
      InfoTypeBufferCount = 0;
      //
      // Check AIP
      //
      Status = Aip->GetSupportedTypes (Aip, &InfoTypesBuffer, &InfoTypeBufferCount);
      if (EFI_ERROR (Status) || (InfoTypesBuffer == NULL)) {
        continue;
      }

      //
      // Check whether the AIP instance has Fvi information block.
      //
      Supported = FALSE;
      for (TypeIndex = 0; TypeIndex < InfoTypeBufferCount; TypeIndex++) {
        if (CompareGuid (&InfoTypesBuffer[TypeIndex], &gTelemetryVersionRecord2Guid)) {
          Supported = TRUE;
          break;
        }
      }

      FreePool (InfoTypesBuffer);
      if (!Supported) {
        continue;
      }

      Status = Aip->GetInformation (Aip, &gTelemetryVersionRecord2Guid, &DataBlock, &BufferSize);
      if (EFI_ERROR (Status) || (DataBlock == NULL) || (BufferSize == 0)) {
        continue;
      } else {
        //
        // skip Revision & RecordCount in EFI_AIP_TELEMETRY_VERSION_RECORD, find the valid
        //
        FviRecordHeader = (UINT8 *) ((EFI_AIP_TELEMETRY_VERSION_RECORD *) DataBlock)->Record;
      }
      //
      // kick Revision & RecordCount out.
      //
      if (BufferSize > OFFSET_OF (EFI_AIP_TELEMETRY_VERSION_RECORD, Record)) {
        AcpiFviSize =  AcpiFviRecordSize * (UINT16 ) (((EFI_AIP_TELEMETRY_VERSION_RECORD *) DataBlock)->RecordCount);
        DEBUG ((DEBUG_INFO, "AcpiFviSize = %d\n", AcpiFviSize));
        if (TempAipBuffHeader != NULL) {
          //
          // Convert Telemetry Aip data to Telemetry ACPI data.
          //
          Status = ConvertFviDataFromAipToAcpi (FviRecordHeader, &AcpiAipRecord, &AcpiFviSize);
          if (Status == EFI_OUT_OF_RESOURCES) {
            DEBUG ((DEBUG_ERROR, "Failed to process ConvertFviDataFromAipToAcpi due to lack of resources!\n"));
            FreePool (DataBlock);
            FreePool (HandleBuffer);
            return EFI_OUT_OF_RESOURCES;
          }
        }
        if ((MAX_UINT16 - *TotalLength) < AcpiFviSize) {
          DEBUG ((DEBUG_ERROR, "TotalLength is Greater than 4G,abandon this Fvi record.\n"));
          FreePool (DataBlock);
          if (AcpiAipRecord != NULL) {
            FreePool (AcpiAipRecord);
          }
          continue;
        }
        if (TempAipBuffHeader != NULL) {
          CopyMem (*TempAipBuffHeader, AcpiAipRecord, AcpiFviSize);
          *TempAipBuffHeader += AcpiFviSize;
          FreePool (AcpiAipRecord);
        }
        *TotalLength += (UINT16) AcpiFviSize;
      }
      FreePool (DataBlock);
    }
  }
  FreePool (HandleBuffer);
  return EFI_SUCCESS;
}

/**
  Get Aip data from Health drivers listed in mDriverHealthSupportList.

  @param[in, out]     AcpiAipBuffHead        A pointer to the buffer which follows up with Fvi record.
                                         May be NULL in order to determine the size buffer needed.
  @param[out]     LengthOfDriverHealth   The size of telemetry Aip driver health
                                         data in bytes.

**/
VOID
CollectHealthDriverDataFromAip (
  IN OUT UINT8   **AcpiAipBuffHead,
  OUT    UINT32  *LengthOfDriverHealth
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  UINT8       *DriverHealthData;
  UINTN       DriverHealthSize;
  UINT8       *MotherBoardTemp;
  UINTN       MotherBoardTempSize;
  UINT8       *MotherBoardFinal;
  UINTN       MotherBoardFinalSize;

  DriverHealthData        = NULL;
  DriverHealthSize        = 0;
  MotherBoardTemp         = NULL;
  MotherBoardTempSize     = 0;
  MotherBoardFinal        = NULL;
  MotherBoardFinalSize    = 0;

  if (LengthOfDriverHealth == NULL) {
    DEBUG ((DEBUG_ERROR, "CollectHealthDriverDataFromAip : Invalid Parameter.\n"));
    return;
  }
  *LengthOfDriverHealth   = 0;

  for (Index = 0; mDriverHealthSupportList[Index] != NULL; Index++ ) {
    Status = DriverHealthAipDataCollection (mDriverHealthSupportList[Index], &DriverHealthData, &DriverHealthSize);
    //
    // Integrate all driver health records
    //
    if (!EFI_ERROR (Status) && (DriverHealthSize > sizeof (EFI_ACPI_6_5_PHAT_FIRMWARE_HEALTH_DATA_RECORD_STRUCTURE))) {

      if (CompareGuid (mDriverHealthSupportList[Index], &gAdapterInfoMotherBoardHealthGuid)) {
        DEBUG ((DEBUG_INFO, "One Mother Board record is matched!  \n"));
        Status = AttachAllFmpInfoToEnd (&MotherBoardTemp, &MotherBoardTempSize);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "Get Mother Board Health special Info failed! Status = [%r] \n", Status));
        } else {
          //
          // combine MotherBoardTemp(with 1 instance by default) and More (from Fmp protocol).
          //
          MotherBoardFinalSize = DriverHealthSize + MotherBoardTempSize - sizeof (EFI_LAST_UPDATE_DATA);
          ((EFI_ACPI_6_5_PHAT_RECORD *) DriverHealthData)->RecordLength = (UINT16) MotherBoardFinalSize;
          if ((DriverHealthSize > sizeof (EFI_LAST_UPDATE_DATA)) && (MotherBoardTempSize > 0)) {

            MotherBoardFinal = AllocateZeroPool (MotherBoardFinalSize);
            if (MotherBoardFinal == NULL) {
              DEBUG ((DEBUG_ERROR, "MotherBoardFinal allocate failed, LastUpdateTime can't be updated! \n"));
              continue;
            }

            //
            // Override the default LastUpdateTime and EsrtID
            //
            CopyMem (MotherBoardFinal, DriverHealthData, DriverHealthSize);

            CopyMem (MotherBoardFinal + DriverHealthSize - sizeof (EFI_LAST_UPDATE_DATA), MotherBoardTemp, MotherBoardTempSize);
            if (AcpiAipBuffHead != NULL) {
              //
              // To attach integrated mother board info to the end of record.
              //
              CopyMem (*AcpiAipBuffHead, MotherBoardFinal, MotherBoardFinalSize);
            }
            DriverHealthSize = MotherBoardFinalSize;
          } else {
            DEBUG ((DEBUG_INFO, "Mother Board Health record is corrupted or its length is 0, drop it!! \n"));
          }

        }

        if (MotherBoardTemp != NULL) {
          FreePool (MotherBoardTemp);
        }

        if (MotherBoardFinal != NULL) {
          FreePool (MotherBoardFinal);
        }

      } else {
        if ((MAX_UINT32 - *LengthOfDriverHealth) < DriverHealthSize) {
          DEBUG ((DEBUG_ERROR, "LengthOfDriverHealth is Greater than 4G,abandon this driverHealth record.\n"));
          if (DriverHealthData != NULL) {
            FreePool (DriverHealthData);
          }
          continue;
        }
        if (AcpiAipBuffHead != NULL) {
          //
          // To handle other driver health record
          //
          CopyMem (*AcpiAipBuffHead, DriverHealthData, DriverHealthSize);
        }
      }
      if (AcpiAipBuffHead != NULL) {
        *AcpiAipBuffHead = *AcpiAipBuffHead + DriverHealthSize;
      }
      *LengthOfDriverHealth = *LengthOfDriverHealth + (UINT32) DriverHealthSize;
      DEBUG ((DEBUG_INFO, "Driver Health -- [%g] has been converted successfully \n", *mDriverHealthSupportList[Index]));
    }
    if (DriverHealthData != NULL) {
      FreePool (DriverHealthData);
    }
  }

}

/**
  Initialize the header of the Platform Health Assessment Table.

  @param[out]  Header     The header of the ACPI Table.
  @param[in]   OemId      The OEM ID.
  @param[in]   OemTableId The OEM table ID for the Phat.
**/
VOID
InitPhatAcpiTableHeader (
  OUT EFI_ACPI_DESCRIPTION_HEADER   *Header,
  IN  UINT8                         *OemId,
  IN  UINT64                        *OemTableId
  )
{
  ZeroMem (Header, sizeof (EFI_ACPI_DESCRIPTION_HEADER));

  Header->Signature = EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_SIGNATURE;
  //
  // total length (Fvi, Driver Health).
  //
  Header->Length    = 0;
  Header->Revision  = EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_REVISION;
  Header->Checksum  = 0;
  CopyMem (Header->OemId, OemId, sizeof (Header->OemId));
  CopyMem (&Header->OemTableId, OemTableId, sizeof (UINT64));
  Header->OemRevision     = EFI_ACPI_OEM_REVISION;
  Header->CreatorId       = EFI_ACPI_CREATOR_ID;
  Header->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
}

/**
  Get length of telemetry from hob and aip.

  @param[in, out]  LengthOfFvi              Pointer to the length of Fvi.
  @param[in, out]  LengthOfDriverHealth     Pointer to total length of telemetry Driver Health data.

  @retval EFI_SUCCESS             Get total length successful.
  @retval EFI_OUT_OF_RESOURCES    There is not enough pool memory to store the results.
  @retval EFI_INVALID_PARAMETER   LengthOfFvi and LengthOfDriverHealth is NULL.

**/
EFI_STATUS
EFIAPI
GetLengthOfTelemetryData (
  IN OUT UINT16  *LengthOfFvi,
  IN OUT UINT32  *LengthOfDriverHealth
  )
{
  EFI_STATUS  Status;
  UINT16      HobLength;

  HobLength = 0;
  Status    = EFI_SUCCESS;

  if ((LengthOfFvi == NULL) && (LengthOfDriverHealth == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetLengthOfTelemetryData : Invalid Parameter.\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (LengthOfFvi != NULL) {
    *LengthOfFvi = 0;
    //
    // Get Fvi length from Aip Hob.
    //
    GetAipHobRecords (NULL, &HobLength);
    //
    // Get Fvi length from Aip Protocol.
    //
    Status = CollectFviDataFromAip (NULL, LengthOfFvi);
    if (Status == EFI_OUT_OF_RESOURCES) {
      DEBUG ((DEBUG_ERROR, "Failed to process CollectFviDataFromAip due to lack of resources!\n"));
      return Status;
    }
    if ((MAX_UINT16 - *LengthOfFvi) < (HobLength + AcpiComPonentHeadSize)) {
      DEBUG ((DEBUG_ERROR, "LengthOfFvi is Greater than 4G,set the value of LengthOfFvi to zero.\n"));
      *LengthOfFvi = AcpiComPonentHeadSize;
    } else {
      *LengthOfFvi += HobLength;
      *LengthOfFvi += AcpiComPonentHeadSize;
    }
  }

  if (LengthOfDriverHealth != NULL) {
    *LengthOfDriverHealth = 0;
    //
    // Get Driver health length form aip protocol.
    //
    CollectHealthDriverDataFromAip (NULL, LengthOfDriverHealth);

    if (LengthOfFvi != NULL) {
      if ((MAX_UINT32 - *LengthOfFvi) < *LengthOfDriverHealth) {
        DEBUG ((DEBUG_ERROR, "TotalLength is Greater than UINT16,set the value of LengthOfDriverHealth to zero.\n"));
        LengthOfDriverHealth = 0;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Collect AIP data including Fvi and driver health, then convert them to Acpi format.
**/
VOID
EFIAPI
BuildPhatAcpiTable (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINT16                       LengthOfFvi;
  UINT32                       LengthOfDriverHealth;
  UINT32                       TotalLength;
  UINT8                        *TempAipBuffHeader;
  UINT8                        *AcpiAipBuff;
  UINT8                        *AcpiAipBuffHead;
  UINT16                       HobLength;
  EFI_ACPI_DESCRIPTION_HEADER  *AcpiPhatTablePointer;

  LengthOfFvi           = 0;
  LengthOfDriverHealth  = 0;
  TotalLength           = 0;
  HobLength             = 0;
  TempAipBuffHeader     = NULL;
  AcpiAipBuff           = NULL;
  AcpiAipBuffHead       = NULL;
  AcpiPhatTablePointer  = NULL;

  //
  // Get LengthOfFvi and LengthOfDriverHealth.
  //
  Status = GetLengthOfTelemetryData (&LengthOfFvi, &LengthOfDriverHealth);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get length of telemetry data. Status = %r\n", Status));
    return;
  }
  DEBUG ((DEBUG_INFO, "Searching Fvi is done: LengthOfFvi ==[%d], LengthOfDriverHealth == [%d] \n", LengthOfFvi, LengthOfDriverHealth));

  TotalLength = LengthOfFvi + LengthOfDriverHealth;
  AcpiAipBuff = AllocateZeroPool (TotalLength);
  AcpiAipBuffHead = AcpiAipBuff;
  if (AcpiAipBuff == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_OUT_OF_RESOURCES : AcpiAipBuff allocate failed.\n"));
    return;
  }

  ((EFI_COMPONENT_VERSION_DATA *) AcpiAipBuffHead)->Revision     = TELEMETRY_FVI_DATA_RECORD_REVISION;
  ((EFI_COMPONENT_VERSION_DATA *) AcpiAipBuffHead)->RecordLength = LengthOfFvi;
  ((EFI_COMPONENT_VERSION_DATA *) AcpiAipBuffHead)->RecordCount  = (LengthOfFvi - AcpiComPonentHeadSize) / AcpiFviRecordSize;
  AcpiAipBuffHead = AcpiAipBuff + AcpiComPonentHeadSize;

  if (LengthOfFvi != AcpiComPonentHeadSize) {

    //
    // Get Aip data from Hobs.
    //
    GetAipHobRecords (&AcpiAipBuffHead, &HobLength);
    TempAipBuffHeader = AcpiAipBuffHead + HobLength;
    //
    // Ready to add Fvi health record
    //
    Status = CollectFviDataFromAip (&TempAipBuffHeader, &LengthOfFvi);
    if (Status == EFI_OUT_OF_RESOURCES) {
      DEBUG ((DEBUG_ERROR, "Failed to process CollectFviDataFromAip due to lack of resources!\n"));
      if (AcpiAipBuff != NULL) {
        FreePool (AcpiAipBuff);
      }
      return;
    }
  }

  //
  // Ready to add driver health record if they can be found
  //
  if (LengthOfDriverHealth != 0) {
    CollectHealthDriverDataFromAip (&TempAipBuffHeader, &LengthOfDriverHealth);
  }
  if ((TempAipBuffHeader == NULL) || (LengthOfDriverHealth == 0)) {
    DEBUG ((DEBUG_ERROR, "No Driver health record was found. \n"));
  }

  //
  // Initialize the Platform Health Assessment Table.
  //
  Status = InstallPhatTable (AcpiAipBuff, TotalLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install Telemetry ACPI table failed, Status = [%x] \n", Status));
  }

  //
  // Clear all Telemetry HwErrRec variable.
  //
  Status = ClearAllTelemetryHwErrRec ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to ClearAll HwErrRec#### variable.  Status = %r\n", Status));
  }

  //
  // Set HwErrRec variable.
  //
  AcpiPhatTablePointer = (EFI_ACPI_DESCRIPTION_HEADER *)EfiLocateFirstAcpiTable (
                                                          EFI_ACPI_6_5_PLATFORM_HEALTH_ASSESSMENT_TABLE_SIGNATURE
                                                          );
  if (AcpiPhatTablePointer == NULL) {
    DEBUG ((DEBUG_ERROR, "EFI_NOT_FUND : AcpiPhatTablePointer is NULL.\n"));
  }
  Status = SetDriverHealthHwErrRecVariable (AcpiPhatTablePointer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set the Health record into a HwErrRec#### variable.  Status = %r\n", Status));
  }

  if (AcpiAipBuff != NULL) {
    FreePool (AcpiAipBuff);
  }
}

/**
  Update PHAT callback function.

  @param[in]  Event   Pointer to this event
  @param[in]  Context Event handler private data.

**/

VOID
EFIAPI
UpdatePhatAcpiTableCallback (
  IN  EFI_EVENT                 Event,
  IN  VOID                      *Context
  )
{
  BuildPhatAcpiTable();
}

/**
  ReadytoBootEvent build PHAT.

  @param[in]  Event   Pointer to this event
  @param[in]  Context Event handler private data.

**/
VOID
EFIAPI
InstallPhatAcpiTableCallback (
  IN  EFI_EVENT                 Event,
  IN  VOID                      *Context
  )
{
  EFI_EVENT   AcpiEvent;
  EFI_STATUS  Status;

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  BuildPhatAcpiTable();

  //
  // Register rebuild PHAT table event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UpdatePhatAcpiTableCallback,
                  NULL,
                  &gPlatformAcpiUpdateEventGuid,
                  &AcpiEvent
                  );

  ASSERT_EFI_ERROR (Status);
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
DxeTelemetryAcpiEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_EVENT   Event;
  EFI_STATUS  Status;

  //
  // Get Report Status Code Handler Protocol.
  //
  Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid, NULL, (VOID **) &mRscHandlerProtocol);
  ASSERT_EFI_ERROR (Status);

  //
  // Register report status code listener for OS Loader load and start.
  //
  Status = mRscHandlerProtocol->Register (PhatStatusCodeListenerDxe, TPL_HIGH_LEVEL);
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             InstallPhatAcpiTableCallback,
             NULL,
             &Event
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
