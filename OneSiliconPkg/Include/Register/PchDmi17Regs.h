/** @file
  Register names for PCH DMI starting from SIP 17

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
#ifndef _PCH_DMI17_REGS_H_
#define _PCH_DMI17_REGS_H_

#define  R_PCH_DMI17_PCR_FCTL                             0x1300
#define  B_PCH_DMI17_PCR_FCTL_CRSPSEL                     BIT12

#define  R_PCH_DMI17_PCR_FCUCTL                           0x1304
#define  B_PCH_DMI17_PCR_FCUCTL_FC_CP_FCM_VCM             BIT28
#define  B_PCH_DMI17_PCR_FCUCTL_FC_CP_FCM_VC1             BIT27
#define  B_PCH_DMI17_PCR_FCUCTL_FC_CP_FCM                 BIT20

#define  R_PCH_DMI17_PCR_FCTL2                            0x1330
#define  B_PCH_DMI17_PCR_FCTL2_HRTCTL_MASK                (BIT9 | BIT8 | BIT7)
#define  B_PCH_DMI17_PCR_FCTL2_HRTCTL_OFFSET              7
#define  V_PCH_DMI17_PCR_FCTL2_HRTCTL                     0x1
#define  B_PCH_DMI17_PCR_FCTL2_RPTOT_MASK                 (BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_FCTL2_RPTOT_OFFSET               4
#define  V_PCH_DMI17_PCR_FCTL2_RPTOT                      0x5

#define  R_PCH_DMI17_PCR_RPR                              0x1334
#define  B_PCH_DMI17_PCR_RPR_S5SMTO_MASK                  (BIT23 | BIT22)
#define  B_PCH_DMI17_PCR_RPR_S5SMTO_OFFSET                22
#define  V_PCH_DMI17_PCR_RPR_S5SMTO                       0x1
#define  B_PCH_DMI17_PCR_RPR_S4SMTO_MASK                  (BIT21 | BIT20)
#define  B_PCH_DMI17_PCR_RPR_S4SMTO_OFFSET                20
#define  V_PCH_DMI17_PCR_RPR_S4SMTO                       0x1
#define  B_PCH_DMI17_PCR_RPR_S3SMTO_MASK                  (BIT19 | BIT18)
#define  B_PCH_DMI17_PCR_RPR_S3SMTO_OFFSET                18
#define  V_PCH_DMI17_PCR_RPR_S3SMTO                       0x1
#define  B_PCH_DMI17_PCR_RPR_CRMTO_MASK                   (BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_RPR_CRMTO_OFFSET                 16
#define  V_PCH_DMI17_PCR_RPR_CRMTO                        0x1
#define  B_PCH_DMI17_PCR_RPR_WRMTO_MASK                   (BIT15 | BIT14)
#define  B_PCH_DMI17_PCR_RPR_WRMTO_OFFSET                 14
#define  V_PCH_DMI17_PCR_RPR_WRMTO                        0x1
#define  B_PCH_DMI17_PCR_RPR_DMTO_MASK                    (BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_RPR_DMTO_OFFSET                  12
#define  V_PCH_DMI17_PCR_RPR_DMTO                         0x1
#define  B_PCH_DMI17_PCR_RPR_S5SM_MASK                    (BIT11 | BIT10)
#define  B_PCH_DMI17_PCR_RPR_S5SM_OFFSET                  10
#define  V_PCH_DMI17_PCR_RPR_S5SM                         0x1
#define  B_PCH_DMI17_PCR_RPR_S4SM_MASK                    (BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_RPR_S4SM_OFFSET                  8
#define  V_PCH_DMI17_PCR_RPR_S4SM                         0x1
#define  B_PCH_DMI17_PCR_RPR_S3SM_MASK                    (BIT7 | BIT6)
#define  B_PCH_DMI17_PCR_RPR_S3SM_OFFSET                  6
#define  V_PCH_DMI17_PCR_RPR_S3SM                         0x1
#define  B_PCH_DMI17_PCR_RPR_CRM_MASK                     (BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_RPR_CRM_OFFSET                   4
#define  V_PCH_DMI17_PCR_RPR_CRM                          0x1
#define  B_PCH_DMI17_PCR_RPR_WRM_MASK                     (BIT3 | BIT2)
#define  B_PCH_DMI17_PCR_RPR_WRM_OFFSET                   2
#define  V_PCH_DMI17_PCR_RPR_WRM                          0x1

#define  R_PCH_DMI17_PCR_DCGEN1                           0x1350
#define  B_PCH_DMI17_PCR_DCGEN1_PXTTSULDCGEN              BIT25
#define  B_PCH_DMI17_PCR_DCGEN1_PXTTSULDCGEN_OFFSET       25
#define  B_PCH_DMI17_PCR_DCGEN1_PXTTULDCGEN               BIT24
#define  B_PCH_DMI17_PCR_DCGEN1_PXTTULDCGEN_OFFSET        24
#define  B_PCH_DMI17_PCR_DCGEN1_PXTRSULDCGEN              BIT17
#define  B_PCH_DMI17_PCR_DCGEN1_PXTRSULDCGEN_OFFSET       17
#define  B_PCH_DMI17_PCR_DCGEN1_PXTRULDCGEN               BIT16
#define  B_PCH_DMI17_PCR_DCGEN1_PXTRULDCGEN_OFFSET        16
#define  B_PCH_DMI17_PCR_DCGEN1_PXLSULDCGEN               BIT11
#define  B_PCH_DMI17_PCR_DCGEN1_PXLSULDCGEN_OFFSET        11
#define  B_PCH_DMI17_PCR_DCGEN1_PXLIULDCGEN               BIT10
#define  B_PCH_DMI17_PCR_DCGEN1_PXLIULDCGEN_OFFSET        10
#define  B_PCH_DMI17_PCR_DCGEN1_PXLRULDCGEN               BIT8
#define  B_PCH_DMI17_PCR_DCGEN1_PXLRULDCGEN_OFFSET        8
#define  B_PCH_DMI17_PCR_DCGEN1_PXCULDCGEN                BIT1
#define  B_PCH_DMI17_PCR_DCGEN1_PXCULDCGEN_OFFSET         1
#define  B_PCH_DMI17_PCR_DCGEN1_PXKGULDCGEN               BIT0
#define  B_PCH_DMI17_PCR_DCGEN1_PXKGULDCGEN_OFFSET        0

#define  R_PCH_DMI17_PCR_DCGEN2                           0x1354
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDRULDCGEN              BIT17
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDRULDCGEN_OFFSET       17
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDTULDCGEN              BIT16
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDTULDCGEN_OFFSET       16
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDIULDCGEN              BIT15
#define  B_PCH_DMI17_PCR_DCGEN2_PXSDIULDCGEN_OFFSET       15
#define  B_PCH_DMI17_PCR_DCGEN2_PXFRULDCGEN               BIT5
#define  B_PCH_DMI17_PCR_DCGEN2_PXFRULDCGEN_OFFSET        5
#define  B_PCH_DMI17_PCR_DCGEN2_PXFTULDCGEN               BIT4
#define  B_PCH_DMI17_PCR_DCGEN2_PXFTULDCGEN_OFFSET        4
#define  B_PCH_DMI17_PCR_DCGEN2_PXPBULDCGEN               BIT2
#define  B_PCH_DMI17_PCR_DCGEN2_PXPBULDCGEN_OFFSET        2
#define  B_PCH_DMI17_PCR_DCGEN2_PXPSULDCGEN               BIT1
#define  B_PCH_DMI17_PCR_DCGEN2_PXPSULDCGEN_OFFSET        1
#define  B_PCH_DMI17_PCR_DCGEN2_PXPIULDCGEN               BIT0
#define  B_PCH_DMI17_PCR_DCGEN2_PXPIULDCGEN_OFFSET        0

#define  R_PCH_DMI17_PCR_DCGM1                            0x1358
#define  B_PCH_DMI17_PCR_DCGM1_PXTTSULDCGM                BIT25
#define  B_PCH_DMI17_PCR_DCGM1_PXTTSULDCGM_OFFSET         25
#define  B_PCH_DMI17_PCR_DCGM1_PXTTULDCGM                 BIT24
#define  B_PCH_DMI17_PCR_DCGM1_PXTTULDCGM_OFFSET          24
#define  B_PCH_DMI17_PCR_DCGM1_PXTRSULDCGM                BIT17
#define  B_PCH_DMI17_PCR_DCGM1_PXTRSULDCGM_OFFSET         17
#define  B_PCH_DMI17_PCR_DCGM1_PXTRULDCGM                 BIT16
#define  B_PCH_DMI17_PCR_DCGM1_PXTRULDCGM_OFFSET          16
#define  B_PCH_DMI17_PCR_DCGM1_PXLSULDCGM                 BIT11
#define  B_PCH_DMI17_PCR_DCGM1_PXLSULDCGM_OFFSET          11
#define  B_PCH_DMI17_PCR_DCGM1_PXLIULDCGM                 BIT10
#define  B_PCH_DMI17_PCR_DCGM1_PXLIULDCGM_OFFSET          10
#define  B_PCH_DMI17_PCR_DCGM1_PXLTULDCGM                 BIT9
#define  B_PCH_DMI17_PCR_DCGM1_PXLTULDCGM_OFFSET          9
#define  B_PCH_DMI17_PCR_DCGM1_PXLRULDCGM                 BIT8
#define  B_PCH_DMI17_PCR_DCGM1_PXLRULDCGM_OFFSET          8
#define  B_PCH_DMI17_PCR_DCGM1_PXCULDCGM                  BIT1
#define  B_PCH_DMI17_PCR_DCGM1_PXCULDCGM_OFFSET           1
#define  B_PCH_DMI17_PCR_DCGM1_PXKGULDCGM                 BIT0
#define  B_PCH_DMI17_PCR_DCGM1_PXKGULDCGM_OFFSET          0

#define  R_PCH_DMI17_PCR_DCGM2                            0x135C
#define  B_PCH_DMI17_PCR_DCGM2_PXSDRULDCGM                BIT17
#define  B_PCH_DMI17_PCR_DCGM2_PXSDRULDCGM_OFFSET         17
#define  B_PCH_DMI17_PCR_DCGM2_PXSDTULDCGM                BIT16
#define  B_PCH_DMI17_PCR_DCGM2_PXSDTULDCGM_OFFSET         16
#define  B_PCH_DMI17_PCR_DCGM2_PXSDIULDCGM                BIT15
#define  B_PCH_DMI17_PCR_DCGM2_PXSDIULDCGM_OFFSET         15
#define  B_PCH_DMI17_PCR_DCGM2_PXFRULDCGM                 BIT5
#define  B_PCH_DMI17_PCR_DCGM2_PXFRULDCGM_OFFSET          5
#define  B_PCH_DMI17_PCR_DCGM2_PXFTULDCGM                 BIT4
#define  B_PCH_DMI17_PCR_DCGM2_PXFTULDCGM_OFFSET          4
#define  B_PCH_DMI17_PCR_DCGM2_PXPBULDCGM                 BIT2
#define  B_PCH_DMI17_PCR_DCGM2_PXPBULDCGM_OFFSET          2
#define  B_PCH_DMI17_PCR_DCGM2_PXPSULDCGM                 BIT1
#define  B_PCH_DMI17_PCR_DCGM2_PXPSULDCGM_OFFSET          1
#define  B_PCH_DMI17_PCR_DCGM2_PXPIULDCGM                 BIT0
#define  B_PCH_DMI17_PCR_DCGM2_PXPIULDCGM_OFFSET          0

#define  R_PCH_DMI17_PCR_DCGEN3                           0x1360
#define  B_PCH_DMI17_PCR_DCGEN3_PXTTSUPDCGEN              BIT25
#define  B_PCH_DMI17_PCR_DCGEN3_PXTTSUPDCGEN_OFFSET       25
#define  B_PCH_DMI17_PCR_DCGEN3_PXTTUPDCGEN               BIT24
#define  B_PCH_DMI17_PCR_DCGEN3_PXTTUPDCGEN_OFFSET        24
#define  B_PCH_DMI17_PCR_DCGEN3_PXTOUPDCGEN               BIT19
#define  B_PCH_DMI17_PCR_DCGEN3_PXTOUPDCGEN_OFFSET        19
#define  B_PCH_DMI17_PCR_DCGEN3_PXTRSUPDCGEN              BIT17
#define  B_PCH_DMI17_PCR_DCGEN3_PXTRSUPDCGEN_OFFSET       17
#define  B_PCH_DMI17_PCR_DCGEN3_PXTRUPDCGEN               BIT16
#define  B_PCH_DMI17_PCR_DCGEN3_PXTRUPDCGEN_OFFSET        16
#define  B_PCH_DMI17_PCR_DCGEN3_PXLIUPDCGEN               BIT10
#define  B_PCH_DMI17_PCR_DCGEN3_PXLIUPDCGEN_OFFSET        10
#define  B_PCH_DMI17_PCR_DCGEN3_PXLTUPDCGEN               BIT9
#define  B_PCH_DMI17_PCR_DCGEN3_PXLTUPDCGEN_OFFSET        9
#define  B_PCH_DMI17_PCR_DCGEN3_PXLRUPDCGEN               BIT8
#define  B_PCH_DMI17_PCR_DCGEN3_PXLRUPDCGEN_OFFSET        8
#define  B_PCH_DMI17_PCR_DCGEN3_PXSRUSSNRDCGEN            BIT6
#define  B_PCH_DMI17_PCR_DCGEN3_PXSRUSSNRDCGEN_OFFSET     6
#define  B_PCH_DMI17_PCR_DCGEN3_PXCUPSNRDCGEN             BIT5
#define  B_PCH_DMI17_PCR_DCGEN3_PXCUPSNRDCGEN_OFFSET      5
#define  B_PCH_DMI17_PCR_DCGEN3_PXCUPSRCDCGEN             BIT4
#define  B_PCH_DMI17_PCR_DCGEN3_PXCUPSRCDCGEN_OFFSET      4
#define  B_PCH_DMI17_PCR_DCGEN3_PXBUPDCGEN                BIT1
#define  B_PCH_DMI17_PCR_DCGEN3_PXBUPDCGEN_OFFSET         1
#define  B_PCH_DMI17_PCR_DCGEN3_PXEUPDCGEN                BIT0
#define  B_PCH_DMI17_PCR_DCGEN3_PXEUPDCGEN_OFFSET         0

#define  R_PCH_DMI17_PCR_DCGM3                            0x1368
#define  B_PCH_DMI17_PCR_DCGM3_PXTTSUPDCGM                BIT25
#define  B_PCH_DMI17_PCR_DCGM3_PXTTSUPDCGM_OFFSET         25
#define  B_PCH_DMI17_PCR_DCGM3_PXTTUPDCGM                 BIT24
#define  B_PCH_DMI17_PCR_DCGM3_PXTTUPDCGM_OFFSET          24
#define  B_PCH_DMI17_PCR_DCGM3_PXTOUPDCGM                 BIT19
#define  B_PCH_DMI17_PCR_DCGM3_PXTOUPDCGM_OFFSET          19
#define  B_PCH_DMI17_PCR_DCGM3_PXTRSUPDCGM                BIT17
#define  B_PCH_DMI17_PCR_DCGM3_PXTRSUPDCGM_OFFSET         17
#define  B_PCH_DMI17_PCR_DCGM3_PXTRUPDCGM                 BIT16
#define  B_PCH_DMI17_PCR_DCGM3_PXTRUPDCGM_OFFSET          16
#define  B_PCH_DMI17_PCR_DCGM3_PXLIUPDCGM                 BIT10
#define  B_PCH_DMI17_PCR_DCGM3_PXLIUPDCGM_OFFSET          10
#define  B_PCH_DMI17_PCR_DCGM3_PXLTUPDCGM                 BIT9
#define  B_PCH_DMI17_PCR_DCGM3_PXLTUPDCGM_OFFSET          9
#define  B_PCH_DMI17_PCR_DCGM3_PXLRUPDCGM                 BIT8
#define  B_PCH_DMI17_PCR_DCGM3_PXLRUPDCGM_OFFSET          8
#define  B_PCH_DMI17_PCR_DCGM3_PXSRUSSNRDCGM              BIT6
#define  B_PCH_DMI17_PCR_DCGM3_PXSRUSSNRDCGM_OFFSET       6
#define  B_PCH_DMI17_PCR_DCGM3_PXCUPSNRDCGM               BIT5
#define  B_PCH_DMI17_PCR_DCGM3_PXCUPSNRDCGM_OFFSET        5
#define  B_PCH_DMI17_PCR_DCGM3_PXCUPSRCDCGM               BIT4
#define  B_PCH_DMI17_PCR_DCGM3_PXCUPSRCDCGM_OFFSET        4
#define  B_PCH_DMI17_PCR_DCGM3_PXBUPDCGM                  BIT1
#define  B_PCH_DMI17_PCR_DCGM3_PXBUPDCGM_OFFSET           1
#define  B_PCH_DMI17_PCR_DCGM3_PXEUPDCGM                  BIT0
#define  B_PCH_DMI17_PCR_DCGM3_PXEUPDCGM_OFFSET           0

#define  R_PCH_DMI17_PCR_IPCLKCTR                         0x1370
#define  B_PCH_DMI17_PCR_IPCLKCTR_MDPCCEN                 BIT3
#define  B_PCH_DMI17_PCR_IPCLKCTR_PXDCGE                  BIT2
#define  B_PCH_DMI17_PCR_IPCLKCTR_PXDCGE_OFFSET           2

#define  R_PCH_DMI17_PCR_LPHYCP1                          0x1410
#define  B_PCH_DMI17_PCR_LPHYCP1_RXEQFNEVC                BIT4
#define  B_PCH_DMI17_PCR_LPHYCP1_RXEQFNEVC_OFFSET         4
#define  B_PCH_DMI17_PCR_LPHYCP1_RXADPHM                  BIT3
#define  B_PCH_DMI17_PCR_LPHYCP1_RXADPHM_OFFSET           3

#define  R_PCH_DMI17_PCR_UPDLPHYCP                        0x1414
#define  B_PCH_DMI17_PCR_UPDLPHYCP_UPDLTCDC               BIT1
#define  B_PCH_DMI17_PCR_UPDLPHYCP_UPDLTCDC_OFFSET        1

#define  R_PCH_DMI17_PCR_RXL0SESQCP1                      0x1420
#define  B_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S_MASK       (BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S_OFFSET     0
#define  V_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S            0x2

#define  R_PCH_DMI17_PCR_RXL0SESQCP2                      0x1424
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4SQPDC_MASK         0xF0000000
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4SQPDC_OFFSET       28
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G4SQPDC              0x0
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4SSDFRXL0S_MASK     (BIT26 | BIT25 | BIT24)
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4SSDFRXL0S_OFFSET   24
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G4SSDFRXL0S          0x2
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3SQPDC_MASK         0xF00000
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3SQPDC_OFFSET       20
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G3SQPDC              0x0
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3SSDFRXL0S_MASK     (BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3SSDFRXL0S_OFFSET   16
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G3SSDFRXL0S          0x2
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2SQPDC_MASK         0xF000
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2SQPDC_OFFSET       12
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G2SQPDC              0x0
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2SSDFRXL0S_MASK     (BIT10 | BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2SSDFRXL0S_OFFSET   8
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G2SSDFRXL0S          0x2
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G1SQPDC_MASK         0xF0
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G1SQPDC_OFFSET       4
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G1SQPDC              0x0
#define  B_PCH_DMI17_PCR_RXL0SESQCP2_G1SSDFRXL0S_MASK     (BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_RXL0SESQCP2_G1SSDFRXL0S_OFFSET   0
#define  V_PCH_DMI17_PCR_RXL0SESQCP2_G1SSDFRXL0S          0x2

#define  R_PCH_DMI17_PCR_RXL0SESQCP4                      0x142C
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST_MASK        0xFF000000
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST_OFFSET      24
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST             0x4
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST_MASK        0xFF0000
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST_OFFSET      16
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST             0x4
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST_MASK        0xFF00
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST_OFFSET      8
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST             0x4
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST_MASK        0xFF
#define  B_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST_OFFSET      0
#define  V_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST             0x4

#define  R_PCH_DMI17_PCR_LPHYCP4                             0x1434
#define  B_PCH_DMI17_PCR_LPHYCP4_DPMDNDBFE                   BIT12
#define  B_PCH_DMI17_PCR_LPHYCP4_DPMDNDBFE_OFFSET            12
#define  B_PCH_DMI17_PCR_LPHYCP4_PLLBUSDRC                   BIT10
#define  B_PCH_DMI17_PCR_LPHYCP4_PLLBUSDRC_OFFSET            10
#define  B_PCH_DMI17_PCR_LPHYCP4_LGUSSP_MASK                 (BIT7 | BIT6)
#define  B_PCH_DMI17_PCR_LPHYCP4_LGUSSP_OFFSET               6
#define  V_PCH_DMI17_PCR_LPHYCP4_LGUSSP                      0x1
#define  B_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS_MASK   (BIT5 | BIT4 | BIT3)
#define  B_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS_OFFSET 3
#define  V_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS        0x2
#define  B_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS_MASK    (BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS_OFFSET  0
#define  V_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS         0x2

#define  R_PCH_DMI17_PCR_IORCCP1                          0x144C
#define  B_PCH_DMI17_PCR_IORCCP1_G5ORRRXECC_MASK          (BIT30 | BIT29 | BIT28)
#define  B_PCH_DMI17_PCR_IORCCP1_G5ORRRXECC_OFFSET        28
#define  V_PCH_DMI17_PCR_IORCCP1_G5ORRRXECC               0x1
#define  B_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC_MASK          (BIT27 | BIT26 | BIT25)
#define  B_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC_OFFSET        25
#define  V_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC               0x1
#define  B_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC_MASK          (BIT24 | BIT23 | BIT22)
#define  B_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC_OFFSET        22
#define  V_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC               0x1
#define  B_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC_MASK          (BIT21 | BIT20 | BIT19)
#define  B_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC_OFFSET        19
#define  V_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC               0x1
#define  B_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC_MASK          (BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC_OFFSET        16
#define  V_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC               0x1
#define  B_PCH_DMI17_PCR_IORCCP1_DISORCRODI               BIT2
#define  B_PCH_DMI17_PCR_IORCCP1_DISORCRODI_OFFSET        2
#define  B_PCH_DMI17_PCR_IORCCP1_DRCORRP                  BIT1
#define  B_PCH_DMI17_PCR_IORCCP1_DRCORRP_OFFSET           1
#define  B_PCH_DMI17_PCR_IORCCP1_DISORCL12REC             BIT0
#define  B_PCH_DMI17_PCR_IORCCP1_DISORCL12REC_OFFSET      0

#define  R_PCH_DMI17_PCR_DETPHYPGE                        0x1590
#define  B_PCH_DMI17_PCR_DETPHYPGE_DLPPGP                 BIT4
#define  B_PCH_DMI17_PCR_DETPHYPGE_DUCFGPHYPGE            BIT3
#define  B_PCH_DMI17_PCR_DETPHYPGE_L23PHYPGE              BIT2
#define  B_PCH_DMI17_PCR_DETPHYPGE_DISPHYPGE              BIT1
#define  B_PCH_DMI17_PCR_DETPHYPGE_DETPHYPGE              BIT0

#define  R_PCH_DMI17_PCR_PIPEPDCTL                        0x1594
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL_MASK      (BIT31 | BIT30 | BIT29 | BIT28)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL_OFFSET    28
#define  V_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL           0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL_MASK     (BIT27 | BIT26 | BIT25 | BIT24)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL_OFFSET   24
#define  V_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL          0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL_MASK        (BIT23 | BIT22 | BIT21 | BIT20)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL_OFFSET      20
#define  V_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL             0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL_MASK      (BIT19 | BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL_OFFSET    16
#define  V_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL           0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL_MASK        (BIT15 | BIT14 | BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL_OFFSET      12
#define  V_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL             0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL_MASK      (BIT11 | BIT10 | BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL_OFFSET    8
#define  V_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL           0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL_MASK        (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL_OFFSET      4
#define  V_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL             0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL_MASK      (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL_OFFSET    0
#define  V_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL           0x3

#define  R_PCH_DMI17_PCR_PIPEPDCTL2                        0x1598
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL_MASK      (BIT31 | BIT30 | BIT29 | BIT28)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL_OFFSET    28
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL           0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL_MASK    (BIT27 | BIT26 | BIT25 | BIT24)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL_OFFSET  24
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL         0x3
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL_MASK       (BIT23 | BIT22 | BIT21 | BIT20)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL_OFFSET     20
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL            0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL_MASK     (BIT19 | BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL_OFFSET   16
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL          0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL_MASK     (BIT15 | BIT14 | BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL_OFFSET   12
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL          0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL_MASK   (BIT11 | BIT10 | BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL_OFFSET 8
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL        0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL_MASK    (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL_OFFSET  4
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL         0x6
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL_MASK     (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL_OFFSET   0
#define  V_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL          0x6

#define  R_PCH_DMI17_PCR_PIPEPDCTL3                        0x159C
#define  B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL_MASK     (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL_OFFSET   4
#define  V_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL          0x4
#define  B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL_MASK   (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL_OFFSET 0
#define  V_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL        0x4

#define  R_PCH_DMI17_PCR_PIPEPDCTLEXT                     0x15A0
#define  B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2T              BIT2
#define  B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2T_OFFSET       2
#define  B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2TCD            BIT1
#define  B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2TCD_OFFSET     1

#define  R_PCH_DMI17_PCR_RPDEC1                           0x1780
#define  B_PCH_DMI17_PCR_RPDEC1_RPCREPT_MASK              0xFF000000
#define  B_PCH_DMI17_PCR_RPDEC1_RPCREPT_OFFSET            24
#define  V_PCH_DMI17_PCR_RPDEC1_RPCREPT                   0x3
#define  B_PCH_DMI17_PCR_RPDEC1_RPCRERT_MASK              0xFF0000
#define  B_PCH_DMI17_PCR_RPDEC1_RPCRERT_OFFSET            16
#define  V_PCH_DMI17_PCR_RPDEC1_RPCRERT                   0x11
#define  B_PCH_DMI17_PCR_RPDEC1_RPWREPT_MASK              0xFF00
#define  B_PCH_DMI17_PCR_RPDEC1_RPWREPT_OFFSET            8
#define  V_PCH_DMI17_PCR_RPDEC1_RPWREPT                   0x3
#define  B_PCH_DMI17_PCR_RPDEC1_RPWRERT_MASK              0xFF
#define  B_PCH_DMI17_PCR_RPDEC1_RPWRERT_OFFSET            0
#define  V_PCH_DMI17_PCR_RPDEC1_RPWRERT                   0x10

#define  R_PCH_DMI17_PCR_RPDEC2                           0x1784
#define  B_PCH_DMI17_PCR_RPDEC2_RPS4EPT_MASK              0xFF000000
#define  B_PCH_DMI17_PCR_RPDEC2_RPS4EPT_OFFSET            24
#define  V_PCH_DMI17_PCR_RPDEC2_RPS4EPT                   0x3
#define  B_PCH_DMI17_PCR_RPDEC2_RPS4ERT_MASK              0xFF0000
#define  B_PCH_DMI17_PCR_RPDEC2_RPS4ERT_OFFSET            16
#define  V_PCH_DMI17_PCR_RPDEC2_RPS4ERT                   0x4
#define  B_PCH_DMI17_PCR_RPDEC2_RPS3EPT_MASK              0xFF00
#define  B_PCH_DMI17_PCR_RPDEC2_RPS3EPT_OFFSET            8
#define  V_PCH_DMI17_PCR_RPDEC2_RPS3EPT                   0x3
#define  B_PCH_DMI17_PCR_RPDEC2_RPS3ERT_MASK              0xFF
#define  B_PCH_DMI17_PCR_RPDEC2_RPS3ERT_OFFSET            0
#define  V_PCH_DMI17_PCR_RPDEC2_RPS3ERT                   0x3

#define  R_PCH_DMI17_PCR_RPDEC3                           0x1788
#define  B_PCH_DMI17_PCR_RPDEC3_RPDH_MASK                 (BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_RPDEC3_RPDH_OFFSET               16
#define  V_PCH_DMI17_PCR_RPDEC3_RPDH                      0x5
#define  B_PCH_DMI17_PCR_RPDEC3_RPS5EPT_MASK              0xFF00
#define  B_PCH_DMI17_PCR_RPDEC3_RPS5EPT_OFFSET            8
#define  V_PCH_DMI17_PCR_RPDEC3_RPS5EPT                   0x3
#define  B_PCH_DMI17_PCR_RPDEC3_RPS5ERT_MASK              0xFF
#define  B_PCH_DMI17_PCR_RPDEC3_RPS5ERT_OFFSET            0
#define  V_PCH_DMI17_PCR_RPDEC3_RPS5ERT                   0x5

#define  R_PCH_DMI17_PCR_DECCTL                           0x1904
#define  B_PCH_DMI17_PCR_DECCTL_RXIMDECEN                 BIT31
#define  B_PCH_DMI17_PCR_DECCTL_URRXMCTPNTCO              BIT30
#define  B_PCH_DMI17_PCR_DECCTL_RXMCTPBRCDECEN            BIT29
#define  B_PCH_DMI17_PCR_DECCTL_URRXUVDMRGRTRC            BIT28
#define  B_PCH_DMI17_PCR_DECCTL_URRXUVDMRBFRC             BIT27
#define  B_PCH_DMI17_PCR_DECCTL_LCRXPTMREQ                BIT26
#define  B_PCH_DMI17_PCR_DECCTL_RSVD_RW                   BIT25
#define  B_PCH_DMI17_PCR_DECCTL_LCRXERRMSG                BIT24
#define  B_PCH_DMI17_PCR_DECCTL_RXLTRMDECH                BIT23
#define  B_PCH_DMI17_PCR_DECCTL_RXADLEDDECEN              BIT22
#define  B_PCH_DMI17_PCR_DECCTL_RXSBEMCAPDECEN            BIT21
#define  B_PCH_DMI17_PCR_DECCTL_RXGPEDECEN                BIT20
#define  B_PCH_DMI17_PCR_DECCTL_RXVDMDECE                 BIT19
#define  B_PCH_DMI17_PCR_DECCTL_RXMCTPDECEN               BIT18
#define  B_PCH_DMI17_PCR_DECCTL_ICHKINVREQRMSGRBID        BIT17
#define  B_PCH_DMI17_PCR_DECCTL_URRXVMCTPBFRC             BIT16
#define  B_PCH_DMI17_PCR_DECCTL_URRXUORVDM                BIT15
#define  B_PCH_DMI17_PCR_DECCTL_URRXURIDVDM               BIT14
#define  B_PCH_DMI17_PCR_DECCTL_URRXURAVDM                BIT13
#define  B_PCH_DMI17_PCR_DECCTL_URRXULTVDM                BIT12
#define  B_PCH_DMI17_PCR_DECCTL_URRXURTRCVDM              BIT11
#define  B_PCH_DMI17_PCR_DECCTL_URRXUVDMUVID              BIT10
#define  B_PCH_DMI17_PCR_DECCTL_URRXUVDMINTELVID          BIT9
#define  B_PCH_DMI17_PCR_DECCTL_URVDME16DW                BIT8
#define  B_PCH_DMI17_PCR_DECCTL_VDMATAC                   BIT7
#define  B_PCH_DMI17_PCR_DECCTL_LCRXINT                   BIT6
#define  B_PCH_DMI17_PCR_DECCTL_DROPCPLATNZCE             BIT4
#define  B_PCH_DMI17_PCR_DECCTL_OUTBEXPCPLCHKEN           BIT3
#define  B_PCH_DMI17_PCR_DECCTL_MTRXLTC                   BIT2
#define  B_PCH_DMI17_PCR_DECCTL_BUSNUMZCHK                BIT1
#define  B_PCH_DMI17_PCR_DECCTL_ATSCE                     BIT0

#define  R_PCH_DMI17_PCR_PIDECCTL                         0x190C
#define  B_PCH_DMI17_PCR_PIDECCTL_CPLBNCHK                BIT0
#define  B_PCH_DMI17_PCR_PIDECCTL_CPLBNCHK_OFFSET         0

#define  R_PCH_DMI17_PCR_G5L0SCTL                         0x1E00
#define  B_PCH_DMI17_PCR_G5L0SCTL_G5UCNFTS_MASK           0xFF00
#define  B_PCH_DMI17_PCR_G5L0SCTL_G5UCNFTS_OFFSET         8
#define  V_PCH_DMI17_PCR_G5L0SCTL_G5UCNFTS                0xFF
#define  B_PCH_DMI17_PCR_G5L0SCTL_G5CCNFTS_MASK           0xFF
#define  B_PCH_DMI17_PCR_G5L0SCTL_G5CCNFTS_OFFSET         0
#define  V_PCH_DMI17_PCR_G5L0SCTL_G5CCNFTS                0xE6

#define  R_PCH_DMI17_PCR_DCAP                             0x2044
#define  B_PCH_DMI17_PCR_DCAP_MPS_MASK                    (BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_DCAP_MPS_OFFSET                  0
#define  V_PCH_DMI17_PCR_DCAP_MPS_256                     0x1

#define  R_PCH_DMI17_PCR_CMD                              0x2004
#define  B_PCH_DMI17_PCR_CMD_SEE                          BIT8

#define  R_PCH_DMI17_PCR_BCTRL                            0x203E
#define  B_PCH_DMI17_PCR_BCTRL_SE                         BIT1

#define  R_PCH_DMI17_PCR_DCTL                             0x2048
#define  B_PCH_DMI17_PCR_DCTL_MPS_MASK                    (BIT7 | BIT6 | BIT5)
#define  B_PCH_DMI17_PCR_DCTL_MPS_OFFSET                  5
#define  V_PCH_DMI17_PCR_DCTL_MPS_256                     0x1
#define  B_PCH_DMI17_PCR_DCTL_URE                         BIT3
#define  B_PCH_DMI17_PCR_DCTL_URE_OFFSET                  3
#define  B_PCH_DMI17_PCR_DCTL_FEE                         BIT2
#define  B_PCH_DMI17_PCR_DCTL_FEE_OFFSET                  2
#define  B_PCH_DMI17_PCR_DCTL_NFE                         BIT1
#define  B_PCH_DMI17_PCR_DCTL_NFE_OFFSET                  1
#define  B_PCH_DMI17_PCR_DCTL_CEE                         BIT0
#define  B_PCH_DMI17_PCR_DCTL_CEE_OFFSET                  0

#define  R_PCH_DMI17_PCR_DCAP2                            0x2064
#define  B_PCH_DMI17_PCR_DCAP2_PX10BTRS                   BIT17
#define  B_PCH_DMI17_PCR_DCAP2_PX10BTRS_OFFSET            17
#define  B_PCH_DMI17_PCR_DCAP2_PX10BTCS                   BIT16
#define  B_PCH_DMI17_PCR_DCAP2_PX10BTCS_OFFSET            16
#define  B_PCH_DMI17_PCR_DCAP2_AC64BS                     BIT8
#define  B_PCH_DMI17_PCR_DACP2_AC64BS_OFFSET              8
#define  B_PCH_DMI17_PCR_DCAP2_AC32BS                     BIT7
#define  B_PCH_DMI17_PCR_DCAP2_AC32BS_OFFSET              7
#define  B_PCH_DMI17_PCR_DCAP2_ARS                        BIT6
#define  B_PCH_DMI17_PCR_DCAP2_ARS_OFFSET                 6

#define  R_PCH_DMI17_PCR_DCTL2                            0x2068
#define  B_PCH_DMI17_PCR_DCTL2_PX10BTRE                   BIT11
#define  B_PCH_DMI17_PCR_DCTL2_PX10BTRE_OFFSET            11
#define  B_PCH_DMI17_PCR_DCTL2_AEB                        BIT7
#define  B_PCH_DMI17_PCR_DCTL2_AEB_OFFSET                 7
#define  B_PCH_DMI17_PCR_DCTL2_ARE                        BIT6
#define  B_PCH_DMI17_PCR_DCTL2_ARE_OFFSET                 6

#define  R_PCH_DMI17_PCR_LCAP                             0x204C
#define  B_PCH_DMI17_PCR_LCAP_ASPMOC                      BIT22
#define  B_PCH_DMI17_PCR_LCAP_ASPMOC_OFFSET               22
#define  B_PCH_DMI17_PCR_LCAP_EL1_MASK                    (BIT17 | BIT16 | BIT15)
#define  B_PCH_DMI17_PCR_LCAP_EL1_OFFSET                  15
#define  V_PCH_DMI17_PCR_LCAP_EL1                         0x6
#define  B_PCH_DMI17_PCR_LCAP_EL0_MASK                    (BIT14 | BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_LCAP_EL0_OFFSET                  12
#define  V_PCH_DMI17_PCR_LCAP_EL0                         0x4
#define  B_PCH_DMI17_PCR_LCAP_APMS_MASK                   (BIT11 | BIT10)
#define  B_PCH_DMI17_PCR_LCAP_APMS_OFFSET                 10
#define  V_PCH_DMI17_PCR_LCAP_APMS_DIS                    0x0
#define  V_PCH_DMI17_PCR_LCAP_APMS_L0S                    0x1
#define  V_PCH_DMI17_PCR_LCAP_APMS_L1                     0x2
#define  V_PCH_DMI17_PCR_LCAP_APMS_L0SL1                  0x3

#define  R_PCH_DMI17_PCR_LCAP2                            0x206C     ///< Link Capabilities 2
#define  R_PCH_DMI17_PCR_LCTL2                            0x2070     ///< Link Control 2

#define  R_PCH_DMI17_PCR_CCFG                             0x20D0
#define  B_PCH_DMI17_PCR_CCFG_UPMWPD                      BIT25
#define  B_PCH_DMI17_PCR_CCFG_UPMWPD_OFFSET               25
#define  B_PCH_DMI17_PCR_CCFG_UPSD                        BIT24
#define  B_PCH_DMI17_PCR_CCFG_UPSD_OFFSET                 24
#define  B_PCH_DMI17_PCR_CCFG_UNRD                        BIT12
#define  B_PCH_DMI17_PCR_CCFG_UNRD_OFFSET                 12
#define  B_PCH_DMI17_PCR_CCFG_UNRD_MASK                   0x3000

#define  R_PCH_DMI17_PCR_MPC2                             0x20D4
#define  B_PCH_DMI17_PCR_MPC2_L1SSESE                     BIT30
#define  B_PCH_DMI17_PCR_MPC2_ORCE_MASK                   (BIT15 | BIT14)
#define  B_PCH_DMI17_PCR_MPC2_ORCE_OFFSET                 14
#define  V_PCH_DMI17_PCR_MPC2_ORCE                        0x0
#define  B_PCH_DMI17_PCR_MPC2_PTNFAE                      BIT12
#define  B_PCH_DMI17_PCR_MPC2_LSTP                        BIT6
#define  B_PCH_DMI17_PCR_MPC2_EOIFD                       BIT1

#define  R_PCH_DMI17_PCR_MPC                              0x20D8
#define  B_PCH_DMI17_PCR_MPC_IRRCE                        BIT25
#define  B_PCH_DMI17_PCR_MPC_IRRCE_OFFSET                 25
#define  B_PCH_DMI17_PCR_MPC_FCDL1E                       BIT21
#define  B_PCH_DMI17_PCR_MPC_CCEL_MASK                    (BIT17 | BIT16 | BIT15)
#define  B_PCH_DMI17_PCR_MPC_CCEL_OFFSET                  15
#define  V_PCH_DMI17_PCR_MPC_CCEL                         0x4
#define  B_PCH_DMI17_PCR_MPC_BT                           BIT2
#define  B_PCH_DMI17_PCR_MPC_BT_OFFSET                    2

#define  R_PCH_DMI17_PCR_PWRCTL                           0x20E8
#define  B_PCH_DMI17_PCR_PWRCTL_DARECE                    BIT28
#define  B_PCH_DMI17_PCR_PWRCTL_LIFECF                    BIT23
#define  B_PCH_DMI17_PCR_PWRCTL_WPDMPGEP                  BIT17
#define  B_PCH_DMI17_PCR_PWRCTL_TXSWING                   BIT13

#define  R_PCH_DMI17_PCR_DC                               0x20EC
#define  B_PCH_DMI17_PCR_DC_DCT1C                         BIT15
#define  B_PCH_DMI17_PCR_DC_DCT1C_OFFSET                  15
#define  B_PCH_DMI17_PCR_DC_DCT0C                         BIT14
#define  B_PCH_DMI17_PCR_DC_DCT0C_OFFSET                  14
#define  B_PCH_DMI17_PCR_DC_COM                           BIT13
#define  B_PCH_DMI17_PCR_DC_COM_OFFSET                    13

#define  R_PCH_DMI17_PCR_IPCS                             0x20F0
#define  B_PCH_DMI17_PCR_IPCS_IMPS                        BIT8
#define  B_PCH_DMI17_PCR_IPCS_IMPS_OFFSET                 8
#define  B_PCH_DMI17_PCR_IPCS_IMPS_MASK                   0x700
#define  V_PCH_DMI17_PCR_IPCS_IMPS                        0x1

#define  R_PCH_DMI17_PCR_STRPFUSECFG                      0x20FC
#define  B_PCH_DMI17_PCR_STRPFUSECFG_SERM                 BIT29
#define  B_PCH_DMI17_PCR_STRPFUSECFG_SERM_OFFSET          29
#define  B_PCH_DMI17_PCR_STRPFUSECFG_PXIP_MASK            (BIT27 | BIT26 | BIT25 | BIT24)
#define  B_PCH_DMI17_PCR_STRPFUSECFG_PXIP_OFFSET          24
#define  V_PCH_DMI17_PCR_STRPFUSECFG_PXIP                 0x0

#define  R_PCH_DMI17_PCR_AECH                             0x2100
#define  B_PCH_DMI17_PCR_AECH_CID_MASK                    0xFFFF
#define  B_PCH_DMI17_PCR_AECH_CID_OFFSET                  0
#define  V_PCH_DMI17_PCR_AECH_CID                         0x1
#define  B_PCH_DMI17_PCR_AECH_CV_MASK                     (BIT19 | BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_AECH_CV_OFFSET                   16
#define  V_PCH_DMI17_PCR_AECH_CV                          0x1
#define  B_PCH_DMI17_PCR_AECH_NCO_MASK                    0xFFF00000
#define  B_PCH_DMI17_PCR_AECH_NCO_OFFSET                  20
#define  V_PCH_DMI17_PCR_AECH_NCO                         0x280

#define  R_PCH_DMI17_PCR_VCCH                             0x2280
#define  B_PCH_DMI17_PCR_VCCH_CID_MASK                    0xFFFF
#define  B_PCH_DMI17_PCR_VCCH_CID_OFFSET                  0
#define  V_PCH_DMI17_PCR_VCCH_CID_3_VC                    0x2
#define  B_PCH_DMI17_PCR_VCCH_CV_MASK                     (BIT19 | BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_VCCH_CV_OFFSET                   16
#define  V_PCH_DMI17_PCR_VCCH_CV_3_VC                     0x1
#define  B_PCH_DMI17_PCR_VCCH_NCO_MASK                    0xFFF00000
#define  B_PCH_DMI17_PCR_VCCH_NCO_OFFSET                  20
#define  V_PCH_DMI17_PCR_VCCH_NCO                         0xA30

#define  R_PCH_DMI17_PCR_L1SCAP                           0x2204
#define  B_PCH_DMI17_PCR_L1SCAP_L1PSS                     BIT4
#define  B_PCH_DMI17_PCR_L1SCAP_AL11S                     BIT3
#define  B_PCH_DMI17_PCR_L1SCAP_AL12S                     BIT2
#define  B_PCH_DMI17_PCR_L1SCAP_PPL11S                    BIT1
#define  B_PCH_DMI17_PCR_L1SCAP_PPL12S                    BIT0

#define  R_PCH_DMI17_PCR_PVCCR1                           0x2284
#define  B_PCH_DMI17_PCR_PVCCR1_EVCC_MASK                 (BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PVCCR1_EVCC_OFFSET               0
#define  V_PCH_DMI17_PCR_PVCCR1_EVCC_2_VC                 0x1
#define  V_PCH_DMI17_PCR_PVCCR1_EVCC_3_VC                 0x2

#define  R_PCH_DMI17_PCR_V0VCRC                           0x2290
#define  B_PCH_DMI17_PCR_V0VCRC_MTS_MASK                  0x7F0000

#define  R_PCH_DMI17_PCR_V0CTL                            0x2294
#define  B_PCH_DMI17_PCR_V0CTL_TVM_MASK                   0xFE
#define  V_PCH_DMI17_PCR_V0CTL_ETVM_MASK                  0xFC00
#define  B_PCH_DMI17_PCR_V0CTL_TVM_OFFSET                 1
#define  V_PCH_DMI17_PCR_V0CTL_TVM_NO_VC                  0x7F
#define  V_PCH_DMI17_PCR_V0CTL_TVM_EN_VC                  0x3E

#define  R_PCH_DMI17_PCR_V0STS                            0x229A

#define  R_PCH_DMI17_PCR_V1VCRC                           0x229C
#define  B_PCH_DMI17_PCR_V1VCRC_MTS_MASK                  0x7F0000

#define  R_PCH_DMI17_PCR_V1CTL                            0x22A0
#define  B_PCH_DMI17_PCR_V1CTL_TVM_MASK                   0xFE
#define  B_PCH_DMI17_PCR_V1CTL_TVM                        BIT1
#define  B_PCH_DMI17_PCR_V1CTL_TVM_OFFSET                 1
#define  V_PCH_DMI17_PCR_V1CTL_TVM_EN_VC                  0x60
#define  B_PCH_DMI17_PCR_V1CTL_ID_MASK                    0xF000000
#define  B_PCH_DMI17_PCR_V1CTL_ID_OFFSET                  24
#define  V_PCH_DMI17_PCR_V1CTL_ID_ONE                     1
#define  B_PCH_DMI17_PCR_V1CTL_EN_MASK                    0x80000000
#define  B_PCH_DMI17_PCR_V1CTL_EN                         BIT31

#define  R_PCH_DMI17_PCR_V1STS                            0x22A6

#define  R_PCH_DMI17_PCR_VMCTL                            0x22B0
#define  B_PCH_DMI17_PCR_VMCTL_TVM_MASK                   0xFE
#define  B_PCH_DMI17_PCR_VMCTL_TVM                        BIT7
#define  B_PCH_DMI17_PCR_VMCTL_ID_MASK                    0xF000000
#define  B_PCH_DMI17_PCR_VMCTL_ID_OFFSET                  24
#define  V_PCH_DMI17_PCR_VMCTL_ID_TWO                     7
#define  B_PCH_DMI17_PCR_VMCTL_EN_MASK                    0x80000000
#define  B_PCH_DMI17_PCR_VMCTL_EN                         BIT31

#define  R_PCH_DMI17_PCR_VMSTS                            0x22B6

#define  R_PCH_DMI17_PCR_PCIERTP1                         0x2300
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X1_MASK               (BIT23 | BIT22 | BIT21 | BIT20)
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X1_OFFSET             20
#define  V_PCH_DMI17_PCR_PCIERTP1_G2X1                    0xB
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X2_MASK               (BIT19 | BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X2_OFFSET             16
#define  V_PCH_DMI17_PCR_PCIERTP1_G2X2                    0x7
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X4_MASK               (BIT15 | BIT14 | BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_PCIERTP1_G2X4_OFFSET             12
#define  V_PCH_DMI17_PCR_PCIERTP1_G2X4                    0xE
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X1_MASK               (BIT11 | BIT10 | BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X1_OFFSET             8
#define  V_PCH_DMI17_PCR_PCIERTP1_G1X1                    0x5
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X2_MASK               (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X2_OFFSET             4
#define  V_PCH_DMI17_PCR_PCIERTP1_G1X2                    0xB
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X4_MASK               (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PCIERTP1_G1X4_OFFSET             0
#define  V_PCH_DMI17_PCR_PCIERTP1_G1X4                    0x0

#define  R_PCH_DMI17_PCR_G4L0SCTL                         0x2310
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL_MASK          0xFF000000
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL_OFFSET        24
#define  V_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL               0x28
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC_MASK            (BIT23 | BIT22)
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC_OFFSET          22
#define  V_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC                 0x3
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS_MASK           0xFF00
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS_OFFSET         8
#define  V_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS                0x80
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS_MASK           0xFF
#define  B_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS_OFFSET         0
#define  V_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS                0x5B

#define  R_PCH_DMI17_PCR_PCIENFTS                         0x2314
#define  B_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS_MASK           0xFF000000
#define  B_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS_OFFSET         24
#define  V_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS                0x7E
#define  B_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS_MASK           0xFF0000
#define  B_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS_OFFSET         16
#define  V_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS                0x5B
#define  B_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS_MASK           0xFF00
#define  B_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS_OFFSET         8
#define  V_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS                0x3F
#define  B_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS_MASK           0xFF
#define  B_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS_OFFSET         0
#define  V_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS                0x2C

#define  R_PCH_DMI17_PCR_PCIECFG2                         0x2320
#define  B_PCH_DMI17_PCR_PCIECFG2_RLLG3R                  BIT27
#define  B_PCH_DMI17_PCR_PCIECFG2_RLLG3R_OFFSET           27
#define  B_PCH_DMI17_PCR_PCIECFG2_CROAOV                  BIT24
#define  B_PCH_DMI17_PCR_PCIECFG2_CROAOE                  BIT23
#define  B_PCH_DMI17_PCR_PCIECFG2_CRSREN                  BIT22
#define  B_PCH_DMI17_PCR_PCIECFG2_CRSREN_OFFSET           22
#define  B_PCH_DMI17_PCR_PCIECFG2_PMET_MASK               (BIT21 | BIT20)
#define  B_PCH_DMI17_PCR_PCIECFG2_PMET_OFFSET             20
#define  V_PCH_DMI17_PCR_PCIECFG2_PMET                    0x1
#define  B_PCH_DMI17_PCR_PCIECFG2_DTCA                    BIT4
#define  B_PCH_DMI17_PCR_PCIECFG2_DTCA_OFFSET             4

#define  R_PCH_DMI17_PCR_PCIEDBG                            0x2324
#define  B_PCH_DMI17_PCR_PCIEDBG_USSP_MASK                  (BIT27 | BIT26)
#define  B_PCH_DMI17_PCR_PCIEDBG_USSP_OFFSET                26
#define  V_PCH_DMI17_PCR_PCIEDBG_USSP                       0x01
#define  B_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS_MASK   (BIT25 | BIT24)
#define  B_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS_OFFSET 24
#define  V_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS        0x00
#define  B_PCH_DMI17_PCR_PCIEDBG_CTONFAE                    BIT14
#define  B_PCH_DMI17_PCR_PCIEDBG_CTONFAE_OFFSET             14
#define  B_PCH_DMI17_PCR_PCIEDBG_LDSWQRP                    BIT13
#define  B_PCH_DMI17_PCR_PCIEDBG_LDSWQRP_OFFSET             13
#define  B_PCH_DMI17_PCR_PCIEDBG_SPCE                       BIT5
#define  B_PCH_DMI17_PCR_PCIEDBG_SPCE_OFFSET                5
#define  B_PCH_DMI17_PCR_PCIEDBG_DTCA                       BIT4
#define  B_PCH_DMI17_PCR_PCIEDBG_DTCA_OFFSET                4

#define  R_PCH_DMI17_PCR_PCIEALC                          0x2338
#define  B_PCH_DMI17_PCR_PCIEALC_ITLRCLD                  BIT29
#define  B_PCH_DMI17_PCR_PCIEALC_ITLRCLD_OFFSET           29
#define  B_PCH_DMI17_PCR_PCIEALC_ILLRCLD                  BIT28
#define  B_PCH_DMI17_PCR_PCIEALC_ILLRCLD_OFFSET           28
#define  B_PCH_DMI17_PCR_PCIEALC_SSRLD                    BIT24
#define  B_PCH_DMI17_PCR_PCIEALC_SSRLD_OFFSET             24
#define  B_PCH_DMI17_PCR_PCIEALC_SSRRS                    BIT23
#define  B_PCH_DMI17_PCR_PCIEALC_SSRRS_OFFSET             23
#define  B_PCH_DMI17_PCR_PCIEALC_RTD3PDSP                 BIT21
#define  B_PCH_DMI17_PCR_PCIEALC_RTD3PDSP_OFFSET          21
#define  B_PCH_DMI17_PCR_PCIEALC_PDSP                     BIT20
#define  B_PCH_DMI17_PCR_PCIEALC_PDSP_OFFSET              20

#define  R_PCH_DMI17_PCR_LTROVR2                          0x2404
#define  B_PCH_DMI17_PCR_LTROVR2_LTRNSOVREN               BIT1
#define  B_PCH_DMI17_PCR_LTROVR2_LTRSOVREN                BIT0

#define  R_PCH_DMI17_PCR_PCIEPMECTL                       0x2420
#define  B_PCH_DMI17_PCR_PCIEPMECTL_DLSULPPGE             BIT30
#define  B_PCH_DMI17_PCR_PCIEPMECTL_DLSULPPGE_OFFSET      30
#define  B_PCH_DMI17_PCR_PCIEPMECTL_DLSULDLSD             BIT29
#define  B_PCH_DMI17_PCR_PCIEPMECTL_DLSULDLSD_OFFSET      29
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV             (BIT16 | BIT15 | BIT14)
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV_OFFSET      14
#define  V_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV             0x2
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV              0x3FF0
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV_OFFSET       4
#define  V_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV              0x32
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1FSOE                BIT0
#define  B_PCH_DMI17_PCR_PCIEPMECTL_L1FSOE_OFFSET         0

#define  R_PCH_DMI17_PCR_STRPFUSECFG2                     0x2414
#define  B_PCH_DMI17_PCR_STRPFUSECFG2                     (BIT9 | BIT8 | BIT7)
#define  N_PCH_DMI17_PCR_STRPFUSECFG2                     7

#define  R_PCH_DMI17_PCR_PCIEPMECTL2                      0x2424
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPMCSRE              BIT27
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPMCSRE_OFFSET       27
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPGEXH_MASK          (BIT15 | BIT14)
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPGEXH_OFFSET        14
#define  V_PCH_DMI17_PCR_PCIEPMECTL2_CPGEXH               0x0
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPGENH_MASK          (BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_PCIEPMECTL2_CPGENH_OFFSET        12
#define  V_PCH_DMI17_PCR_PCIEPMECTL2_CPGENH               0x0

#define  R_PCH_DMI17_PCR_PGCBCTL1                         0x242C
#define  B_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP_MASK          (BIT11 | BIT10)
#define  B_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP_OFFSET        10
#define  V_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP               0x3

#define  R_PCH_DMI17_PCR_PME_CTL3                         0x2434
#define  B_PCH_DMI17_PCR_PME_CTL3_L1PGAUTOPGEN            BIT4
#define  B_PCH_DMI17_PCR_PME_CTL3_L1PGAUTOPGEN_OFFSET     4
#define  B_PCH_DMI17_PCR_PME_CTL3_OSCCGH                  (BIT3 | BIT2)
#define  N_PCH_DMI17_PCR_PME_CTL3_OSCCGH                  2
#define  B_PCH_DMI17_PCR_PME_CTL3_OSCCGH_OFFSET           2
#define  V_PCH_DMI17_PCR_PME_CTL3_OSCCGH_0US              0x0
#define  V_PCH_DMI17_PCR_PME_CTL3_OSCCGH_1US              0x1
#define  B_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH_MASK        (BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH_OFFSET      0
#define  V_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH             0x0

#define  R_PCH_DMI17_PCR_EQCFG1                           0x2450
#define  B_PCH_DMI17_PCR_EQCFG1_LERSMIE                   BIT21
#define  B_PCH_DMI17_PCR_EQCFG1_RTLEPCEB                  BIT16
#define  B_PCH_DMI17_PCR_EQCFG1_RTPCOE                    BIT15
#define  B_PCH_DMI17_PCR_EQCFG1_RWTNEVE_MASK              (BIT11 | BIT10 | BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_EQCFG1_RWTNEVE_OFFSET            8
#define  V_PCH_DMI17_PCR_EQCFG1_RWTNEVE                   0x1
#define  B_PCH_DMI17_PCR_EQCFG1_HAPCCPIE                  BIT5
#define  B_PCH_DMI17_PCR_EQCFG1_TUPP                      BIT1

#define  R_PCH_DMI17_PCR_RTPCL1                           0x2454
#define  N_PCH_DMI17_PCR_RTPCL1_RTPRECL2PL4               24
#define  N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL1PL3              18
#define  N_PCH_DMI17_PCR_RTPCL1_RTPRECL1PL2               12
#define  N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL0PL1              6
#define  N_PCH_DMI17_PCR_RTPCL1_RTPRECL0PL0               0
#define  B_PCH_DMI17_PCR_RTPCL1_PCM                       BIT31
#define  B_PCH_DMI17_PCR_RTPCL1_RTPRECL2PL4               (0x3F << N_PCH_DMI17_PCR_RTPCL1_RTPRECL2PL4)
#define  B_PCH_DMI17_PCR_RTPCL1_RTPOSTCL1PL3              (0x3F << N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL1PL3)
#define  B_PCH_DMI17_PCR_RTPCL1_RTPRECL1PL2               (0x3F << N_PCH_DMI17_PCR_RTPCL1_RTPRECL1PL2)
#define  B_PCH_DMI17_PCR_RTPCL1_RTPOSTCL0PL1              (0x3F << N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL0PL1)
#define  B_PCH_DMI17_PCR_RTPCL1_RTPRECL0PL0               (0x3F << N_PCH_DMI17_PCR_RTPCL1_RTPRECL0PL0)

#define  R_PCH_DMI17_PCR_RTPCL2                           0x2458
#define  N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL4PL9              24
#define  N_PCH_DMI17_PCR_RTPCL2_RTPRECL4PL8               18
#define  N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL3PL7              12
#define  N_PCH_DMI17_PCR_RTPCL2_RTPRECL3PL6               6
#define  N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL2PL5              0
#define  B_PCH_DMI17_PCR_RTPCL2_RTPOSTCL4PL9              (0x3F << N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL4PL9)
#define  B_PCH_DMI17_PCR_RTPCL2_RTPRECL4PL8               (0x3F << N_PCH_DMI17_PCR_RTPCL2_RTPRECL4PL8)
#define  B_PCH_DMI17_PCR_RTPCL2_RTPOSTCL3PL7              (0x3F << N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL3PL7)
#define  B_PCH_DMI17_PCR_RTPCL2_RTPRECL3PL6               (0x3F << N_PCH_DMI17_PCR_RTPCL2_RTPRECL3PL6)
#define  B_PCH_DMI17_PCR_RTPCL2_RTPOSTCL2PL5              (0x3F << N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL2PL5)

#define  R_PCH_DMI17_PCR_HAEQ                             0x2468
#define  B_PCH_DMI17_PCR_HAEQ_HAPCCPI_MASK                0xF0000000
#define  B_PCH_DMI17_PCR_HAEQ_HAPCCPI_OFFSET              28
#define  V_PCH_DMI17_PCR_HAEQ_HAPCCPI                     0x0
#define  B_PCH_DMI17_PCR_HAEQ_MACFOMC                     BIT19
#define  B_PCH_DMI17_PCR_HAEQ_DL_MASK                     0xFF00
#define  B_PCH_DMI17_PCR_HAEQ_DL_OFFSET                   8
#define  V_PCH_DMI17_PCR_HAEQ_DL                          0xE

#define  R_PCH_DMI17_PCR_G3L0SCTL                         0x2478
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL_MASK          0xFF000000
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL_OFFSET        24
#define  V_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL               0x28
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS_MASK           0xFF00
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS_OFFSET         8
#define  V_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS                0x40
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS_MASK           0xFF
#define  B_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS_OFFSET         0
#define  V_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS                0x2C

#define  R_PCH_DMI17_PCR_EQCFG2                           0x247C
#define  B_PCH_DMI17_PCR_EQCFG2_PCET_MASK                 0xF0000
#define  B_PCH_DMI17_PCR_EQCFG2_PCET_OFFSET               16
#define  V_PCH_DMI17_PCR_EQCFG2_PCET                      0x2
#define  B_PCH_DMI17_PCR_EQCFG2_HAPCSB_MASK               0xF000
#define  B_PCH_DMI17_PCR_EQCFG2_HAPCSB_OFFSET             12
#define  V_PCH_DMI17_PCR_EQCFG2_HAPCSB                    0x2
#define  B_PCH_DMI17_PCR_EQCFG2_REWMET_MASK               0xFF
#define  B_PCH_DMI17_PCR_EQCFG2_REWMET_OFFSET             0
#define  V_PCH_DMI17_PCR_EQCFG2_REWMET                    0x10

#define  R_PCH_DMI17_PCR_EQCFG4                           0x248C
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GRTLEPCEB             BIT16
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GRTPCOE               BIT15
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE_MASK         0xF00
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE_OFFSET       8
#define  V_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE              0x3
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI_MASK         0xF0
#define  B_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI_OFFSET       3
#define  V_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI              0x2

#define  R_PCH_DMI17_PCR_CTRL2                            0x24A4
#define  B_PCH_DMI17_PCR_CTRL2_PMETOFD                    BIT6
#define  B_PCH_DMI17_PCR_CTRL2_PMETOFD_OFFSET             6

#define  R_PCH_DMI17_PCR_IOSFC3TC                         0x24C8
#define  B_PCH_DMI17_PCR_IOSFC3TC_C0CPDC_MASK             0xFF00
#define  B_PCH_DMI17_PCR_IOSFC3TC_C0CPDC_OFFSET           8
#define  V_PCH_DMI17_PCR_IOSFC3TC_C0CPDC                  0x4

#define  R_PCH_DMI17_PCR_PX16GRTPCL1                      0x24DC
#define  N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL2PL4          24
#define  N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL1PL3         18
#define  N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL1PL2          12
#define  N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL0PL1         6
#define  N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL0PL0          0
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_PCM                  BIT31
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL2PL4          (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL2PL4)
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL1PL3         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL1PL3)
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL1PL2          (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL1PL2)
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL0PL1         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL0PL1)
#define  B_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL0PL0          (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL0PL0)

#define  R_PCH_DMI17_PCR_PX16GRTPCL2                      0x24E0
#define  N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL4PL9         24
#define  N_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL4PL8          18
#define  N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL3PL7         12
#define  N_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL3PL6          6
#define  N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL2PL5         0
#define  B_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL4PL9         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL4PL9)
#define  B_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL4PL8          (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL4PL8)
#define  B_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL3PL7         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL3PL7)
#define  B_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL3PL6          (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL3PL6)
#define  B_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL2PL5         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL2PL5)

#define  R_PCH_DMI17_PCR_PX16GRTPCL3                      0x24E4
#define  N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL7             24
#define  N_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL6            18
#define  N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL6             12
#define  N_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL5            6
#define  N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL5PL10         0
#define  B_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL7             (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL7)
#define  B_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL6            (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL6)
#define  B_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL6             (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL6)
#define  B_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL5            (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL3_RTPOSTCL5)
#define  B_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL5PL10         (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL3_RTPRECL5PL10)

#define  R_PCH_DMI17_PCR_PX16GRTPCL4                      0x24E8
#define  N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL9            24
#define  N_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL9             18
#define  N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL8            12
#define  N_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL8             6
#define  N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL7            0
#define  B_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL9            (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL9)
#define  B_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL9             (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL9)
#define  B_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL8            (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL8)
#define  B_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL8             (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL4_RTPRECL8)
#define  B_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL7            (0x3F << N_PCH_DMI17_PCR_PX16GRTPCL4_RTPOSTCL7)

#define  R_PCH_DMI17_PCR_EQCFG5                           0x24F8
#define  B_PCH_DMI17_PCR_EQCFG5_PCET_MASK                 0xF0000
#define  B_PCH_DMI17_PCR_EQCFG5_PCET_OFFSET               16
#define  V_PCH_DMI17_PCR_EQCFG5_PCET                      0x2
#define  B_PCH_DMI17_PCR_EQCFG5_HAPCSB_MASK               0xF000
#define  B_PCH_DMI17_PCR_EQCFG5_HAPCSB_OFFSET             12
#define  V_PCH_DMI17_PCR_EQCFG5_HAPCSB                    0x0

#define  R_PCH_DMI17_PCR_L0P0P1PCM                        0x2500
#define  R_PCH_DMI17_PCR_L0P1P2P3PCM                      0x2504
#define  R_PCH_DMI17_PCR_L0P3P4PCM                        0x2508
#define  R_PCH_DMI17_PCR_L0P5P6PCM                        0x250C
#define  R_PCH_DMI17_PCR_L0P6P7P8PCM                      0x2510
#define  R_PCH_DMI17_PCR_L0P8P9PCM                        0x2514
#define  R_PCH_DMI17_PCR_L0P10PCM                         0x2518
#define  R_PCH_DMI17_PCR_L0LFFS                           0x251C

#define  R_PCH_DMI17_PCR_PX16GP0P1PCM                     0x2520
#define  R_PCH_DMI17_PCR_PX16GP1P2P3PCM                   0x2524
#define  R_PCH_DMI17_PCR_PX16GP3P4PCM                     0x2528
#define  R_PCH_DMI17_PCR_PX16GP5P6PCM                     0x252C
#define  R_PCH_DMI17_PCR_PX16GP6P7P8PCM                   0x2530
#define  R_PCH_DMI17_PCR_PX16GP8P9PCM                     0x2534
#define  R_PCH_DMI17_PCR_PX16GP10PCM                      0x2538
#define  R_PCH_DMI17_PCR_PX16GLFFS                        0x253C

#define  R_PCH_DMI17_PCR_COCTL                            0x2594
#define  B_PCH_DMI17_PCR_COCTL_NPCLM_MASK                 (BIT16 | BIT15)
#define  B_PCH_DMI17_PCR_COCTL_NPCLM_OFFSET               13
#define  V_PCH_DMI17_PCR_COCTL_NPCLM                      0x2
#define  B_PCH_DMI17_PCR_COCTL_PCLM_MASK                  (BIT14 | BIT13)
#define  B_PCH_DMI17_PCR_COCTL_PCLM_OFFSET                13
#define  V_PCH_DMI17_PCR_COCTL_PCLM                       0x2
#define  B_PCH_DMI17_PCR_COCTL_ROAOP                      BIT11
#define  B_PCH_DMI17_PCR_COCTL_ROAOP_OFFSET               11

#define  R_PCH_DMI17_PCR_ADVMCTRL                         0x25BC
#define  B_PCH_DMI17_PCR_ADVMCTRL_F10BTSE                 BIT24
#define  B_PCH_DMI17_PCR_ADVMCTRL_F10BTSE_OFFSET          24
#define  B_PCH_DMI17_PCR_ADVMCTRL_CCBE                    BIT23
#define  B_PCH_DMI17_PCR_ADVMCTRL_CCBE_OFFSET             23
#define  B_PCH_DMI17_PCR_ADVMCTRL_INRXL0CTRL              BIT22
#define  B_PCH_DMI17_PCR_ADVMCTRL_INRXL0CTRL_OFFSET       22
#define  B_PCH_DMI17_PCR_ADVMCTRL_ACCRM                   BIT21
#define  B_PCH_DMI17_PCR_ADVMCTRL_ACCRM_OFFSET            21
#define  B_PCH_DMI17_PCR_ADVMCTRL_EIOSDISDS               BIT20
#define  B_PCH_DMI17_PCR_ADVMCTRL_EIOSDISDS_OFFSET        20
#define  B_PCH_DMI17_PCR_ADVMCTRL_EIOSMASKRX              BIT19
#define  B_PCH_DMI17_PCR_ADVMCTRL_EIOSMASKRX_OFFSET       19
#define  B_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC                (BIT18 | BIT17 | BIT16)
#define  B_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC_OFFSET         16
#define  V_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC                0x6
#define  B_PCH_DMI17_PCR_ADVMCTRL_RXL0DC                  BIT15
#define  B_PCH_DMI17_PCR_ADVMCTRL_RXL0DC_OFFSET           15
#define  B_PCH_DMI17_PCR_ADVMCTRL_G3STFER                 BIT13
#define  B_PCH_DMI17_PCR_ADVMCTRL_G3STFER_OFFSET          13
#define  B_PCH_DMI17_PCR_ADVMCTRL_RRLLCL                  BIT11
#define  B_PCH_DMI17_PCR_ADVMCTRL_RRLLCL_OFFSET           11
#define  B_PCH_DMI17_PCR_ADVMCTRL_RLLG12R                 BIT10
#define  B_PCH_DMI17_PCR_ADVMCTRL_RLLG12R_OFFSET          10
#define  B_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT          (BIT7 | BIT6 | BIT5)
#define  B_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT_OFFSET   5
#define  V_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT          0x2

#define  R_PCH_DMI17_PCR_PGTHRES                          0x25C0
#define  B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV              (BIT31 | BIT30 | BIT29)
#define  B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV_OFFSET       29
#define  V_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV              0x2
#define  B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV               0x3FF0000
#define  B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV_OFFSET        16
#define  V_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV               0x32

#define  R_PCH_DMI17_PCR_HWSNR                            0x25F0
#define  B_PCH_DMI17_PCR_HWSNR_EEH_MASK                   (BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_HWSNR_EEH_OFFSET                 8
#define  V_PCH_DMI17_PCR_HWSNR_EEH                        0x2
#define  B_PCH_DMI17_PCR_HWSNR_REPW_MASK                  (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_HWSNR_REPW_OFFSET                4
#define  V_PCH_DMI17_PCR_HWSNR_REPW                       0x1
#define  B_PCH_DMI17_PCR_HWSNR_BEPW_MASK                  (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_HWSNR_BEPW_OFFSET                0
#define  V_PCH_DMI17_PCR_HWSNR_BEPW                       0x5

#define  R_PCH_DMI17_PCR_PGCTRL                           0x25F4
#define  B_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT_MASK         (BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT_OFFSET       0
#define  V_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT              0x2

#define  R_PCH_DMI17_PCR_ACRG3                            0x26CC
#define  B_PCH_DMI17_PCR_ACRG3_RRXDME                     BIT27
#define  B_PCH_DMI17_PCR_ACRG3_RRXDME_OFFSET              27
#define  B_PCH_DMI17_PCR_ACRG3_CBGM                       BIT21
#define  B_PCH_DMI17_PCR_ACRG3_CBGM_OFFSET                21
#define  B_PCH_DMI17_PCR_ACRG3_ADESKEW_DIS                BIT10
#define  B_PCH_DMI17_PCR_ACRG3_ADESKEW_DIS_OFFSET         10

#define  R_PCH_DMI17_PCR_PCIERTP3                         0x26A0
#define  B_PCH_DMI17_PCR_PCIERTP3_G1X8_MASK               (BIT7 | BIT6 | BIT5 | BIT4)
#define  B_PCH_DMI17_PCR_PCIERTP3_G1X8_OFFSET             4
#define  V_PCH_DMI17_PCR_PCIERTP3_G1X8                    0x5
#define  B_PCH_DMI17_PCR_PCIERTP3_G1X16_MASK              (BIT3 | BIT2 | BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_PCIERTP3_G1X16_OFFSET            0
#define  V_PCH_DMI17_PCR_PCIERTP3_G1X16                   0x0

#define  R_PCH_DMI17_PCR_DPCCAPR                          0x2A04
#define  B_PCH_DMI17_PCR_DPCCAPR_RPEFDPC                  BIT5

#define  R_PCH_DMI17_PCR_SPEECH                           0x2A30
#define  B_PCH_DMI17_PCR_SPEECH_NCO_MASK                  0xFFF00000
#define  B_PCH_DMI17_PCR_SPEECH_NCO_OFFSET                20
#define  V_PCH_DMI17_PCR_SPEECH_NCO                       0xA90
#define  B_PCH_DMI17_PCR_SPEECH_CV_MASK                   0xF0000
#define  B_PCH_DMI17_PCR_SPEECH_CV_OFFSET                 16
#define  V_PCH_DMI17_PCR_SPEECH_CV                        0x1
#define  B_PCH_DMI17_PCR_SPEECH_PCIEECID_MASK             0xFFFF
#define  B_PCH_DMI17_PCR_SPEECH_PCIEECID_OFFSET           0
#define  V_PCH_DMI17_PCR_SPEECH_PCIEECID                  0x19

#define  R_PCH_DMI17_PCR_DLFECH                           0x2A90
#define  B_PCH_DMI17_PCR_DLFECH_NCO_MASK                  0xFFF00000
#define  B_PCH_DMI17_PCR_DLFECH_NCO_OFFSET                20
#define  V_PCH_DMI17_PCR_DLFECH_NCO                       0xA9C
#define  B_PCH_DMI17_PCR_DLFECH_CV_MASK                   0xF0000
#define  B_PCH_DMI17_PCR_DLFECH_CV_OFFSET                 16
#define  V_PCH_DMI17_PCR_DLFECH_CV                        0x1
#define  B_PCH_DMI17_PCR_DLFECH_PCIECID_MASK              0xFFFF
#define  B_PCH_DMI17_PCR_DLFECH_PCIECID_OFFSET            0
#define  V_PCH_DMI17_PCR_DLFECH_PCIECID                   0x25

#define  R_PCH_DMI17_PCR_DLFCAP                           0x2A94
#define  B_PCH_DMI17_PCR_DLFCAP_LSFCS                     BIT0
#define  B_PCH_DMI17_PCR_DLFCAP_LSFCS_OFFSET              0

#define  R_PCH_DMI17_PCR_PL16GECH                         0x2A9C
#define  B_PCH_DMI17_PCR_PL16GECH_NCO_MASK                0xFFF00000
#define  B_PCH_DMI17_PCR_PL16GECH_NCO_OFFSET              20
#define  V_PCH_DMI17_PCR_PL16GECH_NCO                     0xEDC
#define  B_PCH_DMI17_PCR_PL16GECH_CV_MASK                 0xF0000
#define  B_PCH_DMI17_PCR_PL16GECH_CV_OFFSET               16
#define  V_PCH_DMI17_PCR_PL16GECH_CV                      0x1
#define  B_PCH_DMI17_PCR_PL16GECH_PCIECID_MASK            0xFFFF
#define  B_PCH_DMI17_PCR_PL16GECH_PCIECID_OFFSET          0
#define  V_PCH_DMI17_PCR_PL16GECH_PCIECID                 0x26

#define  R_PCH_DMI17_PCR_PL16L01EC                        0x2ABC
#define  R_PCH_DMI17_PCR_PL16L23EC                        0x2ABE
#define  R_PCH_DMI17_PCR_PL16L45EC                        0x2AC0
#define  R_PCH_DMI17_PCR_PL16L67EC                        0x2AC2
#define  B_PCH_DMI17_PCR_GEN4_UPL1357TP                   0xF000
#define  N_PCH_DMI17_PCR_GEN4_UPL1357TP                   12
#define  B_PCH_DMI17_PCR_GEN4_UPL0246TP                   0x00F0
#define  N_PCH_DMI17_PCR_GEN4_UPL0246TP                   4
#define  V_PCH_DMI17_PCR_GEN4_UPL0TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL1TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL2TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL3TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL4TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL5TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL6TP                      7
#define  V_PCH_DMI17_PCR_GEN4_UPL7TP                      7

#define  R_PCH_DMI17_PCR_ACGR3S2                          0x2C50
#define  B_PCH_DMI17_PCR_ACGR3S2_DTCAVCM                  BIT27
#define  B_PCH_DMI17_PCR_ACGR3S2_DTCAVCM_OFFSET           27
#define  B_PCH_DMI17_PCR_ACGR3S2_UPL1EPC                  BIT19
#define  B_PCH_DMI17_PCR_ACGR3S2_UPL1EPC_OFFSET           19
#define  B_PCH_DMI17_PCR_ACGR3S2_G4EBM                    BIT9
#define  B_PCH_DMI17_PCR_ACGR3S2_G4EBM_OFFSET             9
#define  B_PCH_DMI17_PCR_ACGR3S2_G3EBM                    BIT8
#define  B_PCH_DMI17_PCR_ACGR3S2_G3EBM_OFFSET             8
#define  B_PCH_DMI17_PCR_ACGR3S2_G2EBM                    BIT7
#define  B_PCH_DMI17_PCR_ACGR3S2_G2EBM_OFFSET             7
#define  B_PCH_DMI17_PCR_ACGR3S2_G1EBM                    BIT6
#define  B_PCH_DMI17_PCR_ACGR3S2_G1EBM_OFFSET             6
#define  B_PCH_DMI17_PCR_ACGR3S2_SRT                      BIT5
#define  B_PCH_DMI17_PCR_ACGR3S2_SRT_OFFSET               5
#define  B_PCH_DMI17_PCR_ACGR3S2_LSTPTLS_MASK             0xF
#define  B_PCH_DMI17_PCR_ACGR3S2_LSTPTLS_OFFSET           0
#define  V_PCH_DMI17_PCR_ACGR3S2_LSTPTLS                  0x1

#define  R_PCH_DMI17_PCR_VNNREMCTL                        0x2C70
#define  B_PCH_DMI17_PCR_VNNREMCTL_FDVNNRE                BIT8
#define  B_PCH_DMI17_PCR_VNNREMCTL_FDVNNRE_OFFSET         8
#define  B_PCH_DMI17_PCR_VNNREMCTL_HPVNNRE                BIT7
#define  B_PCH_DMI17_PCR_VNNREMCTL_HPVNNRE_OFFSET         7
#define  B_PCH_DMI17_PCR_VNNREMCTL_DNPVNNRE               BIT6
#define  B_PCH_DMI17_PCR_VNNREMCTL_DNPVNNRE_OFFSET        6
#define  B_PCH_DMI17_PCR_VNNREMCTL_RTD3VNNRE              BIT5
#define  B_PCH_DMI17_PCR_VNNREMCTL_RTD3VNNRE_OFFSET       5
#define  B_PCH_DMI17_PCR_VNNREMCTL_LDVNNRE                BIT4
#define  B_PCH_DMI17_PCR_VNNREMCTL_LDVNNRE_OFFSET         4
#define  B_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_MASK        (BIT3 | BIT2)
#define  B_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_OFFSET      2
#define  V_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_16CLKS      0x1
#define  V_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_8CLKS       0x0
#define  B_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_MASK        (BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_OFFSET      0
#define  V_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_4CLKS       0x0

#define  R_PCH_DMI17_PCR_RXQC                             0x2C7C
#define  B_PCH_DMI17_PCR_RXQC_VMRQNPCOC_MASK              (BIT15 | BIT14)
#define  B_PCH_DMI17_PCR_RXQC_VMRQNPCOC_OFFSET            14
#define  V_PCH_DMI17_PCR_RXQC_VMRQNPCOC                   0x1
#define  B_PCH_DMI17_PCR_RXQC_VMRQPCOC_MASK               (BIT13 | BIT12)
#define  B_PCH_DMI17_PCR_RXQC_VMRQPCOC_OFFSET             12
#define  V_PCH_DMI17_PCR_RXQC_VMRQPCOC                    0x1
#define  B_PCH_DMI17_PCR_RXQC_V1RQNPCOC_MASK              (BIT9 | BIT8)
#define  B_PCH_DMI17_PCR_RXQC_V1RQNPCOC_OFFSET            8
#define  V_PCH_DMI17_PCR_RXQC_V1RQNPCOC                   0x1
#define  B_PCH_DMI17_PCR_RXQC_V1RQPCOC_MASK               (BIT7 | BIT6)
#define  B_PCH_DMI17_PCR_RXQC_V1RQPCOC_OFFSET             6
#define  V_PCH_DMI17_PCR_RXQC_V1RQPCOC                    0x1
#define  B_PCH_DMI17_PCR_RXQC_V0RQNPCOC_MASK              (BIT3 | BIT2)
#define  B_PCH_DMI17_PCR_RXQC_V0RQNPCOC_OFFSET            2
#define  V_PCH_DMI17_PCR_RXQC_V0RQNPCOC                   0x1
#define  B_PCH_DMI17_PCR_RXQC_V0RQPCOC_MASK               (BIT1 | BIT0)
#define  B_PCH_DMI17_PCR_RXQC_V0RQPCOC_OFFSET             0
#define  V_PCH_DMI17_PCR_RXQC_V0RQPCOC                    0x1

#define  R_PCH_DMI17_PCR_AECR1G3                          0x2C80
#define  B_PCH_DMI17_PCR_AECR1G3_DTCGCM                   BIT14
#define  B_PCH_DMI17_PCR_AECR1G3_DTCGCM_OFFSET            14
#define  B_PCH_DMI17_PCR_AECR1G3_TPSE                     BIT10
#define  B_PCH_DMI17_PCR_AECR1G3_TPSE_OFFSET              10

#define  R_PCH_DMI17_PCR_AECR2G3                          0x2C84

#define  R_PCH_DMI17_PCR_LPCR                             0x2C8C
#define  B_PCH_DMI17_PCR_LPCR_DIDOVRLOCK                  BIT24
#define  B_PCH_DMI17_PCR_LPCR_LTRCFGLOCK                  BIT16
#define  B_PCH_DMI17_PCR_LPCR_SERL                        BIT8
#define  B_PCH_DMI17_PCR_LPCR_SRL                         BIT0

#define  R_PCH_DMI17_PCR_RXMC1                            0x2C90
#define  B_PCH_DMI17_PCR_RXMC1_MSRVS_MASK                 0xFC000000
#define  B_PCH_DMI17_PCR_RXMC1_MSRVS_OFFSET               26
#define  V_PCH_DMI17_PCR_RXMC1_MSRVS                      0x1F
#define  B_PCH_DMI17_PCR_RXMC1_MSRTS_MASK                 0x3F00000
#define  B_PCH_DMI17_PCR_RXMC1_MSRTS_OFFSET               20
#define  V_PCH_DMI17_PCR_RXMC1_MSRTS                      0x3F
#define  B_PCH_DMI17_PCR_RXMC1_TMSLOP_MASK                (BIT14 | BIT13)
#define  B_PCH_DMI17_PCR_RXMC1_TMSLOP_OFFSET              13
#define  V_PCH_DMI17_PCR_RXMC1_TMSLOP                     0x1
#define  B_PCH_DMI17_PCR_RXMC1_VMSLOP_MASK                (BIT12 | BIT11)
#define  B_PCH_DMI17_PCR_RXMC1_VMSLOP_OFFSET              11
#define  V_PCH_DMI17_PCR_RXMC1_VMSLOP                     0x1
#define  B_PCH_DMI17_PCR_RXMC1_MMNOLS_MASK                0x1F0
#define  B_PCH_DMI17_PCR_RXMC1_MMNOLS_OFFSET              4
#define  V_PCH_DMI17_PCR_RXMC1_MMNOLS                     0x1

#define  R_PCH_DMI17_PCR_RXMC2                            0x2C94
#define  B_PCH_DMI17_PCR_RXMC2_MNOTSS_MASK                0x1F80000
#define  B_PCH_DMI17_PCR_RXMC2_MNOTSS_OFFSET              19
#define  V_PCH_DMI17_PCR_RXMC2_MNOTSS                     0x20
#define  B_PCH_DMI17_PCR_RXMC2_MMTOS_MASK                 0x7E000
#define  B_PCH_DMI17_PCR_RXMC2_MMTOS_OFFSET               13
#define  V_PCH_DMI17_PCR_RXMC2_MMTOS                      0x28
#define  B_PCH_DMI17_PCR_RXMC2_MNOVSS_MASK                0x1FC0
#define  B_PCH_DMI17_PCR_RXMC2_MNOVSS_OFFSET              6
#define  V_PCH_DMI17_PCR_RXMC2_MNOVSS                     0x28

#define  R_PCH_DMI17_PCR_EINJCTL                          0x2CA8
#define  B_PCH_DMI17_PCR_EINJCTL_EINJDIS                  BIT0
#define  B_PCH_DMI17_PCR_EINJCTL_EINJDIS_OFFSET           0

#define  R_PCH_DMI17_PCR_PL16MECH                         0x2EDC
#define  B_PCH_DMI17_PCR_PL16MECH_NCO_MASK                0xFFF00000
#define  B_PCH_DMI17_PCR_PL16MECH_NCO_OFFSET              20
#define  V_PCH_DMI17_PCR_PL16MECH_NCO                     0x0
#define  B_PCH_DMI17_PCR_PL16MECH_CV_MASK                 0xF0000
#define  B_PCH_DMI17_PCR_PL16MECH_CV_OFFSET               16
#define  V_PCH_DMI17_PCR_PL16MECH_CV                      0x1
#define  B_PCH_DMI17_PCR_PL16MECH_PCIECID_MASK            0xFFFF
#define  B_PCH_DMI17_PCR_PL16MECH_PCIECID_OFFSET          0
#define  V_PCH_DMI17_PCR_PL16MECH_PCIECID                 0x27

#define  R_PCH_DMI17_PCR_SRL                              0x3E24
#define  B_PCH_DMI17_PCR_SRL_SRL                          BIT0
#endif
