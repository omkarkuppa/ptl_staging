/** @file
  This file provides GPIO topology for PTL_PCD in SEC phase.

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

#include <GpioV2Pad.h>
#include <GpioV2ControllerInterface.h>
#include <Library/Ptl/PcdSecGpioTopologyLib/PtlPcdSecGpioTopologyLib.h>
#include <Register/Ptl/GpioV2PcdRegs/GpioV2PtlPcdRegs.h>
#include <PtlPcdSbPortIds.h>
//
// List of groups for Community 0
//
GPIOV2_SEC_GROUP PtlPcdSecCommunity0Groups[] = {
  {
    // Group: GPP_V
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_V_PAD_CFG_DW0
  },
  {
    // Group: GPP_C
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_C_PAD_CFG_DW0
  }
};

//
// List of groups for Community 1
//
GPIOV2_SEC_GROUP PtlPcdSecCommunity1Groups[] = {
{
    // Group: GPP_F
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_F_PAD_CFG_DW0
  },
  {
    // Group: GPP_E
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_E_PAD_CFG_DW0
  }
};

//
// List of groups for Community 3
//
GPIOV2_SEC_GROUP PtlPcdSecCommunity3Groups[] = {
  {
    // Group: CPUJTAG
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_CPUJTAG_PAD_CFG_DW0
  },
  {
    // Group: GPP_H
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_H_PAD_CFG_DW0
  },
  {
    // Group: GPPASPI0
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPPASPI0_PAD_CFG_DW0
  },
  {
    // Group: vGPIO_3
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_3_PAD_CFG_DW0
  }
};

//
// List of groups for Community 4
//
GPIOV2_SEC_GROUP PtlPcdSecCommunity4Groups[] = {
  {
    // Group: GPP_S
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_S_PAD_CFG_DW0
  }
};

//
// List of groups for Community 5
//
GPIOV2_SEC_GROUP PtlPcdSecCommunity5Groups[] = {
  {
    // Group: GPP_B
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_B_PAD_CFG_DW0
  },
  {
    // Group: GPP_D
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_D_PAD_CFG_DW0
  },
  {
    // Group: vGPIO
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_PAD_CFG_DW0
  }
};

//
// Communities structure
//
GPIOV2_SEC_COMMUNITY PtlPcdSecCommunities[] = {
  {
    // Community 0 info
    .Groups = PtlPcdSecCommunity0Groups,
    .Pid = PTL_SID_F2_PID_GPCOM0
  },
  {
    // Community 1 info
    .Groups = PtlPcdSecCommunity1Groups,
    .Pid = PTL_SID_F2_PID_GPCOM1
  },
  {
    // Community 3 info
    .Groups = PtlPcdSecCommunity3Groups,
    .Pid = PTL_SID_F2_PID_GPCOM3
  },
  {
    // Community 4 info
    .Groups = PtlPcdSecCommunity4Groups,
    .Pid = PTL_SID_F2_PID_GPCOM4
  },
  {
    // Community 5 info
    .Groups = PtlPcdSecCommunity5Groups,
    .Pid = PTL_SID_F2_PID_GPCOM5
  }
};

/**
  Retrieve pointer to community for PCH with community index.

  @param[in] CommunityIndex       Community Index

  @retval                         Pointer to community on requested Index
**/
GPIOV2_SEC_COMMUNITY*
PtlPcdSecGpioGetCommunity (
  IN UINT32 CommunityIndex
  )
{
  return &(PtlPcdSecCommunities[CommunityIndex]);
}

/**
  This procedure retrieves register offset for given Gpio Pad

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdSecGpioGetDw0RegOffset (
  IN  GPIOV2_PAD          GpioPad,
  OUT UINT32              *RegisterOffset
  )
{
  GPIOV2_SEC_COMMUNITY *Community;
  UINT32 CommunityIndex;
  UINT32 GroupIndex;
  UINT32 PadIndex;

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  Community = PtlPcdSecGpioGetCommunity (CommunityIndex);

  *RegisterOffset = Community->Groups[GroupIndex].DW0RegisterOffset + PadIndex * 0x10;
  return EFI_SUCCESS;
}

/**
Retrieve Community PID based on community index.

@param[in] CommunityIndex Community Index

@RetVal Pointer to community on requested Index
**/
UINT32
PtlPcdSecGpioGetCommunityPid (
IN UINT32 CommunityIndex
)
{
  if (CommunityIndex < ARRAY_SIZE(PtlPcdSecCommunities)) {
    return PtlPcdSecCommunities[CommunityIndex].Pid;
  } else {
    return 0;
  }
}