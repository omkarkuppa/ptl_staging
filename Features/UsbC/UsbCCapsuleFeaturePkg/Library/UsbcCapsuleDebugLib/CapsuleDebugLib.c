/** @file
  CAPSULE Debug Capability Protocol implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/CapsuleDebugProtocol.h>
#include <Library/CapsuleDebugLib.h>

/**
  Convert the format string and arguments to log string

  @param[out] LogStr  - String buffer for the converted log string
  @param[in]  BufSize - String buffer size
  @param[in]  Format  - String format specifier of log string
  @param[in]  ...     - Arguments of string format specifier

  @retval The number of ASCII characters in the produced string not including NULL-terminator

**/
UINTN
EFIAPI
LogDataToStr (
  OUT      CHAR8   *LogStr,
  IN       UINT32  BufSize,
  IN CONST CHAR8   *Format,
  ...
  )
{
  UINTN     Count;
  VA_LIST   Marker;

  if ((LogStr == NULL) || (Format == NULL)) {
    DEBUG ((DEBUG_ERROR, "CapsuleToLogStr: Invalid parameter (%p, %p)\n", LogStr, Format));
    Count = 0;
    goto Exit;
  }

  ///
  /// Produce a Null-terminated ASCII string in the log string buffer
  ///
  VA_START (Marker, Format);
  Count = AsciiVSPrint (LogStr, BufSize, Format, Marker);
  VA_END (Marker);

Exit:
  return Count;
}

/**
  Reset the next log read position to 0

  @param[in] This - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance

**/
EFI_STATUS
EFIAPI
LogReadReset (
  IN EFI_CAPSULE_DEBUG_PROTOCOL  *This
  )
{
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Reset next log read position
  ///
  This->CapsuleLogBuf.NextRead = 0;

  return EFI_SUCCESS;
}

/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] This      - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance
  @param[in] LogLevel  - Log level associated with Log data
  @param[in] EventCode - Event code of Log data
  @param[in] EvtArg0   - Argument 0 of Log data
  @param[in] EvtArg1   - Argument 1 of Log data

  @retval EFI_SUCCESS           - Write Log data to Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_OUT_OF_RESOURCES  - Log buffer is full

**/
EFI_STATUS
EFIAPI
LogWrite (
  IN EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  IN UINT8                       LogLevel,
  IN UINT32                      EventCode,
  IN UINT32                      EvtArg0,
  IN UINT32                      EvtArg1
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  if (This == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  ///
  /// Write Log data to Log buffer only if LogLevel <= Capsule Debug Protocol's DebugLevel
  ///
  if (LogLevel <= This->DebugLevel) {
    ///
    /// Check if a free Log entry is available for Log data
    ///
    if (This->CapsuleLogBuf.LogCount >= CAPSULE_LOG_ENTRY_MAX) {
      DEBUG ((DEBUG_ERROR, "CapsuleLogWrite: Log buffer is full!"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    ///
    /// Save Log data to the next available Log entry
    ///
    This->CapsuleLogBuf.Entry[This->CapsuleLogBuf.LogCount].EvtCode = EventCode;
    This->CapsuleLogBuf.Entry[This->CapsuleLogBuf.LogCount].EvtArg0 = EvtArg0;
    This->CapsuleLogBuf.Entry[This->CapsuleLogBuf.LogCount].EvtArg1 = EvtArg1;
    This->CapsuleLogBuf.LogCount++;
  }

Exit:
  return Status;
}

/**
  Read Log data from the next Log entry in Log buffer

  @param[in]  This    - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance
  @param[out] LogData - Pointer to the output buffer for Log data

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_ACCESS_DENIED     - Next Read position is out of range
  @retval EFI_END_OF_FILE       - No more Log data

**/
EFI_STATUS
EFIAPI
LogRead (
  IN  EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  OUT CAPSULE_LOG_ENTRY           *LogData
  )
{
  EFI_STATUS Status;

  if (This == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (LogData == NULL) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogRead: Null Log data buffer\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (This->CapsuleLogBuf.NextRead >= CAPSULE_LOG_ENTRY_MAX) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogRead: NextRead = %d is out of range!\n", This->CapsuleLogBuf.NextRead));
    Status = EFI_ACCESS_DENIED;
    goto Exit;
  }

  Status = EFI_SUCCESS;

  ///
  /// Get Log data if available
  ///
  if (This->CapsuleLogBuf.NextRead < This->CapsuleLogBuf.LogCount) {
    CopyMem (LogData, &(This->CapsuleLogBuf.Entry[This->CapsuleLogBuf.NextRead]), sizeof (CAPSULE_LOG_ENTRY));
    This->CapsuleLogBuf.NextRead++;
  } else {
    DEBUG ((DEBUG_ERROR, "No more Log data to read\n"));
    DEBUG ((DEBUG_ERROR, "NextRead = %d, LogCount = %d\n", This->CapsuleLogBuf.NextRead, This->CapsuleLogBuf.LogCount));
    Status = EFI_END_OF_FILE;
  }

Exit:
  return Status;
}

/**
  Parse Log data and convert to Log string

  @param[in]  This       - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance
  @param[in]  LogData    - Pointer to the Log data
  @param[out] LogStr     - Pointer to the output Log string buffer
  @param[in]  StrBufSize - Log string buffer size

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_UNSUPPORTED       - Fail to produce log string from log data and log mapping table

**/
EFI_STATUS
EFIAPI
LogParse (
  IN  EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  IN  CAPSULE_LOG_ENTRY           *LogData,
  OUT CHAR8                       *LogStr,
  IN  UINT32                      StrBufSize
  )
{
  EFI_STATUS                       Status;
  UINT16                           EvtId;
  UINT32                           Argc;
  const CAPSULE_LOG_MAPPING_ENTRY  *Mappings;

  Status = EFI_SUCCESS;

  if (This == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((LogData == NULL) || (LogStr == NULL)) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Invalid parameter (%p, %p)\n", LogData, LogStr));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (This->LogMappingTable == NULL) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Log mapping table is not found!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  ///
  /// Event ID is used as index for looking up Log mapping table
  /// Make sure Log data Event ID is valid for Log mapping table
  ///
  EvtId = (UINT16) (((UINT32) LogData->EvtCode) >> 2);
  if (EvtId >= This->LogMappingEntries) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Invalid Event ID 0x%x for Log Mapping table\n", EvtId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Mappings = This->LogMappingTable;

  ///
  /// Make sure Event code of Log mapping entry is same with Log data Event code
  ///
  if (Mappings[EvtId].EvtCode != LogData->EvtCode) {
    DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Event ID and Event code mismatch in Log mapping table\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  ///
  /// Get the argument count of event from lower two bits of Event code
  ///
  Argc = (UINT32)(LogData->EvtCode & 0x0003);

  ///
  /// LogDataToStr returns the character count of produced log string
  /// Returned value 0 means the log string is not produced
  ///
  switch (Argc) {
    case 0:
      if (LogDataToStr (LogStr, StrBufSize, Mappings[EvtId].LogStr) == 0) {
        Status = EFI_UNSUPPORTED;
      }
      break;
    case 1:
      if (LogDataToStr (LogStr, StrBufSize, Mappings[EvtId].LogStr, LogData->EvtArg0) == 0) {
        Status = EFI_UNSUPPORTED;
      }
      break;
    case 2:
      if (LogDataToStr (LogStr, StrBufSize, Mappings[EvtId].LogStr, LogData->EvtArg0, LogData->EvtArg1) == 0) {
        Status = EFI_UNSUPPORTED;
      }
      break;
    default:
      DEBUG ((DEBUG_ERROR, "CapsuleLogParse: Unsupported argument number %d\n", Argc));
      Status = EFI_UNSUPPORTED;
      break;
  }

Exit:
  return Status;
}

/**
  Install CAPSULE Debug Protocol.
  CAPSULE Debug Protocol codes can be re-used by other component that needs Debug Log capability in release build.
  The caller can pass its own Protocol GUID and Log mapping table to create its own Debug Protocol.

  @param[in]  ProtocolGuid      - Pointer to Protocol GUID
  @param[in]  DebugLevel        - Debug level for controlling what kind of debug data can be written
  @param[in]  LogMappingTable   - Pointer to the Log mapping table
  @param[in]  LogMappingEntries - The number of mapping entries in Log mapping table
  @param[out] ReturnedProtocol  - Pointer of pointer to the installed CAPSULE Debug Protocol instance

  @retval EFI_SUCCESS           - Install CAPSULE Debug Protocol successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_UNSUPPORTED       - Protocol creation failure
**/
EFI_STATUS
InstallCapsuleDebugLibProtocol (
  IN EFI_GUID                         *ProtocolGuid,
  IN UINT32                           DebugLevel,
  IN const CAPSULE_LOG_MAPPING_ENTRY  *LogMappingTable,
  IN UINT32                           LogMappingEntries,
  OUT EFI_CAPSULE_DEBUG_PROTOCOL      **ReturnedProtocol
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  EFI_CAPSULE_DEBUG_PROTOCOL  *CapsuleDebugProtocol;
  UINT32                      Index;
  UINT32                      EvtId;

  if ((ProtocolGuid == NULL) || (LogMappingTable == NULL)) {
    DEBUG ((DEBUG_ERROR, "InstallCapsuleDebugProtocol: Invalid parameters (%p, %p)\n", ProtocolGuid, LogMappingTable));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (DebugLevel > CAPSULE_DBG_VERBOSE) {
    DEBUG ((DEBUG_ERROR, "InstallCapsuleDebugProtocol: Invalid debug level %d\n", DebugLevel));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (LogMappingEntries == 0) {
    DEBUG ((DEBUG_ERROR, "InstallUsbDebugProtocol: No mapping entry in Log mapping table\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Capsule Log Mapping entries = %d\n", LogMappingEntries));

  ///
  /// Validate Event ID of each mapping entry align with entry index
  ///
  for (Index = 0; Index < LogMappingEntries; Index++) {
    EvtId = LogMappingTable[Index].EvtCode >> 2;
    if (Index != EvtId) {
      DEBUG ((DEBUG_ERROR, "Log mapping entry %d Event ID mismatch, Event ID = %d\n", Index, EvtId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }

  ///
  /// Allocate memory for protocol
  ///
  CapsuleDebugProtocol = (EFI_CAPSULE_DEBUG_PROTOCOL *) AllocateZeroPool (sizeof (EFI_CAPSULE_DEBUG_PROTOCOL));
  if (CapsuleDebugProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "InstallCapsuleDebugProtocol: Insufficient buffer for allocating protocol\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ///
  /// Initialize protocol data and interfaces
  ///
  CapsuleDebugProtocol->CapsuleLogBuf.Signature = EFI_CAPSULE_LOG_SIGNATURE;
  CapsuleDebugProtocol->CapsuleLogBuf.Revision  = EFI_CAPSULE_LOG_REVISION;
  CapsuleDebugProtocol->CapsuleLogBuf.LogCount  = 0x00;
  CapsuleDebugProtocol->CapsuleLogBuf.NextRead  = 0x00;
  CapsuleDebugProtocol->LogMappingTable         = LogMappingTable;
  CapsuleDebugProtocol->LogMappingEntries       = LogMappingEntries;
  CapsuleDebugProtocol->DebugLevel              = DebugLevel;
  CapsuleDebugProtocol->LogWrite                = LogWrite;
  CapsuleDebugProtocol->LogRead                 = LogRead;
  CapsuleDebugProtocol->LogParse                = LogParse;
  CapsuleDebugProtocol->LogReadReset            = LogReadReset;

  ///
  /// Install CAPSULE Debug Protocol.
  ///
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  CapsuleDebugProtocol
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallCapsuleDebugProtocol: Fail to install protocol, %r\n", Status));
    goto Exit;
  }

  ///
  /// Pass the installed protocol pointer if the caller needs it
  ///
  if (ReturnedProtocol != NULL) {
    *ReturnedProtocol = CapsuleDebugProtocol;
  }

Exit:
  return Status;
}
