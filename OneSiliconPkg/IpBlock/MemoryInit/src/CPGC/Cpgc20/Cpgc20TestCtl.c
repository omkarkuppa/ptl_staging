/** @file
  This file implements functions for setting up test control
  registers for CPGC 2.0.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "Cpgc20.h"
#include "MrcCpgcOffsets.h"
#include "MrcSpdProcessingDefs.h" // for MRC_SPD_SDRAM_DENSITY_8Gb
#include "MrcLpddr5.h"

#define SINGLE_RANK     1

/**
  This function programs the masks that enable error checking on the
  requested cachelines and chunks.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  CachelineMask - Bit Mask of cachelines to enable.
  @param[in]  ChunkMask     - Bit Mask of chunks to enable.

  @retval Nothing
**/
void
Cpgc20SetChunkClErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                CachelineMask,
  IN  UINT32                ChunkMask
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     MaxChannel;
  MC0_CH0_CR_CPGC_ERR_CTL_STRUCT     Cpgc20ErrCtl;
  MC0_CH0_CR_CPGC_ERR_LNEN_HI_STRUCT Cpgc20ErrChunkMask;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = MrcGetTestErrCtlOffset (Controller, IpChannel);
      Cpgc20ErrCtl.Data = MrcReadCR (MrcData, Offset);
      Cpgc20ErrCtl.Bits.ERRCHK_MASK_CACHELINE = CachelineMask;
      Cpgc20ErrCtl.Bits.ERRCHK_MASK_CHUNK = ChunkMask;
      MrcWriteCR (MrcData, Offset, Cpgc20ErrCtl.Data);

      Offset = MrcGetCpgcErrCheckingHighOffset (Controller, IpChannel);
      Cpgc20ErrChunkMask.Data = ChunkMask;
      MrcWriteCR (MrcData, Offset, Cpgc20ErrChunkMask.Data);
    }
  }
}

/**
  This function programs the masks that enable error checking on the
  requested bytes.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  WordMask      - Which Word (Lower or Upper) to apply ErrMask to.
  @param[in]  ErrMask       - Error Masking Value to apply.

  @retval Nothing
**/
void
Cpgc20SetDataErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                WordMask,
  IN  UINT32                ErrMask
  )
{
  MrcOutput *Outputs;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT32    Controller;
  UINT32    Word;
  UINT32    MaxWords;
  UINT8     MaxChannel;

  Outputs    = &MrcData->Outputs;
  MaxWords   = 1;
  MaxChannel = Outputs->MaxChannels;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      for (Word = 0; Word < MaxWords; Word++) {
        if (((WordMask >> Word) & 1) != 1) {
          continue;
        }
        Offset = (Word == 0) ? MrcGetCpgcErrCheckingLowOffset (Controller, IpChannel) : MrcGetCpgcErrCheckingHighOffset (Controller, IpChannel);
        MrcWriteCR (MrcData, Offset, ErrMask);
      }
    }
  }
}

/**
  This function programs the masks that enable ecc error checking on the
  requested bytes.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  EccValue      - Error Masking Value to apply.

  @retval Nothing
**/
void
Cpgc20SetEccErrMsk (
  IN MrcParameters *const MrcData,
  IN UINT32               EccValue
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     McChMask;
  UINT8     MaxChannel;
  MC0_CH0_CR_CPGC_ERR_XLNEN_STRUCT Cpgc20EccErrMskRankMask;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = MrcGetEccErrMskRankErrMskOffset (Controller, IpChannel);
      Cpgc20EccErrMskRankMask.Data = MrcReadCR (MrcData, Offset);
      Cpgc20EccErrMskRankMask.Bits.ECC_ERRCHK_MASK = EccValue;
      MrcWriteCR (MrcData, Offset, Cpgc20EccErrMskRankMask.Data);
    }
  }
}

/**
  This function returns a specific channel error status

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Desired Memory Controller.
  @param[in]  Channel           - Desired Memory Channel to read error status.
  @param[in out]  ErrorStatus   - returned value of error status on the Desired Channel.

  @retval Nothing
**/
void
Cpgc20GetChannelError (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  OUT UINT8            *ErrorStatus
  )
{
  MC0_CH0_CR_CPGC_ERR_TEST_ERR_STAT_STRUCT   CpgcErrTestStatus;
  UINT32              Offset;
  MrcOutput           *Outputs;
  UINT32              IpChannel;
  UINT8               MaxChannel;
  UINT32              ErrorStatusMask;
  BOOLEAN             IsLpddr;
  BOOLEAN             IsDdr5;

  Outputs         = &MrcData->Outputs;
  IsLpddr         = Outputs->IsLpddr;
  IsDdr5          = Outputs->IsDdr5;
  MaxChannel      = Outputs->MaxChannels;
  ErrorStatusMask = 0;

  //Mask out errors for other channels that may be aggregated
  ErrorStatusMask = (1 << (Outputs->SdramCount * MAX_BITS / CPGC22_BITS_PER_ERROR_STATUS)) - 1;
  if (Outputs->EccSupport) {
    ErrorStatusMask |= CPGC22_ECC_ERROR_STATUS;
  }

  IpChannel = DDR5_IP_CH (IsDdr5, Channel);
  Offset = MrcGetTestErrStatOffset (Controller, IpChannel);
  CpgcErrTestStatus.Data = MrcReadCR (MrcData, Offset);
  // Channel 0 instance of MCx_CHx_CPGC_ERR_TEST_ERR_STAT reports the aggregate status of all channels.
  // Hence mask out other channels
  if (IsDdr5) {
    CpgcErrTestStatus.Bits.ERROR_STATUS &= CPGC_23_ERROR_STATUS_MASK_DDR5;
  } else if (IsLpddr) {
    CpgcErrTestStatus.Bits.ERROR_STATUS &= CPGC_23_ERROR_STATUS_MASK_LPDDR;
  } // No need to mask on DDR4 as we have only one channel per MC so there is no aggregation
  *ErrorStatus |= (((CpgcErrTestStatus.Bits.ERROR_STATUS & ErrorStatusMask) != 0) << (Channel + (Controller * MaxChannel)));  // Combine results from all controllers and channels into flat bitmask
}

/**
  This function returns a consolidated channel error status

  @param[in]      MrcData       - Pointer to MRC global data.
  @param[in]      McChBitMask   - Valid Channel bit mask

  @retval AllChError   - returns value of error status ORed between all channels
**/
UINT8
Cpgc20GetAllChannelsError (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask
  )
{
  UINT8             Controller;
  UINT8             Channel;
  UINT8             CurMcChMask;
  UINT8             AllChError;
  MrcOutput         *Outputs;
  Outputs = &MrcData->Outputs;
  AllChError = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      CurMcChMask = (1 << ((Controller * Outputs->MaxChannels) + Channel));
      if ((McChBitMask & CurMcChMask) != 0) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          Cpgc20GetChannelError (MrcData, Controller, Channel, &AllChError);
        }
      }
    } // for Channel
  } // for Controller
  return AllChError;
}

/**
  This function programs the CPGC chicken internal config's Read Request doubler

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Desired Memory Controller.
  @param[in]  IsEnabled     - Determines if we are enabling or disabling chicken internal config, if enabled, read request
                              combines two returns into the same TID. If disabled cleans up the register.

  @retval Nothing
**/
void
Cpgc20SetChickenInternalConfig (
  IN  MrcParameters *const  MrcData,
  IN  INT8                  Controller,
  IN BOOLEAN                IsEnabled
  )
{
  UINT32              Offset;
  MC0_SHARED_CR_CPGC2_V_CHICKEN_STRUCT CpgcChickenStruct;

  CpgcChickenStruct.Data = 0;
  Offset = OFFSET_CALC_CH (MC0_SHARED_CR_CPGC2_V_CHICKEN_REG, MC1_SHARED_CR_CPGC2_V_CHICKEN_REG, Controller);
  CpgcChickenStruct.Data = MrcReadCR (MrcData, Offset);
  CpgcChickenStruct.Bits.MPR_TEST_REQ_DBLR = (IsEnabled) ? 1 : 0;
  MrcWriteCR (MrcData, Offset, CpgcChickenStruct.Data);
}

/**
  This function programs the error conditions to stop the CPGC engine on.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  StopType      - Stop type for CPGC engine.
  @param[in]  NumOfErr      - Number of Stop Type errors to wait on before stopping CPGC engine.

  @retval Nothing
**/
void
Cpgc20SetupTestErrCtl (
  IN  MrcParameters       *const  MrcData,
  IN  MRC_TEST_STOP_TYPE          StopType,
  IN  UINT32                      NumOfErr
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT8     McChMask;
  UINT8     MaxChannel;
  UINT32    Offset;
  MC0_CH0_CR_CPGC_ERR_CTL_STRUCT  Cpgc20ErrCtl;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = MrcGetTestErrCtlOffset (Controller, IpChannel);
      Cpgc20ErrCtl.Data = MrcReadCR (MrcData, Offset);
      if (((UINT32) StopType == Cpgc20ErrCtl.Bits.STOP_ON_ERROR_CTL) && (NumOfErr == Cpgc20ErrCtl.Bits.STOP_ON_N)) {
        // Skipping the write as nothing changed
      } else {
        Cpgc20ErrCtl.Bits.STOP_ON_ERROR_CTL = StopType;
        Cpgc20ErrCtl.Bits.STOP_ON_N = NumOfErr;
        MrcWriteCR (MrcData, Offset, Cpgc20ErrCtl.Data);
      }
    }
  }
}

/**
  This function will Setup REUT Error Counters to count errors for specified type.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  CounterPointer  - Specifies in register which counter to setup. Each Channel has 9 counters including ECC.
  @param[in]  ErrControlSel   - Specifies which type of error counter read will be executed.
  @param[in]  CounterSetting  - Specifies in register which Lane/Byte/Chunk to track in specified counter,
                                based on ErrControlSel value.
  @param[in]  CounterScope    - Specifies if the Pointer is used or not.
  @param[in]  CounterUI       - Specifies which UI will be considered when counting errors.
                                  00 - All UI; 01 - Even UI; 10 - Odd UI; 11 - Particular UI (COUNTER_CONTROL_SEL = ErrCounterCtlPerUI)

  @retval mrcWrongInputParameter if CounterSetting is incorrect for the ErrControlSel selected, otherwise mrcSuccess.
**/
MrcStatus
Cpgc20SetupErrCounterCtl (
  IN MrcParameters *const      MrcData,
  IN UINT8                     CounterPointer,
  IN MRC_ERR_COUNTER_CTL_TYPE  ErrControlSel,
  IN UINT32                    CounterSetting,
  IN UINT8                     CounterScope,
  IN UINT8                     CounterUI
  )
{
  MrcStatus                            Status;
  MrcOutput                            *Outputs;
  MrcDebug                             *Debug;
  UINT32                               Offset;
  UINT8                                Controller;
  UINT8                                Channel;
  UINT8                                IpChannel;
  UINT8                                McChMask;
  UINT8                                MaxChannel;
  MC0_CH0_CR_CPGC_ERR_CNTRCTL_0_STRUCT Cpgc20ErrCounterCtl;

  Status     = mrcSuccess;
  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      switch (ErrControlSel) {
        // Setup Error Counter Control for particular lane
        // @todo: This always uses counter 0 for that Channel to count the lane.  This does not allow counting multiple lanes at a time, and should be fixed if feature is needed.
        // @todo: Return Error as this mode isn't supported.
        case ErrCounterCtlPerLane:
          if ((CounterSetting > 72) || (CounterPointer > 8)) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to setup an error counter for invalid lane. Counter: %u, Lane: %u\n", CounterPointer, CounterSetting);
            Status = mrcWrongInputParameter;
          }
          break;

        // Setup Error Counter Control for particular byte
        case ErrCounterCtlPerByte:
          if (((CounterSetting > 8) || (CounterPointer > 8)) && (MrcByteExist (MrcData, Controller, Channel, CounterPointer))) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to setup an error counter for invalid byte. Counter: %u, Byte: %u\n", CounterPointer, CounterSetting);
            Status = mrcWrongInputParameter;
          }
          break;

        // Setup Error Counter Control for particular nibble - used in Server, probably not used in Client
/**
        case ErrCounterCtlPerNibble:
          if ((CounterSetting > 4) || (CounterPointer > 8)) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to setup an error counter for invalid nibble. Counter: %u, Nibble: %u\n", CounterPointer, CounterSetting);
            Status = mrcWrongInputParameter;
          }
          break;
**/
        // Setup Error Counter Control for particular UI
        case ErrCounterCtlPerUI:
          if (CounterPointer > 8) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to setup an error counter for invalid chunk. Counter: %u\n", CounterPointer);
            Status = mrcWrongInputParameter;
          }
          // Setup Error Counter Control for All Lanes
          break;
        case ErrCounterCtlAllLanes:
          // In ErrCounterCtlAllLanes, COUNTER_CONTROL_SCOPE = 0. In this case, COUNTER_POINTER is unused and COUNTER_CONTROL_SEL is don't care.
          break;
        default:
          Status = mrcWrongInputParameter;
          break;
      }
      if (Status == mrcSuccess) {
        Offset = (OFFSET_CALC_MC_CH (MC0_CH0_CR_CPGC_ERR_CNTRCTL_0_REG, MC1_CH0_CR_CPGC_ERR_CNTRCTL_0_REG, Controller, MC0_CH1_CR_CPGC_ERR_CNTRCTL_0_REG, IpChannel)) +
               ((MC0_CH0_CR_CPGC_ERR_CNTRCTL_1_REG - MC0_CH0_CR_CPGC_ERR_CNTRCTL_0_REG) * CounterPointer);
        Cpgc20ErrCounterCtl.Data = 0;
        if (Outputs->IsDdr5 && Outputs->EccSupport && (CounterPointer == MRC_DDR5_ECC_BYTE)) {
          Cpgc20ErrCounterCtl.Bits.COUNTER_POINTER     = MRC_DDR5_ECC_CPGC_COUNTER_INDEX;
        } else {
          Cpgc20ErrCounterCtl.Bits.COUNTER_POINTER     = CounterPointer;
        }
        Cpgc20ErrCounterCtl.Bits.COUNTER_CONTROL_SEL   = ErrControlSel;
        Cpgc20ErrCounterCtl.Bits.COUNTER_CONTROL_SCOPE = CounterScope;
        Cpgc20ErrCounterCtl.Bits.COUNTER_CONTROL_UI    = CounterUI;
        MrcWriteCR (MrcData, Offset, Cpgc20ErrCounterCtl.Data);
      }
    }
  }
  return Status;
}

/**
  This function returns the Error status results for specified MRC_ERR_STATUS_TYPE.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Memory Controller.
  @param[in]  Channel     - Desired Channel.
  @param[in]  Param       - Specifies which type of error status read will be executed.
  @param[out] Buffer      - Pointer to buffer which register values will be read into.
                              Error status bits will be returned starting with bit zero.

  @retval Returns mrcWrongInputParameter if Param value is not supported by this function, otherwise mrcSuccess.
**/
MrcStatus
Cpgc20GetErrEccChunkRankByteStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  IN  MRC_ERR_STATUS_TYPE     Param,
  OUT UINT64          *const  Buffer
  )
{
  MrcOutput                                     *Outputs;
  UINT64                                        Value;
  UINT32                                        Offset;
  UINT32                                        Offset1;
  UINT32                                        CRValue;
  UINT32                                        IpChannel;
  UINT8                                         McChMask;
  UINT8                                         MaxChannel;
  BOOLEAN                                       EccSupport;
  MC0_CH0_CR_CPGC_ERR_BYTE_NTH_PAR_STAT_STRUCT  CpgcErrByteNthStatus;
  MC0_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_STRUCT CpgcErrEccChunkRank;
  MC0_CH0_CR_CPGC_ERR_STAT47_STRUCT             ErrStat47;

  Outputs    = &MrcData->Outputs;
  EccSupport = Outputs->EccSupport;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  Value = 0;
  if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
    return mrcWrongInputParameter;
  }
  IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
  if ((Param == ByteGroupErrStatus) || (Param == EccLaneErrStatus) || (Param == WdbRdChunkNumStatus) || (Param == NthErrStatus)) {
    Offset = OFFSET_CALC_MC_CH (
              MC0_CH0_CR_CPGC_ERR_BYTE_NTH_PAR_STAT_REG,
              MC1_CH0_CR_CPGC_ERR_BYTE_NTH_PAR_STAT_REG, Controller,
              MC0_CH1_CR_CPGC_ERR_BYTE_NTH_PAR_STAT_REG, IpChannel);
  } else if ((Param == RankErrStatus) || (Param == ChunkErrStatus)) {
    Offset = OFFSET_CALC_MC_CH (
              MC0_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG,
              MC1_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG, Controller,
              MC0_CH1_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG, IpChannel);
  } else {
    Offset = 0;
  }

  CRValue = MrcReadCR (MrcData, Offset);
  CpgcErrByteNthStatus.Data = CRValue;
  CpgcErrEccChunkRank.Data = CRValue;

  Offset1 = OFFSET_CALC_MC_CH (
    MC0_CH0_CR_CPGC_ERR_STAT47_REG,
    MC1_CH0_CR_CPGC_ERR_STAT47_REG, Controller,
    MC0_CH1_CR_CPGC_ERR_STAT47_REG, IpChannel);

  switch (Param) {
    case ByteGroupErrStatus:
      Value = CpgcErrByteNthStatus.Bits.BYTEGRP_ERR_STAT;
      if (EccSupport) {
        Value |= (UINT64) (CpgcErrByteNthStatus.Bits.ECCGRP_ERR_STAT << MC0_CH0_CR_CPGC_ERR_BYTE_NTH_PAR_STAT_ECCGRP_ERR_STAT_OFF);
      }
      break;

    case EccLaneErrStatus:
      if (EccSupport) {
        Value = CpgcErrByteNthStatus.Bits.ECCGRP_ERR_STAT;
      }
      break;

    case RankErrStatus:
      Value = CpgcErrEccChunkRank.Bits.RANK_ERR_STAT;
      break;

    case WdbRdChunkNumStatus:
      Value = CpgcErrByteNthStatus.Bits.RD_CHUNK_NUM_STAT;
      break;

    case NthErrStatus:
      Value = CpgcErrByteNthStatus.Bits.Nth_ERROR;
      break;

    case ChunkErrStatus:
      // For BL16/32 cases the chunk error status is in CPGC_ERR_STAT47.LANE_ERR_STAT_HI
      ErrStat47.Data = MrcReadCR (MrcData, Offset1);
      Value = ErrStat47.Bits.LANE_ERR_STAT_HI;
      break;

    case AlertErrStatus:
//    Value = ReutSubChErrChunkRankByteNthStatus.Bits.Alert_Error_Status;
//    break;

    case NthErrOverflow:
//    Value = ReutSubChErrChunkRankByteNthStatus.Bits.Nth_Error_Overflow;
//    break;

    default:
      return mrcWrongInputParameter;
  }

  MRC_DEBUG_ASSERT (Buffer != NULL, &Outputs->Debug, "%s Null Pointer", gErrString);
  *Buffer = Value;
  return mrcSuccess;
}

/**
  This function accesses the Sequence loop count (per McChBitMask).

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  McChBitMask  - Controller / Channel Mask to program
  @param[in]  LoopCount    - Pointer to variable to store or set.

  @retval Nothing.
**/
void
Cpgc20SetLoopCount (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 McChBitMask,
  IN OUT  const UINT32  *const  LoopCount
  )
{
  MrcOutput     *Outputs;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         IpChannel;
  UINT8         MaxChannel;
  UINT32        Offset;
  BOOLEAN       IsLpddr;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_BLOCK_REPEATS_REG,
                MC1_REQ0_CR_CPGC2_BLOCK_REPEATS_REG, Controller,
                MC0_REQ1_CR_CPGC2_BLOCK_REPEATS_REG, IpChannel);
      // MRC_DEBUG_MSG (Outputs->Debug, MSG_LEVEL_ERROR, "Cpgc20SetLoopCount: mc%d ch%d LoopCount = %d\n", Controller, Channel, *LoopCount);
      MrcWriteCR (MrcData, Offset, *LoopCount);
    }
  }
}

/**
  This function overrides the defualt Bank Address Swizzle to account for differences for LP5
  Cpgc Address bit 3 should be mapped to Cpgc Address bit 4, and Cpgc Address bit 3 should be left unused.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  Controller - Desired Controller.
  @param[in]  Channel    - Desired Channel.

  @retval Nothing.
**/
VOID
Cpgc20LpBankAddrSwizzleUpdate (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                Controller,
  IN      UINT32                Channel
  )
{
  MrcOutput     *Outputs;
  UINT32        IpChannel;
  UINT8         MaxChannel;
  UINT8         McChMask;
  UINT32        Offset;
  BOOLEAN       IsLpddr;
  MC0_REQ0_CR_CPGC_SEQ_BANK_ADDR_SWIZZLE_STRUCT BankAddrSwizzle;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  IsLpddr    = Outputs->IsLpddr;
  McChMask   = Outputs->McChBitMask;

  MRC_DEBUG_ASSERT (IsLpddr, &Outputs->Debug, "CPGC bank address swizzle adjustment should only occure for LP5");
  if (!IsLpddr) {
    return;
  }

  if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
    return;
  }
  IpChannel = LP_IP_CH (IsLpddr, Channel);
  Offset = OFFSET_CALC_MC_CH (
            MC0_REQ0_CR_CPGC_SEQ_BANK_ADDR_SWIZZLE_REG,
            MC1_REQ0_CR_CPGC_SEQ_BANK_ADDR_SWIZZLE_REG, Controller,
            MC0_REQ1_CR_CPGC_SEQ_BANK_ADDR_SWIZZLE_REG, IpChannel);
  BankAddrSwizzle.Data = MrcReadCR (MrcData, Offset);
  BankAddrSwizzle.Bits.L2P_BANK3_SWIZZLE = 0x1F; // unused 1'b0
  BankAddrSwizzle.Bits.L2P_BANK4_SWIZZLE = 0x1B; // CPGCBankAddr[3] mapped up to [4]
  MrcWriteCR (MrcData, Offset, BankAddrSwizzle.Data);
}

/**
  This function accesses the Logical to Physical Bank Mapping.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  Controller - Desired Controller
  @param[in]  Channel    - Desired Channel
  @param[in]  Banks      - Pointer to buffer to logical-to-physical bank mapping.
  @param[in]  Count      - Length of the Banks buffer.
  @param[in]  IsGet      - TRUE: Get.  FALSE: Set.

  @retval Nothing.
**/
void
Cpgc20GetSetBankMap (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                Controller,
  IN      UINT32                Channel,
  IN OUT  UINT8 *const          Banks,
  IN      UINT32                Count,
  IN      BOOLEAN               IsGet
  )
{
  MrcOutput     *Outputs;
  UINT8         McChMask;
  UINT8         MaxChannel;
  UINT32        IpChannel;
  UINT32        RegData;
  UINT32        RegIndex;
  UINT32        Index;
  UINT32        BanksPerReg;
  UINT32        Offset;
  UINT32        CrIncrement;
  UINT32        FieldOffsetStep;
  UINT32        FieldMask;
  UINT32        FieldOffset;
  BOOLEAN       EndOfRegister;
  BOOLEAN       IsLpddr;

  RegData     = 0;
  BanksPerReg = 6;
  Outputs     = &MrcData->Outputs;
  MaxChannel  = Outputs->MaxChannels;
  McChMask    = Outputs->McChBitMask;
  IsLpddr     = Outputs->IsLpddr;

  if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
    return;
  }
  IpChannel = LP_IP_CH (IsLpddr, Channel);
  Offset = OFFSET_CALC_MC_CH (
            MC0_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_REG,
            MC1_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_REG, Controller,
            MC0_REQ1_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_REG, IpChannel);
  CrIncrement = MC0_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_B_REG - MC0_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_REG;
  FieldOffsetStep = MC0_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_L2P_BANK1_MAPPING_OFF;
  FieldMask = MC0_REQ0_CR_CPGC_SEQ_BANK_L2P_MAPPING_A_L2P_BANK0_MAPPING_MSK;

  for (Index = 0; Index < Count; Index++) {
    RegIndex      = Index % BanksPerReg;
    FieldOffset   = RegIndex * FieldOffsetStep;
    EndOfRegister = (RegIndex == (BanksPerReg - 1)) || (Index == (Count - 1));
    if (RegIndex == 0) {
      RegData = MrcReadCR (MrcData, Offset);
    }
    if (IsGet) {
      Banks[Index] = (UINT8) ((RegData >> FieldOffset) & FieldMask);
    } else {
      RegData &= ~(FieldMask << FieldOffset);
      RegData |= (Banks[Index] & FieldMask) << FieldOffset;
      // If we're at the end of Banks to write, or we're at the end of the banks in the register,
      // write out the Register data.
      if (EndOfRegister) {
        MrcWriteCR (MrcData, Offset, RegData);
      }
    }
    if (EndOfRegister) {
      // Update the offset to the next register
      Offset += CrIncrement;
    }
  }
}

/**
  This function returns the Bit Group Error status results.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Memory Controller.
  @param[in]  Channel     - Desired Channel.
  @param[out] Status      - Pointer to array where the lane error status values will be stored.

  @retval Nothing.
**/
void
Cpgc20GetBitGroupErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  OUT UINT8                   *Status
  )
{
  MrcOutput     *Outputs;
  UINT32        ErrDataStatus03;
  UINT32        Offset1;
  UINT32        Offset2;
  UINT32        ErrDataStatus47;
  UINT8         Byte;
  UINT32        IpChannel;
  UINT8         EccByte;
  UINT8         McChMask;
  UINT8         MaxChannel;
  MC0_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_STRUCT CpgcErrEccChunkRank;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  EccByte    = MRC_DDR5_ECC_BYTE;

  if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) != 0) {
    IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
    Offset1 = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_ERR_STAT03_REG,
                MC1_CH0_CR_CPGC_ERR_STAT03_REG, Controller,
                MC0_CH1_CR_CPGC_ERR_STAT03_REG, IpChannel);
    Offset2 = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_ERR_STAT47_REG,
                MC1_CH0_CR_CPGC_ERR_STAT47_REG, Controller,
                MC0_CH1_CR_CPGC_ERR_STAT47_REG, IpChannel);

    MRC_DEBUG_ASSERT (Status != NULL, &Outputs->Debug, "%s Null Pointer", gErrString);

    ErrDataStatus03 = MrcReadCR (MrcData, Offset1);
    ErrDataStatus47 = MrcReadCR (MrcData, Offset2);
    for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
      if (Byte < 4) {
        Status[Byte] = (ErrDataStatus03 >> (8 * Byte)) & 0xFF;
      } else {
        Status[Byte] = (ErrDataStatus47 >> (8 * (Byte - 4))) & 0xFF;
      }
    }
    if (Outputs->EccSupport) {
      Offset1 = OFFSET_CALC_MC_CH (
                  MC0_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG,
                  MC1_CH0_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG, Controller,
                  MC0_CH1_CR_CPGC_ERR_ECC_CHNK_RANK_STAT_REG, IpChannel);
      CpgcErrEccChunkRank.Data = MrcReadCR (MrcData, Offset1);
      Status[EccByte] = (UINT8) CpgcErrEccChunkRank.Bits.ECC_LANE_ERR_STAT;
    }
  }
}
/**
  This function returns the Error Counter status for specified counter.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Desired Memory Controller.
  @param[in]  Channel       - Desired Channel.
  @param[in]  CounterSelect - Desired error counter to read from.
  @param[out] CounterStatus - Pointer to buffer where counter status will be held.
  @param[out] Overflow      - Indicates if counter has reached overflow.

  @retval Nothing.
**/
void
Cpgc20GetErrCounterStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                CounterSelect,
  OUT UINT64        *const  CounterStatus,
  OUT BOOLEAN       *const  Overflow
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcInput          *Inputs;
  MrcOutput  *Outputs;
  UINT32     IpChannel;
  UINT32     Offset;
  UINT64     Value;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;
  Outputs   = &MrcData->Outputs;

  IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
  Offset = OFFSET_CALC_MC_CH (
            MC0_CH0_CR_CPGC_ERR_CNTR_0_REG,
            MC1_CH0_CR_CPGC_ERR_CNTR_0_REG, Controller,
            MC0_CH1_CR_CPGC_ERR_CNTR_0_REG, IpChannel);
  Offset += (MC0_CH0_CR_CPGC_ERR_CNTR_1_REG - MC0_CH0_CR_CPGC_ERR_CNTR_0_REG) * CounterSelect;
  // CPGC_ERR_CNTR_0_REG is a 32 bit register therefore MrcReadCR works fine.
  // CounterStatus is 64 bits because the same variable Status is used in the calling function MrcGetErrCounterStatus,
  // for Cpgc20GetErrCounterStatus and MrcGetRasterRepoStatus. The later function reads a 64 bit register.
  Value = MrcReadCR (MrcData, Offset);

  MRC_DEBUG_ASSERT ((CounterStatus != NULL) && (Overflow != NULL), &Outputs->Debug, "%s Null Pointer", gErrString);

  *CounterStatus = Value;

  Offset = OFFSET_CALC_MC_CH (
            MC0_CH0_CR_CPGC_ERR_CNTR_OV_REG,
            MC1_CH0_CR_CPGC_ERR_CNTR_OV_REG, Controller,
            MC0_CH1_CR_CPGC_ERR_CNTR_OV_REG, IpChannel);
  Value = MrcReadCR (MrcData, Offset);

  *Overflow = ((MrcCall->MrcRightShift64(Value, (CounterSelect % 9))) & 1) == 1;
}

/**
  This function writes to all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.
  @param[in]  CpgcSeqCtl    - Data to be written to all CPGC SEQ CTL registers.

  @retval Nothing.
**/
void
Cpgc20ControlRegWrite (
  IN  MrcParameters *const              MrcData,
  IN  UINT8                             McChBitMask,
  IN  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT   CpgcSeqCtl
  )
{
  MrcOutput     *Outputs;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         IpChannel;
  UINT8         MaxChannel;
  UINT32        Offset;
  BOOLEAN       IsLpddr;
  BOOLEAN       GlobalStart;
  BOOLEAN       IssuedStart;

  MC0_REQ0_CR_CPGC_SEQ_STATUS_STRUCT  CpgcStatus;
  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  IsLpddr    = Outputs->IsLpddr;

  GlobalStart = (CpgcSeqCtl.Bits.START_TEST == 1) && MrcData->Save.Data.CpgcGlobalStart;  // Start all CPGC engines together with a single write
  IssuedStart = FALSE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      if (CpgcSeqCtl.Bits.START_TEST == 1) {
        Offset = OFFSET_CALC_MC_CH (
          MC0_REQ0_CR_CPGC_SEQ_STATUS_REG,
          MC1_REQ0_CR_CPGC_SEQ_STATUS_REG, Controller,
          MC0_REQ1_CR_CPGC_SEQ_STATUS_REG, IpChannel);
        CpgcStatus.Data = MrcReadCR (MrcData, Offset);
        if (CpgcStatus.Bits.TEST_DONE == 1) {
          // Clear by writing a '1'
          MrcWriteCR (MrcData, Offset, CpgcStatus.Data);
        }
      }
      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC_SEQ_CTL_REG,
                MC1_REQ0_CR_CPGC_SEQ_CTL_REG, Controller,
                MC0_REQ1_CR_CPGC_SEQ_CTL_REG, IpChannel);

      MrcWriteCR (MrcData, Offset, CpgcSeqCtl.Data);
      if (GlobalStart) {
        IssuedStart = TRUE;
        break;
      }
    } // Channel
    if (IssuedStart) {
      break;
    }
  }
}

/**
  This function Start Cpgc Test on all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.

  @retval Nothing.
**/
void
Cpgc20StartTest (
  IN  MrcParameters *const              MrcData,
  IN  UINT8                             McChBitMask
  )
{
  IN  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT CpgcSeqCtl;

  CpgcSeqCtl.Data = 0;
  CpgcSeqCtl.Bits.START_TEST = 1;
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);
}

/**
  This function Stop Cpgc Test on all enabled CPGC SEQ CTL registers.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Memory Controller Channel Bit mask for which registers should be programmed for.

  @retval Nothing.
**/
void
Cpgc20StopTest (
  IN  MrcParameters *const              MrcData,
  IN  UINT8                             McChBitMask
  )
{
  IN  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT   CpgcSeqCtl;

  CpgcSeqCtl.Data = 0;
  CpgcSeqCtl.Bits.STOP_TEST = 1;
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);
}

/**
  This function aggregates the status of STOP_TEST bit for all enabled CPGC engines.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval UINT8 of the aggregated value of STOP_TEST.
**/
UINT8
Cpgc20ControlRegStopBitStatus (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput                       *Outputs;
  UINT8                           Controller;
  UINT8                           Channel;
  UINT8                           IpChannel;
  UINT8                           StopBit;
  UINT8                           McChMask;
  UINT8                           MaxChannel;
  UINT32                          Offset;
  BOOLEAN                         IsLpddr;
  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT CpgcSeqCtl;

  Outputs    = &MrcData->Outputs;
  StopBit    = 0;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC_SEQ_CTL_REG,
                MC1_REQ0_CR_CPGC_SEQ_CTL_REG, Controller,
                MC0_REQ1_CR_CPGC_SEQ_CTL_REG, IpChannel);
      CpgcSeqCtl.Data = MrcReadCR (MrcData, Offset);
      StopBit |= CpgcSeqCtl.Bits.STOP_TEST;
    }
  }
  return StopBit;
}

/**
  This function writes Command Instructions to all enabled CPGC engines.

  @param[in]  MrcData        - Pointer to MRC global data.
  @param[in]  CmdInstruct    - Array of Command Instructions
  @param[in]  NumInstruct    - Number of Instructions

  @retval MrcStatus - mrcSuccess if does not exceed command instruction registers otherwise mrcFail
**/
MrcStatus
Cpgc20CmdInstructWrite (
  IN  MrcParameters *const                            MrcData,
  IN  MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_STRUCT  *CmdInstruct,
  IN  UINT8                                           NumInstruct
  )
{
  MrcOutput         *Outputs;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             IpChannel;
  UINT8             Index;
  UINT8             McChMask;
  UINT8             MaxChannel;
  UINT32            Offset;
  BOOLEAN           IsLpddr;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  if (NumInstruct > CPGC20_MAX_COMMAND_INSTRUCTION) {
    return mrcFail;
  }
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_REG,
                MC1_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_REG, Controller,
                MC0_REQ1_CR_CPGC2_COMMAND_INSTRUCTION_0_REG, IpChannel);
      for (Index = 0; Index < NumInstruct; Index++) {
        MrcWriteCR8 (MrcData, Offset, CmdInstruct[Index].Data);
        Offset += MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_1_REG - MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_REG;
      }
    }
  }
  return mrcSuccess;
}

/**
  This function writes Algorithm Instructions to all enabled CPGC engines (per Outputs->McChBitMask).

  @param[in]  MrcData               - Pointer to MRC global data.
  @param[in]  AlgoInstruct          - Array of Algorithm Instructions
  @param[in]  AlgoInstructControl   - Algorithm Instruction Control setting to program based on ValidMask
  @param[in]  AlgoWaitEventControl  - Algorithm Wait Event Control setting to program
  @param[in]  ValidMask             - Bit Mask of which Instructions are valid

  @retval Nothing.
**/
void
Cpgc20AlgoInstructWrite (
  IN  MrcParameters *const                                   MrcData,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT       *AlgoInstruct,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_STRUCT  AlgoInstructControl,
  IN  MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_STRUCT  AlgoWaitEventControl,
  IN  UINT8                                                  ValidMask
  )
{
  MrcOutput     *Outputs;
  UINT32        Controller;
  UINT32        Channel;
  UINT32        IpChannel;
  UINT32        Offset;
  UINT32        CtrlOffset;
  UINT32        Index;
  UINT8         McChMask;
  UINT8         MaxChannel;
  BOOLEAN       IsLpddr;
  MC0_REQ0_CR_CPGC2_BASE_CLOCK_CONFIG_STRUCT WaitToStartCfg;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (
                    MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_REG,
                    MC1_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_REG, Controller,
                    MC0_REQ1_CR_CPGC2_ALGORITHM_INSTRUCTION_0_REG, IpChannel);
      CtrlOffset = OFFSET_CALC_MC_CH (
                    MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_REG,
                    MC1_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_REG, Controller,
                    MC0_REQ1_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_REG, IpChannel);
      for (Index = 0; Index < CPGC20_MAX_ALGORITHM_INSTRUCTION; Index++) {
        if (ValidMask & (MRC_BIT0 << Index)) {
          MrcWriteCR8 (MrcData, Offset, AlgoInstruct[Index].Data);
          MrcWriteCR8 (MrcData, CtrlOffset, AlgoInstructControl.Data);
          Offset += MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_1_REG - MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_REG;
          CtrlOffset += MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_REG - MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_REG;
        } else {
          break;
        }
      }
      CtrlOffset = OFFSET_CALC_MC_CH (
                    MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_REG,
                    MC1_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_REG, Controller,
                    MC0_REQ1_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_REG, IpChannel);
      MrcWriteCR (MrcData, CtrlOffset, AlgoWaitEventControl.Data);

      if (AlgoWaitEventControl.Bits.Wait_Clock_Frequency == CPGC20_NATIVE_DUNIT_FREQ) {
        Offset = OFFSET_CALC_MC_CH (
                  MC0_REQ0_CR_CPGC2_BASE_CLOCK_CONFIG_REG,
                  MC1_REQ0_CR_CPGC2_BASE_CLOCK_CONFIG_REG, Controller,
                  MC0_REQ1_CR_CPGC2_BASE_CLOCK_CONFIG_REG, IpChannel);
        WaitToStartCfg.Data = MrcReadCR (MrcData, Offset);
        WaitToStartCfg.Bits.Clock_Freq = 1;
        MrcWriteCR (MrcData, Offset, WaitToStartCfg.Data);
      }
    }
  }
}

/**
  This function writes Data Instructions to all enabled CPGC engines.

  @param[in]  MrcData        - Pointer to MRC global data.
  @param[in]  DataInstruct   - Array of Data Instructions
  @param[in]  ValidMask      - Bit Mask of which Instructions are valid

  @retval Nothing.
**/
VOID
Cpgc20DataInstructWrite (
  IN  MrcParameters *const                        MrcData,
  IN  MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_STRUCT *DataInstruct,
  IN  UINT8                                       ValidMask
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Index;
  UINT32    Offset;
  UINT8     McChMask;
  UINT8     MaxChannel;
  BOOLEAN   IsLpddr;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_REG,
                MC1_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_REG, Controller,
                MC0_REQ1_CR_CPGC2_DATA_INSTRUCTION_0_REG, IpChannel);
      for (Index = 0; Index < CPGC20_MAX_DATA_INSTRUCTION; Index++) {
        if (ValidMask & (MRC_BIT0 << Index)) {
          MrcWriteCR8 (MrcData, Offset, DataInstruct[Index].Data);
          Offset += MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_1_REG - MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_REG;
        } else {
          break;
        }
      }
    }
  }
}

/**
  This function sets up Address related registers (ADDRESS_INSTRUCTION, REGION_LOW_ROW, REGION_LOW_COL, ADDRESS_SIZE) to all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  CPGCAddressArray  - 2D Array of Structure that stores address related settings
  @param[in]  EnCADB            - Set up the address ordering for Command stress

  @retval Nothing.
**/
VOID
Cpgc20AddressSetup (
  IN  MrcParameters *const            MrcData,
  IN  MRC_ADDRESS                     CPGCAddressArray [MAX_CONTROLLER][MAX_CHANNEL],
  IN  UINT8                           EnCADB
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Index;
  UINT32    Offset;
  UINT8     McChMask;
  UINT8     MaxChannel;
  BOOLEAN   IsLpddr;
  MRC_ADDRESS     *CpgcAddrPtr;
  MC0_REQ0_CR_CPGC2_ADDRESS_INSTRUCTION_0_STRUCT  Cpgc2AddrInstruct;
  MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_STRUCT           Cpgc2AddrSize;
  MC0_REQ0_CR_CPGC2_REGION_LOW_STRUCT             Cpgc2Low;
  MC0_REQ0_CR_CPGC2_ADDRESS_PRBS_POLY_STRUCT      AddressPrbsPoly;
  MC0_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_STRUCT   BaseAddressControl;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  AddressPrbsPoly.Data = 0;
  BaseAddressControl.Data = 0;
  Cpgc2Low.Data = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);

      if (EnCADB) {
        Offset = OFFSET_CALC_MC_CH (
          MC0_REQ0_CR_CPGC2_ADDRESS_PRBS_POLY_REG,
          MC1_REQ0_CR_CPGC2_ADDRESS_PRBS_POLY_REG, Controller,
          MC0_REQ1_CR_CPGC2_ADDRESS_PRBS_POLY_REG, IpChannel);
        MrcWriteCR64 (MrcData, Offset, AddressPrbsPoly.Data);

        Offset = OFFSET_CALC_MC_CH (
          MC0_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_REG,
          MC1_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_REG, Controller,
          MC0_REQ1_CR_CPGC2_BASE_ADDRESS_CONTROL_REG, IpChannel);
        MrcWriteCR (MrcData, Offset, BaseAddressControl.Data);
      }

      CpgcAddrPtr = &CPGCAddressArray[Controller][Channel];
      //  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "AddressOrder: %u, AddressDirection: %u, LastValidInstruct: %u\n", AddressOrder, AddressDirection, LastValidInstruct);
      //  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RowStart: %u, RowSizeBits: 0x%x\n", RowStart, RowSizeBits);
      //  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ColStart: %u, ColSizeBits: 0x%x\n", ColStart, ColSizeBits);
      Cpgc2AddrInstruct.Data = 0;
      Cpgc2AddrInstruct.Bits.Address_Order = CpgcAddrPtr->AddrIncOrder;
      Cpgc2AddrInstruct.Bits.Address_Direction = CpgcAddrPtr->AddrDirection;

      Cpgc2Low.Bits.Low_Row = CpgcAddrPtr->RowStart;
      Cpgc2Low.Bits.Low_Col = CpgcAddrPtr->ColStart;

      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_ADDRESS_INSTRUCTION_0_REG,
                MC1_REQ0_CR_CPGC2_ADDRESS_INSTRUCTION_0_REG, Controller,
                MC0_REQ1_CR_CPGC2_ADDRESS_INSTRUCTION_0_REG, IpChannel);
      for (Index = 0; Index < CPGC20_MAX_ADDRESS_INSTRUCTION; Index++) {
        Cpgc2AddrInstruct.Bits.Last = (CpgcAddrPtr->LastValidAddr == Index) ? 1 : 0;
        MrcWriteCR8 (MrcData, Offset, Cpgc2AddrInstruct.Data);
        Offset += MC0_REQ0_CR_CPGC2_ADDRESS_INSTRUCTION_1_REG - MC0_REQ0_CR_CPGC2_ADDRESS_INSTRUCTION_0_REG;
        if (CpgcAddrPtr->LastValidAddr == Index) {
          break;
        }
      }

      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_REGION_LOW_REG,
                MC1_REQ0_CR_CPGC2_REGION_LOW_REG, Controller,
                MC0_REQ1_CR_CPGC2_REGION_LOW_REG, IpChannel);
      MrcWriteCR64 (MrcData, Offset, Cpgc2Low.Data);

      Offset = OFFSET_CALC_MC_CH (
                MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_REG,
                MC1_REQ0_CR_CPGC2_ADDRESS_SIZE_REG, Controller,
                MC0_REQ1_CR_CPGC2_ADDRESS_SIZE_REG, IpChannel);
      Cpgc2AddrSize.Data = MrcReadCR64 (MrcData, Offset);
      // Row and Col bit sizes in BlockSize and RegionSize registers are exponential. Bank and Rank are linear "+1".
      Cpgc2AddrSize.Bits.Block_Size_Bits_Row  = EnCADB ? 0 : CpgcAddrPtr->RowSizeBits; // In command stress we want Row to be updated together with Bank, so block size is zero
      Cpgc2AddrSize.Bits.Block_Size_Bits_Col  = EnCADB ? 2 : CpgcAddrPtr->ColSizeBits; // In command stress we want Bank / Row update every 4 CLs
      Cpgc2AddrSize.Bits.Region_Size_Bits_Row = CpgcAddrPtr->RowSizeBits;
      Cpgc2AddrSize.Bits.Region_Size_Bits_Col = CpgcAddrPtr->ColSizeBits;
      Cpgc2AddrSize.Bits.Block_Size_Max_Bank = (CpgcAddrPtr->BankSize) ? CpgcAddrPtr->BankSize - 1 : 0;
      Cpgc2AddrSize.Bits.Region_Size_Max_Bank = Cpgc2AddrSize.Bits.Block_Size_Max_Bank;
      MrcWriteCR64 (MrcData, Offset, Cpgc2AddrSize.Data);
    }
  }
}

/**
  This function acts as wrapper function and calls Cpgc20UpdateBaseRepeatsForWholeRankExtended ().
  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.
  @param[in]  UseBankAddresses  - Whether to use bank addresses in calculating base repeats. If false, only uses bank groups.
**/
void
Cpgc20UpdateBaseRepeatsForWholeRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask,
  IN  BOOLEAN               UseBankAddresses
  )
{
  Cpgc20UpdateBaseRepeatsForWholeRankExtended (MrcData, Rank, McChBitMask, UseBankAddresses, NULL);
}

/**
  This function updates BASE_REPEATS to match the required number of Writes.
  This is used in memory scrubbing and PPR.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.
  @param[in]  UseBankAddresses  - Whether to use bank addresses in calculating base repeats. If false, only uses bank groups.
  @param[in]  CapNotPowerOf2    - Whether non-power of 2 capacity found per MC/CH
**/
void
Cpgc20UpdateBaseRepeatsForWholeRankExtended (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask,
  IN  BOOLEAN               UseBankAddresses,
  IN  BOOLEAN               CapNotPowerOf2[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  )
{
  UINT8             Controller;
  MrcOutput         *Outputs;
  UINT8             Channel;
  UINT32            Burst;
  UINT8             MaxChannel;
  MrcDimmOut        *DimmOut;
  UINT8             BankCount;
  UINT8             IpChannel;
  UINT16            Columns;
  BOOLEAN           IsLpddr;

  Outputs  = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  IsLpddr = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH(IsLpddr, Channel);
      // get the Bank/Row/Col size from SPD for the whole rank
      DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER(Rank)];
      BankCount = DimmOut->BankGroups;
      if (UseBankAddresses) {
        BankCount *= DimmOut->Banks;
      }

      // BL=16: column increment is 2^4 per CL --> 10 - 4 = 6
      // BL=32: column increment is 2^5 per CL --> 10 - 5 = 5
      if (IsLpddr) { // LPDDR
        Columns = DimmOut->ColumnSize / 32;
      } else { // DDR5
        Columns = DimmOut->ColumnSize / 16;
      }

      // Update BASE_REPEATS to match the required number of Writes
      if ((CapNotPowerOf2 != NULL) && CapNotPowerOf2[Rank][Controller][Channel]) {
        Burst = (DimmOut->RowSize * Columns * BankCount) / 4;
      } else {
        Burst = DimmOut->RowSize * Columns * BankCount;
      }
      Cpgc20BaseRepeatsMcCh (MrcData, Controller, IpChannel, Burst, SINGLE_RANK);
    }
  }
}

/**
  This function sets up ADDRESS_SIZE register per channel according to the Bank/Row/Col size of the given Rank.
  This is used in memory scrubbing and PPR.
  The register will be updated on all enabled CPGC engines.
  This function only supports DDR5 and LP5 BG Mode (Default CPGC settings support LP5 16 Bank Mode therefore it does not need additonal bank mapping)

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  McChbitMask       - Bit masks of MC channels to enable for the test.
  @param[in]  UpdateAddrMaxBank - Update MaxBank value for CPGC

**/
void
MrcUpdateL2PAllsBanksMapping (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Rank,
  IN  const UINT8           McChBitMask,
  IN  BOOLEAN               UpdateAddrMaxBank
  )
{
  MrcOutput         *Outputs;
  MrcControllerOut  *ControllerOut;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             MaxChannel;
  MrcDimmOut        *DimmOut;
  UINT8             BankCount;
  MRC_BG_BANK_PAIR  *BankMapping;
  BOOLEAN           IsLpddr;
  BOOLEAN           IsDdr5;
  MRC_LP5_BANKORG   Lp5BankOrg;

  Outputs    = &MrcData->Outputs;
  MaxChannel = MrcData->Outputs.MaxChannels;
  IsLpddr    = MrcData->Outputs.IsLpddr;
  IsDdr5     = MrcData->Outputs.IsDdr5;
  Lp5BankOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);

  if (IsDdr5 || (IsLpddr && (Lp5BankOrg == MrcLp5BgMode))) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
          continue;
        }
        ControllerOut = &MrcData->Outputs.Controller[Controller];
        DimmOut = &ControllerOut->Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
        BankCount = DimmOut->BankGroups * DimmOut->Banks;
        // Program Bank Logical to Physical mapping, to cover all banks on this DIMM while allowing B2B traffic for maximum speed
        if (IsDdr5) {
          if (BankCount == 8) {
            BankMapping = Ddr5_8Gbx16BankMapB2B;
          } else if (BankCount == 16) {
            BankMapping = (DimmOut->DensityIndex == MRC_SPD_SDRAM_DENSITY_8Gb) ? Ddr5_8Gbx8BankMapB2B : Ddr5_16Gbx16BankMapB2B;
          } else { // 32 banks
            BankMapping = Ddr5_16Gbx8BankMapB2B;
          }
        } else {
          // LP5 BG mode has 4 bank groups / 4 banks, so L2P table is same as Ddr5 16Gb x 16 Bank
          BankMapping = Ddr5_16Gbx16BankMapB2B;
        }
        Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BankMapping, BankCount, MRC_SET);
        if (UpdateAddrMaxBank) {
          Cpgc20SetAddrMaxBank (MrcData, Controller, Channel, (BankCount - 1));
        }
      }
    }
  }
}

/**
  This function sets up Base Repeats to all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  McChBitMask       - Controller / Channel Mask to program
  @param[in]  Burst             - Number of Cachelines (Should be power of 2)
  @param[in]  Ranks             - # of Ranks to test

  @retval Value written to CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20BaseRepeats (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                Burst,
  IN  UINT32                Ranks
  )
{
  MrcOutput     *Outputs;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         IpChannel;
  UINT8         MaxChannel;
  UINT32        BaseRepeats;
  BOOLEAN       IsLpddr;

  Outputs     = &MrcData->Outputs;
  MaxChannel  = Outputs->MaxChannels;
  BaseRepeats = 0;
  IsLpddr     = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      BaseRepeats = Cpgc20BaseRepeatsMcCh (MrcData, Controller, IpChannel, Burst, Ranks);
    }
  }
  return BaseRepeats;
}

/**
  This function sets up Base Repeats for a given CPGC engine (per Controller / Channel).
  The function doesn't check for mc/ch presence. Lp Channel taken care of outside of the function in both use cases.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Controller to work on
  @param[in]  Channel     - Channel to work on
  @param[in]  Burst       - Number of Cachelines (Should be power of 2)
  @param[in]  Ranks       - # of Ranks to test

  @retval Value written to CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20BaseRepeatsMcCh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Burst,
  IN  UINT32                Ranks
  )
{
  UINT32    Offset;
  UINT32    BaseRepeats;
  MC0_REQ0_CR_CPGC2_BASE_REPEATS_STRUCT  Cpgc2BaseRepeats;

  BaseRepeats = Burst * Ranks;
  BaseRepeats -= (BaseRepeats) ? 1 : 0;
  Cpgc2BaseRepeats.Data = 0;
  Cpgc2BaseRepeats.Bits.Base_Repeats = BaseRepeats;
//  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Cpgc20BaseRepeatsMcCh: Mc%d Ch%d Burst = %d, Ranks = %d --> BaseRepeats = %d\n", Controller, Channel, Burst, Ranks, BaseRepeats);
  Offset = OFFSET_CALC_MC_CH (
            MC0_REQ0_CR_CPGC2_BASE_REPEATS_REG,
            MC1_REQ0_CR_CPGC2_BASE_REPEATS_REG, Controller,
            MC0_REQ1_CR_CPGC2_BASE_REPEATS_REG, Channel);
  MrcWriteCR (MrcData, Offset, Cpgc2BaseRepeats.Data);

  return BaseRepeats;
}

/**
This function reads base repeats register the channel/subChannel provided.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on

  @retval Value read from CPGC2_BASE_REPEATS.
**/
UINT32
Cpgc20GetBaseRepeats (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                IpChannel
  )
{
  MC0_REQ0_CR_CPGC2_BASE_REPEATS_STRUCT  CpgcBaseRepeats;
  UINT32     Offset;
  Offset = OFFSET_CALC_MC_CH (
            MC0_REQ0_CR_CPGC2_BASE_REPEATS_REG,
            MC1_REQ0_CR_CPGC2_BASE_REPEATS_REG, Controller,
            MC0_REQ1_CR_CPGC2_BASE_REPEATS_REG, IpChannel);
  CpgcBaseRepeats.Data = MrcReadCR (MrcData, Offset);
  return CpgcBaseRepeats.Data;
}


/**
  Set CPGC command, algorithm and data instruction registers

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in] CmdPat      - 0: PatWrRd (Standard Write/Read Loopback),
                           1: PatWr (Write Only),
                           2: PatRd (Read Only),
                           3: PatRdWrTA (ReadWrite Turnarounds),
                           4: PatWrRdTA (WriteRead Turnarounds),
  @param[in] EnCADB      - Enable test to write random deselect packages on bus to create xtalk/isi
  @param[in] SubSeqWait  - # of Dclks to stall at the end of a sub-sequence
  @param[in] UseAltData  - Enable Alternate Data flag for Command instructions

  @retval Nothing
**/
void
Cpgc20SetCommandSequence (
  IN MrcParameters *const  MrcData,
  IN const UINT8           CmdPat,
  IN const UINT8           EnCADB,
  IN UINT16                SubSeqWait,
  IN BOOLEAN               UseAltData
  )
{
  MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_STRUCT        Cpgc2CmdInstruction[6]; // 6 instructions defined in this function
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      Cpgc2AlgoInstructionWr;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      Cpgc2AlgoInstructionRd;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      Cpgc2AlgoInstructionRdWr;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      Cpgc2AlgoInstructionWrRd;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      AlgoInstruct[CPGC20_MAX_ALGORITHM_INSTRUCTION];
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_STRUCT AlgoInstructControl;
  MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_STRUCT AlgoWaitEventControl;
  MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_STRUCT           DataInstruct[CPGC20_MAX_DATA_INSTRUCTION];

  UINT8             Index;
  UINT8             ValidMask;
  MrcOutput         *Outputs;
  MrcDebug          *Debug;
  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  //###########################################################
  // Program Command Instuctions @todo set this up in GetSet cached and then flushed
  //###########################################################
  // Write CMD [0]
  Cpgc2CmdInstruction[0].Data          = 0;
  Cpgc2CmdInstruction[0].Bits.WriteCmd = 1;
  Cpgc2CmdInstruction[0].Bits.Address_Decode_or_PRBS_En = EnCADB;
  Cpgc2CmdInstruction[0].Bits.Alternate_Data = (UseAltData) ? 1 : 0;
  Cpgc2CmdInstruction[0].Bits.Last     = 1;

  // Read CMD [1]
  Cpgc2CmdInstruction[1].Data          = 0;
  Cpgc2CmdInstruction[1].Bits.Address_Decode_or_PRBS_En = EnCADB;
  Cpgc2CmdInstruction[1].Bits.Alternate_Data = (UseAltData) ? 1 : 0;
  Cpgc2CmdInstruction[1].Bits.Last     = 1;

  // Read-Write CMD [2-3]
  Cpgc2CmdInstruction[2].Data          = 0;
  Cpgc2CmdInstruction[3].Data          = Cpgc2CmdInstruction[0].Data;

  // Write-Read CMD [4-5]
  Cpgc2CmdInstruction[4].Data          = 0;
  Cpgc2CmdInstruction[4].Bits.WriteCmd = 1;
  Cpgc2CmdInstruction[5].Data          = Cpgc2CmdInstruction[1].Data;

  Cpgc20CmdInstructWrite (MrcData, &Cpgc2CmdInstruction[0], ARRAY_COUNT (Cpgc2CmdInstruction));
  //###########################################################
  // Program Algorithm Instructions (pointing to the Command Instructions above)
  //###########################################################
  // Write Algo
  Cpgc2AlgoInstructionWr.Data = 0;
  Cpgc2AlgoInstructionWr.Bits.Command_Start_Pointer = 0;

  // Read Algo
  Cpgc2AlgoInstructionRd.Data = 0;
  Cpgc2AlgoInstructionRd.Bits.Command_Start_Pointer = 1;

  // Read-Write Algo
  Cpgc2AlgoInstructionRdWr.Data = 0;
  Cpgc2AlgoInstructionRdWr.Bits.Command_Start_Pointer = 2;

  // Write-Read Algo
  Cpgc2AlgoInstructionWrRd.Data = 0;
  Cpgc2AlgoInstructionWrRd.Bits.Command_Start_Pointer = 4;

  //###########################################################
  // Program Sequence of Algorithm Instructions based on CmdPat
  //###########################################################
  switch (CmdPat) {
    case PatWrRdTA:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionWr.Data;        // Write CMD All data
      for (Index = 1; Index <= 6; Index++) {
        AlgoInstruct[Index].Data = Cpgc2AlgoInstructionRdWr.Data;// Read-Write CMD
      }
      AlgoInstruct[7].Data = Cpgc2AlgoInstructionRd.Data;        // Read CMD
      AlgoInstruct[7].Bits.Last = 1;
      ValidMask = 0xFF;
      break;

    case PatRdWrTA:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionWrRd.Data;      // Write-Read CMD
      AlgoInstruct[0].Bits.Last = 1;
      ValidMask = 0x01;
      break;

    case PatWrRd:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionWr.Data;        // Write CMD
      AlgoInstruct[1].Data = Cpgc2AlgoInstructionRd.Data;        // Read CMD
      AlgoInstruct[1].Bits.Last = 1;
      ValidMask = 0x03;
      break;

    case PatWr:
    case PatWrScrub:
    case PatWrUp:
    case PatWrDown:
    case PatWrUpInvt:
    case PatWrDownInvt:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionWr.Data;        // Write CMD
      AlgoInstruct[0].Bits.Last = 1;
      ValidMask = 0x01;
      break;

    case PatRd:
    case PatRdUp:
    case PatRdDown:
    case PatRdUpInvt:
    case PatRdDownInvt:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionRd.Data;        // Read CMD
      AlgoInstruct[0].Bits.Last = 1;
      ValidMask = 0x01;
      break;

    case PatRdEndless:
      // Used in endless tests like in HW-based ReadMPR
      for (Index = 0; Index < CPGC20_MAX_ALGORITHM_INSTRUCTION; Index++) {
        AlgoInstruct[Index].Data = Cpgc2AlgoInstructionRd.Data;  // Read CMD, and no Last = endless test, no data rotation
      }
      ValidMask = 0xFF;
      break;

    case PatWrRdEndless:
      for (Index = 0; Index < CPGC20_MAX_ALGORITHM_INSTRUCTION; Index += 2) {
        AlgoInstruct[Index].Data = Cpgc2AlgoInstructionWr.Data;        // Write CMD
        AlgoInstruct[Index + 1].Data = Cpgc2AlgoInstructionRd.Data;    // Read CMD
      }
      ValidMask = 0xFF;
      break;

    case PatWrEndless:
      // Used in endless tests like in VccDll Post Training refinement
      for (Index = 0; Index < CPGC20_MAX_ALGORITHM_INSTRUCTION; Index++) {
        AlgoInstruct[Index].Data = Cpgc2AlgoInstructionWr.Data;  // Wr CMD, and no Last = endless test, no data rotation
      }
      ValidMask = 0xFF;
      break;

    case PatRdWr:
    case PatRdWrUp:
    case PatRdWrDown:
    case PatRdWrUpInvt:
    case PatRdWrDownInvt:
    case PatRdWrUpAlt:
    case PatRdWrDownAlt:
    case PatRdWrUpInvtAlt:
    case PatRdWrDownInvtAlt:
      AlgoInstruct[0].Data = Cpgc2AlgoInstructionRd.Data;        // Read CMD
      AlgoInstruct[1].Data = Cpgc2AlgoInstructionWr.Data;        // Write CMD
      AlgoInstruct[1].Bits.Last = 1;
      ValidMask = 0x03;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "SetupIOTest: Unknown value for Pattern\n");
      ValidMask = 0x00;
      break;
  }

  switch (CmdPat) {
    case PatRdUpInvt:
    case PatRdDownInvt:
    case PatWrUpInvt:
    case PatWrDownInvt:
    case PatRdWrUpInvt:
    case PatRdWrDownInvt:
    case PatRdWrUpInvtAlt:   // AlgoInstruct[1] Invert_Data not needed
    case PatRdWrDownInvtAlt: // AlgoInstruct[1] Invert_Data not needed
      AlgoInstruct[0].Bits.Invert_Data = 1; // Data Inversion
      break;
    case PatRdWrUpAlt:
    case PatRdWrDownAlt:
      AlgoInstruct[1].Bits.Invert_Data = 1; // Data Inversion
      break;
    default:
      break;
  }

  switch (CmdPat) {
    case PatWrDownInvt:
    case PatRdWrDownInvt:
    case PatRdWrDownInvtAlt:
    case PatRdWrDownAlt:
    case PatRdDown:
    case PatWrDown:
    case PatRdWrDown:
      AlgoInstruct[0].Bits.Inverse_Direction = 1;
      break;
    default:
      // Any up-direction pattern
      break;
  }

  AlgoInstructControl.Data = 0;
  AlgoInstructControl.Bits.Deselect_On = EnCADB;

  AlgoWaitEventControl.Data = 0;
  if (SubSeqWait != 0) {
    AlgoInstructControl.Bits.Wait_Count_Start = 1;
    AlgoInstructControl.Bits.Wait_Count_Clear = 1;
    AlgoWaitEventControl.Bits.Wait_Clock_Frequency = CPGC20_NATIVE_DUNIT_FREQ;
    AlgoWaitEventControl.Bits.Wait_Time = SubSeqWait;
  }
  Cpgc20AlgoInstructWrite (MrcData, &AlgoInstruct[0], AlgoInstructControl, AlgoWaitEventControl, ValidMask);

  // Program Write Data Buffer Related Entries.
  DataInstruct[0].Data = 0;
  DataInstruct[0].Bits.Last = 1;
  DataInstruct[1].Data = DataInstruct[0].Data;
  DataInstruct[2].Data = DataInstruct[0].Data;
  DataInstruct[3].Data = DataInstruct[0].Data;

  Cpgc20DataInstructWrite (MrcData, &DataInstruct[0], 0x1);
}

/**
  This function disables/enables global bind for a specific channel

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Controller to work on
  @param[in]  Channel       - Channel to work on
  @param[in]  IsEnabled     - Determines if we are enabling or disabling global bind
  @retval Nothing.
**/
void
Cpgc20SetGlobalBind (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                IpChannel,
  IN  BOOLEAN               IsEnabled
  )
{
  MC0_REQ0_CR_CPGC_SEQ_CFG_A_STRUCT  CpgcSeqCfgA;
  UINT32  Offset;

  if (MrcControllerExist (MrcData, Controller)) {
    Offset = MrcGetCpgcSeqCfgOffset (Controller, IpChannel);
    CpgcSeqCfgA.Data = MrcReadCR (MrcData, Offset);
    CpgcSeqCfgA.Bits.GLOBAL_START_BIND = (IsEnabled) ? 1 : 0;
    MrcWriteCR (MrcData, Offset, CpgcSeqCfgA.Data);
  }
}

/**
  This function adjusts rank mapping register

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @param[in, out]  RankCount    - Number of Ranks
  @retval Nothing.
**/
void
Cpgc20PrepareRankMapping (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN UINT32                 RankBitMask,
  IN OUT  UINT8            *RankCount
)
{
  MC0_REQ0_CR_CPGC_SEQ_RANK_L2P_MAPPING_A_STRUCT CpgcSeqRankL2PMappingA;
  UINT8     Rank;
  UINT32    IpChannel;
  MrcOutput *Outputs;
  BOOLEAN   IsLpddr;
  UINT32    Offset;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;
  IpChannel             = LP_IP_CH (IsLpddr, Channel);
  CpgcSeqRankL2PMappingA.Data = 0;
  // Prepare Rank Mapping and Max Rank
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if ((MRC_BIT0 << Rank) & RankBitMask) {
      CpgcSeqRankL2PMappingA.Data |= (Rank << (MC0_REQ0_CR_CPGC_SEQ_RANK_L2P_MAPPING_A_L2P_RANK0_MAPPING_WID * (*RankCount)));
      *RankCount += 1;
    }
  }
  // Write New Rank Mapping and Max Rank
  if (!(IS_MC_SUB_CH (IsLpddr, Channel))) {
    Offset = OFFSET_CALC_MC_CH (
              MC0_REQ0_CR_CPGC_SEQ_RANK_L2P_MAPPING_A_REG,
              MC1_REQ0_CR_CPGC_SEQ_RANK_L2P_MAPPING_A_REG, Controller,
              MC0_REQ1_CR_CPGC_SEQ_RANK_L2P_MAPPING_A_REG, IpChannel);
    MrcWriteCR (MrcData, Offset, CpgcSeqRankL2PMappingA.Data);
  }
}

/**
  This function sets REQUEST_DATA_SIZE in Cpgc2AddrSize, The rest of configuration determined later on, as it depends on the test.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  IpChannel         - Channel to work on
  @param[in]  RequestDataSize   - Request Data Size value to set

  @retval Nothing.
**/
void
Cpgc20SetRequestDataSize (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                IpChannel,
  IN  UINT32                RequestDataSizeVal
  )
{
  MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_STRUCT Cpgc2AddrSize;
  UINT32            Offset;

  Cpgc2AddrSize.Data = 0;
  Cpgc2AddrSize.Bits.REQUEST_DATA_SIZE = RequestDataSizeVal;
  Offset = OFFSET_CALC_MC_CH (
    MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_REG,
    MC1_REQ0_CR_CPGC2_ADDRESS_SIZE_REG, Controller,
    MC0_REQ1_CR_CPGC2_ADDRESS_SIZE_REG, IpChannel);
  MrcWriteCR64 (MrcData, Offset, Cpgc2AddrSize.Data);
}

/**
  This function sets maximum number of ranks to test for a block and region

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @param[in]  MaxRank           - Maximum number of Ranks to test for a block and region
  @retval Nothing.
**/
void
Cpgc20SetAddrMaxRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN UINT8                  MaxRank,
  IN OUT  UINT8             *OrigRankCount
  )
{
  MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_STRUCT          Cpgc2AddrSize;
  UINT32  Offset;
  Offset = OFFSET_CALC_MC_CH (
              MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_REG,
              MC1_REQ0_CR_CPGC2_ADDRESS_SIZE_REG, Controller,
              MC0_REQ1_CR_CPGC2_ADDRESS_SIZE_REG, Channel);
  Cpgc2AddrSize.Data = MrcReadCR64 (MrcData, Offset);
  *OrigRankCount = (UINT8)(Cpgc2AddrSize.Bits.Block_Size_Max_Rank + 1);
  Cpgc2AddrSize.Bits.Block_Size_Max_Rank = MaxRank;
  Cpgc2AddrSize.Bits.Region_Size_Max_Rank = MaxRank;
  MrcWriteCR64(MrcData, Offset, Cpgc2AddrSize.Data);
}

/**
  This function sets maximum number of banks to test for a region.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on
  @param[in]  MaxBank           - Maximum number of Banks to test for a region
  @retval Nothing.
**/
void
Cpgc20SetAddrMaxBank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT8                 MaxBank
  )
{
  MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_STRUCT Cpgc2AddrSize;
  UINT32 Offset;
  Offset = OFFSET_CALC_MC_CH (
              MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_REG,
              MC1_REQ0_CR_CPGC2_ADDRESS_SIZE_REG, Controller,
              MC0_REQ1_CR_CPGC2_ADDRESS_SIZE_REG, Channel);
  Cpgc2AddrSize.Data = MrcReadCR64 (MrcData, Offset);
  Cpgc2AddrSize.Bits.Region_Size_Max_Bank = MaxBank;
  MrcWriteCR64 (MrcData, Offset, Cpgc2AddrSize.Data);
}

/**
  This function gets maximum number of ranks to test for a block and region

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on

  @retval # programmed into Block_Size_Max_Rank field.
**/
UINT8
Cpgc20GetAddrMaxRank (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel
  )
{
  MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_STRUCT          Cpgc2AddrSize;
  UINT32  Offset;
  Offset = OFFSET_CALC_MC_CH (
              MC0_REQ0_CR_CPGC2_ADDRESS_SIZE_REG,
              MC1_REQ0_CR_CPGC2_ADDRESS_SIZE_REG, Controller,
              MC0_REQ1_CR_CPGC2_ADDRESS_SIZE_REG, Channel);
  Cpgc2AddrSize.Data = MrcReadCR64 (MrcData, Offset);
  return (UINT8) Cpgc2AddrSize.Bits.Block_Size_Max_Rank;
}

