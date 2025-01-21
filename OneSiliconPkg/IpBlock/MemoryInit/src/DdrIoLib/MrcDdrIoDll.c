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
  Save or restore RxDLL VctlInit values

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      SaveRestore   - Control flag to save registers or to restore registers.
  @param[in, out] RxDllVctlInit - data to save / restore
**/
VOID
MrcVctlInitSaveRestore (
  IN     MrcParameters* const MrcData,
  IN     MrcSaveOrRestore     SaveOrRestore,
  IN OUT UINT32               RxDllVctlInit[MRC_DATA_MOBILE_NUM][MRC_DATA_CH_NUM]
  )
{
  UINT32    ChIdx;
  UINT32    DataIdx;
  UINT32    Offset;
  DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT  DllControl0;

  for (DataIdx = 0; DataIdx < MRC_DATA_MOBILE_NUM; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDLLCONTROL0_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      DllControl0.Data = MrcReadCR (MrcData, Offset);
      if (SaveOrRestore == MrcSaveEnum) {
        RxDllVctlInit[DataIdx][ChIdx] = DllControl0.Bits.VctlInit;
      } else {
        DllControl0.Bits.VctlInit = RxDllVctlInit[DataIdx][ChIdx];
        MrcWriteCR (MrcData, Offset, DllControl0.Data);
      }
    }
  }
}

/**
  Enable/Disable DLL WeakLock if needed.
  Note: We don't enable it in McConfig because CKE is still low during that step.

  @param[in] MrcData - The MRC general data.
  @param[in] Enable - BOOLEAN control to enable (if TRUE), or disable (if FALSE) WeakLock.

  @retval None
**/
void
MrcWeaklockEnDis (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              Enable
  )
{
  INT64           GetSetVal;

  GetSetVal = (Enable) ? 1 : 0;
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDllWeakLock, WriteCached, &GetSetVal);
  MrcGetSetPartitionBlock(MrcData, PartitionDataShared, MRC_DATA_SHARED_NUM, GsmIocTxDllWeakLock, WriteCached, &GetSetVal);
  MrcGetSetPartitionBlock(MrcData, PartitionCccShared, MRC_CCC_SHARED_NUM, GsmIocTxDllWeakLock, WriteCached, &GetSetVal);
}
