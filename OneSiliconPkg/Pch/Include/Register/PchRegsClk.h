/** @file
  Register names for iCLK device

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
#ifndef _PCH_REGS_ICLK_H_
#define _PCH_REGS_ICLK_H_

#define R_ICLK_PCR_CMU_ONE_DWORD25                      0x22E4
#define R_ICLK_PCR_CMU_ONE_DWORD26                      0x22E8
#define S_ICLK_PCR_MUXSEL_BITS_PER_FIELD                4      // CLKREQ number is encoded in 4 bits
#define B_ICLK_PCR_MUXSEL_BITS_PER_FIELD                0xF    // CLKREQ number is encoded in 4 bits
#define S_ICLK_PCR_MUXSEL_FIELDS_PER_REG                8      // each CMU_ONE_DWORD register contains bitfields for 8 CLK_SRC

#define R_ICLK_PCR_CAMERA1                              0x8000
#define B_ICLK_PCR_CAMERA1_CAM1_FREQ_SEL_1_0            (BIT0 | BIT1)
#define B_ICLK_PCR_CAMERA1_CAM1_CLK_EN                  BIT2
#define R_ICLK_PCR_BIOS_BUFFEN                          0x8080
#define N_ICLK_PCR_BIOS_BUFFEN_SRC0                     2
#define R_ICLK_PCR_BIOS_BUFFEN_H                        0x8084
#define N_ICLK_PCR_BIOS_BUFFEN_H_SRC7                   1
#define R_ICLK_PCR_SRC_MUXSEL                           0x8104
#define R_ICLK_PCR_SRC_MUXSEL_26                        0x8108
#define R_ICLK_PCR_SRC_MUXSEL_34                        0x810C
#define R_ICLK_PCR_SRC_MUXSEL_42                        0x8110
#define R_ICLK_PCR_SRC_MUXSEL_50                        0x8114
#define S_ICLK_PCR_SRC_MUXSEL_FIELDS_PER_REG            4
#define B_ICLK_PCR_SRC_MUXSEL_BITS_PER_FIELD            0x1F

#endif // _PCH_REGS_ICLK_H_
