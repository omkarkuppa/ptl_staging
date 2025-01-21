/** @file
  Register definition for PSF component

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
#ifndef _PCH_REGS_PSF_H_
#define _PCH_REGS_PSF_H_

//
// PSFx segment registers
//
#define R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG                     0x4000                  ///< PSF Segment Global Configuration Register
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG               BIT12
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENCMDPARDETINGRESS  BIT10
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENEEI               BIT7
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG               BIT4
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG               BIT3
#define B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_IDLENAK             BIT2
#define B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P        BIT1
#define B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN            BIT0


#define N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P        1
#define N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN            0

#define B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO  BIT2
#define B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO BIT1

#define S_PSF_PCR_PSF_X_DEV_GNTCNT_RELOAD_DGCRN_GNTCNTRELOAD         4
#define S_PSF_PCR_PSF_X_TARGET_GNTCNT_RELOAD_PGN_TGTM_GNTCNTRELOAD   4
#define B_PSF_PCR_PSF_X_DEV_GNTCNT_RELOAD_DGCRN_GNTCNTRELOAD         0x1F
#define B_PSF_PCR_PSF_X_TARGET_GNTCNT_RELOAD_PGN_TGTM_GNTCNTRELOAD   0x1F

#define N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC          1
#define B_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_MULTCEN        BIT0

//
// PSFx PCRs definitions
//
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR0                  0                       ///< PCI BAR0
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR1                  0x04                    ///< PCI BAR1
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR2                  0x08                    ///< PCI BAR2
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR3                  0x0C                    ///< PCI BAR3
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR4                  0x10                    ///< PCI BAR4
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR5                  0x20                    ///< PCI BAR5
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN                 0x1C                    ///< PCI configuration space enable bits
#define N_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BARXDIS         16
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR0DIS         BIT16                   ///< Disable BAR0
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR1DIS         BIT17                   ///< Disable BAR1
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR2DIS         BIT18                   ///< Disable BAR2
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR3DIS         BIT19                   ///< Disable BAR3
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR4DIS         BIT20                   ///< Disable BAR4
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BAR5DIS         BIT21                   ///< Disable BAR5
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS          BIT8                    ///< Function disable
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_MEMEN           BIT1                    ///< Memory decoding enable
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_IOEN            BIT0                    ///< IO decoding enable
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PMCSR                 0x20                    ///< PCI power management configuration
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PMCSR_PWRST           (BIT1 | BIT0)           ///< Power status
#define R_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS               0x38                    ///< PCI configuration disable
#define B_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS_CFGDIS        BIT0                    ///< config disable

#define R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN                 0x3C                    ///< PCI configuration space enable bits
#define B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS          BIT8                    ///< Function disable
#define B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_MEMEN           BIT1                    ///< Memory decoding enable
#define B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_IOEN            BIT0                    ///< IO decoding enable

#define B_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_DEVICE     0x01F0                 ///< device number
#define N_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_DEVICE     4
#define B_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION   (BIT3 | BIT2 | BIT1)   ///< function number
#define N_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION   1

#define B_PSF_PCR_PSF_X_RC_OWNER_RSN_CHANNELID             0xFF
#define B_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTID                0x7F00
#define N_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTID                8
#define B_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTGROUPID           BIT15
#define N_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTGROUPID           15
#define B_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID                 0xFF0000
#define N_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID                 16
#define B_PSF_PCR_PSF_X_RC_OWNER_RSN_CHANMAP               BIT31

#define V_PCH_PSFX_PCR_DEV_GNTCNT_MAX                      4

//PSFX Multicast Message Configuration
#define R_PSF_PCR_PSF_X_RC_OWNER_RS0                       0x4008

#define B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN           BIT0                   ///< Deferred Write Buffer Enable
#define B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN          BIT1                   ///< OBFF Enable
#define B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN       BIT2                   ///< Non-xHCI Enable
#define B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD ( BIT6 | BIT5 | BIT4 ) ///< DWB Flush Threshold
#define N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN           0
#define N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN          1
#define N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN       2
#define N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD 4
#endif
