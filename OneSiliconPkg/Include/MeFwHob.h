/** @file
  The GUID definition for MeFwHob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef _ME_FW_HOB_H_
#define _ME_FW_HOB_H_

#include <FwStsSmbiosTable.h>

extern EFI_GUID gMeFwHobGuid;

#pragma pack(1)

typedef struct {
  UINT32  FunNumber;
  UINT32  Reg[MAX_FWSTS_REG];
} FWSTS_REG_MAP;

typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType;
  UINT8                 Revision;
  UINT8                 Count;
  UINT16                Reserved;
  FWSTS_REG_MAP         Group[MAX_HECI_FUNC];
} ME_FW_HOB;
#pragma pack()
#endif
