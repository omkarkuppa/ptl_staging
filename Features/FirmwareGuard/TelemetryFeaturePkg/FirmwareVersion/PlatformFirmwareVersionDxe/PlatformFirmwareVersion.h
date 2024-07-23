/** @file
  Private data for platform firmware version information for telemetry

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

@par Specification Reference:
**/

#ifndef _PLATFORM_FIRMWARE_VERSION_H_
#define _PLATFORM_FIRMWARE_VERSION_H_

enum {
  Edk2Ver = 0,
} PLATFORM_VERSION_INDEX;

enum {
  LanPhyVer = 0,
  EcVer,
  GopVer,
  Undiver,
} PLATFORM_VERSION2_INDEX;

#endif
