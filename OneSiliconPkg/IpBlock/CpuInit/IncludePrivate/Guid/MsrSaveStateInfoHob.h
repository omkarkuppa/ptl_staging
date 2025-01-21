/** @file
  This file defines a HOB to indicate if MsrSaveState is enabled.
  If the HOB doesn't exist, MsrSaveState should be disabled.

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

@par Specification
**/

#ifndef __MSR_SAVE_STATE_INFO_HOB_GUID_H__
#define __MSR_SAVE_STATE_INFO_HOB_GUID_H__

#define EFI_MSR_SAVE_STATE_INFO_HOB_GUID \
  { 0x9646b654,0xfbb7,0x41d2, { 0xa0,0x0a,0xb2,0x25,0xad,0x5f,0xbc,0x8b } }

#pragma pack(1)
typedef struct {
  BOOLEAN  SaveStateInMsr;             /// If MsrSaveState is enabled.
} MSR_SAVE_STATE_INFO_HOB;
#pragma pack()

extern EFI_GUID gMsrSaveStateInfoHobGuid;

#endif
