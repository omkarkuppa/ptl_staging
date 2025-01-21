/** @file
  ESPI config HOB definition.

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

@par Specification
**/

#ifndef _ESPI_CONFIG_HOB_H_
#define _ESPI_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>

/**
  The ESPI_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
  UINT32  BmeHostDeviceEnabled  :  1; ///< When TRUE, then the BME bit enabled in eSPI host and device.
  UINT32  RsvdBits0             : 31; ///< Reserved bits
} ESPI_CONFIG_HOB;

#endif
