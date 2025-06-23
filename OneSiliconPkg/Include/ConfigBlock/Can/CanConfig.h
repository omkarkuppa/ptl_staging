/** @file
  CAN Config policy

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

#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

/**
  The CAN_CONFIG block describes policies related to Controller Area Network(CAN)
  <b>Revision 1</b>:
  - Initial version.
**/

#define CAN_CONFIG_REVISION 1
extern EFI_GUID gCanConfigGuid;

#pragma pack (push,1)

/**
  CAN Config settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal CAN, 0: Disable; <b>1: Enable</b>.
  **/
  UINT8                 Enable[PCH_MAX_CAN_PORT];
  UINT8                 RsvdBytes[2];
} CAN_CONFIG;

#pragma pack (pop)

#endif
