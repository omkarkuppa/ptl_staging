/** @file
  Register names for DMI SIP specific registers

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
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _DMI_SIP_REGS_H_
#define _DMI_SIP_REGS_H_

#include <PcieRegs.h>

/**
  Registers defined in here are in use for DMI controller. Note, that
  for DMI we are using sideband access to access PCI config bank, and
  using this type of access on DMI requires adding an offset of 0x2000 to each
  of PCIe analogic definitions.
**/

// DMI register space offset for registers under 0xFFF range.
// For that registers, address is constructed by adding this
// offset to analogic PCIe register space address.

#define R_DMI_PCR_ROOTCAP                             0x5E
#define B_DMI_PCR_ROOTCAP_CRSSV                       BIT0

#define R_DMI_PCR_IPSCE                               0xE0
#define B_DMI_PCR_IPSCE_RXROM                         (BIT19 | BIT18)
#define N_DMI_PCR_IPSCE_RXROM                         18
#define V_DMI_PCR_IPSCE_RXROM                         0x2
#define B_DMI_PCR_IPSCE_TXROM                         (BIT17 | BIT16)

#define V_DMI_PCR_VXCTL_TVM_MASK                      0xFE
#define B_DMI_PCR_V0CTL_EN                            BIT31                       ///< applies also for V1 and VM
#define B_DMI_PCR_V0CTL_ID                            (BIT26 | BIT25 | BIT24)     ///< applies also for V1 and VM
#define N_DMI_PCR_V0CTL_ID                            24                          ///< applies also for V1 and VM

#define B_DMI_PCR_PL16L01EC_UP16L1TP                  (BIT15 | BIT14 | BIT13 | BIT12)    ///< applies also for lines 3,5,7 from R_DMI_PCR_PL16L01EC-R_DMI_PCR_PL16L67EC
#define N_DMI_PCR_PL16L01EC_UP16L1TP                  12                                 ///< applies also for lines 3,5,7 from R_DMI_PCR_PL16L01EC-R_DMI_PCR_PL16L67EC
#define B_DMI_PCR_PL16L01EC_UP16L0TP                  (BIT7 | BIT6 | BIT5 | BIT4)        ///< applies also for lines 2,4,6 from R_DMI_PCR_PL16L01EC-R_DMI_PCR_PL16L67EC
#define N_DMI_PCR_PL16L01EC_UP16L0TP                  4                                  ///< applies also for lines 2,4,6 from R_DMI_PCR_PL16L01EC-R_DMI_PCR_PL16L67EC

#define B_DMI_PCR_L01EC_UPL1TP                        (BIT27 | BIT26 | BIT25 | BIT24)    ///< applies also for lines 3,5,7 from R_DMI_PCR_L23EC-R_DMI_PCR_L67EC
#define N_DMI_PCR_L01EC_UPL1TP                        24                                 ///< applies also for lines 3,5,7 from R_DMI_PCR_L23EC-R_DMI_PCR_L67EC
#define B_DMI_PCR_L01EC_UPL0TP                        ( BIT11 | BIT10 | BIT9 | BIT8 )    ///< applies also for lines 2,4,6 from R_DMI_PCR_L23EC-R_DMI_PCR_L67EC
#define N_DMI_PCR_L01EC_UPL0TP                        8                                  ///< applies also for lines 2,4,6 from R_DMI_PCR_L23EC-R_DMI_PCR_L67EC

#define R_DMI_PCR_UPHWAWC                             0x49C                       ///< Upstream Port HW Autonomous Width Control
                                                                                  ///< This register's offset is 0x249C and defined as above
                                                                                  ///< to allow for an access from within PCIe space
#define B_DMI_PCR_UPHWAWC_TS3TW                       (BIT15 | BIT14 | BIT13)     ///< Thermal Sensor 3 Target Width
#define N_DMI_PCR_UPHWAWC_TS3TW                       13                          ///< Thermal Sensor 3 Target Width
#define B_DMI_PCR_UPHWAWC_TS2TW                       (BIT12 | BIT11 | BIT10)     ///< Thermal Sensor 2 Target Width
#define N_DMI_PCR_UPHWAWC_TS2TW                       10                          ///< Thermal Sensor 2 Target Width
#define B_DMI_PCR_UPHWAWC_TS1TW                       (BIT9 | BIT8 | BIT7)        ///< Thermal Sensor 1 Target Width
#define N_DMI_PCR_UPHWAWC_TS1TW                       7                           ///< Thermal Sensor 1 Target Width
#define B_DMI_PCR_UPHWAWC_TS0TW                       (BIT6 | BIT5 | BIT4)        ///< Thermal Sensor 0 Target Width
#define N_DMI_PCR_UPHWAWC_TS0TW                       4                           ///< Thermal Sensor 0 Target Width
#define B_DMI_PCR_UPHWAWC_TSAWEN                      BIT0                        ///< Thermal Sensor Autonomous Width Enable


#define B_DMI_PCR_CTRL2_DLDRSP                        BIT13


#define R_DMI_PCR_GCS                                 0x74C                       ///< Generic DMI Control and Status
                                                                                  ///< This register's offset is 0x274C and defined as above
                                                                                  ///< to allow for an access from within PCIe space

#define R_DMI_PCR_TCOBASE                             0x778

#define R_DMI_PCR_RSPVCC                              0x860
#define B_DMI_PCR_RSPVCC_PSTC2VCTSBE                  BIT3
#define B_DMI_PCR_RSPVCC_RS3VCCP                      BIT2

#define R_DMI_PCR_RS3V0CTL                            0x874
#define B_DMI_PCR_RS3V0CTL_TVM                        (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1)

#define R_DMI_PCR_RS3V1CTL                            0x878
#define B_DMI_PCR_RS3V1CTL_EN                         BIT31
#define B_DMI_PCR_RS3V1CTL_TVM                        (BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1)
#define N_DMI_PCR_RS3V1CTL_TVM                        1
#define V_DMI_PCR_RS3V1CTL_TVM                        0x7F

#define R_DMI_CFG_PCR_EEPPES                          0x900                       ///< DMI End-to-End Parity Protection Error Status
#define B_DMI_CFG_PCR_EEPPES_PSSSGE                   BIT31                       ///< End-to-End Parity Error Stop and Scream SERR generation Enable
                                                                                  ///< This register's offset is 0x2900 and defined as above
                                                                                  ///< to allow for an access from within PCIe space

#define R_DMI_PCR_DECCTL1                             0x908
#define B_DMI_PCR_DECCTL1_EMSGRXFP                    BIT0

#define R_DMI_PCR_MEMBARCTL                           0xF00
#define B_DMI_PCR_MEMBARCTL_MEMBAREN                  BIT0

#define R_DMI_PCR_IPCAPOVR1                           0x84
#define B_DMI_PCR_IPCAPOVR1_PCMRTOVRV                 (BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_DMI_PCR_IPCAPOVR1_PCMRTOVRV                 8
#define V_DMI_PCR_IPCAPOVR1_PCMRTOVRV                 0x100
#define B_DMI_PCR_IPCAPOVR1_PCMRTS                    BIT7

#define V_DMI_L1S_TPOS                                0x1                         ///< L1SCTL and L1SCAP TPOS field value
#endif
