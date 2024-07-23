/** @file
  USBC Capsule Log Event and Log String Definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _USBC_CAPSULE_EVENTS_H_
#define _USBC_CAPSULE_EVENTS_H_

#include <Uefi.h>

typedef enum {
  // UsbC Retimer Capsule - PD Drive/Restore
  USBC_RETIMER_CAPSULE_EVT_ID_PD_DRIVE_START = 0,
  USBC_RETIMER_CAPSULE_EVT_ID_PD_RESTORE_START,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_CHANGE_MODE_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_CHANGE_MODE_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_FIRST_GET_MODE_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_ALREADY_IN_UPDATE_MODE,
  USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_ALREADY_IN_UPDATE_MODE,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SET_MODE_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SECOND_GET_MODE_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SET_MODE_NOT_COMPLETE,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_ENTER_FW_UPDATE_MODE,
  USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_ENTER_FW_UPDATE_MODE,
  USBC_RETIMER_CAPSULE_EVT_ID_PD_DRIVE_END,
  USBC_RETIMER_CAPSULE_EVT_ID_PD_RESTORE_END,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_FIRST_GET_MODE_EC_STATUS,
  USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_FIRST_GET_MODE_EC_STATUS,
  USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SECOND_GET_MODE_EC_STATUS,
  USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_SECOND_GET_MODE_EC_STATUS,
  // UsbC Retimer Capsule - TBT Command
  USBC_RETIMER_CAPSULE_EVT_ID_UPDATE_RETIMER_PAYLOAD,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_ENUM_CMD,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_ENUM_CMD_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSUP_CMD,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSUP_CMD_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_USUP_CMD,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_USUP_CMD_FAIL,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSEN_CMD,
  USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSEN_CMD_FAIL,
  // UsbC dTBT Capsule
  USBC_DTBT_CAPSULE_EVT_ID_DTBT_UPDATE_START,
  USBC_DTBT_CAPSULE_EVT_ID_DTBT_UPDATE_INFO,
  USBC_DTBT_CAPSULE_EVT_ID_CREATE_DTBT_DEV_INST,
  USBC_DTBT_CAPSULE_EVT_ID_CREATE_DTBT_DEV_INST_FAIL_STATUS,
  USBC_DTBT_CAPSULE_EVT_ID_UPDATE_DTBT_NVM_FW,
  USBC_DTBT_CAPSULE_EVT_ID_UPDATE_DTBT_NVM_FW_FAIL_STATUS,
} USBC_CAPSULE_EVT_ID;

//
// UsbC Retimer Capsule - PD Drive: PD drive mode start
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_PD_DRIVE_START                                    0
#define USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_START                                    ((USBC_RETIMER_CAPSULE_EVT_ID_PD_DRIVE_START << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_PD_DRIVE_START))
#define USBC_RETIMER_CAPSULE_STR_ID_PD_DRIVE_START                                      "-UsbC Retimer Capsule - PD Drive: Start\n"

//
// UsbC Retimer Capsule - PD Restore: PD restore mode start
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_PD_RESTORE_START                                  0
#define USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_START                                  ((USBC_RETIMER_CAPSULE_EVT_ID_PD_RESTORE_START << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_PD_RESTORE_START))
#define USBC_RETIMER_CAPSULE_STR_ID_PD_RESTORE_START                                    "-UsbC Retimer Capsule - PD Restore: Start\n"

//
// UsbC Retimer Capsule - PD Drive: PD change mode fail
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_CHANGE_MODE_FAIL                         1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_CHANGE_MODE_FAIL                         ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_CHANGE_MODE_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_CHANGE_MODE_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_CHANGE_MODE_FAIL                           "-UsbC Retimer Capsule - PD Drive: Change the PD Controller Mode fail  Status: %r\n"

//
// UsbC Retimer Capsule - PD Restore: PD change mode fail
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_CHANGE_MODE_FAIL                       1
#define USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_CHANGE_MODE_FAIL                       ((USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_CHANGE_MODE_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_CHANGE_MODE_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_RESTORE_PD_CHANGE_MODE_FAIL                         "-UsbC Retimer Capsule - PD Restore: Change the PD Controller Mode fail  Status: %r\n"

//
// UsbC Retimer Capsule - PD Drive: First Get PD mode from EC fail
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_FIRST_GET_MODE_FAIL                      1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_FIRST_GET_MODE_FAIL                      ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_FIRST_GET_MODE_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_FIRST_GET_MODE_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_FIRST_GET_MODE_FAIL                        "-UsbC Retimer Capsule - PD Drive: First Get PD mode from EC fail  Status: %r\n"

//
// UsbC Retimer Capsule - PD Drive: PD already in update model
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_ALREADY_IN_UPDATE_MODE                   0
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ALREADY_IN_UPDATE_MODE                   ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_ALREADY_IN_UPDATE_MODE << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_ALREADY_IN_UPDATE_MODE))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_ALREADY_IN_UPDATE_MODE                     "-UsbC Retimer Capsule - PD Drive: PD already in update mode \n"

//
// UsbC Retimer Capsule - PD Restore: PD already in update model
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_ALREADY_IN_UPDATE_MODE                 0
#define USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ALREADY_IN_UPDATE_MODE                 ((USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_ALREADY_IN_UPDATE_MODE << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_ALREADY_IN_UPDATE_MODE))
#define USBC_RETIMER_CAPSULE_STR_ID_RESTORE_PD_ALREADY_IN_UPDATE_MODE                   "-UsbC Retimer Capsule - PD Restore: PD already in update mode \n"

//
// UsbC Retimer Capsule - PD Drive: Set PD mode from EC fail
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SET_MODE_FAIL                            1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_FAIL                            ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SET_MODE_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SET_MODE_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_SET_MODE_FAIL                              "-UsbC Retimer Capsule - PD Drive: PD Set PD mode from EC fail  Status: %r\n"

//
// UsbC Retimer Capsule - PD Drive: Second Get PD mode from EC fail
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SECOND_GET_MODE_FAIL                     1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_FAIL                     ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SECOND_GET_MODE_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SECOND_GET_MODE_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_SECOND_GET_MODE_FAIL                       "-UsbC Retimer Capsule - PD Drive: Second Get PD mode from EC fail  Status: %r\n"

//
// UsbC Retimer Capsule - PD Drive: Set PD mode from EC is not completed
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SET_MODE_NOT_COMPLETE                    1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_NOT_COMPLETE                    ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SET_MODE_NOT_COMPLETE << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SET_MODE_NOT_COMPLETE))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_SET_MODE_NOT_COMPLETE                      "-UsbC Retimer Capsule - PD Drive: PD Set PD mode from EC is not completed  Status: %r\n"


//
// UsbC Retimer Capsule - PD Drive: PD enter into FW update mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_ENTER_FW_UPDATE_MODE                     0
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ENTER_FW_UPDATE_MODE                     ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_ENTER_FW_UPDATE_MODE << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_ENTER_FW_UPDATE_MODE))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_ENTER_FW_UPDATE_MODE                       "-UsbC Retimer Capsule - PD Drive: PD enter into FW update mode \n"

//
// UsbC Retimer Capsule - PD Restore: PD enter into FW update mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_ENTER_FW_UPDATE_MODE                   0
#define USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ENTER_FW_UPDATE_MODE                   ((USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_ENTER_FW_UPDATE_MODE << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_ENTER_FW_UPDATE_MODE))
#define USBC_RETIMER_CAPSULE_STR_ID_RESTORE_PD_ENTER_FW_UPDATE_MODE                     "-UsbC Retimer Capsule - PD Restore: PD enter into FW update mode \n"

//
// UsbC Retimer Capsule - PD Drive: PD drive mode end
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_PD_DRIVE_END                                      0
#define USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_END                                      ((USBC_RETIMER_CAPSULE_EVT_ID_PD_DRIVE_END << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_PD_DRIVE_END))
#define USBC_RETIMER_CAPSULE_STR_ID_PD_DRIVE_END                                        "UsbC Capsule- PD Drive: End\n"

//
// UsbC Retimer Capsule - PD Restore: PD restore mode end
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_PD_RESTORE_END                                    0
#define USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_END                                    ((USBC_RETIMER_CAPSULE_EVT_ID_PD_RESTORE_END << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_PD_RESTORE_END))
#define USBC_RETIMER_CAPSULE_STR_ID_PD_RESTORE_END                                      "UsbC Capsule- PD Restore: End\n"

//
// UsbC Retimer Capsule - PD Drive: Get EC status of first Get PD mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_FIRST_GET_MODE_EC_STATUS                 1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_FIRST_GET_MODE_EC_STATUS                 ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_FIRST_GET_MODE_EC_STATUS << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_FIRST_GET_MODE_EC_STATUS))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_FIRST_GET_MODE_EC_STATUS                   "-UsbC Retimer Capsule - PD Drive: First Get PD mode PD status %x\n"

//
// UsbC Retimer Capsule - PD Restore: Get EC status of first Get PD mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_FIRST_GET_MODE_EC_STATUS               1
#define USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_FIRST_GET_MODE_EC_STATUS               ((USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_FIRST_GET_MODE_EC_STATUS << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_FIRST_GET_MODE_EC_STATUS))
#define USBC_RETIMER_CAPSULE_STR_ID_RESTORE_PD_FIRST_GET_MODE_EC_STATUS                 "-UsbC Retimer Capsule - PD Restore: First Get PD mode PD status %x\n"

//
// UsbC Retimer Capsule - PD Drive: Get EC status of second Get PD mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SECOND_GET_MODE_EC_STATUS                1
#define USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_EC_STATUS                ((USBC_RETIMER_CAPSULE_EVT_ID_DRIVE_PD_SECOND_GET_MODE_EC_STATUS << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_DRIVE_PD_SECOND_GET_MODE_EC_STATUS))
#define USBC_RETIMER_CAPSULE_STR_ID_DRIVE_PD_SECOND_GET_MODE_EC_STATUS                  "-UsbC Retimer Capsule - PD Drive: Second Get PD mode PD status %x\n"

//
// UsbC Retimer Capsule - PD Restore: Get EC status of second Get PD mode
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_SECOND_GET_MODE_EC_STATUS              1
#define USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_SECOND_GET_MODE_EC_STATUS              ((USBC_RETIMER_CAPSULE_EVT_ID_RESTORE_PD_SECOND_GET_MODE_EC_STATUS << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_RESTORE_PD_SECOND_GET_MODE_EC_STATUS))
#define USBC_RETIMER_CAPSULE_STR_ID_RESTORE_PD_SECOND_GET_MODE_EC_STATUS                "-UsbC Retimer Capsule - PD Restore: Second Get PD mode PD status %x\n"

//
// UsbC Retimer Capsule - Retimer: Update Retimer payload
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_UPDATE_RETIMER_PAYLOAD                            1
#define USBC_RETIMER_CAPSULE_EVT_CODE_UPDATE_RETIMER_PAYLOAD                            ((USBC_RETIMER_CAPSULE_EVT_ID_UPDATE_RETIMER_PAYLOAD << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_UPDATE_RETIMER_PAYLOAD))
#define USBC_RETIMER_CAPSULE_STR_ID_UPDATE_RETIMER_PAYLOAD                              "-UsbC Retimer Capsule - Retimer: Update Retimer Payload Index = %d\n"

//
// UsbC Retimer Capsule - Retimer: Send ENUM command
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_ENUM_CMD                              0
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_ENUM_CMD                              ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_ENUM_CMD << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_ENUM_CMD))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_ENUM_CMD                                "-UsbC Retimer Capsule - Retimer: Send ENUM command\n"

//
// UsbC Retimer Capsule - Retimer: Send ENUM command failed
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_ENUM_CMD_FAIL                         1
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_ENUM_CMD_FAIL                         ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_ENUM_CMD_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_ENUM_CMD_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_ENUM_CMD_FAIL                           "-UsbC Retimer Capsule - Retimer: The Retimer could not perform ENUM, Status %d\n"

//
// UsbC Retimer Capsule - Retimer: Send LSUP command
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSUP_CMD                              0
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSUP_CMD                              ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSUP_CMD << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSUP_CMD))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_LSUP_CMD                                "-UsbC Retimer Capsule - Retimer: Send LSUP command\n"

//
// UsbC Retimer Capsule - Retimer: Send LSUP command failed
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSUP_CMD_FAIL                         1
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSUP_CMD_FAIL                         ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSUP_CMD_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSUP_CMD_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_LSUP_CMD_FAIL                           "-UsbC Retimer Capsule - Retimer: The Retimer could not perform LSUP, Status %d\n"

//
// UsbC Retimer Capsule - Retimer: Send USUP command
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_USUP_CMD                              0
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_USUP_CMD                              ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_USUP_CMD << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_USUP_CMD))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_USUP_CMD                                "-UsbC Retimer Capsule - Retimer: Send USUP command\n"

//
// UsbC Retimer Capsule - Retimer: Send USUP command failed
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_USUP_CMD_FAIL                         1
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_USUP_CMD_FAIL                         ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_USUP_CMD_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_USUP_CMD_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_USUP_CMD_FAIL                           "-UsbC Retimer Capsule - Retimer: The Retimer could not perform USUP, Status %d\n"

//
// UsbC Retimer Capsule - Retimer: Send LSEN command
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSEN_CMD                              1
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSEN_CMD                              ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSEN_CMD << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSEN_CMD))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_LSEN_CMD                                "-UsbC Retimer Capsule - Retimer: Send LSEN(%d) command\n"

//
// UsbC Retimer Capsule - Retimer: Send LSEN command failed
//
#define USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSEN_CMD_FAIL                         2
#define USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSEN_CMD_FAIL                         ((USBC_RETIMER_CAPSULE_EVT_ID_TBTCMD_SEND_LSEN_CMD_FAIL << 2) | (USBC_RETIMER_CAPSULE_EVT_ARGS_TBTCMD_SEND_LSEN_CMD_FAIL))
#define USBC_RETIMER_CAPSULE_STR_ID_TBTCMD_SEND_LSEN_CMD_FAIL                           "-UsbC Retimer Capsule - Retimer: The Retimer could not perform LSEN(%d), Status %d\n"

//
// UsbC dTBT Capsule - Start the dTBT Capsule update
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_DTBT_UPDATE_START                                    2
#define USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_START                                    ((USBC_DTBT_CAPSULE_EVT_ID_DTBT_UPDATE_START << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_DTBT_UPDATE_START))
#define USBC_DTBT_CAPSULE_STR_ID_DTBT_UPDATE_START                                      "-UsbC dTBT Capsule - Start Update Index: %x, Total Payload: %x\n"

//
// UsbC dTBT Capsule - dTBT update information
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_DTBT_UPDATE_INFO                                     2
#define USBC_DTBT_CAPSULE_EVT_CODE_DTBT_UPDATE_INFO                                     ((USBC_DTBT_CAPSULE_EVT_ID_DTBT_UPDATE_INFO << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_DTBT_UPDATE_INFO))
#define USBC_DTBT_CAPSULE_STR_ID_DTBT_UPDATE_INFO                                       "-UsbC dTBT Capsule - Image Offset: %x, Size: %x\n"

//
// UsbC dTBT Capsule - Create dTBT device instance
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_CREATE_DTBT_DEV_INST                                 0
#define USBC_DTBT_CAPSULE_EVT_CODE_CREATE_DTBT_DEV_INST                                 ((USBC_DTBT_CAPSULE_EVT_ID_CREATE_DTBT_DEV_INST << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_CREATE_DTBT_DEV_INST))
#define USBC_DTBT_CAPSULE_STR_ID_CREATE_DTBT_DEV_INST                                   "-UsbC dTBT Capsule - Create dTBT device instance\n"

//
// UsbC dTBT Capsule - Create dTBT device instance failed
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_CREATE_DTBT_DEV_INST_FAIL_STATUS                     2
#define USBC_DTBT_CAPSULE_EVT_CODE_CREATE_DTBT_DEV_INST_FAIL_STATUS                     ((USBC_DTBT_CAPSULE_EVT_ID_CREATE_DTBT_DEV_INST_FAIL_STATUS << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_CREATE_DTBT_DEV_INST_FAIL_STATUS))
#define USBC_DTBT_CAPSULE_STR_ID_CREATE_DTBT_DEV_INST_FAIL_STATUS                       "-UsbC dTBT Capsule - Create dTBT device instance failed (%d) at image index %d\n"

//
// UsbC dTBT Capsule - Update dTBT NVM firmware
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_UPDATE_DTBT_NVM_FW                                   0
#define USBC_DTBT_CAPSULE_EVT_CODE_UPDATE_DTBT_NVM_FW                                   ((USBC_DTBT_CAPSULE_EVT_ID_UPDATE_DTBT_NVM_FW << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_UPDATE_DTBT_NVM_FW))
#define USBC_DTBT_CAPSULE_STR_ID_UPDATE_DTBT_NVM_FW                                     "-UsbC dTBT Capsule - Update dTBT NVM Firmware\n"

//
// UsbC dTBT Capsule - Update dTBT NVM firmware failed
//
#define USBC_DTBT_CAPSULE_EVT_ARGS_UPDATE_DTBT_NVM_FW_FAIL_STATUS                       2
#define USBC_DTBT_CAPSULE_EVT_CODE_UPDATE_DTBT_NVM_FW_FAIL_STATUS                       ((USBC_DTBT_CAPSULE_EVT_ID_UPDATE_DTBT_NVM_FW_FAIL_STATUS << 2) | (USBC_DTBT_CAPSULE_EVT_ARGS_UPDATE_DTBT_NVM_FW_FAIL_STATUS))
#define USBC_DTBT_CAPSULE_STR_ID_UPDATE_DTBT_NVM_FW_FAIL_STATUS                         "-UsbC dTBT Capsule - Update dTBT NVM Firmware failed (%d) at image index %d\n"
#endif
