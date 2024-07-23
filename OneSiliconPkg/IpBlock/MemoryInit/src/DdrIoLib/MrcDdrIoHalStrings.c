/** @file
  Implementation of the hardware abstraction layer DdrIo strings.

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

@par Specification Reference:
**/

// Include files
#include "MrcDdrIoApi.h"  // for prototype declarations
#include "MrcCommon.h"

#ifdef MRC_DEBUG_PRINT
/**
  This function outputs the specified group values to the debug print device.

  @param[in] MrcData   - Pointer to global data.
  @param[in] Group     - DDRIO group to access.
  @param[in] Socket    - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in] Controller- DDR Memory Controller Number within the processor socket (0-based).
  @param[in] Channel   - DDR Channel Number within the memory controller (0-based).
  @param[in] Rank      - Rank number within a channel (0-based).
  @param[in] Strobe    - Dqs data group within the rank (0-based).
  @param[in] Bit       - Bit index within the data group (0-based).
  @param[in] FreqIndex - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval MrcStatus
**/
MrcStatus
MrcPrintDdrIoGroup (
  IN MrcParameters *const  MrcData,
  IN UINT32  const Socket,
  IN UINT32  const Controller,
  IN UINT32  const Channel,
  IN UINT32  const Rank,
  IN UINT32  const Strobe,
  IN UINT32  const Bit,
  IN UINT32  const FreqIndex,
  IN GSM_GT  const Group
  )
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  INT64_STRUCT  Value;
  UINT32        ControllerIndex;
  UINT32        ControllerMax;
  UINT32        ControllerStart;
  UINT32        ChannelIndex;
  UINT32        ChannelMax;
  UINT32        ChannelStart;
  UINT32        RankIndex;
  UINT32        RankMax;
  UINT32        RankStart;
  UINT32        Byte;
  UINT32        ByteMax;
  UINT32        ByteStart;
  UINT32        BitIndex;
  UINT32        BitMax;
  UINT32        BitStart;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  if ((Strobe >= MAX_SDRAM_IN_DIMM) && (Strobe != MRC_IGNORE_ARG)) {
    ByteMax   = Outputs->SdramCount;
    ByteStart = 0;
  } else {
    ByteMax   = Strobe + 1;
    ByteStart = Strobe;
  }
  if ((Bit >= MAX_BITS) && (Bit != MRC_IGNORE_ARG)) {
    BitMax    = MAX_BITS;
    BitStart  = 0;
  } else {
    BitMax    = Bit + 1;
    BitStart  = Bit;
  }
  if ((Channel >= MAX_CHANNEL) && (Channel != MRC_IGNORE_ARG)) {
    ChannelMax    = Outputs->MaxChannels;
    ChannelStart  = 0;
  } else {
    ChannelMax    = Channel + 1;
    ChannelStart  = Channel;
  }
  if ((Rank >= MAX_RANK_IN_CHANNEL) && (Rank != MRC_IGNORE_ARG)) {
    RankMax   = MAX_RANK_IN_CHANNEL;
    RankStart = 0;
  } else {
    RankMax   = Rank + 1;
    RankStart = Rank;
  }
  if ((Controller >= MAX_CONTROLLER) && (Controller != MRC_IGNORE_ARG)) {
    ControllerMax    = MAX_CONTROLLER;
    ControllerStart  = 0;
  } else {
    ControllerMax    = Controller + 1;
    ControllerStart  = Controller;
  }

  switch (Group) {
    case RecEnDelay:
      for (ControllerIndex = ControllerStart; ControllerIndex < ControllerMax; ControllerIndex++) {
        for (ChannelIndex = ChannelStart; ChannelIndex < ChannelMax; ChannelIndex++) {
          for (RankIndex = RankStart; RankIndex < RankMax; RankIndex++) {
            if (!MrcRankExist (MrcData, ControllerIndex, ChannelIndex, RankIndex)) {
              continue;
            }
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RecEnDelay Mc %u Channel %u Rank %u:\n", ControllerIndex, ChannelIndex, RankIndex);
            for (Byte = ByteStart; Byte < ByteMax; Byte++) {
              if (MrcByteExist (MrcData, ControllerIndex, ChannelIndex, Byte)) {
                MrcGetSetStrobe (MrcData, ControllerIndex, ChannelIndex, RankIndex, Byte, RecEnDelay, GSM_READ_ONLY, &Value.Data);
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " B%u: %d\n", Byte, Value.Data32.Low);
              }
            } // Byte
          } // Rank
        } // Channel
      } // Controller
      break;

    case RxDqsBitDelay:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RxDqsBitDelay Mc %u Channel %u Rank %u Byte %u:", Controller, Channel, Rank, Strobe);
      for (BitIndex = BitStart; BitIndex < BitMax; BitIndex++) {
        MrcGetSetBit (MrcData, Controller, Channel, Rank, Strobe, BitIndex, RxDqsBitDelay, GSM_READ_ONLY, &Value.Data);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Value.Data32.Low);
      } // Bit
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      break;

    case RxIoTclDelay:
      for (ControllerIndex = ControllerStart; ControllerIndex < ControllerMax; ControllerIndex++) {
        for (ChannelIndex = ChannelStart; ChannelIndex < ChannelMax; ChannelIndex++) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RxIoTclDelay Mc %u Channel %u:", Controller, Channel);
          MrcGetSetMcCh (MrcData, Controller, Channel, RxIoTclDelay, GSM_READ_ONLY, &Value.Data);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %d", Value.Data32.Low);
        }
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      break;

    default:
      break;
  }
  return (mrcSuccess);
}
#endif // MRC_DEBUG_PRINT

