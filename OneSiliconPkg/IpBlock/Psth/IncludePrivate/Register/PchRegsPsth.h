/** @file
  Register definition for PSTH component

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
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PCH_REGS_PSTH_H_
#define _PCH_REGS_PSTH_H_

//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PSTH and IO Trap PCRs (PID:PSTH)
//
#define R_PSTH_PCR_PSTHCTL                      0x1D00                ///< PSTH control register
#define B_PSTH_PCR_PSTHCTL_PSTHIOSFPTCGE        BIT2                  ///< PSTH IOSF primary trunk clock gating enable
#define B_PSTH_PCR_PSTHCTL_PSTHIOSFSTCGE        BIT1                  ///< PSTH IOSF sideband trunk clock gating enable
#define B_PSTH_PCR_PSTHCTL_PSTHDCGE             BIT0                  ///< PSTH dynamic clock gating enable
#define R_PSTH_PCR_TRPSTS                       0x1E00                ///< Trap status regsiter
#define R_PSTH_PCR_TRPCYC1                      0x1E10                ///< Trapped cycle
#define B_PSTH_PCR_TRPCYC1_TRPRWR               BIT24                 ///< Read/Write#: 1=Read, 0=Write
#define B_PSTH_PCR_TRPCYC1_TRPBE                0xf0000               ///< Active high byte enables
#define B_PSTH_PCR_TRPCYC1_TRPADDR              0x000000000000FFFC    ///< Trap cycle I/O address
#define R_PSTH_PCR_TRPWRDATA1                   0x1E18                ///< Trapped write data
#define R_PSTH_PCR_IOTRP1_1                     0x1E80                ///< IO Trap Registers 1
#define R_PSTH_PCR_IOTRP2_1                     0x1E88                ///< IO Trap Registers 2
#define R_PSTH_PCR_IOTRP3_1                     0x1E90                ///< IO Trap Registers 3
#define R_PSTH_PCR_IOTRP4_1                     0x1E98                ///< IO Trap Registers 4
#define B_PSTH_PCR_IOTRP_TRPXRWM                BIT17                 ///< 49 - 32 for 32 bit access, Read/Write mask
#define B_PSTH_PCR_IOTRP_TRPXRW                 BIT16                 ///< 48 - 32 for 32 bit access, Read/Write#, 1=Read, 0=Write
#define N_PSTH_PCR_IOTRP_TRPXRW                 16                    ///< 48 - 32 for 32 bit access, 16bit shift for Read/Write field
#define N_PSTH_PCR_IOTRP_TRPXBEM                36
#define N_PSTH_PCR_IOTRP_TRPXBE                 32
#define B_PSTH_PCR_IOTRP_TRPXADDR               0x000000000000FFFC    ///< IO Address
#define B_PSTH_PCR_IOTRP_TRPXEN                 BIT0                  ///< Trap and SMI# Enable


#endif
