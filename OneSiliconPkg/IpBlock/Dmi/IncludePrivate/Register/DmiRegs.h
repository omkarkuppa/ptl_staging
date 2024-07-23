/** @file
  Register names for DMI

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
#ifndef _DMI_REGS_H_
#define _DMI_REGS_H_

#define R_DMI_CMD                           0x04
 #define B_DMI_CMD_SEE                      BIT8

#define R_DMI_BCTRL                         0x3e      ///< Bridge Control
#define B_DMI_BCTRL_SE                      BIT1      ///< SERR Enable

#define R_DMI_CAPP                          0x34
 #define B_DMI_CAPP_PTR_MASK                0xFF
 #define V_DMI_CAPP_PTR                     0x40

#define R_DMI_CLIST                         0x40
 #define V_DMI_CLIST_NEXT                   0x80
 #define N_DMI_CLIST_NEXT                   8
 #define V_DMI_CLIST_CID                    0x10

#define R_DMI_XCAP                          0x42
 #define V_DMI_XCAP_CV                      0x2

#define R_DMI_MID                           0x80
 #define V_DMI_MID_NEXT                     0x98
 #define V_DMI_MID_CID                      0x05

#define R_DMI_DCAP                          0x44
 #define B_DMI_DCAP_MPS                     (BIT2 | BIT1 | BIT0)
 #define V_DMI_DCAP_MPS_128                 0x0
 #define V_DMI_DCAP_MPS_256                 0x1

#define R_DMI_DCTL                          0x48
 #define B_DMI_DCTL_MPS                     (BIT7 | BIT6 | BIT5)
 #define N_DMI_DCTL_MPS_256                 5
 #define V_DMI_DCTL_MPS_256                 0x1
 #define B_DMI_DCTL_URE                     BIT3
 #define B_DMI_DCTL_FEE                     BIT2
 #define B_DMI_DCTL_NFE                     BIT1
 #define B_DMI_DCTL_CEE                     BIT0

#define R_DMI_LCAP                          0x4C
 #define B_DMI_LCAP_ASPMOC                  BIT22
 #define B_DMI_LCAP_EL1                     (BIT17 | BIT16 | BIT15)
 #define N_DMI_LCAP_EL1                     15
 #define B_DMI_LCAP_APMS                    (BIT11 | BIT10)
 #define N_DMI_LCAP_APMS                    10
 #define V_DMI_LCAP_APMS_DIS                0x0
 #define V_DMI_LCAP_APMS_L0S                0x1
 #define V_DMI_LCAP_APMS_L1                 0x2
 #define V_DMI_LCAP_APMS_L0S_L1             0x3
 #define B_DMI_LCAP_MLS                     (BIT3 | BIT2 | BIT1 | BIT0) ///< Max Link Speed
 #define V_DMI_LCAP_MLS_AUTO                0
 #define V_DMI_LCAP_MLS_GEN1                1
 #define V_DMI_LCAP_MLS_GEN2                2
 #define V_DMI_LCAP_MLS_GEN3                3
 #define V_DMI_LCAP_MLS_GEN4                4

#define R_DMI_LCTL                          0x0050                      ///< Link Control
 #define B_DMI_LCTL_ES                      BIT7                        ///< Extended Synch
 #define B_DMI_LCTL_CCC                     BIT6                        ///< Common Clock Configuration
 #define B_DMI_LCTL_RL                      BIT5                        ///< link Retrain
 #define B_DMI_LCTL_ASPM                    (BIT1 | BIT0)               ///< Active State Link PM Control
 #define V_DMI_LCTL_ASPM_DIS                0x0                         ///< ASPM Disabled
 #define V_DMI_LCTL_ASPM_L0S                0x1                         ///< L0s Entry Enabled
 #define V_DMI_LCTL_ASPM_L1                 0x2                         ///< L1 Entry Enabled
 #define V_DMI_LCTL_ASPM_L0SL1              0x3                         ///< L0s and L1 Entry Enabled


#define R_DMI_LSTS                          0x52                      ///< Link Status
 #define B_DMI_LSTS_CLS_SIP                 BIT0
 #define B_DMI_LSTS_CLS_OFFSET              0
 #define B_DMI_LSTS_CLS_MASK                0xF
 #define B_DMI_LCTL_RL                      BIT5
 #define B_DMI_LCTL_RL_OFFSET               5
 #define B_DMI_LSTS_LT_OFFSET               BIT11

#define R_DMI_RCTL                          0x5C
 #define B_DMI_RCTL_CRSSVE                  BIT4

#define R_DMI_ROOTCAP                       0x5E
 #define B_DMI_ROOTCAP_CRSSV                BIT0

#define R_DMI_LCTL2                         0x0070                      ///< Link Control 2
 #define B_DMI_LCTL2_TLS                    0xF                         ///< Target Link Speed

#define R_DMI_SVCAP                         0x98
 #define V_DMI_SVCAP_NEXT                   0xA0
 #define V_DMI_SVCAP_CID                    0x0D

#define R_DMI_PMCAP                         0xA0
 #define V_DMI_PMCAP_NEXT                   0x00
 #define V_DMI_PMCAP_CID                    0x01

#define R_DMI_PMCS                          0xA4
 #define B_DMI_PMCS_NSR                     BIT3

#define R_DMI_MPC2                          0xD4
 #define B_DMI_MPC2_DISPLLEWL1SE            BIT16
 #define B_DMI_MPC2_PTNFAE                  BIT12
 #define B_DMI_MPC2_IPF                     BIT11
 #define B_DMI_MPC2_LSTP                    BIT6                        ///< Link Speed Training Policy
 #define B_DMI_MPC2_EOIFD                   BIT1

#define R_DMI_MPC                           0xD8
 #define B_DMI_MPC_MMBNCE                   BIT27
 #define B_DMI_MPC_P8XDE                    BIT26
 #define B_DMI_MPC_IRRCE                    BIT25
 #define B_DMI_MPC_BMERCE                   BIT24
 #define B_DMI_MPC_FCDL1E                   BIT21
 #define B_DMI_MPC_CCEL                     (BIT17 | BIT16 | BIT15)
 #define V_DMI_MPC_CCEL_512NS_1US           0x4
 #define V_DMI_MPC_CCEL_2US_4US             0x6
 #define N_DMI_MPC_CCEL                     15
 #define B_DMI_MPC_FCP                      BIT4
 #define B_DMI_MPC_FCP_MASK                 0x70
 #define N_DMI_MPC_FCP                      4
 #define V_DMI_MPC_FCP                      0x4
 #define B_DMI_MPC_MCTPSE                   BIT3
 #define B_DMI_MPC_BT                       BIT2
 #define B_DMI_CFG_MPC_PCIESD               ( BIT14 | BIT13 | BIT12 ) ///< Speed Disable
 #define N_DMI_CFG_MPC_PCIESD               12

#define R_DMI_PHYCTL2                       0xF5
 #define B_DMI_PHYCTL2_TDFT                 (BIT7 | BIT6)
 #define B_DMI_PHYCTL2_TXCFGCHGWAIT         (BIT5 | BIT4)

#define R_DMI_AECH                          0x100
 #define V_DMI_AECH_NEXT                    0x220
 #define N_DMI_AECH_NEXT                    20
 #define V_DMI_AECH_CV                      0x1
 #define N_DMI_AECH_CV                      16
 #define V_DMI_AECH_CID                     0x0001

#define R_DMI_ACSECH                        0x220
 #define V_DMI_ACSECH_NEXT                  0x200
 #define N_DMI_ACSECH_NEXT                  20
 #define V_DMI_ACSECH_CV                    0x1
 #define N_DMI_ACSECH_CV                    16
 #define V_DMI_ACSECH_CID                   0x000D

#define R_DMI_L1SECH                        0x200
 #define V_DMI_L1SECH_NEXT                  0x150
 #define N_DMI_L1SECH_NEXT                  20
 #define V_DMI_L1SECH_CV                    0x2
 #define N_DMI_L1SECH_CV                    16
 #define V_DMI_L1SECH_CID                   0x001E

#define R_DMI_PTMECH                        0x150
 #define V_DMI_PTMECH_NEXT                  0x280
 #define N_DMI_PTMECH_NEXT                  20
 #define V_DMI_PTMECH_CV                    0x1
 #define N_DMI_PTMECH_CV                    16
 #define V_DMI_PTMECH_CID                   0x001F

#define R_DMI_VCCH                          0x280
 #define V_DMI_VCCH_NEXT                    0xA30
 #define N_DMI_VCCH_NEXT                    20
 #define V_DMI_VCCH_CV                      0x1
 #define N_DMI_VCCH_CV                      16
 #define V_DMI_VCCH_CID                     0x0002

#define R_DMI_SPEECH                        0xA30
 #define V_DMI_SPEECH_NEXT                  0xA90
 #define N_DMI_SPEECH_NEXT                  20
 #define V_DMI_SPEECH_CV                    0x1
 #define N_DMI_SPEECH_CV                    16
 #define V_DMI_SPEECH_CID                   0x0019

#define R_DMI_DLFECH                        0xA90
 #define V_DMI_DLFECH_NEXT                  0xA9C
 #define N_DMI_DLFECH_NEXT                  20
 #define V_DMI_DLFECH_CV                    0x1
 #define N_DMI_DLFECH_CV                    16
 #define V_DMI_DLFECH_CID                   0x0025

#define R_DMI_PL16GECH                      0xA9C
 #define V_DMI_PL16GECH_NEXT                0xEDC
 #define N_DMI_PL16GECH_NEXT                20
 #define V_DMI_PL16GECH_CV                  0x1
 #define N_DMI_PL16GECH_CV                  16
 #define V_DMI_PL16GECH_CID                 0x0025

#define R_DMI_PL16MECH                      0xEDC
 #define V_DMI_PL16MECH_NEXT                0x00
 #define N_DMI_PL16MECH_NEXT                20
 #define V_DMI_PL16MECH_CV                  0x1
 #define N_DMI_PL16MECH_CV                  16
 #define V_DMI_PL16MECH_CID                 0x0027

#define R_DMI_DCAP2                         0x64
 #define B_DMI_DCAP2_EFFS                   BIT20
 #define B_DMI_DCAP2_OBFFS                  BIT19 | BIT18
 #define B_DMI_DCAP2_PX10BTRS               BIT17
 #define B_DMI_DCAP2_PX10BTCS               BIT16
 #define B_DMI_DCAP2_LTRMS                  BIT11
 #define B_DMI_DCAP2_AC64BS                 BIT8
 #define B_DMI_DCAP2_AC32BS                 BIT7
 #define B_DMI_DCAP2_ARS                    BIT6

#define R_DMI_DCTL2                         0x68
 #define B_DMI_DCTL2_PX10BTRE               BIT11
 #define B_DMI_DCTL2_LTREN                  BIT10
 #define B_DMI_DCTL2_AEB                    BIT7
 #define B_DMI_DCTL2_ARE                    BIT6
 #define B_DMI_DCTL2_AFE                    BIT5
 #define B_DMI_DCTL2_CTD                    BIT4

#define R_DMI_LCAP2                         0X6C
  #define B_DMI_LCAP2_RPDS                  BIT23

#define R_DMI_CCFG                          0xD0     ///< Channel Configuration
 #define B_DMI_CCFG_CRE                     BIT31
 #define B_DMI_CCFG_IORE                    BIT30
 #define B_DMI_CCFG_UPSD                    BIT24
 #define B_DMI_CCFG_DCGEISMA                BIT15
 #define B_DMI_CCFG_UNRD                    BIT13 | BIT12
 #define B_DMI_CCFG_UNRS                    BIT6 | BIT5 | BIT4
 #define B_DMI_CCFG_UPRS                    BIT2 | BIT1 | BIT0

#define R_DMI_IPSCE                         0xE0
 #define B_DMI_IPSCE_RXROM                  BIT19 | BIT18
 #define B_DMI_IPSCE_TXROM                  BIT17 | BIT16
 #define N_DMI_IPSCE_TXROM                  16
 #define V_DMI_IPSCE_TXROM                  0x2

#define R_DMI_RPDCGEN                       0xE1     ///< Root Port Dynamic Clock Gate Enable
 #define B_DMI_PTOCGE                       BIT6
 #define B_DMI_LCLKREQEN                    BIT5
 #define B_DMI_BBCLKREQEN                   BIT4
 #define B_DMI_SRDBCGEN                     BIT2
 #define B_DMI_RPDLCGEN                     BIT1
 #define B_DMI_RPDBCGEN                     BIT0

#define R_DMI_RPPGEN                        0xE2
 #define B_DMI_RPPGEN_PTOTOP                BIT6
 #define B_DMI_RPPGEN_SEOSCGE               BIT4

#define R_DMI_PWRCTL                        0xE8    ///< Power Control
 #define B_DMI_PWRCTL_DARECE                BIT28
 #define B_DMI_PWRCTL_LIFECF                BIT23
 #define B_DMI_PWRCTL_WPDMPGEP              BIT17
 #define B_DMI_PWRCTL_DLP                   BIT16
 #define B_DMI_PWRCTL_DLP_OFFSET            16
 #define B_DMI_PWRCTL_DBUPI                 BIT15
 #define B_DMI_PWRCTL_RPL1SQPOL             BIT1
 #define B_DMI_PWRCTL_RPDTSQPOL             BIT0

#define  R_DMI_DC                           0xEC
 #define B_DMI_DC_DCT1C                     BIT15
 #define B_DMI_DC_DCT0C                     BIT14
 #define B_DMI_DC_COM                       BIT13
 #define B_DMI_DC_RCRBNRCE                  BIT4

#define R_DMI_IPCS                          0xF0
 #define B_DMI_IPCS_IMPS                    BIT10 | BIT9 | BIT8
 #define N_DMI_IPCS_IMPS                    8
 #define V_DMI_IPCS_IMPS                    7
 #define B_DMI_IPCS_IMRS                    BIT6 | BIT5 | BIT4
 #define N_DMI_IPCS_IMRS                    4
 #define V_DMI_IPCS_IMRS                    7

#define R_DMI_PHYCTL2                       0xF5
 #define B_DMI_PHYCTL2_PXPG3PLLOFFEN        BIT1
 #define B_DMI_PHYCTL2_PXPG2PLLOFFEN        BIT0

#define R_DMI_PHYCTL3                       0xF6         ///< Physical Layer And AFE Control 3
 #define B_DMI_PHYCTL3_SQDIROVREN           BIT2
 #define B_DMI_PHYCTL3_SQDIRCTRL            BIT1

#define R_DMI_IOSFSBCS                      0xF7         ///< IOSF Sideband Control And Status
 #define B_DMI_IOSFSBCS_SCPTCGE             BIT6
 #define B_DMI_IOSFSBCS_SIID                BIT3 | BIT2

#define R_DMI_PTMCAPR                       0x154
 #define B_DMI_PTMCAPR_PTMREQC              BIT0
 #define B_DMI_PTMCAPR_PTMRSPC              BIT1
 #define B_DMI_PTMCAPR_PTMRC                BIT2
 #define B_DMI_PTMCAPR_EPTMC                BIT3

#define R_DMI_PTMCTLR                       0x158
 #define B_DMI_PTMCTLR_RS                   BIT1
 #define B_DMI_PTMCTLR_PTME                 BIT0

#define R_DMI_L1SCAP                        0x204
 #define B_DMI_L1SCAP_PTV                   BIT23 | BIT22 | BIT21 | BIT20 | BIT19
 #define B_DMI_L1SCAP_PTV_OFFSET            19
 #define B_DMI_L1SCAP_PTV_MASK              0xF80000
 #define V_DMI_L1SCAP_PTV_50US              0x5
 #define B_DMI_L1SCAP_PTPOS                 BIT17 | BIT16
 #define B_DMI_L1SCAP_PTPOS_OFFSET          16
 #define B_DMI_L1SCAP_PTPOS_MASK            0x30000
 #define V_DMI_L1SCAP_PTPOS_10US            0x1
 #define B_DMI_L1SCAP_PCMRT_OFFSET          8
 #define B_DMI_L1SCAP_PCMRT_MASK            0xFF00
 #define B_DMI_L1SCAP_L1SSEIE               BIT6
 #define B_DMI_L1SCAP_L1SSES                BIT5
 #define B_DMI_L1SCAP_L1PSS                 BIT4
 #define B_DMI_L1SCAP_AL11S                 BIT3
 #define B_DMI_L1SCAP_AL12S                 BIT2
 #define B_DMI_L1SCAP_PPL11S                BIT1
 #define B_DMI_L1SCAP_PPL12S                BIT0

#define R_DMI_L1SCTL1                       0x208
 #define B_DMI_L1SCTL1_L12LTRTLSV           BIT31 | BIT30 | BIT29
 #define B_DMI_L1SCTL1_L12LTRTLSV_OFFSET    29
 #define V_DMI_L1SCTL1_L12LTRTLSV           0x2
 #define B_DMI_L1SCTL1_L12LTRTLV_MASK       0x3FF0000
 #define B_DMI_L1SCTL1_L12LTRTLV_OFFSET     16
 #define V_DMI_L1SCTL1_L12LTRTLV            0xE6
 #define B_DMI_L1SCTL1_L1SSEIE              BIT4             ///< CLKREQ Acceleration Interrupt Enable
 #define N_DMI_L1SCTL1_L1SSEIE              4
 #define B_DMI_L1SCTL1_CMRT                 ( BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 ) ///< Common Mode Restore Time
 #define N_DMI_L1SCTL1_CMRT                 8
 #define B_DMI_L1SCTL1_AL11E                BIT3
 #define B_DMI_L1SCTL1_AL12E                BIT2
 #define B_DMI_L1SCTL1_PPL11E               BIT1
 #define B_DMI_L1SCTL1_PPL12E               BIT0

#define R_DMI_L1SCTL2                       0x0000020cU      ///< L1 Sub-States Control 2
 #define B_DMI_L1SCTL2_TPOS                 BIT1 | BIT0      ///< Tpower_on Scale
 #define N_DMI_L1SCTL2_TPOS                 0
 #define V_DMI_L1SCTL2_TPOS                 0x1
 #define B_DMI_L1SCTL2_POWT                 BIT7 | BIT6 | BIT5 | BIT4 | BIT3 ///< Power On Wait Time
 #define N_DMI_L1SCTL2_POWT                 3
 #define V_DMI_L1SCTL2_POWT                 0x5

#define R_DMI_CFG_VNNREMCTL                0x00000c70U      ///< VNN Removal Control
 #define B_DMI_CFG_VNNREMCTL_LRSLFVNNRE    ( BIT1 | BIT0 )  ///< Link Reset Suppression Latency For VNN Removal Exit
 #define N_DMI_CFG_VNNREMCTL_LRSLFVNNRE    0
 #define B_DMI_CFG_VNNREMCTL_ISPLFVNNRE    ( BIT3 | BIT2 )  ///< Internal States Propagation Latency For VNN Removal Exit
 #define N_DMI_CFG_VNNREMCTL_ISPLFVNNRE    2

#define R_DMI_PVCCR1                        0x284
 #define B_DMI_PVCCR1_EVCC                  BIT2 | BIT1 | BIT0
 #define V_DMI_PVCCR1_EVCC                  0x1

#define R_DMI_V0VCRC                        0x290
 #define B_DMI_V0VCRC_MTS_MASK              0x7F0000
 #define N_DMI_V0VCRC_MTS                   0x16
 #define V_DMI_V0VCRC_MTS                   0x1

#define R_DMI_VC0CTL                        0x294
 #define B_DMI_VC0CTL_TVM_MASK              0xFE
 #define N_DMI_VC0CTL_TVM                   1
 #define B_DMI_VC0CTL_ETVM_MASK             0xFC00
 #define N_DMI_VC0CTL_ETVM                  10

#define R_DMI_V1STS                         0x2A6
 #define B_DMI_V1STS_NP                     1

#define R_DMI_V0STS                         0x29A
 #define B_DMI_V0STS_NP                     1

#define R_DMI_V1VCRC                        0x29C
 #define B_DMI_V1VCRC_MTS_MASK              0x7F0000
 #define N_DMI_V1VCRC_MTS                   0x16

#define R_DMI_VC1CTL                        0x2A0
 #define B_DMI_VC1CTL_EN                    BIT31
 #define B_DMI_VC1CTL_ID_MASK               0xF000000
 #define N_DMI_VC1CTL_ID                    24
 #define V_DMI_VC1CTL_ID                    1
 #define B_DMI_VC1CTL_ETVM_MASK             0xFC00
 #define N_DMI_VC1CTL_ETVM                  10
 #define B_DMI_VC1CTL_TVM_MASK              0xFE
 #define N_DMI_VC1CTL_TVM                   1

#define R_DMI_VMCTL                         0x2B0
 #define B_DMI_VMCTL_EN                     BIT31

#define  R_DMI_PCIERTP1                     0x300
 #define  B_DMI_PCIERTP1_G2X1               BIT20
 #define  B_DMI_PCIERTP1_G2X1_OFFSET        20
 #define  B_DMI_PCIERTP1_G2X1_MASK          0xF00000
 #define  B_DMI_PCIERTP1_G2X2               BIT16
 #define  B_DMI_PCIERTP1_G2X2_OFFSET        16
 #define  B_DMI_PCIERTP1_G2X2_MASK          0xF0000
 #define  B_DMI_PCIERTP1_G2X4               BIT12
 #define  B_DMI_PCIERTP1_G2X4_OFFSET        12
 #define  B_DMI_PCIERTP1_G2X4_MASK          0xF000
 #define  V_DMI_PCIERTP1_G2X4               0xF
 #define  B_DMI_PCIERTP1_G1X1               BIT8
 #define  B_DMI_PCIERTP1_G1X1_OFFSET        8
 #define  B_DMI_PCIERTP1_G1X1_MASK          0xF00
 #define  V_DMI_PCIERTP1_G1X1               0x6
 #define  B_DMI_PCIERTP1_G1X2               BIT4
 #define  B_DMI_PCIERTP1_G1X2_OFFSET        4
 #define  B_DMI_PCIERTP1_G1X2_MASK          0xF0
 #define  V_DMI_PCIERTP1_G1X2               0XC
 #define  B_DMI_PCIERTP1_G1X4               BIT0
 #define  B_DMI_PCIERTP1_G1X4_OFFSET        0
 #define  B_DMI_PCIERTP1_G1X4_MASK          0xF
 #define  V_DMI_PCIERTP1_G1X4               0xC

#define  R_DMI_PCIERTP2                     0x304
 #define  B_DMI_PCIERTP2_G3X1               BIT8
 #define  B_DMI_PCIERTP2_G3X1_OFFSET        8
 #define  B_DMI_PCIERTP2_G3X1_MASK          0xF00
 #define  B_DMI_PCIERTP2_G3X2               BIT4
 #define  B_DMI_PCIERTP2_G3X2_OFFSET        4
 #define  B_DMI_PCIERTP2_G3X2_MASK          0xF0
 #define  B_DMI_PCIERTP2_G3X4               BIT0
 #define  B_DMI_PCIERTP2_G3X4_OFFSET        0
 #define  B_DMI_PCIERTP2_G3X4_MASK          0xF

#define R_DMI_G4L0SCTL                      0x310             ///< GEN4 L0s Control
 #define B_DMI_G4L0SCTL_G4ASL0SPL           BIT24
 #define B_DMI_G4L0SCTL_G4ASL0SPL_MASK      0xFF000000
 #define B_DMI_G4L0SCTL_G4ASL0SPL_OFFSET    24
 #define V_DMI_G4L0SCTL_G4ASL0SPL           0x28
 #define B_DMI_G4L0SCTL_G4L0SIC             BIT22
 #define B_DMI_G4L0SCTL_G4L0SICL_OFFSET     22
 #define B_DMI_G4L0SCTL_G4L0SICL_MASK       0x00C00000
 #define V_DMI_G4L0SCTL_G4L0SIC             0x3
 #define B_DMI_G4L0SCTL_G4UCNFTS            BIT8
 #define B_DMI_G4L0SCTL_G4UCNFTS_MASK       0x0000FF00
 #define B_DMI_G4L0SCTL_G4UCNFTS_OFFSET     8
 #define V_DMI_G4L0SCTL_G4UCNFTS            0X80
 #define B_DMI_G4L0SCTL_G4CCNFTS            BIT0
 #define B_DMI_G4L0SCTL_G4CCNFTS_MASK       0xFF
 #define B_DMI_G4L0SCTL_G4CCNFTS_OFFSET     0
 #define V_DMI_G4L0SCTL_G4CCNFTS            0x5E

#define R_DMI_PCIENFTS                      0x314     ///< PCI Express NFTS
 #define B_DMI_PCIENFTS_G2UCNFTS_MASK       0xFF000000
 #define N_DMI_PCIENFTS_G2UCNFTS            24
 #define V_DMI_PCIENFTS_G2UCNFTS            0x7E
 #define B_DMI_PCIENFTS_G2CCNFTS_MASK       0xFF0000
 #define N_DMI_PCIENFTS_G2CCNFTS            16
 #define V_DMI_PCIENFTS_G2CCNFTS            0xC8
 #define B_DMI_PCIENFTS_G1UCNFTS_MASK       0x0000FF00
 #define N_DMI_PCIENFTS_G1UCNFTS            8
 #define V_DMI_PCIENFTS_G1UCNFTS            0xDC
 #define B_DMI_PCIENFTS_G1CCNFTS_MASK       0xFF
 #define N_DMI_PCIENFTS_G1CCNFTS            0
 #define V_DMI_PCIENFTS_G1CCNFTS            0xDC

#define R_DMI_PCIEL0SC                      0x318          ///< PCI Express L0s Control
 #define B_DMI_PCIEL0SC_G2ASL0SPL_MASK      0xFF000000
 #define N_DMI_PCIEL0SC_G2ASL0SPL           24
 #define V_DMI_PCIEL0SC_G2ASL0SPL           0x14
 #define B_DMI_PCIEL0SC_G1ASL0SPL_MASK      0x00FF0000
 #define N_DMI_PCIEL0SC_G1ASL0SPL           16
 #define V_DMI_PCIEL0SC_G1ASL0SPL           0x14

#define R_DMI_PCIEATL                       0x31C         ///< Ack Latency
 #define B_DMI_PCIEATL_G1X1                 BIT8 | BIT7 | BIT6
 #define N_DMI_PCIEATL_G1X1                 6
 #define V_DMI_PCIEATL_G1X1                 0x7

#define R_DMI_PCIECFG2                      0x320         ///< PCI Express Configuration 2
 #define B_DMI_PCIECFG2_LBWSSTE             BIT30
 #define B_DMI_PCIECFG2_RRCP                BIT29
 #define B_DMI_PCIECFG2_RLLG3R              BIT27
 #define B_DMI_PCIECFG2_CROAOV              BIT24
 #define B_DMI_PCIECFG2_CROAOE              BIT23
 #define B_DMI_PCIECFG2_CRSREN              BIT22
 #define B_DMI_PCIECFG2_PMET                BIT21 | BIT20
 #define N_DMI_PCIECFG2_PMET                20
 #define V_DMI_PCIECFG2_PMET_10MS           1
 #define V_DMI_PCIECFG2_PMET_DISABLE        3

#define R_DMI_PCIEDBG                       0x324        ///< PCI Express Debug And Configuration
 #define B_DMI_PCIEDBG_USSP                 BIT27 | BIT26
 #define N_DMI_PCIEDBG_USSP                 26
 #define V_DMI_PCIEDBG_USSP                 2
 #define B_DMI_PCIEDBG_LGCLKSQEXITDBTIMERS  BIT25 | BIT24
 #define N_DMI_PCIEDBG_LGCLKSQEXITDBTIMERS  24
 #define B_DMI_PCIEDBG_CTONFAE              BIT14
 #define B_DMI_PCIEDBG_LDSWQRP              BIT13
 #define B_DMI_PCIEDBG_SQOL0                BIT7
 #define B_DMI_PCIEDBG_SPCE                 BIT5

#define R_DMI_PCIEALC                       0x338
 #define B_DMI_PCIEALC_ITLRCLD              BIT29
 #define B_DMI_PCIEALC_ILLRCLD              BIT28
 #define B_DMI_PCIEALC_BLKDQDASD            BIT25
 #define B_DMI_PCIEALC_SSRLD                BIT24
 #define B_DMI_PCIEALC_SSRRS                BIT23
 #define B_DMI_PCIEALC_RTD3PDSP             BIT21
 #define B_DMI_PCIEALC_ONPRASPML1P          BIT11

#define R_DMI_PTMECFG                       0x3B0
 #define B_DMI_PTMECFG_PLTLFF               BIT20 | BIT19 | BIT18
 #define V_DMI_PTMECFG_PLTLFF               0x1
 #define N_DMI_PTMECFG_PLTLFF               18
 #define B_DMI_PTMECFG_PGTSCFF              BIT11 | BIT10 | BIT9
 #define V_DMI_PTMECFG_PGTSCFF              0x2
 #define N_DMI_PTMECFG_PGTSCFF              9
 #define B_DMI_PTMECFG_PGTSCFE              BIT8
 #define B_DMI_PTMECFG_PTMRPAE              BIT6
 #define B_DMI_PTMECFG_PTMRNOPAD            BIT5 | BIT4
 #define V_DMI_PTMECFG_PTMRNOPAD            0x1
 #define N_DMI_PTMECFG_PTMRNOPAD            0x4
 #define B_DMI_PTMECFG_IOSFMADP             BIT3 | BIT2 | BIT1 | BIT0
 #define V_DMI_PTMECFG_IOSFMADP             0x2

#define R_DMI_LTROVR                        0x00000400U      ///< Latency Tolerance Reporting Override

#define R_DMI_LTROVR2                       0x00000404U      ///< Latency Tolerance Reporting Override 2
 #define B_DMI_LTROVR2_LTRSOVREN            BIT0             ///< LTR Snoop Override Enable
 #define N_DMI_LTROVR2_LTRSOVREN            0
 #define B_DMI_LTROVR2_LTRNSOVREN           BIT1             ///< LTR Non-Snoop Override Enable
 #define N_DMI_LTROVR2_LTRNSOVREN           1
 #define B_DMI_LTROVR2_LTROVRPLCY           BIT3             ///< LTR Override Policy
 #define N_DMI_LTROVR2_LTROVRPLCY           3

#define R_DMI_TNPT                          0x418
 #define B_DMI_TNPT_TP                      0xFF000000
 #define N_DMI_TNPT_TP                      24
 #define V_DMI_TNPT_TP                      0x04
 #define B_DMI_TNPT_TT                      0x00FF0000
 #define B_DMI_TNPT_DRXLTE                  BIT1
 #define B_DMI_TNPT_DTXLTE                  BIT0

#define R_DMI_PCIEPMECTL                    0x420         ///< PCIe PM Extension Control
 #define B_DMI_PCIEPMECTL_DLSULPPGE         BIT30
 #define B_DMI_PCIEPMECTL_DLSULDLSD         BIT29
 #define B_DMI_PCIEPMECTL_L1OCREWD          BIT28
 #define B_DMI_PCIEPMECTL_L1LE              BIT17
 #define B_DMI_PCIEPMECTL_L1LTRTLSV         BIT16 | BIT15 | BIT14
 #define V_DMI_PCIEPMECTL_L1LTRTLSV         0x2
 #define N_DMI_PCIEPMECTL_L1LTRTLSV         14
 #define B_DMI_PCIEPMECTL_L1LTRTLV_MASK     0x3FF0
 #define N_DMI_PCIEPMECTL_L1LTRTLV          4
 #define V_DMI_PCIEPMECTL_L1LTRTLV          0x41
 #define B_DMI_PCIEPMECTL_L1FSOE            BIT0

#define R_DMI_PCIEPMECTL2                   0x424         ///< PCIe PM Extension Control 2
 #define B_DMI_PCIEPMECTL2_CPMCSRE          BIT27
 #define B_DMI_PCIEPMECTL2_CPGENH           BIT13 | BIT12
 #define N_DMI_PCIEPMECTL2_CPGENH           12
 #define V_DMI_PCIEPMECTL2_CPGENH           0x1
 #define B_DMI_PCIEPMECTL2_CPGEXH           BIT15 | BIT14
 #define N_DMI_PCIEPMECTL2_CPGEXH           14
 #define V_DMI_PCIEPMECTL2_CPGEXH           0x1
 #define B_DMI_PCIEPMECTL2_PHYCLPGE         BIT11
 #define B_DMI_PCIEPMECTL2_FDCPGE           BIT8
 #define B_DMI_PCIEPMECTL2_DETSCPGE         BIT7
 #define B_DMI_PCIEPMECTL2_L23RDYSCPGE      BIT6
 #define B_DMI_PCIEPMECTL2_DISSCPGE         BIT5
 #define B_DMI_PCIEPMECTL2_L1SCPGE          BIT4

#define R_DMI_PCE                           0x428          ///< Power Control Enable
 #define B_DMI_PCE_HAE                      BIT5
 #define B_DMI_PCE_SE                       BIT3
 #define B_DMI_PCE_PMCRE                    BIT0

#define R_DMI_PME_CTL3                      0x434          ///< PCIe PM Extension Control 3
 #define B_DMI_PME_CTL3_L1PGAUTOPGEN        BIT4
 #define B_DMI_PME_CTL3_OSCCGH              BIT3 | BIT2
 #define N_DMI_PME_CTL3_OSCCGH              2
 #define V_DMI_PME_CTL3_OSCCGH              0x1
 #define B_DMI_PME_CTL3_PMREQCPGEXH         BIT1 | BIT0
 #define N_DMI_PME_CTL3_PMREQCPGEXH         0
 #define V_DMI_PME_CTL3_PMREQCPGEXH         0x0

#define R_DMI_IORCCP1                     0x44c      ///< IO Recal Control Policy 1
 #define B_DMI_IORCCP1_DISORCL12REC       BIT0             ///< Disable Offset Re-Calibration Request from L1 to Recovery
 #define B_DMI_IORCCP1_DRCORRP            BIT1             ///< Disable Recovery / Configurations Offset Re-calibration Request Policy
 #define B_DMI_IORCCP1_DISORCRODI         BIT2             ///< Disable Offset Re-Calibration Request On Downconfigure / Inactive Lanes
 #define B_DMI_IORCCP1_ORTS               BIT3             ///< Offset Recalibrations Trigger Status
 #define B_DMI_IORCCP1_ORRPGM             ( BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 ) ///< Offset Re-Calibration Request Per Gen Mask
 #define N_DMI_IORCCP1_ORRPGM             8
 #define B_DMI_IORCCP1_G1ORRRXECC         ( BIT18 | BIT17 | BIT16 ) ///< Gen 1 Offset Re-Calibrations RX Error Collapsing Count
 #define N_DMI_IORCCP1_G1ORRRXECC         16
 #define B_DMI_IORCCP1_G2ORRRXECC         ( BIT21 | BIT20 | BIT19 ) ///< Gen 2 Offset Re-Calibrations RX Error Collapsing Count
 #define N_DMI_IORCCP1_G2ORRRXECC         19
 #define B_DMI_IORCCP1_G3ORRRXECC         ( BIT24 | BIT23 | BIT22 ) ///< Gen 3 Offset Re-Calibrations RX Error Collapsing Count
 #define N_DMI_IORCCP1_G3ORRRXECC         22
 #define B_DMI_IORCCP1_G4ORRRXECC         ( BIT27 | BIT26 | BIT25 ) ///< Gen 4 Offset Re-Calibrations RX Error Collapsing Count
 #define N_DMI_IORCCP1_G4ORRRXECC         25
 #define B_DMI_IORCCP1_G5ORRRXECC         ( BIT30 | BIT29 | BIT28 ) ///< Gen 5 Offset Re-Calibrations RX Error Collapsing Count
 #define N_DMI_IORCCP1_G5ORRRXECC         28


#define R_DMI_EQCFG1                        0x0450                      ///< Equalization Configuration 1
 #define B_DMI_EQCFG1_RTLEPCEB              BIT16                       ///< Remote Transmit Link Equalization Preset/Coefficient Evaluation Bypass
 #define  S_DMI_EQCFG1                      4
 #define  B_DMI_EQCFG1_REC                  0xFF000000
 #define  B_DMI_EQCFG1_REC_OFFSET           24
 #define  B_DMI_EQCFG1_REIFECE              BIT23
 #define  B_DMI_EQCFG1_LERSMIE              BIT21
 #define  B_DMI_EQCFG1_LEB                  BIT19
 #define  B_DMI_EQCFG1_LEP23B               BIT18
 #define  B_DMI_EQCFG1_LEP3B                BIT17
 #define  B_DMI_EQCFG1_RTPCOE               BIT15
 #define  B_DMI_EQCFG1_LERSCIE              BIT14
 #define  B_DMI_EQCFG1_HPCMQE               BIT13
 #define  B_DMI_EQCFG1_HAED                 BIT12
 #define  B_DMI_EQCFG1_RWTNEVE              BIT8
 #define  B_DMI_EQCFG1_RWTNEVE_OFFSET       8
 #define  B_DMI_EQCFG1_RWTNEVE_MASK         0x00000F00
 #define  V_DMI_EQCFG1_RWTNEVE_1US          1
 #define  B_DMI_EQCFG1_EQTS2IRRC            BIT7
 #define  B_DMI_EQCFG1_HAPCCPIE             BIT5
 #define  B_DMI_EQCFG1_MEQSMMFLNTL          BIT4
 #define  B_DMI_EQCFG1_MFLNTL               BIT2
 #define  B_DMI_EQCFG1_TUPP                 BIT1
 #define  B_DMI_EQCFG1_RUPP                 BIT0

#define  R_DMI_RTPCL1                       0x454
 #define  B_DMI_RTPCL1_PCM                  BIT31
 #define  B_DMI_RTPCL1_PCM_OFFSET           31
 #define  B_DMI_CFG_RTPCL1_RTPRECL0PL0     ( BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 ) ///< Remote Transmitter Pre-Cursor Coefficient List 0/Preset List 0
 #define  N_DMI_CFG_RTPCL1_RTPOSTCL0PL1    6

#define  R_DMI_FOMS                         0x464
 #define  B_DMI_FOMS_I                      (BIT31 | BIT30 | BIT29)
 #define  B_DMI_FOMS_I_OFFSET               29
 #define  B_DMI_FOMS_LN                     0x1F000000
 #define  B_DMI_FOMS_LN_OFFSET              24
 #define  B_DMI_FOMS_FOMSV                  0x00FFFFFF
 #define  B_DMI_FOMS_FOMSV0                 0x000000FF
 #define  B_DMI_FOMS_FOMSV0_OFFSET          0
 #define  B_DMI_FOMS_FOMSV1                 0x0000FF00
 #define  B_DMI_FOMS_FOMSV1_OFFSET          8
 #define  B_DMI_FOMS_FOMSV2                 0x00FF0000
 #define  B_DMI_FOMS_FOMSV2_OFFSET          16

#define  R_DMI_HAEQ                         0x468
 #define  B_DMI_HAEQ_HAPCCPI                (BIT31 | BIT30 | BIT29 | BIT28)
 #define  B_DMI_HAEQ_HAPCCPI_OFFSET         28
 #define  B_DMI_HAEQ_FOMEM                  0x0FF00000
 #define  B_DMI_HAEQ_FOMEM_OFFSET           20
 #define  B_DMI_HAEQ_MACFOMC                BIT19
 #define  B_DMI_HAEQ_SL                     (BIT18 | BIT17 | BIT16)
 #define  B_DMI_HAEQ_SL_OFFSET              16
 #define  B_DMI_HAEQ_DL                     0x0000FF00
 #define  B_DMI_HAEQ_DL_OFFSET              8
 #define  B_DMI_HAEQ_SFOMFM                 0x000000FF
 #define  B_DMI_HAEQ_SFOMFM_OFFSET          0

#define R_DMI_LTCO1                         0x0470                      ///< Local Transmitter Coefficient Override 1
#define R_DMI_LTCO2                         0x0474                      ///< Local Transmitter Coefficient Override 2
 #define B_DMI_L1357TCOE                    BIT25                       ///< Lane 1/3 Transmitter Coefficient Override Enable
 #define B_DMI_L0246TCOE                    BIT24                       ///< Lane 0/2 Transmitter Coefficient Override Enable
 #define B_DMI_L1357TPOSTCO                 0x00FC0000                  ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override mask
 #define N_DMI_L1357TPOSTCO                 18                          ///< Lane 1/3 Transmitter Post-Cursor Coefficient Override value offset
 #define B_DMI_L1357TPRECO                  0x0003F000                  ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override mask
 #define N_DMI_L1357TPRECO                  12                          ///< Lane 1/3 Transmitter Pre-Cursor Coefficient Override value offset
 #define B_DMI_L0246TPOSTCO                 0x00000FC0                  ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override mask
 #define N_DMI_L0246TPOSTCO                 6                           ///< Lane 0/2 Transmitter Post-Cursor Coefficient Override value offset
 #define B_DMI_L0246TPRECO                  0x0000003F                  ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override mask
 #define N_DMI_L0246TPRECO                  0                           ///< Lane 0/2 Transmitter Pre-Cursor Coefficient Override value offset

#define R_DMI_G3L0SCTL                      0x478
 #define B_DMI_G3L0SCTL_G3ASL0SPL_MASK      0xFF000000
 #define N_DMI_G3L0SCTL_G3ASL0SPL           24
 #define B_DMI_G3L0SCTL_G3UCNFTS_MASK       0x0000FF00
 #define N_DMI_G3L0SCTL_G3UCNFTS            8
 #define V_DMI_G3L0SCTL_G3UCNFTS            0x4B
 #define B_DMI_G3L0SCTL_G3CCNFTS_MASK       0xFF
 #define N_DMI_G3L0SCTL_G3CCNFTS            0
 #define V_DMI_G3L0SCTL_G3CCNFTS            0x4B

#define R_DMI_EQCFG2                        0x47C
 #define  B_DMI_EQCFG2_NTIC                 0xFF000000
 #define  B_DMI_EQCFG2_EMD                  BIT23
 #define  B_DMI_EQCFG2_NTSS                 (BIT22 | BIT21 | BIT20)
 #define  B_DMI_EQCFG2_PCET                 (BIT19 | BIT18 | BIT17 | BIT16)
 #define  B_DMI_EQCFG2_PCET_OFFSET          16
 #define  B_DMI_EQCFG2_HAPCSB               (BIT15 | BIT14 | BIT13 | BIT12)
 #define  B_DMI_EQCFG2_HAPCSB_OFFSET        12
 #define  B_DMI_EQCFG2_NTEME                BIT11
 #define  B_DMI_EQCFG2_MPEME                BIT10
 #define  B_DMI_EQCFG2_REWMETM              (BIT9 | BIT8)
 #define  B_DMI_EQCFG2_REWMETM_OFFSET       8
 #define  B_DMI_EQCFG2_REWMET               0xFF

#define  R_DMI_EQCFG4                       0x48C
 #define  B_DMI_EQCFG4_PX16GTSWLPCE         BIT27
 #define  B_DMI_EQCFG4_PX16GTSWLPCE_OFFSET  27
 #define  B_DMI_EQCFG4_PX16GTSWLPCE_MASK    (BIT29 | BIT28 | BIT27)
 #define  V_DMI_EQCFG4_PX16GTSWLPCE_5US     4
 #define  B_DMI_EQCFG4_PX8GTSWLPCE          BIT24
 #define  B_DMI_EQCFG4_PX8GTSWLPCE_OFFSET   24
 #define  B_DMI_EQCFG4_PX8GTSWLPCE_MASK     (BIT26 | BIT25 | BIT24)
 #define  V_DMI_EQCFG4_PX8GTSWLPCE_4US      3
 #define  B_DMI_EQCFG4_FOMSCP_OFFSET        21
 #define  B_DMI_EQCFG4_FOMSCP_MASK          (BIT23 | BIT22 | BIT21)
 #define  B_DMI_EQCFG4_PX16GREIC            BIT20
 #define  B_DMI_EQCFG4_PX16GLEP23B          BIT18
 #define  B_DMI_EQCFG4_PX16GLEP3B           BIT17
 #define  B_DMI_EQCFG4_PX16GLEPCEB          BIT16
 #define  B_DMI_EQCFG4_PX16GRTPCOE          BIT15
 #define  B_DMI_EQCFG4_PX16GRWTNEVE         BIT8
 #define  B_DMI_EQCFG4_PX16GRWTNEVE_OFFSET  8
 #define  B_DMI_EQCFG4_PX16GRWTNEVE_MASK    0x00000F00
 #define  V_DMI_EQCFG4_PX16GRWTNEVE_1US     1
 #define  V_DMI_EQCFG4_PX16GRWTNEVE_3US     3
 #define  B_DMI_EQCFG4_PX16GHAED            BIT12
 #define  B_DMI_EQCFG4_PX16GEQTS2IRRC       BIT7
 #define  B_DMI_EQCFG4_PX16GHAPCCPI         BIT3
 #define  B_DMI_EQCFG4_PX16GHAPCCPI_OFFSET  3
 #define  B_DMI_EQCFG4_PX16GHAPCCPI_MASK    (BIT6 | BIT5 | BIT4 | BIT3)
 #define  B_DMI_EQCFG4_PX16GHAPCCPIE        BIT2
 #define  B_DMI_EQCFG4_PX16GMEQSMMFLNTL     BIT1
 #define  B_DMI_EQCFG4_PX16GMFLNTL          BIT0

#define R_DMI_CTRL2                         0x4A4
 #define B_DMI_CTRL2_PMETOFD                BIT6

 #define  R_DMI_PX16GRTPCL1                 0x4DC
 #define  B_DMI_PX16GRTPCL1_PCM             BIT31
 #define  B_DMI_PX16GRTPCL1_PCM_OFFSET      31

#define  R_DMI_EQCFG5                       0x4F8
 #define  B_DMI_EQCFG5_NTIC                 BIT24
 #define  B_DMI_EQCFG5_NTIC_MASK            0xFF000000
 #define  B_DMI_EQCFG5_EMD                  BIT23
 #define  B_DMI_EQCFG5_PCET                 BIT16
 #define  B_DMI_EQCFG5_PCET_MASK            (BIT19 | BIT18 | BIT17 | BIT16)
 #define  B_DMI_EQCFG5_PCET_OFFSET          16
 #define  B_DMI_EQCFG5_HAPCSB               BIT12
 #define  B_DMI_EQCFG5_HAPCSB_OFFSET        12
 #define  B_DMI_EQCFG5_HAPCSB_MASK          (BIT15 | BIT14 | BIT13 | BIT12)
 #define  B_DMI_EQCFG5_NTEME                BIT11
 #define  B_DMI_EQCFG5_MPEME                BIT10

#define R_DMI_L0P0P1PCM                     0x0500                      ///< Lane 0 P0 And P1 Preset-Coefficient Mapping
#define R_DMI_L0P1P2P3PCM                   0x0504                      ///< Lane 0 P1, P2 And P3 Preset-Coefficient Mapping
#define R_DMI_L0P3P4PCM                     0x0508                      ///< Lane 0 P3 And P4 Preset-Coefficient Mapping
#define R_DMI_L0P5P6PCM                     0x050C                      ///< Lane 0 P5 And P6 Preset-Coefficient Mapping
#define R_DMI_L0P6P7P8PCM                   0x0510                      ///< Lane 0 P6, P7 And P8 Preset-Coefficient Mapping
#define R_DMI_L0P8P9PCM                     0x0514                      ///< Lane 0 P8 And P9 Preset-Coefficient Mapping
#define R_DMI_L0P10PCM                      0x0518                      ///< Lane 0 P10 Preset-Coefficient Mapping ()
#define R_DMI_L0LFFS                        0x051C                      ///< Lane 0 LF And FS

#define R_DMI_PX16GP0P1PCM                  0x0520                      ///< 16.0 GT/s P0 And P1 Preset-Coefficient Mapping
#define R_DMI_PX16GP1P2P3PCM                0x0524                      ///< 16.0 GT/s P1, P2 And P3 Preset-Coefficient Mapping
#define R_DMI_PX16GP3P4PCM                  0x0528                      ///< 16.0 GT/s P3 And P4 Preset-Coefficient Mapping
#define R_DMI_PX16GP5P6PCM                  0x052C                      ///< 16.0 GT/s P5 And P6 Preset-Coefficient Mapping
#define R_DMI_PX16GP6P7P8PCM                0x0530                      ///< 16.0 GT/s P6, P7 And P8 Preset-Coefficient Mapping
#define R_DMI_PX16GP8P9PCM                  0x0534                      ///< 16.0 GT/s P8 And P9 Preset-Coefficient Mapping
#define R_DMI_PX16GP10PCM                   0x0538                      ///< 16.0 GT/s P10 Preset-Coefficient Mapping
#define R_DMI_PX16GLFFS                     0x053C                      ///< 16.0 GT/s LF And FS

#define R_DMI_COCTL                         0x594
 #define B_DMI_COCTL_CHAINBARBE             BIT18
 #define B_DMI_COCTL_NPCLM                  BIT16 | BIT15
 #define N_DMI_COCTL_NPCLM                  15
 #define V_DMI_COCTL_NPCLM                  0x2
 #define B_DMI_COCTL_PCLM                   BIT14 | BIT13
 #define N_DMI_COCTL_PCLM                   13
 #define V_DMI_COCTL_PCLM                   0x2
 #define B_DMI_COCTL_ROAOP                  BIT11
 #define B_DMI_COCTL_CTE                    BIT10
 #define B_DMI_COCTL_CT                     BIT2
 #define B_DMI_COCTL_CT_MASK                0x3FC
 #define N_DMI_COCTL_CT                     BIT2
 #define V_DMI_COCTL_CT                     0x03
 #define B_DMI_COCTL_DDCE                   BIT1
 #define B_DMI_COCTL_PWCE                   BIT0

#define R_DMI_LTCO3                         0x0598                      ///< Local Transmitter Coefficient Override 3
#define R_DMI_LTCO4                         0x059C                      ///< Local Transmitter Coefficient Override 4

#define R_DMI_ADVMCTRL                      0x5BC                       ///< Hardware Save and Restore 367
 #define B_DMI_ADVMCTRL_F10BTSE             BIT24
 #define B_DMI_ADVMCTRL_CCBE                BIT23
 #define B_DMI_ADVMCTRL_INRXL0CTRL          BIT22
 #define B_DMI_ADVMCTRL_ACCRM               BIT21
 #define B_DMI_ADVMCTRL_EIOSDISDS           BIT20
 #define B_DMI_ADVMCTRL_EIOSMASKRX          BIT19
 #define B_DMI_ADVMCTRL_PMREQCWC            BIT18 | BIT17 | BIT16
 #define N_DMI_ADVMCTRL_PMREQCWC            16
 #define V_DMI_ADVMCTRL_PMREQCWC            0x6
 #define B_DMI_ADVMCTRL_RXL0DC              BIT15
 #define B_DMI_ADVMCTRL_G3STFER             BIT13
 #define B_DMI_ADVMCTRL_RRLLCL              BIT11
 #define B_DMI_ADVMCTRL_RLLG12R             BIT10
 #define B_DMI_ADVMCTRL_PMREQBLKPGRSPT      BIT7 | BIT6 | BIT5
 #define N_DMI_ADVMCTRL_PMREQBLKPGRSPT      5
 #define V_DMI_ADVMCTRL_PMREQBLKPGRSPT      0x2

#define R_DMI_PGTHRES                       0x5C0
 #define B_DMI_PGTHRES_L1PGLTREN            BIT0
 #define B_DMI_PGTHRES_L1PGLTRTLSV          BIT31 | BIT30 | BIT29
 #define N_DMI_PGTHRES_L1PGLTRTLSV          29
 #define V_DMI_PGTHRES_L1PGLTRTLSV          0x2
 #define B_DMI_PGTHRES_L1PGLTRTLV_MASK      0x03FF0000
 #define N_DMI_PGTHRES_L1PGLTRTLV           16
 #define V_DMI_PGTHRES_L1PGLTRTLV           0x32

#define R_DMI_HWSNR                         0x5F0                      ///< Hardware Save and Restore
 #define B_DMI_HWSNR_BEPW                   BIT3 | BIT2 | BIT1 | BIT0
 #define N_DMI_HWSNR_BEPW                   0
 #define V_DMI_HWSNR_BEPW                   0x5
 #define B_DMI_HWSNR_REPW                   BIT7 | BIT6 | BIT5 | BIT4
 #define N_DMI_HWSNR_REPW                   4
 #define V_DMI_HWSNR_REPW                   0x1
 #define B_DMI_HWSNR_EEH                    BIT9 | BIT8
 #define N_DMI_HWSNR_EEH                    8
 #define V_DMI_HWSNR_EEH                    0x2

#define R_DMI_PGCTRL                        0x5F4                      ///< Power Gating Control
 #define B_DMI_PGCTRL_PMREQBLKRSPT          BIT2 | BIT1 | BIT0
 #define N_DMI_PGCTRL_PMREQBLKRSPT          0
 #define V_DMI_PGCTRL_PMREQBLKRSPT          0x2

#define  R_DMI_PX16GLTCO1                   0x600
 #define  B_DMI_PX16GLTCO1_L1357TCOE        BIT25
 #define  B_DMI_PX16GLTCO1_L0246TCOE        BIT24

#define R_DMI_ACRG3                         0x6CC  ///< Advance Control Register Group 3
 #define B_DMI_ACRG3_RRXC                   BIT30 | BIT29 | BIT28
 #define B_DMI_ACRG3_RRXDME                 BIT27
 #define B_DMI_ACRG3_CBGM                   BIT21
 #define B_DMI_ACRG3_ADESKEW_DIS            BIT10

#define R_DMI_RSPVCC                        0x860
 #define B_DMI_RSPVCC_RS3VCCP               BIT2

#define R_DMI_RS3V0CTL                      0x974
 #define B_DMI_RS3V0CTL_TVM                 0xFE
 #define N_DMI_RS3V0CTL_TVM                 1

#define R_DMI_RS3V1CTL                      0x978
 #define B_DMI_RS3V1CTL_EN                  BIT31
 #define B_DMI_RS3V1CTL_TVM                 0xFE
 #define V_DMI_RS3V1CTL_TVM                 0x7F
 #define N_DMI_RS3V1CTL_TVM                 1

#define  R_DMI_DPCCAPR                      0xA04
 #define  B_DMI_DPCCAPR_RPEFDPC             BIT5

#define  R_DMI_LCTL3                        0xA34
 #define  B_DMI_LCTL3_PE                    BIT0
 #define  B_DMI_LCTL3_PE_OFFSET             0

#define R_DMI_CFG_MM                         0x00000480U      ///< Monitor Mux
#define B_DMI_CFG_MM_MSS                     ( BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 ) ///< Monitor Signal Select
#define N_DMI_CFG_MM_MSS                     0
#define B_DMI_CFG_MM_MSST                    0xffffff00U      ///< Monitor Signal State
#define N_DMI_CFG_MM_MSST                    8

#define R_DMI_CFG_CDM                        0x00000484U      ///< Controller Debug And Monitor
#define B_DMI_CFG_CDM_MCS                    ( BIT3 | BIT2 | BIT1 | BIT0 ) ///< Monitor Category Select
#define N_DMI_CFG_CDM_MCS                    0
#define B_DMI_CFG_CDM_MSS                    ( BIT7 | BIT6 | BIT5 | BIT4 ) ///< Monitor Segment Select
#define N_DMI_CFG_CDM_MSS                    4

//
// Monitor signals
//
#define R_DMI_SIP_MONITOR_CAT2                       0x2
#define R_DMI_SIP_MONITOR_CAT2_BEST4COEFF_GEN3       0xF
#define R_DMI_SIP_MONITOR_CAT2_BEST4COEFF_GEN4       0x10
#define R_DMI_SIP_MONITOR_CAT2_BEST4COEFF_GEN5       0x11

#define R_DMI_L01EC                         0x0A3C                      ///< Lane 0 and Lane 1 Equalization Control
#define R_DMI_L23EC                         0x0A40                      ///< Lane 2 and Lane 3 Equalization Control
#define R_DMI_L45EC                         0x0A44                      ///< Lane 4 and Lane 5 Equalization Control
#define R_DMI_L67EC                         0x0A48                      ///< Lane 6 and Lane 7 Equalization Control
 #define  B_DMI_L01EC_DPL0246TP             BIT0
 #define  B_DMI_L01EC_DPL0246TP_OFFSET      0
 #define  B_DMI_L01EC_DPL0246TP_MASK        0xF
 #define  B_DMI_L01EC_DPL0246RPH            BIT4
 #define  B_DMI_L01EC_DPL0246RPH_OFFSET     4
 #define  B_DMI_L01EC_DPL0246RPH_MASK       0x70
 #define  B_DMI_L01EC_DPL1357TP             BIT16
 #define  B_DMI_L01EC_DPL1357TP_OFFSET      16
 #define  B_DMI_L01EC_DPL1357TP_MASK        0xF0000
 #define  B_DMI_L01EC_DPL1357RPH            BIT20
 #define  B_DMI_L01EC_DPL1357RPH_OFFSET     20
 #define  B_DMI_L01EC_DPL1357RPH_MASK       0x700000

 #define  B_DMI_L01EC_UPL0246TP             BIT8
 #define  B_DMI_L01EC_UPL0246TP_OFFSET      8
 #define  B_DMI_L01EC_UPL0246TP_MASK        0xF00
 #define  B_DMI_L01EC_UPL0246RPH            BIT12
 #define  B_DMI_L01EC_UPL0246RPH_OFFSET     12
 #define  B_DMI_L01EC_UPL0246RPH_MASK       0x7000
 #define  B_DMI_L01EC_UPL1357TP             BIT24
 #define  B_DMI_L01EC_UPL1357TP_OFFSET      24
 #define  B_DMI_L01EC_UPL1357TP_MASK        0xF000000
 #define  B_DMI_L01EC_UPL1357RPH            BIT28
 #define  B_DMI_L01EC_UPL1357RPH_OFFSET     28
 #define  B_DMI_L01EC_UPL1357RPH_MASK       0x70000000

#define  R_DMI_PL16L01EC                     0xABC                      ///< Physical Layer 16.0 GT/s Lane 01 Equalization Control Register
#define  R_DMI_PL16L23EC                     0xABE                      ///< Physical Layer 16.0 GT/s Lane 23 Equalization Control Register
#define  R_DMI_PL16L45EC                     0xAC0                      ///< Physical Layer 16.0 GT/s Lane 45 Equalization Control Register
#define  R_DMI_PL16L67EC                     0xAC2                      ///< Physical Layer 16.0 GT/s Lane 67 Equalization Control Register
 #define  B_DMI_PL16L01EC_UP16L1357TP_MASK   0x0000F000
 #define  B_DMI_PL16L01EC_UP16L1357TP        BIT12
 #define  B_DMI_PL16L01EC_UP16L1357TP_OFFSET 12
 #define  B_DMI_PL16L01EC_DP16L1357TP_MASK   0x00000F00
 #define  B_DMI_PL16L01EC_DP16L1357TP        BIT8
 #define  B_DMI_PL16L01EC_DP16L1357TP_OFFSET 8
 #define  B_DMI_PL16L01EC_UP16L0246TP_MASK   0xF0
 #define  B_DMI_PL16L01EC_UP16L0246TP        BIT4
 #define  B_DMI_PL16L01EC_UP16L0246TP_OFFSET 4
 #define  B_DMI_PL16L01EC_DP16L0246TP        BIT0
 #define  B_DMI_PL16L01EC_DP16L0246TP_OFFSET 0
 #define  B_DMI_PL16L01EC_DP16L0246TP_MASK   0xF

#define R_DMI_ACGR3S2                        0xC50
 #define B_DMI_ACGR3S2_G4EBM                 BIT9
 #define B_DMI_ACGR3S2_G3EBM                 BIT8
 #define B_DMI_ACGR3S2_G2EBM                 BIT7
 #define B_DMI_ACGR3S2_G1EBM                 BIT6
 #define B_DMI_ACGR3S2_SRT                   BIT5
 #define B_DMI_ACGR3S2_LSTPTLS               BIT0
 #define V_DMI_ACGR3S2_LSTPTLS               0x1
 #define B_DMI_ACGR3S2_LSTPTLS_OFFSET        0

#define R_DMI_LTRSUBL12                      0xC64
 #define B_DMI_LTRSUBL12_LTRNSLSUBEN         BIT31
 #define B_DMI_LTRSUBL12_LTRNSLSSUBV         (BIT28 | BIT27 | BIT26)
 #define B_DMI_LTRSUBL12_LTRNSLSSUBV_OFFSET  26
 #define V_DMI_LTRSUBL12_LTRNSLSSUBV         0x2
 #define B_DMI_LTRSUBL12_LTRNSLSUBV_OFFSET   16
 #define B_DMI_LTRSUBL12_LTRNSLSUBV_MASK     0x03FF0000
 #define V_DMI_LTRSUBL12_LTRNSLSUBV          0x82
 #define B_DMI_LTRSUBL12_LTRSLSUBEN          BIT15
 #define B_DMI_LTRSUBL12_LTRSLSSUBV          (BIT12 | BIT11 | BIT10)
 #define B_DMI_LTRSUBL12_LTRSLSSUBV_OFFSET   10
 #define V_DMI_LTRSUBL12_LTRSLSSUBV          0x2
 #define B_DMI_LTRSUBL12_LTRSLSUBV_OFFSET    0
 #define B_DMI_LTRSUBL12_LTRSLSUBV_MASK      0x000003FF
 #define V_DMI_LTRSUBL12_LTRSLSUBV           0x82

#define R_DMI_LTRSUBL1STD                     0xC5C
 #define B_DMI_LTRSUBL1STD_LTRNSLSUBEN        BIT31
 #define B_DMI_LTRSUBL1STD_LTRNSLSSUBV        (BIT28 | BIT27 | BIT26)
 #define B_DMI_LTRSUBL1STD_LTRNSLSSUBV_OFFSET 26
 #define V_DMI_LTRSUBL1STD_LTRNSLSSUBV        0x2
 #define B_DMI_LTRSUBL1STD_LTRNSLSUBV_OFFSET  16
 #define B_DMI_LTRSUBL1STD_LTRNSLSUBV_MASK    0x03FF0000
 #define V_DMI_LTRSUBL1STD_LTRNSLSUBV         0x41
 #define B_DMI_LTRSUBL1STD_LTRSLSUBEN         BIT15
 #define B_DMI_LTRSUBL1STD_LTRSLSSUBV         (BIT12 | BIT11 | BIT10)
 #define B_DMI_LTRSUBL1STD_LTRSLSSUBV_OFFSET  10
 #define V_DMI_LTRSUBL1STD_LTRSLSSUBV         0x2
 #define B_DMI_LTRSUBL1STD_LTRSLSUBV          BIT0
 #define B_DMI_LTRSUBL1STD_LTRSLSUBV_OFFSET   0
 #define B_DMI_LTRSUBL1STD_LTRSLSUBV_MASK     0x000003FF
 #define V_DMI_LTRSUBL1STD_LTRSLSUBV          0x41

#define  R_DMI_LPCR                          0xC8C
 #define  B_DMI_LPCR_DIDOVR_LOCK             BIT24
 #define  B_DMI_LPCR_LTRCFGLOCK              BIT16
 #define  B_DMI_LPCR_SERL                    BIT8
 #define  B_DMI_LPCR_SRL                     BIT0

#define  R_DMI_AECR1G3                       0xC80
 #define B_DMI_AECR1G3_DCDCCTDT              BIT0             ///< Disable Config.Dskew to Config.Complete transition delay timer
 #define N_DMI_AECR1G3_DCDCCTDT              0
 #define  B_DMI_AECR1G3_TPSE                 BIT10
 #define  B_DMI_AECR1G3_DTCGCM               BIT14
 #define  B_DMI_AECR1G3_REQTMBOF             BIT31

#define R_DMI_LPCRE                          0xC88      ///< Lock Policy Control Register Extension
 #define B_DMI_LPCRE_IPVCCAPL                BIT8       ///< IP VC Capability Lock

#define  R_DMI_PL16MPCPS                     0xCE4
 #define  B_DMI_PL16MPCPS_MARGINDRISW        BIT0

#define  R_DMI_RXMC1                         0xC90
 #define  B_DMI_RXMC1_MSRVS_OFFSET           26
 #define  B_DMI_RXMC1_MSRVS_MASK             0xFC000000
 #define  V_DMI_RXMC1_MSRVS                  0x3F
 #define  B_DMI_RXMC1_MSRTS_OFFSET           20
 #define  B_DMI_RXMC1_MSRTS_MASK             0x3F00000
 #define  V_DMI_RXMC1_MSRTS                  0x3F
 #define  B_DMI_RXMC1_TMSLOP_OFFSET          13
 #define  B_DMI_RXMC1_TMSLOP_MASK            0x6000
 #define  V_DMI_RXMC1_TMSLOP                 0x1
 #define  B_DMI_RXMC1_VMSLOP_OFFSET          11
 #define  B_DMI_RXMC1_VMSLOP_MASK            0x1800
 #define  V_DMI_RXMC1_VMSLOP                 0x1
 #define  B_DMI_RXMC1_MSRM                   BIT9
 #define  B_DMI_RXMC1_MMNOLS_OFFSET          4
 #define  B_DMI_RXMC1_MMNOLS_MASK            0x1F0
 #define  V_DMI_RXMC1_MMNOLS                 0x0
 #define  B_DMI_RXMC1_MVS                    BIT3
 #define  B_DMI_RXMC1_MILRTS                 BIT2
 #define  B_DMI_RXMC1_MIUDVMS                BIT1
 #define  B_DMI_RXMC1_MIESS                  BIT0

#define  R_DMI_RXMC2                         0xC94
 #define  B_DMI_RXMC2_MNOTSS_OFFSET          19
 #define  B_DMI_RXMC2_MNOTSS_MASK            0x1F80000
 #define  V_DMI_RXMC2_MNOTSS                 0x09
 #define  B_DMI_RXMC2_MMTOS_OFFSET           13
 #define  B_DMI_RXMC2_MMTOS_MASK             0x7E000
 #define  V_DMI_RXMC2_MMTOS                  0x14
 #define  B_DMI_RXMC2_MNOVSS_OFFSET          6
 #define  B_DMI_RXMC2_MNOVSS_MASK            0x1FC0
 #define  V_DMI_RXMC2_MNOVSS                 0x7F
 #define  B_DMI_RXMC2_MMVOS_OFFSET           0
 #define  B_DMI_RXMC2_MMVOS_MASK             0x3F
 #define  V_DMI_RXMC2_MMVOS                  0xC

#define R_DMI_EINJCTL                        0xCA8            ///< EINJ Control
 #define B_DMI_EINJCTL_EINJDIS               BIT0             ///< Error Injection Disable

#define R_DMI_MEMBARCTL                      0xF00
 #define B_DMI_MEMBARCTL_MEMBAREN            BIT0

#define R_DMI_FCTL                           0x300
 #define B_DMI_FCTL_CRSPSEL                  BIT12
 #define B_DMI_FCTL_CTM                      BIT18 | BIT17
 #define N_DMI_FCTL_CTM                      17
 #define V_DMI_FCTL_CTM                      0x2

#define R_DMI_TXCRSTOCTL                     0x320
 #define B_DMI_TXCRSTOCTL_TXNPCTODIS         BIT3
 #define B_DMI_TXCRSTOCTL_CRS_TO_DIS         BIT15

#define R_DMI_LLPC                           0x32C  ///< Link Layer Policy Control
#define B_DMI_LLPC_AL1NTP                    BIT6    ///< ASPM L1 Nak latency Timer Policies

#define R_DMI_FCTL2                          0x330
 #define B_DMI_FCTL2_RXCPPREALLOCEN          BIT27
 #define B_DMI_FCTL2_HRTCTL                  BIT9 | BIT8 | BIT7
 #define B_DMI_FCTL2_HRTCTL_OFFSET           7
 #define V_DMI_FCTL2_HRTCTL                  0x1
 #define B_DMI_FCTL2_RPTOT                   BIT6 | BIT5 | BIT4
 #define B_DMI_FCTL2_RPTOT_OFFSET            4
 #define V_DMI_FCTL2_RPTOT                   0x5

#define  R_DMI_DCGEN1                     0x350
 #define B_DMI_DCGEN1_PXKGULDCGEN         BIT0             ///< PXKG Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXCULDCGEN          BIT1             ///< PXC Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXLRULDCGEN         BIT8             ///< PXLR Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXLTULDCGEN         BIT9             ///< PXLT Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXLIULDCGEN         BIT10            ///< PXLI Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXLSULDCGEN         BIT11            ///< PXLS Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXTRULDCGEN         BIT16            ///< PXTR Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXTRSULDCGEN        BIT17            ///< PXTRS Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXTTULDCGEN         BIT24            ///< PXTT Unit Link Clock DCG Enable
 #define B_DMI_DCGEN1_PXTTSULDCGEN        BIT25            ///< PXTTS Unit Link Clock PXTTSS DCG Enable

#define R_DMI_DCGEN2                      0x354      ///< DCG Enable 2
 #define B_DMI_DCGEN2_PXPIULDCGEN         BIT0             ///< PXPI Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXPSULDCGEN         BIT1             ///< PXPS Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXPBULDCGEN         BIT2             ///< PXPB Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXFIULDCGEN         BIT3             ///< PXFI Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXFTULDCGEN         BIT4             ///< PXFT Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXFRULDCGEN         BIT5             ///< PXFR Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXCP5ULDCGEN        BIT6             ///< PXCP5 Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXSDIULDCGEN        BIT15            ///< PXSDI Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXSDRULDCGEN        BIT17            ///< PXSDR Unit Link Clock DCG Enable
 #define B_DMI_DCGEN2_PXSDTULDCGEN        BIT16            ///< PXSDT Unit Link Clock DCG Enable

#define  R_DMI_DCGM1                      0x358
 #define B_DMI_DCGM1_PXKGULDCGM           BIT0             ///< PXKG Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXCULDCGM            BIT1             ///< PXC Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXLRULDCGM           BIT8             ///< PXLR Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXLTULDCGM           BIT9             ///< PXLT Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXLIULDCGM           BIT10            ///< PXLI Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXLSULDCGM           BIT11            ///< PXLS Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXCP2ULDCGM          BIT12            ///< PXCP2 Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTRULDCGM           BIT16            ///< PXTR Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTRSULDCGM          BIT17            ///< PXTRS Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTRSSULDCGM         BIT18            ///< PXTRSS Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXCP3ULDCGM          BIT19            ///< PXCP3 Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTTULDCGM           BIT24            ///< PXCP3 Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTTSULDCGM          BIT25            ///< PXTTS Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXTTSSULDCGM         BIT26            ///< PXTTS Unit Link Clock DCG Mode
 #define B_DMI_DCGM1_PXCP4ULDCGM          BIT27            ///< PXCP4 Unit Link Clock DCG Mode

#define R_DMI_DCGM2                       0x35c
 #define B_DMI_DCGM2_PXPIULDCGM           BIT0             ///< PXPI Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXPSULDCGM           BIT1             ///< PXPS Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXPBULDCGM           BIT2             ///< PXPB Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXFIULDCGM           BIT3             ///< PXFI Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXFTULDCGM           BIT4             ///< PXFT Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXFRULDCGM           BIT5             ///< PXFR Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXCP5ULDCGM          BIT6             ///< PXCP5 Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXSDIULDCGM          BIT15            ///< PXSDI Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXSDRULDCGM          BIT17            ///< PXSDR Unit Link Clock DCG Mode
 #define B_DMI_DCGM2_PXSDTULDCGM          BIT16            ///< PXSDT Unit Link Clock DCG Mode

#define R_DMI_DCGEN3                      0x360
 #define B_DMI_DCGEN3_PXEUPDCGEN          BIT0             ///< PXE Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXBUPDCGEN          BIT1             ///< PXB Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXCUPDCGEN          BIT2             ///< PXC Unit Prim Port Clock DCG Enable
 #define B_DMI_DCGEN3_PXCUPSRCDCGEN       BIT4             ///< PXC Unit Prim Shared Resource Clock DCG Enable
 #define B_DMI_DCGEN3_PXCUPSNRDCGEN       BIT5             ///< PXC Unit Prim SnR DCG Enable
 #define B_DMI_DCGEN3_PXSRUSSNRDCGEN      BIT6             ///< PXSR Unit Side SnR DCG Enable
 #define B_DMI_DCGEN3_PXLRUPDCGEN         BIT8             ///< PXLR Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXLTUPDCGEN         BIT9             ///< PXLT Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXLIUPDCGEN         BIT10            ///< PXLI Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXCP2UPDCGEN        BIT12            ///< PXCP2 Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTRUPDCGEN         BIT16            ///< PXTR Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTRSUPDCGEN        BIT17            ///< PXTRS Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTRSSUPDCGEN       BIT18            ///< PXTRSS Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTOUPDCGEN         BIT19            ///< PXTO Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXCP3UPDCGEN        BIT20            ///< PXCP3 Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTTUPDCGEN         BIT24            ///< PXTT Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTTSUPDCGEN        BIT25            ///< PXTTS Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXTTSSUPDCGEN       BIT26            ///< PXTTSS Unit Prim Clock DCG Enable
 #define B_DMI_DCGEN3_PXCP4UPDCGEN        BIT27            ///< PXCP4 Unit Prim Clock DCG Enable

#define R_DMI_DCGM3                       0x368      ///< DCG Mode 3
 #define B_DMI_DCGM3_PXEUPDCGM            BIT0             ///< PXE Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXBUPDCGM            BIT1             ///< PXB Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXCUPDCGM            BIT2             ///< PXC Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXCUPSRCDCGM         BIT4             ///< PXC Unit Prim Shared Resource Clock DCG Mode
 #define B_DMI_DCGM3_PXCUPSNRDCGM         BIT5             ///< PXC Unit Prim SnR DCG Mode
 #define B_DMI_DCGM3_PXSRUSSNRDCGM        BIT6             ///< PXSR Unit Side SnR DCG Mode
 #define B_DMI_DCGM3_PXLRUPDCGM           BIT8             ///< PXLR Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXLTUPDCGM           BIT9             ///< PXLT Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXLIUPDCGM           BIT10            ///< PXLI Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTRUPDCGM           BIT16            ///< PXTR Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTRSUPDCGM          BIT17            ///< PXTRS Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTRSSUPDCGM         BIT18            ///< PXTRSS Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTOUPDCGM           BIT19            ///< PXTO Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTTUPDCGM           BIT24            ///< PXTT Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTTSUPDCGM          BIT25            ///< PXTTS Unit Prim Clock DCG Mode
 #define B_DMI_DCGM3_PXTTSSUPDCGM         BIT26            ///< PXTTSS Unit Prim Clock DCG Mode


#define R_DMI_IPCLKCTR                      0x370
  #define B_DMI_IPCLKCTR_MDPCCEN            BIT3

#define  R_DMI_RXL0SESQCP1                   0x420
 #define  B_DMI_RXL0SESQCP1_SSMFRXL0S        BIT1 | BIT0
 #define  V_DMI_RXL0SESQCP1_SSMFRXL0S        0x0

#define  R_DMI_RXL0SESQCP4                   0x42C
 #define B_DMI_RXL0SESQCP4_G1LBWSST          ( BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 ) ///< Gen 1 Low Bandwidth Squelch Settling Timer
 #define N_DMI_RXL0SESQCP4_G1LBWSST          0
 #define V_DMI_RXL0SESQCP4_G1LBWSST          0x18
 #define B_DMI_RXL0SESQCP4_G2LBWSST          ( BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 ) ///< Gen 2 Low Bandwidth Squelch Settling Timer
 #define N_DMI_RXL0SESQCP4_G2LBWSST          8
 #define V_DMI_RXL0SESQCP4_G2LBWSST          0xC
 #define B_DMI_RXL0SESQCP4_G3LBWSST          ( BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16 ) ///< Gen 3 Low Bandwidth Squelch Settling Timer
 #define N_DMI_RXL0SESQCP4_G3LBWSST          16
 #define V_DMI_RXL0SESQCP4_G3LBWSST          0x14
 #define B_DMI_RXL0SESQCP4_G4LBWSST          ( BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24 ) ///< Gen 4 Low Bandwidth Squelch Settling Timer
 #define N_DMI_RXL0SESQCP4_G4LBWSST          24
 #define V_DMI_RXL0SESQCP4_G4LBWSST          0x4

#define R_DMI_LPHYCP4                        0x434
 #define B_DMI_LPHYCP4_LGCLKSQEXITDBTIMERS   ( BIT2 | BIT1 | BIT0 ) ///< Link Clock Domain Squelch Exit Debounce Timers
 #define N_DMI_LPHYCP4_LGCLKSQEXITDBTIMERS   0
 #define V_DMI_LPHYCP4_LGCLKSQEXITDBTIMERS   0x2
 #define B_DMI_LPHYCP4_OSCCLKSQEXITDBTIMERS  ( BIT5 | BIT4 | BIT3 ) ///< OSC Clock Domain Squelch Exit Debounce Timers
 #define N_DMI_LPHYCP4_OSCCLKSQEXITDBTIMERS  3
 #define V_DMI_LPHYCP4_OSCCLKSQEXITDBTIMERS  0x2
 #define B_DMI_LPHYCP4_LGUSSP                ( BIT7 | BIT6 )  ///< LG Clock Un-Squelch Sampling Period
 #define N_DMI_LPHYCP4_LGUSSP                6
 #define V_DMI_LPHYCP4_LGUSSP                0x1
 #define B_DMI_LPHYCP4_OSUSSP                ( BIT9 | BIT8 )  ///< OSC Clock Un-Squelch Sampling Period
 #define N_DMI_LPHYCP4_OSUSSP                8
 #define V_DMI_LPHYCP4_OSUSSP                0x2
 #define B_DMI_LPHYCP4_PLLBUSDRC             BIT10
 #define B_DMI_LPHYCP4_REEFRXL0SED           BIT11
 #define B_DMI_LPHYCP4_DPMDNDBFE             BIT12
 #define B_DMI_LPHYCP4_TLSPDRS               BIT13
 #define B_DMI_LPHYCP4_G1AREDRL0S            BIT16
 #define B_DMI_LPHYCP4_G2AREDRL0S            BIT17
 #define B_DMI_LPHYCP4_G3AREDRL0S            BIT18
 #define B_DMI_LPHYCP4_G4AREDRL0S            BIT19
 #define B_DMI_LPHYCP4_RXL0SEG12FTSE         BIT25 | BIT24
 #define N_DMI_LPHYCP4_RXL0SEG12FTSE         24
 #define V_DMI_LPHYCP4_RXL0SEG12FTSE         0x2
 #define B_DMI_LPHYCP4_RXVLDRXL0SSP          BIT28 | BIT27 | BIT26
 #define N_DMI_LPHYCP4_RXVLDRXL0SSP          26
 #define V_DMI_LPHYCP4_RXVLDRXL0SSP          0x1

#define R_DMI_PHYPG                          0x590      ///< PHY Power Gating
#define B_DMI_PHYPG_DETPHYPGE                BIT0             ///< Detect PHY Power Gating Enable
#define B_DMI_PHYPG_DISPHYPGE                BIT1             ///< Disabled PHY Power Gating Enable
#define B_DMI_PHYPG_L23PHYPGE                BIT2             ///< L23 PHY Power Gating Enable
#define B_DMI_PHYPG_DUCFGPHYPGE              BIT3             ///< Downconfigure PHY Power Gating Enable
#define B_DMI_PHYPG_DLPPGP                   BIT4             ///< Downconfigure Lanes Phy Power Gating Policy

#define R_DMI_PIPEPDCTL                     0x594                        ///< PIPE Power Down Control
 #define B_DMI_PIPEPDCTL_DETNOPGPDCTL       BIT3 | BIT2 | BIT1 | BIT0    ///< Detect without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_DETNOPGPDCTL       0
 #define B_DMI_PIPEPDCTL_DETPGPDCTL         BIT7 | BIT6 | BIT5 | BIT4    ///< Detect with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_DETPGPDCTL         4
 #define V_DMI_PIPEPDCTL_DETPGPDCTL         0xD
 #define B_DMI_PIPEPDCTL_L23NOPGPDCTL       BIT11 | BIT10 | BIT9 | BIT8  ///< L23 without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_L23NOPGPDCTL       8
 #define V_DMI_PIPEPDCTL_L23NOPGPDCTL       0x3
 #define B_DMI_PIPEPDCTL_L23PGPDCTL         BIT15 | BIT14 | BIT13 | BIT12 ///< L23 with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_L23PGPDCTL         12
 #define V_DMI_PIPEPDCTL_L23PGPDCTL         0xD
 #define B_DMI_PIPEPDCTL_DISNOPGPDCTL       BIT19 | BIT18 | BIT17 | BIT16 ///< Disabled without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_DISNOPGPDCTL       16
 #define V_DMI_PIPEPDCTL_DISNOPGPDCTL       0xD
 #define B_DMI_PIPEPDCTL_DISPGPDCTL         BIT23 | BIT22 | BIT21 | BIT20 ///< Disabled with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_DISPGPDCTL         20
 #define V_DMI_PIPEPDCTL_DISPGPDCTL         0xD
 #define B_DMI_PIPEPDCTL_L1PGNOPGPDCTL      BIT27 | BIT26 | BIT25 | BIT24 ///< L1 Powergateable without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_L1PGNOPGPDCTL      24
 #define V_DMI_PIPEPDCTL_L1PGNOPGPDCTL      0x4
 #define B_DMI_PIPEPDCTL_L1PGUPGPDCTL       BIT31 | BIT30 | BIT29 | BIT28 ///< L1 Powergateable with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL_L1PGUPGPDCTL       28
 #define V_DMI_PIPEPDCTL_L1PGUPGPDCTL       0x4

#define R_DMI_PIPEPDCTL2                    0x598                     ///< PIPE Power Down Control 2
 #define B_DMI_PIPEPDCTL2_L1UPNOPGPDCTL     BIT3 | BIT2 | BIT1 | BIT0 ///< L1 Unpowergateable without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1UPNOPGPDCTL     0
 #define V_DMI_PIPEPDCTL2_L1UPNOPGPDCTL     0x4
 #define B_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL    BIT7 | BIT6 | BIT5 | BIT4 ///< L1.1 Unpowergateable with Un Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL    4
 #define V_DMI_PIPEPDCTL2_L1D1UPUPGPDCTL    0x4
 #define B_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL   BIT11 | BIT10 | BIT9 | BIT8 ///< L1.1 Powergateable without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL   8
 #define V_DMI_PIPEPDCTL2_L1D1PGNOPGPDCTL   0x4
 #define B_DMI_PIPEPDCTL2_L1D1PGPGPDCTL     BIT15 | BIT14 | BIT13 | BIT12 ///< L1.1 Powergateable with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1D1PGPGPDCTL     12
 #define V_DMI_PIPEPDCTL2_L1D1PGPGPDCTL     0x4
 #define B_DMI_PIPEPDCTL2_L1D2NOPGPDCTL     BIT19 | BIT18 | BIT17 | BIT16 ///< L1.2 without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1D2NOPGPDCTL     16
 #define V_DMI_PIPEPDCTL2_L1D2NOPGPDCTL     0x4
 #define B_DMI_PIPEPDCTL2_L1D2PGPDCTL       BIT23 | BIT22 | BIT21 | BIT20 ///< L1.2 with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_L1D2PGPDCTL       20
 #define V_DMI_PIPEPDCTL2_L1D2PGPDCTL       0x4
 #define B_DMI_PIPEPDCTL2_DUCFGNOPGPDCTL    BIT27 | BIT26 | BIT25 | BIT24 ///< Down and Unconfigure without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_DUCFGNOPGPDCTL    24
 #define B_DMI_PIPEPDCTL2_DUCFGPGPDCTL      BIT31 | BIT30 | BIT29 | BIT28 ///< Down and Unconfigure with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL2_DUCFGPGPDCTL      28
 #define V_DMI_PIPEPDCTL2_DUCFGPGPDCTL      0xD

#define R_DMI_PIPEPDCTL3                    0x59c                     ///< PIPE Power Down Control 3
 #define B_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL   BIT3 | BIT2 | BIT1 | BIT0 ///< L1.Low without Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL   0
 #define V_DMI_PIPEPDCTL3_L1DLOWNOPGPDCTL   4
 #define B_DMI_PIPEPDCTL3_L1DLOWPGPDCTL     BIT7 | BIT6 | BIT5 | BIT4 ///< L1.Low with Power Gating Power Down Control
 #define N_DMI_PIPEPDCTL3_L1DLOWPGPDCTL     4
 #define V_DMI_PIPEPDCTL3_L1DLOWPGPDCTL     4

#define R_DMI_PIPEPDCTLEXT                   0x5a0            ///< PIPE Power Down Control Extension
 #define B_DMI_PIPEPDCTLEXT_P2TP2TCD         BIT1             ///< P2 to P2 Transition Clock Domain
 #define N_DMI_PIPEPDCTLEXT_P2TP2TCD         1
 #define B_DMI_PIPEPDCTLEXT_P2TP2TP          BIT2             ///< P2 to P2 Transition Policy
 #define N_DMI_PIPEPDCTLEXT_P2TP2TP          2
 #define B_DMI_PIPEPDCTLEXT_P2UGTPGSM        BIT3             ///< P2 Unpower Gating To Power Gating Synchronize Mode
 #define N_DMI_PIPEPDCTLEXT_P2UGTPGSM        3
 #define B_DMI_PIPEPDCTLEXT_LSDPMRFM         BIT11 | BIT10 | BIT9 | BIT8 | BIT7 | BIT6 | BIT5 | BIT4 ///< Link State Deeppm Request Forwarding Mask
 #define N_DMI_PIPEPDCTLEXT_LSDPMRFM         4
 #define V_DMI_PIPEPDCTLEXT_LSDPMRFM         0xE
 #define B_DMI_PIPEPDCTLEXT_PDDPMRFM         0xffff0000U      ///< Powerdown Deeppm Request Forwarding Mask
 #define N_DMI_PIPEPDCTLEXT_PDDPMRFM         16
 #define V_DMI_PIPEPDCTLEXT_PDDPMRFM         0x17

#define R_DMI_DECCTL                         0x904
 #define  B_DMI_DECCTL_RXIMDECEN             BIT31
 #define  B_DMI_DECCTL_URRXMCTPNTCO          BIT30
 #define  B_DMI_DECCTL_RXMCTPBRCDECEN        BIT29
 #define  B_DMI_DECCTL_URRXUVDMRGRTRC        BIT28
 #define  B_DMI_DECCTL_URRXUVDMRBFRC         BIT27
 #define  B_DMI_DECCTL_LCRXPTMREQ            BIT26
 #define  B_DMI_DECCTL_RSVD_RW               BIT25
 #define  B_DMI_DECCTL_LCRXERRMSG            BIT24
 #define  B_DMI_DECCTL_RXLTRMDECH            BIT23
 #define  B_DMI_DECCTL_RXADLEDDECEN          BIT22
 #define  B_DMI_DECCTL_RXSBEMCAPDECEN        BIT21
 #define  B_DMI_DECCTL_RXGPEDECEN            BIT20
 #define  B_DMI_DECCTL_RXVDMDECE             BIT19
 #define  B_DMI_DECCTL_RXMCTPDECEN           BIT18
 #define  B_DMI_DECCTL_ICHKINVREQRMSGRBID    BIT17
 #define  B_DMI_DECCTL_URRXVMCTPBFRC         BIT16
 #define  B_DMI_DECCTL_URRXUORVDM            BIT15
 #define  B_DMI_DECCTL_URRXURIDVDM           BIT14
 #define  B_DMI_DECCTL_URRXURAVDM            BIT13
 #define  B_DMI_DECCTL_URRXULTVDM            BIT12
 #define  B_DMI_DECCTL_URRXURTRCVDM          BIT11
 #define  B_DMI_DECCTL_URRXUVDMUVID          BIT10
 #define  B_DMI_DECCTL_URRXUVDMINTELVID      BIT9
 #define  B_DMI_DECCTL_URVDME16DW            BIT8
 #define  B_DMI_DECCTL_VDMATAC               BIT7
 #define  B_DMI_DECCTL_LCRXINT               BIT6
 #define  B_DMI_DECCTL_DROPCPLATNZCE         BIT4
 #define  B_DMI_DECCTL_OUTBEXPCPLCHKEN       BIT3
 #define  B_DMI_DECCTL_MTRXLTC               BIT2
 #define  B_DMI_DECCTL_BUSNUMZCHK            BIT1
 #define  B_DMI_DECCTL_ATSCE                 BIT0

#define R_DMI_PIDECCTL                       0x90C
 #define B_DMI_PIDECCTL_CPLBNCHK             BIT0

#define R_DMI_PTMPSDC1                       0x920
#define R_DMI_PTMPSDC2                       0x924
#define R_DMI_PTMPSDC3                       0x928
#define R_DMI_PTMPSDC4                       0x92C
#define R_DMI_PTMPSDC5                       0x930
#define R_DMI_PTMPSDC6                       0x934
#define R_DMI_PTMPSDC7                       0x938
#define R_DMI_PTMPSDC8                       0x93C
#define R_DMI_PTMPSDC9                       0x940
#define R_DMI_PTMPSDC10                      0x944
#define R_DMI_PTMPSDC11                      0x948
#define R_DMI_PTMPSDC12                      0x94C
#define R_DMI_PTMPSDC13                      0x950
#define R_DMI_PTMPSDC14                      0x954

#define R_DMI_BAR0                           0x10
 #define B_DMI_BAR0_UBA_MASK                 0xFFFFFFFF
 #define B_DMI_BAR0_MMT                      (BIT2 | BIT1)
 #define N_DMI_BAR0_MMT                      1
 #define V_DMI_BAR0_MMT                      0x2
 #define B_DMI_BAR0_P                        BIT3

#define R_DMI_BAR1                           0x14
 #define B_DMI_BAR1_UBA_MASK                 0xFFFFFFFF

#define R_DMI_BAR0P0                         0x4E00
 #define B_DMI_BAR0P0_MMT                    (BIT2 | BIT1)
 #define N_DMI_BAR0P0_MMT                    1
 #define V_DMI_BAR0P0_MMT                    0x2
 #define B_DMI_BAR0P0_P                      BIT3

#define R_DMI_BAR1P0                         0x4E04
 #define B_DMI_BAR1P0_UBA_MASK               0xFFFFFFFF

#define R_DMI_MEM_LPHYCP1                    0x410            ///< Log Phy Control Policy 1
#define B_DMI_MEM_LPHYCP1_RXADPSVH           BIT2             ///< Receiver Adaptation Status Violation Handling
#define N_DMI_MEM_LPHYCP1_RXADPSVH           2
#define B_DMI_MEM_LPHYCP1_RXADPHM            BIT3             ///< Receiver Adaptation Handshake Mapping
#define N_DMI_MEM_LPHYCP1_RXADPHM            3
#define B_DMI_MEM_LPHYCP1_RXEQFNEVC          BIT4             ///< Receiver Equalization Final Evaluation Control
#define N_DMI_MEM_LPHYCP1_RXEQFNEVC          4
#define B_DMI_MEM_LPHYCP1_PIPEMBIP           BIT6             ///< PIPE Message Bus Initialization Policy
#define N_DMI_MEM_LPHYCP1_PIPEMBIP           6
#define B_DMI_MEM_LPHYCP1_RXADPRECE          BIT9             ///< Receiver Adaptation Recovery Enable
#define N_DMI_MEM_LPHYCP1_RXADPRECE          9
#define B_DMI_MEM_LPHYCP1_BPRXSTNDBYHSRECAL  BIT12            ///< Bypass RxStandby Handshake during Recal
#define N_DMI_MEM_LPHYCP1_BPRXSTNDBYHSRECAL  12

#endif
