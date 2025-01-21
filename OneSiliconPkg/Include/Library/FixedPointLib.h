/** @file
  Fixed Point Library

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

@par Specification Reference:
**/

#ifndef _FIXED_POINT_16_LIB_H_
#define _FIXED_POINT_16_LIB_H_

//
// Attribute of the API ToSignedFixedPoint16, ToUnsignedFixedPoint16, FromUnsignedFixedPoint16 and FromSignedFixedPoint16
//

//
// No Accuracy loss happens when covert from and to fixed-point value and converting from a fixed-point value.
//
#define FIXED_POINT_CONVERT_WITHOUT_ACCURACY_LOSS  0

//
// When accuracy lost happens when covert from and to fixed-point value, including too large or too small input.
//
#define FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS  BIT0

//
// When the input value is larger than the max value the fixed-point value can represent.
//
#define FIXED_POINT_CONVERT_TOO_LARGE_INPUT  BIT1

//
// When the input value is negativem, and smaller than the min value the fixed-point value can represent.
//
#define FIXED_POINT_CONVERT_TOO_SMALL_INPUT  BIT2

/**
  This function converts a fixed-point, while maintaining
  precision by rounding and scaling the output according to the input magnification.
  An attribute is provided to indicate if any accuracy was lost during the conversion
  process.

  The function expects the unsigned input, FixedPointValue16, to follow the format
  U(IntegerBits, FractionBits). If FixedPointValue16 exceeds the representable range
  of the fixed-point format, (1 << (IntegerBits + FractionBits)) - 1, the function
  will return RETURN_INVALID_PARAMETER.

  @param[in]  FixedPointValue16  The fixed point value to be converted
  @param[in]  Magnification      The factor by which the output will be multiplied
  @param[in]  IntegerBits        The number of bits reserved for the integer part of the fixed point value
  @param[in]  FractionBits       The number of bits reserved for the fraction part of the fixed point value
  @param[out] *Output            Output Pointer to store the converted fixed point value
  @param[out] Attribute          Attribute of the converted result

  @retval  RETURN_INVALID_PARAMETER Invalid parameter
  @retval  EFI_SUCCESS              Successful
**/
RETURN_STATUS
EFIAPI
FromUnsignedFixedPoint16 (
  IN  UINT16  FixedPointValue16,
  IN  UINT32  Magnification,
  IN  INTN    IntegerBits,
  IN  INTN    FractionBits,
  OUT UINT64  *Output,
  OUT UINT32  *Attribute    OPTIONAL
  );

/**
  This function converts a fixed-point, while maintaining
  precision by rounding and scaling the output according to the input magnification.
  An attribute is provided to indicate if any accuracy was lost during the conversion
  process.

  The function expects the unsigned input, FixedPointValue16, to follow the format
  S(IntegerBits, FractionBits). If FixedPointValue16 exceeds the representable range
  of the fixed-point format, (1 << (IntegerBits + FractionBits)) - 1, the function
  will return RETURN_INVALID_PARAMETER.

  The "sign bit" will be manually extracted by calculating it from the format.

  @param[in]  FixedPointValue16  Fixed point value
  @param[in]  Magnification      The output is multiplied by Magnification
  @param[in]  IntegerBits        Integer bits of the fixed point
  @param[in]  FractionBits       Fraction bits of the fixed point
  @param[out] *Output            Output Pointer to store the converted fixed point value
  @param[out] Attribute          Attribute of the coverted result

  @retval  RETURN_INVALID_PARAMETER Invalid parameter
  @retval  EFI_SUCCESS              Successful
**/
RETURN_STATUS
EFIAPI
FromSignedFixedPoint16 (
  IN  UINT16  FixedPointValue16,
  IN  UINT32  Magnification,
  IN  INTN    IntegerBits,
  IN  INTN    FractionBits,
  OUT INT64  *Output,
  OUT UINT32  *Attribute    OPTIONAL
  );

/**
  Convert a rational (Numerator/Denominator) to a 16-bit fixed point
  signed value S(A, B), while A represents the integer bits,
  B represents fractional bits, and A+B must be less than or equal to 15.
  If the input value can be exactly represented by the fixed-point format,
  the output Attribute will be zero.
  If the input value can't be represented by the fixed-point format, the most
  approximate value(round half toward infinit) will be returned, and
  Attribute will record the reason.

  @param[in]  Numerator            Numerator of the target value
  @param[in]  Denominator          Denominator of the target value
  @param[in]  IntegerBits          Integer bits of the fixed point
  @param[in]  FractionBits         Fractional bits of the fixed point
  @param[out] Attribute            Attribute of the covert result

  @retval                          The fixed point value U(A, B).
**/
UINT16
EFIAPI
ToSignedFixedPoint16 (
  IN  INT32   Numerator,
  IN  UINT32  Denominator,
  IN  INTN    IntegerBits,
  IN  INTN    FractionBits,
  OUT UINT32  *Attribute    OPTIONAL
  );

/**
  Convert a rational (Numerator/Denominator) to a 16-bit fixed point
  unsigned value U(A, B),  while A represents the integer bits,
  B represents fractional bits, and A+B must be less than or equal to 16.
  If the input value can be exactly represented by the fixed-point format,
  the output Attribute will be zero.
  If the input value can't be represented by the fixed-point format, the most
  approximate value(Using round half toward infinity) will be returned, and
  Attribute will record the reason.

  @param[in]  Numerator            Numerator of the target value
  @param[in]  Denominator          Denominator of the target value
  @param[in]  IntegerBits          Integer bits of the fixed point
  @param[in]  FractionBits         Fractional bits of the fixed point
  @param[out] Attribute            Attribute of the covert result

  @retval                          The fixed point value U(A, B).
**/
UINT16
EFIAPI
ToUnsignedFixedPoint16 (
  IN  UINT32  Numerator,
  IN  UINT32  Denominator,
  IN  INTN    IntegerBits,
  IN  INTN    FractionBits,
  OUT UINT32  *Attribute    OPTIONAL
  );

#endif
