/** @file
  THC Platform DXE driver

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

#ifndef _THC_PLATFORM_DXE_H_
#define _THC_PLATFORM_DXE_H_

#define THC_MAX_PORTS     0x02

typedef struct  {
  UINT16     Index;
  UINT16     DevId;
} HID_OVER_I2C_DEVID;

typedef struct  {
  UINT16                Count;
  HID_OVER_I2C_DEVID    ThcHidOverI2cDevId[THC_MAX_PORTS];
} THC_HID_OVER_I2C_DEVID;

#endif // _THC_PLATFORM_DXE_H_
