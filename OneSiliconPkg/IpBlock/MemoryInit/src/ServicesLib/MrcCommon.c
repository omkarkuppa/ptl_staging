/** @file
  Common routines shared across Blue/Green MRC

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

#include "MrcCommon.h"
#include "MrcCommonPrivate.h"
#include "MrcMemoryApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcPostCodes.h"  // for MRC_TURN_AROUND
#include "CMrcStatsTracker.h" // for MrcStatsIncrementData()
#include "MrcGeneral.h"
#include "MrcDdr5.h"
#include "MrcDdr5Ckd.h"
#include "MrcCkdOffsets.h"
#include "MrcPmic.h"

#ifdef XCC_BUILD
#define CR_SPEED_FLAG
#endif

#ifdef CR_SPEED_FLAG
#include "MrcOemPlatform.h"
#endif

#ifdef MRC_DEBUG_PRINT
const char  CcdString[] = "Controller, Channel, Dimm";
const char  RcvEnDelayString[] = "RcvEnDelay";
const char  WrVDelayString[] = "WriteVoltage";
const char  RdVDelayString[] = "ReadVoltage";
const char  DqsDelayString[] = "DqsDelay";
const char  CmdVDelayString[] = "CmdVoltage";
const char  TxDqDelayString[] = "TxDqDelay";
#endif

typedef struct {
  UINT8 PhyClkToCkdVal;
  UINT8 CkdAddress;
  UINT8 Instance;
} PhyClkToCkdDimmGrp;

const TCkdValue Ddr5CKDTable[MAX_DIMMS_IN_SYSTEM][MAX_CKD_PIN] = {
  {{Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}},
  {{Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}},
  {{Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}},
  {{Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}, {Ict80Ohm, SlewRateFast, LightDrive}},
};

/**
  Return the rank mask if the rank exists in the Controller/Channel.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to work on.
  @param[in] Channel    - Channel to work on.
  @param[in] Rank       - Rank to check.

  @retval Bit mask of Rank requested if the Rank exists in the system.
**/
UINT8
MrcRankExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Rank
  )
{
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;

  if ((Controller < MAX_CONTROLLER) && (Channel < Outputs->MaxChannels) && (Rank < MAX_RANK_IN_CHANNEL)) {
    return ((MRC_BIT0 << Rank) & MrcData->Outputs.Controller[Controller].Channel[Channel].ValidRankBitMask);
  } else {
    return 0;
  }
}

/**
  This function checks if the requested byte in the channel exists.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller  to check.
  @param[in] Channel    - Channel to check.
  @param[in] Byte       - Byte to check.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
BOOLEAN
MrcByteExist (
  IN  MrcParameters *const  MrcData,
  IN  const UINT32          Controller,
  IN  const UINT32          Channel,
  IN  const UINT32          Byte
  )
{
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
#ifdef MRC_DEBUG_PRINT
  MrcDebug      *Debug;
#endif // MRC_DEBUG_PRINT

  Outputs  = &MrcData->Outputs;
#ifdef MRC_DEBUG_PRINT
  Debug    = &Outputs->Debug;
#endif // MRC_DEBUG_PRINT

  MRC_DEBUG_ASSERT (Controller < MAX_CONTROLLER, Debug, "Controller is larger than Max Controller.");
  MRC_DEBUG_ASSERT (Channel < MAX_CHANNEL, Debug, "Channel is larger than Max Channel.");

  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];

  return ((1 << Byte) & ChannelOut->ValidByteMask) != 0;
}

/**
  Return the number of ranks in specific dimm.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to work on.
  @param[in] Channel    - Channel to work on.
  @param[in] Dimm       - Dimm in channel to return.

  @retval The number of ranks in the dimm.
**/
UINT8
MrcGetRankInDimm (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          Channel,
  IN const UINT8          Dimm
  )
{
  return MrcData->Outputs.Controller[Controller].Channel[Channel].Dimm[Dimm].RankInDimm;
}

/**
  Returns whether Controller is or is not present.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
BOOLEAN
MrcControllerExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller
  )
{
  if (Controller < MAX_CONTROLLER) {
    if (MrcData->Outputs.Controller[Controller].Status == CONTROLLER_PRESENT) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Returns whether Controller HW exists even though it may not be populated.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcGetHwControllerExists (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT8 MaxChannels = Outputs->MaxChannels;
  BOOLEAN IsDdr5 = Outputs->IsDdr5;

  UINT8 FullControllerMask = IsDdr5 ? 0x03 : 0xF;

  return (MrcGetValidHwChBitMask (MrcData) & (FullControllerMask << (Controller * MaxChannels))) != 0;
}

/**
  Returns whether Channel HW exists even though it may not be populated.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.
  @param[in] Channel    - Channel to test.

  @retval BOOLEAN - TRUE if exists, FALSE otherwise.
**/
extern
MRC_IRAM0_FUNCTION
BOOLEAN
MrcGetHwChannelExists (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT8 MaxChannels = Outputs->MaxChannels;

  return (MrcGetValidHwChBitMask (MrcData) & (1 << (Controller * MaxChannels + Channel))) != 0;
}

/**
  Returns whether Channel is or is not present.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller of the channel.
  @param[in] Channel    - Channel to test.

  @retval TRUE  - if there is at least one enabled DIMM in the channel.
  @retval FALSE - if there are no enabled DIMMs in the channel.
**/
BOOLEAN
MrcChannelExist (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  if ((Channel < MAX_CHANNEL) && (Controller < MAX_CONTROLLER)) {
    if (MrcData->Outputs.Controller[Controller].Channel[Channel].Status == CHANNEL_PRESENT) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Determine if the partition exists with the current channel population.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PartType     - The partition type to look up.
  @param[in]  PartInstance - The partition instance to look up.
  @param[in]  PartChannel  - The partition channel to look up. Only used by the Data
                             Partitions as there are two "bytes" in 1 Data Instance.

  @return whether partition exists or not.
**/
BOOLEAN
MrcGetPartitionExists (
  IN     MrcParameters      *MrcData,
  IN     PARTITION_TYPE     PartType,
  IN     UINT32             PartInstance,
  IN     UINT32             PartChannel
  )
{
  MrcOutput    *Outputs;
  UINT32       Partition2Ch;

  Outputs = &MrcData->Outputs;

  Partition2Ch = MrcGetPartition2ChMask (
    MrcData,
    PartType,
    PartInstance,
    PartChannel
    );

  return ((Outputs->ValidChBitMask & Partition2Ch) != 0);
}

/**
  Determine if the hardware for a partition exists within memory subsytem even though channel may not be populated.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PartType     - The partition type to look up.
  @param[in]  PartInstance - The partition instance to look up.
  @param[in]  PartChannel  - The partition channel to look up. Only used by the Data
                             Partitions as there are two "bytes" in 1 Data Instance.

  @return whether partition exists or not.
**/
BOOLEAN
MrcGetHwPartitionExists (
  IN     MrcParameters      *MrcData,
  IN     PARTITION_TYPE     PartType,
  IN     UINT32             PartInstance,
  IN     UINT32             PartChannel
  )
{
  UINT32       Partition2Ch;

  Partition2Ch = MrcGetPartition2ChMask (
    MrcData,
    PartType,
    PartInstance,
    PartChannel
    );

  return ((MrcGetValidHwChBitMask (MrcData) & Partition2Ch) != 0);
}

/**
  Determine if PHY Channel exists even though channel may not be populated.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  PhyChannel   - PHY channel number.

  @return whether PHY Channel exists or not.
**/
BOOLEAN
MrcGetHwPhyChannelExists (
  IN     MrcParameters      *MrcData,
  IN     UINT32             PhyChannel
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT8 MaxChannels = Outputs->MaxChannels;
  BOOLEAN IsDdr5 = Outputs->IsDdr5;
  UINT32 SysController = PhyChannel / MAX_CHANNEL;
  UINT32 SysChannel = PhyChannel % MAX_CHANNEL;

  if (IsDdr5) {
    SysChannel = SysChannel / MAX_SUB_CHANNEL;
  }

  return ((MrcGetValidHwChBitMask (MrcData) & (1 << (SysController * MaxChannels + SysChannel))) != 0);
}

/**
  This function disable channel parameters.
  After this function the MRC don't use with the channel.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] ChannelToDisable  - Channel to disable.
  @param[in] SkipDimmCapacity  - Switch to skip setting the DimmCapacity to 0 for the dimms in the channel disabled.

  @retval Nothing
**/
void
MrcChannelDisable (
  IN MrcParameters *const MrcData,
  IN const UINT8          ChannelToDisable,
  IN const UINT8          SkipDimmCapacity
  )
{
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  UINT32        Dimm;

  ChannelOut = &MrcData->Outputs.Controller[0].Channel[ChannelToDisable];
  if (ChannelOut->Status == CHANNEL_PRESENT) {
    ChannelOut->Status            = CHANNEL_DISABLED;
    ChannelOut->ValidRankBitMask  = 0;
    for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
      DimmOut = &ChannelOut->Dimm[Dimm];
      if (DimmOut->Status == DIMM_PRESENT) {
        DimmOut->Status     = DIMM_DISABLED;
        DimmOut->RankInDimm = 0;
        if (!SkipDimmCapacity) {
          DimmOut->DimmCapacity = 0;
        }
      }
    }
  }
}

/**
  ECC byte only has 4 bits DQ
  This function check if the bit is:
      - lower nibble (bit 3:0) for MC0, MC1
  @param[in]      Controller  - Current MC
  @param[in]      Byte        - Current Byte
  @param[in]      Bit         - Current Bit
  @retval BOOLEAN - TRUE if current bit is within the nibble of ECC byte, non-ECC byte, or DQS bit
**/
BOOLEAN
MrcBitExist (
  IN    UINT32         Controller,
  IN    UINT32         Byte,
  IN    UINT32         Bit
  )
{
  BOOLEAN    ValidBit = TRUE;

  if (Byte == MRC_DDR5_ECC_BYTE) {
    if (Bit >= MAX_BITS) { //DQS bit
      ValidBit = TRUE;
    } else {
      // MC0 MC1 uses lower nibble
      ValidBit = (Bit < MRC_DDR5_ECC_MAX_BITS) ? TRUE : FALSE;
    }
  }

  return ValidBit;
}

/**
  This function return the ErrStatus Mask based on byte
  ECC bytes only has 4 bit, so return only lower nibble, else return 0xFF
  @param[in]  Byte - Byte to get ErrStatusMask
**/
UINT8
MrcErrStatusMask (
  UINT32 Byte
  )
{
  if (Byte == MRC_DDR5_ECC_BYTE) {
    return 0x0F;
  } else {
    return 0xFF;
  }
}

/**
  This function return the BitMask based on byte
  ECC byte only has 4 bit, so return bitmask of lower nibble
  @param[in]  Byte - Byte to get BitMask
  @param[in]  Bit  - Bit to get BitMask
**/
UINT16
MrcGetBitMask (
  UINT32 Byte,
  UINT32 Bit
  )
{
  UINT16    BitMask;

  if (Byte < MRC_DDR5_ECC_BYTE) {
    BitMask = MRC_BIT0 << Bit;
  } else {
    BitMask = MRC_BIT0 << (Bit % 4);
  }

  return BitMask;
}

/**
  This function return if bit is Start Bit within the byte
  return TRUE if is bit 0 for MC0, MC1
  @param[in]  Controller - Controller to get StartBit
  @param[in]  Byte       - Byte to get StartBit
  @param[in]  Bit        - Bit to check with StartBit
**/
BOOLEAN
MrcIsStartBit (
  UINT32 Controller,
  UINT32 Byte,
  UINT32 Bit
  )
{
  UINT32 StartBit;

  StartBit = 0;
  return (Bit == StartBit);
}

/**
  This function return the ECC DataTrainFeedback Mask based on MC
  ECC byte uses lower nibble for MC0, MC1
  @param[in]  Controller - Controller to get FeedbackMask
**/
UINT32
MrcEccFeedbackMask (
  UINT32 Controller
  )
{
  return FEEDBACKMASK_ECC;
}

/**
  Convert the given frequency and reference clock to a clock ratio.

  @param[in] MrcData   - Pointer to MRC global data.
  @param[in] Frequency - The memory frequency.

  @retval Returns the memory clock ratio.
**/
MrcClockRatio
MrcFrequencyToRatio (
  IN MrcParameters *const  MrcData,
  IN const MrcFrequency    Frequency
  )
{
  const MRC_FUNCTION *MrcCall;
  UINT64 Value;
  UINT64 FreqValue;
  UINT32 RefClkValue;

  MrcCall     = MrcData->Inputs.Call.Func;
  RefClkValue = 33333;
  FreqValue   = MrcCall->MrcMultU64x32 (Frequency, 1000000000ULL);
  Value       = MrcCall->MrcDivU64x64 (FreqValue, (RefClkValue * (BCLK_DEFAULT / 100000)), NULL);
  Value       = ((UINT32) Value + 500) / 1000;
  return ((MrcClockRatio) Value);
}

/**
  Convert the given ratio and reference clocks to a memory frequency.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Ratio   - The memory ratio.

  @retval Returns the memory frequency.
**/
MrcFrequency
MrcRatioToFrequency (
  IN MrcParameters *const  MrcData,
  IN const MrcClockRatio   Ratio
  )
{
  const MrcInput     *Inputs;
  const MRC_FUNCTION *MrcCall;
  MrcClockRatio      LocalRatio;
  UINT64 Value;
  UINT32 RefClkValue;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  LocalRatio  = Ratio;
  RefClkValue = 33333333;
  Value = BCLK_DEFAULT;
  Value = MrcCall->MrcMultU64x32 (Value, LocalRatio);
  Value = MrcCall->MrcMultU64x32 (Value, RefClkValue);
  Value += 50000000000000ULL;
  Value = MrcCall->MrcDivU64x64 (Value, 100000000000000ULL, NULL);
  return ((MrcFrequency) Value);
}

/**
  Convert the given ratio and reference clocks to a memory clock.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Ratio  - The memory ratio.

  @retval Returns the memory clock in femtoseconds.
**/
UINT32
MrcRatioToClock (
  IN MrcParameters *const  MrcData,
  IN const MrcClockRatio   Ratio
  )
{
  const MrcInput     *Inputs;
  const MRC_FUNCTION *MrcCall;
  UINT32  RefClkValue;
  UINT32  Factor;
  UINT64  Value;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  Factor        = BCLK_DEFAULT / 100000UL;
  RefClkValue   = 333333333UL;
  Value         = MrcCall->MrcMultU64x32 (Factor, RefClkValue);
  Value         = MrcCall->MrcMultU64x32 (Value, Ratio);
  Value         = MrcCall->MrcDivU64x64 (Value, (MrcData->Outputs.IsLpddr5 ? 8 : 2), NULL);
  Value = ((Value == 0) ? 0 : (UINT32) UnsignedDivideRoundx64 (MrcData, 10000000000000000000ULL, Value));
  return (UINT32) Value;
}

/**
  This function determines the number of PI ticks required to cover the delay requested.
  This function will ceiling the result such that the PI tick delay may be greater
  than the requested time.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  TimeFs  - The delay in Femtoseconds to convert to PI ticks.

  @retval UINT32 - The number of PI ticks to reach the delay requested.
**/
UINT32
MrcFemtoSec2PiTick (
  IN  MrcParameters *const  MrcData,
  IN  const UINT32          TimeFs
  )
{
  UINT32  DataRateFs;
  UINT32  PiTickFs;
  UINT32  PiCount;

  DataRateFs = MrcData->Outputs.MemoryClock / 2;

  // Pi Tick is 1/64th a Data Rate UI.
  PiTickFs  = UDIVIDEROUND (DataRateFs, 64);
  PiCount   = DIVIDECEIL (TimeFs, PiTickFs);

  return PiCount;
}

/**
  This function return the DIMM number according to the rank number.

  @param[in] Rank - The requested rank.

  @retval The DIMM number.
**/
UINT8
MrcGetDimmFromRank (
  IN const UINT8 Rank
  )
{
  UINT8 Dimm;

  if (Rank == rRank0 || Rank == rRank1) {
    Dimm = dDIMM0;
  } else {
    Dimm = dDIMM1;
  }

  return Dimm;
}

/**
  This function swaps a subfield, within a 32 bit integer value with the specified value.

  @param[in] CurrentValue - 32 bit input value.
  @param[in] NewValue     - 32 bit New value.
  @param[in] Start        - Subfield start bit.
  @param[in] Length       - Subfield length in bits/

  @retval The updated 32 bit value.
**/
UINT32
MrcBitSwap (
  IN UINT32       CurrentValue,
  IN const UINT32 NewValue,
  IN const UINT8  Start,
  IN const UINT8  Length
  )
{
  UINT32 mask;

  // Do bitwise replacement:
  mask = (MRC_BIT0 << Length) - 1;
  CurrentValue &= ~(mask << Start);
  CurrentValue |= ((NewValue & mask) << Start);

  return CurrentValue;
}

/**
  This function shifts a 32 bit int either positive or negative

  @param[in] Value       - Input value to be shifted
  @param[in] ShiftAmount - Number of bits places to be shifted.

  @retval 0 if ShiftAmount exceeds +/- 31.  Otherwise the updated 32 bit value.
**/
UINT32
MrcBitShift (
  IN const UINT32 Value,
  IN const INT8  ShiftAmount
  )
{
  if ((ShiftAmount > 31) || (ShiftAmount < -31)) {
    return 0;
  }

  if (ShiftAmount > 0) {
    return Value << ShiftAmount;
  } else {
    return Value >> (-1 * ShiftAmount);
  }
}

/**
  This function Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7)

  @param[in] CurrentValue - Input value to be shifted
  @param[in] OldMSB       - The original most significant Bit
  @param[in] NewMSB       - The new most significant bit.

  @retval The updated 8 bit value.
**/
UINT16
MrcSE (
  IN UINT16     CurrentValue,
  IN const UINT16 OldMSB,
  IN const UINT16 NewMSB
  )
{
  UINT16 Scratch;

  Scratch = ((MRC_BIT0 << (NewMSB - OldMSB)) - 1) << OldMSB;
  if (CurrentValue >> (OldMSB - 1)) {
    CurrentValue |= Scratch;
  } else {
    CurrentValue &= (~Scratch);
  }

  return CurrentValue;
}

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the log base 2 of input value + 1.
**/
UINT8
MrcLog2 (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  )
{

    //we should never be feeding this function 0 since log2(0) is undefined.
  MRC_DEBUG_ASSERT (Value > 0, &MrcData->Outputs.Debug, "Inputted value is %d, must be greater than 0.", Value);

  UINT8 Log;
  UINT8 Bit;

  Log = 0;
  for (Bit = 0; Bit < 32; Bit++) {
    if (Value & (0x1 << Bit)) {
      Log = (Bit + 1);
    }
  }

  return Log;
}

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the log base 2 of input value + 1 if result is not round number.
**/
UINT8
MrcLog2Round (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  )
{

  //we should never be feeding this function 0 since log2(0) is undefined.
  MRC_DEBUG_ASSERT (Value > 0, &MrcData->Outputs.Debug, "Inputted value is %d, must be greater than 0.", Value);

  UINT8 Log;
  UINT8 Bit;
  UINT8 Ones;

  Log  = 0;
  Ones = 0;

  for (Bit = 0; Bit < 32; Bit++) {
    if (Value & (0x1 << Bit)) {
      Log = Bit;
      Ones++;
    }
  }

  if (Ones > 1) {
    Log += 1;
  }

  return Log;
}

/**
  This function calculates the Log base 2 of the value to a maximum of Bits

  @param[in] MrcData - Include all MRC global data
  @param[in] Value - Input value

  @retval Returns the round down log base 2 of input value.
**/
UINT8
MrcLog2RoundDown (
  IN MrcParameters *MrcData,
  IN const UINT32 Value
  )
{

  //we should never be feeding this function 0 since log2(0) is undefined.
  MRC_DEBUG_ASSERT (Value > 0, &MrcData->Outputs.Debug, "Inputted value is %d, must be greater than 0.", Value);

  UINT8 Log;
  UINT8 Bit;

  Log = 0;
  for (Bit = 0; Bit < 32; Bit++) {
    if (Value & (0x1 << Bit)) {
      Log = Bit;
    }
  }

  return Log;
}

/**
  This function search for item in a list and return index + 1 or 0 in not found

  @param[in] Param        - Param to search for
  @param[in] ParamList    - list of Params
  @param[in] ParamLen     - Params length
  @param[in] Offset       - list of the corresponded Offsets to work on
  @param[out] Override    - override indicator
  @param[out] LocalOffset - localoffset

  @retval Returns result
**/
UINT8
MrcSearchList (
  IN           UINT8   Param,
  IN     const UINT8   *ParamList,
  IN     const UINT8   ParamLen,
  IN     const INT8    *Offset,
  OUT          BOOLEAN *Override,
  OUT          INT8    *LocalOffset
  )
{
  UINT8 Result = 0;
  UINT8 Idx;

  if ((NULL == ParamList) ||
      (NULL == Offset) ||
      (NULL == Override) ||
      (NULL == LocalOffset)) {
    return 0;
  }

  for (Idx = 0; Idx < ParamLen; Idx++) {
    if (ParamList[Idx] == Param) {
      Result = Idx + 1;
      break;
    }
  }
  if (Result == 0) {
    *Override = FALSE;
    *LocalOffset = 0;
  } else {
    *Override = TRUE;
    *LocalOffset = Offset[Result - 1];
  }
  return Result;
}

/**
  ***** Has Buffer Overflow for 68-71, 544-575, 4352-4607, ... ****
  This function calculates the Log base 8 of the Input parameter using integers

  @param[in] Value - Input value

  @retval Returns 10x the log base 8 of input Value
**/
UINT32
MrcLog8 (
  IN UINT32 Value
  )
{
  static const UINT8 Loglook[18] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10 };
  UINT32    Loga;
  UINT32    Rema;

  Loga  = 0;
  Rema  = 2 * Value;
  while (Value > 8) {
    Rema  = Value >> 2;
    Value = Value >> 3;
    Loga += 10;
  };

  return (Loga + Loglook[Rema]); //returns an integer approximation of "log8(a) * 10"
}

/**
  This function Sorts Arr from largest to smallest

  @param[in,out] Arr     - Array to be sorted
  @param[in]     Channel - Channel to sort.
  @param[in]     lenArr  - Length of the array

  @retval Nothing
**/
void
MrcBsortPerChannel (
  IN OUT UINT32   Arr[MAX_CHANNEL][4],
  IN     const UINT8 Channel,
  IN     const UINT8 lenArr
  )
{
  UINT8 i;
  UINT8 j;
  UINT32 temp;

  for (i = 0; i < lenArr - 1; i++) {
    for (j = 0; j < lenArr - (1 + i); j++) {
      if (Arr[Channel][j] < Arr[Channel][j + 1]) {
        temp                = Arr[Channel][j];
        Arr[Channel][j]     = Arr[Channel][j + 1];
        Arr[Channel][j + 1] = temp;
      }
    }
  }
}

/**
  This function calculates the Natural Log of the Input parameter using integers

  @param[in] Input - 100 times a number to get the Natural log from.
                      Max Input Number is 40,000 (without 100x)

  @retval 100 times the actual result. Accurate within +/- 2
**/
UINT32
MrcNaturalLog (
  UINT32 Input
  )
{
  UINT32 Output;

  Output = 0;
  while (Input > 271) {
    Input = (Input * 1000) / 2718;
    Output += 100;
  }

  Output += ((-16 * Input * Input + 11578 * Input - 978860) / 10000);

  return Output;
}

/**
  This function calculates the number of bits set to 1 in a 32-bit value.

  @param[in] Input - The value to work on.

  @retval The number of bits set to 1 in Input.
**/
UINT8
MrcCountBitsEqOne (
  IN UINT32 Input
  )
{
  UINT8 NumOnes;

  NumOnes = 0;
  while (Input > 0) {
    NumOnes++;
    Input &= (Input - 1);
  }

  return NumOnes;
}

/**
  This function calculates e to the power of the Input parameter using integers.

  @param[in] Input - 100 times a number to elevate e to.

  @retval 100 times the actual result. Accurate within +/- 2.
**/
UINT32
Mrceexp (
  IN UINT32 Input
  )
{
  UINT32 Extra100;
  UINT32 Output;

  Extra100  = 0;
  Output    = 1;
  while (Input > 100) {
    Input -= 100;
    Output = (Output * 2718) / 10;
    if (Extra100) {
      Output /= 100;
    }

    Extra100 = 1;
  }

  Output = ((Output * (8 * Input * Input + 900 * Input + 101000)) / 1000);

  if (Extra100) {
    Output /= 100;
  }

  return Output;
}

/**
  This function approximates the Square Root in integer math.  It scales the number by 100
  such that the ones and tens digit are the first two decimal points of the calculation.

  The algorithm implemented is the Babylonian Method for calculating Square Roots.  The
  approximate of the square root is found when the Result of Radicand / X is repeated.
  The algorithm converges by starting with an initial guess of X, and averaging the
  result and remainder of (Radicand / X) and using that average as the next X.

  The largest Radicand that can be calculated is 429496 as the function uses UINT32.

  The hundredths digit may be off by one due to the methodology used. This is acceptable
  for the application this mathematical function is used for.

  @param[in] Radicand - Value to calculate the Square Root of.

  @retval UINT32 - Approximate Square Root
**/
UINT32
MrcSqrt (
  IN UINT32 Radicand
  )
{
  UINT32 Remainder;
  UINT32 Result;
  UINT32 ResultNminus1;
  UINT32 ResultNminus2;
  UINT32 RadicanPrecise;

  // Since we are dealing with an exponential, 100^2 -> 10,000
  RadicanPrecise = Radicand * 10000;
  // Calculate the initial result to be 1/2 the radicand as iteration 0
  Result = RadicanPrecise / 2;
  ResultNminus1 = 0;
  ResultNminus2 = 0;

  // Two historic variables are here to handle the problem of the result
  // oscillating between N and N+1
  while ((Result != ResultNminus1) && (Result != ResultNminus2)) {
    ResultNminus2 = ResultNminus1;
    ResultNminus1 = Result;
    Remainder = RadicanPrecise / Result;
    Result = (Result + Remainder) / 2;
  };

  return Result;
}

/**
  This function writes a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
UINT32
MrcWriteCrMulticast (
  IN MrcParameters *const MrcData,
  IN const UINT32         Offset,
  IN const UINT32         Value
  )
{
#ifdef CR_SPEED_FLAG
  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);
  MrcOemMmioWrite32 (Offset, Value);
  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);
#else
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarWriteCount++;

  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);

  MrcCall->MrcMmioWrite32 (Inputs->MchBarBaseAddress + Offset, Value);

  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);

  MRC_DEBUG_ASSERT ((Offset % 4) == 0, Debug, "Invalid Offset alignment: 0x%x\n", Offset);
#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_WRITE] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X M %08X</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
#endif // CR_SPEED_FLAG
  return (Value);
}

/**
  This function writes a 64 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
UINT64
MrcWriteCR64 (
  IN MrcParameters *const MrcData,
  IN const UINT32         Offset,
  IN const UINT64         Value
  )
{
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarWriteCount++;

  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);

  MrcCall->MrcMmioWrite64 (Inputs->MchBarBaseAddress + Offset, Value);

  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);

  MRC_DEBUG_ASSERT ((Offset % 8) == 0, Debug, "Invalid Offset alignment: 0x%x\n", Offset);
#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_WRITE] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X W %llX</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
  return (Value);
}

/**
  This function writes a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - Value to write.

  @retval The register value.
**/
UINT32
MrcWriteCR (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT32  Value
  )
{
#ifdef CR_SPEED_FLAG
  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);
  MrcOemMmioWrite32 (Offset, Value);
  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);
#else
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarWriteCount++;

  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);

  MrcCall->MrcMmioWrite32 (Inputs->MchBarBaseAddress + Offset, Value);

  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);

  MRC_DEBUG_ASSERT ((Offset % 4) == 0, Debug, "Invalid Offset alignment: 0x%x\n", Offset);
#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_WRITE] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X W %08X</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
#endif // CR_SPEED_FLAG
  return (Value);
}

/**
  This function writes a 8 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.
  @param[in] Value   - The value to write.

  @retval The register value.
**/
UINT8
MrcWriteCR8 (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const UINT8   Value
  )
{
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarWriteCount++;

  MrcStatsIncrementData (MrcData, MRC_WRITE_CR);
  MrcStatsStartTimer (MrcData, WRITE_CR_TIME);

  MrcCall->MrcMmioWrite8 (MrcData->Inputs.MchBarBaseAddress + Offset, Value);

  MrcStatsEndTimer (MrcData, WRITE_CR_TIME);

#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_WRITE] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X W %02X</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
  return (Value);
}

/**
  This function reads a 64 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.

  @retval Value read from the register.
**/
UINT64
MrcReadCR64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset
  )
{
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;
  UINT64             Value;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarReadCount++;

  MrcStatsIncrementData (MrcData, MRC_READ_CR);
  MrcStatsStartTimer (MrcData, READ_CR_TIME);

  Value = (MrcCall->MrcMmioRead64 (Inputs->MchBarBaseAddress + Offset));

  MrcStatsEndTimer (MrcData, READ_CR_TIME);

  MRC_DEBUG_ASSERT ((Offset % 8) == 0, Debug, "Invalid Offset alignment: 0x%x\n", Offset);
#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_READ] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X R %llX</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
  return (Value);
}

/**
  This function reads a 32 bit register.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - Offset of register from MchBar Base Address.

  @retval Value read from the register
**/
UINT32
MrcReadCR (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset
  )
{
  UINT32 Value;

#ifdef CR_SPEED_FLAG
  MrcStatsIncrementData (MrcData, MRC_READ_CR);
  MrcStatsStartTimer (MrcData, READ_CR_TIME);
  Value = MrcOemMmioRead32 (Offset);
  MrcStatsEndTimer (MrcData, READ_CR_TIME);
#else
  const MRC_FUNCTION *MrcCall;
  const MrcInput     *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Outputs->MchBarReadCount++;

  MrcStatsIncrementData (MrcData, MRC_READ_CR);
  MrcStatsStartTimer (MrcData, READ_CR_TIME);

  Value = MrcCall->MrcMmioRead32 (Inputs->MchBarBaseAddress + Offset);

  MrcStatsEndTimer (MrcData, READ_CR_TIME);

  MRC_DEBUG_ASSERT ((Offset % 4) == 0, Debug, "Invalid Offset alignment: 0x%x\n", Offset);
#ifdef MRC_DEBUG_PRINT
  if (Debug->PostCode[MRC_POST_CODE_READ] == Debug->PostCode[MRC_POST_CODE]) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "<mc>%04X R %08X</mc>\n", Offset, Value);
  }
#endif // MRC_DEBUG_PRINT
#endif // CR_SPEED_FLAG
  return (Value);
}

/**
  Wait for at least the given number of nanoseconds.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] DelayNs  - time to wait in [ns], up to 2^32 [ns] = 4.29 seconds

  @retval Nothing
**/
void
MrcWait (
  IN MrcParameters *const MrcData,
  IN UINT32               DelayNs
  )
{
  const MRC_FUNCTION *MrcCall;

  if (DelayNs == 0) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "%sMrcWait(%dns)\n", gErrString, DelayNs);
    return;
  }

  MrcStatsAddToData (MrcData, MRC_WAIT_NS, DelayNs);
  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcDelayNs (MrcData, DelayNs);
}

/**
  Wait for at least the given number of DRAM clocks.
  The actual delay time depends in the current DRAM clock period.

  @param[in] MrcData   - Include all MRC global data.
  @param[in] DelayTck  - time to wait in clocks, up to 2^32 [ns] = 4.29 seconds

  @retval Nothing
**/
extern
void
MrcWaitClk (
  IN MrcParameters *const MrcData,
  IN UINT32               DelayTck
  )
{
  UINT32 DelayNs;

  DelayNs = (DelayTck * MrcData->Outputs.MemoryClock);
  DelayNs = DIVIDECEIL (DelayNs, FEMTOSECONDS_PER_NANOSECOND);

  MrcWait (MrcData, DelayNs);
}

/**
  Convert # of femto seconds to # of tCK

  @param[in] MrcData        - Include all MRC global data.
  @param[in] TimeInFemto    - Time to convert to tCK.

  @retval The # of tCK.
**/
UINT32
MrcFemtoTimeToTCK (
  IN  MrcParameters *const MrcData,
  IN  UINT32               TimeInFemto
  )
{
  MrcOutput        *Outputs;
  MrcFrequency     Frequency;
  UINT32           Value;
  UINT32           FrequencyInFemto;

  Outputs = &MrcData->Outputs;

  Frequency = Outputs->Frequency;

  Frequency /= 2;

  FrequencyInFemto = 1000000000 / Frequency;

  Value = DIVIDECEIL (TimeInFemto, FrequencyInFemto);

  return Value;
}


/**
  This function converts from the MR address to the index needed to access this MR
  inside the storage array in MrcData.

  @param[in]      MrcData   - Pointer to global MRC Data.
  @param[in, out] MrAddress - Pointer to MR address to translate.

  @retval UINT8 - The index to use in the MR array in MrcData or 0xFF if unsupported.
**/
UINT8
MrcMrAddrToIndex (
  IN      MrcParameters   *MrcData,
  IN OUT  MrcModeRegister *MrAddress
  )
{
  UINT8 MrIndex;

  switch (*MrAddress) {
    case mrMR0:
    case mrMR1:
    case mrMR2:
    case mrMR3:
    case mrMR4:
    case mrMR5:
    case mrMR6:
    case mrMR7:
    case mrMR8:
      MrIndex = *MrAddress;
      break;

    // MR10-12 start at array index 8 (Right after MR8)
    case mrMR10:
    case mrMR11:
    case mrMR12:
      MrIndex = (UINT8) *MrAddress - (mrMR10 - mrIndexMR10);
      break;

      // MR13-30 start at array index 12 (Right after mrIndexMR12Upper)
    case mrMR13:
    case mrMR14:
    case mrMR15:
    case mrMR16:
    case mrMR17:
    case mrMR18:
    case mrMR19:
    case mrMR20:
    case mrMR21:
    case mrMR22:
    case mrMR23:
    case mrMR24:
    case mrMR25:
    case mrMR26:
    case mrMR27:
    case mrMR28:
    case mrMR29:
    case mrMR30:
      MrIndex = (UINT8) *MrAddress - (mrMR13 - mrIndexMR13);
      break;

    case mrMR34:
    case mrMR35:
    case mrMR36:
    case mrMR37:
    case mrMR38:
    case mrMR39:
    case mrMR40:
    case mrMR41:
    case mrMR42:
      MrIndex = (UINT8) *MrAddress - (mrMR34 - mrIndexMR34);
      break;

    case mrMR45:
      MrIndex = mrIndexMR45;
      break;

    case mrMR46:
      MrIndex = mrIndexMR46;
      break;

    case mrMR48:
      MrIndex = mrIndexMR48;
      break;

    case mrMR50:
      MrIndex = mrIndexMR50;
      break;

    case mrMR57:
      MrIndex = mrIndexMR57;
      break;

    case mrMR58:
      MrIndex = mrIndexMR58;
      break;

    case mrMR59:
      MrIndex = mrIndexMR59;
      break;

    case mrMR69:
      MrIndex = mrIndexMR69;
      break;

    case mrMR75:
      MrIndex = mrIndexMR75;
      break;

    case mrMR127:
      MrIndex = 0;  // Dummy index, we do not store any MR data for this one
      break;
    case mrMR111:
      MrIndex = mrIndexMR111;
      break;

    // Becase we only train DIMM DFE per rank, we don't need to store per-bit DFE values in the host struct, they are all the same
    // Use DQL bit [0] Tap1..4 values for all bits: MR129..132
    case mrMR129:
    case mrMR137:
    case mrMR145:
    case mrMR153:
    case mrMR161:
    case mrMR169:
    case mrMR177:
    case mrMR185:
      MrIndex = mrIndexMR129; // DFE Tap1 DQL bit [0]
      break;
    case mrMR193:
    case mrMR201:
    case mrMR209:
    case mrMR217:
    case mrMR225:
    case mrMR233:
    case mrMR241:
    case mrMR249:
      MrIndex = mrIndexMR193; // DFE Tap1 DQU bit [0]
      break;
    case mrMR130:
    case mrMR138:
    case mrMR146:
    case mrMR154:
    case mrMR162:
    case mrMR170:
    case mrMR178:
    case mrMR186:
      MrIndex = mrIndexMR130; // DFE Tap2 DQL bit [0]
      break;
    case mrMR194:
    case mrMR202:
    case mrMR210:
    case mrMR218:
    case mrMR226:
    case mrMR234:
    case mrMR242:
    case mrMR250:
      MrIndex = mrIndexMR194; // DFE Tap2 DQU bit [0]
      break;
    case mrMR131:
    case mrMR139:
    case mrMR147:
    case mrMR155:
    case mrMR163:
    case mrMR171:
    case mrMR179:
    case mrMR187:
      MrIndex = mrIndexMR131; // DFE Tap3 DQL bit [0]
      break;
    case mrMR195:
    case mrMR203:
    case mrMR211:
    case mrMR219:
    case mrMR227:
    case mrMR235:
    case mrMR243:
    case mrMR251:
      MrIndex = mrIndexMR195; // DFE Tap3 DQU bit [0]
      break;
    case mrMR132:
    case mrMR140:
    case mrMR148:
    case mrMR156:
    case mrMR164:
    case mrMR172:
    case mrMR180:
    case mrMR188:
      MrIndex = mrIndexMR132; // DFE Tap4 DQL bit [0]
      break;
    case mrMR196:
    case mrMR204:
    case mrMR212:
    case mrMR220:
    case mrMR228:
    case mrMR236:
    case mrMR244:
    case mrMR252:
      MrIndex = mrIndexMR196; // DFE Tap4 DQU bit [0]
      break;

    // MPC command indexes
    case mpcMR13:
    case mpcMR32a0:
    case mpcMR32a1:
    case mpcMR32b0:
    case mpcMR32b1:
    case mpcMR33a0:
    case mpcMR33:
    case mpcMR33b0:
    case mpcMR34:
    case mpcApplyVrefCa:
    case mpcDllReset:
    case mpcZqCal:
    case mpcEnterCaTrainMode:
    case mpcSetCmdTiming:
    case mpcSelectAllPDA:
      MrIndex = (UINT8) *MrAddress - (mpcMR13 - mrIndexMpcMr13);
      break;

    case mpcZqLat:
      MrIndex = mrIndexMpcZqLat;
      break;

    // Handle ByteMode for LP5 CA Vref.  Return actual MR number
    case mrMR12b:
      MrIndex = mrIndexMR12Upper;
      *MrAddress = mrMR12;
      break;

    case mrMR16FspOp:
      MrIndex = mrIndexMR16;
      *MrAddress = mrMR16;
      break;

    case mrMR28ZqReset:
    case mrMR28ZqStop:
      MrIndex = mrIndexMR28;
      *MrAddress = mrMR28;
      break;

    case mrREFab:
      MrIndex = 0;  // Dummy index, we do not store any MR data for this one
      break;

    case mrPreAll:
      MrIndex = 0;  // Dummy index, we do not store any MR data for this one
      break;


    default:
      MrIndex = 0xFF;
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Untracked MR %d", gErrString, *MrAddress);
      break;
  }

  return MrIndex;
}


/**
  This function calculates the integer approximate for the mean and standard deviation
  from the list of values.

  It uses the MrcSqrt() function which will return with 2 decimal points of precision
  in the Ones and Tens digits.  Therefore, this function will return with the same
  precision.

  @param[in] MrcData        - Pointer to global MRC data.
  @param[in] ValueList      - Input list for calculation
  @param[in] ListLen        - Length of input list
  @param[out,optional] Mean - Pointer to store Mean.

  @retval UINT32 - Integer approximate standard deviation.
**/
UINT32
MrcStd (
  IN MrcParameters *MrcData,
  IN const INT32 ValueList[],
  IN UINT8 ListLen,
  OUT OPTIONAL INT32 *Mean
  )
{
  //MrcDebug *Debug;
  INT32 Idx;
  UINT32 Std;
  INT32 lMean;
  INT32 DeviationSquared;
  INT32 SumOfDeviation;

  if (ListLen == 0) {
    return 0;
  }

  //Debug = &MrcData->Outputs.Debug;

  // Calculate the mean
  lMean = 0;
  for (Idx = 0; Idx < ListLen; Idx++) {
    lMean += ValueList[Idx];
  }
  lMean /= ListLen;
  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mean: %d\n", lMean);

  // Calculate the sum of deviations divided by the mean
  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ListVal\tDev\tSqrd\n");
  SumOfDeviation = 0;
  for (Idx = 0; Idx < ListLen; Idx++) {
    DeviationSquared = ValueList[Idx] - lMean;
    //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%d:\t%d\t", Idx, DeviationSquared);
    DeviationSquared *= DeviationSquared;
    //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%d\n", DeviationSquared);
    SumOfDeviation += DeviationSquared;
  }
  SumOfDeviation /= Idx;
  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SumOfDeviation: %d\n", SumOfDeviation);

  // Calculate the standard devieation
  Std = MrcSqrt (SumOfDeviation);
  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Standard Deviation: %d\n", Std);

  if (Mean != NULL) {
    *Mean = lMean;
  }
  return Std;
}

/**
  Returns the bit position of the lowest bit set in a 32-bit value.

  This function computes the bit position of the lowest bit set in the 32-bit
  value specified by Operand. If Operand is zero, then -1 is returned.
  Otherwise, a value between 0 and 31 is returned.

  @param  Operand The 32-bit operand to evaluate.

  @retval 0..31  The lowest bit set in Operand was found.
  @retval -1     Operand is zero.

**/
INT32
MrcLowBitSet32 (
  IN      UINT32  Operand
  )
{
  INT32  BitIndex;

  if (Operand == 0) {
    return -1;
  }

  for (BitIndex = 0; 0 == (Operand & 1); BitIndex++, Operand >>= 1) {
  }

  return BitIndex;
}

/**
  Determines which is the first populated Rank of each channel

   @param[in]  MrcData   - Pointer to the MRC global data.
   @param[out] FirstRank - Structure to hold which rank is first populated of each channel
**/
void
GetFirstRank (
  IN  MrcParameters *const MrcData,
  OUT UINT8         FirstRank[MAX_CONTROLLER][MAX_CHANNEL]
  )
{
  MrcOutput           *Outputs;
  MrcControllerOut    *ControllerOut;
  UINT32 Controller;
  UINT32 Channel;
  UINT8  Rank;
  UINT8  MaxChannels;

  Outputs = &MrcData->Outputs;
  MaxChannels = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannels; Channel++) {
       // Initalize FirstRank to 0
       FirstRank[Controller][Channel] = 0;
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if ((1 << Rank) & ControllerOut->Channel[Channel].ValidRankBitMask) {  // Pick one rank to get the results from
          FirstRank[Controller][Channel] = Rank;
          break;
        }
      } // Rank
    } // Channel
  } // Controller
}

/**
  Determines which is the last populated Rank of each channel

   @param[in]  MrcData   - Pointer to the MRC global data.
   @param[out] LastRank  - Array to hold which rank is last populated of each channel
**/
void
GetLastRank (
  IN  MrcParameters *const MrcData,
  OUT UINT8         LastRank[MAX_CONTROLLER][MAX_CHANNEL]
  )
{
  MrcOutput           *Outputs;
  UINT32 Controller;
  UINT32 Channel;
  UINT8  Rank;
  UINT8  MaxChannels;

  Outputs = &MrcData->Outputs;
  MaxChannels = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
       // Initalize FirstRank to 0
      LastRank[Controller][Channel] = 0;
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
          LastRank[Controller][Channel] = Rank;
        }
      } // Rank
    } // Channel
  } // Controller
}

/**
  This function determines the Non Target Rank values, including the Non Target Dimm Rank 0 and 1 and
  Target Dimm Non Target Rank.

  @param[in]      Rank               - Rank to check
  @param[in]      NtRankSelect       - Holds the Non Target Rank values

  @retval None
**/
VOID
GetNtRankSelection (
  IN  UINT32 Rank,
  OUT UINT8  NtRankSelect[MaxNtOdt]
  )
{
  UINT8 TarDimm;
  UINT8 NtDimm;

  // Determine if Non Target ODT is supported on any Rank/Dimm
  TarDimm = (UINT8) RANK_TO_DIMM_NUMBER (Rank);
  NtDimm = 1 - TarDimm;
  NtRankSelect[NtDimmR0] = NtDimm * MAX_RANK_IN_DIMM;
  NtRankSelect[NtDimmR1] = NtRankSelect[NtDimmR0] + 1;
  NtRankSelect[TargetDimmNtRank] = ((Rank + 1) % MAX_RANK_IN_DIMM) + (TarDimm * MAX_RANK_IN_DIMM);
}

/**
  This function determines if multicasting should be used based on input parameters.
  This function returns through parameters the start and stop range of Socket, Controller, Channel, Rank, Strobe, Lane based on if the input paramters suggest multicating should be used
  The *Range input paramters are optional since not all cases may need to use them all. If the *Range paramters are NULL, than they will not be programmed in the function
  If the 'Group" parameter is not needed, input GsmGtMax

  @param[in]      MrcData          - Pointer to global data structure.
  @param[in]      Socket           - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller       - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel          - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank             - Rank number within a channel (0-based).
  @param[in]      Strobe           - Dqs data group within the rank (0-based).
  @param[in]      Lane             - Lane index within the GSM_GT group (0-based).
  @param[out]     SocketRange      - Start/Stop sweep range for Socket
  @param[out]     ControllerRange  - Start/Stop sweep range for Controller
  @param[out]     ChannelRange     - Start/Stop sweep range for Channel
  @param[out]     StrobeRange      - Start/Stop sweep range for Strobe
  @param[out]     LaneRange        - Start/Stop sweep range for Lane

  @param[out] MulticastAccess - TRUE if multicasting is desired
**/
BOOLEAN
MrcDetectMulticast (
  IN      MrcParameters *const  MrcData,
  IN      GSM_GT         const  Group,
  IN      UINT32                Socket,
  IN      UINT32                Controller,
  IN      UINT32                Channel,
  IN      UINT32                Rank,
  IN      UINT32                Strobe,
  IN      UINT32                Lane,
  OUT     MRC_RANGE             *SocketRange,       OPTIONAL
  OUT     MRC_RANGE             *ControllerRange,   OPTIONAL
  OUT     MRC_RANGE             *ChannelRange,      OPTIONAL
  OUT     MRC_RANGE             *RankRange,         OPTIONAL
  OUT     MRC_RANGE             *StrobeRange,       OPTIONAL
  OUT     MRC_RANGE             *LaneRange          OPTIONAL
  )
{
  MrcOutput         *Outputs;
  BOOLEAN           MulticastAccess;
  UINT32            RankMax;
  UINT32            StrobeMax;
  UINT32            LaneMax;

  Outputs = &MrcData->Outputs;
  LaneMax = MAX_BITS;
  RankMax = MAX_RANK_IN_CHANNEL;
  StrobeMax = MAX_SDRAM_IN_DIMM;

  MrcGetSetParamMaxAdjust (MrcData, Group, Strobe, &RankMax, &StrobeMax, &LaneMax);

    // Detect and convert all Multicast accesses into unicast.
  MulticastAccess = FALSE;
  if (SocketRange != NULL) {
    if ((Socket != MRC_IGNORE_ARG) && (Socket >= MAX_CPU_SOCKETS)) {
      MulticastAccess = TRUE;
      SocketRange->Start = 0;
      SocketRange->End = MAX_CPU_SOCKETS - 1;
    } else {
      SocketRange->Start = Socket;
      SocketRange->End = Socket;
    }
  }

  if (ControllerRange != NULL) {
    if ((Controller != MRC_IGNORE_ARG) && (Controller >= MAX_CONTROLLER)) {
      MulticastAccess = TRUE;
      ControllerRange->Start = 0;
      ControllerRange->End = MAX_CONTROLLER - 1;
    } else {
      ControllerRange->Start = Controller;
      ControllerRange->End = Controller;
    }
  }

  if (ChannelRange != NULL) {
    if ((Channel != MRC_IGNORE_ARG) && (Channel >= (Outputs->MaxChannels))) {
      MulticastAccess = TRUE;
      ChannelRange->Start = 0;
      ChannelRange->End = (UINT32) (Outputs->MaxChannels) - 1;
    } else {
      ChannelRange->Start = Channel;
      ChannelRange->End = Channel;
    }
  }

  if (RankRange != NULL) {
    if ((Rank != MRC_IGNORE_ARG) && (Rank >= RankMax)) {
      MulticastAccess = TRUE;
      RankRange->Start = 0;
      RankRange->End = MAX_RANK_IN_CHANNEL - 1;
    } else {
      RankRange->Start = Rank;
      RankRange->End = Rank;
    }
  }

  if (StrobeRange != NULL) {
    if ((Strobe != MRC_IGNORE_ARG) && (Strobe >= StrobeMax)) {
      MulticastAccess = TRUE;
      StrobeRange->Start = 0;
      StrobeRange->End = MrcData->Outputs.SdramCount - 1;
    } else {
      StrobeRange->Start = Strobe;
      StrobeRange->End = Strobe;
    }
  }

  if (LaneRange != NULL) {
    if ((Lane != MRC_IGNORE_ARG) && (Lane >= LaneMax)) {
      MulticastAccess = TRUE;
      LaneRange->Start = 0;
      LaneRange->End = LaneMax - 1;
    } else {
      LaneRange->Start = Lane;
      LaneRange->End = Lane;
    }
  }
  return MulticastAccess;
}

#ifdef MRC_DEBUG_PRINT
/**
  This step prints outs param with correct spacing

  @param[in]  GetSetDecValue - Value to print.
  @param[in]  Debug       - MRC global Debug data.

  @retval Nothing
**/
void
PrintParamSpacing (
   IN  INT64        GetSetDecValue,
   IN  MrcDebug     *Debug
  )
{
  UINT32 NumberOfSpaces;

  if (GetSetDecValue <=-100) {
    NumberOfSpaces = 0;
  }
  else if (GetSetDecValue <= -10) {
    NumberOfSpaces = 1;
  }
  else if (GetSetDecValue <= -1) {
    NumberOfSpaces = 2;
  }
  else if (GetSetDecValue < 10) {
    NumberOfSpaces = 3;
  }
  else if (GetSetDecValue < 100) {
    NumberOfSpaces = 2;
  }
  else if (GetSetDecValue  < 1000) {
    NumberOfSpaces = 1;
  } else {
    NumberOfSpaces = 0;
  }

  while (NumberOfSpaces--) {
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, " ");
  }
  MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "%lld", GetSetDecValue);
}

/**
  This function prints outs param by scope

  @param[in] MrcData - Include all MRC global data.
  @param[in] GroupTypeIndex - group type to be read and printed.
  @param[in] Scope   - group type, and its scope.

  @retval Nothing
**/
void
MrcReadPrintByScope (
  IN  MrcParameters*    MrcData,
  IN  GSM_GT            GroupTypeIndex,                ///< group type to be read and printed
  IN  GROUP_SCOPE_TYPE  Scope                          ///< group type granularity
  )
{
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 Byte;
  INT64 GetSetDummy;
  UINT32 MaxChannel;
  UINT32 SdramCount;
  UINT32 CmdGroup;
  UINT32 CmdGroupMax;

  MaxChannel = MrcData->Outputs.MaxChannels;
  SdramCount = MrcData->Outputs.SdramCount;
  CmdGroupMax = MrcGetCmdGroupMax (MrcData);

  switch (Scope) {
    case GroupScopePerMc:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        if (MrcControllerExist (MrcData, Controller)) {
          MrcGetSetMc (MrcData, Controller, GroupTypeIndex, ReadNoCache | PrintValue, &GetSetDummy);
        }
      }
      break;
    case GroupScopePerMcCh:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH(MrcData->Outputs.IsLpddr, Channel)) {
            // For LPDDR5, only program register on even channels.
            continue;
          }
          MrcGetSetMcCh (MrcData, Controller, Channel, GroupTypeIndex, ReadNoCache | PrintValue, &GetSetDummy);
        }
      }
      break;
    case GroupScopePerMcChRank:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
              MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GroupTypeIndex,
                ReadNoCache | PrintValue, &GetSetDummy);
            }
          }
        }
      }
      break;
    case GroupScopePerCcc:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
              MrcGetSetCcc (MrcData, Controller, Channel, Rank, MRC_IGNORE_ARG, GroupTypeIndex,
                ReadNoCache | PrintValue, &GetSetDummy);
            }
          }
        }
      }
      break;
    case GroupScopePerCccNoRank:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          if (!MrcChannelExist (MrcData, Controller, Channel)) {
            continue;
          }
          MrcGetSetCcc (MrcData, Controller, Channel, MRC_IGNORE_ARG, MRC_IGNORE_ARG, GroupTypeIndex,
            ReadNoCache | PrintValue, &GetSetDummy);
        }
      }
      break;
    case GroupScopePerCccLane:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
              MrcGetSetCccLane (MrcData, Controller, Channel, MRC_IGNORE_ARG, Rank, GroupTypeIndex,
                ReadNoCache | PrintValue, &GetSetDummy);
            }
          }
        }
      }
      break;
    case GroupScopePerDdrioCmdGroup:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          if (!MrcChannelExist (MrcData, Controller, Channel)) {
            continue;
          }
          for (CmdGroup = 0; CmdGroup < CmdGroupMax; CmdGroup++) {
            MrcGetSetCcc (MrcData, Controller, Channel, MRC_IGNORE_ARG, CmdGroup, GroupTypeIndex,
              ReadNoCache | PrintValue, &GetSetDummy);
          }
        }
      }
      break;
    case GroupScopePerChStrobe:
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (MrcData->Outputs.IsLpddr, Channel)) {
            // For LPDDR5, only program register on even channels.
            continue;
          }
          for (Byte = 0; Byte < SdramCount; Byte++) {
            MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GroupTypeIndex,
              ReadNoCache | PrintValue, &GetSetDummy);
          }
        }
      }
      break;
    case GroupScopePerNoScope:
      MrcGetSetNoScope (MrcData, GroupTypeIndex, ReadNoCache | PrintValue, &GetSetDummy);
      break;
    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "\n%s - %d Scope is not implemented\n",__func__ , Scope);
      break;
  }
}

/**
  Function that checks if the input GetSet enumeratior is supported on the current system configuration.

  @param[in]  MrcData      - Include all MRC global data.
  @param[in]  GetSetParam  - The GetSet enumerator to check

  @retval TRUE  - The input GetSetParam enumerator is supported
  @retval FALSE - The input GetSetParam enumerator is not supported
**/
BOOLEAN
IsParameterSupported (
  IN  MrcParameters* MrcData,
  IN  GSM_GT         GetSetParam
  )
{
  MrcOutput    *Outputs;
  BOOLEAN      IsSupported;

  Outputs = &MrcData->Outputs;
  IsSupported = TRUE;

  if (Outputs->IsDdr5) {
    switch (GetSetParam) {
      case WckGrpPi:
        IsSupported = FALSE;
        break;

      default:
        break;
    }
  }
  return IsSupported;
}

/**
  This step prints out the paramaters passed in through paramlist

  @param[in]  MrcData              - Include all MRC global data.

  @param[in]  String               - Text for Tag.
  @param[in]  ParamList            - list of GSM_GT to print.
  @param[in]  NumberOfParameters   - number of GSM_GT to print.

  @retval Nothing
**/
void
MrcPrintParameters (
  IN  MrcParameters* MrcData,
  IN  const char*    String,
  IN  const GSM_GT*  ParamList,
  IN  const UINT32   NumberOfParameters,
  IN MrcPostCode     PostCode
  )
{
  MrcDebug     *Debug;
  MrcOutput    *Outputs;
  GROUP_SCOPE_TYPE Scope;
  INT64        GetSetDummy;
  UINT32       ParamIdx;
  UINT32       Controller;
  UINT32       Channel;
  UINT32       Rank;
  UINT32       Byte;
  UINT32       Bit;
  UINT8        MaxChannels;
  UINT32       NumberOfParams = NumberOfParameters;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MaxChannels = Outputs->MaxChannels;

  if (NumberOfParams > 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s%s\n", gStartTagStr, String);
    for (ParamIdx = 0; ParamIdx < NumberOfParams; ParamIdx++) {
      if (!IsParameterSupported (MrcData, ParamList[ParamIdx])) {
        continue;
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s%s\n", gStartTagStr, GsmGtDebugStrings[ParamList[ParamIdx]]);
      Scope = MrcGetScope (MrcData, ParamList[ParamIdx]);
      if (Scope == GroupScopePerStrobe) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Byte\t\t");
        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "   %d", Byte);
        }
      } else if (Scope == GroupScopePerBit) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Bit\t\t");
        for (Bit = 0; Bit < MAX_BITS; Bit++) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "   %d", Bit);
        }
      } else {
        MrcReadPrintByScope (MrcData, ParamList[ParamIdx], Scope);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s%s\n", gStopTagStr, GsmGtDebugStrings[ParamList[ParamIdx]]);
        continue;
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          if ((PostCode == MRC_TURN_AROUND) && ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel))) {
            continue;
          }
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
              continue;
            }
            if (Scope == GroupScopePerStrobe) {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%d.C%d.R%d:\t", Controller, Channel, Rank);
              for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, ParamList[ParamIdx],
                  ReadNoCache, &GetSetDummy);
                PrintParamSpacing (GetSetDummy, Debug);
              }
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
            } else if (Scope == GroupScopePerBit) {
              for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%d.C%d.R%d.B%d:\t", Controller, Channel, Rank, Byte);
                for (Bit = 0; Bit < MAX_BITS; Bit++) {
                  MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, ParamList[ParamIdx],
                    ReadNoCache, &GetSetDummy);
                  PrintParamSpacing (GetSetDummy, Debug);
                }
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
              }
            }
          }
        }
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s%s\n", gStopTagStr, GsmGtDebugStrings[ParamList[ParamIdx]]);
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s%s\n\n", gStopTagStr, String);
  }
}

/**
  This step prints out the Mrs according the type passed through MrType

  @param[in]  MrcData - Include all MRC global data.
  @param[in]  String  - Text for Tag.
  @param[in]  MrType  - MR type to print.

  @retval Nothing
**/
void
MrcPrintMR (
  IN  MrcParameters* MrcData,
  IN  const char*    String,
  IN  MR_TYPE        MrType
  )
{
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  const MrcModeRegister *pArr = NULL;
  int NumOfElements = 0;
  int i;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  static const MrcModeRegister  Lpddr5MrWrVIndex[] =     { mrMR14, mrMR15 };
  static const MrcModeRegister  Lpddr5MrCmdVIndex[] =    { mrMR12 };
  static const MrcModeRegister  Lpddr5MrCaODTIndex[] =   { mrMR11 };
  static const MrcModeRegister  Lpddr5MrDimmRonIndex[] = { mrMR3  };
  static const MrcModeRegister  Ddr5MrWrVIndex[] =       { mrMR10 };
  static const MrcModeRegister  Ddr5MrCmdVIndex[] =      { mrMR11, mrMR12 };
  static const MrcModeRegister  Ddr5MrCaODTIndex[] =     { mpcMR32a0,
                                                           mpcMR32a1,
                                                           mpcMR32b0,
                                                           mpcMR32b1,
                                                           mpcMR33a0,
                                                           mpcMR33,
                                                           mpcMR33b0 };
  static const MrcModeRegister  Ddr5MrDimmRonIndex[] =   { mrMR5 };

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s%s\n", gStartTagStr, String);

  if (Outputs->IsLpddr5) {
    switch (MrType) {
      case MrWrV:
        pArr = Lpddr5MrWrVIndex;
        NumOfElements = ARRAY_COUNT (Lpddr5MrWrVIndex);
        break;

      case MrCmdV:
        pArr = Lpddr5MrCmdVIndex;
        NumOfElements = ARRAY_COUNT (Lpddr5MrCmdVIndex);
        break;

      case MrCaOdt:
        pArr = Lpddr5MrCaODTIndex;
        NumOfElements = ARRAY_COUNT (Lpddr5MrCaODTIndex);
        break;

      case MrDimmRon:
        pArr = Lpddr5MrDimmRonIndex;
        NumOfElements = ARRAY_COUNT (Lpddr5MrDimmRonIndex);
        break;

      default:
        break;
    }
  } else if (Outputs->IsDdr5) {
    switch (MrType) {
      case MrWrV:
        pArr = Ddr5MrWrVIndex;
        NumOfElements = ARRAY_COUNT (Ddr5MrWrVIndex);
        break;

      case MrCmdV:
        pArr = Ddr5MrCmdVIndex;
        NumOfElements = ARRAY_COUNT (Ddr5MrCmdVIndex);
        break;

      case MrCaOdt:
        pArr = Ddr5MrCaODTIndex;
        NumOfElements = ARRAY_COUNT (Ddr5MrCaODTIndex);
        break;

      case MrDimmRon:
        pArr = Ddr5MrDimmRonIndex;
        NumOfElements = ARRAY_COUNT (Ddr5MrDimmRonIndex);
        break;

      default:
        break;
    }
  }

  if (pArr != NULL) {
    for (i = 0; i < NumOfElements; i++) {
      DisplayMRContentFromHost (MrcData, pArr[i]);
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s%s\n\n", gStopTagStr, String);
}

/**
  This step prints out the key training parameters.
  This will allow tracking of current training settings across multiple steps

  @param[in]     MrcData                - Include all MRC global data.
  @param[in]     PrintMrcDoneParamTable - extra list of all group types that is printed after MRC is Done
  @param[in]     DoneNumberOfElements   - Number Of Elements in PrintMrcDoneParamTable
  @param[in]     PrintTableBase         - basic list of all group types that is printed after Comp Offset optimization or after MRC is Done
  @param[in]     BaseNumberOfElements   - Number Of Elements in PrintTableBase
  @param[in]     PrintParamTable        - list of all group types that is printed after a specific training step is Done
  @param[in]     TaskNumberOfElements   - Number Of Elements in PrintParamTable
  @param[in]     PostCode               - Parameter to be updated

  @retval Nothing
**/
void
MrcTrainedStateTrace (
  IN MrcParameters *const  MrcData,
  IN const GSM_GT          *PrintMrcDoneParamTable,
  IN UINT32                DoneNumberOfElements,
  IN const GSM_GT          *PrintTableBase,
  IN UINT32                BaseNumberOfElements,
  IN const GSM_GT          *PrintParamTable,
  IN UINT32                TaskNumberOfElements,
  IN MrcPostCode           PostCode
  )
{
  UINT32 NumberOfParams = MIN (TaskNumberOfElements, PRINT_MAX_PARAM);

  if (NumberOfParams != 0) {
    MrcPrintParameters (MrcData, "Results", PrintParamTable, NumberOfParams, PostCode);
  }

  if (PostCode == MRC_SA_GV_SWITCH) {
    MrcPrintParameters (MrcData, "Results", PrintMrcDoneParamTable, DoneNumberOfElements, PostCode);
    MrcPrintParameters (MrcData, "Results", PrintTableBase, BaseNumberOfElements, PostCode);
  }

  if (PostCode == MRC_CMP_OPT) {
    MrcPrintParameters (MrcData, "Results", PrintTableBase, BaseNumberOfElements, PostCode);
  }

  if ((PostCode == MRC_EARLY_WRITE_TIMING_2D) || (PostCode == MRC_WRITE_VOLTAGE_1D) ||
    (PostCode == MRC_WRITE_DS) || (PostCode == MRC_WRITE_VREF_2D) ||
    (PostCode == MRC_WRITE_TIMING_2D) || (PostCode == MRC_CMP_OPT)) {
    MrcPrintMR (MrcData, "MR_PRINT_Results", MrWrV);
  }

  if ((PostCode == MRC_CMP_OPT) || (PostCode == MRC_CMD_VREF)) {
    MrcPrintMR (MrcData, "MR_PRINT_Results", MrCmdV);
  }

  if (PostCode == MRC_DIMM_ODT_CA) {
    MrcPrintMR (MrcData, "MR_PRINT_Results", MrCaOdt);
  }

  if (PostCode == MRC_DIMM_RON) {
    MrcPrintMR (MrcData, "MR_PRINT_Results", MrDimmRon);
  }
}
#endif //MRC_DEBUG_PRINT

/**
  This function returns the timing value for Read/Write Preamble and Postamble.

  @param[in]  MrcData      - Pointer to global data structure.
  @param[in]  TimingType   - GetTimingRead for Read Preamble/Postable GetTimingWrite for Write Preamble/Postable
  @param[out] PreambleT    - Return pointer for Preamble Toggle time.
  @param[out] PreambleLow  - Return pointer for Preamble low time.
  @param[out] Postamble    - Return pointer for Postamble time.

  @retval MrcStatus - mrcSuccess if successful or an error status
**/
MrcStatus
MrcGetPrePostamble (
  IN  MrcParameters   *const MrcData,
  IN  TIMING_GET_TYPE  const TimingType,
  OUT UINT8           *const PreambleT,
  OUT UINT8           *const PreambleLow,
  OUT UINT8           *const Postamble
  )
{
  MrcStatus         Status;
  MrcOutput         *Outputs;
  UINT8             WrPreambleT;    // Write Preamble Toggle time in tCK
  UINT8             WrPreambleLow;  // Write Preamble low time in tCK
  UINT8             WrPostamble;    // Write Postamble time in tCK.
  UINT8             RdPreambleT;    // Read Preamble Toggle time in tCK.
  UINT8             RdPreambleLow;  // Read Preamble low time in tCK.
  UINT8             RdPostamble;    // Read Postable Toggle time in tCK. Currently always 0
  DDR5_MR8_tRPRE    RpreVal;
  DDR5_MR8_tWPRE    WpreVal;
  DDR5_MR8_tWPOST   WpostVal;
  LPDDR5_MR10_tRPRE Lp5RpreVal;

  Outputs       = &MrcData->Outputs;
  WrPreambleT   = 0;
  WrPreambleLow = 0;
  WrPostamble   = 0;
  RdPreambleT   = 0;
  RdPreambleLow = 0;
  RdPostamble   = 0;

  Status = mrcSuccess;
  if (Outputs->IsLpddr5) {
    if (MrcLp5GetReadPreambleSetting (MrcData, &Lp5RpreVal) == mrcSuccess) {
      switch (Lp5RpreVal) {
        case Lp5tRPRE_2S_2T_00001010:
          RdPreambleT = 2;
          RdPreambleLow = 2;
          break;
        case Lp5tRPRE_0S_4T_10101010:
          RdPreambleT = 4;
          RdPreambleLow = 0;
          break;
        case Lp5tRPRE_2S_4T_000010101010:
          RdPreambleT = 4;
          RdPreambleLow = 2;
          break;
        default:
          break;
      }
    } else {
      Status = mrcWrongInputParameter;
    }
  } else { //Ddr5
    if (MrcDdr5GetReadPreambleSetting (MrcData, &RpreVal) == mrcSuccess) {
      switch (RpreVal) {
        case Ddr5tRPRE_2tCK_0010:
          RdPreambleT = 1;
          RdPreambleLow = 1;
          break;
        case Ddr5tRPRE_3tCK_000010:
          RdPreambleT = 1;
          RdPreambleLow = 2;
          break;
        case Ddr5tRPRE_4tCK_00001010:
          RdPreambleT = 2;
          RdPreambleLow = 2;
          break;
        default:
          break;
      }
    } else {
      Status = mrcWrongInputParameter;
    }
    if (MrcDdr5GetWritePreambleSetting (MrcData, &WpreVal) == mrcSuccess) {
      switch (WpreVal) {
        case Ddr5tWPRE_2tCK_0010:
          WrPreambleT = 1;
          WrPreambleLow = 1;
          break;
        case Ddr5tWPRE_3tCK_000010:
          WrPreambleT = 1;
          WrPreambleLow = 2;
          break;
        case Ddr5tWPRE_4tCK_00001010:
          WrPreambleT = 2;
          WrPreambleLow = 2;
          break;
        default:
          break;
      }
    } else {
      Status = mrcWrongInputParameter;
    }
    if (MrcDdr5GetWritePostambleSetting (MrcData, &WpostVal) == mrcSuccess) {
      WrPostamble = (UINT8) WpostVal;
    } else {
      Status = mrcWrongInputParameter;
    }
  }

  if (TimingType == GetTimingRead) {
    if (PreambleT != NULL) {
      *PreambleT = RdPreambleT;
    }
    if (PreambleLow != NULL) {
      *PreambleLow = RdPreambleLow;
    }
    if (Postamble != NULL) {
      *Postamble = RdPostamble;
    }
  } else { // GetTimingWrite
    if (PreambleT != NULL) {
      *PreambleT = WrPreambleT;
    }
    if (PreambleLow != NULL) {
      *PreambleLow = WrPreambleLow;
    }
    if (Postamble != NULL) {
      *Postamble = WrPostamble;
    }

  }

  return Status;
}

/**
  This function determines if a DDR5 device has been iterated upon based on the incoming DeviceMask
  This function takes into account byte swizzle and will appropriately detect the current device based on byte swizzle
  If the incoming device is not part of the device mask, the function will set the device mask and return FALSE indicating
    that the device was not previously in the DeviceMask
  If the incoming device is part of the device mask, the function will return TRUE

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      Controller  - Current Controller
  @param[in]      Channel     - Current Channel
  @param[in]      Rank        - Current Rank
  @param[in]      Byte        - Current Byte
  @param[in,out]  DeviceMask  - Hold the DeviceMask which will be programmed if the current device is not already part of DeviceMask
  @param[out]     Device      - Returns the device in which the incoming byte is part of
  @param[in]      DebugPrint  - Enable/disable debug printing

  @retval BOOLEAN - TRUE if current Device is already part of DeviceMask
**/
BOOLEAN
HasDeviceBeenSeenDdr5 (
  IN     MrcParameters      *MrcData,
  IN     UINT32             Controller,
  IN     UINT32             Channel,
  IN     UINT32             Rank,
  IN     UINT32             Byte,
  IN OUT UINT8              *DeviceMask,
  OUT    UINT8              *Device,
  IN     BOOLEAN            DebugPrint
  )
{
  MrcOutput      *Outputs;
  MrcChannelOut  *ChannelOut;
  MrcChannelIn   *ChannelIn;
  MrcDimmOut     *DimmOut;
  UINT8          DramByte;
  BOOLEAN        Width16;

  Outputs    = &MrcData->Outputs;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  ChannelIn  = &MrcData->Inputs.Controller[Controller].Channel[Channel];
  DimmOut    = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)];
  Width16    = (DimmOut->SdramWidth == 16);

  if (!Width16) {
    // x8 should always return FALSE and the device is the current byte
    *Device = (UINT8) Byte;
    return FALSE;
  }
  DramByte = ChannelIn->DqsMapCpu2Dram[RANK_TO_DIMM_NUMBER (Rank)][Byte];
  // Bytes 0 and 1 will always point to device 0, Bytes 2 and 3 will always point to device 1
  *Device = DramByte / 2;
  if (DebugPrint) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Byte %d = Device %d\n", Byte, *Device);
  }
  // If device has already been programmed, then skip
  if (*DeviceMask & (1 << *Device)) {
    return TRUE;
  }

  *DeviceMask |=  (1 << *Device);
  return FALSE;
}

/**
  This procedure prints header for Training Steps depending on Scope level
  Input NumTests must be 1 or greater. If NumTests is 0, function will error out.

  @param[in] MrcData        - Include all MRC global data.
  @param[in] DebugLevel     - DebugLevel
  @param[in] McChBitMask    - Determine which Channels to print
  @param[in] Rank           - Current operating rank
  @param[in] Scope          - Lowest Scope we want to print (Channel, Byte, Bit)
  @param[in] NumSpaces      - Number of spaces between the lowest scope.  Valid values are 0:19.
  @param[in] NumTests       - Defines how many consecutive headers to print out. Used for DDR5 x16 Readmpr. Default should be 1.
  @param[in] ErrCount       - Print Error Count Header

  @retval MrcStatus - mrcWrongInputParameter: NumSpaces was invalid
                    - mrcSuccess
*/
MrcStatus
MrcPrintHeaderTestScope (
  IN OUT MrcParameters *const MrcData,
  IN     MrcDebugMsgLevel     DebugLevel,
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN     LinearSweepScope     Scope,
  IN     UINT8                NumSpaces,
  IN     UINT8                NumTests,
  IN     BOOLEAN              ErrCount
  )
{
#ifdef MRC_DEBUG_PRINT
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT32    Index;
  UINT32    Controller;
  UINT32    Channel;
  UINT8     Byte;
  UINT8     Bit;
  UINT8     BitMax;
  UINT8     ChMask;
  UINT8     MaxChannels;
  UINT8     MaxChInMc[MAX_CONTROLLER];
  UINT8     SpaceCount;
  UINT8     SdramCount;
  UINT8     BaseSpaceSize;
  UINT8     RankIndex;
  BOOLEAN   IsAdditionalSpace;
  char      PrintStr[MAX_STRING_LENGTH];
  char      BaseSpaceStr[20];
  char      *StrPtr;
  UINT8     Tests;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  SdramCount    = Outputs->SdramCount;
  MaxChannels   = Outputs->MaxChannels;
  ChMask        = (1 << MaxChannels) - 1;
  // Get # of channel to print in each MC
  MaxChInMc[0] = MrcCountBitsEqOne (McChBitMask & ChMask);
  MaxChInMc[1] = MrcCountBitsEqOne ((McChBitMask >> MaxChannels) & ChMask);
  BitMax = (Scope == BitScope) ? MAX_BITS : MAX_BITS + 1;

  // NumTests needs to be at least 1 to be a valid arguement
  if (NumTests < 1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: NumTests(%d) illegal\n", gErrString, NumTests);
    return mrcWrongInputParameter;
  }

  BaseSpaceSize = ARRAY_COUNT (BaseSpaceStr);
  if (NumSpaces >= BaseSpaceSize) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: NumSpaces(%d) illegal\n", gErrString, NumSpaces);
    return mrcWrongInputParameter;
  }
  MrcData->Inputs.Call.Func->MrcSetMem ((UINT8 *) BaseSpaceStr, NumSpaces, ' ');
  BaseSpaceStr[NumSpaces] = '\0';

  IsAdditionalSpace = ((Scope >= BitScope) && (NumSpaces == 0)) ? TRUE : FALSE;

  if (Rank != MRC_IGNORE_ARG_8) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\nRank = %d\n", Rank);
  }
  MRC_DEBUG_MSG (Debug, DebugLevel, "Mc\t\t");
  for (Tests = 0; Tests < NumTests; Tests++) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MaxChInMc[Controller] == 0) {
        continue;
      }
      StrPtr = PrintStr;
      StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), "%d", Controller) - 1;
      SpaceCount = 1 * MaxChInMc[Controller];
      if (Scope == ChRankScope) {
        SpaceCount *= MAX_RANK_IN_CHANNEL;
      }
      if (Scope >= ByteScope) {
        SpaceCount *= SdramCount;
      }
      if (Scope >= BitScope) {
        if (SdramCount == MRC_DDR5_ECC_BYTE) {
          SpaceCount -= (4 * MaxChInMc[Controller]);
        }
        SpaceCount *= BitMax;
        if (SdramCount == MRC_DDR5_ECC_BYTE) {
          SpaceCount -= (4 * MaxChInMc[Controller]);
        }
      }
      for (Index = 0; Index < SpaceCount; Index++) {
        StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), ((Index == (UINT8) (SpaceCount - 1)) ? "%s" : "%s "), BaseSpaceStr) - 1;
        if (IsAdditionalSpace && ((Index % BitMax) == 0)) {
          StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), " ") - 1;
        }
        // +3 is to ensure room for NULL terminator, extra space for after %s, and space break in special BitScope case
        if ((sizeof (PrintStr) - (StrPtr - PrintStr)) < (sizeof (BaseSpaceStr) + 3)) {
          MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
          StrPtr = PrintStr;
        }
      }
      if (StrPtr != PrintStr) {
        MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
      }
    } // Controller
  } // NumTest

  if (Scope >= ChScope) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\nChannel\t\t");
    SpaceCount = 1;
    if (Scope == ChRankScope) {
      SpaceCount *= MAX_RANK_IN_CHANNEL;
    }
    if (Scope >= ByteScope) {
      SpaceCount *= SdramCount;
    }
    if (Scope >= BitScope) {
      SpaceCount *= BitMax;
      if (SdramCount == MRC_DDR5_ECC_BYTE) {
        SpaceCount -= 4;
      }
    }
    for (Tests = 0; Tests < NumTests; Tests++) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
            continue;
          }
          StrPtr = PrintStr;
          StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), "%d", Channel) - 1;
          for (Index = 0; Index < SpaceCount; Index++) {
            StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), ((Index == (UINT8) (SpaceCount - 1)) ? "%s" : "%s "), BaseSpaceStr) - 1;
            if (IsAdditionalSpace && ((Index % BitMax) == 0)) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), " ") - 1;
            }
            // +3 is to ensure room for NULL terminator, extra space for after %s, and space break in special BitScope case
            if ((sizeof (PrintStr) - (StrPtr - PrintStr)) < (sizeof (BaseSpaceStr) + 3)) {
              MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
              StrPtr = PrintStr;
            }
          }
          if (StrPtr != PrintStr) {
            MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
          }
        } // Channel
      } // Controller
    } // NumTest
  } // ChScope

  if (Scope == ChRankScope) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\nRank\t\t");
    for (Tests = 0; Tests < NumTests; Tests++) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
            continue;
          }
          SpaceCount = 1;
          for (RankIndex = 0; RankIndex < MAX_RANK_IN_CHANNEL; RankIndex++) {
            StrPtr = PrintStr;
            StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), "%d", RankIndex) - 1;
            for (Index = 0; Index < SpaceCount; Index++) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), ((Index == (UINT8) (SpaceCount - 1)) ? "%s" : "%s "), BaseSpaceStr) - 1;
            }
            if (IsAdditionalSpace) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), " ") - 1;
            }
            MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
          } // Rank
        } // Channel
      } // Controller
    } // NumTest
  } // ChRank Scope

  if (Scope >= ByteScope) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\nByte\t\t");
    for (Tests = 0; Tests < NumTests; Tests++) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
            continue;
          }
          SpaceCount = (Scope >= BitScope) ? BitMax : 1;
          for (Byte = 0; Byte < SdramCount; Byte++) {
            if ((Scope >= BitScope) && (Byte == MRC_DDR5_ECC_BYTE)) {
              SpaceCount -= 4;
            }
            StrPtr = PrintStr;
            StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), "%d", Byte) - 1;
            for (Index = 0; Index < SpaceCount; Index++) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), ((Index == (UINT8) (SpaceCount - 1)) ? "%s" : "%s "), BaseSpaceStr) - 1;
            }
            if (IsAdditionalSpace) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), " ") - 1;
            }
            MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
          } // Byte
        } // Channel
      } // Controller
    } // NumTest
  } // Byte Scope

  if ((Scope == BitScope) || (Scope == BitScopeDqs)) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\nBit\t\t");
    for (Tests = 0; Tests < NumTests; Tests++) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
            continue;
          }
          for (Byte = 0; Byte < SdramCount; Byte++) {
            StrPtr = PrintStr;
            for (Bit = 0; Bit < BitMax; Bit++) {
              if (!MrcBitExist (Controller, Byte, Bit)) {
                continue; // DDR5 ECC byte has only 4 bits DQ
              }
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), ((Bit < MAX_BITS) ? "%d" : "S"), Bit) - 1;
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), BaseSpaceStr) - 1;
            }
            // Special case to insert a space between bytes
            if (NumSpaces == 0) {
              StrPtr += MrcSPrintf (MrcData, StrPtr, sizeof (PrintStr) - (StrPtr - PrintStr), " ") - 1;
            }
            MRC_DEBUG_MSG (Debug, DebugLevel, "%s", PrintStr);
          } // Byte
        } //Channel
      } //Controller
    } // NumTests
  } // Bit Scope

  if ((Scope == ByteScope) && (ErrCount)) {
    MRC_DEBUG_MSG (Debug, DebugLevel, " Error Count\n");
  } else {
    MRC_DEBUG_MSG (Debug, DebugLevel, "\n");
  }

  return mrcSuccess;
#else
  return mrcSuccess;
#endif // MRC_DEBUG_PRINT
}

/**
  Top level function which calls the function which  prints header for Training Steps
  Assumes number of tests is 1

  @param[in] MrcData        - Include all MRC global data.
  @param[in] DebugLevel     - DebugLevel
  @param[in] McChBitMask    - Determine which Channels to print
  @param[in] Rank           - Current operating rank
  @param[in] Scope          - Lowest Scope we want to print (Channel, Byte, Bit)
  @param[in] NumSpaces      - Number of spaces between the lowest scope.  Valid values are 0:19.
  @param[in] ErrCount       - Print Error Count Header

  @retval MrcStatus - mrcWrongInputParameter: NumSpaces was invalid
                    - mrcSuccess
*/
MrcStatus
MrcPrintHeader (
  IN OUT MrcParameters *const MrcData,
  IN     MrcDebugMsgLevel     DebugLevel,
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN     LinearSweepScope     Scope,
  IN     UINT8                NumSpaces,
  IN     BOOLEAN              ErrCount
  )
{
#ifdef MRC_DEBUG_PRINT
  return MrcPrintHeaderTestScope (
    MrcData,
    DebugLevel,
    McChBitMask,
    Rank,
    Scope,
    NumSpaces,
    1,
    ErrCount
  );
#else
  return mrcSuccess;
#endif // MRC_DEBUG_PRINT
}

/**
  Obtain CKD Smbus Address through SPD Smbus Address.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
MrcSetupCkdAddress (
  MrcParameters    *const MrcData
  )
{
  MrcInput           *Inputs;
  MrcOutput          *Outputs;
  MrcDimmIn          *DimmIn;
  MrcDimmOut         *DimmOut;
  UINT8              Index;
  UINT32             McIndex;
  UINT32             ChIndex;
  UINT32             DimmIndex;
  UINT8              CkdIndex;
  UINT32             MaxDimm;

  Index = 0;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  MaxDimm = Inputs->IsDdrIoDtHalo ? MAX_DIMMS_IN_CHANNEL : 1;

  // Assign CKD DIMM Index
  for (McIndex = 0; McIndex < MAX_CONTROLLER; McIndex++) {
    for (ChIndex = 0; ChIndex < MAX_DDR5_CHANNEL; ChIndex++) {
      for (DimmIndex = 0; DimmIndex < MaxDimm; DimmIndex++) {
        DimmIn = &Inputs->Controller[McIndex].Channel[ChIndex].Dimm[DimmIndex];
        DimmOut = &Outputs->Controller[McIndex].Channel[ChIndex].Dimm[DimmIndex];
        if (DimmOut->IsCkdSupport == FALSE) {
          continue;
        }
        // For CUDIMM, CSODIMM: set CKD Address based on SPD Address if it's not assigned
        if ((DimmIn->CkdAddress == 0) && (DimmIn->SpdAddress > 0)) {
          DimmIn->CkdAddress = (DimmIn->SpdAddress & 0x0F) | 0xB0;
        }
        if (DimmIn->CkdAddress == 0) {
          continue;
        }
        for (CkdIndex = 0; CkdIndex < Index; CkdIndex++) {
          // CkdAddress already existed
          if (DimmIn->CkdAddress == Outputs->CkdBuffer[CkdIndex].CkdAddress) {
            DimmOut->CkdDimmIndex = CkdIndex;
            break;
          }
        }
        // New CKD Address
        if (CkdIndex == Index) {
          if (CkdIndex >= MAX_DIMMS_IN_SYSTEM) {
            MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s CKD Dimm Index exceeding max DIMM in system (4), please check the CKD Address\n", gErrString);
            return mrcFail;
          }
          DimmOut->CkdDimmIndex = CkdIndex;
          Outputs->CkdBuffer[CkdIndex].CkdAddress = DimmIn->CkdAddress;
          Index++;
        }
      }
    }
  }

  return mrcSuccess;
}


/**
  This function gets the current CKD Control Word value from the CkdBuffer.

  @param[in] MrcData    - Pointer to global data.
  @param[in] CkdDimm    - CKD Dimm Number.
  @param[in] Offset     - Register offset.

  @retval The current CKD Control Word value, either from the CkdBuffer or from hardware if CkdBuffer doesn't exist.
**/
UINT8
MrcCkdBufferRead (
  IN MrcParameters *const MrcData,
  IN const UINT8   CkdDimm,
  IN const UINT32  Offset
  )
{
  RETURN_STATUS       Status;
  MRC_CKD_BUFFER      *CkdBuffer;
  UINT32               Index;

  CkdBuffer    = &MrcData->Outputs.CkdBuffer[CkdDimm];

  if (Offset >= MAX_CKD_REG) {
    return 0;
  } else {
    Index = MrcCkdOffsetToIndex (Offset);
    if (Index != MRC_IGNORE_ARG_8) {
      return CkdBuffer->Data[Index].Data;
    } else {
      // Direct read from CKD DIMM for Read-Only Registers
      return MrcSmbusRead (MrcData, CkdBuffer->CkdAddress | (Offset << 8), &Status);
    }
  }
}

/**
  This function sets the current CKD Control Word value and updates corresponding CKD Buffer.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Mode       - Bit-field flags controlling Get/Set.
  @param[in] CkdDimm    - CKD Dimm Number.
  @param[in] Offset     - Register offset.
  @param[in] Value      - New Control Word value.

  @retval mrcSuccess or mrcWrongInputParameter if Offset is outside CKD register space.
**/
MrcStatus
MrcCkdBufferWrite (
  IN MrcParameters *const MrcData,
  IN const UINT32  Mode,
  IN const UINT8   CkdDimm,
  IN const UINT32  Offset,
  IN const UINT8   Value
  )
{
  MrcDebug            *Debug;
  MrcStatus           Status;
  RETURN_STATUS       SmbusStatus;
  MRC_CKD_BUFFER      *CkdBuffer;
  UINT8               Index;

  Status       = mrcSuccess;
  Debug        = &MrcData->Outputs.Debug;

  if (Offset >= MAX_CKD_REG) {
    return mrcWrongInputParameter;
  } else {
    CkdBuffer = &MrcData->Outputs.CkdBuffer[CkdDimm];
    Index = MrcCkdOffsetToIndex (Offset);
    if (Index != MRC_IGNORE_ARG_8) {
      if ((Mode & GSM_UPDATE_CACHE) != 0) {
        CkdBuffer->Data[Index].Data = Value;
        CkdBuffer->Data[Index].IsDirty = TRUE;
      }
      if (((Mode & GSM_FORCE_WRITE) != 0) || ((Mode & GSM_CACHE_ONLY) == 0)) {
        MrcSmbusWrite (MrcData, CkdBuffer->CkdAddress | (Offset << 8), Value, &SmbusStatus);

        MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "CkdAddress = 0x%x Offset = 0x%x, Data = 0x%x\n", CkdBuffer->CkdAddress, Offset, CkdBuffer->Data[Index].Data);
      }
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to write to a Read-Only Register\n");
    }
  }

  return Status;
}

/**
  Flush the CKD Buffer to CKD Control Words.

  @param[in]  MrcData    - Pointer to MRC global data.
**/
VOID
MrcFlushCkdBuffer (
  IN MrcParameters *const MrcData
  )
{
  MRC_CKD_BUFFER      *CkdBuffer;
  RETURN_STATUS       Status;
  UINT8               Index;
  UINT8               CkdDimm;
  UINT8               Offset;

  for (CkdDimm = 0; CkdDimm < MAX_DIMMS_IN_SYSTEM; CkdDimm++) {
    CkdBuffer = &MrcData->Outputs.CkdBuffer[CkdDimm];
    if (CkdBuffer->CkdAddress == CKD_NOT_PRESENT) {
      continue;
    }
    for (Index = 0; Index < MAX_WRITE_CKD; Index++) {
      if (CkdBuffer->Data[Index].IsDirty) {
        Offset = MrcCkdIndexToOffset (Index);
        if (Offset != MRC_IGNORE_ARG_8) {
          MrcSmbusWrite (MrcData, CkdBuffer->CkdAddress | ((UINT32) Offset << 8), CkdBuffer->Data[Index].Data, &Status);
          CkdBuffer->Data[Index].IsDirty = FALSE;

          MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "CkdAddress = 0x%x Offset = 0x%x, Data = 0x%x\n", CkdBuffer->CkdAddress, Offset, CkdBuffer->Data[Index].Data);
        }
      }
    }
  }
}

/**
  Obtain CKD Output Pin number and CKD Dimm number based on MC/Channel/Rank

  @param[in]  Controller  - Memory Controller Number.
  @param[in]  Channel     - Channel Number.
  @param[in]  Rank        - Rank Number.
  @param[out] CkdDimm     - CKD Dimm Number.
  @param[out] CkdPin      - CKD Pin Number.

  Pin:
  0 - QCLK0_A
  1 - QCLK1_A
  2 - QCLK0_B
  3 - QCLK1_B

  **/
VOID
MrcCalcCkdDimmPin (
  IN MrcParameters      *const MrcData,
  IN     UINT32          const Controller,
  IN     UINT32          const Channel,
  IN     UINT32          const Rank,
  OUT    UINT32          *const CkdDimm, OPTIONAL
  OUT    UINT32          *const CkdPin   OPTIONAL
  )
{
  UINT32       Dimm;
  UINT32       Pin;

  Dimm = MrcData->Outputs.Controller[Controller].Channel[Channel].Dimm[Rank/2].CkdDimmIndex;
  Pin  = (MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Rank/2].ChannelToCkdQckMapping * 2) + (Rank % 2);

  if (CkdDimm != NULL) {
    *CkdDimm = Dimm;
  }
  if (CkdPin != NULL) {
    *CkdPin = Pin;
  }
}

/**
  Check CKD Clock:
  - no more than two instances of the clock requested
  - must be in the same CKD DIMM

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcFail on failure, otherwise mrcSuccess.
**/
MrcStatus
MrcCkdCheckValidInstance (
  IN MrcParameters* const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcStatus           Status;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              Dimm;
  UINT8               CurrPhyClkToCkdDimm;
  UINT8               CurrCkdAddress;
  UINT8               Loop;
  UINT8               Index;
  PhyClkToCkdDimmGrp  PhyClkToCkd[8];

  Inputs       = &MrcData->Inputs;
  MrcCall      = Inputs->Call.Func;
  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;
  Status       = mrcSuccess;
  Index        = 0;

  MrcCall->MrcSetMem ((UINT8*)PhyClkToCkd, sizeof (PhyClkToCkd), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].IsCkdSupport) {
          CurrPhyClkToCkdDimm = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].PhyClockToCkdDimm;
          CurrCkdAddress = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].CkdAddress;
          for (Loop = 0; Loop < Index; Loop++) {
            if (CurrPhyClkToCkdDimm == PhyClkToCkd[Loop].PhyClkToCkdVal) {
              if (PhyClkToCkd[Loop].CkdAddress != CurrCkdAddress) {
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s PhyClockToCkdDimm = %d clock selected is not in the same CKD DIMM, CkdAddress = 0x%x vs 0x%x\n",
                  gErrString, PhyClkToCkd[Loop].PhyClkToCkdVal, CurrCkdAddress, PhyClkToCkd[Loop].CkdAddress);
                Status = mrcFail;
              }
              PhyClkToCkd[Loop].Instance++;
              break;
            }
          }
          if (Loop == Index) {
            PhyClkToCkd[Loop].PhyClkToCkdVal = CurrPhyClkToCkdDimm;
            PhyClkToCkd[Loop].CkdAddress = CurrCkdAddress;
            PhyClkToCkd[Loop].Instance++;
            Index++;
          }
        }
      }
    }
  }

  // Return error if it's more than 2 instances.
  for (Loop = 0; Loop < Index; Loop++) {
    if (PhyClkToCkd[Loop].Instance > 2) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s PhyClockToCkdDimm = %d has %d instances. PhyClockToCkdDimm must be no more than two instances.\n",
        gErrString, PhyClkToCkd[Loop].PhyClkToCkdVal, PhyClkToCkd[Loop].Instance);
      Status = mrcFail;
    }
  }

  return Status;
}

/**
  This Function configures DDR5 CKD Control Word Registers.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval mrcFail if MrcCkdCheckValidInstance is fail, otherwise mrcSuccess
**/
MrcStatus
MrcDdr5CkdConfig (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              Dimm;
  UINT32              Rank;
  UINT32              RankIndex;
  UINT32              CkdDimm;
  UINT32              CkdPin;
  INT64               GetSetVal;

  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;

  MrcEnableDimmPmic (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Programming CKD Control Word.\n");

  if (MrcSetupCkdAddress (MrcData) != mrcSuccess) {
    return mrcFail;
  }

  if (MrcCkdCheckValidInstance (MrcData) != mrcSuccess) {
    return mrcFail;
  }

  // Update CKD Buffer initial values for RW00
  for (CkdDimm = 0; CkdDimm < MAX_DIMMS_IN_SYSTEM; CkdDimm++) {
    if (Outputs->CkdBuffer[CkdDimm].CkdAddress == CKD_NOT_PRESENT) {
      continue;
    }
    Outputs->CkdBuffer[CkdDimm].Data[ckdIndexRW00].Data = CKD_CONFIGURATION_DEF;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].IsCkdSupport == FALSE) {
          continue;
        }
        for (RankIndex = 0; RankIndex < MAX_RANK_IN_DIMM; RankIndex++) {
          Rank = Dimm * MAX_RANK_IN_DIMM + RankIndex;
          MrcCalcCkdDimmPin (MrcData, Controller, Channel, Rank, &CkdDimm, &CkdPin);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%dCh%dR%d CkdDimm = %d, CkdPin = %d\n",
            Controller, Channel, Rank, CkdDimm, CkdPin);
          // Program only QCK Clock Disable if Rank is not enabled
          if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
            // RW00
            GetSetVal = 1;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckClkDisable, WriteToCache, &GetSetVal);
            continue;
          } else {
            GetSetVal = 0;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckClkDisable, WriteToCache, &GetSetVal);
            GetSetVal = SinglePllMode;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdPllMode, WriteToCache, &GetSetVal);
            GetSetVal = Ddr5CKDTable[CkdDimm][0].DckOdt;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdIctInputClkTerm, WriteToCache, &GetSetVal);
            // RW01
            GetSetVal = 1;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckOutputDelayEnable, WriteToCache, &GetSetVal);
            // RW02
            GetSetVal = Ddr5CKDTable[CkdDimm][CkdPin].Ron;
            MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckDrive, WriteToCache, &GetSetVal);
            // RW03
            // Per Channel, only need to program for even rank
            if ((Rank % CKD_QCK_SHARE) == 0) {
              GetSetVal = Ddr5CKDTable[CkdDimm][CkdPin].SlewRate;
              MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckSlewRate, WriteToCache, &GetSetVal);
            }
            if (MrcData->Inputs.BootMode == bmFast) {
              // RW04-07
              GetSetVal = Outputs->CkdShift[Controller][Channel][Rank];
              MrcCkdGetSetCW (MrcData, Controller, Channel, Rank, GsmCkdQckOutputDelay, WriteCached, &GetSetVal);
            }
          }
        }
      }
    }
  }
  MrcFlushCkdBuffer (MrcData);

  return mrcSuccess;
}

/**
  Obtain Physical MC/Channel/Rank based on based on PhyClockToCkdDimm

  @param[in]  MrcData     - Pointer to global data.
  @param[in]  Controller  - Memory Controller Number.
  @param[in]  Channel     - Channel Number.
  @param[in]  Rank        - Rank Number.
  @param[out] PhyController - Physical Controller that the CLK resides in
  @param[out] PhyChannel    - Physical Channel that the CLK resides in
  @param[out] PhyChannel    - Physical Rank that the CLK resides in

  @retval IsCkdSupported

  PhyClockToCkdDimm
  Encoding: a - xCLKy where a is the array index, x is Phy channel, and y is Phy Rank.
            0  - 0CLK0, 4  - 1CLK0, 8  - 2CLK0, 12 - 3CLK0
            1  - 0CLK1, 5  - 1CLK1, 9  - 2CLK1, 13 - 3CLK1
            2  - 0CLK2, 6  - 1CLK2, 10 - 2CLK2, 14 - 3CLK2
            3  - 0CLK3, 7  - 1CLK3, 11 - 2CLK3, 15 - 3CLK3
**/
BOOLEAN
MrcGetDdr5ClkIndex (
  IN MrcParameters      *const MrcData,
  IN     UINT32          const Controller,
  IN     UINT32          const Channel,
  IN     UINT32          const Rank,
  OUT    UINT32         *const PhyController,
  OUT    UINT32         *const PhyChannel,
  OUT    UINT32         *const PhyRank
  )
{
  MrcInput   *Inputs;
  UINT32     Dimm;
  UINT8      PhyClockToCkdDimm;
  BOOLEAN    IsCkdDimm;

  Inputs    = &MrcData->Inputs;
  Dimm      = Rank / MAX_RANK_IN_DIMM;
  IsCkdDimm = FALSE;
  *PhyController = MRC_UINT32_MAX;
  *PhyChannel    = MRC_UINT32_MAX;
  *PhyRank       = MRC_UINT32_MAX;

  if (MrcData->Outputs.Controller[Controller].Channel[Channel].Dimm[Dimm].IsCkdSupport) {
     // Mobile only supports up to 2 ranks
    if ((Inputs->IsDdrIoUlxUlt && (Rank < MAX_RANK_IN_DIMM)) || Inputs->IsDdrIoDtHalo) {
      PhyClockToCkdDimm = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].PhyClockToCkdDimm;
      IsCkdDimm = TRUE;
      *PhyController = PhyClockToCkdDimm / 8;
      *PhyChannel = (PhyClockToCkdDimm / 4) % 2;
      *PhyRank = PhyClockToCkdDimm % 4;
    }
  } else {
    *PhyController = Controller;
    *PhyChannel    = Channel;
    *PhyRank       = Rank;
  }

  return IsCkdDimm;
}


/**
  This function sets the bit-mask FailingChannelBitMask based on inputs Controller, Channel and Dimm value
  @param[in, out] MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
**/
VOID
MrcSetFailingChannelBitMask (
  IN OUT  MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm
  )
{
  MrcInput       *Inputs;
  MrcOutput      *Outputs;
  MrcDebug       *Debug;
  UINT32         DimmLoop;
  UINT32         FailingChannelBitIndex;

  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  FailingChannelBitIndex = 5; //Initialize Index to ReservedBit

  if (ExtInputs->RetrainToWorkingChannel) {
    if (Controller != MRC_IGNORE_ARG) {
      if (Channel != MRC_IGNORE_ARG) {
        for (DimmLoop = 0; DimmLoop < MAX_DIMMS_IN_CHANNEL; DimmLoop++) {
          if (Dimm != MRC_IGNORE_ARG && DimmLoop != Dimm) {
            continue;
          }
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc 0x%x Ch 0x%x Dimm 0x%x Failing\n", Controller, Channel, DimmLoop);
          FailingChannelBitIndex = MRC_DIMM_STATUS_INDEX(Controller, Channel, DimmLoop); //This variable determines which bit should be set to 1 based on MRC_DIMM_STATUS_STRUCT
          Outputs->FailingChannelBitMask.Data16 = Outputs->FailingChannelBitMask.Data16 | (MRC_DIMM_STATUS_ERROR << FailingChannelBitIndex);
        } // DimmLoop
      }
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "FailingChannelBitMask 0x%x\n", Outputs->FailingChannelBitMask.Data16);
  }
}

/**
  This function checks if a specific MR is treated as PDA according to the technology.

  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to work on.
  @param[in] Channel           - channel to work on.
  @param[in] MrAddress         - MR Address to check
  @param[in] IsSagvConfig      - TRUE if Programming MRS FSM for SAGV Transistion.
                                  If TRUE, Force DIMM DFE MRs to enable PDA

  @retval TRUE if the specific MR is PDA otherwise FALSE
**/
BOOLEAN
MrcMrIsPda (
  IN   MrcParameters* MrcData,
  IN   UINT32            Controller,
  IN   UINT32            Channel,
  IN   MrcModeRegister   MrAddress,
  IN   BOOLEAN           IsSagvConfig
  )
{
  MrcOutput* Outputs;
  MrcChannelOut* ChannelOut;
  BOOLEAN         IsDdr5;

  Outputs = &MrcData->Outputs;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  IsDdr5 = Outputs->IsDdr5;

  // check if global PDA is enabled
  if (!MrcData->Inputs.EnablePda || !IsDdr5) {
    return FALSE;
  }

  switch (MrAddress) {
  case mrMR3:
    if (ChannelOut->Mr3PdaEnabled) {
      return TRUE;
    }
    break;
  case mrMR7:
    if (ChannelOut->Mr7PdaEnabled) {
      return TRUE;
    }
    break;
  case mrMR10:
    if (ChannelOut->IsMr10PdaEnabled) {
      return TRUE;
    }
    break;
  case mrMR11:
    if (ChannelOut->IsMr11PdaEnabled) {
      return TRUE;
    }
    break;
  case mrMR48:
    if (ChannelOut->IsMr48PdaEnabled) {
      return TRUE;
    }
    break;
  case mrMR129:
  case mrMR137:
  case mrMR145:
  case mrMR153:
  case mrMR161:
  case mrMR169:
  case mrMR177:
  case mrMR185:
  case mrMR193:
  case mrMR201:
  case mrMR209:
  case mrMR217:
  case mrMR225:
  case mrMR233:
  case mrMR241:
  case mrMR249:
    // Always enable PDA DFE Tap for SAGV FSM Config
    if (IsSagvConfig || ChannelOut->MrXPdaDfeTap1Enabled) {
      return TRUE;
    }
    break;
  case mrMR130:
  case mrMR138:
  case mrMR146:
  case mrMR154:
  case mrMR162:
  case mrMR170:
  case mrMR178:
  case mrMR186:
  case mrMR194:
  case mrMR202:
  case mrMR210:
  case mrMR218:
  case mrMR226:
  case mrMR234:
  case mrMR242:
  case mrMR250:
    // Always enable PDA DFE Tap for SAGV FSM Config
    if (IsSagvConfig || ChannelOut->MrXPdaDfeTap2Enabled) {
      return TRUE;
    }
    break;
  case mrMR131:
  case mrMR139:
  case mrMR147:
  case mrMR155:
  case mrMR163:
  case mrMR171:
  case mrMR179:
  case mrMR187:
  case mrMR195:
  case mrMR203:
  case mrMR211:
  case mrMR219:
  case mrMR227:
  case mrMR235:
  case mrMR243:
  case mrMR251:
    // Always enable PDA DFE Tap for SAGV FSM Config
    if (IsSagvConfig || ChannelOut->MrXPdaDfeTap3Enabled) {
      return TRUE;
    }
    break;
  case mrMR132:
  case mrMR140:
  case mrMR148:
  case mrMR156:
  case mrMR164:
  case mrMR172:
  case mrMR180:
  case mrMR188:
  case mrMR196:
  case mrMR204:
  case mrMR212:
  case mrMR220:
  case mrMR228:
  case mrMR236:
  case mrMR244:
  case mrMR252:
    // Always enable PDA DFE Tap for SAGV FSM Config
    if (IsSagvConfig || ChannelOut->MrXPdaDfeTap4Enabled) {
      return TRUE;
    }
    break;
  default:
    break;
  }

  return FALSE;
}

/**
  If there is a value match with any of the elements in the array, then the matched array index is returned.

  @param[in]  Value     - Value to be matched.
  @param[in]  Array     - Input array.
  @param[in]  ArraySize - Size of Array.

  @retval Array index if there is a value match else returns -1.
**/
INT8
MrcFindIndex (
  IN UINT16 Value,
  IN UINT16 Array[],
  IN UINT8 ArraySize
  )
{
  INT8 Index;

  for (Index = 0; Index < ArraySize; Index++) {
    if (Value == Array[Index]) {
      return Index;
    }
  }

  return -1;
}

/**
  In a given 1D array, finds the maximum value and returns it.

  @param[in]  Array - Input array.
  @param[in]  ArraySize - Size of Array.

  @retval Returns the maximum value from the input array.
**/
INT8
MrcFindMaxVal (
  IN INT8  Array[],
  IN UINT8 ArraySize
  )
{
  UINT32 Index;
  INT8   MaxVal;

  MaxVal = -1;

  for (Index = 0; Index < ArraySize; Index++) {
    if (MaxVal < Array[Index]) {
      MaxVal = Array[Index];
    }
  }

  return MaxVal;
}

/**
  Display MR value from the host struct

  @param[in] MrcData       - Include all MRC global data.
  @param[in] MrAddr         - MR Address.
**/
VOID
DisplayMRContentFromHost (
  IN MrcParameters *const MrcData,
  IN MrcModeRegister      MrAddr
  )
{
  MrcChannelOut   *ChannelOut;
  MrcOutput       *Outputs;
  UINT8           Controller;
  UINT8           Channel;
  UINT8           Rank;
  UINT8           Data;
  UINT8           MrIndex;

  Outputs = &MrcData->Outputs;
  MrIndex = MrcMrAddrToIndex (MrcData, &MrAddr);

  if (MrIndex >= mrIndexMax) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "\nMrPtr argument is out of bounds\n");
    // Prevent accessing index out of bound later in function
    return;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank) == 0) {
          continue;
        }
        Data = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[MrIndex];
        MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "MC%u.C%u.R%u MR%d: 0x%X\n", Controller, Channel, Rank, MrAddr, Data);
      }
    } // Channel
  } // Controller
}
