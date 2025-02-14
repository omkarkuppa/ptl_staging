/** @file
  Implements functions for handling Mode Register routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "MrcModeRegisterHandlerPrivate.h"
#include "MrcModeRegisterHandler.h"
#include "MrcMcApi.h"
#include "MrcChipApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcMemoryApi.h"
#include "MrcHalApi.h"
#include "CMrcStatsTracker.h" // for MrcStatsStartTimer()


/**
  This function creates the DDR5 RankMask needed for the Mrh FSM based on the Rank Occupancy.
  This function supports the options for multicasting

  @param[in] MrcData            - Include all MRC global data.
  @param[in] Controller         - Controller to run MRH on
  @param[in] Channel            - Channels to run MRH on
  @param[in] Rank               - The rank to work on
  @param[in] RankOccupancy      - Current Rank Occupancy

  @retval TargetRanksBitmask    - Rank Mask to use for Mrh Fsm
**/
UINT8
GetMrhFsmRankMaskDdr5 (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  UINT8                RankOccupancy
  )
{
  INT64 OneDpc;
  UINT8 TargetRanksBitmask;

  // DDR5 MRH Command Regsiter 'Rank' field definition:
  // Bit[16] DIMM 0, Rank 0
  // Bit[17] DIMM 0, Rank 1
  // Bit[18] DIMM 1, Rank 0
  // Bit[19] DIMM 1, Rank 1
  if (Rank >= MAX_RANK_IN_CHANNEL) {
    // The Rank_occupancy mask will be correct even for DDR OneDpc (Split Rank) configs
    TargetRanksBitmask = RankOccupancy;
  } else {
    // DDR5 doesn't have a channel remap for MC.
    // Remap target rank if ddr5_1dpc is enabled on this channel
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccOneDpc, ReadFromCache, &OneDpc);
    if (OneDpc == 1) { // 1dpc enabled on DIMM0
      if (Rank == 1) {
        Rank = 3;
      }
    } else if (OneDpc == 2) { // 1dpc enabled on DIMM1
      if (Rank == 2) {
        Rank = 0;
      }
    }
    TargetRanksBitmask = (MRC_BIT0 << Rank);
  }
  return TargetRanksBitmask;
}

/**
  This function creates the LP5 RankMask needed for the Mrh FSM based on the Rank Occupancy.
  This function supports the options for multicasting

  @param[in] MrcData       - Include all MRC global data.
  @param[in] Channel       - The channel to work on
  @param[in] Rank          - The rank to work on
  @param[in] RankOccupancy - Current Rank Occupancy

  @retval TargetRanksBitmask - Rank Mask to use for Mrh Fsm
**/
UINT8
GetMrhFsmRankMaskLpddr5 (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  UINT8                RankOccupancy
  )
{
  UINT8 TargetRanksBitmask;

  // LPDDR5 MRH Command Regsiter 'Rank' field definition:
  // Bit[16] Subchannel 0, Rank 0
  // Bit[17] Subchannel 0, Rank 1
  // Bit[18] Subchannel 1, Rank 0
  // Bit[19] Subchannel 1, Rank 1
  if (Channel >= MAX_CHANNEL && Rank >= MAX_RANK_IN_CHANNEL) {
    // Run on all occupied subchannel ranks
    TargetRanksBitmask = RankOccupancy;
  } else if (Channel >= MAX_CHANNEL) {
    // Run only on input rank on all subchannels
    // Clear the Rank bits of the non-target ranks
    TargetRanksBitmask = RankOccupancy & ((MRC_BIT0|MRC_BIT2) << Rank);
  } else if (Rank >= MAX_RANK_IN_CHANNEL) {
    // Run on all ranks only on input subchannel
    // Clear the Rank bits of the non-target subchannels
    TargetRanksBitmask = RankOccupancy & ((MRC_BIT0|MRC_BIT1) << (MAX_SUB_CHANNEL * (IS_MC_SUB_CH (TRUE, Channel) ? 1 : 0)));
  } else {
    // Run only on input rank at input subchannel
    TargetRanksBitmask = (MRC_BIT0 << (Rank + ((Channel & MRC_BIT0) * MAX_SUB_CHANNEL)));
  }

  return TargetRanksBitmask;
}

/**
  This function calls the appropriate subroutine based on technology to get the rank mask needed for the MRH FSM
  This function supports the options for multicasting

  @param[in] MrcData            - Include all MRC global data.
  @param[in] Channel            - Channels to run MRH on (supports Multicast value)
  @param[in] CurrentController  - Single Controller to run MRH on
  @param[in] CurrentChannel     - Single Channel to run MRH on
  @param[in] Rank               - The rank to work on
  @param[in] RankOccupancy      - Current Rank Occupancy

  @retval TargetRanksBitmask    - Rank Mask to use for Mrh Fsm
**/
UINT8
GetMrhFsmRankMask (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Channel,
  IN  UINT32               CurrentController,
  IN  UINT32               CurrentChannel,
  IN  UINT32               Rank,
  IN  UINT8                RankOccupancy
  )
{
  if (MrcData->Outputs.IsDdr5) {
    return GetMrhFsmRankMaskDdr5 (MrcData, CurrentController, CurrentChannel, Rank, RankOccupancy);
  } else {
    return GetMrhFsmRankMaskLpddr5 (MrcData, Channel, Rank, RankOccupancy);
  }
}

/**
  This function executes the MRH (Mode Register Handler) while accumulating elapsed time for the stats
  framework.

  If the CADB mux is enabled, MRH is not executed.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] Controller        - Controller to run MRH.
  @param[in] Channel           - The channel to work on
  @param[in] Rank              - The rank to work on
  @param[in] Address           - MRW address
  @param[in] Data              - MRW Data
  @param[in] MrhCmd            - MRH command to execute
  @param[in] SpidWrCmdOverride - Force WrCmd on SPID_cmd_type
  @param[in] DebugPrint        - When TRUE, will print debugging information

  @retval mrcSuccess              - MRW was sent successfully
  @retval mrcDeviceBusy           - Timed out waiting for MRH
  @retval mrcWrongInputParameter  - Invalid MRH Command was request for current DDR type.
  @retval mrcFail                 - If CADB Mux is enabled
**/
MrcStatus
MrcRunMrh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  UINT8                 MrhCmd,
  IN  BOOLEAN               SpidWrCmdOverride,
  IN  BOOLEAN               DebugPrint
  )
{
  const MRC_STATISTIC Timer = MRC_MRH_TIME;
  MrcStatus Status;

  MrcStatsStartTimer (MrcData, Timer);
  Status = WrappedMrcRunMrh (MrcData, Controller, Channel, Rank, Address, Data, MrhCmd, SpidWrCmdOverride, DebugPrint);
  MrcStatsEndTimer (MrcData, Timer);

  return Status;
}


/**
  This function executes the MRH (Mode Register Handler).

  If the CADB mux is enabled, MRH is not executed.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] Controller        - Controller to run MRH.
  @param[in] Channel           - The channel to work on
  @param[in] Rank              - The rank to work on
  @param[in] Address           - MRW address
  @param[in] MrhCmd            - MRH command to execute
  @param[in] Data              - MRW Data
  @param[in] SpidWrCmdOverride - Force WrCmd on SPID_cmd_type
  @param[in] DebugPrint        - When TRUE, will print debugging information

  @retval mrcSuccess              - MRW was sent successfully
  @retval mrcDeviceBusy           - Timed out waiting for MRH
  @retval mrcWrongInputParameter  - Invalid MRH Command was request for current DDR type.
  @retval mrcFail                 - If CADB Mux is enabled
**/
MrcStatus
WrappedMrcRunMrh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  UINT8                 MrhCmd,
  IN  BOOLEAN               SpidWrCmdOverride,
  IN  BOOLEAN               DebugPrint
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  MrcInput            *Inputs;
  INT64               GetSetVal;
  UINT32              OffsetMrCommand;
  UINT32              IpChannel;
  UINT32              GetSetIpCh;
  UINT32              ControllerLoop;
  UINT32              ChannelLoop;
  MRC_RANGE           ControllerRange;
  MRC_RANGE           ChannelRange;
  UINT8               TargetRanksBitmask;
  BOOLEAN             Busy;
  BOOLEAN             MRR;
  MrcDdrType          DdrType;
  BOOLEAN             IsLpddr;
  BOOLEAN             IsDdr5;
  BOOLEAN             IsMulticastAccess;
  UINT32              SpidCmdType;
  UINT8               MaxCmd;
  UINT64              Timeout;
  MC0_CH0_CR_DDR_MR_COMMAND_STRUCT MrCommand;

  MrcCall = MrcData->Inputs.Call.Func;
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout
  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  Debug   = &Outputs->Debug;
  DdrType = Outputs->DdrType;
  IsDdr5  = Outputs->IsDdr5;
  IsLpddr = Outputs->IsLpddr;
  MrcCall->MrcSetMem ((UINT8 *) &ControllerRange, sizeof (ControllerRange), 0);
  MrcCall->MrcSetMem ((UINT8 *) &ChannelRange,    sizeof (ChannelRange),    0);

  IsMulticastAccess = MrcDetectMulticast (
                      MrcData,
                      GsmGtMax,
                      MRC_IGNORE_ARG,
                      Controller,
                      Channel,
                      Rank,
                      MRC_IGNORE_ARG,
                      MRC_IGNORE_ARG,
                      NULL,
                      &ControllerRange,
                      &ChannelRange,
                      NULL,
                      NULL,
                      NULL
                    );

  MaxCmd = (IsDdr5 ? MRC_MRH_CMD_DDR5_MAX : MRC_MRH_CMD_LPDDR5_MAX);

  MRR = (MrhCmd == MRC_MRH_CMD_MRR);
  if (MRR && IsMulticastAccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Multicast MRR\n", gErrString);
    return mrcWrongInputParameter;
  }

  for (ControllerLoop = ControllerRange.Start; ControllerLoop <= ControllerRange.End; ControllerLoop++) {
    if (!MrcControllerExist (MrcData, ControllerLoop)) {
      continue;
    }
    for (ChannelLoop = ChannelRange.Start; ChannelLoop <= ChannelRange.End; ChannelLoop++) {
      IpChannel = LP_IP_CH (IsLpddr, ChannelLoop);
      GetSetIpCh = GS_LP_IP_CH (IsLpddr, ChannelLoop);
      if (!MrcChannelExist (MrcData, ControllerLoop, ChannelLoop)) {
        continue;
      } else if (IS_MC_SUB_CH (IsLpddr, ChannelLoop) && Channel == MAX_CHANNEL) {
        // Skip calling for CH 1/3 if we are LPDDR since there isn't any register associated to it.
        continue;
      }

      // Create the TargetRanksBitmask
      // Get the rank mask from the MC RankOccupancy register
      // This mask includes the per-subchannel occupancy in the same format as MrCommand.Bits.Rank
      MrcGetSetMcCh (MrcData, ControllerLoop, GetSetIpCh, GsmMccRankOccupancy, ReadFromCache, &GetSetVal);
      TargetRanksBitmask = GetMrhFsmRankMask (MrcData, Channel, ControllerLoop, GetSetIpCh, Rank, (UINT8) GetSetVal);

      //
      // Make sure MRH is not busy
      //
      OffsetMrCommand = OFFSET_CALC_MC_CH (MC0_CH0_CR_DDR_MR_COMMAND_REG, MC1_CH0_CR_DDR_MR_COMMAND_REG, ControllerLoop, MC0_CH1_CR_DDR_MR_COMMAND_REG, IpChannel);
      do {
        MrCommand.Data = MrcReadCR (MrcData, OffsetMrCommand);
        Busy           = (MrCommand.Bits.Busy == 1);
        if (Inputs->ExtInputs.Ptr->SimicsFlag != 0) {
          Busy = 0;
        }
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

      if (Busy) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Timed out waiting for previous MRH command to finish!\n");
        return mrcDeviceBusy;
      }

      //
      // Send the command
      //
      /**
        Address: MR address
          LPDDR:
            Address [5:0] holds MR#, for MRR/MRW commands
            Address [7:0] holds OP[7:0] of the MPC command,
            Address [2:0] holds [WS_WR, WS_RD, WS_FAST] for CAS command (LPDDR5 only)
            (it should be noted that CAS command is an all rank command)
            CAS command should only be used when the functional lp5_wck_mode is set  to 2'b11 (OFF) so they are mutually exclusive.)

          DDR5:
            Address [7:0] hold MR# for MRR, MRW commands
            Address [7:0] hold Opcode for MPC commands
            Address [6:0] hold OP[6:0] for VrefCA/VrefCS
            Address [7]   indicates VrefCS/VrefCA (VrefCS=1, VrefCA=0)
      **/

      MrCommand.Bits.Address  = Address;

      /**
        For DDR5 or LPDDR5 MRW : Data[7:0] holds MR data
      **/
      // Put non-zero data for MRR.
      MrCommand.Bits.Data     = (MRR) ? 1 : Data;

      /**
          LPDDR5:   00 - MRR  01 - MRW 10 - MPC (not supporting DQ Calibraion and FIFO RD/WR) 11 - CAS command
          DDR5:     00 - MRR  01 - MRW 10 - MPC (not supporting DQ Calibraion and FIFO RD/WR) 11 - VREF CA/CS 100 - NOP
      **/
      if (MrhCmd > MaxCmd) {
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_ERROR,
          "%s %s %s MrhCmd(%d) > MaxCmd(%d)",
          gErrString,
          gWrongInputParameter,
          gDdrTypeStr[DdrType],
          MrhCmd,
          MaxCmd
          );
        return mrcWrongInputParameter;
      }
      MrCommand.Bits.Command = MrhCmd;

      /** SPID_cmd_type
          LPDDR:
            000: No extra indication
            001: LP5 MR35
            010: LP5 MR36
            011: LP5 MR38
            100: LP5 MR39
          DDR5:
            000: No extra indication
            001: DDR5 MR46
            010: DDR5 MR47
            101: WrCMD
      **/
      if (IsDdr5) {

        switch (MrCommand.Bits.Address) {
        case 46:
          SpidCmdType = MRC_MRH_DDR5_SPID_MR46;
          break;

        case 47:
          SpidCmdType = MRC_MRH_DDR5_SPID_MR47;
          break;

        default:
          SpidCmdType = 0;
        }

        // For DDR5 PDA Enumerate ID MRC
        if ((MrhCmd == MRC_MRH_CMD_MPC) && ((MrCommand.Bits.Address >> 4) == (DDR5_MPC_PDA_ENUMERATE_ID (0) >> 4))) {
          SpidCmdType = MRC_MRH_DDR5_SPID_CMD_WR_CMD;
        }
      } else {
        // Lpddr5
        switch (MrCommand.Bits.Address) {
        case 35:
          SpidCmdType = MRC_MRH_LPDDR5_SPID_MR35;
          break;

        case 36:
          SpidCmdType = MRC_MRH_LPDDR5_SPID_MR36;
          break;

        case 38:
          SpidCmdType = MRC_MRH_LPDDR5_SPID_MR38;
          break;

        case 39:
          SpidCmdType = MRC_MRH_LPDDR5_SPID_MR39;
          break;

        default:
          SpidCmdType = 0;
        }
      }
      if (SpidWrCmdOverride == TRUE) {
        SpidCmdType = MRC_MRH_DDR5_SPID_CMD_WR_CMD;
      }

      MrcSetBusCmdType (&(MrCommand.Data), SpidCmdType);
      MrCommand.Bits.Rank          = TargetRanksBitmask;
      MrCommand.Bits.Busy          = 1;
      MrCommand.Bits.Assume_idle   = 0;

      if (IsMulticastAccess && DebugPrint) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MRH to Mc %d Ch %d RankMask 0x%x\n", ControllerLoop, ChannelLoop, TargetRanksBitmask);
      }
      MrcWriteCR (MrcData, OffsetMrCommand, MrCommand.Data);
    } // ChannelLoop
  } // ControllerLoop

  // Check that all channels are done
  for (ControllerLoop = ControllerRange.Start; ControllerLoop <= ControllerRange.End; ControllerLoop++) {
    if (!MrcControllerExist (MrcData, ControllerLoop)) {
      continue;
    }
    for (ChannelLoop = ChannelRange.Start; ChannelLoop <= ChannelRange.End; ChannelLoop++) {
      IpChannel = LP_IP_CH (IsLpddr, ChannelLoop);
      if (!MrcChannelExist (MrcData, ControllerLoop, ChannelLoop)) {
        continue;
      } else if (IS_MC_SUB_CH (IsLpddr, ChannelLoop) && Channel == MAX_CHANNEL) {
        // Skip calling for CH 1/3 if we are LPDDR since there isn't any register associated to it.
        continue;
      }
      //
      // Wait till MRH is done sending the command
      //
      Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout
      OffsetMrCommand = OFFSET_CALC_MC_CH (MC0_CH0_CR_DDR_MR_COMMAND_REG, MC1_CH0_CR_DDR_MR_COMMAND_REG, ControllerLoop, MC0_CH1_CR_DDR_MR_COMMAND_REG, IpChannel);
      do {
        MrCommand.Data = MrcReadCR (MrcData, OffsetMrCommand);
        Busy           = (MrCommand.Bits.Busy == 1);
        if (Inputs->ExtInputs.Ptr->SimicsFlag != 0) {
          Busy = 0;
        }
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

      if (Busy) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Timed out sending MRH command!\n");
        return mrcDeviceBusy;
      } else if (MrCommand.Bits.Assume_idle == 1) {
        // Wait 64 DCLKs after Busy is low, before sending another MRH command
        MrcWait (MrcData, (Outputs->Dclkps * 64 * MRC_TIMER_1NS) / 1000);
      }
    } // ChannelLoop
  } // ControllerLoop

  return mrcSuccess;
}

/**
  Executes Generic MRH Command

  @param[in] MrcData           - Pointer to global data
  @param[in] Controller        - Targeted controller
  @param[in] Channel           - Targeted channel
  @param[in] Rank              - Targeted rank
  @param[in] Ca_Bus            - Command bus instruction
  @param[in] SpidWrCmdOverride - Force WrCmd on SPID_cmd_type
  @param[in] TwoCycleCommand   - Determines whether command should be two cycles or not.

  @retval mrsStatus
**/
MrcStatus
MrcRunGenericMrh (
  IN MrcParameters *const        MrcData,
  IN UINT32                      Controller,
  IN UINT32                      Channel,
  IN UINT32                      Rank,
  IN MRC_GEN_MRH_COMMAND  const  Ca_Bus,
  IN BOOLEAN                     SpidWrCmdOverride,
  IN BOOLEAN                     TwoCycleCommand
  )
{
  MrcStatus   Status;
  UINT32      IpChannel;
  UINT32      Offset;
  MC0_CH0_CR_MRH_GENERIC_COMMAND_STRUCT MrhGenericCommand;

  IpChannel = LP_IP_CH (MrcData->Outputs.IsLpddr, Channel);

  MrhGenericCommand.Data = 0;
  MrhGenericCommand.Bits.ca_bus = Ca_Bus.Data;      // ca_bus: Precharge banks
  MrhGenericCommand.Bits.two_cyc_command = TwoCycleCommand ? 0x1 : 0x0; // two_cyc_command
  MrhGenericCommand.Bits.Generic_MRH_override = 1;  // Generic_MRH_override: True

  Offset = MC0_CH0_CR_MRH_GENERIC_COMMAND_REG +
          ((MC1_CH0_CR_MRH_GENERIC_COMMAND_REG - MC0_CH0_CR_MRH_GENERIC_COMMAND_REG) * Controller) +
          ((MC0_CH1_CR_MRH_GENERIC_COMMAND_REG - MC0_CH0_CR_MRH_GENERIC_COMMAND_REG) * IpChannel);
  MrcWriteCR (MrcData, Offset, MrhGenericCommand.Data);

  Status = MrcRunMrh (MrcData, Controller, Channel, Rank, 0, 0, MRC_MRH_CMD_MPC, SpidWrCmdOverride, FALSE);

  MrhGenericCommand.Bits.Generic_MRH_override = 0;  // Generic_MRH_override: False
  MrcWriteCR (MrcData, Offset, MrhGenericCommand.Data);
  return Status;
}

/**
  Issue LPDDR & DDR5 MRW (Mode Register Write) command using MRH, on all populated channels.

  @param[in] MrcData      - Include all MRC global data.
  @param[in] RankMask     - Bit mask of ranks to consider.
  @param[in] Address      - MRW address
  @param[in] Data         - MRW Data
  @param[in] DebugPrint   - when TRUE, will print debugging information

  @retval mrcSuccess    - MRW was sent successfully
  @retval mrcFail       - MRW was not sent successfully
**/
MrcStatus
MrcIssueMrwMulticast (
  IN MrcParameters* const MrcData,
  IN UINT32               RankMask,
  IN UINT32               Address,
  IN UINT32               Data,
  IN BOOLEAN              DebugPrint
  )
{
  MrcStatus Status;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    Rank;

  Status = mrcSuccess;

  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if ((!MrcRankExist (MrcData, Controller, Channel, Rank)) || ((RankMask & (1 << Rank)) == 0)) {
          continue;
        }
        Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, Address, Data, DebugPrint);
        if (Status != mrcSuccess) {
          return Status;
        }
      }
    }
  }
  return Status;
}

/**
  Issue MRW (Mode Register Write) command using MRH (Mode Register Handler).

  @param[in] MrcData      - Include all MRC global data.
  @param[in] Controller   - Controller to send MRW.
  @param[in] Channel      - the channel to work on
  @param[in] Rank         - the rank to work on
  @param[in] Address      - MRW address
  @param[in] Data         - MRW Data
  @param[in] DebugPrint   - when TRUE, will print debugging information

  @retval mrcSuccess    - MRW was sent successfully
  @retval mrcDeviceBusy - timed out waiting for MRH
**/
MrcStatus
MrcIssueMrw (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Address,
  IN UINT32               Data,
  IN BOOLEAN              DebugPrint
)
{
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  // Send the MRW
  Status = MrcRunMrh (
    MrcData,
    Controller,
    Channel,
    Rank,
    Address,
    Data,
    MRC_MRH_CMD_MRW,
    FALSE,
    DebugPrint
    );

  if (DebugPrint && !Outputs->JedecInitDone) {
    MRC_DEBUG_MSG (
      Debug, MSG_LEVEL_NOTE,
      "MrcIssueMrw on mc %d ch %d rank %d: MR%d, Opcode=0x%02X\n",
      Controller, Channel, Rank, Address, Data
    );
  }

  return Status;
}

/**
  Issue LPDDR MRR (Mode Register Read) command using MRH (Mode Register Handler).
  Use DQ mapping array to deswizzle the MR data.

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to issue MRR.
  @param[in]  Channel    - the channel to work on
  @param[in]  Rank       - the rank to work on
  @param[in]  Address    - MRR address
  @param[out] Data       - MRR Data array per SDRAM device

  @retval mrcSuccess     - MRR was executed successfully
  @retval mrcDeviceBusy  - timed out waiting for MRH
**/
MrcStatus
MrcIssueMrr (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Address,
  OUT UINT8               Data[MRC_MRR_ARRAY_SIZE]
)
{
  const MrcInput        *Inputs;
  MrcDebug              *Debug;
  const MRC_FUNCTION    *MrcCall;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  MrcOutput             *Outputs;
  INT64                 MrResult;
  UINT32                CurrCpuBit;
  UINT32                CurrCpuByte;
  UINT32                CpuByteCnt;
  UINT32                DeviceCnt;
  UINT32                DeviceMax;
  UINT32                CurrDramBit;
  UINT32                DramByte;
  UINT32                BitVal;
  UINT32                DimmIdx;
  BOOLEAN               IsLpddr;

  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  ControllerIn = &Inputs->Controller[Controller];
  ChannelIn = &ControllerIn->Channel[Channel];
  CurrCpuByte = 0;
  IsLpddr = Outputs->IsLpddr;
  MrcCall->MrcSetMem (Data, MRC_MRR_ARRAY_SIZE * sizeof(Data[0]), 0);
  DimmIdx = IsLpddr ? dDIMM0 : RANK_TO_DIMM_NUMBER (Rank); // Lpddr only uses DIMM0 entry

  if (IsLpddr) {
    DeviceMax = (Outputs->LpByteMode) ? 2 : 1;
  } else {
    DeviceMax = Outputs->SdramCount;
  }

  // Send the MRR
  MrcRunMrh (MrcData, Controller, Channel, Rank, Address, 1, MRC_MRH_CMD_MRR, FALSE, FALSE);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MrcIssueMrr on mc %u ch %u rank %u: MR%d\n", Controller, Channel, Rank, Address);

  // Device now maps per byte with LPDDR.
  for (DeviceCnt = 0; DeviceCnt < DeviceMax; DeviceCnt++) {
    MrcGetSetStrobe (MrcData, Controller, Channel, Rank, DeviceCnt, GsmMccMrrResult, ReadCached, &MrResult);

    if (IsLpddr) {
      DramByte = DeviceCnt;
      // Find which CPU byte is mapped to the relevant DRAM byte
      for (CpuByteCnt = 0; CpuByteCnt < Outputs->SdramCount; CpuByteCnt++) {
        if (DramByte == ChannelIn->DqsMapCpu2Dram[DimmIdx][CpuByteCnt]) {
          CurrCpuByte = CpuByteCnt;
          break;
        }
      }

      for (CurrCpuBit = 0; CurrCpuBit < MAX_BITS; CurrCpuBit++) {
        // The actual DRAM bit that is connected to the current CPU DQ pin
       CurrDramBit = ChannelIn->DqMapCpu2Dram[Rank][CurrCpuByte][CurrCpuBit] - 8 * DramByte; // Subtract 8xDramByte
        BitVal = ((UINT8) MrResult >> CurrCpuBit) & 1; // The 0/1 value that came from the DRAM bit
        Data[DeviceCnt] |= (BitVal << CurrDramBit);             // Putting the value in the correct place
      }
    } else {
      // DDR5 MRR doesn't need DQ deswizzling
      Data[DeviceCnt] = (UINT8) MrResult;
    }
  }  // for DeviceCnt

  return mrcSuccess;
}

/**
  Issue NOP command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess               - NOP was sent successfully
  @retval mrcDeviceBusy            - Timed out waiting for MRH
  @retval mrcUnsupportedTechnology - The memory technology is not supported
**/
MrcStatus
MrcIssueNop (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN BOOLEAN              DebugPrint
)
{
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return mrcWrongInputParameter;
  }

  // Send the MPC
  Status = MrcRunMrh (
    MrcData,
    Controller,
    Channel,
    Rank,
    0,
    0,
    MRC_MRH_CMD_DDR5_NOP,
    FALSE,
    DebugPrint
    );

  if (DebugPrint) {
    MRC_DEBUG_MSG (
      Debug, MSG_LEVEL_NOTE,
      "MrcIssueNop on Mc %d Ch %d Rank %d\n",
      Controller, Channel, Rank
    );
  }

  return Status;
}

/**
  Issue VREF command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] Opcode     - VREF Opcode OP[7:0]
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess               - VREFCA was sent successfully
  @retval mrcDeviceBusy            - Timed out waiting for MRH
  @retval mrcUnsupportedTechnology - The memory technology is not supported
**/
MrcStatus
MrcIssueVrefCmd (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Opcode,
  IN BOOLEAN              DebugPrint
)
{
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT32    tCK;
  UINT32    tMRD;
  UINT32    tMRDns;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return mrcUnsupportedTechnology;
  }

  // Send the MPC
  Status = MrcRunMrh (
    MrcData,
    Controller,
    Channel,
    Rank,
    Opcode,
    0,
    MRC_MRH_CMD_DDR5_VREF_CMD,
    FALSE,
    DebugPrint
    );

  if (DebugPrint) {
    MRC_DEBUG_MSG (
      Debug, MSG_LEVEL_NOTE,
      "MrcIssueVrefCmd on Mc %d Ch %d Rank %d: Opcode 0x%X\n",
      Controller, Channel, Rank, Opcode
    );
  }

  tCK    = Outputs->MemoryClock;
  tMRD   = MrcGetTmod (MrcData, tCK);
  tMRDns = tMRD * tCK;
  tMRDns = DIVIDECEIL (tMRDns, FEMTOSECONDS_PER_NANOSECOND);

  // Wait tMRD after issuing VrefCmd command
  MrcWait (MrcData, tMRDns);

  return Status;
}


/**
  Issue LPDDR5, or DDR5 MPC command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] Opcode     - MPC Opcode (LP5\DDR5: OP[7:0])
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess    - MPC was sent successfully
  @retval mrcDeviceBusy - Timed out waiting for MRH
**/
MrcStatus
MrcIssueMpc (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               Opcode,
  IN BOOLEAN              DebugPrint
)
{
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT32    tMpcNck;
  UINT32    tMpcNckFs;
  UINT32    tMpcNs;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  // Send the MPC
  Status = MrcRunMrh (
    MrcData,
    Controller,
    Channel,
    Rank,
    Opcode,
    0,
    MRC_MRH_CMD_MPC,
    FALSE,
    DebugPrint
    );

  if (DebugPrint) {
    MRC_DEBUG_MSG (
      Debug, MSG_LEVEL_NOTE,
      "MrcIssueMpc on Mc %d Ch %d Rank %d: Opcode 0x%X\n",
      Controller, Channel, Rank, Opcode
    );
  }

  tMpcNck = MrcGetTmod (MrcData, Outputs->MemoryClock);
  tMpcNckFs = tMpcNck * Outputs->MemoryClock;
  tMpcNs = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);
  MrcWait (MrcData, tMpcNs);

  return Status;
}

/**
  Issue CAS command using MRH (Mode Register Handler).

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - the controller to work on
  @param[in] Channel    - The channel to work on
  @param[in] Rank       - The rank to work on
  @param[in] OpcodeEnum - CAS Opcode Enum
  @param[in] DebugPrint - When TRUE, will print debugging information

  @retval mrcSuccess    - MPC was sent successfully
  @retval mrcDeviceBusy - Timed out waiting for MRH
**/
MrcStatus
MrcIssueCas (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OpcodeEnum,
  IN BOOLEAN              DebugPrint
)
{
  MrcStatus Status;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT32    Opcode;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  switch(OpcodeEnum) {
    case MrhCasWck2CkSyncOff:
      Opcode = MRC_LP5_CAS_WCK2CK_SYNC_OFF;
      break;
    case MrhCasWckFastSync:
      Opcode = MRC_LP5_CAS_WCK2CK_FAST_SYNC;
      break;
    default:
      return mrcWrongInputParameter;
      break;
  }

  // Send the MPC
  Status = MrcRunMrh (
    MrcData,
    Controller,
    Channel,
    Rank,
    Opcode,
    0,
    MRC_MRH_CMD_LPDDR5_CAS,
    FALSE,
    DebugPrint
    );

  if (DebugPrint) {
    MRC_DEBUG_MSG (
      Debug, MSG_LEVEL_NOTE,
      "MrcIssueCas on Mc %d Ch %d Rank %d: Opcode 0x%X\n",
      Controller, Channel, Rank, Opcode
    );
  }

  return Status;
}

/**
  This function issues the PDA Select ID MPC.  The device ID's selected to listen to following
  MR/CAVref/MPC commands will be defined by Index.
  This will wait tMPC_Delay after sending the PDA Select ID MPC such that the caller does not need to wait
  to send the next command.
  To enable normal Rank operation mode for MR/CAVref/MPC commands, the caller must issue the PDA Select ID MPC
  after finishing the MR/CAVref/MPC updates with a value of 15 in Index.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  Controller - 0-Based index of the Controller to access.
  @param[in]  Channel    - 0-Based index of the Channel to access.
  @param[in]  Rank       - 0-Based index of the Rank to access.
  @param[in]  Index      - 0-Based number of the PDA ID to select. 15 enables all devices
  @param[in]  DebugPrint - When TRUE, will print debugging information

  @retval MrcStatus - mrcTimeout if the FSM does not complete after 1s.
  @retval MrcStatus - mrcSuccess if the MPC is sent successfuly.
  @retval MrcStatus - mrcFail for unexepected failures.
**/
MrcStatus
MrcPdaSelect (
  IN  MrcParameters * const MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT8                 Index,
  IN  BOOLEAN               DebugPrint
  )
{
  MrcOutput *Outputs;
  MrcStatus Status;
  MrcDebug  *Debug;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (DebugPrint) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
      "PdaSelect on MC%d C%d R%d: Device 0x%X\n",
      Controller, Channel, Rank, Index
    );
  }

  //Send PDA Select ID
  Status = MrcIssueMpc (MrcData, Controller, Channel, Rank, DDR5_MPC_PDA_SELECT_ID (Index), DebugPrint);
  return Status;
}

/**
  This function executes the requested ZQ command.
  This function will wait the appropriate wait time as specified in the JEDEC spec.
  ZQ is sent to all populated controllers, channels, and ranks.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSendZqLpddr5 (
  IN  MrcParameters* const  MrcData
)
{
  MrcStatus   Status = mrcSuccess;

  Status = MrcIssueMpc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MRC_LP5_MPC_ZQCAL_LATCH, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }
  // tZQLATCH - 30ns
  MrcWait (MrcData, 30 * MRC_TIMER_1NS);

  return Status;
}

/**
  This function executes the requested ZQ command.
  This function will wait the appropriate wait time as specified in the JEDEC spec.
  ZQ is sent to all populated controllers, channels, and ranks.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSendZqDdr5 (
  IN  MrcParameters* const  MrcData
)
{
  MrcStatus   Status = mrcSuccess;

  Status = MrcIssueMpc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, DDR5_MPC_ZQCAL_START, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }
  // tZQCAL - 1us
  MrcWait (MrcData, 1 * MRC_TIMER_1US);

  Status = MrcIssueMpc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, DDR5_MPC_ZQCAL_LATCH, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }
  // tZQLATCH - 30ns
  MrcWait (MrcData, 30 * MRC_TIMER_1NS);

  return Status;
}

/**
  Issue ZQ calibration command on all populated controller, channels and ranks.
  When done, wait appropriate delay depending on the ZQ type.

  @param[in] MrcData  - include all the MRC data

  @retval MrcStatus - mrcSuccess if passes, otherwise an error status
**/
MrcStatus
MrcIssueZQ (
  IN  MrcParameters* const  MrcData
)
{
  MrcOutput* Outputs;
  MrcStatus  Status;
  MrcDebug*  Debug;
  BOOLEAN    IsLpddr5;

  Outputs = &MrcData->Outputs;
  IsLpddr5 = Outputs->IsLpddr5;
  Debug = &Outputs->Debug;

  if (Outputs->IsDramDvfsqOn) {
    // ZQ Cal is disabled due to DVFSQ, skip sending command.
    return mrcSuccess;
  }

  if (!Outputs->JedecInitDone) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Issue ZQ on all channels / ranks\n");
  }

  // We only support:
  // MRC_ZQ_CAL for DDR5
  if (IsLpddr5) {
    Status = MrcSendZqLpddr5 (MrcData);
  } else {
    Status = MrcSendZqDdr5 (MrcData);
  }

  return Status;
}

