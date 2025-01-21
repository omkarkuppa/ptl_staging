/** @file
  Register definition for HSIO

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
#ifndef _PCH_REGS_HSIO_H_
#define _PCH_REGS_HSIO_H_

#define B_HSIO_PCR_ACCESS_TYPE                          (BIT15 | BIT14)
#define N_HSIO_PCR_ACCESS_TYPE                          14
#define V_HSIO_PCR_ACCESS_TYPE_BDCAST                   (BIT15 | BIT14)
#define V_HSIO_PCR_ACCESS_TYPE_MULCAST                  BIT15
#define B_HSIO_PCR_LANE_GROUP_NO                        (BIT13 | BIT12 | BIT11 | BIT10 | BIT9)
#define B_HSIO_PCR_FUNCTION_NO                          (BIT8  | BIT7)
#define N_HSIO_PCR_FUNCTION_NO                          7
#define B_HSIO_PCR_REG_OFFSET                           (BIT6  | BIT5  | BIT4  | BIT3  | BIT2  | BIT1  | BIT0)

#define V_HSIO_PCR_ACCESS_TYPE_BCAST                    0x03
#define V_HSIO_PCR_ACCESS_TYPE_MCAST                    0x02
#define V_HSIO_PCR_ACCESS_TYPE_UCAST                    0x00

#define V_HSIO_PCR_LANE_GROUP_NO_CMN_LANE               0x00

#define V_HSIO_PCR_FUNCTION_NO_PCS                      0x00
#define V_HSIO_PCR_FUNCTION_NO_TX                       0x01
#define V_HSIO_PCR_FUNCTION_NO_RX                       0x02

#define V_HSIO_PCR_FUNCTION_NO_CMNDIG                   0x00
#define V_HSIO_PCR_FUNCTION_NO_CMNANA                   0x01
#define V_HSIO_PCR_FUNCTION_NO_PLL                      0x02

#define R_HSIO_PCR_PCS_DWORD4                           0x10

#define R_HSIO_PCR_PCS_DWORD8                           0x20
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_PTR_INIT_4_0     0x1F000000
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0      0x001F0000
#define N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0      16
#define B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0      0x00001F00
#define N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0      8

#define R_HSIO_PCR_PCS_DWORD9                           0x24
#define B_HSIO_PCR_PCS_DWORD9_REG_ENABLE_PWR_GATING     BIT29

#define R_HSIO_TX_BASE                                  0x140
#define R_HSIO_RX_BASE                                  0x200

#define R_HSIO_PCR_RX_DWORD2                            0x8
#define B_HSIO_PCR_RX_DWORD2_O_CALIB_RECAL_TARG_7_0     (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define V_HSIO_PCR_RX_DWORD2_O_CALIB_RECAL_COMPLIANCE   (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9)

#define R_HSIO_PCR_RX_DWORD8                            0x20
#define B_HSIO_PCR_RX_DWORD8_ICFGDFETAP3_EN             BIT10

#define R_HSIO_PCR_RX_DWORD9                            0x24
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP4_OVERRIDE_EN     BIT24
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP3_OVERRIDE_EN     BIT26
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP2_OVERRIDE_EN     BIT28
#define B_HSIO_PCR_RX_DWORD9_CFGDFETAP1_OVERRIDE_EN     BIT30

#define R_HSIO_PCR_RX_DWORD12                           0x30
#define B_HSIO_PCR_RX_DWORD12_O_CFGEWMARGINSEL          BIT14

#define R_HSIO_PCR_RX_DWORD20                               0x50
#define B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0  (BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0  24

#define R_HSIO_PCR_RX_DWORD21                               0x54
#define B_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_QUATRATE_5_0  (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_QUATRATE_5_0  8
#define B_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_HALFRATE_5_0  (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define N_HSIO_PCR_RX_DWORD21_ICFGCTLEDATATAP_HALFRATE_5_0  0

#define R_HSIO_PCR_RX_DWORD23                               0x5C
#define B_HSIO_PCR_RX_DWORD23_ICFGVGABLWTAP_OVERRIDE_EN     BIT2
#define B_HSIO_PCR_RX_DWORD23_CFGVGATAP_ADAPT_OVERRIDE_EN   BIT4

#define R_HSIO_PCR_RX_DWORD24                               0x60
#define B_HSIO_PCR_RX_DWORD24_O_CALIB_RECAL_TARG_35_32      (BIT31 | BIT30 | BIT29 | BIT28)
#define V_HSIO_PCR_RX_DWORD24_O_CALIB_RECAL_TARG_COMPLIANCE (BIT31 | BIT30 | BIT28)

#define R_HSIO_PCR_RX_DWORD25                            0x64
#define B_HSIO_PCR_RX_DWORD25_RX_TAP_CFG_CTRL            BIT3
#define B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0  0x1F0000
#define N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0  16

#define R_HSIO_PCR_RX_DWORD26                           0x68
#define B_HSIO_PCR_RX_DWORD26_SATA_EQ_DIS               BIT16

#define R_HSIO_PCR_RX_DWORD34                           0x88
#define B_HSIO_PCR_RX_DWORD34_MM_PH_OFC_SCALE_2_0       (BIT14 | BIT13 | BIT12)
#define N_HSIO_PCR_RX_DWORD34_MM_PH_OFC_SCALE_2_0       12

#define R_HSIO_PCR_RX_DWORD44                           0xB0
#define B_HSIO_PCR_RX_DWORD44_0_DFE_DATASUMCAL0_7_0     0xFF0000
#define N_HSIO_PCR_RX_DWORD44_0_DFE_DATASUMCAL0_7_0     16

#define R_HSIO_PCR_RX_DWORD51                                   0xCC
#define B_HSIO_PCR_RX_DWORD51_FILTER_SEL_N_2_0                  0x38000000
#define N_HSIO_PCR_RX_DWORD51_FILTER_SEL_N_2_0                  27
#define B_HSIO_PCR_RX_DWORD51_FILTER_SEL_P_2_0                  0x7000000
#define N_HSIO_PCR_RX_DWORD51_FILTER_SEL_P_2_0                  24
#define B_HSIO_PCR_RX_DWORD51_OLFPSCFGPULLUPDWNRES_SUS_USB_2_0  0x7

#define R_HSIO_PCR_RX_DWORD56                           0xE0
#define B_HSIO_PCR_RX_DWORD56_ICFGPIDACCFGVALID         BIT16

#define R_HSIO_PCR_RX_DWORD57                           0xE4
#define B_HSIO_PCR_RX_DWORD57_JIM_COURSE                BIT30
#define B_HSIO_PCR_RX_DWORD57_JIM_ENABLE                BIT29
#define B_HSIO_PCR_RX_DWORD57_JIMMODE                   BIT28
#define B_HSIO_PCR_RX_DWORD57_JIMNUMCYCLES_3_0          0x0F000000
#define N_HSIO_PCR_RX_DWORD57_JIMNUMCYCLES_3_0          24
#define B_HSIO_PCR_RX_DWORD57_ICFGMARGINEN              BIT0

#define R_HSIO_PCR_RX_DWORD59                           0xEC
#define R_HSIO_PCR_RX_DWORD60                           0xF0

#define R_HSIO_PCR_TX_DWORD5                            0x14
#define B_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0    (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0    16
#define B_HSIO_PCR_TX_DWORD5_OW2TAPGEN1DEEMPH3P5_5_0    (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD5_OW2TAPGEN1DEEMPH3P5_5_0    8

#define R_HSIO_PCR_TX_DWORD6                            0x18
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN3DEEMPH6P0_5_0    (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD6_OW2TAPGEN3DEEMPH6P0_5_0    16
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN2DEEMPH6P0_5_0    (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD6_OW2TAPGEN2DEEMPH6P0_5_0    8
#define B_HSIO_PCR_TX_DWORD6_OW2TAPGEN1DEEMPH6P0_5_0    (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

#define R_HSIO_PCR_TX_DWORD8                            0x20
#define B_HSIO_PCR_TX_DWORD8_ORATE10MARGIN_5_0          (BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define N_HSIO_PCR_TX_DWORD8_ORATE10MARGIN_5_0          24
#define B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0          (BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0          16
#define B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0          (BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0          8

#define R_HSIO_PCR_TX_DWORD9                            0x24
#define B_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANSCALE_6_0     (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define B_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANENABLE        BIT7
#define N_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANSCALE_6_0     0
#define B_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANSCALE_6_0     (BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define B_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANENABLE        BIT15
#define N_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANSCALE_6_0     8
#define B_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANSCALE_6_0     (BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define B_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANENABLE        BIT23
#define N_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANSCALE_6_0     16
#define B_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANSCALE_6_0     (BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define B_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANENABLE        BIT31
#define N_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANSCALE_6_0     24

#define R_HSIO_PCR_TX_DWORD19                           0x4C

#define R_HSIO_PCR_CLANE0_CMN_ANA_DWORD2                0x80C8
#define B_HSIO_PCR_CLANE0_CMN_ANA_DWORD2_O_DTPLL1_lC_PLLEN_H_OVRDEN                BIT5
#define B_HSIO_PCR_CLANE0_CMN_ANA_DWORD2_O_DTPLL1_lC_FULLCALRESET_L_OVERDEN        BIT3

#define R_HSIO_PCR_PLL_SSC_DWORD2                       0x8188
#define B_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0       (BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)
#define N_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0       16
#define B_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN                BIT10
#define N_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN                10

#define R_HSIO_PCR_PLL_SSC_DWORD3                       0x818C
#define B_HSIO_PCR_PLL_SSC_DWORD3_SSC_PROPAGATE         BIT0

#define R_HSIO_PCR_PLL_DWORD12                          0x8030
#define B_HSIO_PCR_PLL_DWORD12_O_CFG_PWR_GATING_CTRL    BIT0
#define R_HSIO_PCR_PLL_DWORD15                                0x803C
#define B_HSIO_PCR_PLL_DWORD15_O_CFG_CMN_DYNCLKGATE_MODE_1_0  (BIT2 | BIT1 | BIT0)

#define R_HSIO_BTRS_SAI_WRITE_POLICY_LO                 0x10
#define R_HSIO_BTRS_SAI_READ_POLICY_LO                  0x08
#define R_HSIO_REGION0_SAI_WRITE_POLICY_LO              0x28
#define R_HSIO_REGION0_SAI_READ_POLICY_LO               0x20
#define R_HSIO_REGION1_SAI_WRITE_POLICY_LO              0x40
#define R_HSIO_REGION1_SAI_READ_POLICY_LO               0x38
#define R_HSIO_REGION2_SAI_WRITE_POLICY_LO              0x58
#define R_HSIO_REGION2_SAI_READ_POLICY_LO               0x50

#endif //_PCH_REGS_HSIO_H_

