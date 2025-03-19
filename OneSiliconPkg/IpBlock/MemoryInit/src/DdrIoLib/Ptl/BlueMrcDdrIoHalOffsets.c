/** @file
  This file contains functions to get DDR IO Data Offsets
  used memory training.

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

#include "MrcDdrIoApi.h" // for function prototypes and interface definitions
#include "CMcAddress.h"
#include "MrcHalRegisterAccess.h"
#include "MrcCommon.h"
#include "MrcDdrIoDefines.h"
#include "MrcDdrIoHalOffsetsPrivate.h"

/*
  This function returns the offset to access specific Clk of Channel/Rank of DCCFSMCONTROL.

  @params[in]  Channel - 0-based index of Channel to access.
  @params[in]  Rank    - 0-based index of Rank to access.
  @params[in]  IsDdr5  - If system is DDR5 or not

  @retval CR offset
*/
UINT32
ClkDccFsmControl (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  BOOLEAN const IsDdr5
  )
{
  UINT32 Offset;

  Offset = MRC_UINT32_MAX;

  if (!IsDdr5 || (IsDdr5 && Rank < MAX_RANK_IN_DIMM)) {
    // LPDDR5 or DDR5 w/ Rank 0 or 1
    Offset = OFFSET_CALC_CH (CH0CCC_CR_DCCFSMCONTROL_REG, CH1CCC_CR_DCCFSMCONTROL_REG, Channel);
  }

  return Offset;
}

static
UINT32
WriteConfigOffset (
  IN  UINT32  const Channel
  )
{
  UINT32 Offset;

  Offset  = MCMISCS_WRITECFGCH0_REG;
  Offset += (MCMISCS_WRITECFGCH1_REG - MCMISCS_WRITECFGCH0_REG) * Channel;

  return Offset;
}

/**
  Function used to get the CR Offset for DATATCOCONTROL0/1.

  @param[in]  Channel - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe  - DQS data group within the channel (0-based).
  @param[in]  Bit     - Bit within the strobe (0-based).

  @retval CR Offset.
**/
UINT32
GetDdrIoDataTcoControl(
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  IN  UINT32  const   Bit
)
{
  UINT32 Offset;

  Offset = DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG;
  Offset += (DATA0CH1_CR_DDRCRDATATCOCONTROL0_REG - DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG) * Channel +
    (DATA1CH0_CR_DDRCRDATATCOCONTROL0_REG - DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG) * Strobe;

  if (Bit > 4) {
    Offset += (DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG - DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG);
  }

  return Offset;
}

/**
  Function used to get the CR Offset for Data Read Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Controller   - Memory controller in the processor socket (0-based).
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoDataReadOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Bit,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  MrcOutput *Outputs;
  BOOLEAN   IsLpddr;
  UINT32    IpChannel;
  UINT32    Offset;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;
  Offset  = MRC_UINT32_MAX;

  switch (Group) {
    case RxVref:
      Offset = DataControl5Offset (Channel, Strobe);
      break;

    case RxDqVref:
      Offset = ((Bit >= 0) && (Bit <= 2)) ? DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG :
               ((Bit >= 3) && (Bit <= 5)) ? DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG :
                                            DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG;
      Offset += (((DATA0CH1_CR_DDRCRDATAVREFPERBIT012_REG - DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG) * Channel) +
                 ((DATA1CH0_CR_DDRCRDATAVREFPERBIT012_REG - DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG) * Strobe));
      break;

    case RecEnDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
      Offset = DATA0CH0_CR_RXCONTROL0RANK0_REG +
        ((DATA0CH1_CR_RXCONTROL0RANK0_REG - DATA0CH0_CR_RXCONTROL0RANK0_REG) * Channel) +
        ((DATA0CH0_CR_RXCONTROL0RANK1_REG - DATA0CH0_CR_RXCONTROL0RANK0_REG) * Rank) +
        ((DATA1CH0_CR_RXCONTROL0RANK0_REG - DATA0CH0_CR_RXCONTROL0RANK0_REG) * Strobe);
      break;

    case RxDqsBitDelay:
    case RxDqsBitOffset:
    case RxVocRise:
      Offset = DataDqRankXLaneYOffset (Channel, Rank, Strobe, Bit, VolatileMask);
      break;

    case RoundTripDelay:
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_REG, MC1_CH0_CR_SC_ROUNDTRIP_LATENCY_REG, Controller, MC0_CH1_CR_SC_ROUNDTRIP_LATENCY_REG, IpChannel);
      break;

    case SenseAmpDelay:
    case SenseAmpDuration:
    case McOdtDelay:
    case McOdtDuration:
      Offset = DataControl1Offset (Channel, Strobe);
      break;

    default:
      break;
  }

  return Offset;
}

/**
  Function used to get the CR Offset for registers listed under PLL partitions.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoPllOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32  Offset;
  Offset    = MRC_UINT32_MAX;

  switch (Group) {
    case GsmDccPHClkPhaseEn:
    case GsmDccPHClkDutyCycleEn:
      Offset = DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_REG;
      break;

    default:
      break;
  }

  return Offset;
}

/**
  Function used to get the CR Offset for registers listed under Comp partition.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[in]  FreqIndex    - Workpoint Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoCompOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  return MRC_UINT32_MAX;
}

/**
  Function used to get the CR Offset for registers listed under PGx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoPgOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Lane,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32  Offset;

  Offset    = MRC_UINT32_MAX;
  if (Lane < MRC_PG_NUM) {
    switch (Group) {
      case GsmDccPHClkPhaseEn:
      case GsmDccPHClkDutyCycleEn:
        Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DCCCONTROL_REG, DDRDATA_SHARED1_CR_DCCCONTROL_REG, Lane);
        break;

      default:
        break;
    }
  }
  return Offset;
}

/**
  Function used to get the CR Offset for registers listed under DATA_SHAREDx / DATA_SBMEMx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[in]  Lane         - Uses the Lane parameter from GetSet to select which PI Mixer code to access.
  @param[in]  FreqIndex    - Workpoint Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoDataOffsets (
  IN  MrcParameters* MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT* const  VolatileMask
)
{
  UINT32  Offset;

  Offset = MRC_UINT32_MAX;
  UINT32 MaxLane;

  MaxLane = (MrcData->Inputs.IsDdrIoUlxUlt) ? MRC_DATA_MOBILE_NUM : MRC_DATA_DT_NUM;

  if (Lane < MaxLane) {
    switch (Group) {
      case GsmDccPHClkPhaseEn:
      case GsmDccPHClkDutyCycleEn:
        Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DCCCONTROL_REG, DDRDATA_SHARED1_CR_DCCCONTROL_REG, Lane);
        break;

      default:
        break;
    }
  }

  return Offset;
}
/**
  Function used to get the CR Offset for registers listed under CCC_SHAREDx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[in]  Lane         - Uses the Lane parameter from GetSet to select which PI Mixer code to access.
  @param[in]  FreqIndex    - Access Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoCccOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32  Offset;
  UINT32  MaxLane;

  Offset = MRC_UINT32_MAX;

  MaxLane = (MrcData->Inputs.IsDdrIoUlxUlt) ? MRC_CCC_SHARED_MOBILE_NUM : MRC_CCC_SHARED_DT_NUM;

  if (Lane < MaxLane) {
    switch (Group) {
      case GsmDccPHClkPhaseEn:
      case GsmDccPHClkDutyCycleEn:
        Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DCCCONTROL_REG, DDRCCC_SHARED1_CR_DCCCONTROL_REG, Lane);
        break;

      default:
        break;
    }
  }

  return Offset;
}

/**
  Function used to get the CR Offset for Write Data Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoDataWriteOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Bit,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;

  Offset = MRC_UINT32_MAX;

  switch (Group) {
    case TxDqsDelay:
    case TxDqDelay:
      Offset = DATA0CH0_CR_TXCONTROL0RANK0_REG +
        ((DATA0CH1_CR_TXCONTROL0RANK0_REG - DATA0CH0_CR_TXCONTROL0RANK0_REG) * Channel) +
        ((DATA0CH0_CR_TXCONTROL0RANK1_REG - DATA0CH0_CR_TXCONTROL0RANK0_REG) * Rank) +
        ((DATA1CH0_CR_TXCONTROL0RANK0_REG - DATA0CH0_CR_TXCONTROL0RANK0_REG) * Strobe);
      break;

    case TxDqBitDelay:
      Offset = DataDqRankXLaneYOffset (Channel, Rank, Strobe, Bit, VolatileMask);
      break;

    case TxDqFifoWrEnTcwlDelay:
    case TxDqFifoRdEnTcwlDelay:
      Offset = WriteConfigOffset (Channel);
      break;

    case TxRonUp:
    case TxRonDn:
      Offset = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_REG;
      VolatileMask->Data = DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_VOLATILE_BITFIELDS_MSK;
      break;

    default:
      break;
    }

  return Offset;
}

/**
  Function used to get the CR Offset for DDRDATA_SHARED groups.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Socket       - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoDataSharedOffsets (
  IN MrcParameters          *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Bit,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;
  UINT32 ChEvenFloor;

  Offset = MRC_UINT32_MAX;
  ChEvenFloor = Channel & ~MRC_BIT0;

  switch (Group) {
    case WckGrpPi:
      //   Controller   Channel       Byte0            Byte1
      // |            |    0     DATA_SHARED0CH0  DATA_SHARED1CH0
      // |     0      |    1     DATA_SHARED0CH1  DATA_SHARED1CH1
      // |            |    2     DATA_SHARED2CH0  DATA_SHARED3CH0
      // |            |    3     DATA_SHARED2CH1  DATA_SHARED3CH1
      // |____________|__________________________________________
      // |            |
      // |            |    4     DATA_SHARED4CH0  DATA_SHARED5CH1
      // |     1      |    5     DATA_SHARED4CH0  DATA_SHARED5CH1
      // |            |    6     DATA_SHARED6CH0  DATA_SHARED7CH1
      // |            |    7     DATA_SHARED6CH0  DATA_SHARED7CH1
      Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRCCCPICODES_REG, DDRDATA_SHARED1_CR_DDRCRCCCPICODES_REG, ChEvenFloor + Strobe);
      VolatileMask->Data = DDRDATA_SHARED0_CR_DDRCRCCCPICODES_VOLATILE_BITFIELDS_MSK;
      break;

    default:
      break;
  }

  return Offset;
}

/**
  Function used to get the CR Offset for DDR IO Configuration settings.

  @param[in]  MrcData     - Global MRC data structure.
  @param[in]  Group       - DDRIO group being accessed.
  @param[in]  Channel     - DDR Channel Number(0-based).
  @param[in]  Rank        - Rank index in the channel. (0-based).
  @param[in]  Strobe      - Dqs data group within the rank (0-based).
  @param[in]  Lane        - Lane index to GSM_GT Group.
  @param[in]  FreqIndex   - Index supporting multiple operating frequencies.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
MrcGetDdrIoConfigOffsets (
  IN MrcParameters          *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32      Offset;
  BOOLEAN     IsDdr5;

  Offset      = MRC_UINT32_MAX;
  IsDdr5      = MrcData->Outputs.IsDdr5;

  switch (Group) {
    case GsmIocDisDataIdlClkGate:
    case GsmIocScramLp5Mode:
    case GsmIocChNotPop:
    case GsmIocDfiCmdDecoderEn:
    case GsmIocDisIosfSbClkGate:
    case GsmIocScramEn:
    case GsmIocWrRetrainOvrd:
    case GsmIocDdr52NMode:
      Offset = DDRSCRAM_CR_DDRMISCCONTROL0_REG;
      break;

    case GsmIocCs2NReset:
    case GsmIocCsGearDownEnable:
    case GsmIocCsGearDownForce:
    case GsmIocCsGearDownSrDramTrack:
      Offset = DDRSCRAM_CR_CS_GEARDOWN_REG;
      VolatileMask->Data = DDRSCRAM_CR_CS_GEARDOWN_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmIocIoReset:
    case GsmIocDccClkGateDisable:
      Offset = DDRSCRAM_CR_DDRMISCCONTROL1_REG;
      VolatileMask->Data = DDRSCRAM_CR_DDRMISCCONTROL1_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMiscPmMessage:
    case GsmMiscPmMessageRunBusy:
    case GsmMiscPmMessageWpSel:
      Offset = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG;
      VolatileMask->Data = DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmIocRankOverrideEn:
    case GsmIocRankOverrideVal:
      Offset = DataTrainFeedbackOffset (Channel, Strobe, VolatileMask);
      break;

    case GsmIocForceRxAmpOn:
    case GsmIocDisRxSalOffWhenDrvEnHi:
    case GsmIocForceOdtOn:
    case GsmIocTxPiPwrDnDis:
    case GsmIocInternalClocksOn:
    case GsmIocDataDqsOdtParkMode:
      Offset = DataControl0Offset (Channel, Strobe, VolatileMask);
      break;

    case GsmIocDisableTxDqs:
      Offset = DataControl1Offset (Channel, Strobe);
      break;

    case GsmIocDqsMaskPulseCnt:
    case GsmIocDqsPulseCnt:
    case GsmIocDqOverrideData:
    case GsmIocDqOverrideEn:
      Offset = CmdBusTrainOffset (Channel, Strobe);
      break;

    case GsmIocDataInvertNibble:
      Offset = DataControl5Offset (Channel, Strobe);
      break;

    case GsmIocEnableLpMode4:
      Offset = DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_REG;
      break;

    case GsmDdrReset:
      Offset = DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL4_REG;
      VolatileMask->Data = DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL4_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmIocAuxClkOffAuxClk:
      Offset = AuxClkOffOffset (FreqIndex, VolatileMask);
      break;

    case GsmIocQClkCalOff:
      Offset = QclkCalOffOffset (FreqIndex, VolatileMask);
      break;

    case GsmIocPHClkOffsetFastCal:
    case GsmIocAuxOffsetFastCal:
      Offset = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_REG;
      VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmDataDccRankEn:
      Offset = DataDccFsmCtlOffset (Channel, Strobe);
      break;

    case GsmClkDccRankEn:
      Offset = ClkDccFsmControl (Channel, Rank, IsDdr5);
      break;

    case GsmCompVccDdqTarget:
      Offset = OFFSET_CALC_INDEX (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP1_REG, FreqIndex);
      VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmInitCompleteOvrd:
    case GsmInitCompleteOvrdVal:
      Offset = DDRPHY_MISC_SAUG_CR_PHYPMOVRD_REG;
      break;

    case GsmPChannelEn:
    case GsmSkipRestoreCR:
    case GsmSkipRetentionCR:
      Offset = DDRPHY_MISC_PMA_SAUG_CR_PMCFG_REG;
      break;

    case GsmSaxgEnable:
    case GsmSaxgReady:
      Offset = DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG;
      VolatileMask->Data = DDRPHY_MISC_SAUG_CR_PG_CTRL3_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmSaxgPwrGood:
      Offset = DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_REG;
      VolatileMask->Data = DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmWorkPointAuxClk:
    case GsmWorkPointRatio:
    case GsmWorkPointGear4:
      Offset = OFFSET_CALC_INDEX (DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG, DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT1_REG, FreqIndex);
      break;

    case GsmDccPHClkPhaseEn:
      Offset = GetDdrIoPartitionOffsets (MrcData, Group, Rank, Strobe, Lane, FreqIndex, VolatileMask);
      break;

    case GsmCompCtrlPeriodicComp:

    case GsmCompCtrlDVFSComp:
    case GsmCompCtrlCompDist:
    case GsmCompCtrlCompUpdate:
      Offset = MCMISCS_COMPCONTROL_REG;
      VolatileMask->Data = MCMISCS_COMPCONTROL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmTrainWCkPulse:
    case GsmTrainWCkBL:
    case GsmTrainWCkMask:
    case GsmWCkDiffLowInIdle:
      Offset = MCMISCS_DDRWCKCONTROL_REG;
      break;

    case GsmIocRetrainSwizzleCtlRetrainEn:
    case GsmIocRetrainSwizzleCtlSerialMrr:
      Offset = WrRetrainSwizzleControlOffset (Channel, Strobe);
      break;

    case GsmIocRetrainInitPiCode:
    case GsmIocRetrainRoCount:
      Offset = WrRetrainRankOffset (Channel, Rank, Strobe);
      VolatileMask->Data = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_VOLATILE_BITFIELDS_MSK;
      break;


    case GsmIocRetrainCtlInitTrain:
    case GsmIocRetrainCtlDuration:
    case GsmIocRetrainCtlResetStatus:
      Offset = WrRetrainControlStatusOffset (Channel, Strobe);
      VolatileMask->Data = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_VOLATILE_BITFIELDS_MSK;
      break;

    default:
      break;
    }

  return Offset;
}

/**
  Function used to get the CR Offset for DDR IO Command Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[in]  IsDimm1      - If is Dimm 1 or not
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoCommandOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                FreqIndex,
  IN  BOOLEAN               IsDimm1,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32      Offset;

  Offset  = MRC_UINT32_MAX;

  switch (Group) {
    case CmdGrpPi:
    case CtlGrpPi:
      Offset = OFFSET_CALC_CH (CH0CCC_CR_DDRCRCCCPICODES_REG, CH1CCC_CR_DDRCRCCCPICODES_REG, Channel);
      VolatileMask->Data = CH0CCC_CR_DDRCRCCCPICODES_VOLATILE_BITFIELDS_MSK;
      break;

    case DefDrvEnLow:
      Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_REG, DDRCCC_SHARED1_CR_DDRCRCCCPINCONTROLS_REG, Channel);
      VolatileMask->Data = DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_VOLATILE_BITFIELDS_MSK;
      break;

    default:
      break;
    }

  return Offset;
}

/**
  This function handles getting the register offset for the requested parameter.
  It will determine multicast by the parameter exceeding the MAX of the
  Socket/Channel/Rank/Strobe/Bit.

  @param[in]  MrcData     - Pointer to global data.
  @param[in]  Group       - DDRIO group to access.
  @param[in]  Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller  - Memory controller in the processor socket (0-based).
  @param[in]  Channel     - DDR Channel Number within the memory controller (0-based).
  @param[in]  Rank        - Rank number within a channel (0-based).
  @param[in]  Strobe      - Dqs data group within the rank (0-based).
  @param[in]  Lane        - Lane index to GSM_GT Group.
  @param[in]  FreqIndex   - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval CR Offset
**/
UINT32
MrcGetDdrIoRegOffset (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT  const         Group,
  IN  UINT32  const         Socket,
  IN  UINT32  const         Controller,
  IN  UINT32  const         Channel,
  IN  UINT32  const         Rank,
  IN  UINT32  const         Strobe,
  IN  UINT32  const         Lane,
  IN  UINT32  const         FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32  Offset;

  Offset    = MRC_UINT32_MAX;

  switch (Group) {
    case RecEnDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxVref:
    case RxDqVref:
    case RxDqsBitDelay:
    case RxDqsBitOffset:
    case RoundTripDelay:
    case SenseAmpDelay:
    case SenseAmpDuration:
    case McOdtDelay:
    case McOdtDuration:
    case RxVocRise:
      Offset = GetDdrIoDataReadOffsets (MrcData, Group, Controller, Channel, Rank, Strobe, Lane, VolatileMask);
      break;

    case TxRonUp:
    case TxRonDn:
    case TxDqsDelay:
    case TxDqDelay:
    case TxDqBitDelay:
    case TxDqFifoWrEnTcwlDelay:
    case TxDqFifoRdEnTcwlDelay:
      Offset = GetDdrIoDataWriteOffsets (Group, Channel, Rank, Strobe, Lane, VolatileMask);
      break;

    case TxRankMuxDelay:
    case TxR2RDqsEq:
    case TxR2RDqEq:
    case TxR2RDqsPi:
    case TxR2RDqPi:
    case RxRankMuxDelay:
    case RxR2RDqsOff:
    case RxR2RDqOff:
    case RxR2RRxPi:
    case RxR2RRcvEn:
      Offset = DATA0CH0_CR_DDRCRRANKMUXDELAY_REG +
        ((DATA0CH1_CR_DDRCRRANKMUXDELAY_REG - DATA0CH0_CR_DDRCRRANKMUXDELAY_REG) * Channel) +
        ((DATA1CH0_CR_DDRCRRANKMUXDELAY_REG - DATA0CH0_CR_DDRCRRANKMUXDELAY_REG) * Strobe);
      break;

    case WrRetrainDeltaPiCode:
    case RdRetrainDeltaPiCode:
      Offset = GetDdrIoReTrainOffsets (Group, Channel, Strobe, VolatileMask);
      break;

    case RxCompDqsDelayP:
    case RxCompDqsDelayN:
    case RxCompDqsOffset:
      Offset = DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG +
        ((DATA0CH1_CR_DDRCRCOMPDQSDELAY_REG - DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG) * Channel) +
        ((DATA1CH0_CR_DDRCRCOMPDQSDELAY_REG - DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG) * Strobe);
      VolatileMask->Data = DATA0CH0_CR_DDRCRCOMPDQSDELAY_VOLATILE_BITFIELDS_MSK;
      break;

    case RxVrefOffset:
      Offset = DATA0CH0_CR_DDRCRADC_REG +
             ((DATA0CH1_CR_DDRCRADC_REG - DATA0CH0_CR_DDRCRADC_REG) * Channel) +
             ((DATA1CH0_CR_DDRCRADC_REG - DATA0CH0_CR_DDRCRADC_REG) * Strobe);
      VolatileMask->Data = DATA0CH0_CR_DDRCRADC_VOLATILE_BITFIELDS_MSK;
      break;

    // Intentionally no break - for TC RxVrefOffset is in DataOffsetTrain
    case RecEnOffset:
    case RxDqsOffset:
    case TxDqsOffset:
    case TxDqOffset:
      Offset = GetDdrIoDataTrainOffsets (Group, Channel, Strobe);
      break;

    case WckGrpPi:
      Offset = GetDdrIoDataSharedOffsets (MrcData, Group, Socket, Channel, Rank, Strobe, Lane, FreqIndex, VolatileMask);
      break;

    case CtlGrpPi:
    case CmdGrpPi:
    case DefDrvEnLow:
      Offset = GetDdrIoCommandOffsets (Group, Channel, FreqIndex, FALSE, VolatileMask);
      break;

    case CsPerBitCcc:
      Offset = GetDdrIoCccLaneOffsets (Channel, Lane, VolatileMask);
      break;

    default:
      break;
  }

  return Offset;
}

// These functions are called in DdrioInit()
/**
  This function returns the offset to access specific Channel/Strobe of WRRETRAINRANK[0..3].

  @params[in]  Channel - 0-based index of Channel to access.
  @params[in]  Rank    - 0-based index of Rank to access.
  @params[in]  Strobe  - 0-based index of Strobe to access.

  @retval CR offset
**/
UINT32
WrRetrainRankOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe
  )
{
  UINT32 Offset;

  Offset = DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG +
         ((DATA0CH1_CR_DDRCRWRRETRAININITRANK1_REG - DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG) * Channel) +
         ((DATA0CH0_CR_DDRCRWRRETRAININITRANK0_REG - DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG) * (1 - Rank)) +
         ((DATA1CH0_CR_DDRCRWRRETRAININITRANK0_REG - DATA0CH0_CR_DDRCRWRRETRAININITRANK0_REG) * Strobe);

  return Offset;
}

/**
  This function returns the offset to access specific AuxClk Offset Calibrarion for AuxClk

  @params[in] FreqIndex    - Sagv Point
  @param[out] VolatileMask - Volatile Mask

  @retval CR Offset.
**/
UINT32
AuxClkOffOffset (
  IN  UINT32                FreqIndex,
  IN  UINT64_STRUCT  *const VolatileMask
  )
{
  UINT32 Offset;

  if (FreqIndex < 2) {
    // SAGV Index 0/1
    Offset = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_REG;
    VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_VOLATILE_BITFIELDS_MSK;
  } else {
    // SAGV Index 2/3
    Offset = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_REG;
    VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_VOLATILE_BITFIELDS_MSK;
  }

  return Offset;
}

/**
  This function returns the offset to access specific QClkCal

  @params[in] FreqIndex    - Sagv Point
  @param[out] VolatileMask - Volatile Mask

  @retval CR Offset.
**/
UINT32
QclkCalOffOffset (
  IN  UINT32                FreqIndex,
  IN  UINT64_STRUCT  *const VolatileMask
  )
{
  UINT32 Offset;

  if (FreqIndex < 2) {
    // SAGV Index 0/1
    Offset = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_REG;
    VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_VOLATILE_BITFIELDS_MSK;
  } else {
    // SAGV Index 2/3
    Offset = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_REG;
    VolatileMask->Data = DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_VOLATILE_BITFIELDS_MSK;
  }

  return Offset;
}
/**
  Function used to get the CR Offset for training offset and comp offset Groups.

  @param[in]  Group     - DDRIO group being accessed.
  @param[in]  Channel   - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe    - Dqs data group within the rank (0-based).

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataTrainOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Strobe
  )
{
  UINT32 Offset;

  Offset = MRC_UINT32_MAX;

  switch (Group) {
    case RecEnOffset:
    case RxDqsOffset:
    case RxVrefOffset:
    case TxDqsOffset:
    case TxDqOffset:
      Offset = DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG +
             ((DATA0CH1_CR_DDRCRDATAOFFSETTRAIN_REG - DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG) * Channel) +
             ((DATA1CH0_CR_DDRCRDATAOFFSETTRAIN_REG - DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG) * Strobe);
      break;

    default:
      break;
  }

  return Offset;
}