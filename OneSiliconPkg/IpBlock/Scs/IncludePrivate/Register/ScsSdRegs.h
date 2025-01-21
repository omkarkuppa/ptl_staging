/** @file
  Register definitions for SD controllers on SCS.

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
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _SCS_SD_REGS_H_
#define _SCS_SD_REGS_H_

//
// SD Devices MMIO Space Register
//
#define R_SCS_MEM_DMAADR                                    0x00
#define R_SCS_MEM_BLKSZ                                     0x04
#define R_SCS_MEM_BLKCNT                                    0x06
#define R_SCS_MEM_CMDARG                                    0x08
#define R_SCS_MEM_XFRMODE                                   0x0C
#define B_SCS_MEM_XFRMODE_BLKCNT_EN                         BIT1
#define B_SCS_MEM_XFRMODE_DATA_TRANS_DIR                    BIT4               ///< 1: Read (Card to Host), 0: Write (Host to Card)
#define R_SCS_MEM_SDCMD                                     0x0E
#define V_SCS_MEM_SDCMD_RESP_TYPE_SEL_RESP136               1
#define V_SCS_MEM_SDCMD_RESP_TYPE_SEL_RESP48                2
#define V_SCS_MEM_SDCMD_RESP_TYPE_SEL_RESP48_CHK            3
#define B_SCS_MEM_SDCMD_CMD_CRC_CHECK_EN                    BIT3
#define B_SCS_MEM_SDCMD_CMD_INDEX_CHECK_EN                  BIT4
#define B_SCS_MEM_SDCMD_DATA_PRESENT_SEL                    BIT5
#define R_SCS_MEM_RESP                                      0x10
#define R_SCS_MEM_PSTATE                                    0x24
#define R_SCS_MEM_PWRCTL                                    0x29
#define R_SCS_MEM_CLKCTL                                    0x2C
#define R_SCS_MEM_TIMEOUT_CTL                               0x2E               ///< Timeout Control
#define R_SCS_MEM_SWRST                                     0x2F
#define B_SCS_MEM_SWRST_CMDLINE                             BIT1
#define B_SCS_MEM_SWRST_DATALINE                            BIT2
#define R_SCS_MEM_NINTSTS                                   0x30
#define B_SCS_MEM_NINTSTS_CMD_COMPLETE                      BIT0
#define B_SCS_MEM_NINTSTS_TRANSFER_COMPLETE                 BIT1
#define R_SCS_MEM_ERINTSTS                                  0x32
#define B_SCS_MEM_ERINTSTS_MASK                             0x13FF
#define R_SCS_MEM_NINTEN                                    0x34
#define R_SCS_MEM_ERINTEN                                   0x36
#define R_SCS_MEM_HOST_CTL                                  0x28
#define R_SCS_MEM_HOST_CTL2                                 0x3E
#define R_SCS_MEM_CAP1                                      0x40
#define R_SCS_MEM_CAP2                                      0x44
#define R_SCS_MEM_CAP_BYPASS_CONTROL                        0x810
#define V_SCS_MEM_CAP_BYPASS_CONTROL_EN                     0x5A
#define R_SCS_MEM_CAP_BYPASS_REG1                           0x814
#define B_SCS_MEM_CAP_BYPASS_REG1_HS400                     BIT29
#define R_SCS_MEM_CAP_BYPASS_REG2                           0x818
#define R_SCS_MEM_TX_CMD_DLL_CNTL                           0x820
#define R_SCS_MEM_TX_DATA_DLL_CNTL1                         0x824
#define R_SCS_MEM_TX_DATA_DLL_CNTL2                         0x828
#define R_SCS_MEM_RX_CMD_DATA_DLL_CNTL1                     0x82C
#define R_SCS_MEM_RX_STROBE_DLL_CNTL                        0x830
#define R_SCS_MEM_RX_CMD_DATA_DLL_CNTL2                     0x834
#define R_SCS_MEM_SD_CAP_BYPS                               0x880

#endif

