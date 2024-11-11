/** @file
  Instance of Debug Library.

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

@par Specification Reference:
**/

#include <PiPei.h>
#include <FspmUpd.h>
#include <FspsUpd.h>
#include <FsptUpd.h>
#include <Ppi/FspDebugService.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/FspCommonLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/FspSerialIoUartDebugHelperLib.h>
#include <Library/PeiServicesLib.h>
#include <Protocol/SerialIo.h>

//---------------------------------------------
// Serial Debug Interfaces
//---------------------------------------------
#define STATUS_CODE_USE_ISA_SERIAL BIT1
#define STATUS_CODE_USE_SERIALIO   BIT4
#define STATUS_CODE_USE_TRACEHUB   BIT5

#pragma pack(1)
typedef struct {
  FSP_EVENT_HANDLER             *FspEventHandler;
  EFI_STATUS_CODE_TYPE          Type;
  EFI_STATUS_CODE_VALUE         Value;
  UINT32                        Instance;
  EFI_GUID                      *CallerId;
  EFI_STATUS_CODE_STRING_DATA   *Data;
} EVENT_HANDLER_PARAMETER;

typedef struct {
  FSP_DEBUG_HANDLER         *FspDebugHandler;
  CHAR8                     *Buffer;
  UINT32                    BufferSize;
} DEBUG_HANDLER_PARAMETER;
#pragma pack()

UINT8
EFIAPI
GetDebugInterfaceFlags (
  VOID
  );

VOID
EFIAPI
SwitchStackCallDebugEventHandler (
  VOID
  );

VOID
EFIAPI
SwitchIdtCallDebugEventHandler (
  VOID
  );

VOID
EFIAPI
SwitchStackCallDebugHandler (
  VOID
  );

VOID
EFIAPI
SwitchIdtCallDebugHandler (
  VOID
  );

  /**
  The constructor function initialize the Debug Port Library
  @retval EFI_SUCCESS   The constructor always returns RETURN_SUCCESS.
**/
RETURN_STATUS
EFIAPI
DebugLibDebugPortConstructor (
  VOID
  )
{
  FSPM_UPD        *FspmUpd;
  UINT8           StatusCodeFlags;
  FSP_GLOBAL_DATA *FspGlobalData;

  //
  // Check the initialization of FSP Global Data Pointer before using it
  // Constructor will call this first time before FSP Global Data pointer is initialized
  // skip initialization that time. It will be reinitialized by FSP, once FSP Gloabl Data pointer
  // is ready
  //
  FspGlobalData = GetFspGlobalDataPointer ();
  if ((UINTN) FspGlobalData == 0x00 || (UINTN) FspGlobalData == 0xFFFFFFFF) {
    return RETURN_SUCCESS;
  } else if (FspGlobalData->Signature != FSP_GLOBAL_DATA_SIGNATURE) {
    return RETURN_SUCCESS;
  }

  FspmUpd = FspGlobalData->MemoryInitUpdPtr;
  if (FspmUpd == NULL) {
    return RETURN_SUCCESS;
  }

  StatusCodeFlags = FspmUpd->FspmConfig.PcdDebugInterfaceFlags;

  if (StatusCodeFlags & STATUS_CODE_USE_ISA_SERIAL) {
    SerialPortInitialize ();
  }

  return RETURN_SUCCESS;
}

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
  Function print string buffer to Serial Io Uart Port

  @param[in] Buffer              Pointer to string buffer
  @param[in] NumberOfBytes       Size of string buffer

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_NOT_FOUND          If EFI Services not ready or ppi is not installed
**/
EFI_STATUS
WriteToSerialIoPpi (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  EFI_STATUS                Status;
  EFI_SERIAL_IO_PROTOCOL    *SerialIoAccess;

  Status = EFI_NOT_FOUND;
  if (PcdGetBool (PcdFspValidatePeiServiceTablePointer)) {
    Status = PeiServicesLocatePpi (
               &gEfiSerialIoProtocolGuid,
               0,
               NULL,
               (VOID **) &SerialIoAccess
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return Status;
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
  CHAR8                        Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  TRACE_HUB_SEVERITY_TYPE      TraceHubSeverityLevel;
  CONST EFI_PEI_SERVICES       **PeiServices;
  IA32_DESCRIPTOR              Idtr;
  EFI_STATUS                   Status;
  DEBUG_SERVICE_PPI            *DebugServiceData;
  UINT64                       Buffer1[(EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof(UINT64)) + 1];
  EFI_STATUS_CODE_STRING_DATA  *StringData;
  CHAR8                        *FormatString;
  UINTN                        CharCount;
  UINTN                        StringDataMaxSize;
  FSP_GLOBAL_DATA              *FspGlobalData;
  FSPT_UPD                     *FsptUpd;
  FSPS_UPD                     *FspsUpd;
  FSPM_UPD                     *FspmUpd;
  EVENT_HANDLER_PARAMETER      FspEventHandlerParameter;
  DEBUG_HANDLER_PARAMETER      FspDebugHandlerParameter;
  BOOLEAN                      FspEventHandlerExecuted;
  BOOLEAN                      FspDebugHandlerExecuted;
  VOID                         *FsptUpdLocationPpi;

  PeiServices = NULL;
  FspGlobalData = NULL;
  FspEventHandlerParameter.FspEventHandler = NULL;
  FspDebugHandlerParameter.FspDebugHandler = NULL;
  FsptUpd = NULL;
  FspsUpd = NULL;
  FspmUpd = NULL;
  FspEventHandlerExecuted = FALSE;
  FspDebugHandlerExecuted = FALSE;
  FsptUpdLocationPpi = NULL;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & GetDebugPrintErrorLevel ()) == 0) {
    return;
  }

  if (PcdGetBool (PcdFspValidatePeiServiceTablePointer)) {  // New PCD to control do PeiServiceTablepointer check or not. For SEC, this PCD is configured as FALSE. Its default value is TRUE
    //
    // FSP-M / FSP-S
    //
    AsmReadIdtr (&Idtr);
    PeiServices = (CONST EFI_PEI_SERVICES **) (*(UINTN*)(Idtr.Base - sizeof (UINTN)));
    Status =  (*PeiServices)->LocatePpi (
                                  PeiServices,
                                  &gFspDebugServicePpiGuid,
                                  0,
                                  NULL,
                                  (VOID **) &DebugServiceData
                                  );
    if (!EFI_ERROR(Status)) {
      DebugServiceData->DebugPrint (ErrorLevel, Format, VaListMarker);
      return;
    }
    //
    // Locate gFsptUpdLocationPpiGuid for FSPTUPD
    //
    Status = (*PeiServices)->LocatePpi (
                          PeiServices,
                          &gFsptUpdLocationPpiGuid,
                          0,
                          NULL,
                          (VOID **) &FsptUpdLocationPpi
                          );
    if (EFI_ERROR(Status)) {
      FsptUpdLocationPpi = NULL;
    }
  }

  FspGlobalData = GetFspGlobalDataPointer ();
  if (!(((UINTN)FspGlobalData == 0x00 || (UINTN)FspGlobalData == 0xFFFFFFFF) || (FspGlobalData->Signature != FSP_GLOBAL_DATA_SIGNATURE))) {

    //
    // Use StatusCode
    //
    StringData        = (EFI_STATUS_CODE_STRING_DATA *)Buffer1;
    FormatString      = (CHAR8 *)(StringData+1);
    StringDataMaxSize = sizeof(Buffer1) - sizeof(EFI_STATUS_CODE_STRING_DATA);
    //
    // Fill in EFI_STATUS_CODE_STRING_DATA
    //
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.HeaderSize    = (UINT16) sizeof(EFI_STATUS_CODE_STRING_DATA);
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size          = (UINT16) StringDataMaxSize;
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->StringType               = EfiStringAscii;
    ((EFI_STATUS_CODE_STRING_DATA *) StringData)->String.Ascii             = FormatString;
    CopyGuid (&StringData->DataHeader.Type, &gEfiStatusCodeDataTypeStringGuid);

    // Transfer debug message to Ascii format
    CharCount = AsciiVSPrint (FormatString, StringDataMaxSize, Format, VaListMarker);
    if (CharCount < StringDataMaxSize) {
      ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size = (UINT16) CharCount;
    }
    if (FspGlobalData->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
      //
      // Find FspEventHandler
      //
      if (FspGlobalData->SiliconInitUpdPtr != NULL) {
        FspsUpd = FspGlobalData->SiliconInitUpdPtr;
        if (FspsUpd->FspUpdHeader.Signature == FSPS_UPD_SIGNATURE) {
          FspEventHandlerParameter.FspEventHandler = (FSP_EVENT_HANDLER *) (UINTN) FspsUpd->FspsArchUpd.FspEventHandler;
        }
      } else if (FspGlobalData->MemoryInitUpdPtr != NULL) {
        FspmUpd = FspGlobalData->MemoryInitUpdPtr;
        FspEventHandlerParameter.FspEventHandler = (FSP_EVENT_HANDLER *) (UINTN) FspmUpd->FspmArchUpd.FspEventHandler;
      }

      if (FspEventHandlerParameter.FspEventHandler != NULL) {
        //
        // FspEventHandler presence
        //
        FspEventHandlerParameter.Instance = (UINT32) ErrorLevel;
        FspEventHandlerParameter.CallerId = &gEfiStatusCodeDataTypeStringGuid;
        FspEventHandlerParameter.Data = StringData;

        FspGlobalData->FunctionParameterPtr = &FspEventHandlerParameter;

        if (FspGlobalData->OnSeparateStack) {
          SwitchStackCallDebugEventHandler();
        } else {
          SwitchIdtCallDebugEventHandler();
        }
        FspEventHandlerExecuted = TRUE;
      }
    }

    if (FspEventHandlerExecuted == FALSE) {
      if (PeiServices != NULL) {
        //
        // Send the StringData record
        //
        Status = (*PeiServices)->ReportStatusCode (
                                    PeiServices,
                                    EFI_DEBUG_CODE,
                                    EFI_SOFTWARE_PEI_MODULE,
                                    (UINT32) ErrorLevel,
                                    (EFI_GUID *) &gEfiStatusCodeDataTypeStringGuid,
                                    (EFI_STATUS_CODE_DATA *) StringData
                                    );
        if (Status == EFI_SUCCESS) {
          return;
        }
      }

      if ((FspGlobalData->TempRamInitUpdPtr == NULL)) {
        FspGlobalData->TempRamInitUpdPtr = FsptUpdLocationPpi;
      }

      if ((FspGlobalData->TempRamInitUpdPtr != NULL)) {
        //
        //  FsptUpd available
        //
        FsptUpd = FspGlobalData->TempRamInitUpdPtr;
        FspDebugHandlerParameter.FspDebugHandler = (FSP_DEBUG_HANDLER *) (UINTN) FsptUpd->FsptArchUpd.FspDebugHandler;
      }
      if (FspDebugHandlerParameter.FspDebugHandler != NULL) {
        //
        // Convert the DEBUG() message to an ASCII String
        //
        AsciiVSPrint (Buffer, sizeof (Buffer), Format, VaListMarker);

        FspDebugHandlerParameter.Buffer = Buffer;
        FspDebugHandlerParameter.BufferSize = (UINT32) AsciiStrLen (Buffer);

        FspGlobalData->FunctionParameterPtr = &FspDebugHandlerParameter;

        if (FspGlobalData->OnSeparateStack) {
          SwitchStackCallDebugHandler();
        } else {
          SwitchIdtCallDebugHandler();
        }
        FspDebugHandlerExecuted = TRUE;
      }
    }
  }

  if ((!FspEventHandlerExecuted) && (!FspDebugHandlerExecuted)) {
    //
    // Convert the DEBUG() message to an ASCII String
    //
    AsciiVSPrint (Buffer, sizeof (Buffer), Format, VaListMarker);
    //
    // Send the print string to Debug Port
    //
    if (GetDebugInterfaceFlags () & STATUS_CODE_USE_ISA_SERIAL) {
      SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
    }

    if (GetDebugInterfaceFlags () & STATUS_CODE_USE_SERIALIO) {
      if(PcdGetBool (PcdFspValidatePeiServiceTablePointer)){
        Status = WriteToSerialIoPpi ((UINT8 *) Buffer, AsciiStrLen (Buffer));
        if (EFI_ERROR (Status)) {
          SerialPortWrite ((UINT8 *) Buffer, AsciiStrLen (Buffer));
        }
      }
    }
  }

  //
  // Send the print string to Trace Hub
  //
  if (GetDebugInterfaceFlags() & STATUS_CODE_USE_TRACEHUB) {
    //
    // EDK ErrorLevel to TraceHub Severity Mapping
    //
    TraceHubSeverityLevel = SeverityNormal;
    if (ErrorLevel & DEBUG_ERROR) {
      TraceHubSeverityLevel = SeverityError;
    } else if (ErrorLevel & DEBUG_WARN) {
      TraceHubSeverityLevel = SeverityWarning;
    }

    TraceHubDebugWrite (TraceHubSeverityLevel, (UINT8 *)Buffer, AsciiStrLen (Buffer));
  }
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
  CHAR8           Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  EFI_STATUS      Status;

  //
  // Generate the ASSERT() message in Ascii format
  //
  AsciiSPrint (Buffer, sizeof (Buffer), "ASSERT [%a] %a(%d): %a\n", gEfiCallerBaseName, FileName, LineNumber, Description);

  //
  // Send the print string to Debug Port
  //
  if (GetDebugInterfaceFlags() & STATUS_CODE_USE_ISA_SERIAL) {
    SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (GetDebugInterfaceFlags() & STATUS_CODE_USE_SERIALIO) {
    if(PcdGetBool (PcdFspValidatePeiServiceTablePointer)){
      Status = WriteToSerialIoPpi ((UINT8 *) Buffer, AsciiStrLen (Buffer));
      if (EFI_ERROR (Status)) {
        SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
      }
    }
  }

  //
  // Send the print string to Trace Hub
  //
  if (GetDebugInterfaceFlags() & STATUS_CODE_USE_TRACEHUB) {
    TraceHubDebugWrite (SeverityError, (UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

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
  if (Buffer == NULL){
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