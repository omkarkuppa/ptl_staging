/** @file
  This file contains code related to initializing and configuring the DDRIO Duty Cycle Correction

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
#include "MrcDdrIoApiInt.h" // for prototypes
#include "MrcCommon.h"

/**
  This function toggles the DllDacCodeFreeze in RxDLL only

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] Value   - Value to be programmed for DllDacCodeFreeze
**/
VOID
ToggleRxDllDacCodeFreeze (
  IN MrcParameters *const MrcData,
  IN UINT8         Value
  )
{
  UINT32              ChIdx;
  UINT32              DataIdx;
  UINT32              Offset;
  DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT  DllControl0;

  for (DataIdx = 0; DataIdx < MRC_DATA_MOBILE_NUM; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDLLCONTROL0_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      DllControl0.Data = MrcReadCR (MrcData, Offset);
      DllControl0.Bits.DllDacCodeFreeze = Value;
      MrcWriteCR (MrcData, Offset, DllControl0.Data);
    }
  }
}

/**
  This function toggles the DllDacCodeFreeze

  @param[in, out] MrcData - Pointer to MRC global data.
  @param[in]      Value   - Value to be programmed for DllDacCodeFreeze

**/
VOID
ToggleDllDacCodeFreeze (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8         Value
  )
{
  UINT8   Index;
  UINT32  Offset;
  DATASHARED_CR_DDRCRTXDLLCONTROL0_STRUCT        TxDllControl0;
  DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT    DdrCccTxDllControl0;

  ToggleRxDllDacCodeFreeze (MrcData, Value);

  for (Index = 0; Index < MRC_DATA_MOBILE_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_REG, DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL0_REG, Index);
    TxDllControl0.Data = MrcReadCR (MrcData, Offset);
    TxDllControl0.Bits.DllDacCodeFreeze = Value;
    MrcWriteCR (MrcData, Offset, TxDllControl0.Data);
  }

  for (Index = 0; Index < MRC_CCC_SHARED_MOBILE_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0_REG, DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL0_REG, Index);
    DdrCccTxDllControl0.Data = MrcReadCR (MrcData, Offset);
    DdrCccTxDllControl0.Bits.DllDacCodeFreeze = Value;
    MrcWriteCR (MrcData, Offset, DdrCccTxDllControl0.Data);
  }
}
