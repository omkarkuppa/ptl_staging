/** @file
  Hob GUID for containing reset cause relative information

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

#ifndef __INTEL_RESET_CAUSE_HOB_GUID_H__
#define __INTEL_RESET_CAUSE_HOB_GUID_H__

#include <ResetReasonVendorData.h>

// {333E4C8E-E66D-468B-8ECA-36E139A86C6B}
#define INTEL_RESET_CAUSE_HOB_GUID \
  { \
    0x333E4C8E, 0xE66D, 0x468B, { 0x8E, 0xCA, 0x36, 0xE1, 0x39, 0xA8, 0x6C, 0x6B } \
  }

#pragma pack(1)

typedef struct {
  RESET_CAUSE_PM_REGISTERS    ResetCauseRegs;
  PMC_SLEEP_STATE             PmcSleepState;
} RESET_CAUSE_HOB_DATA;

#pragma pack()

extern EFI_GUID  gIntelResetCauseHobGuid;

#endif
