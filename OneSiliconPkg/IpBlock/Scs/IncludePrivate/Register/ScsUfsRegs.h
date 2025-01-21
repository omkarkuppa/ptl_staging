/** @file
  Register definitions for UFS controllers on SCS.

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

#ifndef _SCS_UFS_REGS_H_
#define _SCS_UFS_REGS_H_
//
// UFS Mmio Registers
//
#define R_UFS_MEM_REF_CLK                                0x85C                      ///< Ref Clock Register
#define V_UFS_MEM_REF_CLK                                0x3

//
// MMP_UFS registers
//
#define R_SCS_PCR_MMPUFS_DLANE1                          0x2000

#define R_SCS_PCR_MMPUFS_DLANEX_TX3_SYNC_LEN             0x28
#define R_SCS_PCR_MMPUFS_DLANEX_IMP8                     0x101C
#define R_SCS_PCR_MMPUFS_DLANEX_IMP8_PWRGT               0x101F
#define R_SCS_PCR_MMPUFS_DLANEX_IMP23_INTFILTER          0x105B
#define R_SCS_PCR_MMPUFS_DLANEX_IMP22                    0x1054
#define B_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            (BIT26 | BIT25 | BIT24)
#define N_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            24
#define V_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            0x7

#endif
