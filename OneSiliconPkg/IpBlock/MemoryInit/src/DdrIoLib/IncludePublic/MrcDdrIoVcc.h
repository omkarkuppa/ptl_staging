/** @file
  This file contains code related to initializing and configuring the DDRIO VCC.

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

#ifndef _MrcDdrIoVcc_h_
#define _MrcDdrIoVcc_h_

#include "CMrcInterface.h"  // for MrcParameters

#define MAX_VCCDDQCONTROL_TARGET  (383)
#define SBCLOCK_DEFAULT           (1000000 / 400)

/**
  Set the final VccIog value into Outputs->VccIogVoltage

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
SetVccIo (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the VccClk using linear interpolation. Sets the final value into Outputs->VccClkVoltage

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccClk (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the WakeInc
  LVRRamp*(SBClock/1000) / (BGRef/512*5/(5-FBDivider) )

  @param[in] MrcData - All the MRC global data.
  @param[in] SBClock
  @param[in] FBDivider

  @return calculated value for WakeInc
**/
UINT32
GetWakeInc (
  IN MrcParameters *const MrcData,
  IN UINT32   SBClock,
  IN UINT32   FBDivider
);

/**
  This function programs the Vcc LVR registers

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccLvr (
  IN MrcParameters *const MrcData
  );

/**
  This function sets the final value into Outputs->VccddqVoltage

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Vccddq  - Vccddq
  @retval Returns none
**/
VOID
SetVddq (
  IN MrcParameters *const MrcData,
  IN UINT32               Vccddq
  );

/**
  This function calculates the VddqControl Target value based on VccddqVoltage in Outputs structure

  @param[in] MrcData - All the MRC global data.
  @param[in] VccddqVoltageMv  - Voltage in mV

  @retval calculated value for Target field for VddqControl registers
**/
UINT32
GetVddqControlTarget (
  IN MrcParameters *const MrcData,
  IN UINT32   VccddqVoltageMv
 );

#endif // _MrcDdrIoVcc_h_
