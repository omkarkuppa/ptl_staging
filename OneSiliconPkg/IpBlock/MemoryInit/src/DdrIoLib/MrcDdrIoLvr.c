/** @file
  This file contains common code related to initializing and configuring the DDRIO LVR.

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

#include "CMrcTypes.h"
#include "MrcCommon.h"
#include "MrcDdrIoLvr.h"

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
)
{
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;

  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "VccDdq(mV) = %d \n", Vccddq);
  Outputs->VccddqVoltage = Vccddq;
}

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
 )
{
  MrcOutput         *Outputs;
  UINT32            VccDdq;
  UINT32            VccClk;
  UINT32            VccIog;
  UINT32            TempVar1;
  UINT32            TempVar2;
  UINT32            Data;

  Outputs = &MrcData->Outputs;
  VccDdq  = VccddqVoltageMv;
  VccClk  = Outputs->VccClkVoltage;
  VccIog  = Outputs->VccIogVoltage;

  TempVar1 = MAX (VccClk, VccIog);
  TempVar2 = MAX (TempVar1, VccDdq);
  Data = (TempVar2 > 925) ? 1 : 0;
  // Min(383, Rnd( VccDdq/(BGRef/386*IF(OCDivider,1.5,1)) -1) )
  TempVar1 = (Data ? 1500 : 1000) * BG_REF_MV;
  TempVar2 = THOUSAND_MULTIPLIER * VccDdq * MRC_COMP_VREF_CALCULATION_VALUE;
  Data = UDIVIDEROUND (TempVar2, TempVar1) - 1;
  Data = MIN (MAX_VCCDDQCONTROL_TARGET, Data);

  return Data;
}
