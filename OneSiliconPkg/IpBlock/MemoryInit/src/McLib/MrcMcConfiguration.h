/** @file
  Non training specific memory controller configuration definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef __MrcMcConfiguration_h__
#define __MrcMcConfiguration_h__

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommandTraining.h"
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcRefreshConfiguration.h"
#include "MrcSchedulerParameters.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcTimingConfiguration.h"

#define MRC_LPMODE4_SRX_DELAY_PS (225000)
#define MRC_WR_OSCL_RUNTIME_PS   (1 * 1000 * 1000)
#define MRC_RD_OSCL_RUNTIME_PS   (1 * 1000 * 1000)

/**
  This function finalizes the Memory Controller configuration to prepare for normal operation.
  Some items are set to special values for training that need to be set to the POR value.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful
**/
MrcStatus
MrcMcFinalize (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function verifies if memory is unlocked before making writes to DDRPHY
  @param[in] MrcData - Include all MRC global data.
  @retval MrcStatus - mrcSuccess if successful or mrcTimeout if error
**/
MrcStatus
MrcPmaUnlockStatus (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function sets the PMA Configuration. This should only be run once by the first workpoint

  @param[in, out] MrcData - MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPmaConfig (
  IN OUT MrcParameters *const MrcData
  );

/**
  Use the roundtrip matching feature in the MC to align Roundtrip to the rddata_valid on SPID.
  Issue one Read (MRR/MPR) transaction and use FSM in the MC to find the roundtrip gap.

  @param[in] MrcData - all the global data

  @retval mrcSuccess if all is well
**/
MrcStatus
MrcRoundTripMatch (
  IN     MrcParameters *const MrcData
  );

/**
  In order to stress more easily the display worst case access pattern from the memory we can suggest a change in behavior
  in the IPQ to mimic such case.
  Assumption on worst case pattern occurs due to unfortunate page allocation that does not utilize all the memory banks but
  a small subset of them (one in the extreme case)
  The idea is is to limit the IPQ to only allow operation with one bank at a time, yet still benefit from serving existing PH
  requests to the open bank as even if fewer numbers of banks are used, once a page is open the controller is going to serve
  all the existing PH requests and not just one.
  @param[in] MrcData - all the global data
**/
void
MrcMimicWcDisplayPatternInIPQ (
  IN     MrcParameters *const MrcData
  );

/**
  Programs Safe mode values for specific register fields
  Register values are updated depending on the MC Safe Mode bitmask
  This function does not contain all the MC registers that need to be programmed
    with safe mode values. Some are dispersed throughout the code to not disrupt code flow

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel

**/
VOID
MrcMcApplySafeConfig (
  IN MrcParameters *MrcData,
  IN UINT32         Controller,
  IN UINT32         Channel
  );

/**
  Configures registers to enable LpMode4 features.

  @param[in] MrcData - all the global data
**/
VOID
MrcLpMode4Enable (
  IN OUT MrcParameters *const MrcData
  );

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
  );

/**
  Enables parity generation and disables parity checking (NVL only)
  The register cache should be flushed after calling this function.

  @param[in] MrcData    - Include all MRC global data.

**/
VOID
MrcEnableMcParityGen (
  IN MrcParameters *MrcData
  );

#endif // __MrcMcConfiguration_h__
