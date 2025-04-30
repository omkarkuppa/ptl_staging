/** @file
  This file contains code related to initializing and configuring the DDRIO Rcomp.

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

#ifndef MRC_DDR_IO_COMP_H
#define MRC_DDR_IO_COMP_H

#include "CMrcStatsTracker.h" // for MrcStatsStartTimer()
#include "CMrcTypes.h"
#include "CMrcApi.h"

/**
  This function determines default RcompTargets based on CPU type and DDR Type

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Profile - MemoryProfile

  @retval Returns pointer to RcompTarget
**/
const UINT16 *
MrcGetDefaultRcompTarget (
  IN OUT MrcParameters *const MrcData,
  IN     MrcProfile Profile
  );

/**
  This function calculates the Slew Rate target CR's based on the requested slope.
  It does not run a compensation cycle and does not do limit checking of the CR range.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  SlopePs    - Slew Rate slope in picoseconds.
  @param[out] CompCells  - Return location for calculated Comp Cells value.
  @param[out] CompPC     - Return location for calculated Phase/Cycle config.
  @param[out] CompDiv2En - Return location for calculated clock divider config.

  @retval MrcStatus - mrcWrongInputParameter if CompCells, CompPC or CompDiv2En is NULL.
                    - mrcFail if Target SlopePs is too low to be supported in 16 delay cells.
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCalcScomp (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                SlopePs,
  OUT UINT32                *CompCells,
  OUT UINT32                *CompPC,
  OUT UINT32                *CompDiv2En
  );


/**
  This function runs Comp Update and then a Full Comp cycle.

  @param[in] MrcData  - Pointer to global MRC data.
  @param[in] CompType - Enum of Comp being forced

  @returns the status returned from calling ForceRcomp.
**/
MrcStatus
MrcDccForceComp (
  IN MrcParameters *const  MrcData,
  IN COMP_CYCLE_TYPE       CompType
  );

/**
  This function prints DdrIo COMP registers related to VccDdq.

  @param[in] MrcData - All the MRC global data.
**/
VOID
MrcPrintDdrIoCompVddq (
  IN MrcParameters* const MrcData
);

/**
  This function does Force Comp and Polls

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval mrcSuccess if Poll returns expected otherwise mrcDeviceBusy.
**/
MrcStatus
ForceRcomp (
  IN OUT MrcParameters *const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  );

/**
  Wrap Force Comp and Polls with stats timer to record time spent in the routine.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval mrcSuccess Poll returns expected
  @retval mrcDeviceBusy did not return expected.
**/
MrcStatus
ForceRcompNotCheckAllCompDone (
  IN OUT MrcParameters *const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  );

/**
  The function preforms a frequency switch Rcomp

  @param[in, out] MrcData - MRC global data.
**/
VOID
FreqSwitchComp (
  IN OUT MrcParameters *const MrcData
  );

/**
  Retrieve the current memory frequency from PHY and clock from the memory controller.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in, out] MemoryClock  - The current memory clock.
  @param[in, out] Ratio        - The current memory ratio setting.

  @retval: The current memory frequency.
**/
MrcFrequency
MrcGetPhyCurrentMemoryFrequency (
  MrcParameters* const   MrcData,
  UINT32* const          MemoryClock,
  MrcClockRatio* const   Ratio
  );

/**
  This function programs DdrIo COMP registers and related to VccDdq.

  @param[in] MrcData          - All the MRC global data.
  @param[in] Print            - Whether to print debug
  @param[in] VccddqVoltageMv  - Voltage in mV

  @retval mrcSuccess if all registers configured.
**/
MrcStatus
DdrIoSetVddqImpactedCrs (
  IN MrcParameters *const MrcData,
  IN BOOLEAN  Print,
  IN UINT32   VccddqVoltageMv
  );

/**
  This function calculates the Up/Dn values for the Param (RdOdt/WrDS/WrDSCmd/WrDSCtl/WrDSClk).

  @param[in]   MrcData - All the MRC global data.
  @param[in]   Param   - Parameter to calculate Up/Dn values
  @param[in]   RcompTarget - RcompTarget Override
  @param[in]   Print       - Print out or not
  @param[out]  UpValue     - Value for Up component.
  @param[out]  DnValue     - Value for Dn component.

  @retval mrcSuccess if Param is supported
  @retval mrcFail otherwise
**/
MrcStatus
CalcUpDnVref (
  IN MrcParameters *const MrcData,
  IN UINT32    Param,
  IN UINT16    RcompTarget[MAX_RCOMP_TARGETS],
  IN BOOLEAN   Print,
  OUT INT16    *UpValue,
  OUT INT16    *DnValue
  );

/**
  This function calculates the Dq Rtarg_pup value using Vref_pupcode formula:
  Vref_pupcode = 193 * (Rext / (Rext + Rtarg_pup))
  Rtarg_pup = Rext * ((193 / Vref_pupcode) - 1)

  @param[in] MrcData          - All the MRC global data.
  @param[in] DqVrefUpValue    - The DqVrefUp value.

  @retval Returns Dq Rtarg_pup value
**/
UINT32
CalcDQRodtValueFromDqOdtVrefUp (
  IN MrcParameters *const MrcData,
  IN UINT32  DqVrefUpValue
  );

/**
  Run the comp engine continuously. If the comp completes before the timeout, run it again.
  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     TestLength  - Run time in us
  @retval mrcSuccess Poll returns expected
  @retval mrcDeviceBusy did not return expected.
**/
MrcStatus
ForceRcompContinuous (
  IN OUT MrcParameters* const MrcData,
  IN     UINT32               TestLength
  );

/**
  This function writes to the ForceCkStop field when Xtensa is enabled

  @param[in, out] MrcData        - Include all MRC global data.
  @param[in]      ForceCkStopVal - Set to 1 to bypass Dunit control and force high
  @param[in]      DelayTimeNs    - The amount of time to wait before writing to field.
**/
void
WriteForceCkStopToMptu (
  IN OUT MrcParameters *const MrcData,
  IN     INT64                ForceCkStopVal,
  IN     UINT32               DelayTimeNs
  );

/**
  This function does Force Comp result distribution WA

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval NA
**/
VOID
CompResultDistribution (
  IN OUT MrcParameters* const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  );

#endif // MRC_DDR_IO_COMP_H
