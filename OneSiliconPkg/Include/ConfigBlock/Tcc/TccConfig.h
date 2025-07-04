/** @file
  TCC Config policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _TCC_CONFIG_H_
#define _TCC_CONFIG_H_

#include <ConfigBlock.h>

extern EFI_GUID gTccConfigGuid;
extern EFI_GUID gTccPreMemConfigGuid;

#pragma pack (push,1)

///
/// Subregion error codes
///
typedef enum {
  SUBREGION_SUCCESS,
  SUBREGION_WATCHDOG_ERROR
} SUBREGION_STATUS;

typedef struct {
  UINTN                 BaseAddress;
  UINT32                Size;
  UINT32                Status;
} TCC_SUB_REGION_INFO;

#define TCC_CONFIG_REVISION 3

/**
  TCC Config settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                ///< Config Block Header
  UINT8                 IfuEnable;
  UINT8                 GtClosEnable;
  UINT8                 L2QosEnumerationEn;
  UINT8                 Rsvd;
} TCC_CONFIG;

#pragma pack (pop)

#endif // TCC_CONFIG_H_

