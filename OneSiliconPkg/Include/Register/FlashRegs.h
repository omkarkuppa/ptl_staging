/** @file
  Register names for Flash registers

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
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _FLASH_REGS_H_
#define _FLASH_REGS_H_

#define B_FLASH_FLCOMP_RIDS_FREQ          (BIT29 | BIT28 | BIT27)       ///< Read ID and Read Status Clock Frequency
#define N_FLASH_FLCOMP_RIDS_FREQ          27
#define B_FLASH_FLCOMP_WE_FREQ            (BIT26 | BIT25 | BIT24)       ///< Write and Erase Clock Frequency
#define N_FLASH_FLCOMP_WE_FREQ            24
#define B_FLASH_FLCOMP_FRCF_FREQ          (BIT23 | BIT22 | BIT21)       ///< Fast Read Clock Frequency
#define N_FLASH_FLCOMP_FRCF_FREQ          21
#define B_FLASH_FLCOMP_FR_SUP             BIT20                         ///< Fast Read Support.

//
// Flash Descriptor Base Address Region (FDBAR) from Flash Region 0
//
#define R_FLASH_FDBAR_FLVALSIG            0x00                          ///< Flash Valid Signature
#define V_FLASH_FDBAR_FLVALSIG            0x0FF0A55A
#define R_FLASH_FDBAR_FLASH_MAP0          0x04
#define B_FLASH_FDBAR_FCBA                0x000000FF                    ///< Flash Component Base Address
#define B_FLASH_FDBAR_NC                  0x00000300                    ///< Number Of Components
#define N_FLASH_FDBAR_NC                  8                             ///< Number Of Components
#define V_FLASH_FDBAR_NC_1                0x00000000
#define V_FLASH_FDBAR_NC_2                0x00000100
#define B_FLASH_FDBAR_FRBA                0x00FF0000                    ///< Flash Region Base Address
#define B_FLASH_FDBAR_NR                  0x07000000                    ///< Number Of Regions
#define R_FLASH_FDBAR_FLASH_MAP1          0x08
#define B_FLASH_FDBAR_FMBA                0x000000FF                    ///< Flash Primary Base Address
#define B_FLASH_FDBAR_NM                  0x00000700                    ///< Number Of Masters
#define B_FLASH_FDBAR_FPSBA               0x00FF0000                    ///< PCH Strap Base Address, [23:16] represents [11:4]
#define N_FLASH_FDBAR_FPSBA               16                            ///< PCH Strap base Address bit position
#define N_FLASH_FDBAR_FPSBA_REPR          4                             ///< PCH Strap base Address bit represents position
#define B_FLASH_FDBAR_PCHSL               0xFF000000                    ///< PCH Strap Length, [31:24] represents number of Dwords
#define N_FLASH_FDBAR_PCHSL               24                            ///< PCH Strap Length bit position
#define R_FLASH_FDBAR_FLASH_MAP2          0x0C
#define B_FLASH_FDBAR_FCPUSBA             0x000000FF                    ///< CPU Strap Base Address
#define B_FLASH_FDBAR_CPUSL               0x0000FF00                    ///< CPU Strap Length, [15:8] represents number of Dwords
#define N_FLASH_FDBAR_CPUSL               8                             ///< CPU Strap Length bit position
#define B_FLASH_FDBAR_NTDSOCSBA           0x00FF0000                    ///< CPU Strap Length, [23:16] represents number of Dwords
#define N_FLASH_FDBAR_NTDSOCSBA           16                            ///< CPU Strap Length bit position

//
// Flash Component Base Address (FCBA) from Flash Region 0
//
#define R_FLASH_FCBA_FLCOMP               0x00                          ///< Flash Components Register
#define B_FLASH_FLCOMP_COMP1_MASK         0xF0                          ///< Flash Component 1 Size MASK
#define N_FLASH_FLCOMP_COMP1              4                             ///< Flash Component 1 Size bit position
#define B_FLASH_FLCOMP_COMP0_MASK         0x0F                          ///< Flash Component 0 Size MASK
#define V_FLASH_FLCOMP_COMP_512KB         0x80000

//
// Flash Component Configuration 2
//
#define R_FLASH_FCBA_FLCOMP2              0x0C                          ///< Flash Component Configuration 2 Register
#define B_FLASH_FCBA_FLCOMP2_SSLLSE       BIT0                          ///< Desktop SoC Softstral Linked List Support Enable

//
// Descriptor Upper Map Section from Flash Region 0
//
#define R_FLASH_UMAP1                     0xEFC                         ///< Flash Upper Map 1
#define B_FLASH_UMAP1_VTBA                0x000000FF                    ///< VSCC Table Base Address
#define B_FLASH_UMAP1_VTL                 0x0000FF00                    ///< VSCC Table Length
#define B_FLASH_UMAP1_MDTBA               0xFF000000                    ///< MIP Descriptor Table Base Address
#define N_FLASH_UMAP1_MDTBA               24                            ///< MDTBA bits position
#define N_FLASH_UMAP1_MDTBA_REPR          4                             ///< MDTBA address representation position

//
// Straps location definitions
//

//
// MMP0
//
#define R_FLASH_STRAP_MMP0                 0xC4    ///< MMP0 Soft strap offset
#define B_FLASH_STRAP_MMP0                 0x10    ///< MMP0 Soft strap bit

#define R_FLASH_STRAP_SFDP                 0xF0    ///< PCH Soft Strap SFDP
#define B_FLASH_STRAP_SFDP_QIORE           BIT3    ///< Quad IO Read Enable
#define B_FLASH_STRAP_SFDP_QORE            BIT2    ///< Quad Output Read Enable
#define B_FLASH_STRAP_SFDP_DIORE           BIT1    ///< Dual IO Read Enable
#define B_FLASH_STRAP_SFDP_DORE            BIT0    ///< Dual Output Read Enable

//
// Descriptor Record 0
//
#define R_FLASH_STRAP_DSCR_0               0x00    ///< PCH Soft Strap 0
#define B_FLASH_STRAP_DSCR_0_PTT_SUPP      BIT22   ///< PTT Supported

#define R_FLASH_STRAP_DSCR_EC_DISABLED     0x2F0

#define R_PCH_SPI_STRP_GBETSN_DIS           0xC18    ///< GBETSN_DIS strap offset.
#define B_PCH_SPI_STRP_GBETSN_DIS           BIT3     ///< GBETSN_DIS Start Bit

#define R_PCH_SPI_STRP_THC_DIS              0xC20    ///< THC_DIS strap offset.
#define B_PCH_SPI_STRP_THC0_DIS             BIT25    ///< THC0_DIS Start Bit
#define B_PCH_SPI_STRP_THC1_DIS             BIT26    ///< THC1_DIS Start Bit

#endif
