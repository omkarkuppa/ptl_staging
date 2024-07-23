/** @file
  Register names for ITSS

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
#ifndef _ITSS_REGS_H_
#define _ITSS_REGS_H_

#define R_ITSS_IO_8259_MASK_INITIATOR     0x21
#define R_ITSS_IO_8259_MASK_TARGET        0xA1

//
// ITSS PCRs (PID:ITSS)
//

#define R_ITSS_PCR_PIR0                   0x3140          ///< PCI Interrupt Route 0
#define R_ITSS_PCR_PIR1                   0x3142          ///< PCI Interrupt Route 1
#define R_ITSS_PCR_PIR2                   0x3144          ///< PCI Interrupt Route 2
#define R_ITSS_PCR_PIR3                   0x3146          ///< PCI Interrupt Route 3
#define R_ITSS_PCR_PIR4                   0x3148          ///< PCI Interrupt Route 4
#define R_ITSS_PCR_PIR5                   0x314A          ///< PCI Interrupt Route 5
#define R_ITSS_PCR_PIR6                   0x314C          ///< PCI Interrupt Route 6
#define R_ITSS_PCR_PIR7                   0x314E          ///< PCI Interrupt Route 7
#define R_ITSS_PCR_PIR8                   0x3150          ///< PCI Interrupt Route 8
#define R_ITSS_PCR_PIR9                   0x3152          ///< PCI Interrupt Route 9
#define R_ITSS_PCR_PIR10                  0x3154          ///< PCI Interrupt Route 10
#define R_ITSS_PCR_PIR11                  0x3156          ///< PCI Interrupt Route 11
#define R_ITSS_PCR_PIR12                  0x3158          ///< PCI Interrupt Route 12

#define R_ITSS_PCR_MSGDC                  0x3120          ///< Message Decoder Control
#define V_ITSS_PCR_MSGDC_PIR13          (BIT0|BIT2|BIT3) ///< MSGDC[11:8]: Shift bits to use PIR13
#define R_ITSS_PCR_GIC                    0x31FC          ///< General Interrupt Control
#define B_ITSS_PCR_GIC_MAX_IRQ_24         BIT9            ///< Max IRQ entry size, 1 = 24 entry size, 0 = 120 entry size
#define B_ITSS_PCR_GIC_AME                BIT17           ///< Alternate Access Mode Enable
#define B_ITSS_PCR_GIC_SDPS               BIT16           ///< Shutdown Policy Select
#define N_ITSS_PCR_GIC_SDPS               16              ///< Shutdown Policy Select bit shift
#define V_ITSS_PCR_GIC_SDPS               1               ///< Shutdown Policy Select default value (PLTRST)
#define R_ITSS_PCR_IPC0                   0x3200          ///< Interrupt Polarity Control 0
#define R_ITSS_PCR_IPC1                   0x3204          ///< Interrupt Polarity Control 1
#define R_ITSS_PCR_IPC2                   0x3208          ///< Interrupt Polarity Control 2
#define R_ITSS_PCR_IPC3                   0x320C          ///< Interrupt Polarity Control 3
#define R_ITSS_PCR_ITSSPRC                0x3300          ///< ITSS Power Reduction Control
#define B_ITSS_PCR_ITSSPRC_PGCBDCGE       BIT4            ///< PGCB Dynamic Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_HPETDCGE       BIT3            ///< HPET Dynamic Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_CGE8254        BIT2            ///< 8254 Static Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_SBDCGE         BIT1            ///< IOSF-Sideband Interface Clock Gating Enable
#define B_ITSS_PCR_ITSSPRC_PCIDCGE        BIT0            ///< ITSS Clock Gate Enable


#define R_ITSS_PCR_UEV                    0x3324          ///< Uncorrectable Error Severity
#define R_ITSS_PCR_MMC                    0x3334          ///< Main Message Control
#define B_ITSS_PCR_MMC_MSTRMSG_EN         BIT0            ///< Main Message Enable

//
// ITSS I/O APIC Registers
//
#define R_ITSS_APIC_MEM_IDX               0x00000000U     ///< Index Register
#define R_ITSS_APIC_MEM_WDW               0x00000010U     ///< Window Register
#endif
