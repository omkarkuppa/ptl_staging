/** @file
  Register definition for FIA 19.0 component

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
    Register that is specific to -S denoted by "_PCH_S_" in component name.
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
#ifndef _PCH_FIA_19_REGS_H_
#define _PCH_FIA_19_REGS_H_

//
// PCH FIA lane owner encoding
//
#define V_PCH_FIA_19_PCR_LANE_OWN_NO_OWNER                   0x0
#define V_PCH_FIA_19_PCR_LANE_OWN_USB3                       0x1
#define V_PCH_FIA_19_PCR_LANE_OWN_THUNDERBOLT                0x2
#define V_PCH_FIA_19_PCR_LANE_OWN_DISPLAY_PORT               0x3
#define V_PCH_FIA_19_PCR_LANE_OWN_HTI                        0x4
#define V_PCH_FIA_19_PCR_LANE_OWN_PCIE                       0x5
#define V_PCH_FIA_19_PCR_LANE_OWN_DMI                        0x6
#define V_PCH_FIA_19_PCR_LANE_OWN_SATA                       0x7
#define V_PCH_FIA_19_PCR_LANE_OWN_GBE                        0x8
#define V_PCH_FIA_19_PCR_LANE_OWN_UFS                        0x9
#define V_PCH_FIA_19_PCR_LANE_OWN_TSN                        0xA

#define PCH_FIA_19_MAX_PDRCRM                                8
#define PCH_FIA_19_MAX_PCH_PCIE_CLKREQ                       40
#define PCH_FIA_19_MAX_CPU_PCIE_CLKREQ                       4

#define R_PCH_FIA_CPCB_PMC                                   0x2C
#define B_PCH_FIA_CPCB_PMC_PRDPGE                            BIT3

#define PCH_FIA_19_MAX_PDRCRE                                2
#define R_PCH_FIA_19_PCR_PDRCRE1                           0x100  // CLKREQ Enable. PCH Ports 0-31
#define R_PCH_FIA_19_PCR_PDRCRE2                           0x104  // CLKREQ Enable. PCH Ports 32-39

#define R_PCH_FIA_19_PCR_PDRCRM1                           0x108  // CLKREQ Mapping. PCH Ports 0-4
#define R_PCH_FIA_19_PCR_PDRCRM2                           0x10C  // CLKREQ Mapping. PCH Ports 5-9
#define R_PCH_FIA_19_PCR_PDRCRM3                           0x110  // CLKREQ Mapping. PCH Ports 10-14
#define R_PCH_FIA_19_PCR_PDRCRM4                           0x114  // CLKREQ Mapping. PCH Ports 15-19
#define R_PCH_FIA_19_PCR_PDRCRM5                           0x118  // CLKREQ Mapping. PCH Ports 20-24
#define R_PCH_FIA_19_PCR_PDRCRM6                           0x11C  // CLKREQ Mapping. PCH Ports 25-29
#define R_PCH_FIA_19_PCR_PDRCRM7                           0x120  // CLKREQ Mapping. PCH Ports 30-34
#define R_PCH_FIA_19_PCR_PDRCRM8                           0x124  // CLKREQ Mapping. PCH Ports 35-39

#define R_PCH_FIA_19_PCR_GDRCREM                           0x140  // CLKREQ Mapping and Enable for GbE.
#define B_PCH_FIA_19_PCR_GDRCREM_NUMBER                    0x3E
#define B_PCH_FIA_19_PCR_GDRCREM_ENABLE                    BIT0   // CLKREQ Enable for GbE.
#define N_PCH_FIA_19_PCR_GDRCREM_NUMBER                    1      // CLKREQ Mapping GbE.

#define R_PCH_FIA_19_PCR_CPDRCRES                          0x144
#define B_PCH_FIA_19_PCR_CP0TXCKRQS                        BIT16

#define R_PCH_FIA_19_PCR_CPDRCRM                           0x148  // CLKREQ Mapping. CPU PCIe Device Reference Clock Request Mapping

#define S_PCH_FIA_19_PCR_DRCRM_BITS_PER_FIELD              6     // CLKREQ number is encoded in 6 bits (6th is hardwired to 0)
#define B_PCH_FIA_19_PCR_DRCRM_BITS_PER_FIELD              0x3F  // CLKREQ number is encoded in 6 bits (6th is hardwired to 0)
#define S_PCH_FIA_19_PCR_DRCRM_FIELDS_PER_REG              5     // each DRCRM register contains bitfields for 5 rootports

#define R_PCH_FIA_19_PCR_LOS1_REG_BASE                     0x300
#define R_PCH_FIA_19_PCR_LOS2_REG_BASE                     0x304
#define R_PCH_FIA_19_PCR_LOS3_REG_BASE                     0x308
#define R_PCH_FIA_19_PCR_LOS4_REG_BASE                     0x30C
#define R_PCH_FIA_19_PCR_LOS5_REG_BASE                     0x310

//
// FIA Fuse Status
//
#define R_PCH_FIA_19_PCR_FC1                               0x200
#define R_PCH_FIA_19_PCR_FC2                               0x204
#define N_PCH_FIA_19_PCR_FC2_L0FL7                         28
#define N_PCH_FIA_19_PCR_FC2_L0FL6                         24
#define N_PCH_FIA_19_PCR_FC2_L0FL5                         20
#define N_PCH_FIA_19_PCR_FC2_L0FL4                         16
#define N_PCH_FIA_19_PCR_FC2_L0FL3                         12
#define N_PCH_FIA_19_PCR_FC2_L0FL2                         8
#define N_PCH_FIA_19_PCR_FC2_L0FL1                         4
#define N_PCH_FIA_19_PCR_FC2_L0FL0                         0
#define R_PCH_FIA_19_PCR_FC3                               0x208
#define N_PCH_FIA_19_PCR_FC3_L0FL15                        28
#define N_PCH_FIA_19_PCR_FC3_L0FL14                        24
#define N_PCH_FIA_19_PCR_FC3_L0FL13                        20
#define N_PCH_FIA_19_PCR_FC3_L0FL12                        16
#define N_PCH_FIA_19_PCR_FC3_L0FL11                        12
#define N_PCH_FIA_19_PCR_FC3_L0FL10                        8
#define N_PCH_FIA_19_PCR_FC3_L0FL9                         4
#define N_PCH_FIA_19_PCR_FC3_L0FL8                         0
#define R_PCH_FIA_19_PCR_FC4                               0x20C
#define N_PCH_FIA_19_PCR_FC4_L0FL23                        28
#define N_PCH_FIA_19_PCR_FC4_L0FL22                        24
#define N_PCH_FIA_19_PCR_FC4_L0FL21                        20
#define N_PCH_FIA_19_PCR_FC4_L0FL20                        16
#define N_PCH_FIA_19_PCR_FC4_L0FL19                        12
#define N_PCH_FIA_19_PCR_FC4_L0FL18                        8
#define N_PCH_FIA_19_PCR_FC4_L0FL17                        4
#define N_PCH_FIA_19_PCR_FC4_L0FL16                        0
#define R_PCH_FIA_19_PCR_FC5                               0x210
#define N_PCH_FIA_19_PCR_FC5_L0FL31                        28
#define N_PCH_FIA_19_PCR_FC5_L0FL30                        24
#define N_PCH_FIA_19_PCR_FC5_L0FL29                        20
#define N_PCH_FIA_19_PCR_FC5_L0FL28                        16
#define N_PCH_FIA_19_PCR_FC5_L0FL27                        12
#define N_PCH_FIA_19_PCR_FC5_L0FL26                        8
#define N_PCH_FIA_19_PCR_FC5_L0FL25                        4
#define N_PCH_FIA_19_PCR_FC5_L0FL24                        0
#define R_PCH_FIA_19_PCR_FC6                               0x214

//
// FIA Softstrap Status
//
#define R_PCH_FIA_19_PCR_SC1                               0x280
#define R_PCH_FIA_19_PCR_SC2                               0x284
#define R_PCH_FIA_19_PCR_SC3                               0x288
#define R_PCH_FIA_19_PCR_SC4                               0x28C
#define R_PCH_FIA_19_PCR_SC5                               0x290
#define R_PCH_FIA_19_PCR_SC6                               0x294
#define R_PCH_FIA_19_PCR_SC7                               0x298
#define R_PCH_FIA_19_PCR_SC8                               0x29C

#endif
