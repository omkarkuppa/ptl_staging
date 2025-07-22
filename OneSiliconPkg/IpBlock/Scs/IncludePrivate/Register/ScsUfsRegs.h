/** @file
  Register definitions for UFS controllers on SCS.

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

#ifndef _SCS_UFS_REGS_H_
#define _SCS_UFS_REGS_H_

#define UFS_UIC_ARG_MIB_SEL(x,y)   ((((x) & 0xFFFF) << 16) | ((y) & 0xFFFF))

//
// UFS Mmio Registers
//
#define R_SCS_MEM_UFS_AUTO_LTR_VALUE                     0x808                      ///< Auto_Ltr_Value
#define R_UFS_MEM_REF_CLK                                0x85C                      ///< Ref Clock Register
#define V_UFS_MEM_REF_CLK                                0x3

//
// MMP_UFS registers
//
#define R_SCS_PCR_MMPUFS_DLANE1                          0x2000

#define R_SCS_PCR_MMPUFS_DLANEX_TX3_SYNC_LEN             0x28
#define R_SCS_PCR_MMPUFS_DLANEX_IMP8                     0x101C
#define R_SCS_PCR_MMPUFS_DLANEX_IMP8_PWRGT               0x101F
#define R_SCS_PCR_MMPUFS_DLANEX_IMP23_INTFILTER          0x105B
#define R_SCS_PCR_MMPUFS_DLANEX_IMP22                    0x1054
#define B_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            (BIT26 | BIT25 | BIT24)
#define N_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            24
#define V_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD            0x7

typedef struct {
  UINT32    UicOpcode;
  UINT32    Arg1;
  UINT32    Arg2;
  UINT32    Arg3;
} EDKII_UIC_COMMAND;

//
// UIC command opcodes
//
typedef enum {
  UfsUicDmeGet            = 0x01,
  UfsUicDmeSet            = 0x02,
  UfsUicDmePeerGet        = 0x03,
  UfsUicDmePeerSet        = 0x04,
  UfsUicDmePwrOn          = 0x10,
  UfsUicDmePwrOff         = 0x11,
  UfsUicDmeEnable         = 0x12,
  UfsUicDmeReset          = 0x14,
  UfsUicDmeEndpointReset  = 0x15,
  UfsUicDmeLinkStartup    = 0x16,
  UfsUicDmeHibernateEnter = 0x17,
  UfsUicDmeHibernateExit  = 0x18,
  UfsUicDmeTestMode       = 0x1A
} UFS_UIC_OPCODE;

typedef enum {
  ULP_REG                = 0x8130,
  SRAM_CFG_8116          = 0x8116,
  SRAM_CFG_8117          = 0x8117,
  SRAM_CFG_8118          = 0x8118,
  SRAM_CFG_8119          = 0x8119,
  SRAM_CFG_811C          = 0x811C,
  VS_MPHYCFG_UPDT        = 0xD085,
  MPHY_DISABLE           = 0xD0C1,
} ULP_ATTRIBUTES;

//
// UFS Mmio Registers
//
#define R_SCS_MEM_UFS_EN                                             0x34
#define B_SCS_MEM_UFS_EN                                             BIT0
#define R_SCS_MEM_UFS_UIC_CMD                                        0x90
#define V_SCS_MEM_UFS_UIC_CMD_DME_SET                                0x2
#define R_SCS_MEM_UFS_UCMD_ARG1                                      0x94
#define R_SCS_MEM_UFS_UCMD_ARG2                                      0x98
#define R_SCS_MEM_UFS_UCMD_ARG3                                      0x9C
#define R_SCS_MEM_UFS_IS                                             0x20
#define B_SCS_MEM_UFS_IS_UCCS                                        BIT10
#define R_SCS_MEM_UFS_HC_STATUS                                      0x30
#define B_SCS_MEM_UFS_HCS_UCRDY                                      BIT3
#define UIC_CMD_TIMEOUT                                              500                ///< UIC Command Timeout in us
#define R_UFS_MEM_REF_CLK                                            0x85C              ///< Ref Clock Register
#define V_UFS_MEM_REF_CLK                                            0x3
#define UFS_HC_AHIT_OFFSET                                           0x18               ///< Auto-Hibernate Idle Timer
#define V_UFS_MEM_AHIT_VALUE                                         0x100              ///< Auto-Hibernate Idle Timer Value

#endif
