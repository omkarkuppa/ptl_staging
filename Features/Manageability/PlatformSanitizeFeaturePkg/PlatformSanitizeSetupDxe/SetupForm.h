/**@file
  Platform Sanitize setup driver

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

#ifndef _SETUP_FORM_H_
#define _SETUP_FORM_H_

#include <Guid/PlatformSanitizeVariable.h>

#define  PLATFORM_SANITIZE_FORMSET_GUID \
  { \
    0x561c43df, 0xa490, 0x4444, { 0x91, 0xff, 0x0, 0x1e, 0x8d, 0xfa, 0xe6, 0x8c } \
  }

#define LABEL_START           0x1000
#ifndef LABEL_END
#define LABEL_END             0x1001
#endif

#define STORAGE_ERASE_VARSTORE_ID                 0x0101
// ID's for various forms that will be used by HII
#define FORMID_MAIN_MENU                          0x0200
#define FORMID_STORAGE_ERASE_MENU                 0x0201
#define FORMID_LOCAL_PLATFORM_ERASE_MENU          0x0202

#define KEY_LPE_ERASE_ALL                         0x0301
#define KEY_LPE_ERASE_SSD                         0x0302
#define KEY_LPE_TPM_CLEAR                         0x0303
#define KEY_LPE_CLEAR_NVM                         0x0304
#define KEY_LPE_RELOAD_BIOS                       0x0305
#define KEY_LPE_PLATFORM_ENABLE                   0x0306
#define KEY_LPE_OEM_ACTION                        0x0307
#define KEY_RPE_ENABLE                            0x0308

#define DISK_LIST_QUESTION_ID           0x1200
#define DISK_LIST_VAR_OFFSET ((UINT16)  OFFSET_OF (HII_CONFIG, DiskList))
#define DISK_COUNT_MAX                  16

/**
  Making any setup structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table\n
  <b>Revision 1</b>: - Initial version.
**/
#define HII_CONFIG_REVISION             1

//
// This variable keep configuration of storage erase
//
typedef struct {
  UINT8    SanitizeOrder[SANITIZE_COUNT];
  UINT8    DiskList[DISK_COUNT_MAX];   // This list reflect whether disk is selected
} HII_CONFIG;

#endif //_SETUP_FORM_H_
