/** @file
  Header file for common part for Gpio V2 Controller in SEC Phase

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

#ifndef _PTL_PCD_SEC_GPIOV2_TOPOLOGY_H_
#define _PTL_PCD_SEC_GPIOV2_TOPOLOGY_H_

#include <GpioV2Pad.h>
#include <GpioV2ControllerInterface.h>

typedef struct {
  UINT16                           DW0RegisterOffset;
  UINT16                           HostOwnRegOffset;
} GPIOV2_MIN_GROUP;

typedef struct GPIOV2_MIN_COMMUNITY {
  GPIOV2_MIN_GROUP                    *Groups;
  UINT32                              Pid;
} GPIOV2_MIN_COMMUNITY;

  /**
  This procedure retrieves register offset for given Gpio Pad for MIN phase

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdMinGpioGetDw0RegOffset (
  IN  GPIOV2_PAD       GpioPad,
  OUT UINT32           *RegisterOffset
  );

/**
  Retrieve Community PID based on community index.

  @param[in] CommunityIndex       Community Index

  @retval                         Pointer to community on requested Index
**/
UINT32
PtlPcdMinGpioGetCommunityPid (
  IN UINT32 CommunityIndex
  );

/**
  Retrieve number of communities in GPIO controller.

  @retval  Number of communities
**/
UINT32
PtlPcdMinGpioGetCommunitiesNum (
  VOID
  );

/**
  This procedure will Retrieve the P2SB controller for the specified community

  @param[in] CommunityIndex         Community Index
  @param[out] P2SbController        Pointer to P2SB controller structure
  @param[out] Pid                   Pointer to P2SB port ID structure

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
GetP2SbControllerForCommunity (
  IN UINT32           CommunityIndex,
  OUT P2SB_CONTROLLER *P2SbController,
  OUT P2SB_PORT_16_ID *Pid
  );

/**
  This procedure does minimum Gpio Configuration required for Lpss Devices
  in SEC phase.

  @param[in] GpioPad             GPIO pad
  @param[in] PadModeValue        GPIO pad mode value, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
GpioMinSetPadMode (
  IN  GPIOV2_PAD                  GpioPad,
  IN  GPIOV2_PAD_MODE             PadMode
  );
#endif //_PTL_PCD_MIN_GPIOV2_TOPOLOGY_H_
