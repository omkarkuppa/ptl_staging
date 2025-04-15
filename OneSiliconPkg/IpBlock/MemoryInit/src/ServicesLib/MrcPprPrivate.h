/**@file

  This file contains all private common training algorithm helpers.

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

@par Specification
**/

#ifndef MRC_PPR_PRIVATE_H_
#define MRC_PPR_PRIVATE_H_
#include "MrcCommon.h"

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcPprPrivate)

/**
  Run PPR Advanced Memory Tets.

  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprAdvancedMemoryTests (
  IN MrcParameters *const MrcData,
  PPR_AMT_PARAMETER_DATA  *const PprAmtData
  );

/**
  Execute PPR force repair routine.

  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprForceRepair (
  IN MrcParameters *const MrcData,
  PPR_AMT_PARAMETER_DATA  *const PprAmtData
  );

/**
  Checks if a specific PPR Entry Address exists in the system.

  @param[in] MrcData global MRC data structure.
  @param[in] PprEntryAddress PPR Entry Address to check.

  @returns TRUE if address exists, otherwise FALSE.
**/
BOOLEAN
MrcIsPprEntryAddressExists (
  IN MrcParameters *const MrcData,
  IN MRC_PPR_ENTRY_ADDRESS PprEntryAddress
  );

#endif //MRC_PPR_PRIVATE_H_
