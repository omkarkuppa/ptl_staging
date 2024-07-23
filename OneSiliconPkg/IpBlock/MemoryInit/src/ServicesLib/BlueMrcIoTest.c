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

/// Galois MPR Bit Mask for LFSR Assignment: 0 - LFSR0, 1 - LFSR1
/// Vicim bit is associated with LFSR0
/// Aggressor bit is associated with LFSR1
UINT8 GaloisMprBitMask[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

/// Galois Starting Seeds
static const UINT32 DataGaloisSeeds[MRC_NUM_MUX_SEEDS] = {0x5A, 0x3C, 0xF0};

/**
  This routine programs the EXT_BUF for Victim-aggressor traffic on DQ lanes using a VA pattern
  This routine performs the following steps:
  0: Fill a buffer with the VA pattern
  1: Program the external buffer values into the EXT_BUF of the CPGC pattern generator
  @param[in] MrcData     - Include all MRC global data.
  @param[in] McChBitMask - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] vmask       - 32 bit victim mask.  1 indicates this bit should use LFSR0. Mask length is 16 bits (CPGC_20_NUM_DPAT_EXTBUF)
  @param[in] amask       - 32 bit aggressor mask. 0/1 indicates this bit should use LFSR1/2. Mask length is 16 bits (CPGC_20_NUM_DPAT_EXTBUF)

  @retval Nothing
**/
VOID
MrcProgramVAPattern (
  IN MrcParameters *const MrcData,
  IN UINT8                McChBitMask,
  IN UINT32               vmask,
  IN UINT32               amask
  )
{
  UINT32  Pattern[CPGC_20_NUM_DPAT_EXTBUF];

  Cpgc20FillVAPattern (vmask, amask, Pattern);
  Cpgc20SetDqAssignemnt (MrcData, McChBitMask, Pattern);
}

/**
  Programs all the key registers to define a CPGC test as per input mask and Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program

  @param[in] MrcData       - Include all MRC global data.
  @param[in] McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CmdPat        - [0: PatWrRd (Standard Write/Read Loopback),
                              1: PatWr (Write Only),
                              2: PatRd (Read Only),
                              3: PatRdWrTA (ReadWrite Turnarounds),
                              4: PatWrRdTA (WriteRead Turnarounds)
  @param[in] NumCL         - Number of Cache lines transactions per algorithm instruction within 1 BlockRepeat.
                             Non-zero Inputs->NumCL will override this.
  @param[in] LcExp         - Loop Count exponent. Non-zero Inputs->LoopCount will override this.
  @param[in] AddressArray  - 2D Array of Structure that stores address related settings
  @param[in] SOE           - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
  @param[in] PatCtlPtr     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in] SubSeqWait    - # of Dclks to stall at the end of a sub-sequence
**/
VOID
SetupIOTest (
  IN MrcParameters *const         MrcData,
  IN const UINT8                  McChBitMask,
  IN const UINT8                  CmdPat,
  IN UINT32                       NumCL,
  IN UINT8                        LcExp,
  IN MRC_ADDRESS                  AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN const MRC_TEST_STOP_TYPE     SOE,
  IN MRC_PATTERN_CTL *const       PatCtlPtr,
  IN UINT16                       SubSeqWait
  )
{
  static const UINT32 DataLfsrSeeds[MRC_NUM_MUX_SEEDS]  = {0xA10CA1, 0xEF0D08, 0xAD0A1E};
  static const UINT32 DataStaticSeeds[MRC_NUM_MUX_SEEDS] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0};
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  UINT32            ChunkMask;
  UINT8             Byte;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             IpChannel;
  UINT8             Index;
  UINT8             LaneRotateRate;
  UINT8             OldRankCount;
  UINT32            BlockRepeats;
  UINT32            *LfsrSeed;
  BOOLEAN           IsDdr5;
  BOOLEAN           IsPatSrcAllZeroes;
  BOOLEAN           IsDqPatternRequired;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  IsDdr5      = Outputs->IsDdr5;
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

  TestDataEngineSetLoopCount (MrcData, McChBitMask,BlockRepeats);
  //Set Number of CL transactions per algorithm instruction within 1 BlockRepeat
  Cpgc20BaseRepeats (MrcData, McChBitMask, MAX (NumCL, 1), 1); //@TODO supposed to be part of SetSequence

  // @todo: Need to check that PDWN registers is programmed already.
  //Removed functionality was intended to configure test during Power Down event but was unused

   //###########################################################
  // Program Data Pattern Controls.  PGs are selected for Data
  //###########################################################
  if (!IsPatSrcAllZeroes) {
    // Setup the Pattern Generator for the test.
    //   StaticPattern: The caller programs the pattern
    //   GaloisMprVa: RunIOTest programs the pattern
    IsDqPatternRequired = (PatCtlPtr->DQPat != StaticPattern && PatCtlPtr->DQPat != GaloisMprVa);
    if (IsDqPatternRequired) {
      MrcProgramVAPattern (MrcData, McChBitMask, BASIC_VA_VICTIM_16, BASIC_VA_AGGRESSOR_16);
    }

    // Write the LFSR seeds
    if (PatCtlPtr->DQPat == StaticPattern) {
      LfsrSeed = (UINT32*) DataStaticSeeds;
      LaneRotateRate = 0;
    } else if (PatCtlPtr->DQPat == GaloisMprVa) {
      LfsrSeed = (UINT32*) DataGaloisSeeds;
      LaneRotateRate = 0;
    } else {
      LfsrSeed = (UINT32*) DataLfsrSeeds;
      LaneRotateRate = 2;
    }
    Cpgc20LfsrSeed (MrcData, (const UINT32*) LfsrSeed);

    // Setup CPGC in terms of LFSR selects, LFSR seeds, LMN constants and overall control
    for (Index = 0; Index < MRC_NUM_MUX_SEEDS; Index++) {
      Cpgc20DPatUsqCfg (MrcData, PatCtlPtr, Index);
    }
    //@todo should LSFR Poly match previous code or documentation?
    Cpgc20LfsrCfg (MrcData, LaneRotateRate);

    // Program Write Data Buffer Related Entries.
    // Assumes the PG has already been selected above for Data PG's.
    // Program the data rotation -- IncRate if Dynamic, 0 if Static.
    Cpgc20ConfigPgRotation (MrcData, ((PatCtlPtr->PatSource == MrcPatSrcDynamic) ? PatCtlPtr->IncRate : 0));  // No data rotation
  }

  // Enable/Disable DC/Invert on all lanes, including ECC
  Cpgc20SetPgInvDcEn (MrcData, IsPatSrcAllZeroes ? ENABLE_DQ_LANE_MASK : DISABLE_DQ_LANE_MASK, IsPatSrcAllZeroes ? ENABLE_ECC_LANE_MASK : DISABLE_ECC_LANE_MASK);
  Cpgc20SetPgInvDcCfg (MrcData, IsPatSrcAllZeroes ? Cpgc20DcMode : Cpgc20InvertMode, 0, FALSE, 0, BASIC_STATIC_PATTERN);

  //Setup test command sequence
  Cpgc20SetCommandSequence(MrcData, CmdPat, FALSE, SubSeqWait);

  //###########################################################
  // Program Cpgc Address
  //###########################################################

  Cpgc20AddressSetup (
    MrcData,
    AddressArray,
    FALSE
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
  This function sets up a basic victim-aggressor test for the given channel mask and per Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program

  @param[in,out] MrcData       - Pointer to MRC global data.
  @param[in]     McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in]     LC            - Exponential number of loops to run the test.
  @param[in]     SOE           - Error handling switch for test.
  @param[in]     Spread        - Stopping point of the pattern.
  @param[in]     CmdPat        - Command Pattern
  @param[in]     Wait          - Wait time between subsequences
  @param[in]     NumCLOverride - Override for NumCL (Non-zero causes override)

**/
void
SetupIOTestBasicVA (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          McChBitMask,
  IN     const UINT8          LC,
  IN     const UINT8          SOE,
  IN     const UINT8          Spread,
  IN     const UINT8          CmdPat,
  IN     const UINT8          Wait,
  IN     const UINT16         NumCLOverride
  )
{
  MRC_PATTERN_CTL        PatCtl;
  UINT16                NumCL;
  const MRC_ADDRESS     *Address;
  UINT8                 AdjustedLoopCount;
  MrcOutput             *Outputs;
  MRC_ADDRESS           AddressArr[MAX_CONTROLLER][MAX_CHANNEL];
  static const MRC_ADDRESS Address_Ddr5 = {
    BANK_2_ROW_COL_2_RANK,
    FAST_Y,
    0x0,    // Single Address Instruction
    0,
    0,
    0,
    6,      // ColSizeBits - 2^6 = 64
    2       // BankSize    - 2 bank groups toggle for B2B traffic
  };

  static const MRC_ADDRESS Address_lpddr = {
    BANK_2_ROW_COL_2_RANK,
    FAST_Y,
    0x0,    // Single Address Instruction
    0,
    0,
    0,
    5,      // ColSizeBits - 2^5 = 32
    1       // BankSize    - LP4/5 doesn't need bank groups toggle for B2B   @todo_rpl LP5 BG mode might need this
  };

  AdjustedLoopCount = LC;
  PatCtl.Start      = 0;
  PatCtl.Stop       = Spread - 1;
  PatCtl.DQPat      = BasicVA;
  PatCtl.PatSource  = MrcPatSrcDynamic;
  PatCtl.EnableXor  = FALSE;
  Outputs = &MrcData->Outputs;
  Address = Outputs->IsDdr5 ? &Address_Ddr5 : &Address_lpddr;

  /* fill the local 2D array with the correct data depends on lpddr/ddr device */
  FillAddressArray (MrcData, AddressArr, (MRC_ADDRESS *const)Address);

  // Column limit is 2^10
  // BL=16: column increment is 2^4 per CL --> 10 - 4 = 6, 2^6 = 64
  //  DDR5 toggles 2 Bank Groups so we should multiply NumCL by 2
  // BL=32: column increment is 2^5 per CL --> 10 - 5 = 5, 2^5 = 32
  NumCL = NumCLOverride ? NumCLOverride : (Outputs->IsDdr5 ? 64 : 32);
  PatCtl.IncRate = 4;

  // LPDDR burst length is 32, DDR5 is 16
  if (Outputs->IsLpddr) {
    if (AdjustedLoopCount > 2) {
      AdjustedLoopCount -= 2;
    } else {
      AdjustedLoopCount = 1;
    }
  } else if (Outputs->IsDdr5) {
    if (AdjustedLoopCount > 1) {
      AdjustedLoopCount -= 1;
    } else {
      AdjustedLoopCount = 1;
    }
  }

  SetupIOTest (MrcData, McChBitMask, CmdPat, NumCL, AdjustedLoopCount, AddressArr, SOE, &PatCtl, Wait);

  Outputs->DQPatLC = (AdjustedLoopCount - 8 + 1);
  if (Outputs->DQPatLC < 1) {
    Outputs->DQPatLC = 1;
  }

  Outputs->DQPat = BasicVA;
}

/**
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  This is used for ECC scrubbing or memory clear.

  @param[in,out] MrcData          - Pointer to MRC global data.
  @param[in]     CPGCAddressArray - 2D Array of Structure that stores address Row/Col/Bank sizes that may be different per DIMM.
  @param[in]     McChbitMask      - Bit masks of MC channels to enable for the test.
  @param[in]     CmdPat           - read/write.
**/
VOID
SetupIOTestScrub (
  IN OUT MrcParameters *const MrcData,
  IN     MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN     const UINT8          McChBitMask,
  IN     const UINT8          CmdPat
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

  SetupIOTest (MrcData, McChBitMask, CmdPat, NumCL, LC, CPGCAddressArray, NSOE, &PatCtl, 0);

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
  const MrcInput      *Inputs;
  const MrcChannelIn  *ChannelIn;
  MrcChannelOut       *ChannelOut;
  const MRC_FUNCTION  *MrcCall;
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  MrcStatus           Status;
  MrcDimmOut          *DimmOut;
  INT64               GetSetVal;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              TestIdx;
  UINT8               CurMcChBitMask;
  UINT8               NumTests;
  UINT8               tRDRD_dr_Min[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8               tRDRD_sg_Min[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8               tRDRD_dg_Min[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8               TurnAroundOffset;
  UINT8               MaxChannel;
  BOOLEAN             IsLpddr;
  UINT32              vmask;
  UINT32              amask;
  UINT8               Rank;
  UINT8               RankMask;  // RankBitMask for both channels
  UINT8               NumDevices;
  UINT8               DimmIdx;
  UINT8               Device;
  UINT8               Byte;

  Inputs        = &MrcData->Inputs;
  MrcCall       = Inputs->Call.Func;
  Outputs       = &MrcData->Outputs;
  IsLpddr       = Outputs->IsLpddr;
  MaxChannel    = Outputs->MaxChannels;
  RankMask      = Outputs->ValidRankMask;
  Debug         = &Outputs->Debug;
  MrcCall->MrcSetMem ((UINT8 *) tRDRD_dr_Min, sizeof (tRDRD_dr_Min), 0);
  MrcCall->MrcSetMem ((UINT8 *) tRDRD_sg_Min, sizeof (tRDRD_sg_Min), 0);
  MrcCall->MrcSetMem ((UINT8 *) tRDRD_dg_Min, sizeof (tRDRD_dg_Min), 0);

  NumTests               = 1;
  Status                 = mrcSuccess;

  if ((DQPat == GaloisMprVa) && Inputs->ReadMprVA) {
    NumTests = 8;
  } else if (DQPat == RdRdTA) {
    NumTests = 2;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        CurMcChBitMask = (1 << ((Controller * MaxChannel) + Channel));
        if ((CurMcChBitMask & McChBitMask) == 0) {
          continue;
        }
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, ReadFromCache, &GetSetVal);
        tRDRD_sg_Min[Controller][Channel] = (UINT8) GetSetVal; // save the min value allowed
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, ReadFromCache, &GetSetVal);
        tRDRD_dg_Min[Controller][Channel] = (UINT8) GetSetVal; // save the min value allowed
      }
    }
  } else if (DQPat == RdRdTA_All) {
    NumTests = 8;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        CurMcChBitMask = (1 << ((Controller * MaxChannel) + Channel));
        if ((CurMcChBitMask & McChBitMask) == 0) {
          continue;
        }
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, ReadFromCache, &GetSetVal);
        tRDRD_sg_Min[Controller][Channel] = (UINT8) GetSetVal; // save the min value allowed
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, ReadFromCache, &GetSetVal);
        tRDRD_dg_Min[Controller][Channel] = (UINT8) GetSetVal; // save the min value allowed
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdr, ReadFromCache, &GetSetVal);
        tRDRD_dr_Min[Controller][Channel] = (UINT8) GetSetVal; // save the min value allowed
      }
    }
  }

  // Intial values for VA masks
  if (!Inputs->ReadMprVA) {
    vmask = BASIC_NON_MPR_VA_VICTIM_16;
    amask = BASIC_NON_MPR_VA_AGGRESSOR_16;
  } else {
    vmask = BASIC_VA_VICTIM_16;
    amask = BASIC_VA_AGGRESSOR_16;
  }

  for (TestIdx = 0; TestIdx < NumTests; TestIdx++) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        ChannelIn = &Inputs->Controller[Controller].Channel[Channel];
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        CurMcChBitMask = (1 << ((Controller * MaxChannel) + Channel));
        if (!(CurMcChBitMask & McChBitMask) ||
          (IS_MC_SUB_CH (IsLpddr, Channel) && ((DQPat == RdRdTA) || (DQPat == RdRdTA_All)))) {
          continue;
        }

        if (DQPat == GaloisMprVa) {
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (!((MRC_BIT0 << Rank) & RankMask)) {
              continue; // Skip if both channels empty
            }
            if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
              continue;
            }
            DimmIdx = RANK_TO_DIMM_NUMBER (Rank);
            DimmOut = &ChannelOut->Dimm[DimmIdx];
            // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
            NumDevices = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
            if (DimmOut->EccSupport) {
              NumDevices += 1;
            }
            //Reprogram DRAM
            MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR26, DataGaloisSeeds[0], TRUE); // LFSR0 Seed
            MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR27, DataGaloisSeeds[1], TRUE); // LFSR1 Seed
            if (Inputs->ReadMprVA) {
              // Set up Victim/Aggressor for each device
              for (Device = 0; Device < NumDevices; Device++) {
                MrcPdaSelect (MrcData, Controller, Channel, Rank, Device, MRC_PRINTS_OFF);
                if (DimmOut->SdramWidth == 8) {
                  MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR30, GaloisMprBitMask[ChannelIn->DqMapCpu2Dram[Rank][Device][TestIdx] % 8], FALSE); // LFSR Assignment(0-LFSR0, 1-LFSR1)
                } else { // x16 case
                  // Program MR30 for both bytes in Device
                  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                    if (!MrcByteExist (MrcData, Controller, Channel, Byte)) {
                      continue;
                    }
                    // Account for both the lower and upper byte in the same device
                    if ((((Device * 2) + 1) == ChannelIn->DqsMapCpu2Dram[DimmIdx][Byte]) ||
                      ((Device * 2) == ChannelIn->DqsMapCpu2Dram[DimmIdx][Byte])) {
                      MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR30, GaloisMprBitMask[ChannelIn->DqMapCpu2Dram[Rank][Byte][TestIdx] % 8], FALSE); // LFSR Assignment(0-LFSR0, 1-LFSR1)
                      break;
                    }
                  }
                }
              }
              // Send PDA command of Index 15 to resume normal rank operation mode for MR/CAVREF/MPC
              MrcPdaSelect (MrcData, Controller, Channel, Rank, 15, MRC_PRINTS_OFF);
            } else {
              MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR30, 0xFF, TRUE); // LFSR Assignnent (0-LFSR0, 1-LFSR1)
            } // ReadMprVA
          }
        }

        if (DQPat == RdRdTA) {
          // Program tRDRD parameter
          GetSetVal = tRDRD_sg_Min[Controller][Channel] + TestIdx;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, WriteCached, &GetSetVal);
          GetSetVal = tRDRD_dg_Min[Controller][Channel] + TestIdx;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, WriteCached, &GetSetVal);
        } else if (DQPat == RdRdTA_All) {
          // Program tRDRD for SR and DR
          //  Run 8 tests, Covering tRDRD_sr = 4,5,6,7 and tRDRD_dr = Min,+1,+2,+3
          TurnAroundOffset = (TestIdx % 4);
          GetSetVal = tRDRD_sg_Min[Controller][Channel] + TurnAroundOffset;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, WriteCached, &GetSetVal);
          GetSetVal = tRDRD_dg_Min[Controller][Channel] + TurnAroundOffset;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, WriteCached, &GetSetVal);
          GetSetVal = tRDRD_dr_Min[Controller][Channel] + TurnAroundOffset;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdr, WriteCached, &GetSetVal);
        }
      } // for Channel
    }  // for Controller

    if (DQPat == GaloisMprVa) {
      //Reprogram CPGC - Reset starting Seed
      Cpgc20LfsrSeed (MrcData, DataGaloisSeeds);
      MrcProgramVAPattern (MrcData, McChBitMask, vmask, amask);
    }

    //###########################################################
    // Start Test and Poll on completion
    //###########################################################

    // IO Reset needed before starting test.
    if (!Outputs->IsSkipIoReset) {
      IoReset(MrcData);
    }

    if (ClearErrors && (TestIdx == 0)) {
      Cpgc20ClearErrors (MrcData, McChBitMask);
    }

    // We do not clear error everytime before start test because we want to accumulate errors through various point tests.
    // TestIdx indicates how many point tests are supposed to be run for each parameter.
    // We need to run multiple point tests because of following reasons:
    //   MRC needs to manually rotate victim lanes for CADB Deselects and ReadMPR DDR5.
    //   To adjust Turnaround values for TAT stress tests.
    TestDataEngineTestStart (MrcData, McChBitMask);

    // Wait till CPGC test is done on all participating channels
    Status = TestDataEngineTestDone (MrcData, McChBitMask);
    if (Status != mrcSuccess) {
      MRC_DEBUG_ASSERT(FALSE, Debug, "RunIO test exceeds timeout value\n");
    }

    if ((DQPat == GaloisMprVa) && (Inputs->ReadMprVA)) {
      // Change the VA variables for next test iteration
      vmask = vmask << 1;
      amask = ~vmask;
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
    //} // NumDqRot
  } // NumTests

  if ((DQPat == RdRdTA) || (DQPat == RdRdTA_All)) {
    // Restore original tRDRD value
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        CurMcChBitMask = (1 << ((Controller * MaxChannel) + Channel));
        if (!(CurMcChBitMask & McChBitMask) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        GetSetVal = tRDRD_sg_Min[Controller][Channel];
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, WriteCached, &GetSetVal);
        GetSetVal = tRDRD_dg_Min[Controller][Channel];
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, WriteCached, &GetSetVal);
        if (DQPat == RdRdTA_All) {
          GetSetVal = tRDRD_dr_Min[Controller][Channel];
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdr, WriteCached, &GetSetVal);
        }
      }
    }
  }
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
