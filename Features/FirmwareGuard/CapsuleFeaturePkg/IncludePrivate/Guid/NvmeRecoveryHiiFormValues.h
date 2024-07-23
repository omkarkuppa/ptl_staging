/** @file
  Definition for NvmeRecoverHii Variable and NVMe Hii setup menu used for
  NVMe Recovery support

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

#ifndef _NVME_RECOVERY_HII_FORM_VALUES_H_
#define _NVME_RECOVERY_HII_FORM_VALUES_H_

#include <Guid/HiiPlatformSetupFormset.h>

/* {78B68387-38AE-4554-AB94-3E11CDF324FD} */
#define NVME_RPMB_KEY_MIGRATION_CONFIG_GUID { 0x78b68387, 0x38ae, 0x4554, { 0xab, 0x94, 0x3e, 0x11, 0xcd, 0xf3, 0x24, 0xfd } }
// {54138729-F65C-489D-AF32-A93BB6D0D79C}
#define NVME_BASED_RECOVERY_HII_GUID        { 0x54138729, 0xf65c, 0x489d, { 0xaf, 0x32, 0xa9, 0x3b, 0xb6, 0xd0, 0xd7, 0x9c } }

// ID's for various forms that will be used by HII
#define FORMID_VALUE_MAIN_MENU                             0x01

#define NVME_RECOVERY_SUPPORT_QUESTION_ID                  0x1070

#define NVME_RPMB_KEY_MIGRATION_VALUE_NO_ACTION            0
#define NVME_RPMB_KEY_MIGRATION_VALUE_SET_PIN_KEY          1
#define NVME_RPMB_KEY_MIGRATION_VALUE_UNLOCK_BY_PIN_KEY    2

// Define for HII setup value
#define NVME_RECOVERY_FUNCTION_DISABLE_VALUE               0
#define NVME_RECOVERY_FUNCTION_ENABLE_VALUE                1

// Define for PcdNvmeRecoverySupported value
#define NVME_RECOVERY_FUNCTION_DEFAULT_DISABLE_VALUE       0 // default disable
#define NVME_RECOVERY_FUNCTION_ENABLE_BACKUP_VALUE         1 // set when boot partition has valid backup content
#define NVME_RECOVERY_FUNCTION_ENABLE_NO_BACKUP_VALUE      2
#define NVME_RECOVERY_FUNCTION_CSME_NOT_SUPPORT            3

#define NVME_BASED_RECOVERY_VARIABLE_NAME                  L"NvmeRecoverHii"

#pragma pack(1)

#define NVME_BASED_RECOVERY_HII_CONFIG_REVISION            1

typedef struct {
  UINT8  Revision;
  UINT8  KeyMigrationAction;
  UINT8  NvmeRecoverySupported;
  UINT8  Reserved[21];
} NVME_BASED_RECOVERY_HII_CONFIG;

#pragma pack()

#endif //_NVME_RECOVERY_HII_FORM_VALUES_H_
