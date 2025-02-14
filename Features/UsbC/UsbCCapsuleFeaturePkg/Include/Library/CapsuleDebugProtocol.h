/** @file
  Header file for Capsule Debug Capability Protocol implementation

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

#ifndef _EFI_CAPSULE_DEBUG_PROTOCOL_H_
#define _EFI_CAPSULE_DEBUG_PROTOCOL_H_

#include "CapsuleLogMappingEntry.h"

///
/// Global ID for the EFI_CAPSULE_DEBUG_PROTOCOL
///
#define EFI_CAPSULE_DEBUG_PROTOCOL_GUID \
  { \
    0x90bf4c77, 0x460d, 0x4231, { 0x89, 0x4f, 0xfa, 0xe9, 0x90, 0x55, 0xee, 0x66 } \
  }

///
/// Forward declaration for EFI_CAPSULE_DEBUG_PROTOCOL
///
typedef struct _EFI_CAPSULE_DEBUG_PROTOCOL EFI_CAPSULE_DEBUG_PROTOCOL;

///
/// Define the structure of  log record
///  EvtCode - (EvtId << 2) | (argument number:2 bits)
///  EvtArg0 - Argument 0 of Log data
///  EvtArg1 - Argument 1 of Log data
///
#pragma pack (1)
typedef struct _CAPSULE_LOG_ENTRY {
  UINT32 EvtCode;
  UINT32 EvtArg0;
  UINT32 EvtArg1;
} CAPSULE_LOG_ENTRY;
#pragma pack ()

///
/// Log level definitions of Log data
///
#define CAPSULE_DBG_DISABLED   0
#define CAPSULE_DBG_ERROR      1
#define CAPSULE_DBG_WARN       2
#define CAPSULE_DBG_INFO       3
#define CAPSULE_DBG_VERBOSE    4

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
typedef
EFI_STATUS
(EFIAPI *LOG_WRITE) (
  EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  UINT8                       LogLevel,
  UINT32                      EventCode,
  UINT32                      EvtArg0,
  UINT32                      EvtArg1
  );

/**
  Read Log data from the next Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in]  This    - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance
  @param[out] LogData - Pointer to the output buffer for Log data

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_ACCESS_DENIED     - Next Read position is out of range
  @retval EFI_END_OF_FILE       - No more Log data

**/
typedef
EFI_STATUS
(EFIAPI *LOG_READ) (
  EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  CAPSULE_LOG_ENTRY           *LogData
  );

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
typedef
EFI_STATUS
(EFIAPI *LOG_PARSE) (
  EFI_CAPSULE_DEBUG_PROTOCOL  *This,
  CAPSULE_LOG_ENTRY           *LogData,
  CHAR8                       *LogStr,
  UINT32                      StrBufSize
  );

/**
  Reset the next log read position to 0

  @param[in] This  - Pointer to EFI_CAPSULE_DEBUG_PROTOCOL instance

  @retval EFI_SUCCESS           - Reset the next log read position successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter

**/
typedef
EFI_STATUS
(EFIAPI *LOG_READ_RESET) (
  EFI_CAPSULE_DEBUG_PROTOCOL  *This
  );

#define EFI_CAPSULE_LOG_SIGNATURE SIGNATURE_32 ('C', 'A', 'P', 'L')
#define EFI_CAPSULE_LOG_REVISION  0x01
///
/// The CAPSULE_LOG_ENTRY_MAX is the maximum number of log entries to record Capsule update.
/// This value is based on experimental data showing each retimer capsule needs about 0xFFFFF entries.
/// Platform might support four retimers, CAPSULE_LOG_ENTRY_MAX is defined as 4194303 (0x3FFFFF).
///
#define CAPSULE_LOG_ENTRY_MAX  4194303

#pragma pack (1)
typedef struct _CAPSULE_LOG_BUFFER {
  UINT32             Signature;     /// Used to confirm log buffer is valid
  UINT32             Revision;      /// Revision should be incremented whenever we define new event code is  in mapping table
  UINT32             LogCount;      /// The count of valid Log data
  UINT32             NextRead;      /// The index of Log entry that will be read in the next CapsuleLogRead call
  CAPSULE_LOG_ENTRY  Entry [CAPSULE_LOG_ENTRY_MAX];
} CAPSULE_LOG_BUFFER;
#pragma pack()

///
/// Data structure of Capsule Debug Protocol
///
struct _EFI_CAPSULE_DEBUG_PROTOCOL {
  CAPSULE_LOG_BUFFER               CapsuleLogBuf;         ///< Log buffer for Log data
  const CAPSULE_LOG_MAPPING_ENTRY  *LogMappingTable;      ///< Mapping table for converting log data to log string
  UINT32                           LogMappingEntries;     ///< The number of mapping entries in Log mapping table
  UINT32                           DebugLevel;            ///< Debug level used for controlling what level of Log data can be saved in Log buffer
  LOG_WRITE                        LogWrite;              ///< Write the log data to Log buffer
  LOG_READ                         LogRead;               ///< Read the log data from Log buffer
  LOG_PARSE                        LogParse;              ///< Parse the log data to produce Log string
  LOG_READ_RESET                   LogReadReset;          ///< Reset the log data reading index to the first log entry
} ;

extern EFI_GUID  gEfiCapsuleDebugProtocolGuid;
#endif
