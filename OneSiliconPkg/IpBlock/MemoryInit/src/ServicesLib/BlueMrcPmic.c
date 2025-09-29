/** @file
  This file contains DDR5 PMIC programming routines.

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

@par Specification Reference:
**/
#include "MrcCommon.h"
#include "MrcPmic.h"
//#include "BlueMrcPmic.h"

/**
  Perform PMIC Clear Global Status

  @param[in] MrcCall
  @param[in] PmicAddress

  @retval VOID
**/

VOID
MrcPmicClearGlobalStatus (
  IN MRC_FUNCTION      *MrcCall,
  IN UINT8             PmicAddress
  )
{
  PMIC_REG_14_STRUCT  Reg14;
  RETURN_STATUS       Status;

  Reg14.Data = 0;
  Reg14.Bits.global_clear_status = 1;
  MrcCall->MrcSmbusWrite8 (PmicAddress | (PMIC_REG_14 << 8), Reg14.Data, &Status);
}

/**
  This function calculates the wait time after VR_ENABLE

  @param[in] MrcCall
  @param[in] PmicAddress

  @retval UINT32
**/
UINT32
MrcPmicGetPwrGoodOutTime (
  IN MRC_FUNCTION      *MrcCall,
  IN UINT8             PmicAddress
  )
{
  UINT32              PmicPwrGoodOutTime;
  UINT8               R2CSwaDecode;
  UINT8               R2DSwbDecode;
  UINT8               R2DSwcDecode;
  PMIC_REG_2C_STRUCT  Reg2C;
  PMIC_REG_2D_STRUCT  Reg2D;
  RETURN_STATUS       Status;

  Reg2C.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (PMIC_REG_2C << 8), &Status);
  Reg2D.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (PMIC_REG_2D << 8), &Status);

  R2CSwaDecode = (Reg2C.Bits.swa_output_soft_start_time == 0) ? 1 : (2 * Reg2C.Bits.swa_output_soft_start_time);
  R2DSwbDecode = (Reg2D.Bits.swb_output_soft_start_time == 0) ? 1 : (2 * Reg2D.Bits.swb_output_soft_start_time);
  R2DSwcDecode = (Reg2D.Bits.swc_output_soft_start_time == 0) ? 1 : (2 * Reg2D.Bits.swc_output_soft_start_time);

  PmicPwrGoodOutTime = MAX (R2CSwaDecode, R2DSwbDecode);
  PmicPwrGoodOutTime = MAX (PmicPwrGoodOutTime, R2DSwcDecode);

  PmicPwrGoodOutTime += PMIC_POWER_ON_SEQUENCE_CONFIG_IDLE_MS;

  return PmicPwrGoodOutTime;
}

/**
  This function enables DDR5 DIMM's PMIC's programmable mode and send VR_ENABLE to PMIC

  @param[in, out] MrcData            - The MRC "global data" area.

  @retval VOID
**/
VOID
MrcEnableDimmPmic (
  IN OUT MrcParameters      *MrcData
  )
{
  MrcInput              *Inputs;
  MrcOutput             *Outputs;
  MRC_FUNCTION          *MrcCall;
  MrcDebug              *Debug;
  MrcControllerIn       *ControllerIn;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcChannelIn          *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDimmOut            *DimmOut;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 SpdAddress;
  UINT8                 PmicAddress;
  UINT8                 Value;
  UINT32                Offset;
  RETURN_STATUS         Status;
  UINT32                PwrGoodOutTime;
  PMIC_REG_2F_STRUCT    Reg2F;
  PMIC_REG_32_STRUCT    Reg32;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;

  if ((Outputs->DdrType != MRC_DDR_TYPE_DDR5) ||
      (Inputs->BootMode == bmS3) ||
      (Inputs->BootMode == bmWarm)) {
    // Assume PMIC register configuration survives WarmReset and S3
    return;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn  = &Inputs->Controller[Controller];
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelOut  = &ControllerOut->Channel[Channel];
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (ChannelOut->Dimm[Dimm].Status != DIMM_PRESENT) {
          continue;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%uC%uD%u:\n", Controller, Channel, Dimm);
        DimmIn  = &ChannelIn->Dimm[Dimm];
        DimmOut = &ChannelOut->Dimm[Dimm];

        DimmOut->PmicProgrammable = TRUE;

        SpdAddress = DimmIn->SpdAddress;
        if (SpdAddress == 0) {
          // In case of fixed SPD file
          continue;
        }

        PmicAddress = (SpdAddress & 0xE) | (PMIC0_LID_CODE << 4);
        Offset = PMIC_REG_2F;
        Reg2F.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " PMIC reg 0x%02X orig value: 0x%02x\n", Offset, Reg2F.Data);
        if (Reg2F.Bits.secure_mode == 0) {
          // Need enable Programmable Mode
          Reg2F.Bits.secure_mode = 1;
          MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg2F.Data, &Status);
          Reg2F.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " PMIC reg 0x%02X new value: 0x%02x\n", Offset, Reg2F.Data);
          if (Reg2F.Bits.secure_mode != 1) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, " Cannot enable Programmable Mode!\n");
            DimmOut->PmicProgrammable = FALSE;
          }
        }

        MrcPmicClearGlobalStatus (MrcCall, PmicAddress);

        Offset = PMIC_REG_32;
        Reg32.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " PMIC reg 0x%02X orig value: 0x%02x\n", Offset, Reg32.Data);
        if (Reg32.Bits.vr_enable == 0) {
          // Need send VR_ENABLE command, no matter if Reg2F.Bits.secure_mode is Programmable Mode
          Reg32.Bits.vr_enable = 1;
          MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg32.Data, &Status);
          PwrGoodOutTime = MrcPmicGetPwrGoodOutTime (MrcCall, PmicAddress);
          MrcWait (MrcData, PwrGoodOutTime * MRC_TIMER_1MS);
          Value = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " PMIC reg 0x%02X new value: 0x%02x\n", Offset, Value);
        }
      } // For Dimm
    } // For Channel
  } // For Controller
}

/*
 * Per debug by co-workers, we need dump some PMIC registers.
 */
UINT32 PmicDumpRegs[] = {
  0x22,
  0x26,
  0x28
};

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
  )
{
  MRC_FUNCTION          *MrcCall;
  MrcDebug              *Debug;
  UINT32                Offset;
  UINT8                 PmicAddress;
  UINT8                 Value;
  MrcVddSelect          Data;
  MrcVddSelect          Current;
  MrcVddSelect          NewVoltage;
  RETURN_STATUS         Status;
  UINT32                SWAStepSize;
  UINT32                SWBStepSize;
  UINT32                SWCStepSize;
  BOOLEAN               SWANeedSwitchStep;
  BOOLEAN               SWBNeedSwitchStep;
  BOOLEAN               SWCNeedSwitchStep;
  UINT32                Delta;
  PMIC_REG_21_STRUCT    Reg21;
  PMIC_REG_25_STRUCT    Reg25;
  PMIC_REG_27_STRUCT    Reg27;
  PMIC_REG_2B_STRUCT    Reg2B;
  INT32                 Loop;

  MrcCall = MrcData->Inputs.Call.Func;
  Debug   = &MrcData->Outputs.Debug;

  /*
   * Bit0: R/W
   * Bit1~3: HID
   * Bit4~7: LID
   */
  PmicAddress = (SpdAddress & 0xE) | (PMIC0_LID_CODE << 4);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Spd 0x%02x: Begin to update voltages by Pmic\n", SpdAddress);

  SWANeedSwitchStep = FALSE;
  SWBNeedSwitchStep = FALSE;
  SWCNeedSwitchStep = FALSE;

  /*
   * Although XMP profile defines the default step size, we
   * still access reg 0x2B to get current step size in case
   * XMP profile data is incorrect. Another consideration is
   * we might program PMIC multiple times due to SAGV
   */
  Offset = PMIC_REG_2B;
  Reg2B.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);

  if (!IsPmicSupport10MVStep) {
    //Double-check step size in case memory spd data is incorrect
    if ((Reg2B.Bits.SWAStepSize != PmicStepSize5mV) ||
        (Reg2B.Bits.SWBStepSize != PmicStepSize5mV) ||
        (Reg2B.Bits.SWCStepSize != PmicStepSize5mV)) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PMIC Step Size is incorrect. Switch Step Size back to 5MV. Old Reg2B 0x%x\n", Reg2B.Data);
      Reg2B.Bits.SWAStepSize = PmicStepSize5mV;
      Reg2B.Bits.SWBStepSize = PmicStepSize5mV;
      Reg2B.Bits.SWCStepSize = PmicStepSize5mV;
      MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg2B.Data, &Status);
    }

    SWAStepSize = 5;
    SWBStepSize = 5;
    SWCStepSize = 5;
    if (Vdd != VDD_INVALID) {
      Vdd = RANGE(Vdd, PMIC_SWA_SWB_LOWEST_VOLTAGE, PMIC_SWA_SWB_5MV_HIGHEST_VOLTAGE);
    }
    if (Vddq != VDD_INVALID) {
      Vddq = RANGE(Vddq, PMIC_SWA_SWB_LOWEST_VOLTAGE, PMIC_SWA_SWB_5MV_HIGHEST_VOLTAGE);
    }
    if (Vpp != VDD_INVALID) {
      Vpp = RANGE(Vpp, PMIC_SWC_LOWEST_VOLTAGE, PMIC_SWC_5MV_HIGHEST_VOLTAGE);
    }
  } else {
    /*
     * PMIC new spec has no detailed info about step size
     * switching.
     * To keep better compatibility and stability for memory modules,
     * 1) We use 5mv firstly. When 5mv step size can't satisfy
     * the voltage tuning, switch to 10mv.
     * 2) Assume step size is only a flag. Real voltage change happens
     * only when voltage reg is configured, such like 0x21 and so on.
     */
    SWAStepSize = (Reg2B.Bits.SWAStepSize == PmicStepSize5mV) ? 5 : 10;
    SWBStepSize = (Reg2B.Bits.SWBStepSize == PmicStepSize5mV) ? 5 : 10;
    SWCStepSize = (Reg2B.Bits.SWCStepSize == PmicStepSize5mV) ? 5 : 10;
    if (Vdd != VDD_INVALID) {
      Vdd = RANGE(Vdd, PMIC_SWA_SWB_LOWEST_VOLTAGE, PMIC_SWA_SWB_10MV_HIGHEST_VOLTAGE);
      if ((Reg2B.Bits.SWAStepSize == PmicStepSize5mV) && (Vdd > PMIC_SWA_SWB_5MV_HIGHEST_VOLTAGE)) {
        SWAStepSize = 10;
        // Just Switch from 5MV to 10MV. We never switch back to 5MV except at reset
        SWANeedSwitchStep = TRUE;
        Reg2B.Bits.SWAStepSize = PmicStepSize10mV;
      }
    }
    if (Vddq != VDD_INVALID) {
      Vddq = RANGE(Vddq, PMIC_SWA_SWB_LOWEST_VOLTAGE, PMIC_SWA_SWB_10MV_HIGHEST_VOLTAGE);
      if ((Reg2B.Bits.SWBStepSize == PmicStepSize5mV) && (Vddq > PMIC_SWA_SWB_5MV_HIGHEST_VOLTAGE)) {
        SWBStepSize = 10;
        // Just Switch from 5MV to 10MV. We never switch back to 5MV except at reset
        SWBNeedSwitchStep = TRUE;
        Reg2B.Bits.SWBStepSize = PmicStepSize10mV;
      }
    }
    if (Vpp != VDD_INVALID) {
      Vpp = RANGE(Vpp, PMIC_SWC_LOWEST_VOLTAGE, PMIC_SWC_10MV_HIGHEST_VOLTAGE);
      if ((Reg2B.Bits.SWCStepSize == PmicStepSize5mV) && (Vpp > PMIC_SWC_5MV_HIGHEST_VOLTAGE)) {
        SWCStepSize = 10;
        // Just Switch from 5MV to 10MV. We never switch back to 5MV except at reset
        SWCNeedSwitchStep = TRUE;
        Reg2B.Bits.SWCStepSize = PmicStepSize10mV;
      }
    }

    if (SWANeedSwitchStep || SWBNeedSwitchStep || SWCNeedSwitchStep) {
      Offset = PMIC_REG_2B;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Switch Step Size to 10MV. Reg2B 0x%x\n", Reg2B.Data);
      MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg2B.Data, &Status);
      Reg2B.Data = 0;
      Reg2B.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Reg2B readback value 0x%02x\n", Reg2B.Data);
    }
  }

  NewVoltage = Vdd;
  if (NewVoltage != VDD_INVALID) {
    Offset = PMIC_REG_21;
    Reg21.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
    if (Status != EFI_SUCCESS) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Failed to read PMIC reg 0x%02X!\n", gErrString, Offset);
      return;
    }
    if (!SWANeedSwitchStep) {
      Current = Reg21.Bits.swa_voltage_setting;
    } else {
      Current = 0;
    }
    Data = (NewVoltage - PMIC_SWA_SWB_START_VOLTAGE) / SWAStepSize;
    if (SWANeedSwitchStep || (Current != Data)) {
      Reg21.Bits.swa_voltage_setting = (UINT8) Data;
      Value = MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg21.Data, &Status);
      Delta = ABS((int)Data - (int)Current);
      MrcWait (MrcData, Delta * SWAStepSize * MRC_TIMER_1US);
      Value = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        "*** %s update to %u mV, Pmic reg 0x%02x: new value 0x%02x, readback value 0x%02x\n",
        "VDD",
        NewVoltage,
        Offset,
        Reg21.Data,
        Value);
      if (Value != Reg21.Data) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Update failed, Pmic doesn't accept the value!\n");
      }
    }
  }

  NewVoltage = Vddq;
  if (NewVoltage != VDD_INVALID) {
    Data = (NewVoltage - PMIC_SWA_SWB_START_VOLTAGE) / SWBStepSize;
    Offset = PMIC_REG_25;
    Reg25.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
    if (Status != EFI_SUCCESS) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Failed to read PMIC reg 0x%02X!\n", gErrString, Offset);
      return;
    }
    if (!SWBNeedSwitchStep) {
      Current = Reg25.Bits.swb_voltage_setting;
    } else {
      Current = 0;
    }
    if (SWBNeedSwitchStep || (Current != Data)) {
      Reg25.Bits.swb_voltage_setting = (UINT8) Data;
      Value = MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg25.Data, &Status);
      Delta = ABS((int)Data - (int)Current);
      MrcWait (MrcData, Delta * SWBStepSize * MRC_TIMER_1US);
      Value = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        "*** %s update to %u mV, Pmic reg 0x%02x: new value 0x%02x, readback value 0x%02x\n",
        "VDDQ",
        NewVoltage,
        Offset,
        Reg25.Data,
        Value);
      if (Value != Reg25.Data) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Update failed, Pmic doesn't accept the value!\n");
      }
    }
  }

  NewVoltage = Vpp;
  if (NewVoltage != VDD_INVALID) {
    Data = (NewVoltage - PMIC_SWC_START_VOLTAGE) / SWCStepSize;
    Offset = PMIC_REG_27;
    Reg27.Data = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
    if (Status != EFI_SUCCESS) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Failed to read PMIC reg 0x%02X!\n", gErrString, Offset);
      return;
    }
    if (!SWCNeedSwitchStep) {
      Current = Reg27.Bits.swc_voltage_setting;
    } else {
      Current = 0;
    }
    if (SWCNeedSwitchStep || (Current != Data)) {
      Reg27.Bits.swc_voltage_setting = (UINT8) Data;
      Value = MrcCall->MrcSmbusWrite8 (PmicAddress | (Offset << 8), Reg27.Data, &Status);
      Delta = ABS((int)Data - (int)Current);
      MrcWait (MrcData, Delta * SWCStepSize * MRC_TIMER_1US);
      Value = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        "*** %s update to %u mV, Pmic reg 0x%02x: new value 0x%02x, readback value 0x%02x\n",
        "VPP",
        NewVoltage,
        Offset,
        Reg27.Data,
        Value);
      if (Value != Reg27.Data) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Update failed, Pmic doesn't accept the value!\n");
      }
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Dump Pmic Reg Value:\n");
  for (Loop = 0; Loop < ARRAY_COUNT(PmicDumpRegs); Loop++) {
    Offset = PmicDumpRegs[Loop];
    Value = MrcCall->MrcSmbusRead8 (PmicAddress | (Offset << 8), &Status);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t0x%02x: 0x%02x\n", Offset, Value);
  }
}

