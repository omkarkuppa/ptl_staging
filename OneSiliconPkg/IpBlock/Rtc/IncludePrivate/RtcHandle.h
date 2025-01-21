/** @file
  RTC Handle structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _RTC_HANDLE_H_
#define _RTC_HANDLE_H_

#include <RegisterAccess.h>
#include <RtcConfig.h>
#include <Ppi/SiPolicy.h>

/**
  RTC handle structure
  Stores all data necessary to initialize RTC IP block
**/
typedef struct {
  RTC_CONFIG                 *RtcConfig;
  REGISTER_ACCESS            *RtcPcrAccess;  ///< RTC controller Private Config Register (PCR) space access
} RTC_HANDLE;

#endif // _RTC_HANDLE_H_
