/**@file

  Module Name:

    MonoStatusCode.c

  Abstract:

    Contains Platform specific implementations required to use status codes.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "MonoStatusCode.h"
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TraceHubDebugLib.h>

//
// Module globals
//
EFI_PEI_PROGRESS_CODE_PPI     mStatusCodePpi = { PlatformReportStatusCode };

EFI_PEI_PPI_DESCRIPTOR  mPpiListStatusCode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiStatusCodePpiGuid,
  &mStatusCodePpi
};

EFI_STATUS
EFIAPI
SerialReportStatusCode (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_STATUS_CODE_TYPE        CodeType,
  IN EFI_STATUS_CODE_VALUE       Value,
  IN UINT32                      Instance,
  IN CONST EFI_GUID              * CallerId,
  IN CONST EFI_STATUS_CODE_DATA  * Data OPTIONAL
  )
/*++

Routine Description:

  Provide a serial status code

Arguments:

  Same as ReportStatusCode PPI

Returns:

  EFI_SUCCESS   Always returns success.

--*/
{
  CHAR8           *Filename;
  CHAR8           *Description;
  CHAR8           *Format;
  CHAR8           Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE];
  UINT32          ErrorLevel;
  UINT32          LineNumber;
  UINTN           CharCount;
  BASE_LIST       Marker;
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;

  Buffer[0] = '\0';

  if (Data != NULL &&
      ReportStatusCodeExtractAssertInfo (CodeType, Value, Data, &Filename, &Description, &LineNumber)) {
    //
    // Print ASSERT() information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "\n\rPEI_ASSERT!: %a (%d): %a\n\r",
                  Filename,
                  LineNumber,
                  Description
                  );
    TraceHubSeverityLevel = SeverityFatal;
  } else if (Data != NULL &&
             ReportStatusCodeExtractDebugInfo (Data, &ErrorLevel, &Marker, &Format)) {
    //
    // Print DEBUG() information into output buffer.
    //
    CharCount = AsciiBSPrint (
                  Buffer,
                  sizeof (Buffer),
                  Format,
                  Marker
                  );
    if (ErrorLevel == DEBUG_WARN) {
      TraceHubSeverityLevel = SeverityNormal;
    } else if (ErrorLevel == DEBUG_ERROR) {
      TraceHubSeverityLevel = SeverityError;
    } else {
      TraceHubSeverityLevel = SeverityNormal;
    }
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    //
    // Print ERROR information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "ERROR: C%x:V%x I%x",
                  CodeType,
                  Value,
                  Instance
                  );

    ASSERT(CharCount > 0);

    if (CallerId != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %g",
                     CallerId
                     );
    }

    if (Data != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %x",
                     Data
                     );
    }

    CharCount += AsciiSPrint (
                   &Buffer[CharCount],
                   (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                   "\n\r"
                   );
    TraceHubSeverityLevel = SeverityError;
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
    //
    // Print PROGRESS information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "PROGRESS CODE: V%08x I%x\n\r",
                  Value,
                  Instance
                  );
    TraceHubSeverityLevel = SeverityNormal;
  } else if (Data != NULL &&
             CompareGuid (&Data->Type, &gEfiStatusCodeDataTypeStringGuid) &&
             ((EFI_STATUS_CODE_STRING_DATA *) Data)->StringType == EfiStringAscii) {
    //
    // EFI_STATUS_CODE_STRING_DATA
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "%a\n\r",
                  ((EFI_STATUS_CODE_STRING_DATA *) Data)->String.Ascii
                  );
    TraceHubSeverityLevel = SeverityNormal;
  } else {
    //
    // Code type is not defined.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "Undefined: C%x:V%x I%x\n\r",
                  CodeType,
                  Value,
                  Instance
                  );
    TraceHubSeverityLevel = SeverityNone;
  }

  //
  // Call SerialPort Lib function to do print.
  //
  SerialPortWrite ((UINT8 *) Buffer, CharCount);

  //
  // Call TraceHub Lib function to do print.
  //
  TraceHubDebugWrite (TraceHubSeverityLevel, (UINT8 *) Buffer, CharCount);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PlatformReportStatusCode (
  IN  CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_STATUS_CODE_TYPE        CodeType,
  IN EFI_STATUS_CODE_VALUE       Value,
  IN UINT32                      Instance,
  IN CONST EFI_GUID              *CallerId,
  IN CONST EFI_STATUS_CODE_DATA  *Data OPTIONAL
  )
/*++

Routine Description:

  Call all status code listeners in the MonoStatusCode.

Arguments:

  PeiServices  -  The PEI core services table.
  CodeType     -  Type of Status Code.
  Value        -  Value to output for Status Code.
  Instance     -  Instance Number of this status code.
  CallerId     -  ID of the caller of this status code.
  Data         -  Optional data associated with this status code.

Returns:

  EFI_SUCCESS            -  If status code is successfully reported.
  EFI_NOT_AVAILABLE_YET  -  If StatusCodePpi has not been installed.

--*/
{
  //
  // If we are in debug mode, we will allow serial status codes
  //
  SerialReportStatusCode (PeiServices, CodeType, Value, Instance, CallerId, Data);

  return EFI_SUCCESS;
}

VOID
EFIAPI
InitializeMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++

Routine Description:

  Initialize the platform status codes and publish the platform status code
  PPI.

Arguments:

  FfsHeader   - FV this PEIM was loaded from.
  PeiServices - General purpose services available to every PEIM.

Returns:

  Status -  EFI_SUCCESS

--*/
{
  EFI_STATUS  Status;

  //
  // Publish the status code capability to other modules
  //
  Status = (*PeiServices)->InstallPpi (PeiServices, &mPpiListStatusCode);

  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\nMono Status Code PEIM Loaded\n"));

  return ;
}

EFI_STATUS
EFIAPI
InstallMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++

Routine Description:

  Install the PEIM.  Initialize listeners, publish the PPI and HOB for PEI and
  DXE use respectively.

Arguments:

  FfsHeader    -  FV this PEIM was loaded from.
  PeiServices  -  General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS  -  The function always returns success.

--*/
{
  //
  // Initialize all listeners
  //
  InitializeMonoStatusCode (FfsHeader, PeiServices);

  return EFI_SUCCESS;
}
