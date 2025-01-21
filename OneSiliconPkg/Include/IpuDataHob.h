/** @file
  This file defines the IPU DATA HOB

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _IPU_DATA_HOB_H_
#define _IPU_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gIpuDataHobGuid;
#pragma pack (push,1)
///
/// This HOB is used to pass IPU related information to DXE phase
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                    ///< GUID Hob type structure for gIpuDataHobGuid
  // @todo this HOB will have revision number.
  /**
  IpuSupport - Indicates IPU support status
  1 - IPU is supported by Silicon and is opted-in to BIOS
  0 - IPU is either not supported by Silicon or opted out of BIOS or both
   **/
  BOOLEAN                  IpuSupport;
} IPU_DATA_HOB;
#pragma pack (pop)

#endif  // _IPU_DATA_HOB_H_
