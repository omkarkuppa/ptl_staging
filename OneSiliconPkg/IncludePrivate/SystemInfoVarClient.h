/** @file
  Client System Infor Var Hearder File

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

#ifndef __SYSTEM_INFO_VAR_CLIENT__
#define __SYSTEM_INFO_VAR_CLIENT__

#include <Uefi.h>

#pragma pack (push,1)

typedef struct {
  EFI_PHYSICAL_ADDRESS  SocAddrMapData;
  EFI_PHYSICAL_ADDRESS  SbiAccess;
  EFI_PHYSICAL_ADDRESS  PchSbiAccess;
  EFI_PHYSICAL_ADDRESS  CpuSbiAccess;
  EFI_PHYSICAL_ADDRESS  P2SbCtrl;
} SYSTEM_INFO_VAR;

#pragma pack (pop)

#endif //#ifndef __SYSTEM_INFO_VAR_CLIENT__
