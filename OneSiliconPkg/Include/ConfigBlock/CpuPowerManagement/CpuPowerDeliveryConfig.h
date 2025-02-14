/** @file
  CPU Power Delivery Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _CPU_POWER_DELIVERY_CONFIG_H_
#define _CPU_POWER_DELIVERY_CONFIG_H_

#include <CpuPowerMgmt.h>

extern EFI_GUID gCpuPowerDeliveryConfigGuid;

#pragma pack (push,1)

///
/// Defines the maximum number of custom ConfigTdp entries supported.
/// @warning: Changing this define would cause DWORD alignment issues in policy structures.
///
#define MAX_CUSTOM_CTDP_ENTRIES 3

#define CPU_POWER_DELIVERY_CONFIG_REVISION 1
/**
  CPU Power Delivery Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32 PowerLimit2                    : 1;      ///< Enable or Disable short duration Power Limit (PL2). 0: Disable; <b>1: Enable</b>
  UINT32 PowerLimit3DutyCycle           : 8;      ///< Package PL3 Duty Cycle. Specifies the PL3 duty cycle percentage, Range 0-100. <b>Default: 0</b>.
  UINT32 PowerLimit3Lock                : 1;      ///< Package PL3 MSR 615h lock; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 PowerLimit4Lock                : 1;      ///< Package PL4 MSR 601h lock; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 TccOffsetLock                  : 1;      ///< Tcc Offset Lock for Runtime Average Temperature Limit (RATL) to lock temperature target MSR 1A2h; 0: Disabled; <b>1: Enabled (Lock)</b>.
  UINT32 ApplyConfigTdp                 : 1;      ///< Switch TDP applied setting based on non-cTDP or TDP; 0: non-cTDP; <b>1: cTDP</b>.
  /**
  Dual Tau Base Performance Boost support. This will improve platform performance for desktop sku.
  When enabled, it exposes 2 MMIO PL1 and MMIO Tau registers, and also provides the default value.
  @note: This is only applicable for Desktop 35W/65W/125W sku.
  @note: When this is enabled, it will override the settings in policys PowerLimit1, PowerLimit1Time, PowerLimit2Power.
  <b>0: Disable</b>; 1: Enable
  **/
  UINT32 DualTauBoost                   : 1;
  /**
  ResponseMode - PL3 CONTROL MSR 0x615 BIT 16, it indicates how quickly Pcode reduces power consumption to a low (safe) level; <b>0: Gradual Power Reduction</b>; 1: Aggressive Power Reduction.
  **/
  UINT32 ResponseMode                   : 1;
  UINT32 PlatformAtxTelemetryUnit       : 1;      ///< PMC_SUS_SPARE_GCR_0 bit 0, set by BIOS to indicate the telemetry type for Psys support
  UINT32 RsvdBits                       : 16;     ///< Reserved for future use.
  /**
  Package Long duration turbo mode power limit (PL1).
  Default is the TDP power limit of processor. Units are 125 milliwatt.
  **/
  UINT16 PowerLimit1;
  /**
  Package Short duration turbo mode power limit (PL2). Allows for short excursions above TDP power limit.
  Default = 1.25 * TDP Power Limit. Units are 125 milliwatt.
  **/
  UINT16 PowerLimit2Power;
  /**
  Package PL3 power limit. PL3 is the CPU Peak Power Occurences Limit.
  <b>Default: 0</b>. Range 0-65535. Units are 125 milliwatt.
  **/
  UINT16 PowerLimit3;
  /**
  Package PL4 power limit. PL4 is a Preemptive CPU Package Peak Power Limit, it will never be exceeded.
  Power is premptively lowered before limit is reached. <b>Default: 0</b>. Range 0-65535.
  Units are 125 milliwatt.
  **/
  UINT16 PowerLimit4;
  /**
  Package Long duration turbo mode power limit (PL1) time window in seconds.
  Used in calculating the average power over time.
  Mobile: <b> 28s</b>
  Desktop: <b> 8s</b>
  Range: 0 - 128s
  **/
  UINT32 PowerLimit1Time;
  UINT32 PowerLimit3Time;                         ///< Package PL3 time window. Range from 3ms to 64ms.
  UINT32 PsysPowerLimit1       : 1;               ///< MSR 0x65C[15]: PL1 Enable activates the PL1 value to limit average platform power
  UINT32 PsysPowerLimit1Time   : 8;               ///< MSR 0x65C[23:17]: PL1 timewindow in seconds.
  UINT32 PsysPowerLimit2       : 1;               ///< MSR 0x65C[47]: PL2 Enable activates the PL2 value to limit average platform power
  UINT32 ConfigTdpLock         : 1;               ///< Lock the ConfigTdp mode settings from runtime changes; <b>0: Disable</b>; 1: Enable.
  UINT32 ConfigTdpBios         : 1;               ///< Configure whether to load Configurable TDP SSDT; <b>0: Disable</b>; 1: Enable.
  UINT32 DisableVrThermalAlert : 1;               ///< Enable or Disable VR Thermal Alert; <b>0: Disable</b>; 1: Enable.
  UINT32 ThermalMonitor        : 1;               ///< Enable or Disable Thermal Monitor; 0: Disable; <b>1: Enable</b>.
  UINT32 ConfigTdpLevel        : 8;               ///< Configuration for boot TDP selection; <b>0: TDP Nominal</b>; 1: TDP Level1; 2: TDP Level2.
  UINT32 AcDcPowerState        : 1;               ///< AC/DC Power state
  UINT32 RsvdBits1             : 9;               ///< Reserved for future use.
  UINT16 PsysPowerLimit1Power;                    ///< MSR 0x65C[14:0]: Platform PL1 power. Units are 125 milliwatt.
  UINT16 PsysPowerLimit2Power;                    ///< MSR 0x65C[46:32]]: Platform PL2 power. Units are 125 milliwatt.

  UINT32 CustomPowerLimit1Time      :  8;         ///< Short term Power Limit time window value for custom cTDP level.
  UINT32 CustomTurboActivationRatio :  8;         ///< Turbo Activation Ratio for custom cTDP level.
  UINT32 RsvdBits2                  : 16;         ///< Bits reserved for DWORD alignment.
  UINT16 CustomPowerLimit1;                       ///< Short term Power Limit value for custom cTDP level. Units are 125 milliwatt.
  UINT16 CustomPowerLimit2;                       ///< Long term Power Limit value for custom cTDP level. Units are 125 milliwatt.

  /**
  Package Power Limit 4 Boost configuration.
  <b>Default: 0</b>. Units are 125 milliwatt. Valid Range 0 to 63000 in step size of 125/1000 Watt. The value 0 means disable.
  **/
  UINT16 PowerLimit4Boost;

  /**
  TCC Activation Offset. Offset from factory set TCC activation temperature at which the Thermal Control Circuit must be activated.
  TCC will be activated at (TCC Activation Temperature - TCC Activation Offset), in degrees Celcius.
  <b>Default: 0</b>, causing TCC to activate at TCC Activation temperature.
  @note The policy is recommended for validation purpose only.
  **/
  UINT8  TccActivationOffset;
  UINT8  RsvdBytes128[1];
  UINT16 IsysCurrentLimitL1;                      ///< This field indicated the current limitiation of L1. Units of measurements are 1/8 A. <b>Default: 0x0</b>. Range 0-0x7FFF.
  UINT8  IsysCurrentLimitL1Enable;                ///< This bits enables disables ISYS_CURRENT_LIMIT_L1 algorithm.<b>Default: 0</b>.
  UINT8  IsysCurrentL1Tau;                        ///< Specifies the time window used to calculate average current for ISYS_L1. <b>Default: 0x0</b>. Range 0-255.
  UINT16 IsysCurrentLimitL2;                      ///< This field indicated the current limitiation of L2. Units of measurements are 1/8 A. <b>Default: 0x0</b>. Range 0-0x7FFF.
  UINT8  IsysCurrentLimitL2Enable;                ///< This bits enables disables ISYS_CURRENT_LIMIT_L2 algorithm.<b>Default: 0</b>.
  UINT8  RsvdBytes134[1];
  UINT16 VsysMax;                                 ///< Specifies Vsys Max defined in 1/1000 increments.<b>Default: 0x0 </b>. Range 0-65535.
  /**
  ThETA is name given to this feature which stands for Thermally Equal Turbo-boost Algorithm. Ibatt is Ibatt battery current.
  Enable or Disable ThETA Ibatt Feature. <b>0: Disable</b>; 1: Enable.
  **/
  UINT8  ThETAIbattEnable;
  /**
  SoC firmware based Skin Thermal Control feature programmed via MMIO register.
    - Configures these MMIO option to set 3 sensors: thresholds, gain, min perf
    - Controls setting enable/disable sensor temperature and ranges
      Enable or Disable STC (Skin Thermal Control) Feature. <b>0: Disable</b>; 1: Enable.
  **/
  UINT8  SkinTempControl;
  UINT8  SkinTargetTemp[SKIN_TEMP_CONTROL_SENSOR];           ///< Target temperature is limit to which the control mechanism is regulating.It is defined in 1/2 C increments.Range is 0-122.5 C. For a 0.5 C temperature, enter 1. 0 = AUTO.
  UINT8  SkinTempControlEnable[SKIN_TEMP_CONTROL_SENSOR];    ///< Enables the skin temperature control for MMIO register.<b>0: Disable</b>; 1: Enable.
  UINT8  SkinControlLoopGain[SKIN_TEMP_CONTROL_SENSOR];      ///< Sets the aggressiveness of control loop where 0 is graceful, favors performance on expense of temperature overshoots and 7 is aggressive, favors tight regulation over performance. Range is 0 to 7.
  UINT8  SkinTempOverrideEnable[SKIN_TEMP_CONTROL_SENSOR];   ///< When set, Pcode will use TEMPERATURE_OVERRIDE values instead of reading from corresponding sensor.<b>0: Disable</b>; 1: Enable.
  UINT8  SkinMinPerformanceLevel[SKIN_TEMP_CONTROL_SENSOR];  ///< Minimum Performance level below which the STC limit will not throttle. 0 - all levels of throttling allowed incl. survivability actions. 256 - no throttling allowed.
  UINT8  SkinTempOverride[SKIN_TEMP_CONTROL_SENSOR];         ///< Allows SW to override the input temperature.Pcode will use this value instead of the sensor temperature.EC control is not impacted.Units: 0.5C.Values are 0 to 255 which represents 0C - 122.5C range.
  UINT8  RsvdBytes154[6];
} CPU_POWER_DELIVERY_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_DELIVERY_CONFIG_H_
