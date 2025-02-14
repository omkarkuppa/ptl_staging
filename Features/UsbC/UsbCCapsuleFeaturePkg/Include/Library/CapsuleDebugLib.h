/** @file
  Header file of Capsule Debug Protocol Library functions

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

#ifndef _CAPSULE_DEBUG_LIB_H_
#define _CAPSULE_DEBUG_LIB_H_

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
  );

/**
  Reset the next log read position to 0

  @param[in] This  - Pointer to EFI_Capsule_DEBUG_PROTOCOL instance

  @retval EFI_SUCCESS           - Reset the next log read position successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter

**/
EFI_STATUS
EFIAPI
LogReadReset (
  IN EFI_CAPSULE_DEBUG_PROTOCOL    *This
  );

/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] This      - Pointer to EFI_Capsule_DEBUG_PROTOCOL instance
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
  );

/**
  Read Log data from the next Log entry in Log buffer

  @param[in]  This    - Pointer to EFI_Capsule_DEBUG_PROTOCOL instance
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
  );

/**
  Parse Log data and convert to Log string

  @param[in]  This       - Pointer to EFI_Capsule_DEBUG_PROTOCOL instance
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
  );

/**
  Install Capsule Debug Protocol.
  Capsule Debug Protocol codes can be re-used by other component that needs Debug Log capability in release build.
  The caller can pass its own Protocol GUID and Log mapping table to create its own Debug Protocol.

  @param[in]  ProtocolGuid      - Pointer to Protocol GUID
  @param[in]  DebugLevel        - Debug level for controlling what kind of debug data can be written
  @param[in]  LogMappingTable   - Pointer to the Log mapping table
  @param[in]  LogMappingEntries - The number of mapping entries in Log mapping table
  @param[out] ReturnedProtocol  - Pointer of pointer to the installed Capsule Debug Protocol instance

  @retval EFI_SUCCESS           - Install Capsule Debug Protocol successfully
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
  );

#endif
