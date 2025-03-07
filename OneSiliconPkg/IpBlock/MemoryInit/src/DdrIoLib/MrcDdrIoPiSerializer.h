/** @file
  This file contains code related to configuration and calibration of the Phase Interpolator.

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

#ifndef MRC_DDR_IO_PI_SERIALIZER_SI_SPECIFIC_H_
#define MRC_DDR_IO_PI_SERIALIZER_SI_SPECIFIC_H_

#include "MrcDdrIoApi.h"


/**
  Save and restore certain PI and refpi CR's that must be zero during PI Serializer DCC

  @param[in] MrcData          - Include all MRC global data.
  @param[in] SaveOrRestore    - MrcSaveEnum: read registers->SaveConfig; MrcRestoreEnum: write SaveConfig->registers
  @param[in, out] SaveConfig  - CR's to save and restore.
*/
VOID
MrcDccPiSrzSaveAndRestore (
  IN OUT MrcParameters* const        MrcData,
  IN     MrcSaveOrRestore            SaveOrRestore,
  IN OUT MRC_DCC_PI_SRZ_SAVE_CONFIG* SaveConfig
);

/**
  Sets up all the required PHY register bit-fields to execute the DCC PI serializer calibration.

  @param[in] MrcData          - Include all MRC global data.
*/
VOID
MrcConfigurePhyForPiSerializer (
  IN OUT MrcParameters *const MrcData
);

/**
  Determines if any configuration during DCC Pi Serializer calibration has any dependencies on LPDDR5 memory type.

  @retval TRUE if LPDDR5 memory type does not require separate setup else FALSE
*/
BOOLEAN
MrcIgnoreLpddr5Check (
  VOID
);

/**
  Gets the DCA code (Tx DQS DCC offset) for Data/CCC partition.

  @returns DCA code
*/
INT64
MrcGetDcaCode (
  VOID
);

/**
  Gets the lower DCA code (Tx DQS DCC offset) for WCK partition.

  @returns Lower DCA code
*/
INT64
MrcGetLowerWCKDcaCode (
  VOID
);

/**
  Gets the upper DCA code (Tx DQS DCC offset) for WCK partition.

  @returns Upper DCA code
*/
INT64
MrcGetUpperWCKDcaCode (
  VOID
);

/**
  This function returns the DCC code delta to convert Data/CCC DCC step size to psec.

  @returns DCC code delta
**/
UINT16
MrcGetDCCCodeDelta (
  VOID
);

/**
  This function returns the DCC code delta to convert WCK DCC step size to psec.

  @returns DCC code delta
**/
UINT16
MrcGetWCKDCCCodeDelta (
  VOID
);

/**
  Programs DCA code (Tx DQS/DQ DCC offset) for Data partition.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] DcaCode  - DCA code to be programmed
*/
VOID
MrcConfigureDataDcaCode (
  IN OUT MrcParameters *const MrcData,
  IN INT64   DcaCode
);

/**
  This function does calculation for DCC Step Size of WCK and CCC

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] RankPop      - Array of Rank Population.
  @param[in] UpdateTco    - TcoCode or DccCode.
  @param[in] MeasPoint    - Value to set the MeasPoint.
  @param[in, out] UnexpectedResultOut - Return of whether result is unexpected.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcCalcWckCccDccStepSize (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8  RankPop[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL],
  IN     BOOLEAN UpdateTco,
  IN     UINT8 MeasPoint,
  IN OUT BOOLEAN *UnexpectedResultOut
  );

/**
  This function returns the DCC mask.

  @returs DCC mask for executing large change reset.
**/
UINT32
MrcGetDccTypeMask (
  VOID
);

/**
  Enable CK per channel based on populated.
  Population uses CCC.LaneEn which already includes both ChPopulation and DDR5 CLK usage from DDR Init.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      RankPop - Array of Rank Population.
**/
VOID
MrcEnableCk (
  IN OUT MrcParameters *const MrcData,
  IN UINT8 const RankPop[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL]
  );

/**
  Enable Data per channel based on populated.

  @param[in, out] MrcData - Include all MRC global data.
**/
VOID
MrcEnableDccFsmOnCurrentRank (
  IN OUT MrcParameters *const MrcData
  );

/**
  Enable Data per channel based on populated.

  @param[in, out] MrcData - Include all MRC global data.
**/
VOID
MrcEnableWck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates then returns DCC step size for CCC

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] Ave1 - First data average, must be less than Ave2
  @param[in] Ave2 - Second data average, must be greater than Ave1
  @param[in] Divisor - Divides the subtraction of the averages.
  @param[in] Max - We expect it to be a positive value. Maximum number the retvalue can be.
  @param[in] Steps - We expect it to be a positive value. Number of steps iterated.
  @param[in] DivisorFactor - divisor factor. tick code delta * number of samples.

  @retval Dcc step size. Calculated by dividing the difference between Ave2 and Ave1, then adding half
  of the former to our DCC step size formula, dividing it by the full value and constraining that
  result in a certain range.
  We have it as an INT64 to prevent typecasting for GetSets, although it should be positive in this function.
**/
INT64
MrcCCCDccStepSizeFormula (
  IN     MrcParameters* MrcData,
  IN     UINT32  Ave1,
  IN     UINT32  Ave2,
  IN     UINT32  Divisor,
  IN     INT64   Max,
  IN     UINT16  Steps,
  IN     INT64   DivisorFactor
  );

/**
  Checks if the current configuration can run Pi Serializer calibration.

  @param[in] MrcData - Include all MRC global data.

  @returns TRUE if current configuration supports running Pi Serializer calibration else FALSE.
**/
BOOLEAN
MrcPISerializerCalRunCheck (
  IN MrcParameters *const MrcData
  );

/**
  Print DCC PI Serializer results per partition

  @param[in]  MrcData - Include all MRC global data.
**/
VOID
MrcDccPiSrzPrintResults (
  IN  MrcParameters *const  MrcData
  );
#endif // MRC_DDR_IO_PI_SERIALIZER_SI_SPECIFIC_H_
