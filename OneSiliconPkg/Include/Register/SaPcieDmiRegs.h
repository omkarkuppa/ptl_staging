/** @file
  Register names for SA DMI and PEG

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
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _SA_PCIE_DMI_REGS_H_
#define _SA_PCIE_DMI_REGS_H_

///
/// Device 1 Register Equates
///
#define R_SA_PEG_DID_OFFSET            0x02  ///< Device ID
#define R_SA_PEG_BCTRL_OFFSET          0x3E  ///< Bridge Control
#define R_SA_PEG_SS_OFFSET             0x8C  ///< Subsystem ID
#define R_SA_PEG_LCTL_OFFSET           0xB0  ///< Link Control
#define R_SA_PEG_LSTS_OFFSET           0xB2  ///< Link Status
#define R_SA_PEG_SLOTSTS_OFFSET        0xBA  ///< Slot Status
#define R_SA_PEG_DCAP2_OFFSET          0xC4  ///< Device Capability 2
#define R_SA_PEG_DCTL2_OFFSET          0xC8  ///< Device Control 2
#define R_SA_PEG_VC0RSTS_OFFSET        0x11A ///< PEG VC0 Resource Status
#define R_SA_PEG_L0SLAT_OFFSET         0x22C ///< PEG L0s Control
#define R_SA_PEG_CFG4_OFFSET           0x258 ///< PEG Config 4
#define R_SA_PEG_EQPH3_OFFSET          0x384 ///< PEG Phase 3
#define R_SA_PEG_AFEBND0CFG1_OFFSET    0x904 ///< PEG AFE Bundle Config 1
#define R_SA_PEG_AFEBND0CFG2_OFFSET    0x908 ///< PEG AFE Bundle Config 2
#define R_SA_PEG_AFEBND0CFG3_OFFSET    0x90C ///< PEG AFE Bundle Config 3
#define R_SA_PEG_AFEBND0CFG4_OFFSET    0x910 ///< PEG AFE Bundle Config 4
#define R_SA_PEG_G3CTL0_OFFSET         0x918 ///< PEG Gen3 Control
#define R_SA_PEG_BND0SPARE_OFFSET      0x91C ///< PEG Bundle 0 Spare Register
#define R_SA_PEG_AFELN0CFG0_OFFSET     0xA00 ///< PEG AFE Lane Config 0
#define R_SA_PEG_AFELN0CFG1_OFFSET     0xA04 ///< PEG AFE Lane Config 1
#define R_SA_PEG_AFEOVR_OFFSET         0xC20 ///< PEG AFE Override
#define R_SA_PEG_CMNSPARE_OFFSET       0xC38 ///< PEG Common Spare
#define R_SA_PEG_LCTL3_OFFSET          0xD98 ///< PEG Link Control 3
#define R_SA_PEG_EQCTL0_1_OFFSET       0xDA0 ///< PEG Lane Equalization
#define R_SA_PEG_EQPRESET1_2_OFFSET    0xDC0 ///< PEG Coefficients for P1 and P2
#define R_SA_PEG_EQPRESET2_3_4_OFFSET  0xDC4 ///< PEG Coefficients for P2, P3, and P4
#define R_SA_PEG_EQPRESET4_5_OFFSET    0xDC8 ///< PEG Coefficients for P4 and P5
#define R_SA_PEG_EQPRESET6_7_OFFSET    0xDCC ///< PEG Coefficients for P6 and P7
#define R_SA_PEG_EQPRESET7_8_9_OFFSET  0xDD0 ///< PEG Coefficients for P7, P8, and P9
#define R_SA_PEG_EQPRESET9_10_OFFSET   0xDD4 ///< PEG Coefficients for P9 and P10
#define R_SA_PEG_EQCFG_OFFSET          0xDD8 ///< PEG Equalization Config
#define R_SA_PEG_EQPRESET11_OFFSET     0xDDC ///< PEG Coefficients for P11

#define R_SA_PEG_BND10_CRI2_FUSE_DWORD14_OFFSET   0x2338 ///< PEG Bundle 10(DMI) CRI2 FUSE register DWORD 14

#define R_SA_PEG_BND0_CRI0_CR_DWORD22_OFFSET      0x0058 ///< PEG Bundle 0 CRI0 CR register DWORD 22
#define R_SA_PEG_BND10_CRI0_CR_DWORD22_OFFSET     0x2258 ///< PEG Bundle 10(DMI) CRI0 CR register DWORD 22

#define R_SA_PEG_BND0_CRI2_FUSE_DWORD15_OFFSET    0x013C ///< PEG Bundle 0 CRI2 FUSE register DWORD 15
#define R_SA_PEG_BND10_CRI2_FUSE_DWORD15_OFFSET   0x233C ///< PEG Bundle 10(DMI) CRI2 FUSE register DWORD 15

#define R_SA_PEG_BND0_CRI0_CR_DWORD24_OFFSET      0x0060 ///< PEG Bundle 0 CRI0 CR register DWORD 24
#define R_SA_PEG_BND10_CRI0_CR_DWORD24_OFFSET     0x2260 ///< PEG Bundle 10(DMI) CRI0 CR register DWORD 24

#define R_SA_PEG_BND0_CRI0_CR_DWORD26_OFFSET      0x0068 ///< PEG Bundle 0 CRI0 CR register DWORD 26
#define R_SA_PEG_BND10_CRI0_CR_DWORD26_OFFSET     0x2268 ///< PEG Bundle 10(DMI) CRI0 CR register DWORD 26

#define R_SA_PEG_BND0_CRI0_CR_DWORD28_OFFSET      0x0070 ///< PEG Bundle 0 CRI0 CR register DWORD 28
#define R_SA_PEG_BND10_CRI0_CR_DWORD28_OFFSET     0x2270 ///< PEG Bundle 10(DMI) CRI0 CR register DWORD 28

#define R_SA_PEG_BND10_CRI2_FUSE_DWORD16_OFFSET   0x2340 ///< PEG Bundle 10(DMI) CRI2 FUSE register DWORD 16

#endif
