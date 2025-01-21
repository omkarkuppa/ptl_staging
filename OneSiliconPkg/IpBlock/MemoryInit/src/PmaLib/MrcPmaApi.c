/** @file
  This file contains code related to initializing and configuring the PMA.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include "CMrcTypes.h"
#include "BlueMrcSpdProcessingApi.h" // for ConvertFreq2Clock()
#include "MrcPmaApi.h"
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcSagv.h"
#include "CMcAddress.h"

/**
  This function set the MRC vertion to MCDECS_SPARE register.
  The function need to be call by the wrapper after MrcStartMemoryConfiguration function where the MC CLK enable.
  The function write:
  Major number to bits 16-23
  Minor number to bits 8-15
  Build number to bits 0 - 7

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if MrcRevision read successfully
**/
MrcStatus
MrcSetMrcVersion (
  IN     MrcParameters *const MrcData
  )
{
  MrcVersion const         *Version;
  MEMSS_PMA_CR_MRC_VERSION_STRUCT  MrcRevision;

  Version = &MrcData->Outputs.Version;
  MrcRevision.Data = (((UINT32) Version->Major) << 24) |
                     (((UINT32) Version->Minor) << 16) |
                     (((UINT32) Version->Rev)   << 8)  |
                     (((UINT32) Version->Build));

  MrcWriteCR (MrcData, MEMSS_PMA_CR_MRC_VERSION_REG, MrcRevision.Data);
  return mrcSuccess;
}

/**
  This function reads the PMA Capacity registers and sets the memory controller's capability.

  @param[in, out] MrcData - All the MRC global data.

  @retval mrcSuccess memory controller's capability has been determined
  @retval mrcFail memory controller's capability was not determined
**/
MrcStatus
MrcMcCapability (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput                  *Inputs;
  MrcDebug                  *Debug;
  MrcSaveData               *Save;
  MrcOutput                 *Outputs;
  MrcControllerOut          *ControllerOut;
  MrcChannelOut             *ChannelOut;
  MrcDimmOut                *DimmOut;
  const MRC_FUNCTION        *MrcCall;
  MrcProfile                Profile;
  UINT8                     MaxChannel;
  UINT32                    DimmCount;
  UINT32                    Max;
  UINT32                    Size;
  UINT32                    DimmNum;
  UINT32                    ChDimmCount;
  UINT8                     Controller;
  UINT8                     Channel;
  UINT8                     Dimm;
  BOOLEAN                   Cmd2N;
  BOOLEAN                   IsLpddr5;
  BOOLEAN                   IsDdr5;
  BOOLEAN                   EccSupport;
  BOOLEAN                   IgnoreNonEccDimm;
  BOOLEAN                   EccExist;
  const char                *StrDdrType;
  MRC_EXT_INPUTS_TYPE       *ExtInputs;
  MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_STRUCT PmaFuseTechEn;
  MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT        PmaMcCap;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Save        = &MrcData->Save.Data;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  MrcCall     = Inputs->Call.Func;
  Debug       = &Outputs->Debug;
  ChDimmCount = MAX_DIMMS_IN_CHANNEL;
  Profile     = ExtInputs->MemoryProfile;
  MaxChannel  = Outputs->MaxChannels;
  IsDdr5      = Outputs->IsDdr5;
  IsLpddr5    = Outputs->IsLpddr5;

  MrcCall->MrcSetMem ((UINT8 *) &Outputs->MemoryMapData.McMemorySize[0], sizeof (Outputs->MemoryMapData.McMemorySize), 0);

  PmaFuseTechEn.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_REG);
  PmaMcCap.Data      = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_MISC_REG);

  Save->PmaMcTechEnable = PmaFuseTechEn.Data;
  Save->PmaMcCap        = PmaMcCap.Data;

  // Check that current DDR type is allowed on this CPU
  StrDdrType = NULL;
  if (!(Inputs->IsDdrIoTc)) {
    if (IsLpddr5 && (PmaFuseTechEn.Bits.LPDDR5_EN == 0)) {
      StrDdrType = gDdrTypeStr[MRC_DDR_TYPE_LPDDR5];
    } else if (IsDdr5 && (PmaFuseTechEn.Bits.DDR5_EN == 0)) {
      StrDdrType = gDdrTypeStr[MRC_DDR_TYPE_DDR5];
    }
  }

  if (StrDdrType != NULL) {
    // MRC detected a memory technology and PMA value shows the memory tech is not supported by this CPU.
    // e.g. LPDDR5 memory detected but CPU only supports DDR5.
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "ERROR: %s is not supported on this CPU\n", StrDdrType);
    return mrcDimmNotSupport;
  }

  if (!(IsLpddr5 || IsDdr5)) {
    // MRC detected memory technology besides what is supported by CPU.
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "ERROR: CPU only supports %s or %s; none of which has been detected!\n",
      gDdrTypeStr[MRC_DDR_TYPE_LPDDR5], gDdrTypeStr[MRC_DDR_TYPE_DDR5]);
    return mrcDimmNotSupport;
  }

  MrcMcCheckDDRSZ (MrcData, PmaMcCap.Data);

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "Maximum size of memory allowed on a channel = %uMB.\n",
    Outputs->MrcTotalChannelLimit
    );

  // Determine how many dimms are supported per channel on this memory controller,
  // based on fuse and how many channels have DIMMs installed.
  DimmCount = (PmaMcCap.Bits.DDPCD == 0) ? MAX_DIMMS_IN_CHANNEL : 1;

  if (ExtInputs->Force1Dpc == TRUE) {
    // Only 1DPC is supported on ULX / ULT platform
    DimmCount = 1;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "Number of channels supported = %u\nNumber of DIMMs per channel supported = %u\n",
    MaxChannel,
    DimmCount
    );

  // Determine the ECC capability of the memory controller.
  IgnoreNonEccDimm = (PmaMcCap.Bits.FDEE == 0) ? FALSE : TRUE;

  EccExist = (PmaMcCap.Bits.ECC_EXIST == 0) ? FALSE : TRUE;
  // Set EccSupport flag to TRUE if we must NOT ignore ECC DIMMs
  if (EccExist) {
    if (IgnoreNonEccDimm == TRUE) {
      Outputs->EccSupport = TRUE;
      EccSupport = TRUE; // FDEE has presedence over ECCDIS
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ALL DIMMs MUST be ECC capable\n");
    } else {
      EccSupport = ((PmaMcCap.Bits.ECCDIS > 0) || (Outputs->EccSupport == FALSE)) ? FALSE : TRUE;
    }
  } else {
    EccSupport = FALSE;
  }

  // Now copy ECC and NMode information to the channel and DIMM results.
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          ChannelOut = &ControllerOut->Channel[Channel];
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_NOTE,
                "  %s %u/%u/%u NMode = %u\n",
                CcdString,
                Controller,
                Channel,
                Dimm,
                Outputs->Timing[Profile].NMode
              );
              if (EccSupport == TRUE) {
                if (DimmOut->EccSupport == FALSE) {
                  if (IgnoreNonEccDimm == TRUE) {
                    DimmOut->Status = DIMM_DISABLED;
                    MRC_DEBUG_MSG (
                      Debug,
                      MSG_LEVEL_NOTE,
                      "  %s %u/%u/%u Disabling non-ECC capable DIMM\n",
                      CcdString,
                      Controller,
                      Channel,
                      Dimm
                    );
                  } else {
                    Outputs->EccSupport = FALSE; // Final ECCSupport must be disabled if one DIMM is NOT capable
                  }
                }
              } else {
                DimmOut->EccSupport = FALSE;
                Outputs->EccSupport = FALSE; // Final ECCSupport must be disabled if ECCDIS is set
              }
            }
          }
        }
      }
    }
  }

  // Disable ECC in each DIMM if we are not using ECC
  if (Outputs->EccSupport == FALSE) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];
            DimmOut->EccSupport = FALSE;
          }
        }
      }
    }
  }

  // Make sure we have the same NMode limit on both channels
  Cmd2N = FALSE;

  if (Outputs->Timing[Profile].NMode == 2) {
    Cmd2N = TRUE;
  }

  Outputs->Timing[Profile].NMode = (Cmd2N) ? 2 : 1;


  // Update Final SdramCount
  // SdramCount = Number of bytes lanes per channel
  // DDR5  x32 bit bus / x8 bits per bytelane = 4 bytelanes
  // LPDDR x16 bit bus / x8 bits per bytelane = 2 bytelanes
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              Outputs->SdramCount = (DimmOut->PrimaryBusWidth / 8);
              break;
            }
          }
        }
      }
    }
  }
  if (Outputs->EccSupport == TRUE) {
    Outputs->SdramCount++;
  }

  // Determine the size of memory in each channel.
  // Also determine the channel with the largest amount.
  Max = Outputs->MemoryMapData.TotalPhysicalMemorySize = 0;
  Outputs->NumPopChannel = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        Size        = 0;
        if (MrcChannelExist (MrcData, Controller, Channel)) {
          Outputs->NumPopChannel += 1;
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              Size += DimmOut->DimmCapacity;
            }
          }

          ChannelOut->Capacity = Size;
          if (Size > Max) {
            Max         = Size;
            ChDimmCount = ChannelOut->DimmCount;
          } else if ((Size == Max) && (DimmCount == 1)) {
            // Choose channel with least amount of DIMMs if 2DPC is disabled
            if (ChannelOut->DimmCount < ChDimmCount) {
              ChDimmCount = ChannelOut->DimmCount;
            }
          }
        } // ChannelExist

        Outputs->MemoryMapData.TotalPhysicalMemorySize  += ChannelOut->Capacity;
        Outputs->MemoryMapData.McMemorySize[Controller] += ChannelOut->Capacity;
        // Program ValidByteMask here
        ChannelOut->ValidByteMask = (1 << Outputs->SdramCount) - 1;
      }
    }
  }

  if (DimmCount == 1) {
    // Determine which DIMMs are supported on this memory controller.
    // If fused for one DIMM per channel, we pick the DIMM in a channel with the most memory.
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MrcControllerExist (MrcData, Controller)) {
        ControllerOut = &Outputs->Controller[Controller];
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          ChannelOut = &ControllerOut->Channel[Channel];
          Max = DimmNum = 0;
          if (ChannelOut->Status == CHANNEL_PRESENT) {
            for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
              DimmOut = &ChannelOut->Dimm[Dimm];
              if (DimmOut->Status == DIMM_PRESENT) {
                Size = DimmOut->DimmCapacity;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "C%uD%uDimmCapacity = 0x%x\n", Channel, Dimm, DimmOut->DimmCapacity);
                if (Size > Max) {
                  Max     = Size;
                  DimmNum = Dimm;
                }
              }
            }

            for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
              DimmOut = &ChannelOut->Dimm[Dimm];
              if ((DimmOut->Status == DIMM_PRESENT) && (Dimm != DimmNum)) {
                DimmOut->Status = DIMM_DISABLED;
              }
            }

            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "Controller configured to one DIMM per channel, we've selected channel %u, Dimm %u.\n",
              Channel,
              DimmNum
              );
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ValidRankBitMask = 0x%x\n", ChannelOut->ValidRankBitMask);
          }
        }
      }
    }
  }

  ExtInputs->Ibecc = (ExtInputs->Ibecc == TRUE) && (PmaMcCap.Bits.IBECCDIS == 0);

  // Print Dq / Dqs Swizzle Tables
  PrintDqDqsTable (MrcData);

  return mrcSuccess;
}

/**
  This function reads the MEMSS_PMA_CR_CAPABILITIES_MISC register
  and determines if SAGV Input parameters is to be overwritten.

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
MrcSaGvCapabilityCheck (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput                  *Inputs;
  MrcDebug                  *Debug;
  MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT  PmaMcCap;

  Inputs      = &MrcData->Inputs;
  Debug       = &MrcData->Outputs.Debug;

  // Obtain the capabilities of the memory controller.
  PmaMcCap.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_MISC_REG);

  if ((MrcIsSaGvEnabled (MrcData)) && PmaMcCap.Bits.QCLK_GV_DISABLE) {
    Inputs->ExtInputs.Ptr->SaGv = MRC_SAGV_DISABLED;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "!!!Fuse shows SAGV not supported, disabling corresponding Input parameter!!!\n");
  }
}
