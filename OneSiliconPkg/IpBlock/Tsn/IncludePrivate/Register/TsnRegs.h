/** @file
  Register names for PCH TSN

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
#ifndef _TSN_REGS_H_
#define _TSN_REGS_H_

//
// TSN Controller Registers
//

// TSN PCI Configuration Space Registers
#define R_TSN_CFG_BAR0_LOW                    0x10
#define R_TSN_CFG_BAR0_HIGH                   0x14
#define R_TSN_CFG_BAR1_LOW                    0x18
#define R_TSN_CFG_BAR1_HIGH                   0x1C
#define B_TSN_CFG_BAR0_MASK                   0x0FFF
#define V_TSN_CFG_BAR0_SIZE                   0x2000
#define N_TSN_CFG_BAR0_ALIGNMENT              21

// TSN MEM Space Registers Offset
#define R_TSN_MEM_MAC_MDIO_ADDRESS           0x200        // MAC MDIO Address
#define R_TSN_MEM_MAC_MDIO_DATA              0x204        // MAC MDIO Data
#define R_TSN_MEM_MAC_ADDRESS0_HIGH          0x300        // MAC Address 0 High
#define R_TSN_MEM_MAC_ADDRESS0_LOW           0x304        // MAC Address 0 Low
#define B_TSN_MEM_ADDRESS_ENABLE             0x80000000
#define B_TSN_MEM_MAC_ADDRESS_HIGH_MASK      0xFFFF

#define B_TSN_MEM_MAC_MDIO_ADDRESS_MASK            B_TSN_MEM_MAC_MDIO_PHYAD | B_TSN_MEM_MAC_MDIO_REGAD  | B_TSN_MEM_MAC_MDIO_CLK_TRAIL | B_TSN_MEM_MAC_MDIO_CLK_CSR | B_TSN_MEM_MAC_MDIO_GMII_GOC_1_0 | B_TSN_MEM_MAC_MDIO_CLAUSE45_PHY // MAC MDIO Mask related for Link Mode speed setup
#define B_TSN_MEM_MAC_MDIO_REGAD                   0x001F0000   // MAC MDIO 5-bit PHY Data Register Address (REGAD)[20:16] for IEEE 802.3 Standard Caluse 22
#define N_TSN_MEM_MAC_MDIO_REGAD                   0x10         // MAC MDIO 5-bit PHY Data Register Address (REGAD)[20:16] for IEEE 802.3 Standard Caluse 22
#define B_TSN_MEM_MAC_MDIO_PHYAD                   0x03E00000   // MAC MDIO 5-bit PHY Address (PHYAD)[25:21] for IEE 802.3 Standard Clause 22
#define N_TSN_MEM_MAC_MDIO_PHYAD                   0x15         // MAC MDIO 5-bit PHY Address (PHYAD)[25:21] for IEE 802.3 Standard Clause 22
#define V_TSN_MEM_MAC_MDIO_ADHOC_GCR_ADD           0x15         // MAC MDIO adhoc Global Configuration REgister (GCR) Address PHYAD 15h at bit [25:21] of  R_TSN_MEM_MAC_MDIO_ADDRESS

#define B_TSN_MEM_MAC_MDIO_CLK_TRAIL               0x7000       // MAC MDIO Number of Trailing CLocks [14:12]
#define N_TSN_MEM_MAC_MDIO_CLK_TRAIL               0xC          // MAC MDIO Number of Trailing CLocks [14:12]
#define V_TSN_MEM_MAC_MDIO_CLK_TRAIL               0x4          // MAC MDIO Number of Trailing CLocks [14:12] set to 4
#define B_TSN_MEM_MAC_MDIO_CLK_CSR                 0xF00        // MAC CSR Clock Range (CR) [11:8]
#define N_TSN_MEM_MAC_MDIO_CLK_CSR                 0x8          // MAC CSR Clock Range (CR) [11:8]
#define V_TSN_MEM_MAC_MDIO_CLK_CSR_DIV_10          0xB          // MAC CSR Clock Range (CR) CSR Clock / 10

#define V_TSN_MEM_MAC_MDIO_GMII_22_PHY_READ        0x0C         // MAC GMII_GOC_1 (Bit3) and GMII_GOC_1 (Bit2) are 1
#define V_TSN_MEM_MAC_MDIO_GMII_22_PHY_WRITE       0x04         // MAC GMII_GOC_1 (Bit3) is 0 and GMII_GOC_1 (Bit2) is 1
#define B_TSN_MEM_MAC_MDIO_GMII_GOC_1_0            BIT3 | BIT2

#define B_TSN_MEM_MAC_MDIO_CLAUSE45_PHY            BIT1         // IEEE 802.3 Clause 45 Phy Enable bit
#define B_TSN_MEM_MAC_MDIO_GMII_BUSY               BIT0         // MAC GMII Busy Bit

#define B_TSN_MEM_MAC_ADHOC_PHY_AUTONEG_DISABLE    BIT0         // MAC PHY Auto-Negotiation Disable bit. Set 1 to Disable auto negotioation. Set 0 Auto.
#define B_TSN_MEM_MAC_ADHOC_LINK_MODE              0x6
#define V_TSN_MEM_MAC_ADHOC_LINK_MODE_2_5G         0x6
#define V_TSN_MEM_MAC_ADHOC_LINK_MODE_1G           0x4

// TSN Side Band AXI/TC mapping registers
// VC/TC mapping default data
/*
        Tx         |         Rx
---------------------------------------
TC1: Tx DMA Ch#7-4 | TC1: Rx DMA Ch#7-4
TC0: Tx DMA Ch#0-3 | TC0: Rx DMA Ch#0-3
*/
#define R_TSN_PCR_AXIID_TO_TC0_MAP_0                                 0x400
#define B_TSN_PCR_AXIID_TO_TC0_MAP_0                                 0x0000FFFF
#define V_TSN_PCR_AXIID_TO_TC0_MAP_0                                 0x000000FF
#define R_TSN_PCR_AXIID_TO_TC1_MAP_0                                 0x410
#define B_TSN_PCR_AXIID_TO_TC1_MAP_0                                 0x0000FFFF
#define V_TSN_PCR_AXIID_TO_TC1_MAP_0                                 0x0000FF00

// TSN Side Band MSI/TC mapping registers
// MSI VC/TC mapping default data
/*
      DMA MSI       |     Non-DMA MSI
---------------------------------------------
TC0: MSI vector 0-7  | TC0: MSI vector 26-31
TC1: MSI vector 8-15 |
*/
#define R_TSN_PCR_MSI_TO_TC0_MAP                                     0x500
#define B_TSN_PCR_MSI_TO_TC0_MAP                                     0xFC00FFFF
#define V_TSN_PCR_MSI_TO_TC0_MAP                                     0xFC0000FF
#define R_TSN_PCR_MSI_TO_TC1_MAP                                     0x504
#define B_TSN_PCR_MSI_TO_TC1_MAP                                     0xFC00FFFF
#define V_TSN_PCR_MSI_TO_TC1_MAP                                     0x0000FF00

//
// TSN Iosf2Ocp bridge configuration registers
//
#define R_TSN_PCR_PCICFGCTRL                 0x200                         ///< PCI Configuration Control
#define B_TSN_PCR_PCICFGCTR_PCI_IRQ          0x0FF00000                    ///< PCI IRQ number
#define N_TSN_PCR_PCICFGCTR_PCI_IRQ          20
#define B_TSN_PCR_PCICFGCTR_ACPI_IRQ         0x000FF000                    ///< ACPI IRQ number
#define N_TSN_PCR_PCICFGCTR_ACPI_IRQ         12
#define B_TSN_PCR_PCICFGCTR_IPIN1            (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_TSN_PCR_PCICFGCTR_IPIN1            8

//
//  TSN Link Speed PLL Registers
//

#define R_MODPHY1_PCR_LCPLL_DWORD0                                      0x8180
#define B_MODPHY1_PCR_LCPLL_DWORD0                                      0x7FFFFFFF
#define V_MODPHY1_PCR_LCPLL_DWORD0_CLK24MHZ_2_5G                        0x4D555582
#define V_MODPHY1_PCR_LCPLL_DWORD0_CLK24MHZ_1G                          0x4AAAAA68
#define V_MODPHY1_PCR_LCPLL_DWORD0_CLK38_4MHZ_2_5G                      0x58555551
#define V_MODPHY1_PCR_LCPLL_DWORD0_CLK38_4MHZ_1G                        0x46AAAA41

#define R_MODPHY1_PCR_LCPLL_DWORD2                                      0x8188
#define B_MODPHY1_PCR_LCPLL_DWORD2                                      0x003F05FF
#define V_MODPHY1_PCR_LCPLL_DWORD2_CLK24MHZ_2_5G                        0x0000012D
#define V_MODPHY1_PCR_LCPLL_DWORD2_CLK24MHZ_1G                          0x00000139
#define V_MODPHY1_PCR_LCPLL_DWORD2_CLK38_4MHZ_2_5G                      0x0000012D
#define V_MODPHY1_PCR_LCPLL_DWORD2_CLK38_4MHZ_1G                        0x00000139

#define R_MODPHY1_PCR_LCPLL_DWORD4                                      0x8190
#define B_MODPHY1_PCR_LCPLL_DWORD4                                      0x07071F0F
#define V_MODPHY1_PCR_LCPLL_DWORD4_COMMON                               0x02010804
//#define V_MODPHY1_PCR_LCPLL_DWORD4_CLK24MHZ_2_5G                        0x02010804
//#define V_MODPHY1_PCR_LCPLL_DWORD4_CLK24MHZ_1G                          0x02010804
//#define V_MODPHY1_PCR_LCPLL_DWORD4_CLK38_4MHZ_2_5G                      0x02010804
//#define V_MODPHY1_PCR_LCPLL_DWORD4_CLK38_4MHZ_1G                        0x02010804

#define R_MODPHY1_PCR_LCPLL_DWORD6                                     0x8198
#define B_MODPHY1_PCR_LCPLL_DWORD6                                     0x00020000
#define V_MODPHY1_PCR_LCPLL_DWORD6_COMMON                              0x00020000
//#define V_MODPHY1_PCR_LCPLL_DWORD6_CLK24MHZ_2_5G                       0x00020000
//#define V_MODPHY1_PCR_LCPLL_DWORD6_CLK24MHZ_1G                         0x00020000
//#define V_MODPHY1_PCR_LCPLL_DWORD6_CLK38_4MHZ_2_5G                     0x00020000
//#define V_MODPHY1_PCR_LCPLL_DWORD6_CLK38_4MHZ_1G                       0x00020000

#define R_MODPHY1_PCR_LCPLL_DWORD7                                    0x819C
#define B_MODPHY1_PCR_LCPLL_DWORD7                                    0x003F0003
#define V_MODPHY1_PCR_LCPLL_DWORD7_CLK24MHZ_2_5G                      0x001F0003
#define V_MODPHY1_PCR_LCPLL_DWORD7_CLK24MHZ_1G                        0x002A0003
#define V_MODPHY1_PCR_LCPLL_DWORD7_CLK38_4MHZ_2_5G                    0x001F0003
#define V_MODPHY1_PCR_LCPLL_DWORD7_CLK38_4MHZ_1G                      0x002A0003

#define R_MODPHY1_PCR_LCPLL_DWORD8                                    0x81A0
#define B_MODPHY1_PCR_LCPLL_DWORD8                                    0x00FF0307
#define V_MODPHY1_PCR_LCPLL_DWORD8_COMMON                             0x00000000
//#define V_MODPHY1_PCR_LCPLL_DWORD8_CLK24MHZ_2_5G                      0x00000000
//#define V_MODPHY1_PCR_LCPLL_DWORD8_CLK24MHZ_1G                        0x00000000
//#define V_MODPHY1_PCR_LCPLL_DWORD8_CLK38_4MHZ_2_5G                    0x00000000
//#define V_MODPHY1_PCR_LCPLL_DWORD8_CLK38_4MHZ_1G                      0x00000000

#define R_MODPHY1_PCR_LPPLL_DWORD10                                  0x81A8
#define B_MODPHY1_PCR_LPPLL_DWORD10                                  0x003800FF
#define V_MODPHY1_PCR_LPPLL_DWORD10_CLK24MHZ_2_5G                    0x0008000E
#define V_MODPHY1_PCR_LPPLL_DWORD10_CLK24MHZ_1G                      0x0008000E
#define V_MODPHY1_PCR_LPPLL_DWORD10_CLK38_4MHZ_2_5G                  0x00100008
#define V_MODPHY1_PCR_LPPLL_DWORD10_CLK38_4MHZ_1G                    0x00100008

#define R_MODPHY1_PCR_LCPLL_DWORD11                                  0x81AC
#define B_MODPHY1_PCR_LCPLL_DWORD11                                  0x00000201
#define V_MODPHY1_PCR_LCPLL_DWORD11_COMMON                           0x00000200

#define R_MODPHY1_PCR_LCPLL_DWORD12                                  0x81B0
#define B_MODPHY1_PCR_LCPLL_DWORD12                                  0x08003C00
#define V_MODPHY1_PCR_LCPLL_DWORD12_COMMON                           0x08003C00

#define R_MODPHY1_PCR_LCPLL_DWORD18                                  0x81C8
#define B_MODPHY1_PCR_LCPLL_DWORD18                                  0x000000FF
#define V_MODPHY1_PCR_LCPLL_DWORD18_COMMON                           0x00000001

#define R_MODPHY1_PCR_LCPLL_DWORD19                                  0x81CC
#define B_MODPHY1_PCR_LCPLL_DWORD19                                  0x000000FF
#define V_MODPHY1_PCR_LCPLL_DWORD19_COMMON                           0x00000001

#define R_MODPHY1_PCR_CMN_DIG_DWORD12                                0x8030
#define B_MODPHY1_PCR_CMN_DIG_DWORD12                                0x00000001
#define V_MODPHY1_PCR_CMN_DIG_DWORD12_COMMON                         0x00000001

#define R_MODPHY1_PCR_CMN_ANA_DWORD30                                0x8138
#define B_MODPHY1_PCR_CMN_ANA_DWORD30                                0xFF007800
#define V_MODPHY1_PCR_CMN_ANA_DWORD30_CLK24MHZ_2_5G                  0x00002800
#define V_MODPHY1_PCR_CMN_ANA_DWORD30_CLK24MHZ_1G                    0x82005000
#define V_MODPHY1_PCR_CMN_ANA_DWORD30_CLK38_4MHZ_2_5G                0x00002800
#define V_MODPHY1_PCR_CMN_ANA_DWORD30_CLK38_4MHZ_1G                  0x82005000

#define R_MODPHY1_PCR_CMN_DIG_DWORD15                                0x803C
#define B_MODPHY1_PCR_CMN_DIG_DWORD15                                0x00000006
#define V_MODPHY1_PCR_CMN_DIG_DWORD15_COMMON                         0x00000006

#define R_MODPHY1_PCR_CMN_ANA_DWORD10                                0x80E8
#define B_MODPHY1_PCR_CMN_ANA_DWORD10                                0x00FF0000
#define V_MODPHY1_PCR_CMN_ANA_DWORD10_CLK24MHZ_2_5G                  0x00030000
#define V_MODPHY1_PCR_CMN_ANA_DWORD10_CLK24MHZ_1G                    0x00020000
#define V_MODPHY1_PCR_CMN_ANA_DWORD10_CLK38_4MHZ_2_5G                0x00030000
#define V_MODPHY1_PCR_CMN_ANA_DWORD10_CLK38_4MHZ_1G                  0x00020000

#define R_MODPHY1_DLANE6_PCR_SET3                                   0x804
#define R_MODPHY1_DLANE7_PCR_SET3                                   0xC04
#define B_MODPHY1_PCR_SET3                                          0x00200100
#define V_MODPHY1_PCR_COMMON                                        0x00200100

#define R_MODPHY1_DLANE6_PCR_DWORD6                                 0x818
#define R_MODPHY1_DLANE7_PCR_DWORD6                                 0xC18
#define B_MODPHY1_PCR_DWORD6                                        0xE0000000
#define V_MODPHY1_PCR_DWORD6                                        0x80000000

#define R_MODPHY1_DLANE6_PCR_2TAP_DEEMPH3P5_RATE1                   0x954
#define R_MODPHY1_DLANE7_PCR_2TAP_DEEMPH3P5_RATE1                   0xD54
#define B_MODPHY1_PCR_2TAP_DEEMPH3P5_RATE1                          0x003f0004
#define V_MODPHY1_PCR_2TAP_DEEMPH3P5_RATE1_COMMON                   0x002d0004

#define R_MODPHY1_DLANE6_PCR_2TAP_DEEMPH3P5_RATE0                   0x958
#define R_MODPHY1_DLANE7_PCR_2TAP_DEEMPH3P5_RATE0                   0xD58
#define B_MODPHY1_PCR_2TAP_DEEMPH3P5_RATE0                          0x00003F00
#define V_MODPHY1_PCR_2TAP_DEEMPH3P5_RATE0_COMMON                   0x00002800

#define R_MODPHY1_DLANE6_PCR_RXPICTRL1                              0xA00
#define R_MODPHY1_DLANE7_PCR_RXPICTRL1                              0xE00
#define B_MODPHY1_PCR_RXPICTRL1                                     0x0000FC00
#define V_MODPHY1_PCR_RXPICTRL1_COMMON                              0x00002800

#define R_MODPHY1_DLANE6_PCR_RSVD4                                  0xA04
#define R_MODPHY1_DLANE7_PCR_RSVD4                                  0xE04
#define B_MODPHY1_PCR_RSVD4                                         0x00003F80
#define V_MODPHY1_PCR_RSVD4_COMMON                                  0x00000000

#define R_MODPHY1_DLANE6_PCR_RSVD9                                  0xA08
#define R_MODPHY1_DLANE7_PCR_RSVD9                                  0xE08
#define B_MODPHY1_PCR_RSVD9                                         0xFF000010
#define V_MODPHY1_PCR_RSVD9_COMMON                                  0x21000010

#define R_MODPHY1_DLANE6_PCR_RSVD19                                 0xA18
#define R_MODPHY1_DLANE7_PCR_RSVD19                                 0xE18
#define B_MODPHY1_PCR_RSVD19                                        0x00004000
#define V_MODPHY1_PCR_RSVD19_COMMON                                 0x00000000

#define R_MODPHY1_DLANE6_PCR_REG1C                                  0xA1C
#define R_MODPHY1_DLANE7_PCR_REG1C                                  0xE1C
#define B_MODPHY1_PCR_REG1C                                         0x000000F0
#define V_MODPHY1_PCR_REG1C_COMMON                                  0x00000050

#define R_MODPHY1_DLANE6_PCR_REG21                                  0xA20
#define R_MODPHY1_DLANE7_PCR_REG21                                  0xE20
#define B_MODPHY1_PCR_REG21                                         0x00000800
#define V_MODPHY1_PCR_REG21_COMMON                                  0x00000800

#define R_MODPHY1_DLANE6_PCR_RXUPIFCFGGHALFRATE_0                   0xA44
#define R_MODPHY1_DLANE7_PCR_RXUPIFCFGGHALFRATE_0                   0xE44
#define B_MODPHY1_PCR_RXUPIFCFGGHALFRATE_0                          0x00FFFFFF
#define V_MODPHY1_PCR_RXUPIFCFGGHALFRATE_0_COMMON                   0x00001548

#define R_MODPHY1_DLANE6_PCR_RXUPPFCFGGQUARTERRATE_0                0xA48
#define R_MODPHY1_DLANE7_PCR_RXUPPFCFGGQUARTERRATE_0                0xE48
#define B_MODPHY1_PCR_RXUPPFCFGGQUARTERRATE_0                       0x00FFFFFF
#define V_MODPHY1_PCR_RXUPPFCFGGQUARTERRATE_0_COMMON                0x0000194C

#define R_MODPHY1_DLANE6_PCR_REG54                                  0xA54
#define R_MODPHY1_DLANE7_PCR_REG54                                  0xE54
#define B_MODPHY1_PCR_REG54                                         0xC0003F3F
#define V_MODPHY1_PCR_REG54_COMMON                                  0x00000102

#define R_MODPHY1_DLANE6_PCR_RSVD5E                                 0xA5C
#define R_MODPHY1_DLANE7_PCR_RSVD5E                                 0xE5C
#define B_MODPHY1_PCR_RSVD5E                                        0x00FF0000
#define V_MODPHY1_PCR_RSVD5E_COMMON                                 0x00300000

#define R_MODPHY1_DLANE6_PCR_RSVD75                                 0xA74
#define R_MODPHY1_DLANE7_PCR_RSVD75                                 0xE74
#define B_MODPHY1_PCR_RSVD75                                        0x00007F00
#define V_MODPHY1_PCR_RSVD75_COMMON                                 0x00007F00

#define R_MODPHY1_DLANE6_PCR_REG7A                                  0xA78
#define R_MODPHY1_DLANE7_PCR_REG7A                                  0xE78
#define B_MODPHY1_PCR_REG7A                                         0x00030000
#define V_MODPHY1_PCR_REG7A_COMMON                                  0x00030000

#define R_MODPHY1_DLANE6_PCR_RSVD8B                                 0xA88
#define R_MODPHY1_DLANE7_PCR_RSVD8B                                 0xE88
#define B_MODPHY1_PCR_RSVD8B                                        0xC0000000
#define V_MODPHY1_PCR_RSVD8B_COMMON                                 0x40000000

#define R_MODPHY1_DLANE6_PCR_RSVD8D                                 0xA8C
#define R_MODPHY1_DLANE7_PCR_RSVD8D                                 0xE8C
#define B_MODPHY1_PCR_RSVD8D                                        0x00F00F00
#define V_MODPHY1_PCR_RSVD8D_COMMON                                 0x00000000

#define R_MODPHY1_DLANE6_PCR_RSVDEB                                 0xAE8
#define R_MODPHY1_DLANE7_PCR_RSVDEB                                 0xEE8
#define B_MODPHY1_PCR_RSVDEB                                        0xFF000000
#define V_MODPHY1_PCR_RSVDEB_COMMON                                 0x2C000000
#endif
