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
  in BG mode.

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
  MRC_LP5_BANKORG Lp5BGOrg;
  INT64           tRdRdOffset;
  INT64           RdWrInterleaving;

  Outputs  = &MrcData->Outputs;
  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  RdWrInterleaving = Enable ? 1 : 0;
  tRdRdOffset = Enable ? + 4 : -4;

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctRDRDdg, WriteOffsetCached, &tRdRdOffset);

  if (Lp5BGOrg == MrcLp516Bank) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctRDRDsg, WriteOffsetCached, &tRdRdOffset);
  }

  if (Lp5BGOrg == MrcLp5BgMode) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccDisLpddr5RdwrInterleaving, WriteCached, &RdWrInterleaving);
  }
}
