/**@file
  Definition of AMT PPR Enable variable

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _AMT_PPR_ENABLE_VARIABLE_H_
#define _AMT_PPR_ENABLE_VARIABLE_H_

//
// Global ID for AMT PPR Enablement Variable
//
#define AMT_PPR_ENABLE_VARIABLE_GUID \
  { 0x49a9ffd3, 0x3ee9, 0x46a3, { 0x41, 0x26, 0x5d, 0xa4, 0xcd, 0x2f, 0x81, 0xa5 } }

extern EFI_GUID gAmtPprEnableVariableGuid;

#define AMT_PPR_ENABLE_VARIABLE_NAME  L"AmtPprEnable"
#define AMT_PPR_ENABLED               0x3

//
//  AMT PPR ENABLE
//      Bits [7:2] Reserved
//      Bits [1:0] AmtPprEnable
//          0X - Disabled AMT, Disabled PPR
//          10 - Enabled AMT,  Disabled PPR
//          11 - Enabled AMT,  Enabled PPR
//
typedef union {
  struct {
    UINT8 Reserved      : 6;
    UINT8 AmtEnabled    : 1;
    UINT8 PprEnabled    : 1;
  } Bits;
} AMT_PPR_ENABLE;

//
// Memory Telemetry variable revision
//
#define AMT_PPR_ENABLE_VARIABLE_REVISION   1

#endif // _AMT_PPR_ENABLE_VARIABLE_H_
