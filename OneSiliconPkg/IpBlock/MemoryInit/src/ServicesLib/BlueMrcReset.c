/** @file
  The functions in this file implement the LP5 reset sequence.

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
#include "MrcReset.h"
#include "MrcCommon.h"
#include "CMrcApi.h"
#include "MrcHalApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"
#include "CMrcStatsTracker.h" // for MrcStatsIncrementData()


typedef struct {
  MRC_GEN_MRS_FSM_MR_TYPE                             MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM];
  MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT                   MrsFsmCtlSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS];
  GENERIC_MRS_FSM_CONTROL_STRUCT                      GenMrsFsmCtlSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_MR_GEN_FSM];
  GENERIC_MRS_FSM_STORAGE_VALUES_STRUCT               GenMrsFsmValSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_STORAGE_GEN_FSM];
  GENERIC_MRS_FSM_TIMING_STORAGE_STRUCT               GenMrsFsmTimeSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_TIMING_GEN_FSM];
} JEDECRESET_SAVERESTORE;

/**
  Reset the MC IO module. The MC hardware will handle creating the 20 dclk pulse
  after the bit is set and will also clear the bit.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess IO Reset was done successfully
  @retval mrcDeviceBusy Timed out waiting for the IO to clear the bit
**/
MrcStatus
IoReset (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  INT64               GetSetVal;
  BOOLEAN             Busy;
  UINT64              Timeout;
  UINT8               FirstController;
  UINT8               FirstChannel;
  INT64               InternalClocksOnSave;
  MrcInput           *Inputs;

  MrcCall = MrcData->Inputs.Call.Func;
  Inputs = &MrcData->Inputs;
  Busy = TRUE;
  Outputs = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;

  Timeout = MrcCall->MrcGetCpuTime() + MRC_WAIT_TIMEOUT;   // 10 seconds timeout

  // Set InternalClocksOn to avoid clock gating when we issue IoReset
  MrcGetSetChStrb(MrcData, FirstController, FirstChannel, 0, GsmIocInternalClocksOn, ReadFromCache, &InternalClocksOnSave);
  GetSetVal = 1;
  MrcGetSetChStrb(MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocInternalClocksOn, WriteCached, &GetSetVal);

  MrcGetSetNoScope(MrcData, GsmIocIoReset, ForceWriteUncached, &GetSetVal);

  //
  // Wait until the bit is cleared by hardware
  //
  do {
    MrcGetSetNoScope(MrcData, GsmIocIoReset, ReadNoCache, &GetSetVal);
    Busy = (GetSetVal == 1);
    if (Inputs->ExtInputs.Ptr->SimicsFlag != 0) {
      Busy = 0;
    }
  } while (Busy && (MrcCall->MrcGetCpuTime() < Timeout ));

  // Restore InternalClocksOn
  MrcGetSetChStrb(MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocInternalClocksOn, WriteCached, &InternalClocksOnSave);

  return (Busy ? mrcDeviceBusy : mrcSuccess);
}

/**
  This function will save and restore the generic mrs fsm register values.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  SaveValue         - Save the value if true.
  @param[in]  RestoreValue      - Restore the value if true.
  @param[in]  MrsFsmCtlSave     - Storage for saving and restoring MrsFsmCtl.
  @param[in]  GenMrsFsmCtlSave  - Storage for saving and restoring GenMrsFsmCtl.
  @param[in]  GenMrsFsmValSave  - Storage for saving and restoring GenMrsFsmVal.
  @param[in]  GenMrsFsmTimeSave - Storage for saving and restoring GenMrsFsmTime.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSaveRestoreGenericMrsFsm (
  IN  MrcParameters* const  MrcData,
  IN  BOOLEAN               SaveValue,
  IN  BOOLEAN               RestoreValue,
  IN  MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT      MrsFsmCtlSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS],
  IN  GENERIC_MRS_FSM_CONTROL_STRUCT         GenMrsFsmCtlSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_MR_GEN_FSM],
  IN  GENERIC_MRS_FSM_STORAGE_VALUES_STRUCT  GenMrsFsmValSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_STORAGE_GEN_FSM],
  IN  GENERIC_MRS_FSM_TIMING_STORAGE_STRUCT  GenMrsFsmTimeSave[MAX_CONTROLLER][MAX_CHANNEL_SHARE_REGS][MAX_TIMING_GEN_FSM]
  )
{
  const MrcInput* Inputs;
  MrcStatus       Status;
  MrcOutput*      Outputs;
  UINT32          Channel;
  UINT32          Data;
  UINT32          Controller;
  UINT32          Offset;
  UINT32          IpChannel;
  UINT32          ControlRegIdx;
  BOOLEAN         IsFastBoot;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  Data = 0;
  IsFastBoot = (Inputs->BootMode == bmFast);
  Status = mrcSuccess;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {

      if (((IsFastBoot) && (MrcChannelExist (MrcData, Controller, Channel))) && (SaveValue)) {
        // Back up the runtime values of Generic MRS FSM
        if ((!IS_MC_SUB_CH (Outputs->IsLpddr5, Channel))) {
          IpChannel = LP_IP_CH (Outputs->IsLpddr5, Channel);
          Offset = MrcGetMrsFsmCtlOffset (MrcData, Controller, IpChannel);
          MrsFsmCtlSave[Controller][IpChannel].Data = MrcReadCR (MrcData, Offset);
          for (ControlRegIdx = 0; ControlRegIdx < MAX_MR_GEN_FSM; ControlRegIdx++) {
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxRead, GmfMailboxTypeControl, ControlRegIdx, &Data);
            GenMrsFsmCtlSave[Controller][IpChannel][ControlRegIdx].Data = Data;
          }
          for (ControlRegIdx = 0; ControlRegIdx < MAX_STORAGE_GEN_FSM; ControlRegIdx++) {
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxRead, GmfMailboxTypeStorage, ControlRegIdx, &Data);
            GenMrsFsmValSave[Controller][IpChannel][ControlRegIdx].Data = Data;
          }
          for (ControlRegIdx = 0; ControlRegIdx < MAX_TIMING_GEN_FSM; ControlRegIdx++) {
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxRead, GmfMailboxTypeTiming, ControlRegIdx, &Data);
            GenMrsFsmTimeSave[Controller][IpChannel][ControlRegIdx].Data = Data;
          }
        }
      } // bmFast
    } // Channel
  } // Controller

  if ((IsFastBoot) && (RestoreValue)) {
    // Restore the runtime values of Generic MRS FSM
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if (!MrcChannelExist (MrcData, Controller, Channel)) {
          continue;
        }

        if ((!IS_MC_SUB_CH (Outputs->IsLpddr5, Channel))) {
          IpChannel = LP_IP_CH (Outputs->IsLpddr5, Channel);
          Offset = MrcGetMrsFsmCtlOffset (MrcData, Controller, IpChannel);
          MrcWriteCR (MrcData, Offset, MrsFsmCtlSave[Controller][IpChannel].Data);
          for (ControlRegIdx = 0; ControlRegIdx < MAX_MR_GEN_FSM; ControlRegIdx++) {
            Data = GenMrsFsmCtlSave[Controller][IpChannel][ControlRegIdx].Data;
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxWrite, GmfMailboxTypeControl, ControlRegIdx, &Data);
          }
          for (ControlRegIdx = 0; ControlRegIdx < MAX_STORAGE_GEN_FSM; ControlRegIdx++) {
            Data = GenMrsFsmValSave[Controller][IpChannel][ControlRegIdx].Data;
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxWrite, GmfMailboxTypeStorage, ControlRegIdx, &Data);
          }
          for (ControlRegIdx = 0; ControlRegIdx < MAX_TIMING_GEN_FSM; ControlRegIdx++) {
            Data = GenMrsFsmTimeSave[Controller][IpChannel][ControlRegIdx].Data;
            GenericMrsFsmMailboxAccess (MrcData, Controller, IpChannel, GmfMailboxWrite, GmfMailboxTypeTiming, ControlRegIdx, &Data);
          }
        }
      } // for Channel
    } // for Controller
  } // if FastBoot

  return Status;
}

/**
  Perform full JEDEC reset and init sequence.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] SaveBuffer - Data buffer for Fast Boot Generic MRS FSM Save\Restore

  @retval MrcStatus - mrcSuccess or reason for failure.
**/
MrcStatus
MrcResetSequencePrivate (
  IN MrcParameters *const MrcData,
  IN JEDECRESET_SAVERESTORE *SaveBuffer OPTIONAL
  )
{
  const MRC_FUNCTION    *MrcCall;
  MrcStatus             Status;
  MrcInput              *Inputs;
  MrcOutput             *Outputs;
  BOOLEAN               IsLpddr;
  BOOLEAN               Print;
  BOOLEAN               IsFastBoot;
  INT64                 GetSetVal;
  INT64                 CmdTristateSave;
  UINT32                Controller;
  UINT32                Channel;

  Inputs     = &MrcData->Inputs;
  Outputs    = &MrcData->Outputs;
  IsLpddr    = Outputs->IsLpddr;
  IsFastBoot = (Inputs->BootMode == bmFast);
  Print      = Outputs->EctDone;
  MrcCall    = Inputs->Call.Func;
  Status     = mrcSuccess;

  if (SaveBuffer != NULL) {
    MrcCall->MrcSetMem ((UINT8 *) SaveBuffer, sizeof (JEDECRESET_SAVERESTORE), 0);
  }

  if (IsFastBoot) {
    Print = 0;
    // SaveBuffer must not be NULL on Fast Boot
    MRC_DEBUG_ASSERT (
      SaveBuffer != NULL,
      &MrcData->Outputs.Debug,
      "%s: SaveBuffer",
      gNullPtrErrStr
      );
  }

  // Disable CA Tristating so NOP can occur
  Controller = Outputs->FirstPopController;
  Channel = Outputs->Controller[Controller].FirstPopCh;
  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCmdTriStateDis, ReadFromCache, &CmdTristateSave);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCmdTriStateDis, WriteCached, &GetSetVal);

  // Start with an IO reset
  Status = IoReset (MrcData);

  if (mrcSuccess == Status) {
    // Perform jedec reset.
    // If RTT_NOM is to be enabled in MR1, the ODT input signal must be statically held LOW
    // in our system RTT_NOM is always enable.

    if (!Outputs->RestoreMRs) {
      // Setup Init MR's to a default value, and update MrcData.
      Status = MrcMrwInit (MrcData);
      if (Status != mrcSuccess) {
        return Status;
      }
    }

    if (IsFastBoot) {
      // Save the generic mrs fsm
      MrcSaveRestoreGenericMrsFsm (MrcData, TRUE, FALSE, SaveBuffer->MrsFsmCtlSave, SaveBuffer->GenMrsFsmCtlSave, SaveBuffer->GenMrsFsmValSave, SaveBuffer->GenMrsFsmTimeSave);
      MrcGenMrsFsmClean (MrcData, SaveBuffer->MrData, TRUE);
    }

    if (Outputs->IsCs2NEnabled) {
      DdrioCs2NReset (MrcData);
    }

    Status = MrcJedecReset (MrcData);
    if (Status == mrcSuccess) {
      Status = MrcJedecInit (MrcData);
    }

    if (IsFastBoot) {
      // Restore the generic mrs fsm value
      MrcSaveRestoreGenericMrsFsm (MrcData, FALSE, TRUE, SaveBuffer->MrsFsmCtlSave, SaveBuffer->GenMrsFsmCtlSave, SaveBuffer->GenMrsFsmValSave, SaveBuffer->GenMrsFsmTimeSave);
    }

    if (IsLpddr && (Status == mrcSuccess)) {
      Status = MrcLpddrSwitchToHigh (MrcData, !Print);  // Switch to FSP-OP1
      if (Status != mrcSuccess) {
        return Status;
      }
    }
  }

  // Restore Command Tristate setting
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCmdTriStateDis, WriteCached, &CmdTristateSave);

  if (IsLpddr && IsFastBoot) {
    // MRC Controls the FSP-OP switch on bmCold and bmFast boot flows as part of JEDEC Reset \ JEDEC Init.
    // JEDEC spec requires MR16 VRCG="VREF Fast Response (high current)" mode any time FSP-OP is switched.
    // VRCG must be set back to "Normal Operation" at the end of MRC
    // For bmFast, switch to VRCG after Jedec Init
    // Configuring MrhDelay for 250ns as it's the maximum for LP5.
    GetSetVal = DIVIDECEIL(MRC_LP_tFC_LONG_NS * 1000, Outputs->Wckps);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
    MrcSetFspVrcg (MrcData, ALL_RANK_MASK, LpVrcgNormal, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8);

    GetSetVal = 0;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctMrhAfterCommandDelay, WriteCached, &GetSetVal);
  }

  // Set flag to restore from host structure instead from look-up table
  Outputs->RestoreMRs = TRUE;
  Outputs->JedecInitDone = TRUE;

  return Status;
}

/**
  Perform full JEDEC reset and init sequence with a fast boot save \ restore stack buffer.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess or reason for failure.
**/
MrcStatus
MrcResetSequenceFastBootPrivate (
  IN MrcParameters* const MrcData
)
{
  JEDECRESET_SAVERESTORE SaveBuffer;
  return MrcResetSequencePrivate (MrcData, &SaveBuffer);
}

/**
  Perform full JEDEC reset and init sequence.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess or reason for failure.
**/
MrcStatus
MrcResetSequence (
  IN MrcParameters *const MrcData
  )
{
  MrcStatsIncrementData (MrcData, MRC_JEDEC_RESET_COUNT);

  if (MrcData->Inputs.BootMode == bmFast) {
    // Call a seperate function that allocates a stack buffer for the fast boot save \ restore operations
    return MrcResetSequenceFastBootPrivate (MrcData);
  } else {
    return MrcResetSequencePrivate (MrcData, NULL);
  }
}
