/** @file
  Overclocking Config Block.

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
#ifndef _OVERCLOCKING_PREMEM_CONFIG_H_
#define _OVERCLOCKING_PREMEM_CONFIG_H_

#include <OcLimits.h>

#define OVERCLOCKING_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gOverclockingPreMemConfigGuid;

#pragma pack (push,1)

/**
  Overclocking Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;     ///< Config Block Header
  /**
  Overclocking support. This controls whether OC mailbox transactions are sent.
  If disabled, all policies in this config block besides OcSupport and OcLock will be ignored.
  <b>0: Disable</b>;
  1: Enable.
  @note If PcdOverclockEnable is disabled, this should also be disabled.
  **/
  UINT8  OcSupport;
  UINT8  PerCoreRatioOverride;      ///< Enable or disable Per Core PState OC supported by writing OCMB 0x1D to program new favored core ratio to each Core. <b>0: Disable</b>, 1: enable
  /**
  Ring Downbin enable/disable.
  When enabled, the CPU will force the ring ratio to be lower than the core ratio.
  Disabling will allow the ring and core ratios to run at the same frequency.
  Uses OC Mailbox command 0x19.
  0: Disables Ring Downbin feature. <b>1: Enables Ring downbin feature.</b>
  **/
  UINT8  RingDownBin;
  /**
  This service controls the Intel Thermal Velocity Boost (TVB) feature enabling. If enabled, High temperatures
  may result in lower ratios than P0. It is required to be disabled for supporting overclocking at frequencies
  higher than the default max turbo frequency.
  0: Disables Thermal Turbo. <b>1: Enables Thermal Turbo</b>.
  **/
  UINT8  ThermalTurbo;
  /**
  This service controls voltage reduction based True Temperature Dependency for processors that implement
  the IntelThermal Velocity Boost (TVB) feature. Core voltages will be lowered based on Core temperature
  0: Disables Negative ITD. <b>1: Enables Negative ITD.</b>
  **/
  UINT8  NegativeItd;
  UINT8  IaIccUnlimitedMode;             ///< Support IA Unlimited ICCMAX up to maximum value 512A., <b>0: Disable</b>. 1: Enable.
  UINT8  GtIccUnlimitedMode;             ///< Support GT Unlimited ICCMAX up to maximum value 512A., <b>0: Disable</b>. 1: Enable.
  /**
  This service controls VCCSA Boot Voltage by programming the EPOC2 bits (strap).
  <b> Default: 0: Nominal </b> 1: High Voltage(up to 1.2/1.3V)
  **/
  UINT8  VccsaBootVoltageSel;
  /**
  This service controls CPU Bandgap bypass or not by programming the CPU EPOC2 bits (strap).
  <b> Default: 0: Normal </b> 1: Bandgap Bypassed
  **/
  UINT8  CpuBandgapRefMode;
  /**
  This service controls VCCIA boot voltage by programming SOC-North soft straps.
  <b> Default: 0: Nominal </b> 1: High Voltage (support VCCIA boot voltage higher than 1.65v (max 2.01v))
  **/
  UINT8  VcciaBootVoltageSel;
  UINT8  BclkAdaptiveVoltage;             ///< Bclk Adaptive Voltage enable/disable. <b>0: Disabled</b>, 1: Enabled. When enabled, the CPU V/F curves are aware of BCLK frequency when calculated.
  UINT8  RealtimeMemoryTiming;            ///< Enable/Disable the message sent to the CPU to allow realtime memory timing changes after MRC_DONE. <b>0=Disable</b>, 1=Enable
  UINT8  Avx2RatioOffset;                 ///< AVX2 Ratio Offset. <b>0: No offset</b>. Range is 0-31. Used to lower the AVX ratio to maximize possible ratio for SSE workload.
  UINT8  GranularRatioOverride;           ///< Enable or disable OC Granular Ratio Override. <b>0: Disable</b>, 1: enable

  /**
  Avx2 Voltage Guardband Scale Factor
  This controls the AVX2 Voltage Guardband Scale factor applied to AVX2 workloads.
  Valid range is 0-200 in 1/100 units, where a value of 125 would apply a 1.25 scale factor.
  A value of 0 means no scale factor applied (no change to voltage on AVX commands)
  A value of 100 applies the default voltage guardband values (1.0 factor).
  A value > 100 will increase the voltage guardband on AVX2 workloads.
  A value < 100 will decrease the voltage guardband on AVX2 workloads.
  <b>0. No scale factor applied</b>
  **/
  UINT8  Avx2VoltageScaleFactor;
  /**
  TjMax Offset. Specified value here is clipped by pCode (125 - TjMax Offset) to support TjMax in the range of 62 to 115 deg Celsius.
  <b> Default: 0 Hardware Defaults </b> Range 10 to 63. 0 = No offset / Keep HW default.
  **/
  UINT8  TjMaxOffset;
  /**
  Core VF Configuration Scope.
  Alows both all-core VF curve or per-core VF curve configuration.
  <b>0: All-core Scope, setting the VF curve for all cores.</b>
  1: Per-core Scope, setting the VF curve per-core.
  **/
  UINT8  CoreVfConfigScope;
  /**
  Voltage Regulator Current Limit (Icc Max) for IA, GT and SA. This value represents the Maximum instantaneous current allowed at any given time.
  The value is represented in 1/4 A increments. A value of 400 = 100A. <b>0 (HW default)</b>. Range is 4-2047.
  **/
  UINT16 IaIccMax;
  UINT16 GtIccMax;
  UINT16 SaIccMax;
  UINT8  CorePllVoltageOffset;            ///< Core PLL voltage offset. <b>0: No offset</b>. Range 0-15 in 17.5mv units.
  UINT8  RingPllVoltageOffset;            ///< Ring PLL voltage offset. <b>0: No offset</b>. Range 0-15 in 17.5mv units.
  UINT8  IaAtomPllVoltageOffset;          ///< IA Atom PLL voltage offset. <b>0: No offset</b>. Range 0-15 in 17.5mv units.
  UINT8  CpuDlvrMode;                     ///< Select Core(s) and RING DLVR Mode, <b>0: Regulation Mode</b>. 1: Power Gate Mode.
  UINT8  TscDisableHwFixup;               ///< Core HW Fixup disable during TSC copy from PMA to APIC, <b>0: Enable</b>. 1: Disable.
  UINT8  SaIccUnlimitedMode;              ///< Support SA Unlimited ICCMAX up to maximum value 512A., <b>0: Disable</b>. 1: Enable.
  /**
  When UnderVolt Protection is enabled, user will be not be able to program under voltage in OS runtime.
  0: protection is disabled, <b> 1:protection is enabled </b>.
  **/
  UINT8  UnderVoltProtection;
  UINT8  CoreOpPointReportingEn; ///< Enables Core Operating point reporting. <b>0: Disable</b>, 1: Enable.

  /**
  FLL_OVERCLOCK_MODE: 0x0 = no overclocking,
  0x1 = ratio overclocking with nominal (0.5-1x) reference clock frequency,
  0x2 = BCLK overclocking with elevated (1-3x) reference clock frequency,
  0x3 = BCLK overclocking with extreme elevated (3-5x) reference clock frequency and ratio limited to 63.
  **/
  UINT8  FllOverclockMode;
  /**
  Set CPU D2D Ratio from Range 15 to 40. Default 0 indicates no setting.
  **/
  UINT8  CpuD2dRatio;

  /**
  Array used to specifies the selected Core Offset Voltage.
  This voltage is specified in millivolts.
  **/
  INT16  PerCoreVoltageOffset[MAX_OC_BIG_CORES];
  UINT8  PerCoreRatio[MAX_OC_BIG_CORES];
  UINT8  PerCoreVoltageMode[MAX_OC_BIG_CORES];                  ///< Array used to specifies the selected Core Voltage Mode.
  UINT16 PerCoreVoltageOverride[MAX_OC_BIG_CORES];              ///< Array used to specifies the selected Core Voltage Override.
  UINT16 PerCoreAdaptiveVoltage[MAX_OC_BIG_CORES];              ///< Array used to specifies the selected Core Adaptive Voltage.
  UINT8  PerAtomClusterVoltageMode[MAX_OC_ATOM_CLUSTERS];       ///< Array used to specifies the selected Atom Core Voltage Mode.
  UINT16 PerAtomClusterVoltageOverride[MAX_OC_ATOM_CLUSTERS];   ///< Array used to specifies the selected Atom Core Voltage Override.
  UINT16 PerAtomClusterAdaptiveVoltage[MAX_OC_ATOM_CLUSTERS];   ///< Array used to specifies the selected Atom Core Adaptive Voltage.
  /**
  The Atom cluster max ratio overrides.
  <b>Default: 0</b> Range: 0 to 120.
  **/
  UINT8  AtomClusterRatio[MAX_OC_ATOM_CLUSTERS];
  INT16  PerAtomClusterVoltageOffset[MAX_OC_ATOM_CLUSTERS];

  /**
  Enable or Disable Compute Die SSC configuration.
    - <b>0: Disable</b>
    -    1: Enable
  **/
  UINT8  ComputeDieSscEnable;

  /**
  Enable or Disable Soc Die SSC configuration.
    - <b>0: Disable</b>
    -    1: Enable
  **/
  UINT8  SocDieSscEnable;

  UINT32 CpuBclkOcFrequency;    ///< CPU BCLK OC Frequency in 10KHz units increasing. Value 9800 (10KHz) = 98MHz <b>0 - Auto</b>. Range is 8000-50000 (10KHz)
  UINT32 SocBclkOcFrequency;    ///< SOC BCLK OC Frequency in 10KHz units increasing. Value 9800 (10KHz) = 98MHz <b>0 - Auto</b>. Range is 4000-100000 (10KHz)
  /**
  The Pvd Ratio Threshold for HUb die is the threshold value for input ratio (P0 to Pn) to select the multiplier
  so that the output is within the DCO frequency range. This threshold is applied to SA and MC/CMI PLL for Hub die.
  Range 0-63. When the threshold is 0, static PVD ratio is selected based on the PVD Mode for Hub. <b>0: Default</b>.
  **/
  UINT8  PvdRatioThreshold;
  /**
  PVD Mode selects Static PVD ratio for Hub-die (when PVD THRESHOLD value is 0).
  0x0 = div-1 (VCO = Output clock), 0x1 = div-2 (VCO = 2x Output clock)
  0x2 = div-4 (VCO = 4x Output clock), 0x3 = div-8 (VCO = 8x Output clock)
  **/
  UINT8  PvdMode;
  /**
  Per Module Granularity Bins Override.
  Array is used to specify Granularity Bins for specific Module on a specific CDie.
  It's a negative offset to current resolved P0 ratio. Programming Granularity Bins will always reduce the core frequency.
  value is how many bins of 'Granularity Units' should be subtracted from the otherwise-configured P0 frequency.
  <b>value = 0 (default)</b> Range 0-255.
  **/
  UINT8  GranularityBins[MAX_CDIE][MAX_OC_CCP_MODULES];
  /**
  Temperature Threshold 0 for all Pcores (in degrees C). Running ABOVE this temperature will clip delta
  Down Bins for T0 from the resolved OC Ratio, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>70 </b>. Range 0-100.
  **/
  UINT8  PcoreTvbTempThreshold0;
  /**
  Temperature Threshold 1 for all Pcores (in degrees C). Running ABOVE this temperature will clip delta
  Down Bins for T1 from the resolved OC Ratio, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>100 </b>. Range 0-100.
  **/
  UINT8  PcoreTvbTempThreshold1;
  /**
  Temperature Threshold 0 for all Ecores (in degrees C). Running ABOVE this temperature will clip delta
  Down Bins for T0 from the resolved OC Ratio, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>70 </b>. Range 0-100.
  **/
  UINT8  EcoreTvbTempThreshold0;
  /**
  Temperature Threshold 1 for all Ecores (in degrees C). Running ABOVE this temperature will clip delta
  Down Bins for T1 from the resolved OC Ratio, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>100 </b>. Range 0-100.
  **/
  UINT8  EcoreTvbTempThreshold1;
  /**
  Core disable mask is a bitwise indication of which core should be disabled, while ActiveCoreCount is to set the number of active cores.
  Each bit represents physical core id for both P-core and E-core
  1 - Disable
  0 - Ignored
  Default is 0, means no Core disable mask setting.
  **/
  UINT64 DisablePerCoreMask;

  /**
  Limits Select,
  0 - Per CCP (Module), Command Param2 = CCP ID, CCP ID can range from 0-15.
  1 - Per Pcore group number, Command Param2 = Pcore Group Number (0-7), a Pcore active group has number of active cores as present in MSR 0x1AE.
  <b> 0 </b>.
  **/
  UINT8  TvbConfigLimitSelect;
  /**
  OC TVB Enable/Disable. This control will come into picture only when Thermal Turbo
  is enabled from OCMB 0x18/0x19 (bit 2). When disabled, BIOS will not send OC TVB parameter
  commands 0x24/0x25. When OcTvb is enabled, BIOS will allow user to modify and program
  new parameters for temperature thresholds T0, T1 and delta DownBins for T0 and T1.
  <b>0: Disable</b>, 1: Enable.
   */
  UINT8  OcTvb;
  /**
  Down Bins (delta) for Temperature Threshold 0. When running above T0, the ratio of Per Pcore module will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>1 </b>. Range 0-10.
  **/
  UINT8  PerPcoreRatioDownBinAboveT0[MAX_OC_BIG_CORES];
  /**
  Down Bins (delta) for Temperature Threshold 1. When running above T1, the ratio of Per Pcore module will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>2 </b>. Range 0-10.
  **/
  UINT8  PerPcoreRatioDownBinAboveT1[MAX_OC_BIG_CORES];
  /**
  Down Bins (delta) for Temperature Threshold 0. When running above T0, the ratio of Per Ecore module will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>0 </b>. Range 0-10.
  **/
  UINT8  PerEcoreCcpRatioDownBinAboveT0[MAX_OC_ATOM_CLUSTERS];
  /**
  Down Bins (delta) for Temperature Threshold 1. When running above T1, the ratio of Per Ecore module will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>0 </b>. Range 0-10.
  **/
  UINT8  PerEcoreCcpRatioDownBinAboveT1[MAX_OC_ATOM_CLUSTERS];
  /**
  Down Bins (delta) for Temperature Threshold 0. When running above T0, the ratio of Per Pcore group will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>1 </b>. Range 0-10.
  **/
  UINT8  PerPcoreGrRatioDownBinAboveT0[MAX_OC_BIG_CORE_TRL_GROUPS];
  /**
  Down Bins (delta) for Temperature Threshold 1. When running above T1, the ratio of Per Pcore group will be clipped by
  MAX_RATIO[n]-this value, when Thermal Turbo is enabled from OCMB 0x18 Bit 2 = 0 (Enabled).
  <b>2 </b>. Range 0-10.
  **/
  UINT8  PerPcoreGrRatioDownBinAboveT1[MAX_OC_BIG_CORE_TRL_GROUPS];
  UINT8  PerCoreMaxRatio[MAX_OC_BIG_CORES];                  ///< Array used to specifies the selected Core Max Ratio.
  UINT8  PerAtomClusterMaxRatio[MAX_OC_ATOM_CLUSTERS];       ///< Array used to specifies the selected Atom Cluster Max Ratio.

  /**
  Voltage mode, specifies which voltage mode the processor will be operating.
  <b>0: Adaptive Mode</b> allows the voltage curve when beyond fused range;
  1: Override, sets one voltage for the entire frequency range.
  **/
  UINT8  VoltageMode[MAX_OC_DOMAINS];
  /**
  Maximum turbo ratio override allows to increase frequency beyond the fused max turbo ratio limit (P0).
  <b>0. no override/HW defaults.</b>. Range non-turbo max.
  **/
  UINT8  MaxOcRatio[MAX_OC_DOMAINS];
  /**
  The voltage override which is applied to the entire range of cpu core frequencies.
  Used when VoltageMode = Override.
  <b>0. no override</b>. Range 0-2000 mV.
  **/
  UINT16 VoltageOverride[MAX_OC_DOMAINS];
  /**
  Adaptive voltage target used to define the interpolation voltage point when operating in adaptive mode range.
  Used when VoltageMode = Adaptive.
  <b>0. no override</b>. Range 0-2000mV.
  **/
  UINT16 AdaptiveVoltage[MAX_OC_DOMAINS];
  /**
  The voltage offset applied on top of all other voltage modes. This offset is applied over the entire frequency range.
  This is a 2's complement number in mV units. <b>Default: 0</b> Range: -1000 to 1000.
  **/
  INT16  VoltageOffset[MAX_OC_DOMAINS];
  /**
  Selects Voltage & Frequency Point Offset between Legacy and Selection modes.
  Need Reset System after enabling OverClocking Feature to initialize the default value.
  <b>0: In Legacy Mode, setting a global offset for the entire VF curve.</b>
  1: In Selection modes, setting a selected VF point.
  **/
  UINT8  VfPointOffsetMode[MAX_OC_DOMAINS];
  UINT8  VfPointCount[MAX_OC_DOMAINS];                          ///< Number of supported Voltage & Frequency Point.
  UINT8  VfPointRatio[MAX_OC_DOMAINS][MAX_OC_VF_POINTS];    ///< Array for the each selected VF Point to display the Core Ration. It's only for read command and not for write.
  /**
  Array used to specifies the Voltage Offset applied to the each selected VF Point.
  This voltage is specified in millivolts. <b>Default: 0</b> Range: 0 to 1000
  **/
  INT16  VfPointOffset[MAX_OC_DOMAINS][MAX_OC_VF_POINTS];

  /**
  Array for the each Domain to set OC Max Voltage limits  <b>Default: 0 </b> Range: 0 to 2000
  NOTE: Only Core/Ring Domain support multi-die, for the other domains DIE = 0 as they are package scope.
  **/
  UINT16 MaxVoltageLimit[MAX_CDIE][MAX_OC_DOMAINS];

  /**
  Disabling Process Vmax Limit will allow user to set any voltage. If this limit is not disabled then, request above factory set limits will be rejected.
  This is a sticky bit, BIOS can only set this bit, it cannot be cleared by mailbox command. This setting persists over warm reset, and cleared on cold reset.
  MailBox control is to disable the ProcessVmaxLimit (reverse encoding is used). 0: Disable;<b>1: Enable</b>;
  Note: Disabling the voltage limit checks may cause permanent damage to processor.
  Note: This bit cannot be set after BIOS_RST_CPL.
  */
  UINT8  ProcessVmaxLimit;
  UINT8  CorePllCurrentRefTuningOffset;               ///< Core PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15
  UINT8  RingPllCurrentRefTuningOffset;               ///< Ring PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15
  UINT8  IaAtomPllCurrentRefTuningOffset;             ///< IaAtom PLL Current Reference Tuning Offset. <b>0: No offset</b>. Range 0-15
  /**
  Request Core Min Ratio. Limit Core minimum ratio for extreme overclocking. Default 0 indicates no request.
  **/
  UINT8  CoreMinRatio;
  UINT8  NegativeTemperatureReporting;                ///< Negative Temperature Reporting Enable. <b>0: Disable</b>, 1: enable
  /**
  Power Density Throttle control allows user to disable P-core and E-core throttling for power density protection.
  This is a sticky bit, Once set, will remain set until cold or warm reset is performed.
  MailBox control is to disable the Power Density Throttling (reverse encoding is used). 0: Disable;<b>1: Enable</b>;
  **/
  UINT8  PcorePowerDensityThrottle;
  UINT8  EcorePowerDensityThrottle;
  UINT8  RsvdByte0[11];
} OVERCLOCKING_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CPU_OVERCLOCKING_CONFIG_H_
