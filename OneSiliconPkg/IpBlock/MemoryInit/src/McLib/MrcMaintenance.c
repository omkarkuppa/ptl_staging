/** @file
  This module contains functions to configure and use Memory Controller
  maintenance features.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#include "CMrcTypes.h"
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcMaintenance.h"
#include "MrcGeneral.h"
#include "MrcDdr5.h"
#include "MrcRefreshConfiguration.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcMcSiSpecific.h"

/**
  This function will disable the DQS Oscillator maintenance feature in the Memory Controller.

  @params[in] MrcData - Pointer to MRC global data.
**/
VOID
MrcDisableDqsOscillatorMc (
  IN  MrcParameters * const MrcData
  )
{
  INT64   GetSetDis;
  UINT32  Controller;
  UINT32  Channel;
  BOOLEAN IsLpddr;
  GetSetDis = 0;
  IsLpddr = MrcData->Outputs.IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnWrRetraining,    WriteToCache, &GetSetDis);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnRdRetraining,    WriteToCache, &GetSetDis);
    } // Channel
  } // Controller
  MrcFlushRegisterCachedData (MrcData);
}

VOID
MrcRankOccupancy (
  IN  MrcParameters * const MrcData
  )
{
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  INT64         GetSetVal;
  UINT32        Controller;
  UINT32        Channel;
  UINT32        IpChannel;
  UINT8         RankMask;
  BOOLEAN       IsLpddr;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      RankMask   = ChannelOut->ValidRankBitMask;

      // Set the Rank_occupancy per channel.
      // For LPDDR technologies, the mapping is the following:
      //    Bit     Ch      Rank
      //    0   -   0         0
      //    1   -   0         1
      //    2   -   1         0
      //    3   -   1         1
      //    4   -   0         2
      //    5   -   0         3
      //    6   -   1         2
      //    7   -   1         3
      if (IsLpddr) {
        if (((Channel % 2) == 0)) {
          GetSetVal = (RankMask & 0x3); // Bits 0/1
          GetSetVal |= (UINT64) ((RankMask & 0xC) << 2); // Bits 4/5
          // Only write the register on Channel 1/3 for LP.
          continue;
        } else {
          // Channel 1/3 rank pop start at bits 2
          GetSetVal |= (UINT64) ((RankMask & 0x3) << 2); // Bits 2/3
          GetSetVal |= (UINT64) ((RankMask & 0xC) << 4); // Bits 6/7
          //Set the channel to the previous channel as 1/3 do not have registers.
          IpChannel = Channel - 1;
        }
      } else {
        IpChannel = Channel;
        GetSetVal = ChannelOut->ValidRankBitMask;
      }
      MrcGetSetMcCh (MrcData, Controller, IpChannel, GsmMccRankOccupancy, WriteCached | PrintValue, &GetSetVal);
    } // Channel
  } // Controller
}

/**
  This function configures probeless config register.

  @param[in, out] MrcData - All the MRC global data.
**/
void
MrcProbelessConfig (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput  *Inputs;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;
  UINT32          Controller;
  UINT32          Channel;
  UINT32          IpChannel;
  UINT32          Offset;
  INT64           GetSetVal;
  BOOLEAN         IsLpddr;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;
  MC0_DDRPL_CFG_DTF_STRUCT            ProbelessCfg;
  MC0_CH0_CR_PL_AGENT_CFG_DTF_STRUCT  PlAgentCfg;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsLpddr = Outputs->IsLpddr;
  ExtInputs = Inputs->ExtInputs.Ptr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }

    Offset = OFFSET_CALC_CH (MC0_DDRPL_CFG_DTF_REG, MC1_DDRPL_CFG_DTF_REG, Controller);
    ProbelessCfg.Data = MrcReadCR (MrcData, Offset);

    //If channel 0 is populated, program MCHTrace to 0. Else program to 1.
    Channel = (MrcChannelExist (MrcData, Controller, 0)) ? 0 : 1;
    ProbelessCfg.Bits.MCHTrace = Channel;
    ProbelessCfg.Bits.SCHTrace = Channel;



    //Set DDRPL_Activate based on setup option ProbelessTrace Enable or Disable. Default is Disabled.
    ProbelessCfg.Bits.DDRPL_Activate = ExtInputs->ProbelessTrace;

    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "MCHTrace = 0x%x\nSCHTrace = 0x%x\nDDRPL_Activate = 0x%x\n",
      ProbelessCfg.Bits.MCHTrace,
      ProbelessCfg.Bits.SCHTrace,
      ProbelessCfg.Bits.DDRPL_Activate
      );
    GetSetVal = 1;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteCached, &GetSetVal);

    MrcWriteCR (MrcData, Offset, ProbelessCfg.Data);

    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteCached, &GetSetVal); // Unblock CKE


    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_PL_AGENT_CFG_DTF_REG, MC1_CH0_CR_PL_AGENT_CFG_DTF_REG, Controller, MC0_CH1_CR_PL_AGENT_CFG_DTF_REG, IpChannel);
        PlAgentCfg.Data = MrcReadCR (MrcData, Offset);
        PlAgentCfg.Bits.data_trace_mode = 0;
        PlAgentCfg.Bits.DDRPL_Activate = ExtInputs->ProbelessTrace;
        //If ECC is enabled, set ECC_EN to 1.
        PlAgentCfg.Bits.ECC_EN = Outputs->EccSupport;
        MrcWriteCR (MrcData, Offset, PlAgentCfg.Data);
      }
    } // Channel
  } // Controller
}

/**
  Program DESWIZZLE_SCH0/1 registers for MR4 decoding on LP5

  @param[in] MrcData - The MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.
**/
MrcStatus
MrcMcProgramDeswizzleRegisters (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput      *Inputs;
  MrcDebug      *Debug;
  MrcOutput     *Outputs;
  MrcChannelIn  *ChannelIn;
  MrcChannelOut *ChannelOut;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         Rank;
  UINT8         RankIncrement;
  UINT32        Byte;
  UINT8         Bit;
  INT64         GetSetVal;
  BOOLEAN       OneDpc;
  BOOLEAN       IsLpddr5;
  BOOLEAN       IsDdr5;
  BOOLEAN       Dimm;
  UINT8         DramBit;
  UINT8         FirstRank[MAX_CONTROLLER][MAX_CHANNEL];
#ifdef MRC_DEBUG_PRINT
  UINT32        Offset;
  UINT64        ByteData;
  UINT64        BitData;
#endif


  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  MrcCall   = Inputs->Call.Func;
  IsLpddr5  = Outputs->IsLpddr5;
  IsDdr5    = Outputs->IsDdr5;

  MrcCall->MrcSetMem ((UINT8 *) FirstRank, sizeof (FirstRank), 0);
  GetFirstRank (MrcData, FirstRank);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel))) {
        continue;
      }
      ChannelIn  = &Inputs->Controller[Controller].Channel[Channel];
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      OneDpc = (ChannelOut->DimmCount == 1) ? TRUE : FALSE;

      // if DDR5 1DPC increment each rank in DIMM
      if (IsDdr5 && OneDpc) {
        RankIncrement = 1;
      } else {
        // If LP or DDR5 2DPC only increment through the first rank of each DIMM since Deswizzle
        // is duplicated on all ranks in the same DIMM
        RankIncrement = 2;
      }

      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank += RankIncrement) {
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }

        // If DDR5 1DPC Rank 1 route Deswizzle to Rank3 (SCH1_BIT_DESWIZZLE_REG)
        if (IsDdr5 && OneDpc && (Rank == 1)) {
          Rank = 3;
        }

        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          if (IsDdr5 && (Byte == MRC_DDR5_ECC_BYTE)) {
            continue;   // DDR5 ECC byte does not have DESWIZZLE register in the MC, it's always routed 1:1
          }
          GetSetVal = Byte;
          // DqsMapCpu2Dram maps CPU bytes to DRAM, we need to find the reverse mapping here
          MrcGetSetStrobe (MrcData, Controller, Channel, Rank, ChannelIn->DqsMapCpu2Dram[Dimm][Byte], GsmMccDeswizzleByte, WriteToCache | PrintValue, &GetSetVal);

          if (IsLpddr5) {  // Bit-level deswizzle is not used in DDR5, keeping reset values
            for (Bit = 0; Bit < MAX_BITS; Bit++) {
              // DqMapCpu2Dram maps CPU bits to DRAM, we need to find the reverse mapping here
              GetSetVal = Bit;
              DramBit = ChannelIn->DqMapCpu2Dram[Rank][Byte][Bit] % 8;
              if (DramBit < 5) {  // MC needs to know how to deswizzle only DRAM bits [0..4] for temperature read
                MrcGetSetBit (MrcData, Controller, Channel, Rank, ChannelIn->DqsMapCpu2Dram[Dimm][Byte], DramBit, GsmMccDeswizzleBit, WriteToCache | PrintValue, &GetSetVal);
              }
            }
          }
        } // Byte
      } // Rank
    } // Channel
  } // Controller
  MrcFlushRegisterCachedData (MrcData);


#ifdef MRC_DEBUG_PRINT
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL_SHARE_REGS; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BYTE_DESWIZZLE_REG, Channel);
        ByteData = MrcReadCR (MrcData, Offset);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BIT_DESWIZZLE_REG, Channel);
        BitData = MrcReadCR (MrcData, Offset);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u.Ch%u DESWIZZLE_SCH0_BYTE = 0x%llx, DESWIZZLE_SCH0_BIT = 0x%llx\n", Controller, Channel, ByteData, BitData);

        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BYTE_DESWIZZLE_REG, Channel);
        ByteData = MrcReadCR (MrcData, Offset);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BIT_DESWIZZLE_REG, Channel);
        BitData = MrcReadCR (MrcData, Offset);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u.Ch%u DESWIZZLE_SCH1_BYTE = 0x%llx, DESWIZZLE_SCH1_BIT = 0x%llx\n", Controller, Channel, ByteData, BitData);
      }
    }
  }
#endif // MRC_DEBUG_PRINT

  // Read some MR values using MC after MC deswizzle registers are programmed
  ShowLpddrInfo (MrcData);

  return mrcSuccess;
}

/**
  This function provides the initial configuration of the Memory Controller's
  maintenance services.  Some items are disabled during MRC training, and will need
  to be configured at the end of MRC.

  @params[in] MrcData - Pointer to MRC global data.
**/
VOID
MrcMaintenanceConfig (
  IN  MrcParameters * const MrcData
  )
{
  // Configure Rank Occupancy:
  MrcRankOccupancy (MrcData);
  // Disable Dqs Oscillator at the start of MRC.
  MrcDisableDqsOscillatorMc (MrcData);
  // Configure probeless.
  MrcProbelessConfig (MrcData);

  MrcMcRfFastSleepDisable (MrcData);
}
