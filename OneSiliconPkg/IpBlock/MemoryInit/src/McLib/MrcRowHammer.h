/** @file
    This file contains the Row Hammering training algorithm definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _MrcRowHammer_h_
#define _MrcRowHammer_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"

#define DRFM_ENABLED_BOTH_DIMMS  (0x3) // Enables DRFM on both DIMMs for each Channel
#define DRFM_DISABLED_BOTH_DIMMS (0x0) // Disables DRFM on both DIMMs for each Channel (which therefore enables pTRR on both DIMMs)

#define SUBTRACT_NO_OVERFLOW(val,sub) (((val) > (sub)) ? ((val) - (sub)) : (0)) // Keep overflow from occurring after subtraction.

typedef struct {
  UINT8  RfmDimmRequiredMap;
  UINT32 Raaimt;
  UINT32 RaaimtOneDimm;
  UINT32 Raammt;
  UINT32 RaammtOneDimm;
  UINT32 RefSub;
  UINT32 RefSubOneDimm;
  UINT32 NormalRefSub;
  UINT32 NormalRefSubOneDimm;
} MRC_RFM_SETUP_CONFIG;

/**
  Configure Row Hammer mitigation modes if they are supported and enabled

  @param[in] MrcData - The MRC general data.

  @retval - mrcSuccess if nothing went wrong while checking and configuring Row Hammer Mitigation Options
**/
MrcStatus
MrcRhPrevention (
  IN MrcParameters *const MrcData
  );

/**
  Configure Pseudo Targeted Row Refresh (pTRR) on the given controller / channel

   @param[in]  MrcData      - Pointer to global MRC data.
   @param[in]  Controller   - MC index
   @param[in]  Channel      - Channel index
   @param[in]  DimmMap      - Bitmap of present DIMMs of the Channel
**/
VOID
MrcConfigPtrr (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                DimmMap
  );

/**
  Print Adaptive Refresh Management (ARFM) Level according to JEDEC Spec

  @param[in] MrcData - The MRC general data.
**/
VOID
MrcPrintDramRfmMode (
  IN MrcParameters *const MrcData
  );

#endif // _MrcRowHammer_h_
