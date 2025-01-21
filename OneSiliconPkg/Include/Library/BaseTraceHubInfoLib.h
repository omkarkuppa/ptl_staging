/** @file
  BaseTraceHubInfoLib Library header file

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
#ifndef _TRACE_HUB_INFO_LIB_H_
#define _TRACE_HUB_INFO_LIB_H_

/**
  This function will check if TraceHub device is enabled or not

  @retval TRUE                TraceHub device is enabled
  @retval FALSE               TraceHub device is disabled
**/
BOOLEAN
IsTraceHubEnabled (
  VOID
  );

#endif  //_TRACE_HUB_INFO_LIB_H_
