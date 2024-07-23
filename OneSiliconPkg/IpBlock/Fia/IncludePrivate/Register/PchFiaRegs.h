/** @file
  Register definition for FIA component

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
#ifndef _PCH_FIA_REGS_H_
#define _PCH_FIA_REGS_H_


//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PID:FIA
//
#define R_PCH_FIA_PCR_CC                                  0x0
#define B_PCH_FIA_PCR_CC_SRL                              BIT31
#define B_PCH_FIA_PCR_CC_PTOCGE                           BIT17
#define B_PCH_FIA_PCR_CC_OSCDCGE                          BIT16
#define B_PCH_FIA_PCR_CC_SCPTCGE                          BIT15

#define R_PCH_FIA_PCR_PDSDM1                              0x18
#define B_PCH_FIA_PCR_PDSDM1_GBEPDIPSLSD                  (BIT21 | BIT20)
#define N_PCH_FIA_PCR_PDSDM1_GBEPDIPSLSD                  20
#define V_PCH_FIA_PCR_PDSDM1_GBEPDIPSLSD_3CLK             2
#define B_PCH_FIA_PCR_PDSDM1_GBEPDIPSDM                   (BIT23 | BIT22)
#define N_PCH_FIA_PCR_PDSDM1_GBEPDIPSDM                   22
#define V_PCH_FIA_PCR_PDSDM1_GBEPDIPSDM_1                 0
#define  B_PCH_FIA_PCR_PDSDM1_DMIPDIPSLSD                 (BIT13 | BIT12)  ///< DMI PowerDown IP-Stagger Lane Stagger Delay
#define  N_PCH_FIA_PCR_PDSDM1_DMIPDIPSLSD                 12
#define  V_PCH_FIA_PCR_PDSDM1_DMIPDIPSLSD_1CLK            0
#define  B_PCH_FIA_PCR_PDSDM1_DMIPDIPSDM                  (BIT15 | BIT14)
#define  N_PCH_FIA_PCR_PDSDM1_DMIPDIPSDM                  14
#define  V_PCH_FIA_PCR_PDSDM1_DMIPDIPSDM_16               2
#define  B_PCH_FIA_PCR_PDSDM1_SATAPDIPSLSD                (BIT5 | BIT4)
#define  N_PCH_FIA_PCR_PDSDM1_SATAPDIPSLSD                4
#define  V_PCH_FIA_PCR_PDSDM1_SATAPDIPSLSD_3CLK           2
#define  B_PCH_FIA_PCR_PDSDM1_SATAPDIPSDM                 (BIT7 | BIT6)
#define  N_PCH_FIA_PCR_PDSDM1_SATAPDIPSDM                 6
#define  V_PCH_FIA_PCR_PDSDM1_SATAPDIPSDM_1               0
#define  B_PCH_FIA_PCR_PDSDM1_USBXPDIPSLSD                (BIT1 | BIT0)
#define  N_PCH_FIA_PCR_PDSDM1_USBXPDIPSLSD                0
#define  V_PCH_FIA_PCR_PDSDM1_USBXPDIPSLSD_3CLK           2
#define  B_PCH_FIA_PCR_PDSDM1_USBXPDIPSDM                 (BIT3 | BIT2)
#define  N_PCH_FIA_PCR_PDSDM1_USBXPDIPSDM                 2
#define  V_PCH_FIA_PCR_PDSDM1_USBXPDIPSDM_1               0

#define  R_PCH_FIA_PCR_RXSDM1                             0x1C
#define  B_PCH_FIA_PCR_RXSDM1_GBERXIPSLSD                 (BIT21 | BIT20)
#define  N_PCH_FIA_PCR_RXSDM1_GBERXIPSLSD                 20
#define  V_PCH_FIA_PCR_RXSDM1_GBERXIPSLSD_3CLK            2
#define  B_PCH_FIA_PCR_RXSDM1_GBERXIPSDM                  (BIT23 | BIT22)
#define  N_PCH_FIA_PCR_RXSDM1_GBERXIPSDM                  22
#define  V_PCH_FIA_PCR_RXSDM1_GBERXIPSDM_1                0
#define  B_PCH_FIA_PCR_RXSDM1_DMIRXIPSLSD                 (BIT13 | BIT12)
#define  N_PCH_FIA_PCR_RXSDM1_DMIRXIPSLSD                 12
#define  V_PCH_FIA_PCR_RXSDM1_DMIRXIPSLSD_1CLK            0
#define  B_PCH_FIA_PCR_RXSDM1_DMIRXIPSDM                  (BIT15 | BIT14)
#define  N_PCH_FIA_PCR_RXSDM1_DMIRXIPSDM                  14
#define  V_PCH_FIA_PCR_RXSDM1_DMIRXIPSDM_16               2
#define  B_PCH_FIA_PCR_RXSDM1_SATARXIPSLSD                (BIT5 | BIT4)
#define  N_PCH_FIA_PCR_RXSDM1_SATARXIPSLSD                4
#define  V_PCH_FIA_PCR_RXSDM1_SATARXIPSLSD_3CLK           2
#define  B_PCH_FIA_PCR_RXSDM1_SATARXIPSDM                 (BIT7 | BIT6)
#define  N_PCH_FIA_PCR_RXSDM1_SATARXIPSDM                 6
#define  V_PCH_FIA_PCR_RXSDM1_SATARXIPSDM_1               0
#define  B_PCH_FIA_PCR_RXSDM1_USBXRXIPSLSD                (BIT1 | BIT0)
#define  V_PCH_FIA_PCR_RXSDM1_USBXRXIPSLSD_3CLK           2
#define  B_PCH_FIA_PCR_RXSDM1_USBXRXIPSDM                 (BIT3 | BIT2)
#define  N_PCH_FIA_PCR_RXSDM1_USBXRXIPSDM                 2
#define  V_PCH_FIA_PCR_RXSDM1_USBXRXIPSDM_1               0

#define R_PCH_FIA_PCR_PLLCTL                              0x20
#define B_PCH_FIA_PCR_PLLCTL_PLLCLKVADT                   (BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT                   (BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT                   (BIT17 | BIT16 | BIT15)
#define B_PCH_FIA_PCR_PLLCTL_CL2PLLWAIT                   (BIT20 | BIT19 | BIT18)
#define B_PCH_FIA_PCR_PLLCTL_CL3PLLWAIT                   (BIT23 | BIT22 | BIT21)
#define B_PCH_FIA_PCR_PLLCTL_PLLACBGD                     BIT31
#define B_PCH_FIA_PCR_PLLCTL_CL1PLLFO                     BIT1
#define B_PCH_FIA_PCR_PLLCTL_CL0PLLFO                     BIT0
#define N_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT                   12
#define N_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT                   15
#define N_PCH_FIA_PCR_PLLCTL_CL2PLLWAIT                   18
#define N_PCH_FIA_PCR_PLLCTL_CL3PLLWAIT                   21

#define  R_PCH_FIA_PCR_PDSDM2                             0x28             ///< PowerDown Stagger Delay and Multiplier 2
#define  B_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD             (BIT17 | BIT16)
#define  N_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD             16
#define  V_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD_3CLK        2
#define  V_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD_1CLK        0
#define  B_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSDM              (BIT19 | BIT18)
#define  N_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSDM              18
#define  V_PCH_FIA_PCR_PDSDM2_PWRPWRREQIPSDM_1            0
#define  B_PCH_FIA_PCR_PDSDM2_PXPCPDIPSLSD                (BIT9 | BIT8)
#define  N_PCH_FIA_PCR_PDSDM2_PXPCPDIPSLSD                8
#define  V_PCH_FIA_PCR_PDSDM2_PXPCPDIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_PDSDM2_PXPCPDIPSDM                 (BIT11 | BIT10)
#define  N_PCH_FIA_PCR_PDSDM2_PXPCPDIPSDM                 10
#define  V_PCH_FIA_PCR_PDSDM2_PXPCPDIPSDM_4               1
#define  B_PCH_FIA_PCR_PDSDM2_PXPBPDIPSLSD                (BIT5 | BIT4)
#define  N_PCH_FIA_PCR_PDSDM2_PXPBPDIPSLSD                4
#define  V_PCH_FIA_PCR_PDSDM2_PXPBPDIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_PDSDM2_PXPBPDIPSDM                 (BIT7 | BIT6)
#define  N_PCH_FIA_PCR_PDSDM2_PXPBPDIPSDM                 6
#define  V_PCH_FIA_PCR_PDSDM2_PXPBPDIPSDM_4               1
#define  B_PCH_FIA_PCR_PDSDM2_PCIEPDIPSLSD                (BIT1 | BIT0)  ///< PCIe PowerDown IP-Stagger Lane Stagger Delay
#define  N_PCH_FIA_PCR_PDSDM2_PCIEPDIPSLSD                0
#define  V_PCH_FIA_PCR_PDSDM2_PCIEPDIPSLSD_1CLK           0
#define  V_PCH_FIA_PCR_PDSDM2_PCIEPDIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_PDSDM2_PCIEPDIPSDM                 (BIT3 | BIT2)
#define  N_PCH_FIA_PCR_PDSDM2_PCIEPDIPSDM                 2
#define  V_PCH_FIA_PCR_PDSDM2_PCIEPDIPSDM_4               1
#define  V_PCH_FIA_PCR_PDSDM2_PCIEPDIPSDM_16              2

#define  R_PCH_FIA_PCR_RXSDM2                             0x2C
#define  B_PCH_FIA_PCR_RXSDM2_PXPCRXIPSLSD                (BIT9 | BIT8)
#define  N_PCH_FIA_PCR_RXSDM2_PXPCRXIPSLSD                8
#define  V_PCH_FIA_PCR_RXSDM2_PXPCRXIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_RXSDM2_PXPCRXIPSDM                 (BIT11 | BIT10)
#define  N_PCH_FIA_PCR_RXSDM2_PXPCRXIPSDM                 10
#define  V_PCH_FIA_PCR_RXSDM2_PXPCRXIPSDM_4               1
#define  B_PCH_FIA_PCR_RXSDM2_PXPBRXIPSLSD                (BIT5 | BIT4)
#define  N_PCH_FIA_PCR_RXSDM2_PXPBRXIPSLSD                4
#define  V_PCH_FIA_PCR_RXSDM2_PXPBRXIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_RXSDM2_PXPBRXIPSDM                 (BIT7 | BIT6)
#define  N_PCH_FIA_PCR_RXSDM2_PXPBRXIPSDM                 6
#define  V_PCH_FIA_PCR_RXSDM2_PXPBRXIPSDM_4               1
#define  B_PCH_FIA_PCR_RXSDM2_PCIERXIPSLSD                (BIT1 | BIT0)
#define  N_PCH_FIA_PCR_RXSDM2_PCIERXIPSLSD                0
#define  V_PCH_FIA_PCR_RXSDM2_PCIERXIPSLSD_1CLK           0
#define  V_PCH_FIA_PCR_RXSDM2_PCIERXIPSLSD_2CLK           1
#define  B_PCH_FIA_PCR_RXSDM2_PCIERXIPSDM                 (BIT3 | BIT2)
#define  N_PCH_FIA_PCR_RXSDM2_PCIERXIPSDM                 2
#define  V_PCH_FIA_PCR_RXSDM2_PCIERXIPSDM_4               1
#define  V_PCH_FIA_PCR_RXSDM2_PCIERXIPSDM_16              2

#define R_PCH_FIA_PCR_PLLCTL2                             0x30
#define B_PCH_FIA_PCR_PLLCTL2_CL4PLLWAIT                  (BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_PLLCTL2_CL5PLLWAIT                  (BIT5 | BIT4 | BIT3)
#define N_PCH_FIA_PCR_PLLCTL2_CL4PLLWAIT                  0
#define N_PCH_FIA_PCR_PLLCTL2_CL5PLLWAIT                  3
#define R_PCH_FIA_PCR_PMC                                 0x40
#define B_PCH_FIA_PCR_PMC_OILODVRE                        (BIT21 | BIT20)
#define B_PCH_FIA_PCR_PMC_PRDPGE                          BIT3
#define R_PCH_FIA_PCR_PGCUC                               0x48
#define B_PCH_FIA_PCR_PGCUC_ACC_CLKGATE_DISABLED          BIT0
#define B_PCH_FIA_PCR_PGCUC_T_CLKGATE                     (BIT8 | BIT9 | BIT10 | BIT11)
#define N_PCH_FIA_PCR_PGCUC_T_CLKGATE                     8
#define V_PCH_FIA_PCR_PGCUC_T_CLKGATE                     8
#define R_PCH_FIA_PCR_PGCUCSOC                            0x50
#define B_PCH_FIA_PCR_PGCUCSOC_ACC_SOC_CLKGATE_DISABLED   BIT0

#define R_PCH_FIA_PCR_TREFCLKONTMR1                       0x180 // T refclkon timer 1, controls delay between CLKREQ asserted and CLKSRC
#define R_PCH_FIA_PCR_TREFCLKONTMR2                       0x184
#define R_PCH_FIA_PCR_TREFCLKONTMR3                       0x188
#define R_PCH_FIA_PCR_TREFCLKONTMR4                       0x18C
#define B_PCH_FIA_PCR_TREFCLKON_FIELD                     0xF   // each TREFCLKON field has 4 bits
#define S_PCH_FIA_PCR_TREFCLKON_FIELD                     4     // each TREFCLKON field has 4 bits
#define S_PCH_FIA_PCR_TREFCLKON_FIELDS_PER_REG            8     // there are 8 TREFCLKON fields per register
#define V_PCH_FIA_PCR_TREFCLKON_0us                       0x0
#define V_PCH_FIA_PCR_TREFCLKON_5us                       0x1
#define V_PCH_FIA_PCR_TREFCLKON_10us                      0x2
#define V_PCH_FIA_PCR_TREFCLKON_15us                      0x3
#define V_PCH_FIA_PCR_TREFCLKON_20us                      0x4
#define V_PCH_FIA_PCR_TREFCLKON_25us                      0x5
#define V_PCH_FIA_PCR_TREFCLKON_30us                      0x6
#define V_PCH_FIA_PCR_TREFCLKON_35us                      0x7
#define V_PCH_FIA_PCR_TREFCLKON_40us                      0x8
#define V_PCH_FIA_PCR_TREFCLKON_45us                      0x9
#define V_PCH_FIA_PCR_TREFCLKON_50us                      0xA
#define V_PCH_FIA_PCR_TREFCLKON_60us                      0xB
#define V_PCH_FIA_PCR_TREFCLKON_70us                      0xC
#define V_PCH_FIA_PCR_TREFCLKON_80us                      0xD
#define V_PCH_FIA_PCR_TREFCLKON_90us                      0xE
#define V_PCH_FIA_PCR_TREFCLKON_100us                     0xF

#define B_PCH_FIA_PCR_LOS1_L0O                            (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_LOS1_L1O                            (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_LOS1_L2O                            (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_LOS1_L3O                            (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_LOS1_L4O                            (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_LOS1_L5O                            (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_LOS1_L6O                            (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_LOS1_L7O                            (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_LOS2_L8O                            (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_LOS2_L9O                            (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_LOS2_L10O                           (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_LOS2_L11O                           (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_LOS2_L12O                           (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_LOS2_L13O                           (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_LOS2_L14O                           (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_LOS2_L15O                           (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_LOS3_L16O                           (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_LOS3_L17O                           (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_LOS3_L18O                           (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_LOS3_L19O                           (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_LOS3_L20O                           (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_LOS3_L21O                           (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_LOS3_L22O                           (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_LOS3_L23O                           (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_LOS4_L24O                           (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_LOS4_L25O                           (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_LOS4_L26O                           (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_LOS4_L27O                           (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_LOS4_L28O                           (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_LOS4_L29O                           (BIT23 | BIT22 | BIT21 | BIT20)
#define B_PCH_FIA_PCR_LOS4_L30O                           (BIT27 | BIT26 | BIT25 | BIT24)
#define B_PCH_FIA_PCR_LOS4_L31O                           (BIT31 | BIT30 | BIT29 | BIT28)
#define B_PCH_FIA_PCR_LOS5_L32O                           (BIT3 | BIT2 | BIT1 | BIT0)
#define B_PCH_FIA_PCR_LOS5_L33O                           (BIT7 | BIT6 | BIT5 | BIT4)
#define B_PCH_FIA_PCR_LOS5_L34O                           (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_FIA_PCR_LOS5_L35O                           (BIT15 | BIT14 | BIT13 | BIT12)
#define B_PCH_FIA_PCR_LOS5_L36O                           (BIT19 | BIT18 | BIT17 | BIT16)
#define B_PCH_FIA_PCR_LOS5_L37O                           (BIT23 | BIT22 | BIT21 | BIT20)

#endif
