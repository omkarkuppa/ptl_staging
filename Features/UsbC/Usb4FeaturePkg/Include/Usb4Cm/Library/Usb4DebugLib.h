/** @file
  Header file of USB4 Debug Protocol Library functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _USB4_DEBUG_LIB_H_
#define _USB4_DEBUG_LIB_H_

#include <Uefi.h>
#include <Usb4Debug/Usb4DebugProtocol.h>
#include <Usb4Debug/Usb4LogEvents.h>

/**
  Reset the next log read position to 0

  @param[in] This  - Pointer to EFI_USB4_DEBUG_PROTOCOL instance

  @retval EFI_SUCCESS           - Reset the next log read position successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
**/
EFI_STATUS
EFIAPI
Usb4LogReadReset (
  IN EFI_USB4_DEBUG_PROTOCOL    *This
  );

/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] This      - Pointer to EFI_USB4_DEBUG_PROTOCOL instance
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
Usb4LogWrite (
  IN EFI_USB4_DEBUG_PROTOCOL    *This,
  IN UINT8                      LogLevel,
  IN UINT16                     EventCode,
  IN UINT32                     EvtArg0,
  IN UINT32                     EvtArg1
  );

/**
  Read Log data from the next Log entry in Log buffer

  @param[in]  This    - Pointer to EFI_USB4_DEBUG_PROTOCOL instance
  @param[out] LogData - Pointer to the output buffer for Log data

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_ACCESS_DENIED     - Next Read position is out of range
  @retval EFI_END_OF_FILE       - No more Log data
**/
EFI_STATUS
EFIAPI
Usb4LogRead (
  IN  EFI_USB4_DEBUG_PROTOCOL    *This,
  OUT USB4_LOG_ENTRY             *LogData
  );

/**
  Parse Log data and convert to Log string

  @param[in]  This       - Pointer to EFI_USB4_DEBUG_PROTOCOL instance
  @param[in]  LogData    - Pointer to the Log data
  @param[out] LogStr     - Pointer to the output Log string buffer
  @param[in]  StrBufSize - Log string buffer size

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_UNSUPPORTED       - Fail to produce log string from log data and log mapping table
**/
EFI_STATUS
EFIAPI
Usb4LogParse (
  IN  EFI_USB4_DEBUG_PROTOCOL    *This,
  IN  USB4_LOG_ENTRY             *LogData,
  OUT CHAR8                      *LogStr,
  IN  UINT32                     StrBufSize
  );

/**
  Install USB4 Debug Protocol.
  USB4 Debug Protocol codes can be re-used by other component that needs Debug Log capability in release build.
  The caller can pass its own Protocol GUID and Log mapping table to create its own Debug Protocol.

  @param[in]  ProtocolGuid      - Pointer to Protocol GUID
  @param[in]  DebugLevel        - Debug level for controlling what kind of debug data can be written
  @param[in]  LogMappingTable   - Pointer to the Log mapping table
  @param[in]  LogMappingEntries - The number of mapping entries in Log mapping table
  @param[out] ReturnedProtocol  - Pointer of pointer to the installed USB4 Debug Protocol instance

  @retval EFI_SUCCESS           - Install USB4 Debug Protocol successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_UNSUPPORTED       - Protocol creation failure
**/
EFI_STATUS
InstallUsb4DebugProtocol (
  IN EFI_GUID                        *ProtocolGuid,
  IN UINT32                          DebugLevel,
  IN const USB4_LOG_MAPPING_ENTRY    *LogMappingTable,
  IN UINT32                          LogMappingEntries,
  OUT EFI_USB4_DEBUG_PROTOCOL        **ReturnedProtocol
  );

#endif
