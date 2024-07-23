/** @file
  Platform Policy Initialization Driver.

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

#include "PlatformSetup.h"
#include <OemSetup.h>
#include <PlatformBoardId.h>
#include <MemInfoHob.h>
#include <IndustryStandard/SmBios.h>

/**
  Populate MEM_INFO_PROTOCOL from the Memory Info Data HOB

  @param[in, out] MemoryInfoProtocol          Pointer to MEM_INFO_PROTOCOL
**/
VOID
SetupPlatformPolicies (
  IN OUT  MEM_INFO_PROTOCOL *MemoryInfoProtocol
  )
{
  UINT8                     Node;
  UINT8                     Channel;
  UINT8                     Dimm;
  MEMORY_INFO_DATA_HOB      *MemInfo;
  MEMORY_INFO_DATA          *MemInfoData;
  CONTROLLER_INFO           *ControllerInfo;
  CHANNEL_INFO              *ChannelInfo;
  DIMM_INFO                 *DimmInfo;
  MEMORY_TIMING             *MemoryTiming;
  MRC_CH_TIMING             *Timing;
  UINT8                     Profile;
  UINT32                    Index;
  EFI_HOB_GUID_TYPE         *GuidHob;

  MemInfo = NULL;
  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }

  MemInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);

  MemInfoData                   = &MemoryInfoProtocol->MemInfoData;
  MemInfoData->Profile          = MemInfo->MemoryProfile;
  MemInfoData->XmpProfileEnable = MemInfo->XmpProfileEnable;
  MemInfoData->memSize          = (UINT32) (MemInfo->TotalPhysicalMemorySize);
  MemInfoData->ddrFreq          = (UINT16) MemInfo->ConfiguredMemoryClockSpeed;
  MemInfoData->ddrFreqMax       = (UINT16) MemInfo->MaximumMemoryClockSpeed;
  MemInfoData->EccSupport       = MemInfo->EccSupport;
  MemInfoData->Ratio            = MemInfo->Ratio;
  MemInfoData->RefClk           = MemInfo->RefClk;
  MemInfoData->PprDetectedErrors                = MemInfo->PprDetectedErrors;
  MemInfoData->PprRepairFails                   = MemInfo->PprRepairFails;
  MemInfoData->PprForceRepairStatus             = MemInfo->PprForceRepairStatus;
  switch (MemInfo->MemoryType) {
    case MemoryTypeDdr5:
      MemInfoData->DdrType = MRC_DDR_TYPE_DDR5;
      break;

    case MemoryTypeLpddr5:
      MemInfoData->DdrType = MRC_DDR_TYPE_LPDDR5;
      break;

    default:
      MemInfoData->DdrType = MRC_DDR_TYPE_UNKNOWN;
      break;
  }

  for (Profile = 0; Profile < PROFILE_NUM; Profile++) {
    MemInfoData->VddVoltage[Profile]  = (UINT16) MemInfo->VddVoltage[Profile];
  }
  Index = 0;
  for (Node = 0; Node < NODE_NUM; Node++) {
    ControllerInfo = &MemInfo->Controller[Node];
    for (Channel = 0; Channel < CH_NUM; Channel++) {
      ChannelInfo = &ControllerInfo->ChannelInfo[Channel];
      if (ChannelInfo->Status == CHANNEL_PRESENT) {
        //
        // Updating memory timings to MemInfoProtocol from MemInfoHob for each memory profile.
        //
        for (Profile = 0; Profile < PROFILE_NUM; Profile++) {
          MemoryTiming          = &MemInfoData->Timing[Profile];
          Timing                = &ChannelInfo->Timing[Profile];
          MemoryTiming->tCK     = Timing->tCK;
          MemoryTiming->tCL     = Timing->tCL;
          MemoryTiming->tCWL    = Timing->tCWL;
          MemoryTiming->tFAW    = Timing->tFAW;
          MemoryTiming->tRAS    = Timing->tRAS;
          MemoryTiming->tRCDtRP = Timing->tRCDtRP;
          MemoryTiming->tREFI   = Timing->tREFI;
          MemoryTiming->tRFC    = Timing->tRFC;
          MemoryTiming->tRPab   = Timing->tRPab;
          MemoryTiming->tRRD    = Timing->tRRD;
          MemoryTiming->tRTP    = Timing->tRTP;
          MemoryTiming->tWR     = Timing->tWR;
          MemoryTiming->tWTR    = Timing->tWTR;
          MemoryTiming->NMode   = (UINT8) Timing->NMode;
        }
      }
      for (Dimm = 0; Dimm < DIMM_NUM; Dimm++,Index++) {
        DimmInfo = &ChannelInfo->DimmInfo[Dimm];
        MemInfoData->dimmSize[Index] = (UINT16) DimmInfo->DimmCapacity;
        MemInfoData->DimmStatus[Index] = DimmInfo->Status;
        MemInfoData->RankInDimm[Index] = DimmInfo->RankInDimm;
        /**
          Get pointer of SPD data read from MRC for SMBIOS driver to report w/o reading SMBus again.
          MRC only provides SMBIOS data and Manufacturing data after first boot is saved to NVRAM.
        **/
        MemInfoData->DimmsSpdData[Index] = (DimmInfo->Status < DIMM_NOT_PRESENT) ? (UINT8 *) &(DimmInfo->SpdSave) : NULL;
      }
    }
  }
  return;
}
