/** @file
  This file contains code required for handling PSR setup settings.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>

#include <PsrSetup.h>
#include <PsrSettings.h>

#include <Protocol/PsrProtocol.h>

extern EFI_HII_HANDLE  mPsrHiiHandle;

/**
  Get Usb Device Description.

  @param[in]  UsbIo          The Pointer to EFI_USB_IO_PROTOCOL.

  @retval     Description    Return USB device description string.
**/
CHAR16 *
GetUsbDeviceDescription (
  IN EFI_USB_IO_PROTOCOL  *UsbIo
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_USB_DEVICE_DESCRIPTOR  DevDesc;
  CHAR16                     *Manufacturer;
  CHAR16                     *Product;
  CHAR16                     NullChar;
  UINTN                      DescMaxSize;
  CHAR16                     *Description;
  CONST UINT16               PsrUsbLangId     = 0x0409; // English
  CHAR16                     PsrUefiPrefix[]  = L"UEFI ";
  CHAR16                     PsrSpacePrefix[] = L" ";

  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  NullChar = L'\0';

  Status = UsbIo->UsbGetDeviceDescriptor (UsbIo, &DevDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] UsbGetDeviceDescriptor %r\n", __FUNCTION__, Status));
    return PSR_DEFAULT_USB_DEVICE_NAME;
  }

  Status = UsbIo->UsbGetStringDescriptor (
                    UsbIo,
                    PsrUsbLangId,
                    DevDesc.StrManufacturer,
                    &Manufacturer
                    );
  if (EFI_ERROR (Status)) {
    Manufacturer = &NullChar;
  }

  Status = UsbIo->UsbGetStringDescriptor (
                    UsbIo,
                    PsrUsbLangId,
                    DevDesc.StrProduct,
                    &Product
                    );
  if (EFI_ERROR (Status)) {
    Product = &NullChar;
  }

  if ((Manufacturer == &NullChar) && (Product == &NullChar)) {
    DEBUG ((DEBUG_ERROR, "[%a] Get USB descriptor failed.\n", __FUNCTION__));
    return PSR_DEFAULT_USB_DEVICE_NAME;
  }

  DescMaxSize = sizeof (PsrUefiPrefix) + StrSize (Manufacturer) + StrSize (Product) +  (2 * sizeof (PsrSpacePrefix));
  Description = AllocateZeroPool (DescMaxSize);
  ASSERT (Description != NULL);
  StrCatS (Description, DescMaxSize/sizeof(CHAR16), PsrUefiPrefix);

  StrCatS (Description, DescMaxSize/sizeof(CHAR16), Manufacturer);
  StrCatS (Description, DescMaxSize/sizeof(CHAR16), L" ");

  StrCatS (Description, DescMaxSize/sizeof(CHAR16), Product);
  StrCatS (Description, DescMaxSize/sizeof(CHAR16), L" ");

  DEBUG ((EFI_D_INFO, "[%a] Description = %s\n", __FUNCTION__ , Description));

  if (Manufacturer != &NullChar) {
    FreePool (Manufacturer);
  }
  if (Product != &NullChar) {
    FreePool (Product);
  }

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

  return Description;
}

/**
  List all Usb Storages available for export of data.

  @retval PsrUsbExportOption    Return the pointer that holding PSR_USB_EXPORT_OPTION.
**/
PSR_EXPORT_INFO_LINK *
DetectAndListAvailableFlashDrives (
  VOID
  )
{
  EFI_STATUS             Status;
  UINTN                  HandleCount;
  EFI_HANDLE             *Handles;
  UINTN                  HandleIndex;
  EFI_BLOCK_IO_PROTOCOL  *BlkIo;
  EFI_USB_IO_PROTOCOL    *UsbIo;
  PSR_EXPORT_INFO_LINK   *PsrExportInfoLink;
  PSR_EXPORT_INFO_LINK   *CurExportLink;

  DEBUG ((DEBUG_INFO, "%a Enter\n", __FUNCTION__));

  Status             = EFI_SUCCESS;
  HandleCount        = 0;
  Handles            = NULL;
  BlkIo              = NULL;
  UsbIo              = NULL;
  CurExportLink      = NULL;
  PsrExportInfoLink  = NULL;

  PsrExportInfoLink = AllocateZeroPool (sizeof (PSR_EXPORT_INFO_LINK));
  if (PsrExportInfoLink == NULL) {
    return PsrExportInfoLink;
  }

  PsrExportInfoLink->Signature = PSR_EXPORT_INFO_SIGNATURE;
  InitializeListHead (&PsrExportInfoLink->Link);

  //
  // Locate all handles of BlockIo protocol
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] BlockIo locate failed. Can't found target device. \n", __FUNCTION__));
    FreePool (PsrExportInfoLink);
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "[%a] Found %d options and start to check is it a valid usb storage. \n", __FUNCTION__, HandleCount));

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    Handles[HandleIndex],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **) &BlkIo
                    );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Skip option [%d]. \n", __FUNCTION__, HandleIndex));
      continue;
    }

    //
    // Found all usb removable device.
    //
    Status = gBS->HandleProtocol (
                    Handles[HandleIndex],
                    &gEfiUsbIoProtocolGuid,
                    (VOID **) &UsbIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Skip option [%d]. It's not a USB Mass Storage.\n", __FUNCTION__, HandleIndex));
      continue;
    }

    DEBUG ((DEBUG_INFO, "[%a] Option [%d] is a valid USB storage device, save PSR_EXPORT_INFO_LINK Data. \n", __FUNCTION__, HandleIndex));
    //
    // Save PSR_EXPORT_INFO_LINK Data.
    //
    CurExportLink = AllocateZeroPool (sizeof (PSR_EXPORT_INFO_LINK));
    if (CurExportLink == NULL) {
      //
      // Out of resources to save data, return the head of the linked list.
      //
      FreePool (Handles);
      return PsrExportInfoLink;
    }

    CurExportLink->Signature            = PSR_EXPORT_INFO_SIGNATURE;
    CurExportLink->TargetblkHandle      = Handles[HandleIndex];
    CurExportLink->TargetDevicePath     = DevicePathFromHandle (Handles[HandleIndex]);
    CurExportLink->TargetUsbDescription = GetUsbDeviceDescription (UsbIo);
    InsertTailList (&PsrExportInfoLink->Link, &CurExportLink->Link);
  }

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

  FreePool (Handles);
  return PsrExportInfoLink;
}

/**
  Initial PSR configuration.

  @param[out] PsrConfiguration    Pointer of PSR Configuration.

  @retval EFI_SUCCESS             Initial PSR Configuration successfully.
  @retval Others                  Failed to get PSR data.
**/
EFI_STATUS
PsrGetInfo (
  OUT PSR_CONFIGURATION    *PsrConfiguration
  )
{
  PLATFORM_SERVICE_RECORD_PROTOCOL  *PlatformServiceRecordProtocol;
  EFI_STATUS                        Status;
  UINT8                             *RetrievedData;
  UINTN                             TotalBlobSize;
  UINT8                             Index;
  UINT32                            TempEvent;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  RetrievedData = NULL;
  PlatformServiceRecordProtocol = NULL;

  Status = gBS->LocateProtocol (
                  &gPlatformServiceRecordProtocolGuid,
                  NULL,
                  (VOID **) &PlatformServiceRecordProtocol
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  RetrievedData = AllocateZeroPool (sizeof (PSR_DATA));
  if (RetrievedData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TotalBlobSize = sizeof (PSR_DATA);
  Status = PlatformServiceRecordProtocol->PsrGetData (
                                            PlatformServiceRecordProtocol,
                                            TotalBlobSize,
                                            RetrievedData
                                            );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "[%a] Unable to retrieve PSR data.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Parse the data received from CSME.
  //
  CopyMem (&PsrConfiguration->PsrLogState     , (RetrievedData + PSR_LOG_STATE_OFFSET)  , sizeof (UINT8));
  CopyMem (&PsrConfiguration->PsrVersion.Major, (RetrievedData + PSR_VERISON_OFFSET)    , sizeof (UINT16));
  CopyMem (&PsrConfiguration->PsrVersion.Minor, (RetrievedData + PSR_VERISON_OFFSET + 2), sizeof (UINT16));

  CopyMem (PsrConfiguration->Psrid      , (RetrievedData + PSRID_OFFSET)    , sizeof (UINT8) * PSR_UUID_LENGTH);
  CopyMem (&PsrConfiguration->Upid.Name1, (RetrievedData + UPID_OFFSET)     , sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name2, (RetrievedData + UPID_OFFSET +  8), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name3, (RetrievedData + UPID_OFFSET + 16), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name4, (RetrievedData + UPID_OFFSET + 24), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name5, (RetrievedData + UPID_OFFSET + 32), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name6, (RetrievedData + UPID_OFFSET + 40), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name7, (RetrievedData + UPID_OFFSET + 48), sizeof (UINT64));
  CopyMem (&PsrConfiguration->Upid.Name8, (RetrievedData + UPID_OFFSET + 56), sizeof (UINT64));

  CopyMem (&PsrConfiguration->Capabilities               , (RetrievedData + PSR_CAPABILITY_MASK_OFFSET)    , (sizeof (UINT8) * PSR_MAX_SUPPORTED_CAPABILITIES));

  CopyMem (&PsrConfiguration->GenesisDate                , (RetrievedData + GENESIS_DATE_OFFSET)           , MAX_DATE_LENGTH);
  CopyMem (PsrConfiguration->OemInfo                     , (RetrievedData + GENESIS_OEM_INFO_OFFSET)       , MAX_STRING_LENGTH);
  CopyMem (PsrConfiguration->OemMakeInfo                 , (RetrievedData + GENESIS_OEM_MAKE_OFFSET)       , MAX_STRING_LENGTH);
  CopyMem (PsrConfiguration->OemModelInfo                , (RetrievedData + GENESIS_OEM_MODEL_OFFSET)      , MAX_STRING_LENGTH);
  CopyMem (PsrConfiguration->ManufCOE                    , (RetrievedData + GENESIS_MANUFAC_COE_OFFSET)    , MAX_STRING_LENGTH);
  CopyMem (PsrConfiguration->OemDataStore                , (RetrievedData + GENESIS_OEM_DATA_STORE_OFFSET) , (MAX_STRING_LENGTH * 16));

  CopyMem (&PsrConfiguration->S0RuntimeCtr               , (RetrievedData + LEDGER_CTR_S0_OFFSET)          , sizeof (UINT32));
  CopyMem (&PsrConfiguration->S0toS5Ctr                  , (RetrievedData + LEDGER_CTR_S5_OFFSET)          , sizeof (UINT32));
  CopyMem (&PsrConfiguration->S0toS4Ctr                  , (RetrievedData + LEDGER_CTR_S4_OFFSET)          , sizeof (UINT32));
  CopyMem (&PsrConfiguration->S0toS3Ctr                  , (RetrievedData + LEDGER_CTR_S3_OFFSET)          , sizeof (UINT32));
  CopyMem (&PsrConfiguration->WarmResetCtr               , (RetrievedData + LEDGER_CTR_WARM_RESET)         , sizeof (UINT32));
  CopyMem (&PsrConfiguration->IshConnectionCtr           , (RetrievedData + LEDGER_CTR_ISH_CONNECTION)     , sizeof (UINT32));
  CopyMem (&PsrConfiguration->CsmeResetCtr               , (RetrievedData + LEDGER_CTR_CSME_RESET)         , sizeof (UINT32));
  CopyMem (&PsrConfiguration->PrtcFailureCtr             , (RetrievedData + LEDGER_CTR_PRTC_FAILURE)       , sizeof (UINT32));
  CopyMem (&PsrConfiguration->CsmeInvalidStateCtr        , (RetrievedData + LEDGER_CTR_CSME_INVALID_STATE) , sizeof (UINT32));
  CopyMem (&PsrConfiguration->CsmeDamCtr                 , (RetrievedData + LEDGER_CTR_CSME_DAM_STATE)     , sizeof (UINT32));
  CopyMem (&PsrConfiguration->CsmeUnlockedCtr            , (RetrievedData + LEDGER_CTR_CSME_UNLOCK_STATE)  , sizeof (UINT32));
  CopyMem (&PsrConfiguration->SvnIncreaseCtr             , (RetrievedData + LEDGER_CTR_SVN_INCREASE)       , sizeof (UINT32));
  CopyMem (&PsrConfiguration->ExcessiveShockCtr          , (RetrievedData + LEDGER_CTR_EXCESSIVE_SHOCK)    , sizeof (UINT32));
  CopyMem (&PsrConfiguration->ExcessiveOperationalTempCtr, (RetrievedData + LEDGER_CTR_EXCESSIVE_OPER_TEMP), sizeof (UINT32));
  CopyMem (&PsrConfiguration->FwRecoveryCtr              , (RetrievedData + LEDGER_CTR_FW_RECOVERY)        , sizeof (UINT32));
  CopyMem (&PsrConfiguration->FwUpdateCtr                , (RetrievedData + LEDGER_CTR_FW_UPDATE)          , sizeof (UINT32));
  CopyMem (&PsrConfiguration->SysHangCtr                 , (RetrievedData + LEDGER_CTR_SYS_HANG)           , sizeof (UINT32));
  CopyMem (&PsrConfiguration->PwrDropCtr                 , (RetrievedData + LEDGER_CTR_PWR_DROP)           , sizeof (UINT32));

  PsrConfiguration->TotalEventCount =  *(UINT32*)(RetrievedData + EVENT_DATA_TOTAL_COUNT_START_OFFSET);
  if (PsrConfiguration->TotalEventCount > PSR_MAX_EVENT_COUNT) {
    PsrConfiguration->TotalEventCount = PSR_MAX_EVENT_COUNT;
  }

  for (Index = 0; Index < PsrConfiguration->TotalEventCount; Index++) {
    //
    // ----------------------------
    //   Event Data Ledger
    // ----------------------------
    // Event Id:       1 Byte
    // Event Sub Id:   3 Bytes (Event specific)
    //   Bit   0: EventStatus
    //   Bit 2-6: EventAction
    //   Bit 7-8: EventSource
    // Event Time:     4 Bytes
    // Event Data:     4 Bytes (Reserved)
    // ----------------------------
    //
    PsrConfiguration->Events[Index].EventId = *(RetrievedData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index));
    if (PsrConfiguration->Events[Index].EventId == PsrErase) {
      TempEvent = *(UINT32*)(RetrievedData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 1);
      PsrConfiguration->Events[Index].EventStatus = (UINT8) (TempEvent & EVENT_STA_MASK);
      PsrConfiguration->Events[Index].EventAction = (UINT8) ((TempEvent & EVENT_ACT_MASK) >> EVENT_ACT_BIT);
      PsrConfiguration->Events[Index].EventSource = (UINT8) ((TempEvent & EVENT_SRC_MASK) >> EVENT_SRC_BIT);
      DEBUG ((DEBUG_INFO, "TempEvent[%02d] = 0x%x, Sta = %x, Act = %x, Src = %x\n",
        Index,
        TempEvent,
        PsrConfiguration->Events[Index].EventStatus,
        PsrConfiguration->Events[Index].EventAction,
        PsrConfiguration->Events[Index].EventSource
        ));
    }
    //
    // Event Time data should get from the address = EVENT_LOG_START_OFFSET  + Event Id data size (1 Byte) + Reserved 3 Bytes.
    // Event Data should get from the address = EVENT_LOG_START_OFFSET + Event ID/Sub ID data size (4 Byte) + Time Stamp (4 Bytes).
    //
    CopyMem (&PsrConfiguration->Events[Index].EventTime  , (RetrievedData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 4), MAX_DATE_LENGTH);
    CopyMem (&PsrConfiguration->Events[Index].EventData  , (RetrievedData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 8), PSR_MAX_EVENT_DATA_LENGTH);
  }

  CopyMem (&PsrConfiguration->FwVersion.Major , (RetrievedData + CSME_FW_VERSION_OFFSET)    , sizeof (UINT16));
  CopyMem (&PsrConfiguration->FwVersion.Minor , (RetrievedData + CSME_FW_VERSION_OFFSET + 2), sizeof (UINT16));
  CopyMem (&PsrConfiguration->FwVersion.Hotfix, (RetrievedData + CSME_FW_VERSION_OFFSET + 4), sizeof (UINT16));
  CopyMem (&PsrConfiguration->FwVersion.Build , (RetrievedData + CSME_FW_VERSION_OFFSET + 6), sizeof (UINT16));

  DEBUG ((DEBUG_INFO, "%a Done\n", __FUNCTION__));

  return EFI_SUCCESS;
}
