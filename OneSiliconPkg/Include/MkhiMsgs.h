/** @file
  MKHI Messages

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _MKHI_MSGS_H
#define _MKHI_MSGS_H

#include <HeciAddresses.h>

#pragma pack(1)

///
/// MKHI message header. MKHI supports CSME's Bringup and Policy processes only
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  GroupId    : 8;
    UINT32  Command    : 7;
    UINT32  IsResponse : 1;
    UINT32  Reserved   : 8;
    UINT32  Result     : 8;
  } Fields;
} MKHI_MESSAGE_HEADER;


///
/// Defines for GroupID
///
#define MKHI_CBM_GROUP_ID     0x00
#define MKHI_PWD_GROUP_ID     0x02
#define MKHI_FWCAPS_GROUP_ID  0x03
#define MKHI_HMRFPO_GROUP_ID  0x05
#define MKHI_GEN_GROUP_ID     0xFF


///
/// Defines for Command
///
///
/// Defines for CBM_GROUP Command
///
#define CBM_RESET_CMD                     0x0B
#define CBM_GET_INVOCATION_CODE           0x0D
#define CBM_SET_INVOCATION_CODE           0x0E
#define CBM_CLR_INVOCATION_CODE           0x0F
///
/// Defines for PWD_GROUP Command
///
#define PWD_CHANGE_CMD                    0x01
#define PWD_VALIDATE_CMD                  0x02
#define PWD_IS_MODIFIED_CMD               0x03
///
/// Defines for FWCAPS_GROUP Command
///
#define FWCAPS_GET_RULE_CMD               0x02
#define FWCAPS_SET_RULE_CMD               0x03
#define FWCAPS_GET_RULE_STATE_CMD         0x05
///
/// Defines for HMRFPO_GROUP Command
///
#define HMRFPO_ENABLE_CMD_ID              0x01
#define HMRFPO_LOCK_CMD_ID                0x02
#define HMRFPO_GET_STATUS_CMD_ID          0x03

///
/// Defines for GEN_GROUP Command
///
#define GEN_GET_MKHI_VERSION_CMD                  0x01
#define GEN_GET_FW_VERSION_CMD                    0x02
#define GEN_CORE_BIOS_DONE_CMD                    0x05
#define GEN_SEND_CPU_BRAND_CLASS_FUSE_CMD         0x07
#define GEN_GET_CPU_TYPE_CHANGE_USER_MESSAGE_CMD  0x08
#define GEN_SET_CPU_TYPE_CHANGE_USER_RESPONSE_CMD 0x09
#define GEN_END_OF_POST_CMD                       0x0C
#define GEN_UNCFG_WO_PWD_CMD                      0x0D
#define GEN_GET_ME_UNCFG_STATE_CMD                0x0E
#define GEN_SET_ME_RESET_HALT_CMD                 0x10
#define GEN_FW_FEATURE_STATE_OVERRIDE_CMD         0x14
#define GEN_GET_IMAGE_FW_VERSION                  0x1C
#define GEN_SET_FIPS_MODE_CMD                     0x20
#define GEN_GET_FIPS_MODE_CMD                     0x21
#define GEN_SET_MEASURED_BOOT_STATE_CMD           0x22
#define GEN_GET_MEASURED_BOOT_STATE_CMD           0x23
#define GEN_READ_FROM_SPI                         0x24
#define GEN_SET_STRAP_OVERRIDE_CMD                0x26
#define GEN_GET_STRAP_OVERRIDE_CMD                0x27

//
// Typedef for Result field of MHKI Header
//
typedef enum {
  MkhiStatusSuccess,
  MkhiStatusInvalidState,
  MkhiStatusMessageSkipped,
  MkhiStatusSizeError       = 0x5,
  MkhiStatusNotFound        = 0x81,
  MkhiStatusInvalidAccess   = 0x84,
  MkhiStatusInvalidParams   = 0x85,
  MkhiStatusNotReady        = 0x88,
  MkhiStatusNotSupported    = 0x89,
  MkhiStatusInvalidAddress  = 0x8C,
  MkhiStatusInvalidCommand  = 0x8D,
  MkhiStatusFailure         = 0x9E,
  MkhiStatusInvalidResource = 0xE4,
  MkhiStatusResourceInUse   = 0xE5,
  MkhiStatusNoResource      = 0xE6,
  MkhiStatusDuplicated      = 0xE8,
  MkhiStatusRetry           = 0xFE,
  MkhiStatusGeneralError    = 0xFF
} MKHI_RESULT;

///
/// MKHI_CBM_GROUP Definitions
///
///
/// Reset request message data
///
//
// Reset Request Origin Codes.
//
#define CBM_RR_REQ_ORIGIN_BIOS_MEMORY_INIT 0x01
#define CBM_RR_REQ_ORIGIN_BIOS_POST        0x02
#define CBM_RR_REQ_ORIGIN_MEBX             0x03

//
// Reset Type Codes.
//
#define CBM_HRR_GLOBAL_RESET               0x01
#define CBM_HRR_HOST_RESET                 0x02
#define CBM_HRR_CSME_RESET                 0x03

//
// Reset Response Codes.
//
#define CBM_HRR_RES_REQ_NOT_ACCEPTED       0x01

typedef struct {
  UINT8 RequestOrigin;
  UINT8 ResetType;
} CBM_RESET_REQ_DATA;

///
/// Reset request MKHI message
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  CBM_RESET_REQ_DATA  Data;
} CBM_RESET_REQUEST;

///
/// Reset request MKHI ack message
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} CBM_RESET_RESPONSE;

typedef union {
  CBM_RESET_REQUEST   Request;
  CBM_RESET_RESPONSE  Response;
} CBM_RESET_BUFFER;

typedef union {
  UINT32 Data;
  struct {
    UINT32 TableSyncError        : 1;   ///< BIT_0  No longer used
    UINT32 CpuReplacement        : 1;   ///< BIT_1
    UINT32 SyncMngState          : 1;   ///< BIT_2
    UINT32 SolStateChange        : 1;   ///< BIT_3  No longer used
    UINT32 IderStateChange       : 1;   ///< BIT_4  No longer used
    UINT32 KvmStateChange        : 1;   ///< BIT_5  No longer used
    UINT32 SyncRaAvailability    : 1;   ///< BIT_6
    UINT32 FwUnconfiguration     : 1;   ///< BIT_7
    UINT32 UserConsentFlow       : 1;   ///< BIT_8
    UINT32 WlanPowerConfigChange : 1;   ///< BIT_9
    UINT32 FwUpdate              : 1;   ///< BIT_10 No longer used
    UINT32 FirstMebxBoot         : 1;   ///< BIT_11
    UINT32 DataSyncConfirmation  : 1;   ///< BIT_12
    UINT32 ClsUserInfo           : 1;   ///< BIT_13 No longer used
    UINT32 Reserved2             : 18;  ///< BIT_14-31
  } Fields;
} INVOCATION_CODE_BITS;

#define DATA_SYNC_CONFIRMATION   BIT12

//
// Get Invocation Code
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GET_INVOCATION_CODE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              InvocationCode;
} GET_INVOCATION_CODE_RESPONSE;

typedef union {
  GET_INVOCATION_CODE_REQUEST   Request;
  GET_INVOCATION_CODE_RESPONSE  Response;
} GET_INVOCATION_CODE_BUFFER;

//
// Set Invocation Code
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              InvocationCode;
} SET_INVOCATION_CODE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} SET_INVOCATION_CODE_RESPONSE;

typedef union {
  SET_INVOCATION_CODE_REQUEST   Request;
  SET_INVOCATION_CODE_RESPONSE  Response;
} SET_INVOCATION_CODE_BUFFER;

//
// Clear Invocation Code
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              InvocationCode;
} CLEAR_INVOCATION_CODE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} CLEAR_INVOCATION_CODE_RESPONSE;

typedef union {
  CLEAR_INVOCATION_CODE_REQUEST   Request;
  CLEAR_INVOCATION_CODE_RESPONSE  Response;
} CLEAR_INVOCATION_CODE_BUFFER;

//
// MKHI_PWD_GROUP_ID Definitions
//
//
// Password Information
// The following is the information required for authenticating and storing a password.
//
// Password Identifier
// Identifier to track passwords used by various entities interacting with ME Kernel
//
#define MEPWDID_ME_BIOS_EXT    0    ///< MEBx password Identifier

#define MEPWDMGMT_PWD_MAX_SIZE 32   ///< Maximum Password Length
#define MEPWDMGMT_PWD_MIN_SIZE 8    ///< Minimum Password Length

//
// Password Management Status Codes.
//
typedef enum {
  ME_PWDSTS_PWD_SUCCESS = 0,        ///< Password change/verification was successful.
  ME_PWDSTS_INVALID_PWD,            ///< Current password in use is invalid.
  ME_PWDSTS_INVALID_NEW_PWD,        ///< New password provided does not meet the required syntax.
  ME_PWDSTS_OUT_OF_RESOURCES,       ///< Not sufficient flash space.
  ME_PWDSTS_OPERATION_NOT_ALLOWED,  ///< Password change/verification cannot be performed at this point in time.
  ME_PWDSTS_FAILURE                 ///< Unknown failure has occured
} ME_PWD_STATUS;

//
// Change Password
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              PwdId;
  UINT8               Reserved[2];
  UINT8               OldPwdSize;
  UINT8               OldPwd[MEPWDMGMT_PWD_MAX_SIZE];
  UINT8               NewPwdSize;
  UINT8               NewPwd[MEPWDMGMT_PWD_MAX_SIZE];
} PWD_CHANGE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              PwdId;
} PWD_CHANGE_RESPONSE;

typedef union {
  PWD_CHANGE_REQUEST  Request;
  PWD_CHANGE_RESPONSE Response;
} PWD_CHANGE_BUFFER;

//
// Validate Password
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              PwdId;
  UINT8               Reserved[3];
  UINT8               PwdSize;
  UINT8               Pwd[MEPWDMGMT_PWD_MAX_SIZE];
} PWD_VALIDATE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              PwdId;
  UINT32              MaxCountExpired;
} PWD_VALIDATE_RESPONSE;

typedef union {
  PWD_VALIDATE_REQUEST  Request;
  PWD_VALIDATE_RESPONSE Response;
} PWD_VALIDATE_BUFFER;

//
// Is Password Modified
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} PWD_IS_MODIFIED_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              IsModified;
} PWD_IS_MODIFIED_RESPONSE;

typedef union {
  PWD_IS_MODIFIED_REQUEST  Request;
  PWD_IS_MODIFIED_RESPONSE Response;
} PWD_IS_MODIFIED_BUFFER;

//
// MKHI_FWCAPS_GROUP_ID Definitions
//

#define RULE_DATA_LENGTH               0x04

//Macro to build a rule identifier. For Me rules all other fields are zeros
#define FWCAPS_RULE_ID(FeatureId, RuleTypeId) ((FeatureId << 16) | RuleTypeId)

typedef enum {
  KernelFeatureId = 0,
  AmtFeatureId    = 3
} FWCAPS_FEATURE_ID;

typedef enum {
  FwCapsRuleTypeId              = 0,
  MePlatformStateRuleTypeId     = 6,
  MeFwUpdateRuleTypeId          = 7,
  UserCapsRuleTypeId            = 9,
  PlatTypeRuleTypeId            = 29,
  FeatureStateRuleTypeId        = 32,
  StateForAllFeaturesRuleTypeId = 33,
  AvailableBitmapRuleTypeId     = 39,
  OemTagRuleTypeId              = 43,
  AcmTpmRuleTypeId              = 47,
  UnconfigOnRtcClarRuleTypeId   = 48
} KERNEL_FEATURE_RULE_TYPE_ID;

typedef enum {
  AmtBiosSyncInfoRuleTypeId     = 5,
  DisplayUserConsentRuleTypeId  = 10
} AMT_FEATURE_RULE_TYPE_ID;

typedef union {
  UINT32  Data;
  struct {
    UINT32  RuleTypeId : 16;
    UINT32  FeatureId  : 8;
    UINT32  Reserved   : 8;
  } Fields;
} RULE_ID;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              RuleId;
} GET_RULE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              RuleId;
  UINT8               RuleDataLen;
  UINT32              RuleData;
} GET_RULE_RESPONSE;

typedef union {
  GET_RULE_REQUEST    Request;
  GET_RULE_RESPONSE   Response;
} GET_RULE_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              RuleId;
  UINT8               RuleDataLen;
  UINT32              RuleData;
} SET_RULE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              RuleId;
} SET_RULE_RESPONSE;

typedef union {
  SET_RULE_REQUEST    Request;
  SET_RULE_RESPONSE   Response;
} SET_RULE_BUFFER;

///
/// Get Firmware Capability
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  FullMng         : 1;      ///< [0] Full Manageability
    UINT32  StdMng          : 1;      ///< [1] Standard Manageability
    UINT32  Amt             : 1;      ///< [2] Any Manageability
    UINT32  OCR             : 1;      ///< [3] One Click Recovery (OCR)
    UINT32  RPE             : 1;      ///< [4] Remote Platform Erase (RPE)
    UINT32  PSR             : 1;      ///< [5] Platform Service Record Support (PSR)
    UINT32  Reserved1       : 4;      ///< [9:6] Reserved
    UINT32  ISH             : 1;      ///< [10] IntelR Sensor Hub
    UINT32  Reserved2       : 6;      ///< [16:11] Reserved
    UINT32  IPV6            : 1;      ///< [17] IPV6
    UINT32  KVM             : 1;      ///< [18] KVM Remote Control (KVM)
    UINT32  Reserved3       : 2;      ///< [20:19] Reserved
    UINT32  TLS             : 1;      ///< [21] Cipher Transport Layer (TLS)
    UINT32  Reserved4       : 1;      ///< [22] Reserved
    UINT32  WLAN            : 1;      ///< [23] Wireless LAN (WLAN)
    UINT32  Reserved5       : 3;      ///< [26:24] Reserved
    UINT32  TbtDock         : 1;      ///< [27] TBT Dock
    UINT32  Upid            : 1;      ///< [28] UPID
    UINT32  Reserved6       : 3;      ///< [31:29] Reserved
  } Fields;
} MEFWCAPS_SKU;

///
/// Get/Set Local FW Update
///
typedef enum {
  UpdateDisabled,
  UpdateEnabled
} LOCAL_FW_UPDATE;

///
/// Get Platform Type
///

/// [3:0]
typedef enum {
  PlatformMobile          = 1,
  PlatformDesktop         = 2,
  PlatformServer          = 4,
  PlatformWorkstation     = 8
} PLATFORM_TARGET_USAGE_TYPE;

/// [6]
typedef enum {
  RegularSku,
  SuperSku
} SKU_TYPE;

/// [11:8]
typedef enum {
  IntelMeConsumerFw        = 3,
  IntelMeCorporateFw       = 4,
  IntelMeLiteFw            = 5
} ME_IMAGE_TYPE;

/// [15:12]
typedef enum {
  NoBrand,
  IntelAmtBrand,
  IntelStandardManageabilityBrand,
  IntelReservedBrand1,
  IntelReservedBrand2,
  IntelReservedBrand3
} PLATFORM_BRAND;

typedef union {
  UINT32  Data;
  struct {
    UINT32  PlatformTargetUsageType : 4;
    UINT32  Reserved                : 2;
    UINT32  SuperSku                : 1;
    UINT32  Reserved1               : 1;
    UINT32  IntelMeFwImageType      : 4;
    UINT32  PlatformBrand           : 4;
    UINT32  Reserved2               : 16;
  } Fields;
} PLATFORM_TYPE_RULE_DATA;

///
/// Set ME Disable
///

#define ME_DISABLE         0

///
/// Set ACM TPM
///

typedef union {
  UINT32 Data;
  struct {
    UINT32 Reserved      : 8;
    UINT32 TpmDeactivate : 1;
    UINT32 Reserved1     : 23;
  } Fields;
} TPM_STATE;

///
/// Unconfig On Rtc Clear Status
///
typedef enum {
  EnableState  = 0,  ///< [Unconfigure on RTC Clear] is enabled.
  DisableState = 1   ///< [Unconfigure on RTC Clear] is disabled.
} ME_UNCONFIG_ON_RTC_CLEAR_STATUS;

typedef enum {
  AmtProvisioningStatePre  = 0,
  AmtProvisioningStateIn   = 1,
  AmtProvisioningStatePost = 2
} AMT_PROVISIONING_STATE;

typedef struct {
  UINT32 Reserved                : 1;
  UINT32 CiraAvailable           : 1; ///< CIRA Available
  UINT32 ProvisioningAvailable   : 1; ///< Corporate Provisioning Available
  UINT32 ProvisioningState       : 2; ///< Provisioning state. @see AMT_PROVISIONING_STATE
  UINT32 Reserved1               : 27;
} AMT_BIOS_SYNC_INFO;

//
// MKHI_HMRFPO_GROUP_ID Definitions
//

///
/// HMRFPO ENABLE
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              Reserved[2];
} HMRFPO_ENABLE_REQUEST;

///
/// HMRFPO_ENABLE returned Status
///
#define HMRFPO_ENABLE_SUCCESS         0x00
#define HMRFPO_ENABLE_LOCKED          0x01
#define HMRFPO_NVAR_FAILURE           0x02
#define HMRFPO_ENABLE_UNKNOWN_FAILURE 0x05

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              Reserved[2];
  UINT8               Status;
  UINT8               Reserved1[3];
} HMRFPO_ENABLE_RESPONSE;

typedef union {
  HMRFPO_ENABLE_REQUEST   Request;
  HMRFPO_ENABLE_RESPONSE  Response;
} HMRFPO_ENABLE_BUFFER;

///
/// HMRFPO LOCK
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} HMRFPO_LOCK_REQUEST;

///
/// HMRFPO_LOCK_RESPONSE returned Status
///
#define HMRFPO_LOCK_SUCCESS 0x00
#define HMRFPO_LOCK_FAILURE 0x01

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              Reserved[4];
  UINT8               Status;
  UINT8               Reserved1[3];
} HMRFPO_LOCK_RESPONSE;

typedef union {
  HMRFPO_LOCK_REQUEST   Request;
  HMRFPO_LOCK_RESPONSE  Response;
} HMRFPO_LOCK_BUFFER;

///
/// HMRFPO GET STATUS
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} HMRFPO_GET_STATUS_REQUEST;

///
/// HMRFPO_GET_STATUS returned Status
///
#define HMRFPO_DISABLED         0x00
#define HMRFPO_LOCKED           0x01
#define HMRFPO_ENABLED          0x02

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               Status;
  UINT8               Reserved[3];
} HMRFPO_GET_STATUS_RESPONSE;

typedef union {
  HMRFPO_GET_STATUS_REQUEST   Request;
  HMRFPO_GET_STATUS_RESPONSE  Response;
} HMRFPO_GET_STATUS_BUFFER;

///
/// Read File, Ver3 API only
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               FilePath[64];
  UINT32              Offset;
  UINT32              DataSize;
  UINT8               Flags;
} READ_FILE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              DataSize;
  UINT8               Data[];    ///< Data Buffer
} READ_FILE_RESPONSE;

typedef union {
  READ_FILE_REQUEST   Request;
  READ_FILE_RESPONSE  Response;
} READ_FILE_BUFFER;

///
/// Set File, Ver3 API only
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               FilePath[64];
  UINT32              Offset;
  UINT32              DataSize;
  UINT8               Flags;
  UINT8               Data[];     ///< Data Buffer
} SET_FILE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} SET_FILE_RESPONSE;

typedef union {
  SET_FILE_REQUEST    Request;
  SET_FILE_RESPONSE   Response;
} SET_FILE_BUFFER;

//
// MKHI_GEN_GROUP_ID Definitions
//

///
/// Get MKHI Version
///
#define MKHI_MSG_VERSION_MAJOR 0x1
#define MKHI_MSG_VERSION_MINOR 0x1

typedef union {
  UINT32 Data;
  struct {
    UINT32 Minor : 16;
    UINT32 Major : 16;
  } Fields;
} MKHI_VERSION;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GET_MKHI_VERSION_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  MKHI_VERSION        MkhiVersion;
} GET_MKHI_VERSION_RESPONSE;

typedef union {
  GET_MKHI_VERSION_REQUEST  Request;
  GET_MKHI_VERSION_RESPONSE Response;
} GET_MKHI_VERSION_BUFFER;

///
/// Get FW Version
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GET_FW_VER_REQUEST;

typedef struct {
  UINT32  CodeMinor   : 16;
  UINT32  CodeMajor   : 16;
  UINT32  CodeBuildNo : 16;
  UINT32  CodeHotFix  : 16;
  UINT32  RcvyMinor   : 16;
  UINT32  RcvyMajor   : 16;
  UINT32  RcvyBuildNo : 16;
  UINT32  RcvyHotFix  : 16;
  UINT32  FitcMinor   : 16;
  UINT32  FitcMajor   : 16;
  UINT32  FitcBuildNo : 16;
  UINT32  FitcHotFix  : 16;
} GET_FW_VER_ACK_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  GET_FW_VER_ACK_DATA Data;
} GET_FW_VER_RESPONSE;

typedef union {
  GET_FW_VER_REQUEST  Request;
  GET_FW_VER_RESPONSE Response;
} GET_FW_VER_BUFFER;

///
/// Core Bios Done
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} CORE_BIOS_DONE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} CORE_BIOS_DONE_RESPONSE;

typedef union {
  CORE_BIOS_DONE_REQUEST  Request;
  CORE_BIOS_DONE_RESPONSE Response;
} CORE_BIOS_DONE_BUFFER;

///
/// Send CPU brand class fuse
///

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               CpuBrandClass;
} GEN_SEND_CPU_BRAND_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GEN_SEND_CPU_BRAND_RESPONSE;

typedef union {
  GEN_SEND_CPU_BRAND_REQUEST  Request;
  GEN_SEND_CPU_BRAND_RESPONSE Response;
} GEN_SEND_CPU_BRAND_BUFFER;

//
// Get CPU Type Change message
//
#define MAX_SKU_BITMAP            8 ///< CPU type change response feature masks size in DWORDS
#define USER_FEEDBACK_REQUESTED   1 ///< User feedback is requested

typedef struct {
  UINT8               UserFeedback;
  UINT32              FeaturesDisabled[MAX_SKU_BITMAP];
  UINT32              FeaturesEnabled[MAX_SKU_BITMAP];
  UINT8               GlobalResetRequired;
} GEN_GET_CPU_TYPE_CHANGE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GEN_GET_CPU_TYPE_CHANGE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER          MkhiHeader;
  GEN_GET_CPU_TYPE_CHANGE_DATA Data;
} GEN_GET_CPU_TYPE_CHANGE_RESPONSE;

typedef union {
  GEN_GET_CPU_TYPE_CHANGE_REQUEST   Request;
  GEN_GET_CPU_TYPE_CHANGE_RESPONSE  Response;
} GEN_GET_CPU_TYPE_CHANGE_BUFFER;

//
// Set CPU Type Change message
//
#define CPU_TYPE_CHANGE_ACCEPTED  1 ///< CPU type change accepted

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               UserResponse;
} GEN_SET_CPU_TYPE_CHANGE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GEN_SET_CPU_TYPE_CHANGE_RESPONSE;

typedef union {
  GEN_SET_CPU_TYPE_CHANGE_REQUEST   Request;
  GEN_SET_CPU_TYPE_CHANGE_RESPONSE  Response;
} GEN_SET_CPU_TYPE_CHANGE_BUFFER;


///
/// End of Post
///
#define EOP_DISABLED    0
#define EOP_SEND_IN_PEI 1
#define EOP_SEND_IN_DXE 2

#define MAX_EOP_SEND_RETRIES 3

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} END_OF_POST_REQUEST;

typedef enum {
  HeciEopStatusSuccess,
  HeciEopPerformGlobalReset
} EOP_REQUESTED_ACTIONS;

typedef struct {
  UINT32  RequestedActions;
} EOP_ACK_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  EOP_ACK_DATA        Data;
} END_OF_POST_RESPONSE;

typedef union {
 END_OF_POST_REQUEST    Request;
 END_OF_POST_RESPONSE   Response;
} END_OF_POST_BUFFER;

//
// Unconfig without password messages
//
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} UNCONFIG_WO_PASSWORD_BUFFER;

///
/// Get ME Unconfiguration State
///
#define ME_UNCONFIG_TIMEOUT         200  ///< Unconfig timeout in seconds

#define ME_UNCONFIG_IN_PROGRESS     0x01 ///< Unconfig without password is in progress
#define ME_UNCONFIG_NOT_IN_PROGRESS 0x02 ///< Normal case, there is no unconfigure w/o password to be processed
#define ME_UNCONFIG_FINISHED        0x03 ///< CSME finished processing unconfigure w/o password. Bios will perform a global reset
#define ME_UNCONFIG_ERROR           0x80 ///< CSME encountered error while processing revert back to default. Specific error status defined starting at 0x81

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GEN_GET_ME_UNCFG_STATE;

///
/// Set Manufacturing ME Reset and Halt messages
///
typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
} SET_ME_RESET_HALT_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
} SET_ME_RESET_HALT_RESPONSE;

///
/// FW Feature Shipment Time State Override
///
typedef struct {
  UINT32  EnableFeature;
  UINT32  DisableFeature;
} FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER                          MkhiHeader;
  FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_DATA FeatureState;
} FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_REQUEST;

//
// FW Feature Shipment Time State Override returned Status
//
typedef enum {
  SetFeatureStateAccepted,
  SetFeatureStateRejected
} FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_STATUS;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT32                Status;
} FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_RESPONSE;

typedef union {
  FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_REQUEST   Request;
  FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_RESPONSE  Response;
} FW_FEATURE_SHIPMENT_TIME_STATE_OVERRIDE_BUFFER;

///
/// Get Image FW Version
///
#define FTP_PARTITION_NAME_PMCP    0x50434D50
#define FPT_PARTITION_NAME_EFWP    0x50574645

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT32                PartitionId;
} GET_IMAGE_FW_VER_REQUEST;

typedef struct {
  UINT16                Major;
  UINT16                Minor;
  UINT16                Hotfix;
  UINT16                Build;
} FW_VERSION;

typedef struct {
  UINT32                PartitionId;   ///< Partition ID
  UINT8                 Reserved[8];   ///< Should be 0
  FW_VERSION            Version;       ///< Module version
  UINT32                VendorId;      ///< Module vendor ID(0x8086 for Intel)
  UINT32                TcbSvn;        ///< TCB SVN
  UINT32                Reserved1[15]; ///< Reserved bytes for future extension. Should be 0 while reserved
} FLASH_PARTITION_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT32                NumOfModules;
  FLASH_PARTITION_DATA  ManifestData;
} GET_IMAGE_FW_VER_RESPONSE;

typedef union {
  GET_IMAGE_FW_VER_REQUEST  Request;
  GET_IMAGE_FW_VER_RESPONSE Response;
} GET_IMAGE_FW_VERSION_BUFFER;

///
/// Set FIPS Mode
///
#define FIPS_MODE_DISABLED 0
#define FIPS_MODE_ENABLED  1

typedef struct {
  UINT32              FipsMode;
} SET_FIPS_MODE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  SET_FIPS_MODE_DATA  Data;
} SET_FIPS_MODE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} SET_FIPS_MODE_RESPONSE;

typedef union {
  SET_FIPS_MODE_REQUEST   Request;
  SET_FIPS_MODE_RESPONSE  Response;
} SET_FIPS_MODE_BUFFER;

///
/// Get FIPS Mode
///
typedef struct {
  UINT16              Major;
  UINT16              Minor;
  UINT16              Hotfix;
  UINT16              Build;
} FIPS_VERSION;

typedef struct {
  UINT32              FipsMode;
  FIPS_VERSION        CryptoVersion;
  UINT8               Reserved[8];
} GET_FIPS_MODE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GET_FIPS_MODE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  GET_FIPS_MODE_DATA  Data;
} GET_FIPS_MODE_RESPONSE;

typedef union {
  GET_FIPS_MODE_REQUEST   Request;
  GET_FIPS_MODE_RESPONSE  Response;
} GET_FIPS_MODE_BUFFER;

///
/// Set CSME Measured Boot
///
typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT8                 Data;
} SET_MEASURED_BOOT_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
} SET_MEASURED_BOOT_RESPONSE;

typedef union {
  SET_MEASURED_BOOT_REQUEST   Request;
  SET_MEASURED_BOOT_RESPONSE  Response;
} SET_MEASURED_BOOT_BUFFER;

///
/// Get CSME Measured Boot
///
typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
} GET_MEASURED_BOOT_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT8                 Data;
} GET_MEASURED_BOOT_RESPONSE;

typedef union {
  GET_MEASURED_BOOT_REQUEST   Request;
  GET_MEASURED_BOOT_RESPONSE  Response;
} GET_MEASURED_BOOT_BUFFER;

///
/// Read from SPI
///
typedef enum {
  SpiReadTypePrimaryAccess,
  SpiReadTypeGbeVersion,
  SpiReadTypeFlashInfo,
  SpiReadTypeFlashComponents,               ///< Not available in MKHI Client
  SpiReadTypeSpiSoftstraps,                 ///< Not available in MKHI Client
  SpiReadTypeFlashDevicesSfdpData,          ///< Not available in MKHI Client
  SpiReadTypeDescriptorResilienceInfo = 10
} SPI_READ_TYPE;

typedef struct {
  UINT32 ReadAccessExt  : 4;
  UINT32 WriteAccessExt : 4;
  UINT32 ReadAccess     : 12;
  UINT32 WriteAccess    : 12;
} SPI_DESC_PRIMARY;

typedef struct {
  SPI_DESC_PRIMARY  PrimaryAcess[6];
} SPI_DESC_PRIMARY_DATA;

typedef struct {
  UINT16 Version;
} SPI_GBE_VERSION;

typedef struct {
  UINT32 ComponentsNumber;
  UINT32 Flash1Id;
  UINT32 Flash2Id;
} SPI_FLASH_INFO;

#define MAX_DESCRIPTOR_RESILIENCE_ENTRIES 2

typedef struct {
  UINT8  DescriptorHash[32];
  UINT8  DescriptorRecoveryPolicy;
  UINT8  DescriptorFlags;
  UINT16 Reserved;
} SPI_DESCRIPTOR_RESILIENCE_ENTRY;

typedef struct {
  UINT8 NumOfDescriptorResilenceEntries;
  SPI_DESCRIPTOR_RESILIENCE_ENTRY Entry[MAX_DESCRIPTOR_RESILIENCE_ENTRIES];
} SPI_DESCRIPTOR_RESILIENCE_INFO;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              ReadType;
} READ_FROM_SPI_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               SpiData[0];
} READ_FROM_SPI_RESPONSE;

typedef union {
  READ_FROM_SPI_REQUEST   Request;
  READ_FROM_SPI_RESPONSE  Response;
} READ_FROM_SPI_BUFFER;

typedef enum {
  StrapGroupModularIoTypeCConfigStraps = 1
} STRAP_GROUPS;

///
/// MODULAR_IO_TYPE_C_PORT_CONFIG
///
#define  MODULAR_IO_USB4_40G    0x0
#define  MODULAR_IO_DP          0x1
#define  MODULAR_IO_HDMI        0x2
#define  MODULAR_IO_EDP         0x3
#define  MODULAR_IO_USB_DP_ALT  0xE

#define MODULAR_IO_DDI_ENABLE 0
#define MODULAR_IO_EDP_ENABLE 1

typedef struct {
  UINT32 Port1Change : 1;  ///< 0: no change required, 1: change required for port 1
  UINT32 Port1Config : 4;  ///< @see config values in MODULAR_IO_TYPE_C_PORT_CONFIG
  UINT32 Port2Change : 1;  ///< 0: no change required, 1: change required for port 2
  UINT32 Port2Config : 4;  ///< @see config values in MODULAR_IO_TYPE_C_PORT_CONFIG
  UINT32 Port3Change : 1;  ///< 0: no change required, 1: change required for port 3
  UINT32 Port3Config : 4;  ///< @see config values in MODULAR_IO_TYPE_C_PORT_CONFIG
  UINT32 Port4Change : 1;  ///< 0: no change required, 1: change required for port 4
  UINT32 Port4Config : 4;  ///< @see config values in MODULAR_IO_TYPE_C_PORT_CONFIG
  UINT32 EdpChange   : 1;  ///< 0: no change required, 1: change required for eDP configuration
  UINT32 EdpConfig   : 1;  ///< 0: DDI (eDP disable) enable, 1: eDP enable
  UINT32 Reserved    : 10; ///< Reserved - should be 0
} MODULAR_IO_TYPE_C_CONFIG_DATA;

typedef union {
  MODULAR_IO_TYPE_C_CONFIG_DATA               ModularIOTypeCConfigData;
  UINT32                                      ConfigData;
} CONFIG_DATA;

typedef struct {
  STRAP_GROUPS                                StrapGroupId;
  CONFIG_DATA                                 OverrideData;
} STRAP_OVERRIDE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER                         MkhiHeader;
  UINT32                                      StrapGroupsNumber; // number of soft straps groups to set
  STRAP_OVERRIDE_DATA                         StrapsData[];      // array of soft strap config data per soft strap group
} GEN_SET_STRAP_OVERRIDE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER                         MkhiHeader;
  UINT32                                      Flags;             // BIT0: 0/1 - Global reset is not required/Global reset is required
} GEN_SET_STRAP_OVERRIDE_RESPONSE;

typedef union {
  GEN_SET_STRAP_OVERRIDE_REQUEST              Request;
  GEN_SET_STRAP_OVERRIDE_RESPONSE             Response;
} GEN_SET_STRAP_OVERRIDE_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER                         MkhiHeader;
  STRAP_GROUPS                                StrapGroupId;      // soft strap group to retrive the override data
} GEN_GET_STRAP_OVERRIDE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER                         MkhiHeader;
  UINT32                                      StrapGroupsNumber; // number of retrived soft straps groups
  STRAP_OVERRIDE_DATA                         StrapsData[];      // array of soft strap config data per soft strap group
} GEN_GET_STRAP_OVERRIDE_RESPONSE;

typedef union {
  GEN_GET_STRAP_OVERRIDE_REQUEST              Request;
  GEN_GET_STRAP_OVERRIDE_RESPONSE             Response;
} GEN_GET_STRAP_OVERRIDE_BUFFER;

#pragma pack()

#endif // _MKHI_MSGS_H
