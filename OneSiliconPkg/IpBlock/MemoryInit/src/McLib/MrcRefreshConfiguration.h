/** @file
  This module include MRC_RefreshConfiguration external data

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
#ifndef _MrcRefreshConfiguration_h_
#define _MrcRefreshConfiguration_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"

///
/// Self refresh idle timer value
///
#define SELF_REFRESH_IDLE_COUNT (0x200)
#define MIN_SELF_REFRESH_TIME_LP (15000 * 1000) // 15ns

/**
  This function configures refresh parameters:
    tREFI, tRFC, tREFIx9, tRFCpb

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
SetTcRfpRftpReg (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  This function executes the refresh configuration process.

  @param[in] MrcData         - Include all MRC global data.

  @retval Nothing.
**/
extern
void
MrcRefreshConfiguration (
  IN MrcParameters *const MrcData
  );

/**
  Perform the required steps to exit self refresh in S3/Warm reset:
    Download the Read Reg File for all populated ranks.
    Assert CKE for all the ranks present to pull Dimms out of Self-Refresh.
    Issue long ZQ Calibration for all the ranks present in the channel.
    Set REUT to normal mode for all channels.
    Set the Power Down Config Register.

  @param[in] MrcData         - Include all MRC global data.
  @param[in] IsSrxResetSet   - Decides whether we set or clear srx_reset

  @retval mrcSuccess
**/
MrcStatus
MrcSelfRefreshExit (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsSrxResetSet
  );

/**
  Perform the required steps to exit self refresh in Cold Boot after MC takes control over DFI, DRAM needs to come out of self-refresh
   1. Set MC_INIT_STATE_G.pure_srx to 1 to avoid any additional maintenance flows. MRC needs to clear pure_srx=0 after SRX is complete and before issuing any SR related actions.
   2. Set Force_SR_State to 0b11 to force MC to exit self refresh by sending SRX command and stay out of self refresh (should be enabled when pure_srx mode is set).

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcSelfRefreshExitPostDfi (
  IN MrcParameters *const MrcData
  );

#endif
