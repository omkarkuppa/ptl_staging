/** @file
  This file provides GPIO topology for PTL_PCD in MIN phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Library/Ptl/PcdMinGpioTopologyLib/PtlPcdMinGpioTopologyLib.h>
#include <Library/Ptl/PtlPcdP2SbSocLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/IoLib.h>
#include <PtlPcdSbPortIds.h>
#include <Register/Ptl/GpioV2PcdRegs/GpioV2PtlPcdRegs.h>


//
// List of groups for Community 0
//
GPIOV2_MIN_GROUP PtlPcdMinCommunity0Groups[] = {
  {
    // Group: GPP_V
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_V_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_V_HOSTSW_OWN
  },
  {
    // Group: GPP_C
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_C_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_C_HOSTSW_OWN
  }
};

//
// List of groups for Community 1
//
GPIOV2_MIN_GROUP PtlPcdMinCommunity1Groups[] = {
{
    // Group: GPP_F
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_F_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_F_HOSTSW_OWN
  },
  {
    // Group: GPP_E
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_E_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_E_HOSTSW_OWN
  }
};

//
// List of groups for Community 3
//
GPIOV2_MIN_GROUP PtlPcdMinCommunity3Groups[] = {
  {
    // Group: CPUJTAG
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_CPUJTAG_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_CPUJTAG_HOSTSW_OWN
  },
  {
    // Group: GPP_H
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_H_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_H_HOSTSW_OWN
  },
  {
    // Group: GPPASPI0
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPPASPI0_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPPASPI0_HOSTSW_OWN
  },
  {
    // Group: vGPIO_3
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_3_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_3_HOSTSW_OWN
  }
};

//
// List of groups for Community 4
//
GPIOV2_MIN_GROUP PtlPcdMinCommunity4Groups[] = {
  {
    // Group: GPP_S
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_S_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_S_HOSTSW_OWN
  }
};

//
// List of groups for Community 5
//
GPIOV2_MIN_GROUP PtlPcdMinCommunity5Groups[] = {
  {
    // Group: GPP_B
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_B_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_B_HOSTSW_OWN
  },
  {
    // Group: GPP_D
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_GPP_D_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_GPP_D_HOSTSW_OWN
  },
  {
    // Group: vGPIO
    .DW0RegisterOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_PAD_CFG_DW0,
    .HostOwnRegOffset = R_GPIOV2_PTL_PCD_PCR_VGPIO_HOSTSW_OWN
  }
};

//
// Communities structure
//
GPIOV2_MIN_COMMUNITY PtlPcdMinCommunities[] = {
  {
    // Community 0 info
    .Groups = PtlPcdMinCommunity0Groups,
    .Pid = PTL_SID_F2_PID_GPCOM0
  },
  {
    // Community 1 info
    .Groups = PtlPcdMinCommunity1Groups,
    .Pid = PTL_SID_F2_PID_GPCOM1
  },
  {
    // Community 3 info
    .Groups = PtlPcdMinCommunity3Groups,
    .Pid = PTL_SID_F2_PID_GPCOM3
  },
  {
    // Community 4 info
    .Groups = PtlPcdMinCommunity4Groups,
    .Pid = PTL_SID_F2_PID_GPCOM4
  },
  {
    // Community 5 info
    .Groups = PtlPcdMinCommunity5Groups,
    .Pid = PTL_SID_F2_PID_GPCOM5
  }
};


/**
  Retrieve number of communities in GPIO controller.

  @retval  Number of communities
**/
UINT32
PtlPcdMinGpioGetCommunitiesNum (
  VOID
  )
{
  return ARRAY_SIZE (PtlPcdMinCommunities);
}

/**
  Retrieve pointer to community for PCH with community index.

  @param[in] CommunityIndex       Community Index

  @retval                         Pointer to community on requested Index
**/
GPIOV2_MIN_COMMUNITY*
PtlPcdMinGpioGetCommunity (
  IN UINT32 CommunityIndex
  )
{
  return &(PtlPcdMinCommunities[CommunityIndex]);
}

/**
  This procedure retrieves register offset for given Gpio Pad

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdMinGpioGetDw0RegOffset (
  IN  GPIOV2_PAD          GpioPad,
  OUT UINT32              *RegisterOffset
  )
{
  GPIOV2_MIN_COMMUNITY *Community;
  UINT32 CommunityIndex;
  UINT32 GroupIndex;
  UINT32 PadIndex;

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  Community = PtlPcdMinGpioGetCommunity (CommunityIndex);

  *RegisterOffset = Community->Groups[GroupIndex].DW0RegisterOffset + PadIndex * 0x10;
  return EFI_SUCCESS;
}

/**
  This procedure retrieves register offset for given Gpio Pad

  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdMinGpioGetHostOwnRegOffset (
  IN  GPIOV2_PAD          GpioPad,
  OUT UINT32              *RegisterOffset
  )
{
  GPIOV2_MIN_COMMUNITY *Community;
  UINT32 CommunityIndex;
  UINT32 GroupIndex;
  UINT32 PadIndex;

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  Community = PtlPcdMinGpioGetCommunity (CommunityIndex);

  *RegisterOffset = Community->Groups[GroupIndex].HostOwnRegOffset + PadIndex * 0x10;
  return EFI_SUCCESS;
}

/**
Retrieve Community PID based on community index.

@param[in] CommunityIndex Community Index

@RetVal Pointer to community on requested Index
**/
UINT32
PtlPcdMinGpioGetCommunityPid (
IN UINT32 CommunityIndex
)
{
  if (CommunityIndex < ARRAY_SIZE(PtlPcdMinCommunities)) {
    return PtlPcdMinCommunities[CommunityIndex].Pid;
  }
 return 0;
}


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
  )
{
  if (Pid == NULL || P2SbController == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (CommunityIndex >= PtlPcdMinGpioGetCommunitiesNum ()) {
    DEBUG ((DEBUG_ERROR, "Invalid Community Index: %d\n", CommunityIndex));
    return EFI_INVALID_PARAMETER;
  }

  // Retrieve the P2SB controller for the specified community
  Pid->Pid16bit = ((UINT16) (PtlPcdMinGpioGetCommunityPid (CommunityIndex)));
  return PtlPcdGetP2SbController (P2SbController, *Pid);
}

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
  )
{
  UINT32                          RegisterOffset;
  UINT32                          AndValue;
  UINT32                          OrValue;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_PORT_16_ID                 Pid;
  UINT32                          Value;
  UINT32                          CommunityIndex;
  EFI_STATUS                      Status;

  if (GpioPad == 0) {
    ASSERT (GpioPad == GPIOV2_PAD_NONE);
    return EFI_INVALID_PARAMETER;
  }
  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  Status = GetP2SbControllerForCommunity (CommunityIndex, &P2SbController, &Pid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get P2SB controller for community %d\n", CommunityIndex));
    return Status;
  }
  //
  //
  //  Get DW0 Register Offset to program Pad Mode
  //
  PtlPcdMinGpioGetDw0RegOffset (GpioPad, &RegisterOffset);

  //
  //  Program Pad Mode
  //
  AndValue = (UINT32)~(GPIOV2_PAD_MODE_MASK << GPIOV2_PAD_MODE_DW0_POS);
  OrValue  = ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK) << GPIOV2_PAD_MODE_DW0_POS;
  Value    = MmioRead32(P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, RegisterOffset));
  Value    &= AndValue;
  Value    |= OrValue;
  MmioWrite32 ( P2SB_PCR_ADDRESS (P2SbController.Mmio, Pid.PortId.LocalPid, RegisterOffset), Value);

  return EFI_SUCCESS;
}