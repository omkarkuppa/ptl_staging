/** @file
  Register names for LPSS I3C Controllers

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

  @par Specification Reference:
**/
#ifndef _LPSS_REGS_H_
#define _LPSS_REGS_H_

//
// LPSS Subsystem Private Configuration Registers (PCR)
//
#define R_LPSS_PCR_PMCTL                              0x1D0    //< Power Management Control Register
#define V_LPSS_PCR_PMCTL_PWR_GATING                   0x3F

#define R_LPSS_PCR_PCICFGCTR1                         0x200    //< PCI Configuration Control 1 Register
#define B_LPSS_PCR_PCICFGCTR1_PCI_CFG_DIS1            BIT0     //< Pcicfgctr1  Pci Cfg Disable Field
#define B_LPSS_PCR_PCICFGCTR1_ACPI_INTR_EN1           BIT1     //< Pcicfgctr1  Acpi Interrupt En Field
#define B_LPSS_PCR_PCICFGCTR1_BAR1_DISABLE1           BIT7     //< Pcicfgctr1  Bar Disable Field
#define B_LPSS_PCR_PCICFGCTR1_IPIN1                   ( BIT11 | BIT10 | BIT9 | BIT8 ) //< Pcicfgctr1  Interrupt Pin Field
#define N_LPSS_PCR_PCICFGCTR1_IPIN1                   8
#define B_LPSS_PCR_PCICFGCTR1_ACPI_IRQ1               ( BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12 ) //< Pcicfgctr1  Acpi Irq Field
#define N_LPSS_PCR_PCICFGCTR1_ACPI_IRQ1               12
#define B_LPSS_PCR_PCICFGCTR1_PCI_IRQ1                ( BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 ) //< Pcicfgctr1  Pci Irq Num Field
#define N_LPSS_PCR_PCICFGCTR1_PCI_IRQ1                20

//
// General Purpose
//
#define R_LPSS_PCR_GEN_PVT_LOW_REGRW2                  0x604    //< General Purpose PRV RW Register 2
#define V_LPSS_PCR_GEN_PVT_LOW_REGRW2_CLK_GATING       (BIT1 | BIT11) //PGCB_FRC_CLK_CP_EN (BIT1), CDC_SIDE_CFG_CLKREQ_CTL_EN (BIT11)

#define R_LPSS_PCR_GEN_PVT_LOW_REGRW4                  0x60C    //< General Purpose PRV RW Register 4
#define V_LPSS_PCR_GEN_PVT_LOW_REGRW4_SLC_CLK_CGC      BIT16    //< Clock Gate Control [0: Clock Gating Enabled, 1: Force Clock

#define R_LPSS_PCR_GEN_PVT_HIGH_REGRW3                 0x618    //< General Purpose Register 7
#define V_LPSS_PCR_GEN_PVT_HIGH_REGRW3_UART_BYTE_ADDR_EN BIT0

#endif //_LPSS_REGS_H_
