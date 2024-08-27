/** @file
  This file implements functions for setting up test control
  registers for CPGC 2.0.

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
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "Cpgc20.h"
#include "MrcCpgcOffsets.h"
#include "MrcSpdProcessingDefs.h" // for MRC_SPD_SDRAM_DENSITY_8Gb
#include "MrcLpddr5.h"

/**
  Wait till CPGC test is done on current Controller/Channel
  Time out if CPGC test takes longer then 10 seconds

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Controller to work on
  @param[in]  Channel           - Channel to work on

  @retval Success if CPGC test finishes without exceeding timeout value
**/
MrcStatus
Cpgc20TestDone (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel
  )
{
  MC0_REQ0_CR_CPGC_SEQ_STATUS_STRUCT   CpgcSeqStatus;
  MrcOutput               *Outputs;
  const MrcInput          *Inputs;
  MC0_STALL_DRAIN_STRUCT  StallDrain;
  const MRC_FUNCTION      *MrcCall;
  UINT32                  Offset;
  BOOLEAN                 IsLpddr;
  UINT32                  IpChannel;
  UINT64                  Timeout;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  IsLpddr = Outputs->IsLpddr;
  MrcCall = Inputs->Call.Func;
  CpgcSeqStatus.Data = 0;
  Timeout = (Outputs->PprRunningState == PPR_IS_RUNNING) ? 120000 : 10000;
  Timeout += (UINT32) MrcCall->MrcGetCpuTime ();
  // Wait till CPGC test is done on all participating channels
  do {
    Offset = OFFSET_CALC_CH (MC0_STALL_DRAIN_REG, MC1_STALL_DRAIN_REG, Controller);
    StallDrain.Data = MrcReadCR (MrcData, Offset);
    IpChannel = LP_IP_CH (IsLpddr, Channel);
    Offset = OFFSET_CALC_MC_CH (MC0_REQ0_CR_CPGC_SEQ_STATUS_REG, MC1_REQ0_CR_CPGC_SEQ_STATUS_REG, Controller, MC0_REQ1_CR_CPGC_SEQ_STATUS_REG, IpChannel);
    CpgcSeqStatus.Data = MrcReadCR (MrcData, Offset);

    // Error out if test exceeds timeout
    if (((UINT32) MrcCall->MrcGetCpuTime()) > Timeout) {
      return mrcFail;
    }
  } while (((CpgcSeqStatus.Bits.TEST_DONE) && (StallDrain.Bits.mc_drained)) != 1);
  return mrcSuccess;
}


