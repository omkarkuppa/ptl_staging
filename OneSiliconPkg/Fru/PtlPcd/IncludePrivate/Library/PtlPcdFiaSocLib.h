/** @file
  Header file for FiaSocLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _FIA_SOC_LIB_H_
#define _FIA_SOC_LIB_H_

#include <Library/PchFiaLib.h>

/**
  Returns a FIA lane number for a GbE controller

  @param[out] LaneNum  FIA lane number that is owned by GbE

  @retval TRUE   Found FIA lane assigned to GbE
  @retval FALSE  No lanes assigned to GbE
**/
BOOLEAN
PtlPcdFiaGetGbeLaneNum (
  OUT UINT8  *LaneNum
  );

/**
  Returns a FIA lane number for a given PCIe lane.

  @param[in]  PciePhysicalLane  Index of the PCIe lane
  @param[out] LaneNum           Pointer to the variable that stores lane number.
                                The output value is only valid if this function returns TRUE.

  @return TRUE if given PciePhysicalLane owns FIA lane, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaGetPcieLaneNum (
  IN  UINT32  PciePhysicalLane,
  OUT UINT8   *LaneNum
  );

/**
  Print FIA LOS registers.
**/
VOID
PtlPcdFiaPrintLosRegisters (
  VOID
  );

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFiaAssignPchPciePortsClkReq (
  IN UINT8  *ClkReqMap,
  IN UINT8  ClkReqMapSize
  );

/**
  Disables ClkReq mapping for PCIe root port

  @param[in] FiaInst  FIA Instance
  @param[in] RpIndex  Root port index
**/
VOID
PtlPcdFiaDisablePchPciePortClkReq (
  IN UINT32  RpIndex
  );

/**
  Assigns CLKREQ# to GbE

  @param[in]  ClkReqNum  CLKREQ# number
**/
VOID
PchFiaAssignGbeClkReq (
  IN UINT8  ClkReqNum
  );

/**
  Configures lower bound of delay between ClkReq assertion and driving RefClk.
**/
VOID
PtlPcdFiaSetClockOutputDelay (
  VOID
  );

/**
  Performs FIA programming required at the end of configuration and locks lockable FIA registers
**/
VOID
PtlPcdFiaFinalizeConfigurationAndLock (
  VOID
  );

/**
  Returns number of FIA lanes

  @return Number of FIA lanes
**/
UINT8
PtlPcdFiaGetMaxLaneNum (
  VOID
  );

/**
  Return FIA lane owner.

  @param[in] FiaInst  FIA Instance
  @param[in] LaneNum  FIA lane number

  @return  Code of the FIA lane owner, PchFiaOwnerInvalid if lane number wasn't valid
**/
PCH_FIA_LANE_OWNER
PtlPcdFiaGetLaneOwner (
  IN  FIA_INSTANCE FiaInst,
  IN  UINT8        LaneNum
  );

/**
  Returns number of FIA lanes

  @param[in] FiaInst  FIA Instance

  @return Number of FIA lanes
**/
UINT8
PtlPcdFiaGetMaxLaneNumEx (
  IN  FIA_INSTANCE FiaInst
  );

/**
  Checks if given lane is DMI

  @param[in]  FiaLaneNum  Fia lane num

  @return TRUE if given lane is DMI, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaIsLaneDmi (
  IN UINT8  FiaLaneNum
  );

/**
  Checks if a given USB3 port is assigned any FIA lane

  @param[in]  Usb3PortIndex  USB3 port index

  @return TRUE if given USB3 port is assigned a FIA lane, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaIsUsb3PortConnected (
  IN UINT32  Usb3PortIndex
  );

#endif
