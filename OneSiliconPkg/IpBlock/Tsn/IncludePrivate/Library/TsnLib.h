/** @file
  This file provides Tsn public library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _TSN_LIB_H_
#define _TSN_LIB_H_

#include <TsnHandle.h>

/**
  Check whether TSN controller is enabled in the platform.

  @param[in] TsnHandle            Pointer to Tsn Handle

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
IsTsnDevicePresent (
  IN  TSN_HANDLE    *TsnHandle
  );

#endif // _TSN_LIB_H_
