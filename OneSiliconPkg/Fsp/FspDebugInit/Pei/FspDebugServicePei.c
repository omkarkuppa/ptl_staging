/** @file

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
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Ppi/FspDebugService.h>
#include <Library/FspCommonLib.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <FsptUpd.h>
#include <FspsUpd.h>
#include <FspmUpd.h>
#include <Library/FspSwitchStackLib.h>

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
  Function print string buffer to Serial Io Uart Port

  @param[in] Buffer              Pointer to string buffer
  @param[in] NumberOfBytes       Size of string buffer

  @retval EFI_SUCCESS            The function completed successfully
**/
EFI_STATUS
WriteToSerialIoPpi (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  EFI_STATUS                Status;
  EFI_SERIAL_IO_PROTOCOL    *SerialIoAccess;

  Status = PeiServicesLocatePpi (
             &gEfiSerialIoProtocolGuid,
             0,
             NULL,
             (VOID **) &SerialIoAccess
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SerialIoAccess->Write (SerialIoAccess, &NumberOfBytes, Buffer);
  return EFI_SUCCESS;
}

/**
  Prints a debug message to the debug output device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then print the message specified by Format and the
  associated variable argument list to the debug output device.

  If Format is NULL, then ASSERT().

  If the length of the message string specificed by Format is larger than the maximum allowable
  record length, then directly return and not print it.

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      Format string for the debug message to print.
  @param  Marker      Variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
VOID
EFIAPI
FspDebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CONST CHAR8  *Format,
  IN  VA_LIST      VaListMarker
  )
{
  CONST EFI_PEI_SERVICES       **PeiServices;
  CHAR8                        Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  TRACE_HUB_SEVERITY_TYPE      TraceHubSeverityLevel;
  UINT64                       Buffer1[(EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof(UINT64)) + 1];
  EFI_STATUS_CODE_STRING_DATA  *StringData;
  CHAR8                        *FormatString;
  UINTN                        CharCount;
  UINTN                        StringDataMaxSize;
  FSP_GLOBAL_DATA              *FspGlobalData;
  FSPS_UPD                     *FspsUpd;
  FSPM_UPD                     *FspmUpd;
  EVENT_HANDLER_PARAMETER      FspEventHandlerParameter;
  BOOLEAN                      FspEventHandlerExecuted;
  EFI_STATUS                   Status;

  FspGlobalData = NULL;
  FspEventHandlerParameter.FspEventHandler = NULL;
  FspsUpd = NULL;
  FspmUpd = NULL;
  FspEventHandlerExecuted = FALSE;

  //
  // If Format is NULL, then ASSERT().
  //
  ASSERT (Format != NULL);

  FspGlobalData = GetFspGlobalDataPointer ();

  if (((UINTN)FspGlobalData != 0x00 && (UINTN)FspGlobalData != 0xFFFFFFFF) && (FspGlobalData->Signature == FSP_GLOBAL_DATA_SIGNATURE)) {
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
    FormatString[0] = '\0';
    CharCount = AsciiBSPrint (FormatString, StringDataMaxSize, Format, (BASE_LIST) VaListMarker);
    if (CharCount < StringDataMaxSize) {
      ((EFI_STATUS_CODE_STRING_DATA *) StringData)->DataHeader.Size = (UINT16) CharCount;
    }
    //
    // Found Valid Global Data
    //
    if (FspGlobalData->FspMode == FSP_IN_DISPATCH_MODE) { // FSP_MODE_CHECK
      //
      // Send the StringData record
      //
      PeiServices = GetPeiServicesTablePointer ();
      (*PeiServices)->ReportStatusCode (
                        PeiServices,
                        EFI_DEBUG_CODE,
                        EFI_SOFTWARE_PEI_MODULE,
                        (UINT32) ErrorLevel,
                        (EFI_GUID *) &gEfiStatusCodeDataTypeStringGuid,
                        (EFI_STATUS_CODE_DATA *) StringData
                        );
      return;
    } else { // API Mode flow
      //
      // Find Fsp Handler
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
  }

  if (!FspEventHandlerExecuted) {
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
      Status = WriteToSerialIoPpi ((UINT8 *) Buffer, AsciiStrLen (Buffer));
      if (EFI_ERROR (Status)) {
        SerialPortWrite ((UINT8 *) Buffer, AsciiStrLen (Buffer));
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
    } else if (ErrorLevel & DEBUG_WARN ) {
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
  @param  DebugPropertyMask  PropertyMask to the description of the assert.

**/
VOID
EFIAPI
FspDispatchDebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description,
  IN UINT8        DebugPropertyMask
  )
{
  UINT64                 Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof(UINT64)];
  EFI_DEBUG_ASSERT_DATA  *AssertData;
  UINTN                  HeaderSize;
  UINTN                  TotalSize;
  CHAR8                  *Temp;
  UINTN                  FileNameSize;
  UINTN                  DescriptionSize;

  //
  // Get string size
  //
  HeaderSize       = sizeof (EFI_DEBUG_ASSERT_DATA);
  FileNameSize     = AsciiStrSize (FileName);
  DescriptionSize  = AsciiStrSize (Description);

  //
  // Make sure it will all fit in the passed in buffer.
  //
  if (HeaderSize + FileNameSize + DescriptionSize > sizeof (Buffer)) {
    //
    // FileName + Description is too long to be filled into buffer.
    //
    if (HeaderSize + FileNameSize < sizeof (Buffer)) {
      //
      // Description has enough buffer to be truncated.
      //
      DescriptionSize = sizeof (Buffer) - HeaderSize - FileNameSize;
    } else {
      //
      // FileName is too long to be filled into buffer.
      // FileName will be truncated. Reserved one byte for Description NULL terminator.
      //
      DescriptionSize = 1;
      FileNameSize    = sizeof (Buffer) - HeaderSize - DescriptionSize;
    }
  }

  //
  // Fill in EFI_DEBUG_ASSERT_DATA
  //
  AssertData = (EFI_DEBUG_ASSERT_DATA *)Buffer;
  AssertData->LineNumber = (UINT32)LineNumber;
  TotalSize  = sizeof (EFI_DEBUG_ASSERT_DATA);

  //
  // Copy Ascii FileName including NULL terminator.
  //
  Temp = CopyMem (AssertData + 1, FileName, FileNameSize);
  Temp[FileNameSize - 1] = 0;
  TotalSize += FileNameSize;

  //
  // Copy Ascii Description include NULL terminator.
  //
  Temp = CopyMem (Temp + FileNameSize, Description, DescriptionSize);
  Temp[DescriptionSize - 1] = 0;
  TotalSize += DescriptionSize;

  REPORT_STATUS_CODE_EX (
    (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
    (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE),
    0,
    NULL,
    NULL,
    AssertData,
    TotalSize
    );

  //
  // Generate a Breakpoint, DeadLoop, or NOP based on PCD settings
  //
  if ((DebugPropertyMask & DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED) != 0) {
    CpuBreakpoint ();
  } else if ((DebugPropertyMask & DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED) != 0) {
    CpuDeadLoop ();
  }

}

VOID
EFIAPI
FspApiDebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description,
  IN UINT8        DebugPropertyMask
  )
{
  CHAR8      Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  EFI_STATUS Status;

  //
  // Generate the ASSERT() message in Ascii format
  //
  AsciiSPrint (Buffer, sizeof (Buffer), "ASSERT %a(%d): %a\n", FileName, LineNumber, Description);

  //
  // Send the print string to the Console Output device
  //
  SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  Status = WriteToSerialIoPpi ((UINT8 *) Buffer, AsciiStrLen (Buffer));
  if (EFI_ERROR (Status)) {
    SerialPortWrite ((UINT8 *) Buffer, AsciiStrLen (Buffer));
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
  if ((DebugPropertyMask & DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED) != 0) {
    CpuBreakpoint ();
  } else if ((DebugPropertyMask & DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED) != 0) {
    CpuDeadLoop ();
  }
}

VOID
EFIAPI
FspDebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description,
  IN UINT8        DebugPropertyMask
  )
{
  FSP_GLOBAL_DATA *FspData;

  FspData = GetFspGlobalDataPointer ();
  if (((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) || (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    FspApiDebugAssert((CHAR8 *) FileName,LineNumber,(CHAR8 *) Description,DebugPropertyMask);
  } else {
    if (FspData->FspMode == FSP_IN_DISPATCH_MODE) { // FSP_MODE_CHECK
      FspDispatchDebugAssert((CHAR8 *) FileName,LineNumber,(CHAR8 *) Description,DebugPropertyMask);
    } else {
      FspApiDebugAssert((CHAR8 *) FileName,LineNumber,(CHAR8 *) Description,DebugPropertyMask);
    }
  }
}

DEBUG_SERVICE_PPI mFspDebugServicePpi = {
  FspDebugPrint,
  FspDebugAssert
};

EFI_PEI_PPI_DESCRIPTOR mPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gFspDebugServicePpiGuid,
    (VOID *) &mFspDebugServicePpi
  }
};

/**
  Main entry

  @param[in]  FileHandle              Handle of the file being invoked.
  @param[in]  PeiServices             Pointer to PEI Services table.

  @retval EFI_SUCCESS Install function successfully.

**/
EFI_STATUS
EFIAPI
DebugServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS               Status;
  FSP_GLOBAL_DATA          *FspData;
  BOOLEAN                  FspDataValid;

  FspDataValid = FALSE;
  FspData = GetFspGlobalDataPointer ();
  if (((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) || (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    Status = SerialPortInitialize ();
    ASSERT_EFI_ERROR (Status);
  } else {
    FspDataValid = TRUE;
    if (FspData->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
      Status = SerialPortInitialize ();
      ASSERT_EFI_ERROR (Status);
    }
  }

  Status = PeiServicesInstallPpi (&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  if (FspDataValid == TRUE) {
    DEBUG ((DEBUG_INFO, " FspData: Signature             - 0x%x\n", FspData->Signature));
    DEBUG ((DEBUG_INFO, " FspData: Version               - 0x%x\n", FspData->Version));
    DEBUG ((DEBUG_INFO, " FspData: PerfIdx               - 0x%x\n", FspData->PerfIdx));
    DEBUG ((DEBUG_INFO, " FspData: PerfSig               - 0x%x\n", FspData->PerfSig));
    DEBUG ((DEBUG_INFO, " FspData: FspMode               - %s\n",   FspData->FspMode ? L"DISPATCH" : L"API"));
    DEBUG ((DEBUG_INFO, " FspData: FspArch               - %s\n",   ((sizeof (UINTN) == sizeof (UINT64)) != 0)? L"X64" : L"IA32"));
    DEBUG ((DEBUG_INFO, " FspData: TempRamInitUpdPtr     - 0x%x\n", FspData->TempRamInitUpdPtr));
  }

  return EFI_SUCCESS;
}
