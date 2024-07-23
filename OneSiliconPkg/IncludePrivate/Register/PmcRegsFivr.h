/** @file
  Register names for PCH PMC FIVR device

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _PCH_REGS_PMC_FIVR_H_
#define _PCH_REGS_PMC_FIVR_H_

//
// PWRM Registers
//
#define R_PMC_PWRM_EXT_RAIL_CONFIG                                    0x11B8                       ///< External Rail Config

#define B_PMC_PWRM_EXT_RAIL_CONFIG_VNN_SUPPORTED                      0xF0000000                   ///< Supported External VNN Rail configuration
#define N_PMC_PWRM_EXT_RAIL_CONFIG_VNN_SUPPORTED                      28                           ///< Supported External VNN Rail configuration
#define B_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_SUPPORTED                    0x0F000000                   ///< Supported External V1P05 Rail configuration
#define N_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_SUPPORTED                    24                           ///< Supported External V1P05 Rail configuration
#define B_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_S0IX_SX                      0x1F00                       ///< Enable External V1P05 Rail in S0x/Sx
#define N_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_S0IX_SX                      8                            ///< Enable External V1P05 Rail in S0x/Sx
#define B_PMC_PWRM_EXT_RAIL_CONFIG_VNN_S0IX_SX                        0x1F                         ///< Enable External VNN Rail in S0ix/Sx
#define N_PMC_PWRM_EXT_RAIL_CONFIG_VNN_S0IX_SX                        0                            ///< Enable External VNN Rail in S0ix/Sx

#define R_PMC_PWRM_EXT_V1P05_VR_CONFIG                                0x11C0                       ///< External v1p05 Rail Config
#define B_PMC_PWRM_EXT_V1P05_VR_CONFIG_VOLTAGE                        0x7FF0000                    ///< External v1p05 Voltage Value
#define N_PMC_PWRM_EXT_V1P05_VR_CONFIG_VOLTAGE                        16                           ///< External v1p05 Voltage Value
#define B_PMC_PWRM_EXT_V1P05_VR_CONFIG_ICC_MAX                        0xFFFF                       ///< External v1p05 Icc Max Value
#define N_PMC_PWRM_EXT_V1P05_VR_CONFIG_ICC_MAX                        0                            ///< External v1p05 Icc Max Value

#define R_PMC_PWRM_EXT_VNN_VR_CONFIG                                  0x11C4                       ///< External VNN Rail Config
#define B_PMC_PWRM_EXT_VNN_VR_CONFIG_VOLTAGE                          0x7FF0000                    ///< External VNN Voltage Value
#define N_PMC_PWRM_EXT_VNN_VR_CONFIG_VOLTAGE                          16                           ///< External VNN Voltage Value
#define B_PMC_PWRM_EXT_VNN_VR_CONFIG_ICC_MAX                          0xFFFF                       ///< External VNN Icc Max Value
#define N_PMC_PWRM_EXT_VNN_VR_CONFIG_ICC_MAX                          0                            ///< External VNN Icc Max Value

#define R_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF                        0x11C8                       ///< Hold Off Control for V1p05
#define B_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_V1P05_CTRL_RAMP_TMR    0x0000FF00                   ///< V1p05 Control Ramp Timer
#define N_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_V1P05_CTRL_RAMP_TMR    8                            ///< V1p05 Control Ramp Timer
#define B_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_VNN_CTRL_RAMP_TMR      0x000000FF                   ///< VNN Control Ramp Timer
#define N_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_VNN_CTRL_RAMP_TMR      0                            ///< VNN Control Ramp Timer

#define R_PMC_PWRM_EXT_FET_RAMP_CONFIG                                0x11CC         ///< External FET Ramp Time Configuration
// [RW/L][7:0] Defines the ramp time of the external V1p05-PHY FET.
// Each increment is 31us (ie. 0x4=124us).
// locked by V1P05_PHY_FRT_LOCK.
// Default 0x4
#define B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FET_RAMPTIME         0xFF
// [RW/L][15] The bit is used to lock V1P05_PHY_FET_RAMP_TIME.
// This bit is self-locking (i.e. once written to 1, it can only be cleared by host_prim_rst_b).
// Default 0x0
#define B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FRT_LOCK             BIT15
// [RW/L][23:16] Defines the ramp time  of the external V1p05-IS FET
// Each increment is 10us (ie. 0x4=40us).
// locked by V1P05_IS_FRT_LOCK.
// Default 0x4
#define B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_IS_FET_RAMPTIME          0xFF0000
#define N_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_IS_FET_RAMPTIME_SHIFT    16
// [RW/L][31] The bit is used to lock V1P05_IS_FET_RAMPTIME.
// This bit is self-locking (i.e. once written to 1, it can only be cleared by host_prim_rst_b).
// Default 0x0
#define B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_IS_FRT_LOCK          BIT31

#define R_PMC_PWRM_VCCIN_AUX_CFG1                                     0x11D0                       ///< VCCIN AUX CONFIG Register1
#define B_PMC_PWRM_VCCIN_AUX_CFG1_LCM_HCM_VOLT_TRANS_TIME             0xFF0000                     ///< Low Current Mode Voltage to High Current Mode Voltage Transition Time
#define N_PMC_PWRM_VCCIN_AUX_CFG1_LCM_HCM_VOLT_TRANS_TIME             16                           ///< Low Current Mode Voltage to High Current Mode Voltage Transition Time
#define B_PMC_PWRM_VCCIN_AUX_CFG1_RMV_HCM_VOLT_TRANS_TIME             0xFF00                       ///< Retention Mode Voltage to High Current Mode Voltage Transition Time
#define N_PMC_PWRM_VCCIN_AUX_CFG1_RMV_HCM_VOLT_TRANS_TIME             8                            ///< Retention Mode Voltage to High Current Mode Voltage Transition Time
#define B_PMC_PWRM_VCCIN_AUX_CFG1_RMV_LCM_VOLT_TRANS_TIME             0xFF                         ///< Retention Mode Voltage to Low Current Mode Voltage Transition Time
#define N_PMC_PWRM_VCCIN_AUX_CFG1_RMV_LCM_VOLT_TRANS_TIME             0                            ///< Retention Mode Voltage to Low Current Mode Voltage Transition Time

#define R_PMC_PWRM_VCCIN_AUX_CFG2                                     0x11D4                       ///< VCCIN AUX CONFIG Register2
#define B_PMC_PWRM_VCCIN_AUX_CFG2_OFF_HCM_VOLT_TRANS_TIME             0x3FF                        ///< Voltage transition time when PCH changes the VCCIN_AUX regulator from 0V to the high current mode voltage
#define N_PMC_PWRM_VCCIN_AUX_CFG2_OFF_HCM_VOLT_TRANS_TIME             0                            ///< Voltage transition time when PCH changes the VCCIN_AUX regulator from 0V to the high current mode voltage

#define R_PMC_PWRM_CPPM_FIVR_POL1A                                    0x11F0                       ///< Register Configures If And How CPPM Interacts With FIVR
#define B_PMC_PWRM_CPPM_FIVR_POL1A_CPPM_FIVR_VOLT_QUAL                BIT30                        ///< CPPM Qualifier Enable for FIVR
#define B_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_SEL                  BIT29                        ///< ASLT/PLT SELECTION for FIVR
#define B_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_THRESH               0x1FF                        ///<  LTR Threshold for FIVR
#define V_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_THRESH               0x4
#define R_PMC_PWRM_CPPM_FIVR_POL1B                                    0x11F4                       ///< Register Configures If And How CPPM Interacts With FIVR
#define B_PMC_PWRM_CPPM_FIVR_POL1B_TNTE_FIVR_VOLT_PRE_WAKE            0x1FFFF                      ///< TNTE PreWake for FIVR Voltage

#define B_PMC_PWRM_CFG3_BIOS_FIVR_DYN_EN                              BIT28                        ///< Enable Dynamic FIVR Management

#endif // _PCH_REGS_PMC_FIVR_H_
