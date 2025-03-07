/** @file
  This file contains project independent code related to initializing and
  configuring the DDRIO CCC Partition.

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

#include "MrcDdrIoApi.h"  // for prototypes
#include "MrcCommon.h"  // for MrcChannelExist()
#include "MrcHalRegisterAccess.h"  // for MrcGetSetLimits()
#include "MrcDdrIoPiSerializer.h" // for DCC PI Serializer support functions
#include "MrcDdrIoComp.h"

static const UINT8 CccDdr5IlMap[MRC_CCC_NUM] = {
  0,  // CCC0
  2,  // CCC1
  1,  // CCC2
  3,  // CCC3
  4,  // CCC4
  6,  // CCC5
  5,  // CCC6
  7,  // CCC7
};

/**
  This function translates HW CCC Partition instance to RAL instance based on interleave configuration.

  @param[in] MrcData     - MRC global data.
  @param[in] CccInstance - CCC instance to be translated.

  @retval Translated CCC instance.
**/
UINT8
MrcTranslateCccInstance (
  IN MrcParameters *const MrcData,
  IN UINT8          const CccInstance
  )
{
  MrcInput *Inputs;
  UINT8    CccTranslatedInstance;

  Inputs  = &MrcData->Inputs;

  if (Inputs->ExtInputs.Ptr->CccPinsInterleaved) {
    CccTranslatedInstance = CccDdr5IlMap[CccInstance];
  } else {
    CccTranslatedInstance = CccInstance;
  }
  return CccTranslatedInstance;
}

/**
  Get the max CMD Groups per channel associated with the current memory technology

  @param[in] MrcData  - Pointer to global MRC data.

  @return The maximum number of CMD Groups per channel for the current memory technology
**/
UINT8
MrcGetCmdGroupMax (
  MrcParameters *const MrcData
  )
{
  return (MrcData->Outputs.IsDdr5) ? MRC_DDR5_CMD_GRP_MAX: 1;
}

/**
  This function decodes Controller/Channel/Rank to a group index.
  Desktop has two CCC groups for CS in MC0.Ch0.  Rank2/3 are assigned to group 1
  while Rank0/1 is group 0.  All other combinations are a single group.

  @param[in] MrcData    - Pointer to global MRC data.
  @param[in] Controller - 0-based index
  @param[in] Channel    - 0-based index
  @param[in] Rank       - 0-based index

  @retval CccChCsPiGroup - Group Index value.
**/
CccChCsPiGroup
MrcGetCsRankPiGroup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank
  )
{
  CccChCsPiGroup GroupIdx;

  if (MrcData->Inputs.IsDdrIoDtHalo && (Controller == 0) && (Channel == 0) && (Rank >= MAX_RANK_IN_DIMM)) {
    GroupIdx = CccChCsPiGroup1;
  } else {
    GroupIdx = CccChCsPiGroup0;
  }

  return GroupIdx;
}

/**
  This function takes the input CS timings (CtlPiCode[]) for multiple ranks on a
  single channel and normalizes the timing between a shared PHY CCC Channel timing
  register field (CtlGrpPi) and a per-rank offset register field (CsPerBitCcc).

  The registers are updated with the normalized values before returning.

  @param[in]  MrcData      - Include all MRC global data.
  @param[in]  Controller   - 0-based index instance to select.
  @param[in]  Channel      - 0-based index instance to select.
  @param[in]  RankMask     - Bit mask of ranks sharing CCC Channel
  @param[in]  CtlPiCode    - The effective CS PI code to program for each rank

  @return MrcStatus - mrcFail if the per-rank offset is larger than the per-pin register max
                    - mrcSuccess in all other cases
**/
MrcStatus
MrcCsPerPinNormalizeRanks (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               RankMask,
  IN UINT16               CtlPiCode[MAX_RANK_IN_CHANNEL]
  )
{
  UINT32             MinPiCode;
  UINT32             MaxPiCode;
  INT64              PerBitMax;
  UINT32             Rank;
  UINT32             CsGrpRankMask;
  UINT16             PiCode;
  INT64              CsPiCodeShared;
  INT64              CsPiCodePerPin;
  CccChCsPiGroup     CsGrp;
  CccChCsPiGroup     RankCsGrp;
  MrcOutput          *Outputs;
  INT64              TriCaSave;

  if (RankMask == 0) {
    // Nothing to do
    return mrcSuccess;
  }

  Outputs = &MrcData->Outputs;

  MrcGetSetLimits (MrcData, CsPerBitCcc, MRC_IGNORE_ARG, NULL, &PerBitMax, NULL);
  TriCaSave = MrcTristateCa (MrcData, MRC_ENABLE, 0);

  for (CsGrp = CccChCsPiGroup0; CsGrp < CccChCsPiGroupMax; CsGrp++) {
    // Calculate the rank mask for the current CsGrp
    CsGrpRankMask = 0;
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      if ((RankMask & (MRC_BIT0 << Rank)) == 0) {
        continue;
      }
      // This needs to be reviewed for each pin layout to ensure we map the group<->rank correctly.
      RankCsGrp = MrcGetCsRankPiGroup (MrcData, Controller, Channel, Rank);
      if (RankCsGrp == CsGrp){
        CsGrpRankMask |= (MRC_BIT0 << Rank);
      }
    }
    if (CsGrpRankMask == 0) {
      // Skip this CS group if no ranks are associated with it
      // This will be true for CsGrp = CccChCsPiGroup1 in all cases except Desktop DDR5 MC0.CH0
      continue;
    }
    // Initialize Min \ Max values to limit values
    MinPiCode = MRC_UINT32_MAX;
    MaxPiCode = MRC_UINT_MIN;
    // Find the Min\Max PiCode ranges
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      if ((CsGrpRankMask & (MRC_BIT0 << Rank)) == 0) {
        continue;
      }
      PiCode = CtlPiCode[Rank];
      MinPiCode = MIN (MinPiCode, PiCode);
      MaxPiCode = MAX (MaxPiCode, PiCode);
    }
    // The difference between the min and max per-rank PI Code must not exceed the per-pin register max value
    if ((MaxPiCode - MinPiCode) > PerBitMax) {
      MRC_DEBUG_ASSERT (FALSE, &Outputs->Debug, "CS Per-Pin delta > %lld", PerBitMax);
      MrcTristateCa (MrcData, MRC_DISABLE, TriCaSave);
      return mrcParamSaturation;
    }
    // Normalize by distributing the CS timing between the shared CCC delay and the Per-Pin CCC offset
    CsPiCodeShared = MinPiCode;
    // Only need to index CtlGrpPi using the first rank sharing the CCC Channel
    Rank = MrcLowBitSet32(CsGrpRankMask);
    MrcGetSetCcc (MrcData, Controller, Channel, Rank, MRC_IGNORE_ARG, CtlGrpPi, WriteToCache, &CsPiCodeShared);
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      if ((CsGrpRankMask & (MRC_BIT0 << Rank)) == 0) {
        continue;
      }
      CsPiCodePerPin = CtlPiCode[Rank] - MinPiCode;
      MrcGetSetCccLane (MrcData, Controller, Channel, MRC_IGNORE_ARG, Rank, CsPerBitCcc, WriteToCache, &CsPiCodePerPin);
    }
  }
  MrcFlushRegisterCachedData (MrcData);

  // Disable CA tristate after changing PIs
  MrcTristateCa (MrcData, MRC_DISABLE, TriCaSave);

  return mrcSuccess;
}

/**
  This function uses the MrcData->Outputs.Controller[].CmdTiming[].CtlPiCode[]
  host structure to finalize the CS timings on all populated ranks. The timings
  are normalized to distribute the per-rank timings between the shared CCC
  channel register field and the per-pin offset field.

  @param[in]  MrcData      - Include all MRC global data.

  @return MrcStatus - mrcFail if the per-rank offset is larger than the per-pin register max
                    - mrcSuccess in all other cases
**/
MrcStatus
MrcCsPerPinNormalize (
  IN MrcParameters *const MrcData
  )
{
  UINT32    Controller;
  UINT32    Channel;
  UINT32    ValidRankBitMask;
  MrcOutput *Outputs;
  MrcStatus Status;
  BOOLEAN   IsFailed;
  MrcIntCmdTimingOut *IntCmdTiming;

  Outputs  = &MrcData->Outputs;
  IsFailed = FALSE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      IntCmdTiming = &Outputs->Controller[Controller].CmdTiming[Channel];
      ValidRankBitMask = Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;
      Status = MrcCsPerPinNormalizeRanks (MrcData, Controller, Channel, ValidRankBitMask, IntCmdTiming->CtlPiCode);
      IsFailed = IsFailed || (Status != mrcSuccess);
    }
  }
  return IsFailed ? mrcFail : mrcSuccess;
}
