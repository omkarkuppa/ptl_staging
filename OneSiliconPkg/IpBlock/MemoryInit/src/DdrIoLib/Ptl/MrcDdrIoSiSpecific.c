/** @file
  This file contains code for Ddrio Silicon W/As

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

#include "MrcCommon.h"
#include "MrcBlueGreenCommunication.h"
#include "MrcDdrIo.h"
#include "MrcCrosser.h"

/**
  Clear the top 32KB of Xtensa DRAM0 region.
  @param[in] MrcData - Include all MRC global data.
**/
VOID
BlueMrcClearXtensaDramTop (
  IN MrcParameters *const MrcData
  )
{
  MrcInput  *Inputs;
  MrcDebug  *Debug;
  UINT32    BytesWritten;

  Inputs  = &MrcData->Inputs;
  Debug   = &MrcData->Outputs.Debug;

  // Zero out the top 32 KB of DRAM0 manually: [64..96KB]
  if (Inputs->IsDdrIoMbA0) {
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_REG, MRC_UCSS_DRAM_START + (64 * 1024));
    BytesWritten = 0;
    while (BytesWritten < (32 * 1024)) {
      MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_DATA_REG, 0);
      BytesWritten += 4;
      if ((BytesWritten % 8192) == 0) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "."); // Print a dot every 8KB
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }
}

/**
  This function sets the bit of InternalClocksOn in DataControl0 struct.

  @param[in out] DataControl0  - Pointer to DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT.
  @param[in]     Enable        - Bit value.

  @retval mrcSuccess - If Write was successful
**/
MrcStatus
MrcSetInternalClocksOn (
  IN OUT DATA0CH0_CR_DDRCRDATACONTROL0_STRUCT *DataControl0,
  IN BOOLEAN Enable
  )
{
  if (DataControl0 == NULL) {
    return mrcWrongInputParameter;
  }

  DataControl0->Bits.InternalClocksOn = Enable ? 1 : 0;

  return mrcSuccess;
}

/**
  This function gets the LaneEn value for data partition.

  @param[in] MrcData    - Include all MRC global data.

  @returns LaneEn value
**/
UINT32
MrcGetDataLaneEn (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  UINT32        LaneEn;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
#ifndef HVM_MODE
    LaneEn = 0x0FF; //No TxDqs Lane
#else
    // In HVM LPDDR5 enable TxDqs to feed to RxDqs
    LaneEn = 0x2FF;
#endif
  } else {
    LaneEn = 0x2FF;
  }

  if (Inputs->IsDdrIoMbA0) {
    LaneEn = 0x0FF;
  }

  return LaneEn;
}

/**
  Set TerDcd Override Values

  @param[in] MrcData      - Include all MRC global data.
  @param[in]  Controller  - Memory controller in the processor socket (0-based).
  @param[in]  Channel     - DDR Channel Number within the memory controller (0-based).
  @param[in]  Byte        - DDR Byte (0-based).
  @param[in]  IsEnable    - TRUE if override values should be set

**/
VOID
MrcOverrideTerDcd (
  IN OUT MrcParameters* const MrcData,
  UINT8     Controller,
  UINT8     Channel,
  UINT8     Byte,
  BOOLEAN   IsEnable
  )
{
  INT64 OvrVal;
  INT64 OvrEn;
  INT64 TargetSel;

  if (IsEnable) {
    OvrVal    = 3;
    OvrEn     = 1;
    TargetSel = 1;
  } else {
    OvrVal    = 0;
    OvrEn     = 0;
    TargetSel = 0;
  }

  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocITerdcdselovrval, WriteCached, &OvrVal);
  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocITerdcdselovren,  WriteCached, &OvrEn);
  MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmDccDllDcdTargetSel,  WriteCached, &TargetSel);
}

/**
  Enable running Voc Search in seseamp offset training or not

  @param[in] MrcData    - Include all MRC global data.

  @returns TRUE
**/
BOOLEAN
EnableSenseAmpOffsetVocSearch (
  IN OUT MrcParameters *const MrcData
  )
{
  return TRUE;
}

/**
  Override NUI for SOT Vref Search

  @param[in]      MrcData  - Include all MRC global data.
  @param[in]      Enable   - Enable or disable the override
  @param[in, out] Override - Save/restore struct
**/
VOID
SenseAmpOffsetOverrideNui (
  IN MrcParameters           *const MrcData,
  IN BOOLEAN                        Enable,
  IN OUT SenseAmpTrainingOverride   *Override
  )
{
  MrcOutput *Outputs;
  INT64     DqsOffsetNUI;
  UINT32    FirstController;
  UINT32    FirstChannel;
  UINT32    Byte;

  Outputs = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;
  Byte = 0;


  if (Enable) {
    MrcGetSetChStrb (MrcData, FirstController, FirstChannel, Byte, RxCompDqsOffset, ReadCached, &Override->DqsOffsetNUI);
    DqsOffsetNUI = 0;
  } else { // Restore
    DqsOffsetNUI = Override->DqsOffsetNUI;
  }
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, RxCompDqsOffset, WriteCached, &DqsOffsetNUI);
}

/**
  Use RxDqVrefOffsetR for VocFall

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
SenseAmpOffsetUseVocRiseForFall (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     Controller;
  UINT8     Channel;
  UINT8     Byte;
  UINT8     Lane;
  UINT8     FirstRank[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8     FirstPopulatedRank;
  INT64     GetSetVal;

  Outputs = &MrcData->Outputs;

  if (!MrcData->Inputs.IsDdrIoMbA0) {
    return;
  }
  // Get 1st populated Rank
  GetFirstRank (MrcData, FirstRank);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      FirstPopulatedRank = FirstRank[Controller][Channel];
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        for (Lane = 0; Lane < MAX_BITS_FOR_OFFSET_TRAINING; Lane++) {
          MrcGetSetBit (MrcData, Controller, Channel, FirstPopulatedRank, Byte, Lane, RxVocFall, ReadCached,  &GetSetVal);
          MrcGetSetBit (MrcData, Controller, Channel, FirstPopulatedRank, Byte, Lane, RxVocRise, WriteCached, &GetSetVal);
        }
      } // Byte
    } // Channel
  } // Controller
}

/**
  Program Voc with the average of VocRise and VocFall

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller to work on
  @param[in]      Channel     - Channel to work on
  @param[in]      Byte        - Byte to work on
  @param[in]      Bit         - Bit to work on
  @param[in]      Rank        - Rank to work on
  @param[in, out] VocRise     - VocRise value, will be updated with the average if needed
  @param[in, out] VocFall     - VocFall value, will be updated with the average if needed
**/
VOID
SenseAmpOffsetSetVocAverage (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                Byte,
  IN UINT8                Bit,
  IN UINT8                Rank,
  IN OUT INT64            *VocRise,
  IN OUT INT64            *VocFall
  )
{
  INT64 VocAverage;

  if (!MrcData->Inputs.IsDdrIoMbA0) {
    return;
  }

  VocAverage = ((INT8) *VocRise + (INT8) *VocFall) / 2;
  *VocRise = VocAverage;
  *VocFall = VocAverage;
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "\t%d", (INT8) VocAverage);
  MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, RxVocRise, WriteToCache, &VocAverage);
  MrcGetSetBit (MrcData, Controller, Channel, Rank, Byte, Bit, RxVocFall, WriteToCache, &VocAverage);
  // CR Cache will be flushed by caller
}
