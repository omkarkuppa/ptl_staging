/** @file
  DCI SoC library.
  All functions in this library are available for PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/PeiBpkPrivateLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

/**
  Return if probe connected override is applied

  @retval TRUE    probe connected override is applied
  @retval FALSE   probe connected override is not applied
**/
BOOLEAN
IsProbeConnectedOverrideSoc (
  VOID
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS   BpkPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);
  P2SBPid.Pid16bit = PTL_SID_F2_PID_BPK;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &BpkPcrAccess);

  return !!(IsDciHostConnected ((REGISTER_ACCESS *) &DciPcrAccess) || IsBpkConnected ((REGISTER_ACCESS *) &BpkPcrAccess));
}
