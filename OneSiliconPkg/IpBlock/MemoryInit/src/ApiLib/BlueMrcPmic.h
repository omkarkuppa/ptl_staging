/** @file
  DDR5 PMIC Register Definitions

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

@par Specification Reference: PMIC5100 Spec Rev 0.99c
**/

#ifndef  _BLUE_MRC_PMIC_H
#define  _BLUE_MRC_PMIC_H

/**
  This function changes the DDR5 DIMM Voltages using PMIC.

  @param[in, out] MrcData                 - The MRC "global data" area.
  @param[in]      SpdAddress              - Selects the Dimm by SpdAddress
  @param[in]      IsPmicSupport10MVStep   - Pmic Supports 10mv step size
  @param[in]      Vdd                     - New Vdd Voltage
  @param[in]      Vddq                    - New Vddq Voltage
  @param[in]      Vpp                     - New Vpp Voltage

  @retval VOID.
**/
VOID
MrcDefaultSetMemoryPmicVoltage (
  IN OUT MrcParameters  *MrcData,
  IN UINT8              SpdAddress,
  IN BOOLEAN            IsPmicSupport10MVStep,
  IN UINT32             Vdd,
  IN UINT32             Vddq,
  IN UINT32             Vpp
  );

#endif   // _BLUE_MRC_PMIC_H

