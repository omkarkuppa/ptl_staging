/** @file
  TraceHub status code reporting worker.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PcdLib.h>

#include <Library/TraceHubDebugLib.h>

#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>

#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugPrintErrorLevelLib.h>

/**
  Convert status code value and extended data to readable ASCII string, send string to TraceHub device.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to TraceHub successfully.

**/
EFI_STATUS
EFIAPI
TraceHubStatusCodeReportWorker (
  IN CONST  EFI_PEI_SERVICES      **PeiServices,
  IN EFI_STATUS_CODE_TYPE         CodeType,
  IN EFI_STATUS_CODE_VALUE        Value,
  IN UINT32                       Instance,
  IN CONST EFI_GUID               *CallerId,
  IN CONST EFI_STATUS_CODE_DATA   *Data OPTIONAL
  )
{
  CHAR8                     *Filename;
  CHAR8                     *Description;
  CHAR8                     *Format;
  CHAR8                     Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE];
  UINT32                    ErrorLevel;
  UINT32                    LineNumber;
  UINTN                     CharCount;
  BASE_LIST                 Marker;
  UINT32                    DebugPrintErrorLevel;
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;

  Buffer[0] = '\0';

  DebugPrintErrorLevel = GetDebugPrintErrorLevel ();

  if (Data != NULL &&
      CompareGuid (&Data->Type, &gEfiStatusCodeDataTypeStringGuid) &&
      ((EFI_STATUS_CODE_STRING_DATA *) Data)->StringType == EfiStringAscii) {
    if ((Instance & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print String Data.
    //
    TraceHubSeverityLevel = SeverityNormal;
    TraceHubDebugWrite (
      TraceHubSeverityLevel,
      (UINT8 *) (((EFI_STATUS_CODE_STRING_DATA *) Data)->String.Ascii),
      ((EFI_STATUS_CODE_STRING_DATA *) Data)->DataHeader.Size
      );
    return EFI_SUCCESS;
  } else if (Data != NULL &&
             ReportStatusCodeExtractAssertInfo (CodeType, Value, Data, &Filename, &Description, &LineNumber)) {
    if ((DEBUG_ERROR & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
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
    if ((ErrorLevel & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
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
    if ((DEBUG_ERROR & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print ERROR information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "ERROR: C%08x:V%08x I%x",
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
    if ((DEBUG_INFO & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
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

#if FixedPcdGetBool (PcdTraceHubCatalogEnable) == 1
  EFI_GUID   *DrvGui = NULL;
  if (((Value & EFI_STATUS_CODE_CLASS_MASK)|(Value & EFI_STATUS_CODE_SUBCLASS_MASK)) == EFI_SOFTWARE_PEI_CORE) {
   if (((Value & EFI_STATUS_CODE_OPERATION_MASK) == EFI_SW_PC_INIT_BEGIN) || ((Value & EFI_STATUS_CODE_OPERATION_MASK) == EFI_SW_PC_INIT_END)){
    if (Data != NULL) {
      CopyMem (&DrvGui,Data+1,sizeof(EFI_GUID*));
      if (DrvGui != NULL)
        TraceHubWriteCataLog64StatusCode (TraceHubSeverityLevel, Value,DrvGui);
     }
    }
  } else {
    TraceHubWriteCataLog64_0 (TraceHubSeverityLevel, Value);
  }
  return EFI_SUCCESS;
#endif
  } else {
    if ((DEBUG_INFO & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Code type is not defined.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "Undefined: C%08x:V%08x I%x\n\r",
                  CodeType,
                  Value,
                  Instance
                  );

    TraceHubSeverityLevel = SeverityNone;
  }

  //
  // Call TraceHub Lib function to do print.
  //
  TraceHubDebugWrite (TraceHubSeverityLevel, (UINT8 *) Buffer, CharCount);

  return EFI_SUCCESS;
}

/**
  Constructor function of PeiPostCodeStatusCodeHandlerLib.

  This function is the constructor function of this PostCode Status Code Handler Library for PEI Phase.
  It check whether need beep, and register it to gEfiPeiRscHandlerPpiGuid.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCESS  The entry point of DXE IPL PEIM executes successfully.

**/
RETURN_STATUS
EFIAPI
PeiTraceHubStatusCodeHandlerLibConstructor (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;

  if (!PcdGetBool (PcdStatusCodeUseTraceHub)) {
    return RETURN_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = RscHandlerPpi->Register (TraceHubStatusCodeReportWorker);
  ASSERT_EFI_ERROR (Status);

  return RETURN_SUCCESS;
}
