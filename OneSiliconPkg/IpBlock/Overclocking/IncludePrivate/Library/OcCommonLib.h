/** @file
  Header file for overclocking common library implementation.

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

#ifndef _OC_COMMON_LIB_H_
#define _OC_COMMON_LIB_H_

#include <Uefi/UefiBaseType.h>

/**
  Converts the input voltage target to the fixed point U12.2.10 Volt format or
  the Binary millivolts representation based on the ConversionType

  @param[in]   InputVoltageTarget     UINT16 voltage input value with fixed point U12.2.10 Volt format
                                      or Binary millivolts representation
  @param[out]  OutputVoltageTarget    A pointer to UINT16 voltage output value
  @param[in]   ConversionType         0:fixed point, 1:Binary millivolts
**/
VOID
EFIAPI
ConvertVoltageTarget (
  IN   UINT16  InputVoltageTarget,
  OUT  UINT16  *OutputVoltageTarget,
  IN   UINT8   ConversionType
  );

#endif
