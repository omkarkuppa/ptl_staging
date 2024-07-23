/** @file
  Intel Platform Service Record Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _PLATFORM_SERVICE_RECORD_H_
#define _PLATFORM_SERVICE_RECORD_H_

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
  );

/**
  Export PSR to selected Usb Storage.

  @param[in] *This                      Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in] *ExportHandle              BlockIO handle of the selected device.

  @retval    EFI_SUCCESS                Export PSR data to specific USB device successfully.
  @retval    Others                     Failed to export PSR data to specific USB device.

**/
EFI_STATUS
EFIAPI
ExportPsrToUsbStorage (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN EFI_HANDLE                       *ExportHandle
  );

/**
  Inform CSME of Chassis Intrusion via HECI cmds

  @param[in] *This                     Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS                  Chassis Intrusion event has been successfully send to CSME.
  @retval EFI_NOT_READY                CSME not ready to receive the event
**/
EFI_STATUS
EFIAPI
NotifyChassisIntrusionDetection (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This
  );

/**
  Inform CSME the last shutdown was due to Critical Temperature

  @param[in] *This                     Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS                  Event information has been successfully send to CSME.
  @retval EFI_NOT_READY                CSME not ready to receive the event
**/
EFI_STATUS
EFIAPI
NotifyCriticalTempShutdown (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This
  );

/**
  Inform CSME of below events via HECI interface only when PSR log state is started.
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
  @retval Others             Failed to nofity CSME the Event ID.
**/
EFI_STATUS
EFIAPI
NotifyEventReport (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN UINT32                           EventId,
  IN UINT32                           EventData
  );
#endif
