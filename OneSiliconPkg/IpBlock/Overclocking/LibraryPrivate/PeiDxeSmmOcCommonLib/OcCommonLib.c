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
#include <Library/BaseLib.h>
#include <Library/OcCommonLib.h>
#include <Library/OcMailboxLib.h>

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
  )
{
  UINT32 Remainder;
  UINT64 Voltage;
  ///  Fixed point representation:
  ///
  ///  U12.2.10V format
  ///  | | | |
  ///  | | | v
  ///  | | v Exponent
  ///  | v Significand Size
  ///  v Size
  ///  Signed/Unsigned
  ///
  ///  Float Value = Significand x (Base ^ Exponent)
  ///  (Base ^ Exponent) = 2 ^ 10 = 1024
  ///
  Remainder = 0;

  if (InputVoltageTarget == 0) {
    *OutputVoltageTarget = 0;
    return;
  }

  if (ConversionType == CONVERT_TO_FIXED_POINT_VOLTS) {
    ///
    /// Input Voltage is in number of millivolts. Clip the input Voltage
    /// to the max allowed by the fixed point format
    ///
    if (InputVoltageTarget > MAX_TARGET_MV)
      InputVoltageTarget = MAX_TARGET_MV;

    ///
    /// InputTargetVoltage is the significand in mV. Need to convert to Volts
    ///
    Voltage = MultU64x32 (1024, InputVoltageTarget);
    *OutputVoltageTarget = (UINT16) DivU64x32Remainder (Voltage, MILLIVOLTS_PER_VOLT, &Remainder);

    if (Remainder >= 500) {
      *OutputVoltageTarget += 1;
    }
  } else if (ConversionType == CONVERT_TO_BINARY_MILLIVOLT) {
    ///
    /// InputVoltage is specified in fixed point representation, need to
    /// convert to millivolts
    ///
    Voltage = MultU64x32 (MILLIVOLTS_PER_VOLT, InputVoltageTarget);
    *OutputVoltageTarget = (UINT16) DivU64x32Remainder (Voltage, 1024, &Remainder);

    if (Remainder >= 500) {
      *OutputVoltageTarget += 1;
    }
  }
}
