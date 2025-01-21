/** @file
  This module includes the power metering definitions.

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
#ifndef _MrcPowerMetering_h_
#define _MrcPowerMetering_h_
#pragma pack(push, 1)

typedef struct {
  UINT32  ReadPwr;
  UINT32  WritePwr;
  UINT32  IdlePwr;
  UINT32  CkePwr;
} MrcPowerWeightTypes;

typedef struct {
  MrcFrequency Freq;
  MrcPowerWeightTypes PowerValue;
} MrcPowerWeightMapping;

#define MAX_FREQ  7

/**
  This function configures the Power Metering weights.

  @param[in]  MrcData     - Include all MRC global data.
  @param[out] ReadPower   - DDRPHY Read energy weight.
  @param[out] WritePower  - DDRPHY Write energy weight.
  @param[out] IdlePower   - DDRPHY Idle energy weight.
  @param[out] CkePower    - DDRPHY Cke energy weight.

  @retval Nothing.
**/
VOID
MrcPowerMeteringConfig (
  IN MrcParameters *const MrcData
  );


#pragma pack(pop)
#endif // _MrcPowerModes_h_
