/** @file
  Instance of Debug Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiDxe.h>
#include <Protocol/StatusCode.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>

//
// Define the maximum debug and assert message length that this library supports
//
#define MAX_DEBUG_MESSAGE_LENGTH  0x100

//
// The maximum size of an EFI_DEBUG_INFO structure.
// This is from MdeModulePkg\Include\Guid\StatusCodeDataTypeDebug.h
//
#ifndef EFI_STATUS_CODE_DATA_MAX_SIZE
#define EFI_STATUS_CODE_DATA_MAX_SIZE 200
#endif

GLOBAL_REMOVE_IF_UNREFERENCED EFI_STATUS_CODE_PROTOCOL *mStatusCodeRuntimeProtocol=NULL;

/**
  Prints a debug message to the debug output device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then print the message specified by Format and the
  associated variable argument list to the debug output device.

  If Format is NULL, then ASSERT().

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      Format string for the debug message to print.
  @param  ...         Variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
VOID
EFIAPI
DebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CONST CHAR8  *Format,
  ...
  )
{
  VA_LIST  Marker;

  VA_START (Marker, Format);
  DebugVPrint (ErrorLevel, Format, Marker);
  VA_END (Marker);
}

/**
  Prints a debug message to the debug output device if the specified
  error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then print the message specified by Format and
  the associated variable argument list to the debug output device.

  If Format is NULL, then ASSERT().

  @param  ErrorLevel    The error level of the debug message.
  @param  Format        Format string for the debug message to print.
  @param  VaListMarker  VA_LIST marker for the variable argument list.

**/
VOID
EFIAPI
DebugVPrint (
  IN  UINTN         ErrorLevel,
  IN  CONST CHAR8   *Format,
  IN  VA_LIST       VaListMarker
  )
{
  EFI_STATUS                   Status;
  UINT64                       Buffer[(EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof(UINT64)) + 1];
  EFI_STATUS_CODE_STRING_DATA  *StringData;
  CHAR8                        *FormatString;
  UINTN                        CharCount;
  UINTN                        StringDataMaxSize;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  if (mStatusCodeRuntimeProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiStatusCodeRuntimeProtocolGuid, NULL, (VOID **) &mStatusCodeRuntimeProtocol);
    if (EFI_ERROR (Status)) {
      return;
    }
  }

  //
  // Use StatusCode
  //
  StringData        = (EFI_STATUS_CODE_STRING_DATA *)Buffer;
  FormatString      = (CHAR8 *)(StringData+1);
  StringDataMaxSize = sizeof(Buffer) - sizeof(EFI_STATUS_CODE_STRING_DATA);

  //
  // Fill in EFI_STATUS_CODE_STRING_DATA
  //
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.HeaderSize    = (UINT16) sizeof(EFI_STATUS_CODE_STRING_DATA);
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size          = (UINT16) StringDataMaxSize;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Type          = gEfiStatusCodeDataTypeStringGuid;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->StringType               = EfiStringAscii;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->String.Ascii             = FormatString;
  // Transfer debug message to Ascii format
  CharCount = AsciiVSPrint (FormatString, StringDataMaxSize, Format, VaListMarker);
  if (CharCount < StringDataMaxSize) {
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size = (UINT16) CharCount;
  }
  //
  // Send the StringData record
  //
  Status = mStatusCodeRuntimeProtocol->ReportStatusCode (
                                         EFI_DEBUG_CODE,
                                         EFI_SOFTWARE_DXE_BS_DRIVER,
                                         (UINT32) ErrorLevel,
                                         (EFI_GUID *) &gEfiStatusCodeDataTypeStringGuid,
                                         (EFI_STATUS_CODE_DATA *) StringData
                                         );
}

/**
  Prints an assert message containing a filename, line number, and description.
  This may be followed by a breakpoint or a dead loop.

  Print a message of the form "ASSERT <FileName>(<LineNumber>): <Description>\n"
  to the debug output device.  If DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED bit of
  PcdDebugProperyMask is set then CpuBreakpoint() is called. Otherwise, if
  DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED bit of PcdDebugProperyMask is set then
  CpuDeadLoop() is called.  If neither of these bits are set, then this function
  returns immediately after the message is printed to the debug output device.
  DebugAssert() must actively prevent recursion.  If DebugAssert() is called while
  processing another DebugAssert(), then DebugAssert() must return immediately.

  If FileName is NULL, then a <FileName> string of "(NULL) Filename" is printed.
  If Description is NULL, then a <Description> string of "(NULL) Description" is printed.

  @param  FileName     Pointer to the name of the source file that generated the assert condition.
  @param  LineNumber   The line number in the source file that generated the assert condition
  @param  Description  Pointer to the description of the assert condition.

**/
VOID
EFIAPI
DebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description
  )
{
  EFI_STATUS                   Status;
  UINT64                       Buffer[(EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof(UINT64)) + 1];
  EFI_STATUS_CODE_STRING_DATA  *StringData;
  CHAR8                        *FormatString;
  UINTN                        CharCount;
  UINTN                        StringDataMaxSize;



  if (mStatusCodeRuntimeProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiStatusCodeRuntimeProtocolGuid, NULL, (VOID **) &mStatusCodeRuntimeProtocol);
    if (EFI_ERROR (Status)) {
      return;
    }
  }

  //
  // Use StatusCode
  //
  StringData        = (EFI_STATUS_CODE_STRING_DATA *)Buffer;
  FormatString      = (CHAR8 *)(StringData+1);
  StringDataMaxSize = sizeof(Buffer) - sizeof(EFI_STATUS_CODE_STRING_DATA);

  //
  // Fill in EFI_STATUS_CODE_STRING_DATA
  //
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.HeaderSize    = (UINT16) sizeof(EFI_STATUS_CODE_STRING_DATA);
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size          = (UINT16) StringDataMaxSize;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Type          = gEfiStatusCodeDataTypeStringGuid;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->StringType               = EfiStringAscii;
  ((EFI_STATUS_CODE_STRING_DATA *) StringData)->String.Ascii             = FormatString;
  //
  // Generate the ASSERT() message in Ascii format
  //
  CharCount = AsciiSPrint (FormatString, StringDataMaxSize, "DXE_ASSERT [%a] %a(%d): %a\n", gEfiCallerBaseName, FileName, LineNumber, Description);
  if (CharCount < StringDataMaxSize) {
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size = (UINT16) CharCount;
  }
  //
  // Send the StringData record
  //
  Status = mStatusCodeRuntimeProtocol->ReportStatusCode (
                                         EFI_ERROR_CODE,
                                         EFI_SOFTWARE_DXE_BS_DRIVER,
                                         DEBUG_ERROR,
                                         (EFI_GUID *) &gEfiStatusCodeDataTypeStringGuid,
                                         (EFI_STATUS_CODE_DATA *) StringData
                                         );

  //
  // Generate a Breakpoint, DeadLoop, or NOP based on PCD settings
  //
  if ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED) != 0) {
    CpuBreakpoint ();
  } else if ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED) != 0) {
    CpuDeadLoop ();
  }
}


/**
  Fills a target buffer with PcdDebugClearMemoryValue, and returns the target buffer.

  This function fills Length bytes of Buffer with the value specified by
  PcdDebugClearMemoryValue, and returns Buffer.

  If Buffer is NULL, then ASSERT() or return NULL.
  If Length is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param   Buffer  Pointer to the target buffer to be filled with PcdDebugClearMemoryValue.
  @param   Length  Number of bytes in Buffer to fill with zeros PcdDebugClearMemoryValue.

  @return  Buffer  Pointer to the target buffer filled with PcdDebugClearMemoryValue.
  @return  NULL    Return NULL if Buffer pointer is NULL

**/
VOID *
EFIAPI
DebugClearMemory (
  OUT VOID  *Buffer,
  IN UINTN  Length
  )
{
  if (Buffer == NULL) {
    ASSERT (FALSE);
    return (Buffer);
  } else {
    return SetMem (Buffer, Length, PcdGet8 (PcdDebugClearMemoryValue));
  }
}


/**
  Returns TRUE if ASSERT() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugAssertEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED) != 0);
}


/**
  Returns TRUE if DEBUG() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_PRINT_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugPrintEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_PRINT_ENABLED) != 0);
}


/**
  Returns TRUE if DEBUG_CODE() macros are enabled.

  This function returns TRUE if the DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugCodeEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_DEBUG_CODE_ENABLED) != 0);
}


/**
  Returns TRUE if DEBUG_CLEAR_MEMORY() macro is enabled.

  This function returns TRUE if the DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of
  PcdDebugProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE    The DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of PcdDebugProperyMask is set.
  @retval  FALSE   The DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED bit of PcdDebugProperyMask is clear.

**/
BOOLEAN
EFIAPI
DebugClearMemoryEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8 (PcdDebugPropertyMask) & DEBUG_PROPERTY_CLEAR_MEMORY_ENABLED) != 0);
}

/**
  Returns TRUE if any one of the bit is set both in ErrorLevel and PcdFixedDebugPrintErrorLevel.

  This function compares the bit mask of ErrorLevel and PcdFixedDebugPrintErrorLevel.

  @retval  TRUE    Current ErrorLevel is supported.
  @retval  FALSE   Current ErrorLevel is not supported.

**/
BOOLEAN
EFIAPI
DebugPrintLevelEnabled (
  IN  CONST UINTN        ErrorLevel
  )
{
  return (BOOLEAN) ((ErrorLevel & PcdGet32(PcdFixedDebugPrintErrorLevel)) != 0);
}
