/** @file
  This file contains functions to find and track DRAM Failing addresses for the
  Post Package Repair feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "MrcPpr.h"
#include "MrcAmt.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "MrcGeneral.h"

UINT64 AmtCacheLine1[8] = {
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA
};

UINT64 AmtCacheLine2[8] = {
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

UINT64 AmtXMarchPattern[16] = {
  0x0000000000000000, 0x0000000000000000,
  0xFF00FF00FF00FF00, 0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000, 0xFFFF0000FFFF0000,
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFF00FF00FF00FF00, 0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000, 0xFFFF0000FFFF0000,
  0x0000000000000000, 0x0000000000000000
};

UINT64 AmtXMarchGPattern[8] = {
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00
};

#if 0
/**
This executes the necessary ACT and DQ override needed for PPR DDR5 DRAM sequence, based on provided address

@param[in] MrcData     - Pointer to MRC global data.
@param[in] Controller  - Target Controller
@param[in] Channel     - Target Channel
@param[in] Rank        - Target Rank
@param[in] Row         - Target Row for repair
@param[in] BankGroup   - Target BankGroup for repair
@param[in] BankAddress - Target BankAddress for repair
@param[in] ByteMask    - Mask of Bytes for repair

@retval MrcStatus
**/
MrcStatus
MrcPprActAndDqLow (
  IN MrcParameters* const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Rank,
  IN const UINT32         Row,
  IN const UINT32         BankGroup,
  IN const UINT32         BankAddress,
  IN const UINT16         ByteMask
)
{
  MrcOutput           *Outputs        = &MrcData->Outputs;
  //MrcDebug            *Debug          = &Outputs->Debug;
  MrcStatus           Status          = mrcSuccess;
  UINT8               McChBitMask     = 0;
  UINT8               Byte;
  INT64               GetSetVal;
  INT64               GetSetDis;
  INT64               GetSetEn;
  INT64               GetSetValSave;
  INT64               tCWL4TxDqFifoRdEn;
  INT64               tCWL4TxDqFifoRdEnSave;
  UINT32              Offset;
  UINT32              GaloisPoly;
  UINT32              IpChannel;
  UINT32              PatternBuffer[3];
  BOOLEAN             Ddr5;
  UINT8               Cke[2];
  UINT8               Cs[2];
  UINT8               RankMask;
  MRC_CA_MAP_TYPE     CmdAddr;
  UINT64_STRUCT       CadbPatternChunks[CADB_20_MAX_CHUNKS];
  MRC_PATTERN_CTL     PatCtl;
  UINT8               CadbBufferIndex;
  UINT8               BaseRankMask;
  UINT8               Index;
  MC0_CH0_CR_SCHED_SECOND_CBIT_STRUCT            SchedSecondCBit;
  MC0_CH0_CR_SCHED_SECOND_CBIT_STRUCT            SchedSecondCBitSave;
  MC0_CH0_CR_SC_GS_CFG_STRUCT                    ScGsCfg;
  MC0_CH0_CR_SC_GS_CFG_STRUCT                    ScGsCfgSave;
  MC0_CH0_CR_CADB_CTL_STRUCT                     CadbCtl;
  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT                CpgcSeqCtl;
  MC0_SHARED_CR_CPGC2_V_CHICKEN_STRUCT           Cpgc2Chicken;
  MC0_SHARED_CR_CPGC2_V_CHICKEN_STRUCT           Cpgc2ChickenSave;
  MC0_REQ0_CR_CPGC_SEQ_CFG_B_STRUCT              CpgcSeqCfgB;
  MC0_REQ0_CR_CPGC_SEQ_CFG_B_STRUCT              CpgcSeqCfgBSave;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT       AlgoInstruct;
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_STRUCT  AlgoInstructControl;
  MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_STRUCT  AlgoWaitEventControl;
  MC0_CH0_CR_CADB_CFG_STRUCT                             CadbCfg;
  MC0_CH0_CR_CADB_AO_MRSCFG_STRUCT                       MrsConfig;
  static const MRC_PG_UNISEQ_TYPE                        UniseqMode[3] = {MrcPgMuxPatBuffer, MrcPgMuxPatBuffer, MrcPgMuxPatBuffer};
  static const MRC_PG_LFSR_TYPE                          CadbLfsrPoly = MrcLfsr8;

  GetSetEn  = 1;
  GetSetDis = 0;
  Ddr5 = Outputs->DdrType == MRC_DDR_TYPE_DDR5;
  McChBitMask = (1 << ((Controller * Outputs->MaxChannels) + Channel));
  BaseRankMask = (UINT8) ((1 << MAX_RANK_IN_CHANNEL) - 1);

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Repairing Controller %d, Channel%d, Rank%d: McChBitMask = 0x%X\n", Controller, Channel, Rank, McChBitMask);

  IpChannel = DDR5_IP_CH (Ddr5, Channel);
  MrcGetSetMc (MrcData, Controller, GsmMccOppSrefEnable, ReadNoCache, &GetSetValSave);  // Save state of Self Refresh
  MrcGetSetMc (MrcData, Controller, GsmMccOppSrefEnable, WriteNoCache, &GetSetDis);     // Disable Self Refresh
  MrcWait (MrcData, MRC_TIMER_1NS);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteNoCache, &GetSetEn);  // block XARB
  MrcWait (MrcData, MRC_TIMER_1NS);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteNoCache, &GetSetDis);  // Unblock XARB
  MrcWait (MrcData, MRC_TIMER_1NS);

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_SECOND_CBIT_REG, MC1_CH0_CR_SCHED_SECOND_CBIT_REG, Controller, MC0_CH1_CR_SCHED_SECOND_CBIT_REG, Channel);
  SchedSecondCBit.Data = MrcReadCR (MrcData, Offset);
  SchedSecondCBitSave.Data = SchedSecondCBit.Data;
  SchedSecondCBit.Bits.disable_starved_prio_on_new_req = 1;
  MrcWriteCR (MrcData, Offset, SchedSecondCBit.Data);

  MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, ReadNoCache, &tCWL4TxDqFifoRdEnSave);
  tCWL4TxDqFifoRdEn = tCWL4TxDqFifoRdEnSave + 4;  // Original W/A adds 2 cycles; 2N timing needs 2 more clks, adding 2 to ensure we meet tCWL
  MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, WriteToCache, &tCWL4TxDqFifoRdEn);
  MrcFlushRegisterCachedData (MrcData);

  // ------------- Config CPGC ----------------------
  SetupIOTestPPR (MrcData, Controller, Channel, Row, BankGroup, BankAddress, &ByteMask);

  GetSetVal = MRC_UINT8_MAX; // Override all DQs in bytes not included in ByteMask
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (((1 << Byte) & ByteMask) == 0) { // If a byte is not present in ByteMask, override DQs to "0xFF"
      MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDqOverrideData, WriteToCache, &GetSetVal);
    } else {
      MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDqOverrideData, WriteToCache, &GetSetDis);
    }
    MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDqOverrideEn, WriteToCache, &GetSetVal);
  }
  MrcFlushRegisterCachedData (MrcData);
  //Cpgc20SetGlobalBind (MrcData, Controller, Channel, FALSE); // Disable Global Start/Stop

  Offset = OFFSET_CALC_CH (MC0_SHARED_CR_CPGC2_V_CHICKEN_REG, MC1_SHARED_CR_CPGC2_V_CHICKEN_REG, Controller);
  Cpgc2ChickenSave.Data = MrcReadCR (MrcData, Offset);
  Cpgc2Chicken.Data = Cpgc2ChickenSave.Data;
  Cpgc2Chicken.Bits.XPQ_CH_CREDIT = 16;
  MrcWriteCR (MrcData, Offset, Cpgc2Chicken.Data);

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_GS_CFG_REG, MC1_CH0_CR_SC_GS_CFG_REG, Controller, MC0_CH1_CR_SC_GS_CFG_REG, Channel);
  ScGsCfgSave.Data = MrcReadCR64 (MrcData, Offset);
  ScGsCfg.Data = ScGsCfgSave.Data;
  ScGsCfg.Bits.disable_tristate = 0;
  ScGsCfg.Bits.disable_ca_tristate = 0;
  ScGsCfg.Bits.disable_ck_tristate = 0;
  ScGsCfg.Bits.enable_odt_matrix = 0;
  MrcWriteCR64 (MrcData, Offset, ScGsCfg.Data);

  Offset = OFFSET_CALC_MC_CH (MC0_REQ0_CR_CPGC_SEQ_CFG_B_REG, MC1_REQ0_CR_CPGC_SEQ_CFG_B_REG, Controller, MC0_REQ1_CR_CPGC_SEQ_CFG_B_REG, Channel);
  CpgcSeqCfgBSave.Data = MrcReadCR (MrcData, Offset);
  CpgcSeqCfgB.Data = CpgcSeqCfgBSave.Data;
  CpgcSeqCfgB.Bits.START_DELAY = 100;
  MrcWriteCR (MrcData, Offset, CpgcSeqCfgB.Data);

  AlgoInstruct.Data = 0;
  AlgoInstruct.Bits.Last = 1;
  AlgoInstructControl.Data = 0;
  AlgoInstructControl.Bits.Select_On = 1;
  AlgoInstructControl.Bits.Deselect_On = 1;
  AlgoWaitEventControl.Data = 0;
  Cpgc20AlgoInstructWrite (MrcData, &AlgoInstruct, AlgoInstructControl, AlgoWaitEventControl, 0x1);

  Cadb20UniseqCfg (MrcData, UniseqMode, CadbLfsrPoly);
  PatCtl.DQPat = StaticPattern;
  for (Index = 0; Index < MRC_NUM_MUX_SEEDS; Index++) {
      Cpgc20DPatUsqCfg (MrcData, &PatCtl, Index);
  }

  CadbCfg.Data = 0;
  CadbCfg.Bits.CADB_MODE                = 3; // Normal Select Triggered Deselect
  CadbCfg.Bits.INITIAL_DSEL_SSEQ_EN     = 1;
  CadbCfg.Bits.INITIAL_DSEL_EN          = 0;
  CadbCfg.Bits.CADB_TO_CPGC_BIND        = 0; // CPGC disabled
  CadbCfg.Bits.CADB_SEL_THROTTLE_MODE   = 2; // Good to agree with cmd_stretch
  CadbCfg.Bits.CADB_DSEL_THROTTLE_MODE  = 1;
  CadbCfg.Bits.LANE_DESELECT_EN         = 0x7;
  CadbCfg.Bits.CMD_DESELECT_START       = 0x5;
  CadbCfg.Bits.CMD_DESELECT_STOP        = 0;
  Cadb20ConfigRegWrite (MrcData, CadbCfg);

  MrsConfig.Data = 0;
  MrsConfig.Bits.MRS_AO_REPEATS = 0x15;
  MrsConfig.Bits.MRS_GAP = 0x9;
  Cadb20MrsConfigRegWrite (MrcData, MrsConfig);

  GaloisPoly = 0x80000000;  // Could also be set to 0x200
  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_CADB_DSEL_UNISEQ_POLY_0_REG, MC1_CH0_CR_CADB_DSEL_UNISEQ_POLY_0_REG, Controller, MC0_CH1_CR_CADB_DSEL_UNISEQ_POLY_0_REG, IpChannel);
  MrcWriteCR (MrcData, Offset, GaloisPoly); // Galois Poly = 0x80000000
  Offset += MC0_CH0_CR_CADB_DSEL_UNISEQ_POLY_1_REG - MC0_CH0_CR_CADB_DSEL_UNISEQ_POLY_0_REG;
  MrcWriteCR (MrcData, Offset, GaloisPoly); // Galois Poly = 0x80000000
  Offset += MC0_CH0_CR_CADB_DSEL_UNISEQ_POLY_1_REG - MC0_CH0_CR_CADB_DSEL_UNISEQ_POLY_0_REG;
  MrcWriteCR (MrcData, Offset, GaloisPoly); // Galois Poly = 0x80000000

  PatternBuffer[0] = 0x252;
  PatternBuffer[1] = 0x39C;
  PatternBuffer[2] = 0x3E0;
  Cadb20LfsrSeed (MrcData, PatternBuffer);
  Cpgc20BaseRepeats (MrcData, McChBitMask, 0, 1);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCadbEnable, WriteToCache, &GetSetEn);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDeselectEnable, WriteToCache, &GetSetEn);
  MrcFlushRegisterCachedData (MrcData);

  // Build 2N mode ACT command
  CadbBufferIndex = 0;
  CmdAddr.Ddr5.Ca = 0;
  CmdAddr.Ddr5.Ca |= (Row & 0xF) << 2;
  CmdAddr.Ddr5.Ca |= (BankGroup & 0x7) << 8;
  CmdAddr.Ddr5.Ca |= (BankAddress & 0x3) << 6;

  RankMask = BaseRankMask;
  RankMask = (UINT8) MrcBitSwap (RankMask, 0, (UINT8) Rank, 1);

  Cs[0] = Cs[1] = (UINT8) BaseRankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 1st CMD cycle, CS High
  Cs[0] = Cs[1] = (UINT8) RankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 1st CMD cycle, CS Low

  CmdAddr.Data32 = 0;
  CmdAddr.Ddr5.Ca |= (Row & 0x3FFF0) >> 4;
  Cs[0] = Cs[1] = (UINT8) BaseRankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 2nd full CMD cycle

  CmdAddr.Data32 = 0x0;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // Deselect
  CadbPatternChunks[CadbBufferIndex - 1].Data32.Low = MrcBitSwap (CadbPatternChunks[3].Data32.Low, 0, 16, 1); // Unset VAL bit
  CadbPatternChunks[CadbBufferIndex - 1].Data32.High = 1;

  // Build 2N mode WRA command
  CmdAddr.Ddr5.Ca = 0xD; // WRA
  CmdAddr.Ddr5.Ca |= (BankGroup & 0x7) << 8;
  CmdAddr.Ddr5.Ca |= (BankAddress & 0x3) << 6;
  RankMask = BaseRankMask;
  RankMask = (UINT8) MrcBitSwap (RankMask, 0, (UINT8) Rank, 1);
  Cs[0] = Cs[1] = (UINT8) BaseRankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 1st CMD cycle, CS High
  Cs[0] = Cs[1] = (UINT8) RankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 1st CMD cycle, CS Low
  CmdAddr.Data32 = 0;
  if (MrcData->Inputs.ExtInputs.Ptr->PprRepairType == HARD_PPR) {
    CmdAddr.Ddr5.Ca = 0x800; // Not Partial, with Auto Precharge
  } else {
    CmdAddr.Ddr5.Ca = 0xC00; // Not Partial, without Auto Precharge
  }
  Cs[0] = Cs[1] = BaseRankMask;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // 2nd full CMD cycle
  CmdAddr.Data32 = 0x0;
  CpgcConvertDdrToCadb (MrcData, &CmdAddr, Cke, Cs, &CadbPatternChunks[CadbBufferIndex++]); // Deselect
  CadbPatternChunks[CadbBufferIndex - 1].Data32.Low = MrcBitSwap (CadbPatternChunks[7].Data32.Low, 0, 16, 1); // Unset VAL bit
  CadbPatternChunks[CadbBufferIndex - 1].Data32.High = 1;

  MrcSetCadbPgPattern (MrcData, McChBitMask, CadbPatternChunks, CadbBufferIndex, 0);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMaskCs,    WriteNoCache, &GetSetEn);
  MrcWait (MrcData, MRC_TIMER_1NS * 2);

  CadbCtl.Data = 0;
  CadbCtl.Bits.START_TEST = 1;
  Cadb20ControlRegWrite (MrcData, McChBitMask, CadbCtl);
  CpgcSeqCtl.Data = 0;
  CpgcSeqCtl.Bits.START_TEST = 1;
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);

  MrcWait (MrcData, MRC_TIMER_1MS * 2);

  CadbCtl.Bits.START_TEST = 0;
  CadbCtl.Bits.STOP_TEST = 1;
  Cadb20ControlRegWrite (MrcData, McChBitMask, CadbCtl);
  CpgcSeqCtl.Bits.START_TEST = 0;
  CpgcSeqCtl.Bits.STOP_TEST = 1;
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);

  // Cleanup
  //Cpgc20SetGlobalBind (MrcData, Controller, Channel, TRUE); // Global bind disabled; do not Re-Enable Global Start/Stop

  CadbCfg.Data = 0;
  Cadb20ConfigRegWrite (MrcData, CadbCfg);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCadbEnable, WriteToCache, &GetSetDis);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDeselectEnable, WriteToCache, &GetSetDis);
  MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn,   WriteToCache, &GetSetDis);
  MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteToCache, &GetSetDis);
  MrcFlushRegisterCachedData (MrcData);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMaskCs,    WriteNoCache, &GetSetDis);

  MrcGetSetMc (MrcData, Controller, GsmMccOppSrefEnable, WriteNoCache, &GetSetValSave);  // Restore state of Self Refresh

  MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, WriteToCache, &tCWL4TxDqFifoRdEnSave);
  MrcFlushRegisterCachedData(MrcData);

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_SECOND_CBIT_REG, MC1_CH0_CR_SCHED_SECOND_CBIT_REG, Controller, MC0_CH1_CR_SCHED_SECOND_CBIT_REG, Channel);
  MrcWriteCR (MrcData, Offset, SchedSecondCBitSave.Data);

  Offset = OFFSET_CALC_CH (MC0_SHARED_CR_CPGC2_V_CHICKEN_REG, MC1_SHARED_CR_CPGC2_V_CHICKEN_REG, Controller);
  MrcWriteCR (MrcData, Offset, Cpgc2ChickenSave.Data);

  Offset = OFFSET_CALC_MC_CH (MC0_REQ0_CR_CPGC_SEQ_CFG_B_REG, MC1_REQ0_CR_CPGC_SEQ_CFG_B_REG, Controller, MC0_REQ1_CR_CPGC_SEQ_CFG_B_REG, Channel);
  MrcWriteCR (MrcData, Offset, CpgcSeqCfgBSave.Data);

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_GS_CFG_REG, MC1_CH0_CR_SC_GS_CFG_REG, Controller, MC0_CH1_CR_SC_GS_CFG_REG, Channel);
  MrcWriteCR64 (MrcData, Offset, ScGsCfgSave.Data);

  return Status;
}

/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row

  @retval MrcStatus
**/
MrcStatus
Ddr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row,
  IN UINT16               ByteMask
)
{
  MrcOutput                     *Outputs;
  MrcInput                      *Inputs;
  MrcDebug                      *Debug;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  BOOLEAN                       Width16;
  UINT8                         DimmIdx;
  MrcStatus                     Status;
  UINT8                         Byte;
  UINT8                         MrrByte;
  UINT8                         PprResource;
  UINT32                        Offset = 0;
  UINT32                        Index;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  MRC_GEN_MRH_COMMAND           MrhCommand;    // Used for MRH generic commands
  MC0_CH0_CR_SCHED_CBIT_STRUCT  SchedCbit;
  MC0_CH0_CR_SCHED_CBIT_STRUCT  SchedCbitSave;
  DDR5_MODE_REGISTER_23_TYPE    MR23;
  MrcModeRegister               MRNumber;
  const MrcSpd                  *SpdIn;
  const SPD5_MANUFACTURING_DATA *Ddr5ManufactureData;
  BOOLEAN                       IsSppr;

  Outputs       = &MrcData->Outputs;
  Inputs        = &MrcData->Inputs;
  Debug         = &Outputs->Debug;
  Status        = mrcSuccess;

  ChannelOut    = &Outputs->Controller[Controller].Channel[Channel];
  DimmOut       = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)];
  Width16       = (DimmOut->SdramWidth == 16);
  IsSppr        = Inputs->ExtInputs.Ptr->PprRepairType == SOFT_PPR;

  // DDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT Ddr5PprGuardKeySequence[] = {
    { mrMR24, 0xCF },
    { mrMR24, 0x73 },
    { mrMR24, 0xBB },
    { mrMR24, 0x3B },
    { mrEndOfSequence, 0 }
  };

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Entering PPR\n");
  DimmIdx = (UINT8) RANK_TO_DIMM_NUMBER (Rank);

  if ((BankGroup == 0x0) || (BankGroup == 0x1)) {
    MRNumber = mrMR54;
  } else if ((BankGroup == 0x2) || (BankGroup == 0x3)) {
    MRNumber = mrMR55;
  } else if ((BankGroup == 0x4) || (BankGroup == 0x5)) {
    MRNumber = mrMR56;
  } else if ((BankGroup == 0x6) || (BankGroup == 0x7)) {
    MRNumber = mrMR57;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "DDR5 PPR Error, incorrect parameters: BankGroup%d, BankAddress%d\n", BankGroup, BankAddress);
    return mrcWrongInputParameter;
  }

  MrcIssueMrr (MrcData, Controller, Channel, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BGa
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (Byte == MRC_DDR5_ECC_BYTE) {
      continue;
    }

    if (Width16) {
      // Need combine 2 bytes for repair
      if ((Byte % 2) == 0) {
        // If either byte is set, set 2 bytes
        if (((0x3 << Byte) & ByteMask) != 0) {
          ByteMask |= 0x3 << Byte;
        }
      }
      // DRAM returns only the even byte
      MrrByte = Byte & 0xFE;
    } else {
      MrrByte = Byte;
    }

    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "  MR%d Byte %d MrrByte %d: ", MRNumber, Byte, MrrByte);

    if (((1 << Byte) & ByteMask) == 0) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Disabled in ByteMask\n");
      continue;
    }
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Value = 0x%X\n", MrrResult[MrrByte]);

    PprResource = MrrResult[MrrByte];
    if (BankGroup & 0x1) { // Odd number bank group
      PprResource = (PprResource & (MRC_BIT4 << BankAddress));
    } else { // Even number bank group
      PprResource = (PprResource & (MRC_BIT0 << BankAddress));
    }
    if (PprResource == 0) { // Resource is not available
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PPR Resource not available: BankGroup%d, BankAddress%d, Byte%d, removing from ByteMask\n", BankGroup, BankAddress, Byte);
      ByteMask &= ~(1 << Byte); // Remove current byte from ByteMask
    }
  }
  if (ByteMask == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Exiting PPR\n", BankGroup, BankAddress);
    return mrcPPRBankRowUnavailable;
  }

  MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Repair ByteMask: 0x%X\n", ByteMask);

  // PPR resource for Bank Group/Address pair is available
  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_CBIT_REG, MC1_CH0_CR_SCHED_CBIT_REG, Controller, MC0_CH1_CR_SCHED_CBIT_REG, Channel);
  SchedCbit.Data = MrcReadCR (MrcData, Offset);
  SchedCbitSave.Data = SchedCbit.Data;
  SchedCbit.Bits.dis_pt_it = 0x1;     // Disabling page-table idle timer, to allow custom wait time before PRE issued
  MrcWriteCR (MrcData, Offset, SchedCbit.Data);

  MrhCommand.Ddr5.CA_0 = 0x000B; // Precharge All command
  MrhCommand.Ddr5.CA_1 = 0x0;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, FALSE); // Issue PREA using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }

  // Zero the contents of MR23 to ensure only one bit is set
  MR23.Data8 = 0;
  if (!IsSppr) {
    MR23.Bits.Hppr = 1; // Setting bit OP[0] to high for hPPR
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PprRepairType enable SOFT PPR, executing sPPR\n");
    MR23.Bits.Sppr = 1; // Setting bit OP[1] to high for sPPR
  }
  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }

  for (Index = 0; Ddr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, Ddr5PprGuardKeySequence[Index].MRnum, Ddr5PprGuardKeySequence[Index].Data, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  Status = MrcPprActAndDqLow (MrcData, Controller, Channel, Rank, Row, BankGroup, BankAddress, ByteMask); // Issue ACT with target row and drive DQ low
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcWait (MrcData, 2000 * MRC_TIMER_1MS);// Wait 2000ms for Repair to complete

  MrhCommand.Ddr5.CA_0 = 0x001B; // Precharge PB
  MrhCommand.Ddr5.CA_0 |= (BankGroup & 0x7) << 8;
  MrhCommand.Ddr5.CA_0 |= (BankAddress & 0x3) << 6;
  MrhCommand.Ddr5.CA_1 = 0x0;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, FALSE); // Issue PREpb for this BA/BG
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcWait (MrcData, 50 * MRC_TIMER_1US);// Wait 50us for tPGMPST

  // Setting bits OP[0] and OP[1] to low to exit PPR
  MR23.Bits.Hppr = 0;
  MR23.Bits.Sppr = 0;
  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);

  MrcWriteCR (MrcData, Offset, SchedCbitSave.Data);

  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[DimmIdx].Spd.Data;
  Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
  if ((!IsSppr) &&
    ((Ddr5ManufactureData->DramIdCode.Data == 0xCE80) ||
      (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xCE80))) {
    MrcResetSequence(MrcData);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PPR ended\n");
  return Status;
}
#else
/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row

  @retval MrcStatus
**/
MrcStatus
Ddr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row,
  IN UINT16               ByteMask
)
{
  MrcOutput                     *Outputs;
  MrcInput                      *Inputs;
  MRC_EXT_INPUTS_TYPE           *ExtInputs;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  BOOLEAN                       Width16;
  MrcTiming                     *Timing;
  MrcDebug                      *Debug;
  MrcStatus                     Status;
  INT64                         GetSetEn;
  INT64                         GetSetDis;
  INT64                         GetSetVal;
  UINT32                        tMpcNck;
  UINT32                        tMpcNckFs;
  UINT32                        tMpcNs;
  UINT8                         DimmIdx;
  UINT8                         Byte;
  UINT8                         MrrByte;
  UINT32                        Index;
  UINT32                        tRCD;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_23_TYPE    MR23;
  MrcModeRegister               MRNumber;
  UINT8                         PprResource;
  MRC_GEN_MRH_COMMAND           MrhCommand;    // Used for MRH generic commands
  Ddr5ActStruct                 Ddr5ActCommand;
  const MrcSpd                  *SpdIn;
  const SPD5_MANUFACTURING_DATA *Ddr5ManufactureData;
  INT64                         DisPageTableIdleTimerSave;
  BOOLEAN                       IsSppr;

  Outputs     = &MrcData->Outputs;
  Inputs      = &MrcData->Inputs;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Timing      = &Outputs->Timing[ExtInputs->MemoryProfile];
  Debug       = &Outputs->Debug;
  Status      = mrcSuccess;
  GetSetEn    = 1;
  GetSetDis   = 0;
  tMpcNck     = MrcGetTmod (MrcData, Timing->tCK);
  tMpcNckFs   = tMpcNck * Outputs->MemoryClock;
  tMpcNs      = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);
  tRCD        = Timing->tRCDtRP;
  tRCD        *= Outputs->tCKps; // convert from nCK to ps
  tRCD        /= 1000; //convert from ps to ns
  IsSppr      = ExtInputs->PprRepairType == SOFT_PPR;

  // DDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT Ddr5PprGuardKeySequence[] = {
    { mrMR24, 0xCF },
    { mrMR24, 0x73 },
    { mrMR24, 0xBB },
    { mrMR24, 0x3B },
    { mrEndOfSequence, 0 }
  };

  DimmIdx = (UINT8) RANK_TO_DIMM_NUMBER (Rank);
  DimmOut = &ChannelOut->Dimm[DimmIdx];
  Width16 = (DimmOut->SdramWidth == 16);

  // Get MR associated with BankGroup
  if ((BankGroup == 0x0) || (BankGroup == 0x1)) {
    MRNumber = mrMR54;
  } else if ((BankGroup == 0x2) || (BankGroup == 0x3)) {
    MRNumber = mrMR55;
  } else if ((BankGroup == 0x4) || (BankGroup == 0x5)) {
    MRNumber = mrMR56;
  } else if ((BankGroup == 0x6) || (BankGroup == 0x7)) {
    MRNumber = mrMR57;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "DDR5 PPR Error, incorrect parameters: BankGroup%d, BankAddress%d\n", BankGroup, BankAddress);
    return mrcWrongInputParameter;
  }

  // Save value of page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, ReadCached, &DisPageTableIdleTimerSave);
  // Disabling page-table idle timer, to allow custom wait time before PRE issued
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &GetSetEn);

  MrcIssueMrr (MrcData, Controller, Channel, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (Byte == MRC_DDR5_ECC_BYTE) {
      continue;
    }

    if (Width16) {
      // Need combine 2 bytes for repair
      if ((Byte % 2) == 0) {
        // If either byte is set, set 2 bytes
        if (((0x3 << Byte) & ByteMask) != 0) {
          ByteMask |= 0x3 << Byte;
        }
      }
      // DRAM returns only the even byte
      MrrByte = Byte & 0xFE;
      } else {
      MrrByte = Byte;
    }

    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "  MR%d Byte %d MrrByte %d: ", MRNumber, Byte, MrrByte);

    if (((1 << Byte) & ByteMask) == 0) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Disabled in ByteMask\n");
      continue;
    }
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Value = 0x%X\n", MrrResult[MrrByte]);

    PprResource = MrrResult[MrrByte];
    if (BankGroup & 0x1) { // Odd number bank group
      PprResource = (PprResource & (MRC_BIT4 << BankAddress));
    } else { // Even number bank group
      PprResource = (PprResource & (MRC_BIT0 << BankAddress));
    }
    if (PprResource == 0) { // Resource is not available
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PPR Resource not available: BankGroup%d, BankAddress%d, Byte%d, removing from ByteMask\n", BankGroup, BankAddress, Byte);
      ByteMask &= ~(1 << Byte); // Remove current byte from ByteMask
    }
    }

  if (ByteMask == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Exiting PPR\n", BankGroup, BankAddress);
    return mrcPPRBankRowUnavailable;
  }

  // Initalize all DQ pins
  GetSetVal = 0xFF;
  MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteCached, &GetSetVal);

  // Zero the contents of MR23 to ensure only one bit is set
  MR23.Data8 = 0;

  MrhCommand.Data = 0;
  MrhCommand.Ddr5.CA_0 = 0x000B; // Precharge All command
  MrhCommand.Ddr5.CA_1 = 0x0;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, FALSE); // Issue PREA using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  if (IsSppr) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PprRepairType enables SOFT PPR, executing sPPR\n");
    MR23.Bits.Sppr = 1; // Setting bit OP[0] to high for sPPR
  } else {
    MR23.Bits.Hppr = 1; // Setting bit OP[0] to high for hPPR
  }

  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tRCD * MRC_TIMER_1NS); // wait tRCD

  for (Index = 0; Ddr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, Ddr5PprGuardKeySequence[Index].MRnum, Ddr5PprGuardKeySequence[Index].Data, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }
    MrcWait (MrcData, tRCD * MRC_TIMER_1NS); // wait tRCD
  }

  // Issue ACT to Failing Row
  Ddr5ActCommand.Data32 = Row;
  MrhCommand.Ddr5.CA_0 = (Ddr5ActCommand.Bits.RowBits0_3 << 2) | (BankAddress << 6) | (BankGroup << 8);
  MrhCommand.Ddr5.CA_1 = Ddr5ActCommand.Bits.RowBits4_16;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, TRUE); // Issue 2N ACT using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (((1 << Byte) & ByteMask) == 0) {
      continue;
    }
    GetSetVal = 0;
    MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDqOverrideData, WriteToCache, &GetSetVal);
    GetSetVal = 0xFF;
    MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDqOverrideEn, WriteToCache, &GetSetVal);
  }
  MrcFlushRegisterCachedData (MrcData); // Override DQ to LOW

  MrhCommand.Data = 0;
  MrhCommand.Ddr5.CA_0 = 0xD | ((BankGroup & 0x7) << 8) | ((BankAddress & 0x3) << 6); // WRA
  MrhCommand.Ddr5.CA_1 = 0x800; // Not Partial

  // Enable multicyccmd
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMultiCycCmd, WriteNoCache, &GetSetEn);
  // WRA, this should send WRCMD indication, for DQS
  MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, TRUE, TRUE);
  MrcWait (MrcData, tMpcNs);
  // Disable multicyccmd
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMultiCycCmd, WriteNoCache, &GetSetDis);

  // Wait tPGMa/tPGMb for Repair to complete. tPGMa is used for x8, tPGMb for x16
  if (DimmOut->SdramWidth == 8) {
    MrcWait (MrcData, MRC_DDR5_tPGMa * MRC_TIMER_1MS);
  } else {
    MrcWait (MrcData, MRC_DDR5_tPGMb * MRC_TIMER_1MS);
  }

  GetSetVal = 0x0;
  MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn, WriteToCache, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);

  MrhCommand.Ddr5.CA_0 = 0x001B;
  MrhCommand.Ddr5.CA_0 |= (BankGroup & 0x7) << 8;
  MrhCommand.Ddr5.CA_0 |= (BankAddress & 0x3) << 6;
  MrhCommand.Ddr5.CA_1 = 0x0;
  MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, FALSE); // Precharge PB
  MrcWait (MrcData, tMpcNs);

  // Wait tPGM_Exit(min) = tRP = tRCD
  MrcWait (MrcData, tRCD * MRC_TIMER_1US);

  // Setting bits OP[1:0] to low to exit PPR
  MR23.Bits.Sppr = 0;
  MR23.Bits.Hppr = 0;
  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcWait (MrcData, MRC_DDR5_tPGMPST * MRC_TIMER_1US);

  IoReset (MrcData);

  // Restoring page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &DisPageTableIdleTimerSave);
  MrcWait (MrcData, 20 * MRC_TIMER_1NS);

  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[DimmIdx].Spd.Data;
  Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
  if ((!IsSppr) &&
    ((Ddr5ManufactureData->DramIdCode.Data == 0xCE80) ||
      (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xCE80))) {
    MrcResetSequence(MrcData);
  }

  return Status;
}
#endif  // DDR5 PPR

/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row

  @retval MrcStatus
**/

MrcStatus
LpDdr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row
  )
{
  MRC_EXT_INPUTS_TYPE             *ExtInputs;
  MrcOutput                       *Outputs;
  //MrcFrequency                    CurrentFrequency;
  MrcChannelOut                   *ChannelOut;
  MrcTiming                       *Timing;
  MrcStatus                       Status;
  INT64                           GetSetEn;
  INT64                           GetSetDis;
  INT64                           GetSetVal;
  INT64                           WckMode;
  INT64                           SavetWckStop;
  INT64                           tWckStop;
  INT64                           tWckOff;
  INT64                           RdWckAsync;
  UINT32                          tMpcNck;
  UINT32                          tMpcNckFs;
  UINT32                          tMpcNs;
  UINT32                          WckDelay;
  UINT8                           MrrResult[MRC_MRR_ARRAY_SIZE];
  //BOOLEAN                         IsFreqSwitchRequired;
  //UINT8                           RankMask;
  UINT32                          tCK;
  UINT32                          tMRD;
  UINT32                          tMRDns;

  LpDdr5ActStruct                 LpDdr5ActCommand;
  LPDDR5_MODE_REGISTER_41_TYPE    Mr41;
  MRC_GEN_MRH_COMMAND             MrhCommand;    // Used for MRH generic commands
  MRC_LP5_BANKORG                 BankMode;
  UINT32                          PprResource;

  ExtInputs             = MrcData->Inputs.ExtInputs.Ptr;
  Outputs               = &MrcData->Outputs;
  GetSetEn              = 1;
  GetSetDis             = 0;
  //IsFreqSwitchRequired  = FALSE;
  BankMode              = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  ChannelOut            = &Outputs->Controller[Controller].Channel[Channel];
  Timing                = &Outputs->Timing[ExtInputs->MemoryProfile];
  tMpcNck               = MrcGetTmod (MrcData, Timing->tCK);
  tMpcNckFs             = tMpcNck * Outputs->MemoryClock;
  tMpcNs                = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);
  tCK                   = Outputs->MemoryClock;
  tMRD                  = MrcGetTmod (MrcData, tCK);
  tMRDns                = tMRD * tCK;
  tMRDns                = DIVIDECEIL (tMRDns, FEMTOSECONDS_PER_NANOSECOND);
  //RankMask              = 1 << Rank;

  // LPDDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT LpDdr5PprGuardKeySequence[] = {
    { mrMR42, 0xCF },
    { mrMR42, 0x73 },
    { mrMR42, 0xBB },
    { mrMR42, 0x3B },
    { mrEndOfSequence, 0 }
  };

  MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR29, MrrResult);
  switch (BankMode) {
    case MrcLp58Bank:
      PprResource = (MrrResult[0] & (MRC_BIT0 << BankAddress));
      break;
    case MrcLp516Bank:
      PprResource = (MrrResult[0] & (MRC_BIT0 << (BankAddress & 0x7)));
      break;
    case MrcLp5BgMode:
      if ((BankGroup % 2) == 0) { // Even number bank groups
        PprResource = (MrrResult[0] & (MRC_BIT0 << BankAddress));
      } else { // Odd number bank groups
        PprResource = (MrrResult[0] & (MRC_BIT4 << BankAddress));
      }
      break;
    default:
      return mrcWrongInputParameter;
      break;
  }
  if (PprResource == 0) { // Resource is not available
    return mrcPPRBankRowUnavailable;
  }

  // PPR resource for Bank Group/Address pair is available

  /*
  CurrentFrequency = Outputs->Frequency;
  // PPR must run at a frequency <= 6400MHz
  if (CurrentFrequency > f6400) {
    IsFreqSwitchRequired = TRUE;
    Status = MrcSetFspVrcg (MrcData, RankMask, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, LpFspOpPoint0);
    Status |= MrcFrequencySwitch (MrcData, Outputs->LowFrequency, MRC_PRINTS_OFF);
    if (Status != mrcSuccess) {
      return Status;
    }
  }
  */

  // Disabling page-table idle timer, to allow custom wait time before PRE issued
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &GetSetEn);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccLp5WckMode, ReadCached, &WckMode);
  if (WckMode == MrcLp5WckSafe) {
    // If in Wck Safe Mode, Extend out twckstop and disable Wck on all channels
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttWckStop, ReadCached, &SavetWckStop);
    GetSetVal = MC0_CH0_CR_TC_WCK_tWCKSTOP_MAX;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMcttWckStop, WriteCached, &GetSetVal);
    MrcIssueCas (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MrhCasWck2CkSyncOff, FALSE);
  } else if (WckMode == MrcLp5WckManual) {
    // If Wck is in manual mode ensure Wck has stopped before moving on
    MrcGetSetMcCh (MrcData, Controller, Channel, GSmMcttWckOff, ReadCached, &tWckOff);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRdWckAsyncGap, ReadFromCache, &RdWckAsync);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttWckStop, ReadFromCache, &tWckStop);
    WckDelay = (UINT32) tWckOff + (UINT32) RdWckAsync + (UINT32) tWckStop;
    WckDelay *= Outputs->Wckps;
    WckDelay /= 1000;

    // Wait for Wck to stop
    MrcWait (MrcData, WckDelay);
  }

  MrhCommand.Lpddr5.CA_0 = 0x78; // Precharge All command
  MrhCommand.Lpddr5.CA_1 = 0x40;
  MrhCommand.Lpddr5.CA_2 = 0;
  MrhCommand.Lpddr5.CA_3 = 0;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, TRUE); // Issue PREA using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  Mr41.Data8 = (UINT8) ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[mrIndexMR41];
  Mr41.Bits.Ppre = 1; // Setting bit OP[0] to high for PPR
  MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR41, Mr41.Data8, FALSE);
  MrcWait (MrcData, tMRDns);

  for (UINT32 Index = 0; LpDdr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    MrcIssueMrw (MrcData, Controller, Channel, Rank, LpDdr5PprGuardKeySequence[Index].MRnum, LpDdr5PprGuardKeySequence[Index].Data, FALSE);
    MrcWait (MrcData, tMRDns);
  }

  LpDdr5ActCommand.Data32 = Row;

  MrhCommand.Lpddr5.CA_0 = (LpDdr5ActCommand.Bits.RowBits14_17 << 3) | 0x07; // ACT command 1
  switch (BankMode) {
    case MrcLp58Bank:
    case MrcLp516Bank:
      MrhCommand.Lpddr5.CA_1 = BankAddress | (LpDdr5ActCommand.Bits.RowBits11_13 << 4);
      break;
    case MrcLp5BgMode:
      MrhCommand.Lpddr5.CA_1 = BankAddress | (BankGroup << 2) | (LpDdr5ActCommand.Bits.RowBits11_13 << 4);
      break;
  }
  MrhCommand.Lpddr5.CA_2 = (LpDdr5ActCommand.Bits.RowBits7_10 << 3) | 0x03;  // ACT command 2
  MrhCommand.Lpddr5.CA_3 = LpDdr5ActCommand.Bits.RowBits0_6;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, FALSE); // Issue ACT using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  // Wait for PPR programming time
  MrcWait (MrcData, MRC_LPDDR5_tPGM * MRC_TIMER_1MS);

  MrhCommand.Lpddr5.CA_0 = 0x78; // Precharge All command
  MrhCommand.Lpddr5.CA_1 = 0x40;
  MrhCommand.Lpddr5.CA_2 = 0;
  MrhCommand.Lpddr5.CA_3 = 0;

  Status = MrcRunGenericMrh (MrcData, Controller, Channel, Rank, MrhCommand, FALSE, TRUE); // Issue PREA using MRH command
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  // Wait PPR Exit time
  MrcWait (MrcData, MRC_LPDDR5_tPGM_EXIT * MRC_TIMER_1NS);

  Mr41.Bits.Ppre = 0; // Setting bit OP[0] to low to exit PPR
  MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR41, Mr41.Data8, FALSE);

  // Wait PPR new Address Setting time
  MrcWait (MrcData, MRC_LPDDR5_tPGMPST * MRC_TIMER_1US);

  // Reenabling page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &GetSetDis);

  /*
  if (IsFreqSwitchRequired) {
    // Set FSP-OP = 1, set High frequency
    Status = MrcLpddrSwitchToHigh (MrcData, MRC_PRINTS_OFF);
    if (Status != mrcSuccess) {
      return Status;
    }
  }
  */

  MrcResetSequence (MrcData);

  if (WckMode == MrcLp5WckSafe) {
    // Start WCK on all Channels
    Status = MrcIssueCas (MrcData, MAX_CONTROLLER, MAX_CHANNEL, Rank, MrhCasWckFastSync, FALSE);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMcttWckStop, WriteCached, &SavetWckStop);
  }

  return Status;
}

/**
  This function enables and runs Post Package Repair on detected fail rows.
  @param[in] MrcData     - Pointer to global MRC data.
  @param[in] Controller  - 0-based index to controller
  @param[in] Channel     - 0-based index to channel
  @param[in] Rank        - Physical rank index inside the dimm
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row
  @retval MrcStatus - mrcSuccess if the DelayType is supported.
**/
MrcStatus
MrcPostPackageRepair (
  IN  MrcParameters *const MrcData,
  IN  UINT32         Controller,
  IN  UINT32         Channel,
  IN  UINT32         Rank,
  IN  UINT32         BankGroup,
  IN  UINT32         BankAddress,
  IN  UINT32         Row,
  IN  UINT16         ByteMask
  )
{
  MrcOutput      *Outputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDdrType     DdrType;
  MrcStatus      Status;

  Outputs = &MrcData->Outputs;
  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  DdrType = Outputs->DdrType;

  if (ExtInputs->PprRepairType == NOREPAIR_PPR) {
    return mrcSuccess;
  }

  Status      = mrcSuccess;
  if (DdrType == MRC_DDR_TYPE_DDR5) {
    Status = Ddr5PostPackageRepair (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row, ByteMask);
  } else {
    if (ExtInputs->PprRepairType == HARD_PPR) {
      Status = LpDdr5PostPackageRepair (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row);
    }
  }

  // Updating MRC PPR Status
  MrcData->Outputs.MrcPprStatus = Status;
  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "MrcPostPackageRepair Status: %s\n", Status == mrcSuccess ? "Success" : "Failed");
  return Status;
}

/**

  Get status whether PPR resource is available

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] Rank           - Rank number
  @param[in] DeviceByte     - Byte number used to identify DRAM
  @param[in] BankGroup      - BankGroup number
  @param[in] BankAddress    - BankAddress number

  @retval TRUE if PPR resource available; FALSE if not available

**/
BOOLEAN
GetPprResourceAvailable (
  MrcParameters   *const    MrcData,
  UINT32                    Controller,
  UINT32                    Channel,
  UINT32                    Rank,
  UINT16                    DeviceByte,
  UINT8                     BankGroup,
  UINT8                     BankAddress
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  BOOLEAN         PprResourceAvailable;
  UINT32          MRNumber;
  UINT8           MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8           PprResource;
  MRC_LP5_BANKORG BankMode;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  PprResourceAvailable = FALSE;
  BankMode      = MrcGetBankBgOrg(MrcData, Outputs->Frequency);

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      if ((BankGroup == 0x0) || (BankGroup == 0x1)) {
        MRNumber = mrMR54;
      } else if ((BankGroup == 0x2) || (BankGroup == 0x3)) {
        MRNumber = mrMR55;
      } else if ((BankGroup == 0x4) || (BankGroup == 0x5)) {
        MRNumber = mrMR56;
      } else if ((BankGroup == 0x6) || (BankGroup == 0x7)) {
        MRNumber = mrMR57;
      } else {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "DDR5 PPR Error, incorrect parameters: BankGroup%d, BankAddress%d\n", BankGroup, BankAddress);
        return mrcWrongInputParameter;
      }

      MrcIssueMrr (MrcData, Controller, Channel, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG
      PprResource = MrrResult[DeviceByte];
      if (BankGroup & 0x1) { // Odd number bank group
        PprResource = (PprResource & (MRC_BIT4 << BankAddress));
      } else { // Even number bank group
        PprResource = (PprResource & (MRC_BIT0 << BankAddress));
      }
      if (PprResource == 0) { // Resource is not available
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Device%d\n", BankGroup, BankAddress, DeviceByte);
        PprResourceAvailable = FALSE;
      } else {
        PprResourceAvailable = TRUE;
      }
      break;
    case MRC_DDR_TYPE_LPDDR5:
      MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR29, MrrResult);
      PprResource = MrrResult[0];

      switch (BankMode) {
        case MrcLp58Bank:
          PprResource = (PprResource & (MRC_BIT0 << BankAddress));
          break;
        case MrcLp516Bank:
          PprResource = (PprResource & (MRC_BIT0 << (BankAddress & 0x7)));
          break;
        case MrcLp5BgMode:
          if ((BankGroup % 2) == 0) { // Even number bank groups
            PprResource = (PprResource & (MRC_BIT0 << BankAddress));
          } else { // Odd number bank groups
            PprResource = (PprResource & (MRC_BIT4 << BankAddress));
          }
          break;
        default:
          return mrcWrongInputParameter;
          break;
      }
      if (PprResource == 0) { // Resource is not available
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Device%d\n", BankGroup, BankAddress, DeviceByte);
        PprResourceAvailable = FALSE;
      } else {
        PprResourceAvailable = TRUE;
      }
      break;
    default:
      break;
  }
  return PprResourceAvailable;
}

/**
  Get status for all PPR resources and store number of resources available
  per Controller/Channel/Rank/Dimm into Outputs->PprAvailableResources.

  @param[in] MrcData        - Global MRC data structure
**/
VOID
GetAllPprResources (
  MrcParameters   *const    MrcData
  )
{
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  UINT8               Controller;
  UINT8               Ch;
  UINT8               Rank;
  UINT8               Dram;
  UINT8               BytesPerDram;
  MrcDimmOut          *DimmOut;
  UINT8               MaxChDdr;
  MrcChannelOut       *ChannelOut;
  UINT8               DeviceCount;
  UINT8               BankGroup;
  UINT32              MRNumber;
  UINT8               MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8               PprResource;
  UINT8               Ddr5BgIdx;
  UINT8               Lp5BankIdx;


  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MaxChDdr    = Outputs->MaxChannels;
  DeviceCount = Outputs->SdramCount;


  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Ch];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Ch, Rank)) {
          continue;
        }

        DimmOut = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER(Rank)];

        if (DimmOut->SdramWidth == 8) { // x8
          BytesPerDram = 1;
        } else { // x16
          BytesPerDram = 2;
        }

        switch (Outputs->DdrType) {
          case MRC_DDR_TYPE_DDR5:
            // Minimize the number of MRRs by reading the MrrResult for every device before moving to the next MR number
            for (BankGroup = 0; BankGroup < DimmOut->BankGroups; BankGroup += 2) {
              switch (BankGroup) {
                case 0x0:
                case 0x1:
                  MRNumber = mrMR54;
                  break;
                case 0x2:
                case 0x3:
                  MRNumber = mrMR55;
                  break;
                case 0x4:
                case 0x5:
                  MRNumber = mrMR56;
                  break;
                case 0x6:
                case 0x7:
                  MRNumber = mrMR57;
                  break;
                default:
                  MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Warning: BankGroup %d is outside of range 0-7\n", BankGroup);
                  continue;
              }
              MrcIssueMrr (MrcData, Controller, Ch, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG

              for (Dram = 0; Dram < DeviceCount; Dram += BytesPerDram) {
                Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] = 0;
                PprResource = MrrResult[Dram];
                for (Ddr5BgIdx = 0; Ddr5BgIdx < 2; Ddr5BgIdx++) {
                  // Ddr5BgIdx 0 => Even BG - take the logical AND of bytes 0-3
                  // Ddr5BgIdx 1 => Odd BG - take the logical AND of bytes 4-7
                  if (((PprResource >> (4 * Ddr5BgIdx)) & 0xF) == 0xF) {
                    Outputs->PprAvailableResources[Controller][Ch][Rank][Dram]++;
                  }
                }
              } // for Dram
            }
            break;
          case MRC_DDR_TYPE_LPDDR5:
            MrcIssueMrr (MrcData, Controller, Ch, Rank, mrMR29, MrrResult);
            for (Dram = 0; Dram < DeviceCount; Dram += BytesPerDram) {
              Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] = 0;
              PprResource = MrrResult[Dram];
              for (Lp5BankIdx = 0; Lp5BankIdx < 8; Lp5BankIdx++) {
                if (PprResource & (1 << Lp5BankIdx)) {
                  Outputs->PprAvailableResources[Controller][Ch][Rank][Dram]++;
                }
              }
              // Available resources for LP5 are the number of banks with PPR resource available
              Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] *= 2;
            }
            break;
          default:
            break;
        }
      } // for Rank
    } // for Ch
  } // for Controller
}


/**
  Returns the number of low order bank group and bank address bits that are not included in the BG interleave

  @param[in] MrcData                - Pointer to MrcData

  @retval BaseBits                  - Number of bank bits for software bank loop
**/
UINT32
GetBaseBits (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput           *Outputs;
  UINT8               BaseBits = 0;

  Outputs = &MrcData->Outputs;

  if (Outputs->DdrType == MRC_DDR_TYPE_DDR5) { // DDR5 has 32 banks (8 BG, 4 banks per BG)
    // BG_Interleave 8
    BaseBits = MrcLog2 (MrcData, MAX_BANKS / MAX_DDR5_BANK_GROUPS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
  } else { // LPDDR
    if (MrcGetBankBgOrg (MrcData, Outputs->Frequency) == MrcLp5BgMode) {
      // BG_Interleave 4
      BaseBits = MrcLog2 (MrcData, MAX_LPDDR5_BANKS / MAX_LPDDR5_BANK_GROUPS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
    } else {
      BaseBits = MrcLog2 (MrcData, MAX_LPDDR5_BANKS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
    }
  }

  return BaseBits;
} // GetBaseBits

/**
  Cleans up registers and values set during PPR memory tests.

  @param[in] MrcData            - Global MRC data structure
  @param[in] McChBitMask        - Bit masks of MC Channels to program the register for.
  @param[in] BankMapping        - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers.
**/
VOID
PprCleanup (
  IN MrcParameters *const MrcData,
  IN UINT8                McChBitMask,
  IN MRC_BG_BANK_PAIR     BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS]
  )
{
  MrcOutput       *Outputs;
  UINT8           Controller;
  UINT8           Channel;
  INT64           DefaultRefreshMask;
  BOOLEAN         StopOnRaster;
  BOOLEAN         RasterRepoClear;
  UINT8           RasterRepoMode;
  UINT8           Mode3Banks;
  UINT32          Mode3Max;

  Outputs = &MrcData->Outputs;
  MrcGetMcConfigGroupLimits (MrcData, GsmMccRefreshRankMask, NULL, &DefaultRefreshMask, NULL);
  StopOnRaster = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_StopOnRaster_DEF;
  RasterRepoClear = 1;
  RasterRepoMode = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_RasterRepoMode_DEF;
  Mode3Banks = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_MODE3_BANKS_DEF;
  Mode3Max = MC0_CH0_CR_CPGC2_RASTER_MODE3_MAX_FAIL_MAX_DEF;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      // Restore refreshes on all ranks to default value
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteNoCache, &DefaultRefreshMask);
      // Restore Bank L2P Mapping
      Cpgc20GetSetBankSequence (MrcData, Controller, Channel, &BankMapping[Controller][Channel][0], MAX_BANKS, MRC_SET);
    }
  }

  // Clear raster repo, set other fields to default
  Cpgc20RasterRepoConfig (MrcData, McChBitMask, &Mode3Banks, NULL, &StopOnRaster, &RasterRepoClear, &RasterRepoMode, (UINT8 *) &Mode3Max);

  // Clean values set in DPAT_BUF_CFG and DPAT_ALT_BUF_CFG
  // Cpgc20SetDpatBufCtl (MrcData, 0, 0, 0, 0);
  Cpgc20SetDpatAltBufCtl (MrcData, 0, 0, 0, 0);

  Cpgc20SetDataControl (MrcData, MC0_REQ0_CR_CPGC2_DATA_CONTROL_Data_Select_Rotation_Repeats_DEF, MC0_REQ0_CR_CPGC2_DATA_CONTROL_SPLIT_BACKGROUND_DEF);
  MrcSetLoopcount (MrcData, Outputs->McChBitMask, 0);
  AmtSetBaseAddressControl (MrcData, AdvMtWcMats8); // Reset fields to default (0)

  Outputs->PprPatBufShift = 0;
}

/**
  Check row failure if repairs are required

  @param[in] FailRangePtr         - Pointer to Row Fail
  @param[in] BankGroup            - Bank group

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsRowFailureRepairRequired (
  ROW_FAIL_RANGE  *const FailRangePtr,
  UINT8                  BankGroup
  )
{
  // the bankgroup has failure and it's not repaired.
  return (FailRangePtr->BankGroupMask & (1 << BankGroup)) != 0
          && (FailRangePtr->BankGroupRepairedMask & (1 << BankGroup)) == 0;
}
/**
  Check row failure list and PPR resource list to determine if repairs are required

  @param[in] MrcData              - Global MRC data structure
  @param[in] BaseBits             - Number of bank bits in SW loop

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsPprRepairRequired (
  MrcParameters  *const   MrcData,
  UINT8                   BaseBits
  )
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT32              FailIndex;
  UINT8               Controller;
  UINT8               Ch;
  UINT8               Rank;
  UINT8               Bank;
  UINT8               BankInterleaveIndex;
  UINT8               Dram;
  UINT8               BytesPerDram;
  UINT32              RowFailMax;
  MrcDimmOut          *DimmOut;
  UINT8               MaxChDdr;
  MrcOutput           *Outputs;
  MrcChannelOut       *ChannelOut;
  UINT8               MSVx4; // device count

  Outputs         = &MrcData->Outputs;
  MSVx4           = Outputs->SdramCount;

  // Inspect test results for given rank, bank
  MaxChDdr = Outputs->MaxChannels;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Ch];

      // Was this channel part of the test group?
      if ((MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Ch, MaxChDdr) != 0)) {
        // Loop through all row failure ranges
        RowFailMax = AmtGetRowFailListSize (MrcData, Controller, Ch);
        for (FailIndex = 0; FailIndex < RowFailMax; FailIndex++) {
          FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Ch, FailIndex);

          Rank = (UINT8) FailRangePtr->Addr.Bits.Rank;
          Bank = (UINT8) FailRangePtr->Addr.Bits.BankPair;

          DimmOut = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)];
          for (BankInterleaveIndex = 0; BankInterleaveIndex < MAX_MEMTEST_BANK_INTERLEAVE_NUMBER; BankInterleaveIndex++) {
            if (IsRowFailureRepairRequired(FailRangePtr, BankInterleaveIndex)) {

              if (DimmOut->SdramWidth == 8) { // x8
                BytesPerDram = 1;
              } else { // x16
                BytesPerDram = 2;
              }
              for (Dram = 0; Dram < MSVx4; Dram += BytesPerDram) {
                if (GetPprResourceAvailable (MrcData, Controller, Ch, Rank, Dram, BankInterleaveIndex, Bank)) {
                  return 1;
                } // GetPprResourceAvailable
              } // for Dram
            } // if FailRangePtr->BankGroupMask
          } // for BankInterleaveIndex
        } // for FailIndex
      } // if MC_CH_MASK_CHECK
    } // for Ch
  } // for Controller

  return 0;
} // IsPprRepairRequired

/**
  Checks row failure list for any failures, and checks whether PPR resource exists for any failures.
  If PPR resources exist, runs disposition flow for each failure.

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     BankMapping         - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers.
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE
**/
MrcStatus
PprDispositionFailRange (
  IN     MrcParameters *const    MrcData,
  IN     MRC_BG_BANK_PAIR        BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  IN OUT UINT8                   *RepairDone
  )
{
  MrcOutput           *Outputs;
  UINT32              RankChEnMap = 0;
  UINT8               Channel;
  UINT8               Controller;
  UINT8               Rank;
  UINT8               BaseBits;
  UINT8               RowFailuresFound;
  MrcStatus           RetVal = mrcSuccess;
  BOOLEAN             PprRequired;
  UINT8               MaxChDdr;

  Outputs = &MrcData->Outputs;
  MaxChDdr = Outputs->MaxChannels;

  *RepairDone = 0;

  // Are there any failures in the list?
  RowFailuresFound = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, MaxChDdr) == 0) {
        continue;
      }
      if (AmtGetRowFailListSize (MrcData, Controller, Channel) > 0) {
        RowFailuresFound = 1;
        break;
      }
    }
  }

  if (RowFailuresFound) {

     BaseBits = (UINT8) GetBaseBits (MrcData);
     // Check if any failure is on a device with a PPR resource
     PprRequired = IsPprRepairRequired (MrcData, BaseBits);
     if (PprRequired) {
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "PprDispositionFailRange: Row Failures found and PPR resources available - Execute PPR flow to repair row failures\n");
     }

    // Loop over all ranks
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      // check if any rank enabled per controller/channel
      RankChEnMap = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
          if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
            RankChEnMap |= (1 << ((Controller * Outputs->MaxChannels) + Channel));
          }
        }
      }

      // skip to next rank if no ranks enabled
      if (RankChEnMap == 0) {
        continue;
      }
      // Search row fail list and apply PPR repairs as needed
      if (PprRequired) {
        DispositionFailRangesWithPprFlow(MrcData, RankChEnMap, Rank, BaseBits, BankMapping, RepairDone);
      }
    } // for Rank
  } // RowFailuresFound

  return RetVal;
} // PprDispositionFailRange

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in]     Rank                - Rank number
  @param [in]     BaseBits            - Number of BaseBits per technology
  @param [in]     BankMapping         - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers.
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE

**/
MrcStatus
DispositionFailRangesWithPprFlow (
  IN     MrcParameters* const    MrcData,
  IN     UINT32                  McChBitMask,
  IN     UINT32                  Rank,
  IN     UINT8                   BaseBits,
  IN     MRC_BG_BANK_PAIR        BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  IN OUT UINT8*                  RepairDone
)
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT8               Controller;
  UINT8               Channel;
  UINT8               Bank;
  UINT8               BankInterleaveIndex;
  UINT8               PprBank;
  UINT8               Byte;
  UINT16              ByteMask;
  UINT32              FailIndex;
  ROW_ADDR            CurAddr;
  MrcStatus           Status;
  BOOLEAN             RepairStatus = FALSE;
  UINT8               MaxChDdr;
  MrcOutput           *Outputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT32              DqMask[DQ_MASK_INDEX_MAX];
  UINT32              NumRowsError;
  UINT32              NumRepairsIssued;
  UINT32              NumRepairsSuccessful;
  UINT8               McChRowTestBitMask;
  INT64               DisableRefreshMask;
 
  Outputs = &MrcData->Outputs;
  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  MaxChDdr = Outputs->MaxChannels;
  MrcGetMcConfigGroupLimits (MrcData, GsmMccRefreshRankMask, NULL, &DisableRefreshMask, NULL);
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "DispositionFailRangesWithPprFlow Starts:  McChBitMask=0x%x\n", McChBitMask);

  // Inspect test results for given rank and bank
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      // Was this channel part of the test group?
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChDdr) == 0) {
        continue;
      }
      NumRowsError = 0;
      NumRepairsIssued = 0;
      NumRepairsSuccessful = 0;

      // For each bank pair
      for (Bank = 0; Bank < (1 << BaseBits); Bank++) {
        CurAddr.Data = 0;
        CurAddr.Bits.BankPair = Bank;
        CurAddr.Bits.Rank = Rank;
        CurAddr.Bits.Valid = 1;

        // Loop through all row failure ranges
        for (FailIndex = 0; FailIndex < AmtGetRowFailListSize (MrcData, Controller, Channel); FailIndex++) {
          FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Channel, FailIndex);
          if (ExtInputs->PprRepairType == NOREPAIR_PPR) {
            // Set RepairedMask
            if (FailRangePtr->BankGroupRepairedMask != FailRangePtr->BankGroupMask) {
              FailRangePtr->BankGroupRepairedMask = FailRangePtr->BankGroupMask;
              NumRowsError++;
            }
            continue;
          }

          // Check for address match, masking row number
          if (AmtCurrentAddrMatch (CurAddr, FailRangePtr->Addr)) {
            Status = mrcFail;
            for (BankInterleaveIndex = 0; BankInterleaveIndex < 8; BankInterleaveIndex++) {
              // If current BG has a failure.
              if (IsRowFailureRepairRequired(FailRangePtr, BankInterleaveIndex)) {

                // Get the failing bank address. The interleavingIndex contains the upper bank(bank group) address bit.
                PprBank = (BankInterleaveIndex << BaseBits) + Bank;

                DqMask[0] = 0xFFFFFFFF;
                DqMask[1] = (Outputs->EccSupport) ? 0xFF : 0;

                // Construct DRAM Mask
                ByteMask = 0;
                for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                  //
                  // Set byte mask bits based on failing DQ bits
                  //
                  if ((DqMask[Byte / 4] >> (((Byte * 8) % 32))) & 0xFF) {
                    ByteMask |= 1 << Byte;
                  }
                }
                //
                // If ByteMask bits are set, then execute a PPR repair flow
                //
                if (ByteMask) {

                  NumRowsError++;

                  RepairStatus = FALSE;

                  // Test on paired channels for LPDDR
                  if (Outputs->IsLpddr) {
                    McChRowTestBitMask = 3 << ((Controller * Outputs->MaxChannels) + 2 * LP_IP_CH(Outputs->IsLpddr, Channel));
                  } else {
                    McChRowTestBitMask = 1 << MC_CH_IDX(Controller, Channel, Outputs->MaxChannels);
                  }
                  // Run clean-up before PPR execution
                  PprCleanup (MrcData, McChRowTestBitMask, BankMapping);
                  // Test row
                  RowTestPpr(MrcData, McChRowTestBitMask, Rank, PprBank, FailRangePtr->Addr.Bits.Row);
                  // Disable refresh
                  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteNoCache, &DisableRefreshMask);

                  // Attempt PPR repair
                  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Calling PPR for Mc%d, Ch%d, Rank: 0x%x, BG: 0x%x, Bank: 0x%x Row: 0x%x, ByteMask = 0x%x\n",
                    Controller, Channel, Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  Status = MrcPostPackageRepair (MrcData, Controller, Channel, Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  ++NumRepairsIssued;
                  FailRangePtr->BankGroupAttempts[BankInterleaveIndex]++;

                  // Post-repair analysis
                  if (Status == mrcSuccess && (RowTestPpr(MrcData, McChRowTestBitMask, Rank, PprBank, FailRangePtr->Addr.Bits.Row) == mrcSuccess)) {
                    ++NumRepairsSuccessful;
                    RepairStatus = TRUE;
                  } else {
                    Outputs->PprRepairFails++;
                  }
                  // Clean up after PPR execution
                  PprCleanup (MrcData, McChRowTestBitMask, BankMapping);
                } // if (ByteMask)

                if (RepairStatus) {
                  *RepairDone = 1;
                  FailRangePtr->BankGroupRepairedMask |= MRC_BIT0 << BankInterleaveIndex;
                  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Repair Success\n");
                } else {
                  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Repair Fail\n");
                }
              } // if ((FailRangePtr->BankGroupMask
            } // BankInterleaveIndex
          } // CurrentAddrMatch
        } // FailIndex
        if (ExtInputs->PprRepairType == NOREPAIR_PPR) {
          break;
        }
      } // Bank pair
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "DispositionFailRangesWithPprFlow: Rank%d Mc%d Ch%d:\n  Number of rows with errors = %d\n  Number of repairs issued = %d\n  Number of repairs successful = %d\n",
        Rank, Controller, Channel, NumRowsError, NumRepairsIssued, NumRepairsSuccessful);
      Outputs->PprNumSuccessfulRepairs += (UINT16) NumRepairsSuccessful;
    } // Ch
  } // Controller

  return mrcSuccess;
} // DispositionFailRangesWithPprFlow

/**
  Check to see if Retry is required after a PPR repair

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  RepairDone  - Indicator whether PPR repair was done
  @param[in]  RetryCount  - Count of retry attempts

  @retval TRUE if Retry is required; FALSE otherwise
**/
BOOLEAN
IsAmtRetryRequiredAfterRepair (
  IN MrcParameters *const MrcData,
  IN UINT8                RepairDone,
  IN UINT32               RetryCount
  )
{
  MrcDebug  *Debug;
  BOOLEAN   RetryRequired;

  Debug = &MrcData->Outputs.Debug;

  if (RetryCount == 0) {
    RetryRequired = TRUE;
  } else if ((RepairDone != 0) && (RetryCount < PPR_REPAIR_RETRY_LOOPS)) {
    RetryRequired = TRUE;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nPPR repair done so retry MemTest!\n");
  } else {
    RetryRequired = FALSE;
  }
  return RetryRequired;
}

/**

  Inject error for given Rank/Controller/Channel

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  Rank          - Phyiscal rank index inisde the dimm
  @param[in]  Controller    - 0-based index to controller
  @param[in]  Channel       - 0-based index to channel
  @param[in]  Bank          - bank address number
  @param[in]  Row           - row address
  @param[in]  TestSize      - number of rows to test
  @param[in]  Pattern       - Array of 64-bit test pattern
  @param[in]  PatternDepth  - Length of PatternQW in number of UIs
  @param[in]  ErrInjMask16  - Bitmask of DQ lanes to inject error
  @param[in]  Direction     - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in]  SeqDataInv[2] - Enables pattern inversion per subsequence
  @param[in]  IsUseInvtPat  - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in]  UiShl         - Enables pattern rotation between all UIs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
InjectMemtestError (
  IN MrcParameters* const MrcData,
  IN UINT32         Rank,
  IN UINT32         Controller,
  IN UINT32         Channel,
  IN UINT32         Bank,
  IN UINT32         Row,
  IN UINT32         TestSize,
  IN UINT64         Pattern[],
  IN UINT8          PatternDepth,
  IN UINT16         ErrInjMask16,
  IN UINT8          Direction,
  IN UINT8          SeqDataInv[2],
  IN BOOLEAN        IsUseInvtPat,
  IN UINT8          UiShl,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
  )
{
  MrcStatus         Status = mrcSuccess;
  MrcOutput         *Outputs;
  MrcDimmOut        *DimmOut;
  MRC_FUNCTION      *MrcCall;
  UINT8             LocalMcChBitMask;
  UINT8             ColumnBits[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL];
  UINT8             RowBits[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL];
  UINT8             BankBits[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL];
  UINT8             RankMask;
  UINT32            BaseBits;
  UINT32            ChannelLoop;
  UINT32            ControllerLoop;
  UINT32            CurrentRow[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32            TestSizeArray[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32            DeviceMask[MAX_CONTROLLER][MAX_CHANNEL][DQ_MASK_INDEX_MAX];
  UINT8             IpChannel;
  UINT32            CpgcBank;

  Outputs         = &MrcData->Outputs;
  MrcCall         = MrcData->Inputs.Call.Func;

  MrcCall->MrcSetMem ((UINT8 *) DeviceMask, sizeof (DeviceMask), 0);
  MrcCall->MrcSetMem ((UINT8 *) CurrentRow, sizeof (CurrentRow), 0);
  MrcCall->MrcSetMem ((UINT8 *) RowBits, sizeof (RowBits), 0); // Only 1 row in test
  MrcCall->MrcSetMem ((UINT8 *) BankBits, sizeof (BankBits), 0);
  MrcCall->MrcSetMem ((UINT8 *) ColumnBits, sizeof (ColumnBits), 0);

  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Error injection beginning\n");

  //manually inject error here (only one controller/channel?)
  //hardcode a bunch of dimm settings
  LocalMcChBitMask = 0; // Reset bitmask for new Rank

  RankMask = (1 << Rank);
  LocalMcChBitMask |= SelectReutRanks (MrcData, (UINT8)Controller, (UINT8)Channel, RankMask, FALSE);
  for (ControllerLoop = 0; ControllerLoop < MAX_CONTROLLER; ControllerLoop++) {
    for (ChannelLoop = 0; ChannelLoop < Outputs->MaxChannels; ChannelLoop++) {
      DimmOut = &Outputs->Controller[ControllerLoop].Channel[ChannelLoop].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
      IpChannel = (UINT8) LP_IP_CH (Outputs->IsLpddr, ChannelLoop);
      if (MC_CH_MASK_CHECK (LocalMcChBitMask, ControllerLoop, ChannelLoop, Outputs->MaxChannels) == 0) {
        if (!IS_MC_SUB_CH (Outputs->IsLpddr, ChannelLoop)) {
          TestSizeArray[ControllerLoop][IpChannel] = 0;
        }
        continue;
      } else {
        CurrentRow[ControllerLoop][IpChannel] = Row;
        TestSizeArray[ControllerLoop][IpChannel] = TestSize;
        // Initialize per-Rank Column Row and Bank sizes
        // The MrcLog2 function returns +1 so we subtract 1
        ColumnBits[Rank][ControllerLoop][ChannelLoop] = MrcLog2 (MrcData, DimmOut->ColumnSize) - 1;
        BankBits[Rank][ControllerLoop][ChannelLoop] = (MrcLog2 (MrcData, DimmOut->Banks) - 1) + (MrcLog2 (MrcData, DimmOut->BankGroups) - 1);
        MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "ErrInj! Rank[%d]Cont[%d]Channel[%d]:\n\tColumnBits=%d\n\tRowBits=%d\n\tBankBits=%d\n", Rank, Controller, Channel, ColumnBits[Rank][Controller][Channel], RowBits[Rank][Controller][Channel], BankBits[Rank][Controller][Channel]);
      }
    }
  }

  BaseBits = GetBaseBits (MrcData);
  CpgcBank = (Outputs->IsLpddr) ? (Bank & 0xF) : Bank;  // For LP5, limit injected bank to between 0 and 15

  // Reset Some cpgc registers for AdvMtWcMats8
  SetupIOTestRetention (MrcData, LocalMcChBitMask, (UINT8) Rank, 1, PatWr, SeqDataInv, Pattern, PatternDepth, IsUseInvtPat, UiShl, 1, Direction, AdvMtWcMats8);
  MrcProgramMATSPattern (MrcData, LocalMcChBitMask, Pattern, PatternDepth, UiShl, ErrInjMask16);
  Status = AmtExecuteRowRangeTest (MrcData, LocalMcChBitMask, Rank, ColumnBits, RowBits, BankBits, PatWr, Direction,
                                  CpgcBank, BaseBits, CurrentRow, TestSizeArray, TRUE, AdvMtWcMats8); // Set FromRowTestPpr = TRUE since we need to program a single bank

  return Status;
}

/**
  Wrapper function for the Advanced Memory test algorithm that tracks time and splits read/write patterns.

  @param[in] MrcData        - Global MRC data structure
  @param[in] CmdPat         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv     - Specifies whether data pattern should be inverted per subsequence
  @param[in] Pattern        - Array of 64-bit Data Pattern for the test
  @param[in] PatternDepth   - Length of PatternQW in number of UIs
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] UiShl          - Bit-shift value per UI
  @param[in] NumCL          - Number of cachelines to use in WDB
  @param[in] Direction      - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DOWN
  @param[in] Status         - mrcSuccess / mrcFail
  @param[in] TotalTime      - Accumulated time over all AMT runs for the current data pattern
  @param[in] PatternNumber  - Which pattern in the MATS8 sequence is this test
  @param[in] TestType       - Memory test type that is currently being run

  @retval None
**/
VOID
MrcRunPprDetection (
  IN MrcParameters*             const MrcData,
  IN UINT8                      CmdPat,
  IN UINT8                      SeqDataInv[2], // MT_MAX_SUBSEQ = 2
  IN UINT64                     Pattern[],
  IN UINT8                      PatternDepth,
  IN BOOLEAN                    IsUseInvtPat,
  IN UINT8                      UiShl, // Pattern Rotation per UI
  IN UINT8                      NumCL,
  IN UINT8                      Direction,
  IN MrcStatus                  *Status,
  IN UINT32                     *TotalTime,
  IN UINT8                      PatternNumber,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
  )
{
  const MRC_FUNCTION   *MrcCall;
  UINT64               StartTime;
  UINT32               ElapsedTime;

  MrcCall = (&MrcData->Inputs)->Call.Func;

  if (*Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Pattern #%d Begin\n", PatternNumber);
    StartTime = MrcCall->MrcGetCpuTime();
    if (CmdPat == PatRdWr) {
      // Ensure all reads are done before any writes occur
      // Need to split Rd/Wr at this level to ensure SetupIOTestRetention configures correctly for PatRd and PatWr tests
      *Status = MrcAdvancedMemTest(MrcData, PatRd, SeqDataInv, Pattern, PatternDepth, IsUseInvtPat, UiShl, NumCL, Direction, TestType);
      if (*Status != mrcFail) {
        *Status = MrcAdvancedMemTest(MrcData, PatWr, SeqDataInv, Pattern, PatternDepth, IsUseInvtPat ^ SeqDataInv[1], UiShl, NumCL, Direction, TestType);  // Invert write cmd if SubSequence 1 was set to be inverted
      }
    } else {
      *Status = MrcAdvancedMemTest(MrcData, CmdPat, SeqDataInv, Pattern, PatternDepth, IsUseInvtPat, UiShl, NumCL, Direction, TestType);
    }
    ElapsedTime = (UINT32)(MrcCall->MrcGetCpuTime() - StartTime);
    *TotalTime += ElapsedTime;
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Pattern #%d End, ElapsedTime = %dms\n\n", PatternNumber, ElapsedTime);
  }
}

/**
  Pause refreshes during Data Retention memory test

  @param[in] MrcData - Global MRC data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
AdvMemTestPauseDataRet (
  IN MrcParameters* const MrcData
)
{
  UINT32 DelayTimeNs;
  INT64  GetSetDis;
  INT64  GetSetEn;

  GetSetDis = 0;
  GetSetEn = 1;

  // If CTE, shorten wait time
  DelayTimeNs = 32000000;  // 32ms is 1x refresh interval for DDR5 and LP5

  // Ensure all pages closed and rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  // Disable refresh commands
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetDis);

  // Pause for given test delay
  MrcWait (MrcData, DelayTimeNs);

  // Enable refresh commands
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetEn);

  // Ensure all rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "AdvMemTestPauseDataRet = %d ms\n", DelayTimeNs / 1000000);

  return mrcSuccess;
} // AdvMemTestPauseDataRet

/**
  Runs Advanced Memory Tests based on the MATS + algorithm

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] InvertedPassEn - Whether to run MATS+ patterns 4-6, which are the same as patterns 1-3 but with inverted data
  @param[in] TotalTestTime  - Accumulated time over all AMT runs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMATSN (
  IN MrcParameters  *const MrcData,
  MRC_BG_BANK_PAIR  BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  BOOLEAN           InvertedPassEn,
  UINT32            *TotalTestTime
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  MrcStatus            Status;
  UINT8                Test;
  BOOLEAN              SeqDataInv[2];
  UINT8                UiShl;
  UINT8                NumCL;
  UINT8                PatternDepth = 1;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Value;
  UINT64               BasePattern;
  UINT64               Pattern[8];
  UINT64               InvPattern[8];
  UINT32               TotalTime;
  UINT32               TotalTestTimeMATS8 = 0;
  UINT32               Controller;
  UINT32               Channel;

  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestMATS8\n");

  /**
  8-bit Pattern, where a bitshift of 1 is applied on each UI
  0101010101010101h
  0202020202020202h
  0404040404040404h
  0808080808080808h
  1010101010101010h
  2020202020202020h
  4040404040404040h
  8080808080808080h
  0101010101010101h
  **/
  Value = 0x0101010101010101ULL;
  BasePattern = Value;
  SeqDataInv[0] = 0; // Indicates if CPGC should invert Data pattern for SubSequence 0
  SeqDataInv[1] = 1; // Indicates if CPGC should invert Data pattern for SubSequence 1 (only applicable during PatRdWr tests, with 2 SubSequence commands)
  Controller = Outputs->FirstPopController;
  Channel = Outputs->Controller[Controller].FirstPopCh;
  for (Test = 0; Test < PPR_TEST_DETECTION_LOOPS; Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);

    Pattern[0] = MrcCall->MrcLeftShift64 (BasePattern, Test);
    InvPattern[0] = ~(Pattern[0]);
    UiShl = 1;  // Indicate a bit-shift of 1 for all UIs of the Data Pattern
    RepairDone = 1;
    RetryCount = 0;
    NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      TotalTime = 0;
      //1. Write sliding data pattern to all of memory
      MrcRunPprDetection(MrcData, PatWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 1, AdvMtWcMats8);

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        // Rank 0, Controller 0, Ch 0, Bank 5, Row 1, TestSize 1, ErrInjMask16 0x000F
        if (Test == 0 && RetryCount == 1) {
          InjectMemtestError(MrcData, 0, Controller, Channel, 5, 1, 1, Pattern, PatternDepth, 0x0F, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtWcMats8);
          InjectMemtestError(MrcData, 0, Controller, Channel, 4, 7, 1, Pattern, PatternDepth, 0x0C, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtWcMats8);
          InjectMemtestError(MrcData, 0, Controller, Channel, 27, 0xE, 1, Pattern, PatternDepth, 0x01, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtWcMats8);
          InjectMemtestError(MrcData, 0, Controller, Channel, 22, 0xE, 1, Pattern, PatternDepth, 0x02, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtWcMats8);
        }
      }

      //2. Read and compare data pattern
      //   Write inverse pattern
      if (InvertedPassEn == TRUE) {
        MrcRunPprDetection(MrcData, PatRdWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 2, AdvMtWcMats8);
      } else {
        // Modified step: Use down direction to pressure tWR on upper BG address
        MrcRunPprDetection(MrcData, PatRdWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_DOWN, &Status, &TotalTime, 2, AdvMtWcMats8);
      }

      //3. Read and compare inverse pattern
      //   Write original pattern
      if (InvertedPassEn == TRUE) {
        MrcRunPprDetection(MrcData, PatRdWr, SeqDataInv, InvPattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_DOWN, &Status, &TotalTime, 3, AdvMtWcMats8);
      } else {
        // Modified step: Just read and compare the inverse pattern without writing the original pattern
        MrcRunPprDetection(MrcData, PatRd, SeqDataInv, InvPattern, PatternDepth, TRUE, UiShl, NumCL, MT_ADDR_DIR_DOWN, &Status, &TotalTime, 3, AdvMtWcMats8);
      }

      // Make another MATS+ pass with inverted data
      if (InvertedPassEn == TRUE) {
        //4. Write inverse pattern to all of memory
        MrcRunPprDetection(MrcData, PatWr, SeqDataInv, InvPattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 4, AdvMtWcMats8);

        //5. Read and compare inverse pattern
        //   Write original pattern
        MrcRunPprDetection(MrcData, PatRdWr, SeqDataInv, InvPattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 5, AdvMtWcMats8);

        //6. Read and compare original pattern
        //   Write inverse pattern
        MrcRunPprDetection(MrcData, PatRdWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_DOWN, &Status, &TotalTime, 6, AdvMtWcMats8);
      } // InvertedPassEn

      TotalTestTimeMATS8 += TotalTime;
      if (Status == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
  }
#endif
        PprDispositionFailRange(MrcData, BankMapping, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "MemTestMATS8: Test #%d Failed!\n\n", Test);
        break;
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "End Test #%d, TotalTime Elapsed = %dms\n", Test, TotalTime);
    } // while
    if (Status != mrcSuccess) {
      break;
    }
  } // Test

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All MemTestMATS8 Tests Completed! TotalTime Elapsed for all MemTestMATS8 Tests: %dms\n", TotalTestTimeMATS8);
  }
#if POISON_ROW_FAIL_LIST
  AmtPrintTotalRowFailures(MrcData);
#endif

  *TotalTestTime += TotalTestTimeMATS8;
  return Status;
}

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] TotalTestTime  - Accumulated time over all AMT runs

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestDataRetention (
  IN MrcParameters  *const MrcData,
  MRC_BG_BANK_PAIR  BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  UINT32            *TotalTestTime
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  MrcStatus            Status;
  UINT8                Test;
  BOOLEAN              SeqDataInv[2];
  UINT8                UiShl;
  UINT8                NumCL;
  UINT8                PatternDepth = 8;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[8];
  UINT32               TotalTime;
  UINT32               TotalTestTimeDataRet = 0;
  UINT32               Controller;
  UINT32               Channel;

  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestDataRetention\n");
  for (Test = 0; Test < 2; Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);
    switch (Test) {
      case 0:
        // Copy cacheline1 into Pattern
        MrcCall->MrcCopyMem((UINT8 *) Pattern, (UINT8 *) AmtCacheLine1, sizeof (AmtCacheLine1));
        break;
      case 1:
        // Copy cacheline2 into Pattern
        MrcCall->MrcCopyMem((UINT8 *) Pattern, (UINT8 *) AmtCacheLine2, sizeof (AmtCacheLine2));
        break;
    }

    UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
    NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
    SeqDataInv[0] = 0;
    SeqDataInv[1] = 0;

    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      TotalTime = 0;
      if (Status == mrcSuccess) {
        //1. Write data pattern to all of memory
        MrcRunPprDetection(MrcData, PatWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 1, AdvMtDataRet);
      }
      if (Status == mrcSuccess) {
        Status = AdvMemTestPauseDataRet (MrcData);
      }

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        // Rank 0, Controller 0, Ch 0, Bank 0, Row 1, TestSize 1, ErrInjMask16 0x000F
        if (Test == 0 && RetryCount == 1) {
          //InjectMemtestError(MrcData, 0, Controller, Channel, 0, 1, 1, Pattern, PatternDepth, 0x0F, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl);
          InjectMemtestError(MrcData, 0, Controller, Channel, 5, 1, 1, Pattern, PatternDepth, 0x0F, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtDataRet);
          InjectMemtestError(MrcData, 0, Controller, Channel, 4, 4, 1, Pattern, PatternDepth, 0x0C, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtDataRet);
          InjectMemtestError(MrcData, 0, Controller, Channel, 27, 0x7, 1, Pattern, PatternDepth, 0x01, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtDataRet);
          InjectMemtestError(MrcData, 0, Controller, Channel, 22, 0x7, 1, Pattern, PatternDepth, 0x02, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, AdvMtDataRet);
        }
      }

      if (Status == mrcSuccess) {
        //2. Read and compare data pattern
        MrcRunPprDetection(MrcData, PatRd, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 2, AdvMtDataRet);
      }

      TotalTestTimeDataRet += TotalTime;
      // Verify that the last test was successful
      if (Status == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, BankMapping, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "MemTestDataRetention: Test #%d Failed!\n\n", Test);
        break;
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "End Test #%d, TotalTime Elapsed = %dms\n", Test, TotalTime);
    } // while
    if (Status != mrcSuccess) {
      break;
    }
  } // Test

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All MemTestDataRetention Tests Completed! TotalTime Elapsed for all MemTestDataRetention Tests: %dms\n", TotalTestTimeDataRet);
  }

  *TotalTestTime += TotalTestTimeDataRet;

  return Status;
} // MemTestDataRetention

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] BankMapping    - Array containing L2P bank mapping data for all CPGC_SEQ_BANK_L2P_MAPPING registers
  @param[in] TotalTestTime  - Accumulated time over all AMT runs
  @param[in] TestType       - Memory test type that is currently being run

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMarch (
  IN MrcParameters              *const MrcData,
  MRC_BG_BANK_PAIR              BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS],
  UINT32                        *TotalTestTime,
  IN MRC_ADVANCED_MEM_TEST_TYPE TestType
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  MrcStatus            Status;
  BOOLEAN              SeqDataInv[2];
  UINT8                UiShl;
  UINT8                NumCL;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[16];
  UINT8                PatternDepth;
  UINT32               TotalTime;
  UINT32               TotalTestTimeMarch = 0;
  const CHAR8          *TestName;
  UINT32               Controller;
  UINT32               Channel;
  UINT8                PatBufShift;


  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  // Copy cacheline1 into Pattern
  switch (TestType) {
    case AdvMtXMarch:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));
      PatternDepth = 16;
      TestName = "MemTestXMarch";
      break;
    case AdvMtXMarchG:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchGPattern, sizeof(AmtXMarchGPattern));
      PatternDepth = 8;
      TestName = "MemTestXMarchG";
      break;
    case AdvMtYMarchShort:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));  // Y March Short pattern is the same as X March
      PatternDepth = 16;
      TestName = "MemTestYMarchShort";
      break;
    case AdvMtYMarchLong:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));  // Y March Long pattern is the same as X March
      PatternDepth = 16;
      TestName = "MemTestYMarchLong";
      break;
    default:
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MemTestMarch: Unsupported test type %d\n", TestType);
      return mrcFail;
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start %s\n", TestName);

  UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
  NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
  SeqDataInv[0] = 0;
  SeqDataInv[1] = 1;
  for (PatBufShift = 0; (PatBufShift < 4) && (Status == mrcSuccess); PatBufShift++) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PatBufShift=%d\n", PatBufShift);
    Outputs->PprPatBufShift = PatBufShift << 3; // Shift by 8 * PatBufShift
    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      TotalTime = 0;
      if (Status == mrcSuccess) {
        //1. Execute write data pattern over all of memory
        MrcRunPprDetection (MrcData, PatWr, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 1, TestType);
      }
      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        if (RetryCount == 1) {
          //InjectMemtestError(MrcData, 0, Controller, Channel, 0, 1, 1, Pattern, PatternDepth, 0x0F, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl);
          InjectMemtestError(MrcData, 0, Controller, Channel, 5, 1, 1, Pattern, PatternDepth, 0x0F, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, TestType);
          InjectMemtestError(MrcData, 0, Controller, Channel, 4, 4, 1, Pattern, PatternDepth, 0x0C, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, TestType);
          InjectMemtestError(MrcData, 0, Controller, Channel, 27, 0x7, 1, Pattern, PatternDepth, 0x01, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, TestType);
          InjectMemtestError(MrcData, 0, Controller, Channel, 22, 0x7, 1, Pattern, PatternDepth, 0x02, MT_ADDR_DIR_UP, SeqDataInv, FALSE, UiShl, TestType);
        }
      }

      if (Status == mrcSuccess) {
        //2. Execute read data pattern over all of memory
        MrcRunPprDetection (MrcData, PatRd, SeqDataInv, Pattern, PatternDepth, FALSE, UiShl, NumCL, MT_ADDR_DIR_UP, &Status, &TotalTime, 2, TestType);
      }

      TotalTestTimeMarch += TotalTime;
      // Verify that the last test was successful
      if (Status == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, BankMapping, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "%s: Test Failed!\n\n", TestName);
        break;
      }
    } // while
    if (TestType == AdvMtXMarchG) {
      break;  // XMarchG has one data instruction and does not need to shift pattern
    } else if (PatBufShift < 3) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Rerun with next PatBufShift=%d (different data pattern)\n", PatBufShift + 1);
    }
  } // for

  Outputs->PprPatBufShift = 0;

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All %s Tests Completed! TotalTime Elapsed: %dms\n", TestName, TotalTestTimeMarch);
    *TotalTestTime += TotalTestTimeMarch;
  }
  return Status;
} // MemTestMarch

/**
  Top-level PPR function that determines which memory tests should be run.
  Executes Post Package Repair flow to repair failures found during memory test.

  @param[in]  MrcData      - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess if the entire memory test/PPR flow executes, otherwise mrcFail if it is cut short
**/
MrcStatus
MrcPostPackageRepairEnable (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  MrcInput            *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcStatus           Status;
  UINT8               Test;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              IpChannel;
  UINT32              BankIndex;
  INT64               GetSetDis;
  BOOLEAN             InvertedPassEn;
  UINT32              TotalTestTime = 0;
  INT64               GetSetVal;
  INT64               EnableSrSave;
  MRC_BG_BANK_PAIR    BankMapping[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS];
  UINT32              SavedCpgc20Credits[MAX_CONTROLLER];
  BOOLEAN             IsUlxUlt;
  BOOLEAN             SaveCpgcGlobalStart;
  UINT8               BG;
  UINT8               FirstChannel;
  MrcChannelOut       *ChannelOut;
  UINT8               Rank;

  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  ExtInputs       = Inputs->ExtInputs.Ptr;
  MrcCall         = Inputs->Call.Func;
  GetSetDis       = 0;
  InvertedPassEn  = TRUE;
  IsUlxUlt        = Inputs->IsDdrIoUlxUlt;
  SaveCpgcGlobalStart = MrcData->Save.Data.CpgcGlobalStart;
  MrcData->Save.Data.CpgcGlobalStart = FALSE;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
      Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, FALSE);
    }
  }

  /*
   *  Run PPR at:
   *  1) Coldboot
   *  2) Fastboot and users config PprRunAtFastboot to TRUE
   */
  if ((Inputs->BootMode == bmFast) && (ExtInputs->PprRunAtFastboot == 0)) {
    return mrcSuccess;
  }

  MrcModifyRdRdTimings (MrcData, TRUE);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  Outputs->PprRunningState = PPR_IS_RUNNING;

  Outputs->PprNumDetectedErrors = 0;
  Outputs->PprRepairFails = 0;
  Outputs->PprForceRepairStatus = mrcSuccess;

  MrcCall->MrcSetMem ((UINT8 *) BankMapping, sizeof(BankMapping), 0);

  // Store all Bank L2P mapping data so it can be restored later
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      Status = Cpgc20GetSetBankSequence (MrcData, Controller, Channel, &BankMapping[Controller][Channel][0], MAX_BANKS, MRC_GET);
      if (Status != mrcSuccess) {
        break;
      }
    }

    if (Status != mrcSuccess) {
      break;
    }
  }
  // If function call fails for some reason, set BankMapping to default values
  if (Status != mrcSuccess) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        for (BankIndex = 0; BankIndex < MAX_BANKS; BankIndex++) {
          MrcConvertCpgcBanktoBankAddress(MrcData, BankIndex, &BankMapping[Controller][Channel][BankIndex].Bank, &BankMapping[Controller][Channel][BankIndex].BankGroup);
        }
      }
    }
  }

  Status = mrcSuccess;


  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprTestType=0x%02x\n", ExtInputs->PprTestType);
  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprRepairType=0x%02x\n", ExtInputs->PprRepairType);

  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetVal);

  MrcGetSetMc (MrcData, Outputs->FirstPopController, GsmMccOppSrefEnable, ReadNoCache, &EnableSrSave);  // Save state of Self Refresh
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccOppSrefEnable, WriteNoCache, &GetSetDis); // Disable Self Refresh

  MrcCall->MrcSetMemDword (SavedCpgc20Credits, ARRAY_COUNT (SavedCpgc20Credits), RDCPL_CREDITS_SCRUB);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if ((MrcControllerExist (MrcData, Controller)) && IsUlxUlt) {
       MrcCpgcModifyReadCredits (MrcData, Controller, TRUE, &SavedCpgc20Credits[Controller]);
    }
  }

  if ((ExtInputs->PprRepairPhysicalAddrLow != 0) || (ExtInputs->PprRepairPhysicalAddrHigh != 0)) {
    Status = mrcFail;
    if (ExtInputs->PprRepairController < MAX_CONTROLLER) {
      // Assume BIOS menu passes correct memory address
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Repair Physical address: 0x%x%08x\n", ExtInputs->PprRepairPhysicalAddrHigh, ExtInputs->PprRepairPhysicalAddrLow);
      Status = MrcPostPackageRepair(MrcData, ExtInputs->PprRepairController, ExtInputs->PprRepairChannel, (UINT16)(MAX_RANK_IN_DIMM * ExtInputs->PprRepairDimm + ExtInputs->PprRepairRank), ExtInputs->PprRepairBankGroup, ExtInputs->PprRepairBank, ExtInputs->PprRepairRow, (1 << Outputs->SdramCount) - 1);
    }
    if (Status != mrcSuccess) {
      Outputs->PprRepairFails++;
    }
    goto Done;
  }

  // If no memory tests are enabled (first 6 bits), print a warning message
  if ((ExtInputs->PprTestType & 0x3F) == 0) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MrcPostPackageRepairEnable: Warning - No memory tests are enabled, detection and repair flow will not occur\n");
  }

  Status = mrcSuccess;
  // Run all memory tests
  for (Test = 0; Test < 6; Test++) {
    if (ExtInputs->PprTestType & (1 << Test)) {
      switch (Test) {
        case PPR_MEMTEST_WCMATS8_BIT:
          Status |= MemTestMATSN(MrcData, BankMapping, InvertedPassEn, &TotalTestTime);
          break;
        case PPR_MEMTEST_DATA_RETENTION_BIT:
          Status |= MemTestDataRetention(MrcData, BankMapping, &TotalTestTime);
          break;
        case PPR_MEMTEST_XMARCH_BIT:
        case PPR_MEMTEST_XMARCHG_BIT:
        case PPR_MEMTEST_YMARCHSHORT_BIT:
        case PPR_MEMTEST_YMARCHLONG_BIT:
          Status |= MemTestMarch(MrcData, BankMapping, &TotalTestTime, (MRC_ADVANCED_MEM_TEST_TYPE) Test);
          break;
      }
    }
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MrcPostPackageRepairEnable: Last memory test failed, skipping other tests\n");
      break;
    }
    PprCleanup (MrcData, Outputs->McChBitMask, BankMapping);  // Clean up programming between different test setups
  }

  if (ExtInputs->PprForceRepair) {  // Force repair sequence
    BG      = 0;
    Status  = mrcSuccess;
    for (Test = 0; Test < 90; Test++) {
      PprCleanup (MrcData, Outputs->McChBitMask, BankMapping);
      if (Outputs->IsLpddr && (MrcGetBankBgOrg(MrcData, Outputs->Frequency) != MrcLp5BgMode)) {
        BG = 0;
      } else {
        BG = 3;
      }
      FirstChannel = Outputs->Controller[Outputs->FirstPopController].FirstPopCh;
      ChannelOut = &Outputs->Controller[Outputs->FirstPopController].Channel[FirstChannel];
      // Get first rank
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if ((1 << Rank) & ChannelOut->ValidRankBitMask) {
          break;
        }
      } // Rank
#ifdef MRC_DEBUG_PRINT
      Status |= MrcPostPackageRepair(MrcData, Outputs->FirstPopController, FirstChannel, Rank, BG, 2, 0xAB, 0xF);
      MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "Ppr Status = %d\n", Status);
#else
      MrcPostPackageRepair(MrcData, Outputs->FirstPopController, FirstChannel, Rank, BG, 2, 0xAB, 0xF);
#endif
    }
    Outputs->PprForceRepairStatus |= Status;
    Status = mrcSuccess;
  }

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All Tests Completed! TotalTime Elapsed for all Tests: %dms\n", TotalTestTime);
  }

Done:

  PprCleanup (MrcData, Outputs->McChBitMask, BankMapping);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if ((MrcControllerExist(MrcData, Controller)) && IsUlxUlt) {
      MrcCpgcModifyReadCredits(MrcData, Controller, FALSE, &SavedCpgc20Credits[Controller]);
    }
  }
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetDis); // Cleanup, disable refreshes
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccOppSrefEnable, WriteNoCache, &EnableSrSave); // Restore Self Refresh

  // Restore CpgcGlobalStart
  MrcData->Save.Data.CpgcGlobalStart = SaveCpgcGlobalStart;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
      Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, SaveCpgcGlobalStart);
    }
  }

  MrcModifyRdRdTimings (MrcData, FALSE);

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  // Get status of PPR resources for MHI report
  GetAllPprResources (MrcData);


  Outputs->PprRunningState = PPR_IS_DONE;

  /*
   * Always return mrcSuccess:
   * 1) BIOS menu will show PPR stat;
   * 2) Avoid brick
   */
  return mrcSuccess;
}
