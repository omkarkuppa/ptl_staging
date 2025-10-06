/** @file
  This header exposes private members of MrcRowHammer such that they can be
  used in Unit Testing

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

#ifndef MrcRowHammerPrivate_h_
#define MrcRowHammerPrivate_h_

#include "MrcRowHammer.h" // for MRC_RFM_SETUP_CONFIG

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcRowHammerPrivate)

// This enum is used to map BRC value to RH_TRR_CONTROL.BRC field value.
typedef enum {
  TrrControlBrcDisabled = 0,
  TrrControlBrc2,
  TrrControlBrc3,
  TrrControlBrc4
} MrcTrrControlBrcType;

/**
 * This functions maps MrcRhDrfmBrcType to MrcTrrControlBrcType.
 *
 * @param[in] DrfmBrc BRC value as MrcRhDrfmBrcType
 *
 * @returns Convert BRC value as MrcTrrControlBrcType
**/
MrcTrrControlBrcType
ConvertDrfmBrcToTrrControlBrcType (
  MrcRhDrfmBrcType BrcVal
  );

/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR27 (LP5) or MR58 (DDR5)

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcIsRfmRequired (
  IN MrcParameters *const MrcData,
  IN UINT32            Controller,
  IN UINT32            Channel,
  IN UINT32            Dimm
  );

/**
  This function sets a bit in a BitMap (representing a DIMM) to 1 for that
  DIMM.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  Channel    - Channel number
  @param[in]  Dimm       - Dimm number
  @param[out] DimmBitMap - Holds if a DIMM is enabled or exists within each bit.
**/
VOID
MrcSetDimmBit (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  OUT UINT8                *DimmBitMap
  );

/**
  This function assigns default values to RfmSetupConfig fields.

  @param[out] RfmSetupConfig  - Holds Rfm Required and Threshold information
**/
VOID
MrcRfmSetupConfigInit (
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );

/**
  This function checks whether ARFM was requested based on a BIOS knob value.

  @param[in]  MrcData - Pointer to global MRC data.

  @retval - FALSE means ARFM was not requested
  @retval - TRUE means ARFM was requested
**/
BOOLEAN
MrcIsArfmRequested (
  IN MrcParameters *const  MrcData
  );

/**
  Check if Adaptive Refresh Management (ARFM) is supported on a DIMM.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - DIMM number
   @param[out] ArfmLevelToConfigure - Maximum supported ARFM Level lower than requested,
                                      only valid if function returns TRUE. Applies to DDR5 only.

   @retval - TRUE if ARFM is supported
   @retval - FALSE if ARFM is not supported
**/
BOOLEAN
MrcIsArfmSupported (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Dimm,
  OUT MrcDramRfmModeType  *ArfmLevelToConfigure
  );

/**
  If RFM is required: The RFM threshold values (used in operations on Rolling
  Accumulated ACT (RAA) counter) for that DIMM for RFM will be configured.

  @param[in]  MrcData        - Pointer to global MRC data.
  @param[in]  Controller     - MC number
  @param[in]  Channel        - Channel number
  @param[in]  Dimm           - Dimm number
  @param[out] RfmSetupConfig - Holds Rfm Required and Threshold information

  @retval - mrcUnsupportedTechnology if not DDR5 or LP5
  @retval - mrcSuccess if RFM threshold info gathered successfully
*/
MrcStatus
MrcSetRfmThreshold (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );

/**
  Grab Refresh Management (RFM) threshold information.
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter.

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] RfmSetupConfig  - Holds Rfm Required and Threshold information

   @retval - mrcUnsupportedTechnology if not DDR5 or LP5
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcGetRfmThresholdInfo (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel,
  IN UINT32                Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );

/**
  Check if Directed Refresh Management (DRFM) is supported based on MR1 (LP5) or MR59 (DDR5)

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - FALSE mean DRFM is not supported
   @retval - TRUE means DRFM is supported
**/
BOOLEAN
MrcIsDrfmSupported (
  IN MrcParameters *const MrcData,
  IN UINT32            Controller,
  IN UINT32            Channel,
  IN UINT32            Dimm
  );

/**
  Configure Adaptive Refresh Management (ARFM) Levels in MR57 (LP5) or MR59 (DDR5) based on BIOS Inputs

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - Dimm number
   @param[in]  ArfmLevelToConfigure - ARFM level to configure in DRAM

   @retval - mrcUnsupportedTechnology if not DDR5 or LP5
   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  IN  MrcDramRfmModeType   ArfmLevelToConfigure
  );

/**
  Configure Directed Refresh Management (DRFM) in MR75 (LP5) or MR59 (DDR5)
  Configure DRFM on given Conroller/Channel

   @param[in]  MrcData          - Pointer to global MRC data.
   @param[in]  Controller       - MC index
   @param[in]  Channel          - Channel index
   @param[in]  DimmMap          - Bitmap of present dimms of the Channel
   @param[in]  DrfmEnableBits   - 2-bit bit mask indicating which DIMMs support DRFM

   @retval - mrcUnsupportedTechnology if DDR technology unsupported
   @retval - mrcSuccess if DRFM configured properly
**/
MrcStatus
MrcConfigDrfm (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                DimmMap,
  IN UINT8                DrfmEnableBits
  );

/**
  Configure Refresh Management (RFM) on the given controller / channel

   @param[in] MrcData        - Pointer to global MRC data.
   @param[in] Controller     - MC index
   @param[in] Channel        - Channel index
   @param[in] RfmSetupConfig - Holds Rfm Required and Threshold information
**/
VOID
MrcConfigRfm (
  IN MrcParameters *const MrcData,
  IN UINT8  Controller,
  IN UINT8  Channel,
  IN MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );

#endif // MrcRowHammerPrivate_h_
