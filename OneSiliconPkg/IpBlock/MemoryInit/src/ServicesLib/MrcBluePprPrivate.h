/**@file

  This file contains all private common training algorithm helpers.

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

@par Specification
**/

#ifndef MRC_BLUE_PPR_PRIVATE_H_
#define MRC_BLUE_PPR_PRIVATE_H_
#include "MrcCommon.h"

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcBluePprPrivate)

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE

**/
MrcStatus
DispositionFailRangesWithPprFlow (
  IN     MrcParameters      *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN     UINT32                     McChBitMask,
  IN OUT UINT8                      *RepairDone
);

/**
  Checks if a Post Package Repair succeeded and updates internal outputs structure based on the results.

  @param[in] MrcData     - Global MRC data structure
  @param[in] PprAmtData  - PPR and AMT data structure
  @param[in] Controller  - Contrller index
  @param[in] Channel     - Channel index
  @param[in] Bank        - Current bank address
  @param[in] Row         - Row address to run test

  @retval True when PPR succeeded.
  @retval False when PPR failed.
**/
BOOLEAN
MrcPostPprAnalysis (
  IN MrcParameters  *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Bank,
  IN UINT32                         Row
  );

/**
  Retrieves the Post Package Repair (PPR) channel bit mask for a specified controller and channel.

  @param[in]  MrcData     - Pointer to the MRC global data structure
  @param[in]  Controller  - The controller index
  @param[in]  Channel     - The channel index

  @returns The PPR channel bit mask for the specified controller and channel.
**/
UINT8
MrcGetPprChBitMask (
  IN MrcParameters  *const          MrcData,
  IN UINT32                         Controller,
  IN UINT32                         Channel
);
#endif // MRC_BLUE_PPR_PRIVATE_H_
