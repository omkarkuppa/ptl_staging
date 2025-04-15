/** @file
  The third stage of the write training is determining the PI setting for
  each byte strobe to make sure that data is sent at the optimal location.
  In order to do that a pattern of alternating zeros and ones is written to
  a block of the memory, and then read out.  By identifying the location
  where it is farthest away from where errors are shown the DQS will be
  aligned to the center of the eye.

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
//
// Include files
//
#include "MrcWriteDqDqs.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcDdr5.h"
#include "MrcLpddr5Registers.h"
#include "MrcCpgcOffsets.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "MrcChipApi.h"
#include "MrcGeneral.h"
#include "MrcCommon.h"
#include "MrcDdrIoDefines.h"
#include "MrcIoTest.h"
#include "BlueMrcJedecApi.h"  // for MrcGetTosco()
#include "BlueMrcIoTest.h"

/**
  Perform LP5/DDR5 WrDqDqs Re-Training.

  @param[in] MrcData       - Include all MRC global data.

  @retval MrcStatus        - mrcSuccess otherwise a failure occurred
**/
MrcStatus
MrcWriteDqDqsReTraining (
  IN     MrcParameters *const MrcData
  )
{
  MrcStatus         Status;
  MrcStatus         SubStatus;
  MrcOutput         *Outputs;
  MrcDebug          *Debug;
  UINT32            FirstController;
  UINT32            FirstChannel;
  UINT32            Controller;
  UINT8             Channel;
  UINT8             Rank;
  UINT8             MaxDevice;
  UINT8             MaxByte;
  UINT8             Device;
  UINT8             Byte;
  UINT8             ByteInChannel;
  UINT8             MrrOscLsbResult[MRC_MRR_ARRAY_SIZE];
  UINT8             MrrOscMsbResult[MRC_MRR_ARRAY_SIZE];
  UINT32            ReTrainDuration;
  UINT8             DqioDuration;
  UINT16            RORunTime;
  UINT16            RunTimeClocksBy16;
  INT64             GetSetVal;
  INT64             GetSetEn;
  INT64             GetSetDis;
  BOOLEAN           IsLpddr;
  UINT32            tMpcNck;
  UINT32            tMpcNckFs;
  UINT32            tMpcNs;
  UINT32            tOscoNck;
  UINT32            tOscoNckFs;
  UINT32            tOscoNs;
  DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT  MiscControl2;

  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;
  IsLpddr      = Outputs->IsLpddr;
  DqioDuration = 0;
  GetSetEn     = 1;
  GetSetDis    = 0;
  MaxDevice    = (Outputs->LpByteMode) ? 2 : 1;
  MaxByte      = IsLpddr ? ((Outputs->LpByteMode) ? 1 : 2) : Outputs->SdramCount;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;

  Status = MrcGetDqioDuration (MrcData, &DqioDuration, &RunTimeClocksBy16);
  if (Status == mrcSuccess) {
    // Convert nCK/16 to nCK
    RORunTime = RunTimeClocksBy16 * 16;
    ReTrainDuration = RORunTime*128; // In steps of UI/128

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DqioDuration = %d\nRORunTime = %d\nReTrainDuration = %d\n", DqioDuration, RORunTime, ReTrainDuration);
    // Write encoded value to DRAM MR
    SubStatus = MrcSetIntervalTimerMr (MrcData, DqioDuration);
    if (SubStatus != mrcSuccess) {
      Status = SubStatus;
    }

    // Send the Start Oscillator MPC
    SubStatus = MrcIssueMpc (
                MrcData,
                MAX_CONTROLLER,
                MAX_CHANNEL,
                MAX_RANK_IN_CHANNEL,
                IsLpddr ? MRC_LP5_MPC_START_WCK2DQI_OSC : DDR5_MPC_START_DQS_INTERVAL_OSC,
                TRUE
                );
    if (SubStatus != mrcSuccess) {
      Status = SubStatus;
    }
    // Delay for the configured oscillator duration
    MrcWaitClk (MrcData, RORunTime);

    // On LP5 Delay an additional tOSCODQI minus the tMPC already covered by MrcIssueMpc
    // DDR5 tOSCS is already covered at this point by tMPC + oscillator duration
    if (IsLpddr) {
      tMpcNck = MrcGetTmod (MrcData, Outputs->MemoryClock);
      tMpcNckFs = tMpcNck * Outputs->MemoryClock;
      tMpcNs = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);

      tOscoNck = MrcGetTosco (MrcData);
      if (Outputs->IsLpddr5) {
        tOscoNck = ((UINT32) tOscoNck) * WCK_TO_CK_RATIO;
      }
      tOscoNckFs = tOscoNck * Outputs->MemoryClock;
      tOscoNs = DIVIDECEIL (tOscoNckFs, FEMTOSECONDS_PER_NANOSECOND);
      if (tOscoNs > tMpcNs) {
        MrcWait (MrcData, tOscoNs - tMpcNs);
      }
    }

    // Enable retraining in the PHY
    MrcGetSetChStrb (MrcData, FirstController, FirstChannel, 0, GsmIocRetrainSwizzleCtlRetrainEn, ReadFromCache, &GetSetVal);
    // Enable Write DQDQS (WCK2DQI) Training
    GetSetVal |= WR_RETRAIN_EN;
    MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainSwizzleCtlRetrainEn, WriteToCache, &GetSetVal);

    // Write the calculated RunTimeClocksBy16 and set the InitTrain that will indicate that HW will update the InitPiCode to registers when we issue MRR
    // RunTimeClocksBy16 is in 16xtCK units, MC expects the value in 16xWCK units for LPDDR5
    GetSetVal = RunTimeClocksBy16 * (IsLpddr ? 4 : 1);
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctWrOsclRuntime, WriteToCache | PrintValue, &GetSetVal);
    MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainCtlDuration,  WriteToCache | PrintValue, &GetSetVal);
    MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainCtlInitTrain, WriteToCache, &GetSetEn);
    if (!IsLpddr) { // DDR5 returns MRR data serially
      MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainSwizzleCtlSerialMrr, WriteToCache, &GetSetEn);
    }
    MrcFlushRegisterCachedData (MrcData);

    MiscControl2.Data = MrcReadCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG);
    MiscControl2.Bits.lpdeltadqstrainmode = 1;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);

    if (Status == mrcSuccess) {
      for (Controller = 0; (Controller < MAX_CONTROLLER); Controller++) {
        for (Channel = 0; (Channel < Outputs->MaxChannels); Channel++) {
          if (!(MrcChannelExist (MrcData, Controller, Channel))) {
            continue;
          }
          for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
            if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
              continue;
            }
            MrcIssueMrr (MrcData, Controller, Channel, Rank, IsLpddr ? LSB_WCK2DQI_OSQ_MR : LSB_DQS_OSQ_MR, MrrOscLsbResult);
            MrcIssueMrr (MrcData, Controller, Channel, Rank, IsLpddr ? MSB_WCK2DQI_OSQ_MR : MSB_DQS_OSQ_MR, MrrOscMsbResult);
            for (Device = 0; Device < MaxDevice; Device++) {
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_NOTE,
                "Mc%d Ch%d R%d: MR%d Device%d= 0x%02X\nMR%d Device%d = 0x%02X\n",
                Controller,
                Channel,
                Rank,
                IsLpddr ? LSB_WCK2DQI_OSQ_MR : LSB_DQS_OSQ_MR,
                Device,
                MrrOscLsbResult[Device],
                IsLpddr ? MSB_WCK2DQI_OSQ_MR : MSB_DQS_OSQ_MR,
                Device,
                MrrOscMsbResult[Device]
                );
              for (Byte = 0; Byte < MaxByte; Byte++) {
                ByteInChannel = (Device * MaxByte) + Byte;
                MrcGetSetStrobe (MrcData, Controller, Channel, Rank, ByteInChannel, GsmIocRetrainInitPiCode,  ReadNoCache | PrintValue, &GetSetVal);
                MrcGetSetStrobe (MrcData, Controller, Channel, Rank, ByteInChannel, WrRetrainDeltaPiCode, ReadNoCache | PrintValue, &GetSetVal);
                MrcGetSetStrobe (MrcData, Controller, Channel, Rank, ByteInChannel, GsmIocRetrainRoCount,     ReadNoCache | PrintValue, &GetSetVal);
              } // for Byte
            } // for Device
          } // for Rank
        } // for Channel
      } // for Controller

      // After all RO values are written, set up the OSC Params per channel
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
          if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
            continue;
          }
          GetSetVal = (IsLpddr ? ZQCS_PERIOD_LPDDR : ZQCS_PERIOD_DDR5) * 128;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRetrainPeriod,     WriteToCache | PrintValue, &GetSetVal);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnWrRetraining,    WriteToCache | PrintValue, &GetSetEn);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, WriteToCache | PrintValue, &GetSetEn);
        } // for Channel
      } // for Controller
      // Reset status and clear InitTrain on each byte
      MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainCtlResetStatus, WriteCached, &GetSetEn);
      MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocRetrainCtlInitTrain,   WriteToCache, &GetSetDis);
      MrcFlushRegisterCachedData (MrcData);
      // Clear WrRetrainOvrd once Write Retraining is Done
      MrcGetSetNoScope (MrcData, GsmIocWrRetrainOvrd, WriteCached | PrintValue, &GetSetDis);
    } // (Status == mrcSuccess)
  } // (Status == mrcSuccess)
  return Status;
}
