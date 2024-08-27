/** @file
  Implementation of LPDDR5 Specific functions, and constants.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
  JEDEC
**/
#include "BlueMrcJedecApi.h"  // to ensure prototypes match implementations
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcLpddr5Registers.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcHalApi.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdrIoDefines.h"
#include "MrcPmic.h"
#include "BlueMrcPmic.h"
#include "MrcDdrIoApi.h"

///
/// Initialization Timing Parameters
///

#define MRC_DDR5_tINIT0_MS   20      ///< Max voltage-ramp time
#define MRC_DDR5_tINIT1_US   200     ///< Min RESET_n LOW time after completion of voltage Ramp
#define MRC_DDR5_tINIT2_NS   10      ///< Min CS_n LOW time before RESET_n HIGH
#define MRC_DDR5_tINIT3_MS   4       ///< Min CS_n LOW time after RESET_n HIGH
#define MRC_DDR5_tINIT4_US   2       ///< Minimum time for DRAM to register EXIT on CS_n HIGH
#define MRC_DDR5_tINIT5_NCK  3       ///< Minimum cycles required after CS_n HIGH

/**
  Perform JEDEC Init sequence for DDR5.

  @param[in] MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcJedecInitDdr5 (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  MrcInput  *Inputs;
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    Offset;
  UINT32    MrsFsmCtlSave;
  MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT MrsFsmCtl;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Status  = mrcSuccess;

  if (Inputs->BootMode == bmFast) {
    // Disable do_dq_osc_start for the JEDEC Init FSM run
    MrsFsmCtlSave = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_CONTROL_REG, MC1_CH0_CR_MRS_FSM_CONTROL_REG, Controller, MC0_CH1_CR_MRS_FSM_CONTROL_REG, Channel);
          MrsFsmCtl.Data = MrcReadCR (MrcData, Offset);
          MrsFsmCtlSave = MrsFsmCtl.Data;                     // MRS_FSM_CONTROL has the same value on all channels, hence backup from any channel
          MrsFsmCtl.Bits.do_dq_osc_start = 0;
          MrcWriteCR (MrcData, Offset, MrsFsmCtl.Data);
        }
      }
    }

    // Disable multicycle MPC command on MC
    // DRAM will be configured to 1tCK MPC via MR2
    DisableMcMulticycleCs (MrcData, FALSE);

    if (PerformGenericMrsFsmSequence (MrcData, JedecInitSequenceSingleCycle, FALSE) != mrcSuccess) {
      return mrcFail;
    }
    if (Outputs->IsCs2NEnabled) {
      Status = Ddr5EnableCsGearDown (MrcData);
      if (Status != mrcSuccess) {
        return Status;
      }
      DdrIoCs2NEnableOrDisable (MrcData, MRC_ENABLE);
    }
    // Enumerate DRAM devices for PDA use
    if (Inputs->EnablePda) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
          if (!MrcChannelExist (MrcData, Controller, Channel)) {
            continue;
          }
          MrcSetDramVref (
            MrcData,
            (UINT8) Controller,
            (UINT8) Channel,
            0xF,
            1, //non_PDA mode
            WrV,
            0,
            FALSE,
            FALSE,
            TRUE
            );
        } // For Channel
      } // For Controller

      Status = MrcPdaEnumeration (MrcData);
      if (Status != mrcSuccess) {
        return Status;
      }
    }

    // Run the FSM second time, after PDA enumeration and multicycle MPC config
    if (PerformGenericMrsFsmSequence (MrcData, JedecInitSequenceMultiCycle, FALSE) != mrcSuccess) {
      return mrcFail;
    }

    // Restore do_dq_osc_start
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_CONTROL_REG, MC1_CH0_CR_MRS_FSM_CONTROL_REG, Controller, MC0_CH1_CR_MRS_FSM_CONTROL_REG, Channel);
          MrcWriteCR (MrcData, Offset, MrsFsmCtlSave);
        }
      }
    }
  } else {
    if (PerformGenericMrsFsmSequence (MrcData, JedecInitSequenceSingleCycle, FALSE) != mrcSuccess) {
      return mrcFail;
    }

    if (Outputs->IsCs2NEnabled) {
      Status = Ddr5EnableCsGearDown (MrcData);
      if (Status != mrcSuccess) {
        return Status;
      }
      DdrIoCs2NEnableOrDisable (MrcData, MRC_ENABLE);
    }
    if (Outputs->EctDone) {
      // Enumerate DRAM devices for PDA use
      if (Inputs->EnablePda) {
        for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
          for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
            if (!MrcChannelExist (MrcData, Controller, Channel)) {
              continue;
            }
            MrcSetDramVref (
              MrcData,
              (UINT8) Controller,
              (UINT8) Channel,
              0xF,
              1, //non_PDA mode
              WrV,
              0,
              FALSE,
              FALSE,
              TRUE
              );
          } // For Channel
        } // For Controller
        Status = MrcPdaEnumeration (MrcData);
        if (Status != mrcSuccess) {
          return Status;
        }
      }

      // Once ECT is done, we can send out multi-cycle MRW commands (first non-PDA, then PDA)
      if (PerformGenericMrsFsmSequence (MrcData, JedecInitSequenceMultiCycle, FALSE) != mrcSuccess) {
        return mrcFail;
      }
    }
  }
  return Status;
}

/**
  This function issues the PDA Enumeration ID MPC. This will first send a MPC to enter PDA Enumeration Mode.
  It will then iterate through all DRAM devices and send an MPC to assign it an ID number, starting at 0.
  Once the iteration is done, a MPC will be sent to exit PDA Mode. This will wait tMPC Delay between PDA Entry
  and the first PDA Enumerate ID MPC's. This will wait tPDA_DELAY between the PDA Enumerate ID MPC's.

  @param[in]  MrcData       - Pointer to global MRC data

  @retval MrcStatus - mrcTimeout if the FSM does not complete after 1s
  @retval MrcStatus - mrcSuccess if the MPC is sent successfuly
  @retval MrcStatus - mrcFail for unexepected failures
**/
MrcStatus
MrcPdaEnumeration (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcStatus     Status;
  MrcOutput     *Outputs;
  MrcInput      *Inputs;
  MrcChannelIn  *ChannelIn;
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  MrcDebug      *Debug;
  INT64         GetSetVal;
  INT64         GetSetEn;
  INT64         GetSetDis;
  INT64         tCWL4TxDqFifoRdEnSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64         tCWL4TxDqFifoWrEnSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64         TxPiPwrDnDis;
  UINT32        tCWL4TxDqFifoRdEn;
  UINT32        tCWL4TxDqFifoWrEn;
  INT64         TcwlSave;
  INT64         TcwlDecSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64         TcwlAddSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64         TxDqsSave[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL];
  INT64         TxDqsOffsetSave[MAX_CONTROLLER][MAX_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL];
  INT64         OdtParkModeSave;
  INT64         ForceOdtOnSave;
  INT64         ForceRxOnSave;
  UINT8         Controller;
  UINT8         Channel;
  UINT8         TcwlCalc;
  UINT8         FirstController;
  UINT32        TckPs;
  UINT32        tcwlNs;
  UINT32        tPdaDelay;
  UINT8         Rank;
  UINT8         Byte;
  UINT8         DramByte;
  UINT8         NumDevices;
  UINT8         Device;
  UINT8         DimmIdx;
  UINT8         FirstChannel;
  UINT8         BytesPerDevice;
  BOOLEAN       IsDdr5;
  BOOLEAN       IsGear4;
  UINT8         CpuByteList[MAX_BYTE_IN_DDR5_CHANNEL];
  UINT8         PdaIdList[MAX_BYTE_IN_DDR5_CHANNEL];

  Outputs           = &MrcData->Outputs;
  Inputs            = &MrcData->Inputs;
  MrcCall           = Inputs->Call.Func;
  Status            = mrcSuccess;
  Debug             = &Outputs->Debug;
  FirstController   = Outputs->FirstPopController;
  FirstChannel      = Outputs->Controller[FirstController].FirstPopCh;
  IsDdr5            = Outputs->IsDdr5;
  TckPs             = Outputs->tCKps;
  IsGear4           = Outputs->GearMode ? 1 : 0;
  GetSetEn          = 1;
  GetSetDis         = 0;

  // Only run PDA Enumeration on DDR5
  if (!IsDdr5) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Skip PDA Enumeration, only supported for DDR5 \n");
    return Status;
  }

  // Save OdtForceQDrvEn and ForceRxOn
  MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmIocForceOdtOn,   ReadCached, &ForceOdtOnSave);
  MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmIocForceRxAmpOn, ReadCached, &ForceRxOnSave);
  MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmIocTxPiPwrDnDis, ReadCached, &TxPiPwrDnDis);
  // Set OdtForceQDrvEn = 0 and ForceRxOn = 0 during PDA enumeration
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceOdtOn,   ForceWriteCached, &GetSetDis);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceRxAmpOn, ForceWriteCached, &GetSetDis);

  // Initialize arrays
  MrcCall->MrcSetMem ((UINT8 *) CpuByteList, sizeof (CpuByteList), 0);
  MrcCall->MrcSetMem ((UINT8 *) PdaIdList, sizeof (PdaIdList), 0);
  MrcCall->MrcSetMem ((UINT8 *) tCWL4TxDqFifoRdEnSave, sizeof (tCWL4TxDqFifoRdEnSave), 0);
  MrcCall->MrcSetMem ((UINT8 *) tCWL4TxDqFifoWrEnSave, sizeof (tCWL4TxDqFifoWrEnSave), 0);
  MrcCall->MrcSetMem ((UINT8 *) TcwlDecSave, sizeof (TcwlDecSave), 0);
  MrcCall->MrcSetMem ((UINT8 *) TcwlAddSave, sizeof (TcwlAddSave), 0);
  MrcCall->MrcSetMem ((UINT8 *) TxDqsSave, sizeof (TxDqsSave), 0);
  MrcCall->MrcSetMem ((UINT8 *) TxDqsOffsetSave, sizeof (TxDqsOffsetSave), 0);

  MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmIocDataDqsOdtParkMode, ReadCached, &OdtParkModeSave);
  // Drive DQS low
  GetSetVal = MrcGetEnDqsOdtParkModeValue();
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDataDqsOdtParkMode, WriteToCache, &GetSetVal);

  MrcGetSetNoScope (MrcData, GsmIocDfiCmdDecoderEn, WriteToCache, &GetSetDis);

  // Set DQ Pins HIGH, to a known state
  GetSetVal = 0xFF;
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteToCache, &GetSetVal);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn,   WriteToCache, &GetSetVal);

  MrcFlushRegisterCachedData (MrcData);

  // Save all tCWL and related settings
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMctCWL, ReadNoCache, &TcwlSave);

  // Set custom tCWL settings for PDA Enumeration
  // tCWL = 11.5ns - This is the middle of the tPDA_DQS_Delay range
  if (IsGear4 && (Outputs->Frequency == f3200)) {
    tcwlNs = 14000;
  } else {
    tcwlNs = 11500;
  }

  TcwlCalc = (UINT8) DIVIDECEIL (tcwlNs, TckPs);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, ReadCached, &tCWL4TxDqFifoRdEnSave[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoWrEnTcwlDelay, ReadCached, &tCWL4TxDqFifoWrEnSave[Controller][Channel]);
      MrcGetTxDqFifoDelay (MrcData, TcwlCalc, Controller, Channel, &tCWL4TxDqFifoWrEn, &tCWL4TxDqFifoRdEn);

      GetSetVal = tCWL4TxDqFifoRdEn;
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, WriteToCache, &GetSetVal);
      GetSetVal = tCWL4TxDqFifoWrEn;
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoWrEnTcwlDelay, WriteToCache, &GetSetVal);
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        MrcGetSetChStrb (MrcData, Controller, Channel, Byte, TxDqsOffset, ReadFromCache, &TxDqsOffsetSave[Controller][Channel][Byte]);
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
            MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqsDelay, ReadFromCache, &TxDqsSave[Controller][Channel][Rank][Byte]);
          }
        }
      }
    }
  }

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, TxDqsOffset, WriteToCache, &GetSetDis);
  MrcGetSetStrobe (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MAX_SDRAM_IN_DIMM, TxDqsDelay, WriteToCache, &GetSetDis);
  GetSetVal = TcwlCalc;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctCWL, WriteToCache, &GetSetVal);

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocTxPiPwrDnDis, WriteToCache, &GetSetEn);

  MrcFlushRegisterCachedData (MrcData);

  // Prepare GetSetVal for use below
  GetSetVal = 0xFF;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        ChannelIn  = &Inputs->Controller[Controller].Channel[Channel];
        DimmIdx = (UINT8) RANK_TO_DIMM_NUMBER (Rank);
        DimmOut = &ChannelOut->Dimm[DimmIdx];

        // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
        NumDevices = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
        BytesPerDevice = DimmOut->SdramWidth / 8;
        if (DimmOut->EccSupport) {
          NumDevices += 1;
        }

        if (Outputs->BitByteSwizzleFound) {
          DramByte = 0;
          for (Device = 0; Device < NumDevices; Device++) {
            for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
              if (!MrcByteExist (MrcData, Controller, Channel, Byte)) {
                continue;
              }
              if (ChannelIn->DqsMapCpu2Dram[DimmIdx][Byte] == DramByte) {
                CpuByteList[Device] = Byte;
                PdaIdList[Device] = Device;
                DramByte = DramByte + BytesPerDevice;
                break;
              }
            } // Byte
          } // DevId
        } else { // PDA Enumeration before bit/byte swizzle is found
          for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
            if (!MrcByteExist (MrcData, Controller, Channel, Byte)) {
              continue;
            }
            // Initialize CpuByteList and PdaIdList for first PDA Enumeration
            CpuByteList[Byte] = Byte;
            PdaIdList[Byte] = Byte;

            // Set Inital values for Byte mapping
            ChannelIn->DqsMapCpu2Dram[DimmIdx][Byte] = Byte;
            // Use max for NumDevices, until the Byte Swizzle detection is completed.
            NumDevices = Outputs->SdramCount;
          } // Byte
        }

        // Enter PDA Enumerate ID
        Status = MrcIssueMpc (MrcData, Controller, Channel, Rank, DDR5_MPC_ENTER_PDA_ENUM_PROG_MODE, MRC_PRINTS_OFF);
        if(Status != mrcSuccess) {
            return Status;
        }

        MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocRankOverrideEn, WriteToCache, &GetSetEn);
        GetSetVal = Rank;
        MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocRankOverrideVal, WriteToCache, &GetSetVal);
        MrcFlushRegisterCachedData (MrcData);

        // Set up each device
        for (Device = 0; Device < NumDevices; Device++) {
          // Set DQ Pins LOW
          MrcGetSetChStrb (MrcData, Controller, Channel, CpuByteList[Device], GsmIocDqOverrideData, WriteToCache, &GetSetDis);
          MrcFlushRegisterCachedData (MrcData);

          // Send PDA Enumerate ID
          Status = MrcIssueMpc (MrcData, Controller, Channel, Rank, DDR5_MPC_PDA_ENUMERATE_ID (PdaIdList[Device]), MRC_PRINTS_OFF);
          if(Status != mrcSuccess) {
            return Status;
          }
          tPdaDelay = MrcGetTpdaDelay (MrcData);
          MrcWait (MrcData, tPdaDelay * MRC_TIMER_1NS);

          // Set DQ Pins back to HIGH
          GetSetVal = 0xFF;
          MrcGetSetChStrb (MrcData, Controller, Channel, CpuByteList[Device], GsmIocDqOverrideData, WriteToCache, &GetSetVal);
          MrcFlushRegisterCachedData (MrcData);
        }

        MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocRankOverrideEn, WriteToCache, &GetSetDis);
        MrcGetSetChStrb (MrcData, Controller, Channel, MAX_SDRAM_IN_DIMM, GsmIocRankOverrideVal, WriteToCache, &GetSetDis);
        MrcFlushRegisterCachedData (MrcData);

        //Exit PDA Enumerate ID
        Status = MrcIssueMpc (MrcData, Controller, Channel, Rank, DDR5_MPC_EXIT_PDA_ENUM_PROG_MODE, MRC_PRINTS_OFF);
        if (Status != mrcSuccess) {
          return Status;
        }
      }
    }
  }

  // Disable DQ override
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn,   WriteToCache, &GetSetDis);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteToCache, &GetSetDis);

  MrcGetSetNoScope (MrcData, GsmIocDfiCmdDecoderEn, WriteToCache, &GetSetEn);

  // Restore ODTParkMode
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDataDqsOdtParkMode, WriteToCache, &OdtParkModeSave);

  // Restore all tCWL settings
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctCWL, WriteToCache, &TcwlSave);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoRdEnTcwlDelay, WriteToCache, &tCWL4TxDqFifoRdEnSave[Controller][Channel]);
      MrcGetSetMcCh (MrcData, Controller, Channel, TxDqFifoWrEnTcwlDelay, WriteToCache, &tCWL4TxDqFifoWrEnSave[Controller][Channel]);
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
            MrcGetSetStrobe (MrcData, Controller, Channel, Rank, Byte, TxDqsDelay, WriteToCache, &TxDqsSave[Controller][Channel][Rank][Byte]);
          }
        }
        MrcGetSetChStrb (MrcData, Controller, Channel, Byte, TxDqsOffset, WriteToCache, &TxDqsOffsetSave[Controller][Channel][Byte]);
      }
    }
  }

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocTxPiPwrDnDis, WriteToCache, &TxPiPwrDnDis);

  MrcFlushRegisterCachedData (MrcData);

  // Restore OdtForceQDrvEn
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceOdtOn, ForceWriteCached, &ForceOdtOnSave);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceRxAmpOn, ForceWriteCached, &ForceRxOnSave);

  return Status;
}

/**
  This function returns type of DimmOdt for DDR5 based on OdtMatrix and Read/Write mode

  Returns OptDimmOdtPark if OdtMatrix is 0
          OptDimmOdtNomRd if OdtMatrix is 1 and OdtType is ReadOdt
          OptDimmOdtNomWr if OdtMatrix is 1 and OdtType is WriteOdt

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      Controller    - Controller to setup
  @param[in]      Channel       - Channel to setup
  @param[in]      Rank          - Rank to setup
  @param[in]      OdtType       - ReadOdt or WriteOdt
  @param[in]      NtRank        - Non Target Rank

  @retval OdtParam
**/
UINT8
MrcGetOdtParam (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OdtType,
  IN UINT8                NtRank
  )
{
  MrcOutput        *Outputs;
  INT64            OdtMatrix;
  UINT8            NtOdt;

  Outputs = &MrcData->Outputs;

  MRC_DEBUG_ASSERT (Outputs->IsDdr5, &Outputs->Debug, "Unsupported DRAM Type: %d\n", Outputs->DdrType);

  MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, (OdtType == ReadOdt) ? GsmMccOdtMatrixRd : GsmMccOdtMatrixWr, ReadFromCache, &OdtMatrix);
  NtOdt = ((UINT8) OdtMatrix >> NtRank) & (MRC_BIT0);

  return ((NtOdt == 0) ? OptDimmOdtPark : ((OdtType == ReadOdt) ? OptDimmOdtNomRd : OptDimmOdtNomWr));
}

/**
  Override CS to the input CsOverrideVal value.

  @param[in] MrcData       - Pointer to MRC global data.
  @param[in] CsOverrideVal - Value to override CS to. Any non-zero value sets CS high.
  @param[in] CsOverrideEn  - Input specifying whether to enable or disable CS Override.
**/
void
OverrideCs (
  IN MrcParameters *const MrcData,
  IN UINT8                CsOverrideVal,
  IN BOOLEAN              CsOverrideEn
  )
{
  MrcOutput     *Outputs;
  INT64         ValidRankBitMask;
  INT64         OverrideValRankBitMask;
  INT64         OverrideEnRankBitMask;
  INT32         Channel;
  UINT32        Controller;

  Outputs = &MrcData->Outputs;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        ValidRankBitMask = Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;
        OverrideValRankBitMask = (CsOverrideVal == 0) ? 0 : ValidRankBitMask;
        OverrideEnRankBitMask = (CsOverrideEn == FALSE) ? 0 : ValidRankBitMask;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCsOverrideVal0, WriteToCache, &OverrideValRankBitMask);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCsOverride0, WriteToCache, &OverrideEnRankBitMask);
        if (Outputs->IsDdr5 && Outputs->GearMode) {
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCsOverrideVal1, WriteToCache, &OverrideValRankBitMask);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCsOverride1, WriteToCache, &OverrideEnRankBitMask);
        }
      }
    } // Channel
  } // Controller
  MrcFlushRegisterCachedData (MrcData);
}

/**
  Helper function to check whether a DIMM supports a given ARFM level
  via reading a corresponding SPD byte.

   @param[in]  RequestedArfmLevel    - ARFM level requested
   @param[in]  Spd                   - DDR5 SPD structure pointer

   @retval - TRUE if ARFM is supported for a requested level
   @retval - FALSE if ARFM is not supported for a requested level
**/
BOOLEAN
Ddr5IsArfmSupportedForRequestedLevel (
  IN MrcDramRfmModeType RequestedArfmLevel,
  IN MrcSpdDdr5         *Spd
  )
{
  SPD5_ARFM_BYTE_1_STRUCT FirstByte;

  switch (RequestedArfmLevel) {
    case ArfmLevelA:
      FirstByte.Data = Spd->Base.tARFM_A_Byte1.Data;
      break;

    case ArfmLevelB:
      FirstByte.Data = Spd->Base.tARFM_B_Byte1.Data;
      break;

    case ArfmLevelC:
      FirstByte.Data = Spd->Base.tARFM_C_Byte1.Data;
      break;

    default:
      return FALSE;

  }

  return (FirstByte.Bits.ArfmLevel == 1);
}

/**
  Check if Adaptive Refresh Management (ARFM) supported on a DIMM
  for a user requested ARFM level (A, B or C) or lower.

   @param[in]  MrcData               - Pointer to global MRC data
   @param[in]  Controller            - MC number
   @param[in]  Channel               - Channel number
   @param[in]  Dimm                  - DIMM number
   @param[out] ArfmLevelToConfigure  - Maximum supported ARFM Level lower than requested,
                                       only valid if function returns TRUE

   @retval - TRUE if ARFM is supported
   @retval - FALSE if ARFM is not supported
**/
BOOLEAN
MrcDdr5IsArfmSupported (
  IN  MrcParameters*      const MrcData,
  IN  UINT32              Controller,
  IN  UINT32              Channel,
  IN  UINT32              Dimm,
  OUT MrcDramRfmModeType  *ArfmLevelToConfigure
  )
{
  MrcDebug *Debug;
  MrcSpdDdr5 *Spd;
  const MrcInput* Inputs = &MrcData->Inputs;
  MrcDramRfmModeType ArfmLevelToCheck = Inputs->ExtInputs.Ptr->DramRfmMode;

  BOOLEAN ArfmSupported = FALSE;
  BOOLEAN DoneChecking  = FALSE;

  Debug = &MrcData->Outputs.Debug;
  Spd = &MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data.Ddr5;

  if (ArfmLevelToCheck < ArfmLevelA || ArfmLevelToCheck > ArfmLevelC) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MrcDdr5IsArfmSupported called with incorrect ARFM Level %u\n", ArfmLevelToCheck);
    return FALSE;
  }

  while (!DoneChecking) {
    ArfmSupported = Ddr5IsArfmSupportedForRequestedLevel (ArfmLevelToCheck, Spd);
    if (ArfmSupported || (ArfmLevelToCheck == ArfmLevelA && !ArfmSupported)) {
      DoneChecking = TRUE;
    } else {
      ArfmLevelToCheck = (UINT8) ArfmLevelToCheck - 1;
    }
  }

  *ArfmLevelToConfigure = ArfmLevelToCheck;

  return ArfmSupported;
}

/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR58

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcDdr5IsRfmRequired (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  BOOLEAN                       RfmRequired;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_58_TYPE    Ddr5MR58;

  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  RfmRequired = FALSE;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR58, MrrResult);
    Ddr5MR58.Data8 = MrrResult[0];

    if (!Ddr5MR58.Bits.RfmRequired) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM is not required\n");
      continue;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM required\n");
    RfmRequired = TRUE;
  }
  return RfmRequired;
}

/**
  Grab Refresh Management (RFM) threshold info from MR59
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] Raaimt          - RAA Initial Management Threshold (RAAIMT)
   @param[out] Raammt          - RAA Maximum Management Threshold (RAAMMT)
   @param[out] RefSub          - Decrement for RAA from RFMab or RFMpb commands (RAAIMT * RAADEC)
   @param[out] NormalRefSub    - Decrement for RAA from normal refresh commands (RAAIMT)

   @retval - mrcFail if bad RAAIMT value
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcDdr5GetRfmThresholdInfo (
  IN  MrcParameters *const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  MrcStatus                  Status;
  MrcOutput                  *Outputs;
  MrcDebug                   *Debug;
  UINT32                     Rank;
  UINT32                     RankInDimm;
  UINT32                     RankRaaimt;
  UINT32                     RankRaa;
  UINT8                      MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_58_TYPE Ddr5MR58;
  DDR5_MODE_REGISTER_59_TYPE Ddr5MR59;

  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Debug           = &Outputs->Debug;

  RfmSetupConfig->RaaimtOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MAX;
  RfmSetupConfig->RaammtOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MAX;
  RfmSetupConfig->RefSubOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MAX;
  RfmSetupConfig->NormalRefSubOneDimm = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MAX;
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u C%u Dimm%u R%u: ", Controller, Channel, Dimm, Rank);

    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR58, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }
    Ddr5MR58.Data8 = MrrResult[0];

    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR59, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }
    Ddr5MR59.Data8 = MrrResult[0];

    // Still check values just in case (would mean RFM cannot be required)
    if (Ddr5MR58.Bits.Raaimt <= 3 || Ddr5MR58.Bits.Raaimt >= 11) {
      // Raaimt: 0~3, 11~15 are RFU
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Ddr5 MR58.Raaimt bad value: %u\n", Ddr5MR58.Bits.Raaimt);
      return mrcFail;
    }
    if ((Ddr5MR58.Bits.Raammt <= 2) || (Ddr5MR58.Bits.Raammt >= 7)) {
      // Raammt: 0~2, 7 are RFU
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Ddr5 MR58.Raammt bad value: %u\n", Ddr5MR58.Bits.Raammt);
      return mrcFail;
    }

    // DDR5 should always use Fine Granularity Refresh numbers
    // RankRaa (RAAMMT) value will be later adjusted based on whether pTRR will be enabled:
    // RAAMMT-1 if pTRR is not enabled
    // RAAMMT-3 if pTRR is enabled
    RankRaaimt = Ddr5MR58.Bits.Raaimt * 4;
    RankRaa    = (RankRaaimt * Ddr5MR58.Bits.Raammt * 2);
    switch (Ddr5MR59.Bits.RfmRaaCounter) {
      case 0:
        RfmSetupConfig->NormalRefSubOneDimm = MIN (RfmSetupConfig->NormalRefSubOneDimm, RankRaaimt);
        break;
      case 1:
        RfmSetupConfig->NormalRefSubOneDimm = MIN (RfmSetupConfig->NormalRefSubOneDimm, RankRaaimt / 2);
        break;
      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5 MR59.RfmRaaCounter bad value: %u\n", Ddr5MR59.Bits.RfmRaaCounter);
        continue;
        break;
    }

    RfmSetupConfig->RaaimtOneDimm = MIN (RfmSetupConfig->RaaimtOneDimm, RankRaaimt);
    RfmSetupConfig->RaammtOneDimm = MIN (RfmSetupConfig->RaammtOneDimm, RankRaa);
    RfmSetupConfig->RefSubOneDimm = MIN (RfmSetupConfig->RefSubOneDimm, RankRaaimt);
  }
  return Status;
}

/**
  Check if Directed Refresh Management (DRFM) is supported on a DIMM.
  Returns true if any rank on a DIMM supports DRFM.

   @param[in]  MrcData        - Pointer to global MRC data
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - TRUE if at least one rank on the DIMM supports DRFM
   @retval - FALSE if no ranks on the DIMM support DRFM
**/
BOOLEAN
MrcDdr5IsDrfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_59_TYPE    Ddr5MR59;
  BOOLEAN                       DrfmSupported = FALSE;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR59, MrrResult);
    Ddr5MR59.Data8 = MrrResult[0];

    if (Ddr5MR59.Bits.DRFE == 1) {
      DrfmSupported = TRUE;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM is supported\n");
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM is not supported\n");
      continue;
    }
  }

  return DrfmSupported;
}

/**
  Configure Adaptive Refresh Management (ARFM) in DRAM.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - Dimm number
   @param[in]  ArfmLevelToConfigure - ARFM level to configure in DRAM

   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcDdr5ConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  IN  MrcDramRfmModeType ArfmLevelToConfigure
  )
{
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_59_TYPE    Ddr5MR59;

  MrcDebug *Debug;
  Debug = (MrcDebug *) &MrcData->Outputs.Debug;
  MrcStatus Status = mrcSuccess;

  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u C%u Dimm%u R%u: ", Controller, Channel, Dimm, Rank);

    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR59, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }

    Ddr5MR59.Data8 = MrrResult[0];
    Ddr5MR59.Bits.ARFM = ArfmLevelToConfigure;

    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR59, Ddr5MR59.Data8, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM Level configured.\n");
  }

  return Status;
}

/**
  Configure Directed Refresh Management (DRFM) Blast Radius Configuration (BRC) in DRAM.

   @param[in]  MrcData          - Pointer to global MRC data
   @param[in]  Controller       - MC index
   @param[in]  Channel          - Channel index
   @param[in]  Dimm             - Dimm index
   @param[out] BrcSupported     - True, if BRC is supported

   @retval - mrcSuccess if DRFM configured properly
**/
MrcStatus
MrcDdr5ConfigDrfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT BOOLEAN *BrcSupported
  )
{
  UINT32                        RankInDimm;
  UINT32                        Rank;
  DDR5_MODE_REGISTER_59_TYPE    Ddr5MR59;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];

  MrcInput *Inputs = &MrcData->Inputs;
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcDebug *Debug;
  Debug = (MrcDebug *) &MrcData->Outputs.Debug;
  Outputs->DrfmBrc = Inputs->ExtInputs.Ptr->DrfmBrc;

  MrcStatus Status  = mrcSuccess;

  *BrcSupported = TRUE; // For DDR5, at least BRC2 is supported
  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u C%u Dimm%u R%u Blast Radius Configuration (BRC):\n",
                   Controller, Channel, Dimm, Rank);

    Status = MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR59, MrrResult);
    if (Status != mrcSuccess) {
      return Status;
    }

    Ddr5MR59.Data8 = MrrResult[0];

    if (Ddr5MR59.Bits.BRCSupport) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tOnly BRC2 is supported.\n");
      Outputs->DrfmBrc = DrfmBrc2;
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tBRC set to Blast Radius Level %d\n", Outputs->DrfmBrc);
    Ddr5MR59.Bits.BRC = ConvertDrfmBrcToTMrcJedecBrcType (Outputs->DrfmBrc);
    Ddr5MR59.Bits.DRFE = 1;

    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR59, Ddr5MR59.Data8, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  return mrcSuccess;
}

/**
  Perform JEDEC reset sequence for DDR5.

  @param[in] MrcData - Include all MRC global data.

  @retval - mrcSuccess if no errors encountered, mrcFail otherwise
**/
MrcStatus
MrcJedecResetDdr5 (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus         Status;
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcTiming         *Timing;
  INT64             GetSetVal;
  UINT32            tInit1;
  UINT32            tInit3;
  UINT32            tInit4;
  UINT32            tInit5;
  UINT32            tInit5InNck;
  UINT32            CsOverrideDelay;
  UINT32            tXS;
  UINT32            tXPR;
  UINT32            FirstController;
  UINT8             FirstChannel;
  MrcProfile        Profile;
  static const MrcModeRegister JedecResetExitSequence[] = {
    mpcMR13,
    mpcDllReset,
    mpcZqCal,
    mpcZqLat,
    mrEndOfSequence
    };

  Inputs            = &MrcData->Inputs;
  Outputs           = &MrcData->Outputs;
  FirstController   = Outputs->FirstPopController;
  FirstChannel      = Outputs->Controller[FirstController].FirstPopCh;
  Profile           = Inputs->ExtInputs.Ptr->MemoryProfile;
  Timing            = &Outputs->Controller[FirstController].Channel[FirstChannel].Timing[Profile];
  tXS               = MrcGetTxsr (MrcData, Timing);
  Status            = mrcSuccess;

  // Init Timings in Nanoseconds
  tInit1 = MRC_DDR5_tINIT1_US * MRC_TIMER_1US;
  tInit3 = MRC_DDR5_tINIT3_MS * MRC_TIMER_1MS;
  tInit4 = MRC_DDR5_tINIT4_US * MRC_TIMER_1US;
  tInit5InNck = MRC_DDR5_tINIT5_NCK;
  // Convert nCK to FS
  tInit5 = (tInit5InNck * Outputs->MemoryClock);
  tInit5 = DIVIDECEIL (tInit5, FEMTOSECONDS_PER_NANOSECOND);
  tXPR   = (tXS * Outputs->MemoryClock);
  tXPR   = DIVIDECEIL (tXPR, FEMTOSECONDS_PER_NANOSECOND);
  CsOverrideDelay = (3 * Outputs->MemoryClock); //3tCK
  CsOverrideDelay = DIVIDECEIL (CsOverrideDelay, FEMTOSECONDS_PER_NANOSECOND);

  // Enable Multicycle CS on the MC which is the DRAM's default MPC mode after DRAM Reset
  EnableMcMulticycleCs (MrcData);

  // Force CKE_ON before DRAM Reset
  // Even though there is no CKE pin on DDR5, CKE override is used
  // to prevent power down entry in all technologies.
  MrcCkeOnProgramming (MrcData);
  MrcGetMcConfigGroupLimits (MrcData, GsmMccCkeOverride, NULL, &GetSetVal, NULL);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOverride, WriteCached, &GetSetVal);

  // CA Tristate is diabled in MrcMcConfiguration
  // Disabling CA forces CA[4:0] high when the CA bus is inactive.
  // This allows us to issue multiple cycles of NOP commands by simply
  // driving CS_N low using OverrideCs();

  // Check if this is the first time we are performing JEDEC RESET
  if (Outputs->JedecInitDone){
    // Reset Initialization with Stable Power:
    // Only need to assert RESET for 1us (tPW_RESET)
    tInit1 = MRC_TIMER_1US;
  }

  // Override Vdd / Vddq / Vpp using PMIC
  if (!Outputs->VoltageDone) {
    MrcEnableDimmPmic (MrcData);
    MrcDdr5VoltageCheckAndSwitch (MrcData);
  }

  // Assert DIMM reset signal
  GetSetVal = 1;
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetVal);

  // Override CS_N to go LOW
  OverrideCs (MrcData, 0, TRUE);

  // After tINIT1, bring Dram out of Reset
  MrcWait (MrcData, tInit1);
  GetSetVal = 0;
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetVal);


  // After tINIT3, override CS_N to go HIGH
  MrcWait (MrcData, tInit3);
  OverrideCs (MrcData, 1, TRUE);

  // Wait for tINIT4 time for CS_N to remain HIGH
  MrcWait (MrcData, tInit4);

  // At tINIT4, issue NOP command by driving CS_N low for tINIT5
  OverrideCs (MrcData, 0, TRUE);
  MrcWait (MrcData, tInit5);

  // After tINIT5, override CS_N to go HIGH
  OverrideCs (MrcData, 1, TRUE);

  // Disabling the CS Override causes CA[5:13] to go low.
  // Wait at least 3tCK after setting CS_N high
  // to avoid CA transitioning during the NOP.
  MrcWait (MrcData, CsOverrideDelay);
  OverrideCs (MrcData, 1, FALSE);

  // Step8: Wait for tXPR timings before sending any command
  MrcWait (MrcData, tXPR);

  if ((Inputs->BootMode == bmCold) || (Inputs->BootMode == bmFast)) {
    if (PerformGenericMrsFsmSequence (MrcData, JedecResetExitSequence, FALSE) != mrcSuccess) {
      Status = mrcFail;
    }
  }

  return Status;
}

/**
  This function completes setting up the Generic MRS FSM configuration to enable SAGV during normal operation.

  @param[in] MrcData  - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
**/
MrcStatus
MrcFinalizeDdr5MrSeq (
  IN  MrcParameters *const MrcData
  )
{
  MrcStatus                   Status;

  static const MrcModeRegister Ddr5SagvMrOrder[] = {
    mrPreAll,
    // MR13 tCCD_L is different per speed. The shadow register value is updated
    // here but the new setting is applied automatically during the SREF flow.
    // The mpcDllReset command is not required to apply the new setting.
    mpcMR13,
    mpcSetCmdTiming,
    mpcMR32a0,
    mpcMR32a1,
    mpcMR33a0,
    mpcMR32b0,
    mpcMR32b1,
    mpcMR33b0,
    // CA and CS VREF shadow register value is updated here. The new setting is
    // applied automatically during the SREF flow. the mpcApplyVrefCa command
    // is not required to apply then new setting.
    mrMR11,
    mrMR12,
    // Self Refresh Entry w/ Frequency Change (SREF) starts after mrMR12
    // FSM will issue remaining entries at the new target frequency after SREF exit
    mpcMR33,
    mpcMR34,
    mrMR0,
    mrMR2,
    mrMR4,
    mrMR5,
    mrMR6,
    mrMR8,
    mrMR34,
    mrMR35,
    mrMR36,
    mrMR37,
    mrMR38,
    mrMR39,
    mrMR40,
    mrMR45,
    // PDA MRs start from here
    mrMR129,
    mrMR130,
    mrMR131,
    mrMR132,
    mrMR137,
    mrMR138,
    mrMR139,
    mrMR140,
    mrMR145,
    mrMR146,
    mrMR147,
    mrMR148,
    mrMR153,
    mrMR154,
    mrMR155,
    mrMR156,
    mrMR161,
    mrMR162,
    mrMR163,
    mrMR164,
    mrMR169,
    mrMR170,
    mrMR171,
    mrMR172,
    mrMR177,
    mrMR178,
    mrMR179,
    mrMR180,
    mrMR185,
    mrMR186,
    mrMR187,
    mrMR188,
    mrMR193,
    mrMR194,
    mrMR195,
    mrMR196,
    mrMR201,
    mrMR202,
    mrMR203,
    mrMR204,
    mrMR209,
    mrMR210,
    mrMR211,
    mrMR212,
    mrMR217,
    mrMR218,
    mrMR219,
    mrMR220,
    mrMR225,
    mrMR226,
    mrMR227,
    mrMR228,
    mrMR233,
    mrMR234,
    mrMR235,
    mrMR236,
    mrMR241,
    mrMR242,
    mrMR243,
    mrMR244,
    mrMR249,
    mrMR250,
    mrMR251,
    mrMR252,
    mrMR10,
    mrMR3,
    mrMR7,
    mrMR48,
    mpcSelectAllPDA,
    mrEndOfSequence
  };

  Status = PerformGenericMrsFsmSequence (MrcData, Ddr5SagvMrOrder, TRUE);

  return Status;
}


/**
  This function decodes and returns the DDR5 ODT offsets from MR37, MR38, and MR39

  @param[in]  MrcData          - Include all MRC global data.
  @param[in]  Controller       - Current Controller
  @param[in]  Channel          - Current Channel
  @param[in]  Rank             - Current Rank
  @param[in]  NtOdt_Wr         - TRUE if Non Target WR ODT is supported
  @param[in] IsPreFuncTraining - TRUE if board flight delay impact from Read Leveling / Write Leveling is not known yet
  @param[out] Odt_Wr_On        - Hold the value for Odt_Wr_On Offset
  @param[out] Odt_Wr_Off       - Hold the value for Odt_Wr_Off Offset
  @param[out] Odt_Rd_On        - Hold the value for Odt_Rd_On Offset
  @param[out] Odt_Rd_Off       - Hold the value for Odt_Rd_Off Offset
**/
VOID
GetDdr5OdtOffsets (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  BOOLEAN              NtOdt_Wr,
  IN  BOOLEAN              IsPreFuncTraining,
  OUT INT8                 *Odt_Wr_On,
  OUT INT8                 *Odt_Wr_Off,
  OUT INT8                 *Odt_Rd_On,
  OUT INT8                 *Odt_Rd_Off
  )
{
  MrcOutput      *Outputs;
  MrcChannelOut  *ChannelOut;
  UINT32 Dimm;
  UINT32 DimmRank;
  UINT8  *MR;
  INT8 OdtlOnWrNtOffset;
  INT8 OdtlOffWrNtOffset;
  INT8 OdtlOnWrOffset;
  INT8 OdtlOffWrOffset;
  INT8 OdtlOnRdNtOffset;
  INT8 OdtlOffRdNtOffset;
  DDR5_MODE_REGISTER_37_TYPE Ddr5MR37;
  DDR5_MODE_REGISTER_38_TYPE Ddr5MR38;
  DDR5_MODE_REGISTER_39_TYPE Ddr5MR39;

  Outputs    = &MrcData->Outputs;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  Dimm = RANK_TO_DIMM_NUMBER (Rank);
  DimmRank = Rank % MAX_RANK_IN_DIMM;

  MR = ChannelOut->Dimm[Dimm].Rank[DimmRank].MR;

  Ddr5MR37.Data8 = MR[mrIndexMR37];
  OdtlOnWrOffset    = IsPreFuncTraining ? OdtlOnWrOffsetMinus4 : Ddr5MR37.Bits.OdtlOnWrOffset;
  OdtlOffWrOffset   = IsPreFuncTraining ? OdtlOffWrOffsetPlus4 : Ddr5MR37.Bits.OdtlOffWrOffset;

  Ddr5MR38.Data8 = MR[mrIndexMR38];
  OdtlOnWrNtOffset  = IsPreFuncTraining ? OdtlOnWrOffsetMinus4 : Ddr5MR38.Bits.OdtlOnWrNtOffset;
  OdtlOffWrNtOffset = IsPreFuncTraining ? OdtlOffWrOffsetPlus4 : Ddr5MR38.Bits.OdtlOffWrNtOffset;

  Ddr5MR39.Data8 = MR[mrIndexMR39];
  OdtlOnRdNtOffset  = IsPreFuncTraining ? OdtlOnRdNtOffsetMinus3 : Ddr5MR39.Bits.OdtlOnRdNtOffset;
  OdtlOffRdNtOffset = IsPreFuncTraining ? OdtlOffRdNtOffsetPlus3 : Ddr5MR39.Bits.OdtlOffRdNtOffset;

  // The translation between MR unsigned value and signed value is calculated by DDR5_ODT_OFFSET_DECODER
  if (Odt_Wr_On != NULL) {
    *Odt_Wr_On = (NtOdt_Wr ? OdtlOnWrNtOffset : OdtlOnWrOffset) - DDR5_ODT_OFFSET_DECODER;
  }
  if (Odt_Wr_Off != NULL) {
    *Odt_Wr_Off = DDR5_ODT_OFFSET_DECODER - (NtOdt_Wr ? OdtlOffWrNtOffset : OdtlOffWrOffset);
  }
  if (Odt_Rd_On != NULL) {
    *Odt_Rd_On = OdtlOnRdNtOffset - DDR5_ODT_OFFSET_DECODER;
  }
  if (Odt_Rd_Off != NULL) {
    *Odt_Rd_Off = DDR5_ODT_OFFSET_DECODER - OdtlOffRdNtOffset;
  }
}

/**
  This function returns the tRFM in DCLKs.
  DDR5 timing is based on tRFMsb

  @param[in] DeviceCapacity    - DRAM Device Capacity

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetDdr5Trfm (
  IN  UINT32  DeviceCapacity
  )
{
  UINT32  tRFM;

  switch (DeviceCapacity) {
    case 8192:
      tRFM = MRC_DDR5_tRFMSB_8GB_PS;
      break;
    case 16384:
      tRFM = MRC_DDR5_tRFMSB_16GB_PS;
      break;
    case 24576:
      tRFM = MRC_DDR5_tRFMSB_24GB_PS;
      break;
    case 32768:
      tRFM = MRC_DDR5_tRFMSB_32GB_PS;
      break;
    default:
      tRFM = MRC_DDR5_tRFMSB_8GB_PS;
      break;
  }

  return tRFM;
}

/**
  Check if RFM is required through MR58 / MR59. If it requires, save 4 parameters into vars

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number
   @param[out] RAAIMT         - RFM RAAIMT
   @param[out] RAAMMT         - RFM RAAMMT
   @param[out] REF_SUB        - RFM REF_SUB
   @param[out] NORMAL_REF_SUB - RFM NORMAL_REF_SUB

   @retval TRUE RFM is required; RAAIMT and other outputs save the parameter values
   @retval FALSE RFM is not required; RAAIMT and other outputs are changed
**/
BOOLEAN
MrcGetDdr5RowHammeringData (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT UINT32 *RAAIMT,
  OUT UINT32 *RAAMMT,
  OUT UINT32 *REF_SUB,
  OUT UINT32 *NORMAL_REF_SUB
  )
{
  MrcOutput                     *Outputs;
  MrcDebug                      *Debug;
  UINT32                        Rank;
  UINT32                        RankInDimm;
  UINT32                        RankRAAIMT;
  UINT32                        RankRAA;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_58_TYPE    Ddr5MR58;
  DDR5_MODE_REGISTER_59_TYPE    Ddr5MR59;
  BOOLEAN                       RfmRequired;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  RfmRequired = FALSE;

  *RAAIMT         = MRC_UINT32_MAX;
  *RAAMMT         = MRC_UINT32_MAX;
  *REF_SUB        = MRC_UINT32_MAX;
  *NORMAL_REF_SUB = MRC_UINT32_MAX;

  for (RankInDimm = 0; RankInDimm < MAX_RANK_IN_DIMM; RankInDimm++) {
    Rank = Dimm * MAX_RANK_IN_DIMM + RankInDimm;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u C%u Dimm%u R%u: ", Controller, Channel, Dimm, Rank);

    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR58, MrrResult);
    Ddr5MR58.Data8 = MrrResult[0];
    if (Ddr5MR58.Bits.RfmRequired == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM is not required\n");
      continue;
    }

    if (Ddr5MR58.Bits.Raaimt <= 3 || Ddr5MR58.Bits.Raaimt >= 11) {
      // Raaimt: 0~3, 11~15 are RFU
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5 MR58.Raaimt bad value: %u\n", Ddr5MR58.Bits.Raaimt);
      continue;
    }

    if ((Ddr5MR58.Bits.Raammt <= 2) || (Ddr5MR58.Bits.Raammt >= 7)) {
      // Raammt: 0~2, 7 are RFU
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5 MR58.Raammt bad value: %u\n", Ddr5MR58.Bits.Raammt);
      continue;
    }

    // DDR5 should always use Fine Granularity Refresh numbers
    RankRAAIMT = ((UINT32) Ddr5MR58.Bits.Raaimt) * 4;
    RankRAA = RankRAAIMT * Ddr5MR58.Bits.Raammt * 2;


    MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR59, MrrResult);
    Ddr5MR59.Data8 = MrrResult[0];
    switch (Ddr5MR59.Bits.RfmRaaCounter) {
      case 0:
        *NORMAL_REF_SUB = MIN (*NORMAL_REF_SUB, RankRAAIMT);
        break;
      case 1:
        *NORMAL_REF_SUB = MIN (*NORMAL_REF_SUB, RankRAAIMT / 2);
        break;
      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5 MR59.RfmRaaCounter bad value: %u\n", Ddr5MR59.Bits.RfmRaaCounter);
        continue;
        break;
    }

    *RAAIMT = MIN (*RAAIMT, RankRAAIMT);
    *RAAMMT = MIN (*RAAMMT, RankRAA);
    *REF_SUB = MIN (*REF_SUB, RankRAAIMT);
    RfmRequired = TRUE;
  }
  return RfmRequired;
}

/**
  This function changes the DDR5 DIMM Voltages using PMIC.

  @param[in, out] MrcData - The MRC "global data" area.
**/
VOID
MrcDdr5VoltageCheckAndSwitch (
  IN OUT MrcParameters      *MrcData
  )
{
  MrcInput          *Inputs;
  MRC_FUNCTION      *MrcCall;
  MrcOutput         *Outputs;
  MrcSaveData       *SaveData;
  MrcDebug          *Debug;
  MrcControllerIn   *ControllerIn;
  MrcControllerOut  *ControllerOut;
  MrcChannelOut     *ChannelOut;
  MrcChannelIn      *ChannelIn;
  const MrcDimmIn   *DimmIn;
  MrcDimmOut        *DimmOut;
  MrcProfile        MemoryProfile;
  MrcVddSelect      Vdd;
  MrcVddSelect      Vddq;
  MrcVddSelect      Vpp;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             Dimm;
  UINT8             SpdAddress;
  UINT8             Value;
  UINT32            Offset;
  RETURN_STATUS     Status;

  //Below are used to check duplicate configuration as
  //one Ddr5 dimm is plugged into 2 channels
  UINT8             DimmConfigured[MAX_CONTROLLER * MAX_DDR5_CHANNEL * MAX_DIMMS_IN_CHANNEL];
  UINT8             DimmNextFree;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  SaveData = &MrcData->Save.Data;
  MrcCall  = Inputs->Call.Func;
  Debug    = &Outputs->Debug;
  MemoryProfile = Inputs->ExtInputs.Ptr->MemoryProfile;

  if (SaveData->IsXmpSagvEnabled || SaveData->DynamicMemoryBoostTrainingFailed) {
    // Use XMP Profile, so both SAGV points use the same voltages of DIMM
    MemoryProfile = Inputs->MemoryProfileSave;
  }

  if ((Outputs->VddVoltage[MemoryProfile]  == VDD_INVALID) ||
      (Outputs->VddqVoltage[MemoryProfile] == VDD_INVALID) ||
      (Outputs->VppVoltage[MemoryProfile]  == VDD_INVALID)) {
    return;
  }

  MrcCall->MrcSetMem (DimmConfigured, sizeof (DimmConfigured), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn  = &Inputs->Controller[Controller];
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelOut  = &ControllerOut->Channel[Channel];
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (ChannelOut->Dimm[Dimm].Status != DIMM_PRESENT) {
          continue;
        }
        DimmIn  = &ChannelIn->Dimm[Dimm];
        DimmOut = &ChannelOut->Dimm[Dimm];

        SpdAddress = DimmIn->SpdAddress;
        if (SpdAddress == 0) {
          // In case of fixed spd file
          continue;
        }

         // Check if it's already configured
        for (DimmNextFree = 0; DimmNextFree < sizeof(DimmConfigured); DimmNextFree++) {
          if (0 == DimmConfigured[DimmNextFree]) {
            break;
          }

          if (DimmConfigured[DimmNextFree] == SpdAddress) {
            // Already configured
            DimmNextFree = 0xFF;
            break;
          }
        }

        if (DimmNextFree >= sizeof (DimmConfigured)) {
          continue;
        }

        Offset = SPD5_MR5;
        Value = MrcCall->MrcSmbusRead8 (SpdAddress | (Offset << 8), &Status);
        if ((Value & SPD5_MR5_HUB_SUPPORT) == 0) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Dimm SpdAddress 0x%x: Doesn't support HUB\n", SpdAddress);
          continue;
        }

        Vdd  = DimmOut->VddVoltage[MemoryProfile];
        Vddq = DimmOut->VddqVoltage[MemoryProfile];
        Vpp  = DimmOut->VppVoltage[MemoryProfile];
        if (DimmOut->PmicProgrammable) {
          // Call default config function which assumes only one PMIC
          MrcDefaultSetMemoryPmicVoltage (MrcData, SpdAddress, DimmOut->IsPmicSupport10MVStep, Vdd, Vddq, Vpp);
        }
        DimmConfigured[DimmNextFree] = SpdAddress;
      } // For Dimm
    } // For Channel
  } // For Controller

  Outputs->VoltageDone = TRUE;
}
