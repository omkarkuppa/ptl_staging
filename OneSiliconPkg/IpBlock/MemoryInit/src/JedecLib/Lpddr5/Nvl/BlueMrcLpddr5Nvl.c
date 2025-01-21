/** @file
  Implementation of NVL's LPDDR5 Specific functions, and constants.

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
#include "MrcMemoryApi.h"
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
  UINT32        tCSH;
  INT64         GetSetEn;
  INT64         GetSetDis;

  Status = mrcSuccess;
  GetSetEn = 1;
  GetSetDis = 0;
  tInit1 = MRC_LP_tINIT1_US * MRC_TIMER_1US;
  tInit3 = MRC_LP_tINIT3_US * MRC_TIMER_1US;
  tInit5 = MRC_LP_tINIT5_US * MRC_TIMER_1US;
  tCSH   = 3 * MRC_TIMER_1NS;
  
  // Assert DRAM_RESET signal after Voltage Ramp (Step 2). MC will have driven CS_N low
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetEn);

  // Ensure tINIT1 (200us) before de-asserting DRAM reset. This will cover tINIT2 (10ns) as well.
  // Override CS_N to go LOW
  MrcWait (MrcData, tInit1);
  OverrideCs (MrcData, 0, TRUE);

  // De-Assert DRAM_RESET signal.
  MrcGetSetNoScope (MrcData, GsmDdrReset, WriteCached, &GetSetDis);

  // Wait tINIT3 (2ms) and tINIT4 (5tCK) before driving CS high. Waiting tINIT3 is sufficient enough as there
  // is no toggling in between.
  MrcWait (MrcData, tInit3);
  OverrideCs (MrcData, 1, TRUE);

  // Wait tCSH (3ns) before driving CS val to 0
  MrcWait (MrcData, tCSH);
  OverrideCs (MrcData, 0, TRUE);

  // Wait tINIT5: Min idle time before first MR[R,W] command.
  // Disable CS Override
  MrcWait (MrcData, tInit5);
  OverrideCs (MrcData, 0, FALSE);

  return Status;
}
