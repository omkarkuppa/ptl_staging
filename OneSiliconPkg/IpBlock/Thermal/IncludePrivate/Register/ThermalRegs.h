/** @file
  Register names for PCH Thermal Device

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

@par Specification Reference:
**/
#ifndef _THERMAL_REGS_H_
#define _THERMAL_REGS_H_

//
//  Thermal Device Registers
//
#define R_PMC_PWRM_THERMAL_CTEN                                      0x150C  ///< Catastrophic Trip Point Enable
#define B_PMC_PWRM_THERMAL_CTEN_CTENLOCK                             BIT31   ///< Policy Lock-Down Bit
#define B_PMC_PWRM_THERMAL_CTEN_CPDEN                                BIT0    ///< Catastrophic Power-Down Enable

#define R_PMC_PWRM_THERMAL_ECRPTEN                                   0x1510  ///< EC Thermal Sensor Reporting Enable
#define B_PMC_PWRM_THERMAL_ECRPTEN_ECRPTENLOCK                       BIT31   ///< Lock-Down Bit
#define B_PMC_PWRM_THERMAL_ECRPTEN_EN_PMC_TO_EC_TEMP_RPT             BIT0    ///< Enable PMC to EC Temperature Reporting

#define R_PMC_PWRM_THERMAL_TL                                        0x1520  ///< Throttle Levels
#define B_PMC_PWRM_THERMAL_TL_TLLOCK                                 BIT31   ///< TL LOCK
#define B_PMC_PWRM_THERMAL_TL_TTEN                                   BIT29   ///< TT Enable

#define R_PMC_PWRM_THERMAL_TLEN                                      0x1528  ///< Throttle Levels Enable
#define B_PMC_PWRM_THERMAL_TLEN_TLENLOCK                             BIT31   ///< TLENLOCK
#define B_PMC_PWRM_THERMAL_TLEN_PMCTEN                               BIT14   ///< PMC Throttle Enable

#define R_PMC_PWRM_THERMAL_PHLC                                      0x1540  ///< PCH Hot Level Control
#define B_PMC_PWRM_THERMAL_PHLC_PHLCLOCK                             BIT31   ///< PHL Lock
#define B_PMC_PWRM_THERMAL_PHLC_PHLE                                 BIT15   ///< PHL Enable

#define R_PMC_PWRM_DTS_S0IX_CONFIG                                   0x1580  ///< DTS-SIP operation in S0ix
#define B_PMC_PWRM_DTS_S0IX_CONFIG_DISABLE_DTS_IN_S0IX               BIT0    ///< DISABLE_DTS_IN_S0IX
#define B_PMC_PWRM_DTS_S0IX_CONFIG_ENABLE_DTS_THRESHOLD_XING_CHECK   BIT1    ///< ENABLE DTS THRESHOLD XING CHECK
#define B_PMC_PWRM_DTS_S0IX_CONFIG_S0IXLOCK                          BIT31   ///< Policy Lock-Down Bit

#define R_PMC_PWRM_SOCIFTTC                                          0x15A8  ///< SoC Internal Fabric Thermal Throttling Configuration
#define B_PMC_PWRM_SOCIFTTC_SOCIFTTML                                BIT1    ///< SoC Internal Fabric Thermal Throttling Mechanism Lock

#define R_PMC_PWRM_THERMAL_PM_DOWN_PPB_CFG                           0x18E4  ///< PM_DOWN PCH_POWER_BUDGET CONFIGURATION
#define B_PMC_PWRM_THERMAL_PM_DOWN_PPB_CFG_PPB_CFG_LOCK              BIT0    ///< PCH Power Budget Configuration lock

#endif
