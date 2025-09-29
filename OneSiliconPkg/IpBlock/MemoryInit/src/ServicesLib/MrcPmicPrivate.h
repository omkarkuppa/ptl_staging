/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef MRC_PMIC_PRIVATE_H
#define MRC_PMIC_PRIVATE_H

PRIVATE_HEADER_DEFENDER (MrcPmicPrivate)

typedef enum PMIC_RAIL {
  PmicRailSwa = 0,
  PmicRailSwb,
  PmicRailSwc,
  PmicRailSwd,
  MaxPmicRails,
} PMIC_RAIL;

// Make sure that it is aligned with PMIC_RAIL enum
static const UINT8 PmicRailToVoltageRegMap[MaxPmicRails] = {
  PMIC_REG_21,
  PMIC_REG_25,
  PMIC_REG_27,
  PMIC_REG_23
};

/**
  This function configures PMIC voltages for LPDDR5.

  @param[in, out] MrcData - MRC global data.
**/
VOID
PmicVoltageConfigurationLpddr5 (
  IN OUT MrcParameters *MrcData
  );

/**
  Get PMIC type from MRC global data structure.

  @param[in] MrcData - MRC global data.

  @return PMIC type.
**/
SPD_PMIC_DEVICE_TYPE
GetPmicType (
  IN OUT MrcParameters *MrcData
  );

/**
  Enable PMIC programmable mode.

  @param[in] MrcData - MRC global data.

  @returns mrcSuccess if successful, otherwise an error code.
**/
MrcStatus
EnableProgrammableMode (
  IN OUT MrcParameters *MrcData
  );

/**
  Send VR Enable command to PMIC.

  @param[in] MrcData - MRC global data.
**/
VOID
MrcEnablePmic (
  IN OUT MrcParameters *MrcData
  );

/**
  Program PMIC voltages.

  @param[in] MrcData - MRC global data.
  @param[in] PmicVoltages - PMIC voltages to be programmed.
**/
VOID
ProgramVoltages (
  IN OUT MrcParameters *MrcData,
  const UINT16* const PmicVoltages
  );

/**
  Get PMIC reg field value for encoded voltage.
  @param[in] Voltage - Voltage to encode.
  @param[in] Rail - PMIC rail type.
  @param[in] StepSize - PMIC step size.

  @returns Encoded voltage value.
**/
UINT8
GetPmicVoltageRegValue (
  UINT16 Voltage,
  PMIC_RAIL Rail,
  PMIC_STEP_SIZE StepSize
);

/**
 * Print PMIC registers.
 *
 * @param[in] MrcData - MRC global data.
 * @param[in] SpdAddress - SPD address
 */
VOID
MrcPmicPrintRegisters (
  IN OUT MrcParameters *MrcData,
  UINT8 SpdAddress
  );

#endif // MRC_PMIC_PRIVATE_H
