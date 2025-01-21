/** @file
  Initializes PSTH Device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include "PtlPcdInitPei.h"
#include <Register/PchRegsPsth.h>
#include <Library/PchPcrLib.h>
#include <PcdSbPortIds.h>

/**
  The function performs PSTH specific programming.

  @param[in] SiPolicy          The SI Policy instance

**/
VOID
PchPsthConfigure (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  P2SB_PORT_16_ID             P2SBPid;

  //
  // PSTH power management settings.
  //
  ///
  /// PSTHCTL (0xD00h[2]) = 0, PSTH IOSF Primary Trunk Clock Gating Enable (PSTHIOSFPTCGE)
  /// PSTHCTL (0xD00h[1]) = 1, PSTH IOSF Sideband Trunk Clock Gating Enable (PSTHIOSFSTCGE)
  /// PSTHCTL (0xD00h[0]) = 1, PSTH Dynamic Clock Gating Enable (PSTHDCGE)
  ///
  P2SBPid.Pid16bit = PTL_SID_F2_PID_PSTH;
  PchPcrAndThenOr32 (
    P2SBPid.PortId.LocalPid, R_PSTH_PCR_PSTHCTL,
    (UINT32) ~(B_PSTH_PCR_PSTHCTL_PSTHIOSFPTCGE),
    B_PSTH_PCR_PSTHCTL_PSTHIOSFSTCGE |
    B_PSTH_PCR_PSTHCTL_PSTHDCGE
    );
}

