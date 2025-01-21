/** @file
  This file contains code related to initializing and configuring the DDRIO Rcomp.

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

#ifndef _MrcDdrIoComp_h_
#define _MrcDdrIoComp_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"

/// Constants
extern const UINT8 CompParamList[5];

/**
  This function determines default RcompTargets based on CPU type and DDR Type

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Profile - MemoryProfile

  @retval Returns pointer to RcompTarget
**/
const UINT16 *
MrcGetDefaultRcompTarget (
  IN OUT MrcParameters *const MrcData,
  IN     MrcProfile Profile
  );

/**
  This function calculates the Slew Rate target CR's based on the requested slope.
  It does not run a compensation cycle and does not do limit checking of the CR range.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  SlopePs    - Slew Rate slope in picoseconds.
  @param[out] CompCells  - Return location for calculated Comp Cells value.
  @param[out] CompPC     - Return location for calculated Phase/Cycle config.
  @param[out] CompDiv2En - Return location for calculated clock divider config.

  @retval MrcStatus - mrcWrongInputParameter if CompCells, CompPC or CompDiv2En is NULL.
                    - mrcFail if Target SlopePs is too low to be supported in 16 delay cells.
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCalcScomp (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                SlopePs,
  OUT UINT32                *CompCells,
  OUT UINT32                *CompPC,
  OUT UINT32                *CompDiv2En
  );


/**
  This function runs Comp Update and then a Full Comp cycle.

  @param[in] MrcData  - Pointer to global MRC data.
  @param[in] CompType - Enum of Comp being forced

  @returns the status returned from calling ForceRcomp.
**/
MrcStatus
MrcDccForceComp (
  IN MrcParameters *const  MrcData,
  IN COMP_CYCLE_TYPE       CompType
  );

/**
  This function prints DdrIo COMP registers related to VccDdq.

  @param[in] MrcData - All the MRC global data.
**/
VOID
MrcPrintDdrIoCompVddq (
  IN MrcParameters* const MrcData
);

#endif // _MrcDdrIoComp_h_
