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
  This function sets MC IP Version, Derivative, Segment, Stepping based on MC0_MC_IP_VERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval mrcSuccess if the IP Segment and IP Version is valid.  Otherwise mrcFail
**/
MrcStatus
MrcSetupMcIpInfo (
  OUT MrcParameters *const MrcData
  )
{
  MrcInput                     *Inputs;
  MrcOutput                    *Outputs;
  MrcDebug                     *Debug;
  MrcStatus                    Status;
  UINT32                       Offset;
  MC0_MC_IP_VERSION_STRUCT     McIpVersion;
  MrcMcIpVersion               *IpVersion;
  const char                   *StepStr;

  Inputs          = &MrcData->Inputs;
  Outputs         = &MrcData->Outputs;
  Debug           = &MrcData->Outputs.Debug;
  IpVersion       = &Inputs->McIpVersion;
  Status          = mrcSuccess;
  StepStr         = NULL;

  Offset = OFFSET_CALC_CH (MC0_MC_IP_VERSION_REG, MC1_MC_IP_VERSION_REG, Outputs->FirstPopController);
  McIpVersion.Data = MrcReadCR (MrcData, Offset);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC_IP_VERSION: 0x%x\n", McIpVersion.Data);
  MRC_DEBUG_ASSERT (McIpVersion.Data != 0, Debug, "McIpVersion is 0, this is an improbable value");
  IpVersion->Bits.Version    = (UINT8) McIpVersion.Bits.IP_VERSION;
  IpVersion->Bits.Derivative = (UINT8) McIpVersion.Bits.IP_DERIVATIVE;
  IpVersion->Bits.Segment    = (UINT8) McIpVersion.Bits.IP_SEGMENT;
  IpVersion->Bits.Stepping   = (UINT8) McIpVersion.Bits.IP_SUBVERSION;

  if (IpVersion->Bits.Stepping > ipStepLast) {
    IpVersion->Bits.Stepping = ipStepLast;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s MC IP: %s = %d\n", gWarnString, gMrcIpSteppingStr, ipStepLast);
  }

  if (IpVersion->Bits.Version != ipVerMcLnlPtl) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s MC IP: %s\n", gWarnString, gMrcIpVersionStr);
    Status  = mrcFail;
  }

  switch (IpVersion->Bits.Segment) {
    case IpSegmentDesktop:
      Inputs->IsMcDtHalo = TRUE;
      switch (IpVersion->Bits.Stepping) {
        case ipStepA0:
        default:
          Inputs->IsMcDtA0 = TRUE;
          StepStr = "A0";
          break;
      }
      break;

    case MC_PTLPUH_IP_SEGMENT:
      Inputs->IsMcUlxUlt = TRUE;
      switch (IpVersion->Bits.Stepping) {
        case ipStepB0:
          Inputs->IsMcMbB0 = TRUE;
          StepStr = "B0";
          break;

        default:
        case ipStepA0:
          Inputs->IsMcMbA0 = TRUE;
          StepStr = "A0";

          break;
      }
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s MC IP: %s\n", gWarnString, gMrcIpSegmentStr);
      Status  = mrcFail;
      break;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "\nMC IP Version: %d.%d.%d %s\n",
    IpVersion->Bits.Derivative,
    IpVersion->Bits.Segment,
    IpVersion->Bits.Version,
    (StepStr == NULL) ? "Unknown" : StepStr
 );

  return Status;
}

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

/**
  This function configures the Power Metering weights.

  @param[in out]  MrValue     - MR value.
  @param[in]      BusValue     - The filed bus value to be programmed.

  @retval Nothing.
**/
VOID
MrcSetBusCmdType (
  IN UINT32         *MrValue,
  IN UINT32         BusValue
  )
{
  MC0_CH0_CR_DDR_MR_COMMAND_STRUCT MrCommand;
  MrCommand.Data = *MrValue;
  MrCommand.Bits.SPID_cmd_type = BusValue;
  *MrValue = MrCommand.Data;
}

