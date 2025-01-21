/** @file
  Register names for Serial IO Controllers

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
#ifndef _SERIAL_IO_REGS_H_
#define _SERIAL_IO_REGS_H_

//
// Serial IO Controllers PCI Configuration Registers
// registers accessed using PciD21FxRegBase + offset
//
#define R_SERIAL_IO_CFG_BAR                                0x10      //< Base Address Register
#define R_SERIAL_IO_CFG_BAR_HIGH                           0x14      //< Base Address Register High
#define R_SERIAL_IO_CFG_BAR1                               0x18      //< Base Address Register1
#define R_SERIAL_IO_CFG_BAR1_HIGH                          0x1C      //< Base Address Register1 High
#define R_SERIAL_IO_CFG_PMECTRLSTATUS                      0x84      //< Power Management Control And Status Register
#define R_SERIAL_IO_CFG_D0I3_MAX_POW_LAT_PG_CONFIG         0xA0      //< D0i3 And Power Control Enable Register

//
// Serial IO Controllers MMIO Registers
// registers accessed : BAR0 + offset
//
#define R_SERIAL_IO_MEM_RESETS                             0x204     //< REG RESETS

//
// Bridge Private Configuration Registers
// accessed only through SB messaging. SB access = SerialIo IOSF2OCP Bridge Port ID + offset
//
#define R_SERIAL_IO_PCR_PMCTL                               0x1D0    //< Power Management Control Register
#define V_SERIAL_IO_PCR_PMCTL_PWR_GATING                    0x3F

//
//  General Purpose
//
#define R_SERIAL_IO_PCR_GEN_REGRW2                          0x604    //< General Purpose Register2
#define B_SERIAL_IO_PCR_GPPRVRW2_PGCB_FRC_CLK_CP_EN         BIT1
#define B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CG_EN         BIT5
#define B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CLKREQ_CTL_EN BIT11
#define V_SERIAL_IO_PCR_GPPRVRW2_CLK_GATING                 (B_SERIAL_IO_PCR_GPPRVRW2_PGCB_FRC_CLK_CP_EN | B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CG_EN | B_SERIAL_IO_PCR_GPPRVRW2_CDC_SIDE_CFG_CLKREQ_CTL_EN)

#define R_SERIAL_IO_PCR_GEN_REGRW4                          0x60C    //< General Purpose Register4
#define B_SERIAL_IO_PCR_GPPRVRW4_I2C4_IO_CONTROL            BIT8
#define B_SERIAL_IO_PCR_GPPRVRW4_I3C_SLC_CLK_CGC            BIT16    // I3C Slice Clock Gate Control [0: Clock Gating Enabled, 1: Force Clock On]

#endif //_SERIAL_IO_REGS_H_
