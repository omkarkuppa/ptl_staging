/** @file

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
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/SerialPortLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Library/Usb3DebugPortLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Protocol/SerialIo.h>

//
// Define the maximum debug and assert message length that this library supports
//
#define MAX_DEBUG_MESSAGE_LENGTH  0x100

/**
  Write data to SerialIo Access protocol

  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
EFI_STATUS
WriteToSerialIoUartProtocol (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  EFI_STATUS                Status;
  EFI_SERIAL_IO_PROTOCOL    *SerialIoAccess;

  if ((gBS == NULL) || (gBS->LocateProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSerialIoProtocolGuid,
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
  CHAR8                     Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST                   Marker;
  UINT8                     StatusCodeFlags;
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;

  StatusCodeFlags = GetDebugInterface ();

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

  //
  // Convert the DEBUG() message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Send the print string to all Debug Port
  //
  if (StatusCodeFlags & STATUS_CODE_USE_ISA_SERIAL) {
    SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_SERIALIO) {
    WriteToSerialIoUartProtocol ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_TRACEHUB) {
    //
    // EDK ErrorLevel to TraceHub Severity Mapping
    //
    TraceHubSeverityLevel = SeverityNormal;
    if (ErrorLevel & EFI_D_ERROR) {
      TraceHubSeverityLevel = SeverityError;
    } else if (ErrorLevel & EFI_D_WARN) {
      TraceHubSeverityLevel = SeverityWarning;
    }

    TraceHubDebugWrite (TraceHubSeverityLevel, (UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
    Usb3DebugPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
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
  CHAR8                     Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  UINT8                     StatusCodeFlags;

  StatusCodeFlags = GetDebugInterface();

  //
  // Generate the ASSERT() message in Ascii format
  //
  AsciiSPrint (Buffer, sizeof (Buffer), "ASSERT [%a] %a(%d): %a\n", gEfiCallerBaseName, FileName, LineNumber, Description);

  //
  // Send the print string to all Debug Port
  //
  if (StatusCodeFlags & STATUS_CODE_USE_ISA_SERIAL) {
    SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_TRACEHUB) {
    TraceHubDebugWrite (SeverityFatal, (UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
    Usb3DebugPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  if (StatusCodeFlags & STATUS_CODE_USE_SERIALIO) {
    WriteToSerialIoUartProtocol ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  }

  //
  // Generate a Breakpoint, DeadLoop, or NOP based on PCD settings
  //
  if ((PcdGet8(PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_BREAKPOINT_ENABLED) != 0) {
    CpuBreakpoint ();
  } else if ((PcdGet8(PcdDebugPropertyMask) & DEBUG_PROPERTY_ASSERT_DEADLOOP_ENABLED) != 0) {
    CpuDeadLoop ();
  }
}
