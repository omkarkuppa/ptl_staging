/** @file
  Register names for PCH TSN

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
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _TSN_REGS_H_
#define _TSN_REGS_H_

//
// TSN Controller Registers
//

// TSN MEM Space Registers Offset
#define R_TSN_MEM_MAC_EXTENDED_CONFIGURATION         0x140        // The MAC Extended Configuration register establishes the operating mode of the MAC transmitter
#define B_TSN_MEM_MAC_EXTENDED_CONFIGURATION_MA0IE   BIT28        // MAC Address 0 Input Enable

#define R_TSN_MEM_MAC_ADDRESS0_HIGH                  0x300        // MAC Address 0 High
#define B_TSN_MEM_MAC_ADDRESS0_HIGH_AE               BIT31        // Address Enable
#define B_TSN_MEM_MAC_ADDRESS_HIGH_MASK              0xFFFF       // Upper 16 bits of the MAC address
#define R_TSN_MEM_MAC_ADDRESS0_LOW                   0x304        // MAC Address 0 Low

//
// TSN Iosf2Ocp bridge configuration registers
//
#define R_TSN_PCR_PCICFGCTRL                         0x200                         ///< PCI Configuration Control
#define B_TSN_PCR_PCICFGCTR_PCI_IRQ                  0x0FF00000                    ///< PCI IRQ number
#define N_TSN_PCR_PCICFGCTR_PCI_IRQ                  20
#define B_TSN_PCR_PCICFGCTR_ACPI_IRQ                 0x000FF000                    ///< ACPI IRQ number
#define N_TSN_PCR_PCICFGCTR_ACPI_IRQ                 12
#define B_TSN_PCR_PCICFGCTR_IPIN1                    (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_TSN_PCR_PCICFGCTR_IPIN1                    8

#endif
