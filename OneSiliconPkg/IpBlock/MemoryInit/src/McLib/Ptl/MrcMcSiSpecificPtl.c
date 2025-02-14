/** @file
  This file contains code for MC Silicon W/As

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

#include "MrcMcSiSpecific.h"
#include "MrcHalApi.h"
#include "CMcAddress.h"

/**
  This function sets minimum time allowed in self refresh mode.
  @param[in]  MrcData           - Pointer to MRC global data.
**/
VOID
MrcMcLpmodeSelfRefreshLength (
  IN  MrcParameters *const  MrcData
  )
{
  const MrcInput *Inputs;
  INT64          GetSetVal;

  Inputs = &MrcData->Inputs;

  if (Inputs->IsMcMbA0) {
    GetSetVal = 0xF;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSelfRefreshLength, WriteCached | PrintValue, &GetSetVal);
  }
}

/**
  Disable Full Rank Block if TRR is used and DRFM is not enabled.

  @param[in] MrcData      - The MRC general data
  @param[in] Controller   - MC index
  @param[in] Channel      - System Channel index
**/
VOID
MrcFullRankBlockOptimization (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel
  )
{
  MrcInput             *Inputs;
  MrcDdrIoIpVersion    *IpVersion;
  INT64                GetSetVal;

  MC0_CH0_CR_RH_TRR_CONTROL_STRUCT RhTrrControlStruct;

  Inputs              = &MrcData->Inputs;
  IpVersion           = &Inputs->DdrIoIpVersion;

  if (IpVersion->Bits.Derivative == ipDerivativeWcl ||
     (IpVersion->Bits.Derivative == ipDerivativePtl && IpVersion->Bits.Stepping == ipStepB0)) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhDrfmEnable, ReadCached, &GetSetVal);
    RhTrrControlStruct.Bits.DRFM_Enabled = (UINT32) GetSetVal;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhTRRDimmEnable, ReadCached, &GetSetVal);
    RhTrrControlStruct.Bits.TRR_DRFM_Dimm_Enabled = (UINT32) GetSetVal;

    if (RhTrrControlStruct.Bits.TRR_DRFM_Dimm_Enabled && !RhTrrControlStruct.Bits.DRFM_Enabled) {
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMcMntsSpareRw, ReadCached, &GetSetVal);
      GetSetVal |= MRC_BIT3;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMcMntsSpareRw, WriteCached, &GetSetVal);
    }
  }
}

/**
  This function disables independent fast sleep option.

  @param[in]  MrcData  - Pointer to MRC global data.
**/
VOID
MrcMcRfFastSleepDisable (
  IN  MrcParameters *const  MrcData
  )
{
  INT64          GetSetEn;

  GetSetEn = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMcMntsRfFastSleepDisable, WriteCached | PrintValue, &GetSetEn);
}

/**
  This function modifies MC Address Decoder values.

  @param[in]  MrcData  - Pointer to MRC global data.
**/
VOID
MrcModifyMcAddressDecoderValues (
  IN  MrcParameters *const  MrcData
  )
{
  INT64 GetSetVal;

  // Set MAD to 2Rx8, Density = 0xF (max value)
  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch0RankCnt,    WriteCached, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch1RankCnt,    WriteCached, &GetSetVal);
  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch0SdramWidth, WriteCached, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch1SdramWidth, WriteCached, &GetSetVal);
  GetSetVal = 0xF;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch0Density,    WriteCached, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccSubch1Density,    WriteCached, &GetSetVal);
}