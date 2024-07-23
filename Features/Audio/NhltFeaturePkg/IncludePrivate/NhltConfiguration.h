/** @file
  Header file for creating NHLT configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _NHLT_CONFIGURATION_H_
#define _NHLT_CONFIGURATION_H_

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>

#define NHLT_ENDPOINTS_TABLE_MAX_CONFIGURATION_NUMBER 40

typedef struct {
  UINT8  NhltConfigurationEnabled[NHLT_ENDPOINTS_TABLE_MAX_CONFIGURATION_NUMBER]; ///< Nhlt config enablement: <b>0: Disable</b>; 1: Enable
} NHLT_CONFIGURATION;

#endif // _NHLT_CONFIGURATION_H_
