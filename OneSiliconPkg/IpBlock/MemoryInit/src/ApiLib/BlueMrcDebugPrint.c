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


#ifdef MRC_DEBUG_PRINT

/**

@brief

  This function converts a character to uppercase.

  @param[in] c - The character to convert.

  @retval Returns the converted character.

**/
unsigned char
Mrctoupper (
  const unsigned char c
  )
{
  return ((c) >= 'a') && ((c) <= 'z') ? ((c) - ('a' - 'A')) : (c);
}

/**

@brief

  This function converts a string to an unsigned long value.
  This function does not implement the full strtoul functionality.

  @param[in]  nptr   - Null-terminated string to convert.
  @param[out] endptr - Pointer to character that stops the scan.

  @retval Returns the converted value.

**/
unsigned long
MrcStrtoul (
   const char *nptr,
   char **endptr
)
{
  unsigned long value;
  unsigned char count;
  unsigned char hex;
  unsigned char c;
  unsigned char c1;

  value = 0;
  count = 0;
  c     = *nptr;
  c1    = Mrctoupper (*(nptr + 1));
  if ((c == '0') && (c1 == 'X')) {
    nptr += 2;
    hex = 1;
  } else {
    hex = 0;
  }
  while (count < 8) {
    c = Mrctoupper (*nptr);
    if (hex) {
      if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F'))) {
        nptr++;
        value <<= 4;
        if ((c >= '0') && (c <= '9')) {
          value |= c - '0';
        } else {
          value |= c - 'A' + 10;
        }
      } else {
        break;
      }
    } else {
      if ((c >= '0') && (c <= '9')) {
        nptr++;
        value = (value * 10) + (c - '0');
      } else {
        break;
      }
    }
  }
  if (endptr != NULL) {
    *endptr = (char *) nptr;
  }
  return (value);
}
#endif

/**

@brief

  This function compares two strings, up to count characters or first null character in either string.

  @param[in] String1 - First null-terminated string to compare.
  @param[in] String2 - Second null-terminated string to compare.
  @param[in] Count   - Number of characters to compare, starting from beginning.

  @retval 0 on compare or initial Count value == 0.
  @retval < 0, string1 substring less than string2 substring.
  @retval > 0, string2 substring less than string1 substring.

**/
int
Mrcstrncmp (
  const char *String1,
  const char *String2,
  int        Count
)
{
  int Loop;

  if (Count != 0) {
    for ( Loop = 0; Loop < Count; Loop++) {
      if ((*String1 == '\0') || (*String1 != *String2)) {
        return (*((unsigned char *) String1) - *((unsigned char *) String2));
      }
      String1++;
      String2++;
    }
  }
  return (0);
}

#ifdef MRC_DEBUG_PRINT

/**
  Read line status register from the uart device.

  @param[in] MrcData     - Pointer to the MRC global data area.

  @retval Returns the read status.
**/
char
GetLineStatusRegister (
  MrcParameters *MrcData
  )
{
   UINT8        UartStatus;
   MRC_FUNCTION *MrcCall;

   MrcCall    = MrcData->Inputs.Call.Func;
   UartStatus = MrcCall->MrcIoRead8 (GLOBALCOMPORT + LINE_STATUS);
   return ((char) UartStatus);
}


/**
  Read break status from the uart device.

  @param[in] MrcData  - Pointer to the MRC global data area.

  @retval 1 = break, 0 = no break.
**/
char
GetBreak (
  MrcParameters *MrcData
)
{
  UINT8 UartStatus;

  UartStatus = GetLineStatusRegister (MrcData);
  return ((UartStatus & BREAK_INTERRUPT) ? 1 : 0);
}

/**
  Read char from the uart device.

  @param[in] MrcData  - Pointer to the MRC global data area.

  @retval Returns the read char.
**/
char
MrcGetChar (
  MrcParameters *MrcData
  )
{
  MRC_FUNCTION *MrcCall;

  MrcCall = MrcData->Inputs.Call.Func;
  return (((GetLineStatusRegister (MrcData) & DATA_READY) == DATA_READY) ? (char) MrcCall->MrcIoRead8 (GLOBALCOMPORT + RECEIVER_BUFFER) : MRC_EOF);
}

/**
  Put char in the uart device.

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] c        - char to put in the uart.

  @retval Returns the output char.
**/
int
MrcPutChar (
  MrcParameters *MrcData,
  int c
  )
{
   UINT8        UartStatus;
   MRC_FUNCTION *MrcCall;

   MrcCall = MrcData->Inputs.Call.Func;
   // Wait to uart empty
   do {
      UartStatus = MrcCall->MrcIoRead8 (GLOBALCOMPORT + LINE_STATUS);
   } while ((UartStatus & TRANS_HOLDING_REG_EMPTY) != TRANS_HOLDING_REG_EMPTY);

   // Send the char
   MrcCall->MrcIoWrite8 (GLOBALCOMPORT + TRANSMIT_HOLDING, (UINT8) c);

   return c;
}

/**
  Print message and wait for user to enter a numeric value and press ENTER in the terminal.
  Used for debug.

  @param[in] MrcData - Pointer to the MRC global data area.
  @param[in] Prompt  - Debug message

  @retval - the number that was entered in the terminal
**/
UINT32
MrcGetNumber (
  IN MrcParameters *MrcData,
  IN char *Prompt
  )
{
  char   s[16];
  char   *EndPtr;
  UINT32 Data32;

  Data32 = 0;
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "\n--> %s ", Prompt);
  MrcGets (MrcData, s, sizeof (s));
  Data32 = MrcStrtoul ((const char *) s, &EndPtr);
  return Data32;
}
/**
  This function outputs a string to the default device.

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] String   - The null terminated string to output.

  @retval Returns a nonnegative value if successful.
**/
int
MrcPuts (
  MrcParameters *MrcData,
  const char *String
  )
{
  int count = 0;
  while (*String != '\0') {
    MrcPutChar (MrcData, *String++);
    count++;
  }
  return (count);
}

/**
  Print message and wait for user to press ENTER in the terminal. Used for debug.

  @param[in] MrcData - Pointer to the MRC global data area.
  @param[in] Info    - Debug message
**/
void
MrcHitEnter (
  MrcParameters *MrcData,
  char *Info
  )
{
  char s[16];

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "\n---- Hit Enter: %s ----\n", Info);
  MrcGets (MrcData, s, sizeof (s));
}

/**
  This function reads a null terminated string into a buffer.

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] Buffer   - Buffer to copy string into.
  @param[in] Count    - Size of the buffer, in characters.

  @retval Returns the converted value.

**/
char *
MrcGets (
  MrcParameters *MrcData,
  char *Buffer,
  int  Count
)
{
  char In;
  int  Index;
  char Ctl;

  Index = 0;
  do {
    In  = MrcGetChar (MrcData);
    Ctl = MRC_ISPRINT (In);
    if ((Ctl > 0) || (In == MRC_CHAR_CR) || (In == MRC_CHAR_DELETE) || (In == MRC_CHAR_BACKSPACE)) {
      if (Ctl > 0) {
        MrcPutChar (MrcData, In);
        Buffer[Index++] = In;
      } else if ((In == MRC_CHAR_DELETE) || (In == MRC_CHAR_BACKSPACE)) {
        if (Index > 0) {
          MrcPutChar (MrcData, In);
          MrcPutChar (MrcData, ' ');
          MrcPutChar (MrcData, In);
          Index--;
        }
      } else if (In == MRC_CHAR_CR) {
        MrcPutChar (MrcData, In);
        MrcPutChar (MrcData, MRC_CHAR_LF);
      }
    }
  } while ((Index < (Count - 1)) && (In != MRC_CHAR_CR));
  Buffer[Index] = '\0';
  return (Buffer);
}

/*++

  Print to output stream/device. Limited support for sizes other than
  unsigned 32-bit to save code space. Type overrides like {h | I | I64 | L}
  are not supported.

  @param[in] Debug  - Location to store debug message print information for future use.
  @param[in] Level  - The debug level.
  @param[in] Format - String containing characters to print and formatting data.
    %[flags][width]type
    [flags] '-' left align
    [flags] '+' prefix with sign (+ or -)
    [flags] '0' zero pad numbers
    [flags] ' ' prefix black in front of postive numbers
    [flags] 'l' used with x, X, u, d, i types to indicate 64-bit value.
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
  @param[in] Marker - Variable argument marker

  @retval Number of characters printed.
**/
int
MrcPrintfVaList (
  IN MrcDebug *const    Debug,
  IN MrcDebugMsgLevel   Level,
  IN const char *const  Format,
  IN MrcVaList          Marker
  )
{
  const MrcInput     *Inputs;
  const MRC_FUNCTION *MrcCall;
  UINT32             CharCount;
  UINT8              Buffer[MAX_STRING_LENGTH];

  Inputs    = (const MrcInput *) &((MrcParameters *)(Debug->MrcData.Ptr))->Inputs;
  MrcCall   = Inputs->Call.Func;
  CharCount = 0;
  if (Debug->LowestStackAddr.DataN >= (UINTN) Buffer) {
    Debug->LowestStackAddr.DataN = (UINTN) Buffer;
  }

  // Skip printing
  if ((Format == NULL) || (Level == MSG_LEVEL_NONE) || (Debug->Level == MSG_LEVEL_NONE)) {
    return CharCount;
  }

  // Skip printing
  if ((Debug->Level != MSG_LEVEL_ALL) && !(Debug->Level & Level)) {
    return CharCount;
  }

  // Format the string to be printed
  CharCount = MrcStringFormatter (MrcCall, Format, Marker, sizeof (Buffer), Buffer);

  // Print the string
  if (Debug->Stream.DataN > 0) {
#ifdef MRC_MINIBIOS_BUILD
    MrcCall->MrcPrintString (Debug->MrcData.Ptr, Buffer);
#else
    MrcCall->MrcPrintString (Buffer);
#endif
  }

  return CharCount;
}
#endif // MRC_DEBUG_PRINT

/**
  Write formatted data to a string.
  Uses same format specifiers as MrcPrintf().

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] String   - The string buffer to use. Max size is MAX_STRING_LENGTH.
  @param[in] Size     - Buffer size.
  @param[in] Format   - String containing characters to print and formatting data.
  @param[in] ...      - Variable number of arguments to print

  @retval Number of characters printed, including the terminating NULL character.
**/
int
MrcSPrintf (
  IN const MrcParameters * const  MrcData,
  IN OUT char                     *String,
  IN UINTN                        Size,
  IN const char *const            Format,
  ...
  )
{
#ifdef MRC_DEBUG_PRINT
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  MrcVaList           Marker;

  MRC_VA_START (Marker, Format);
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  return MrcStringFormatter (MrcCall, Format, Marker, Size, (UINT8 *) String);
#else
  return 0;
#endif // MRC_DEBUG_PRINT
}

/**
  Print to output stream/device.  Gets the start Marker for the variable arguments
  and calls MrcPrintfVaList().

  @param[in] Debug  - Location to store debug message print information for future use.
  @param[in] Level  - The debug level.
  @param[in] Format - String containing characters to print and formatting data.
    %[flags][width]type
    [flags] '-' left align
    [flags] '+' prefix with sign (+ or -)
    [flags] '0' zero pad numbers
    [flags] ' ' prefix black in front of postive numbers
    [flags] 'l' used with x, X, u, d, i types to indicate 64-bit value.
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
  @param[in] ...    - Variable number of arguments to print

  @retval Number of characters printed.
**/
int
MrcPrintf (
  IN MrcDebug *const    Debug,
  IN MrcDebugMsgLevel   Level,
  IN const char *const  Format,
  ...
  )
{
#ifdef MRC_DEBUG_PRINT
  MrcVaList Marker;

  MRC_VA_START (Marker, Format);

  return (Debug != NULL) ? MrcPrintfVaList (Debug, Level, Format, Marker) : 0;
#else
  return 0;
#endif // MRC_DEBUG_PRINT
}

#ifdef MRC_DEBUG_PRINT
/**
  printf is used for format checking but does not get called.
  This stub is required by the MiniBIOS build target.
**/
int
printf (
  const char *format,
  ...
  )
{
  return 0;
}

/**
  Save the output stream/device. This function must be called prior to debug
  string output.

  @param[in, out] Global - Pointer to the MRC global data structure.
  @param[in]      Level  - The debug level to set. Messages at or above this level are printed.
  @param[in]      Stream - Pointer to the stream/device to use.

  @retval Nothing.
**/
void
MrcFopen (
  IN OUT void *const    Global,
  IN  MrcDebugMsgLevel  Level,
  IN  UINT32            Stream
  )
{
  MrcParameters      *MrcData;
  MrcDebug           *Debug;
  MrcInput           *Inputs;
  const MRC_FUNCTION *MrcCall;

  MrcData             = (MrcParameters *) Global;
  Inputs              = &MrcData->Inputs;
  MrcCall             = Inputs->Call.Func;
  Debug               = &MrcData->Outputs.Debug;
  Debug->MrcData.Ptr  = MrcData;
  Debug->Level        = Level;
  Debug->Stream.DataN = (UINTN) Stream;
  Debug->TopStackAddr.DataN     = Inputs->MrcStackTop.DataN;
  Debug->LowestStackAddr.DataN  = Inputs->MrcStackTop.DataN;
  MrcCall->MrcSetMem ((UINT8 *) &Debug->PostCode[0], sizeof (Debug->PostCode), 0);
  Debug->Start.DataN   = 0;
  Debug->End.DataN     = 0;
  Debug->Current.DataN = 0;

  return;
}

#endif // MRC_DEBUG_PRINT

