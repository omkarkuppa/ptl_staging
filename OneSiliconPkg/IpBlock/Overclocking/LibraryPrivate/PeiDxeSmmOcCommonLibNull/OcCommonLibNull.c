/** @file
  Overclocking common library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/OcCommonLib.h>

/**
  Converts the input voltage target to the fixed point U12.2.10 Volt format or
  the Binary millivolts representation based on the ConversionType

  @param[in]  InputVoltageTarget
  @param[out] *OutputVoltageTarget
  @param[in]  ConversionType - 0:fixed point, 1:Binary millivolts
**/
VOID
EFIAPI
ConvertVoltageTarget (
  IN   UINT16  InputVoltageTarget,
  OUT  UINT16  *OutputVoltageTarget,
  IN   UINT8   ConversionType
  )
{
}
