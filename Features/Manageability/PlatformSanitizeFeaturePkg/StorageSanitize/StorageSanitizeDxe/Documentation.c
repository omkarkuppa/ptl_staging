/**@file
  Documentation for Storage erase driver

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

#include "StorageErase.h"

CHAR8 ClearMethodString[] = "Clear with \"00\"";

CHAR8 *EraseTypeString[] = {
  "Clear",
  "Purge",
  "Overwrite",
  "Block Erase",
  "Crypto Erase"
};

CHAR8 *MediaTypeString[] = {
  "NVMe",
  "UFS",
  "SATA",
  "NOT_DEFINED"
};

/**
  Get string of erase category

  @param[in] EraseType  Erase type

  @return String matched to the erase category.

**/
CHAR8*
GetMethodType (
  IN UINT8 EraseType
  )
{
  if (EraseType == 1) {  // Clear
    return EraseTypeString[0];
  } else {
    return EraseTypeString[1];
  }
}

/**
  Get string of erase type

  @param[in] EraseType  Erase type

  @return String matched to the erase type.

**/
CHAR8*
GetMethodUsed (
  IN UINT8 EraseType
  )
{
  if (EraseType == 1) {  // Clear
    return ClearMethodString;
  } else {
    return EraseTypeString[EraseType];
  }
}

/**
  Get string of device type

  @param[in] DevType Device type

  @return String matched to the device type.

**/
CHAR8*
GetMediaType (
  IN DEVICE_TYPE DevType
  )
{
  switch (DevType) {
    case NVME:
      return MediaTypeString[0];
    case UFS:
      return MediaTypeString[1];
    case SATA:
      return MediaTypeString[2];
    case NOT_DEFINED:
      return MediaTypeString[3];
  }
  return NULL;
}

/**
  Print the information of all erase devices for debugging purpose.

**/
VOID
PrintReportData (
  VOID
  )
{
  UINTN        DeviceIndex;
  LIST_ENTRY   *Node;
  DOC_INFO     *DocInfo;

  DeviceIndex = 0;

  DEBUG ((DEBUG_INFO, "==========================================="));
  // Device info
  for (Node = GetFirstNode (&mStorageEraseDriver.DocInfoList);
        !IsNull (&mStorageEraseDriver.DocInfoList, Node);
        Node = GetNextNode (&mStorageEraseDriver.DocInfoList, Node)) {

    DeviceIndex = DeviceIndex + 1;
    DEBUG ((DEBUG_INFO, "\nDevice: %d /", DeviceIndex));
    DocInfo = DOC_INFO_FROM_LINK (Node);

    // Model
    DEBUG ((DEBUG_INFO, " Model: %a /", DocInfo->ModelName));
    // Serial Number
    DEBUG ((DEBUG_INFO, " Serial Number: %a /", DocInfo->SerialNumber));
    // Media Type
    DEBUG ((DEBUG_INFO, " Media Type: %a /", DocInfo->MediaType));
    // Sanitization Description
    DEBUG ((DEBUG_INFO, " Sanitization Description: %a /", DocInfo->EraseCat));
    // Method used
    DEBUG ((DEBUG_INFO, " Method used: %a ", DocInfo->EraseType));
    // Result
    DEBUG ((DEBUG_INFO, " Result: %r ", DocInfo->ValResult));
  }

  DEBUG ((DEBUG_INFO, "\n===========================================\n"));

}

/**
  Update DocInfo list by internal data list

**/
VOID
AddDocInfoToList (
  VOID
  )
{
  LIST_ENTRY             *Node2;
  PLATFORM_ERASE_DEVICE  *Dev;
  DOC_INFO               *DocInfo;

  //
  // Find all platform erase device which linked to the same setup device
  //
  for (Node2 = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
        !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node2);
        Node2 = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node2)) {
    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node2);

    if (!Dev->Selected) {
      continue;
    }

    // Create new doc node
    DocInfo = AllocateZeroPool (sizeof (DOC_INFO));
    if (DocInfo == NULL) {
      DEBUG((DEBUG_INFO, "Allocate DOC_INFO failed.\n"));
      continue;
    }
    DocInfo->Signature = DOC_INFO_SIGNATURE;
    // model name
    DocInfo->ModelName = Dev->EraseInfo.ModelName;
    // serial number
    DocInfo->SerialNumber = Dev->EraseInfo.SerialNumber;
    // Media Type
    DocInfo->MediaType = GetMediaType (Dev->DeviceType);
    // Erase Category
    DocInfo->EraseCat = GetMethodType (Dev->EraseInfo.Method);
    // Erase Type
    DocInfo->EraseType = GetMethodUsed (Dev->EraseInfo.Method);
    DocInfo->ValResult = EFI_SUCCESS;
    InsertTailList (&mStorageEraseDriver.DocInfoList, &DocInfo->Link);

    if ((Dev->Result != ERASE_RESULT_SUCCESS) && (DocInfo != NULL)) {
      DocInfo->ValResult = EFI_DEVICE_ERROR;
    }
  }
  PrintReportData ();
}

/**
  Display message of USB detecting result.

  @param[in] PopUpString1     The first line message
  @param[in] PopUpString2     The second line message
  @param[in] Status           If Status is not NULL, the value would be appended to PopUpString2.

**/
VOID
PopUpMsg (
  IN CHAR16         *PopUpString1,
  IN CHAR16         *PopUpString2,
  IN EFI_STATUS     *Status
  )
{
  CHAR16 StringWithStatus[200];
  UINTN  MsgAttribute;
  UINTN  MsgRelay;

  MsgAttribute = EFI_DARKGRAY | EFI_BACKGROUND_CYAN;
  MsgRelay     = 3000000; // three seconds delay after poping up message

  if (Status != NULL) {
    ZeroMem (StringWithStatus, sizeof (CHAR16) * 200);
    UnicodeSPrint (StringWithStatus, sizeof (StringWithStatus), L"%s (%r)", PopUpString2, *Status);
    PopUpString2 = StringWithStatus;

    if (EFI_ERROR (*Status)) {
      MsgAttribute = EFI_LIGHTGRAY | EFI_BACKGROUND_RED;
      MsgRelay     = MsgRelay + 3000000; // Three more seconds delay for error status case.
    }
  }

  CreatePopUp (
    MsgAttribute,
    NULL,
    PopUpString1,
    PopUpString2,
    NULL
    );

  gBS->Stall (MsgRelay);
  ClearPopupScreen();
}

/**
  ActionToGetSanitizationReport

  @param[in] PopUpString1  The first line message
  @param[in] PopUpString2  The second line message
  @param[in] PopUpString3  Line message.
  @param[out] GetResponse  Yes/No to report sanitization result.

**/
VOID
ActionToGetSanitizationReport  (
  IN CHAR16                 *PopUpString,
  IN CHAR16                 *PopUpString2,
  IN CHAR16                 *PopUpString3,
  OUT BOOLEAN               *GetResponse
  )
{
  EFI_INPUT_KEY             InputKey;
  UINTN                     InputLength;
  CHAR16                    Mask[1];
  CHAR16                    Unicode[1];

  *GetResponse = FALSE;
  ZeroMem (Unicode, sizeof (Unicode));
  ZeroMem (Mask, sizeof (Mask));
  ClearPopupScreen();
  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "Enter action pop up function\n"));

  InputLength = 0;
  Mask[InputLength] = L'_';
  while (TRUE) {
    if (PopUpString2 == NULL) {
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &InputKey,
        PopUpString,
        L"---------------------",
        Mask,
        NULL
      );
    } else {
      if (PopUpString3 == NULL) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &InputKey,
          PopUpString,
          PopUpString2,
          L"---------------------",
          Mask,
          NULL
        );
      } else {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &InputKey,
          PopUpString,
          PopUpString2,
          PopUpString3,
          L"---------------------",
          Mask,
          NULL
        );
      }
    }

    //
    // Check key.
    //
    if (InputKey.ScanCode == SCAN_NULL) {
      if (InputKey.UnicodeChar == CHAR_CARRIAGE_RETURN) {
        DEBUG ((DEBUG_INFO, "Enter is pressed\n"));
        if (Unicode[0] == L'Y' || Unicode[0] == L'y') {
          *GetResponse = TRUE;
          DEBUG ((DEBUG_INFO, "Replied YES\n"));
        }
        else {
          *GetResponse = FALSE;
          DEBUG ((DEBUG_INFO, "Replied NO or others\n"));
        }

        //
        // Add the null terminator.
        //
        Unicode[InputLength] = 0;
        Mask[InputLength] = 0;
        break;
      } else if ((InputKey.UnicodeChar == CHAR_NULL) ||
                 (InputKey.UnicodeChar == CHAR_TAB) ||
                 (InputKey.UnicodeChar == CHAR_LINEFEED)
                ) {
        continue;
      } else {
        //
        // delete last key entered
        //
        if (InputKey.UnicodeChar == CHAR_BACKSPACE) {
          Mask[InputLength] = L'_';
        } else {
          //
          // update key entry
          //
          Unicode[InputLength] = InputKey.UnicodeChar;
          Mask[InputLength] = InputKey.UnicodeChar;
        }
      }
    }
    DEBUG((DEBUG_INFO, "The response user input is '%s'\n", Unicode));
  }
}

/**
  Connect USB host controller

  @return EFI_SUCCESS   Connect USB host controller successful.
  @retval EFI_NOT_FOUND No USB host controller.

**/
EFI_STATUS
ConnectUsbDevice (
  VOID
  )
{
  BOOLEAN                         AtLeastOneConnected;
  EFI_STATUS                      Status;
  EFI_HANDLE                      *Handles;
  UINTN                           HandleCount;
  UINTN                           Index;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  UINT8                           ClassCode[3];

  Handles = NULL;
  HandleCount = 0;

  DEBUG ((DEBUG_INFO, "%a - enter\n", __FUNCTION__));
  //
  // Find the usb host controller firstly, then connect with the remaining device path
  //
  AtLeastOneConnected = FALSE;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );

    if (!EFI_ERROR (Status)) {
      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint8,
                            PCI_CLASSCODE_OFFSET,
                            sizeof (ClassCode),
                            ClassCode
                            );
      ASSERT_EFI_ERROR (Status);

      if ((ClassCode[1] == PCI_CLASS_SERIAL_USB) &&
          (ClassCode[2] == PCI_CLASS_SERIAL)) {

        Status = gBS->ConnectController (Handles[Index], NULL, NULL, TRUE);
        if (!EFI_ERROR(Status)) {
          AtLeastOneConnected = TRUE;
          DEBUG ((DEBUG_INFO, "USB controller is connected\n"));
        }
      }
    }
  }
  if (Handles != NULL) {
    FreePool (Handles);
  }
  return AtLeastOneConnected ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
  Write a file to an assigned file system.

  @param[in] FileSystemHandle    Handle of the file system that file would be write into.
  @param[in] FileName            Pointer to file name.
  @param[in] FileBuffer          The buffer to be written into file system.
  @param[in] FileSize            The size of FileBuffer.

  @retval EFI_SUCCESS            Wrote the file successfully.
  @retval Others                 Failed to write the file.

**/
EFI_STATUS
WriteFile (
  IN   EFI_HANDLE       FileSystemHandle,
  IN   CHAR16           *FileName,
  IN   UINT8            *FileBuffer,
  IN   UINTN            FileSize
  )
{
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *FileSystem;
  EFI_FILE                              *Root;
  EFI_FILE                              *FileHandle;
  UINTN                                 WriteSize;

  DEBUG ((DEBUG_INFO, "WriteBackupFile (%s) - entry\n", FileName));

  Status = gBS->HandleProtocol (FileSystemHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&FileSystem);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Cannot locate SFS protocol. Status = %r\n", Status));
    return Status;
  }

  //
  // Open the root directory, get EFI_FILE_PROTOCOL
  //
  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Cannot open volume. Status = %r\n", Status));
    return Status;
  }

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Cannot open file: %s. Status = %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_INFO, "Failed to open root dir on partition for writing. Status = %r\n", Status));
    return Status;
  }

  do {
    WriteSize = (FileSize > SIZE_4KB) ? SIZE_4KB : FileSize;
    Status = FileHandle->Write (FileHandle, &WriteSize, FileBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }
    FileSize = FileSize - WriteSize;
    FileBuffer = FileBuffer + WriteSize;
    DEBUG ((DEBUG_INFO, "FileSize is %d, WriteSize is %d and FileBuffer is %p\n", FileSize, WriteSize, FileBuffer));
  } while (FileSize > 0);

  DEBUG ((DEBUG_INFO, "Write %s %r\n", FileName, Status));
  FileHandle->Close (FileHandle);
  Root->Close (Root);

  return Status;
}

/**
  Detect available USB device.

  @return EFI_SUCCESS Detect available USB device status.
  @return Others      Fail.

**/
EFI_STATUS
DetectUsb (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 HandleCount;
  UINTN                                 HandleIndex;
  EFI_HANDLE                            *Handles;
  EFI_HANDLE                            Handle;

  EFI_BLOCK_IO_PROTOCOL                 *BlkIo;
  EFI_DEVICE_PATH_PROTOCOL              *DevicePath;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *Fs;
  UINTN                                 Index;
  CHAR16                                FileNameFormat[] = L"EraseReport%d.json";
  CHAR16                                FileName[(sizeof (FileNameFormat) / sizeof (CHAR16))];

  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  Status             = EFI_SUCCESS;
  HandleCount        = 0;
  Handles            = NULL;
  BlkIo              = NULL;

  //
  // Locate all handles of simple file system
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Locate simple file system failed.\n", __FUNCTION__));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[%a] Found %d options and start to check is it a valid usb storage. \n", __FUNCTION__, HandleCount));

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {

    Status = gBS->HandleProtocol (
                    Handles[HandleIndex],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&DevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Locate the parent handle
    //
    Status = gBS->LocateDevicePath (
                    &gEfiUsbIoProtocolGuid,
                    &DevicePath,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&BlkIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Get the SFS protocol from the handle
    //
    Status = gBS->HandleProtocol (
                    Handles[HandleIndex],
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID **)&Fs
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "[%a] removable: -%d.\n", __FUNCTION__, BlkIo->Media->RemovableMedia));

    //
    // Found targe device, start to write data to USB
    //
    for (Index = 0; Index < mStorageEraseDriver.NumOfDocumentation; Index ++) {
      UnicodeSPrint (FileName, sizeof (FileName), FileNameFormat, Index + 1);
      Status = WriteFile (
                 Handles[HandleIndex],
                 FileName,
                 (UINT8 *)(UINTN)mStorageEraseDriver.EraseDocumentation[Index].Data,
                 mStorageEraseDriver.EraseDocumentation[Index].DataSize
                 );
    }
    break;
  }

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
  FreePool (Handles);
  return Status;
}

/**
  Add the header of the report

  @param[in,out] Data  Target data buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AddReportHeader (
  IN OUT CHAR16  *Data
  )
{
  CHAR16  TempData[500];
  UINTN   StrLength;
  UINT16  BomValue = 0xFEFF;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;
  CopyMem (Data, &BomValue, sizeof (UINT16));

  SetMem (TempData, sizeof (TempData), 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                sizeof (TempData),
                "%a%a%a%a%a%a%a%a",
                "{\r\n\t\"Information\": {",
                "\r\n\t\t\"Purpose\": \"Media Sanitization metadata generation in support of NIST 800-88r1 documentation requirements as listed in Section 4.8\",",
                "\r\n\t\t\"Reference Documentation\": {",
                "\r\n\t\t\t\"NIST Specification\": \"NIST 800-88r1\",",
                "\r\n\t\t\t\"NIST Document Name\": \"Guidelines for Media Sanitization\",",
                "\r\n\t\t\t\"NIST Document URL\": \"https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-88r1.pdf\"\r\n\t\t},",
                "\r\n\t\t\"Disclaimer\": \"\",",
                "\r\n\t\t\"Support\": \"https://www.acme.com/content/www/us/en/support.html\"\r\n\t},"
                );

  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Add erase report of device

  @param[in,out] Data  Target data buffer
  @param[in] DocInfo   Device report data

  @return EFI_SUCCESS  Add device information to report successfully.
  @return Others       Fail.

**/
EFI_STATUS
AppendDeviceReport (
  IN OUT CHAR16  *Data,
  IN DOC_INFO    *DocInfo
  )
{
  CHAR16    TempData[800];
  UINTN     StrLength;
  CHAR8     TimeStr[100];
  EFI_TIME  Time;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if (Data == NULL || DocInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;

  // Content
  SetMem (TempData, sizeof (TempData), 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                sizeof (TempData),
                "%a%a%a%a%a%a%a%a%a%a%a%a%a%a%a%a%a",
                "\r\n\t\"Content\": {",
                "\r\n\t\t\"Manufacturer\": \"\",",
                "\r\n\t\t\"Model\": \"",
                DocInfo->ModelName,
                "\",\r\n\t\t\"Serial Number\": \"",
                DocInfo->SerialNumber,
                "\",\r\n\t\t\"Media Type\": \"",
                DocInfo->MediaType,
                "\",\r\n\t\t\"Media Source\": \"\",",
                "\r\n\t\t\"Pre-Sanitization Confidentiality Categorization\": \"\",",
                "\r\n\t\t\"Sanitization Description\": \"",
                DocInfo->EraseCat,
                "\",\r\n\t\t\"Method used\": \"",
                DocInfo->EraseType,
                "\",\r\n\t\t\"Method Used Detail\": \"\",",
                "\r\n\t\t\"Tool Used\": \"\",",
                "\r\n\t\t\"Verification Method\": \"Quick Sample 10% Pseudorandom\","
                );
  DEBUG ((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  // Sanitization Person
  SetMem (TempData, sizeof (TempData), 0x0);
  gRT->GetTime (&Time, NULL);
  ZeroMem (TimeStr, sizeof (TimeStr));
  AsciiSPrint (TimeStr, sizeof (TimeStr), "\r\n\t\t\t\"Date\": \"%d-%d-%dT%d:%d:%d\",", Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                sizeof (TempData),
                "%a%a%a%a%a%a",
                "\r\n\t\t\"Sanitization Person\": {",
                "\r\n\t\t\t\"Name\": \"\",",
                "\r\n\t\t\t\"Position/Title\": \"\",",
                TimeStr,
                "\r\n\t\t\t\"Location\": \"\",",
                "\r\n\t\t\t\"Contact information\": \"\"\r\n\t\t},"
                );
  DEBUG ((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  // Verification Person
  SetMem (TempData, sizeof (TempData), 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                sizeof (TempData),
                "%a%a%a%a%a%a",
                "\r\n\t\t\"Verification Person\": {",
                "\r\n\t\t\t\"Name\": \"\",",
                "\r\n\t\t\t\"Position/Title\": \"\",",
                TimeStr,
                "\r\n\t\t\t\"Location\": \"\",",
                "\r\n\t\t\t\"Contact information\": \"\"\r\n\t\t}\r\n\t}"
                );
  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/**
  Add the footer of the report

  @param[in,out] Data  Target data buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AppendReportFooter (
  IN OUT CHAR16  *Data
  )
{
  CHAR16  TempData[100];
  UINTN   StrLength;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;

  SetMem (TempData, sizeof (TempData), 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                sizeof (TempData),
                "%a",
                "\r\n}"
                );

  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Create report data with JSON format

  @return EFI_SUCCESS Create file and write data status.
  @return Others      Fail.

**/
EFI_STATUS
CreateReport (
  VOID
  )
{
  EFI_STATUS    Status;
  CHAR16        *ReportData;
  UINTN         StrLengthAll;
  UINTN         Index;
  LIST_ENTRY    *Node;
  DOC_INFO      *DocInfo;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  ReportData   = NULL;
  StrLengthAll = 0;
  Index        = 0;

  for (Node = GetFirstNode (&mStorageEraseDriver.DocInfoList);
        !IsNull (&mStorageEraseDriver.DocInfoList, Node);
        Node = GetNextNode (&mStorageEraseDriver.DocInfoList, Node)) {

    //
    // Create a new documentation
    //
    if ((Index % MAX_DEVICE_IN_FILE) == 0) {
      DEBUG ((DEBUG_INFO, "Create new report: \n"));
      ReportData  = AllocateZeroPool (REPORT_DATA_MAX_LENGTH * sizeof (CHAR16));
      if (ReportData == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }
      //
      // Add header
      //
      AddReportHeader (ReportData);
      DEBUG ((DEBUG_INFO, "all string length = %d.\n\n", StrLen (ReportData)));

      DEBUG ((DEBUG_INFO, "Write to the table: \n"));
      mStorageEraseDriver.EraseDocumentation[mStorageEraseDriver.NumOfDocumentation].Data = ReportData;
      mStorageEraseDriver.NumOfDocumentation ++;
    }

    //
    // Add Device info to the report
    //
    DocInfo = DOC_INFO_FROM_LINK (Node);
    if (DocInfo->ValResult == EFI_SUCCESS) {
      AppendDeviceReport (ReportData, DocInfo);
      DEBUG ((DEBUG_INFO, "all string length = %d.\n\n", StrLen (ReportData)));
      Index ++;
    }
  }

  for (Index = 0; Index < mStorageEraseDriver.NumOfDocumentation; Index ++) {
    AppendReportFooter (mStorageEraseDriver.EraseDocumentation[Index].Data);
    StrLengthAll = StrLen (mStorageEraseDriver.EraseDocumentation[Index].Data);
    DEBUG ((DEBUG_INFO, "total string length of the documentation[%d] = %d.\n", Index, StrLengthAll));
    mStorageEraseDriver.EraseDocumentation[Index].DataSize = (UINT32) (StrLengthAll * sizeof (CHAR16));
  }

  if (mStorageEraseDriver.NumOfDocumentation == 0) {
    Status = EFI_NOT_FOUND;
  } else {
    Status = EFI_SUCCESS;
    for (Index = 0; Index < mStorageEraseDriver.NumOfDocumentation; Index ++) {
      DEBUG ((DEBUG_INFO, "Report %d : size = %d bytes, data address = 0x%08x\n",
              Index, mStorageEraseDriver.EraseDocumentation[Index].DataSize, (UINTN)mStorageEraseDriver.EraseDocumentation[Index].Data));
    }
  }

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));

  return Status;
}

/**
  Generate erase report in JSON format.

  @return EFI_SUCCESS  Generate erase report successful.
  @return Others       Fail.

**/
EFI_STATUS
GetStorageEraseReport (
  VOID
  )
{
  EFI_STATUS  Status;

  AddDocInfoToList ();
  Status = CreateReport ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to create report for erase devices(%r).\n", Status));
  }
  return Status;
}

/**
  Write JSON file to external USB device.

  @return EFI_SUCCESS  Write report to USB successful.
  @return Others       Fail.

**/
VOID
WriteToUsb (
  VOID
  )
{
  EFI_STATUS   Status;
  BOOLEAN      GetResponse;
  UINTN        AttemptCount;

  AttemptCount = 0;
  GetResponse = FALSE;

  ActionToGetSanitizationReport (
    L"   Do you want the sanitization report   ",
    L"  to be stored in external USB storage?  ",
    L"Please type 'Y' to continue or 'N' to abort.",
    &GetResponse);
  do {
    ClearPopupScreen();
    if (GetResponse == FALSE) {
      break;
    } else {
      Status = ConnectUsbDevice ();
      if (EFI_ERROR (Status)) {
        PopUpMsg (L"No USB device has been detected", L"------------------------", NULL);
      } else{
        PopUpMsg (L"Writing file to USB device", L"------------------------", NULL);
        Status = DetectUsb ();
        DEBUG ((DEBUG_INFO, "Detect USB and write file: %r\n", Status));
        if (!EFI_ERROR (Status)) {
          PopUpMsg (L"Write file to USB device success", L"------------------------", NULL);
          break;
        } else {
          PopUpMsg (L"Write file to USB device failed", L"------------------------", NULL);
        }
      }
      if ((AttemptCount ++) >= REPORT_USB_MAX_ATTEPMT) {
        break;
      }
      ActionToGetSanitizationReport (
        L"   Failed to write data to USB storage device  ",
        L"   type 'Y' to try again or 'N' to abort,   ",
        L"   Please make sure the USB device is conncted. ",
        &GetResponse);
    }
  } while (GetResponse == TRUE);

}
