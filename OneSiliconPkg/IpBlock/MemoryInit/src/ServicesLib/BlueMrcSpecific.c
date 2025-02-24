/** @file
  This file include all the common tolls for the mrc algo

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
// Include files
#include "MrcCommon.h"
#include "MrcDdrCommon.h"
#include "MrcMaintenance.h"
#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20Patterns.h"
#include "Cpgc20TestCtl.h"
#include "MrcMemoryApi.h"
#include "MrcCpgcOffsets.h"
#include "MrcLpddr5.h"
#include "MrcLpddr5Registers.h"
#include "MrcDdr5.h"
#include "MrcChipApi.h"
#include "MrcCrosser.h"
#include "MrcPostCodes.h"
#include "MrcCommandTraining.h"
#include "MrcWriteDqDqs.h"
#include "MrcDebugPrint.h"
#include "BlueTestDataEngine.h"
#include "MrcDdrIoUtils.h"
#include "MrcHalRegisterAccess.h"
#include "MrcReset.h"
#include "BlueMrcJedecApi.h"  // for MrcGetSagvMrSeq()

/// Local Defines
#define MRC_CHNG_MAR_GRP_NUM  (2)


#define MRC_EXIT_VALUE (0xFF)

///
/// Global Constant
///

/// Bank Array Function parameter is a constant, but due to compiler type mismatch
/// with GetSet function, cannot define as constant.
/// Set the bank mapping so that bank groups are toggled before banks (we will use logical banks 0 and 1 only).
/// Only update the first two Banks in the mapping.
/// Logical Bank:         0  1  2 ... 15
///                       --------------
/// Physical Bank Group:  0  1  X      X
/// Physical Bank:        0  0  X      X
MRC_BG_BANK_PAIR Ddr5RdMprBankL2p[2] = {{0, 0}, {1, 0}};

/// Set the bank mapping so that bank groups are toggled before banks (we will use logical banks 0 and 1 only).
/// Only update the first two Banks in the mapping.
/// Logical Bank:         0  1  2 ... 15
///                       --------------
/// Physical Bank Group:  0  1  X      X
/// Physical Bank:        0  0  X      X
MRC_BG_BANK_PAIR B2BL2p[MAX_CPGC_B2B_BANKS] = {{0, 0}, {1, 0}};

/// Set the bank mapping so that 4 B2B reads will occur on toggling Bank Groups and then a gap between each 4 B2B reads by using RDRDsg
/// Logical Bank:         0  1  2  3... 15
///                       --------------
/// Physical Bank Group:  0  1  2  3     X
/// Physical Bank:        0  0  0  0     X
MRC_BG_BANK_PAIR BurstIdleBurstDdr5G4L2p[MAX_CPGC_RECEN_BG_BANKS] = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };

/// Set the bank mapping so that bank groups are toggled before banks (can use logical banks 0 to 15).
/// DDR5 x16 has 2 BA bits and 2 BG bits
/// 16 banks case (x8):
/// Logical Bank:         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
///                       -----------------------------------------------
/// Physical Bank:        0  0  0  0  1  1  1  1  2  2  2  2  3  3  3  3
/// Physical Bank Group:  0  1  2  3  0  1  2  3  0  1  2  3  0  1  2  3
MRC_BG_BANK_PAIR Ddr5_16Gbx16BankMapB2B[MAX_DDR5_16Gb_x16_BANKS] = {
  { 0,0 }, { 1,0 }, { 2,0 }, { 3,0 }, { 0,1 }, { 1,1 }, { 2,1 }, { 3,1 },
  { 0,2 }, { 1,2 }, { 2,2 }, { 3,2 }, { 0,3 }, { 1,3 }, { 2,3 }, { 3,3 }
};

/// Set the bank mapping so that bank groups are toggled before banks (can use logical banks 0 to 7).
/// DDR5 8Gb x16 has 1 BA bit and 2 BG bits, 8 banks total
/// Logical Bank:         0  1  2  3  4  5  6  7
///                       ----------------------
/// Physical Bank:        0  0  0  0  1  1  1  1
/// Physical Bank Group:  0  1  2  3  0  1  2  3
MRC_BG_BANK_PAIR Ddr5_8Gbx16BankMapB2B[MAX_DDR5_8Gb_x16_BANKS] = {
  { 0,0 }, { 1,0 }, { 2,0 }, { 3,0 }, { 0,1 }, { 1,1 }, { 2,1 }, { 3,1 }
};

/// Set the bank mapping so that bank groups are toggled before banks (can use logical banks 0 to 15).
/// DDR5 8Gb x8 has 1 BA bit and 3 BG bits, 16 banks total
/// Logical Bank:         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
///                       -----------------------------------------------
/// Physical Bank:        0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1
/// Physical Bank Group:  0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7
MRC_BG_BANK_PAIR Ddr5_8Gbx8BankMapB2B[MAX_DDR5_8Gb_x8_BANKS] = {
  { 0,0 }, { 1,0 }, { 2,0 }, { 3,0 }, { 4,0 }, { 5,0 }, { 6,0 }, { 7,0 },
  { 0,1 }, { 1,1 }, { 2,1 }, { 3,1 }, { 4,1 }, { 5,1 }, { 6,1 }, { 7,1 }
};

/// Set the bank mapping so that bank groups are toggled before banks (can use logical banks 0 to 15).
/// DDR5 16Gb x8 has 2 BA bits and 3 BG bits, 32 banks total
/// Logical Bank:         0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
///                       -----------------------------------------------------------------------------------------------
/// Physical Bank:        0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  3  3  3  3  3  3  3  3
/// Physical Bank Group:  0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7  0  1  2  3  4  5  6  7
MRC_BG_BANK_PAIR Ddr5_16Gbx8BankMapB2B[MAX_DDR5_16Gb_x8_BANKS] = {
  { 0,0 }, { 1,0 }, { 2,0 }, { 3,0 }, { 4,0 }, { 5,0 }, { 6,0 }, { 7,0 },
  { 0,1 }, { 1,1 }, { 2,1 }, { 3,1 }, { 4,1 }, { 5,1 }, { 6,1 }, { 7,1 },
  { 0,2 }, { 1,2 }, { 2,2 }, { 3,2 }, { 4,2 }, { 5,2 }, { 6,2 }, { 7,2 },
  { 0,3 }, { 1,3 }, { 2,3 }, { 3,3 }, { 4,3 }, { 5,3 }, { 6,3 }, { 7,3 }
};

/**
  This function defines the Command pattern spread per DRAM technology.

  @param[in]  MrcData - Pointer to MRC global data

  @retval UINT8 - Number of bits in the set of the Victim Aggressor pattern.
**/
UINT8
MrcCommandSpread (
  IN  MrcParameters *const  MrcData
  )
{
  UINT8 Spread;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Spread = 7;
      break;

    default:
      Spread = 7;
      break;
  }

  return Spread;
}

/**
  Fill AddressArray with address related settings data per channel

  @param[in]  MrcData        - Include all MRC global data.
  @param[out] AddressArray   - 2D Array of Structure that stores address related settings per channel.
  @param[in]  Address        - Structure that stores address related settings.

  @retval Nothing
**/
void
FillAddressArray (
  IN  MrcParameters *const    MrcData,
  OUT MRC_ADDRESS             AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN  MRC_ADDRESS *const      Address
  )
{
  UINT8             Controller;
  UINT8             Channel;
  MRC_FUNCTION     *MrcCall;

  MrcCall = MrcData->Inputs.Call.Func;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      MrcCall->MrcCopyMem ((UINT8 *)&AddressArray[Controller][Channel], (UINT8 *)Address, sizeof (MRC_ADDRESS));
    }
  }
}

/**
  Divides TestLength by Number of Cache lines per loop to define how many loops do we need to perform a test

  @param[in]  NumCL                - Number of Cache lines per loop
  @param[in]  TestLengthExp        - Exponential Value of the total test length

  @retval Number of loops needed to perform the test (LoopCount)
**/
UINT32
MrcCalculateLoopCount (
  IN const UINT32                 NumCL,
  IN const UINT8                  TestLengthExp
)
{
  UINT32 LoopCount;
  UINT32 TestLength;

  TestLength = (1 << MIN (TestLengthExp, 31)); //TestLengthExp defines the total number of Cacheline Transactions for the test: 2^TestLengthExp.
  LoopCount = TestLength /  MAX (NumCL, 1);
  LoopCount -= (LoopCount) ? 1 : 0;
  return LoopCount;
}

/**
  Programs REUT to run on the selected physical ranks.

  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to enable.
  @param[in] ch                - Channel to enable.
  @param[in] RankBitMask       - Bit mask of ranks to enable.
  @param[in] SkipRankL2P       - Boolean control over skipping Logical 2 Physical programming for Rank.

  @retval Bit mask of channel enabled if rank in the channel exists.
**/
UINT8
SelectReutRanks (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          ch,
  IN UINT8                RankBitMask,
  IN BOOLEAN              SkipRankL2P
  )
{
  MrcOutput     *Outputs;
  UINT8   NewRankCount;
  UINT8   OldRankCount;
  UINT32  OldBaseRepeats;
  UINT32  Burst;
  UINT8   IpChannel;
  BOOLEAN IsLpddr;
  BOOLEAN IsGlobalBindEnabled;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;
  IsGlobalBindEnabled = 0;

  // Make sure valid rank bit mask for this channel
  RankBitMask &= Outputs->Controller[Controller].Channel[ch].ValidRankBitMask;
  IpChannel             = LP_IP_CH (IsLpddr, ch);

  // Check if nothing is selected
  if ((RankBitMask & 0xF) == 0) {
    // Disable channel's CPGC Global Start.
    Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, IsGlobalBindEnabled);

    // Disable CPGC / CADB engine on this channel
    // For LPDDR5, only program register on even channels.
    return 0;
  } else {
    // Normal case
    // Setup REUT Test to iteration through appropriate ranks during test
    NewRankCount = 0;
    // Prepare Rank Mapping and Max Rank
    if (!SkipRankL2P) {
      Cpgc20PrepareRankMapping (MrcData, Controller, ch, RankBitMask, &NewRankCount);
    } else {
      NewRankCount = 1; //to avoid negative value being passes into Cpgc20SetAddrMaxRank
    }
    IsGlobalBindEnabled = 1;
  }

  // Disable/Enable channel`s CPGC Global Start.
  Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, IsGlobalBindEnabled);

  if (!(IS_MC_SUB_CH (IsLpddr, ch))) {
    Cpgc20SetAddrMaxRank (MrcData, Controller, IpChannel, NewRankCount - 1, &OldRankCount);
    OldBaseRepeats = Cpgc20GetBaseRepeats(MrcData, Controller, IpChannel);
    if (OldRankCount == 0) {
      OldRankCount = 1;  // Divide by 0 prevention
      MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "%s SelectReutRanks: Old Rank Count is zero!", gErrString);
    }
    Burst = (OldBaseRepeats + 1) / OldRankCount;
    Cpgc20BaseRepeatsMcCh (MrcData, Controller, IpChannel, Burst, NewRankCount);
  }
  return (UINT8) (MRC_BIT0 << ((Controller * Outputs->MaxChannels) + ch));
}

/**
  Returns the index into the array MarginResult in the MrcOutput structure.

  @param[in] ParamV - Margin parameter

  @retval One of the following values: LastRxV(0), LastRxT (1), LastTxV(2), LastTxT (3), LastRcvEna (4),
                                       LastWrLevel (5), LastCmdT (6), LastCmdV (7)
**/
UINT8
GetMarginResultType (
  IN const UINT8 ParamV
  )
{
  switch (ParamV) {
    case WrV:
      return LastTxV;

    case WrT:
      return LastTxT;

    case RdV:
      return LastRxV;

    case RdT:
      return LastRxT;

    case RcvEna:
    case RcvEnaX:
      return LastRcvEna;

    case WrLevel:
      return LastWrLevel;

    case CmdT:
      return LastCmdT;

    case CmdV:
      return LastCmdV;

    default:
      //MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "GetMarginByte: Unknown Margin Parameter\n");
      break;
  }

  return 0; // Return LastRxV to point to the beginning of the array
}

/**
  This function will return the rank used to store the results based on the Rank Mask passed in.
  Results will be stored in the first Rank existing in the RankMask.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  RankMask - Bit mask of Ranks being margined.

  @retval mrcSuccess
**/
UINT8
GetRankToStoreResults (
  IN      MrcParameters *const MrcData,
  IN      UINT16               RankMask
  )
{
  UINT8              Rank;
  UINT8              RankLoop;
  MrcOutput         *Outputs;

  Outputs = &MrcData->Outputs;

  Rank = 0;
  if (RankMask == 0xFF) {
    return Rank;
  }
  // We return results on first available rank and RankIn is RankMask.
  for (RankLoop = 0; RankLoop < MAX_RANK_IN_CHANNEL; RankLoop++) {
    if ((1 << RankLoop) & RankMask & Outputs->ValidRankMask) {
    Rank = RankLoop;
    break;
    }
  }
  return Rank;
}

/**
  This function checks to see if the margin parameter is a rank based parameter.  These tend to be
  things which are controlled on the DRAM Mode Register side.

  @param[in]  Param   - The parameter to check.  Must be from MrcMarginTypes.

  @retval BOOLEAN - TRUE if it is a rank based margin parameter.  Otherwise FALSE.
**/
BOOLEAN
IsPerRankMarginParam (
  IN UINT8                Param
  )
{
  BOOLEAN   PerRankMp;

  switch (Param) {
    case WrV:
      // WrV technology dependent
      PerRankMp = TRUE;
      break;

    case CmdV:
      // CmdV technology dependent
      PerRankMp = TRUE;
      break;

    default:
      PerRankMp = FALSE;
      break;
  }

  return PerRankMp;
}

/**
  Retrieve the current memory frequency and clock from the memory controller.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in, out] MemoryClock  - The current memory clock.
  @param[in, out] Ratio        - The current memory ratio setting.

  @retval: The current memory frequency.
**/
MrcFrequency
MrcGetCurrentMemoryFrequency (
  MrcParameters * const   MrcData,
  UINT32 * const          MemoryClock,
  MrcClockRatio * const   Ratio
  )
{
  INT64                      GetSetVal;
  UINT32                     QclkRatioData;
  INT64                      QclkGearData;

  MrcGetSetNoScope (MrcData, GsmPmaQclkRatioData, ReadCached | PrintValue, &GetSetVal);
  QclkRatioData = (UINT32) GetSetVal;
  MrcGetSetNoScope (MrcData, GsmPmaGearTypeData, ReadCached | PrintValue, &QclkGearData);

  if (QclkGearData == 0) {
    QclkRatioData *= 2; // In gear2 the actual ratio is twice the ratio in the register
  }
  if (QclkGearData == 1) {
    QclkRatioData *= 4; // In gear4 the actual ratio is 4x the ratio in the register
  }

  if (MemoryClock != NULL) {
    *MemoryClock = MrcRatioToClock (MrcData, (MrcClockRatio) QclkRatioData);
  }
  if (Ratio != NULL) {
    *Ratio = (MrcClockRatio) QclkRatioData;
  }

  return MrcRatioToFrequency (
          MrcData,
          (MrcClockRatio) QclkRatioData
          );
}

/**
  This function is used to calculate Vref or VSwing of a generic voltage divider.
  ReceiverOdt must be non-Zero, or exceptions occurs.
  VSS, the voltage level the PullDown is attached to, is assumed to be 0.

  @param[in]  MrcData         - Pointer to global data structure.
  @param[in]  DriverPullUp    - Used to calculate Vhigh.  Value in Ohms.
  @param[in]  DriverPullDown  - Used to calculate Vlow.  Value in Ohms.
  @param[in]  ReceiverOdt     - Value in Ohms.
  @param[in]  Vdd             - Voltage level PullUp is tied to.  Must be the same unit size as Vtermination.
  @param[in]  Vtermination    - Voltage level ReceiverOdt is tied to.  Must be the same unit size as Vdd.
  @param[in]  IsVref          - Boolean to select Vref or Vswing calculation.

  @retval - Vref in units of Vdd/Vterm
**/
UINT32
MrcCalcGenericVrefOrSwing (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DriverPullUp,
  IN  UINT32                DriverPullDown,
  IN  UINT32                ReceiverOdt,
  IN  UINT32                Vdd,
  IN  UINT32                Vtermination,
  IN  BOOLEAN               IsVref
  )
{
  MrcDebug  *Debug;
  UINT32  Vhigh;
  UINT32  Vlow;
  UINT32  Result;
  UINT32  Numerator;
  UINT32  Denominator;

  Debug = &MrcData->Outputs.Debug;

  Numerator   = ReceiverOdt;
  Denominator = DriverPullUp + ReceiverOdt;
  MRC_DEBUG_ASSERT (Denominator != 0, Debug, "Divide by 0!\n");
  //Vhigh = Vtermination + ((Vdd - Vtermination) * (ReceiverOdt / (DriverPullUp + ReceiverOdt)));
  Vhigh = (Vdd - Vtermination) * Numerator;
  Vhigh = UDIVIDEROUND (Vhigh, Denominator) + Vtermination;
  Numerator   = DriverPullDown;
  Denominator = ReceiverOdt + DriverPullDown;
  MRC_DEBUG_ASSERT (Denominator != 0, Debug, "Divide by 0!\n");
  //Vlow  = Vtermination * (DriverPullDown / (DriverPullDown + ReceoverOdt))
  Vlow = Vtermination * Numerator;
  Vlow = UDIVIDEROUND (Vlow, Denominator);

  if (IsVref) {
    Result = Vhigh + Vlow;
    Result = UDIVIDEROUND (Result, 2);
  } else {
    Result = Vhigh - Vlow;
  }

  return Result;
}

/**
  This function is used to gather current system parameters to pass to the function,
  which calculates the ideal Read/Write/Command Vref.  It will return the result to the caller.

  @param[in]  MrcData         - Pointer to global data structure.
  @param[in]  DriverPullUp    - Used to calculate Vhigh.  Value in Ohms.
  @param[in]  DriverPullDown  - Used to calculate Vlow.  Value in Ohms.
  @param[in]  ReceiverOdt     - Value in Ohms.
  @param[in]  VrefType        - MrcMarginTypes: WrV, CmdV, RdV.
  @param[in]  Print           - Boolean switch to print the results and parameters.

  @retval - Vref in units of mV
**/
UINT32
MrcCalcIdealVref (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DriverPullUp,
  IN  UINT32                DriverPullDown,
  IN  UINT32                ReceiverOdt,
  IN  MrcMarginTypes        VrefType,
  IN  BOOLEAN               Print
  )
{
  MrcInput    *Inputs;
  MrcOutput   *Outputs;
  MrcDebug    *Debug;
  MrcDdrType  DdrType;
  MRC_ODT_MODE_TYPE OdtMode;
  UINT32      Vref;
  UINT32      Vdd;
  UINT32      Vss;
  UINT32      Vterm;
  BOOLEAN     IsLpddr;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  DdrType = Outputs->DdrType;
  OdtMode = Outputs->OdtMode;
  IsLpddr   = Outputs->IsLpddr;

  if (VrefType != WrV && VrefType != CmdV && VrefType != RdV) {
    MRC_DEBUG_ASSERT (1==0, Debug, "%s MrcCalcIdealVref - Invalid VrefType: %d\n", gErrString, VrefType);
    return 0;
  }

  Vdd   = Outputs->VddVoltage[Inputs->ExtInputs.Ptr->MemoryProfile];
  Vss   = 0;
  Vterm = MRC_UINT32_MAX;
  Vref  = MRC_UINT32_MAX;

  if (VrefType == WrV) {
    if (IsLpddr) {
      Vdd = Outputs->VccddqVoltage;
    }
  } else if (VrefType == CmdV) {
    // Command Vref
    if (IsLpddr) {
      Vdd = Outputs->VccddqVoltage;
    }
  }

  // Setup PullUp, PullDown, Vterm,
  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      Vterm = Vdd;
      break;

    case MRC_DDR_TYPE_LPDDR5:
      Vterm = Vss;
      break;

    default:
      MRC_DEBUG_ASSERT (FALSE, Debug, "Unsupported DRAM Type and Odt Mode: %d, %s\n", DdrType, gIoOdtModeStr[OdtMode]);
      return Vref;
  }

  Vref = MrcCalcGenericVrefOrSwing (MrcData, DriverPullUp, DriverPullDown, ReceiverOdt, Vdd, Vterm, TRUE);

  if (Print) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "DriverPullUp = %u, DriverPullDown = %u, ReceiverOdt = %u, Vdd = %u, Vterm = %u\nVrefType = %s, Vref = %u\n",
      DriverPullUp,
      DriverPullDown,
      ReceiverOdt,
      Vdd,
      Vterm,
      gMarginTypesStr[VrefType],
      Vref
      );
  }

  return Vref;
}

/**
  This function completes setting up the Generic MRS FSM configuration to enable SAGV during normal operation.

  @param[in] MrcData  - Pointer to global MRC data.
  @param[in] Print    - Boolean control for debug print messages.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
**/
MrcStatus
MrcFinalizeMrSeq (
  IN  MrcParameters *const MrcData,
  IN  BOOLEAN              Print
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  MrcModeRegister *MrPerRank;
  MrcModeRegister SagvMrSeq[MAX_MR_GEN_FSM];
  MrcModeRegister CurMrAddr;
  MrcModeRegisterIndex MrIndex;
  MrcDebugMsgLevel DebugLevel;
  GmfTimingIndex MrDelay[MAX_MR_GEN_FSM];
  UINT16 Delay;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 Index;
  UINT32 MrSeqLen;
  UINT8 Data;
  LPDDR5_MODE_REGISTER_16_TYPE *Mr16Lp5;
  LPDDR5_MODE_REGISTER_28_TYPE *Mr28Lp5;
  MRC_GEN_MRS_FSM_MR_TYPE *GenMrsFsmMr;
  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM];

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MrSeqLen = ARRAY_COUNT (SagvMrSeq);
  MrcCall = MrcData->Inputs.Call.Func;
  DebugLevel = ((Print) ? MSG_LEVEL_ERROR : MSG_LEVEL_NONE);

  if (Outputs->IsDdr5) {
    return MrcFinalizeDdr5MrSeq (MrcData);
  }

  // Clear out our array
  MrcCall->MrcSetMem ((UINT8 *) MrData, sizeof (MrData), 0);

  MrPerRank = NULL;
  Status = MrcGetSagvMrSeq (MrcData, SagvMrSeq, MrDelay, &MrSeqLen, &MrPerRank);
  Mr16Lp5 = (LPDDR5_MODE_REGISTER_16_TYPE *) &Data;
  Mr28Lp5 = (LPDDR5_MODE_REGISTER_28_TYPE *) &Data;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        MRC_DEBUG_MSG (Debug, DebugLevel, "MC%u.C%u.R%u\tData\tDelay\n", Controller, Channel, Rank);
        for (Index = 0; Index < MrSeqLen; Index++) {
          // Get MR value from host structure and store it in the GEN_FSM array.
          CurMrAddr = SagvMrSeq[Index];
          // Translate from MrcModeRegister to Host structure index
          MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);
          if (MrIndex < MAX_MR_IN_DIMM) {
            GenMrsFsmMr = &MrData[Controller][Channel][Rank][Index];
            Data = Outputs->Controller[Controller].Channel[Channel].Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[MrIndex];
            if (Outputs->IsLpddr5 && (CurMrAddr == mrMR16)) { // Cover mrMR16 and mrMR16FspOp cases
              // Always keep VRCG enabled because it is enabled before this MR sequence, and disabled after.
              Mr16Lp5->Bits.Vrcg = 1;
              if (SagvMrSeq[Index] == mrMR16FspOp) {
                GenMrsFsmMr->FspOpToggle = TRUE;
                GenMrsFsmMr->FreqSwitchPoint = TRUE;
              } else {  // First mrMR16
                GenMrsFsmMr->FspWrToggle = TRUE;
              }
            }
            if (Outputs->IsLpddr5 && (CurMrAddr == mrMR28) && Outputs->IsDvfsqEnabled) {
              if (SagvMrSeq[Index] == mrMR28ZqReset) {
                Mr28Lp5->Bits.ZqReset = 1;
              } else if (SagvMrSeq[Index] == mrMR28ZqStop) {
                Mr28Lp5->Bits.ZqStop = 1;
              }
            }

            if (CurMrAddr == mrREFab) {
              GenMrsFsmMr->CmdType = GmfCmdREFab;
            } else if (CurMrAddr == mrPreAll) {
              GenMrsFsmMr->CmdType = GmfCmdPreAll;
            }

            GenMrsFsmMr->PdaMr = FALSE;
            GenMrsFsmMr->MrData = Data;
            GenMrsFsmMr->MrAddr = CurMrAddr;
            GenMrsFsmMr->Valid = TRUE;
            GenMrsFsmMr->DelayIndex = MrDelay[Index];
            if (SagvMrSeq[Index] >= mpcMR13) {
              GenMrsFsmMr->CmdType = GmfCmdMpc;
            }
            MrcGetGmfDelayTiming (MrcData, MrDelay[Index], &Delay);
            MRC_DEBUG_MSG (Debug, DebugLevel, " MR%u:\t\t0x%02X\t%3u\t%s%s\n", CurMrAddr, Data, Delay,
              GenMrsFsmMr->FspWrToggle ? "FspWrToggle" : "", GenMrsFsmMr->FspOpToggle ? " FspOpToggle" : "");
          } else {
            MRC_DEBUG_MSG (Debug, DebugLevel, "MR index(%d) exceeded MR array length(%d)\n", MrIndex, MAX_MR_IN_DIMM);
            return mrcWrongInputParameter;
          }
        } // MR Index
      } // Rank
    } // Channel
  } // Controller

  if (Status == mrcSuccess) {
    // Program Generic MRS FSM Per Controller/Channel
    Status = MrcGenMrsFsmConfig (MrcData, MrData, TRUE, MrPerRank);
  }

  return Status;
}

/**
  Convert the input timing value from picoseconds to nCK (number of clocks).
  Returns 0 if tCKmin is 0 to avoid divide by 0 error.

  Utilizes the DDR5 SPD INT math Rounding Algorithm which reduces the clock by a
  correction factor of .3% (multiply by 99.7%) and then rounds up to the next integer value

    @param[in] TimingInPs - Timing value in units of picoseconds.
    @param[in] tCKmin     - The memory clock period in units of femtoseconds.

    @return The input timing value in units of nCK
**/
UINT32
PicoSecondsToClocks (
  IN UINT32 TimingInPs,
  IN UINT32 tCKmin
  )
{
  UINT32 Clocks;
  UINT32 MathTemp;
  UINT32 tCKps;

  // Convert tCK from Femtoseconds to Picoseconds
  tCKps = UDIVIDEROUND (tCKmin, 1000);

  if (tCKps == 0) {
    Clocks = 0;
  } else {
    // DDR5 SPD spec INT math Rounding Algorithm
    MathTemp = (TimingInPs * 997) / tCKps;
    MathTemp = MathTemp + 1000;
    Clocks = MathTemp / 1000;
  }

  return Clocks;
}



/**
  Print DqMapCpu2Dram and DqsMapCpu2Dram array

  @param[in] MrcData     - The MRC global data.

  @retval void
**/
VOID
PrintDqDqsTable (
  IN OUT MrcParameters *const   MrcData
)
{
#ifdef MRC_DEBUG_PRINT
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcControllerIn     *ControllerIn;
  MrcChannelIn        *ChannelIn;
  UINT8               Controller;
  UINT8               Channel;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Byte;
  UINT8               Iteration;
  UINT8               Bit;
  UINT8               BytesPerChannel;

  static const char PrintBorder[]    = "\n*************************************\n";

  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  Debug           = &Outputs->Debug;
  BytesPerChannel = Outputs->IsLpddr ? MAX_BYTE_IN_LP_CHANNEL : MAX_BYTE_IN_DDR5_CHANNEL;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s   DQDQS SWIZZLING   %s", "*****", "*****");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn = &Inputs->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        ChannelIn = &ControllerIn->Channel[Channel];
        if (Dimm == 1) {
          MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "\n");
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u.Ch%u.D%u.R%u", Controller, Channel, Dimm, Rank);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, " DqsMapCpu2Dram: ");
        for (Iteration = 0; Iteration < BytesPerChannel; Iteration++) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", ChannelIn->DqsMapCpu2Dram[Dimm][Iteration]);
        }

        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, " DqMapCpu2Dram: ");
        for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
          for (Bit = 0; Bit < MAX_BITS; Bit++) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%d ", ChannelIn->DqMapCpu2Dram[Rank][Byte][Bit]);
          }
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      } // Rank
    } // Channel
  } // Controller
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
#endif // MRC_DEBUG_PRINT
}

/**
  Get the current SaGvPoint from the input MrcData

  @param[in] MrcData  - Pointer to global MRC data.

  @return The current SaGvPoint
**/
MrcSaGvPoint
MrcGetSaGvPoint (
  IN  MrcParameters *const MrcData
  )
{
  return MrcData->Outputs.SaGvPoint;
}

/**
  This function maps a given Ch/Byte to a physical DataShared partition

  @param[in]  MrcData  - The global host structure
  @param[in]  Channel  - The channel to calculate
  @param[in]  Byte     - The byte to calculate
  @param[out] PartByte - The physical byte the input logical channel / byte maps to

  @retval Partition
**/
UINT8
MrcCalcDataPartition (
  IN  MrcParameters *const MrcData,
  IN  UINT8                Channel,
  IN  UINT8                Byte,
  OUT UINT8                *PartByte OPTIONAL
  )
{
  const MrcInput *Inputs;
  MrcOutput      *Outputs;
  BOOLEAN        IsDdr5;
  BOOLEAN        IsNIL;
  UINT8          UpperCh;
  UINT8          OddByte;
  UINT8          ChBy2;
  UINT8          Partition;
  UINT8          UpCh;
  UINT8          PartByteLocal;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  IsNIL   = (Inputs->ExtInputs.Ptr->DqPinsInterleaved) ? 0 : 1;
  IsDdr5  = Outputs->IsDdr5;
  UpCh    = IsDdr5 ? 1 : 3;
  UpperCh = (Channel > UpCh) ? 1 : 0;
  OddByte = (Byte & 0x1);
  ChBy2   = (Channel >> 1);

  Partition = 0;

  if (Byte == 4) { // ECC
    Partition = 8 + UpperCh;
    PartByteLocal = Channel % 2;
  } else {
    if (IsDdr5) {
      if (IsNIL) {
        Partition = (2 * Channel) + OddByte;
        PartByteLocal = (Byte < 2) ? 0 : 1;
      } else {
        Partition = (4 * UpperCh) + Byte;
        PartByteLocal = Channel % 2;
      }
    } else { // Lpddr
      Partition = (2 * ChBy2) + Byte;
      PartByteLocal = Channel % 2;
    }
  }

  if (PartByte != NULL) {
    *PartByte = PartByteLocal;
  }

  return Partition;
}

/**
  This function maps a given Ch to a physical CCCShared partition

  @param[in]  MrcData - The global host structure
  @param[in]  Channel - The channel to calculate

  @retval Partition
**/
UINT8
MrcCalcCccPartition (
  IN  MrcParameters *const MrcData,
  IN  UINT8                Channel
  )
{
  UINT8          ChBy2;
  UINT8          Partition;

  ChBy2   = (Channel >> 1);

  if (MrcData->Outputs.IsDdr5) {
    Partition = Channel;
  } else { // Lpddr
    Partition = ChBy2;
  }

  return Partition;
}

/**
  Set the following bits at first SAGV point before sending PM0 message:
   DDRPHY_MISC_PMA_SAUG_CR_PMCFG.PChannelEn     = 1 (0 for TC)
   PMA_MCMISCSSAUG_CR_PMMESSAGE.SkipRestoreCR   = 1
   PMA_MCMISCSSAUG_CR_PMMESSAGE.SkipRetentionCR = 1
  Set the following bit right after PLL is locked
   SAXG_Enable
   and poll for SAXG_Ready (enable case) / SAXGPwrGood (disable case)

  Set the above bits to the opposite values after sending PM14 for last SAGV point only (except PChannelEn and SkipRestoreCR).
  Also program mptu_rst, mptu_crirst, mptu_en and xt_fe_sram_pg_en to power gate MPTU and FE SRAM.
  If SAGV is disabled then the bits are set and cleared at their respective locations within the same run.

  @param[in] MrcData      - Include all MRC global data.
  @param[in] IsPrePllLock - Defines which bits to enable based on when the function is called i.e. before or after PM0 message.
  @param[in] IsSet        - Decides whether we set or clear the bits accessed in this branch

  @retval mrcSuccess    - SAXG_Ready was set successfully / SAXGPwrGood was cleared successfully
  @retval mrcDeviceBusy - Timed out waiting for the SAXG_Ready / SAXGPwrGood
**/
MrcStatus
MrcPmCfgCrAccess (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsPrePllLock,
  IN BOOLEAN              IsSet
  )
{
  MRC_FUNCTION      *MrcCall;
  MrcInput          *Inputs;
  INT64             GetSetEn;
  INT64             GetSetDis;
  INT64             SaxgReady;
  INT64             SaxgPwrGood;
  UINT64            Timeout;
  BOOLEAN           Busy;
  DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_STRUCT       MptussCtrl0;

  Inputs = &MrcData->Inputs;
  MrcCall = MrcData->Inputs.Call.Func;
  Busy = FALSE;
  GetSetEn = 1;
  GetSetDis = 0;
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout

  // Set
  if (IsSet) {
    if (IsPrePllLock) {
      MrcGetSetNoScope (MrcData, GsmPChannelEn,         WriteCached | PrintValue, &GetSetEn);
      MrcGetSetNoScope (MrcData, GsmSkipRestoreCR,      WriteCached | PrintValue, &GetSetEn);
      MrcGetSetNoScope (MrcData, GsmSkipRetentionCR,    WriteCached | PrintValue, &GetSetEn);
    } else {
      MrcGetSetNoScope (MrcData, GsmSaxgEnable, WriteCached | PrintValue, &GetSetEn);
      MrcWait (MrcData, 10);
      // Poll to check if SAXG_Ready is asserted.
      do {
        MrcGetSetNoScope (MrcData, GsmSaxgReady, ReadNoCache, &SaxgReady);
        Busy = (SaxgReady == 0);
        // Simics does not have this modelled yet so skip polling for Simics
        if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag == 1) {
          Busy = FALSE;
        }
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

      // Power up the SRAMs, they are power gated by default
      MrcPowerGateUcssSrams (MrcData, MRC_DISABLE);
    }
  } else {
    MrcGetSetNoScope (MrcData, GsmSkipRetentionCR,    WriteCached | PrintValue, &GetSetDis);

    if (!Inputs->IsKeepUcssPostMrc) {
      // Disable MPTU
      MptussCtrl0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG);
      MptussCtrl0.Bits.mptu_rst = 1;
      MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG, MptussCtrl0.Data);

      MptussCtrl0.Bits.mptu_crirst = 1;
      MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG, MptussCtrl0.Data);

      MptussCtrl0.Bits.mptu_en = 0;
      MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG, MptussCtrl0.Data);

      MrcGetSetNoScope (MrcData, GsmSaxgEnable,         WriteCached | PrintValue, &GetSetDis);
      MrcWait (MrcData, 10);
      // Poll to check if SAXGPwrGood is deasserted.
      do {
        MrcGetSetNoScope (MrcData, GsmSaxgPwrGood, ReadNoCache, &SaxgPwrGood);
        Busy = (SaxgPwrGood == 1);
        // Simics does not have this modelled yet so skip polling for Simics
        if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag == 1) {
          Busy = FALSE;
        }
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

      // Power down the SRAMs
      MrcPowerGateUcssSrams (MrcData, MRC_ENABLE);
    }
  }
  if (Busy) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s %s on SAXG_%s!\n", gErrString, gTimeout, IsSet ? "Ready" : "PwrGood");
  }

  return (Busy ? mrcDeviceBusy : mrcSuccess);
}

/**
  Switch LPDDR to high operating frequency:
   - Set FSP_OP = 1
   - Lock MC PLL at high frequency

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] DebugPrint - Enable/disable debug printing

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcLpddrSwitchToHigh (
  IN  MrcParameters *const  MrcData,
  IN BOOLEAN                DebugPrint
  )
{
  MrcOutput     *Outputs;
  MrcStatus     Status;
  INT64         GetSetVal;

  Status = mrcSuccess;
  Outputs = &MrcData->Outputs;

  if (Outputs->EctDone) {
    // Set FSP-OP = 1 and VRCG = 1
    // Configuring MrhDelay for 250ns as it's the maximum for LP5.
    GetSetVal = DIVIDECEIL (MRC_LP_tFC_LONG_NS * 1000, Outputs->Wckps);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
    Status = MrcSetFspVrcg (MrcData, ALL_RANK_MASK, LpVrcgHighCurrent, MRC_IGNORE_ARG_8, LpFspOpPoint1);

    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  return Status;
}

/**
  This function enables CK based on population.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval none.
**/
void
MrcCkeOnProgramming (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput     *Outputs;
  INT64         GetSetVal;
  INT32         Channel;
  UINT32        Controller;
  UINT32        ValidRankMask;
  INT64         DdrOneDpc;

  Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr) {
    GetSetVal = 0;
    // Walk the array backwards as 3/1 are sub channels of 0/2 for MC.
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = (MAX_CHANNEL - 1); Channel >= 0; Channel--) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          ValidRankMask = Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;
          if (Channel % 2) {
            //Channel 3/1 are bits 3:2 for Rank Mask.
            GetSetVal = ValidRankMask << 2;
          } else {
            GetSetVal |= ValidRankMask;
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOn, WriteCached | PrintValue, &GetSetVal);
            // Clear the variable for the next set of channels.
            GetSetVal = 0;
          }
        }
      }
    }
  } else {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          // Set the Valid CKE - step 4
          GetSetVal = Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;

          if (Outputs->IsDdr5) {
            // Adjust CKE mask if ddr5_1dpc is enabled on this channel
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccOneDpc, ReadFromCache, &DdrOneDpc);
            if (DdrOneDpc != 0) {
              GetSetVal = 9;  // '1001' - MC is using ranks 0 and 3 when ddr5_1dpc is enabled
            }
          }
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOn, WriteCached, &GetSetVal);
        }
      }
    }
  }
}

/**
  This function programs the CkeOverride field based on population.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval none.
**/
void
MrcCkeOverrideProgramming (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput     *Outputs;
  INT64         GetSetVal;
  INT32         Channel;
  UINT32        Controller;
  UINT32        ValidRankMask;
 
  Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr) {
    GetSetVal = 0;
    // Walk the array backwards as 3/1 are sub channels of 0/2 for MC.
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = (MAX_CHANNEL - 1); Channel >= 0; Channel--) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          ValidRankMask = Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;
          if (Channel % 2) {
            //Channel 3/1 are bits 3:2 for Rank Mask.
            GetSetVal = ValidRankMask << 2;
          } else {
            GetSetVal |= ValidRankMask;
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOverride, WriteCached | PrintValue, &GetSetVal);
            // Clear the variable for the next set of channels.
            GetSetVal = 0;
          }
        }
      }
    }
  } else {
    MrcGetMcConfigGroupLimits (MrcData, GsmMccCkeOverride, NULL, &GetSetVal, NULL);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOverride, WriteCached, &GetSetVal);
  }
}

/**
  Set Near End (NE) and Far End (FE) SRAM Power Gate setting

  @param[in] MrcData      - Include all MRC global data.
  @param[in] PGEnable     - Enable/Disable power gating

  @retval None
**/
VOID
MrcPowerGateUcssSrams (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              PGEnable
  )
{
  UINT64  Timeout;
  BOOLEAN Done;
  DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_STRUCT        MptussCtrl0;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_STRUCT  XtStatus;

  MptussCtrl0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG);

  // Apply FE and NE SRAM PG setting
  MptussCtrl0.Bits.xt_fe_sram_pg_en = PGEnable;
  MptussCtrl0.Bits.xt_ne_sram_pg_en = PGEnable;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG, MptussCtrl0.Data);

  // Wait until SRAM PG setting takes effect
  Timeout = MrcData->Inputs.Call.Func->MrcGetCpuTime() + MRC_WAIT_TIMEOUT;
  do {
    XtStatus.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_REG);
    Done = (BOOLEAN)XtStatus.Bits.xt_sram_pg_status == (PGEnable ? 1 : 0);

    if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag == 1) {
      Done = TRUE;
    }
  } while (!Done && (MrcData->Inputs.Call.Func->MrcGetCpuTime () < Timeout));

  // Clock gate FE SRAM and glue logic
  if (PGEnable) {
    MptussCtrl0.Bits.xt_fe_side_clken = 0;
    MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG, MptussCtrl0.Data);
  }
}

/**
  This function returns the scope of the group type

  @param[in] MrcData - Include all MRC global data.
  @param[in] Param   - group type

  @retval GROUP_SCOPE_TYPE
**/
GROUP_SCOPE_TYPE
MrcGetScope (
  IN MrcParameters* MrcData,
  IN GSM_GT Param
  )
{
  GROUP_SCOPE_TYPE RetVal = GroupScopePerNoScope;

  switch (Param) {
  case GsmMctRDRDdr:
  case GsmMctWRWRdr:
  case GsmMctWRRDdr:
  case GsmMctRDWRdr:
  case GsmMctWRRDdd:
  case GsmMctRDWRdd:
  case GsmMctRDRDdd:
  case GsmMctWRWRdd:
    RetVal = GroupScopePerMcCh;
    break;
  default:
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "\n%s - %d Group is not defined\n", __func__, Param);
  }
  return RetVal;
}
