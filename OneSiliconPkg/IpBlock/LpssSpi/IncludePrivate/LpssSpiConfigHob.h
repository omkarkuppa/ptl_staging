/** @file
  LPSS SPI config HOB definition.

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

#ifndef _LPSS_SPI_CONFIG_HOB_H_
#define _LPSS_SPI_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <LpssSpiConfig.h>

/**
  The LPSS_SPI_CONFIG_HOB describes settings for PCH.
**/
typedef struct {
  LPSS_SPI_DEVICE_CONFIG      SpiDeviceConfig[PCH_MAX_LPSS_SPI_CONTROLLERS];    ///< LPSS SPI Device Configuration
} LPSS_SPI_CONFIG_HOB;

#endif
