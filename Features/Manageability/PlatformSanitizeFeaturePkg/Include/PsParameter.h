/** @file
  Platform Sanitize Structure definitions Header file.

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

#ifndef _PS_PARAMETER_H_
#define _PS_PARAMETER_H_

//
// Parameter Definitions
//

//
// Platform Sanitize Bit representation
//
#define PS_RPE_SUPPORTED                        BIT0
#define PS_ERASE_STORAGE_MEDIA                  BIT2
#define PS_VERIFY_STORAGE_MEDIA                 BIT3
#define PS_GENERATE_SANITIZE_REPORT             BIT4
#define PS_TPM_CLEAR                            BIT6
#define PS_OEM_CUSTOM_ACTION                    BIT16
#define PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION   BIT25
#define PS_BIOS_RELOAD_GOLDEN_CONFIG            BIT26
#define PS_CSME_UNCONFIGURE                     BIT31

// Storage Password Length
#define STORAGE_DEV_PSW_MAX_LEN                 33  // 32 Bytes for password + 1 byte for Null Character
// Storage Erase Report Maximum file length
#define MAX_FILE_NAME_LENGTH                    12

// OEM Custom Action TLV Length
#define PS_OEM_CUSTOM_ERASE_TLV_LEN             500  // 500 Bytes for OEM TLV Blob data

//
// RPE Capabilities
//
#define PS_CAPABILITY_MASK                        (PS_RPE_SUPPORTED | \
                                                  PS_ERASE_STORAGE_MEDIA | \
                                                  PS_VERIFY_STORAGE_MEDIA | \
                                                  PS_GENERATE_SANITIZE_REPORT | \
                                                  PS_TPM_CLEAR | \
                                                  PS_OEM_CUSTOM_ACTION | \
                                                  PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | \
                                                  PS_BIOS_RELOAD_GOLDEN_CONFIG | \
                                                  PS_CSME_UNCONFIGURE \
                                                  )


#define PS_PERFORM_ACTION_MASK                    (~PS_RPE_SUPPORTED & \
                                                  PS_CAPABILITY_MASK \
                                                  )

#define PS_PERFORM_ACTION_EXCLUDE_STORAGE_MASK    (~PS_ERASE_STORAGE_MEDIA & \
                                                  ~PS_VERIFY_STORAGE_MEDIA & \
                                                  ~PS_GENERATE_SANITIZE_REPORT & \
                                                  PS_PERFORM_ACTION_MASK \
                                                  )


#define PSR_LOGGING_SUPPORT                       (PS_ERASE_STORAGE_MEDIA | \
                                                  PS_VERIFY_STORAGE_MEDIA | \
                                                  PS_TPM_CLEAR | \
                                                  PS_OEM_CUSTOM_ACTION | \
                                                  PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | \
                                                  PS_BIOS_RELOAD_GOLDEN_CONFIG | \
                                                  PS_CSME_UNCONFIGURE \
                                                  )

//
// Platform Sanitize Progress Definitions
//
// Status = Progress / Failure
typedef enum {
  PsProgressFailure = 0,
  PsProgressInProgress,
} PS_PET_STATUS;

typedef enum {
  PsPetEraseGeneral      = 0,
  PsPetEraseStorage      = PS_ERASE_STORAGE_MEDIA,
  PsPetEraseTpmClear     = PS_TPM_CLEAR,
  PsPetEraseCustomAction = PS_OEM_CUSTOM_ACTION,
  PsPetEraseClearNvm     = PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION,
  PsPetEraseReloadConfig = PS_BIOS_RELOAD_GOLDEN_CONFIG,
  PsPetEraseCsmeUnconfig = PS_CSME_UNCONFIGURE
} PS_PET_ERASE_ACTION;

typedef enum {
  //
  // General Operation Code
  //
  PsBootParameterReceive,
  PsBeginPlatformSanitize,
  PsClearReservedParameters,
  PsInvalidBootParameters,
  PsAllSettingsDecremented,
  PsGeneralOpcodeMax = 0x5,

  //
  // Erase Action Code
  //
  PsOperationStarted = PsGeneralOpcodeMax,
  PsOperationSuccessful,
  PsOperationFailed,
  PsOperationNotSupported,
  PsEraseActionCodeMax
} PS_PET_OPCODE;

//
// Platform Sanitize BIOS Status Update
//
typedef enum {
  PsSuccess,
  PsInProgress,
  PsGeneralFailure,
  PsAccessDenied,
  PsUnsupported,
} PS_REPORT_BIOS_STATUS;

//
// Platform Sanitize boot type
//
typedef enum {
  BootNone,
  BootLpe,
  BootRpe,
  BootConfigDataReset,
} PS_BOOT_TYPE;

#pragma pack(1)

//
// PS Status
//
typedef struct {
  UINT32      PsAttempted;        // 32 bits, Each bit represent corresponding erase action. value 1 = erase attempted, 0 = erase not attempted.
  UINT32      PsAttemptResult;    // 32 bits, Each bit represent corresponding erase action results. value 1 = Success ; 0 = Failure
  UINT8       PsFailReason[32];   // Each Byte corresponding to each bit in PsAttemptResult and its corresponding erase action.
} PS_STATUS;

//
// PS Boot Parameter Structure
//
typedef struct {
  PS_BOOT_TYPE     TriggerSource;
  UINT32           PsRequestedList;
  UINT32           PsPendingList;
  BOOLEAN          IsSanitizeSummaryDisplayed;
  PS_STATUS        PsStatus;
} PS_BOOT_PARAMETERS;

//
// PS Boot Parameter Structure
//
typedef struct {
  PS_BOOT_PARAMETERS   PsBootParameters;
  CHAR8                PsStorageDevPassword[STORAGE_DEV_PSW_MAX_LEN];
} PS_BOOT_STORAGE_PARAMETERS;

typedef struct {
  UINT16      OemVid;
  UINT16      OemParameterTypeId;
  UINT32      OemDataLength;
  UINT8       OemData[PS_OEM_CUSTOM_ERASE_TLV_LEN];
} PS_OEM_HOOK_TLV_DATA;

#pragma pack()

#endif // _PS_PARAMETER_H_
