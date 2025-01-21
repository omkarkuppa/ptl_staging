/** @file
  ITSS public library for PTL SOC.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/ItssPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

/**
  This function sets the 8254CGE bit
  After BIOSDONE, the function only works inside SMM.

  @param[in]  ClockGateEnable    Enable or disable clock gate

  @retval  EFI_SUCCESS           Set 8254CGE status successfully.
           EFI_UNSUPPORTED       Doesn't support 8254CGE set.
**/
EFI_STATUS
PtlPcdItssSet8254ClockGateState (
  IN BOOLEAN          ClockGateEnable
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   ItssPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_ITSS;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &ItssPcrAccess);

  return ItssSet8254ClockGateState (ClockGateEnable, (REGISTER_ACCESS *) &ItssPcrAccess);
}
