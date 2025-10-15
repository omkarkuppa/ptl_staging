/** @file
  Register definition for PTL SOC component

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
#ifndef _PSF_REGS_PTL_PCD_
#define _PSF_REGS_PTL_PCD_

// I2C:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F0_OFFSET3 0x00000180U      ///< Offset 0180h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F1_OFFSET4 0x00000200U      ///< Offset 0200h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F2_OFFSET5 0x00000280U      ///< Offset 0280h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F3_OFFSET6 0x00000300U      ///< Offset 0300h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F0_OFFSET7 0x00000380U      ///< Offset 0380h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F1_OFFSET8 0x00000400U      ///< Offset 0400h: PCI BAR

// I3C:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9 0x00000480U      ///< Offset 0480h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10 0x00000500U      ///< Offset 0500h: PCI BAR

// SPI:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F2_OFFSET11 0x00000580U      ///< Offset 0580h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F3_OFFSET12 0x00000600U      ///< Offset 0600h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D18_F6_OFFSET13 0x00000680U      ///< Offset 0680h: PCI BAR

// UART:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F0_OFFSET14 0x00000700U      ///< Offset 0700h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F1_OFFSET15 0x00000780U      ///< Offset 0780h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F2_OFFSET16 0x00000800U      ///< Offset 0800h: PCI BAR

// NPK ACPI:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4 0x00000200U      ///< Offset 0200h: PCI BAR

// NPK:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D31_F7_OFFSET5 0x00000280U      ///< Offset 0280h: PCI BAR

// HECI:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F0_OFFSET9 0x00000480U      ///< Offset 0480h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F1_OFFSET10 0x00000500U      ///< Offset 0500h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F4_OFFSET11 0x00000580U      ///< Offset 0580h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F5_OFFSET12 0x00000600U      ///< Offset 0600h: PCI BAR

// SSE HECI:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F0_OFFSET16 0x00000800U      ///< Offset 0800h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F1_OFFSET17 0x00000880U      ///< Offset 0880h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F2_OFFSET18 0x00000900U      ///< Offset 0900h: PCI BAR

// PSE HECI:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_OSSE_RS0_D19_F0_OFFSET21 0x00000a80U      ///< Offset 0A80h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_OSSE_RS0_D19_F1_OFFSET22 0x00000b00U      ///< Offset 0B00h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_OSSE_RS0_D19_F2_OFFSET23 0x00000b80U      ///< Offset 0B80h: PCI BAR

// IDER:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F2_OFFSET26 0x00000d00U      ///< Offset 0D00h: PCI BAR

// SOL:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F3_OFFSET27 0x00000d80U      ///< Offset 0D80h: PCI BAR

// HDA:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ACE_RS0_D31_F3_OFFSET28 0x00000e00U      ///< Offset 0E00h: PCI BAR

// THC:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC0_RS0_D16_F0_OFFSET29 0x00000e80U      ///< Offset 0E80h: PCI BAR
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC1_RS0_D16_F1_OFFSET30 0x00000f00U      ///< Offset 0F00h: PCI BAR

// XDCI:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_XDCI_RS0_D20_F1_OFFSET31 0x00000f80U      ///< Offset 0F80h: PCI BAR

// ISH:
#define R_PTL_PCD_P_H_PSF_0_AGNT_T0_SHDW_BAR0_ISH_RS0_D18_F0_OFFSET2 0x00000100U      ///< Offset 0100h: PCI BAR

// GBE:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_GBE_RS0_D31_F6_OFFSET32 0x00001000U      ///< Offset 1000h: PCI BAR

// SMBUS:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_SMB_RS0_D31_F4_OFFSET24 0x00000c00U      ///< Offset 0C00h: PCI BAR

// UFS:
#define R_PTL_PCD_P_PSF_0_AGNT_T0_SHDW_BAR0_UFS_RS0_D23_F0_OFFSET4 0x00000200U      ///< Offset 0200h: PCI BAR

// CNVI:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_WIFI_RS0_D20_F3_OFFSET34 0x00001100U      ///< Offset 1100h: PCI BAR

// CNVI BT:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_BT_RS0_D20_F7_OFFSET30 0x00000f00U      ///< Offset 0F00h: PCI BAR

// PMC:
#define R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_PMC_RS0_D31_F2_OFFSET21 0x00000a80U      ///< Offset 0A80h: PCI BAR

// Rootspace Configuration registers:
#define R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register

#define R_PTL_PCD_H_PSF_5_ROOTSPACE_CONFIG_RS0 0x00004018U      ///< Offset 4018h: Rootspace Configuration Register
#define R_PTL_PCD_H_PSF_5_ROOTSPACE_CONFIG_RS3 0x00004024U      ///< Offset 4024h: Rootspace Configuration Register

#define R_PTL_PCD_P_H_PSF_6_BUS_SHADOW_RS3    0x0000000cU      ///< Offset 000Ch: Bus Number Shadow Register

// PCIe Root Ports registers:
#define R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET64 0x00002000U      ///< Offset 2000h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET65 0x00002080U      ///< Offset 2080h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET66 0x00002100U      ///< Offset 2100h: PCI BAR
#define R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET67 0x00002180U      ///< Offset 2180h: PCI BAR
#define R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64 0x00002000U      ///< Offset 2000h: PCI BAR
#define R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65 0x00002080U      ///< Offset 2080h: PCI BAR
#define R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66 0x00002100U      ///< Offset 2100h: PCI BAR
#define R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67 0x00002180U      ///< Offset 2180h: PCI BAR

#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET68 0x00002200U      ///< Offset 2200h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET69 0x00002280U      ///< Offset 2280h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET70 0x00002300U      ///< Offset 2300h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET71 0x00002380U      ///< Offset 2380h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64 0x00002000U      ///< Offset 2000h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65 0x00002080U      ///< Offset 2080h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66 0x00002100U      ///< Offset 2100h: PCI BAR
#define R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67 0x00002180U      ///< Offset 2180h: PCI BAR

#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET71 0x0000440cU      ///< Offset 440ch: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET70 0x00004410U      ///< Offset 4410h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET69 0x00004414U      ///< Offset 4414h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET68 0x00004418U      ///< Offset 4418h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67 0x0000441cU      ///< Offset 441ch: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66 0x00004420U      ///< Offset 4420h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65 0x00004424U      ///< Offset 4424h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64 0x00004428U      ///< Offset 4428h: PCI Function Configuration

#define R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET67 0x0000494cU      ///< Offset 494Ch: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET66 0x00004950U      ///< Offset 4950h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET65 0x00004954U      ///< Offset 4954h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET64 0x00004958U      ///< Offset 4958h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67 0x00004294U      ///< Offset 4294h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66 0x00004298U      ///< Offset 4298h: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65 0x0000429cU      ///< Offset 429Ch: PCI Function Configuration
#define R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64 0x000042a0U      ///< Offset 42A0h: PCI Function Configuration

// EOI:
#define R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI 0x00004060U      ///< Offset 4060h: Destination ID
#define R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI 0x00004058U      ///< Offset 4058h: Destination ID
#define R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI 0x00004070U      ///< Offset 4070h: Destination ID
#define R_PTL_PCD_P_H_PSF_8_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI 0x0000406cU      ///< Offset 406Ch: Destination ID

#define R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST0_RS0_EOI 0x00004040U      ///< Offset 4040h: Multicast Control Register
#define R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST0_RS0_EOI 0x00004038U      ///< Offset 4038h: Multicast Control Register
#define R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST0_RS0_EOI 0x00004050U      ///< Offset 4050h: Multicast Control Register
#define R_PTL_PCD_P_H_PSF_8_PSF_MC_CONTROL_MCAST0_RS0_EOI 0x0000405cU      ///< Offset 405Ch: Multicast Control Register

// MCTP:
#define R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1 0x0000407cU      ///< Offset 407Ch: Destination ID
#define R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1 0x00004074U      ///< Offset 4074h: Destination ID
#define R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1 0x000040a0U      ///< Offset 40A0h: Destination ID

#define R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1 0x00004050U      ///< Offset 4050h: Multicast Control Register
#define R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST1_RS0_MCTP1 0x00004048U      ///< Offset 4048h: Multicast Control Register
#define R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST1_RS0_MCTP1 0x00004060U      ///< Offset 4060h: Multicast Control Register

// Grant Count registers:
#define R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR0 0x000042a4U      ///< Offset 42A4h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR0 0x0000495cU      ///< Offset 495Ch: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT0 0x000043c4U      ///< Offset 43C4h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT0 0x00004b30U      ///< Offset 4B30h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG0_TGT1                               0x43B8     // Offset 43B8h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG0_TGT2                               0x43BC     // Offset 43BCh: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT2                               0x43CC     // Offset 43CCh: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT2                               0x455C     // Offset 455Ch: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT8                               0x4584     // Offset 4584h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT3                               0x4560     // Offset 4560h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT9                               0x4588     // Offset 4588h: Grant Count Reload Register

#define R_PTL_PCD_H_PSF_5_TARGET_GNTCNT_RELOAD_PG0_TGT0                               0x4110     // Offset 4110h: Grant Count Reload Register
#define R_PTL_PCD_H_PSF_5_TARGET_GNTCNT_RELOAD_PG1_TGT0                               0x411C     // Offset 411Ch: Grant Count Reload Register
#define R_PTL_PCD_H_PSF_5_TARGET_GNTCNT_RELOAD_PG0_TGT1                               0x4114     // Offset 4114h: Grant Count Reload Register
#define R_PTL_PCD_H_PSF_5_TARGET_GNTCNT_RELOAD_PG1_TGT3                               0x4124     // Offset 4124h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT4                               0x4564     // Offset 4564h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT0                               0x4574     // Offset 4574h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT5                               0x4568     // Offset 4568h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT1                               0x4578     // Offset 4578h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN0  0x47A4
#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN1  0x47A8
#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN2  0x47AC
#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN3  0x47B0
#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN4  0x47B4
#define R_PTL_PCD_P_H_PSF_0_LINK_GNTCNT_RELOAD_PG1_GRP0_PORT0_CHAN5  0x47B8

// Relaxed ordering registers:
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT0 0x0000402cU      ///< Offset 402Ch: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT1 0x00004030U      ///< Offset 4030h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT2 0x00004034U      ///< Offset 4034h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT3 0x00004038U      ///< Offset 4038h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT4 0x0000403cU      ///< Offset 403Ch: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT0 0x0000402cU      ///< Offset 402Ch: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT1 0x00004030U      ///< Offset 4030h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT2 0x00004034U      ///< Offset 4034h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT0 0x0000402cU      ///< Offset 402Ch: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT1 0x00004030U      ///< Offset 4030h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT2 0x00004034U      ///< Offset 4034h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT3 0x00004038U      ///< Offset 4038h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT4 0x0000403cU      ///< Offset 403Ch: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT5 0x00004040U      ///< Offset 4040h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT6 0x00004044U      ///< Offset 4044h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT7 0x00004048U      ///< Offset 4048h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT8 0x0000404cU      ///< Offset 404Ch: PSF Port Configuration Register

#define R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_14_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_15_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_8_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register
#define R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register

#define R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG1_PORT0 0x0000402cU      ///< Offset 402Ch: PSF Port Configuration Register
#define R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG1_PORT1 0x00004030U      ///< Offset 4030h: PSF Port Configuration Register

#define R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG0_PORT0 0x00004028U      ///< Offset 4028h: PSF Port Configuration Register


#define R_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0 0x000040d0U      ///< Offset 40D0h: Deferred Write Buffer Configuration Register
#define B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN BIT0             ///< Deferred Write Buffer Enable
#define N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN 0
#define B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN BIT1             ///< OBFF Enable
#define N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN 1
#define B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN BIT2             ///< Non-xHCI Enable
#define N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN 2
#define B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD ( BIT6 | BIT5 | BIT4 ) ///< DWB Flush Threshold
#define N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD 4

#define R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR14 0x00004994U      ///< Offset 4994h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR16 0x0000499cU      ///< Offset 499Ch: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR18 0x000049a4U      ///< Offset 49A4h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR20 0x000049acU      ///< Offset 49ACh: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR3 0x000042b0U      ///< Offset 42B0h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR5 0x000042b8U      ///< Offset 42B8h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR7 0x000042c0U      ///< Offset 42C0h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR9 0x000042c8U      ///< Offset 42C8h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT0 0x00004b30U      ///< Offset 4B30h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT2 0x00004b38U      ///< Offset 4B38h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT4 0x00004b40U      ///< Offset 4B40h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT6 0x00004b48U      ///< Offset 4B48h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT8 0x000043d4U      ///< Offset 43D4h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT10 0x000043dcU      ///< Offset 43DCh: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT12 0x000043e4U      ///< Offset 43E4h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT14 0x000043ecU      ///< Offset 43ECh: Grant Count Reload Register

// PSF4.PSF0
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT1 0x00004558U      ///< Offset 4558h: Grant Count Reload Register
#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT7 0x00004580U      ///< Offset 4580h: Grant Count Reload Register

#define R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT22 0x00004598U      ///< Offset 4598h: Grant Count Reload Register

// Rootspace Configs
#define N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_ENADDRP2P  1
#define N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_VTDEN      0
#endif
