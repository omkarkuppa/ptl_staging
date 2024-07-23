/** @file
  Chip Library Routing definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _MrcChipRouting_h_
#define _MrcChipRouting_h_

#include "CMrcInterface.h"  // for MrcParameters


/**
  This function locks the DDR frequency requested from SPD, User, or POR Tables.
  It will update the frequency related members in the output structure.

  McFrequencySet(): ChipApi - Accesses PMA and DDRIO.  Initializes MrcData Variables
  MrcInternalCheckPoint(): Api - External/Customer/Flow control notification of state.
  MrcSetMrcVersion(): PmaApi - Writes PMA register which stores MRC FW Version.

  @param[in, out] MrcData - MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcWorkPointLock (
  IN OUT MrcParameters *const MrcData
  );

#endif // _MrcChipRouting_h_
