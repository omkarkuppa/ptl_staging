/** @file
  AMT PPR Enable variable definition

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

///
/// AMT PPR variable name
///
#define AMT_PPR_ENABLE_VARIABLE_NAME  L"AmtPprEnable"

/**
  AMT PPR ENABLE
      Bits [7:2] Reserved
      Bits [1]   AmtEnable
      Bits [0]   PprEnable
          0X - Disabled AMT, Disabled PPR
          10 - Enabled AMT,  Disabled PPR
          11 - Enabled AMT,  Enabled PPR
**/
typedef union {
  struct {
    UINT8 PprEnabled    : 1;
    UINT8 AmtEnabled    : 1;
    UINT8 Reserved      : 6;
  } Bits;
} AMT_PPR_ENABLE;

///
/// Memory Telemetry variable revision
///
#define AMT_PPR_ENABLE_VARIABLE_REVISION   1
