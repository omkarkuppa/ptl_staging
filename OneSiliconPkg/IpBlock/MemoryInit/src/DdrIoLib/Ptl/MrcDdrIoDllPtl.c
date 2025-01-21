/** @file
  This file contains code releated to DDR IO DLL operations.

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

/**
  This function toggles the DLL Reset.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Value   - DLL Reset value
**/
VOID
MrcToggleDllReset (
  IN MrcParameters * const MrcData,
  IN UINT32                Value
  )
{
  UINT32     Offset;
  UINT32     Index;
  DATASHARED_CR_DDRCRDLLCONTROL1_STRUCT       DllControl1;
  DATASHARED_CR_DDRCRTXDLLCONTROL1_STRUCT     DataTxDllControl1;
  CCCSHARED_CR_DDRCRTXDLLCONTROL1_STRUCT      CccTxDllControl1;

  // Issue DLL Reset
  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL1_REG, DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL1_REG, Index);
    CccTxDllControl1.Data = MrcReadCR (MrcData, Offset);
    CccTxDllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, CccTxDllControl1.Data);
  }
  for (Index = 0; Index < MRC_DATA_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL1_REG, DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL1_REG, Index);
    DataTxDllControl1.Data = MrcReadCR (MrcData, Offset);
    DataTxDllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, DataTxDllControl1.Data);

    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG, DDRDATA_SHARED1_CR_DDRCRDLLCONTROL1_REG, Index);
    DllControl1.Data = MrcReadCR (MrcData, Offset);
    DllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, DllControl1.Data);
  }
}
