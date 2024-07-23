/** @file
  Register names for PCH TCO device

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

@par Specification
**/
#ifndef _TCO_REGS_H_
#define _TCO_REGS_H_

//
// TCO register I/O map
//
#define R_TCO_IO_TRLD                                0x0              ///< TCO_RLD Register
#define R_TCO_IO_TSTS1                               0x04             ///< TCO1_STS Register
#define S_TCO_IO_TSTS1_STS                           2
#define B_TCO_IO_TSTS1_CPUSERR_STS                   BIT12            ///< CPUSERR_STS
#define B_TCO_IO_TSTS1_CPUSMI_STS                    BIT10            ///< CPUSMI_STS
#define B_TCO_IO_TSTS1_CPUSCI_STS                    BIT9             ///< CPUSCI_STS
#define B_TCO_IO_TSTS1_BIOSWR_STS                    BIT8             ///< BIOSWR_STS
#define B_TCO_IO_TSTS1_NEWCENTURY_STS                BIT7             ///< NEWCENTURY_STS
#define B_TCO_IO_TSTS1_TIMEOUT                       BIT3             ///< TIMEOUT
#define B_TCO_IO_TSTS1_TCO_INT_STS                   BIT2             ///< TCO_INT_STS
#define B_TCO_IO_TSTS1_OS_TCO_SMI                    BIT1             ///< OS_TCO_SMI
#define N_TCO_IO_TSTS1_CPUSMI_STS                    10
#define N_TCO_IO_TSTS1_BIOSWR_STS                    8
#define N_TCO_IO_TSTS1_NEWCENTURY_STS                7
#define N_TCO_IO_TSTS1_TIMEOUT                       3
#define N_TCO_IO_TSTS1_OS_TCO_SMI                    1
#define N_TCO_IO_TSTS1_NMI2SMI_STS                   0

#define R_TCO_IO_TSTS2                               0x06             ///< TCO2_STS Register
#define S_TCO_IO_TSTS2_STS                           2
#define B_TCO_IO_TSTS2_NRSTRAP_STS                   BIT2             ///< NO_REBOOT_PIN_STRAP_STATUS
#define B_TCO_IO_TSTS2_SECOND_TO_STS                 BIT1             ///< SECOND_TO_STS
#define B_TCO_IO_TSTS2_INTRD_DET                     BIT0             ///< INTRD_DET
#define N_TCO_IO_TSTS2_INTRD_DET                     0

#define R_TCO_IO_TCTL1                               0x08             ///< TCO1_CNT Register
#define S_TCO_IO_TCTL1_CNT                           2
#define B_TCO_IO_TCTL1_TCO_LOCK                      BIT12            ///< TCO_LOCK
#define B_TCO_IO_TCTL1_TCO_TMR_HALT                  BIT11            ///< TCO_TMR_HALT
#define N_TCO_IO_TCTL1_NMI2SMI_EN                    9
#define B_TCO_IO_TCTL1_NR_MSUS                       BIT0             ///< NO_REBOOT_MSUS

#define R_TCO_IO_TCTL2                               0x0A             ///< TCO2_CNT Register
#define S_TCO_IO_TCTL2_CNT                           2
#define N_TCO_SMI_INTRD_SEL                          2

#define R_TCO_IO_TWDS                                0x0E             ///< TCO_WDSTATUS Register
#define R_TCO_IO_TTMR                                0x12             ///< TCO_TMR Register
#endif
