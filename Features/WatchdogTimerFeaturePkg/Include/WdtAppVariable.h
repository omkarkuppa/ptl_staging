/** @file
  Definition of non-volatile EFI variable used by both PEI and DXE parts of WdtApp

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

#ifndef _WDT_APP_VARIABLE_H_
#define _WDT_APP_VARIABLE_H_

#define WDT_PERSISTENT_DATA_C_NAME         L"WdtPersistentData"

typedef struct _WDT_PERSISTENT_DATA {
  UINT8  Enable;
  UINT8  RsvdBytes[3];
} WDT_PERSISTENT_DATA;

#endif
