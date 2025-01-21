/** @file
  Implementation of memory abstraction interface code that runs
  on the Intel processor.

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

#include "MrcMemoryApi.h" // temp to ensure prototypes match implementation (prototypes will move soon)
#include "MrcLpddr5.h"  // for MRC_LP5_BANKORG
#include "MrcDdr5.h"  // for MRC_DDR5_tPPD_ALL_FREQ
#include "MrcSpdProcessingDefs.h"  // for SdramCapacityTable[]

/**
  This function looks up the SAGV MR sequence and delays for the memory being initialized.
  The values will be stored in the array pointers passed in sequential order.
  The function will use the Length parameter to ensure it does not overflow the array passed.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[out] MrSeq   - Pointer to array storing MR addresses.
  @param[out] MrDelay - Pointer to array storing MR delay after write.
  @param[in, out] Length - Pointer containing the number of entries available in the array.  Will be updated to the number of entries used upon return.
  @param[out] MrPerRank  - Output pointer to an array containing a list of MRs that must be configured on a per-rank basis
  due to possible unique values per rank. The is terminated using the value mrEndOfSequence.

  @retval MrcStatus - mrcSuccess if no errors
  @retval MrcStatus - mrcUnsupportedTechnology if the memory type is not supported.
  @retval MrcStatus - The return value of MrcSagvMrSeq per technology function.
**/
MrcStatus
MrcGetSagvMrSeq (
  IN      MrcParameters *const  MrcData,
  OUT     MrcModeRegister       *MrSeq,
  OUT     GmfTimingIndex        *MrDelay,
  IN OUT  UINT32                *Length,
  OUT     MrcModeRegister       **MrPerRank OPTIONAL
  )
{
  MrcStatus Status;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcSagvMrSeqLpddr5 (MrcData, MrSeq, MrDelay, Length, MrPerRank);
      break;

    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s %s\n", gErrString, gUnsupportedTechnology);
      Length = 0;
      Status = mrcUnsupportedTechnology;
      if (MrPerRank != NULL) {
        *MrPerRank = NULL;
      }
      break;
  }

  return Status;
}


/**
  This function returns the tRFM in DCLKs.
  DDR5 timing is based on tRFMsb
  LPDDR5 timing is based on tRFMpb

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetTrfm (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcOutput     *Outputs;
  MrcDdrType     DdrType;
  MrcChannelOut  *ChannelOut;
  UINT32         tRFM;
  UINT32         tCKps;
  UINT32         DeviceCapacity;

  Outputs   = &MrcData->Outputs;
  DdrType   = Outputs->DdrType;
  tCKps     = Outputs->tCKps;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];

  DeviceCapacity = MAX ((SdramCapacityTable[ChannelOut->Dimm[dDIMM0].DensityIndex] * 8),
                        (SdramCapacityTable[ChannelOut->Dimm[dDIMM1].DensityIndex] * 8));
  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tRFM = MrcGetDdr5Trfm (DeviceCapacity);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tRFM = MrcGetLpddr5Trfm (DeviceCapacity);
      break;

    default:
      tRFM = 0;
      break;
  }

  tRFM = DIVIDECEIL (tRFM / tCKps, 1);

  return tRFM;
}
