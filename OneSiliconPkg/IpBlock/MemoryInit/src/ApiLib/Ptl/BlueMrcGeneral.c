/** @file
  This file contains all the MRC general API to the MRC wrapper.

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
#include "MrcCommon.h"
#include "MrcLpddr5.h"

/**
  This function sets the project specific overrides in the Input and Output structure

  @param[in, out] MrcData - All the MRC global data.
 */
VOID
MrcSetPrjOverrides (
  IN OUT MrcParameters *const MrcData
  )
{
  // At this time there are no PTL-specific overrides to be added here
}

/**
  Modify tRDRDdg and tRDRDsg timings by 1tCK. Does not apply modification to tRDRDsg
  in BG mode. Expects caller to flush cache.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Enable  - TRUE to add 1tCK, FALSE to subtract.
 */
VOID
MrcModifyRdRdTimings (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              Enable
  )
{
  MrcOutput       *Outputs;
  BOOLEAN          IsLpddr;
  MRC_LP5_BANKORG  Lp5BGOrg;

  UINT32 Channel;
  UINT32 Controller;

  INT64 tRdRdsg;
  INT64 tRdRddg;
  INT64 GetSetVal;

  Outputs  = &MrcData->Outputs;
  IsLpddr  = Outputs->IsLpddr;
  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);

  GetSetVal = Enable ? 1 : 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel) && (!IS_MC_SUB_CH (IsLpddr, Channel))) {
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, ReadCached, &tRdRddg);
          tRdRddg = Enable ? (tRdRddg + 4) : (tRdRddg - 4);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDdg, WriteToCache, &tRdRddg);

          if (Lp5BGOrg == MrcLp516Bank) {
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, ReadCached, &tRdRdsg);
            tRdRdsg = Enable ? (tRdRdsg + 4) : (tRdRdsg - 4);
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctRDRDsg, WriteToCache, &tRdRdsg);
          }

          if (Lp5BGOrg == MrcLp5BgMode) {
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDisLpddr5RdwrInterleaving, WriteToCache, &GetSetVal);
          }
        }
      }
    }
  }
}
