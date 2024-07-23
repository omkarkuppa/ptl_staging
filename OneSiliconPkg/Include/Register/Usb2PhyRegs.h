/** @file
  Register defines for USB2 PHY controller

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
#ifndef _USB_DEVICE_REGS_H_
#define _USB_DEVICE_REGS_H_

//
// USB2 Private Configuration Registers
// USB2 HIP design featured
// (PID:USB2)
// Doesn't apply to CPU (only USB3 functionality present)
//
#define R_USB2_PCR_GLOBAL_PORT                0x4001                    ///< USB2 GLOBAL PORT
#define R_USB2_PCR_PP_LANE_BASE_ADDR          0x4000                    ///< PP LANE base address
#define R_USB2_PCR_PER_PORT                   0x00                      ///< USB2 PER PORT          Addr[7:2] = 0x00
#define B_USB2_PCR_PER_PORT_PERPORTRXISET     (BIT19 | BIT18 | BIT17)
#define N_USB2_PCR_PER_PORT_PERPORTRXISET     17
#define B_USB2_PCR_PER_PORT_PERPORTTXPEHALF   BIT14
#define N_USB2_PCR_PER_PORT_PERPORTTXPEHALF   14
#define B_USB2_PCR_PER_PORT_PERPORTPETXISET   (BIT13 | BIT12 | BIT11)
#define N_USB2_PCR_PER_PORT_PERPORTPETXISET   11
#define B_USB2_PCR_PER_PORT_PERPORTTXISET     (BIT10 | BIT9 | BIT8)
#define N_USB2_PCR_PER_PORT_PERPORTTXISET     8
#define R_USB2_PCR_UTMI_MISC_PER_PORT         0x08                      ///< UTMI MISC REG PER PORT Addr[7:2] = 0x08
#define R_USB2_PCR_PER_PORT_2                 0x26                      ///< USB2 PER PORT 2        Addr[7:2] = 0x26
#define B_USB2_PCR_PER_PORT_2_PREDEEMP_SEMIFLEX_EN  BIT30               ///< Semi Flexi Pre/DE-emphasis bit
#define N_USB2_PCR_PER_PORT_2_PREDEEMP_SEMIFLEX_EN  30                  ///< Semi Flexi Pre/DE-emphasis bit position
#define B_USB2_PCR_PER_PORT_2_TXEMPHASISEN    (BIT24 | BIT23)           ///< HSNPREDRVSEL bits value USB2 PER PORT2 register
#define N_USB2_PCR_PER_PORT_2_TXEMPHASISEN    23
#define R_USB2_PCR_GLB_ADP_VBUS_REG           0x402B                    ///< GLB ADP VBUS REG
#define R_USB2_PCR_GLOBAL_PORT_2              0x402C                    ///< USB2 GLOBAL PORT 2
#define R_USB2_PCR_PLLDIVRATIOS_0             0x7000                    ///< PLLDIVRATIOS_0
#define B_USB2_PCR_PLLDIVRATIOS_0_FBDIVRATIO  (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_USB2_PCR_PLLDIVRATIOS_0_FBDIVRATIO  24
#define R_USB2_PCR_CONFIG_0                   0x7008                    ///< CONFIG_0
#define B_USB2_PCR_CONFIG_0_INT_COEFF         (BIT17 | BIT16 | BIT15 | BIT14 | BIT13)
#define N_USB2_PCR_CONFIG_0_INT_COEFF         13
#define B_USB2_PCR_CONFIG_0_PROP_COEFF        (BIT12 | BIT11 | BIT10 | BIT9)
#define N_USB2_PCR_CONFIG_0_PROP_COEFF        9
#define R_USB2_PCR_CONFIG_3                   0x7014                    ///< CONFIG_3
#define B_USB2_PCR_CONFIG_3_SFRCALIB_FMINCNT  (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_USB2_PCR_CONFIG_3_SFRCALIB_FMINCNT  24
#define B_USB2_PCR_CONFIG_3_SFRCALIB_FMAXCNT  (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_USB2_PCR_CONFIG_3_SFRCALIB_FMAXCNT  16
#define B_USB2_PCR_CONFIG_3_TDCCALIB_OSCCNT   (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_USB2_PCR_CONFIG_3_TDCCALIB_OSCCNT   8
#define B_USB2_PCR_CONFIG_3_LOCKTIMERCNT_TH   (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1)
#define N_USB2_PCR_CONFIG_3_LOCKTIMERCNT_TH   1
#define R_USB2_PCR_DFT_1                      0x7024                    ///< DFT_1
#define B_USB2_PCR_DFT_1_KPSCALE              (BIT16 | BIT15)
#define N_USB2_PCR_DFT_1_KPSCALE              15
#define B_USB2_PCR_DFT_1_KISCALE              (BIT14 | BIT13)
#define N_USB2_PCR_DFT_1_KISCALE              13
#define R_USB2_PCR_SFRCONFIG_0                0x702C                    ///< SFRCONFIG_0
#define B_USB2_PCR_SFRCONFIG_0_SFRTIMER_COEF  (BIT7 | BIT6 | BIT5 | BIT4)
#define N_USB2_PCR_SFRCONFIG_0_SFRTIMER_COEF  4
#define R_USB2_PCR_PLL1                       0x7F02                    ///< USB2 PLL1
#define R_USB2_PCR_CFG_COMPBG                 0x7F04                    ///< USB2 COMPBG

//
// External USB2 PHY private registers
//
#define R_USB2_PCR_SNPSPHYSIPPOL                0x00        ///< SNPS PHY SIP POLICIES
#define R_USB2_PCR_SNPSPHYSIPPOL_ENCMNONNOVRL1  BIT12       ///< Enable COMMONONN Override In L1
#define B_USB2_PCR_SNPSPHYSIPPOL_CLKCORETCGL2   BIT3        ///< CLKCORE (refclk) Trunk Gating ENable on SUSPEND/L2
#define R_USB2_PCR_SNPSPHYSIPPOL_CLKCORETCGL1   BIT2        ///< CLKCORE (refclk) Trunk Gating Enable on SLEEP/L1

#endif
