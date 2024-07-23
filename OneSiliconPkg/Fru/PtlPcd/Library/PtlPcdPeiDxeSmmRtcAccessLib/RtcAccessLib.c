/** @file
  RTC Access public Lib for PTL SOC.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/RtcPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
/**
  Check RTC BIOS interface is locked

  @retval TRUE  RTC BIOS interface is locked
          FALSE RTC BIOS interface is not locked
**/
BOOLEAN
PtlPcdRtcIsBiosInterfaceLock (
  VOID
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   RtcPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_RTC_HOST;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &RtcPcrAccess);

  return RtcIsBiosInterfaceLock ((REGISTER_ACCESS *) &RtcPcrAccess);
}

/**
  Get TopSwap Status

  @retval TRUE  TopSwap is set
  @retval FALSE TopSwap is clear

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
BOOLEAN
PtlPcdTopSwapStatus (
  VOID
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   RtcPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_RTC_HOST;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &RtcPcrAccess);

  return TopSwapStatus ((REGISTER_ACCESS *) &RtcPcrAccess);
}

/**
  Set TopSwap Status

  @param[in]  TopSwapEnable     Enable TopSwap or Disable it.

  @retval     EFI_SUCCESS       TopSwap set successfully
  @retval     EFI_DEVICE_ERROR  SBI command error

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
EFI_STATUS
PtlPcdTopSwapSet (
  IN  BOOLEAN         TopSwapEnable
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   RtcPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_RTC_HOST;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &RtcPcrAccess);

  return TopSwapSet (TopSwapEnable, (REGISTER_ACCESS *) &RtcPcrAccess);
}
