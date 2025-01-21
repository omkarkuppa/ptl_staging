/** @file
  Definitions for functions used for handling PSR Settings.

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

#ifndef _PSR_SETTINGS_H
#define _PSR_SETTINGS_H

#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/BlockIo.h>
#include <Protocol/UsbIo.h>
#include <Protocol/DevicePath.h>

#include <PsrSetup.h>

#define PLATFORM_SERVICE_RECORD_VERSION  1
#define MAX_DATE_LENGTH                  4
#define MAX_STRING_LENGTH                64
#define MAX_OEM_DATA_LENGTH              (MAX_STRING_LENGTH * MAX_STRING_LENGTH)
#define SINGLE_EVENT_SIZE                12
#define PSR_UUID_LENGTH                  16

// PSR Generic Info
#define PSR_LOG_STATE_OFFSET                      0    // 4 bytes in total
#define PSR_VERISON_OFFSET                        4    // 4 bytes in total
#define PSRID_OFFSET                              8    // 16 bytes in total
#define UPID_OFFSET                              24    // 64 bytes in total

// GENESIS DATA LEDGER - 1284 bytes in total
#define GENESIS_DATE_OFFSET                      88    // Genesis Date occupies 4 bytes; 88 + 4 = 92 (next offset)
#define GENESIS_OEM_INFO_OFFSET                  92    // Oem Info occupies 64 bytes
#define GENESIS_OEM_MAKE_OFFSET                 156    // Oem Make occupies 64 bytes
#define GENESIS_OEM_MODEL_OFFSET                220    // Oem Model occupies 64 bytes
#define GENESIS_MANUFAC_COE_OFFSET              284    // Oem Country of origin occupies 64 bytes
#define GENESIS_OEM_DATA_STORE_OFFSET           348    // Oem Data Store occupies 1024 bytes

#define PSR_CAPABILITY_MASK_OFFSET             1372    // 32 bytes in total

// LEDGER (POWER STATUS HISTORICAL COUNTER)  -  128 bytes in total (each 4 bytes)
#define LEDGER_CTR_S0_OFFSET                   1404
#define LEDGER_CTR_S5_OFFSET                   1408
#define LEDGER_CTR_S4_OFFSET                   1412
#define LEDGER_CTR_S3_OFFSET                   1416
#define LEDGER_CTR_WARM_RESET                  1420
#define LEDGER_CTR_ISH_CONNECTION              1424
#define LEDGER_CTR_CSME_RESET                  1428
#define LEDGER_CTR_RESERVED1_OFFSET            1432    // Reserved from 1432 - 1468 = 36 bytes for future use
#define LEDGER_CTR_PRTC_FAILURE                1468
#define LEDGER_CTR_CSME_INVALID_STATE          1472
#define LEDGER_CTR_CSME_DAM_STATE              1476
#define LEDGER_CTR_CSME_UNLOCK_STATE           1480
#define LEDGER_CTR_SVN_INCREASE                1484
#define LEDGER_CTR_RESERVED2_OFFSET            1488    // Reserved from 1488 - 1500 = 12 bytes for future use
#define LEDGER_CTR_EXCESSIVE_SHOCK             1500
#define LEDGER_CTR_EXCESSIVE_OPER_TEMP         1504
#define LEDGER_CTR_FW_RECOVERY                 1508
#define LEDGER_CTR_FW_UPDATE                   1512
#define LEDGER_CTR_SYS_HANG                    1516
#define LEDGER_CTR_PWR_DROP                    1520
#define LEDGER_CTR_RESERVED3_OFFSET            1524    // Reserved from 1524 - 1532 = 8 bytes for future use

// EVENT DATA LEDGER
#define EVENT_DATA_TOTAL_COUNT_START_OFFSET    1532    // Total Event Count occupies 4 bytes
#define EVENT_LOG_START_OFFSET                 1536    // 12 * 100 = 1200 bytes in total (Offset 1536 to 2736)

// CSME FW VERSION
#define CSME_FW_VERSION_OFFSET                 2840    // 8 bytes in total

#define PSR_CFG_VARIABLE_NAME                  L"PsrCfg"
#define PSR_DEFAULT_USB_DEVICE_NAME            L"UEFI USB Device"

typedef enum {
  PsrStartLogEvent = 8,
  PsrEndLogEvent,
  PsrMaxEventNumberReachedEvent,
  PsrReplayProtectionInfraFailureEvent = 16,
  PsrMissingEvent,
  PsrInvalidEvent,
  PsrPrtcFailureEvent,
  PsrCsmeRecoveryEvent,
  PsrCsmeEnteredDamStateEvent,
  PsrCsmeEnteredUnlockStateEvent,
  PsrSvnIncreaseEvent,
  PsrChassisIntrusionEvent = 32,
  PsrExcessiveShockEvent,
  PsrExcessiveOperTemp,
  PsrErase,
  PsrFwRecoveryEvent,
  PsrFwUpdateEvent,
  PsrSysHangEvent,
  PsrPwrDropEvent
} PSR_EVENT_TYPE;

typedef enum {
  PsrChassisIntrusion = 0,
  PsrCriticalTemp,
  PsrShockDetection,
  PsrRpe,
  PsrLpe,
  PsrFwRecovery,
  PsrFwUpdate,
  PsrSysHang,
  PsrPwrDrop
} PSR_CAPABILITY_LIST;

typedef enum {
  PsrEvtSrcRemote = 0,
  PsrEvtSrcLocal,
  PsrEvtSrcOther,
} PSR_EVENT_SOURCE_TYPE;

//
// PSR_EXPORT_INFO_LINK is a linked list that will store the informations of valid export usb storage options.
//
#define PSR_EXPORT_INFO_SIGNATURE        SIGNATURE_32 ('P', 'S', 'R', 'E')
#define PSR_EXPORT_INFO_FROM_LINK(a)     CR(a, PSR_EXPORT_INFO_LINK, Link, PSR_EXPORT_INFO_SIGNATURE)

//
// Signature           : Signature of PSR_EXPORT_INFO_LINK data structure.
// Link                : Node entry of PSR_EXPORT_INFO_LINK data structure.
// TargetblkHandle     : Pointer to store Block Io handle for the usb storage options.
// TargetDevicePath    : Device Path for the export usb storage  options.
// TargetUsbDescription: The USB descriptions of the export usb storage options.
//
typedef struct PsrExportInfoLink {
  UINTN                       Signature;
  LIST_ENTRY                  Link;
  EFI_HANDLE                  *TargetblkHandle;
  EFI_DEVICE_PATH_PROTOCOL    *TargetDevicePath;
  CHAR16                      *TargetUsbDescription;
} PSR_EXPORT_INFO_LINK;

/**
  Get Usb Device Description.

  @param[in]  UsbIo          The Pointer to EFI_USB_IO_PROTOCOL.

  @retval     Description    Return USB device description string.

**/
CHAR16 *
GetUsbDeviceDescription (
  IN EFI_USB_IO_PROTOCOL    *UsbIo
  );

/**
  List all Usb Storages available for export of data.

  @retval PsrUsbExportOption    Return the pointer that holding PSR_USB_EXPORT_OPTION.
**/
PSR_EXPORT_INFO_LINK *
DetectAndListAvailableFlashDrives (
  VOID
  );

/**
  Initial PSR configuration.

  @param[out] PsrConfiguration    Pointer of PSR Configuration.

  @retval EFI_SUCCESS             Initial PSR Configuration successfully.
  @retval Others                  Failed to get PSR data.
**/
EFI_STATUS
PsrGetInfo (
  OUT PSR_CONFIGURATION    *PsrConfiguration
  );
#endif // _PSR_SETTINGS_H
