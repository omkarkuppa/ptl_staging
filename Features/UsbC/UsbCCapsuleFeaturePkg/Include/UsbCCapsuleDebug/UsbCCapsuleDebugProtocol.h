/** @file
  Header file for UsbC Capsule Capability Protocol implementation

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

#ifndef _USBC_CAPSULE_DEBUG_PROTOCOL_H_
#define _USBC_CAPSULE_DEBUG_PROTOCOL_H_

#include "CapsuleLogMappingEntry.h"

///
/// Global ID for the USBC_CAPSULE_DEBUG_PROTOCOL
///
#define USBC_CAPSULE_DEBUG_PROTOCOL_GUID \
  { \
    0x70CE41F4, 0x51AC, 0x5A2B, { 0x12, 0xAD, 0x1B, 0xC9, 0x87, 0x29, 0xEE, 0x7C } \
  }

//
// Log level definitions of Log data
//
#define USBC_CAPSULE_DBG_DISABLED  0
#define USBC_CAPSULE_DBG_ERROR     1
#define USBC_CAPSULE_DBG_WARN      2
#define USBC_CAPSULE_DBG_INFO      3
#define USBC_CAPSULE_DBG_VERBOSE   4

///
/// Forward declaration for USBC_CAPSULE_DEBUG_PROTOCOL
///
typedef struct _USBC_CAPSULE_DEBUG_PROTOCOL USBC_CAPSULE_DEBUG_PROTOCOL;

///
/// Define the structure of log record
///  EvtCode Structure (32 bits):
///    - Bit 0: Indicates if EvtArg0 is a Status value (1 = Yes, 0 = No).
///    - Bit 1: Indicates if EvtArg1 is a Status value (1 = Yes, 0 = No).
///    - Bits 2-3: Number of arguments (0-2).
///    - Bits 4-31: Event ID
///  EvtCode = (EventId << EVT_CODE_EVT_ID_OFFSET) | (ArgumentNumber << EVT_CODE_ARG_NUM_OFFSET) | EVT_CODE_ARG1_STATUS | EVT_CODE_ARG0_STATUS
///  EvtArg0 - Argument 0 of Log data
///  EvtArg1 - Argument 1 of Log data
///

#define EVT_CODE_ARG0_STATUS       BIT0
#define EVT_CODE_ARG1_STATUS       BIT1
#define EVT_CODE_EVT_ID_OFFSET     4
#define EVT_CODE_ARG_NUM_OFFSET    2

#define EVT_CODE_ARG_NUM_MASK(Value) ((((Value) & (BIT2 | BIT3))) >> 2)

#pragma pack (1)
typedef struct _CAPSULE_LOG_ENTRY {
  UINT32 EvtCode;
  UINT32 EvtArg0;
  UINT32 EvtArg1;
} CAPSULE_LOG_ENTRY;
#pragma pack ()

/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] This      - Pointer to USBC_CAPSULE_DEBUG_PROTOCOL instance
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
  USBC_CAPSULE_DEBUG_PROTOCOL  *This,
  UINT8                        LogLevel,
  UINT32                       EventCode,
  UINT32                       EvtArg0,
  UINT32                       EvtArg1
  );

/**
  Read Log data from the next Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in]  This    - Pointer to USBC_CAPSULE_DEBUG_PROTOCOL instance
  @param[out] LogData - Pointer to the output buffer for Log data

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_ACCESS_DENIED     - Next Read position is out of range
  @retval EFI_END_OF_FILE       - No more Log data

**/
typedef
EFI_STATUS
(EFIAPI *LOG_READ) (
  USBC_CAPSULE_DEBUG_PROTOCOL  *This,
  CAPSULE_LOG_ENTRY            *LogData
  );

/**
  Parse Log data and convert to Log string

  @param[in]  This       - Pointer to USBC_CAPSULE_DEBUG_PROTOCOL instance
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
  USBC_CAPSULE_DEBUG_PROTOCOL  *This,
  CAPSULE_LOG_ENTRY            *LogData,
  CHAR8                        *LogStr,
  UINT32                       StrBufSize
  );

/**
  Reset the next log read position to 0

  @param[in] This  - Pointer to USBC_CAPSULE_DEBUG_PROTOCOL instance

  @retval EFI_SUCCESS           - Reset the next log read position successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter

**/
typedef
EFI_STATUS
(EFIAPI *LOG_READ_RESET) (
  USBC_CAPSULE_DEBUG_PROTOCOL  *This
  );

#define CAPSULE_LOG_SIGNATURE  SIGNATURE_32 ('C', 'A', 'P', 'L')
#define CAPSULE_LOG_REVISION   0x03
///
/// The CAPSULE_LOG_ENTRY_MAX is the maximum number of log entries to record Capsule update.
/// This value is based on experimental data showing each retimer capsule needs about 0xFFFFF entries.
/// Platform might support four retimers, CAPSULE_LOG_ENTRY_MAX is defined as 6291455 (0x5FFFFF).
///
#define CAPSULE_LOG_ENTRY_MAX  6291455

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
struct _USBC_CAPSULE_DEBUG_PROTOCOL {
  CAPSULE_LOG_BUFFER               *CapsuleLogBuf;        ///< Log buffer for Log data
  const CAPSULE_LOG_MAPPING_ENTRY  *LogMappingTable;      ///< Mapping table for converting log data to log string
  UINT32                           LogMappingEntries;     ///< The number of mapping entries in Log mapping table
  UINT32                           DebugLevel;            ///< Debug level used for controlling what level of Log data can be saved in Log buffer
  LOG_WRITE                        LogWrite;              ///< Write the log data to Log buffer
  LOG_READ                         LogRead;               ///< Read the log data from Log buffer
  LOG_PARSE                        LogParse;              ///< Parse the log data to produce Log string
  LOG_READ_RESET                   LogReadReset;          ///< Reset the log data reading index to the first log entry
} ;

extern EFI_GUID  gUsbCCapsuleDebugProtocolGuid;
#endif
