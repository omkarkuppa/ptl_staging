/** @file
  This file contains project independent code related to initializing and
  configuring the DDRIO CCC Partition.

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

**/

#include "MrcDdrIoApi.h"

/**
  Programming of CCC_CR_DDRCRCCCCLKCONTROLS_BlockTrainRst

  @param[in] MrcData - The MRC global data.
  @param[in] BlockTrainReset - TRUE to BlockTrainReset for most training algos.  FALSE for specific training algos that need PiDivider sync.

**/
VOID
MrcBlockTrainResetToggle (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              BlockTrainReset
)
{
  UINT8   Value;
  UINT8   Index;
  UINT32  Offset;
  DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT  CccPinControls;

  Value = (BlockTrainReset) ? 1 : 0;

  for (Index = 0; Index < (MAX_SYS_CHANNEL / MRC_NUM_PAR_PER_SHARED); Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_REG, DDRCCC_SHARED1_CR_DDRCRCCCPINCONTROLS_REG, Index);
    CccPinControls.Data = MrcReadCR (MrcData, Offset);
    if (CccPinControls.Bits.BlockTrainRst == Value) {
      continue;
    }
    CccPinControls.Bits.BlockTrainRst = Value;
    MrcWriteCR (MrcData, Offset, CccPinControls.Data);
  }
}
