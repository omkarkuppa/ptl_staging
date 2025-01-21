/** @file
This API provides an interface to abstract out memory protocol differences.
The functions here are only run on the IA core(s).

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

#ifndef BLUE_MRC_JEDEC_API_H_
#define BLUE_MRC_JEDEC_API_H_

#include "CMrcApi.h"  // indirectly provides "IN" and "OUT"
#include "MrcMemoryApi.h"  // for LPDDR_ODT_TYPE
#include "MrcRowHammer.h"  // for MRC_RFM_SETUP_CONFIG

/**
  This function returns the ODTL timing.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Current data rate.
  @param[in]  OdtType   - Selects which command ODT type.
  @param[in]  OdtlParam - Determines if this is for ODT On or Off.

  @retval INT8 - ODT impact to the command timing.
**/
INT8
MrcGetOdtlTiming (
  IN  MrcParameters *const  MrcData,
  IN  MrcFrequency          Frequency,
  IN  LPDDR_ODT_TYPE        OdtType,
  IN  LPDDR_ODTL_PARAM      OdtlParam
  );

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
  );

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
  );

/**
  This function completes setting up the Generic MRS FSM configuration to enable SAGV during normal operation.

  @param[in] MrcData  - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
**/
MrcStatus
MrcFinalizeDdr5MrSeq (
  IN  MrcParameters *const MrcData
  );

/**
  Check if Adaptive Refresh Management (ARFM) supported on a DIMM
  for a user requested ARFM level (A, B or C) or lower.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - DIMM number
   @param[out] ArfmLevelToConfigure - Maximum supported ARFM Level lower than requested,
                                      only valid if function returns TRUE

   @retval - TRUE if ARFM is supported
   @retval - FALSE if ARFM is not supported
**/
BOOLEAN
MrcDdr5IsArfmSupported (
  IN  MrcParameters*      const MrcData,
  IN  UINT32              Controller,
  IN  UINT32              Channel,
  IN  UINT32              Dimm,
  OUT MrcDramRfmModeType  *ArfmLevelToConfigure
  );


/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR58

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcDdr5IsRfmRequired (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );


/**
  Grab Refresh Management (RFM) threshold info from MR59
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] Raaimt          - RAA Initial Management Threshold (RAAIMT)
   @param[out] Raammt          - RAA Maximum Management Threshold (RAAMMT)
   @param[out] RefSub          - Decrement for RAA from RFMab or RFMpb commands (RAAIMT * RAADEC)
   @param[out] NormalRefSub    - Decrement for RAA from normal refresh commands (RAAIMT)

   @retval - mrcFail if bad RAAIMT value
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcDdr5GetRfmThresholdInfo (
  IN  MrcParameters *const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );


/**
  Check if Directed Refresh Management (DRFM) is supported on a DIMM.
  Returns true if any rank on a DIMM supports DRFM.

   @param[in]  MrcData        - Pointer to global MRC data
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - TRUE if at least one rank on the DIMM supports DRFM
   @retval - FALSE if no ranks on the DIMM support DRFM
**/
BOOLEAN
MrcDdr5IsDrfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );


/**
  Configure Adaptive Refresh Management (ARFM) in DRAM.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - Dimm number
   @param[in]  ArfmLevelToConfigure - ARFM level to configure in DRAM

   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcDdr5ConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  IN  MrcDramRfmModeType ArfmLevelToConfigure
  );


/**
  Configure Directed Refresh Management (DRFM) Blast Radius Configuration (BRC) in DRAM.

   @param[in]  MrcData          - Pointer to global MRC data
   @param[in]  Controller       - MC index
   @param[in]  Channel          - Channel index
   @param[in]  Dimm             - Dimm index
   @param[out] BrcSupported     - True if BRC > 2 supported by the DIMM

   @retval - mrcSuccess if DRFM configured properly
**/
MrcStatus
MrcDdr5ConfigDrfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT BOOLEAN *BrcSupported
  );

/**
  This function changes the DDR5 DIMM Voltages using PMIC.

  @param[in, out] MrcData - The MRC "global data" area.
**/
VOID
MrcDdr5VoltageCheckAndSwitch (
  IN OUT MrcParameters      *MrcData
  );

#endif // BLUE_MRC_JEDEC_API_H_
