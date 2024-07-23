/** @file
  Header file for USB4 Debug Capability Protocol implementation

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

#ifndef _EFI_USB4_DEBUG_PROTOCOL_H_
#define _EFI_USB4_DEBUG_PROTOCOL_H_

#include "Usb4LogMappingEntry.h"

///
/// Global ID for the EFI_USB4_DEBUG_PROTOCOL
///
#define EFI_USB4_DEBUG_PROTOCOL_GUID \
  { \
    0x80bf4c72, 0x460d, 0x4231, { 0x89, 0x4f, 0xfa, 0xe9, 0x90, 0x15, 0xe0, 0x6d } \
  }

///
/// Forward declaration for EFI_USB4_DEBUG_PROTOCOL
///
typedef struct _EFI_USB4_DEBUG_PROTOCOL EFI_USB4_DEBUG_PROTOCOL;

//
// Define the structure of  log record
//  EvtCode - (EvtId << 2) | (argument number:2 bits)
//  EvtArg0 - Argument 0 of Log data
//  EvtArg1 - Argument 1 of Log data
//
#pragma pack (1)
typedef struct _USB4_LOG_ENTRY {
    UINT16 EvtCode;
    UINT32 EvtArg0;
    UINT32 EvtArg1;
} USB4_LOG_ENTRY;
#pragma pack ()

//
// Log level definitions of Log data
//
#define USB4_DBG_DISABLED   0
#define USB4_DBG_ERROR      1
#define USB4_DBG_WARN       2
#define USB4_DBG_INFO       3
#define USB4_DBG_VERBOSE    4

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
typedef
EFI_STATUS
(EFIAPI *USB4_LOG_WRITE) (
    EFI_USB4_DEBUG_PROTOCOL    *This,
    UINT8                      LogLevel,
    UINT16                     EventCode,
    UINT32                     EvtArg0,
    UINT32                     EvtArg1
    );

/**
  Read Log data from the next Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in]  This    - Pointer to EFI_USB4_DEBUG_PROTOCOL instance
  @param[out] LogData - Pointer to the output buffer for Log data

  @retval EFI_SUCCESS           - Read Log data from Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_ACCESS_DENIED     - Next Read position is out of range
  @retval EFI_END_OF_FILE       - No more Log data
**/
typedef
EFI_STATUS
(EFIAPI *USB4_LOG_READ) (
    EFI_USB4_DEBUG_PROTOCOL    *This,
    USB4_LOG_ENTRY             *LogData
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
typedef
EFI_STATUS
(EFIAPI *USB4_LOG_PARSE) (
    EFI_USB4_DEBUG_PROTOCOL    *This,
    USB4_LOG_ENTRY             *LogData,
    CHAR8                      *LogStr,
    UINT32                     StrBufSize
    );

/**
  Reset the next log read position to 0

  @param[in] This  - Pointer to EFI_USB4_DEBUG_PROTOCOL instance

  @retval EFI_SUCCESS           - Reset the next log read position successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
**/
typedef
EFI_STATUS
(EFIAPI *USB4_LOG_READ_RESET) (
    EFI_USB4_DEBUG_PROTOCOL    *This
    );

#define EFI_USB4_LOG_SIGNATURE SIGNATURE_32 ('U', 'S', 'B', '4')
#define EFI_USB4_LOG_REVISION  0x01
#define USB4_LOG_ENTRY_MAX  65535

#pragma pack (1)
typedef struct _USB4_LOG_BUFFER {
  UINT32           Signature;     // Used to confirm log buffer is valid
  UINT32           Revision;      // Revision should be incremented whenever we define new event code is  in mapping table
  UINT32           LogCount;      // The count of valid Log data
  UINT32           NextRead;      // The index of Log entry that will be read in the next Usb4LogRead call
  USB4_LOG_ENTRY   Entry [USB4_LOG_ENTRY_MAX];
} USB4_LOG_BUFFER;
#pragma pack()

//
// Data structure of USB4 Debug Protocol
//
struct _EFI_USB4_DEBUG_PROTOCOL {
  USB4_LOG_BUFFER               Usb4LogBuf;            // Log buffer for Log data
  const USB4_LOG_MAPPING_ENTRY  *LogMappingTable;      // Mapping table for converting log data to log string
  UINT32                        LogMappingEntries;     // The number of mapping entries in Log mapping table
  UINT32                        DebugLevel;            // Debug level used for controlling what level of Log data can be saved in Log buffer
  USB4_LOG_WRITE                Usb4LogWrite;          // Write the log data to Log buffer
  USB4_LOG_READ                 Usb4LogRead;           // Read the log data from Log buffer
  USB4_LOG_PARSE                Usb4LogParse;          // Parse the log data to produce Log string
  USB4_LOG_READ_RESET           Usb4LogReadReset;      // Reset the log data reading index to the first log entry
} ;

extern EFI_GUID  gEfiUsb4DebugProtocolGuid;
#endif
