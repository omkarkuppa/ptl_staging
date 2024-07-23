/** @file
  Register names for PCH Integrated Sensor Hub (ISH3.0)

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
#ifndef _ISH_REGS_H_
#define _ISH_REGS_H_

#define R_ISH_CFG_PMECTRLSTATUS               0x84             ///< Power Management Control And Status Register
#define B_ISH_CFG_PMECTRLSTATUS_POWERSTATE    ( BIT1 | BIT0 )  ///< Power State Field

#define R_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG               0xA0                      ///< D0i3 And Power Control Enable Register
#define B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_POW_LAT_VALUE 0x3FF                     ///< Power On Latency value
#define B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_POW_LAT_SCALE ( BIT12 | BIT11 | BIT10 ) ///< Power On Latency Scale
#define B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_HAE           BIT21

//
// ISH Private Configuration Space Registers (IOSF2OCP)
// (PID:ISH)
//
#define R_ISH_PCR_PMCTL                          0x1D0    ///< Power Management
#define B_ISH_PCR_PMCTL_IOSF_SB_LOCAL_GATE_EN    BIT5     ///< sb Local Gate Enable
#define B_ISH_PCR_PMCTL_IOSF_PRIM_LOCAL_GATE_EN  BIT4     ///< Primary Local Gate Enable
#define B_ISH_PCR_PMCTL_AXI_LOCAL_GATE_EN        BIT3     ///< AXI local clock gate enable
#define B_ISH_PCR_PMCTL_IOSF_PRIM_TRUNK_GATE_EN  BIT2     ///< IOSF Primary trunk gate enable
#define B_ISH_PCR_PMCTL_IOSF_SB_TRUNK_GATE_EN    BIT1     ///< Sideband clock gate enable
#define B_ISH_PCR_PMCTL_AXI_TRUNK_GATE_EN        BIT0     ///< AXI clock gate enable

#define R_ISH_PCR_PCICFGCTR1                     0x200    ///< PCI Configuration Control
#define B_ISH_PCR_PCICFGCTR1_DIS_MSI_CAP1        BIT29
#define B_ISH_PCR_PCICFGCTR1_PCI_IRQ1            ( BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 ) ///< Pci Irqn Field
#define N_ISH_PCR_PCICFGCTR1_PCI_IRQ1            20
#define B_ISH_PCR_PCICFGCTR1_ACPI_IRQ1           ( BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12 ) ///< Acpi Irqn Field
#define N_ISH_PCR_PCICFGCTR1_ACPI_IRQ1           12
#define B_ISH_PCR_PCICFGCTR1_IPIN1               (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_ISH_PCR_PCICFGCTR1_IPIN1               8
#define B_ISH_PCR_PCICFGCTR1_BAR1_DISABLE1       BIT7             ///< Bar1 Disable Field

#endif
