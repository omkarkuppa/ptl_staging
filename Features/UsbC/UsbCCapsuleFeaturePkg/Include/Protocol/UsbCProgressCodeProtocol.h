/** @file
  UsbC Capsule Debug Progress Code Protocol definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _USB_C_PROGRESS_CODE_PROTOCOL_H_
#define _USB_C_PROGRESS_CODE_PROTOCOL_H_

#include <Uefi.h>
#include <PiDxe.h>

///
/// Forward declaration for the _USBC_PROGRESS_CODE_PROTOCOL.
///
typedef struct _USBC_PROGRESS_CODE_PROTOCOL  USBC_PROGRESS_CODE_PROTOCOL;

#define USBC_PROGRESS_CODE_PROTOCOL_GUID \
{ \
    0xAE97B655, 0xB3A1, 0x40A6, {0xA5, 0xAD, 0xAF, 0xE3, 0x00, 0x76, 0xE7, 0x0E} \
}

extern EFI_GUID gUsbCCapsuleDebugProgressCodeProtocolGuid;

/*
TCSS Status Code format:
  +----------------------------------------------------------------------------------------------------------+
  | TCSS Subclass Code | TCSS type     | Phase             | IP             | Operation                      |
  +--------------------+---------------+-------------------+----------------+--------------------------------+
  | [19:16]            | [15:14]       | [13:11]           | [10:8]         | [7:0]                          |
  +--------------------+---------------+-------------------+----------------+--------------------------------+
  | 0x8 - TCSS         | 0x2 - Capsule | 0x2 - End Of PEI  | 0x0 - TCSS     | 0x02 - 0xDF (Reserved)         |
  |                    |               | 0x3 - DXE         |                | 0xE0 - Retimer Capsule Entry   |
  |                    |               |                   |                | 0xE1 - Offline Mode Entry      |
  |                    |               |                   |                | 0xE2 - Offline Mode Exit       |
  |                    |               |                   |                | 0xE3 - ENUM CMD                |
  |                    |               |                   |                | 0xE4 - LSUP CMD                |
  |                    |               |                   |                | 0xE5 - USUP CMD                |
  |                    |               |                   |                | 0xE6 - PD Drive                |
  |                    |               |                   |                | 0xE7 - PD Restore              |
  |                    |               |                   |                | 0xE8 - Retimer FP Assert       |
  |                    |               |                   |                | 0xE9 - Retimer FP De-Assert    |
  |                    |               |                   |                | 0xEA - Update Retimer NVM      |
  |                    |               |                   |                | 0xEB - dTBT Capsule Entry      |
  |                    |               |                   |                | 0xEC - dTBT Create Device Inst |
  |                    |               |                   |                | 0xED - Update dTBT NVM         |
  |                    |               |                   |                | 0xEE - Read Retimer Version    |
  +----------------------------------------------------------------------------------------------------------+
*/

//
// UsbC Capsule Debug Progress Code Definitions
//
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_START                       0x98E0
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_ENTRY          0x98E1
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_OFFLINE_MODE_EXIT           0x98E2
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_ENUM_CMD                    0x98E3
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_LSUP_CMD                    0x98E4
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_USUP_CMD                    0x98E5
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_DRIVE                    0x98E6
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_PD_RESTORE                  0x98E7
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_ASSERT      0x98E8
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_RETIMER_FP_GPIO_DEASSERT    0x98E9
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_UPDATE_RETIMER_NVM_FW       0x98EA
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_DTBT_CAPSULE_START                          0x98EB
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_DTBT_CAPSULE_CREATE_DEV_INST                0x98EC
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_DTBT_CAPSULE_UPDATE_NVM_FW                  0x98ED
#define USBC_DEBUG_PROGRESS_CODE_FEATURES_READ_RETIMER_VERSION_START                  0x98EE

/**
  Show UsbC Capsule Progress Code

  @param[in] ProgressCode  The progress code to be displayed.

**/
typedef
VOID
(*SHOW_PROGRESS_CODE) (
  IN  UINT32 ProgressCode
  );

//
// Interface structure for the UsbC Progress Code Protocol
//
struct _USBC_PROGRESS_CODE_PROTOCOL {
  SHOW_PROGRESS_CODE  ShowProgressCode;  ///  Show Capsule Debug Progress Code
};

#endif
