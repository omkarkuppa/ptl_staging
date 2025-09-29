/** @file
  This file contains DDR5 PMIC programming routines

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

@par Specification Reference: PMIC5100 (JEDEC JESD301-2) and PMIC5200 (JEDEC JESD301-2) Specifications
**/
#include "MrcCommon.h"
#include "MrcPmic.h"
#include "MrcPmicPrivate.h"

#define MAX_PWR_GOOD_OUT_TIME (4 * 14) // 4 (SWA-SWD) * SoftStartTime (14 ms) = 56 ms, Config0-2_Delay assumed as 0.

typedef struct PMIC_RAIL_CONFIG_DATA {
  MRC_RANGE Range;
  UINT16 StepSize; // 100 uV units
} PMIC_RAIL_CONFIG_DATA;

static const PMIC_RAIL_CONFIG_DATA PmicVoltageRange[MaxPmicRails][MaxPmicStepSize] = {
  [PmicRailSwa] = {
    [PmicStepSize5mV]   = {.Range = {800, 1435}, .StepSize = 50},
    [PmicStepSize10mV]  = {.Range = {800, 2070}, .StepSize = 100},
  },
  [PmicRailSwb] = {
    [PmicStepSize5mV]   = {.Range = {200, 835}, .StepSize = 50},
    [PmicStepSize2p5mV] = {.Range = {200, 515}, .StepSize = 25},
  },
  [PmicRailSwc] = {
    [PmicStepSize5mV]   = {.Range = {1500, 2135}, .StepSize = 50},
    [PmicStepSize10mV]  = {.Range = {1500, 2770}, .StepSize = 100},
  },
  [PmicRailSwd] = {
    [PmicStepSize5mV]   = {.Range = {800, 1435}, .StepSize = 50},
    [PmicStepSize10mV]  = {.Range = {800, 2070}, .StepSize = 100},
  },
};

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
  )
{
  UINT32 Result;

  Result =  RANGE (Voltage,
                   PmicVoltageRange[Rail][StepSize].Range.Start,
                   PmicVoltageRange[Rail][StepSize].Range.End);
  Result = (Result - PmicVoltageRange[Rail][StepSize].Range.Start);

  Result *= 10; // Convert mV to 100 uV units
  Result /= PmicVoltageRange[Rail][StepSize].StepSize;

  return (UINT8) Result;
}

/**
  This function configures PMIC voltages.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcPmicVoltageConfiguration (
  IN OUT MrcParameters      *MrcData
  )
{
  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      PmicVoltageConfigurationLpddr5 (MrcData);
      break;

    default:
      break;
  }
}

/**
  This function configures PMIC voltages for LPDDR5.

  @param[in, out] MrcData - MRC global data.
**/
VOID
PmicVoltageConfigurationLpddr5 (
  IN OUT MrcParameters *MrcData
  )
{
  MRC_EXT_INPUTS_TYPE *ExtInputs  = MrcData->Inputs.ExtInputs.Ptr;
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcDebug *Debug;

  UINT16 PmicVoltageConfig[MaxPmicRails] = {0};
  MrcStatus Status;
  SPD_PMIC_DEVICE_TYPE PmicType;
  Debug = &Outputs->Debug;

  PmicType = GetPmicType (MrcData);
  if (PmicType != Pmic5200 && PmicType != Pmic5100) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PMIC voltage configuration failed beacuse PMIC type is not supported.\n");
    return;
  }

  Status = EnableProgrammableMode (MrcData);
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Failed to enable PMIC programmable mode.\n");
    return;
  }

  MrcEnablePmic (MrcData);

  PmicVoltageConfig[PmicRailSwa] = ExtInputs->Vdd2HVoltage;
  PmicVoltageConfig[PmicRailSwb] = ExtInputs->VddqVoltage;
  PmicVoltageConfig[PmicRailSwc] = ExtInputs->Vdd1Voltage;
  PmicVoltageConfig[PmicRailSwd] = ExtInputs->Vdd2LVoltage;
  ProgramVoltages (MrcData, PmicVoltageConfig);
}

/**
  Get PMIC type from MRC global data structure.

  @param[in] MrcData - MRC global data.

  @return PMIC type.
**/
SPD_PMIC_DEVICE_TYPE
GetPmicType (
  IN OUT MrcParameters *MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;

  UINT8 FirstController = Outputs->FirstPopController;
  UINT8 FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  UINT8 FirstDimm = MrcGetFirstPopulatedDimm (MrcData, FirstController, FirstChannel);

  if (FirstDimm >= MAX_DIMMS_IN_CHANNEL) {
    MRC_DEBUG_ASSERT (FALSE, &Outputs->Debug, "No populated DIMM found.");
    return PmicTypeUnknown;
  }

  return Outputs->Controller[FirstController].Channel[FirstChannel].Dimm[FirstDimm].PmicType;
}

/**
  Enable PMIC programmable mode.

  @param[in] MrcData - MRC global data.

  @returns mrcSuccess if at least one PMIC is successfully configured, otherwise mrcFail.
**/
MrcStatus
EnableProgrammableMode (
  IN OUT MrcParameters *MrcData
  )
{
  MrcInput *Inputs = &MrcData->Inputs;
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcDebug *Debug;
  MrcStatus Status = mrcFail;

  const MrcDimmIn *DimmIn;
  MrcDimmOut *DimmOut;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Dimm;

  UINT8  SpdAddressList[MAX_CONTROLLER * MAX_CHANNEL * MAX_DIMMS_IN_CHANNEL];
  UINT32 SpdAddressCount = 0;
  UINT32 SpdAddressIndex;
  BOOLEAN IsAlreadyConfigured;

  UINT32 SpdAddress;
  UINT32 PmicAddress;
  PMIC_REG_2F_STRUCT Reg2F;
  UINT32 SmbusCommand;
  RETURN_STATUS SmbusStatus;

  Debug = &Outputs->Debug;


  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {

        DimmIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm];
        DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];

        IsAlreadyConfigured = FALSE;

        SpdAddress = DimmIn->SpdAddress;

        for (SpdAddressIndex = 0; SpdAddressIndex < SpdAddressCount; SpdAddressIndex++) {
          if (SpdAddressList[SpdAddressIndex] == SpdAddress) {
            IsAlreadyConfigured = TRUE;
            break;
          }
        }

        if (DimmOut->Status != DIMM_PRESENT ||
            SpdAddress == 0                 ||
            IsAlreadyConfigured) {
          continue;
        }

        DimmOut->PmicProgrammable = TRUE;

        PmicAddress = (PMIC0_LID_CODE << 4) | (SpdAddress & 0xE);

        SmbusCommand = (PMIC_REG_2F << 8) | PmicAddress;
        Reg2F.Data = MrcSmbusRead (MrcData,  SmbusCommand, &SmbusStatus);

        if (Reg2F.Bits.secure_mode == 0) {
          // Enable Programmable Mode
          Reg2F.Bits.secure_mode = 1;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg2F.Data, &SmbusStatus);

          Reg2F.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);
          if (Reg2F.Bits.secure_mode == 1) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Programmable Mode enabled successfully.\n", SpdAddress);
            Status = mrcSuccess;
          } else {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "[SPD 0x%02X] PMIC Programmable Mode enabling failed.\n", SpdAddress);
            MrcPmicPrintRegisters (MrcData, (UINT8) SpdAddress);
            DimmOut->PmicProgrammable = FALSE;

            return mrcFail;
          }
        } else {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Programmable Mode is already enabled.\n", SpdAddress);
          Status = mrcSuccess;
        }

        SpdAddressList[SpdAddressCount++] = (UINT8) SpdAddress;
      }
    }
  }

  return Status;
}

/**
  Send VR Enable command to PMIC.

  @param[in] MrcData - MRC global data.
**/
VOID
MrcEnablePmic (
  IN OUT MrcParameters *MrcData
  )
{
  MrcInput *Inputs = &MrcData->Inputs;
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcDebug *Debug;

  UINT32 Controller;
  UINT32 Channel;
  UINT32 Dimm;

  UINT8  SpdAddressList[MAX_CONTROLLER * MAX_CHANNEL * MAX_DIMMS_IN_CHANNEL];
  UINT32 SpdAddressCount = 0;
  UINT32 SpdAddressIndex;
  BOOLEAN IsAlreadyConfigured;

  UINT32 SpdAddress;
  UINT32 PmicAddress;
  UINT32 SmbusCommand;
  PMIC_REG_32_STRUCT Reg32;
  RETURN_STATUS SmbusStatus;

  BOOLEAN IsVrEnableCmdSent = FALSE;

  Debug = &Outputs->Debug;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        IsAlreadyConfigured = FALSE;

        SpdAddress = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].SpdAddress;

        for (SpdAddressIndex = 0; SpdAddressIndex < SpdAddressCount; SpdAddressIndex++) {
          if (SpdAddressList[SpdAddressIndex] == SpdAddress) {
            IsAlreadyConfigured = TRUE;
            break;
          }
        }

        if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status != DIMM_PRESENT ||
            SpdAddress == 0                                                                    ||
            IsAlreadyConfigured) {
          continue;
        }

        PmicAddress = (PMIC0_LID_CODE << 4) | (SpdAddress & 0xE);

        SmbusCommand = (PMIC_REG_32 << 8) | PmicAddress;
        Reg32.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);

        if (Reg32.Bits.vr_enable == 1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Voltage Regulator (VR) is already enabled.\n", SpdAddress);
        } else {
          IsVrEnableCmdSent = TRUE;

          Reg32.Bits.vr_enable = 1;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg32.Data, &SmbusStatus);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Voltage Regulator (VR) Enable command sent.\n", SpdAddress);
        }

        SpdAddressList[SpdAddressCount++] = (UINT8) SpdAddress;
      }
    }
  }

  if (IsVrEnableCmdSent) {
    MrcWait (MrcData, MAX_PWR_GOOD_OUT_TIME * MRC_TIMER_1MS);
  }
}

/**
  Program PMIC voltages.

  @param[in] MrcData - MRC global data.
  @param[in] PmicVoltages - PMIC voltages to be programmed.
**/
VOID
ProgramVoltages (
  IN OUT MrcParameters *MrcData,
  const UINT16* const PmicVoltages
  )
{
  MrcInput *Inputs = &MrcData->Inputs;
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcDebug *Debug;

  UINT8  SpdAddressList[MAX_CONTROLLER * MAX_CHANNEL * MAX_DIMMS_IN_CHANNEL];
  UINT32 SpdAddressCount = 0;
  UINT32 SpdAddressIndex;
  BOOLEAN IsAlreadyConfigured;

  UINT32 Controller;
  UINT32 Channel;
  UINT32 Dimm;

  UINT32 SpdAddress;
  SPD_PMIC_DEVICE_TYPE PmicType;
  UINT32 PmicAddress;

  PMIC_REG_2B_STRUCT Reg2B;
  PMIC_REG_21_STRUCT Reg21;
  PMIC_REG_23_STRUCT Reg23;
  PMIC_REG_25_STRUCT Reg25;
  PMIC_REG_27_STRUCT Reg27;
  UINT8 VoltageEncoding;

  RETURN_STATUS SmbusStatus;
  UINT32 SmbusCommand;

  Debug = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Configuring PMIC voltages:\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tSWA: %d mV\n"
                                        "\tSWB: %d mV\n"
                                        "\tSWC: %d mV\n"
                                        "\tSWD: %d mV\n",
                                          PmicVoltages[PmicRailSwa],
                                          PmicVoltages[PmicRailSwb],
                                          PmicVoltages[PmicRailSwc],
                                          PmicVoltages[PmicRailSwd]);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        IsAlreadyConfigured = FALSE;

        SpdAddress = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].SpdAddress;

        for (SpdAddressIndex = 0; SpdAddressIndex < SpdAddressCount; SpdAddressIndex++) {
          if (SpdAddressList[SpdAddressIndex] == SpdAddress) {
            IsAlreadyConfigured = TRUE;
            break;
          }
        }

        if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status != DIMM_PRESENT ||
            SpdAddress == 0                                                                    ||
            IsAlreadyConfigured) {
          continue;
        }

        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Voltages before configuration:\n", SpdAddress);
        MrcPmicPrintRegisters (MrcData, (UINT8) SpdAddress);

        PmicType = Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].PmicType;

        PmicAddress = (PMIC0_LID_CODE << 4) | (SpdAddress & 0xE);
        SmbusCommand = (PMIC_REG_2B << 8) | PmicAddress;
        Reg2B.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);

        // SWA
        if (PmicVoltages[PmicRailSwa] != VDD_INVALID) {
          SmbusCommand = (PMIC_REG_21 << 8) | PmicAddress;

          VoltageEncoding =  GetPmicVoltageRegValue (PmicVoltages[PmicRailSwa], PmicRailSwa, Reg2B.Bits.SWAStepSize);

          Reg21.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);
          Reg21.Bits.swa_voltage_setting = VoltageEncoding;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg21.Data, &SmbusStatus);
        }

        // SWB
        if (PmicVoltages[PmicRailSwb] != VDD_INVALID) {
          SmbusCommand = (PMIC_REG_25 << 8) | PmicAddress;

          VoltageEncoding =  GetPmicVoltageRegValue (PmicVoltages[PmicRailSwb], PmicRailSwb, Reg2B.Bits.SWBStepSize);

          Reg25.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);
          Reg25.Bits.swb_voltage_setting = VoltageEncoding;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg25.Data, &SmbusStatus);
        }

        // SWC
        if (PmicVoltages[PmicRailSwc] != VDD_INVALID) {
          SmbusCommand = (PMIC_REG_27 << 8) | PmicAddress;

          VoltageEncoding =  GetPmicVoltageRegValue (PmicVoltages[PmicRailSwc], PmicRailSwc, Reg2B.Bits.SWCStepSize);

          Reg27.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);

          Reg27.Bits.swc_voltage_setting = VoltageEncoding;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg27.Data, &SmbusStatus);
        }

        // SWD
        if (PmicVoltages[PmicRailSwd] != VDD_INVALID && PmicType == Pmic5200) {
          SmbusCommand = (PMIC_REG_23 << 8) | PmicAddress;

          VoltageEncoding =  GetPmicVoltageRegValue (PmicVoltages[PmicRailSwd], PmicRailSwd, Reg2B.Bits.SWDStepSize);

          Reg23.Data = MrcSmbusRead (MrcData, SmbusCommand, &SmbusStatus);
          Reg23.Bits.swd_voltage_setting = VoltageEncoding;
          MrcSmbusWrite (MrcData, SmbusCommand, Reg23.Data, &SmbusStatus);
        }

        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[SPD 0x%02X] PMIC Voltages are configured.\nReading regs after configuration:\n", SpdAddress);
        MrcPmicPrintRegisters (MrcData, (UINT8) SpdAddress);

        SpdAddressList[SpdAddressCount++] = (UINT8) SpdAddress;
      }
    }
  }
}

/**
 * Print PMIC registers.
 *
 * @param[in] MrcData - MRC global data.
 * @param[in] SpdAddress - SPD address.
 */
VOID
MrcPmicPrintRegisters (
  IN OUT MrcParameters *MrcData,
  UINT8 SpdAddress
  )
{
  MrcDebug *Debug;
  UINT32 Rail;

  UINT32 PmicAddress;
  UINT32 PmicRegOffset;
  UINT32 Value;

  UINT32 SmbusCommand;
  RETURN_STATUS SmbusStatus;

  Debug = &MrcData->Outputs.Debug;

  for (Rail = 0; Rail < MaxPmicRails; Rail++) {
    PmicRegOffset = PmicRailToVoltageRegMap[Rail];
    PmicAddress = (PMIC0_LID_CODE << 4) | (SpdAddress & 0xE);
    SmbusCommand = (PmicRegOffset << 8) | PmicAddress;
    Value = MrcSmbusRead (MrcData,  SmbusCommand, &SmbusStatus);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t[PMIC Reg Read] SPD: 0x%02x, Reg:0x%02X, Voltage Setting: 0x%02x, PWR GOOD THS: %u\n",
                   SpdAddress,
                   PmicRegOffset,
                   (Value & 0b11111110) >> 1,
                   Value & 1);
  }
}
