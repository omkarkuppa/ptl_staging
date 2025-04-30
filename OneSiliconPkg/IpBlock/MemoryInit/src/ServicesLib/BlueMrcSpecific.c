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
#include "BlueMrcIoTest.h"
#include "MrcSagv.h"
#include "MrcIoTest.h"
#include "MrcGeneral.h"
#include "MrcMcSiSpecific.h"


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

      // Power down the SRAMs
      MrcPowerGateUcssSrams (MrcData, MRC_ENABLE);

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

/**
  This function implements the flow to properly switch MC PLL during run time.
  @params[in] MrcData     - Pointer to MRC global data.
  @params[in] NewFreq     - The new frequency to lock MC PLL.
  @params[in] DebugPrint  - Boolean parameter to enable/disable debug messages for the callee.
  @retval MrcStatus - mrcSuccess if frequency is updated properly, otherwise an error status.
**/
MrcStatus
MrcFrequencySwitch (
  IN  MrcParameters* const  MrcData,
  IN  MrcFrequency          NewFreq,
  IN  BOOLEAN               DebugPrint
  )
{
  return mrcSuccess; // Freq Switch not used for Blue MRC
}

/**
  Check whether ParamType is supported.
  @param[in]  ParamType - MRC_MarginTypes: WrV, WrT, RdT, RdV, RcvEnaX, WrDqsT.

  @return IsSupported - TRUE if param is supported.
**/
BOOLEAN
IsDataParamTypeSupported (
  IN MrcMarginTypes ParamType
  )
{
  BOOLEAN IsSupported = FALSE;

  switch (ParamType) {
    case RdT:
    case RdV:
    case WrT:
    case WrV:
    case RcvEnaX:
    case WrDqsT:
      IsSupported = TRUE;
      break;
    default:
      break;
  }

  return IsSupported;
}

/**
  Check whether there is errors at Point RdT/RdV or WrT/WrV
  @param[in]  MrcData            - Include all MRC global data.
  @param[in]  McChBitMask        - Bit mask of present MC channels
  @param[in]  RankMask           - Bit mask of Ranks to change margins for
  @param[in]  MarginPoint        - Margin Point to test
  @param[in]  ParamType          - MRC_MarginTypes: WrV, WrT, RdT, RdV.
  @param[in]  UsePerDeviceValues - Use per device margin values

  @retval MrcStatus - mrcSuccess if point successful pass, otherwise returns an error status.
**/
MrcStatus
MrcDataPointTest (
  IN     MrcParameters*    MrcData,
  IN     UINT8             McChBitMask,
  IN     UINT8             RankMask,
  IN     MarginCheckPoint* MarginPoint,
  IN     MrcMarginTypes    ParamType,
  IN     BOOLEAN           UsePerDeviceValues
  )
{
  MrcOutput* Outputs;
  MrcDebug*  Debug;
  MrcStatus  Status;
  UINT8      AllChannelError;
  INT32      Value;
  UINT8      Controller;
  UINT8      Channel;
  UINT16     Byte;
  UINT32     MaxChannels;
  UINT8      SkipWait;
  UINT8      MaxByte;
#ifdef MRC_DEBUG_PRINT
  UINT64     ErrStatus;
#endif // MRC_DEBUG_PRINT

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MaxChannels = Outputs->MaxChannels;
  // When ParamType is eqal WrV then MaxByte is set to 1 to run WrV per Channel, to run the test for all Bytes ChangeMargin is called with 0x1FF.
  MaxByte = (ParamType == WrV) ? 1 : Outputs->SdramCount;

  if (!IsDataParamTypeSupported (ParamType)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Function MrcDataPointTest, Invalid ParamType: %d\n", ParamType);
    return mrcWrongInputParameter;
  }

  // Change margin before the test
  if ((ParamType == RdV) || (ParamType == WrV)) {
    Value = MarginPoint->VoltageMargin;
  } else {
    Value = MarginPoint->TimingMargin;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
        continue;
      }
      SkipWait = (McChBitMask >> ((Controller * MaxChannels) + (Channel + 1))); // Skip if there are more channels
      if (ParamType == WrV) {
        Byte = (UsePerDeviceValues) ? 0x1FF : 0;
        ChangeMargin (MrcData, ParamType, Value, 0, 0, Controller, Channel, RankMask, Byte, 0, SkipWait);
      } else {
        MaxByte = (UsePerDeviceValues) ? Outputs->SdramCount : 1;
        for (Byte = 0; Byte < MaxByte; Byte++) {
          ChangeMargin (MrcData, ParamType, Value, 0, 0, Controller, Channel, RankMask, Byte, 0, SkipWait);
        }
      }
    }
  }

  // Run Test
  // Rank is not needed here because this is not a CaParity/CaParityPerLane test.
  RunIoTestNoRank (MrcData, McChBitMask, Outputs->DQPat, 1);
  AllChannelError = Cpgc20GetAllChannelsError (MrcData, McChBitMask);

#ifdef MRC_DEBUG_PRINT
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      MrcGetMiscErrStatus (MrcData, Controller, Channel, ByteGroupErrStatus, &ErrStatus);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Param: %s, Mc%uCh%u, Result=%xh\n", gMarginTypesStr[ParamType], Controller, Channel, (UINT16) ErrStatus);
    }
  }
#endif // MRC_DEBUG_PRINT

  // Set whether it passed/failed
  Status = (AllChannelError > 0) ? mrcFail : mrcSuccess;

  // Clean up margin
  ChangeMargin (MrcData, ParamType, 0, 0, 1, 0, 0, RankMask, 0x1FF, 0, 0);

  return Status;
}

/**
  Check whether there is errors at Point RdT/RdV or WrT/WrV
  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      McChBitMask - Bit mask of present MC channels
  @param[in]      RankMask    - Bit mask of Ranks to change margins for
  @param[in]      MarginPoint - Margin Point to test
  @param[in]      ParamType   - MRC_MarginTypes: WrV, WrT, RdT, RdV.
  @retval MrcStatus - mrcSuccess if point successful pass, otherwise returns an error status.
**/
MrcStatus
MrcCmdPointTest (
  IN     MrcParameters    *MrcData,
  IN     UINT8            McChBitMask,
  IN     UINT8            RankMask,
  IN     MarginCheckPoint *MarginPoint,
  IN     MrcMarginTypes   ParamType
  )
{
  MrcOutput*  Outputs;
  MrcDebug*   Debug;
  MrcStatus   Status;
  UINT8       AllChannelError;
  UINT8       SkipWait;
  UINT32      MaxChannels;
  UINT8       Controller;
  UINT8       Channel;
#ifdef MRC_DEBUG_PRINT
  UINT64      ErrStatus;
#endif

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MaxChannels = Outputs->MaxChannels;

  // Check to make sure type is CmdT/CmdV
  if ((ParamType != CmdV) && (ParamType != CmdT)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Function MrcCmdPointTest, Invalid ParamType: %d\n",ParamType);
    return mrcWrongInputParameter;
  }

  // Change CmdV/CmdT before the test
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
        continue;
      }
      SkipWait = (McChBitMask >> ((Controller * MaxChannels) + (Channel + 1))); // Skip if there are more channels
      if (ParamType == CmdV) {
        // Change CmdV
        ChangeMargin (MrcData, MarginPoint->VoltageType, MarginPoint->VoltageMargin, 0, 0, Controller, Channel, RankMask, 0, 0, SkipWait);
      } else {
        // Change CmdT
        ShiftPIforCmdTraining (MrcData, Controller, Channel, MrcIterationCmdCtl, RankMask, 0xFF, MarginPoint->TimingMargin, 0);
      } // CmdT
    }
  }

  // Run Test
  // Rank is not needed here because this is not a CaParity/CaParityPerLane test.
  RunIoTestNoRank (MrcData, McChBitMask, Outputs->DQPat, 1);
  AllChannelError = Cpgc20GetAllChannelsError (MrcData, McChBitMask);

#ifdef MRC_DEBUG_PRINT
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
        continue;
      }
      MrcGetMiscErrStatus (MrcData, Controller, Channel, ByteGroupErrStatus, &ErrStatus);
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Param: %s, Mc%uCh%u, Result=%xh\n", gMarginTypesStr[ParamType], Controller, Channel, (UINT16)ErrStatus);
    }
  }
#endif

  // Set whether it passed/failed
  Status = (AllChannelError > 0) ? mrcFail : mrcSuccess;

  // Clean up margin
  if (ParamType == CmdT) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
          continue;
        }
        ShiftPIforCmdTraining (MrcData, Controller, Channel, MrcIterationCmdCtl, RankMask, 0xFF, 0, 0);
      } // Channel
    } // Controller
    // Normalize per-rank CS timings
    MrcCsPerPinNormalize (MrcData);
  } else {
    // Clean up CmdV
    ChangeMargin (MrcData, MarginPoint->VoltageType, 0, 0, 1, 0, 0, RankMask, 0, 0, 0);
  }

  if (AllChannelError > 0) {
    MrcResetSequence (MrcData);
  }

  return Status;
}

/**
  Check Margin Limits by utilizing box test.

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess if succeeded
**/
MrcStatus
MrcMarginLimitCheck (
  IN OUT MrcParameters* const MrcData
  )
{
  const MrcInput      *Inputs;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  MrcStatus           Status;
  MrcStatus           Pass;
  UINT8               LoopCount;
  UINT8               Controller;
  UINT8               Channel;
  UINT8               McChBitMask;
  UINT8               Rank;
  UINT8               RankMask;
  UINT8               Param;
  UINT8               MarginLevel;
  UINT8               MarginLevelMask;
  UINT8               TimingSign;
  UINT8               VoltageSign;
  UINT8               SignalIdx;
  MarginCheckPoint    MarginCheckL2Table[MAX_MARGIN_CHECK];
  MarginCheckPoint    MarginPoint;
  const MarginCheckPoint *MarginPointTemp;
  BOOLEAN             IsMarginCheckBoth;
  MarginCheckMode     MarginCheckResult;
  UINT8               MaxChannels;
  UINT8               FailResultsT[MAX_MARGIN_CHECK][MAX_RANK_IN_CHANNEL][MAX_EDGES];
  UINT8               FailResultsV[MAX_MARGIN_CHECK][MAX_RANK_IN_CHANNEL][MAX_EDGES];
  UINT8               MarginTypesCount;
  INT64               GetSetVal;
  MRC_MC_AD_SAVE      MadSavedValues;

  static const MarginCheckPoint MarginCheckL1Table[] = {
    { RdT,  12, RdV,  55 },
    { WrT,  12, WrV,  22 },
    { CmdT, 10, CmdV, 10 }
  };

#ifdef MRC_DEBUG_PRINT
  UINT8 ParamV;
  // Margin check strings are used for debug printing
  // Note: The order needs to same as MarginCheckMode Enum
  static const char* MarginCheckStr[] = {
    "Pass",
    "L1",
    "L2",
    "L1/L2"
  };
#endif

  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Outputs = &MrcData->Outputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Debug = &Outputs->Debug;
  MaxChannels = Outputs->MaxChannels;
  Status = mrcSuccess;
  MarginTypesCount = (Outputs->IsDdr5) ? MAX_MARGIN_CHECK : (MAX_MARGIN_CHECK - 1); // LP5 doesn't support CmdT/CmdV

  MrcCall->MrcSetMem ((UINT8 *) FailResultsT, sizeof (FailResultsT), 0);
  MrcCall->MrcSetMem ((UINT8 *) FailResultsV, sizeof (FailResultsV), 0);
  MrcCall->MrcSetMem ((UINT8 *) MarginCheckL2Table, sizeof (MarginCheckL2Table), 0);

  IsMarginCheckBoth = (ExtInputs->MarginLimitCheck == Margin_Check_Both);

  // Setup Loop Count
  LoopCount = (ExtInputs->RMTLoopCount != 0) ? ExtInputs->RMTLoopCount : 12;


#ifdef MRC_DEBUG_PRINT
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "L1 limits:\n");
  for (SignalIdx = 0; SignalIdx < MarginTypesCount; SignalIdx++) {
    Param = MarginCheckL1Table[SignalIdx].TimingType;
    ParamV = MarginCheckL1Table[SignalIdx].VoltageType;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %s:\t%d\t%s:\t%d\n",
      gMarginTypesStr[Param], MarginCheckL1Table[SignalIdx].TimingMargin,
      gMarginTypesStr[ParamV], MarginCheckL1Table[SignalIdx].VoltageMargin
    );
  }
#endif // MRC_DEBUG_PRINT

  MrcModifyRdRdTimings (MrcData, TRUE);
  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcSaveEnum, &MadSavedValues);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  MrcModifyMcAddressDecoderValues (MrcData);

  // Setup L2 table
  for (SignalIdx = 0; SignalIdx < MarginTypesCount; SignalIdx++) {
    MarginCheckL2Table[SignalIdx].TimingType = MarginCheckL1Table[SignalIdx].TimingType;
    MarginCheckL2Table[SignalIdx].TimingMargin = (INT8) ((MarginCheckL1Table[SignalIdx].TimingMargin * ExtInputs->MarginLimitL2) / 100);
    MarginCheckL2Table[SignalIdx].VoltageType = MarginCheckL1Table[SignalIdx].VoltageType;
    MarginCheckL2Table[SignalIdx].VoltageMargin = (INT8) ((MarginCheckL1Table[SignalIdx].VoltageMargin * ExtInputs->MarginLimitL2) / 100);
  }

  for (SignalIdx = 0; SignalIdx < MarginTypesCount; SignalIdx++) {
    Param = MarginCheckL1Table[SignalIdx].TimingType;

    // Note: The following statement depends on the order of MarginCheckL1Table.
    if (Param == RdT) {
      // Test Margin for RdT/RdV, WrT/WrV, CmdT/CmdV
      // SOE=1, EnCADB=0, EnCKE=0 SOE=1 sets bit12 of REUT_CH_ERR_CTL
      SetupIOTestStatic (MrcData, Outputs->McChBitMask, LoopCount, NSOE, 0, 0, PatWrRd, 0, 0, 0x5a);
    }

    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      // Select rank for REUT test
      RankMask = 1 << Rank;

      McChBitMask = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          McChBitMask |= SelectReutRanks (MrcData, Controller, Channel, RankMask, FALSE);
        }
      }

      // Continue with next rank if this rank is not present on any channel
      if (McChBitMask == 0) {
        continue;
      }

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Rank%u\n", Rank);

      if (Outputs->IsDdr5) {
        // Update bank mapping to get B2B writes.
        MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask, TRUE);
      }

      for (MarginLevel = Margin_Check_L2; MarginLevel > Margin_Check_Disable; MarginLevel--) {

        // check wether to run margin level
        MarginLevelMask = 1 << (MarginLevel - 1);
        if ((MarginLevelMask & ExtInputs->MarginLimitCheck) == 0) {
          continue;
        }

        if (MarginLevel == Margin_Check_L2) {
          MarginPointTemp = &MarginCheckL2Table[SignalIdx];
        } else {
          MarginPointTemp = &MarginCheckL1Table[SignalIdx];
        }

        MrcCall->MrcCopyMem ((UINT8 *) &MarginPoint, (UINT8 *) MarginPointTemp, sizeof (MarginPoint));

        for (TimingSign = 0; TimingSign < 2; TimingSign++) {
          // Skip testing point on L1 check if L2 check passed
          if ((MarginLevel == Margin_Check_L1) && (FailResultsT[SignalIdx][Rank][TimingSign] != Margin_Check_L2 && IsMarginCheckBoth)) {
            continue;
          }
          MarginPoint.TimingMargin = MarginPointTemp->TimingMargin * ((TimingSign * 2) - 1);
          // test margin point
          if (Param == CmdT) {
            Pass = MrcCmdPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, MarginPoint.TimingType);
          } else {
            Pass = MrcDataPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, MarginPoint.TimingType, TRUE);
          }
          if (Pass != mrcSuccess) {
            FailResultsT[SignalIdx][Rank][TimingSign] = ((MarginLevel == Margin_Check_L1) && IsMarginCheckBoth) ? Margin_Check_Both : MarginLevel;
          }
        } // TimingSign

        for (VoltageSign = 0; VoltageSign < 2; VoltageSign++) {
          // Skip testing point on L1 check if L2 check passed
          if ((MarginLevel == Margin_Check_L1) && (FailResultsV[SignalIdx][Rank][VoltageSign] != Margin_Check_L2 && IsMarginCheckBoth)) {
            continue;
          }
          MarginPoint.VoltageMargin = MarginPointTemp->VoltageMargin * ((VoltageSign * 2) - 1);
          // test margin point
          if (Param == CmdT) {
            Pass = MrcCmdPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, MarginPoint.VoltageType);
          } else {
            Pass = MrcDataPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, MarginPoint.VoltageType, TRUE);
          }

          if (Pass != mrcSuccess) {
            FailResultsV[SignalIdx][Rank][VoltageSign] = ((MarginLevel == Margin_Check_L1) && IsMarginCheckBoth) ? Margin_Check_Both : MarginLevel;
          }
        } // VoltageSign
      } // MarginLevel
    }  // Rank
  }  // Param

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Margin Check level\nParams: \tRdT/RdV\t\t\t\tWrT/WrV%s\n", (Outputs->IsDdr5) ? "\t\t\t\tCmdT/CmdV" : "");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t\t+V\t-V\t+T\t-T\t+V\t-V\t+T\t-T%s", (Outputs->IsDdr5) ? "\t+V\t-V\t+T\t-T" : "");

  MarginCheckResult = 0;

  // Find the Worst level result
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if (((1 << Rank) & Outputs->ValidRankMask) == 0) {
      // Skip if this rank is not present on any of the channels
      continue;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nR%d\t\t", Rank);

    for (SignalIdx = 0; SignalIdx < MarginTypesCount; SignalIdx++) {
      for (TimingSign = 0; TimingSign < 2; TimingSign++) {
        if (MarginCheckResult < FailResultsT[SignalIdx][Rank][TimingSign]) {
          MarginCheckResult = FailResultsT[SignalIdx][Rank][TimingSign];
        }
      }  // TimingSign
      for (VoltageSign = 0; VoltageSign < 2; VoltageSign++) {
        if (MarginCheckResult < FailResultsV[SignalIdx][Rank][VoltageSign]) {
          MarginCheckResult = FailResultsV[SignalIdx][Rank][VoltageSign];
        }
      }// VoltageSign
      MRC_DEBUG_MSG (Debug,
        MSG_LEVEL_NOTE,
        "%s\t%s\t%s\t%s\t",
        MarginCheckStr[FailResultsV[SignalIdx][Rank][1]],
        MarginCheckStr[FailResultsV[SignalIdx][Rank][0]],
        MarginCheckStr[FailResultsT[SignalIdx][Rank][1]],
        MarginCheckStr[FailResultsT[SignalIdx][Rank][0]]);
    }  // SignalIdx
  }  // Rank

  MrcModifyRdRdTimings (MrcData, FALSE);
  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcRestoreEnum, &MadSavedValues);

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  // Determine if a cold boot required
  Status = (MarginCheckResult == ExtInputs->MarginLimitCheck) ? mrcResetFullTrain : mrcSuccess;


  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nMargin check result:%s Frequency %d \n", MarginCheckStr[MarginCheckResult], Outputs->Frequency);

  if (Status == mrcResetFullTrain) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Margin Check Failed - cold boot required\n");
  }

  return Status;
}

/**
  This function is used to move CMD/CTL/CLK/CKE PIs during training

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     Controller - Controller to shift PI.
  @param[in]     Channel    - Channel to shift PI.
  @param[in]     Iteration  - Determines which PI to shift:
                              MrcIterationClock  = 0
                              MrcIterationCmd    = 1
                              MrcIterationCtl    = 2
                              MrcIterationCmdCtl = 3
  @param[in]     RankMask   - Ranks to work on
  @param[in]     GroupMask  - Which groups to work on for CLK/CMD/CTL. See MrcGetCmdGroupMax()
  @param[in]     NewValue   - value to shift in case of CLK Iteration, New value for all other cases
  @param[in]     UpdateHost - Determines if MrcData structure is updated

  @retval mrcSuccess if successful, otherwise the function returns an error if the register setting was out of range for MrcIterationCmdCtl.
**/
MrcStatus
ShiftPIforCmdTraining (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT8          Iteration,
  IN     const UINT8          RankMask,
  IN     const UINT8          GroupMask,
  IN     INT32                NewValue,
  IN     const UINT8          UpdateHost
  )
{
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  MrcIntCmdTimingOut  *IntCmdTiming;
  GSM_GT              GsmMode;
  INT64               GetSetVal;
  INT64               GetSetValZero;
  INT64               GroupLimitMax;
  UINT8               Rank;
  UINT8               Group;
  UINT8               CmdGroupMax;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  IntCmdTiming  = &Outputs->Controller[Controller].CmdTiming[Channel];
  GsmMode       = ForceWriteCached;
  CmdGroupMax   = MrcGetCmdGroupMax (MrcData);
  GetSetValZero = 0;

  MrcGetSetLimits (MrcData, CmdGrpPi, 0, NULL, &GroupLimitMax, NULL);


  if (Iteration == MrcIterationCmd || Iteration == MrcIterationCtl) {
    if ((NewValue < 0) || (NewValue > GroupLimitMax)) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Mc%d.C%d ShiftPIforCmdTraining: value (%d) out of limits, ", Controller, Channel, NewValue);
      if (NewValue < 0) {
        NewValue = 0;
      } else if (NewValue > GroupLimitMax) {
        NewValue = (UINT16)GroupLimitMax;
      }
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "New value = %d\n", NewValue);
    }
  }
  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nShiftPIforCmdTraining: Iteration: %d, Channel: %d, RankMask: %d, GroupMask: %d, NewValue = 0x%x\n", Iteration, Channel, RankMask, GroupMask, NewValue);

  switch (Iteration) {
    case MrcIterationCmdCtl:
    case MrcIterationCmd: // Shift Command
      GetSetVal = NewValue;
      for (Group = 0; Group < CmdGroupMax; Group++) {
        if ((1 << Group) & GroupMask) {
          if (Iteration == MrcIterationCmdCtl) {
            GetSetVal = (INT16)(IntCmdTiming->CmdPiCode[Group] + NewValue);
            if ((GetSetVal < 0) || (GetSetVal > GroupLimitMax)) { // Check for out-of-bounds
              return mrcRegisterSettingOutOfBounds;
            }
          }
          MrcGetSetCcc (MrcData, Controller, Channel, MRC_IGNORE_ARG, Group, CmdGrpPi, GsmMode, &GetSetVal);
          if (UpdateHost) {
            IntCmdTiming->CmdPiCode[Group] = (UINT16) GetSetVal;
          }
        }
      }
      if (Iteration == MrcIterationCmd) {
        break;
      }
      /*FALLTHROUGH*/

    case MrcIterationCtl: // Shift CS/ODT
      GetSetVal = NewValue;
      // Shift CS/ODT
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (RankMask & (1 << Rank)) {
          if (Iteration == MrcIterationCmdCtl) {
            GetSetVal = (INT16)(IntCmdTiming->CtlPiCode[Rank] + NewValue);
            if ((GetSetVal < 0) || (GetSetVal > GroupLimitMax)) { // Check for out-of-bounds
              return mrcRegisterSettingOutOfBounds;
            }
          }
          // Clear the per rank offset before setting the CCC Channel shared timing
          MrcGetSetCccLane (MrcData, Controller, Channel, MRC_IGNORE_ARG, Rank, CsPerBitCcc, GsmMode, &GetSetValZero);
          // Set the CCC Channel shared timing
          MrcGetSetCcc (MrcData, Controller, Channel, Rank, 0, CtlGrpPi, GsmMode, &GetSetVal);
          if (UpdateHost) {
            IntCmdTiming->CtlPiCode[Rank] = (UINT16) GetSetVal;
          }
        }
      }
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%sUnknown parameter to shift\n", gWarnString);
      break;
  }

  return mrcSuccess;
}

/**
  This function is used by most margin search functions to change the underlying margin parameter.
  This function allows single search function to be used for different types of margins with minimal impact.
  It provides multiple different parameters, including 2D parameters like Read or Write FAN.
  It can work in either MultiCast or single register mode.

  @param[in,out] MrcData       - Include all MRC global data.
  @param[in]     param         - Includes parameter(s) to change including two dimensional.
  @param[in]     value0        - Selected value to program margin param to
  @param[in]     value1        - Selected value to program margin param to in 2D mode (FAN mode)
  @param[in]     EnMultiCast   - To enable Multicast (broadcast) or single register mode
  @param[in]     Controller    - Desired Memory Controller
  @param[in]     Channel       - Desired Channel
  @param[in]     RankMaskIn    - Desired Rank mask that changes should be applied to
  @param[in]     byte          - Desired byte offset register
  @param[in]     UpdateMrcData - Used to decide if MRC host must be updated
  @param[in]     SkipWait      - Used to skip wait until all channel are done

  @retval MrcStatus -  if succeeded, return mrcSuccess
**/
MrcStatus
ChangeMargin (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          param,
  IN     const INT32          value0,
  IN     const INT32          value1,
  IN     const UINT8          EnMultiCast,
  IN     const UINT8          Controller,
  IN     const UINT8          Channel,
  IN     const UINT8          RankMaskIn,
  IN     const UINT16         byte,
  IN     const UINT8          UpdateMrcData,
  IN     const UINT8          SkipWait
  )
{
  // Programs margin param to the selected value0
  // If param is a 2D margin parameter (ex: FAN), then it uses both value0 and value1
  //    For an N point 2D parameter, value1 can be an integer from 0 to (N-1)
  //    For per bit timing parameter, value1 is the sign of the shift
  //   param = {0:RcvEn, 1:RdT, 2:WrT, 3: WrDqsT, 4:RdV, 5:WrV, 6:WrLevel,
  //         7:RdVBit, 8:RdVBit1, 9:RcvEnaX, 10:WrTUnMatched, 11:CmdT, 12:CmdV,
  //         13:CtlV, 14:RdTN, 15:RdTP, 16:CmdTBit, 17:RdTDbi, 18:WrTDbi,
  //         19:RdVDbi, 20:WrTBit, 21:RdTBit}
  // Note: For Write Vref, the trained value and margin register are the same
  // Note: rank is only used for the RxTBit, TxTBit, RdVBit and RdVBit1 settings, and to propagate RdVref
  // Note: PerBit Settings (RdVBit, RdVBit1) provide all 8 offsets in value0

  MrcDebug          *Debug;
  MrcOutput         *Outputs;
  MrcIntCmdTimingOut *IntCmdTiming;
  MrcStatus         Status;
  GSM_GT            Group[MRC_CHNG_MAR_GRP_NUM];
  INT64             GetSetVal[MRC_CHNG_MAR_GRP_NUM];
  UINT32            GsmMode;
  UINT32            CurrentMc;
  UINT32            McStart;
  UINT32            McEnd;
  UINT32            ChannelStart;
  UINT32            ChannelEnd;
  UINT32            McChannelMask;
  UINT32            ByteStart;
  UINT32            ByteEnd;
  UINT32            CurrentCh;
  UINT32            CurrentRank;
  UINT32            CurrentByte;
  UINT32            GrpIdx;
  UINT8             CmdGroupMax;
  UINT8             CmdGroup;
  UINT16            Max0;
  INT32             v0;
  INT32             v0Input;
  INT32             Min;
  BOOLEAN           UpdateGrp[MRC_CHNG_MAR_GRP_NUM];
  BOOLEAN           PdaMode;
  BOOLEAN           IsDdr5;
  UINT8             RankMask;
  UINT16            DeviceMask;
  GSM_GT            GsmGroup;

#if defined(LOCAL_STUB_STATE_FLAG)
  MRC_LOCAL_STUB_STATE_STRUCT LsState;
#endif

  Status   = mrcSuccess;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  IsDdr5   = Outputs->IsDdr5;
  RankMask = RankMaskIn;

  if (IsPerRankMarginParam (param)) {
    if ((EnMultiCast == 1) && (RankMask == 0)) {
      // If EnMultiCast is 1 and RankMask is 0 assume Rank Multicast
      RankMask = 0xF;
    }
  } else {
    // For non controllable per rank parameter don't use RankMask
    RankMask = 0;
  }

  // Used for CMDV and WRV
  McChannelMask = CalcMcChannelMask (MrcData, param, Controller, Channel, EnMultiCast);
  // Used for CMDT
  CmdGroupMax = MrcGetCmdGroupMax (MrcData);
  GsmMode = (UpdateMrcData) ? ForceWriteCached : ForceWriteUncached;
  for (v0 = 0; v0 < MRC_CHNG_MAR_GRP_NUM; v0++) {
    UpdateGrp[v0] = FALSE;
    Group[v0]     = GsmGtMax;
    GetSetVal[v0] = 0;
  }
  if (EnMultiCast == 0) {
    McStart = Controller;
    McEnd = Controller + 1;
    ChannelStart = Channel;
    ChannelEnd = Channel + 1;
    ByteStart = (UINT8) byte;
    ByteEnd = (UINT8) byte + 1;
  } else {
    McStart = 0;
    McEnd = MAX_CONTROLLER;
    ChannelStart = 0;
    ChannelEnd = Outputs->MaxChannels;
    ByteStart = 0;
    ByteEnd = Outputs->SdramCount;
  }
  // Pre-Process the margin numbers
  Max0 = 0;

  if ((param == WrT) || (param == RdT) || (param == CmdT) || (param == RcvEnaX) || (param == WrDqsT)) {
    GsmGroup = MrcGetChangeMarginGroup (MrcData, param);
    if (GsmGroup == MRC_INT32_MAX) {
      return mrcWrongInputParameter;
    }
    MrcGetSetLimits (MrcData, GsmGroup, 0, NULL, &GetSetVal[0], NULL);
    Max0 = (UINT16)GetSetVal[0];
  } else if (param == RdV) {
    Max0 = GetVrefOffsetLimits (MrcData, param); // Vref for RdV and RdFan modes
  }
  // Pre-Process the margin numbers.
  v0    = value0;

  // Value parameter checking is handled within MrcUpdateVref for WrV and CmdV
  if ((param != WrV) && (param != CmdV)) {
    Min = (-1) - Max0;
    v0 = RANGE (v0, Min, Max0);
  }

  switch (param) {
    case RdT:
      UpdateGrp[0]  = TRUE;
      GetSetVal[0]  = v0;
      Group[0]      = RxDqsOffset;
      break;

    case WrT:
      UpdateGrp[0]  = TRUE;
      GetSetVal[0]  = v0;
      Group[0]      = TxDqOffset;
      break;

    case RdV:
      UpdateGrp[0]  = TRUE;
      GetSetVal[0]  = v0;
      Group[0]      = RxVrefOffset;
      break;

    case WrDqsT:
      UpdateGrp[0]  = TRUE;
      GetSetVal[0]  = v0;
      Group[0]      = TxDqsOffset;
      break;

    case RcvEnaX:
      UpdateGrp[0]  = TRUE;
      GetSetVal[0]  = v0 * RCVENAX_STEPSIZE;
      Group[0]      = RecEnOffset;
      break;

    case CmdV:
      PdaMode = IsDdr5 && (byte != 0); // Force DDR5 PDA for CmdV to OFF until specified otherwise
      MrcUpdateVref (MrcData, McChannelMask, RankMask, byte, param, v0, UpdateMrcData, PdaMode, SkipWait, TRUE);
      if (IsDdr5 && (byte == 0) && (!Outputs->IsOCProfile)) {
        for (CurrentRank = 0; CurrentRank < MAX_RANK_IN_CHANNEL; CurrentRank++) {
          if ((1 << CurrentRank) & RankMask) {
            for (CurrentMc = McStart; CurrentMc < McEnd; CurrentMc++) {
              for (CurrentCh = ChannelStart; CurrentCh < ChannelEnd; CurrentCh++) {
                if (MrcRankExist (MrcData, CurrentMc, CurrentCh, CurrentRank)) {
                  MrcDdr5SetVrefCsCachedOffset (MrcData, CurrentMc, CurrentCh, CurrentRank, v0, UpdateMrcData, FALSE);
                }
              }
            }
          }
        }
      }
      break;

    case CmdT:
      for (CurrentMc = McStart; CurrentMc < McEnd; CurrentMc++) {
        for (CurrentCh = ChannelStart; CurrentCh < ChannelEnd; CurrentCh++) {
          if (MrcChannelExist (MrcData, CurrentMc, CurrentCh)) {
            IntCmdTiming = &Outputs->Controller[CurrentMc].CmdTiming[CurrentCh];
            for (CmdGroup = 0; CmdGroup < CmdGroupMax; CmdGroup++) {
              v0Input = (INT32)(IntCmdTiming->CmdPiCode[CmdGroup]) + v0;
              ShiftPIforCmdTraining (MrcData, CurrentMc, CurrentCh, MrcIterationCmd, MRC_IGNORE_ARG_8, 1 << CmdGroup, v0Input, UpdateMrcData);
            }
          }
        }
      }
      break;

    case WrV:
#if defined(LOCAL_STUB_STATE_FLAG)
      LsState.Data = MrcReadCR64 (MrcData, MRC_LOCAL_STUB_STATE_REG);
      LsState.Bits.CurrentWrV = v0;
      MrcWriteCR64(MrcData, MRC_LOCAL_STUB_STATE_REG, LsState.Data);
#endif
      PdaMode     = IsDdr5 && (byte != 0);
      DeviceMask  = PdaMode ? byte  : 1;
      MrcUpdateVref (MrcData, McChannelMask, RankMask, DeviceMask, param, v0, UpdateMrcData, PdaMode, SkipWait, TRUE);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Function ChangeMargin, Invalid parameter %d\n", param);
      return mrcWrongInputParameter;
  } // end switch (param)

  if ((param == RdV) && (EnMultiCast == 1) && UpdateGrp[0]) { // Direct CR multicast for speed
    MrcWriteDirectMulticast (MrcData, param, v0);
  } else {
    for (GrpIdx = 0; GrpIdx < MRC_CHNG_MAR_GRP_NUM; GrpIdx++) {
      if (UpdateGrp[GrpIdx] == FALSE) {
        continue;
      }
      // Write CR
      for (CurrentMc = McStart; CurrentMc < McEnd; CurrentMc++) {
        for (CurrentCh = ChannelStart; CurrentCh < ChannelEnd; CurrentCh++) {
          if (MrcChannelExist (MrcData, CurrentMc, CurrentCh)) {
            for (CurrentByte = ByteStart; CurrentByte < ByteEnd; CurrentByte++) {
              MrcGetSetChStrb (
                MrcData,
                CurrentMc,
                CurrentCh,
                CurrentByte,
                Group[GrpIdx],
                GsmMode,
                &GetSetVal[GrpIdx]
                );
            } // CurrentByte
          } //MrcChannelExist
        } // CurrentCh
      } // Controller
    } // GrpIdx
  }

  return Status;
}

/**
  This function maps the Change Margin param to the Get Set group
  This function only supports params: RcvEna, RdT, WrT, WrLevel, WrDqsT, RcvEnaX, WrTUnMatched

  @param[in] MrcData  - Include all MRC global data.
  @param[in] param    - Change margin parameter

  return GSM_GT - The Get Set group that maps to the input parameter
**/
GSM_GT
MrcGetChangeMarginGroup (
  IN  MrcParameters* const MrcData,
  IN  const UINT8          param
  )
{
  MrcDebug* Debug;
  Debug = &MrcData->Outputs.Debug;

  switch (param) {
    case RdT:
      return RxDqsOffset;
      break;
    case WrT:
      return TxDqOffset;
      break;
    case CmdT:
      return CmdGrpPi;
      break;
    case RcvEnaX:
      return RecEnOffset;
      break;
    case WrDqsT:
      return TxDqsOffset;
      break;
    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Param %d is unsupported\n", gErrString, param);
      return MRC_INT32_MAX;
      break;
  }
}

/**
  Enable / Disable CA tristate on populated MPTU's

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     IsTrisate  - TRUE : Enable Tristate, FALSE : Restore TRI_CA to original programming
  @param[in]     TriCaSave  - TRI_CA value to be restored when IsTristate is FALSE

  @retval  Original TRI_CA value
**/
INT64
MrcTristateCa (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              IsTristate,
  IN     INT64                TriCaSave
  )
{
  return TRUE; // MrcTristateCa not used for Blue MRC
}

/**
  Rank Margin Tool for Blue MRC.
  Measure Margins across various parameters.

  @param[in, out] MrcData - Include all MRC global data.

  @returns MrcStatus -  mrcSuccess if succeeded
**/
MrcStatus
MrcRankMarginToolCpgc (
  IN OUT MrcParameters * const  MrcData
  )
{

  MrcStatus Status;
  Status    = mrcSuccess;

#ifdef MRC_DEBUG_PRINT
  const MrcInput             *Inputs;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;
  MrcDebug                   *Debug;
  MrcOutput                  *Outputs;
  MrcStatus                  CheckStatus;
  UINT64                     ErrStatus;
  UINT8                      Controller;
  UINT8                      Channel;
  UINT8                      McChBitMask;
  UINT8                      McChBitMaskSaved;
  UINT8                      Rank;
  UINT8                      RankMask;
  UINT8                      Param;
  UINT8                      Sign;
  UINT8                      ParamIdx;
  MarginCheckPoint           MarginPoint;
  UINT8                      MaxChannels;
  UINT8                      ParamCount;
  UINT8                      LoopCount;
  INT64                      GetSetVal;
  MRC_MC_AD_SAVE             MadSavedValues;
  UINT8                      MaxMargin;
  UINT8                      CachedMaxMargin[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                      ParamList[] = { RcvEnaX, WrDqsT, RdT, WrT, RdV, WrV, CmdT, CmdV };
  INT16                      CheckResultTable[ARRAY_COUNT (ParamList)][MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_EDGES];
  UINT16                     TwoFail[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                      MarginStep;
  UINT16                     MarginValue;
  BOOLEAN                    KeepGoing;

  Inputs           = &MrcData->Inputs;
  Outputs          = &MrcData->Outputs;
  ExtInputs        = Inputs->ExtInputs.Ptr;
  Debug            = &Outputs->Debug;
  MaxChannels      = Outputs->MaxChannels;
  ParamCount       = ARRAY_COUNT (ParamList);
  ErrStatus        = 0;
  Channel          = 0;
  MarginStep       = 1;
  MarginValue      = 0;
  McChBitMaskSaved = 0;
  KeepGoing        = TRUE;

  // Setup Loop Count
  LoopCount = (ExtInputs->RMTLoopCount != 0) ? ExtInputs->RMTLoopCount : 12;

  MrcModifyRdRdTimings (MrcData, TRUE);
  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcSaveEnum, &MadSavedValues);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  MrcModifyMcAddressDecoderValues (MrcData);

  SetupIOTestStatic (MrcData, Outputs->McChBitMask, LoopCount, NSOE, 0, 0, PatWrRd, 0, 0, 0x5a);

  for (ParamIdx = 0; ParamIdx < ParamCount; ParamIdx++) {
    Param = ParamList[ParamIdx];

    if ((Param == WrDqsT) && Outputs->IsLpddr5) {
      continue;
    }

    if ((Param == RdV) || (Param == WrV) || (Param == CmdV)) {
      MaxMargin = GetVrefOffsetLimits (MrcData, Param);
    } else if (Param == RcvEnaX) {
      MaxMargin = MAX_POSSIBLE_TIME_RCVENX_RMT;
    } else if (Param == CmdT) {
      MaxMargin = PI_HIGH_LP5_CACS;
    } else {
      MaxMargin = MAX_POSSIBLE_TIME;
    }

    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      // Select rank for REUT test
      RankMask    = 1 << Rank;
      McChBitMask = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          McChBitMask     |= SelectReutRanks (MrcData, Controller, Channel, RankMask, FALSE);
          McChBitMaskSaved = McChBitMask;
        }
      }

      // Continue with next rank if this rank is not present on any channel
      if (McChBitMask == 0) {
        continue;
      }

      if (Outputs->IsDdr5) {
        // Update bank mapping to get B2B writes.
        MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask, TRUE);
      }

      for (Sign = 0; Sign < 2; Sign++) {
        KeepGoing = TRUE;
        CheckResultTable[ParamIdx][Controller][Channel][Rank][Sign] = 0;

        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          for (Channel = 0; Channel < MaxChannels; Channel++) {
            TwoFail[Controller][Channel] = 0;
            CachedMaxMargin[Controller][Channel] = MaxMargin;

            if ((Param == RdT) || (Param == RdV)) {
              CachedMaxMargin[Controller][Channel] = MrcCalcMaxRxMargin (MrcData, Param, Controller, Channel, RankMask, 0, MRC_IGNORE_ARG_8, Sign, MaxMargin);
            } else if ((Param == WrV) || (Param == CmdV)) {
              CachedMaxMargin[Controller][Channel] = MrcCalcMaxVrefMargin (MrcData, Controller, Channel, RankMask, 0, Param, Sign, MaxMargin, FALSE);
            }
          }
        }

        for (MarginValue = MarginStep; (MarginValue <= MaxMargin) && KeepGoing; MarginValue += MarginStep) {
          MarginPoint.TimingType    = Param;
          MarginPoint.VoltageType   = Param;
          MarginPoint.TimingMargin  = MarginValue * (Sign == 0 ? -1 : 1);
          MarginPoint.VoltageMargin = MarginValue * (Sign == 0 ? -1 : 1);

          // This avoids running unnecessary tests and prevents clamping.
          for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
            for (Channel = 0; Channel < MaxChannels; Channel++) {
              if ((MarginValue > CachedMaxMargin[Controller][Channel])
                  || (TwoFail[Controller][Channel] >= 2)) {
                McChBitMask &= ~(1 << MC_CH_IDX (Controller, Channel, Outputs->MaxChannels));
              }
            }
          }

          // Test margin point
          if ((Param == CmdT) || (Param == CmdV)) {
            CheckStatus = MrcCmdPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, Param);
          } else {
            CheckStatus = MrcDataPointTest (MrcData, McChBitMask, RankMask, &MarginPoint, Param, Param != WrV);
          }

          KeepGoing = FALSE;
          for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
            for (Channel = 0; Channel < MaxChannels; Channel++) {
              if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
                continue;
              }

              MrcGetMiscErrStatus (MrcData, Controller, Channel, ByteGroupErrStatus, &ErrStatus);

              if ((UINT16)ErrStatus != mrcSuccess) {
                TwoFail[Controller][Channel]++;
              } else {
                TwoFail[Controller][Channel] = 0;
              }

              if (TwoFail[Controller][Channel] < 2) {
                KeepGoing = TRUE;
              }

              if ((TwoFail[Controller][Channel] == 1) || (CheckStatus == mrcSuccess)) {
                CheckResultTable[ParamIdx][Controller][Channel][Rank][Sign] = MarginValue * (Param == RcvEnaX ? RCVENAX_STEPSIZE : 1);
              }
            }
          }
        }

        McChBitMask = McChBitMaskSaved;
      }
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%sBLUE_RMT\n", gStartTagStr);

  MRC_DEBUG_MSG (
                 Debug,
                 MSG_LEVEL_NOTE,
                 "Params:\t\t RcvEnaX%s RdT\t\t WrT\t\t RdV\t\t WrV\t\t CmdT\t\t CmdV\n",
                 (Outputs->IsDdr5) ? "\t WrDqsT\t\t" : "\t"
                 );

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u.C%u.R%u:", Controller, Channel, Rank);
          for (ParamIdx = 0; ParamIdx < ParamCount; ParamIdx++) {
            Param = ParamList[ParamIdx];
            if ((Param == WrDqsT) && Outputs->IsLpddr5) {
              continue;
            }

            MRC_DEBUG_MSG (
                           Debug,
                           MSG_LEVEL_NOTE,
                           "\t%4d %4d",
                           (CheckResultTable[ParamIdx][Controller][Channel][Rank][0] * -1),
                           CheckResultTable[ParamIdx][Controller][Channel][Rank][1]
                           );
          }

          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
        }
      }
    }
  }

  MrcModifyRdRdTimings (MrcData, FALSE);
  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcRestoreEnum, &MadSavedValues);

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);
#endif // MRC_DEBUG_PRINT

  return Status;
}
