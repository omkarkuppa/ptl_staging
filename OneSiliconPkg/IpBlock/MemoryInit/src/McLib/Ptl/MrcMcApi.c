/** @file
  This file contains code related to initializing and configuring the MC.

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
**/

#include "MrcMcApi.h"
#include "MrcCommon.h"


/**
  This function sets workpoint data for MC.

  @param[in, out] MrcData - MRC global data.
**/
VOID
SetWorkPointDataForMc (
  IN OUT MrcParameters *const MrcData
  )
{
}

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
  )
{
  MrcDebug           *Debug;
  UINT32             WritePower;
  UINT32             ReadPower;
  UINT32             CkePower;
  UINT32             IdlePower;

  DDRPHY_MISC_SAUG_CR_DDR_PHY_VDD2_POWER_STRUCT DdrPhyPower;

  Debug      = &MrcData->Outputs.Debug;

  //DDRPHY weights
  MrcGetDdrPhyWeights (MrcData, &ReadPower, &WritePower, &IdlePower, &CkePower);
  MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "DDRPHY Read Power = %u, Write Power = %u, Idle Power = %u, CKE power = %u\n", ReadPower, WritePower, IdlePower, CkePower);
  DdrPhyPower.Bits.ReadpJ = ReadPower;
  DdrPhyPower.Bits.WritepJ = WritePower;
  DdrPhyPower.Bits.IdlemW = IdlePower;
  DdrPhyPower.Bits.CKEmW = CkePower;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_DDR_PHY_VDD2_POWER_REG, DdrPhyPower.Data);
  return;
}