/** @file
  Base Debug library instance base on TraceHub Catalog library.
  It uses TraceHubDebugLib to send catalog messages to TraceHub.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/TraceHubDebugLib.h>

//
// Define the maximum number of catalog message partameters that this library supports
//
#define MAX_CATALOG_PARAMETER     0x6

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
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;
  VA_LIST                   Marker;
  UINTN                     Param[MAX_CATALOG_PARAMETER];
  UINTN                     NumberOfParams;
  UINTN                     Id;
  UINTN                     Index;

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & GetDebugPrintErrorLevel ()) == 0) {
    return;
  }

  //
  // EDK ErrorLevel to TraceHub Severity Mapping
  //
  TraceHubSeverityLevel = SeverityNormal;
  if (ErrorLevel & EFI_D_ERROR) {
    TraceHubSeverityLevel = SeverityError;
  } else if (ErrorLevel & EFI_D_WARN) {
    TraceHubSeverityLevel = SeverityWarning;
  }

  VA_START (Marker, Format);

  //
  // Returns if it is not a Catalog Debug Message. Currenlty the Catalog Message has the Format as NULL.
  //
  if (Format != NULL) {
    VA_END (Marker);
    return;
  }

  NumberOfParams = VA_ARG (Marker, UINTN);
  Id = VA_ARG (Marker, UINTN);

  //
  // Since MAX_CATALOG_PARAMETER is 0x6, set mask as 0x7 to avoid unsupported Params
  //
  NumberOfParams &= 0x7;
  if (NumberOfParams > MAX_CATALOG_PARAMETER) {
    NumberOfParams = MAX_CATALOG_PARAMETER;
  }
  for (Index = 0; Index < NumberOfParams; Index++) {
    Param[Index] = VA_ARG (Marker, UINTN);
  }
  VA_END (Marker);

  //
  // Send the print string to Trace Hub
  //
  switch (NumberOfParams) {
  case 0:
    TraceHubWriteCataLog64_0 (TraceHubSeverityLevel, Id);
    break;
  case 1:
    TraceHubWriteCataLog64_1 (TraceHubSeverityLevel, Id, (UINT32)Param[0]);
    break;
  case 2:
    TraceHubWriteCataLog64_2 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1]);
    break;
  case 3:
    TraceHubWriteCataLog64_3 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2]);
    break;
  case 4:
    TraceHubWriteCataLog64_4 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3]);
    break;
  case 5:
    TraceHubWriteCataLog64_5 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3], (UINT32)Param[4]);
    break;
  case 6:
    TraceHubWriteCataLog64_6 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3], (UINT32)Param[4], (UINT32)Param[5]);
    break;
  default:
    //
    // There is an unsupported debug message with more than 6 parameter
    //
    break;
  }
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
  IN  UINTN        ErrorLevel,
  IN  CONST CHAR8  *Format,
  IN  VA_LIST      VaListMarker
  )
{
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

  @param  FileName     The pointer to the name of the source file that generated the assert condition.
  @param  LineNumber   The line number in the source file that generated the assert condition
  @param  Description  The pointer to the description of the assert condition.

**/
VOID
EFIAPI
DebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description
  )
{
}


/**
  Fills a target buffer with PcdDebugClearMemoryValue, and returns the target buffer.

  This function fills Length bytes of Buffer with the value specified by
  PcdDebugClearMemoryValue, and returns Buffer.

  If Buffer is NULL, then ASSERT().
  If Length is greater than (MAX_ADDRESS - Buffer + 1), then ASSERT().

  @param   Buffer  The pointer to the target buffer to be filled with PcdDebugClearMemoryValue.
  @param   Length  The number of bytes in Buffer to fill with zeros PcdDebugClearMemoryValue.

  @return  Buffer  The pointer to the target buffer filled with PcdDebugClearMemoryValue.

**/
VOID *
EFIAPI
DebugClearMemory (
  OUT VOID  *Buffer,
  IN UINTN  Length
  )
{
  return Buffer;
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
  return FALSE;
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
  return TRUE;
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
  return FALSE;
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
  return FALSE;
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
  return (BOOLEAN) ((ErrorLevel & FixedPcdGet32(PcdFixedDebugPrintErrorLevel)) != 0);
}

