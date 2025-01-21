/** @file
  Include file for Intel Extended ASF definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _INTEL_EXTENDED_ASF_H_
#define _INTEL_EXTENDED_ASF_H_

///
/// Intel extended ASF Messages Command
///
#define ASF_MESSAGE_COMMAND_LAN                       0x06
#define ASF_MESSAGE_COMMAND_KVM                       0x08
#define ASF_MESSAGE_COMMAND_MAC                       0x0A

///
/// Intel extended ASF Messages Sub-Command for Command ASF_CONFIGURATION (0x03)
///
#define ASF_MESSAGE_SUBCOMMAND_REPORT_BIOS_STATUS           0x19
#define ASF_MESSAGE_SUBCOMMAND_SET_UEFI_BOOT_OPTIONS_STATE  0x20
#define ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_OPTIONS_STATE  0x21
#define ASF_MESSAGE_SUBCOMMAND_UPDATE_UEFI_BOOT_OPTIONS     0x22
#define ASF_MESSAGE_SUBCOMMAND_GET_UEFI_BOOT_PARAMETERS     0x23
#define ASF_MESSAGE_SUBCOMMAND_GET_ROOT_CA_CERTIFICATE      0x24
#define ASF_MESSAGE_SUBCOMMAND_GET_WIFI_PROFILE_NAME        0x25
#define ASF_MESSAGE_SUBCOMMAND_GET_WIFI_PROFILE_DATA        0x26
#define ASF_MESSAGE_SUBCOMMAND_GET_WIFI_8021X_CLIENT_CERT   0x27
#define ASF_MESSAGE_SUBCOMMAND_GET_WIFI_8021X_CLIENT_KEY    0x28
#define ASF_MESSAGE_SUBCOMMAND_WRITE_FILE                   0x29
#define ASF_MESSAGE_SUBCOMMAND_GET_HTTP_PROXY               0x31

///
/// Intel extended ASF Messages Sub-Command for Command LAN (0x06)
///
#define ASF_MESSAGE_SUBCOMMAND_SET_ACTIVE_LAN         0x01
#define ASF_MESSAGE_SUBCOMMAND_GET_ACTIVE_LAN         0x02

///
/// Intel extended ASF Messages Sub-Command for Command KVM (0x08)
///
#define ASF_MESSAGE_SUBCOMMAND_KVM_QUERY              0x01

///
/// Intel extended ASF Messages Sub-Command for Command MAC (0x0A)
///
#define ASF_MESSAGE_SUBCOMMAND_SET_MAC_PASSTHROUGH    0x01

///
/// Intel extended ASF Entity Codes
///
#define ASF_ENTITY_INTEL_AMT                          0x26

///
/// Intel extended ASF Event Data Codes for System Firmware Progress Events
///
#define ASF_FP_AMTBX_INIT                             0x0D

///
/// Intel extended ASF Event Data Codes for System Firmware Error Events
///
#define ASF_FE_AMTBX_FAILURE                          0x00

///
/// Intel extended ASF Event Data Codes for RPE Fw Progress/Error Events
///
#define ASF_RPE_EVENT_OFFSET_PROGRESS                         0x05
#define ASF_RPE_EVENT_OFFSET_FAILURE                          0x03

#define ASF_RPE_EVENT_DATA0_EVENT_DATA_SET_BY_OEM             0xAA
#define ASF_RPE_EVENT_DATA2_REMOTE_PLATFORM_ERASE             0x40

#define ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVED           0x00
#define ASF_RPE_EVENT_DATA3_STARTED_ERASING_DEVICE            0x01
#define ASF_RPE_EVENT_DATA3_DEVICE_ERASED_SUCCESSFULLY        0x02
#define ASF_RPE_EVENT_DATA3_BEGINNING_PLATFORM_ERASE          0x03
#define ASF_RPE_EVENT_DATA3_CLEAR_RESERVED_PARAMETERS         0x04
#define ASF_RPE_EVENT_DATA3_ALL_SETTINGS_DECREMENTED          0x05
#define ASF_RPE_EVENT_DATA3_PSID_NOT_RECEIVED                 0x06

#define ASF_RPE_EVENT_DATA3_BOOT_PARAMETER_RECEIVE_FAILED     0x00
#define ASF_RPE_EVENT_DATA3_ERROR_ERASING_DEVICE              0x01
#define ASF_RPE_EVENT_DATA3_ERASING_DEVICE_UNSUPPORTED        0x02
#define ASF_RPE_EVENT_DATA3_REACHED_MAX_RETRY_COUNT           0x03
#define ASF_RPE_EVENT_DATA3_INVALID_BOOT_PARAMETERS           0x04
#define ASF_RPE_EVENT_DATA3_INVALID_MAX_COUNTER               0x05

#define ASF_RPE_EVENT_DATA4_PYRITE_ERASE_ACTION               0x01
#define ASF_RPE_EVENT_DATA4_SSD_ERASE_ACTION                  0x02
#define ASF_RPE_EVENT_DATA4_TPM_ERASE_ACTION                  0x03
#define ASF_RPE_EVENT_DATA4_CLEAR_BIOS_NVM_VARIABLE           0x04
#define ASF_RPE_EVENT_DATA4_RELOAD_BIOS_GOLDEN_CONFIG         0x05
#define ASF_RPE_EVENT_DATA4_OEM_CUSTOM_ACTIONS                0x06
#define ASF_RPE_EVENT_DATA4_CSME_UNCONFIGURE                  0x07

///
/// Intel extended ASF Report BIOS Status Additional Data
///
#define ASF_RBS_BIOS_STATUS_SUCCESS                  0x0000
#define ASF_RBS_BIOS_STATUS_IN_PROGRESS              0x0001
#define ASF_RBS_BIOS_STATUS_NOT_REPORTED             0x0002
#define ASF_RBS_BIOS_STATUS_FAILED                   0xFFFF

#define ASF_RBS_STATUS_CAUSE_SUCCESS_INPROGRESS      0x0000
#define ASF_RBS_STATUS_CAUSE_GENERAL_FAILURE         0x0001
#define ASF_RBS_STATUS_CAUSE_AUTHENTICATION_FAILURE  0x0002
#define ASF_RBS_STATUS_CAUSE_UNSUPPORTED             0x0003

typedef enum {
  AsfRbsSuccess        = (ASF_RBS_STATUS_CAUSE_SUCCESS_INPROGRESS      << 16) | ASF_RBS_BIOS_STATUS_SUCCESS,
  AsfRbsInProgress     = (ASF_RBS_STATUS_CAUSE_SUCCESS_INPROGRESS      << 16) | ASF_RBS_BIOS_STATUS_IN_PROGRESS,
  AsfRbsGeneralFailure = (ASF_RBS_STATUS_CAUSE_GENERAL_FAILURE         << 16) | ASF_RBS_BIOS_STATUS_FAILED,
  AsfRbsAccessDenied   = (ASF_RBS_STATUS_CAUSE_AUTHENTICATION_FAILURE  << 16) | ASF_RBS_BIOS_STATUS_FAILED,
  AsfRbsUnsupported    = (ASF_RBS_STATUS_CAUSE_UNSUPPORTED             << 16) | ASF_RBS_BIOS_STATUS_FAILED
} ASF_REPORT_BIOS_STATUS;

///
/// Intel extended ASF Event Data Codes for OCR Fw Progress/Error Events
///
#define ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_ERROR      0x03
#define ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS   0x05

#define ASF_OCR_EVENT_DATA1_EVENT_DATA_SET_BY_OEM      0xAA

#define ASF_OCR_EVENT_DATA2_EVENT_DATA_REASON          0x30

///
/// OCR Fw Progress Events
///
#define ASF_OCR_EVENT_DATA3_BOOT_PARAMETER_RECEIVED    0x00
#define ASF_OCR_EVENT_DATA3_CSME_BOOT_OPTION_ADDED     0x01
#define ASF_OCR_EVENT_DATA3_HTTPS_URI_NAME_RESOLVED    0x02
#define ASF_OCR_EVENT_DATA3_HTTPS_CONNECTED_SUCCESS    0x03
#define ASF_OCR_EVENT_DATA3_HTTPS_BOOT_DOWNLOAD        0x04
#define ASF_OCR_EVENT_DATA3_ATTEMPT_TO_BOOT            0x05
#define ASF_OCR_EVENT_DATA3_EXIT_BOOT_SERVICES         0x06

///
/// OCR Fw Error Events
///
#define ASF_OCR_EVENT_DATA3_SUCCESS                    0x00
#define ASF_OCR_EVENT_DATA3_NO_NETWORK_CONNECTION      0x01
#define ASF_OCR_EVENT_DATA3_NAME_RESOLUTION_URI_FAILED 0x02
#define ASF_OCR_EVENT_DATA3_CONNECT_TO_URI_FAILED      0x03
#define ASF_OCR_EVENT_DATA3_FILEPATH_NOT_FOUND         0x04
#define ASF_OCR_EVENT_DATA3_HTTPS_TLS_AUTH_FAILED      0x05
#define ASF_OCR_EVENT_DATA3_HTTPS_DIGEST_AUTH_FAILED   0x06
#define ASF_OCR_EVENT_DATA3_VERIFIED_BOOT_FAILED       0x07
#define ASF_OCR_EVENT_DATA3_HTTPS_BOOT_FILE_NOT_FOUND  0x08

//
// AMT BX Software Class DXE Subclass Progress Code definitions.
//
#define SW_DXE_AMTBX_OPROM_DONE               (EFI_OEM_SPECIFIC | 0x00000000)
#define SW_DXE_AMTBX_OPROM_NOT_STARTED        (EFI_OEM_SPECIFIC | 0x00000001)
#define SW_DXE_AMTBX_SLEEP_S4_NOT_TAKEN       (EFI_OEM_SPECIFIC | 0x00000002)
#define SW_DXE_AMTBX_SLEEP_S5_NOT_TAKEN       (EFI_OEM_SPECIFIC | 0x00000003)
#define SW_DXE_AMTBX_SLEEP_UNSPECD_NOT_TAKEN  (EFI_OEM_SPECIFIC | 0x00000004)
#define SW_DXE_AMTBX_ERROR_ACTION_INTF        (EFI_OEM_SPECIFIC | 0x00000005)
#define SW_DXE_AMTBX_ERROR_ACTION_INV         (EFI_OEM_SPECIFIC | 0x00000006)
#define SW_DXE_AMTBX_ERROR_ACTION_SETUP       (EFI_OEM_SPECIFIC | 0x00000007)
#define SW_DXE_AMTBX_ERROR_ACTION_FAIL        (EFI_OEM_SPECIFIC | 0x00000008)
#define SW_DXE_AMTBX_ERROR_ACTION_UNSPECD     (EFI_OEM_SPECIFIC | 0x00000009)
#define SW_DXE_AMTBX_OTHER_UNSPECD            (EFI_OEM_SPECIFIC | 0x00000009)

typedef enum {
  AsfMessageBiosPresent,
  AsfMessageMemInit,
  AsfMessageMemInitDone,
  AsfMessageHddInit,
  AsfMessageApInit,
  AsfMessageUserAuthentication,
  AsfMessageUserInitSetup,
  AsfMessageUsbResourceConfig,
  AsfMessagePciResourceConfig,
  AsfMessageOptionRomInit,
  AsfMessageVideoInit,
  AsfMessageCacheInit,
  AsfMessageSmbusInit,
  AsfMessageKbcInit,
  AsfMessageEmControllerInit,
  AsfMessageDockAttached,
  AsfMessageEnableDock,
  AsfMessageDockEject,
  AsfMessageDisableDock,
  AsfMessageOSWakeVector,
  AsfMessageMotherBoardInit,
  AsfMessageFdcInit,
  AsfMessageKeyboardTest,
  AsfMessageMouseTest,
  AsfMessageBspInit,
  AsfMessageAmtBxDone,
  AsfMessageAmtBxNotStarted,
  AsfMessageAmtBxSleepS4ReportedNotTaken,
  AsfMessageAmtBxSleepS5ReportedNotTaken,
  AsfMessageAmtBxSleepUnspecifiedReportedNotTaken,
  AsfMessageAmtBxErrActionIntF,
  AsfMessageAmtBxErrActionInv,
  AsfMessageAmtBxErrActionSetup,
  AsfMessageAmtBxErrActionFailure,
  AsfMessageAmtBxErrActionUnspecified,
  AsfMessageAmtBxOtherUnspecified,
  AsfMessageNoMemory,
  AsfMessageMemoryFailure,
  AsfMessageHddFailure,
  AsfMessageBoardFailure,
  AsfMessageFdcFailure,
  AsfMessageHdcFailure,
  AsfMessageKbdFailure,
  AsfMessageNoFdd,
  AsfMessageVideoControllerFailure,
  AsfMessageNoVideo,
  AsfMessageFirmwareCorruption,
  AsfMessageCpuVidMismatch,
  AsfMessageCpuSpeedMatchingFailure,
  AsfMessageUserAuthenticationInvalid,
  AsfMessageHddAuthenticationInvalid,
  AsfMessageNoBootMedia,
  AsfMessageTemperatureGenericCritical,
  AsfMessageTemperatureGenericWarning,
  AsfMessageTemperatureOverCritical,
  AsfMessageTemperatureOverWarning,
  AsfMessageTemperatureUnderCritical,
  AsfMessageTemperatureUnderWarning,
  AsfMessageVoltageGenericCritical,
  AsfMessageVoltageOverCritical,
  AsfMessageVoltageUnderCritical,
  AsfMessageFanGenericCritical,
  AsfMessageFanPredictiveFailure,
  AsfMessageFanLowSpeedCritical,
  AsfMessageFanLowSpeedWarning,
  AsfMessageBatteryLowWarning,
  AsfMessageBatteryLowCritical,
  AsfMessageBatteryPresenceDetected,
  AsfMessagePowerFailure,
  AsfMessageChassisIntrusion,
  AsfMessageTermination = 0xFFFF
} ASF_FRAMEWORK_MESSAGE_TYPE;

///
/// Intel extended StatusCode extension for ASF
///


///
/// Intel extended Computing Unit Host Processor Subclass Error Code definitions.
///
#define CU_HP_EC_VID_MISMATCH                   (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define CU_HP_EC_SPEED_MATCHING_FAILURE         (EFI_SUBCLASS_SPECIFIC | 0x0000000F)

///
/// Intel extended Peripheral Subclass definitions.
///
#define PERIPHERAL_EMBEDDED_CONTROLLER          (EFI_PERIPHERAL | 0x000D0000)
#define PERIPHERAL_DOCK                         (EFI_PERIPHERAL | 0x000E0000)

#define PERIPHERAL_BATTERY                      (EFI_PERIPHERAL | 0x000F0000)
#define P_BATTERY_PC_LOW_WARNING                (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_BATTERY_EC_LOW_CRITICAL               (EFI_SUBCLASS_SPECIFIC | 0x00000000)

#define PERIPHERAL_FAN                          (EFI_PERIPHERAL | 0x00100000)
#define P_FAN_EC_GENERIC_CRITICAL               (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_FAN_EC_PREDICTIVE_FAILURE             (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define P_FAN_EC_LOW_SPEED_CRITICAL             (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define P_FAN_PC_LOW_SPEED_WARNING              (EFI_SUBCLASS_SPECIFIC | 0x00000002)

#define PERIPHERAL_TEMPERATURE                  (EFI_PERIPHERAL | 0x00110000)
#define P_TEMERATURE_EC_GENERIC_CRITICAL        (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_TEMERATURE_PC_GENERIC_WARNING         (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_TEMERATURE_EC_OVER_CRITICAL           (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define P_TEMERATURE_PC_OVER_WARNING            (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define P_TEMERATURE_EC_UNDER_CRITICAL          (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define P_TEMERATURE_PC_UNDER_WARNING           (EFI_SUBCLASS_SPECIFIC | 0x00000002)

#define PERIPHERAL_VOLTAGE                      (EFI_PERIPHERAL | 0x00120000)
#define P_VOLTAGE_EC_GENERIC_CRITICAL           (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_VOLTAGE_EC_OVER_CRITICAL              (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define P_VOLTAGE_EC_UNDER_CRITICAL             (EFI_SUBCLASS_SPECIFIC | 0x00000002)

#define PERIPHERAL_POWER_SUPPLY                 (EFI_PERIPHERAL | 0x00130000)
#define P_POWER_SUPPLY_EC_FAILURE               (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define P_POWER_SUPPLY_EC_PREDICTIVE_FAILURE    (EFI_SUBCLASS_SPECIFIC | 0x00000001)

///
/// Intel extended Computing Unit Chipset Subclass Progress Code definitions.
///
#define CU_CHIPSET_PC_INTRUDER_DETECT           (EFI_SUBCLASS_SPECIFIC | 0x00000000)

///
/// Intel extended Software Class DXE Boot Service Driver Subclass Error Code definitions.
///
#define SW_DXE_BS_EC_NO_BOOT_MEDIA              (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define SW_DXE_BS_EC_AUTHENTICATE_USER_INVALID  (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define SW_DXE_BS_EC_AUTHENTICATE_HDD_INVALID   (EFI_SUBCLASS_SPECIFIC | 0x00000003)

#endif
