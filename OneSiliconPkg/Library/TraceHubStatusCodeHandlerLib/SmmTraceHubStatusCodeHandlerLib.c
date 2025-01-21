/** @file
  TraceHub status code reporting worker.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <Protocol/SmmReportStatusCodeHandler.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Core/Dxe/DxeMain.h>
#include "TraceHubStatusWorkerCommon.h"
#include <Library/TraceHubDebugLib.h>

#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>

#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugPrintErrorLevelLib.h>

/**
  Convert status code value and extended data to readable ASCII string, send string to TraceHub device.

  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or software entity.
                           This included information about the class and subclass that is used to
                           classify the entity as well as an operation.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to TraceHub successfully.
  @retval EFI_DEVICE_ERROR EFI TraceHub device cannot work after ExitBootService() is called.
  @retval EFI_DEVICE_ERROR EFI TraceHub device cannot work with TPL higher than TPL_CALLBACK.

**/
EFI_STATUS
EFIAPI
TraceHubStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
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
                  "\n\rSMM_ASSERT!: %a (%d): %a\n\r",
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
    ASSERT (CharCount > 0);

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
  EFI_HANDLE  Imagehandle=NULL;
  LOADED_IMAGE_PRIVATE_DATA  *Imagepriv=NULL;
  EFI_GUID   DrvGui = {0};
  if (((Value & EFI_STATUS_CODE_CLASS_MASK)|(Value & EFI_STATUS_CODE_SUBCLASS_MASK)) == EFI_SOFTWARE_SMM_DRIVER) {
    if (((Value & EFI_STATUS_CODE_OPERATION_MASK) == EFI_SW_PC_INIT_BEGIN) || ((Value & EFI_STATUS_CODE_OPERATION_MASK) == EFI_SW_PC_INIT_END)) {
      if (Data != NULL) {
        CopyMem (&Imagehandle,Data+1,4);
        if (Imagehandle != NULL) {
          Imagepriv = GetLoadedImageInfo (Imagehandle);
          if (Imagepriv && Imagepriv->Info.FilePath != NULL) {
            if (DevicePathType (Imagepriv->Info.FilePath) == MEDIA_DEVICE_PATH && DevicePathSubType (Imagepriv->Info.FilePath) == MEDIA_PIWG_FW_FILE_DP) {
              DrvGui = ((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *)Imagepriv->Info.FilePath)->FvFileName ;
              TraceHubWriteCataLog64StatusCode(TraceHubSeverityLevel, Value,&DrvGui);
            }
          }
        }
      }
    }
  } else {
    TraceHubWriteCataLog64_0 (TraceHubSeverityLevel, Value);
  }
  return EFI_SUCCESS;
#endif
  } else if (Data != NULL &&
             CompareGuid (&Data->Type, &gEfiStatusCodeDataTypeStringGuid) &&
             ((EFI_STATUS_CODE_STRING_DATA *) Data)->StringType == EfiStringAscii) {
    if ((DEBUG_INFO & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
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
  Register status code callback function only when Report Status Code protocol
  is installed.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
EFI_STATUS
EFIAPI
RegisterTraceHubBootTimeHandlers (
  IN CONST EFI_GUID        *Protocol,
  IN VOID                  *Interface,
  IN EFI_HANDLE            Handle
)
{
  EFI_STATUS                    Status;
  EFI_SMM_RSC_HANDLER_PROTOCOL  *RscHandlerProtocol;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &RscHandlerProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  RscHandlerProtocol->Register (TraceHubStatusCodeReportWorker);

  return EFI_SUCCESS;
}

/**
  Constructor function of SmmTraceHubStatusCodeHandlerLib.

  This function allocates memory for extended status code data, caches
  the report status code service, and registers events.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmTraceHubStatusCodeHandlerLibConstructor (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                    Status;
  VOID                          *Registration;
  EFI_SMM_RSC_HANDLER_PROTOCOL  *RscHandlerProtocol;

  if (!PcdGetBool (PcdStatusCodeUseTraceHub)) {
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &RscHandlerProtocol
                    );
  if (!EFI_ERROR (Status)) {
    RegisterTraceHubBootTimeHandlers (NULL, NULL, NULL);
  } else {
    gSmst->SmmRegisterProtocolNotify (
             &gEfiSmmRscHandlerProtocolGuid,
             RegisterTraceHubBootTimeHandlers,
             &Registration
             );
  }

  return EFI_SUCCESS;
}
