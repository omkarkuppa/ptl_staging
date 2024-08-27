/** @file
  This module configures the memory controller address decoder.

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
#include "MrcAddressDecodeConfiguration.h"
#include "MrcAddressDecodeConfigurationPrivate.h"
#include "MrcMcApi.h"
#include "MrcSpdProcessingDefs.h"

/**
  This function configures the zone configuration registers MAD-CR and MAD-ZR-CR.

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller to configure.

  @retval Nothing.
**/
void
ZoneConfiguration (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller
  )
{
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcControllerOut    *ControllerOut;
  MrcChannelOut       *ChannelOut;
  INT64               DramType;
  INT64               LsbMaskBit;
  INT64               HashMask;
  INT64               EccMode;
  INT64               HashLsb;
  UINT32              Channel;
  BOOLEAN             FullLpChPop;
  BOOLEAN             IsLpddr;
  MC0_MAD_SUB_CHANNEL_HASH_STRUCT  SubChannelHash;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs              = &MrcData->Inputs;
  Outputs             = &MrcData->Outputs;
  ExtInputs           = Inputs->ExtInputs.Ptr;
  Debug               = &Outputs->Debug;
  ControllerOut       = &Outputs->Controller[Controller];
  DramType            = Outputs->DdrType;
  IsLpddr             = Outputs->IsLpddr;

  EccMode = Outputs->EccSupport;
  MrcGetSetMc (MrcData, Controller, GsmMccEccMode, WriteToCache | PrintValue, &EccMode);

  HashLsb = IsLpddr ? 0x2 : 0x3;
  MrcGetSetMc (MrcData, Controller, GsmMccHashLsb, WriteToCache | PrintValue, &HashLsb);

  FullLpChPop = TRUE;   // Assume all LPDDR channels (4x16) are populated in the controller.
  for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
    ChannelOut = &ControllerOut->Channel[Channel];
    if (ChannelOut->Status != CHANNEL_PRESENT) {
      // If any channel is not present in the controller, set the flag to false for LPDDR.
      FullLpChPop = FALSE;
      break;
    }
  }

  // Interleaved mode
  // Check for any Channel hash support
  // Channel hash override enabled then take values from input parameters
  if (ExtInputs->ChHashOverride == TRUE) {
    if (ExtInputs->ChHashEnable) {
      HashMask = ExtInputs->ChHashMask;
      LsbMaskBit = ExtInputs->ChHashInterleaveBit;
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Channel HASH Enabled\n");
    }
  } else {
    if (IsLpddr) {
      HashMask = 0x934;
      LsbMaskBit = 2;
    } else { // DDR5
      HashMask = 0x938;
      LsbMaskBit = 3;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Channel HASH Auto Enabled - Default to DramType:%lld\n", DramType);
  }
  MrcGetSetMc (MrcData, Controller, GsmMccLsbMaskBit, WriteToCache | PrintValue, &LsbMaskBit);
  MrcGetSetMc (MrcData, Controller, GsmMccHashMask, WriteToCache | PrintValue, &HashMask);

  SubChannelHash.Data = 0;
  if (IsLpddr && Outputs->EnhancedChannelMode && FullLpChPop) {
    // HASH can only be enabled if all LPDDR channels are populated on the controller.
    // If RI is off, HASH must be off.
    if (ExtInputs->SubChHashOverride == TRUE) {
      SubChannelHash.Bits.SUBCH_HASH_LSB_MASK_BIT = ExtInputs->SubChHashInterleaveBit;
      SubChannelHash.Bits.SUBCH_HASH_MASK = ExtInputs->SubChHashMask;
    } else {
      SubChannelHash.Bits.SUBCH_HASH_LSB_MASK_BIT = 3;
      SubChannelHash.Bits.SUBCH_HASH_MASK = 0x838;
    }
  }
  MrcWriteCR (MrcData, OFFSET_CALC_CH (MC0_MAD_SUB_CHANNEL_HASH_REG, MC1_MAD_SUB_CHANNEL_HASH_REG, Controller), SubChannelHash.Data);

  MrcGetSetMc (MrcData, Controller, GsmMccAddrDecodeDdrType,  WriteToCache     | PrintValue, &DramType);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function checks the possiblity of enabling

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - MC Controller ID.

  @retval True/False    - True: enable EBH, False: disable EBH.
**/

BOOLEAN
MrcEnableExtendedBankHashing (
  IN MrcParameters *const MrcData,
  IN     UINT32           Controller
)
{
  MrcChannelOut                     *ChannelOut;
  MrcControllerOut                  *ControllerOut;
  MRC_EXT_INPUTS_TYPE               *ExtInputs;
  MrcDimmOut                        *DimmOut;
  MrcInput                          *Inputs;
  MrcOutput                         *Outputs;
  MrcDebug                          *Debug;
  UINT32                            Channel;
  UINT32                            Dimm;
  UINT32                            DimmCapacity;
  UINT8                             Ranks;
  UINT8                             SdramWidth;
  BOOLEAN                           FirstDimm;

  Inputs                          = &MrcData->Inputs;
  ExtInputs                       = Inputs->ExtInputs.Ptr;
  Outputs                         = &MrcData->Outputs;
  Debug                           = &Outputs->Debug;
  ControllerOut                   = &Outputs->Controller[Controller];
  FirstDimm                       = TRUE;
  DimmCapacity                    = 0;
  Ranks                           = 0;
  SdramWidth                      = 0;

  if (!ExtInputs->ExtendedBankHashing) {
    return FALSE;  //exit without enable EBH
  }

  //Can be only enabled when EIM
  if (!ExtInputs->EnhancedInterleave) {
    return FALSE;
  }

  // Check symmetrical DRAM configs, if same rank occupancy, rank size and device width
  for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
    ChannelOut = &ControllerOut->Channel[Channel];
    for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
      DimmOut = &ChannelOut->Dimm[Dimm];
      if (DimmOut->Status != DIMM_PRESENT) {
        continue;
      }
      if (FirstDimm) {
        DimmCapacity  = DimmOut->DimmCapacity;
        Ranks         = DimmOut->RankInDimm;
        SdramWidth    = DimmOut->SdramWidth;
        FirstDimm     = FALSE;
      } else {
        if ((DimmCapacity != DimmOut->DimmCapacity) ||
          (Ranks        != DimmOut->RankInDimm)   ||
          (SdramWidth   != DimmOut->SdramWidth)) {
          return FALSE;
        }
      }
    } // Dimm
  } // Channel

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Extended Bank Hashing Enabled on Mc %u\n", Controller);

  return TRUE;
}

/**
  Map SDRAM Density Index to MC Subch Density register value.

  @param[in] Index   - SDRAM Density Index value.
  @param[in] IsLpddr - is LPDDR.

  @retval Result - MC Density register value
*/
UINT8
MapSdramDensityIndex (
  UINT8   Index,
  BOOLEAN IsLpddr
  )
{
  UINT8 Result = 0;

  switch (Index) {
    case MrcDensity8Gb:
      Result = IsLpddr ? 2 : 0; // 8GB supported only for LPDDR
      break;
    case MrcDensity12Gb:
      Result = IsLpddr ? 3 : 0; // 12GB supported only for LPDDR
      break;
    case MrcDensity16Gb:
      Result = 4;
      break;
    case MrcDensity24Gb:
      Result = 6;
      break;
    case MrcDensity32Gb:
      Result = 8;
      break;

    default:
      Result = 0;
  }
  return Result;
}

/**
  Set DIMM Config register for channel in MC

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
*/
void
SetChannelDimmConfig (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcOutput           *Outputs;
  MrcChannelOut       *ChannelOut;
  MrcControllerOut    *ControllerOut;
  MrcDimmOut          *Dimm0Out;
  MrcDimmOut          *Dimm1Out;
  INT64               GetSetVal;
  INT64               Dimm0RankCount;
  INT64               Dimm1RankCount;
  INT64               AutoPrecharge;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Outputs           = &MrcData->Outputs;
  ControllerOut     = &Outputs->Controller[Controller];
  ChannelOut        = &ControllerOut->Channel[Channel];
  ExtInputs         = MrcData->Inputs.ExtInputs.Ptr;

  Dimm0Out = &ChannelOut->Dimm[dDIMM0];
  // If we are in EnhancedChannelMode (LP5) treat LPDDR Ch 1/3 as the second DIMM in the MC Channel.
  // Otherwise (DDR5) use DIMM1 in the same struct.
  Dimm1Out = Outputs->IsLpddr5 ? &ControllerOut->Channel[Channel + 1].Dimm[dDIMM0] : &ChannelOut->Dimm[dDIMM1];

  if (Dimm0Out->Status == DIMM_PRESENT) {
    Dimm0RankCount = (Dimm0Out->RankInDimm == 2) ? 1 : 0;      // SUBCH_0_RANKS: 0x0 - 1 Rank, 0x1 - 2 Ranks
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0RankCnt,   WriteToCache | PrintValue, &Dimm0RankCount);

    GetSetVal = (Dimm0Out->SdramWidth == 8) ? 1 : 0;           // SUBCH_0_WIDTH: 0x0 - x16, 0x1 - 0x8
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0SdramWidth, WriteToCache | PrintValue, &GetSetVal);

    GetSetVal = MapSdramDensityIndex (Dimm0Out->DensityIndex, Outputs->IsLpddr5);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch0Density, WriteToCache | PrintValue, &GetSetVal);
  }

  if (Dimm1Out->Status == DIMM_PRESENT) {
    Dimm1RankCount = (Dimm1Out->RankInDimm == 2) ? 1 : 0;      // SUBCH_0_RANKS: 0x0 - 1 Rank, 0x1 - 2 Ranks
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1RankCnt,   WriteToCache | PrintValue, &Dimm1RankCount);

    GetSetVal = (Dimm1Out->SdramWidth == 8) ? 1 : 0;           // SUBCH_0_WIDTH: 0x0 - x16, 0x1 - 0x8
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1SdramWidth, WriteToCache | PrintValue, &GetSetVal);

    GetSetVal = MapSdramDensityIndex (Dimm1Out->DensityIndex, Outputs->IsLpddr5);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSubch1Density, WriteToCache | PrintValue, &GetSetVal);
  }

  if (Outputs->IsDdr5) {
    if (ExtInputs->Ddr5AutoPrechargeEnable == 0) { // 0 - Auto
      AutoPrecharge = ((Dimm0Out->SdramWidth) == 16 || (Dimm1Out->SdramWidth == 16)) ? 0 : 1;
    } else { // 1 - Disabled, 2 - Enabled
      AutoPrecharge = (ExtInputs->Ddr5AutoPrechargeEnable == 1) ? 0 : 1;
    }
  } else {
    AutoPrecharge = 0;
  }
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAutoPrechargeEn, WriteToCache | PrintValue, &AutoPrecharge);

  MrcFlushRegisterCachedData (MrcData);
}


/**
  This function configures the MAD_DIMM_CH0/1 register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
ChannelAddressDecodeConfiguration (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  const MrcInput      *Inputs;
  INT64               GetSetVal;
  INT64               EnhancedInterleave;
  INT64               ExtendedBankHashing;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs            = &MrcData->Inputs;
  ExtInputs         = Inputs->ExtInputs.Ptr;

  SetChannelDimmConfig (MrcData, Controller, Channel);

  //IntraChannel Config
  EnhancedInterleave = ExtInputs->EnhancedInterleave;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnhancedInterleave, WriteToCache | PrintValue, &EnhancedInterleave);

  ExtendedBankHashing = MrcEnableExtendedBankHashing (MrcData, Controller);
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccExtendedBankHash, WriteToCache | PrintValue, &ExtendedBankHashing);

  //Bg0hash Config
  GetSetVal = 0x3;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccBg0Hash, WriteToCache | PrintValue, &GetSetVal);

  GetSetVal = MrcData->Outputs.IsLpddr ? 0x1000 : 0x2000;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMR4Period, WriteToCache | PrintValue, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function is the main address decoding configuration function.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
MrcAdConfiguration (
  IN MrcParameters *const MrcData
  )
{
  UINT32      Controller;
  UINT32      Channel;
  BOOLEAN     IsLpddr;

  IsLpddr = MrcData->Outputs.IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }
    ZoneConfiguration (MrcData, Controller);
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      ChannelAddressDecodeConfiguration (MrcData, Controller, Channel);
    } // for Channel
  } // for Controller

  SetMcSliceHashing (MrcData);
}

/**
  Initialize MC inter hashing.

  @param[in] MrcData          - Include all MRC global data.

  @retval Nothing.
*/
void
SetMcSliceHashing (
  IN MrcParameters *const MrcData
)
{
  MrcMemoryMap *MemoryMapData = &MrcData->Outputs.MemoryMapData;
  INT32 Controller;
  INT64 HashEnable;
  INT64 Zone1Start;

  UINT32 McCapacity;
  UINT32 MaxCapacity = 0;
  UINT32 MinCapacity = 0xFFFFFFFF;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    McCapacity  = MemoryMapData->McMemorySize[Controller]/1024; // Convert MB to GB

    MaxCapacity = MAX (MaxCapacity, McCapacity);
    MinCapacity = MIN (MinCapacity, McCapacity);
  }

  HashEnable = (MinCapacity == 0 ? 0 : 1);
  Zone1Start = HashEnable ? MinCapacity * 2 : 0;

  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccHashEnabled, WriteToCache | PrintValue, &HashEnable);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccHashZone1Start, WriteToCache | PrintValue, &Zone1Start);

  MrcFlushRegisterCachedData (MrcData);
}
