/** @file
  The functions in this file implement the memory controller registers that
  are not training specific. The file holds the project MC specific changes.

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

#include "MrcAddressDecodeConfiguration.h"
#include "MrcMcConfiguration.h"
#include "MrcMemoryApi.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcDdrIoDefines.h"

/**
  Verifies if memory is unlocked before proceeding to training steps

  @param[in] MrcData - Include all MRC global data.
  @retval MrcStatus - mrcSuccess if successful or mrcTimeout/mrcFail if error
**/
MrcStatus
MrcPmaUnlockStatus (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput         *Inputs;
  MrcOutput        *Outputs;
  MrcStatus        Status;
  MRC_FUNCTION     *MrcCall;
  MrcDebug         *Debug;
  UINT64           Timeout;
  BOOLEAN          Done;
  MEMSS_PMA_CR_MISC_ENABLES_STRUCT MemssPmaCrMiscEnables;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Status  = mrcSuccess;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;

  // Verify that memory is unlocked
  // Poll until MEM_UNLOCK_STS_VALID becomes 1, then read MEM_UNLOCK_STS
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;
  do {
    MemssPmaCrMiscEnables.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_MISC_ENABLES_REG);
    Done = (MemssPmaCrMiscEnables.Bits.MEM_UNLOCK_STS_VALID == 1);
    if (Inputs->ExtInputs.Ptr->SimicsFlag != 0) {
      MemssPmaCrMiscEnables.Bits.MEM_UNLOCK_STS = 1;
      Done = TRUE;
    }
  } while (!Done && (MrcCall->MrcGetCpuTime () < Timeout));

  if (!Done) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s MEM_UNLOCK_STS_VALID Timeout!\n", gErrString);
    Status = mrcTimeout;
  } else {
    if (MemssPmaCrMiscEnables.Bits.MEM_UNLOCK_STS == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s MEM_UNLOCK_STS is 0!\n", gErrString);
      Status = mrcFail;
    }
  }

  return Status;
}

/**
  Configures DDR5 MC Echo Mask.
  This function expects the caller to flush the register cache.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel

**/
VOID
MrcConfigureDdr5EchoMask (
  IN MrcParameters *MrcData,
  IN UINT32         Controller,
  IN UINT32         Channel
  )
{
}

/**
  Enables parity generation and disables parity checking (NVL only)
  The register cache should be flushed after calling this function.

  @param[in] MrcData    - Include all MRC global data.

**/
VOID
MrcEnableMcParityGen (
  IN MrcParameters *MrcData
  )
{
}

/**
  Update tREFI to 97% of nominal value.

  @param[in]      MrcData - Include all MRC global data.
  @param[in, out] tRefi   - tRefi value to update
**/
VOID
MrcUpdateTRefi (
  IN     MrcParameters *MrcData,
  IN OUT UINT32        *tRefi
  )
{
  *tRefi = UDIVIDEROUND (*tRefi * 97, 100);
}
