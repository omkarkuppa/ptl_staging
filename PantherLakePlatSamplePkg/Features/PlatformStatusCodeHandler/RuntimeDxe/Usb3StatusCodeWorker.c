/** @file
  Usb3 status code reporting worker.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include "PlatformStatusCodeHandlerRuntimeDxe.h"

#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>

#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugPrintErrorLevelLib.h>

/**
  Convert status code value and extended data to readable ASCII string, send string to Usb3 device.

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

  @retval EFI_SUCCESS      Status code reported to Usb3 successfully.
  @retval EFI_DEVICE_ERROR Usb3 device cannot work after ExitBootService() is called.
  @retval EFI_DEVICE_ERROR Usb3 device cannot work with TPL higher than TPL_CALLBACK.

**/
EFI_STATUS
EFIAPI
Usb3StatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  CHAR8           *Filename;
  CHAR8           *Description;
  CHAR8           *Format;
  CHAR8           Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE];
  UINT32          ErrorLevel;
  UINT32          LineNumber;
  UINTN           CharCount;
  BASE_LIST       Marker;
  UINT32          DebugPrintErrorLevel;

  if (!(GetDebugInterface () & STATUS_CODE_USE_USB3)) {
    return RETURN_ABORTED;
  }

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
    Usb3DebugPortWrite (
      (UINT8 *) (((EFI_STATUS_CODE_STRING_DATA *) Data)->String.Ascii),
      ((EFI_STATUS_CODE_STRING_DATA *) Data)->DataHeader.Size
      );
    return EFI_SUCCESS;
  } else if (Data != NULL &&
      ReportStatusCodeExtractAssertInfo (CodeType, Value, Data, &Filename, &Description, &LineNumber)) {
    if ((EFI_D_ERROR & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print ASSERT() information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "\n\rDXE_ASSERT!: %a (%d): %a\n\r",
                  Filename,
                  LineNumber,
                  Description
                  );
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
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    if ((EFI_D_ERROR & DebugPrintErrorLevel) == 0) {
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
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
    if ((EFI_D_INFO & DebugPrintErrorLevel) == 0) {
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
  } else {
    if ((EFI_D_INFO & DebugPrintErrorLevel) == 0) {
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
  }

  //
  // Call Usb3DebugPortLib function to do print.
  //
  Usb3DebugPortWrite ((UINT8 *) Buffer, CharCount);

  //
  // If register an unregister function of gEfiEventExitBootServicesGuid,
  // then some log called in ExitBootServices() will be lost,
  // so unregister the handler after receive the value of exit boot service.
  //
  if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE &&
      Value == (EFI_SOFTWARE_EFI_BOOT_SERVICE | EFI_SW_BS_PC_EXIT_BOOT_SERVICES)) {
    UnregisterUsb3BootTimeHandler();
  }

  return EFI_SUCCESS;
}

