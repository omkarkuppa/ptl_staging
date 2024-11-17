/** @file
  Output debug messages to the debug port.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "CMrcTypes.h"
#include "MrcPostCodes.h"
#include "MrcDebugPrint.h"

///
/// These strings are used inside of C-code for non-debug print purposes.
///
const char *gDdrTypeStr[MAX_MRC_DDR_TYPE]  = {"LPDDR5", "DDR5", "UNKNOWN"}; // Note: Must align to MrcDdrType

#ifdef MRC_DEBUG_PRINT
///
/// Common Debug String Definition
///
const char Hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
/// Parsing Tags Start
const char gStartTagStr[]           = "START_";
const char gStopTagStr[]            = "STOP_";
/// Parsing Tags End
const char gStrZero[]               = "0";
const char gErrString[]             = "***ERROR***";
const char gTimeout[]               = "Timeout";
const char gWarnString[]            = "Warning:";
const char gNullPtrErrStr[]         = "Null Pointer Error";
const char gRdString[]              = "Read";
const char gWrString[]              = "Write";
const char gCmdString[]             = "Command";
const char gDataString[]            = "Data";
const char gSocString[]             = "SOC";
const char gDrvStr[]                = "Drive Strength";
const char gControllerStr[]         = "Controller";
const char gChannelStr[]            = "Channel";
const char gRankStr[]               = "Rank";
const char gByteStr[]               = "Byte";
const char gDqOdt[]                 = "DQ ODT";
const char gWrongInputParam[]       = "Wrong Input Parameter";
const char gArrayOverflow[]         = "Array Overflow";
const char gUnsupportedTechnology[] = "Unsupported DDR Technology";
const char *gIoOdtModeStr[]         = {"Default", "Vddq", "Vss", "Max"}; // Note: must match MRC_ODT_MODE_TYPE values
const char *gResTypeStr[MAX_RESULT_TYPE] = {"RxV", "RxT", "TxV", "TxT", "RcvEna", "WrLevel", "CmdT", "CmdV"};
const char gRankNotSupported[] = "RankNotSupported";
const char gWrongInputParameter[] = "WrongInputParameter";
const char gParamSaturation[] = "ParamSaturation";
const char gReadLevelingError[] = "ReadLevelingError";
const char  *gMarginTypesStr[] = {
  "RcvEna", // 0
  "RdT",
  "WrT",
  "WrDqsT",
  "RdV",
  "WrV",
  "WrL",
  "RdVBit",
  "RdVBit1",
  "RcvEnaX",
  "WrTUnMatched", // 10
  "CmdT",
  "CmdV",
  "CtlV",
  "RdTN",
  "RdTP",
  "RdTDbi",
  "WrTDbi",
  "RdVDbi",
  "MarginTypeMax"
};

const char *gMrcSkuTypeStr[MrcSkuTypeMax] = {
  "MrcSkuTypeUnspecified",
  "MrcSkuTypeM",
  "MrcSkuTypeP",
  "MrcSkuTypeS",
  "MrcSkuTypeU",
  "MrcSkuTypeH12Xe",
  "MrcSkuTypeH4Xe"
};

const char gMrcIpVersionStr[]  = "Version unsupported.";
const char gMrcIpSegmentStr[]  = "Segment Type unsupported.";
const char gMrcIpSteppingStr[] = "Unknown stepping, using MRC for last known step";

/**
  Convert an unsigned integer to a string.

  @param[in]  MrcCall         - Optional when value is 32-bit, required for 64-bit values.
  @param[in]  Value           - Value to work on.
  @param[in]  ValueSize       - The size of the value in bytes with valid values of 4 and 8.
  @param[out] Str             - The return string to print.
  @param[in]  Width           - The width of string to print
  @param[in]  Flags           - The flag type to print out, like '-' or '+'.
  @param[in]  Base            - Number base to work on, as in 10, or 16.

  @retval Number of characters in the resulting string.
**/
UINT32
MrcUintnToStr (
  IN  const MRC_FUNCTION *const MrcCall,
  IN  const UINT64 Value,
  IN  const UINT32 ValueSize,
  OUT char         *Str,
  IN  const UINT32 Width,
  IN  const UINT32 Flags,
  IN  const UINT32 Base
  )
{
  volatile char *Ptr;
  UINT32        Negative;
  UINT64        Int;
  UINT64        Remainder;
  UINT32        i;
  char          Prefix;
  char          c;

  Ptr = Str;
  if ((ValueSize == sizeof (UINT32)) && (Value > MRC_INT32_MAX) && (Flags & MRC_INT_SIGNED)) {
    Int       = (~Value & 0xFFFFFFFF) + 1; /* -Value */
    Negative  = 1;
  } else if ((ValueSize == sizeof (UINT64)) && (Value > MRC_INT64_MAX) && (Flags & MRC_INT_SIGNED)) {
    Int       = ~Value + 1; /* -Value */
    Negative  = 1;
  } else {
    Int       = Value;
    Negative  = 0;
  }

  i = 0;
  do {           /* Generate digits in reverse order */
    i++;
    Remainder = 0;
    if (ValueSize == sizeof (UINT64)) {
      Int = MrcCall->MrcDivU64x64 (Int, Base, &Remainder);
    } else {
      Remainder = (UINT32) Int % Base;
      Int       = (UINT32) Int / Base;
    }
    *Ptr++ = Hex[(UINT32) Remainder]; // only works right now on base 16
    if (Flags & MRC_COMMA_TYPE) {
      if (Base == 16) {
        if (i % 8 == 0) {
          *Ptr++ = '_';
        }
      } else if (Base == 10) {
        if ((i % 3) == 0) {
          *Ptr++ = ',';
        }
      }
    }
  } while (Int > 0);
  if ((*(Ptr - 1) == ',') || (*(Ptr - 1) == '_')) {
    Ptr--;
  }

  if (Negative) {
    *Ptr++ = '-';
    i++;
  } else if (Flags & MRC_PREFIX_SIGN) {
    *Ptr++ = '+';
    i++;
  }

  if (Flags & MRC_PREFIX_ZERO) {
    Prefix = '0';
  } else if (!(Flags & MRC_LEFT_JUSTIFY)) {
    Prefix = ' ';
  } else {
    Prefix = 0x00;
  }

  if (Prefix != 0x00) {
    for (i = (int) (Ptr - Str); i < Width; i++) {
      *Ptr++ = Prefix;
    }
  }

  *Ptr = '\0';

  /* Reverse string */
  while (Str < --Ptr) {
    c       = *Str;
    *Str++  = *Ptr;
    *Ptr    = c;
  }

  return i;
}

/**
  Convert a string to a number.

  @param[in, out] String - String to convert.

  @retval Returns the string in number.
**/
UINT32
MrcStrToNumber (
  IN OUT char **String
  )
{
  UINT32  Sum;
  char    *Str;

  Str = *String;
  if (*Str == '0') {
    Str++;
  }

  Sum = 0;
  while (MRC_ISDIGIT (*Str)) {
    Sum = Sum * 10 + (*Str++ -'0');
  }

  *String = Str;
  return Sum;
}

/**
  Compute the number of bytes in the string to which String points, not including the terminating NUL character.

  @param[in] String - Pointer to the MRC global data area.

  @retval The number of characters in the input String.
**/
INT32
MrcStrLen (
  char *String
 )
{
  INT32 Count;
  Count = 0;
  while (*String != '\0' && Count < MAX_STRING_LENGTH) {
    Count++;
    String++;
  }
  return Count;
}

/*++
  Format string using specified format specifier.
  Supporting up to 64-bit unsigned integer.

  @param[in] Format     - String containing characters to print and formatting data.
    %[flags][width]type
    [flags] '-' left align
    [flags] '+' prefix with sign (+ or -)
    [flags] '0' zero pad numbers
    [flags] ' ' prefix black in front of postive numbers
    [flags] 'l' or 'll' used with x, X, u, d, i types to indicate 64-bit value.
    [flags] ','|'_' used with x, X, u, d, i types to add comma (dec) or underscore (hex) to display value. Specify after 'l' flag when 'l' flag is used.
    [width] non negative decimal integer that specifies the width to print a value.
    [width] '*' get the width from a int argument on the stack.
    type    'd'|'i' signed decimal integer
    type    'u' unsigned integer
    type    'x'|'X' hexidecimal using "ABCDEF"
    type    'c' print character
    type    'p' print a pointer to void
    type    's' print a null terminated string
    type    '%%' prints a percent sign
  @param[in] Marker     - Variable argument marker.
  @param[in] BufferSize - Size of the buffer, in bytes.
  @param[in] Buffer     - The buffer.

  @retval Number of characters printed.
**/
int
MrcStringFormatter (
  IN const MRC_FUNCTION     *const MrcCall,
  IN const char *const      Format,
  IN MrcVaList              Marker,
  IN UINTN                  BufferSize,
  IN OUT UINT8              *Buffer
  )
{
  char        *p;
  char        *String;
  UINT64      ArgValue;
  UINT32      ArgSize;
  UINT32      Width;
  INT32       WidthRemainder;
  UINT32      ArgStrLen;
  UINT32      Flags;
  UINT32      CharCount;
  UINT32      RetrunedStringSize;
  char        LocalBuffer[21]; // Enough to hold all digits of UINT64 plus the null-terminator

  CharCount          = 0;
  RetrunedStringSize = 0;

  if (Format != NULL) {
    for (p = (char *) Format; *p && (CharCount < BufferSize); p++) {
      if (*p != '%') {
        if (*p == MRC_CHAR_LF) {
          //
          // Make LF into CR LF
          //
#if !defined(XCC_BUILD)
          MRC_PUTCC (Buffer, MRC_CHAR_CR, CharCount);
#endif
        }

        MRC_PUTCC (Buffer, *p, CharCount);
      } else {
        p++;
        //
        // Check for flags
        //
        Flags = 0;
        if (*p == '-') {
          Flags |= MRC_LEFT_JUSTIFY;
        } else if (*p == '+') {
          Flags |= MRC_PREFIX_SIGN;
        } else if (*p == ' ') {
          Flags |= MRC_PREFIX_BLANK;
        }

        if (Flags != 0) {
          p++;
        }
        //
        // Check for width
        //
        if (MRC_ISDIGIT (*p)) {
          if (*p == '0') {
            Flags |= MRC_PREFIX_ZERO;
          }

          Width = MrcStrToNumber (&p);
        } else if (*p == '*') {
          Width = MRC_VA_ARG (Marker, int);
          p++;
        } else {
          Width = 0;
        }

        // Check for "long" or "long long"
        if (*p == 'l') {
          Flags |= MRC_LONG_TYPE;
          if (*(p + 1) == 'l') {  // Treat 'll' same as 'l'
            p++;
          }
          p++;
        }

        if ((*p == ',') || (*p == '_')) {
          Flags |= MRC_COMMA_TYPE;
          p++;
        }
        //
        // Get type
        //
        switch (*p) {
          case 'd':
          case 'i':
            Flags |= MRC_INT_SIGNED;
            /*FALLTHROUGH*/
          case 'u':
            if (Flags & MRC_LONG_TYPE) {
              ArgSize  = sizeof (UINT64);
              ArgValue = (UINT64) MRC_VA_ARG (Marker, UINT64);
            } else {
              ArgSize  = sizeof (UINT32);
              ArgValue = (UINT64) MRC_VA_ARG (Marker, UINT32);
            }

            RetrunedStringSize = MrcUintnToStr (MrcCall, ArgValue, ArgSize, LocalBuffer, Width, Flags, 10);
            for (UINT8 Index = 0; ((Index < RetrunedStringSize) && (CharCount < BufferSize)) ; Index++) {
              MRC_PUTCC(Buffer, LocalBuffer[Index], CharCount);
            }
            break;

          case 'x':
          case 'X':
          case 'p':
            if (Flags & MRC_LONG_TYPE) {
              ArgSize  = sizeof (UINT64);
              ArgValue = (UINT64) MRC_VA_ARG (Marker, UINT64);
            } else {
              ArgSize  = sizeof (UINT32);
              ArgValue = (UINT64) MRC_VA_ARG (Marker, UINT32);
            }

            RetrunedStringSize = MrcUintnToStr (MrcCall, ArgValue, ArgSize, LocalBuffer, Width, Flags, 16);
            for (UINT8 Index = 0; ((Index < RetrunedStringSize) && (CharCount < BufferSize)) ; Index++) {
              MRC_PUTCC(Buffer, LocalBuffer[Index], CharCount);
            }
            break;

          case 'c':
            MRC_PUTCC (Buffer, (UINT8) MRC_VA_ARG (Marker, int), CharCount);
            if (CharCount < BufferSize) {
              Buffer[CharCount] = '\0';
            }
            break;

          case 's':
            String = (char *) MRC_VA_ARG (Marker, char *);
            ArgStrLen = MrcStrLen (String);
            WidthRemainder = Width - ArgStrLen;
            if ((Flags & MRC_LEFT_JUSTIFY) == 0) {
              // Right justify: pad with leading spaces
              while (WidthRemainder > 0) {
                MRC_PUTCC (Buffer, ' ', CharCount);
                WidthRemainder--;
              }
            }
            while ((*String != '\0') && (CharCount < BufferSize)) {
              if (*String == MRC_CHAR_LF) {
                MRC_PUTCC (Buffer, MRC_CHAR_CR, CharCount);
              }
              MRC_PUTCC (Buffer, *String++, CharCount);
            }
            if (Flags & MRC_LEFT_JUSTIFY) {
              // Left justify: pad with trailing spaces
              while (WidthRemainder > 0) {
                MRC_PUTCC (Buffer, ' ', CharCount);
                WidthRemainder--;
              }
            }
            break;

          case '%':
            MRC_PUTCC (Buffer, '%', CharCount);
#ifndef MRC_MINIBIOS_BUILD
            MRC_PUTCC (Buffer, '%', CharCount); // DEBUG() will treat it as format string, so add one more %
#endif
            break;
        }
      }
    }

    if (CharCount < BufferSize) {
      MRC_PUTCC (Buffer, '\0', CharCount);
    } else {
      Buffer[BufferSize - 1] = '\0';
    }
  }

  return CharCount;
}
#endif
