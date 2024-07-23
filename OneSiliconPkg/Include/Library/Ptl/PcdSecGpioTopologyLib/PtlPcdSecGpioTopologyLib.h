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
} GPIOV2_SEC_GROUP;

typedef struct GPIOV2_SEC_COMMUNITY {
  GPIOV2_SEC_GROUP                    *Groups;
  UINT32                              Pid;
} GPIOV2_SEC_COMMUNITY;

  /**
  This procedure retrieves register offset for given Gpio Pad for SEC phase

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdSecGpioGetDw0RegOffset (
  IN  GPIOV2_PAD       GpioPad,
  OUT UINT32           *RegisterOffset
  );

/**
  Retrieve Community PID based on community index.

  @param[in] CommunityIndex       Community Index

  @retval                         Pointer to community on requested Index
**/
UINT32
PtlPcdSecGpioGetCommunityPid (
  IN UINT32 CommunityIndex
  );
  

#endif //_PTL_PCD_SEC_GPIOV2_TOPOLOGY_H_
