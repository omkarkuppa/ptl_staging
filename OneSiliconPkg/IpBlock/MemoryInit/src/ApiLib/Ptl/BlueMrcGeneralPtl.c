/** @file
  This file contains all the MRC general API to the MRC wrapper.

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
#include "MrcCommon.h"
#include "MrcLpddr5.h"
#include "MrcChipApi.h"

/**
  This function sets the project specific overrides in the Input and Output structure

  @param[in, out] MrcData - All the MRC global data.
 */
VOID
MrcSetPrjOverrides (
  IN OUT MrcParameters *const MrcData
  )
{
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcOutput           *Outputs;

  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  Outputs   = &MrcData->Outputs;

  // Resolving 'Auto' for weaklock and rxdqsdelay comp:
  //  Low frequency:  Enable weaklock and disable RxDqsDelay comp
  //  High frequency: Disable weaklock and enable RxDqsDelay comp
  if (ExtInputs->WeaklockEn == MrcAuto) {
    Outputs->WeaklockEn = (Outputs->Frequency <= f3200);
  } else {
    Outputs->WeaklockEn = (ExtInputs->WeaklockEn == MrcEnable);
  }
  if (ExtInputs->RxDqsDelayCompEn == MrcAuto) {
    Outputs->RxDqsDelayCompEn = (Outputs->Frequency > f3200);
  } else {
    Outputs->RxDqsDelayCompEn = (ExtInputs->RxDqsDelayCompEn == MrcEnable);
  }
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "RxDqsDelayCompEn: %sabled\n", Outputs->RxDqsDelayCompEn ? "En" : "Dis");
}

/**
  Modify tRDRDdg and tRDRDsg timings by 1tCK. Does not apply modification to tRDRDsg
  in BG mode.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Enable  - TRUE to add 1tCK, FALSE to subtract.
 */
VOID
MrcModifyRdRdTimings (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              Enable
  )
{
  MrcOutput       *Outputs;
  MRC_LP5_BANKORG Lp5BGOrg;
  INT64           tRdRdOffset;
  INT64           RdWrInterleaving;

  Outputs  = &MrcData->Outputs;
  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  RdWrInterleaving = Enable ? 1 : 0;
  tRdRdOffset = Enable ? + 4 : -4;

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctRDRDdg, WriteOffsetCached, &tRdRdOffset);

  if (Lp5BGOrg == MrcLp516Bank) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctRDRDsg, WriteOffsetCached, &tRdRdOffset);
  }

  if (Lp5BGOrg == MrcLp5BgMode) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccDisLpddr5RdwrInterleaving, WriteCached, &RdWrInterleaving);
  }
}

/**
 * @brief Get the Mc Ibecc Hash Mask and LSB values.
 *
 * @param[in]      MrcData  - All the MRC global data.
 * @param[in, out] HashMask - Pointer to the Hash Mask set.
 * @param[in, out] HashLsb  - Pointer to the Hash LSB to set.
 */
VOID
GetMcIbeccHash (
  IN     MrcParameters *const MrcData,
  IN OUT UINT32               *HashMask,
  IN OUT UINT32               *HashLsb
  )
{
  BOOLEAN   IsDdr5;

  IsDdr5    = MrcData->Outputs.IsDdr5;

  *HashMask = IsDdr5 ? 0x2098 : 0x2094;
  *HashLsb  = IsDdr5 ? 3 : 2;
}

/**
  This function is a wrapper for MrcGenMrsFsmClean().

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] CleanAll - If set to TRUE, MrData values will be ignored and all Control Registers will be cleared

  @retval mrcFail if clean failed.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmCleanNonFastBoot (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN       CleanAll
  )
{
  return mrcSuccess;
}
