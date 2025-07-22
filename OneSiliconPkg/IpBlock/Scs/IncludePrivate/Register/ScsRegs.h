/** @file
  Register names for PCH Storage and Communication Subsystem

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
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _SCS_REGS_H_
#define _SCS_REGS_H_

//
// SCS Devices proprietary PCI Config Space Registers
//
#define R_SCS_CFG_PCS                     0x84                          ///< PME Control Status
#define B_SCS_CFG_PCS_PS                  (BIT1 | BIT0)                 ///< Power State
#define B_SCS_CFG_PCS_PS_D3HOT            (BIT1 | BIT0)                 ///< Power State: D3Hot State
#define R_SCS_CFG_PG_CONFIG               0xA2                          ///< PG Config
#define B_SCS_CFG_PG_CONFIG_HAE           BIT5                          ///< HA Enable
#define B_SCS_CFG_PG_CONFIG_SE            BIT3                          ///< Sleep Enable
#define B_SCS_CFG_PG_CONFIG_PGE           BIT2                          ///< PG Enable
#define B_SCS_CFG_PG_CONFIG_I3E           BIT1                          ///< I3 Enable

#define R_SCS_UFS_PGA_FUSE_BIOS_DISABLE   0x3500
#define B_SCS_UFS_BIOS_DISABLE            BIT8
#endif
