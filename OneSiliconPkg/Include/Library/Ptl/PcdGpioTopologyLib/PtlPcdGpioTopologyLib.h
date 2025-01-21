/** @file
  Header file for common part for Gpio V2 Controller PEI and DXE modules

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

#ifndef _GPIOV2_TOPOLOGY_PTL_SOC_H_
#define _GPIOV2_TOPOLOGY_PTL_SOC_H_

#include <Library/GpioV2AccessLib.h>
#include <GpioV2ControllerInterface.h>

/**
  This procedure retrieves number of communities for PTL SOC.

  @retval  Number of communities on PTL SOC
**/
UINT32
PtlPcdGpioGetCommunitiesNum (
  VOID
  );

/**
  This procedure retrieves pointer to array of communities for PTL SOC.

  @retval  pointer to array of communities on PTL SOC.
**/
GPIOV2_COMMUNITY *
PtlPcdGpioGetCommunities (
  VOID
  );

/**
  This procedure retrieves number of native signals for PTL SOC

  @retval  Number of native signals on PTL SOC
**/
UINT32
PtlPcdGpioGetNativeSignalsNum (
  VOID
  );

/**
  This procedure retrieves pointer to array of native signals for PTL SOC

  @retval  pointer to array of native signals on PTL SOC
**/
GPIOV2_NATIVE_SIGNAL_DATA *
PtlPcdGpioGetNativeSignals (
  VOID
  );

/**
  This procedure retrieves Community PID based on community index.

  @param[in] CommunityIndex       Community Index

  @retval                         PID of requested community index.
**/
UINT32
PtlPcdGpioGetCommunityPid (
  IN UINT32 CommunityIndex
  );

/**
  This procedure retrieves register offset for given Gpio Pad

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in]  Register            Register for which user want to retrieve offset. Please refer to GpioV2Pad.h
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdGpioGetRegOffset (
  IN  GPIOV2_PAD       GpioPad,
  IN  GPIOV2_REGISTER  Register,
  OUT UINT32           *RegisterOffset
  );

#endif //_GPIOV2_TOPOLOGY_PTL_SOC_H_
