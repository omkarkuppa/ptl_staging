/** @file
  This API provides an interface to abstract out memory protocol differences.

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
#ifndef MRC_LPDDR5_SETTINGS_H_
#define MRC_LPDDR5_SETTINGS_H_

#include "CMrcApi.h"  // Indirectly provides "IN" and "OUT"
// AUTO-GENERATED LP5 TABLE DECLARATIONS START
// AUTO-GENERATED CODE
typedef struct {
  UINT8  Dfeq;
  UINT16 PdDrvStr;
  UINT16 SocOdt;
  INT8   PreEmpDn;
  INT8   PreEmpUp;
  INT8   WckDcaWr;
  INT8   WckDcaRd;
  UINT16 RttWr;
  UINT16 RttCa;
  UINT16 RttNT;
} NnFlexLpddr5Params;

extern const NnFlexLpddr5Params NnFlexInitialSettingsLpddr5[MaxNnFlexDramPart];
// AUTO-GENERATED LP5 TABLE DECLARATIONS END

/**
  This function selects the ODT table according to the board type.

  @param[in] MrcData  - Include all the MRC general data.
  @param[in] Dimm     - selected DIMM.
  @param[in] OdtIndex - selected ODT index.

  @return TOdtValueLpddr * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueLpddr *
SelectTableLpddr5 (
  IN MrcParameters *const MrcData,
  IN const UINT32         Dimm,
  IN const TOdtIndex      OdtIndex
  );

/**
  This function returns DRAM type for NN Flex per device usage.

  @param[in] MrcData - Pointer to global MRC data.

  @returns NnFlexDramPart - table entry containing configuration values defined for that specific combination
**/
NnFlexDramPart
Lpddr5NnFlexGetDramType (
  IN  MrcParameters* MrcData
  );
#endif // MRC_LPDDR5_SETTINGS_H_
