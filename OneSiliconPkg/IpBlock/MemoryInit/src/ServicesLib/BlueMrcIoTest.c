/** @file
  This file include IoTest methods.

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
#include "BlueMrcIoTest.h"
#include "MrcCpgcApi.h"
#include "MrcDdrCommon.h"
#include "MrcReset.h"
#include "BlueTestDataEngine.h"
#include "Cpgc20TestCtl.h"
#include "MrcMcApi.h"
#include "MrcPostCodes.h"

/**
  Programs all the key registers to define a CPGC test as per input mask and Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program
  This function shall be used in Blue MRC CPGC-related calls

  @param[in] MrcData        - Include all MRC global data.
  @param[in] McChBitMask    - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CmdPat         - [0: PatWrRd (Standard Write/Read Loopback),
                               1: PatWr (Write Only),
                               2: PatRd (Read Only),
                               3: PatRdWrTA (ReadWrite Turnarounds),
                               4: PatWrRdTA (WriteRead Turnarounds)
  @param[in] NumCL          - Number of Cache lines transactions per algorithm instruction within 1 BlockRepeat.
                              Non-zero Inputs->NumCL will override this.
  @param[in] LcExp          - Loop Count exponent. Non-zero Inputs->LoopCount will override this.
  @param[in] AddressArray   - 2D Array of Structure that stores address related settings
  @param[in] SOE            - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
  @param[in] PatCtlPtr      - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in] SubSeqWait     - # of Dclks to stall at the end of a sub-sequence
  @param[in] CapNotPowerOf2 - Whether non-power of 2 capacity found per MC/CH
**/
VOID
SetupIOTestCpgc (
  IN MrcParameters *const         MrcData,
  IN const UINT8                  McChBitMask,
  IN const UINT8                  CmdPat,
  IN UINT32                       NumCL,
  IN UINT8                        LcExp,
  IN MRC_ADDRESS                  AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN const MRC_TEST_STOP_TYPE     SOE,
  IN MRC_PATTERN_CTL *const       PatCtlPtr,
  IN UINT16                       SubSeqWait,
  IN BOOLEAN                      CapNotPowerOf2[MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  )
{
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MRC_FUNCTION      *MrcCall;
  UINT32            ChunkMask;
  UINT8             Byte;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             IpChannel;
  UINT8             OldRankCount;
  UINT64            DqLaneMask;
  UINT32            BlockRepeats;
  BOOLEAN           IsDdr5;
  BOOLEAN           IsPatSrcAllZeroes;
  BOOLEAN           UseAltData;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  MrcCall     = Inputs->Call.Func;
  IsDdr5      = Outputs->IsDdr5;
  UseAltData  = FALSE;
  IsPatSrcAllZeroes = (PatCtlPtr->PatSource == MrcPatSrcAllZeroes);

  if (Inputs->NumCL != 0) {
    NumCL = Inputs->NumCL;
  }
  if (Inputs->LoopCount != 0) {
    LcExp = Inputs->LoopCount;
  }

   for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (!IsDdr5, Channel)) {
        continue;
      }
      IpChannel = LP_IP_CH (!IsDdr5, Channel);
      Cpgc20SetAddrMaxRank (MrcData, Controller, IpChannel, 0, &OldRankCount);
    }
  }

  BlockRepeats = MrcCalculateLoopCount (NumCL, LcExp);

  TestDataEngineSetLoopCount (MrcData, McChBitMask, BlockRepeats);
  //Set Number of CL transactions per algorithm instruction within 1 BlockRepeat
  Cpgc20BaseRepeats (MrcData, McChBitMask, MAX (NumCL, 1), 1); //@TODO supposed to be part of SetSequence

  // @todo: Need to check that PDWN registers is programmed already.
  //Removed functionality was intended to configure test during Power Down event but was unused

   //###########################################################
  // Program Data Pattern Controls.  PGs are selected for Data
  //###########################################################

  if (IsPatSrcAllZeroes) {
    // Enable/Disable DC/Invert on all lanes, including ECC
    Cpgc20SetPgInvDcEn (MrcData, ENABLE_DQ_LANE_MASK, ENABLE_ECC_LANE_MASK);
    Cpgc20SetPgInvDcCfg (MrcData, Cpgc20DcMode, 0, FALSE, 0, BASIC_STATIC_PATTERN);
  } else {
    // Setup the Pattern Generator for the test.
    //   StaticPattern: The caller programs the pattern
    if (PatCtlPtr->DQPat == StaticPattern) {
      // Use single byte as a fixed pattern using DC mode
      UseAltData = TRUE;
      MrcCall->MrcSetMem ((UINT8 *) &DqLaneMask, sizeof (DqLaneMask), PatCtlPtr->StaticPattern);
      Cpgc20SetPgInvDcEn (MrcData, DqLaneMask, PatCtlPtr->StaticPattern);
      Cpgc20SetPgInvDcCfg (MrcData, Cpgc20DcMode, 1, FALSE, 0, BASIC_STATIC_PATTERN);
    } else {
      Cpgc20SetPgInvDcEn (MrcData, DISABLE_DQ_LANE_MASK, DISABLE_ECC_LANE_MASK);
      Cpgc20SetPgInvDcCfg (MrcData, Cpgc20InvertMode, 0, FALSE, 0, BASIC_STATIC_PATTERN);
    }

    Cpgc20LfsrCfg (MrcData, 0);
  }

  //Setup test command sequence
  Cpgc20SetCommandSequence (MrcData, CmdPat, FALSE, SubSeqWait, UseAltData);

  //###########################################################
  // Program Cpgc Address
  //###########################################################

  Cpgc20AddressSetup (
    MrcData,
    AddressArray,
    FALSE,
    CapNotPowerOf2
    );
  //###########################################################
  // Program Error Checking
  //###########################################################

  // Enable selective_error_enable_chunk and selective_error_enable_cacheline, mask later
  // the bits we don't want to check.
  // Burst Length: DDR5: 16, LP4/LP5: 32
  ChunkMask = (IsDdr5) ? 0x0000FFFF: 0xFFFFFFFF;
  Cpgc20SetupTestErrCtl  (MrcData, SOE, 1);
  MrcSetChunkAndClErrMsk (MrcData, McChBitMask, 0xFF, ChunkMask);

  // Setup Data and ECC Errors
  MrcSetDataAndEccErrMsk (MrcData, McChBitMask, 0xFFFFFFFFFFFFFFFFULL, 0xFF);
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    MrcSetupErrCounterCtl (MrcData, Byte, ErrCounterCtlPerByte, 0); //Counts errors per Byte Group, 1 count per UI with an error across all lanes in the Byte Group, all UI
  }

  MrcIssueZQ (MrcData);

}

/**
  This function sets up a basic static WR/RD test for the given channel mask.

  @param[in,out] MrcData       - Pointer to MRC global data.
  @param[in]     McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in]     LC            - Exponential number of loops to run the test.
  @param[in]     SOE           - Error handling switch for test.
  @param[in]     EnCADB        - Switch to enable CADB
  @param[in]     EnCKE         - Switch to enable CKE.
  @param[in]     CmdPat        - Command Pattern
  @param[in]     Wait          - Wait time between subsequences
  @param[in]     NumCLOverride - Override for NumCL (Non-zero causes override)
  @param[in]     DataPtn       - Data Pattern
**/
void
SetupIOTestStatic (
  IN OUT MrcParameters* const MrcData,
  IN     const UINT8          McChBitMask,
  IN     const UINT8          LC,
  IN     const UINT8          SOE,
  IN     const UINT8          EnCADB,
  IN     const UINT8          EnCKE,
  IN     const UINT8          CmdPat,
  IN     const UINT8          Wait,
  IN     const UINT16         NumCLOverride,
  IN     const UINT8          DataPtn
  )
{
  MRC_PATTERN_CTL       PatCtl;
  UINT16                NumCL;
  const MRC_ADDRESS     *Address;
  UINT8                 AdjustedLoopCount;
  MrcOutput             *Outputs;
  MRC_ADDRESS           AddressArr[MAX_CONTROLLER][MAX_CHANNEL];

  static const MRC_ADDRESS AddressDdr5 = {
    BANK_2_ROW_COL_2_RANK,
    FAST_Y,
    0x0,    // Single Address Instruction
    0,
    0,
    0,
    6,      // ColSizeBits - 2^6 = 64
    8       // since the BG is sawapped before Banks, 8 banks is enough for B2B
  };

  static const MRC_ADDRESS AddressLpddr = {
    BANK_2_ROW_COL_2_RANK,
    FAST_Y,
    0x0,    // Single Address Instruction
    0,
    0,
    0,
    5,      // ColSizeBits - 2^5 = 32
    1       // BankSize    - LP4/5 doesn't need bank groups toggle for B2B   @todo_rpl LP5 BG mode might need this
  };

  Outputs = &MrcData->Outputs;
  Address = Outputs->IsDdr5 ? &AddressDdr5 : &AddressLpddr;

  PatCtl.DQPat = StaticPattern;
  PatCtl.PatSource = MrcPatSrcStatic;
  PatCtl.StaticPattern = DataPtn;
  // fill the local 2D array with the correct data depends on lpddr/ddr device
  FillAddressArray (MrcData, AddressArr, (MRC_ADDRESS* const)Address);

  // Column limit is 2^10
  // BL=16: column increment is 2^4 per CL --> 10 - 4 = 6, 2^6 = 64
  // DDR5 toggles 8 Banks so we should multiply NumCL by 8
  // BL=32: column increment is 2^5 per CL --> 10 - 5 = 5, 2^5 = 32
  NumCL = NumCLOverride ? NumCLOverride : (Outputs->IsDdr5 ? 512 : 32);

  AdjustedLoopCount = LC;
  if (Outputs->IsDdr5) {
    // Divide the Loopcount by 2 for Data stress, because DDR5 has twice longer burst length comparing to DDR4.
    if (AdjustedLoopCount >= 1) {
      AdjustedLoopCount -= 1;
    }
  }
  if (Outputs->IsLpddr) {
    // Divide the Loopcount by 4 for Data stress, because LP4/LP5 has 4 times longer burst length comparing to DDR4.
    if (AdjustedLoopCount >= 2) {
      AdjustedLoopCount -= 2;
    } else {
      AdjustedLoopCount = 0;
    }
  }

  SetupIOTestCpgc (MrcData, McChBitMask, PatWrRd, NumCL, AdjustedLoopCount, AddressArr, NSOE, &PatCtl, 0, NULL);
  MrcData->Outputs.DQPat = PatCtl.DQPat;
}

/**
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  This is used for ECC scrubbing or memory clear.

  @param[in,out] MrcData          - Pointer to MRC global data.
  @param[in]     CPGCAddressArray - 2D Array of Structure that stores address Row/Col/Bank sizes that may be different per DIMM.
  @param[in]     McChbitMask      - Bit masks of MC channels to enable for the test.
  @param[in]     CmdPat           - read/write.
  @param[in]     CapNotPowerOf2   - Whether non-power of 2 capacity found per MC/CH.
**/
VOID
SetupIOTestScrubCpgc (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN     const UINT8          McChBitMask,
  IN     const UINT8          CmdPat,
  IN     BOOLEAN              CapNotPowerOf2[MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  )
{
  MRC_PATTERN_CTL       PatCtl;
  UINT8                 NumCL;
  UINT8                 LC;

  NumCL = 0;
  LC = 0;
  PatCtl.IncRate = 0;
  PatCtl.Start = 0;
  PatCtl.Stop = 0;
  PatCtl.DQPat = StaticPattern;
  PatCtl.PatSource = MrcPatSrcAllZeroes;  // DC zero
  PatCtl.EnableXor = FALSE;

  SetupIOTestCpgc (MrcData, McChBitMask, CmdPat, NumCL, LC, CPGCAddressArray, NSOE, &PatCtl, 0, CapNotPowerOf2);

  MrcData->Outputs.DQPat = PatCtl.DQPat;
}

/**
  Runs one or more REUT tests (based on TestType).

  @param[in] MrcData      - Include all MRC global data.
  @param[in] McChBitMask  - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CaParityRank - Optional parameter only used when DQPat is CaParity and passed to CA Parity IO runner. //needed only for Green side
  @param[in] DQPat        - The Pattern type for the test.  See MrcDqPat.
  @param[in] ClearErrors  - Decision to clear or not errors.
  @param[in] IsCmdVC      - Determine if command voltage training is being executed.

**/
UINT8
RunIOTest (
  IN MrcParameters *const MrcData,
  IN const UINT8          McChBitMask,
  IN const UINT8          CaParityRank, OPTIONAL
  IN const UINT8          DQPat,
  IN const UINT8          ClearErrors,
  IN BOOLEAN              IsCmdVC
  )
{
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  MrcStatus           Status;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  Status        = mrcSuccess;

  //###########################################################
  // Start Test and Poll on completion
  //###########################################################

  // IO Reset needed before starting test.
  if (!Outputs->IsSkipIoReset) {
    IoReset(MrcData);
  }

  if (ClearErrors) {
    Cpgc20ClearErrors (MrcData, McChBitMask);
  }

  TestDataEngineTestStart (MrcData, McChBitMask);

  // Wait till CPGC test is done on all participating channels
  Status = TestDataEngineTestDone (MrcData, McChBitMask);
  if (Status != mrcSuccess) {
    MRC_DEBUG_ASSERT(FALSE, Debug, "RunIO test timeout\n");
  }

  // For x64 Channels, we can break out as soon as either SubChannel has an error for the channels populated.
  // Same as Error Status mask.
  // Current assumption is SubChannels are run sequentially.  Traffic is only sent on tested sub channel.  If a failure occurs, report it as an error for that Channel.
  // If a Sch is not populated, its Error status is Don't Care.
  // Not Valid (NV)
  // Sc1,Sc0   | 0,0 | 0,1 | 1,1 | 1,0 |
  // Sc1E,Sc0E |-----------------------|
  //    0,0    | NV  |  0  |  0  |  0  |
  //    0,1    | NV  |  1  |  1  |  0  |
  //    1,1    | NV  |  1  |  1  |  1  |
  //    1,0    | NV  |  0  |  1  |  1  |
  //           |-----------------------|
  return McChBitMask;
}

/**
  Calls RunIoTest with Rank fixed as MRC_IGNORE_ARG_8.

  @param[in] MrcData      - Include all MRC global data.
  @param[in] McChBitMask  - Memory Controller Bit mask for which test should be setup for.
  @param[in] DQPat        - The Pattern type for the test.  See MrcDqPat.
  @param[in] ClearErrors  - Decision to clear or not errors.

  @retval Returns ch errors
**/
UINT8
RunIoTestNoRank (
  IN MrcParameters *const MrcData,
  IN const UINT8          McChBitMask,
  IN const UINT8          DQPat,
  IN const UINT8          ClearErrors
  )
{
  return (RunIOTest (MrcData, McChBitMask, MRC_IGNORE_ARG_8, DQPat, ClearErrors, FALSE));
}
