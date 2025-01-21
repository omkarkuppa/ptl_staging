/** @file
  Register names for High Definition Audio device.

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
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _ACE_HDA_REGS_H_
#define _ACE_HDA_REGS_H_

#define R_ACE_CFG_VID                                  0x00
#define R_ACE_CFG_DID                                  0x02
#define R_ACE_CFG_PI                                   0x09
#define R_ACE_CFG_SCC                                  0x0A
#define R_ACE_CFG_BCC                                  0x0B
#define R_ACE_CFG_HDALBA                               0x10
#define R_ACE_CFG_HDAUBA                               0x14
#define V_ACE_HDABAR_SIZE                              0x400000
#define R_ACE_CFG_DSPLBA                               0x20
#define R_ACE_CFG_DSPUBA                               0x24

#define R_ACE_MEM_GCTL                                 0x0008
#define B_ACE_MEM_GCTL_CRSTB                           BIT0

#define R_ACE_MEM_WAKEEN                               0x000C

#define R_ACE_MEM_WAKESTS                              0x000E

#define R_ACE_MEM_IC                                   0x60
#define R_ACE_MEM_IR                                   0x64
#define R_ACE_MEM_ICS                                  0x68
#define B_ACE_MEM_ICS_ICB                              BIT0
#define B_ACE_MEM_ICS_IRV                              BIT1

#define R_ACE_MEM_TCA                                  0x1C10
#define B_ACE_MEM_TCA_TC0                              (BIT3 | BIT2 | BIT1 | BIT0) ///< VC0 Traffic Class

#define R_ACE_MEM_LTRP                                 0x1C2C
#define B_ACE_MEM_LTRP_GB                              (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define V_ACE_MEM_LTRP_GB                              0x29

#define R_ACE_MEM_CDCCFG                               0x1C34

#define R_ACE_MEM_EM1                                  0x1C40
#define B_ACE_MEM_EM1_BBRK                             (BIT31 | BIT30)
#define B_ACE_MEM_EM1_TMODE                            BIT12
#define B_ACE_MEM_EM1_NODEID                           (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
#define B_ACE_MEM_EM1_ETMODE                           (BIT28 | BIT27)
#define N_ACE_MEM_EM1_ETMODE                           27
#define V_ACE_MEM_EM1_ETMODE_1T_2T                     0x0
#define V_ACE_MEM_EM1_ETMODE_4T                        0x1
#define V_ACE_MEM_EM1_ETMODE_8T                        0x2
#define V_ACE_MEM_EM1_ETMODE_16T                       0x3

#define R_ACE_MEM_EM2                                  0x1C44
#define B_ACE_MEM_EM2_BSMT                             (BIT27 | BIT26)
#define N_ACE_MEM_EM2_BSMT                             26
#define V_ACE_MEM_EM2_BSMT                             0x1
#define B_ACE_MEM_EM2_FNSNPEN                          BIT25
#define B_ACE_MEM_EM2_VC0SNR                           BIT24
#define B_ACE_MEM_EM2_DUM                              BIT23

#define R_ACE_MEM_EM3L                                 0x1C48
#define B_ACE_MEM_EM3L_ISL1EXT2                        (BIT21 | BIT20)
#define N_ACE_MEM_EM3L_ISL1EXT2                        20
#define V_ACE_MEM_EM3L_ISL1EXT2                        0x3

#define R_ACE_MEM_EM4L                                 0x1C50
#define B_ACE_MEM_EM4L_OSL1EXT2                        (BIT21 | BIT20)
#define N_ACE_MEM_EM4L_OSL1EXT2                        20
#define V_ACE_MEM_EM4L_OSL1EXT2                        0x3

#define R_ACE_MEM_PPCTL                                 0x0804
#define B_ACE_MEM_PPCTL_GPROCEN                         BIT30

#define R_ACE_MEM_HDALCTL                               0x0C44
#define B_ACE_MEM_HDALCTL_CPA                           BIT23
#define B_ACE_MEM_HDALCTL_SPA                           BIT16
#define B_ACE_MEM_HDALCTL_SCF                           (BIT3 | BIT2 | BIT1 | BIT0)
#define N_ACE_MEM_HDALCTL_SCF                           0

#define R_ACE_MEM_IDALCTL                               0x0C84
#define B_ACE_MEM_IDALCTL_CPA                           BIT23
#define B_ACE_MEM_IDALCTL_SPA                           BIT16
#define B_ACE_MEM_IDALCTL_SCF                           (BIT3 | BIT2 | BIT1 | BIT0)
#define N_ACE_MEM_IDALCTL_SCF                           0

#define R_ACE_MEM_HSTRVAL                               0x1C90
#define B_ACE_MEM_HSTRVAL_XOCFS                         (BIT1 | BIT0)
#define V_ACE_MEM_HSTRVAL_XOCFS_XTAL_24MHZ              0x0
#define V_ACE_MEM_HSTRVAL_XOCFS_XTAL_38_4MHZ            0x1
#define V_ACE_MEM_HSTRVAL_XOCFS_XTAL_19_2MHZ            0x3

#define R_ACE_MEM_HDAHWI1                               0x1C9C
#define B_ACE_MEM_HDAHWI1_INPAY                         0x0000ffff
#define B_ACE_MEM_HDAHWI1_OUTPAY                        0xffff0000
#define N_ACE_MEM_HDAHWI1_INPAY                         0
#define N_ACE_MEM_HDAHWI1_OUTPAY                        16

#define R_ACE_MEM_PTDC                                  0x1E28
#define B_ACE_MEM_PTDC_SRMIW                            (BIT6 | BIT5 | BIT4)
#define N_ACE_MEM_PTDC_SRMIW                            4
#define V_ACE_MEM_PTDC_SRMIW_32AON                      0x3
#define V_ACE_MEM_PTDC_SRMIW_64AON                      0x4
#define R_ACE_MEM_FNCFG                                 0x1E30
#define B_ACE_MEM_FNCFG_ACED                            BIT0
#define B_ACE_MEM_FNCFG_DSPSD                           BIT2
#define B_ACE_MEM_FNCFG_CGD                             BIT3
#define B_ACE_MEM_FNCFG_BCLD                            BIT4
#define B_ACE_MEM_FNCFG_PGD                             BIT5

#define R_ACE_MEM_HFPCICFGHWI0                          0x1CA0
#define B_ACE_MEM_HFPCICFGHWI0_PI                       ( BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 )
#define N_ACE_MEM_HFPCICFGHWI0_PI                       8
#define V_ACE_MEM_HFPCICFGHWI0_PI_UAA                   0x80
#define V_ACE_MEM_HFPCICFGHWI0_PI_NON_UAA               0x0
#define V_ACE_MEM_HFPCICFGHWI0_PI_DSP_DIS               0x0
#define B_ACE_MEM_HFPCICFGHWI0_SCC                      ( BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16 )
#define N_ACE_MEM_HFPCICFGHWI0_SCC                      16
#define V_ACE_MEM_HFPCICFGHWI0_SCC_UAA                  0x3
#define V_ACE_MEM_HFPCICFGHWI0_SCC_NON_UAA              0x1
#define V_ACE_MEM_HFPCICFGHWI0_SCC_DSP_DIS              0x3
#define B_ACE_MEM_HFPCICFGHWI0_BCC                      ( BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24 )
#define N_ACE_MEM_HFPCICFGHWI0_BCC                      24
#define V_ACE_MEM_HFPCICFGHWI0_BCC_UAA                  0x4
#define V_ACE_MEM_HFPCICFGHWI0_BCC_NON_UAA              0x4
#define V_ACE_MEM_HFPCICFGHWI0_BCC_DSP_DIS              0x4

#define R_ACE_MEM_HFPCICFGHWIS                          0x1CB0
#define B_ACE_MEM_HFPCICFGHWIS_PI                       BIT2
#define B_ACE_MEM_HFPCICFGHWIS_SCC                      BIT3
#define B_ACE_MEM_HFPCICFGHWIS_BCC                      BIT4
#define B_ACE_MEM_HFPCICFGHWIS_SVID                     BIT5
#define B_ACE_MEM_HFPCICFGHWIS_SID                      BIT6

#define R_ACE_MEM_HFPCICFGHWI1                          0x1CA4

#define R_ACE_MEM_FUSVAL                                0x1C80
#define B_ACE_MEM_FUSVAL_DSPSD                          BIT1

#define R_ACE_MEM_DFMICPVCP                             0x71A40
#define B_ACE_MEM_DFMICPVCP_DDZWT                       0xFFFF
#define B_ACE_MEM_DFMICPVCP_DDZE                        ( BIT17 | BIT16 )
#define N_ACE_MEM_DFMICPVCP_DDZE                        16
#define B_ACE_MEM_DFMICPVCP_DGE                         BIT18
#define B_ACE_MEM_DFMICPVCP_DDZPL                       BIT19
#define B_ACE_MEM_DFMICPVCP_DDZLS                       ( BIT27 | BIT26 | BIT25 | BIT24 | BIT23 | BIT22 | BIT21 | BIT20 )
#define N_ACE_MEM_DFMICPVCP_DDZLS                       20

#define V_ACE_MEM_SNDWXLVSCTL_MLCS_APLL                 0x1
#define R_ACE_MEM_SNDW0LVSCTL                           0x00036004
#define N_ACE_MEM_SNDW0LVSCTL_MLCS                      27

#endif
