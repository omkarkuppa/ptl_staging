/** @file
  This file contains project independent code related to initializing and
  configuring the DDRIO CCC Partition.

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

**/

#include "MrcDdrIoApi.h"  // for prototypes
#include "MrcCommon.h"  // for MrcChannelExist()
#include "MrcHalRegisterAccess.h"  // for MrcGetSetLimits()
#include "MrcDdrIoPiSerializer.h" // for DCC PI Serializer support functions
#include "MrcDdrIoComp.h"

static const UINT8 CccDdr5IlMap[MRC_CCC_NUM] = {
  0,  // CCC0
  2,  // CCC1
  1,  // CCC2
  3,  // CCC3
  4,  // CCC4
  6,  // CCC5
  5,  // CCC6
  7,  // CCC7
};

/**
  This function translates HW CCC Partition instance to RAL instance based on interleave configuration.

  @param[in] MrcData     - MRC global data.
  @param[in] CccInstance - CCC instance to be translated.

  @retval Translated CCC instance.
**/
UINT8
MrcTranslateCccInstance (
  IN MrcParameters *const MrcData,
  IN UINT8          const CccInstance
  )
{
  MrcInput *Inputs;
  UINT8    CccTranslatedInstance;

  Inputs  = &MrcData->Inputs;

  if (Inputs->ExtInputs.Ptr->CccPinsInterleaved) {
    CccTranslatedInstance = CccDdr5IlMap[CccInstance];
  } else {
    CccTranslatedInstance = CccInstance;
  }
  return CccTranslatedInstance;
}

/**
  Get the max CMD Groups per channel associated with the current memory technology

  @param[in] MrcData  - Pointer to global MRC data.

  @return The maximum number of CMD Groups per channel for the current memory technology
**/
UINT8
MrcGetCmdGroupMax (
  MrcParameters *const MrcData
  )
{
  return (MrcData->Outputs.IsDdr5) ? MRC_DDR5_CMD_GRP_MAX: 1;
}

/**
  This function decodes Controller/Channel/Rank to a group index.
  Desktop has two CCC groups for CS in MC0.Ch0.  Rank2/3 are assigned to group 1
  while Rank0/1 is group 0.  All other combinations are a single group.

  @param[in] MrcData    - Pointer to global MRC data.
  @param[in] Controller - 0-based index
  @param[in] Channel    - 0-based index
  @param[in] Rank       - 0-based index

  @retval CccChCsPiGroup - Group Index value.
**/
CccChCsPiGroup
MrcGetCsRankPiGroup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank
  )
{
  CccChCsPiGroup GroupIdx;

  if (MrcData->Inputs.IsDdrIoDtHalo && (Controller == 0) && (Channel == 0) && (Rank >= MAX_RANK_IN_DIMM)) {
    GroupIdx = CccChCsPiGroup1;
  } else {
    GroupIdx = CccChCsPiGroup0;
  }

  return GroupIdx;
}
