/** @file
  Log Event and Log String Definitions - TbtRetimerNvmUpdateLib

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

#ifndef _USBC_CAPSULE_TBT_RETIMER_NVM_H_
#define _USBC_CAPSULE_TBT_RETIMER_NVM_H_

#include "UsbCCapsuleLogEventsId.h"


///< TbtRetimerNvmUpdateLib.c - start
///
/// CreateHrDevInstance - RetimerDeviceInstancesCount is zero
///
#define EVT_ARGS_HR_DEV_COUNT_ZERO                           0
#define EVT_CODE_HR_DEV_COUNT_ZERO                           ((EVT_ID_HR_DEV_COUNT_ZERO << 2) | (EVT_ARGS_HR_DEV_COUNT_ZERO))
#define STR_ID_HR_DEV_COUNT_ZERO                             "RetimerDeviceInstancesCount is zero.\n"
///
/// CreateHrDevInstance - HrDeviceInstances already exists
///
#define EVT_ARGS_HR_DEV_ALREADY                              0
#define EVT_CODE_HR_DEV_ALREADY                              ((EVT_ID_HR_DEV_ALREADY << 2) | (EVT_ARGS_HR_DEV_ALREADY))
#define STR_ID_HR_DEV_ALREADY                                "HrDeviceInstances already exists.\n"
///
/// CreateHrDevInstance - CreateHrDevInstance start
///
#define EVT_ARGS_HR_DEV_CREATE_START                         0
#define EVT_CODE_HR_DEV_CREATE_START                         ((EVT_ID_HR_DEV_CREATE_START << 2) | (EVT_ARGS_HR_DEV_CREATE_START))
#define STR_ID_HR_DEV_CREATE_START                           "CreateHrDevInstance - Start\n"
///
/// CreateRetimerDevInstance - CreateRetimerDevInstance: Failed to allocate memory for PCIE_BDF
///
#define EVT_ARGS_RT_BDF_ALLOCATE_MEM_FAIL                    0
#define EVT_CODE_RT_BDF_ALLOCATE_MEM_FAIL                    ((EVT_ID_RT_BDF_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_RT_BDF_ALLOCATE_MEM_FAIL))
#define STR_ID_RT_BDF_ALLOCATE_MEM_FAIL                      "CreateRetimerDevInstance: Failed to allocate memory for PCIE_BDF.\n"
///
/// CreateRetimerDevInstance - CreateTBTDevInstance: Failed to allocate memory for PCIE_RP_CONFIG
///
#define EVT_ARGS_RT_RP_ALLOCATE_MEM_FAIL                     0
#define EVT_CODE_RT_RP_ALLOCATE_MEM_FAIL                     ((EVT_ID_RT_RP_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_RT_RP_ALLOCATE_MEM_FAIL))
#define STR_ID_RT_RP_ALLOCATE_MEM_FAIL                       "CreateTBTDevInstance: Failed to allocate memory for PCIE_RP_CONFIG.\n"
///
/// CreateRetimerDevInstance - Error DevAddress->Port value
///
#define EVT_ARGS_RT_DEV_ADDRESS_ERROR                        0
#define EVT_CODE_RT_DEV_ADDRESS_ERROR                        ((EVT_ID_RT_DEV_ADDRESS_ERROR << 2) | (EVT_ARGS_RT_DEV_ADDRESS_ERROR))
#define STR_ID_RT_DEV_ADDRESS_ERROR                          "Error DevAddress->Port value.\n"
///
/// CreateRetimerDevInstance - Failed to create a device context for the retimer.
///
#define EVT_ARGS_RT_DEV_CREATE_FAIL                          0
#define EVT_CODE_RT_DEV_CREATE_FAIL                          ((EVT_ID_RT_DEV_CREATE_FAIL << 2) | (EVT_ARGS_RT_DEV_CREATE_FAIL))
#define STR_ID_RT_DEV_CREATE_FAIL                            "Failed to create a device context for the retimer.\n"
///
/// CreateTBTDevInstance - Failed to allocate memory for PCIE_RP_CONFIG
///
#define EVT_ARGS_CREATE_TBT_ALLOCATE_MEM_FAIL                0
#define EVT_CODE_CREATE_TBT_ALLOCATE_MEM_FAIL                ((EVT_ID_CREATE_TBT_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_CREATE_TBT_ALLOCATE_MEM_FAIL))
#define STR_ID_CREATE_TBT_ALLOCATE_MEM_FAIL                  "CreateTBTDevInstance: Failed to allocate memory for PCIE_RP_CONFIG.\n"
///
/// CreateTBTDevInstance - Failed to create DMA
///
#define EVT_ARGS_CREATE_TBT_DMA_FAIL                         0
#define EVT_CODE_CREATE_TBT_DMA_FAIL                         ((EVT_ID_CREATE_TBT_DMA_FAIL << 2) | (EVT_ARGS_CREATE_TBT_DMA_FAIL))
#define STR_ID_CREATE_TBT_DMA_FAIL                           "CreateTBTDevInstance: Failed to create DMA.\n"
///
/// ReadRetimerNvmVersion - TbtDrvReadNvmVersion error
///
#define EVT_ARGS_READ_NVM_VERSION_ERROR                      1
#define EVT_CODE_READ_NVM_VERSION_ERROR                      ((EVT_ID_READ_NVM_VERSION_ERROR << 2) | (EVT_ARGS_READ_NVM_VERSION_ERROR))
#define STR_ID_READ_NVM_VERSION_ERROR                        "TbtDrvReadNvmVersion error %r.\n"
///
/// ReadRetimerNvmVersion - Current NVM version is rev%02x v%02x.
///
#define EVT_ARGS_READ_NVM_VERSION_INFO                       2
#define EVT_CODE_READ_NVM_VERSION_INFO                       ((EVT_ID_READ_NVM_VERSION_INFO << 2) | (EVT_ARGS_READ_NVM_VERSION_INFO))
#define STR_ID_READ_NVM_VERSION_INFO                         "Current NVM version is rev%02x v%02x."
///
/// ReadRetimerNvmVersion - %02x Official bit: %d
///
#define EVT_ARGS_READ_NVM_VERSION_INFO2                      2
#define EVT_CODE_READ_NVM_VERSION_INFO2                      ((EVT_ID_READ_NVM_VERSION_INFO2 << 2) | (EVT_ARGS_READ_NVM_VERSION_INFO2))
#define STR_ID_READ_NVM_VERSION_INFO2                        "%02x Official bit: %d\n"
///
/// UpdateRetimerNvmInformation - Failed to get setup variable
///
#define EVT_ARGS_UPDATE_RT_GET_VARIABLE_FAIL                 0
#define EVT_CODE_UPDATE_RT_GET_VARIABLE_FAIL                 ((EVT_ID_UPDATE_RT_GET_VARIABLE_FAIL << 2) | (EVT_ARGS_UPDATE_RT_GET_VARIABLE_FAIL))
#define STR_ID_UPDATE_RT_GET_VARIABLE_FAIL                   "Failed to get setup variable.\n"
///
/// UpdateRetimerNvmInformation - FirmwareType is not supported
///
#define EVT_ARGS_UPDATE_RT_FW_TYPE_UNSUPPORT                 0
#define EVT_CODE_UPDATE_RT_FW_TYPE_UNSUPPORT                 ((EVT_ID_UPDATE_RT_FW_TYPE_UNSUPPORT << 2) | (EVT_ARGS_UPDATE_RT_FW_TYPE_UNSUPPORT))
#define STR_ID_UPDATE_RT_FW_TYPE_UNSUPPORT                   "FirmwareType is not supported.\n"
///
/// UpdateRetimerNvmInformation - TcssRetimerNumber setting Index = %x
///
#define EVT_ARGS_UPDATE_RT_INVALID_RETIMER_INDEX             0
#define EVT_CODE_UPDATE_RT_INVALID_RETIMER_INDEX             ((EVT_ID_UPDATE_RT_INVALID_RETIMER_INDEX << 2) | (EVT_ARGS_UPDATE_RT_INVALID_RETIMER_INDEX))
#define STR_ID_UPDATE_RT_INVALID_RETIMER_INDEX               "UsbCRetimerNumber is more than Index setting.\n"
///
/// UpdateRetimerNvmInformation - RetimerConfigTable config %x is matched.
///
#define EVT_ARGS_UPDATE_RT_CONFIG_MATCH                      1
#define EVT_CODE_UPDATE_RT_CONFIG_MATCH                      ((EVT_ID_UPDATE_RT_CONFIG_MATCH << 2) | (EVT_ARGS_UPDATE_RT_CONFIG_MATCH))
#define STR_ID_UPDATE_RT_CONFIG_MATCH                        "RetimerConfigTable config %x is matched.\n"
///
/// UpdateRetimerNvmInformation - Fail on RetimerPayloadConfig header in capsule file does not match the RetimerConfigTable
///
#define EVT_ARGS_UPDATE_RT_CONFIG_NOT_MATCH                  0
#define EVT_CODE_UPDATE_RT_CONFIG_NOT_MATCH                  ((EVT_ID_UPDATE_RT_CONFIG_NOT_MATCH << 2) | (EVT_ARGS_UPDATE_RT_CONFIG_NOT_MATCH))
#define STR_ID_UPDATE_RT_CONFIG_NOT_MATCH                    "UpdateRetimerNvmInformation - Fail on RetimerPayloadConfig header in capsule file does not match the RetimerConfigTable.\n"
///
/// UpdateRetimerNvmInformation - Fail to set variable in UsbCRetimerSetup
///
#define EVT_ARGS_UPDATE_RT_SET_VARIABLE_FAIL                 0
#define EVT_CODE_UPDATE_RT_SET_VARIABLE_FAIL                 ((EVT_ID_UPDATE_RT_SET_VARIABLE_FAIL << 2) | (EVT_ARGS_UPDATE_RT_SET_VARIABLE_FAIL))
#define STR_ID_UPDATE_RT_SET_VARIABLE_FAIL                   "UpdateRetimerNvmInformation - Fail to set variable in UsbCRetimerSetup.\n"
///
/// InitRetimerHW - InitRetimerHW START
///
#define EVT_ARGS_INIT_RT_HW_START                            0
#define EVT_CODE_INIT_RT_HW_START                            ((EVT_ID_INIT_RT_HW_START  << 2) | (EVT_ARGS_INIT_RT_HW_START))
#define STR_ID_INIT_RT_HW_START                              "InitRetimerHW START\n"
///
/// InitRetimerHW - Got an error status
///
#define EVT_ARGS_INIT_RT_HW_GOT_ERROR                        1
#define EVT_CODE_INIT_RT_HW_GOT_ERROR                        ((EVT_ID_INIT_RT_HW_GOT_ERROR << 2) | (EVT_ARGS_INIT_RT_HW_GOT_ERROR))
#define STR_ID_INIT_RT_HW_GOT_ERROR                          "InitRetimerHW: Got an ERROR(%d)\n"
///
/// InitRetimerHW - InitRetimerHW success
///
#define EVT_ARGS_INIT_RT_HW_SUCCESS                          0
#define EVT_CODE_INIT_RT_HW_SUCCESS                          ((EVT_ID_INIT_RT_HW_SUCCESS << 2) | (EVT_ARGS_INIT_RT_HW_SUCCESS))
#define STR_ID_INIT_RT_HW_SUCCESS                            "InitRetimerHW success\n"
///
/// TerminateRetimerHW - TerminateRetimerHW START
///
#define EVT_ARGS_TERMINATE_START                             0
#define EVT_CODE_TERMINATE_START                             ((EVT_ID_TERMINATE_START << 2) | (EVT_ARGS_TERMINATE_START))
#define STR_ID_TERMINATE_START                               "TerminateRetimerHW START\n"
///
/// TerminateRetimerHW - Got an error status
///
#define EVT_ARGS_TERMINATE_GOT_ERROR                         1
#define EVT_CODE_TERMINATE_GOT_ERROR                         ((EVT_ID_TERMINATE_GOT_ERROR << 2) | (EVT_ARGS_TERMINATE_GOT_ERROR))
#define STR_ID_TERMINATE_GOT_ERROR                           "TerminateRetimerHW: Got an ERROR(%d)\n"
///
/// TerminateRetimerHW - TerminateRetimerHW success
///
#define EVT_ARGS_TERMINATE_SUCCESS                           0
#define EVT_CODE_TERMINATE_SUCCESS                           ((EVT_ID_TERMINATE_SUCCESS << 2) | (EVT_ARGS_TERMINATE_SUCCESS))
#define STR_ID_TERMINATE_SUCCESS                             "TerminateRetimerHW success\n"
///
/// UpdateRetimerNvmFirmware - Update Retimer NVM Firmware
///
#define EVT_ARGS_UPDATE_NVM_RT_FW_START                      1
#define EVT_CODE_UPDATE_NVM_RT_FW_START                      ((EVT_ID_UPDATE_NVM_RT_FW_START << 2) | (EVT_ARGS_UPDATE_NVM_RT_FW_START))
#define STR_ID_UPDATE_NVM_RT_FW_START                        "UpdateRetimerNvmFirmware START (0x%06x bytes to write):\n"
///
/// UpdateRetimerNvmFirmware - Got a FATAL error, exiting...
///
#define EVT_ARGS_UPDATE_NVM_FATAL_ERROR                      0
#define EVT_CODE_UPDATE_NVM_FATAL_ERROR                      ((EVT_ID_UPDATE_NVM_FATAL_ERROR << 2) | (EVT_ARGS_UPDATE_NVM_FATAL_ERROR))
#define STR_ID_UPDATE_NVM_FATAL_ERROR                        "Got a FATAL error, exiting...\n"
///
/// UpdateRetimerNvmFirmware - The total data to write is 0x%6x
///
#define EVT_ARGS_UPDATE_NVM_WRITE_DATA                       1
#define EVT_CODE_UPDATE_NVM_WRITE_DATA                       ((EVT_ID_UPDATE_NVM_WRITE_DATA << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_DATA))
#define STR_ID_UPDATE_NVM_WRITE_DATA                         "The total data to write is 0x%6x\n"
///
/// UpdateRetimerNvmFirmware - Written so far : 0x%05x bytes
///
#define EVT_ARGS_UPDATE_NVM_WRITE_SIZE                       1
#define EVT_CODE_UPDATE_NVM_WRITE_SIZE                       ((EVT_ID_UPDATE_NVM_WRITE_SIZE << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_SIZE))
#define STR_ID_UPDATE_NVM_WRITE_SIZE                         "Written so far : 0x%05x bytes\n"
///
/// UpdateRetimerNvmFirmware - Writing at Offset - 0x%x
///
#define EVT_ARGS_UPDATE_NVM_WRITE_INFO                       1
#define EVT_CODE_UPDATE_NVM_WRITE_INFO                       ((EVT_ID_UPDATE_NVM_WRITE_INFO << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_INFO))
#define STR_ID_UPDATE_NVM_WRITE_INFO                         "Writing at Offset - 0x%x "
///
/// UpdateRetimerNvmFirmware - length of 0x%0x, first reg 0x%04x
///
#define EVT_ARGS_UPDATE_NVM_WRITE_INFO2                      2
#define EVT_CODE_UPDATE_NVM_WRITE_INFO2                      ((EVT_ID_UPDATE_NVM_WRITE_INFO2 << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_INFO2))
#define STR_ID_UPDATE_NVM_WRITE_INFO2                        "length of 0x%0x, first reg 0x%04x\n"
///
/// UpdateRetimerNvmFirmware - Got an error at offset xxx
///
#define EVT_ARGS_UPDATE_NVM_ERROR_OFFSET                     1
#define EVT_CODE_UPDATE_NVM_ERROR_OFFSET                     ((EVT_ID_UPDATE_NVM_ERROR_OFFSET << 2) | (EVT_ARGS_UPDATE_NVM_ERROR_OFFSET))
#define STR_ID_UPDATE_NVM_ERROR_OFFSET                       "Got an error at offset 0x%08x\n"
///
/// UpdateRetimerNvmFirmware - Image write finished.
///
#define EVT_ARGS_UPDATE_NVM_WRITE_FINISH                     0
#define EVT_CODE_UPDATE_NVM_WRITE_FINISH                     ((EVT_ID_UPDATE_NVM_WRITE_FINISH << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_FINISH))
#define STR_ID_UPDATE_NVM_WRITE_FINISH                       "Image write finished.\n"
///
/// UpdateRetimerNvmFirmware - Got an error while writing the image
///
#define EVT_ARGS_UPDATE_NVM_WRITE_IM_ERROR                   0
#define EVT_CODE_UPDATE_NVM_WRITE_IM_ERROR                   ((EVT_ID_UPDATE_NVM_WRITE_IM_ERROR << 2) | (EVT_ARGS_UPDATE_NVM_WRITE_IM_ERROR))
#define STR_ID_UPDATE_NVM_WRITE_IM_ERROR                     "Got an error while writing the image. As a recovery, starting again.\n"
///
/// UpdateRetimerNvmFirmware - Image write wasn't successful due to a device error
///
#define EVT_ARGS_UPDATE_NVM_DEV_ERROR                        0
#define EVT_CODE_UPDATE_NVM_DEV_ERROR                        ((EVT_ID_UPDATE_NVM_DEV_ERROR << 2) | (EVT_ARGS_UPDATE_NVM_DEV_ERROR))
#define STR_ID_UPDATE_NVM_DEV_ERROR                          "Image write wasn't successful due to a device error.\n"
///
/// UpdateRetimerNvmFirmware - Retimer firmware update and authentication success
///
#define EVT_ARGS_UPDATE_NVM_AUTH_SUCCESS                     0
#define EVT_CODE_UPDATE_NVM_AUTH_SUCCESS                     ((EVT_ID_UPDATE_NVM_AUTH_SUCCESS << 2) | (EVT_ARGS_UPDATE_NVM_AUTH_SUCCESS))
#define STR_ID_UPDATE_NVM_AUTH_SUCCESS                        "UpdateRetimerNvmFirmware: Retimer firmware update and authentication success\n"
///
/// UpdateDiscreteTbtNvmFirmware - UpdateDiscreteTbtNvmFirmware START
///
#define EVT_ARGS_UPDATE_DTBT_START                           1
#define EVT_CODE_UPDATE_DTBT_START                           ((EVT_ID_UPDATE_DTBT_START << 2) | (EVT_ARGS_UPDATE_DTBT_START))
#define STR_ID_UPDATE_DTBT_START                             "UpdateDiscreteTbtNvmFirmware START (0x%06x bytes to write):\n"
///
/// UpdateDiscreteTbtNvmFirmware - Discrete TBT NVM firmware update and authentication success
///
#define EVT_ARGS_UPDATE_DTBT_FW_AUTH_SUCCESS                 0
#define EVT_CODE_UPDATE_DTBT_FW_AUTH_SUCCESS                 ((EVT_ID_UPDATE_DTBT_FW_AUTH_SUCCESS << 2) | (EVT_ARGS_UPDATE_DTBT_FW_AUTH_SUCCESS))
#define STR_ID_UPDATE_DTBT_FW_AUTH_SUCCESS                   "UpdateDiscreteTbtNvmFirmware: Discrete TBT NVM firmware update and authentication success\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Controller GET MODE Not Responding. Returning with Status = %r
///
#define EVT_ARGS_TBT_PD_GET_MODE_FAIL                        1
#define EVT_CODE_TBT_PD_GET_MODE_FAIL                        ((EVT_ID_TBT_PD_GET_MODE_FAIL << 2) | (EVT_ARGS_TBT_PD_GET_MODE_FAIL))
#define STR_ID_TBT_PD_GET_MODE_FAIL                          "PD Controller GET MODE Not Responding. Returning with Status = %r\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Drive: First Get PD mode PD Status 0x%x
///
#define EVT_ARGS_TBT_PD_DRIVE_FIRST_GET_MODE                 1
#define EVT_CODE_TBT_PD_DRIVE_FIRST_GET_MODE                 ((EVT_ID_TBT_PD_DRIVE_FIRST_GET_MODE << 2) | (EVT_ARGS_TBT_PD_DRIVE_FIRST_GET_MODE))
#define STR_ID_TBT_PD_DRIVE_FIRST_GET_MODE                   "PD Drive: First Get PD mode PD Status 0x%x\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Restore: First Get PD mode PD Status 0x%x
///
#define EVT_ARGS_TBT_PD_RESTORE_PD_FIRST_GET_MODE            1
#define EVT_CODE_TBT_PD_RESTORE_PD_FIRST_GET_MODE            ((EVT_ID_TBT_PD_RESTORE_PD_FIRST_GET_MODE << 2) | (EVT_ARGS_TBT_PD_RESTORE_PD_FIRST_GET_MODE))
#define STR_ID_TBT_PD_RESTORE_PD_FIRST_GET_MODE              "PD Restore: First Get PD mode PD Status 0x%x\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Drive: Second Get PD mode PD Status 0x%x
///
#define EVT_ARGS_TBT_PD_DRIVE_SECOND_GET_MODE                1
#define EVT_CODE_TBT_PD_DRIVE_SECOND_GET_MODE                ((EVT_ID_TBT_PD_DRIVE_SECOND_GET_MODE << 2) | (EVT_ARGS_TBT_PD_DRIVE_SECOND_GET_MODE))
#define STR_ID_TBT_PD_DRIVE_SECOND_GET_MODE                  "PD Drive: Second Get PD mode PD Status 0x%x\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Restore: Second Get PD mode PD Status 0x%x
///
#define EVT_ARGS_TBT_PD_RESTORE_PD_SECOND_GET_MODE           1
#define EVT_CODE_TBT_PD_RESTORE_PD_SECOND_GET_MODE           ((EVT_ID_TBT_PD_RESTORE_PD_SECOND_GET_MODE << 2) | (EVT_ARGS_TBT_PD_RESTORE_PD_SECOND_GET_MODE))
#define STR_ID_TBT_PD_RESTORE_PD_SECOND_GET_MODE             "PD Restore: Second Get PD mode PD Status 0x%x\n"
///
/// TbtPdRetimerFwUpdateModeChange - TbtPdRetimerFwUpdateMode.Enable: PD Controller is already in FW Update Mode
///
#define EVT_ARGS_TBT_PD_CONTROLLER_ALREADY                   0
#define EVT_CODE_TBT_PD_CONTROLLER_ALREADY                   ((EVT_ID_TBT_PD_CONTROLLER_ALREADY << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_ALREADY))
#define STR_ID_TBT_PD_CONTROLLER_ALREADY                     "\nTbtPdRetimerFwUpdateMode.Enable: PD Controller is already in FW Update Mode.\n"
///
/// TbtPdRetimerFwUpdateModeChange - TbtPdRetimerFwUpdateMode.Disable: PD Controller is already exited FW Update Mode
///
#define EVT_ARGS_TBT_PD_CONTROLLER_EXITED                    0
#define EVT_CODE_TBT_PD_CONTROLLER_EXITED                    ((EVT_ID_TBT_PD_CONTROLLER_EXITED << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_EXITED))
#define STR_ID_TBT_PD_CONTROLLER_EXITED                      "\nTbtPdRetimerFwUpdateMode.Disable: PD Controller is already exited FW Update Mode.\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Controller SET MODE Not Responding. Returning with Status = %r
///
#define EVT_ARGS_TBT_PD_SET_MODE_FAIL                        1
#define EVT_CODE_TBT_PD_SET_MODE_FAIL                        ((EVT_ID_TBT_PD_SET_MODE_FAIL << 2) | (EVT_ARGS_TBT_PD_SET_MODE_FAIL))
#define STR_ID_TBT_PD_SET_MODE_FAIL                          "PD Controller SET MODE Not Responding. Returning with Status = %r\n"
///
/// TbtPdRetimerFwUpdateModeChange - PD Controller SET MODE is not Completed. Returning with Status = %r
///
#define EVT_ARGS_TBT_PD_CONTROLLER_SET_MODE_INCOMPLETE       1
#define EVT_CODE_TBT_PD_CONTROLLER_SET_MODE_INCOMPLETE       ((EVT_ID_TBT_PD_CONTROLLER_SET_MODE_INCOMPLETE << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_SET_MODE_INCOMPLETE))
#define STR_ID_TBT_PD_CONTROLLER_SET_MODE_INCOMPLETE         "PD Controller SET MODE is not Completed. Returning with Status = %r\n"
///
/// TbtPdRetimerFwUpdateModeChange - TbtPdRetimerFwUpdateMode.Drive:
///
#define EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_STATUS        0
#define EVT_CODE_TBT_PD_CONTROLLER_UPDATE_MODE_STATUS        ((EVT_ID_TBT_PD_CONTROLLER_UPDATE_MODE_STATUS << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_STATUS))
#define STR_ID_TBT_PD_CONTROLLER_UPDATE_MODE_STATUS          "\nTbtPdRetimerFwUpdateMode.Drive: "
///
/// TbtPdRetimerFwUpdateModeChange - PD Controller Enter Into FW Update Mode with Status = %r
///
#define EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_ENTER         1
#define EVT_CODE_TBT_PD_CONTROLLER_UPDATE_MODE_ENTER         ((EVT_ID_TBT_PD_CONTROLLER_UPDATE_MODE_ENTER << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_ENTER))
#define STR_ID_TBT_PD_CONTROLLER_UPDATE_MODE_ENTER           "PD Controller Enter Into FW Update Mode with Status = %r\n"
///
/// TbtPdRetimerFwUpdateModeChange - TbtPdRetimerFwUpdateMode.Restore:
///
#define EVT_ARGS_TBT_PD_RESTORE_CONTROLLER_MODE_STATUS       0
#define EVT_CODE_TBT_PD_RESTORE_CONTROLLER_MODE_STATUS       ((EVT_ID_TBT_PD_RESTORE_CONTROLLER_MODE_STATUS << 2) | (EVT_ARGS_TBT_PD_RESTORE_CONTROLLER_MODE_STATUS))
#define STR_ID_TBT_PD_RESTORE_CONTROLLER_MODE_STATUS         "\nTbtPdRetimerFwUpdateMode.Restore: "
///
/// TbtPdRetimerFwUpdateModeChange - PD Controller Exit from FW Update Mode with Status = %r
///
#define EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_EXIT          1
#define EVT_CODE_TBT_PD_CONTROLLER_UPDATE_MODE_EXIT          ((EVT_ID_TBT_PD_CONTROLLER_UPDATE_MODE_EXIT << 2) | (EVT_ARGS_TBT_PD_CONTROLLER_UPDATE_MODE_EXIT))
#define STR_ID_TBT_PD_CONTROLLER_UPDATE_MODE_EXIT            "PD Controller Exit from FW Update Mode with Status = %r\n"
///
/// DriveToFwUpdateMode - PD drive mode start
///
#define EVT_ARGS_DRIVE_START                                 0
#define EVT_CODE_DRIVE_START                                 ((EVT_ID_DRIVE_START << 2) | (EVT_ARGS_DRIVE_START))
#define STR_ID_DRIVE_START                                   "\nDriveToFwUpdateMode - Start\n"
///
/// DriveToFwUpdateMode - Invalid Retimer Guid Passed for UsbCRetimer
///
#define EVT_ARGS_DRIVE_GUID_CHECK_FAILED                     1
#define EVT_CODE_DRIVE_GUID_CHECK_FAILED                     ((EVT_ID_DRIVE_GUID_CHECK_FAILED << 2) | (EVT_ARGS_DRIVE_GUID_CHECK_FAILED))
#define STR_ID_DRIVE_GUID_CHECK_FAILED                       "\nDriveToFwUpdateMode: Invalid Retimer Guid Passed for UsbCRetimer. Returning with Status = %r\n"
///
/// DriveToFwUpdateMode - Usb Type-C Retimer is not supported
///
#define EVT_ARGS_DRIVE_USBC_RT_UNSUPPORT                     1
#define EVT_CODE_DRIVE_USBC_RT_UNSUPPORT                     ((EVT_ID_DRIVE_USBC_RT_UNSUPPORT << 2) | (EVT_ARGS_DRIVE_USBC_RT_UNSUPPORT))
#define STR_ID_DRIVE_USBC_RT_UNSUPPORT                       "\nDriveToFwUpdateMode: Usb Type-C Retimer is not supported. Returning with Status = %r\n"
///
/// DriveToFwUpdateMode - Failed to Enable Retimer FW Update Mode with Status = %r
///
#define EVT_ARGS_DRIVE_CHANGE_MODE_FAIL                      1
#define EVT_CODE_DRIVE_CHANGE_MODE_FAIL                      ((EVT_ID_DRIVE_CHANGE_MODE_FAIL << 2) | (EVT_ARGS_DRIVE_CHANGE_MODE_FAIL))
#define STR_ID_DRIVE_CHANGE_MODE_FAIL                        "\nDriveToFwUpdateMode: Failed to Enable Retimer FW Update Mode with Status = %r\n"
///
/// DriveToFwUpdateMode - PD Drive: PD drive mode end
///
#define EVT_ARGS_DRIVE_END                                   0
#define EVT_CODE_DRIVE_END                                   ((EVT_ID_DRIVE_END << 2) | (EVT_ARGS_DRIVE_END))
#define STR_ID_DRIVE_END                                     "\nDriveToFwUpdateMode : End\n"
///
/// RestoreToOriginalMode - PD restore mode start
///
#define EVT_ARGS_RESTORE_START                               0
#define EVT_CODE_RESTORE_START                               ((EVT_ID_RESTORE_START << 2) | (EVT_ARGS_RESTORE_START))
#define STR_ID_RESTORE_START                                 "\nRestoreToOriginalMode - Start\n"
///
/// RestoreToOriginalMode - Invalid Retimer Guid Passed for UsbCRetimer
///
#define EVT_ARGS_RESTORE_GUID_CHECK_FAILED                   1
#define EVT_CODE_RESTORE_GUID_CHECK_FAILED                   ((EVT_ID_RESTORE_GUID_CHECK_FAILED << 2) | (EVT_ARGS_RESTORE_GUID_CHECK_FAILED))
#define STR_ID_RESTORE_GUID_CHECK_FAILED                     "\nRestoreToOriginalMode: Invalid Retimer Guid Passed for UsbCRetimer. Returning with Status = %r\n"
///
/// RestoreToOriginalMode - Usb Type-C Retimer is not supported.
///
#define EVT_ARGS_RESTORE_USBC_RT_UNSUPPORT                   1
#define EVT_CODE_RESTORE_USBC_RT_UNSUPPORT                   ((EVT_ID_RESTORE_USBC_RT_UNSUPPORT << 2) | (EVT_ARGS_RESTORE_USBC_RT_UNSUPPORT))
#define STR_ID_RESTORE_USBC_RT_UNSUPPORT                     "\nRestoreToOriginalMode: Usb Type-C Retimer is not supported. Returning with Status = %r\n"
///
/// RestoreToOriginalMode - Failed to Exit Retimer FW Update Mode For PD Controller with Status = %r
///
#define EVT_ARGS_RESTORE_CHANGE_MODE_FAIL                    1
#define EVT_CODE_RESTORE_CHANGE_MODE_FAIL                    ((EVT_ID_RESTORE_CHANGE_MODE_FAIL << 2) | (EVT_ARGS_RESTORE_CHANGE_MODE_FAIL))
#define STR_ID_RESTORE_CHANGE_MODE_FAIL                      "\nRestoreToOriginalMode: Failed to Exit Retimer FW Update Mode For PD Controller with Status = %r\n"
///
/// RestoreToOriginalMode - PD Restore: PD restore mode end
///
#define EVT_ARGS_RESTORE_END                                 0
#define EVT_CODE_RESTORE_END                                 ((EVT_ID_RESTORE_END << 2) | (EVT_ARGS_RESTORE_END))
#define STR_ID_RESTORE_END                                   "\nRestoreToOriginalMode - End\n"
///< TbtRetimerNvmUpdateLib.c - end


///< TbtNvmDrvYflRouter.c - start
///
/// TbtNvmDrvYflForcePwrFunc - Performing force power on TBT Controller
///
#define EVT_ARGS_YFL_FORCE_PWR_PERF                          0
#define EVT_CODE_YFL_FORCE_PWR_PERF                          ((EVT_ID_YFL_FORCE_PWR_PERF << 2) | (EVT_ARGS_YFL_FORCE_PWR_PERF))
#define STR_ID_YFL_FORCE_PWR_PERF                            "TbtNvmDrvYflForcePwrFunc: Performing force power on TBT Controller.\n"
///
/// TbtNvmDrvYflForcePwrFunc - Disabling force power on TBT Controller
///
#define EVT_ARGS_YFL_FORCE_PWR_DISABLE                       0
#define EVT_CODE_YFL_FORCE_PWR_DISABLE                       ((EVT_ID_YFL_FORCE_PWR_DISABLE << 2) | (EVT_ARGS_YFL_FORCE_PWR_DISABLE))
#define STR_ID_YFL_FORCE_PWR_DISABLE                         "TbtNvmDrvYflForcePwrFunc: Disabling force power on TBT Controller.\n"
///
/// TbtNvmDrvYflForcePwrFunc - PciIoProto parameter is NULL
///
#define EVT_ARGS_YFL_FORCE_PWR_PCI_IO_NULL                   0
#define EVT_CODE_YFL_FORCE_PWR_PCI_IO_NULL                   ((EVT_ID_YFL_FORCE_PWR_PCI_IO_NULL << 2) | (EVT_ARGS_YFL_FORCE_PWR_PCI_IO_NULL))
#define STR_ID_YFL_FORCE_PWR_PCI_IO_NULL                     "TbtNvmDrvYflForcePwrFunc: PciIoProto parameter is NULL.\n"
///
/// TbtNvmDrvYflForcePwrFunc - TBTControllerWasPowered parameter is NULL
///
#define EVT_ARGS_YFL_FORCE_PWR_PARAMETER_NULL                0
#define EVT_CODE_YFL_FORCE_PWR_PARAMETER_NULL                ((EVT_ID_YFL_FORCE_PWR_PARAMETER_NULL << 2) | (EVT_ARGS_YFL_FORCE_PWR_PARAMETER_NULL))
#define STR_ID_YFL_FORCE_PWR_PARAMETER_NULL                  "TbtNvmDrvYflForcePwrFunc: TBTControllerWasPowered parameter is NULL.\n"
///
/// TbtNvmDrvYflForcePwrFunc - Couldn't read from PCIe register
///
#define EVT_ARGS_YFL_FORCE_PWR_READ_PCIE_REG_ERROR           2
#define EVT_CODE_YFL_FORCE_PWR_READ_PCIE_REG_ERROR           ((EVT_ID_YFL_FORCE_PWR_READ_PCIE_REG_ERROR << 2) | (EVT_ARGS_YFL_FORCE_PWR_READ_PCIE_REG_ERROR))
#define STR_ID_YFL_FORCE_PWR_READ_PCIE_REG_ERROR             "TbtNvmDrvYflForcePwrFunc: Couldn't read from PCIe register - 0x%x, Status - %r\n"
///
/// TbtNvmDrvYflForcePwrFunc - FW of TBT controller is ready
///
#define EVT_ARGS_YFL_TBT_CONTROLLER_READY                    0
#define EVT_CODE_YFL_TBT_CONTROLLER_READY                    ((EVT_ID_YFL_TBT_CONTROLLER_READY << 2) | (EVT_ARGS_YFL_TBT_CONTROLLER_READY))
#define STR_ID_YFL_TBT_CONTROLLER_READY                      "FW of TBT controller is ready.\n"
///
/// TbtNvmDrvYflForcePwrFunc - FW of TBT controller is not yet ready
///
#define EVT_ARGS_YFL_TBT_CONTROLLER_NOT_READY                0
#define EVT_CODE_YFL_TBT_CONTROLLER_NOT_READY                ((EVT_ID_YFL_TBT_CONTROLLER_NOT_READY << 2) | (EVT_ARGS_YFL_TBT_CONTROLLER_NOT_READY))
#define STR_ID_YFL_TBT_CONTROLLER_NOT_READY                  "FW of TBT controller is not yet ready.\n"
///
/// TbtNvmDrvYflForcePwrFunc - Waiting too much on power on
///
#define EVT_ARGS_YFL_FORCE_PWR_WAIT_ERROR                    0
#define EVT_CODE_YFL_FORCE_PWR_WAIT_ERROR                    ((EVT_ID_YFL_FORCE_PWR_WAIT_ERROR << 2) | (EVT_ARGS_YFL_FORCE_PWR_WAIT_ERROR))
#define STR_ID_YFL_FORCE_PWR_WAIT_ERROR                      "TbtNvmDrvYflForcePwrFunc: Waiting too much on power on. Exiting...\n"
///< TbtNvmDrvYflRouter.c - end


///< TbtNvmDrvRetimerThruHr.c - start
///
/// WriteIecs - WriteIecs Length 0x%x
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_LENGTH                    1
#define EVT_CODE_TBTCMD_WRITE_IECS_LENGTH                    ((EVT_ID_TBTCMD_WRITE_IECS_LENGTH << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_LENGTH))
#define STR_ID_TBTCMD_WRITE_IECS_LENGTH                      "WriteIecs Length 0x%x\n"
///
/// WriteIecs - RetimerThruHr::WriteIecs: IECS reg num - 0x%x, Length - 0x%x
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_REG_NUM_LENGTH            2
#define EVT_CODE_TBTCMD_WRITE_IECS_REG_NUM_LENGTH            ((EVT_ID_TBTCMD_WRITE_IECS_REG_NUM_LENGTH << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_REG_NUM_LENGTH))
#define STR_ID_TBTCMD_WRITE_IECS_REG_NUM_LENGTH              "RetimerThruHr::WriteIecs: IECS reg num - 0x%x, Length - 0x%x\n"
///
/// WriteIecs - WriteIecs: ERROR when writing the data
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_DATA_ERROR                1
#define EVT_CODE_TBTCMD_WRITE_IECS_DATA_ERROR                ((EVT_ID_TBTCMD_WRITE_IECS_DATA_ERROR << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_DATA_ERROR))
#define STR_ID_TBTCMD_WRITE_IECS_DATA_ERROR                  "TbtNvmDrvRetimerThruHr::WriteIecs: ERROR when writing the data, Status - %d. Exiting...\n"
///
/// WriteIecs - TbtNvmDrvRetimerThruHrWriteIecs: Writing USB 4.0 reg, USB4CapRegWrite = 0x%08x
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_USB4_REG                  1
#define EVT_CODE_TBTCMD_WRITE_IECS_USB4_REG                  ((EVT_ID_TBTCMD_WRITE_IECS_USB4_REG << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_USB4_REG))
#define STR_ID_TBTCMD_WRITE_IECS_USB4_REG                    "TbtNvmDrvRetimerThruHrWriteIecs: Writing USB 4.0 reg, USB4CapRegWrite = 0x%08x\n"
///
/// WriteIecs - RetimerThruHr::WriteIecs WaitForMsgOutTxDone::failed:
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_FAIL                      0
#define EVT_CODE_TBTCMD_WRITE_IECS_FAIL                      ((EVT_ID_TBTCMD_WRITE_IECS_FAIL << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_FAIL))
#define STR_ID_TBTCMD_WRITE_IECS_FAIL                        "RetimerThruHr::WriteIecs WaitForMsgOutTxDone::failed: "
///
/// WriteIecs - RegNum=%x, Data[0]=%x
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_REGNUM_DATA               2
#define EVT_CODE_TBTCMD_WRITE_IECS_REGNUM_DATA               ((EVT_ID_TBTCMD_WRITE_IECS_REGNUM_DATA << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_REGNUM_DATA))
#define STR_ID_TBTCMD_WRITE_IECS_REGNUM_DATA                 "RegNum=%x, Data[0]=%x, "
///
/// WriteIecs - Length=%d, Status - %d
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_LENGTH_STATUS             2
#define EVT_CODE_TBTCMD_WRITE_IECS_LENGTH_STATUS             ((EVT_ID_TBTCMD_WRITE_IECS_LENGTH_STATUS << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_LENGTH_STATUS))
#define STR_ID_TBTCMD_WRITE_IECS_LENGTH_STATUS               "Length=%d, TbtStatus - %d. Exiting...\n"
///
/// WriteIecs - RetimerThruHr::WriteIecs TBT_STATUS_RETRY WaitForMsgOutTxDone::failed:
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_RETRY_FAIL                0
#define EVT_CODE_TBTCMD_WRITE_IECS_RETRY_FAIL                ((EVT_ID_TBTCMD_WRITE_IECS_RETRY_FAIL << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_RETRY_FAIL))
#define STR_ID_TBTCMD_WRITE_IECS_RETRY_FAIL                  "RetimerThruHr::WriteIecs TBT_STATUS_RETRY WaitForMsgOutTxDone::failed: \n"
///
/// WriteIecs - RetimerThruHr::WriteIecs: RetryCnt=%d
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_RETRY_REG                 1
#define EVT_CODE_TBTCMD_WRITE_IECS_RETRY_REG                 ((EVT_ID_TBTCMD_WRITE_IECS_RETRY_REG << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_RETRY_REG))
#define STR_ID_TBTCMD_WRITE_IECS_RETRY_REG                   "RetimerThruHr::WriteIecs: RetryCnt=%d, "
///
/// WriteIecs - Offset=%x, Cmd=%x
///
#define EVT_ARGS_TBTCMD_WRITE_IECS_OFFSET_CMD                2
#define EVT_CODE_TBTCMD_WRITE_IECS_OFFSET_CMD                ((EVT_ID_TBTCMD_WRITE_IECS_OFFSET_CMD << 2) | (EVT_ARGS_TBTCMD_WRITE_IECS_OFFSET_CMD))
#define STR_ID_TBTCMD_WRITE_IECS_OFFSET_CMD                  "Off=%x, Cmd=%x\n"
///
/// ReadIecs - IECS reg num - 0x%x
///
#define EVT_ARGS_TBTCMD_READ_IECS_REG                        1
#define EVT_CODE_TBTCMD_READ_IECS_REG                        ((EVT_ID_TBTCMD_READ_IECS_REG << 2) | (EVT_ARGS_TBTCMD_READ_IECS_REG))
#define STR_ID_TBTCMD_READ_IECS_REG                          "RetimerThruHr::ReadIecs: IECS reg num - 0x%x\n"
///
/// ReadIecs - ERROR when reading from IECS rdata
///
#define EVT_ARGS_TBTCMD_READ_IECS_DATA_ERROR                 1
#define EVT_CODE_TBTCMD_READ_IECS_DATA_ERROR                 ((EVT_ID_TBTCMD_READ_IECS_DATA_ERROR << 2) | (EVT_ARGS_TBTCMD_READ_IECS_DATA_ERROR))
#define STR_ID_TBTCMD_READ_IECS_DATA_ERROR                   "RetimerThruHr::ReadIecs: ERROR when reading from IECS rdata, TbtStatus - %d. Exiting...\n"
///
/// ReadIecs - RetimerThruHr::ReadIecs: RetryCnt=%d
///
#define EVT_ARGS_TBTCMD_READ_IECS_RETRYCNT                   1
#define EVT_CODE_TBTCMD_READ_IECS_RETRYCNT                   ((EVT_ID_TBTCMD_READ_IECS_RETRYCNT << 2) | (EVT_ARGS_TBTCMD_READ_IECS_RETRYCNT))
#define STR_ID_TBTCMD_READ_IECS_RETRYCNT                     "RetimerThruHr::ReadIecs: RetryCnt=%d, "
///
/// ReadIecs - Offset=%x, RegNum=%x
///
#define EVT_ARGS_TBTCMD_READ_IECS_OFFSET_REG                 2
#define EVT_CODE_TBTCMD_READ_IECS_OFFSET_REG                 ((EVT_ID_TBTCMD_READ_IECS_OFFSET_REG << 2) | (EVT_ARGS_TBTCMD_READ_IECS_OFFSET_REG))
#define STR_ID_TBTCMD_READ_IECS_OFFSET_REG                   "Offset=%x, RegNum=%x\n"
///
/// ReadIecs - Read IECS operation failed after several retries
///
#define EVT_ARGS_TBTCMD_READ_IECS_OPERATION_FAIL             1
#define EVT_CODE_TBTCMD_READ_IECS_OPERATION_FAIL             ((EVT_ID_TBTCMD_READ_IECS_OPERATION_FAIL << 2) | (EVT_ARGS_TBTCMD_READ_IECS_OPERATION_FAIL))
#define STR_ID_TBTCMD_READ_IECS_OPERATION_FAIL               "RetimerThruHr::ReadIecs: Read IECS operation failed after several retries, TbtStatus - %d. Exiting...\n"
///
/// CapabilityParsing - RetimerThruHr::CapabilityParsing
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING                    0
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING                    ((EVT_ID_TBTCMD_CAPABILITY_PARSING << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING))
#define STR_ID_TBTCMD_CAPABILITY_PARSING                      "RetimerThruHr::CapabilityParsing.\n"
///
/// CapabilityParsing - RetimerThruHr::CapabilityParsing, NULL device provided
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING_DEV_NULL           0
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING_DEV_NULL           ((EVT_ID_TBTCMD_CAPABILITY_PARSING_DEV_NULL << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING_DEV_NULL))
#define STR_ID_TBTCMD_CAPABILITY_PARSING_DEV_NULL             "RetimerThruHr::CapabilityParsing, NULL device provided.\n"
///
/// CapabilityParsing - Data read 0x%08x
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_DATA          1
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_DATA          ((EVT_ID_TBTCMD_CAPABILITY_PARSING_READ_DATA << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_DATA))
#define STR_ID_TBTCMD_CAPABILITY_PARSING_READ_DATA            "RetimerThruHr::CapabilityParsing: Data read 0x%08x.\n"
///
/// CapabilityParsing - ERROR when reading from Adapter config space, Status - %d
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_ERROR         1
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_ERROR         ((EVT_ID_TBTCMD_CAPABILITY_PARSING_READ_ERROR << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_ERROR))
#define STR_ID_TBTCMD_CAPABILITY_PARSING_READ_ERROR           "RetimerThruHr::CapabilityParsing: ERROR when reading from Adapter config space, TbtStatus - %d. Exiting...\n"
///
/// CapabilityParsing - Read NextCapability = %x
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_NEXT          1
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_NEXT          ((EVT_ID_TBTCMD_CAPABILITY_PARSING_READ_NEXT << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING_READ_NEXT))
#define STR_ID_TBTCMD_CAPABILITY_PARSING_READ_NEXT            "RetimerThruHr::CapabilityParsing: Read NextCapability = %x\n"
///
/// CapabilityParsing - ERROR, USB4 Port Capability was not found
///
#define EVT_ARGS_TBTCMD_CAPABILITY_USB4_FOUND_ERROR          0
#define EVT_CODE_TBTCMD_CAPABILITY_USB4_FOUND_ERROR          ((EVT_ID_TBTCMD_CAPABILITY_USB4_FOUND_ERROR << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_USB4_FOUND_ERROR))
#define STR_ID_TBTCMD_CAPABILITY_USB4_FOUND_ERROR            "RetimerThruHr::CapabilityParsing: ERROR, USB4 Port Capability was not found.\n"
///
/// CapabilityParsing - Capability ID = 0x%x
///
#define EVT_ARGS_TBTCMD_CAPABILITY_ID                        1
#define EVT_CODE_TBTCMD_CAPABILITY_ID                        ((EVT_ID_TBTCMD_CAPABILITY_ID << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_ID))
#define STR_ID_TBTCMD_CAPABILITY_ID                          "RetimerThruHr::CapabilityParsing: Capability ID = 0x%x.\n"
///
/// CapabilityParsing - USB4 Port Capability found at offset 0x%x
///
#define EVT_ARGS_TBTCMD_CAPABILITY_USB4_OFFSET               1
#define EVT_CODE_TBTCMD_CAPABILITY_USB4_OFFSET               ((EVT_ID_TBTCMD_CAPABILITY_USB4_OFFSET << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_USB4_OFFSET))
#define STR_ID_TBTCMD_CAPABILITY_USB4_OFFSET                 "RetimerThruHr::CapabilityParsing: USB4 Port Capability found at offset 0x%x.\n"
///
/// CapabilityParsing - failed to parse capabilities.
///
#define EVT_ARGS_TBTCMD_CAPABILITY_PARSING_FAIL               0
#define EVT_CODE_TBTCMD_CAPABILITY_PARSING_FAIL               ((EVT_ID_TBTCMD_CAPABILITY_PARSING_FAIL << 2) | (EVT_ARGS_TBTCMD_CAPABILITY_PARSING_FAIL))
#define STR_ID_TBTCMD_CAPABILITY_PARSING_FAIL                 "RetimerThruHr::CapabilityParsing, failed to parse capabilities.\n"
///
/// InitHW - Retimer: Send ENUM command
///
#define EVT_ARGS_TBTCMD_SEND_ENUM_CMD                        0
#define EVT_CODE_TBTCMD_SEND_ENUM_CMD                        ((EVT_ID_TBTCMD_SEND_ENUM_CMD << 2) | (EVT_ARGS_TBTCMD_SEND_ENUM_CMD))
#define STR_ID_TBTCMD_SEND_ENUM_CMD                          "InitRetimerHW: Sending ENUM cmd.\n"
///
/// InitHW - Retimer: Send ENUM command failed
///
#define EVT_ARGS_TBTCMD_SEND_ENUM_CMD_FAIL                   1
#define EVT_CODE_TBTCMD_SEND_ENUM_CMD_FAIL                   ((EVT_ID_TBTCMD_SEND_ENUM_CMD_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_ENUM_CMD_FAIL))
#define STR_ID_TBTCMD_SEND_ENUM_CMD_FAIL                     "InitRetimerHW: The Retimer could not perform ENUM, Status - %d.\n"
///
/// InitHW - Retimer: Send LSUP command
///
#define EVT_ARGS_TBTCMD_SEND_LSUP_CMD                        0
#define EVT_CODE_TBTCMD_SEND_LSUP_CMD                        ((EVT_ID_TBTCMD_SEND_LSUP_CMD << 2) | (EVT_ARGS_TBTCMD_SEND_LSUP_CMD))
#define STR_ID_TBTCMD_SEND_LSUP_CMD                          "InitRetimerHW: Sending LSUP cmd.\n"
///
/// InitHW - Retimer: Send LSUP command failed
///
#define EVT_ARGS_TBTCMD_SEND_LSUP_CMD_FAIL                   1
#define EVT_CODE_TBTCMD_SEND_LSUP_CMD_FAIL                   ((EVT_ID_TBTCMD_SEND_LSUP_CMD_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_LSUP_CMD_FAIL))
#define STR_ID_TBTCMD_SEND_LSUP_CMD_FAIL                     "InitRetimerHW: The Retimer could not perform LSUP, Status %d.\n"
///
/// TerminateHW - Retimer: Send USUP command
///
#define EVT_ARGS_TBTCMD_SEND_USUP_CMD                        0
#define EVT_CODE_TBTCMD_SEND_USUP_CMD                        ((EVT_ID_TBTCMD_SEND_USUP_CMD << 2) | (EVT_ARGS_TBTCMD_SEND_USUP_CMD))
#define STR_ID_TBTCMD_SEND_USUP_CMD                          "TbtNvmDrvRetimerThruHr::Sending USUP cmd.\n"
///
/// TerminateHW - Retimer: Send USUP command failed
///
#define EVT_ARGS_TBTCMD_SEND_USUP_CMD_FAIL                   0
#define EVT_CODE_TBTCMD_SEND_USUP_CMD_FAIL                   ((EVT_ID_TBTCMD_SEND_USUP_CMD_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_USUP_CMD_FAIL))
#define STR_ID_TBTCMD_SEND_USUP_CMD_FAIL                     "TbtNvmDrvRetimerThruHr::Dtor Failed to send USUP to retimer.\n"
///
/// Dtor - Dtor was called
///
#define EVT_ARGS_TBTCMD_DTOR_CALL                            0
#define EVT_CODE_TBTCMD_DTOR_CALL                            ((EVT_ID_TBTCMD_DTOR_CALL << 2) | (EVT_ARGS_TBTCMD_DTOR_CALL))
#define STR_ID_TBTCMD_DTOR_CALL                              "TbtNvmDrvRetimerThruHr::Dtor was called.\n"
///
/// TbtNvmDrvRetimerThruHrCtor - Failed to allocate memory for RETIMER_THRU_HR
///
#define EVT_ARGS_TBTCMD_THRU_ALLOCATE_MEM_FAIL               0
#define EVT_CODE_TBTCMD_THRU_ALLOCATE_MEM_FAIL               ((EVT_ID_TBTCMD_THRU_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_TBTCMD_THRU_ALLOCATE_MEM_FAIL))
#define STR_ID_TBTCMD_THRU_ALLOCATE_MEM_FAIL                 "TbtNvmDrvRetimerThruHrCtor: Failed to allocate memory for RETIMER_THRU_HR.\n"
///
/// TbtNvmDrvRetimerThruHrCtor - Failed to allocate memory for DEVICE_COMMUNICATION_INTERFACE
///
#define EVT_ARGS_TBTCMD_DEV_ALLOCATE_MEM_FAIL                0
#define EVT_CODE_TBTCMD_DEV_ALLOCATE_MEM_FAIL                ((EVT_ID_TBTCMD_DEV_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_TBTCMD_DEV_ALLOCATE_MEM_FAIL))
#define STR_ID_TBTCMD_DEV_ALLOCATE_MEM_FAIL                  "TbtNvmDrvRetimerThruHrCtor: Failed to allocate memory for DEVICE_COMMUNICATION_INTERFACE.\n"
///
/// TbtNvmDrvRetimerThruHrCtor - Failed to create DMA
///
#define EVT_ARGS_TBTCMD_CREATE_DMA_FAIL                      0
#define EVT_CODE_TBTCMD_CREATE_DMA_FAIL                      ((EVT_ID_TBTCMD_CREATE_DMA_FAIL << 2) | (EVT_ARGS_TBTCMD_CREATE_DMA_FAIL))
#define STR_ID_TBTCMD_CREATE_DMA_FAIL                        "TbtNvmDrvRetimerThruHrCtor: Failed to create DMA.\n"
///
/// TbtNvmDrvRetimerThruHrCtor - The retimer could not perform Capability Parsing, Status %d. Exiting...
///
#define EVT_ARGS_TBTCMD_RT_CAPABILITY_PARSING_FAIL           1
#define EVT_CODE_TBTCMD_RT_CAPABILITY_PARSING_FAIL           ((EVT_ID_TBTCMD_RT_CAPABILITY_PARSING_FAIL << 2) | (EVT_ARGS_TBTCMD_RT_CAPABILITY_PARSING_FAIL))
#define STR_ID_TBTCMD_RT_CAPABILITY_PARSING_FAIL             "TbtNvmDrvRetimerThruHrCtor: The retimer could not perform Capability Parsing, Status %d. Exiting...\n"
///< TbtNvmDrvRetimerThruHr.c - end


///< TbtNvmDrvRetimerThruHrHelpers.c - start
///
/// WaitForMsgOutTxDone - Read data = 0x%x
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_READ_DATA                   1
#define EVT_CODE_TBTCMD_WAIT_CMD_READ_DATA                   ((EVT_ID_TBTCMD_WAIT_CMD_READ_DATA << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_READ_DATA))
#define STR_ID_TBTCMD_WAIT_CMD_READ_DATA                     "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: Read data = 0x%x\n"
///
/// WaitForMsgOutTxDone - ERROR! Reading register MSG_OUT_CMD is failed
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_READ_REG_FAIL               2
#define EVT_CODE_TBTCMD_WAIT_CMD_READ_REG_FAIL               ((EVT_ID_TBTCMD_WAIT_CMD_READ_REG_FAIL << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_READ_REG_FAIL))
#define STR_ID_TBTCMD_WAIT_CMD_READ_REG_FAIL                 "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: ERROR! Reading register MSG_OUT_CMD is failed. TbtStatus is %d. d=%d Exiting...\n"
///
/// WaitForMsgOutTxDone - IECS transaction read data=%x
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_READ_TRAN_DATA              1
#define EVT_CODE_TBTCMD_WAIT_CMD_READ_TRAN_DATA              ((EVT_ID_TBTCMD_WAIT_CMD_READ_TRAN_DATA << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_READ_TRAN_DATA))
#define STR_ID_TBTCMD_WAIT_CMD_READ_TRAN_DATA                "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction read data=%x\n"
///
/// WaitForMsgOutTxDone - ERROR! Local LC seems to be stuck
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_LC_STUCK_ERROR              0
#define EVT_CODE_TBTCMD_WAIT_CMD_LC_STUCK_ERROR              ((EVT_ID_TBTCMD_WAIT_CMD_LC_STUCK_ERROR << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_LC_STUCK_ERROR))
#define STR_ID_TBTCMD_WAIT_CMD_LC_STUCK_ERROR                "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: ERROR! Local LC seems to be stuck. "
///
/// WaitForMsgOutTxDone - d=%d, Data=%x
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_LC_DATA                     2
#define EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA                     ((EVT_ID_TBTCMD_WAIT_CMD_LC_DATA << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_LC_DATA))
#define STR_ID_TBTCMD_WAIT_CMD_LC_DATA                       "d=%d, Data=%x "
///
/// WaitForMsgOutTxDone - m=%x, AccessCnt=%d
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_LC_DATA2                    2
#define EVT_CODE_TBTCMD_WAIT_CMD_LC_DATA2                    ((EVT_ID_TBTCMD_WAIT_CMD_LC_DATA2 << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_LC_DATA2))
#define STR_ID_TBTCMD_WAIT_CMD_LC_DATA2                      "m=%x, AccessCnt=%d Exiting...\n"
///
/// WaitForMsgOutTxDone - IECS transaction was timeouted
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_LC_TIMEOUT                  0
#define EVT_CODE_TBTCMD_WAIT_CMD_LC_TIMEOUT                  ((EVT_ID_TBTCMD_WAIT_CMD_LC_TIMEOUT << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_LC_TIMEOUT))
#define STR_ID_TBTCMD_WAIT_CMD_LC_TIMEOUT                    "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction was timeouted.\n"
///
/// WaitForMsgOutTxDone - IECS transaction was invalid
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_LC_INVALID                  0
#define EVT_CODE_TBTCMD_WAIT_CMD_LC_INVALID                  ((EVT_ID_TBTCMD_WAIT_CMD_LC_INVALID << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_LC_INVALID))
#define STR_ID_TBTCMD_WAIT_CMD_LC_INVALID                    "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: IECS transaction was invalid.\n"
///
/// WaitForMsgOutTxDone - TBT_STATUS_RETRY, d=%d
///
#define EVT_ARGS_TBTCMD_WAIT_CMD_RETRY_STATUS                1
#define EVT_CODE_TBTCMD_WAIT_CMD_RETRY_STATUS                ((EVT_ID_TBTCMD_WAIT_CMD_RETRY_STATUS << 2) | (EVT_ARGS_TBTCMD_WAIT_CMD_RETRY_STATUS))
#define STR_ID_TBTCMD_WAIT_CMD_RETRY_STATUS                  "TbtNvmDrvRetimerThruHr::WaitForMsgOutTxDone: TBT_STATUS_RETRY, d=%d\n"
///
/// SendCommandToLocalLc - ERROR when writing 0x%x data to local LC, TbtStatus - %d
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_ERROR                 2
#define EVT_CODE_TBTCMD_LOCAL_LC_WRITE_ERROR                 ((EVT_ID_TBTCMD_LOCAL_LC_WRITE_ERROR << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_ERROR))
#define STR_ID_TBTCMD_LOCAL_LC_WRITE_ERROR                   "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: ERROR when writing 0x%x data to local LC, TbtStatus - %d. Exiting...\n"
///
/// SendCommandToLocalLc - TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: Write USB4 port cap = %x
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_USB4_CAP              1
#define EVT_CODE_TBTCMD_LOCAL_LC_WRITE_USB4_CAP              ((EVT_ID_TBTCMD_LOCAL_LC_WRITE_USB4_CAP << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_USB4_CAP))
#define STR_ID_TBTCMD_LOCAL_LC_WRITE_USB4_CAP                "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: Write USB4 port cap = %x\n"

///
/// SendCommandToLocalLc - ERROR when writing 0x%x cmd to local LC, TbtStatus - %d
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR             2
#define EVT_CODE_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR             ((EVT_ID_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR))
#define STR_ID_TBTCMD_LOCAL_LC_WRITE_CMD_ERROR               "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: ERROR when writing 0x%x cmd to local LC, TbtStatus - %d. Exiting...\n"
///
/// SendCommandToLocalLc - ERROR when waiting for command 0x%x to complete
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR     2
#define EVT_CODE_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR     ((EVT_ID_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR))
#define STR_ID_TBTCMD_LOCAL_LC_WAIT_CMD_COMPLETE_ERROR       "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: ERROR when waiting for command 0x%x to complete, TbtStatus - %d. Exiting...\n"
///
/// SendCommandToLocalLc - Local LC Couldn't perform 0x%x command - timeouted
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_CMD_TIMEOUT                 1
#define EVT_CODE_TBTCMD_LOCAL_LC_CMD_TIMEOUT                 ((EVT_ID_TBTCMD_LOCAL_LC_CMD_TIMEOUT << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_CMD_TIMEOUT))
#define STR_ID_TBTCMD_LOCAL_LC_CMD_TIMEOUT                   "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: Local LC Couldn't perform 0x%x command - timeouted. Exiting...\n"
///
/// SendCommandToLocalLc - Local LC reported error while performing 0x%x command - got error: 0x%x.
///
#define EVT_ARGS_TBTCMD_LOCAL_LC_REPORTED_ERROR              2
#define EVT_CODE_TBTCMD_LOCAL_LC_REPORTED_ERROR              ((EVT_ID_TBTCMD_LOCAL_LC_REPORTED_ERROR << 2) | (EVT_ARGS_TBTCMD_LOCAL_LC_REPORTED_ERROR))
#define STR_ID_TBTCMD_LOCAL_LC_REPORTED_ERROR                "TbtNvmDrvRetimerThruHr::SendCommandToLocalLc: Local LC reported error while performing 0x%x command - got error: 0x%x. Exiting...\n"
///
/// SendEnumCmd - TbtNvmDrvRetimerThruHr::Sending ENUM to adapter %x of the HR
///
#define EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD                   1
#define EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD                   ((EVT_ID_TBTCMD_SEND_ENUM_THRU_CMD << 2) | (EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD))
#define STR_ID_TBTCMD_SEND_ENUM_THRU_CMD                     "\nTbtNvmDrvRetimerThruHr::Sending ENUM to adapter %x of the HR\n"
///
/// SendEnumCmd - Enum wasn't sent successfully
///
#define EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD_FAIL              0
#define EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD_FAIL              ((EVT_ID_TBTCMD_SEND_ENUM_THRU_CMD_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD_FAIL))
#define STR_ID_TBTCMD_SEND_ENUM_THRU_CMD_FAIL                "TbtNvmDrvRetimerThruHr::Enum wasn't sent successfully, got error...\n"
///
/// SendEnumCmd - TbtNvmDrvRetimerThruHr::Enum was sent successfully
///
#define EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS           0
#define EVT_CODE_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS           ((EVT_ID_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS << 2) | (EVT_ARGS_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS))
#define STR_ID_TBTCMD_SEND_ENUM_THRU_CMD_SUCCESS             "TbtNvmDrvRetimerThruHr::Enum was sent successfully!\n"
///
/// SendLsupCmdDis - Sending USUP to adapter %x of the HR
///
#define EVT_ARGS_TBTCMD_SEND_USUP_RETIMER_INDEX              2
#define EVT_CODE_TBTCMD_SEND_USUP_RETIMER_INDEX              ((EVT_ID_TBTCMD_SEND_USUP_RETIMER_INDEX << 2) | (EVT_ARGS_TBTCMD_SEND_USUP_RETIMER_INDEX))
#define STR_ID_TBTCMD_SEND_USUP_RETIMER_INDEX                "\nTbtNvmDrvRetimerThruHr::Sending USUP to adapter %x of the HR, retimer index - %d. The required operation is disable\n"
///
/// SendLsupCmdDis - USUP send is failed
///
#define EVT_ARGS_TBTCMD_SEND_USUP_FAIL                       0
#define EVT_CODE_TBTCMD_SEND_USUP_FAIL                       ((EVT_ID_TBTCMD_SEND_USUP_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_USUP_FAIL))
#define STR_ID_TBTCMD_SEND_USUP_FAIL                         "TbtNvmDrvRetimerThruHr::USUP send is failed!\n"
///
/// SendLsupCmdDis - USUP was sent successfully
///
#define EVT_ARGS_TBTCMD_SEND_USUP_SUCCESS                    0
#define EVT_CODE_TBTCMD_SEND_USUP_SUCCESS                    ((EVT_ID_TBTCMD_SEND_USUP_SUCCESS << 2) | (EVT_ARGS_TBTCMD_SEND_USUP_SUCCESS))
#define STR_ID_TBTCMD_SEND_USUP_SUCCESS                      "TbtNvmDrvRetimerThruHr::USUP was sent successfully!\n"
///
/// SendLsupCmdEn - Sending LSUP to adapter %x of the HR
///
#define EVT_ARGS_TBTCMD_SEND_LSUP_RETIMER_INDEX              2
#define EVT_CODE_TBTCMD_SEND_LSUP_RETIMER_INDEX              ((EVT_ID_TBTCMD_SEND_LSUP_RETIMER_INDEX << 2) | (EVT_ARGS_TBTCMD_SEND_LSUP_RETIMER_INDEX))
#define STR_ID_TBTCMD_SEND_LSUP_RETIMER_INDEX                "\nTbtNvmDrvRetimerThruHr::Sending LSUP to adapter %x of the HR, retimer index - %d. The required operation is enable\n"
///
/// SendLsupCmdEn - LSUP command send had succeeded
///
#define EVT_ARGS_TBTCMD_SEND_LSUP_SUCCESS                    0
#define EVT_CODE_TBTCMD_SEND_LSUP_SUCCESS                    ((EVT_ID_TBTCMD_SEND_LSUP_SUCCESS << 2) | (EVT_ARGS_TBTCMD_SEND_LSUP_SUCCESS))
#define STR_ID_TBTCMD_SEND_LSUP_SUCCESS                      "TbtNvmDrvRetimerThruHr::LSUP command send had succeeded.\n"
///
/// SendLsupCmdEn - LSUP command send had failed
///
#define EVT_ARGS_TBTCMD_SEND_LSUP_FAIL                       0
#define EVT_CODE_TBTCMD_SEND_LSUP_FAIL                       ((EVT_ID_TBTCMD_SEND_LSUP_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_LSUP_FAIL))
#define STR_ID_TBTCMD_SEND_LSUP_FAIL                         "TbtNvmDrvRetimerThruHr::LSUP command send had failed.\n"
///
/// SendOfflineMode - Retimer: Send LSEN command
///
#define EVT_ARGS_TBTCMD_SEND_LSEN_CMD                        2
#define EVT_CODE_TBTCMD_SEND_LSEN_CMD                        ((EVT_ID_TBTCMD_SEND_LSEN_CMD << 2) | (EVT_ARGS_TBTCMD_SEND_LSEN_CMD))
#define STR_ID_TBTCMD_SEND_LSEN_CMD                          "\nTbtNvmDrvRetimerThruHr::Sending OfflineMode(%d) to adapter %x of the HR, "
///
/// SendOfflineMode - Retimer: Send LSEN command - 1
///
#define EVT_ARGS_TBTCMD_SEND_LSEN_CMD1                       1
#define EVT_CODE_TBTCMD_SEND_LSEN_CMD1                       ((EVT_ID_TBTCMD_SEND_LSEN_CMD1  << 2) | (EVT_ARGS_TBTCMD_SEND_LSEN_CMD1))
#define STR_ID_TBTCMD_SEND_LSEN_CMD1                         "retimer index - %d. The required operation is enable\n"
///
/// SendOfflineMode - Retimer: Send LSEN command failed
///
#define EVT_ARGS_TBTCMD_SEND_LSEN_CMD_FAIL                   2
#define EVT_CODE_TBTCMD_SEND_LSEN_CMD_FAIL                   ((EVT_ID_TBTCMD_SEND_LSEN_CMD_FAIL << 2) | (EVT_ARGS_TBTCMD_SEND_LSEN_CMD_FAIL))
#define STR_ID_TBTCMD_SEND_LSEN_CMD_FAIL                     "TbtNvmDrvRetimerThruHrCtor: The retimer could not perform LS %d (1->Enable, 0->Disable), Status %d. Exiting...\n"
///< TbtNvmDrvRetimerThruHrHelpers.c - end


///< TbtNvmDrvHr.c - start
///
/// TbtNvmDrvHrSendDrvReady - Failed to generate CRC from the data
///
#define EVT_ARGS_TBT_DRV_GEN_CRC_FAIL                        1
#define EVT_CODE_TBT_DRV_GEN_CRC_FAIL                        ((EVT_ID_TBT_DRV_GEN_CRC_FAIL << 2) | (EVT_ARGS_TBT_DRV_GEN_CRC_FAIL))
#define STR_ID_TBT_DRV_GEN_CRC_FAIL                          "TbtNvmDrvHrSendDrvReady: Failed to generate CRC from the data. Status - %d. Exiting...\n"
///
/// TbtNvmDrvHrSendDrvReady - The driver ready command failed
///
#define EVT_ARGS_TBT_DRV_RDY_CMD_SEND_FAIL                   1
#define EVT_CODE_TBT_DRV_RDY_CMD_SEND_FAIL                   ((EVT_ID_TBT_DRV_RDY_CMD_SEND_FAIL << 2) | (EVT_ARGS_TBT_DRV_RDY_CMD_SEND_FAIL))
#define STR_ID_TBT_DRV_RDY_CMD_SEND_FAIL                     "TbtNvmDrvHrSendDrvReady: The driver ready command failed. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvHrSendDrvReady - Didn't receive response for driver ready command
///
#define EVT_ARGS_TBT_DRV_RDY_CMD_RECEIVE_FAIL                1
#define EVT_CODE_TBT_DRV_RDY_CMD_RECEIVE_FAIL                ((EVT_ID_TBT_DRV_RDY_CMD_RECEIVE_FAIL << 2) | (EVT_ARGS_TBT_DRV_RDY_CMD_RECEIVE_FAIL))
#define STR_ID_TBT_DRV_RDY_CMD_RECEIVE_FAIL                  "TbtNvmDrvHrSendDrvReady: Didn't receive response for driver ready command. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvHrSendDrvReady - TbtNvmDrvHrSendDrvReady: Sent successfully
///
#define EVT_ARGS_TBT_DRV_CMD_SUCCESS                         0
#define EVT_CODE_TBT_DRV_CMD_SUCCESS                         ((EVT_ID_TBT_DRV_CMD_SUCCESS << 2) | (EVT_ARGS_TBT_DRV_CMD_SUCCESS))
#define STR_ID_TBT_DRV_CMD_SUCCESS                           "TbtNvmDrvHrSendDrvReady: Sent successfully\n"
///
/// TbtNvmDrvHrWriteCioReg - TbtNvmDrvHrWriteCioReg: ConfigurationSpace - %d
///
#define EVT_ARGS_TBT_DRV_WRITE_CIO_REG                       1
#define EVT_CODE_TBT_DRV_WRITE_CIO_REG                       ((EVT_ID_TBT_DRV_WRITE_CIO_REG << 2) | (EVT_ARGS_TBT_DRV_WRITE_CIO_REG))
#define STR_ID_TBT_DRV_WRITE_CIO_REG                         "TbtNvmDrvHrWriteCioReg: ConfigurationSpace - %d "
///
/// TbtNvmDrvHrWriteCioReg - Adapter - %d, RegNum - 0x%x
///
#define EVT_ARGS_TBT_DRV_WRITE_CIO_CONFIG                    2
#define EVT_CODE_TBT_DRV_WRITE_CIO_CONFIG                    ((EVT_ID_TBT_DRV_WRITE_CIO_CONFIG << 2) | (EVT_ARGS_TBT_DRV_WRITE_CIO_CONFIG))
#define STR_ID_TBT_DRV_WRITE_CIO_CONFIG                      "Adapter - %d, RegNum - 0x%x\n"
///
/// TbtNvmDrvHrWriteCioReg - Length - 0x%x, Data - %x
///
#define EVT_ARGS_TBT_DRV_WRITE_CIO_DATA                      2
#define EVT_CODE_TBT_DRV_WRITE_CIO_DATA                      ((EVT_ID_TBT_DRV_WRITE_CIO_DATA << 2) | (EVT_ARGS_TBT_DRV_WRITE_CIO_DATA))
#define STR_ID_TBT_DRV_WRITE_CIO_DATA                        "TbtNvmDrvHrWriteCioReg: Length - 0x%x, Data - %x\n"
///
/// TbtNvmDrvHrWriteCioReg - TbtNvmDrvHrWriteCioReg: Couldn't allocate memory for write request struct. Exiting...
///
#define EVT_ARGS_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR            0
#define EVT_CODE_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR            ((EVT_ID_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR << 2) | (EVT_ARGS_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR))
#define STR_ID_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR              "TbtNvmDrvHrWriteCioReg: Couldn't allocate memory for write request struct. Exiting...\n"
///
/// TbtNvmDrvHrWriteCioReg - Avoid buffer overflow
///
#define EVT_ARGS_TBT_DRV_AVOID_BUFFER                        0
#define EVT_CODE_TBT_DRV_AVOID_BUFFER                        ((EVT_ID_TBT_DRV_AVOID_BUFFER << 2) | (EVT_ARGS_TBT_DRV_AVOID_BUFFER))
#define STR_ID_TBT_DRV_AVOID_BUFFER                          "TbtNvmDrvHrWriteCioReg: Avoid buffer overflow.\n"
///
/// TbtNvmDrvHrWriteCioReg - Failed to generate CRC from the data
///
#define EVT_ARGS_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL              1
#define EVT_CODE_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL              ((EVT_ID_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL << 2) | (EVT_ARGS_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL))
#define STR_ID_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL                "TbtNvmDrvHrWriteCioReg: Failed to generate CRC from the data. Status - %d. Exiting...\n"
///
/// TbtNvmDrvHrWriteCioReg - Couldn't transmit DMA frame to write to reg - 0x%x, length - 0x%x
///
#define EVT_ARGS_TBT_DRV_TRANSMIT_DMA_ERROR                  2
#define EVT_CODE_TBT_DRV_TRANSMIT_DMA_ERROR                  ((EVT_ID_TBT_DRV_TRANSMIT_DMA_ERROR << 2) | (EVT_ARGS_TBT_DRV_TRANSMIT_DMA_ERROR))
#define STR_ID_TBT_DRV_TRANSMIT_DMA_ERROR                    "TbtNvmDrvHrWriteCioReg: Couldn't transmit DMA frame to write to reg - 0x%x, length - 0x%x. Exiting...\n"
///
/// TbtNvmDrvHrWriteCioReg - Couldn't receive response while writing to reg - 0x%x, length - 0x%x.
///
#define EVT_ARGS_TBT_DRV_RECEIVE_ERROR                       2
#define EVT_CODE_TBT_DRV_RECEIVE_ERROR                       ((EVT_ID_TBT_DRV_RECEIVE_ERROR << 2) | (EVT_ARGS_TBT_DRV_RECEIVE_ERROR))
#define STR_ID_TBT_DRV_RECEIVE_ERROR                         "TbtNvmDrvHrReadCioDevReg: TbtNvmDrvHrWriteCioReg: Couldn't receive response while writing to reg - 0x%x, length - 0x%x. Exiting...\n"
///
/// TbtNvmDrvHrReadCioDevReg - Avoid buffer overflow
///
#define EVT_ARGS_TBT_DRV_READ_AVOID_BUFFER                   0
#define EVT_CODE_TBT_DRV_READ_AVOID_BUFFER                   ((EVT_ID_TBT_DRV_READ_AVOID_BUFFER << 2) | (EVT_ARGS_TBT_DRV_READ_AVOID_BUFFER))
#define STR_ID_TBT_DRV_READ_AVOID_BUFFER                     "TbtNvmDrvHrReadCioDevReg: Avoid buffer overflow.\n"
///
/// TbtNvmDrvHrReadCioDevReg - ConfigurationSpace - %d
///
#define EVT_ARGS_TBT_DRV_READ_CIO_CONFIG                     1
#define EVT_CODE_TBT_DRV_READ_CIO_CONFIG                     ((EVT_ID_TBT_DRV_READ_CIO_CONFIG << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_CONFIG))
#define STR_ID_TBT_DRV_READ_CIO_CONFIG                       "TbtNvmDrvHrReadCioDevReg: ConfigurationSpace - %d "
///
/// TbtNvmDrvHrReadCioDevReg - Adapter - %d, RegNum - 0x%x
///
#define EVT_ARGS_TBT_DRV_READ_CIO_CONFIG2                    2
#define EVT_CODE_TBT_DRV_READ_CIO_CONFIG2                    ((EVT_ID_TBT_DRV_READ_CIO_CONFIG2 << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_CONFIG2))
#define STR_ID_TBT_DRV_READ_CIO_CONFIG2                      "Adapter - %d, RegNum - 0x%x\n"
///
/// TbtNvmDrvHrReadCioDevReg - Couldn't allocate memory for read request struct
///
#define EVT_ARGS_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR         0
#define EVT_CODE_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR         ((EVT_ID_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR))
#define STR_ID_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR           "Couldn't allocate memory for read request struct.\n"
///
/// TbtNvmDrvHrReadCioDevReg - Failed to generate CRC from the data
///
#define EVT_ARGS_TBT_DRV_READ_CIO_GEN_CRC_FAIL               1
#define EVT_CODE_TBT_DRV_READ_CIO_GEN_CRC_FAIL               ((EVT_ID_TBT_DRV_READ_CIO_GEN_CRC_FAIL << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_GEN_CRC_FAIL))
#define STR_ID_TBT_DRV_READ_CIO_GEN_CRC_FAIL                 "TbtNvmDrvHrReadCioDevReg: Failed to generate CRC from the data. Status - %d. Exiting...\n"
///
/// TbtNvmDrvHrReadCioDevReg - Couldn't transmit DMA frame to read from reg - 0x%x
///
#define EVT_ARGS_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL          1
#define EVT_CODE_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL          ((EVT_ID_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL))
#define STR_ID_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL            "TbtNvmDrvHrReadCioDevReg: Couldn't transmit DMA frame to read from reg - 0x%x. Exiting...\n"
///
/// TbtNvmDrvHrReadCioDevReg - Couldn't receive response while reading from reg - 0x%x
///
#define EVT_ARGS_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL           1
#define EVT_CODE_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL           ((EVT_ID_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL))
#define STR_ID_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL             "TbtNvmDrvHrReadCioDevReg: Couldn't receive response while reading from reg - 0x%x. Exiting...\n"
///
/// TbtNvmDrvHrReadCioDevReg - Read data is - 0x%x
///
#define EVT_ARGS_TBT_DRV_READ_CIO_READ_DATA                  1
#define EVT_CODE_TBT_DRV_READ_CIO_READ_DATA                  ((EVT_ID_TBT_DRV_READ_CIO_READ_DATA << 2) | (EVT_ARGS_TBT_DRV_READ_CIO_READ_DATA))
#define STR_ID_TBT_DRV_READ_CIO_READ_DATA                    "Read data is - 0x%x\n"
///
/// GetTbtHostRouterList - GetTbtHostRouterList:
///
#define EVT_ARGS_TBT_DRV_GET_HOST_ROUTER_LIST                0
#define EVT_CODE_TBT_DRV_GET_HOST_ROUTER_LIST                ((EVT_ID_TBT_DRV_GET_HOST_ROUTER_LIST << 2) | (EVT_ARGS_TBT_DRV_GET_HOST_ROUTER_LIST))
#define STR_ID_TBT_DRV_GET_HOST_ROUTER_LIST                  "GetTbtHostRouterList: "
///
/// GetTbtHostRouterList - FirmwareType=%d ForcePwrFunc=%
///
#define EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO                    2
#define EVT_CODE_TBT_DRV_HOST_ROUTER_INFO                    ((EVT_ID_TBT_DRV_HOST_ROUTER_INFO << 2) | (EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO))
#define STR_ID_TBT_DRV_HOST_ROUTER_INFO                      "FirmwareType=%d ForcePwrFunc=%p, "
///
/// GetTbtHostRouterList - B%d D%d
///
#define EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO2                   2
#define EVT_CODE_TBT_DRV_HOST_ROUTER_INFO2                   ((EVT_ID_TBT_DRV_HOST_ROUTER_INFO2 << 2) | (EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO2))
#define STR_ID_TBT_DRV_HOST_ROUTER_INFO2                     "B%d D%d "
///
/// GetTbtHostRouterList - F%d PcieRpType=%d
///
#define EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO3                   2
#define EVT_CODE_TBT_DRV_HOST_ROUTER_INFO3                   ((EVT_ID_TBT_DRV_HOST_ROUTER_INFO3 << 2) | (EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO3))
#define STR_ID_TBT_DRV_HOST_ROUTER_INFO3                     "F%d PcieRpType=%d "
///
/// GetTbtHostRouterList - PcieRootPort=%d ret=%p
///
#define EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO4                   2
#define EVT_CODE_TBT_DRV_HOST_ROUTER_INFO4                   ((EVT_ID_TBT_DRV_HOST_ROUTER_INFO4 << 2) | (EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO4))
#define STR_ID_TBT_DRV_HOST_ROUTER_INFO4                     "PcieRootPort=%d ret=%p "
///
/// GetTbtHostRouterList - RefCount=%d
///
#define EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO5                   1
#define EVT_CODE_TBT_DRV_HOST_ROUTER_INFO5                   ((EVT_ID_TBT_DRV_HOST_ROUTER_INFO5 << 2) | (EVT_ARGS_TBT_DRV_HOST_ROUTER_INFO5))
#define STR_ID_TBT_DRV_HOST_ROUTER_INFO5                     "RefCount=%d\n"
///
/// AddTbtHostRouterList - AddTbtHostRouterList ERROR: increase MAX_HOST_ROUTERS
///
#define EVT_ARGS_TBT_DRV_ADD_ROUTER_ERROR                    0
#define EVT_CODE_TBT_DRV_ADD_ROUTER_ERROR                    ((EVT_ID_TBT_DRV_ADD_ROUTER_ERROR << 2) | (EVT_ARGS_TBT_DRV_ADD_ROUTER_ERROR))
#define STR_ID_TBT_DRV_ADD_ROUTER_ERROR                      "AddTbtHostRouterList ERROR: increase MAX_HOST_ROUTERS\n"
///
/// AddTbtHostRouterList - AddTbtHostRouterList:
///
#define EVT_ARGS_TBT_DRV_ADD_ROUTER_LIST                     0
#define EVT_CODE_TBT_DRV_ADD_ROUTER_LIST                     ((EVT_ID_TBT_DRV_ADD_ROUTER_LIST << 2) | (EVT_ARGS_TBT_DRV_ADD_ROUTER_LIST))
#define STR_ID_TBT_DRV_ADD_ROUTER_LIST                       "AddTbtHostRouterList: "
///
/// RemoveTbtHostRouterList - RemoveTbtHostRouterList:
///
#define EVT_ARGS_TBT_DRV_REMOVE_ROUTER_LIST                  0
#define EVT_CODE_TBT_DRV_REMOVE_ROUTER_LIST                  ((EVT_ID_TBT_DRV_REMOVE_ROUTER_LIST << 2) | (EVT_ARGS_TBT_DRV_REMOVE_ROUTER_LIST))
#define STR_ID_TBT_DRV_REMOVE_ROUTER_LIST                    "RemoveTbtHostRouterList: "
///
/// RemoveTbtHostRouterList - RemoveTbtHostRouterList ERROR: TbtHostRouter=%p
///
#define EVT_ARGS_TBT_DRV_REMOVE_ROUTER_ERROR                 1
#define EVT_CODE_TBT_DRV_REMOVE_ROUTER_ERROR                 ((EVT_ID_TBT_DRV_REMOVE_ROUTER_ERROR << 2) | (EVT_ARGS_TBT_DRV_REMOVE_ROUTER_ERROR))
#define STR_ID_TBT_DRV_REMOVE_ROUTER_ERROR                   "RemoveTbtHostRouterList ERROR: TbtHostRouter=%p\n"
///
/// TbtNvmDrvHrDtor - TbtNvmDrvHrDtor is called RefCount=%d
///
#define EVT_ARGS_TBT_DRV_DTOR                                1
#define EVT_CODE_TBT_DRV_DTOR                                ((EVT_ID_TBT_DRV_DTOR << 2) | (EVT_ARGS_TBT_DRV_DTOR))
#define STR_ID_TBT_DRV_DTOR                                  "TbtNvmDrvHrDtor is called RefCount=%d\n"
///
/// TbtNvmDrvHrCtor - LocateHandleBuffer of gEfiPciIoProtocolGuid failed
///
#define EVT_ARGS_TBT_DRV_CTOR_PROTOCOL_FAIL                  1
#define EVT_CODE_TBT_DRV_CTOR_PROTOCOL_FAIL                  ((EVT_ID_TBT_DRV_CTOR_PROTOCOL_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_PROTOCOL_FAIL))
#define STR_ID_TBT_DRV_CTOR_PROTOCOL_FAIL                    "TbtNvmDrvHrCtor: LocateHandleBuffer of gEfiPciIoProtocolGuid failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: TbtNvmDrvAllocateMem failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL              1
#define EVT_CODE_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL              ((EVT_ID_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL))
#define STR_ID_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL                "TbtNvmDrvHrCtor: TbtNvmDrvAllocateMem failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: Type:PCH FirmwareType %x
///
#define EVT_ARGS_TBT_DRV_CTOR_TYPE_PCH                       1
#define EVT_CODE_TBT_DRV_CTOR_TYPE_PCH                       ((EVT_ID_TBT_DRV_CTOR_TYPE_PCH << 2) | (EVT_ARGS_TBT_DRV_CTOR_TYPE_PCH))
#define STR_ID_TBT_DRV_CTOR_TYPE_PCH                         "TbtNvmDrvHrCtor: Type:PCH FirmwareType %x "
///
/// TbtNvmDrvHrCtor - PchRpNumber %x PchBusNumber %x
///
#define EVT_ARGS_TBT_DRV_CTOR_TYPE_PCH1                      2
#define EVT_CODE_TBT_DRV_CTOR_TYPE_PCH1                      ((EVT_ID_TBT_DRV_CTOR_TYPE_PCH1 << 2) | (EVT_ARGS_TBT_DRV_CTOR_TYPE_PCH1))
#define STR_ID_TBT_DRV_CTOR_TYPE_PCH1                        "PchRpNumber %x PchBusNumber %x\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: Type:CPU FirmwareType %x
///
#define EVT_ARGS_TBT_DRV_CTOR_TYPE_CPU                       1
#define EVT_CODE_TBT_DRV_CTOR_TYPE_CPU                       ((EVT_ID_TBT_DRV_CTOR_TYPE_CPU << 2) | (EVT_ARGS_TBT_DRV_CTOR_TYPE_CPU))
#define STR_ID_TBT_DRV_CTOR_TYPE_CPU                         "TbtNvmDrvHrCtor: Type:CPU FirmwareType %x "
///
/// TbtNvmDrvHrCtor - CpuRpNumber %x CpuBusNumber %x
///
#define EVT_ARGS_TBT_DRV_CTOR_TYPE_CPU1                      2
#define EVT_CODE_TBT_DRV_CTOR_TYPE_CPU1                      ((EVT_ID_TBT_DRV_CTOR_TYPE_CPU1 << 2) | (EVT_ARGS_TBT_DRV_CTOR_TYPE_CPU1))
#define STR_ID_TBT_DRV_CTOR_TYPE_CPU1                        "CpuRpNumber %x CpuBusNumber %x\n"
///
/// TbtNvmDrvHrCtor - Failed at unsupported type value %x in PCIE_RP_TYPE
///
#define EVT_ARGS_TBT_DRV_CTOR_INVALID_RP_TYPE                1
#define EVT_CODE_TBT_DRV_CTOR_INVALID_RP_TYPE                ((EVT_ID_TBT_DRV_CTOR_TYPE_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_INVALID_RP_TYPE))
#define STR_ID_TBT_DRV_CTOR_INVALID_RP_TYPE                  "TbtNvmDrvHrCtor: Failed at unsupported type value %x in PCIE_RP_TYPE.\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: HandleProtocol of gEfiPciIoProtocolGuid failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL           1
#define EVT_CODE_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL           ((EVT_ID_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL))
#define STR_ID_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL             "TbtNvmDrvHrCtor: HandleProtocol of gEfiPciIoProtocolGuid failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - Pci.Read failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_READ_FAIL                  1
#define EVT_CODE_TBT_DRV_CTOR_PCI_READ_FAIL                  ((EVT_ID_TBT_DRV_CTOR_PCI_READ_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_READ_FAIL))
#define STR_ID_TBT_DRV_CTOR_PCI_READ_FAIL                    "TbtNvmDrvHrCtor: Pci.Read failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - check for USB class failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_CHECK_USB_FAIL                 1
#define EVT_CODE_TBT_DRV_CTOR_CHECK_USB_FAIL                 ((EVT_ID_TBT_DRV_CTOR_CHECK_USB_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_CHECK_USB_FAIL))
#define STR_ID_TBT_DRV_CTOR_CHECK_USB_FAIL                   "TbtNvmDrvHrCtor: check for USB class failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - PCI Device, class code
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_DEV_CLASS                  1
#define EVT_CODE_TBT_DRV_CTOR_PCI_DEV_CLASS                  ((EVT_ID_TBT_DRV_CTOR_PCI_DEV_CLASS << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_DEV_CLASS))
#define STR_ID_TBT_DRV_CTOR_PCI_DEV_CLASS                    "PCI Device, class code = (%0xh, "
///
/// TbtNvmDrvHrCtor - PCI Device, class code
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_DEV_CLASS2                 2
#define EVT_CODE_TBT_DRV_CTOR_PCI_DEV_CLASS2                 ((EVT_ID_TBT_DRV_CTOR_PCI_DEV_CLASS2 << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_DEV_CLASS2))
#define STR_ID_TBT_DRV_CTOR_PCI_DEV_CLASS2                   "%0xh, %0xh)\n"

///
/// TbtNvmDrvHrCtor - USB4 PCI device
///
#define EVT_ARGS_TBT_DRV_CTOR_USB4_PCI                       0
#define EVT_CODE_TBT_DRV_CTOR_USB4_PCI                       ((EVT_ID_TBT_DRV_CTOR_USB4_PCI << 2) | (EVT_ARGS_TBT_DRV_CTOR_USB4_PCI))
#define STR_ID_TBT_DRV_CTOR_USB4_PCI                         "TbtNvmDrvHrCtor: USB4 PCI device.\n"
///
/// TbtNvmDrvHrCtor - GetLocation failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_LOCATION_GET_FAIL              1
#define EVT_CODE_TBT_DRV_CTOR_LOCATION_GET_FAIL              ((EVT_ID_TBT_DRV_CTOR_LOCATION_GET_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_LOCATION_GET_FAIL))
#define STR_ID_TBT_DRV_CTOR_LOCATION_GET_FAIL                "TbtNvmDrvHrCtor: GetLocation failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - The TBT Controller %x:
///
#define EVT_ARGS_TBT_DRV_CTOR_CONTROLLER                     1
#define EVT_CODE_TBT_DRV_CTOR_CONTROLLER                     ((EVT_ID_TBT_DRV_CTOR_CONTROLLER << 2) | (EVT_ARGS_TBT_DRV_CTOR_CONTROLLER))
#define STR_ID_TBT_DRV_CTOR_CONTROLLER                       "The TBT Controller %x: "
///
/// TbtNvmDrvHrCtor - %04x:%02x:
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_INFO                       2
#define EVT_CODE_TBT_DRV_CTOR_PCI_INFO                       ((EVT_ID_TBT_DRV_CTOR_PCI_INFO << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_INFO))
#define STR_ID_TBT_DRV_CTOR_PCI_INFO                         "%04x:%02x:"
///
/// TbtNvmDrvHrCtor - %02x.%02x is not the target
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_INFO2                      2
#define EVT_CODE_TBT_DRV_CTOR_PCI_INFO2                      ((EVT_ID_TBT_DRV_CTOR_PCI_INFO2 << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_INFO2))
#define STR_ID_TBT_DRV_CTOR_PCI_INFO2                        "%02x.%02x is not the target.\n"
///
/// TbtNvmDrvHrCtor - %02x.%02x
///
#define EVT_ARGS_TBT_DRV_CTOR_PCI_INFO3                      2
#define EVT_CODE_TBT_DRV_CTOR_PCI_INFO3                      ((EVT_ID_TBT_DRV_CTOR_PCI_INFO3 << 2) | (EVT_ARGS_TBT_DRV_CTOR_PCI_INFO3))
#define STR_ID_TBT_DRV_CTOR_PCI_INFO3                        "%02x.%02x\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: Found Thunderbolt Controller
///
#define EVT_ARGS_TBT_DRV_CTOR_CONTROLLER_FOUND               1
#define EVT_CODE_TBT_DRV_CTOR_CONTROLLER_FOUND               ((EVT_ID_TBT_DRV_CTOR_CONTROLLER_FOUND << 2) | (EVT_ARGS_TBT_DRV_CTOR_CONTROLLER_FOUND))
#define STR_ID_TBT_DRV_CTOR_CONTROLLER_FOUND                 "TbtNvmDrvHrCtor: Found Thunderbolt Controller %x: "
///
/// TbtNvmDrvHrCtor - PCI device with bdf = 0x%x,0x%x,
///
#define EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER     2
#define EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER     ((EVT_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER << 2) | (EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER))
#define STR_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER       "TbtNvmDrvHrCtor: PCI device with bdf = 0x%x,0x%x,"
///
/// TbtNvmDrvHrCtor - 0x%x is not found!, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2    2
#define EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2    ((EVT_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2 << 2) | (EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2))
#define STR_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2      "0x%x is not found!, Status = %r\n"
///
/// TbtNvmDrvHrCtor - PCI device is not found with root port = %d
///
#define EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER     2
#define EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER     ((EVT_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER << 2) | (EVT_ARGS_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER))
#define STR_ID_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER       "TbtNvmDrvHrCtor: PCI device is not found with root port = %d!, Status = %r\n"
///
/// TbtNvmDrvHrCtor - Attributes get is failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL            1
#define EVT_CODE_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL            ((EVT_ID_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL))
#define STR_ID_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL              "TbtNvmDrvHrCtor: Attributes get is failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - Attributes set failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL            1
#define EVT_CODE_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL            ((EVT_ID_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL))
#define STR_ID_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL              "TbtNvmDrvHrCtor: Attributes set failed, Status = %r\n"
///
/// TbtNvmDrvHrCtor - Force power the ITBT
///
#define EVT_ARGS_TBT_DRV_CTOR_FORCE_PWR_ITBT                 0
#define EVT_CODE_TBT_DRV_CTOR_FORCE_PWR_ITBT                 ((EVT_ID_TBT_DRV_CTOR_FORCE_PWR_ITBT << 2) | (EVT_ARGS_TBT_DRV_CTOR_FORCE_PWR_ITBT))
#define STR_ID_TBT_DRV_CTOR_FORCE_PWR_ITBT                   "TbtNvmDrvHrCtor: Force power the ITBT.\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: Could not perform device %x%x force pwr
///
#define EVT_ARGS_TBT_DRV_CTOR_PERFORM_PWR_ERROR              2
#define EVT_CODE_TBT_DRV_CTOR_PERFORM_PWR_ERROR              ((EVT_ID_TBT_DRV_CTOR_PERFORM_PWR_ERROR << 2) | (EVT_ARGS_TBT_DRV_CTOR_PERFORM_PWR_ERROR))
#define STR_ID_TBT_DRV_CTOR_PERFORM_PWR_ERROR                "TbtNvmDrvHrCtor: Could not perform device %x force pwr, Status = %d\n"
///
/// TbtNvmDrvHrCtor - Force powered device %x%x
///
#define EVT_ARGS_TBT_DRV_CTOR_FORCE_PWR_DEV                  1
#define EVT_CODE_TBT_DRV_CTOR_FORCE_PWR_DEV                  ((EVT_ID_TBT_DRV_CTOR_FORCE_PWR_DEV << 2) | (EVT_ARGS_TBT_DRV_CTOR_FORCE_PWR_DEV))
#define STR_ID_TBT_DRV_CTOR_FORCE_PWR_DEV                    "TbtNvmDrvHrCtor: Force powered device %x\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: Could not initialize DMA for device
///
#define EVT_ARGS_TBT_DRV_CTOR_INIT_DMA_FAIL                  0
#define EVT_CODE_TBT_DRV_CTOR_INIT_DMA_FAIL                  ((EVT_ID_TBT_DRV_CTOR_INIT_DMA_FAIL << 2) | (EVT_ARGS_TBT_DRV_CTOR_INIT_DMA_FAIL))
#define STR_ID_TBT_DRV_CTOR_INIT_DMA_FAIL                    "TbtNvmDrvHrCtor: Could not initialize DMA for device.\n"
///
/// TbtNvmDrvHrCtor - Could not allocate memory for TBT_HOST_ROUTER
///
#define EVT_ARGS_TBT_DRV_HOST_ALLOCATE_MEM_FAIL              0
#define EVT_CODE_TBT_DRV_HOST_ALLOCATE_MEM_FAIL              ((EVT_ID_TBT_DRV_HOST_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_TBT_DRV_HOST_ALLOCATE_MEM_FAIL))
#define STR_ID_TBT_DRV_HOST_ALLOCATE_MEM_FAIL                "TbtNvmDrvHrCtor: Could not allocate memory for TBT_HOST_ROUTER.\n"
///
/// TbtNvmDrvHrCtor - No integrated USB4 host rounter found
///
#define EVT_ARGS_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND             0
#define EVT_CODE_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND             ((EVT_ID_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND << 2) | (EVT_ARGS_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND))
#define STR_ID_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND               "No integrated USB4 host rounter found!\n"
///
/// TbtNvmDrvHrCtor - TbtNvmDrvHrCtor: CM Mode = %x
///
#define EVT_ARGS_TBT_DRV_CM_MODE                             1
#define EVT_CODE_TBT_DRV_CM_MODE                             ((EVT_ID_TBT_DRV_CM_MODE << 2) | (EVT_ARGS_TBT_DRV_CM_MODE))
#define STR_ID_TBT_DRV_CM_MODE                               "TbtNvmDrvHrCtor: CM Mode = %x\n"
///
/// TbtNvmDrvHrCtor - Failed to send driver ready, Status=%d
///
#define EVT_ARGS_TBT_DRV_SEND_DRIVER_RDY_FAIL                1
#define EVT_CODE_TBT_DRV_SEND_DRIVER_RDY_FAIL                ((EVT_ID_TBT_DRV_SEND_DRIVER_RDY_FAIL << 2) | (EVT_ARGS_TBT_DRV_SEND_DRIVER_RDY_FAIL))
#define STR_ID_TBT_DRV_SEND_DRIVER_RDY_FAIL                  "TbtNvmDrvHrCtor: Failed to send driver ready, Status=%d\n"
///
/// TbtNvmDrvHrCtor - Completed
///
#define EVT_ARGS_TBT_DRV_HR_CTOR_COMPLETE                    0
#define EVT_CODE_TBT_DRV_HR_CTOR_COMPLETE                    ((EVT_ID_TBT_DRV_HR_CTOR_COMPLETE << 2) | (EVT_ARGS_TBT_DRV_HR_CTOR_COMPLETE))
#define STR_ID_TBT_DRV_HR_CTOR_COMPLETE                      "TbtNvmDrvHrCtor: Completed.\n"
///< TbtNvmDrvHr.c - end


///< TbtNvmDrvDma.c - start
///
/// TbtNvmDrvGetTxBuf - DMA is not functional, producer %x out of range
///
#define EVT_ARGS_TBT_DRV_DMA_ERROR                           1
#define EVT_CODE_TBT_DRV_DMA_ERROR                           ((EVT_ID_TBT_DRV_DMA_ERROR << 2) | (EVT_ARGS_TBT_DRV_DMA_ERROR))
#define STR_ID_TBT_DRV_DMA_ERROR                             "TbtDrvGetTxBuf: DMA is not functional, producer %x out of range.\n"
///
/// TbtNvmDrvGetTxBuf - DMA is not functional, TX ring is full
///
#define EVT_ARGS_TBT_DRV_DMA_TX_RING_FULL                    0
#define EVT_CODE_TBT_DRV_DMA_TX_RING_FULL                    ((EVT_ID_TBT_DRV_DMA_TX_RING_FULL << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_RING_FULL))
#define STR_ID_TBT_DRV_DMA_TX_RING_FULL                      "TbtDrvGetTxBuf: DMA is not functional, TX ring is full.\n"
///
/// TbtNvmDrvInitiateTx - Sending packet with pdf=0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_TX_PACKET_INFO                  1
#define EVT_CODE_TBT_DRV_DMA_TX_PACKET_INFO                  ((EVT_ID_TBT_DRV_DMA_TX_PACKET_INFO << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_PACKET_INFO))
#define STR_ID_TBT_DRV_DMA_TX_PACKET_INFO                    "TbtNvmDrvInitiateTx: Sending packet with pdf=0x%x,"
///
/// TbtNvmDrvInitiateTx - length=0x%x, prod=0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_TX_PACKET_INFO2                 2
#define EVT_CODE_TBT_DRV_DMA_TX_PACKET_INFO2                 ((EVT_ID_TBT_DRV_DMA_TX_PACKET_INFO2 << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_PACKET_INFO2))
#define STR_ID_TBT_DRV_DMA_TX_PACKET_INFO2                   "length=0x%x, prod=0x%x\n"
///
/// TbtNvmDrvInitiateTx - consumer is bigger than ring size
///
#define EVT_ARGS_TBT_DRV_DMA_TX_FRAME_ERROR                  0
#define EVT_CODE_TBT_DRV_DMA_TX_FRAME_ERROR                  ((EVT_ID_TBT_DRV_DMA_TX_FRAME_ERROR << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_FRAME_ERROR))
#define STR_ID_TBT_DRV_DMA_TX_FRAME_ERROR                    "The producer - consumer is bigger than ring size!\n"
///
/// TbtNvmDrvWaitForRxResponse - Before: RxDescPtr - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO                     2
#define EVT_CODE_TBT_DRV_DMA_RX_PTR_INFO                     ((EVT_ID_TBT_DRV_DMA_RX_PTR_INFO << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO))
#define STR_ID_TBT_DRV_DMA_RX_PTR_INFO                       "Before: RxDescPtr - 0x%x, attributes - 0x%x, "
///
/// TbtNvmDrvWaitForRxResponse - Attributes - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO2                    2
#define EVT_CODE_TBT_DRV_DMA_RX_PTR_INFO2                    ((EVT_ID_TBT_DRV_DMA_RX_PTR_INFO2 << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO2))
#define STR_ID_TBT_DRV_DMA_RX_PTR_INFO2                      "data addr - 0x%x%x\n"
///
/// TbtNvmDrvWaitForRxResponse - Stall had failed, Status %d
///
#define EVT_ARGS_TBT_DRV_DMA_RX_STALL_ERROR                  1
#define EVT_CODE_TBT_DRV_DMA_RX_STALL_ERROR                  ((EVT_ID_TBT_DRV_DMA_RX_STALL_ERROR << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_STALL_ERROR))
#define STR_ID_TBT_DRV_DMA_RX_STALL_ERROR                    "Stall had failed, Status %d\n"
///
/// TbtNvmDrvWaitForRxResponse - ERROR! Timeout waiting for any response packet from DMA
///
#define EVT_ARGS_TBT_DRV_DMA_RX_WAIT_TIMEOUT                 0
#define EVT_CODE_TBT_DRV_DMA_RX_WAIT_TIMEOUT                 ((EVT_ID_TBT_DRV_DMA_RX_WAIT_TIMEOUT << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_WAIT_TIMEOUT))
#define STR_ID_TBT_DRV_DMA_RX_WAIT_TIMEOUT                   "TbtNvmDrvWaitForRxResponse: ERROR! Timeout waiting for any response packet from DMA.\n"
///
/// TbtNvmDrvWaitForRxResponse - Waited for %d us. Exiting...
///
#define EVT_ARGS_TBT_DRV_DMA_RX_WAIT_TIMEOUT2                1
#define EVT_CODE_TBT_DRV_DMA_RX_WAIT_TIMEOUT2                ((EVT_ID_TBT_DRV_DMA_RX_WAIT_TIMEOUT2 << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_WAIT_TIMEOUT2))
#define STR_ID_TBT_DRV_DMA_RX_WAIT_TIMEOUT2                  "Waited for %d us. Exiting...\n"
///
/// TbtNvmDrvWaitForRxResponse - The descriptor Info:
///
#define EVT_ARGS_TBT_DRV_DMA_RX_DESCRIPT                     2
#define EVT_CODE_TBT_DRV_DMA_RX_DESCRIPT                     ((EVT_ID_TBT_DRV_DMA_RX_DESCRIPT << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_DESCRIPT))
#define STR_ID_TBT_DRV_DMA_RX_DESCRIPT                       "TbtNvmDrvWaitForRxResponse: The descriptor address - 0x%x, attributes - 0x%x\n"
///
/// TbtNvmDrvWaitForRxResponse - DMA RX frame: pdf 0x%x cmd 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_FRAME_INFO                   2
#define EVT_CODE_TBT_DRV_DMA_RX_FRAME_INFO                   ((EVT_ID_TBT_DRV_DMA_RX_FRAME_INFO << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_FRAME_INFO))
#define STR_ID_TBT_DRV_DMA_RX_FRAME_INFO                     "DMA RX frame: pdf 0x%x cmd 0x%x "
///
/// TbtNvmDrvWaitForRxResponse - msg_len 0x%x. Consumer index 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_FRAME_INFO2                  2
#define EVT_CODE_TBT_DRV_DMA_RX_FRAME_INFO2                  ((EVT_ID_TBT_DRV_DMA_RX_FRAME_INFO2 << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_FRAME_INFO2))
#define STR_ID_TBT_DRV_DMA_RX_FRAME_INFO2                    "msg_len 0x%x. Consumer index 0x%x\n"
///
/// TbtNvmDrvWaitForRxResponse - After: RxDescPtr - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO_AFTER               2
#define EVT_CODE_TBT_DRV_DMA_RX_PTR_INFO_AFTER               ((EVT_ID_TBT_DRV_DMA_RX_PTR_INFO_AFTER << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_PTR_INFO_AFTER))
#define STR_ID_TBT_DRV_DMA_RX_PTR_INFO_AFTER                 "After: RxDescPtr - 0x%x, attributes - 0x%x, "
///
/// TbtNvmDrvWaitForRxResponse - Timeout waiting for the response with given Pdf 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_RESP_WAIT_TIMEOUT            1
#define EVT_CODE_TBT_DRV_DMA_RX_RESP_WAIT_TIMEOUT            ((EVT_ID_TBT_DRV_DMA_RX_RESP_WAIT_TIMEOUT << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_RESP_WAIT_TIMEOUT))
#define STR_ID_TBT_DRV_DMA_RX_RESP_WAIT_TIMEOUT              "Timeout waiting for the response with given Pdf 0x%x. Exiting...\n"
///
/// TbtNvmDrvRxCfgPkt - The frame received has length other than expected
///
#define EVT_ARGS_TBT_DRV_DMA_RX_RECEIVED_ERROR               0
#define EVT_CODE_TBT_DRV_DMA_RX_RECEIVED_ERROR               ((EVT_ID_TBT_DRV_DMA_RX_RECEIVED_ERROR << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_RECEIVED_ERROR))
#define STR_ID_TBT_DRV_DMA_RX_RECEIVED_ERROR                 "TbtNvmDrvRxCfgPkt: ERROR! The frame received has length other than expected.\n"
///
/// TbtNvmDrvRxCfgPkt - Expected - 0x%x, Received - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_RECEIVED_EXP                 2
#define EVT_CODE_TBT_DRV_DMA_RX_RECEIVED_EXP                 ((EVT_ID_TBT_DRV_DMA_RX_RECEIVED_EXP << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_RECEIVED_EXP))
#define STR_ID_TBT_DRV_DMA_RX_RECEIVED_EXP                   "Expected - 0x%x, Received - 0x%x\n"
///
/// TbtNvmDrvDmaWriteMmio - Write MMIO offset - 0x%x, wdata - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_WRITE_MMIO                      2
#define EVT_CODE_TBT_DRV_DMA_WRITE_MMIO                      ((EVT_ID_TBT_DRV_DMA_WRITE_MMIO << 2) | (EVT_ARGS_TBT_DRV_DMA_WRITE_MMIO))
#define STR_ID_TBT_DRV_DMA_WRITE_MMIO                        "Write MMIO offset - 0x%x, wdata - 0x%x\n"
///
/// TbtNvmDrvDmaReadMmio - Read MMIO offset - 0x%x, rdata - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_READ_MMIO                       2
#define EVT_CODE_TBT_DRV_DMA_READ_MMIO                       ((EVT_ID_TBT_DRV_DMA_READ_MMIO << 2) | (EVT_ARGS_TBT_DRV_DMA_READ_MMIO))
#define STR_ID_TBT_DRV_DMA_READ_MMIO                         "Read MMIO offset - 0x%x, rdata - 0x%x\n"
///
/// TbtNvmDrvDmaDebugPrint - symbol "------"
///
#define EVT_ARGS_TBT_DRV_DMA_SYMBOL                          0
#define EVT_CODE_TBT_DRV_DMA_SYMBOL                          ((EVT_ID_TBT_DRV_DMA_SYMBOL << 2) | (EVT_ARGS_TBT_DRV_DMA_SYMBOL))
#define STR_ID_TBT_DRV_DMA_SYMBOL                            "----------------------------------------------------------------------\n"
///
/// TbtNvmDrvDmaDebugPrint - Printing the DMA internal state for debug:
///
#define EVT_ARGS_TBT_DRV_DMA_STATE_PRINT                     0
#define EVT_CODE_TBT_DRV_DMA_STATE_PRINT                     ((EVT_ID_TBT_DRV_DMA_STATE_PRINT << 2) | (EVT_ARGS_TBT_DRV_DMA_STATE_PRINT))
#define STR_ID_TBT_DRV_DMA_STATE_PRINT                       "Printing the DMA internal state for debug:\n"
///
/// TbtNvmDrvDmaDebugPrint - ARC_DEBUG - 0x%x\n
///
#define EVT_ARGS_TBT_DRV_DMA_ARC_DEBUG                       1
#define EVT_CODE_TBT_DRV_DMA_ARC_DEBUG                       ((EVT_ID_TBT_DRV_DMA_ARC_DEBUG << 2) | (EVT_ARGS_TBT_DRV_DMA_ARC_DEBUG))
#define STR_ID_TBT_DRV_DMA_ARC_DEBUG                         "ARC_DEBUG - 0x%x\n\n"
///
/// TbtNvmDrvDmaDebugPrint - TX ring descriptor DW%d - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_TX_RING                         2
#define EVT_CODE_TBT_DRV_DMA_TX_RING                         ((EVT_ID_TBT_DRV_DMA_TX_RING << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_RING))
#define STR_ID_TBT_DRV_DMA_TX_RING                           "TX ring descriptor DW%d - 0x%x\n"
///
/// TbtNvmDrvDmaDebugPrint - TX ring table DW%d - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_TX_RING_TABLE                   2
#define EVT_CODE_TBT_DRV_DMA_TX_RING_TABLE                   ((EVT_ID_TBT_DRV_DMA_TX_RING_TABLE << 2) | (EVT_ARGS_TBT_DRV_DMA_TX_RING_TABLE))
#define STR_ID_TBT_DRV_DMA_TX_RING_TABLE                     "TX ring table DW%d - 0x%x\n"
///
/// TbtNvmDrvDmaDebugPrint - RX ring descriptor DW%d - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_RING                         2
#define EVT_CODE_TBT_DRV_DMA_RX_RING                         ((EVT_ID_TBT_DRV_DMA_RX_RING << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_RING))
#define STR_ID_TBT_DRV_DMA_RX_RING                           "RX ring descriptor DW%d - 0x%x\n"
///
/// TbtNvmDrvDmaDebugPrint - RX ring table DW%d - 0x%x
///
#define EVT_ARGS_TBT_DRV_DMA_RX_RING_TABLE                   2
#define EVT_CODE_TBT_DRV_DMA_RX_RING_TABLE                   ((EVT_ID_TBT_DRV_DMA_RX_RING_TABLE << 2) | (EVT_ARGS_TBT_DRV_DMA_RX_RING_TABLE))
#define STR_ID_TBT_DRV_DMA_RX_RING_TABLE                     "RX ring table DW%d - 0x%x\n"
///
/// TbtNvmDrvDmaDebugPrint - \n
///
#define EVT_ARGS_TBT_DRV_DMA_NEWLINE                         0
#define EVT_CODE_TBT_DRV_DMA_NEWLINE                         ((EVT_ID_TBT_DRV_DMA_NEWLINE << 2) | (EVT_ARGS_TBT_DRV_DMA_NEWLINE))
#define STR_ID_TBT_DRV_DMA_NEWLINE                           "\n"
///
/// TbtNvmDrvDmaDtor - TbtNvmDrvDmaDtor is called
///
#define EVT_ARGS_TBT_DRV_DMA_DTOR                            0
#define EVT_CODE_TBT_DRV_DMA_DTOR                            ((EVT_ID_TBT_DRV_DMA_DTOR << 2) | (EVT_ARGS_TBT_DRV_DMA_DTOR))
#define STR_ID_TBT_DRV_DMA_DTOR                              "TbtNvmDrvDmaDtor is called.\n"
///
/// TbtNvmDrvDmaCtor - AllocateRuntimeZeroPool failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_DMA_CTOR_ALLOCATE_MEM_FAIL          1
#define EVT_CODE_TBT_DRV_DMA_CTOR_ALLOCATE_MEM_FAIL          ((EVT_ID_TBT_DRV_DMA_CTOR_ALLOCATE_MEM_FAIL << 2) | (EVT_ARGS_TBT_DRV_DMA_CTOR_ALLOCATE_MEM_FAIL))
#define STR_ID_TBT_DRV_DMA_CTOR_ALLOCATE_MEM_FAIL            "TbtNvmDrvDmaCtor: AllocateRuntimeZeroPool failed, Status = %r\n"
///
/// TbtNvmDrvDmaCtor - AllocateBuffer failed, Status = %r
///
#define EVT_ARGS_TBT_DRV_DMA_CTOR_ALLOCATE_BUF_FAIL          1
#define EVT_CODE_TBT_DRV_DMA_CTOR_ALLOCATE_BUF_FAIL          ((EVT_ID_TBT_DRV_DMA_CTOR_ALLOCATE_BUF_FAIL << 2) | (EVT_ARGS_TBT_DRV_DMA_CTOR_ALLOCATE_BUF_FAIL))
#define STR_ID_TBT_DRV_DMA_CTOR_ALLOCATE_BUF_FAIL            "TbtNvmDrvDmaCtor: AllocateBuffer failed, Status = %r\n"
///
/// TbtNvmDrvDmaCtor - Map failed, Status=%r
///
#define EVT_ARGS_TBT_DRV_DMA_CTOR_MAP_FAIL                   1
#define EVT_CODE_TBT_DRV_DMA_CTOR_MAP_FAIL                   ((EVT_ID_TBT_DRV_DMA_CTOR_MAP_FAIL << 2) | (EVT_ARGS_TBT_DRV_DMA_CTOR_MAP_FAIL))
#define STR_ID_TBT_DRV_DMA_CTOR_MAP_FAIL                     "TbtNvmDrvDmaCtor: Map failed, Status=%r\n"
///
/// TbtNvmDrvDmaCtor - Map returned %x bytes, Status=%r
///
#define EVT_ARGS_TBT_DRV_DMA_CTOR_MAP_RETURN                 2
#define EVT_CODE_TBT_DRV_DMA_CTOR_MAP_RETURN                 ((EVT_ID_TBT_DRV_DMA_CTOR_MAP_RETURN << 2) | (EVT_ARGS_TBT_DRV_DMA_CTOR_MAP_RETURN))
#define STR_ID_TBT_DRV_DMA_CTOR_MAP_RETURN                   "TbtNvmDrvDmaCtor: Map returned %x bytes, Status=%r\n"
///
/// TbtNvmDrvDmaCtor - AllocateRuntimeZeroPool failed, Status=%r
///
#define EVT_ARGS_TBT_DRV_DMA_CTOR_INIT_MEM_FAIL              1
#define EVT_CODE_TBT_DRV_DMA_CTOR_INIT_MEM_FAIL              ((EVT_ID_TBT_DRV_DMA_CTOR_INIT_MEM_FAIL << 2) | (EVT_ARGS_TBT_DRV_DMA_CTOR_INIT_MEM_FAIL))
#define STR_ID_TBT_DRV_DMA_CTOR_INIT_MEM_FAIL                "TbtNvmDrvDmaCtor: AllocateRuntimeZeroPool failed, Status=%r\n"
///< TbtNvmDrvDma.c - end


///< TbtNvmRetimerDrvHelpers.c - start
///
/// TbtNvmDrvWaitForCmdCpl - TbtNvmDrvWaitForCmdCpl() Called
///
#define EVT_ARGS_NVM_IECS_CALL                               0
#define EVT_CODE_NVM_IECS_CALL                               ((EVT_ID_NVM_IECS_CALL << 2) | (EVT_ARGS_NVM_IECS_CALL))
#define STR_ID_NVM_IECS_CALL                                 "TbtNvmDrvWaitForIecsCmdCpl: TbtNvmDrvWaitForCmdCpl() Called.\n"
///
/// TbtNvmDrvWaitForCmdCpl - Check reg for completion
///
#define EVT_ARGS_NVM_IECS_CHECK_REG                          0
#define EVT_CODE_NVM_IECS_CHECK_REG                          ((EVT_ID_NVM_IECS_CHECK_REG << 2) | (EVT_ARGS_NVM_IECS_CHECK_REG))
#define STR_ID_NVM_IECS_CHECK_REG                            "TbtNvmDrvWaitForIecsCmdCpl: Check reg for completion\n"
///
/// TbtNvmDrvWaitForCmdCpl - Got an error while reading from IECS_CMD register, Status - %d,
///
#define EVT_ARGS_NVM_IECS_READ_GOT_ERROR                     1
#define EVT_CODE_NVM_IECS_READ_GOT_ERROR                     ((EVT_ID_NVM_IECS_READ_GOT_ERROR << 2) | (EVT_ARGS_NVM_IECS_READ_GOT_ERROR))
#define STR_ID_NVM_IECS_READ_GOT_ERROR                       "TbtNvmDrvWaitForIecsCmdCpl: Got an error while reading from IECS_CMD register, TbtStatus - %d, "
///
/// TbtNvmDrvWaitForCmdCpl - AccessCount = %d, Cmd = %x. Exiting...
///
#define EVT_ARGS_NVM_IECS_READ_GOT_ERROR1                    2
#define EVT_CODE_NVM_IECS_READ_GOT_ERROR1                    ((EVT_ID_NVM_IECS_READ_GOT_ERROR1 << 2) | (EVT_ARGS_NVM_IECS_READ_GOT_ERROR1))
#define STR_ID_NVM_IECS_READ_GOT_ERROR1                      "AccessCount = %d, Cmd = %x. Exiting...\n"
///
/// TbtNvmDrvWaitForCmdCpl - Remote IECS CMD is timeouted
///
#define EVT_ARGS_NVM_IECS_CMD_TIMEOUT                        2
#define EVT_CODE_NVM_IECS_CMD_TIMEOUT                        ((EVT_ID_NVM_IECS_CMD_TIMEOUT << 2) | (EVT_ARGS_NVM_IECS_CMD_TIMEOUT))
#define STR_ID_NVM_IECS_CMD_TIMEOUT                          "TbtNvmDrvWaitForIecsCmdCpl: Remote IECS CMD is timeouted. AccessCount = %d, ReadData = %x Exiting...\n"
///
/// TbtNvmDrvWaitForCmdCpl - Command not supported. ReadData = %x,
///
#define EVT_ARGS_NVM_IECS_CMD_NOT_SUP                        1
#define EVT_CODE_NVM_IECS_CMD_NOT_SUP                        ((EVT_ID_NVM_IECS_CMD_NOT_SUP << 2) | (EVT_ARGS_NVM_IECS_CMD_NOT_SUP))
#define STR_ID_NVM_IECS_CMD_NOT_SUP                          "TbtNvmDrvWaitForIecsCmdCpl: Command not supported. AccessCount = %d, "
///
/// TbtNvmDrvWaitForCmdCpl - ReadData = %x, Cmd = %x
///
#define EVT_ARGS_NVM_IECS_CMD_NOT_SUP1                       2
#define EVT_CODE_NVM_IECS_CMD_NOT_SUP1                       ((EVT_ID_NVM_IECS_READ_CMD_NOT_SUP1 << 2) | (EVT_ARGS_NVM_IECS_CMD_NOT_SUP1))
#define STR_ID_NVM_IECS_CMD_NOT_SUP1                         "ReadData = %x, Cmd = %x\n"
///
/// TbtNvmDrvWaitForCmdCpl - Got error from remote LC while performing command = %x
///
#define EVT_ARGS_NVM_IECS_CMD_GOT_ERR                        1
#define EVT_CODE_NVM_IECS_CMD_GOT_ERR                        ((EVT_ID_NVM_IECS_CMD_GOT_ERR << 2) | (EVT_ARGS_NVM_IECS_CMD_GOT_ERR))
#define STR_ID_NVM_IECS_CMD_GOT_ERR                          "TbtNvmDrvWaitForIecsCmdCpl: Got error from remote LC while performing command = %x, "
///
/// TbtNvmDrvWaitForCmdCpl - AccessCount = %d, ReadData = %x
///
#define EVT_ARGS_NVM_IECS_CMD_GOT_ERR1                       2
#define EVT_CODE_NVM_IECS_CMD_GOT_ERR1                       ((EVT_ID_NVM_IECS_CMD_GOT_ERR1 << 2) | (EVT_ARGS_NVM_IECS_CMD_GOT_ERR1))
#define STR_ID_NVM_IECS_CMD_GOT_ERR1                         "AccessCount = %d, ReadData = %x\n"
///
/// TbtNvmDrvWaitForCmdCpl - Got an error while reading from CMD register, Status - %d
///
#define EVT_ARGS_NVM_IECS_CMD_READ_GOT_ERR                   1
#define EVT_CODE_NVM_IECS_CMD_READ_GOT_ERR                   ((EVT_ID_NVM_IECS_CMD_READ_GOT_ERR << 2) | (EVT_ARGS_NVM_IECS_CMD_READ_GOT_ERR))
#define STR_ID_NVM_IECS_CMD_READ_GOT_ERR                     "TbtNvmDrvWaitForCmdCpl: Got an error while reading from CMD register, TbtStatus - %d, "
///
/// TbtNvmDrvWaitForCmdCpl - TbtNvmDrvWaitForCmdCpl: CMD is timeouted
///
#define EVT_ARGS_NVM_TBT_CMD_TIMEOUT                         2
#define EVT_CODE_NVM_TBT_CMD_TIMEOUT                         ((EVT_ID_NVM_TBT_CMD_TIMEOUT << 2) | (EVT_ARGS_NVM_TBT_CMD_TIMEOUT))
#define STR_ID_NVM_TBT_CMD_TIMEOUT                           "TbtNvmDrvWaitForCmdCpl: CMD is timeouted. AccessCount = %d, ReadData = %x Exiting...\n"
///
/// TbtNvmDrvWaitForCmdCpl - CMD not supported. AccessCount = %d, ReadData = %x Exiting...
///
#define EVT_ARGS_NVM_TBT_CMD_NOT_SUP                         2
#define EVT_CODE_NVM_TBT_CMD_NOT_SUP                         ((EVT_ID_NVM_TBT_CMD_NOT_SUP << 2) | (EVT_ARGS_NVM_TBT_CMD_NOT_SUP))
#define STR_ID_NVM_TBT_CMD_NOT_SUP                           "TbtNvmDrvWaitForCmdCpl: CMD not supported. AccessCount = %d, ReadData = %x Exiting...\n"
///
/// TbtNvmDrvWaitForCmdCpl - TbtNvmDrvWaitForCmdCpl: CMD 0x%x encountered an error
///
#define EVT_ARGS_NVM_TBT_CMD_DATA_ERR                        1
#define EVT_CODE_NVM_TBT_CMD_DATA_ERR                        ((EVT_ID_NVM_TBT_CMD_DATA_ERR << 2) | (EVT_ARGS_NVM_TBT_CMD_DATA_ERR))
#define STR_ID_NVM_TBT_CMD_DATA_ERR                          "TbtNvmDrvWaitForCmdCpl: CMD 0x%x encountered an error. "
///
/// TbtNvmDrvWaitForCmdCpl - AccessCount = %d, ReadData = %x Exiting...
///
#define EVT_ARGS_NVM_TBT_CMD_DATA_ERR1                       2
#define EVT_CODE_NVM_TBT_CMD_DATA_ERR1                       ((EVT_ID_NVM_TBT_CMD_DATA_ERR1 << 2) | (EVT_ARGS_NVM_TBT_CMD_DATA_ERR1))
#define STR_ID_NVM_TBT_CMD_DATA_ERR1                         "AccessCount = %d, ReadData = %x Exiting...\n"
///
/// TbtNvmDrvSendCmd - Sending CMD 0x%x
///
#define EVT_ARGS_NVM_SEND_IECS_CMD                           1
#define EVT_CODE_NVM_SEND_IECS_CMD                           ((EVT_ID_NVM_SEND_IECS_CMD << 2) | (EVT_ARGS_NVM_SEND_IECS_CMD))
#define STR_ID_NVM_SEND_IECS_CMD                             "Sending CMD 0x%x\n"
///
/// TbtNvmDrvSendCmd - Fail to write to IECS CMD reg, TbtStatus = %d
///
#define EVT_ARGS_NVM_WRITE_IECS_CMD_FAIL                     1
#define EVT_CODE_NVM_WRITE_IECS_CMD_FAIL                     ((EVT_ID_NVM_WRITE_IECS_CMD_FAIL << 2) | (EVT_ARGS_NVM_WRITE_IECS_CMD_FAIL))
#define STR_ID_NVM_WRITE_IECS_CMD_FAIL                       "TbtNvmDrvSendCmd: Fail to write to IECS CMD reg, TbtStatus = %d.\n"
///
/// TbtNvmDrvSendCmd - TbtNvmDrvSendCmd: Fail to write to CMD reg, TbtStatus = %d
///
#define EVT_ARGS_NVM_WRITE_IECS_CMD_REG_FAIL                 1
#define EVT_CODE_NVM_WRITE_IECS_CMD_REG_FAIL                 ((EVT_ID_NVM_WRITE_IECS_CMD_REG_FAIL << 2) | (EVT_ARGS_NVM_WRITE_IECS_CMD_REG_FAIL))
#define STR_ID_NVM_WRITE_IECS_CMD_REG_FAIL                   "TbtNvmDrvSendCmd: Fail to write to CMD reg, TbtStatus = %d.\n"
///
/// TbtNvmDrvSendCmd - Command was 0x%x. Exiting...
///
#define EVT_ARGS_NVM_WRITE_IECS_CMD                          1
#define EVT_CODE_NVM_WRITE_IECS_CMD                          ((EVT_ID_NVM_WRITE_IECS_CMD << 2) | (EVT_ARGS_NVM_WRITE_IECS_CMD))
#define STR_ID_NVM_WRITE_IECS_CMD                            "Command was 0x%x. Exiting...\n"
///
/// TbtNvmDrvDeviceWrBlk - Writing block with length - 0x%x
///
#define EVT_ARGS_NVM_WRITE_BLK_LENGTH                        1
#define EVT_CODE_NVM_WRITE_BLK_LENGTH                        ((EVT_ID_NVM_WRITE_BLK_LENGTH << 2) | (EVT_ARGS_NVM_WRITE_BLK_LENGTH))
#define STR_ID_NVM_WRITE_BLK_LENGTH                          "Writing block with length - 0x%x\n"
///
/// TbtNvmDrvDeviceWrBlk - ERROR! The data to REMOTE_WDATA register wasn't written.
///
#define EVT_ARGS_NVM_WRITE_BLK_WDATA_ERROR                   1
#define EVT_CODE_NVM_WRITE_BLK_WDATA_ERROR                   ((EVT_ID_NVM_WRITE_BLK_WDATA_ERROR << 2) | (EVT_ARGS_NVM_WRITE_BLK_WDATA_ERROR))
#define STR_ID_NVM_WRITE_BLK_WDATA_ERROR                     "TbtNvmDrvDeviceWrBlk: ERROR! The data to REMOTE_WDATA register wasn't written. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrBlk - Sending IECS_BLKW_CMD command failed
///
#define EVT_ARGS_NVM_SEND_BLK_CMD_ERROR                      1
#define EVT_CODE_NVM_SEND_BLK_CMD_ERROR                      ((EVT_ID_NVM_SEND_BLK_CMD_ERROR << 2) | (EVT_ARGS_NVM_SEND_BLK_CMD_ERROR))
#define STR_ID_NVM_SEND_BLK_CMD_ERROR                        "TbtNvmDrvDeviceWrBlk: ERROR! Sending IECS_BLKW_CMD command failed. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrBlk - Sending OPCODE_NVM_WRITE_CMD command failed
///
#define EVT_ARGS_NVM_SEND_BLK_OP_CMD_ERROR                   1
#define EVT_CODE_NVM_SEND_BLK_OP_CMD_ERROR                   ((EVT_ID_NVM_SEND_BLK_OP_CMD_ERROR << 2) | (EVT_ARGS_NVM_SEND_BLK_OP_CMD_ERROR))
#define STR_ID_NVM_SEND_BLK_OP_CMD_ERROR                     "TbtNvmDrvDeviceWrBlk: ERROR! Sending OPCODE_NVM_WRITE_CMD command failed. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrBlk - Finished writing block with length - 0x%x. Status - %d
///
#define EVT_ARGS_NVM_WRITE_BLK_FINISH                        2
#define EVT_CODE_NVM_WRITE_BLK_FINISH                        ((EVT_ID_NVM_WRITE_BLK_FINISH << 2) | (EVT_ARGS_NVM_WRITE_BLK_FINISH))
#define STR_ID_NVM_WRITE_BLK_FINISH                          "Finished writing block with length - 0x%x. TbtStatus - %d\n"
///
/// TbtNvmDrvDeviceWrOffset - Write device OffsetInDW - 0x%x
///
#define EVT_ARGS_NVM_WRITE_OFFSET                            1
#define EVT_CODE_NVM_WRITE_OFFSET                            ((EVT_ID_NVM_WRITE_OFFSET << 2) | (EVT_ARGS_NVM_WRITE_OFFSET))
#define STR_ID_NVM_WRITE_OFFSET                              "Write device OffsetInDW - 0x%x\n"
///
/// TbtNvmDrvDeviceWrOffset - ERROR! Writing data to IECS_METADATA_ADDR register failed
///
#define EVT_ARGS_NVM_WRITE_DATA_ERROR                        1
#define EVT_CODE_NVM_WRITE_DATA_ERROR                        ((EVT_ID_NVM_WRITE_DATA_ERROR << 2) | (EVT_ARGS_NVM_WRITE_DATA_ERROR))
#define STR_ID_NVM_WRITE_DATA_ERROR                          "TbtNvmDrvDeviceWrOffset: ERROR! Writing data to IECS_METADATA_ADDR register failed. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrOffset - ERROR! Sending command IECS_BOPS_CMD failed
///
#define EVT_ARGS_NVM_SEND_CMD_ERROR                          1
#define EVT_CODE_NVM_SEND_CMD_ERROR                          ((EVT_ID_NVM_SEND_CMD_ERROR << 2) | (EVT_ARGS_NVM_SEND_CMD_ERROR))
#define STR_ID_NVM_SEND_CMD_ERROR                            "TbtNvmDrvDeviceWrOffset: ERROR! Sending command IECS_BOPS_CMD failed. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrOffset - ERROR! The data to Metadata register wasn't written
///
#define EVT_ARGS_NVM_WRITE_RG_ERROR                          1
#define EVT_CODE_NVM_WRITE_RG_ERROR                          ((EVT_ID_NVM_WRITE_RG_ERROR << 2) | (EVT_ARGS_NVM_WRITE_RG_ERROR))
#define STR_ID_NVM_WRITE_RG_ERROR                            "TbtNvmDrvDeviceWrOffset: ERROR! The data to Metadata register wasn't written. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceWrOffset - Finished writing device OffsetInDW - 0x%x (%d)
///
#define EVT_ARGS_NVM_WRITE_DV_FINISH                         2
#define EVT_CODE_NVM_WRITE_DV_FINISH                         ((EVT_ID_NVM_WRITE_DV_FINISH << 2) | (EVT_ARGS_NVM_WRITE_DV_FINISH))
#define STR_ID_NVM_WRITE_DV_FINISH                           "Finished writing device OffsetInDW - 0x%x (%d)\n"
///
/// TbtNvmDrvDeviceExecAuth - Sending Auth command
///
#define EVT_ARGS_SEND_AUTH_COMMAND                           0
#define EVT_CODE_SEND_AUTH_COMMAND                           ((EVT_ID_SEND_AUTH_COMMAND << 2) | (EVT_ARGS_SEND_AUTH_COMMAND))
#define STR_ID_SEND_AUTH_COMMAND                             "Sending Auth command\n"
///
/// TbtNvmDrvDeviceExecAuth - Got an error status while writing to IECS_DATA register
///
#define EVT_ARGS_IECS_DATA_GOT_ERROR                         1
#define EVT_CODE_IECS_DATA_GOT_ERROR                         ((EVT_ID_IECS_DATA_GOT_ERROR << 2) | (EVT_ARGS_IECS_DATA_GOT_ERROR))
#define STR_ID_IECS_DATA_GOT_ERROR                           "TbtNvmDrvDeviceExecAuth: Got an error while writing to IECS_DATA register, TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Got an error while writing to IECS_CMD register
///
#define EVT_ARGS_IECS_CMD_GOT_ERROR                          1
#define EVT_CODE_IECS_CMD_GOT_ERROR                          ((EVT_ID_IECS_CMD_GOT_ERROR << 2) | (EVT_ARGS_IECS_CMD_GOT_ERROR))
#define STR_ID_IECS_CMD_GOT_ERROR                            "TbtNvmDrvDeviceExecAuth: Got an error while writing to IECS_CMD register, TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Waiting %d seconds for authentication to complete...
///
#define EVT_ARGS_WAIT_AUTH_SECONDS                           1
#define EVT_CODE_WAIT_AUTH_SECONDS                           ((EVT_ID_WAIT_AUTH_SECONDS << 2) | (EVT_ARGS_WAIT_AUTH_SECONDS))
#define STR_ID_WAIT_AUTH_SECONDS                             "Waiting %d seconds for authentication to complete...\n"
///
/// TbtNvmDrvDeviceExecAuth - The retimer could not enumerate
///
#define EVT_ARGS_RT_ENUMERATE_ERROR                          1
#define EVT_CODE_RT_ENUMERATE_ERROR                          ((EVT_ID_RT_ENUMERATE_ERROR << 2) | (EVT_ARGS_RT_ENUMERATE_ERROR))
#define STR_ID_RT_ENUMERATE_ERROR                            "TbtNvmDrvDeviceExecAuth: The retimer could not enumerate, TbtStatus %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Authentication failed
///
#define EVT_ARGS_AUTH_FAIL                                   1
#define EVT_CODE_AUTH_FAIL                                   ((EVT_ID_AUTH_FAIL << 2) | (EVT_ARGS_AUTH_FAIL))
#define STR_ID_AUTH_FAIL                                     "TbtNvmDrvDeviceExecAuth: Authentication failed!!! TbtStatus from LC - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Got an error while reading from IECS_DATA register
///
#define EVT_ARGS_AUTH_IECS_DATA_GOT_ERROR                    1
#define EVT_CODE_AUTH_IECS_DATA_GOT_ERROR                    ((EVT_ID_AUTH_IECS_DATA_GOT_ERROR << 2) | (EVT_ARGS_AUTH_IECS_DATA_GOT_ERROR))
#define STR_ID_AUTH_IECS_DATA_GOT_ERROR                      "TbtNvmDrvDeviceExecAuth: Got an error while reading from IECS_DATA register, TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Auth has failed with metadata = %x
///
#define EVT_ARGS_AUTH_METADATA_FAIL                          1
#define EVT_CODE_AUTH_METADATA_FAIL                          ((EVT_ID_AUTH_METADATA_FAIL << 2) | (EVT_ARGS_AUTH_METADATA_FAIL))
#define STR_ID_AUTH_METADATA_FAIL                            "TbtNvmDrvDeviceExecAuth: Auth has failed with metadata = %x. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - The data to Metadata register wasn't written
///
#define EVT_ARGS_WRITE_META_REG_ERROR                        1
#define EVT_CODE_WRITE_META_REG_ERROR                        ((EVT_ID_WRITE_META_REG_ERROR << 2) | (EVT_ARGS_WRITE_META_REG_ERROR))
#define STR_ID_WRITE_META_REG_ERROR                          "TbtNvmDrvDeviceExecAuth: ERROR! The data to Metadata register wasn't written. TbtStatus - %d. Exiting...\n"
///
/// TbtNvmDrvDeviceExecAuth - Fail to write to  CMD reg
///
#define EVT_ARGS_WRITE_CMD_REG_ERROR                         1
#define EVT_CODE_WRITE_CMD_REG_ERROR                         ((EVT_ID_WRITE_CMD_REG_ERROR << 2) | (EVT_ARGS_WRITE_CMD_REG_ERROR))
#define STR_ID_WRITE_CMD_REG_ERROR                           "TbtNvmDrvDeviceExecAuth: Fail to write to  CMD reg, TbtStatus = %d.\n"
///
/// TbtNvmDrvDeviceExecAuth - Waiting %d seconds for authentication to complete
///
#define EVT_ARGS_AUTH_WAIT_SECONDS                           1
#define EVT_CODE_AUTH_WAIT_SECONDS                           ((EVT_ID_AUTH_WAIT_SECONDS << 2) | (EVT_ARGS_AUTH_WAIT_SECONDS))
#define STR_ID_AUTH_WAIT_SECONDS                             "Waiting %d seconds for authentication to complete...\n"
///
/// TbtNvmDrvDeviceExecAuth - Authentication success
///
#define EVT_ARGS_AUTH_SUCCESS                                0
#define EVT_CODE_AUTH_SUCCESS                                ((EVT_ID_AUTH_SUCCESS << 2) | (EVT_ARGS_AUTH_SUCCESS))
#define STR_ID_AUTH_SUCCESS                                  "TbtNvmDrvDeviceExecAuth: Authentication success!\n"
///
/// TbtDrvReadDwFromNvm - Reading DW from NVM, offset 0x%x
///
#define EVT_ARGS_NVM_READ_DW_OFFSET                          1
#define EVT_CODE_NVM_READ_DW_OFFSET                          ((EVT_ID_NVM_READ_DW_OFFSET << 2) | (EVT_ARGS_NVM_READ_DW_OFFSET))
#define STR_ID_NVM_READ_DW_OFFSET                            "Reading DW from NVM, offset 0x%x\n"
///
/// TbtDrvReadDwFromNvm - ERROR! Writing data to IECS_METADATA_ADDR register failed
///
#define EVT_ARGS_WRITE_IECS_ERROR                            1
#define EVT_CODE_WRITE_IECS_ERROR                            ((EVT_ID_WRITE_IECS_ERROR << 2) | (EVT_ARGS_WRITE_IECS_ERROR))
#define STR_ID_WRITE_IECS_ERROR                              "TbtDrvReadDwFromNvm: ERROR! Writing data to IECS_METADATA_ADDR register failed. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - ERROR! Sending command IECS_AFRR_CMD failed
///
#define EVT_ARGS_IECS_CMD_ERROR                              1
#define EVT_CODE_IECS_CMD_ERROR                              ((EVT_ID_IECS_CMD_ERROR << 2) | (EVT_ARGS_IECS_CMD_ERROR))
#define STR_ID_IECS_CMD_ERROR                                "TbtDrvReadDwFromNvm: ERROR! Sending command IECS_AFRR_CMD failed. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - ERROR! Failed to read data from IECS_DATA register
///
#define EVT_ARGS_IECS_READ_REG_ERROR                         1
#define EVT_CODE_IECS_READ_REG_ERROR                         ((EVT_ID_IECS_READ_REG_ERROR << 2) | (EVT_ARGS_IECS_READ_REG_ERROR))
#define STR_ID_IECS_READ_REG_ERROR                           "TbtDrvReadDwFromNvm: ERROR! Failed to read data from IECS_DATA register. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - ERROR! The data to DATA register wasn't written
///
#define EVT_ARGS_IECS_WRITE_REG_ERROR                        1
#define EVT_CODE_IECS_WRITE_REG_ERROR                        ((EVT_ID_IECS_WRITE_REG_ERROR << 2) | (EVT_ARGS_IECS_WRITE_REG_ERROR))
#define STR_ID_IECS_WRITE_REG_ERROR                          "TbtDrvReadDwFromNvm: ERROR! The data to DATA register wasn't written. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - The command wasn't send
///
#define EVT_ARGS_SEND_CMD_ERROR                              1
#define EVT_CODE_SEND_CMD_ERROR                              ((EVT_ID_SEND_CMD_ERROR << 2) | (EVT_ARGS_SEND_CMD_ERROR))
#define STR_ID_SEND_CMD_ERROR                                "TbtDrvReadDwFromNvm: ERROR! The command wasn't send. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - ERROR! The read from RDATA has failed
///
#define EVT_ARGS_READ_DATA_ERROR                             1
#define EVT_CODE_READ_DATA_ERROR                             ((EVT_ID_READ_DATA_ERROR << 2) | (EVT_ARGS_READ_DATA_ERROR))
#define STR_ID_READ_DATA_ERROR                               "TbtDrvReadDwFromNvm: ERROR! The read from RDATA has failed. TbtStatus - %d. Exiting...\n"
///
/// TbtDrvReadDwFromNvm - Finished reading DW from NVM offset - 0x%x
///
#define EVT_ARGS_READ_DW_OFFSET                              1
#define EVT_CODE_READ_DW_OFFSET                              ((EVT_ID_READ_DW_OFFSET << 2) | (EVT_ARGS_READ_DW_OFFSET))
#define STR_ID_READ_DW_OFFSET                                "Finished reading DW from NVM offset - 0x%x\n"
///
/// TbtDrvReadNvmVersion - Reading nvm version
///
#define EVT_ARGS_NVM_READ_VERSION                            0
#define EVT_CODE_NVM_READ_VERSION                            ((EVT_ID_NVM_READ_VERSION << 2) | (EVT_ARGS_NVM_READ_VERSION))
#define STR_ID_NVM_READ_VERSION                              "Reading nvm version\n"
///
/// TbtDrvReadNvmVersion - ERROR! Couldn't read from NVM.
///
#define EVT_ARGS_NVM_READ_VERSION_FAIL                       1
#define EVT_CODE_NVM_READ_VERSION_FAIL                       ((EVT_ID_NVM_READ_VERSION_FAIL << 2) | (EVT_ARGS_NVM_READ_VERSION_FAIL))
#define STR_ID_NVM_READ_VERSION_FAIL                         "TbtDrvReadNvmVersion: ERROR! Couldn't read from NVM. TbtStatus %d\n"
///
/// TbtDrvReadNvmVersion - Finished reading NVM version
///
#define EVT_ARGS_NVM_READ_VERSION_FINISH                     1
#define EVT_CODE_NVM_READ_VERSION_FINISH                     ((EVT_ID_NVM_READ_VERSION_FINISH << 2) | (EVT_ARGS_NVM_READ_VERSION_FINISH))
#define STR_ID_NVM_READ_VERSION_FINISH                       "Finished reading NVM version - 0x%x\n"
///< TbtNvmRetimerDrvHelpers.c - end
#endif
