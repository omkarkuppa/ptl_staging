/** @file
  Definitions of registers for ACE (Audio Context Engine)

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

#ifndef __SNDW_ACE_REGS_H__
#define __SNDW_ACE_REGS_H__

///
/// The default PCH PCI bus number
///
#define DEFAULT_PCI_BUS_NUMBER_PCH              0
#define DEFAULT_PCI_SEGMENT_NUMBER_PCH          0

#define B_PCI_BAR_MEMORY_TYPE_MASK              (BIT1 | BIT2)
#define B_PCI_BAR_MEMORY_TYPE_64                BIT2

//
// PCI Configuration Space Registers
//
#define PCI_DEVICE_NUMBER_PCH_HDA               31
#define PCI_FUNCTION_NUMBER_PCH_HDA             3

#define R_HDA_CFG_HDALBA                        0x10
#define R_HDA_CFG_HDAUBA                        0x14
#define R_HDA_CFG_ADSPLBA                       0x20
#define R_HDA_CFG_ADSPUBA                       0x24

#define R_HDA_MEM_GCTL                          0x08
#define B_HDA_MEM_GCTL_CRST                     BIT0

#define R_ACE_MEM_PPCTL                         0x0804
#define B_ACE_MEM_PPCTL_GPROCEN                 BIT30

#define R_HDA_MEM2_HFDSSCS                      0x1000
#define B_HDA_MEM2_HFDSSCS_SPA                  BIT16
#define B_HDA_MEM2_HFDSSCS_CPA                  BIT24

#define R_HDA_MEM2_HFPWRCTL                     0x1D18
#define B_HDA_MEM2_HFPWRCTL_WPDSPHPxPG          BIT0
#define B_HDA_MEM2_HFPWRCTL_PHSTPG              BIT4
#define B_HDA_MEM2_HFPWRCTL_PHUBHPPG            BIT6
#define B_HDA_MEM2_HFPWRCTL_WPIOxPG             (BIT8 | BIT9)
#define B_HDA_MEM2_HFPWRCTL_PHUBULPPG           BIT15

#define R_HDA_MEM2_HFPWRSTS                     0x1D1C
#define B_HDA_MEM2_HFPWRSTS_DSPHPxPGS           BIT0
#define B_HDA_MEM2_HFPWRSTS_HSTPGS              BIT4
#define B_HDA_MEM2_HFPWRSTS_HUBHPPGS            BIT6
#define B_HDA_MEM2_HFPWRSTS_IOxPGS              (BIT8 | BIT9)
#define B_HDA_MEM2_HFPWRSTS_HUBULPPGS           BIT15

#define R_HDA_MEM2_DSP2C0CTL                    0x178D04
#define B_HDA_MEM2_DSP2C0CTL_SPA                BIT0
#define B_HDA_MEM2_DSP2C0CTL_CPA                BIT8

#define R_HDA_MEM2_SNDW_SNDWLCAP                0x0D80
#define B_HDA_MEM2_SNDW_SNDWLCAP_LCOUNT         (BIT22 | BIT21 | BIT20)
#define N_HDA_MEM2_SNDW_SNDWLCAP_LCOUNT         20
#define R_HDA_MEM2_SNDW_SNDWLCTL                R_HDA_MEM2_SNDW_SNDWLCAP + 0x04
#define N_HDA_MEM2_SNDW_SNDWLCTL_SPA            16
#define B_HDA_MEM2_SNDW_SNDWLCTL_SPA(x)         (BIT0 << ((x) + N_HDA_MEM2_SNDW_SNDWLCTL_SPA))
#define N_HDA_MEM2_SNDW_SNDWLCTL_CPA            23
#define B_HDA_MEM2_SNDW_SNDWLCTL_CPA(x)         (BIT0 << ((x) + N_HDA_MEM2_SNDW_SNDWLCTL_CPA))
#define R_HDA_MEM2_SNDW_SNDWIPPTR               R_HDA_MEM2_SNDW_SNDWLCAP + 0x20
#define B_HDA_MEM2_SNDW_SNDWIPPTR_PRT           0xFFFFF

#define R_HDA_MEM2_SNDW                         0x36000
#define R_HDA_MEM2_SNDW_SNDWxIOCTL(x)           R_HDA_MEM2_SNDW + 0xC + (x) * 0x8000
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_WPDD         BIT6
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_BKE          BIT5
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_DOE          BIT4
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_DO           BIT3
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_COE          BIT2
#define B_HDA_MEM2_SNDW_SNDWxIOCTL_MIF          BIT0
#define R_HDA_MEM2_SNDW_SNDWxACTMCTL(x)         R_HDA_MEM2_SNDW + 0xE + (x) * 0x8000
#define B_HDA_MEM2_SNDW_SNDWxACTMCTL_DACTQE     BIT0
#define R_HDA_MEM2_SNDW_SNDWWAKEEN(x)           R_HDA_MEM2_SNDW + 0x8 + (x) * 0x8000
#define B_HDA_MEM2_SNDW_SNDWWAKEEN_PWE          (BIT3 | BIT2 | BIT1 | BIT0)
#define R_HDA_MEM2_SNDW_SNDWWAKESTS(x)          R_HDA_MEM2_SNDW + 0xA + (x) * 0x8000
#define B_HDA_MEM2_SNDW_SNDWWAKESTS_PWS         (BIT3 | BIT2 | BIT1 | BIT0)

#define R_SNDW_MCP_B0_CLOCK_CTRL                0x130
#define R_SNDW_MCP_B1_CLOCK_CTRL                0x138
#define V_SNDW_MCP_CLOCK_DIVIDER_BY_8           0x7
#define V_SNDW_MCP_CLOCK_DIVIDER_BY_4           0x3
#define V_SNDW_MCP_CLOCK_DIVIDER_BY_2           0x1

#define R_ACE_MEM_WAKEEN                        0x000C

#define R_ACE_MEM_WAKESTS                       0x000E

#define R_ACE_MEM_IC                            0x60
#define R_ACE_MEM_IR                            0x64
#define R_ACE_MEM_ICS                           0x68
#define B_ACE_MEM_ICS_ICB                       BIT0
#define B_ACE_MEM_ICS_IRV                       BIT1

#define R_ACE_MEM_HDAHWI1                       0x1C9C
#define B_ACE_MEM_HDAHWI1_INPAY                 0x0000ffff
#define B_ACE_MEM_HDAHWI1_OUTPAY                0xffff0000
#define N_ACE_MEM_HDAHWI1_INPAY                 0
#define N_ACE_MEM_HDAHWI1_OUTPAY                16

#define R_ACE_MEM_HDALCTL                       0x0C44
#define B_ACE_MEM_HDALCTL_CPA                   BIT23
#define B_ACE_MEM_HDALCTL_SPA                   BIT16
#define B_ACE_MEM_HDALCTL_SCF                   (BIT3 | BIT2 | BIT1 | BIT0)
#define N_ACE_MEM_HDALCTL_SCF                   0

#define R_HDA_MEM2_SNDW_OFFSETS                 0x8000
#define HDA_ADSP_CORE1                          0
#define HDA_ADSP_CORE2                          1
#define HDA_ADSP_CORE3                          2
#define HDA_ADSP_CORE4                          3

#define HDA_MAX_LOOP_TIME                      4000
#define HDA_WAIT_PERIOD                        4

#endif /** __SNDW_ACE_REGS_H__ **/
