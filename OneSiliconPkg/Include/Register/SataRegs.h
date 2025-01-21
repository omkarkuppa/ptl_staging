/** @file
  Register names for SATA controllers

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
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _SATA_REGS_H_
#define _SATA_REGS_H_

//
// SATA Controller Common Registers
//
#define R_SATA_CFG_ABAR                     0x24                                        ///< AHCI Base Address
#define B_SATA_CFG_BAR_BA                   0xFFFFF800
#define R_SATA_CFG_PID                      0x70                                        ///< PCI Power Management Capability ID
#define R_SATA_CFG_PMCS                     0x74                                        ///< PCI Power Management Control And Status
#define R_SATA_CFG_MAP                      0x90                                        ///< Port Mapping Register
#define B_SATA_CFG_MAP_PCD                  (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)   ///< Port Clock Disable
#define N_SATA_CFG_MAP_SPD0                 16
#define R_SATA_CFG_PCS                      0x94                                        ///< Port Control And Status
#define B_SATA_CFG_PCS_P0P                  BIT16                                       ///< Port 0 Present
#define B_SATA_CFG_PCS_PXE_MASK             0xFF
#define R_SATA_CFG_SATAGC                   0x9C                                        ///< SATA General Configuration
#define V_SATA_CFG_SATAGC_SMS_RAID          0x1
#define B_SATA_CFG_SATAGC_DPPEE             BIT15                                       ///< Data Phase Parity Error Enable
#define B_SATA_CFG_SATAGC_CPEE              BIT11                                       ///< Command Parity Error Enable
#define B_SATA_CFG_SATAGC_AIE               BIT7                                        ///< Alternate ID Enable
#define B_SATA_CFG_SATAGC_DEVIDSEL          BIT6                                        ///< AIE0 DevID Selection
#define B_SATA_CFG_SATAGC_MSS               (BIT4 | BIT3)                               ///< MXTBA Size Select
#define V_SATA_CFG_SATAGC_MSS_8K            0x2
#define N_SATA_CFG_SATAGC_MSS               3
#define B_SATA_CFG_SATAGC_ASSEL             (BIT2 | BIT1 | BIT0)                        ///< ABAR Size Select
#define V_SATA_CFG_SATAGC_ASSEL_2K          0x0
#define V_SATA_CFG_SATAGC_ASSEL_16K         0x1
#define V_SATA_CFG_SATAGC_ASSEL_32K         0x2
#define V_SATA_CFG_SATAGC_ASSEL_64K         0x3
#define V_SATA_CFG_SATAGC_ASSEL_128K        0x4
#define V_SATA_CFG_SATAGC_ASSEL_512K        0x6

//
//  SATA Initialization Register
//
#define R_SATA_CFG_SIRI                     0xA0              ///< SATA Initialization Register Index
#define R_SATA_CFG_SIRD                     0xA4              ///< SATA Initialization Register Data
#define R_SATA_CFG_SIR_0C                   0x0C
#define R_SATA_CFG_SIR_80                   0x80
#define R_SATA_CFG_SIR_84                   0x84
#define R_SATA_CFG_SIR_8C                   0x8C
#define R_SATA_CFG_SIR_90                   0x90
#define R_SATA_CFG_SIR_98                   0x98
#define R_SATA_CFG_SIR_9C                   0x9C
#define R_SATA_CFG_SIR_A0                   0xA0
#define B_SATA_CFG_SIR_A0_CWICBRLMAX        0x1F
#define N_SATA_CFG_SIR_A0_CWICBRLMAX        24
#define B_SATA_CFG_SIR_A0_CWCIBRUMIN        0x3F
#define N_SATA_CFG_SIR_A0_CWCIBRUMIN        18
#define R_SATA_CFG_SIR_A4                   0xA4
#define R_SATA_CFG_SIR_A8                   0xA8
#define R_SATA_CFG_SIR_C4                   0xC4
#define B_SATA_CFG_SIR_C4_TCLKSNACK_CP      (BIT27 | BIT26)
#define R_SATA_CFG_SIR_C8                   0xC8
#define R_SATA_CFG_SIR_CC                   0xCC
#define N_SATA_CFG_SIR_CC_LAT_VAL           0
#define N_SATA_CFG_SIR_CC_LAT_SCALE         10
#define B_SATA_CFG_SIR_CC_LAT_REQ           BIT15
#define N_SATA_CFG_SIR_CE_LTR_VAL           0
#define N_SATA_CFG_SIR_CE_LTR_SCALE         10
#define B_SATA_CFG_SIR_CE_LAT_REQ           BIT15
#define R_SATA_CFG_SIR_D0                   0xD0
#define N_SATA_CFG_SIR_D0_LAT_VAL           0
#define N_SATA_CFG_SIR_D0_LAT_SCALE         10
#define B_SATA_CFG_SIR_D0_LAT_REQ           BIT15
#define R_SATA_CFG_SIR_D4                   0xD4
#define B_SATA_CFG_SIR_D4_CIGMAX            0x3F
#define N_SATA_CFG_SIR_D4_CIGMAX            24
#define B_SATA_CFG_SIR_D4_CIGMIN            0x3F
#define N_SATA_CFG_SIR_D4_CIGMIN            16
#define B_SATA_CFG_SIR_D4_CWGMAX            0x1F
#define N_SATA_CFG_SIR_D4_CWGMAX            8
#define B_SATA_CFG_SIR_D4_CWGMIN            0x1F
#define N_SATA_CFG_SIR_D4_CWGMIN            0
#define R_SATA_CFG_MXID                     0xD0              ///< MSI-X Identifiers
#define N_SATA_CFG_MXID_NEXT                8
#define R_SATA_CFG_MXT                      0xD4              ///< MSI-X Table Offset And Table BIR
#define R_SATA_CFG_MXP                      0xD8              ///< MSI-X PBA Offset And PBA BIR

//
// Vendor-Specific Capabilities Registers
//
#define R_SATA_MEM_VS_CAP                   0xA4              ///< Vendor-Specific Capabilities Register
#define B_SATA_MEM_VS_CAP_NRMBE             BIT0              ///< NVM Remap Memory BAR Enable
#define N_SATA_MEM_VS_CAP_MSL               1
#define V_SATA_MEM_VS_CAP_MSL               0x1EF             ///< Memory Space Limit Field Value
#define N_SATA_MEM_VS_CAP_NRMO              16
#define V_SATA_MEM_VS_CAP_NRMO              0x10              ///< NVM Remapped Memory Offset Field Value

//
// AHCI BAR Area related Registers
//
#define R_SATA_MEM_GHC_CAP                       0x0                               ///< HBA Capabilities
#define B_SATA_MEM_GHC_CAP_SMPS                  BIT28                             ///< Supports Mechanical Presence Switch
#define B_SATA_MEM_GHC_CAP_SSS                   BIT27                             ///< Supports Staggered Spin-up
#define B_SATA_MEM_GHC_CAP_SALP                  BIT26                             ///< Supports Activity LED
#define B_SATA_MEM_GHC_CAP_SAL                   BIT25                             ///< Supports Activity LED
#define B_SATA_MEM_GHC_CAP_SCLO                  BIT24                             ///< Supports Command List Override
#define B_SATA_MEM_GHC_CAP_ISS                   (BIT23 | BIT22 | BIT21 | BIT20)   ///< Interface Speed Support
#define N_SATA_MEM_GHC_CAP_ISS                   20                                ///< Interface Speed Support
#define V_SATA_MEM_CAP_ISS_1_5_G                 0x01
#define V_SATA_MEM_CAP_ISS_3_0_G                 0x02
#define V_SATA_MEM_CAP_ISS_6_0_G                 0x03
#define B_SATA_MEM_GHC_CAP_SPM                   BIT17                             ///< Supports Port Multiplier
#define B_SATA_MEM_GHC_CAP_PMD                   BIT15                             ///< PIO Multiple DRQ Block
#define B_SATA_MEM_GHC_CAP_SSC                   BIT14                             ///< Slumber State Capable
#define B_SATA_MEM_GHC_CAP_PSC                   BIT13                             ///< Partial State Capable
#define B_SATA_MEM_GHC_CAP_EMS                   BIT6                              ///< Enclosure Management Supported
#define B_SATA_MEM_GHC_CAP_SXS                   BIT5                              ///< Supports External SATA
#define R_SATA_MEM_GHC_PI                        0x0C                              ///< Ports Implemented
#define B_SATA_MEM_PI_PORT_MASK                  0xFF
#define R_SATA_MEM_GHC_CAP2                      0x24                              ///< HBA Capabilities Extended
#define B_SATA_MEM_GHC_CAP2_DESO                 BIT5                              ///< DEVSLP Entrance from Slumber Only
#define B_SATA_MEM_GHC_CAP2_SADM                 BIT4                              ///< Supports Aggressive DEVSLP Management
#define B_SATA_MEM_GHC_CAP2_SDS                  BIT3                              ///< Supports DEVSLP
#define B_SATA_MEM_GHC_CAP2_APST                 BIT2                              ///< Automatic Partial to Slumber Transitions
#define R_SATA_MEM_SFM                           0xC8                              ///< SW Feature Mask
#define B_SATA_MEM_SFM_CPU_STORAGE               BIT14
#define B_SATA_MEM_SFM_OMA                       BIT13
#define B_SATA_MEM_SFM_LEGACY                    BIT12
#define N_SATA_MEM_SFM_OROM_UI_NORMAL_DELAY      10
#define B_SATA_MEM_SFM_SMART_RESPONSE_TECHNOLOGY BIT9                              ///< Smart Response Technology.
#define B_SATA_MEM_SFM_IRRT_ONLY_ON_ESATA        BIT8                              ///< RRT
#define B_SATA_MEM_SFM_LED_LOCATE                BIT7                              ///< LED Locate
#define B_SATA_MEM_SFM_HDDUNLOCK                 BIT6                              ///< HDDUNLOCK
#define B_SATA_MEM_SFM_OROM_UI_AND_BANNER        BIT5                              ///< OROM UI and BANNER
#define B_SATA_MEM_SFM_IRRT                      BIT4                              ///< RRT
#define B_SATA_MEM_SFM_R5                        BIT3                              ///< R5
#define B_SATA_MEM_SFM_R10                       BIT2                              ///< R1
#define B_SATA_MEM_SFM_R1                        BIT1                              ///< R1
#define B_SATA_MEM_SFM_R0                        BIT0                              ///< R0
#define R_SATA_MEM_PXCMD0                        0x118                             ///< Port 0 To 7 Command
#define R_SATA_MEM_PXDEVSLP0                     0x144                             ///< Port 0 To 7 Device Sleep
#define B_SATA_MEM_PXDEVSLP0_DSP                 BIT1                              ///< Device Sleep Present
#define B_SATA_MEM_PXDEVSLP0_DITO                0x01FF8000                        ///< DEVSLP Idle Timeout
#define V_SATA_MEM_PxDEVSLP_DITO_625             0x01388000
#define B_SATA_MEM_PXDEVSLP0_DM                  (BIT28 | BIT27 | BIT26 | BIT25)   ///< DITO Multiplier
#define V_SATA_MEM_PxDEVSLP_DM_16                0x1E000000
#define B_SATA_MEM_PxCMD_MASK                    (BIT27 | BIT26 | BIT22 | BIT21 | BIT19 | BIT18)
#define B_SATA_MEM_PXCMD0_ALPE                   BIT26                             ///< Aggressive Link Power Management Enable
#define B_SATA_MEM_PXCMD0_ESP                    BIT21                             ///< External SATA Port
#define B_SATA_MEM_PXCMD0_MPSP                   BIT19                             ///< Mechanical Presence Switch Attached to Port
#define B_SATA_MEM_PXCMD0_HPCP                   BIT18                             ///< Hot Plug Capable Port
#define B_SATA_MEM_PXCMD0_SUD                    BIT1                              ///< Spin-Up Device
#define R_SATA_MEM_PXSCTL0                       0x12C                             ///< Port 0 To 7 Serial ATA Control
#define B_SATA_MEM_PXSCTL0_SPD                   (BIT7 | BIT6 | BIT5 | BIT4)       ///< Speed Allowed
#define V_SATA_MEM_PXSCTL_SPD_2                  0x00000020

//
// SATA adapter
//
#define R_SATA_ADAPTER_PWR_GATE_CTRL                   0x104
#define B_SATA_ADAPTER_PWR_GATE_CTRL_HAE               BIT26
#define B_SATA_ADAPTER_PWR_GATE_CTRL_ACC_CLK_GATE_DIS  BIT25

#endif /* _SATA_REGS_H_ */
