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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <PiPei.h>
#include <Library/FixedPointLib.h>

/**
  This function converts a fixed-point value, while maintaining
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
  )
{
  UINT32  FixedPoint16Mask;
  UINT32  FractionBitsPoint16;
  UINT32  Remainder;
  INT64   Integer;
  UINT64  Fraction;

  //
  // Only supports below config.
  //
  ASSERT ((IntegerBits >= 0) && (IntegerBits <= 16));
  ASSERT ((FractionBits >= 0) && (FractionBits <= 16));
  ASSERT (IntegerBits + FractionBits <= 16);
  FixedPoint16Mask = (1 << (IntegerBits + FractionBits)) - 1;

  //
  // Verify if the input fixed point value falls within the specified format's limitations.
  //
  if ((FixedPointValue16 & FixedPoint16Mask) != FixedPointValue16) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // Conversion and scaling
  //
  FractionBitsPoint16 = (1 << ((UINT32)FractionBits)) - 1;
  Integer             = (INT64)(FixedPointValue16 >> FractionBits) * Magnification;
  Fraction            = DivU64x32Remainder (((FixedPointValue16 & FractionBitsPoint16) * Magnification), 1<<FractionBits, &Remainder);
  *Output             = Integer + Fraction;

  //
  // Rounding
  //
  if (Remainder*2 >= (UINT32)(1 << FractionBits)) {
    (*Output)++;
  }

  //
  // Determing the precision loss
  //
  if (Attribute != NULL) {
    *Attribute  = FIXED_POINT_CONVERT_WITHOUT_ACCURACY_LOSS;
    *Attribute |= (Remainder == 0) ? 0 : FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
  }

  return EFI_SUCCESS;
}

/**
  This function converts a fixed-point value, while maintaining
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
  OUT INT64   *Output,
  OUT UINT32  *Attribute    OPTIONAL
  )
{
  UINT32  FixedPoint16Mask;
  UINT32  FractionBitsPoint16;
  INT32   Sign;
  UINT64  AbsoluteFixedPointValue16;
  INT64   Integer;
  UINT64  Fraction;
  UINT32  Remainder;

  //
  // Only supports below config.
  //
  ASSERT ((IntegerBits >= 0) && (IntegerBits <= 16));
  ASSERT ((FractionBits >= 0) && (FractionBits <= 16));
  ASSERT (IntegerBits + FractionBits <= 16);
  FixedPoint16Mask    = (1 << (IntegerBits + FractionBits)) - 1;
  FractionBitsPoint16 = (1 << (FractionBits)) - 1;

  if ((FixedPointValue16 & ((1 << (IntegerBits + FractionBits + 1)) - 1)) != FixedPointValue16) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // The sign bit of the fixed point format is dependent on the specific format used instead of the highest bit.
  //
  Sign = ((FixedPointValue16 >> (IntegerBits + FractionBits)) & 1) == 1 ? -1 : 1;

  //
  // Extend the sign bit and then obtain the absolute value.
  //
  if (Sign == -1) {
    AbsoluteFixedPointValue16 = ABS (FixedPointValue16 | (INT32) ~FixedPoint16Mask);
  } else {
    AbsoluteFixedPointValue16 = FixedPointValue16 & FixedPoint16Mask;
  }

  //
  // Conversion and scaling
  //
  Integer  = (INT64)(AbsoluteFixedPointValue16 >> FractionBits) * Magnification;
  Fraction = DivU64x32Remainder (((AbsoluteFixedPointValue16 & FractionBitsPoint16) * Magnification), 1<<FractionBits, &Remainder);
  *Output  = Integer + Fraction;

  //
  // Rounding
  //
  if (Remainder*2 >= (UINT32)(1 << FractionBits)) {
    (*Output)++;
  }

  //
  // Reattached the sign
  //
  *Output *= Sign;

  //
  // Determing the precision loss
  //
  if (Attribute != NULL) {
    *Attribute  = FIXED_POINT_CONVERT_WITHOUT_ACCURACY_LOSS;
    *Attribute |= (Remainder == 0) ? 0 : FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
  }

  return EFI_SUCCESS;
}

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
  )
{
  INT32   InputSigned;
  UINT64  PositiveValue;
  UINT16  ReturnValue;
  UINT16  MaxFixedPoint16;
  UINT16  MinFixedPoint16;
  UINT32  Remainder;

  //
  // Currently, only supports below config.
  //
  ASSERT ((IntegerBits >= 0) && (IntegerBits <= 15));
  ASSERT ((FractionBits >= 0) && (FractionBits <= 15));
  ASSERT (IntegerBits + FractionBits <= 15);
  MaxFixedPoint16 = (1 << (UINTN)(IntegerBits + FractionBits)) - 1;
  MinFixedPoint16 = (1 << (UINTN)(IntegerBits + FractionBits));

  //
  // +(Denominator/2) to use ROUND instead of FLOOR to have better accuracy.
  // Always make sure operands of division opration is positive because negetive division is not well defined.
  //
  InputSigned   = (Numerator < 0) ? -1 : 1;
  PositiveValue = LShiftU64 (ABS ((INT64)(UINT64)Numerator), (UINTN)FractionBits);
  PositiveValue = DivU64x32Remainder (PositiveValue, Denominator, &Remainder);

  if ((Remainder != 0) && (Remainder >= Denominator/2)) {
    PositiveValue++;
  }

  if (Attribute != NULL) {
    *Attribute  = FIXED_POINT_CONVERT_WITHOUT_ACCURACY_LOSS;
    *Attribute |= (Remainder == 0) ? 0 : FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
  }

  if (PositiveValue > MaxFixedPoint16) {
    if (Attribute != NULL) {
      *Attribute |= (Numerator < 0) ? FIXED_POINT_CONVERT_TOO_SMALL_INPUT : FIXED_POINT_CONVERT_TOO_LARGE_INPUT;
      *Attribute |= FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
    }

    return (Numerator < 0) ? MinFixedPoint16 : MaxFixedPoint16;
  } else {
    ReturnValue  = (UINT16)(InputSigned * (INT32)(INT64)PositiveValue);
    ReturnValue &= ((1 << (IntegerBits + FractionBits + 1)) - 1);
    return ReturnValue;
  }
}

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
  )
{
  UINT64  Value;
  UINT16  MaxFixedPoint16;
  UINT32  Remainder;

  //
  // Currently, only supports below config.
  //
  ASSERT ((IntegerBits >= 0) && (IntegerBits <= 16));
  ASSERT ((FractionBits >= 0) && (FractionBits <= 16));
  ASSERT (IntegerBits + FractionBits <= 16);
  MaxFixedPoint16 = (1 << (UINTN)(IntegerBits + FractionBits)) - 1;

  Value = LShiftU64 ((UINT64)Numerator, (UINTN)FractionBits);
  //
  // +(Denominator/2) to use ROUND instead of FLOOR to have better accuracy.
  //
  Value = DivU64x32Remainder ((UINT64)Value, Denominator, &Remainder);

  if ((Remainder != 0) && (Remainder >= Denominator/2)) {
    Value++;
  }

  if (Attribute != NULL) {
    *Attribute  = FIXED_POINT_CONVERT_WITHOUT_ACCURACY_LOSS;
    *Attribute |= (Remainder == 0) ? 0 : FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
  }

  if (Value > MaxFixedPoint16) {
    if (Attribute != NULL) {
      *Attribute |= FIXED_POINT_CONVERT_TOO_LARGE_INPUT;
      *Attribute |= FIXED_POINT_CONVERT_WITH_ACCURACY_LOSS;
    }

    return MaxFixedPoint16;
  } else {
    return (UINT16)Value;
  }
}
