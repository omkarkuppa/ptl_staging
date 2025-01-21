/** @file
  Implementation of PTL's LPDDR5 Specific functions, and constants.

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
  JEDEC
**/
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcLpddr5.h"

/**
  This function performs the reset sequence required by JEDEC spec for LPDDR5.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcJedecResetLpddr (
  IN  MrcParameters* const  MrcData
  )
{
  MrcStatus     Status;
  UINT32        tInit1;
  UINT32        tInit3;
  UINT32        tInit5;
  INT64         GetSetEn;
  INT64         GetSetDis;

  Status = mrcSuccess;
  GetSetEn = 1;
  GetSetDis = 0;
  tInit1 = MRC_LP_tINIT1_US * MRC_TIMER_1US;
  tInit3 = MRC_LP_tINIT3_US * MRC_TIMER_1US;
  tInit5 = MRC_LP_tINIT5_US * MRC_TIMER_1US;

  // Keep CS Low when DRAM_RESET# is asserted
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOn, WriteCached, &GetSetDis);

  // Assert DRAM RESET# signal after Voltage Ramp (Step 2)
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetEn);

  // Ensure tINIT1 (200us) before de-asserting DRAM reset. This will cover tINIT2 (10ns) as well.
  MrcWait (MrcData, tInit1);

  // De-Assert DRAM RESET# signal.
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetDis);

  // Wait tINIT3 (2ms) - covers Min CS low after RESET# high and tINIT4 (5tCK) - Min Stable CLK before CS high
  MrcWait (MrcData, tInit3);

  // Set the valid CKE's
  MrcCkeOnProgramming (MrcData);

  // Wait tINIT5: Min idle time before first MR[R,W] command.
  MrcWait (MrcData, tInit5);

  return Status;
}
