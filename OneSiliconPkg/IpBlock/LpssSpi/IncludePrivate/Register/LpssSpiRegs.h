/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _LPSS_SPI_REGS_H_
#define _LPSS_SPI_REGS_H_

#define R_LPSS_SPI_CFG_BAR                                0x10      //< Base Address Register
#define R_LPSS_SPI_CFG_BAR_HIGH                           0x14      //< Base Address Register High
#define R_LPSS_SPI_CFG_BAR1                               0x18      //< Base Address Register1
#define R_LPSS_SPI_CFG_BAR1_HIGH                          0x1C      //< Base Address Register1 High

#define R_LPSS_SPI_CFG_PMECTRLSTATUS                      0x84      //< Power Management Control And Status Register
#define B_LPSS_SPI_CFG_PMECTRLSTATUS_POWERSTATE           ( BIT1 | BIT0 ) ///< Power State Field

#define R_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG         0xA0      //< D0i3 And Power Control Enable Register
#define B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE   BIT16     //< Pmc Request Enable Field
#define B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN BIT17    //< Device Idle En Field
#define B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN   BIT18     //< D3 Hen Field

//
// LPSS SPI MMIO Registers
//
#define R_LPSS_SPI_MEM_RESETS                             0x204     //< REG RESETS
#define B_LPSS_SPI_MEM_RESETS_RESET_IP                    ( BIT1 | BIT0 ) //< reset_ip
#define B_LPSS_SPI_MEM_RESETS_RESET_DMA                   BIT2      //< reset_dma

#define R_LPSS_SPI_MEM_SSCR0                              0x00      //< REG SSCR0
#define V_LPSS_MEM_SPI_SSCR0_DSS_8                        0x07
#define R_LPSS_SPI_MEM_SSCR1                              0x04      //< REG SSCR1
#define R_LPSS_SPI_MEM_SSSR                               0x08      //< REG SSSR
#define R_LPSS_SPI_MEM_SSITR                              0x0C      //< REG SSITR
#define R_LPSS_SPI_MEM_SSDR                               0x10      //< REG SSDR
#define R_LPSS_SPI_MEM_SIRF                               0x48      //< REG SIRF

//
//  LPSS SPI MMIO Convergence Layer
//
#define R_LPSS_SPI_MEM_CLOCKS                             0x200      //< REG CLOCKS
#define R_LPSS_SPI_MEM_SPI_CS_CONTROL                     0x224      //< REG SPI_CS_CONTROL
#define N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY        12
#define N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS1_POLARITY        13
#define N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS2_POLARITY        14
#define N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS3_POLARITY        15

//
// LPSS Private Configuration Registers
//
#define R_LPSS_SPI_PCR_PCICFGCTR1                         0x200    //< PCI Configuration Control 1 Register
#define B_LPSS_SPI_PCR_PCICFGCTR1_PCI_CFG_DIS1            BIT0     //< Pcicfgctr1  Pci Cfg Disable Field
#define B_LPSS_SPI_PCR_PCICFGCTR1_ACPI_INTR_EN1           BIT1     //< Pcicfgctr1  Acpi Interrupt En Field
#define B_LPSS_SPI_PCR_PCICFGCTR1_BAR1_DISABLE1           BIT7     //< Pcicfgctr1  Bar Disable Field
#define B_LPSS_SPI_PCR_PCICFGCTR1_IPIN1                   ( BIT11 | BIT10 | BIT9 | BIT8 ) //< Pcicfgctr1  Interrupt Pin Field
#define N_LPSS_SPI_PCR_PCICFGCTR1_IPIN1                   8
#define B_LPSS_SPI_PCR_PCICFGCTR1_ACPI_IRQ1               ( BIT19 | BIT18 | BIT17 | BIT16 | BIT15 | BIT14 | BIT13 | BIT12 ) //< Pcicfgctr1  Acpi Irq Field
#define B_LPSS_SPI_PCR_PCICFGCTR1_PCI_IRQ1                ( BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 ) //< Pcicfgctr1  Pci Irq Num Field
#define N_LPSS_SPI_PCR_PCICFGCTR1_PCI_IRQ1                20

/**
  0x224 ChipSelect Control - R_LPSS_MEM_SPI_CS_CONTROL
**/
typedef union {
  struct {
    /**
    SPI Chip Select Mode Section.
    0 = HW Mode - CS is under SSP control,
    1 = SW Mode - CS is under SW Control using CsState bit
    **/
    UINT32  CsMode         : 1;    //[RW][0]
    /**
    Manual SW control of SPI Chip Select (CS)
    The state of this bit 0=>CS asserted; 1 => CS de-asserted. The
    state of the CS pin also includes the polarity setting in bits 15:12 of this register.
    **/
    UINT32  CsState        : 1;    //[RW][1]
    UINT32  Reserved0      : 6;    //[N/A][2:7]
    /**
    These Bits select which SPI CS Signal is to be driven by the SSP Frame (CS).
    The steering logic will ensure that when switching to another active SPI CS port
    the inactive port will be forced to the inactive polarity defined by the
    corresponding SPI CS Polarity Bit
    **/
    UINT32  CsOutputSelect : 2;    //[RW][8:9]
    UINT32  Reserved1      : 2;    //[N/A][10:11]
    /**
    Sets Inactive/Idle polarity of CS0.
    When switching to another active SPI CS port the inactive port
    will be forced to the inactive polarity 0 = Low, 1 = High
    **/
    UINT32  Cs0Polarity    : 1;    //[RW][12]
    /**
    Sets Inactive/Idle polarity of CS1.
    When switching to another active SPI CS port the inactive port
    will be forced to the inactive polarity 0 = Low, 1 = High
    **/
    UINT32  Cs1Polarity    : 1;    //[RW][13]
    /**
    Sets Inactive/Idle polarity of CS2.
    When switching to another active SPI CS port the inactive port
    will be forced to the inactive polarity 0 = Low, 1 = High
    **/
    UINT32  Cs2Polarity    : 1;    //[RW][14]
    /**
    Sets Inactive/Idle polarity of CS3.
    When switching to another active SPI CS port the inactive port
    will be forced to the inactive polarity 0 = Low, 1 = High
    **/
    UINT32  Cs3Polarity    : 1;    //[RW][15]
    UINT32  Reserved2      : 15;   //[N/A][16:31]
  } Fields;
  UINT32 Data32;
} LPSS_SPI_CS_CONTROL_REG;

#endif // _LPSS_SPI_REGS_H_
