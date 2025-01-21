/** @file
  Initializes PTL RTC

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <RtcConfig.h>
#include <RtcHandle.h>
#include <Library/PeiRtcLib.h>
#include <Library/P2SbSocLib.h>
#include <Register/RtcRegs.h>
#include <PcdSbPortIds.h>

/*
  Initialize SOC RTC handle

  @param[in] SiPolicy                The SI Policy PPI instance
  @param[in] RtcHandle               Pointer to RTC handle structure
  @param[in] P2SbController          P2SB controller pointer
  @param[in] RtcPcrAccess            P2SB Register Access to RTC
*/
STATIC
VOID
InitPtlPcdRtcHandle (
  IN SI_POLICY_PPI                   *SiPolicyPpi,
  IN RTC_HANDLE                      *RtcHandle,
  IN P2SB_CONTROLLER                 *P2SbController,
  IN P2SB_SIDEBAND_REGISTER_ACCESS   *RtcPcrAccess
  )
{
  EFI_STATUS                     Status;
  RTC_CONFIG                     *RtcConfig;
  P2SB_PORT_16_ID                P2SBPid;

  ZeroMem (RtcHandle, sizeof (RTC_HANDLE));
  ZeroMem (RtcPcrAccess, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gRtcConfigGuid, (VOID *) &RtcConfig);
  ASSERT_EFI_ERROR (Status);
  RtcHandle->RtcConfig = RtcConfig;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_RTC_HOST;
  PtlPcdGetP2SbController (P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, RtcPcrAccess);
  RtcHandle->RtcPcrAccess = (REGISTER_ACCESS *) RtcPcrAccess;
}

/**
  Initialize SOC RTC

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdRtcInit (
  IN  SI_POLICY_PPI              *SiPolicyPpi
  )
{
  RTC_HANDLE                     RtcHandle;
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  RtcPcrAccess;

  InitPtlPcdRtcHandle (
    SiPolicyPpi,
    &RtcHandle,
    &P2SbController,
    &RtcPcrAccess
    );
  RtcConfiguration (&RtcHandle);
}

/**
  The function performs SOC RTC lock setting.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdRtcLock (
  IN  SI_POLICY_PPI              *SiPolicyPpi
  )
{
  RTC_HANDLE                     RtcHandle;
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  RtcPcrAccess;

  InitPtlPcdRtcHandle (SiPolicyPpi, &RtcHandle, &P2SbController, &RtcPcrAccess);
  RtcLock (&RtcHandle);
}