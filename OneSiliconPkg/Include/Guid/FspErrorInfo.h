/** @file
  FSP Error Information to describe errors inside FSP that bootloader may
  take some actions to handle those error scenarios.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _FSP_ERROR_INFO_H_
#define _FSP_ERROR_INFO_H_

#include <Guid/FspErrorInfoHob.h>
///
/// GUID value indicating the FSP error information.
///
#define STATUS_CODE_DATA_TYPE_FSP_ERROR_GUID \
  { \
    0x611e6a88, 0xadb7, 0x4301, { 0x93, 0xff, 0xe4, 0x73, 0x04, 0xb4, 0x3d, 0xa6 } \
  }

#define FSP_ERROR_INFO_STATUS_CODE_VALUE         0
#define FSP_ERROR_INFO_STATUS_CODE_INSTANCE      0

///
/// FSP Error Information Block.
///
#pragma pack(1)

typedef struct {
  ///
  /// A GUID identifying the nature of the fatal error.
  /// This GUID is platform speciic.
  ///
  EFI_GUID               ErrorType;

  ///
  /// A code describing the error encountered.
  ///
  EFI_STATUS             Status;
} FSP_ERROR_INFO;

#pragma pack()

extern GUID gFspErrorInfoHobGuid;

#endif //_FSP_ERROR_INFO_H_
