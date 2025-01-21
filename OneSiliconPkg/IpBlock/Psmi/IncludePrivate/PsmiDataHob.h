/** @file
  The GUID definition for PsmiDataHob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _PSMI_DATA_HOB_H_
#define _PSMI_DATA_HOB_H_

#include <Base.h>
#include <PsmiConfig.h>

extern EFI_GUID gPsmiDataHobGuid;
#pragma pack (push,1)

/**
  This structure contains the PSMI Ver1 Trace Region Info
**/
typedef struct {
  UINT64   TraceRegionBase;
  UINT32   Size;
  UINT8    CacheType;
  UINT8    RangeCompatible;
  UINT8    ErrCode;
} PSMI_TRACE_REGION_INFO_VER1;

/**
  This structure will be used for passing PSMI trace Info to MRC
**/
typedef struct {
  UINT64   TotalSize;
  UINT8    CacheType;
} PSMI_MRC_INPUTS;

/**
  This structure contains the PSMI Handler Info
**/
typedef struct {
  UINT8    HandlerSize;       ///< Handler Size
  UINT32   HandlerBase;       ///< Handler Info
  UINT8    HandlerErrCode;    ///< Error Code for  Handler
  UINT8    Rsvd[2];
} PSMI_HANDLER_INFO;

//
// This HOB structure contains Handler , Trace Region Info , PSMI Ver1 MRC Input Info.
//
typedef struct {
  PSMI_HANDLER_INFO           HandlerInfo;                        ///< Handler Info
  PSMI_TRACE_REGION_INFO_VER1 TraceRegionInfo[MAX_TRACE_REGION];  ///< Trace Region Info
  PSMI_MRC_INPUTS             PsmiMrcParam[MAX_TRACE_CACHE_TYPE]; ///< PSMI Trace MRC Input
  PSMI_MRC_INPUTS             PsmiMrcRegionParam[MAX_TRACE_REGION]; ///< PSMI Trace MRC Input
  UINT32                      PsmiInputRegValue;                  ///< PSMI Scratchpad Input value
  UINT8                       ContiguousTraceRegion;              ///< ContiguousTraceRegion Enable Disable
} PSMI_CONFIG_VER1;

///
/// PSMI Ver1 Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;    ///< GUID Hob type structure for gPsmiDataHobGuid
  PSMI_CONFIG_VER1         PsmiConfig;        ///< PSMI Handler/Trace/PSMI MRC Info
  UINT8                    Rsvd1[4];          ///< Reserved for future use.
} PSMI_DATA_HOB_VER1;

/**
  This structure contains the PSMI Ver2 Trace Region Info
**/
typedef struct {
  UINT64   TraceRegionBase;
  UINT32   TraceRegionSize;
  UINT8    TraceRegionAlignment;
  UINT8    TraceRegionErrCode;
  UINT64   TraceRegionTotalSize;
} PSMI_TRACE_REGION_INFO_VER2;

/**
  This structure contains the PSMI Ver2 Handler Info
**/
typedef struct {
  UINT8    HandlerSize;       ///< Handler Size
  UINT8    HandlerBelow4G;
  UINT8    HandlerAlignment;
  UINT64   HandlerBase;       ///< Handler Info
  UINT8    HandlerErrCode;    ///< Error Code for  Handler
  UINT64   HandlerTotalSize;
} PSMI_HANDLER_INFO_VER2;

//
// This HOB structure contains Handler, Trace Region Info, PSMI Ver2 Info.
//
typedef struct {
  PSMI_HANDLER_INFO_VER2      HandlerInfo;                            ///< Handler Info
  PSMI_TRACE_REGION_INFO_VER2 TraceRegionInfo[MSR_MAX_TRACE_REGION];  ///< Trace Region Info
  UINT64                      PsmiHandlerRequest;                     ///< PSMI MSR Request value
  UINT64                      PsmiHandlerResponse;                    ///< PSMI MSR Response value
} PSMI_CONFIG_VER2;

///
/// PSMI Ver2 Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;    ///< GUID Hob type structure for gPsmiDataHobGuid
  PSMI_CONFIG_VER2         PsmiConfig;        ///< PSMI Handler/Trace
  UINT8                    Rsvd1[4];          ///< Reserved for future use.
} PSMI_DATA_HOB_VER2;
#pragma pack (pop)
#endif
