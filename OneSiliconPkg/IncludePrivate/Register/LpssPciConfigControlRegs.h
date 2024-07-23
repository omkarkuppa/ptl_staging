/** @file
   PCI Configuration Control Register for LPSS Controllers.

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
    Register that is specific to -N denoted by "_PCH_N_" in component name.
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
#ifndef _LPSS_REGS_PCI_CFG_CTR_H_
#define _LPSS_REGS_PCI_CFG_CTR_H_

#define R_LPSS_PCR_PCICFGCTR1            0x200     ///< PCI Configuration Control 1 Register
#define R_LPSS_PCR_PCICFGCTR2            0x204     ///< Pci Configuration Control 2 Register
#define R_LPSS_PCR_PCICFGCTR3            0x208     ///< Pci Configuration Control 3 Register
#define R_LPSS_PCR_PCICFGCTR4            0x20c     ///< Pci Configuration Control 4 Register
#define R_LPSS_PCR_PCICFGCTR5            0x210     ///< Pci Configuration Control 5 Register
#define R_LPSS_PCR_PCICFGCTR6            0x214     ///< Pci Configuration Control 6 Register
#define R_LPSS_PCR_PCICFGCTR7            0x218     ///< Pci Configuration Control 7 Register
#define R_LPSS_PCR_PCICFGCTR8            0x21c     ///< Pci Configuration Control 8 Register
#define R_LPSS_PCR_PCICFGCTR9            0x220     ///< Pci Configuration Control 9 Register
#define R_LPSS_PCR_PCICFGCTR10           0x224     ///< Pci Configuration Control 10 Register
#define R_LPSS_PCR_PCICFGCTR11           0x228     ///< Pci Configuration Control 11 Register
#define R_LPSS_PCR_PCICFGCTR12           0x22c     ///< Pci Configuration Control 12 Register
#define R_LPSS_PCR_PCICFGCTR13           0x230     ///< Pci Configuration Control 13 Register
#define R_LPSS_PCR_PCICFGCTR14           0x234     ///< Pci Configuration Control 14 Register
#define R_LPSS_PCR_PCICFGCTR15           0x238     ///< Pci Configuration Control 15 Register
#define R_LPSS_PCR_PCICFGCTR16           0x23C     ///< Pci Configuration Control 16 Register
#define R_LPSS_PCR_PCICFGCTR17           0x240     ///< Pci Configuration Control 17 Register
#define R_LPSS_PCR_PCICFGCTR18           0x244     ///< Pci Configuration Control 18 Register
#define R_LPSS_PCR_PCICFGCTR19           0x248     ///< Pci Configuration Control 19 Register
#define R_LPSS_PCR_PCICFGCTR20           0x24C     ///< Pci Configuration Control 20 Register
#define R_LPSS_PCR_PCICFGCTR21           0x250     ///< Pci Configuration Control 21 Register
#define R_LPSS_PCR_PCICFGCTR22           0x254     ///< Pci Configuration Control 22 Register

#endif // _LPSS_REGS_PCI_CFG_CTR_H_
