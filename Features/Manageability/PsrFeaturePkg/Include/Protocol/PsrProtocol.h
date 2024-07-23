/** @file
  Intel Platform Service Record Protocol.

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

#ifndef _PSR_PROTOCOL_H_
#define _PSR_PROTOCOL_H_

typedef struct _PLATFORM_SERVICE_RECORD_PROTOCOL    PLATFORM_SERVICE_RECORD_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gPlatformServiceRecordProtocolGuid;

/**
  PSR Supported Capabilities
**/
#define CHASSIS_INTRUSION      BIT0
#define CRITICAL_TEMPERATURE   BIT1
#define EXCESSIVE_SHOCK        BIT2
#define REMOTE_PLAT_ERASE      BIT3
#define LOCAL_PLAT_ERASE       BIT4
#define FW_RECOVERY            BIT5
#define FW_UPDATE              BIT6
#define SYS_HANG               BIT7
#define PWR_DROP               BIT8

#define PSR_CAPABILITY_MASK    (CHASSIS_INTRUSION | CRITICAL_TEMPERATURE | EXCESSIVE_SHOCK | REMOTE_PLAT_ERASE | LOCAL_PLAT_ERASE | FW_RECOVERY | FW_UPDATE | SYS_HANG | PWR_DROP)

/**
  PSR Event ID
**/
#define FW_RECOVERY_EVENT_ID  36
#define FW_UPDATE_EVENT_ID    37
#define SYSTEM_HANG_EVENT_ID  38
#define POWER_DROP_EVENT_ID   39

/**
  PSR Event data
**/
#define DEFAULT_EVENT_DATA    0

/**
  Intel PSR Protocol revision number
**/
#define PLATFORM_SERVICE_RECORD_PROTOCOL_REVISION       3

/**
  Get PSR blob from CSME via HECI cmds.

  @param[in]      *This                  Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in]      PsrDataBlobSize        PSR Data Buffer Size.
  @param[in, out] *PsrDataBlob           Pointer of the PSR Data Blob.

  @retval         EFI_SUCCESS            Retrieved PSR from CSME successfully.
  @retval         Others                 Failed to retrieve the record.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_SERVICE_RECORD) (
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
typedef
EFI_STATUS
(EFIAPI *EXPORT_PSR_TO_USB_STORAGE) (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN EFI_HANDLE                       *ExportHandle
);

/**
  Inform CSME of Chassis Intrusion via HECI cmds

  @param[in] *This                     Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS                  Chassis Intrusion event has been successfully send to CSME.
**/
typedef
EFI_STATUS
(EFIAPI *CHASSIS_INTRUSION_DETECTED) (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This
);

/**
  Inform CSME the last shutdown was due to Critical Temperature

  @param[in] *This                     Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.

  @retval EFI_SUCCESS                  Event information has been successfully send to CSME.
**/
typedef
EFI_STATUS
(EFIAPI *CRITICAL_TEMP_SHUTDOWN) (
  IN  PLATFORM_SERVICE_RECORD_PROTOCOL *This
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

  @param[in] This                     Pointer to PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in] EventId                  Specific event ID to notify CSME the event.
  @param[in] EventData                Data payload.

  @retval EFI_SUCCESS                 Event ID has been successfully send to CSME.
  @retval EFI_NOT_READY               CSME not ready to receive the event.
  @retval Others                      Failed to nofity CSME the Event ID.
**/
typedef
EFI_STATUS
(EFIAPI *EVENT_REPORT) (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN UINT32                           EventId,
  IN UINT32                           EventData
);

/**
  Platform Service Record Protocol
  The interface functions are used to communicate with CSME via HECI inteface.

  <b>Revision 1:</b>: - Initial version.
  <b>Revision 2:</b>: - Add NotifyCriticalTempShutdown function.
  <b>Revision 3:</b>: - Add NotifyEventReport function for Firmware Recovery, Firmware Update,
                        System Hang and Power Drop events.
**/
typedef struct _PLATFORM_SERVICE_RECORD_PROTOCOL {
  UINT8                               Revision;
  GET_PLATFORM_SERVICE_RECORD         PsrGetData;
  EXPORT_PSR_TO_USB_STORAGE           PsrExportToUsbStorage;
  CHASSIS_INTRUSION_DETECTED          NotifyChassisIntrusionDetection;
  CRITICAL_TEMP_SHUTDOWN              NotifyCriticalTempShutdown;
  EVENT_REPORT                        NotifyEventReport;
} PLATFORM_SERVICE_RECORD_PROTOCOL;

#endif
