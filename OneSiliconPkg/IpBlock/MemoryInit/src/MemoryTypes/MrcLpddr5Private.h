/** @file
  Definition of private LPDDR5 Specific functions, constants,
  defines, and data structures private to MrcLpddr5.c

  In the future, the functions declared here should be made
  static.

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

@par Specification
**/
#ifndef MRC_LPDDR_5_PRIVATE_H_
#define MRC_LPDDR_5_PRIVATE_H_

#include "MrcCommon.h"  // for MrcMarginTypes


// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcLpddr5Private)

/**
  This function converts from MRC training index to DRAM Vref encoding:
    Vref [10:127] - Offset [-58:58] :: LP5 (15% - 73.5%) * 500mV.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - Training index to convert.
  @param[out] Vref    - Pointer to return the Vref MR encoding.

  @retval mrcSuccess              Input parameters are valid (LPDDR5 Spec).
  @retval mrcWrongInputParameter  Input parameters are invalid (LPDDR5 Spec).
**/
MrcStatus
OffsetToMRencoding (
  IN  MrcParameters *const  MrcData,
  IN  INT32                 Offset,
  OUT UINT8                 *Vref
  );

/**
  This function returns the DFE Setting for MR24.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] Dfeql     - DFE Quantity for Lower Byte.
  @param[out] Dfequ     - DFE Quantity for Upper Byte.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcLp5GetDFE (
  IN  MrcParameters  *MrcData,
  OUT UINT8          *Dfeql,
  OUT UINT8          *Dfequ
  );

/**
  This function checks that the Vref requested is within the spec defined range for LPDDR5.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[in]  Vref      - Requested Vref in mV
  @param[in]  VrefType  - CmdV or WrV

  @param  UINT16 - The requested Vref or a clamped Vref value.
**/
UINT16
CheckVrefLimits (
  IN  MrcParameters   *MrcData,
  IN  UINT16          Vref,
  IN  MrcMarginTypes  VrefType
  );

/**
  This function converts the Vref number in millivolts to the MRC training offset.
  LPDDR5 has one flat range from 15% of VDDq to 75% of VDDq.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  VrefMv  - Request Vref in millivolts.
  @param[out] Offset  - Pointer to return training index.

  @retval mrcSuccess              Input parameters are valid.
  @retval mrcWrongInputParameter  Input parameters are invalid.
**/
MrcStatus
VrefToOffset (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                VrefMv,
  OUT INT32                 *Offset
  );

/**
  This function converts from the integer defined Write Latency to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Write Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeWriteLatencyLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  );

/**
  This function converts from the integer defined Write Recovery to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Write Recovery value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeWriteRecoveryLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  );

#endif // MRC_LPDDR_5_PRIVATE_H_
