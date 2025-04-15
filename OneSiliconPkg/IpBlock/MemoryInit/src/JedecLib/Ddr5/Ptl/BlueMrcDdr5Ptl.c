/** @file
  This file contains code related to initializing and configuring the DDRIO.

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

#include "MrcDdr5.h"
#include "MrcDdrIoApi.h"
#include "MrcDdrIoUtils.h"

/**
  This function issues the PDA Enumeration ID MPC. This will first send a MPC to enter PDA Enumeration Mode.
  It will then iterate through all DRAM devices and send an MPC to assign it an ID number, starting at 0.
  Once the iteration is done, a MPC will be sent to exit PDA Mode. This will wait tMPC Delay between PDA Entry
  and the first PDA Enumerate ID MPC's. This will wait tPDA_DELAY between the PDA Enumerate ID MPC's.

  @param[in]  MrcData       - Pointer to global MRC data

  @retval MrcStatus - mrcTimeout if the FSM does not complete after 1s
  @retval MrcStatus - mrcSuccess if the MPC is sent successfully
  @retval MrcStatus - mrcFail for unexpected failures
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
  GetSetVal = 2; // Park Diff Low
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDataDqsOdtParkMode, WriteToCache, &GetSetVal);

  MrcGetSetNoScope (MrcData, GsmIocDfiCmdDecoderEn, WriteToCache, &GetSetDis);

  MrcFlushRegisterCachedData (MrcData);

  // Set DQ Pins HIGH, to a known state
  MrcConfigureDqPins(MrcData, TRUE);

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
  MrcConfigureDqPins (MrcData, FALSE);

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
