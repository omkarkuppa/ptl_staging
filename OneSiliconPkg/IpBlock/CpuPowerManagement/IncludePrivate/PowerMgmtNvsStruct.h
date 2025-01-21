/** @file
  This file contains CPU Gnvs Struct specific to processor

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _POWER_MGMT_NVS_STRUCT_H_
#define _POWER_MGMT_NVS_STRUCT_H_

#include <CpuNvsAreaDef.h>
//
// Processor Power Management GlobalNvs Revisions
//
#define CPU_NVS_AREA_REVISION 1


typedef struct _CPU_NVS_AREA_CONFIG {
  CPU_NVS_AREA *Area;
} CPU_NVS_AREA_CONFIG;

typedef struct _FVID_HEADER {
  UINT32 Stepping;    ///< Matches value returned by CPUID function 1
  UINT16 MaxBusRatio; ///< Matches BUS_RATIO_MAX field in PERF_STS_MSR
  UINT16 EistStates;  ///< Number of states of FVID (N)
} FVID_HEADER;

typedef struct _FVID_STATE {
  UINT32 State;           ///< State Number (0 - N-1)
  UINT16 BusRatio;        ///< BUS_RATIO_SEL value to be written to PERF_CTL
  UINT32 Power;           ///< Typical power consumed by CPU in this state
  UINT32 Limit16State;    ///< State Number (0 - N-1) with limit 16 P-states
  UINT16 Limit16BusRatio; ///< BUS_RATIO_SEL value to be written to PERF_CTL with limit 16 P-states
  UINT32 Limit16Power;    ///< Typical power consumed by CPU in this state with limit 16 P-states
} FVID_STATE;

typedef union _FVID_TABLE {
  FVID_HEADER FvidHeader;
  FVID_STATE  FvidState;
  UINT64      FvidData;
} FVID_TABLE;

#endif
