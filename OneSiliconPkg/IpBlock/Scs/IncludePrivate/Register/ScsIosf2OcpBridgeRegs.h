/** @file
  Register definitions for Iosf2Ocp bridge.

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

#ifndef _SCS_IOSF2OCP_BRIDGE_REGS_H_
#define _SCS_IOSF2OCP_BRIDGE_REGS_H_
//
// SCS Iosf2Ocp bridge configuration registers
//
#define R_SCS_PCR_IOSFCTL                     0x00                          ///< IOSF Control
#define B_SCS_PCR_IOSFCTL_NSNPDIS             BIT7                          ///< Non-Snoop Disable
#define R_SCS_PCR_PMCTL                       0x1D0                         ///< Power Management Control
#define R_SCS_PCR_PCICFGCTR_BASE              0x200
#define B_SCS_PCR_PCICFGCTR_BAR1DIS           BIT7                          ///< BAR 1 Disable
#define B_SCS_PCR_PCICFGCTR_PCI_CFG_DIS       BIT0                          ///< PCI Configuration Space Disable
#define B_SCS_PCR_PCICFGCTR_PCI_IRQ           0x0FF00000                    ///< PCI IRQ number
#define N_SCS_PCR_PCICFGCTR_PCI_IRQ           20
#define B_SCS_PCR_PCICFGCTR_ACPI_IRQ          0x000FF000                    ///< ACPI IRQ number
#define B_SCS_PCR_PCICFGCTR_IPIN1             (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_SCS_PCR_PCICFGCTR_IPIN1             8

#define R_SCS_PCR_GPPRVRW1                    0x600                         ///< Clock Gating Control
#define B_SCS_PCR_GPPRVRW1_PG_CG              0x7FFFFF                      ///< Clock and power gating control bits in GPPRVRW1
#define B_SCS_PCR_GPPRVRW1_UFS1_IES           BIT31
#define B_SCS_PCR_GPPRVRW1_UFS0_IES           BIT30
#define B_SCS_PCR_GPPRVRW1_UFS0_ENABLE        BIT23
#define B_SCS_PCR_GPPRVRW1_UFS0_DUAL_LANE     BIT24
#define B_SCS_PCR_GPPRVRW1_UFS1_ENABLE        BIT25
#define B_SCS_PCR_GPPRVRW1_UFS1_DUAL_LANE     BIT26
#define R_SCS_PCR_GPPRVRW2                    0x604                         ///< Host Controller Disable
#define B_SCS_PCR_GPPRVRW2_VDD1_INVERT        BIT16                         ///< VDD1_Invert to 1 to make VDD1_POWER_ENABLE active LOW
#define R_SCS_PCR_GPPRVRW6                    0x614                         ///< 1.8V Signal Select Delay Control
#define B_SCS_PCR_GPPRVRW6_1P8_SEL_DELAY      0xFF
#define V_SCS_PCR_GPPRVRW6_1P8_SEL_DELAY      0x7F                          ///< Rcomp SDCARD 10ms delay during switch

#define R_SCS_PCR_I0_IA_AGENT_CONTROL              0x1C20
#define R_SCS_PCR_SCC2MAIN_T0_TA_AGENT_CONTROL     0x4820
#endif

