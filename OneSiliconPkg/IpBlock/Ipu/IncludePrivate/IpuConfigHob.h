/** @file
  This file defines the IPU CONFIG HOB

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
#ifndef _IPU_CONFIG_HOB_H_
#define _IPU_CONFIG_HOB_H_

#include <Base.h>

//
// Maximum CSI (Camera Serial Interface) Port
//
#define MAX_CSI_PORT  8

extern EFI_GUID gIpuConfigHobGuid;
#pragma pack (push,1)
///
/// This HOB is used to pass IPU related Private information to DXE phase
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                    ///< GUID Hob type structure for gIpuConfigHobGuid
  /**
  IpuAcpi - Indicates IPU ACPI mode
  0 - Disabled
  1 - IGF Child device
  2 - ACPI device
  **/
  UINT8                    IpuAcpiMode;
} IPU_CONFIG_HOB;
#pragma pack (pop)

#endif  // _IPU_CONFIG_HOB_H_
