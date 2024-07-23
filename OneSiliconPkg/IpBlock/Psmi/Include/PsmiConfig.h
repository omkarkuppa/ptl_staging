/** @file
  Header file for PSMI  Config

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PSMI_CONFIG_PTL_CDIE_H_
#define _PSMI_CONFIG_PTL_CDIE_H_

#include <Base.h>

#pragma pack (push,1)

//
// PSMI Error codes
//
#define PSMI_NO_RESERV_REQUESTED     0
#define PSMI_ALLOCATION_SUCCESS      1
#define ERROR_OUT_OF_RESOURCES       2
#define ERROR_CACHE_TYPE             3
#define ERROR_MTRR_SHORTAGE          4
#define ERROR_NO_ABOVE_4G_MEMORY     5
//
// PSMI Max Trace/Cache Type
//
#define MAX_TRACE_REGION             5
#define MAX_TRACE_CACHE_TYPE         2
//
// PSMI MSR Max Trace Type
//
#define MSR_MAX_TRACE_REGION         3
#define TRACE_REGION_INDEX_0         0
#define TRACE_REGION_INDEX_1         1
#define TRACE_REGION_INDEX_2         2

#define PSMI_HANDLER_MEMORY_OFFSET_1000  0x1000

//
// PSMI Trace Memory cache types
//
typedef enum {
  RegionDonotCare      = 0,
  RegionWriteBack      = 1,
} PSMI_TRACE_CACHE_TYPE;

typedef enum {
  PsmiMsrRequest  = 0,
  PsmiMsrResponse
} PSMI_MSG_TYPE;

typedef enum {
  PsmiMemAbove4G  = 0,
  PsmiMemBelow4G
} PSMI_MEM_RESERVATION;

//
// PSMI Input Scratchpad Register
//
typedef union {
  UINT32 RegValue;
  struct {
    UINT32 PsmiHandler:2;                  /// 1:0   - PSMI Handler Size
    UINT32 TraceRegion0Size:4;             /// 5:2   - Trace Region 0 Size
    UINT32 TraceRegion0CacheType:1;        /// 6     - Trace Region 0 Cache Type
    UINT32 TraceRegion0RangeCompatible:1;  /// 7     - Trace Region 0 Range Compatibility
    UINT32 TraceRegion1Size:4;             /// 11:8  - Trace Region 1 Size
    UINT32 TraceRegion1CacheType:1;        /// 12    - Trace Region 1 Cache Type
    UINT32 TraceRegion1RangeCompatible:1;  /// 13    - Trace Region 1 Range Compatibility
    UINT32 TraceRegion2Size:4;             /// 17:14 - Trace Region 2 Size
    UINT32 TraceRegion2CacheType:1;        /// 18    - Trace Region 2 Cache Type
    UINT32 TraceRegion2RangeCompatible:1;  /// 19    - Trace Region 2 Range Compatibility
    UINT32 TraceRegion3Size:4;             /// 23:20 - Trace Region 3 Size
    UINT32 TraceRegion3CacheType:1;        /// 24    - Trace Region 3 Cache Type
    UINT32 TraceRegion3RangeCompatible:1;  /// 25    - Trace Region 3 Range Compatibility
    UINT32 TraceRegion4Size:4;             /// 29:26 - Trace Region 4 Size
    UINT32 TraceRegion4CacheType:1;        /// 30    - Trace Region 4 Cache Type
    UINT32 TraceRegion4RangeCompatible:1;  /// 31    - Trace Region 4 Range Compatibility
  } Bits;
} PSMI_INPUT_SCRPD1_REGISTER;

//
// PSMI Output Scratchpad Register
//
typedef union {
  UINT32 RegValue;
  struct {
    UINT32 PsmiHandlerResultCode:3;  /// 2:0   - Handler Error Code
    UINT32 TraceRegion0ResultCode:3; /// 5:3   - Trace Region 0 Error Code
    UINT32 TraceRegion1ResultCode:3; /// 8:6   - Trace Region 1 Error Code
    UINT32 TraceRegion2ResultCode:3; /// 11:9  - Trace Region 2 Error Code
    UINT32 TraceRegion3ResultCode:3; /// 14:12 - Trace Region 3 Error Code
    UINT32 TraceRegion4ResultCode:3; /// 17:15 - Trace Region 4 Error Code
    UINT32 PsmiHandlerBase:14;       /// 31:18 - PSMI Handler Base
  } Bits;
} PSMI_OUTPUT_SCRPD1_REGISTER;

//
// PSMI Handler Space Offset 0x1000 Decode
//
typedef struct {
  UINT64 PsmiHandlerBaseAddress;  /// PSMI Handler BaseAddress
  UINT64 TraceRegion0BaseAddress; /// Trace Region 0 BaseAddress
  UINT64 TraceRegion1BaseAddress; /// Trace Region 1 BaseAddress
  UINT64 TraceRegion2BaseAddress; /// Trace Region 2 BaseAddress
  UINT64 TraceRegion3BaseAddress; /// Trace Region 3 BaseAddress
  UINT64 TraceRegion4BaseAddress; /// Trace Region 4 BaseAddress
  UINT32 PsmiInputRegValue;       /// PSMI Request Data
} PSMI_HANDLER_SPACE_OFFSET;

//
// PSMI Trace Alignment types
//
typedef enum {
  Psmi2MBAlignment        = 0,
  PsmiNaturalAlignment    = 1,
} PSMI_ALIGNEMNT_TYPE;

//
// PSMI Input Scratchpad Register
//
typedef union {
  UINT64 RegValue;
  struct {
    UINT64 PsmiHandlerSize:5;              // 4:0   - PSMI Handler Size
    UINT64 Rsvd1:1;                        // 5:5   - Reserved
    UINT64 HandlerBelow4G:1;               // 6:6   - Handler Below 4GB
    UINT64 HandlerAlignment:1;             // 7:7   - Handler Alignment
    UINT64 TraceRegion0Size:5;             // 12:8  - Trace Region 0 Size
    UINT64 Rsvd2:2;                        // 14:13 - Reserved
    UINT64 TraceRegion0Alignment:1;        // 15:15 - Trace Region 0 Alignment
    UINT64 TraceRegion1Size:5;             // 20:16 - Trace Region 1 Size
    UINT64 Rsvd3:2;                        // 22:21 - Reserved
    UINT64 TraceRegion1Alignment:1;        // 23:23 - Trace Region 1 Alignment
    UINT64 TraceRegion2Size:5;             // 28:24 - Trace Region 2 Size
    UINT64 Rsvd4:2;                        // 30:29 - Reserved
    UINT64 TraceRegion2Alignment:1;        // 31:31 - Trace Region 2 Alignment
    UINT64 Rsvd5:28;                       // 59:32 - Reserved
    UINT64 Version:3;                      // 62:60 - Version of PSMI memory reservation interface
    UINT64 MessageType:1;                  // 63:63 - Message Type
  } Bits;
} PSMI_REQUEST_MSR_REGISTER;

//
// PSMI Output Scratchpad Register
//
typedef union {
  UINT64 RegValue;
  struct {
    UINT64 PsmiHandlerResultCode:3;  /// 2:0   - Handler Error Code
    UINT64 Rsvd1:1;                  /// 3     - Reserved
    UINT64 TraceRegion0ResultCode:3; /// 6:4   - Trace Region 0 Error Code
    UINT64 Rsvd2:1;                  /// 7     - Reserved
    UINT64 TraceRegion1ResultCode:3; /// 10:8  - Trace Region 1 Error Code
    UINT64 Rsvd3:1;                  /// 11    - Reserved
    UINT64 TraceRegion2ResultCode:3; /// 14:12 - Trace Region 2 Error Code
    UINT64 Rsvd4:6;                  /// 20    - Reserved
    UINT64 PsmiHandlerBase:39;       /// 59:21 - PSMI Handler Base
    UINT64 Version:3;                /// 62:60 - Version of PSMI memory reservation interface
    UINT64 MessageType:1;            /// 63    - Message Type
  } Bits;
} PSMI_RESPONSE_MSR_REGISTER;

//
// PSMI Handler Space Offset 0x1000 Decode
//
typedef struct {
  UINT64 PsmiHandlerRequest;      /// PSMI Handler Request
  UINT64 PsmiHandlerResponse;     /// PSMI Handler Response
  UINT64 TraceRegion0BaseAddress; /// Trace Region 0 BaseAddress
  UINT64 TraceRegion1BaseAddress; /// Trace Region 1 BaseAddress
  UINT64 TraceRegion2BaseAddress; /// Trace Region 2 BaseAddress
} PSMI_MSR_HANDLER_SPACE_OFFSET;

#pragma pack (pop)

#endif
