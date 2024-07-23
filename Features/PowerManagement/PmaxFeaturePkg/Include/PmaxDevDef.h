/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _PMAX_DEV_DEF_H_
#define _PMAX_DEV_DEF_H_

#define MAX_PEAK_POWER_VALUE                        0x3A98
#define MAX_PMAX_DEVICE_STRING_LENGTH               31
#define PMAX_DEVICE_NUMBER                          4

#define DEFAULT_REALTEK_CODEC_DEVICE_STRING         "                               "
#define DEFAULT_REALTEK_CODEC_DEVICE_D0_PEAK_POWER  0xFFFF
#define DEFAULT_REALTEK_CODEC_DEVICE_DX_PEAK_POWER  0xFFFF

#define DEFAULT_WF_CAMERA_STRING                    "                               "
#define DEFAULT_WF_CAMERA_D0_PEAK_POWER             0xFFFF
#define DEFAULT_WF_CAMERA_DX_PEAK_POWER             0xFFFF

#define DEFAULT_UF_CAMERA_STRING                    "                               "
#define DEFAULT_UF_CAMERA_D0_PEAK_POWER             0xFFFF
#define DEFAULT_UF_CAMERA_DX_PEAK_POWER             0xFFFF

#define DEFAULT_FLASH_DEVICE_STRING                 "                               "
#define DEFAULT_FLASH_DEVICE_D0_PEAK_POWER          0xFFFF
#define DEFAULT_FLASH_DEVICE_DX_PEAK_POWER          0xFFFF

//SBC VPD helper structure that's used to update PMAX device configuration
typedef struct {
  CHAR8  DevStr[MAX_PMAX_DEVICE_STRING_LENGTH];
  UINT16 DevD0;
  UINT16 DevDx;
} VPD_PMAX_DEV;

#endif
