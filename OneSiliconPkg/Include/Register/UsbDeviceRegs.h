/** @file
  Register defines for USB device controller

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

@par Specification Reference:
**/
#ifndef _USB_DEVICE_REGS_H_
#define _USB_DEVICE_REGS_H_

//
// xDCI (OTG) PCI Config Space Registers
//
#define R_XDCI_CFG_PMECTRLSTATUS               0x84     ///< Power Management Control and Status Register
#define R_XDCI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG  0xA0     ///< Chassis Power Gate Enable

//
// xDCI (OTG) MMIO registers
//
#define R_XDCI_MEM_GCTL                          0xC110 ///< Xdci Global Ctrl
#define B_XDCI_MEM_GCTL_GBLHIBERNATIONEN         BIT1   ///< Hibernation enable
#define R_XDCI_MEM_GUSB2PHYCFG_0                 0xC200 ///< Global USB2 PHY Configuration Register
#define B_XDCI_MEM_GUSB2PHYCFG_0_SUSPENDUSB20    BIT6   ///< Suspend USB2.0 HS/FS/LS PHY
#define R_XDCI_MEM_GUSB3PIPECTL_0                0xC2C0 ///< Global USB3 PIPE Control Register 0
#define B_XDCI_MEM_GUSB3PIPECTL_0_SUSPENDENABLE  BIT17  ///< Suspend USB3.0 SS PHY (Suspend_en)
#define R_XDCI_MEM_APBFC_U3PMU_CFG2              0x10F810
#define R_XDCI_MEM_APBFC_U3PMU_CFG4              0x10F818
#define R_XDCI_MEM_APBFC_U3PMU_CFG5              0x10F81C
#define R_XDCI_MEM_APBFC_U3PMU_CFG6              0x10F820
#define B_XDCI_MEM_APBFC_U3PMU_CFG6_RXSBCTRLEN   BIT13
#define B_XDCI_MEM_APBFC_U3PMU_CFG6_ENLRIP2      BIT4

//
// xDCI (OTG) Private Configuration Registers
// (PID:OTG)
// @todo: Verify PCR vaLidity for CPU
//
#define R_XDCI_PCR_PMCTL                     0x1D0
#define R_XDCI_PCR_PCICFGCTR1                0x200
#define B_XDCI_PCR_PCICFGCTR1_PCI_IRQ1       (BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20)
#define N_XDCI_PCR_PCICFGCTR1_PCI_IRQ1       20
#define B_XDCI_PCR_PCICFGCTR1_ACPI_IRQ1      (BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12)
#define N_XDCI_PCR_PCICFGCTR1_ACPI_IRQ1      12
#define B_XDCI_PCR_PCICFGCTR1_IPIN1          (BIT11 | BIT10 | BIT9 | BIT8)
#define N_XDCI_PCR_PCICFGCTR1_IPIN1          8
#define B_XDCI_PCR_PCICFGCTR1_ACPI_INTR_EN1  BIT1

#endif
