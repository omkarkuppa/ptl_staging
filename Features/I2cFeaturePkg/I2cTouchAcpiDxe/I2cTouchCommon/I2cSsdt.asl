/** @file
  I2C Rtd3 Ssdt table
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#include <I2cDevices.h>

DefinitionBlock (
    "Rtd3.aml",
    "SSDT",
    2,
    "Rtd3",
    "I2C_DEVT",
    0x1000
    )
{
  External (\ADBG,MethodObj)
  External (TPLM)
  External (TPDM)
  Include("I2cDevices.asl")
  Include ("I2cRtd3Buffer.asl")
  Include ("I2cRtd3.asl")
}
