/** @file
    This file contains definitions required for creation of TSE Data HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _TSE_DATA_HOB_H_
#define _TSE_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gTseDataHobGuid;

#define TSE_DATA_HOB_REVISION  1

#pragma pack(push, 1)
///
/// TSE Hob data.
///
typedef struct _TSE_DATA_DATA {
  EFI_HOB_GUID_TYPE EfiHobGuidType; /// EFI Hob Guid Type.
  /**
     This member specifies the revision of the TSE Data HOB. This field is used
     to indicate backwards compatible changes to the protocol. Platform code that
     consumes this protocol must read the correct revision value to correctly interpret
     the content of the protocol fields.
  **/
  UINT8         Revision;
  BOOLEAN       TseSupported:              1;  // 0: Not Supported, 1: Supported
  BOOLEAN       TseEnabled:                1;  // 0: Not Enabled, 1: Enabled
  BOOLEAN       Rsvdbits:                  6;
  UINT32        TseDramMemSize;
  UINT64        TseDramMemBase;
} TSE_DATA_HOB;

#pragma pack(pop)

#endif // _TSE_DATA_HOB_H_
