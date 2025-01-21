/** @file
  CPU Power Management Basic Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _CPU_POWER_MGMT_BASIC_CONFIG_H_
#define _CPU_POWER_MGMT_BASIC_CONFIG_H_

extern EFI_GUID gCpuPowerMgmtBasicConfigGuid;

#ifndef TURBO_RATIO_LIMIT_ARRAY_SIZE
#define TURBO_RATIO_LIMIT_ARRAY_SIZE 8
#endif

#pragma pack (push,1)

/**
  CPU Power Management Basic Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/

#define CPU_POWER_MGMT_BASIC_CONFIG_REVISION 1

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
  Sets the boot frequency starting from reset vector.
   - 0: Maximum battery performance.
   - 1: Maximum non-turbo performance.
   - <b>2: Turbo performance.</b>
  @note If Turbo is selected BIOS will start in max non-turbo mode and switch to Turbo mode.
  **/
  UINT32 BootFrequency                  : 2;
  UINT32 SkipSetBootPState              : 1;      ///< Choose whether to skip SetBootPState function for all APs; <b>0: Do not skip</b>; 1: Skip.
  /**
  Enable or Disable Intel Speed Shift Technology.
  Enabling allows for processor control of P-state transitions.
  0: Disable; <b>1: Enable;</b> Bit 1 is ignored.
  @note Currently this feature is recommended to be enabled only on win10
  **/
  UINT32 Hwp                            : 2;
  UINT32 TurboPowerLimitLock            : 1;      ///< MSR 0x610[63] and 0x618[63]: Locks all Turbo power limit settings to read-only; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 TurboMode                      : 1;      ///< Enable or Disable Turbo Mode. Disable; <b>1: Enable</b>
  UINT32 HwpInterruptControl            : 1;      ///< Set HW P-State Interrupts Enabled  for MISC_PWR_MGMT MSR 0x1AA[7]; <b>0: Disable</b>; 1: Enable.
  UINT32 HwpLock                        : 1;      ///< HWP Lock in MISC PWR MGMT MSR 1AAh; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 RsvdBits                       : 23;     ///< Reserved for future use.

  /**
  1-Core Ratio Limit: LFM to Fused 1-Core Ratio Limit. For overclocking parts: LFM to Fused 1-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 1-Core Ratio Limit Must be greater than or equal to 2-Core Ratio Limit, 3-Core Ratio Limit, 4-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  OneCoreRatioLimit;
  /**
  2-Core Ratio Limit: LFM to Fused 2-Core Ratio Limit, For overclocking part: LFM to Fused 2-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 2-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  TwoCoreRatioLimit;
  /**
  3-Core Ratio Limit: LFM to Fused 3-Core Ratio Limit, For overclocking part: LFM to Fused 3-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 3-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  ThreeCoreRatioLimit;
  /**
  4-Core Ratio Limit: LFM to Fused 4-Core Ratio Limit, For overclocking part: LFM to Fused 4-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 4-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  FourCoreRatioLimit;
  /**
   5-Core Ratio Limit: LFM to Fused 5-Core Ratio Limit, For overclocking part: LFM to Fused 5-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
     - This 5-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
     - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  FiveCoreRatioLimit;
  /**
  6-Core Ratio Limit: LFM to Fused 6-Core Ratio Limit, For overclocking part: LFM to Fused 6-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 6-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  SixCoreRatioLimit;
  /**
  7-Core Ratio Limit: LFM to Fused 7-Core Ratio Limit, For overclocking part: LFM to Fused 7-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 7-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  SevenCoreRatioLimit;
  /**
  8-Core Ratio Limit: LFM to Fused 8-Core Ratio Limit, For overclocking part: LFM to Fused 8-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 8-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  EightCoreRatioLimit;
  /**
  HwP Autonomous Per Core P State
  Disabling will set Bit 30 = 1, command 0x11. When set, autonomous will request the same value
  for all cores all the time.
  0: Disable;<b> 1: Enable;</b>
  **/
  UINT8  EnableHwpAutoPerCorePstate     : 1;
  /**
  HwP Autonomous EPP grouping.
  Disabling will set Bit 29 = 1, command 0x11. When set, autonomous will not necesarrily request the same value
  for all cores with same EPP.
  Enabling will clean Bit 29 = 0, command 0x11. Autonomous will request same values for all cores with same EPP.
  <b> 0: Disable </b>; 1: Enable;
  **/
  UINT8  EnableHwpAutoEppGrouping       : 1;
  /**
  Support for Fast MSR for IA32_HWP_REQUEST.
  On systems with HwP enabled, if this feature is available as indicated by MSR 0x65F[0] = 1,
  set MSR 0x657[0] = 1.
  0: Disable; <b> 1: Enable;</b>
  **/
  UINT8  EnableFastMsrHwpReq            : 1;
  UINT8  ReservedBits1                  : 5;      ///< Reserved for future use.
  UINT8  MinRingRatioLimit;                       ///< Minimum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default
  UINT8  MaxRingRatioLimit;                       ///< Maximum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default
  /**
  Turbo Ratio Limit Ratio array, TurboRatioLimitRatio[7-0] will pair with TurboRatioLimitNumCore[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x1AD.
  Note:
    - These policies are for platforms support MSR pair 0x1AD and 0x1AE.
  **/
  UINT8  TurboRatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  Turbo Ratio Limit Num Core array, TurboRatioLimitNumCore[7-0] will pair with TurboRatioLimitRatio[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x1AE.
  Note:
    - These policies are for platforms support MSR pair 0x1AD and 0x1AE.
  **/
  UINT8  TurboRatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  ATOM Turbo Ratio Limit Ratio array, AtomTurboRatioLimitRatio[7-0] will pair with AtomTurboRatioLimitNumCore[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x650.
  Note:
    - These policies are for platforms support ATOM MSR pair 0x650 and 0x651.
  **/
  UINT8  AtomTurboRatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  ATOM Turbo Ratio Limit Num Core array, AtomTurboRatioLimitNumCore[7-0] will pair with AtomTurboRatioLimitRatio[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x651.
  Note:
    - These policies are for platforms support ATOM MSR pair 0x650 and 0x651.
  **/
  UINT8  AtomTurboRatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  Resource Priority Feature
  If Rp is Enable, BIOS configures Rp.
  <b>0: Disable</b>; 1: Enable;
  **/
  UINT8  EnableRp;
  /**
  Survivability Feature: SoC Power Floor Management control
  This option is to disable SoC's aggressive throttling to manage SoC floor power (minimum power consumption).
  Power Floor Management is enabled by default. Disabling this might effectively raise power floor of the SoC and may lead to stability issues.
  0: Disable;<b>1: Enable</b>; Mailbox parameter is to disable the throttling (reverse encoding is used).
  **/
  UINT8  PowerFloorManagement;
  /**
  Survivability Feature: Display Disconnect for Floor Power Management
  This option is applicable only if SoC Floor Power Management is enabled.
  When Enabled: SoC can disconnect secondary/external display to lower SoC floor power (Default).
  When Disabled: Display disconnect will not be used by SoC for power floor management.
  0: Disable;<b>1: Enable</b>; MB control is to disable the disconnect (reverse encoding is used).
  **/
  UINT8  PowerFloorDisplayDisconnect;
  /**
  Survivability Feature: Pcie Gen Speed Downgrade option for Floor Power Management
  This option is applicable only if SoC Floor Power Management is enabled.
  When Enabled: SoC can reduce PCIE GEN speed to lower SoC floor power (Default).
  When Disabled: SoC will not reduce PCIE GEN speed to lower SoC floor power.
  0: Disable;<b>1: Enable</b>; MB control is to disable the disconnect (reverse encoding is used).
  **/
  UINT8  PowerFloorPcieGenDowngrade;
  UINT8  Reserved[5];
} CPU_POWER_MGMT_BASIC_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_BASIC_CONFIG_H_
