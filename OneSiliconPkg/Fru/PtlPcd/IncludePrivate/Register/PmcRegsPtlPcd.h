/** @file
  Register definition for PTL PCD PMC.

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
    Register that is specific to -H denoted by "_PCH_S_" in component name.
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

#ifndef _PMC_REGS_PTL_PCD_P_
#define _PMC_REGS_PTL_PCD_P_

#define B_PTL_PCD_P_PMC_PWRM_GEN_PMCON_B_ST_FDIS_LK             BIT21
#define B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_0_LPSS              BIT3

#define B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI              BIT27
#define B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE               BIT19

#define B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_2_ISH               BIT7

#define R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0               0x1E30
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_SPB           BIT13
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_SPA           BIT12
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_XHCI          BIT5
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_XDCI          BIT4
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_LPSS          BIT3

#define R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1               0x1E34
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_UFS           BIT28
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_CNVI          BIT27
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_GBE           BIT19
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_NPK           BIT4

#define R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD2               0x1E38
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD2_ISH           BIT7

#define R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3               0x1E3C
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3_THC1          BIT15
#define B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3_THC0          BIT14

#define R_PTL_PCD_P_PMC_PWRM_PMCR                               0x1DB0
#define B_PTL_PCD_P_PMC_PWRM_PMCR_PGD_LOCK                      BIT0

#define R_PTL_PCD_P_PMC_PMSYNC_TPR_CFG                          0x18D4
#define B_PTL_PCD_P_PMC_PMSYNC_TPR_CFG_PCH2CPU_TPR_CFG_LOCK     BIT31

#define R_PTL_PCD_P_PMC_PWRM_TSS0                               0x1560
#define B_PTL_PCD_P_PMC_PWRM_TSS0_TSS0LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS1                               0x1564
#define B_PTL_PCD_P_PMC_PWRM_TSS1_TSS1LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS2                               0x1568
#define B_PTL_PCD_P_PMC_PWRM_TSS2_TSS2LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS3                               0x156C
#define B_PTL_PCD_P_PMC_PWRM_TSS3_TSS3LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS4                               0x1570
#define B_PTL_PCD_P_PMC_PWRM_TSS4_TSS4LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS5                               0x1574
#define B_PTL_PCD_P_PMC_PWRM_TSS5_TSS5LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS6                               0x1578
#define B_PTL_PCD_P_PMC_PWRM_TSS6_TSS6LOCK                      BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS7                               0x157C
#define B_PTL_PCD_P_PMC_PWRM_TSS7_TSS7LOCK                      BIT31

#define R_PTL_PCD_P_PMC_PWRM_TTF                                0x15A0
#define B_PTL_PCD_P_PMC_PWRM_TTF_TTFL                           BIT31


#endif
