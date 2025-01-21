/** @file
  Header file for PchFia19

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Register/PchFia19Regs.h>
#include <Library/PchFiaLib.h>

/**
  Get FIA lane owner

  @param[in] FiaInst  FIA Instance
  @param[in] LaneNum  lane number

  @retval PCH_FIA_LANE_OWNER  FIA lane owner
**/
PCH_FIA_LANE_OWNER
PchFia19GetLaneOwner (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        LaneNum
  );

/**
  Print FIA LOS registers.

  @param[in] FiaInst  FIA Instance
**/
VOID
PchFia19PrintLosRegisters (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        MaxLaneNum
  );

/**
  Returns number of FIA lanes

  @param[in] FiaInst  FIA Instance

  @return Number of FIA lanes
**/
UINT8
PchFia19GetMaxLaneNum (
  IN  FIA_INSTANCE FiaInst
  );

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] FiaInst        FIA Instance
  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFia19AssignPchPciePortsClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        *ClkReqMap,
  IN UINT8        ClkReqMapSize
  );

/**
  Print CLKREQ# assigined to PCH PCIe ports

  @param[in] FiaInst        FIA Instance
**/
VOID
PchFia19PrintPciePortsClkReq (
  IN UINT8        FiaPid,
  IN FIA_INSTANCE FiaInst
  );

/**
  Assigns CLKREQ# to GbE

  @param[in] FiaInst    FIA Instance
  @param[in] ClkReqNum  CLKREQ# number
**/
VOID
PchFia19AssignGbeClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        ClkReqNum
  );

/**
  Configures lower bound of delay between ClkReq assertion and driving RefClk.

  @param[in] FiaInst      FIA Instance
  @param[in] OutputDelay  Clock output delay
**/
VOID
PchFia19SetClockOutputDelay (
  IN FIA_INSTANCE            FiaInst,
  IN FIA_CLOCK_OUTPUT_DELAY  OutputDelay
  );

/**
  Disables ClkReq mapping for PCIe root port

  @param[in] FiaInst  FIA Instance
  @param[in] RpIndex  Root port index
**/
VOID
PchFia19DisablePchPciePortClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT32       RpIndex
  );
