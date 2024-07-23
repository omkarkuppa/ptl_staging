/** @file
  Configurations for TraceHub

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _TRACE_HUB_CONFIG_H_
#define _TRACE_HUB_CONFIG_H_

#include <ConfigBlock.h>

#define PCH_TRACE_HUB_PREMEM_CONFIG_REVISION 1
#define TRACE_HUB_PREMEM_CONFIG_REVISION 1
#define TRACEHUB_MEM_SIZE(TRACE_BUFFER_SIZE) (((TRACE_BUFFER_SIZE) == TraceBufferNone) ? ((UINT64) (0x0)) : (LShiftU64 ((UINT64) (1), (TRACE_BUFFER_SIZE + 20))))

extern EFI_GUID gPchTraceHubPreMemConfigGuid;
extern EFI_GUID gTraceHubPreMemConfigGuid;

typedef enum {
  SocTraceHub,
  PchTraceHub,
  CpuTraceHub,
  MaxTraceHub
} TRACE_HUB_DEVICE;
///
/// The TRACE_HUB_ENABLE_MODE describes TraceHub mode of operation
///
typedef enum {
  TraceHubModeDisabled = 0,
  TraceHubModeEnabled = 1,
  TraceHubModeHostDebugger = 2,
  TraceHubModeMax
} TRACE_HUB_ENABLE_MODE;

///
/// The TRACE_BUFFER_SIZE describes the desired TraceHub buffer size
///
typedef enum {
  TraceBuffer1M   = 0x00,
  TraceBuffer8M   = 0x03,
  TraceBuffer64M  = 0x06,
  TraceBuffer128M = 0x07,
  TraceBuffer256M = 0x08,
  TraceBuffer512M = 0x09,
  TraceBuffer1G   = 0x0A,
  TraceBuffer2G   = 0x0B,
  TraceBuffer4G   = 0x0C,
  TraceBuffer8G   = 0x0D,
  TraceBufferNone = 0x0E
} TRACE_BUFFER_SIZE;

#pragma pack (push,1)
///
/// TRACE_HUB_CONFIG block describes TraceHub settings
///
typedef struct {

  /**
  Trace hub memory buffer region size value.
  Supported size refer to TRACE_BUFFER_SIZE
  **/
  UINT64                     MemReg0Size;
  UINT64                     MemReg1Size;
  /**
  Trace hub mode. Default is disabled.
  <b>0 = Disable</b>; 1 = Enabled
  Refer to TRACE_HUB_ENABLE_MODE
  **/
  UINT8                      EnableMode;
  /**
  AET is one of FW trace agents which is exclusively rounted to one TraceHub.
  i.e. when one TraceHub device enableds it, the other one must disable it.
  <b>0 = Disable</b>; 1 = Enabled
  **/
  UINT8                      AetEnabled;
  /**
  Indicate if BIOS trace is sent to this TraceHub instance
  <b>0 = Disable</b>; 1 = Enabled
  **/
  UINT8                      BiosTraceSink;
  /**
  Filling reserved for future use & Config block DW alignment
  **/
  UINT8                      Reserved[5];
} TRACE_HUB_CONFIG;

/**
  PCH Trace Hub PreMem Configuration
  Contains Trace Hub settings for PCH side tracing
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;           ///< Config Block Header
  UINT8                      Reserved[4];
  TRACE_HUB_CONFIG           TraceHub;         ///< Trace Hub Config
} PCH_TRACE_HUB_PREMEM_CONFIG;

/**
  Trace Hub PreMem Configuration
  Contains Trace Hub settings
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;                 ///< Config Block Header
  UINT8                      Reserved[4];
  TRACE_HUB_CONFIG           TraceHub[MaxTraceHub];  ///< Trace Hub Config
} TRACE_HUB_PREMEM_CONFIG;

#pragma pack (pop)

#endif
