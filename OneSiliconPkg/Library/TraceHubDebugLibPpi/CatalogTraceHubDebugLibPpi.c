/** @file
  Debug Library Instance based on Catalog Debug Service.

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

#include <PiPei.h>
#include <CatalogTHDebug.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/BaseLib.h>
#include <Library/TraceHubDebugLib.h>
#define MAX_CATALOG_PARAMETER     0x6

VOID
EFIAPI
DebugPrint (
  IN  UINTN         ErrorLevel,
  IN  CONST CHAR8   *Format,
  ...
  )
{
  EFI_STATUS                Status;
  CATALOG_TH_DEBUG_PPI      *CatalogTHDebugPpi;
  VA_LIST                   Marker;
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;
  UINTN                     Param[MAX_CATALOG_PARAMETER];
  UINTN                     NumberOfParams;
  UINTN                     Id;
  UINTN                     Index;


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

  Status = PeiServicesLocatePpi (
              &gCatalogTHDebugPpiGuid,
              0,
              NULL,
              (VOID **)&CatalogTHDebugPpi
              );
  if (EFI_ERROR (Status)) {
    return;
  }

  CatalogTHDebugPpi->DebugPrint (
              ErrorLevel,
              Format,
              TraceHubSeverityLevel,
              Id,
              Param,
              NumberOfParams
              );
}

VOID
EFIAPI
DebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description
  )
{
  return;
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
  PcdDebugProperyMask is set.  Otherwise, FALSE is returned.

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
  PcdDebugProperyMask is set.  Otherwise, FALSE is returned.

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
  PcdDebugProperyMask is set.  Otherwise, FALSE is returned.

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
  PcdDebugProperyMask is set.  Otherwise, FALSE is returned.

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

