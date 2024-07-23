/**@file
  Intel Platform Service Record Implementation.

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
#include <PiDxe.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/DxeMeLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/PsrProtocol.h>
#include <PsrMsgs.h>
#include <PlatformServiceRecord.h>
#include <MeBiosPayloadHob.h>

GLOBAL_REMOVE_IF_UNREFERENCED ME_BIOS_PAYLOAD_HOB   *mMbpHob = NULL;

#define PSR_MAX_FILENAME_SIZE  50

UINTN  mPsrDataBlobSize;
UINT8  *mPsrDataBlob = NULL;

GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_SERVICE_RECORD_PROTOCOL mPlatformServiceRecordProtocol = {
  PLATFORM_SERVICE_RECORD_PROTOCOL_REVISION,
  GetPsrData,
  ExportPsrToUsbStorage,
  NotifyChassisIntrusionDetection,
  NotifyCriticalTempShutdown,
  NotifyEventReport
};

/**
  Check if CSME supports PSR feature.

  @retval TRUE     PSR feature is supported by CSME
  @retval FALSE    PSR is NOT supported
**/
BOOLEAN
IsPsrSupported (
  VOID
  )
{
  if ((mMbpHob != NULL) && (mMbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.PSR == 1)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Inform CSME of updated PSR Capability list.

  @retval         EFI_SUCCESS            Updated PSR Capability list to CSME.
  @retval         Others                 Failed to update capabilities.
**/
EFI_STATUS
NotifyPsrCapabilitySet (
  VOID
  )
{
  EFI_STATUS             Status;
  UINT32                 PsrCapabilities;

  Status = EFI_SUCCESS;
  PsrCapabilities = PcdGet32 (VpdPcdPsrSupportedCapabilities) & PSR_CAPABILITY_MASK;
  DEBUG ((DEBUG_INFO, "%a PsrCapabilities=%x\n", __FUNCTION__, PsrCapabilities));

  if (mMbpHob->MeBiosPayload.Psr.PsrCapabilities != PsrCapabilities) {
    Status = HeciSendPsrCapabilitySet (PsrCapabilities);
  }

  return Status;
}

/**
  Get PSR blob from CSME via HECI cmds.

  @param[in]      *This                  Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in]      PsrDataBlobSize        PSR Data Buffer Size.
  @param[in, out] *PsrDataBlob           Pointer of the PSR Data Blob.

  @retval         EFI_SUCCESS            Retrieved PSR from CSME successfully.
  @retval         Others                 Failed to retrieve the record.
**/
EFI_STATUS
EFIAPI
GetPsrData (
  IN     PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN     UINTN                            PsrDataBlobSize,
  IN OUT UINT8                            *PsrDataBlob
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  if ((PsrDataBlob == NULL) || (PsrDataBlobSize != sizeof (PSR_DATA))) {
    DEBUG ((DEBUG_ERROR, "PSR data buffer is null or invalid buffer size\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Allocate buffer for PSR data and get from CSME FW, for the first time in the
  // boot. if the function is called again then return PSR Data from existing buffer.
  //
  if (mPsrDataBlob == NULL) {
    //
    // Check CSME FW Init Done (CSME FID) before sending HECI cmd
    //
    Status = WaitFirmwareInitComplete ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FW Init Complete - Timeout\n"));
      return EFI_ACCESS_DENIED;
    }

    mPsrDataBlob = AllocateZeroPool (sizeof (PSR_DATA));
    if (mPsrDataBlob == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Get PSR Data from CSME FW.
    //
    Status = HeciGetPsrRecord ((PSR_DATA*) mPsrDataBlob);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Extract Psr data from CSME failed\n"));
      return EFI_ACCESS_DENIED;
    }

    mPsrDataBlobSize = sizeof (PSR_DATA);
    DEBUG ((DEBUG_INFO, "[%a] Received PSR Data from CSME\n", __FUNCTION__));
  }

  //
  // Save PSR data to PsrDataBlob for display to user
  //
  CopyMem (PsrDataBlob, mPsrDataBlob, mPsrDataBlobSize);

  DEBUG ((DEBUG_INFO, "[%a] Copied PSR data blob to the caller buffer = 0x%08x\n", __FUNCTION__, PsrDataBlob));

  return Status;
}

/**
  Export PSR to selected Usb Storage.

  @param[in] *This              Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in] ExportHandle       BlockIO handle of the selected device.

  @retval    EFI_SUCCESS        Export PSR data to specific USB device successfully.
  @retval    Others             Fail to export PSR data to specific USB device.

**/
EFI_STATUS
EFIAPI
ExportPsrToUsbStorage (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN EFI_HANDLE                       *ExportHandle
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  UINTN                            HandleCount;
  UINTN                            HandleIndex;
  EFI_HANDLE                       *HandlesBuffer;
  EFI_HANDLE                       FsHandle;
  EFI_HANDLE                       CheckHandle;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *FileSys;
  EFI_DEVICE_PATH_PROTOCOL         *FsDevicePath;
  EFI_FILE                         *FileSysHandle;
  EFI_FILE                         *FileHandle;
  CHAR16                           *Str;
  CHAR16                           *FileName;
  UINTN                            PsrDataSize;
  EFI_TIME                         TimeStamp;
  EFI_BLOCK_IO_PROTOCOL            *BlkIo;

  DEBUG((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  FileSys       = NULL;
  FsDevicePath  = NULL;
  FileSysHandle = NULL;
  FileHandle    = NULL;
  FsHandle      = NULL;
  FileName      = NULL;
  BlkIo         = NULL;

  if ((ExportHandle == NULL) || (mPsrDataBlob == NULL)) {
    DEBUG ((DEBUG_ERROR, "Invalid parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  PsrDataSize = sizeof (PSR_DATA);
  //
  // Locate all handles of SimpleFileSystem protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandlesBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] SimpleFileSystem locate failed.\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    //
    // Check whether this handle is the one that user selected.
    //

    //
    // Get the device path for fileSystem.
    //
    FsDevicePath = DevicePathFromHandle (HandlesBuffer[HandleIndex]);
    if (FsDevicePath == NULL) {
      continue;
    }

    Str = ConvertDevicePathToText (
            FsDevicePath,
            TRUE,
            TRUE
            );
    DEBUG ((DEBUG_INFO, "[%a] FsDevicePath = %s\n", __FUNCTION__, Str));


    //
    // Locate the parent handle
    //
    Status = gBS->LocateDevicePath (
                    &gEfiUsbIoProtocolGuid,
                    &FsDevicePath,
                    &CheckHandle
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = gBS->HandleProtocol (
                    CheckHandle,
                    &gEfiBlockIoProtocolGuid,
                    (VOID **)&BlkIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if ((EFI_HANDLE *) CheckHandle == ExportHandle) {
      DEBUG ((DEBUG_INFO, "[%a] Find the correspond file system: %s \n", __FUNCTION__, Str));
      FsHandle = HandlesBuffer[HandleIndex];
      break;
    }
  }

  //
  // Start export process.
  //
  Status = gBS->HandleProtocol (
                  FsHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID **) &FileSys
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] FsHandle = %x, can't find file system.\n", __FUNCTION__, FsHandle));
    FreePool (HandlesBuffer);
    return EFI_UNSUPPORTED;
  }

  Status = FileSys->OpenVolume (
                      FileSys,
                      &FileSysHandle
                      );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Open Volume Failed\n", __FUNCTION__));
    FreePool (HandlesBuffer);
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_ERROR, "[%a] Allocate Memory for Filename\n", __FUNCTION__));
  FileName = AllocateZeroPool (sizeof (CHAR16) * PSR_MAX_FILENAME_SIZE);
  if (FileName == NULL) {
    FreePool (HandlesBuffer);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_ERROR, "[%a] Allocate Memory for Timestamp\n", __FUNCTION__));
  ZeroMem (&TimeStamp, sizeof (EFI_TIME));
  Status = gRT->GetTime (&TimeStamp, NULL);
  if (!EFI_ERROR (Status)) {
    UnicodeSPrint (FileName, sizeof (CHAR16) * PSR_MAX_FILENAME_SIZE, L"PlatformServiceRecord_%4d-%02d-%02d %02d-%02d-%02d.bin", TimeStamp.Year,TimeStamp.Month, TimeStamp.Day, TimeStamp.Hour, TimeStamp.Minute, TimeStamp.Second);
  } else {
    UnicodeSPrint (FileName, sizeof (CHAR16) * PSR_MAX_FILENAME_SIZE, L"PlatformServiceRecord.bin");
  }

  DEBUG ((DEBUG_ERROR, "[%a] FileName = %s\n", __FUNCTION__,  FileName));

  //
  // Create Files.
  //
  Status = FileSysHandle->Open (
                            FileSysHandle,
                            &FileHandle,
                            FileName,
                            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                            EFI_FILE_ARCHIVE
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Create file Failed.\n", __FUNCTION__));
    FreePool (HandlesBuffer);
    FreePool (FileName);
    return Status;
  }

  //
  // Export Files.
  //
  Status = FileSysHandle->Write (
                            FileHandle,
                            &PsrDataSize,
                            mPsrDataBlob
                            );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Write file Failed.\n", __FUNCTION__));
  }

  DEBUG((DEBUG_INFO, "[%a] Write PSR Data done.\n", __FUNCTION__));
  Status = FileSysHandle->Close (FileHandle);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Close file Failed.\n", __FUNCTION__));
  }

  FreePool (FileName);
  FreePool (HandlesBuffer);

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

  return Status;
}

/**
  Inform CSME of Chassis Intrusion via HECI cmds

  @param[in] *This           Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS        Chassis Intrusion event has been successfully send to CSME
  @retval EFI_NOT_READY      CSME not ready to receive the event
**/
EFI_STATUS
EFIAPI
NotifyChassisIntrusionDetection (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This
  )
{
  // CSME will include PsrData bit in the MBP.
  // BIOS will read the bit and send/not send the chassis intrusion status according to its value.
  if ((mMbpHob->MeBiosPayload.Psr.PsrData.ChassisIntrusionEvent == 1)) {
    return HeciSendChassisIntrDetEvent ();
  }
  return EFI_NOT_READY;
}

/**
  Inform CSME of Critical Temperature Shutdown event via HECI cmds if Psr logging is enabled

  @param[in] *This           Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS        Chassis Intrusion event has been successfully send to CSME
  @retval EFI_NOT_READY      CSME not ready to receive the event
**/
EFI_STATUS
EFIAPI
NotifyCriticalTempShutdown (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This
  )
{
  if (mMbpHob->MeBiosPayload.Psr.PsrData.PsrLogState != 0) {
    // Inform CSME last shutdown is due to Critical Temp
    return HeciSendCriticalTempShutdownEvent ();
  }
  return EFI_NOT_READY;
}

/**
  Inform CSME one of below events via HECI interface only when PSR log state is started.
  ----------------------------------------------------------
  | Event                      |   Event ID   | Event Data |
  ----------------------------------------------------------
  | Firmware Recovery          |     0x36     |      0     |
  | Firmware Update            |     0x37     |      0     |
  | System Hang                |     0x38     |      0     |
  | Power Drop                 |     0x39     |      0     |
  ----------------------------------------------------------

  @param[in] This            Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in] EventId         Specific event ID to notify CSME the event.
  @param[in] EventData       Data payload.

  @retval EFI_SUCCESS        Event ID has been successfully send to CSME
  @retval EFI_NOT_READY      CSME not ready to receive the event
  @retval EFI_UNSUPPORTED    Failed to nofity CSME the Event ID.
**/
EFI_STATUS
EFIAPI
NotifyEventReport (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN UINT32                           EventId,
  IN UINT32                           EventData
  )
{
  UINT32  PsrCapabilities;
  PsrCapabilities = PcdGet32 (VpdPcdPsrSupportedCapabilities);

  if (mMbpHob->MeBiosPayload.Psr.PsrData.PsrLogState != 0) {
    if ((((PsrCapabilities & FW_RECOVERY) == FW_RECOVERY) && (EventId == FW_RECOVERY_EVENT_ID)) ||
        (((PsrCapabilities & FW_UPDATE) == FW_UPDATE) && (EventId == FW_UPDATE_EVENT_ID)) ||
        (((PsrCapabilities & SYS_HANG) == SYS_HANG) && (EventId == SYSTEM_HANG_EVENT_ID)) ||
        (((PsrCapabilities & PWR_DROP) == PWR_DROP) && (EventId == POWER_DROP_EVENT_ID))) {
      return HeciEventReport (EventId, EventData);
    } else {
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_NOT_READY;
}

/**
  Entry point of the Intel Platform Service Record DXE Driver

  Installs the PSR Setup Protocol for other functions to locate and use

  @param[in] ImageHandle     The firmware allocated handle for the EFI image.
  @param[in] SystemTable     A pointer to the EFI System Table.

  @retval EFI_UNSUPPORTED    Feature is not supported by the FW
  @retval EFI_SUCCESS        The protocol has installed successfully
  @retval Others             The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
PlatformServiceRecordEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  mMbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if ((mMbpHob == NULL) || (mMbpHob->MeBiosPayload.Psr.Available == FALSE)) {
    DEBUG ((DEBUG_ERROR, "No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  if (!IsPsrSupported ()) {
    return EFI_UNSUPPORTED;
  }

  // Notify PSR Capability list to CSME
  Status = NotifyPsrCapabilitySet ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NotifyPsrCapabilitySet status : %r \n", Status));
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPlatformServiceRecordProtocolGuid,
                  &mPlatformServiceRecordProtocol,
                  NULL
                  );

  ASSERT_EFI_ERROR(Status);
  DEBUG ((DEBUG_INFO, "PSR Protocol Install Status: %r\n", Status));

  DEBUG ((DEBUG_INFO, "%a exit\n", __FUNCTION__));

  return Status;
}
