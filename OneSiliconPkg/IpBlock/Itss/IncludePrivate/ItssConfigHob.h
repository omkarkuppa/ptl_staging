/** @file
  ITSS config HOB definition.

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

#ifndef _ITSS_CONFIG_HOB_H_
#define _ITSS_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>
#include <InterruptConfig.h>

/**
  The ITSS_CONFIG_HOB describes interrupt settings for PCH.
**/
typedef struct {
UINT8                        NumOfDevIntConfig;                               ///< Number of entries in DevIntConfig table
UINT8                        GpioIrqRoute;                                    ///< Interrupt routing for GPIO. Default is <b>14</b>.
PCH_DEVICE_INTERRUPT_CONFIG  DevIntConfig[PCH_MAX_DEVICE_INTERRUPT_CONFIG];   ///< Array which stores PCH devices interrupts settings
} ITSS_CONFIG_HOB;

#endif
