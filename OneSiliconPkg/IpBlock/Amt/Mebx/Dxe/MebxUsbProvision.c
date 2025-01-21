/** @file
  Performs USB Key Provisioning for AMT.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FileHandleLib.h>
#include <Library/DxeMeLib.h>
#include <Library/AmthiMsgsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/DxeMebxDisplayWrapperLib.h>
#include <MebxUsbProvision.h>

#define MEBX_PROVISIONING_TIMEOUT             30      ///< 30 seconds
#define MEBX_STALL_1_SECOND                   1000000 ///< 1 second

/**
  Wait for the availability bits of FW features defined in the input BitMask.

  The function checks if the specified bits of FW features are set until those bits are set
  or the determined timeout expires. This function checks FW features every 1/4 second.

  @param[in] BitMask            Bitmask of FW features meant to be available
  @param[in] Timeout            Timeout in seconds

  @retval EFI_SUCCESS           FW features defined in bitmask available
  @retval EFI_TIMEOUT           FW features defined in bitmask unavailable
**/
EFI_STATUS
MeWaitFwFeatureAvailable (
  IN UINT32 BitMask,
  IN UINT32 Timeout
  )
{
  EFI_STATUS   Status;
  MEFWCAPS_SKU FwFeatures;

  FwFeatures.Data = 0;

  Status = HeciGetFwFeatureStateMsg (&FwFeatures);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (FwFeatures.Data & BitMask) {
    for (Timeout *= 4; Timeout > 0; Timeout--) {
      Status = HeciGetFwAvailableFeatures (&FwFeatures.Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if (FwFeatures.Data & BitMask) {
        return EFI_SUCCESS;
      }

      // wait 250 miliseconds
      MicroSecondDelay (250000);
    }

    if (Timeout == 0 && !(FwFeatures.Data & BitMask)) {
      return EFI_TIMEOUT;
    }
  }

  return Status;
}

/**
  Prints data record entries information.

  @param[in] ProvisionRecordHandle  Pointer to provision record
**/
STATIC
VOID
PrintDataRecordInfo (
  IN PROVISION_RECORD_HANDLE  *ProvisionRecordHandle
  )
{
  DATA_RECORD_ENTRY      *DataRecordEntry;
  PROVISION_DATA_RECORD  *DataRecord;
  UINT32                 DataRecordEntryCount;
  UINT16                 Padding;
  UINT32                 DataRecordSize;

  DataRecordSize       = ProvisionRecordHandle->ProvisionRecordSize;
  DataRecord           = ProvisionRecordHandle->ProvisionRecordBuffer;
  DataRecordEntry      = (DATA_RECORD_ENTRY *) ((UINTN) DataRecord + DataRecord->Header.RecordHeaderByteCount);
  DataRecordEntryCount = 0;

  while ((UINTN) DataRecordEntry < ((UINTN) DataRecord + DataRecordSize)) {
    switch (DataRecordEntry->ModuleId) {
      case ModuleIdInvalid:
        break;

      case ModuleIdMeKernel:
        DEBUG ((DEBUG_INFO, "MODULE_IDENTIFIER_ME_KERNEL\n"));
        switch (DataRecordEntry->VariableId) {
          case MeKernelVariableIdInvalid:
            DEBUG ((DEBUG_ERROR, "Invalid variable identifier\n"));
            return;

          case MeKernelVariableIdCurrentMebxPwd:
            DataRecordEntryCount++;
            DEBUG ((DEBUG_INFO, "VARIABLE_IDENTIFIER_ME_KERNEL_CURRENT_MEBX_PASSWORD\n"));
            break;

          case MeKernelVariableIdNewMebxPwd:
            DataRecordEntryCount++;
            DEBUG ((DEBUG_INFO, "VARIABLE_IDENTIFIER_ME_KERNEL_NEW_MEBX_PASSWORD\n"));
            break;
        }
        break;

      case ModuleIdIntelAmtCm:
        DEBUG ((DEBUG_INFO, "MODULE_IDENTIFIER_INTEL_AMT_CM\n"));
        switch (DataRecordEntry->VariableId) {
          case IntelAmtVariableIdInvalid:
            DEBUG ((DEBUG_ERROR, "Invalid variable identifier\n"));
            return;

          case IntelAmtVariableIdPid:
            DataRecordEntryCount++;
            DEBUG ((DEBUG_INFO, "VARIABLE_IDENTIFIER_INTEL_AMT_CM_PID\n"));
            break;

          case IntelAmtVariableIdPps:
            DataRecordEntryCount++;
            DEBUG ((DEBUG_INFO, "VARIABLE_IDENTIFIER_INTEL_AMT_CM_PPS\n"));
            break;
        }
        break;
    }

    Padding = (4 - (DataRecordEntry->VariableLength % 4)) % 4;
    DataRecordEntry = (DATA_RECORD_ENTRY*) (((UINTN) &DataRecordEntry->VariableValue) + DataRecordEntry->VariableLength + Padding);
  }

  DEBUG ((DEBUG_INFO, "Found total of %d entries\n", DataRecordEntryCount));
}

/**
  Find and return first unconsumed data record offset in provision file.

  @param[in]  ProvisionFileHeader        Pointer to provision file header
  @param[out] DataRecordSize             On output Data Record size.
                                         Valid only if function returned TRUE
  @param[out] ProvisionDataRecordOffset  On output first unconsumed Data Record offset.
                                         Valid only if function returned TRUE

  @retval TRUE   Found unconsumed data record
  @retval FALSE  Couldn't find unconsumed data record
**/
STATIC
BOOLEAN
GetFirstUnconsumedDataRecordOffset (
  IN  PROVISION_FILE_HEADER  *ProvisionFileHeader,
  OUT UINTN                  *DataRecordSize,
  OUT UINT64                 *ProvisionDataRecordOffset
  )
{
  UINTN  FileHeaderSize;
  UINTN  ConsumedDataRecordsSize;

  //
  // If all data records have been consumed then there is nothing left to do provision with
  //
  if (ProvisionFileHeader->DataRecordsConsumed == ProvisionFileHeader->DataRecordCount) {
    return FALSE;
  }

  *DataRecordSize            = ProvisionFileHeader->DataRecordChunkCount * USB_DATACHUNK_SIZE;
  ConsumedDataRecordsSize    = ProvisionFileHeader->DataRecordsConsumed  * (*DataRecordSize);
  FileHeaderSize             = ProvisionFileHeader->RecordChunkCount     * USB_DATACHUNK_SIZE;
  *ProvisionDataRecordOffset = (UINT64) (FileHeaderSize + ConsumedDataRecordsSize);
  return TRUE;
}

/**
  Checks if provision file is valid and supported base
  on File UUID and major and minor version.

  @param[in] ProvisionFileHeader  Pointer to provision file header.
  @param[in] ProvisionFileSize    Size of provision file.

  @retval TRUE   Provision file is valid and supported
  @retval FALSE  Provision file is not valid.
**/
STATIC
BOOLEAN
IsProvisionFileValid (
  IN PROVISION_FILE_HEADER  *ProvisionFileHeader,
  IN UINT64                 ProvisionFileSize
  )
{
  UINT32                         Index;
  UINT64                         TotalChunks;
  STATIC PROVISION_FILE_VERSION  SupportedVersions[] = {
    {FILE_TYPE_1_UUID, MAJOR_VERSION_1, MINOR_VERSION_0},
    {FILE_TYPE_2_UUID, MAJOR_VERSION_2, MINOR_VERSION_0},
    {FILE_TYPE_2_UUID, MAJOR_VERSION_2, MINOR_VERSION_1},
    {FILE_TYPE_3_UUID, MAJOR_VERSION_3, MINOR_VERSION_0},
    {FILE_TYPE_4_UUID, MAJOR_VERSION_4, MINOR_VERSION_0}
  };

  TotalChunks = (ProvisionFileHeader->RecordChunkCount + ((UINT64) ProvisionFileHeader->DataRecordCount * ProvisionFileHeader->DataRecordChunkCount));
  if ((TotalChunks * USB_DATACHUNK_SIZE) != ProvisionFileSize) {
    DEBUG ((DEBUG_ERROR, "Provision file size mismatched\n"));
    return FALSE;
  }

  if (ProvisionFileHeader->DataRecordsConsumed > ProvisionFileHeader->DataRecordCount) {
    DEBUG ((DEBUG_ERROR, "DataRecordsConsumed should not larger than DataRecordCount\n"));
    return FALSE;
  }

  for (Index = 0; Index < ARRAY_SIZE (SupportedVersions); Index++) {
    if (CompareGuid ((EFI_GUID*) &ProvisionFileHeader->FileTypeUuid, &SupportedVersions[Index].Guid) &&
        ProvisionFileHeader->MajorVersion == SupportedVersions[Index].MajorVersion &&
        ProvisionFileHeader->MinorVersion == SupportedVersions[Index].MinorVersion) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Read provision data file, and fill provided buffer with data.

  @param[in]   ProvisionFileHandle    File handle of the provision file
  @param[out]  ProvisionFileHeader    Pointer to Provision File Header
  @param[out]  ProvisionFileSize      Size of Provision File

  @retval TRUE   Operation carried out successfully
  @retval FALSE  Reading process failed
**/
STATIC
BOOLEAN
ReadProvisionFile (
  IN   EFI_FILE_HANDLE         ProvisionFileHandle,
  OUT  PROVISION_FILE_HEADER   *ProvisionFileHeader,
  OUT  UINT64                  *ProvisionFileSize
  )
{
  EFI_STATUS  Status;
  UINT64      ProvisionFileHeaderSize;

  *ProvisionFileSize = 0;
  Status = FileHandleSetPosition (ProvisionFileHandle, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set file handle position\n"));
    return FALSE;
  }
  ProvisionFileHeaderSize = sizeof (PROVISION_FILE_HEADER);
  Status = FileHandleRead (ProvisionFileHandle, (UINTN*) &ProvisionFileHeaderSize, ProvisionFileHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read provision file header\n"));
    return FALSE;
  }

  Status = FileHandleGetSize (ProvisionFileHandle, ProvisionFileSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read provision file size\n"));
    return FALSE;
  }
  return TRUE;
}

/**
  Build provision record

  @param[in]  ProvisionFileHandle        File handle of the provision file
  @param[in]  ProvisionDataRecordSize    Data record size
  @param[in]  ProvisionDataRecordOffset  Data record offset
  @param[out] ProvisionRecordHandle      Handle of the provision record

  @retval TRUE   Operation carried out successfully
  @retval FALSE  Reading process failed
**/
STATIC
BOOLEAN
BuildProvisionRecordHandle (
  IN  EFI_FILE_HANDLE         ProvisionFileHandle,
  IN  UINTN                   ProvisionDataRecordSize,
  IN  UINT64                  ProvisionDataRecordOffset,
  OUT PROVISION_RECORD_HANDLE *ProvisionRecordHandle
  )
{
  EFI_STATUS  Status;

  ProvisionRecordHandle->ProvisionRecordSize   = (UINT32) (ProvisionDataRecordSize);
  ProvisionRecordHandle->ProvisionRecordBuffer = AllocateZeroPool ((UINTN) ProvisionDataRecordSize);
  if (ProvisionRecordHandle->ProvisionRecordBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate provision file buffer\n"));
    return FALSE;
  }

  Status = FileHandleSetPosition (ProvisionFileHandle, ProvisionDataRecordOffset);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set file handle position\n"));
    return FALSE;
  }

  Status = FileHandleRead (
             ProvisionFileHandle,
             (UINTN *) &ProvisionDataRecordSize,
             (VOID *) ProvisionRecordHandle->ProvisionRecordBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read provision file\n"));
    return FALSE;
  }
  //
  // Save the file handle for the remove record run.
  // We leave the file handle open for the time being, it is
  // closed later on after record has been removed.
  //
  ProvisionRecordHandle->ProvisionFileHandle = ProvisionFileHandle;
  ProvisionRecordHandle->HandleValid         = TRUE;
  return TRUE;
}

/**
  Find a valid provision data record and constructs a
  provision record handle to return to user code.

  @param[in]  ProvisionFileHandle    File handle of the provision file
  @param[out] ProvisionRecordHandle  On output handle of the valid provision record

  @retval TRUE   Valid provision record found
  @retval FALSE  Couldn't find valid provision record
**/
STATIC
BOOLEAN
FindValidDataRecordForProvision (
  IN  EFI_FILE_HANDLE          ProvisionFileHandle,
  OUT PROVISION_RECORD_HANDLE  *ProvisionRecordHandle
  )
{
  PROVISION_FILE_HEADER  ProvisionFileHeader;
  UINT64                 ProvisionFileSize;
  UINT64                 ProvisionDataRecordOffset;
  UINTN                  ProvisionDataRecordSize;

  if (!ReadProvisionFile (ProvisionFileHandle, &ProvisionFileHeader, &ProvisionFileSize)) {
    DEBUG ((DEBUG_INFO, "Failed reading provision file header!\n"));
    return FALSE;
  }

  if (!IsProvisionFileValid (&ProvisionFileHeader, ProvisionFileSize)) {
    DEBUG ((DEBUG_INFO, "Provision file unsupported!\n"));
    return FALSE;
  }

  if (GetFirstUnconsumedDataRecordOffset (&ProvisionFileHeader, &ProvisionDataRecordSize, &ProvisionDataRecordOffset)) {
    if (!BuildProvisionRecordHandle (ProvisionFileHandle, ProvisionDataRecordSize, ProvisionDataRecordOffset, ProvisionRecordHandle)) {
      DEBUG ((DEBUG_INFO, "Failed building provision data record!\n"));
      return FALSE;
    }
    DEBUG_CODE_BEGIN ();
    PrintDataRecordInfo (ProvisionRecordHandle);
    DEBUG_CODE_END ();
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Returns EFI_FILE_HANDLE of the provision file on the file system
  if present. Provisioning file search is limited to the root directory.

  This function opens file handle to provision file it is a caller responsibility
  to close it later on.

  @param[in]  FileSystemHandle     File system handle
  @param[out] ProvisionFileHandle  On output file handle to provision file.
                                   Valid only if this function returned TRUE.

  @retval TRUE   Provision file found
  @retval FALSE  Provision file not present
**/
STATIC
BOOLEAN
GetProvisionFileHandle (
  IN  EFI_HANDLE       FileSystemHandle,
  OUT EFI_FILE_HANDLE  *ProvisionFileHandle
  )
{
  EFI_STATUS                       Status;
  EFI_FILE_HANDLE                  RootFileHandle;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *ProvisionFileVolume;

  Status = gBS->HandleProtocol (
                  FileSystemHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID**) &ProvisionFileVolume
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = ProvisionFileVolume->OpenVolume (ProvisionFileVolume, &RootFileHandle);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = RootFileHandle->Open (
                             RootFileHandle,
                             ProvisionFileHandle,
                             PROVISION_FILE_NAME,
                             EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ,
                             0
                             );
  RootFileHandle->Close (RootFileHandle);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Checks if file system is located on USB mass storage.

  @param[in] FileSystemHandle  File system handle

  @retval TRUE  File system is located on USB mass storage
  @retval FALSE File system is not located on USB mass storage
**/
STATIC
BOOLEAN
IsFileSystemOnUsbMassStorage (
  IN EFI_HANDLE  FileSystemHandle
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  DevicePath = DevicePathFromHandle (FileSystemHandle);
  if (DevicePath == NULL) {
    return FALSE;
  }

  while (!IsDevicePathEnd (DevicePath)) {
    if (DevicePath->Type == MESSAGING_DEVICE_PATH && DevicePath->SubType == MSG_USB_DP) {
      return TRUE;
    } else {
      DevicePath = NextDevicePathNode (DevicePath);
    }
  }

  return FALSE;
}

/**
  This function searches file system for valid provision file
  and extract first valid provision record.

  @param[out] ProvisionRecord  Pointer to provision record.

  @retval TRUE  provision record found
  @retval FALSE provision record not found
**/
BOOLEAN
GetUsbProvisionRecord (
  OUT PROVISION_RECORD_HANDLE  *ProvisionRecordHandle
  )
{
  EFI_STATUS       Status;
  UINTN            FileSystemHandlesNum;
  EFI_HANDLE       *FileSystemHandles;
  EFI_FILE_HANDLE  ProvisionFileHandle;
  UINTN            Index;
  BOOLEAN          ProvisionRecordFound;

  ZeroMem (ProvisionRecordHandle, sizeof (PROVISION_RECORD_HANDLE));

  FileSystemHandles = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &FileSystemHandlesNum,
                  &FileSystemHandles
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  ProvisionRecordFound = FALSE;
  for (Index = 0; Index < FileSystemHandlesNum; Index++) {
    if (!IsFileSystemOnUsbMassStorage (FileSystemHandles[Index])) {
      continue;
    }

    if (!GetProvisionFileHandle (FileSystemHandles[Index], &ProvisionFileHandle)) {
      continue;
    }

    if (!FindValidDataRecordForProvision (ProvisionFileHandle, ProvisionRecordHandle)) {
      FileHandleClose (ProvisionFileHandle);
      continue;
    } else {
      //
      // In case we did find a valid record we leave the file handle open since
      // it will be used during record remove which is run after MEBx invocation.
      //
      ProvisionRecordFound = TRUE;
      break;
    }
  }

  if (FileSystemHandles) {
    FreePool (FileSystemHandles);
  }
  return ProvisionRecordFound;
}

/**
  Frees up all of the resources allocated for the purpose of ProvisionRecord.
  After call to this function provision record object is no longer valid.

  @param[in] ProvisionRecord  Provision record to clean up.
**/
STATIC
VOID
CleanupProvisionRecordResources (
  IN PROVISION_RECORD_HANDLE  *ProvisionRecordHandle
  )
{
  ProvisionRecordHandle->HandleValid = FALSE;
  FileHandleClose (ProvisionRecordHandle->ProvisionFileHandle);
  FreePool (ProvisionRecordHandle->ProvisionRecordBuffer);
}

/**
  Check if record removal is required.

  @param[in] ProvisionFileBuffer  pointer to memory with provision file
  @param[in] ReturnedDataRecord   data record as returned from MEBx

  @retval TRUE   Data record should be removed
  @retval FALSE  Data record should not be removed
**/
STATIC
BOOLEAN
IsRecordConsumptionRequired (
  IN PROVISION_FILE_HEADER  *ProvisionFileHeader,
  IN PROVISION_DATA_RECORD  *ReturnedDataRecord
  )
{
  //
  // MEBx will clear the record valid flag if record has been consumed.
  // If flag has not been cleared it means that MEBx encountered an error during
  // provisioning flow and didn't consume the record.
  //
  if (ReturnedDataRecord->Header.RecordFlags.Bits.Valid == DATA_RECORD_FLAG_VALID) {
    DEBUG ((DEBUG_ERROR, "MEBx failed to consume data record\n"));
    return FALSE;
  }

  if (ProvisionFileHeader->MajorVersion >= MAJOR_VERSION_2 &&
      (ProvisionFileHeader->FileFlags & FILE_FLAGS_DONT_CONSUME_RECORDS)) {
    DEBUG ((DEBUG_INFO, "Provision file doesn't support record consumption\n"));
    return FALSE;
  }
  return TRUE;
}

/**
  Removes consumed provision record from provision file if necessary.

  @param[in] ProvisionRecordHandle  Consumed provision record
  @param[in] ReturnedDataRecord     Data record returned from MEBx
**/
STATIC
VOID
RemoveProvisionRecordFromFile (
  IN PROVISION_RECORD_HANDLE  *ProvisionRecordHandle,
  IN PROVISION_DATA_RECORD    *ReturnedDataRecord
  )
{
  EFI_STATUS             Status;
  PROVISION_FILE_HEADER  ProvisionFileHeader;
  UINT64                 ProvisionFileSize;
  UINT64                 ProvisionDataRecordOffset;
  PROVISION_DATA_RECORD  *ProvisionDataRecord;
  UINTN                  ProvisionDataRecordSize;
  UINT32                 ConsumedRecordsNumberSize;

  Status = EFI_SUCCESS;
  ProvisionDataRecord = ProvisionRecordHandle->ProvisionRecordBuffer;

  if (!ReadProvisionFile (ProvisionRecordHandle->ProvisionFileHandle, &ProvisionFileHeader, &ProvisionFileSize)) {
    DEBUG ((DEBUG_INFO, "Failed reading provision file header!\n"));
    return;
  }

  if (!IsRecordConsumptionRequired (&ProvisionFileHeader, ReturnedDataRecord)) {
    DEBUG ((DEBUG_INFO, "Data record should not be removed!\n"));
    return;
  }

  if (!GetFirstUnconsumedDataRecordOffset (&ProvisionFileHeader, &ProvisionDataRecordSize, &ProvisionDataRecordOffset)) {
    DEBUG ((DEBUG_INFO, "Could not find unconsumed data record\n"));
    return;
  }
  if (ProvisionDataRecordSize < ProvisionDataRecord->Header.RecordHeaderByteCount) {
    DEBUG ((DEBUG_INFO, "Invalid Data record size\n"));
    return;
  }

  ProvisionFileHeader.DataRecordsConsumed++;
  ZeroMem (
    (VOID*) (((UINTN)ProvisionDataRecord) + ProvisionDataRecord->Header.RecordHeaderByteCount),
    ProvisionDataRecordSize - ProvisionDataRecord->Header.RecordHeaderByteCount
    );
  ProvisionDataRecord->Header.RecordFlags.Bits.Valid = 0;
  Status = FileHandleSetPosition (ProvisionRecordHandle->ProvisionFileHandle, ProvisionDataRecordOffset);
  if (!EFI_ERROR (Status)) {
    Status = FileHandleWrite (
               ProvisionRecordHandle->ProvisionFileHandle,
               (UINTN *) &ProvisionDataRecordSize,
               (VOID *) ProvisionRecordHandle->ProvisionRecordBuffer
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed writing provision file data record!\n"));
    }
  }
  //
  // Updating field Header.DataRecordsConsumed in Provision File
  //
  ConsumedRecordsNumberSize = sizeof (ProvisionFileHeader.DataRecordsConsumed);
  Status = FileHandleSetPosition (ProvisionRecordHandle->ProvisionFileHandle, OFFSET_OF (PROVISION_FILE_HEADER, DataRecordsConsumed));
  if (!EFI_ERROR (Status)) {
    Status = FileHandleWrite (
               ProvisionRecordHandle->ProvisionFileHandle,
               (UINTN*) &ConsumedRecordsNumberSize,
               &ProvisionFileHeader.DataRecordsConsumed
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed writing provision file header!\n"));
    }
  }

  ASSERT_EFI_ERROR (Status);
}

/**
  Mark provision record as consumed if provision file
  allows consuming and MEBx consumed the record.
  This function should be called after MEBx invocation.

  This function is also responsible for cleanup of all of the
  resources allocated during initial data record discovery.
  After call to this function ProvisionRecord is no longer valid and
  shouldn't be used.

  @param[in] ProvisionRecordHandle  Provision record to consume
  @param[in] ReturnedDataRecord     Provision data record as returned from MEBx.
**/
VOID
RemoveUsbProvisionRecord (
  IN PROVISION_RECORD_HANDLE  *ProvisionRecordHandle,
  IN VOID                     *ReturnedDataRecord
  )
{
  if (ProvisionRecordHandle->HandleValid) {
    RemoveProvisionRecordFromFile (ProvisionRecordHandle, (PROVISION_DATA_RECORD *) ReturnedDataRecord);
    CleanupProvisionRecordResources (ProvisionRecordHandle);
  }
}

/**
  This function displays error message to the user and
  wait for user to read the message and hit a key to continue.

  @param [in] MsgId         Message Id which will be displayed
**/
VOID
DisplayUsbProvisionError (
  IN MEBX_MSG_ID  MsgId
  )
{
  EFI_STATUS          Status;
  MEBX_USER_INPUT     UserInput;

  MebxDisplayError (MsgId);

  MebxDisplayText (PressAnyKeyMsg);

  do {
    Status = MebxReadUserInput (&UserInput);
    if (EFI_ERROR (Status)) {
      MebxDisplayError (ConinError);
    }
  } while (UserInput == MebxUserInputNoInput);
}

/**
  Converts byte-array IpAddress to ASCII string.

  @param [out] String     Output ASCII string
  @param [in]  Binary     Input byte-array IP Address
  @param [in]  IsIpv6     Determines if this is IPv6 string

  @retval EFI_SUCCESS     Conversion success
  @retval Others          Failed to convert binary to string
**/
EFI_STATUS
ConvertIpBinaryToString (
  OUT UINT8   *String,
  IN  UINT8   *Binary,
  IN  BOOLEAN IsIpv6
  )
{
  if (Binary == NULL || String == NULL) {
    return EFI_ABORTED;
  }

  if (IsIpv6) {
    // convert to IPv6 string
    AsciiSPrint (
      (CHAR8 *) String,
      CFG_IPV6_ADDR_LEN_MAX,
      "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
      Binary[15], Binary[14], Binary[13], Binary[12],
      Binary[11], Binary[10], Binary[9], Binary[8],
      Binary[7], Binary[6], Binary[5], Binary[4],
      Binary[3], Binary[2], Binary[1], Binary[0]
      );
  } else {
    // convert to IPv4 string
    AsciiSPrint (
      (CHAR8 *) String,
      CFG_IPV6_ADDR_LEN_MAX,
      "%d.%d.%d.%d",
      Binary[3],
      Binary[2],
      Binary[1],
      Binary[0]
      );
  }

  return EFI_SUCCESS;
}

/**
  Scramble/unscramble all data entries in USB data record.

  @param[in]      ScrambleFunction     Determines whether scramble or unscramble is requested
  @param[in, out] DataRecord           USB data record
**/
VOID
ProcessScrambling (
  IN     BOOLEAN                      ScrambleFunction,
  IN OUT PROVISION_DATA_RECORD_HEADER *DataRecord
  )
{
  UINT8  *TempPtr;
  UINT8  *EndAddress;

  TempPtr = (UINT8*) DataRecord + DataRecord->RecordHeaderByteCount;
  EndAddress = (UINT8*) ((UINTN) DataRecord + DataRecord->RecordChunkCount * USB_DATACHUNK_SIZE);

  if (ScrambleFunction == UNSCRAMBLE) {
    for ( ; TempPtr < EndAddress; TempPtr++) {
      *TempPtr = *TempPtr - SCRAMBLE_VALUE;
    }
  } else {// SCRAMBLE
    for ( ; TempPtr < EndAddress; TempPtr++) {
      *TempPtr = (*TempPtr + SCRAMBLE_VALUE) % 256;
    }
  }
}

/**
  Get user agreement to perform USB Provisioning.

  @retval TRUE              User agreed to perform USB Provisioning
  @retval FALSE             User didn't agree to perform USB Provisioning.
**/
BOOLEAN
GetUserAgreement (
  VOID
  )
{
  BOOLEAN         Result;
  EFI_STATUS      Status;
  MEBX_USER_INPUT UserInput;

  Result = FALSE;

  //
  // print question about usb provisioning
  //
  MebxDisplayText (FoundUsbKeyMsg);
  MebxDisplayText (ContinueAutoProvMsg);

  //
  //Wait for user response and take appropriate action
  //
  while (TRUE) {
    Status = MebxReadUserInput (&UserInput);

    if (EFI_ERROR (Status)) {
      break;
    }

    if (UserInput == MebxUserInputEnter) {
      Result = TRUE;
      break;
    } else if (UserInput == MebxUserInputEsc) {
      Result = FALSE;
      break;
    }
  }

  return Result;
}

/**
  Checks if data entry from an usb DataRecord is valid and can be safely parsed

  @param[in] DataRecord     USB data record
  @param[in] DataEntry      USB data entry

  @retval TRUE              Data entry valid
  @retval FALSE             Data entry invalid
**/
BOOLEAN
DataEntryCorrect (
  IN PROVISION_DATA_RECORD_HEADER *DataRecord,
  IN DATA_RECORD_ENTRY            *DataEntry
  )
{
  UINTN RecordSize;

  //check total data record size
  RecordSize = USB_DATACHUNK_SIZE * DataRecord->RecordChunkCount;

  //is DataEntry's VariableLength outside of record?
  if ((UINTN) (&DataEntry->VariableLength) > (UINTN) (DataRecord) + RecordSize) {
    return FALSE;
  } else {
    //does entry's data continue past the end of record?
    if ((UINTN) (DataEntry) + sizeof (DATA_RECORD_ENTRY) + DataEntry->VariableLength > (UINTN) (DataRecord) + RecordSize) {
      return FALSE;
    }

    //is this entry empty (meaning end of the record)?
    //validity of these fields will be checked when the record is parsed
    if (DataEntry->ModuleId == ModuleIdInvalid && DataEntry->VariableId == 0 && DataEntry->VariableLength == 0) {
      return FALSE;
    }

    //data seems fine and accessing it won't cause buffer overflow
    return TRUE;
  }
}

/**
  Finds first DataEntry from an usb DataRecord. Returns NULL pointer if it can't be found.

  @param[in] DataRecord      USB data record

  @return Pointer to first DataEntry,or  NULL if it was not found
**/
DATA_RECORD_ENTRY*
FindFirstDataEntry (
  IN PROVISION_DATA_RECORD_HEADER *DataRecord
  )
{
  UINT8  *Ptr;

  //avoid pointer math - use pointers to UINT8 instead of bigger structures
  Ptr = (UINT8 *) DataRecord;

  Ptr = Ptr + DataRecord->RecordHeaderByteCount;

  if (!DataEntryCorrect (DataRecord, (DATA_RECORD_ENTRY*) Ptr)) {
    return NULL;
  }
  return (DATA_RECORD_ENTRY*) Ptr;
}

/**
  Given previous DataEntry, finds next DataEntry from an usb DataRecord. Returns NULL pointer if it can't be found

  @param[in] DataRecord     USB data record
  @param[in] DataEntry      USB data entry

  @return Pointer to next DataEntry, or NULL if it was not found

**/
DATA_RECORD_ENTRY*
FindNextDataEntry (
  IN PROVISION_DATA_RECORD_HEADER *DataRecord,
  IN DATA_RECORD_ENTRY            *DataEntry
  )
{
  UINT8  *Ptr;

  //avoid pointer math - use pointers to UINT8 instead of bigger structures
  Ptr = (UINT8 *) DataEntry;

  Ptr = Ptr + sizeof (DATA_RECORD_ENTRY) + ((DataEntry->VariableLength + 3) / 4) * 4;
  if (!DataEntryCorrect (DataRecord, (DATA_RECORD_ENTRY*) Ptr)) {
    return NULL;
  }
  return (DATA_RECORD_ENTRY*) Ptr;
}

/**
  This function checks if manual setup is requested.

  @param[in]  DataRecord     USB Provisioning data record

  @retval TRUE               Manual setup request found
  @retval FALSE              Manual setup request not found
**/
BOOLEAN
IsManualSetupRequested (
  IN  PROVISION_DATA_RECORD_HEADER *DataRecord
  )
{
  DATA_RECORD_ENTRY  *DataEntry;

  DataEntry = FindFirstDataEntry (DataRecord);

  while (DataEntry != NULL) {
    if ((DataEntry->ModuleId == ModuleIdIntelAmtCm) && (DataEntry->VariableId == IntelAmtVariableIdManualSetupAndConfig)) {
      if ((DataEntry->VariableLength != 1) || (DataEntry->VariableValue[0] > 1)) {
        DisplayUsbProvisionError (ManualSetupConfDataInvalidError);
        return FALSE;
      }

      if (DataEntry->VariableValue[0] == 1) {
        return TRUE;
      } else {
        return FALSE;
      }
    }

    DataEntry = FindNextDataEntry (DataRecord, DataEntry);
  }
  return FALSE;
}

/**
  This function searches for password data entry.

  @param[in]  DataRecord     USB Provisioning data record
  @param[out] Password       MEBx password found in the record

  @retval TRUE               Data entry found
  @retval FALSE              Data entry not found or password invalid
**/
BOOLEAN
FindPasswordDataEntry (
  IN  PROVISION_DATA_RECORD_HEADER *DataRecord,
  OUT IMB_ANSI_STRING              *Password
  )
{
  DATA_RECORD_ENTRY  *DataEntry;

  DataEntry = FindFirstDataEntry (DataRecord);

  while (DataEntry != NULL) {
    if ((DataEntry->ModuleId == ModuleIdMeKernel) && (DataEntry->VariableId == MeKernelVariableIdCurrentMebxPwd)) {
      if (DataEntry->VariableLength > MAX_ASCII_PSWD_LENGTH) {
        //password found but invalid (too long)
        return FALSE;
      }
      Password->Length = DataEntry->VariableLength;
      CopyMem (Password->Buffer, DataEntry->VariableValue, DataEntry->VariableLength);
      //password found
      return TRUE;
    }

    DataEntry = FindNextDataEntry (DataRecord, DataEntry);
  }
  //password not found
  return FALSE;
}

/**
  This function searches for manageability data entry.

  @param[in] DataRecord     USB Provisioning data record

  @retval TRUE              Data entry found
  @retval FASLE             Data entry not found or manageability disabled
**/
BOOLEAN
FindManageabilityDataEntry (
  IN PROVISION_DATA_RECORD_HEADER *DataRecord
  )
{
  DATA_RECORD_ENTRY  *DataEntry;

  DataEntry = FindFirstDataEntry (DataRecord);

  while (DataEntry != NULL) {
    if ((DataEntry->ModuleId == ModuleIdMeKernel) && (DataEntry->VariableId == MeKernelVariableIdMngFeatureSel)) {
      if (DataEntry->VariableLength != 1 || DataEntry->VariableValue[0] != ME_FW_CAPS_MANAGEABILITY_SUPP_AMT_ENABLED) {
        DisplayUsbProvisionError (RecordInvalidError);
        //data entry found but invalid
        return FALSE;
      }
      //data entry found and correct
      return TRUE;
    }

    DataEntry = FindNextDataEntry (DataRecord, DataEntry);
  }
  //data entry not found
  return FALSE;

}

/**
  The function processes MeKernelVariableIdLocalFwUpdate record.

  It enables/disables the local firmware update interface basing on a setting read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessLocalFwUpdateVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT8      FwUpdate;
  EFI_STATUS Status;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataRecord->VariableLength != 1) {
    DisplayUsbProvisionError (FwUpdateError);
    return EFI_ABORTED;
  }

  if (DataRecord->VariableValue[0] > 1) {
    DisplayUsbProvisionError (FwUpdateDeprecatedError);
    FwUpdate = 0;
  } else {
    FwUpdate = DataRecord->VariableValue[0];
  }

  Status = HeciSetFwUpdateState (FwUpdate);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (FwUpdateError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdPkiDnsSuffix record.

  The current PKI FQDN DNS Suffix is updated basing on a setting read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessPkiDnsSuffixVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  FQDN_SUFFIX_ANSI_STRING TempPkiDnsSuffix;
  EFI_STATUS              Status;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&TempPkiDnsSuffix, sizeof (FQDN_SUFFIX_ANSI_STRING));

  if ((DataRecord->VariableLength > MAX_ADDRESS_LENGTH) || (DataRecord->VariableLength == 0)) {
    DisplayUsbProvisionError (PkiDnsSuffixInvalidError);
    return EFI_ABORTED;
  }

  CopyMem (TempPkiDnsSuffix.Buffer, DataRecord->VariableValue, DataRecord->VariableLength);
  TempPkiDnsSuffix.Length = DataRecord->VariableLength;

  //
  // Set PKI DNS Suffix
  //
  Status = AmthiSetPkiFqdnSuffix (&TempPkiDnsSuffix);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (PkiDnsSuffixError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdZeroTouchSetupEnable record.

  The Zero Touch Enabled State is updated basing on a setting read from USB provisioning record.
    @attention The Zero Touch Enabled State is updated only if privacy level isn't set to extreme.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessZeroTouchSetupEnableVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT8      PrivacyLevel;
  EFI_STATUS Status;
  UINT32     ZeroTouchEnable;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Omit setting ZTC enabled for extreme privacy level
  //
  Status = AmthiGetAmtPrivacyLevel (&PrivacyLevel);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetPrivacyLevelApiError);
    return Status;
  }

  if (PrivacyLevel == PrivacyLevelExtreme) {
    return EFI_SUCCESS;
  }

  if ((DataRecord->VariableLength != 1) || (DataRecord->VariableValue[0] > 1)) {
    DisplayUsbProvisionError (RemoteConfigEnDisInvalidError);
    return EFI_ABORTED;
  }

  ZeroTouchEnable = DataRecord->VariableValue[0];

  //
  // Set Zero Touch Enabled
  //
  Status = AmthiSetZeroTouchEnabled (ZeroTouchEnable);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (RemoteConfigEnDisError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdUserDefinedCertsConfig record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessUserDefinedCertsConfigVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT8           UserDefinedCertsConfig;
  EFI_STATUS      Status;
  UINT32          NumHandles;
  UINT32          Handles[MAX_HASH_ENTRIES];
  UINTN           Index;
  CERT_HASH_ENTRY TempHash;

  ZeroMem (&TempHash, sizeof (TempHash));

  if ((DataRecord->VariableLength != 1) || (DataRecord->VariableValue[0] > 2)) {
    DisplayUsbProvisionError (CustomHashConfigInvalidError);
    return EFI_ABORTED;
  }

  UserDefinedCertsConfig = DataRecord->VariableValue[0];

  //
  // Get a list of the current hashes on the system
  //
  Status = AmthiEnumerateHashHandles (&NumHandles, Handles);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (CustomHashConfigError);
    return Status;
  }

  //
  // Iterate through hashes to find the user defined hash
  //
  for (Index = 0; Index < NumHandles; Index++) {
    //
    // Determine if this is a user defined hash
    //
    Status = AmthiGetCertificateHashEntry (Handles[Index], &TempHash);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (CustomHashConfigError);
      return Status;
    }

    //
    // If user defined hash, then disable it.
    //
    if (!(TempHash.CertDefault)) {
      //
      // UserDefinedCertsConfig
      // 0 - Disable all user defined certs
      // 1 - Enable all user defined certs
      // 2 - Delete all user defined certs
      //
      if (UserDefinedCertsConfig == DELETE_ALL) {
        Status = AmthiDeleteCustomizedCertificateHashEntry (&Handles[Index]);
        if (EFI_ERROR (Status)) {
          DisplayUsbProvisionError (DeleteHashError);
          return Status;
        }
      } else {
        //
        //Enable=1 and Disable=0 both handled by following
        //
        Status = AmthiSetCertificateHashState (&Handles[Index], UserDefinedCertsConfig);
        if (EFI_ERROR (Status)) {
          DisplayUsbProvisionError (CustomHashStateError);
          return Status;
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdPreinstalledCertEnable record.

  The Certificate Hash States are updated basing on settings read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessPreinstalledCertEnableVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT8           PreInstalledCertEnable;
  EFI_STATUS      Status;
  UINT32          NumHandles;
  UINT32          Handles[MAX_HASH_ENTRIES];
  UINTN           Index;
  CERT_HASH_ENTRY TempHash;

  ZeroMem (&TempHash, sizeof (TempHash));

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DataRecord->VariableLength != 1) || (DataRecord->VariableValue[0] > 1)) {
    DisplayUsbProvisionError (DefaultHashEnInvalidError);
    return EFI_ABORTED;
  }

  PreInstalledCertEnable = DataRecord->VariableValue[0];

  //
  // Get a list of the current hashes on the system
  //
  Status = AmthiEnumerateHashHandles (&NumHandles, Handles);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (DefaultHashEnError);
    return Status;
  }

  for (Index = 0; Index < NumHandles; Index++) {
    //
    //Determine if this is a preinstalled hash
    //
    Status = AmthiGetCertificateHashEntry (Handles[Index], &TempHash);
    if (EFI_ERROR (Status)) {
      if (PreInstalledCertEnable == ENABLE_ALL) {
        DisplayUsbProvisionError (DefaultHashEnError);
      } else {
        DisplayUsbProvisionError (DefaultHashDisError);
      }
      return Status;
    }

    if (TempHash.CertDefault) {
      //
      // enable or disable it according to the parameter passed from file
      //
      Status = AmthiSetCertificateHashState (&Handles[Index], PreInstalledCertEnable);
      if (EFI_ERROR (Status)) {
        if (PreInstalledCertEnable == ENABLE_ALL) {
          DisplayUsbProvisionError (DefaultHashEnError);
        } else {
          DisplayUsbProvisionError (DefaultHashDisError);
        }
        return Status;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdSolStorageRedirConfig record.

  The current SOL and Storage Redirection states are updated basing on settings read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessSolStorageRedirConfigVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  EFI_STATUS Status;
  UINT8      SolStorageRedirData;
  UINT16     TmpSolStatus;
  UINT16     TmpStorageRedirStatus;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataRecord->VariableLength != 1) {
    DisplayUsbProvisionError (SolStorageRedirConfigInvalidError);
    return EFI_ABORTED;
  }

  SolStorageRedirData = DataRecord->VariableValue[0];

  TmpSolStatus          = (UINT16) (SolStorageRedirData & 0x01);      // SOL feature - Bit 0
  TmpStorageRedirStatus = (UINT16) (SolStorageRedirData & 0x02) >> 1; // Storage Redirection feature - Bit 1

  Status = AmthiSetSolStorageRedir (TmpSolStatus, TmpStorageRedirStatus);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (SolStorageRedirDataError);
    return Status;
  }

  //
  // Display deprecation status for Sol/Storage Redirection Authentication Type
  //

  if ((SolStorageRedirData & 0x04) != 0) {    // Authentication type - Bit 2
    DisplayUsbProvisionError (SolStorageRedirAuthProtError);
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdIdleTimeout record.

  The current AMT Idle Timeout is updated basing on a setting read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessIdleTimeoutVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT32     IdleTimeout; // Time in seconds before fw timeout occurs during provisioning
  EFI_STATUS Status;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataRecord->VariableLength != 4) {
    DisplayUsbProvisionError (IdleTimeoutInvalidError);
    return EFI_ABORTED;
  }

  IdleTimeout = ((UINT32*) (DataRecord->VariableValue))[0];

  Status = AmthiSetIdleTimeout ((UINT16) IdleTimeout);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (IdleTimeoutError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdPasswordPolicyFlag record.

  The current MEBx password change policy is updated basing on a setting read from USB provisioning record.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessPasswordPolicyVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  UINT32     PasswordPolicy;
  EFI_STATUS Status;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataRecord->VariableLength != 1) {
    DisplayUsbProvisionError (PwdPolicyInvalidError);
    return EFI_ABORTED;
  }

  PasswordPolicy = DataRecord->VariableValue[0];

  Status = AmthiSetMebxPwdPolicy (PasswordPolicy);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (PwdPolicyInvalidError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdIpv6Settings record.

  The IPv6 settings read from USB provisioning record are used to update current AMT IPV6 LAN Settings.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessIpv6Settings (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  IPv6_DATARECORD                 *IPv6Record;
  UINT8                           InterfaceIndex;
  CFG_IPV6_LAN_INTERFACE_SETTINGS Ipv6Settings;
  EFI_STATUS                      Status;

  ZeroMem (&Ipv6Settings, sizeof (Ipv6Settings));

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataRecord->VariableLength != sizeof (IPv6_DATARECORD)) {
    DisplayUsbProvisionError (Ipv6DataInvalidError);
    return EFI_ABORTED;
  }

  IPv6Record = (IPv6_DATARECORD*) (DataRecord->VariableValue);

  InterfaceIndex = IPv6Record->InterfaceIndex;

  //
  // Copy IPV6 Settings from USB key
  //
  Ipv6Settings.Ipv6Enabled     = IPv6Record->IPv6Enabled;
  Ipv6Settings.InterfaceIdType = IPv6Record->InterfaceIdType;
  CopyMem (Ipv6Settings.InterfaceId.InterfaceId, IPv6Record->InterfaceId, INTERFACE_ID_SIZE);

  //
  // Static settings can only be applied in WIRED interface
  //
  if (InterfaceIndex == LanInterfaceWired) {
    Status = ConvertIpBinaryToString (Ipv6Settings.Ipv6Address.Address, IPv6Record->IPv6Address, TRUE);
    Status |= ConvertIpBinaryToString (Ipv6Settings.Ipv6DefaultRouter.Address, IPv6Record->IPv6DefaultRouter, TRUE);
    Status |= ConvertIpBinaryToString (Ipv6Settings.DnsPrimaryIpv6Address.Address, IPv6Record->DnsPrimaryIPv6Address, TRUE);
    Status |= ConvertIpBinaryToString (Ipv6Settings.DnsSecondaryIpv6Address.Address, IPv6Record->DnsSecondaryIPv6Address, TRUE);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (Ipv6DataInvalidError);
      return Status;
    }
  }

  Status = AmthiSetIpv6LanInterfaceSettings (InterfaceIndex, &Ipv6Settings);
  if (EFI_ERROR (Status) && Status != EFI_NO_MEDIA) {
    DisplayUsbProvisionError (Ipv6SettingError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdKvmState record.

  The variable read from the USB Provisioning data record is used to update current state of KVM Feature.

  @param[in]  DataRecord           Pointer to the data record

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessKvmStateVariable (
  IN DATA_RECORD_ENTRY *DataRecord
  )
{
  EFI_STATUS Status;
  UINT8      KvmState;

  if (DataRecord == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DataRecord->VariableLength != 1) || (DataRecord->VariableValue[0] > 1)) {
    DisplayUsbProvisionError (KvmStateInvalidError);
    return EFI_ABORTED;
  }

  KvmState = DataRecord->VariableValue[0];

  Status = AmthiSetKvmEnabled (KvmState);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (KvmStateError);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The function processes IntelAmtVariableIdMeProvisioningHalt record.

  @param[in]  DataRecord           Pointer to the data record
  @param[out] StartConfig          Information if provisioning should be started

  @retval EFI_SUCCESS              The function succeeded.
  @retval EFI_INVALID_PARAMETER    Invalid parameters.
  @retval EFI_ABORTED              Invalid record.
**/
EFI_STATUS
ProcessMeProvisioningHaltVariable (
  IN     DATA_RECORD_ENTRY *DataRecord,
     OUT BOOLEAN           *StartConfig
  )
{
  UINT8      ProvisionStopStart;
  EFI_STATUS Status;
  UINT32     ProvisioningState;
  UINT32     Timeout;
  UINT32     UnprovisioningState;

  Timeout = MEBX_PROVISIONING_TIMEOUT;

  if (DataRecord == NULL || StartConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DataRecord->VariableLength != 1) || (DataRecord->VariableValue[0] > 1)) {
    DisplayUsbProvisionError (MeProvHaltDataInvalidError);
    return EFI_ABORTED;
  }

  ProvisionStopStart = DataRecord->VariableValue[0];

  //
  // 0 - Stop Configuration
  // 1 - Start Configuration
  //
  if (ProvisionStopStart == 0) {
    //
    // Get Provision Status
    //
    Status = AmthiGetProvisioningState (&ProvisioningState);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (FailedGetProvStatusError);
      return Status;
    }

    if (ProvisioningState == ProvisioningStateIn) {
      Status = AmthiStopConfiguration ();
      if (EFI_ERROR (Status)) {
        DisplayUsbProvisionError (MeProvHaltError);
        return Status;
      }

      // After stop configuration is sent, poll unprovisioning state until it has left the IN state.
      Status = AmthiGetUnprovisioningState (&UnprovisioningState);
      if (EFI_ERROR (Status)) {
        MebxDisplayError (AmthiStopConfigUnprovisionApiError);
        return Status;
      }

      while ((UnprovisioningState == UnprovisioningStateIn) && (Timeout != 0)) {
        Status = AmthiGetUnprovisioningState (&UnprovisioningState);
        if (EFI_ERROR (Status)) {
          MebxDisplayError (AmthiStopConfigUnprovisionApiError);
          return Status;
        }

        // Delay 1s
        MicroSecondDelay (MEBX_STALL_1_SECOND);
        Timeout--;
      }

    }
  } else if (ProvisionStopStart == 1) {
    *StartConfig = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  This function validates the USB Provisioning data record and parses through
  each data entry and passes the data to ME/AMT FW.

  @param[in] MebxUsbRecordPtr     Pointer to the USB Provisioning record

  @retval EFI_SUCCESS             USBProvisioning was successful
  @retval Others                  USB Provisioning has failed
**/
EFI_STATUS
MebxOneTouchProvisioning (
  IN VOID *MebxUsbRecordPtr
  )
{
  UINT32                          ProvisioningState = 0;
  UINT8                           SetPowerpackage = 0;
  BOOLEAN                         SkipAMTSettings = FALSE;
  MEFWCAPS_SKU                    FwFeatures;
  BOOLEAN                         MaxRetries;
  BOOLEAN                         NewPasswordFound = FALSE;
  BOOLEAN                         OptInDataPresent = FALSE;
  UINT32                          TempKVMOptIn = 0;
  UINT32                          TempRemoteKvmOptIn = 0;
  UINT32                          AmtStatus = 0;                // Used to hold the status returned by fw for AMTHI messages
  UINT8                           ProvServerDataExists = FALSE;
  UINT32                          NumHandles = 0;               // Number of hash Handles present in system
  UINTN                           i = 0;
  BOOLEAN                         StartConfig = FALSE;
  UINT8                           ExistingCustomHashCount = 0;
  BOOLEAN                         FqdnDataPresent = FALSE;
  BOOLEAN                         StaticIpFound = FALSE;
  BOOLEAN                         DhcpFound = FALSE;
  UINT32                          DhcpMode = 0;                 // DhcpMode enabled or disabled
  UINT8                           NumBytesInCert = 0;
  UINT8                           CustomHashNameLength = 0;
  UINT8                           CustomHashNameIndex = 0;
  UINT32                          Handles[MAX_HASH_ENTRIES];    // Handle array of certificate hashes
  BOOLEAN                         HostNameFound = FALSE;
  BOOLEAN                         DomainNameFound = FALSE;
  UINT16                          DomainNameLength = 0;
  UINT8                           Buffer[MAX_DOMAINNAME_LENGTH + MAX_HOSTNAME_LENGTH];
  UINT16                          ProvisioningServerPort = 0;
  FQDN_ANSI_STRING                TempConfigServerFQDN;
  USER_DEFINED_CERT_ADD           CustomizedHashes[MAX_CUSTOMIZED_HASHES];
  FQDN_DATA                       FqdnData;
  CERT_HASH_ENTRY                 TempHash;
  CFG_IP_ADDR                     ServerAddress;
  IMB_ANSI_STRING                 NewPasswordBuf;
  IMB_ANSI_STRING                 Password;
  PROVISION_DATA_RECORD_HEADER    *pDataRecord;
  DATA_RECORD_ENTRY               *pRecordEntryBase;
  IMB_TCPIP_PARAMS                TcpIpParams;
  EFI_GUID                        MePowerPackage;
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINTN                           CustomHashCount = 0;          // number of user defined hashes currently in the system
  UINT8                           *UsbProvScratchBuffer;
  UINTN                           NumberOfChunks;
  ME_PWD_STATUS                   PwdChangeStatus;
  UINT8                           LanLessPlatform = 0;
  MEBX_USER_INPUT                 UserInput;
  PROVISION_DATA_RECORD           *ProvisioningDataRecord;

  ZeroMem (&FqdnData, sizeof (FqdnData));
  ZeroMem (&TempHash, sizeof (TempHash));
  ZeroMem (&ServerAddress, sizeof (ServerAddress));
  ZeroMem (&NewPasswordBuf, sizeof (NewPasswordBuf));
  ZeroMem (&Password, sizeof (Password));
  ZeroMem (&TcpIpParams, sizeof (TcpIpParams));
  ZeroMem (&MePowerPackage, sizeof (MePowerPackage));

  if (!GetUserAgreement ()) {
    return EFI_SUCCESS;
  }

  ProvisioningDataRecord = MebxUsbRecordPtr;

  //
  // check how many data chunks are there
  //
  NumberOfChunks = ProvisioningDataRecord->Header.RecordChunkCount;

  if (NumberOfChunks == 0 ||
      NumberOfChunks > 10 ||
      ProvisioningDataRecord->Header.RecordFlags.Bits.Valid != DATA_RECORD_FLAG_VALID ||
      ProvisioningDataRecord->Header.RecordHeaderByteCount == 0) {
    DisplayUsbProvisionError (RecordInvalidError);
    return EFI_ABORTED;
  }

  //
  //Copy USB Config Data to local buffer. Skip useless header added by BIOS.
  //
  UsbProvScratchBuffer = AllocatePool (NumberOfChunks * USB_DATACHUNK_SIZE);
  if (UsbProvScratchBuffer == NULL) {
    ASSERT (UsbProvScratchBuffer != NULL);;
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (UsbProvScratchBuffer, &ProvisioningDataRecord->Header, NumberOfChunks * USB_DATACHUNK_SIZE);

  pDataRecord = (PROVISION_DATA_RECORD_HEADER*) UsbProvScratchBuffer;

  if (pDataRecord->RecordFlags.Bits.Scrambled == 1) {
    ProcessScrambling (UNSCRAMBLE, pDataRecord);
  }

  if (IsManualSetupRequested (pDataRecord)) {
    DisplayUsbProvisionError (AmtManualConfigUnsupportedMsg);
    goto Exit;
  }

  if (FindPasswordDataEntry (pDataRecord, &Password)) {
    //
    //Log in with the password provided by USB key
    //
    Status = HeciValidateMebxPassword ((UINT8) (Password.Length), Password.Buffer, &MaxRetries);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (LoginFailedError);
      goto Exit;
    }
  } else {
    DisplayUsbProvisionError (ProvDataMissingError);
    goto Exit;
  }

  //
  //Get the Manageability Mode to check if AMT is enabled
  //
  Status = HeciGetFwFeatureStateMsg (&FwFeatures);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (FailedEnabledFeaturesError);
    goto Exit;
  }

  //
  //If AMT is not enabled, then set AMT from USB key
  //
  if (!(FwFeatures.Fields.Amt)) {
    if (FindManageabilityDataEntry (pDataRecord)) {
      FwFeatures.Fields.Amt = 1;

      MebxDisplayText (LoadingAmtMsg);

      //
      // Update manageability mode
      //
      Status = HeciSetFwEnabledFeatures (FwFeatures.Data, AMT_BITMASK);
      if (EFI_ERROR (Status)) {
        DisplayUsbProvisionError (FailedUpdateManageabilityModeError);
        goto Exit;
      }

      //
      //Delay is required to wait for the enable message to propagate to all
      //app rules. Without this delay the app rules could return incorrect data.
      //
      MicroSecondDelay (AMT_ENABLE_DELAY);

      //
      //If the feature is being enabled, we need to wait until the corresponding
      //available bit is set before MEBx can send messages to this feature.
      //If the feature is being disabled, we are done with the update.
      //

      Status = MeWaitFwFeatureAvailable (AMT_BITMASK, WAIT_FOR_FW_FEATURE_DEFAULT_TIMEOUT);
      if (EFI_ERROR (Status)) {
        DisplayUsbProvisionError (AppFailedLoadError);
        goto Exit;
      }

      // check if this is LAN-less platform
      Status = AmthiGetLanInterfaceSettings (NULL, 0);
      if (EFI_ERROR (Status) && Status == EFI_NO_MEDIA) {
        LanLessPlatform = 1;
      }

      //
      //Indicate AMT is loaded.
      //
      MebxDisplayText (DoneMsg);
    } else {
      MebxDisplayText (AmtManageabilityUsbDataMissingMsg);
      MicroSecondDelay (2 * PT_CONSOLE_DELAY);
      SkipAMTSettings = TRUE;
    }
  }

  if (!SkipAMTSettings) {
    //
    //Get Provision Status
    //
    Status = AmthiGetProvisioningState (&ProvisioningState);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (FailedGetProvStatusError);
      goto Exit;
    }

    //
    //If provision mode in POST, display warning.
    //
    if (ProvisioningState == ProvisioningStatePost) {
      MebxDisplayText (AmtManageabilityAlreadyProvMsg);
      goto Exit;
    }
  }

  pRecordEntryBase = FindFirstDataEntry (pDataRecord);

  while (pRecordEntryBase != NULL) {
    if (pRecordEntryBase->ModuleId == ModuleIdMeKernel) {
      //
      //Read kernel data.
      //
      switch (pRecordEntryBase->VariableId) {
        case MeKernelVariableIdNewMebxPwd:
          //
          //If VariableID == NewPassword save it for later send
          //
          if (pRecordEntryBase->VariableLength > MAX_ASCII_PSWD_LENGTH) {
            break;
          }
          NewPasswordFound = TRUE;
          NewPasswordBuf.Length = pRecordEntryBase->VariableLength;
          CopyMem (NewPasswordBuf.Buffer, pRecordEntryBase->VariableValue, pRecordEntryBase->VariableLength);

          break;

        case MeKernelVariableIdLocalFwUpdate:
          Status = ProcessLocalFwUpdateVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case MeKernelVariableIdPowerPkgs:
          if (pRecordEntryBase->VariableLength != sizeof (EFI_GUID)) {
            break;
          }
          CopyMem ((UINT8*) &MePowerPackage, pRecordEntryBase->VariableValue, pRecordEntryBase->VariableLength);
          SetPowerpackage = 1; //RCR 1023083
          break;

          //
          //No action required on CurrentPassword/Manageability cases - this is already handled
          //
        case MeKernelVariableIdCurrentMebxPwd:
        case MeKernelVariableIdMngFeatureSel:
          break;

      default:
        MebxDisplayErrorId (pRecordEntryBase->ModuleId, pRecordEntryBase->VariableId);
        goto Exit;
      }
    } else if ((pRecordEntryBase->ModuleId == ModuleIdIntelAmtCm) && (!SkipAMTSettings)) {
      switch (pRecordEntryBase->VariableId) {
        case IntelAmtVariableIdPkiDnsSuffix:
          Status = ProcessPkiDnsSuffixVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdConfigServerFqdn:

          if ((pRecordEntryBase->VariableLength > MAX_ADDRESS_LENGTH) || (pRecordEntryBase->VariableLength == 0)) {
            DisplayUsbProvisionError (ConfigServerFqdnInvalidError);
            goto Exit;
          }

          CopyMem (TempConfigServerFQDN.Buffer, pRecordEntryBase->VariableValue, pRecordEntryBase->VariableLength);
          TempConfigServerFQDN.Length = pRecordEntryBase->VariableLength;
          ProvServerDataExists = TRUE;
          break;

        case IntelAmtVariableIdZeroTouchSetupEnable:
          Status = ProcessZeroTouchSetupEnableVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdPreinstalledCertEnable:
          Status = ProcessPreinstalledCertEnableVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdUserDefinedCertsConfig:
          Status = ProcessUserDefinedCertsConfigVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdUserDefinedCertAdd:

          //
          //UserDefinedCertAdd
          //Byte: Hash Algorithm
          //1 - SHA1
          //2 - SHA2 - 256
          //3 - SHA2 - 384
          //Byte[20]: Cert SHA-1 hash
          //Byte[21]: ASCII Cert Friendly Name Length
          //Byte[22:..]: ASCII Cert Friendly Name
          //Verify data is valid
          //
          if ((pRecordEntryBase->VariableLength < 22) || (pRecordEntryBase->VariableLength > 81)) {
            DisplayUsbProvisionError (InvalidCustomHashError);
            goto Exit;
          }

          //
          //Copy record entry into CustomizedHashes struct for storage.
          //Byte 0 - Algorithm Type
          //
          CustomizedHashes[CustomHashCount].AlgorithmType = pRecordEntryBase->VariableValue[0];

          switch (pRecordEntryBase->VariableValue[0]) {
            case CertHashAlgorithmSha1:
              NumBytesInCert = 20;
              break;

            case CertHashAlgorithmSha256:
              NumBytesInCert = 32;
              break;

            case CertHashAlgorithmSha384:
              NumBytesInCert = 48;
              break;

            default:
              break;
          }

          //
          //Bytes 1-20 - Cert SHA-1 Hash
          //Bytes 1-32 - Cert SHA-2 256
          //Bytes 1-48 - Cert SHA-2 384
          //

          CopyMem (CustomizedHashes[CustomHashCount].CertHash, &pRecordEntryBase->VariableValue[1], NumBytesInCert);

          //
          //Bytes 21-52 - Cert name for SHA1
          //Byres 33-64 - Cert name for SHA2 256
          //Bytes 49-80 - Cert name for SHA2 356
          //
          CustomHashNameLength = pRecordEntryBase->VariableValue[NumBytesInCert + 1];
          CustomHashNameIndex =  NumBytesInCert + 2;

          CopyMem (CustomizedHashes[CustomHashCount].CertName.Buffer, &pRecordEntryBase->VariableValue[CustomHashNameIndex], CustomHashNameLength);
          CustomizedHashes[CustomHashCount].CertName.Length = CustomHashNameLength;

          //
          //Do not update IA_VID_USERDEFINEDCERTADD flag yet, there could be multiple user defined certs.
          //
          CustomHashCount++;
          break;

        case IntelAmtVariableIdSolStorageRedirConfig:
          Status = ProcessSolStorageRedirConfigVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdHostname:

          if (pRecordEntryBase->VariableLength > MAX_HOSTNAME_LENGTH) {
            DisplayUsbProvisionError (HostNameLargeError);
            goto Exit;
          }

          if (DomainNameFound == FALSE) {
            //
            //Domain name not present in buffer
            //
            CopyMem (FqdnData.Fqdn.Buffer, pRecordEntryBase->VariableValue, pRecordEntryBase->VariableLength);
            FqdnData.HostNameLength = pRecordEntryBase->VariableLength;
          } else {
            //
            //Domain name already present in buffer
            //Copy domain name to temp buffer
            //
            CopyMem (Buffer, FqdnData.Fqdn.Buffer, DomainNameLength+1);
            //
            //Copy Hostname to FQDN Buffer
            //
            CopyMem (
              (UINT8*) FqdnData.Fqdn.Buffer,
              (UINT8*) (pRecordEntryBase->VariableValue),
              pRecordEntryBase->VariableLength
              );
            FqdnData.HostNameLength = pRecordEntryBase->VariableLength;
            CopyMem (
              (UINT8*) &FqdnData.Fqdn.Buffer[FqdnData.HostNameLength + 1],
              Buffer,
              DomainNameLength + 1
              );
          }

          FqdnDataPresent = TRUE;
          HostNameFound = TRUE;

          break;

        case IntelAmtVariableIdDomainName:

          if (pRecordEntryBase->VariableLength > MAX_DOMAINNAME_LENGTH) {
            DisplayUsbProvisionError (DomainNameLargeError);
            goto Exit;
          }

          if (HostNameFound) {
            //
            //If Hostname is already present, we need to add a "." after the hostname.
            //
            CopyMem ((UINT8*) &FqdnData.Fqdn.Buffer[FqdnData.HostNameLength], ".", 1);
            CopyMem (
              (UINT8*) &FqdnData.Fqdn.Buffer[FqdnData.HostNameLength + 1],
              (UINT8*) (pRecordEntryBase->VariableValue),
              pRecordEntryBase->VariableLength
              );
          } else {
            //
            //else the "." is in the beginning.
            //
            CopyMem ((UINT8*) &FqdnData.Fqdn.Buffer[0], ".", 1);
            CopyMem (
              (UINT8*) &FqdnData.Fqdn.Buffer[1],
              (UINT8*) (pRecordEntryBase->VariableValue),
              pRecordEntryBase->VariableLength
              );
          }

          DomainNameLength = pRecordEntryBase->VariableLength;
          DomainNameFound = TRUE;
          FqdnDataPresent = TRUE;

          break;

        case IntelAmtVariableIdDhcp:
          if ((pRecordEntryBase->VariableLength != 1) || (pRecordEntryBase->VariableValue[0] > 2) || (pRecordEntryBase->VariableValue[0] < 1)) {
            DisplayUsbProvisionError (DhcpInvalidError);
            goto Exit;
          }

          //
          //only update the dhcp mode.
          //
          DhcpMode = pRecordEntryBase->VariableValue[0];
          DhcpFound = TRUE;

          break;

        case IntelAmtVariableIdIdleTimeout:
          Status = ProcessIdleTimeoutVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdProvisioningServerAddress:
          if ((pRecordEntryBase->VariableLength != IPV4_BINARY_ADDRESS_SIZE) && (pRecordEntryBase->VariableLength != IPV6_BINARY_ADDRESS_SIZE)) {
            DisplayUsbProvisionError (ProvServerInvalidError);
            goto Exit;
          }

          if (pRecordEntryBase->VariableLength == IPV6_BINARY_ADDRESS_SIZE) {
            ConvertIpBinaryToString (ServerAddress.Address, pRecordEntryBase->VariableValue, TRUE);
          } else {
            ConvertIpBinaryToString (ServerAddress.Address, pRecordEntryBase->VariableValue, FALSE);
          }

          ProvServerDataExists = TRUE;
          break;

        case IntelAmtVariableIdProvisioningServerPort:
          if (pRecordEntryBase->VariableLength != 2) {
            DisplayUsbProvisionError (ProvServerPortInvalidError);
            goto Exit;
          }

          ProvisioningServerPort = ((UINT16*) (pRecordEntryBase->VariableValue))[0];
          break;

        case IntelAmtVariableIdStaticIpv4Parameters:
          if (pRecordEntryBase->VariableLength != 20) {
            DisplayUsbProvisionError (Ipv4ParamsInvalidError);
            goto Exit;
          }

          CopyMem ((UINT8*) &TcpIpParams.LocalAddr, pRecordEntryBase->VariableValue, 20);
          ZeroMem ((UINT8*) &TcpIpParams.DomainName, sizeof (TcpIpParams.DomainName));
          StaticIpFound = TRUE;
          break;

        case IntelAmtVariableIdPasswordPolicyFlag:
          Status = ProcessPasswordPolicyVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdIpv6Settings:
          Status = ProcessIpv6Settings (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdSharedFqdn:

          if ((pRecordEntryBase->VariableLength != 1) || (pRecordEntryBase->VariableValue[0] > 1)) {
            DisplayUsbProvisionError (SharedFqdnInvalidError);
            goto Exit;
          }

          FqdnData.SharedFqdn = pRecordEntryBase->VariableValue[0];
          FqdnDataPresent = TRUE;

          break;

        case IntelAmtVariableIdKvmState:
          Status = ProcessKvmStateVariable (pRecordEntryBase);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdKvmOptinUser:

          if ( ((pRecordEntryBase->VariableValue[0] > OPT_IN_KVM)
            && (pRecordEntryBase->VariableValue[0] != USB_TOOL_OPT_IN_ALL))
            || (pRecordEntryBase->VariableLength != 1) ) {
            DisplayUsbProvisionError (OptInDataInvalidError);
            goto Exit;
          }

          if (pRecordEntryBase->VariableValue[0] == USB_TOOL_OPT_IN_ALL) {
            //
            //AMTHI message requires a UINT32 of all F's.
            //
            TempKVMOptIn = OPT_IN_ALL;
          } else {
            TempKVMOptIn = pRecordEntryBase->VariableValue[0];
          }

          OptInDataPresent = TRUE;
          break;

        case IntelAmtVariableIdKvmOptinRemoteIt:

          if (((pRecordEntryBase->VariableValue[0] > OPT_IN_KVM)
            && (pRecordEntryBase->VariableValue[0] != 0xFF))
            || (pRecordEntryBase->VariableLength != 1)) {
            DisplayUsbProvisionError (OptInDataInvalidError);
            goto Exit;
          }

          TempRemoteKvmOptIn = pRecordEntryBase->VariableValue[0];
          OptInDataPresent = TRUE;
          break;

        case IntelAmtVariableIdMeProvisioningHalt:
          Status = ProcessMeProvisioningHaltVariable (pRecordEntryBase, &StartConfig);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;

        case IntelAmtVariableIdDdnsUpdate:
        case IntelAmtVariableIdManualSetupAndConfig:
          // Skipping. Setting no longer supported
          break;

        default:
          MebxDisplayErrorId (pRecordEntryBase->ModuleId, pRecordEntryBase->VariableId);
          goto Exit;
      }
    } else if (pRecordEntryBase->ModuleId != ModuleIdIntelAmtCm) {
      MebxDisplayErrorId (pRecordEntryBase->ModuleId, pRecordEntryBase->VariableId);
      goto Exit;
    }

    pRecordEntryBase = FindNextDataEntry (pDataRecord, pRecordEntryBase);
  }

  if (SkipAMTSettings) {
    goto USBexit;
  }

  //
  //Set provisioning Server Data
  //
  if (ProvServerDataExists) {
    Status = AmthiSetConfigServerData (ProvisioningServerPort, ServerAddress.Address, &TempConfigServerFQDN);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (ProvServAddressError);
      goto Exit;
    }
  }

  //
  //if CustomHashCount is non zero we know that there was a valid
  //USERDEFINEDCERTADD data record and the user wishes to add a
  //customized hash to the system.
  //
  if (CustomHashCount > 0) {
    //
    //Get a list of the current hashes on the system
    //
    Status = AmthiEnumerateHashHandles (&NumHandles, Handles);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (HashHandlesError);
      goto Exit;
    }

    //
    //If adding new customized hashes, first check how many there are already.
    // If sum exceeds 3, print error message and skip
    //
    for (i = 0; i < NumHandles; i++) {
      //
      //Determine if this is a user defined hash
      //
      Status = AmthiGetCertificateHashEntry (Handles[i], &TempHash);
      if (EFI_ERROR (Status)) {
        DisplayUsbProvisionError (HashEntriesError);
        goto Exit;
      }

      //
      //If user defined hash, then mark it.
      //
      if (!(TempHash.CertDefault)) {
        ExistingCustomHashCount++;
      }
    }

    if ((CustomHashCount + ExistingCustomHashCount) > 3) {
      MebxDisplayText (StringNotAppliedTooManyHashesMsg);
      MicroSecondDelay (PT_CONSOLE_DELAY);
      goto skipCHadd;
    }

    //
    //Now add the new user defined hashes from the USB key
    //

    for (i = 0; i < CustomHashCount; i++) {
      UINT32 IsActive = 1;
      //
      //Add the customized hashes with IsActive bit always asserted
      //
      Status = AmthiAddCustomizedCertificateHashEntry (
                 &IsActive,
                 CustomizedHashes[i].CertHash,
                 &CustomizedHashes[i].AlgorithmType,
                 &CustomizedHashes[i].CertName
                 );
      if (EFI_ERROR (Status)) {
        DisplayUsbProvisionError (AddCustomHashError);
        goto Exit;
      }
    }
  }

  Status = AmthiGetProvisioningState (&ProvisioningState);
  if (EFI_ERROR (Status)) {
    DisplayUsbProvisionError (FailedGetProvStatusError);
    goto Exit;
  }

  if ((StartConfig == TRUE) && (ProvisioningState == ProvisioningStatePre)) {
    //
    //RCR 1022955 - IPV6 disable default state
    //
    Status = AmthiStartConfigurationEx (&AmtStatus);
    if (EFI_ERROR (Status)) {
      //
      //Bug 2948090 - Ignore error code 49 with start configuration command.
      //
      if (AmtStatus != AmtStatusCertificateNotReady) {
        DisplayUsbProvisionError (MeProvActivateError);
        goto Exit;
      }
    }
  }

skipCHadd:

  if (DhcpFound || StaticIpFound) {
    if (DhcpFound) {
      TcpIpParams.DhcpMode = DhcpMode;
    }

    if (!LanLessPlatform && (AmthiSetIpv4Params (&TcpIpParams) != EFI_SUCCESS)) {
      DisplayUsbProvisionError (Ipv4ParametersError);
      goto Exit;
    }
  }

  if (FqdnDataPresent) {
    FqdnData.Fqdn.Length = (UINT16) AsciiStrLen ((CHAR8 *) &FqdnData.Fqdn.Buffer);

    Status = AmthiSetFqdn (&FqdnData);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (FqdnSettingError);
      goto Exit;
    }
  }

  //
  //Set KVM Data
  //
  if (OptInDataPresent) {
    Status = AmthiSetOptInState (TempKVMOptIn, TempRemoteKvmOptIn);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (OptinDataError);
      goto Exit;
    }
  }

  //
  //send new password last, if available in this record
  //
  if (NewPasswordFound) {
    //
    //Send the new password to ME
    //
    PwdChangeStatus = HeciModifyMebxPassword (
                        (UINT8) (Password.Length),
                        Password.Buffer,
                        (UINT8) (NewPasswordBuf.Length),
                        NewPasswordBuf.Buffer
                        );
    if (PwdChangeStatus != ME_PWDSTS_PWD_SUCCESS) {
      DisplayUsbProvisionError (NewPasswordError);
      goto Exit;
    }
  }

USBexit:

  if (NewPasswordFound && SkipAMTSettings) {
    PwdChangeStatus = HeciModifyMebxPassword (
                        (UINT8) (Password.Length),Password.Buffer,
                        (UINT8)(NewPasswordBuf.Length),
                        NewPasswordBuf.Buffer
                        );
    if (PwdChangeStatus != ME_PWDSTS_PWD_SUCCESS) {
      DisplayUsbProvisionError (NewPasswordError);
      goto Exit;
    }
  }

  if (SetPowerpackage == 1) {
    Status = AmthiSetPowerPackage (MePowerPackage);
    if (EFI_ERROR (Status)) {
      DisplayUsbProvisionError (PowerPackagesError);
      goto Exit;
    }
  }

  //
  //This is the last operation, mark the record consumed now
  //if we get here Clear valid flag to indicate data was used by MEBx.
  //
  pDataRecord->RecordFlags.Bits.Valid = 0;

  if (pDataRecord->RecordFlags.Bits.Scrambled == 1) {
    ProcessScrambling (SCRAMBLE, pDataRecord);
  }

  //
  //Copy USB record buffer
  //
  CopyMem (&ProvisioningDataRecord->Header, &UsbProvScratchBuffer, NumberOfChunks * USB_DATACHUNK_SIZE);

  //
  //Display a success message to the user and exit MEBx
  //
  MebxDisplayText (ConfigAppliedMsg);

  //
  //TS: OEM complaining about this since BIOS does not get chance to cleanup the key if
  //user does power down.
  //MebxAsciiPrint("Please power down the system for settings to take effect\n");
  //

  MebxDisplayText (ContinueBootMsg);

  do {
    Status = MebxReadUserInput (&UserInput);
    if (EFI_ERROR (Status)) {
      break;
    }
  } while (UserInput == MebxUserInputNoInput);

  Status = EFI_SUCCESS;

Exit:
  //
  // free allocated buffers, destroy copies of passwords
  //
  ZeroMem (&Password.Length, sizeof (IMB_ANSI_STRING));
  ZeroMem (&NewPasswordBuf.Length, sizeof (IMB_ANSI_STRING));
  ZeroMem (UsbProvScratchBuffer, NumberOfChunks*USB_DATACHUNK_SIZE);

  FreePool (UsbProvScratchBuffer);

  return Status;
}
