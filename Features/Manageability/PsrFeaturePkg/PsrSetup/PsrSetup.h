/** @file

  This file contains definitions for PSR initialization

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

#ifndef _PSR_SETUP_H
#define _PSR_SETUP_H

#include <PsrMsgs.h>

#define PSR_MAX_EVENT_NAME_SIZE         50
#define PSR_LOG_STATE_NAME_SIZE         16
#define EPOCH                           1970           // 1970-01-01 00:00:00
#define LEAP_YEARS_FROM_ONE_TO_EPOCH    477            // Number of Leap years in range (1, EPOCH) = (EPOCH / 4) - (EPOCH / 100) + (EPOCH / 400);
#define SEC_IN_A_DAY                    (24 * 60 * 60)
#define PSR_MAX_EVENT_DATA_LENGTH       4
#define PSR_TOTAL_EVENT_DATA            400            // (PSR_MAX_EVENT_COUNT * PSR_MAX_EVENT_DATA_LENGTH)

#define PSR_CAPABILITIES_SUPPORTED       L"Supported"
#define PSR_CAPABILITIES_NOT_SUPPORTED   L"Not Supported"

#define EVENT_ACT_BIT   2
#define EVENT_SRC_BIT   7
#define EVENT_STA_MASK  BIT0                               // Status: 0 (Fail); 1(Success)
#define EVENT_ACT_MASK  (BIT2 | BIT3 | BIT4 | BIT5 | BIT6) // Action Type
#define EVENT_SRC_MASK  (BIT7 | BIT8)                      // Source: 0 (RPE); 1 (LPE); 2 (Other)

#define PSR_CONFIGURATION_REVISION      1

#pragma pack(1)

typedef struct {
  UINT64       Name1;
  UINT64       Name2;
  UINT64       Name3;
  UINT64       Name4;

  UINT64       Name5;
  UINT64       Name6;
  UINT64       Name7;
  UINT64       Name8;
} EFI_UPID;

typedef struct {
  UINT16  Year;
  UINT8   Month;
  UINT8   Day;
  UINT8   Hour;
  UINT8   Minute;
  UINT8   Second;
} EVENT_DATE_FORMAT;

typedef struct {
  UINT8            EventId;
  UINT32           EventTime;
  UINT8            EventStatus;
  UINT8            EventAction;
  UINT8            EventSource;
  UINT8            EventData[32];
} PSR_EVENTS;

/**
  The data structure is for PSR initialization
**/
typedef struct {
  UINT8            PsrLogState;
  PSR_VERSION      PsrVersion;
  UINT8            Psrid[16];
  EFI_UPID         Upid;
  UINT8            Capabilities[PSR_MAX_SUPPORTED_CAPABILITIES];
  UINT32           GenesisDate;
  CHAR16           OemInfo[32];
  CHAR16           OemMakeInfo[32];
  CHAR16           OemModelInfo[32];
  CHAR16           ManufCOE[32];
  CHAR16           OemDataStore[512];

// Counter values
  UINT32           S0RuntimeCtr;
  UINT32           WarmResetCtr;
  UINT32           S0toS5Ctr;
  UINT32           S0toS4Ctr;
  UINT32           S0toS3Ctr;
  UINT32           PrtcFailureCtr;
  UINT32           CsmeInvalidStateCtr;   // CSME entered Recovery/Disabled/SKU mismatch state
  UINT32           CsmeDamCtr;
  UINT32           CsmeUnlockedCtr;
  UINT32           SvnIncreaseCtr;
  UINT32           ExcessiveShockCtr;
  UINT32           ExcessiveOperationalTempCtr;
  UINT32           IshConnectionCtr;
  UINT32           CsmeResetCtr;
  UINT32           FwRecoveryCtr;
  UINT32           FwUpdateCtr;
  UINT32           SysHangCtr;
  UINT32           PwrDropCtr;

 //Event data
  UINT32           TotalEventCount;
  PSR_EVENTS       Events[PSR_MAX_EVENT_COUNT];
  CSME_FW_VERSION  FwVersion;
  UINT8            Rsvd[32];               // RSVD for future use
} PSR_CONFIGURATION;

#pragma pack()

#define PSR_FORMSET_GUID { 0x23148e5f, 0xf63f, 0x403c, {0xa6, 0x00, 0x03, 0x25, 0x28, 0x04, 0x09, 0x75}}

#define FORM_ID_PSR                    6001
#define FORM_ID_PSR_EDL                6002
#define FORM_ID_PSR_RECORD_EXPORT      6003

//
// Varstore ID defined for Buffer Storage
//
#define VARSTORE_ID_PSR                0x5566

#define PSR_LABEL_START                0x6100
#define PSR_LABEL_END                  0x6101

#define PSR_EXPORT_OPTION_LABEL        0x6102
#define PSR_EXPORT_OPTION_LABEL_END    0x6103

#define PSR_EVENT_LABEL                0x6104
#define PSR_EVENT_LABEL_END            0x6105

#define KEY_PSR_EXPORT_BASE            0x6200

//
// For Accounting number format, ex:"1,234,567,890", max power sec digits will include 3 commas and '\0'
//
#define MAX_POWER_SEC_DIGITS           14

// VFR common Definitions
#define INVENTORY(Name,Value)                \
          text                               \
            help  = STRING_TOKEN(STR_EMPTY), \
            text  = Name,                    \
            text  = Value,                   \
            flags = 0,                       \
            key   = 0;

//
// These are the VFR compiler generated data representing our VFR data.
//
extern UINT8 PsrSetupVfrBin[];
extern UINT8 PsrSetupStrings[];

#endif // _PSR_SETUP_H
