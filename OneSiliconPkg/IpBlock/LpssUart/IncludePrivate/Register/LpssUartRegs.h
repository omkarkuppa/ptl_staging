/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _LPSS_UART_REGS_H_
#define _LPSS_UART_REGS_H_

//
// LPSS UART Controllers PCI Configuration Registers
//
#define R_LPSS_UART_CFG_BAR                                0x10      //< Base Address Register
#define R_LPSS_UART_CFG_BAR_HIGH                           0x14      //< Base Address Register High
#define R_LPSS_UART_CFG_BAR1                               0x18      //< Base Address Register1
#define R_LPSS_UART_CFG_BAR1_HIGH                          0x1C      //< Base Address Register1 High

#define R_LPSS_UART_CFG_PMECTRLSTATUS                      0x84      //< Power Management Control And Status Register

#define R_LPSS_UART_CFG_D0I3_MAX_POW_LAT_PG_CONFIG         0xA0      //< D0i3 And Power Control Enable Register
#define B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE   BIT16     //< Pmc Request Enable Field
#define B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN BIT17    //< Device Idle En Field
#define B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN   BIT18     //< D3 Hen Field

//
// LPSS UART MMIO
//
#define R_LPSS_UART_MEM_RBR                                0x00      //< REG RBR Receive Buffer Register.
#define R_LPSS_UART_MEM_THR                                0x00      //< REG THR Transmit Holding Register
#define R_LPSS_UART_MEM_DLL                                0x00      //< REG DLL Divisor Latch (Low) Register.
#define R_LPSS_UART_MEM_DLH                                0x04      //< REG DLH Divisor Latch (High) Register.
#define R_LPSS_UART_MEM_FCR                                0x08      //< REG FCR FIFO Control Register.
#define R_LPSS_UART_MEM_IIR                                0x08      //< REG IIR Interrupt Control Register.
#define R_LPSS_UART_MEM_LCR                                0x0C      //< REG LCR Line Control Register.
#define R_LPSS_UART_MEM_MCR                                0x10      //< REG MCR Modem Control Register.
#define R_LPSS_UART_MEM_LSR                                0x14      //< REG LSR Line Status Register
#define R_LPSS_UART_MEM_USR                                0x7C      //< REG USR UART Status Register.
#define R_LPSS_UART_MEM_CTR                                0xFC      //< REG CTR Component Type Register.
#define UART_COMPONENT_IDENTIFICATION_CODE                 0x44570110

//
// LPSS UART MMIO Convergance Layer
//
#define R_LPSS_UART_MEM_RESETS                             0x204      //< REG RESETS
#define B_LPSS_UART_MEM_RESETS_RESET_IP                    ( BIT1 | BIT0 ) ///< reset_ip
#define B_LPSS_UART_MEM_RESETS_RESET_DMA                    BIT2      //< reset_dma

#define R_LPSS_UART_MEM_CLOCKS                             0x200      //< REG CLOCKS Private Clock Configuraton
#define V_LPSS_UART_MEM_PPR_CLK_M_DIV                      0x25A
#define V_LPSS_UART_MEM_PPR_CLK_N_DIV                      0x7FFF

//
//  General Purpose
//
#define R_LPSS_UART_PCR_GEN_PVT_LOW_REGRW7                 0x618      //< General Purpose Register7
#define V_LPSS_UART_PCR_GEN_PVT_LOW_REGRW7_UART0_BYTE_ADDR_EN BIT0

#define R_LPSS_UART_CFG_PMECTRLSTATUS                      0x84
#define B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE           (BIT1| BIT0)

//
// LPSS UART Private Configuration Registsers (PCR)
//
#define R_LPSS_UART_PCR_PCICFGCTR1                         0x200    //< PCI Configuration Control 1 Register
#define B_LPSS_UART_PCR_PCICFGCTR1_PCI_CFG_DIS1            BIT0     //< Pcicfgctr1  Pci Cfg Disable Field
#define B_LPSS_UART_PCR_PCICFGCTR1_ACPI_INTR_EN1           BIT1     //< Pcicfgctr1  Acpi Interrupt En Field
#define B_LPSS_UART_PCR_PCICFGCTR1_BAR1_DISABLE1           BIT7     //< Pcicfgctr1  Bar Disable Field
#define B_LPSS_UART_PCR_PCICFGCTR1_IPIN1                   ( BIT11 | BIT10 | BIT9 | BIT8 ) //< Pcicfgctr1  Interrupt Pin Field
#define N_LPSS_UART_PCR_PCICFGCTR1_IPIN1                   8
#define B_LPSS_UART_PCR_PCICFGCTR1_ACPI_IRQ1               ( BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12 ) //< Pcicfgctr1  Acpi Irq Field
#define N_LPSS_UART_PCR_PCICFGCTR1_ACPI_IRQ1               12
#define B_LPSS_UART_PCR_PCICFGCTR1_PCI_IRQ1                ( BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 ) //< Pcicfgctr1  Pci Irq Num Field
#define N_LPSS_UART_PCR_PCICFGCTR1_PCI_IRQ1                20

#endif
