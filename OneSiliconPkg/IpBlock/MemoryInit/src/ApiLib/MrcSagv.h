/** @file
  MRC Common / Generic functions

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
#ifndef _MrcSagv_h_
#define _MrcSagv_h_

#include "CMrcInterface.h"  // for MrcParameters

typedef enum {
  LowBwSaGv,         ///< Low Bandwidth
  MidBwSaGv,         ///< Medium Bandwidth
  MidHiBwSaGv,       ///< Mid-High Bandwidth
  HighBwSaGv,        ///< High Bandwidth
  LowLatencySaGv,    ///< Low Latency
  RfiHighBwSaGv,     ///< High Bandwidth RFI Mitigation
  RfiLowLatencySaGv, ///< Low Latency RFI Mitigation
  SaGvPointTypeMax,
} SaGvTypes;

/**
  Takes in an input memory frequency (data rate) and calculates next supported frequency
  from the 'Supported Frequencies' tables that is less than or equal to the input frequency.

  Returns the input Freq value if the input Freq is lower than the min supported frequency.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Freq    - The frequency to

  @retval The supported frequency <= input Freq
**/
MrcFrequency
MrcGetNextSupportedFreq (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Freq
  );

/**
  This function returns the current configuration of Frequency and Gear ratio based on the
  current SAGV point, DDR type, and SAGV input parameters: SaGvFreq, SaGvGear.

  It will update FreqOut and GearOut with the result.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvPoint   - Current operating SAGV point.
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations
  @param[in]  MaxQclkFreq - The maximum QCLK frequency supporte by the current hardware
  @param[out] FreqOut     - Pointer to return the SAGV point Frequency.
  @param[out] GearOut     - Pointer to return the SAGV point Gear.
**/
VOID
MrcGetSagvConfig (
  IN  MrcParameters *const  MrcData,
  IN  MrcSaGvPoint          SaGvPoint,
  IN  MrcFrequency          FreqMax,
  IN  UINT32                MaxQclkFreq,
  OUT MrcFrequency          *FreqOut,   OPTIONAL
  OUT BOOLEAN               *GearOut    OPTIONAL
  );

/**
  Determine if the SaGv configuration is allowed.

  @param[in] MrcData - include all the MRC general data.

  @retval mrcSuccess - If SaGv Config is allowed
          mrcFailure - Anything else
**/
MrcStatus
MrcSaGvBoundsCheck (
  IN  MrcParameters *const  MrcData
  );

/**
  Determine if SaGvWpMask is a fixed point.

  @param[in] MrcData - include all the MRC general data.

  @retval TRUE  - If SaGv is Fixed Point.
          FALSE - If SaGv is anything else.
**/
BOOLEAN
MrcIsSaGvFixed (
  IN  MrcParameters *const  MrcData
  );

/**
  Save the effective SaGv configuration for the current SaGv Point
  for output to the wrapper.

  @param[in,out] MrcData - include all the MRC general data.
**/
void
MrcSaveSagvOutputs (
  IN OUT MrcParameters *const  MrcData
  );

/**
  SA GV flow for the cold boot

  @param[in] MrcData - include all the MRC general data.

  @retval mrcStatus if succeeded.
**/
MrcStatus
MrcSaGvSwitch (
  IN  MrcParameters *const  MrcData
  );

/**
  This function provides sum of low frequency SAGV points

  @param[out] MrcData     - The Mrc Host data structure

  @retval LowFrequencySagvSum Sum of low frequency SAGV points
**/
UINT32
MrcGetLowFrequencySagvPointsSum (
  IN MrcParameters  *MrcData
  );

/**
  This function fills in the MRS FSM to finalize the SAGV configuration for normal operation.

  @param[in] MrcData - The global host structure

  @retval mrcSuccess.
**/
MrcStatus
MrcSaGvFinal (
  IN     MrcParameters *const MrcData
  );

/**
  This function sets the MRC override values for Sagv freq and gear

  @param[in]  MrcData     - Pointer to global MRC data.
**/
VOID
MrcSagvOverride (
  IN  MrcParameters *const MrcData
  );

/**
  Returns the frequency point POR type for the input SaGvPoint and the current
  global system configuration.

  Returns HighBwSaGv by default if the current config is not specified in the POR config tables.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvPoint   - Current operating SAGV point.
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations.

  @retval The SAGV Point config type
**/
SaGvTypes
MrcGetSaGvPointType (
  IN  MrcParameters *const  MrcData,
  IN  MrcSaGvPoint          SaGvPoint,
  IN  MrcFrequency          FreqMax
  );

/**
  This function returns the current configuration of Frequency and Gear ratio based on the
  input SaGvTypem, FreqMax, MaxQclkFreq, and global system configuration.

  It will update FreqOut and GearOut with the result.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvType    - Point config type
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations
  @param[in]  MaxQclkFreq - The maximum QCLK frequency supported by the current hardware
  @param[out] FreqOut     - Pointer to return the SAGV point Frequency.
  @param[out] GearOut     - Pointer to return the SAGV point Gear.
**/
VOID
MrcCalcSagvTypeConfig (
  IN  MrcParameters *const  MrcData,
  IN  SaGvTypes             SaGvType,
  IN  MrcFrequency          FreqMax,
  IN  UINT32                MaxQclkFreq,
  OUT MrcFrequency          *FreqOut,     OPTIONAL
  OUT UINT32                *GearRatioOut OPTIONAL
  );

/**
  Determine if SaGv is enabled from SaGv Input.

  @param[in] MrcData - include all the MRC general data.

  @retval TRUE  - If SaGv is enabled.
          FALSE - If SaGv is disabled.
**/
BOOLEAN
MrcIsSaGvEnabled (
  IN  MrcParameters *const  MrcData
  );

/**
  Enable Refresh Interval Rate (RIR) feature per channel for DDR5.
  All DIMMs in channel must support RIR to enable feature.

  @param[in] MrcData - include all the MRC general data.

  @returns nothing.
**/
VOID
MrcRirInit (
  IN MrcParameters *const  MrcData
  );

#endif // _MrcGeneral_h_
