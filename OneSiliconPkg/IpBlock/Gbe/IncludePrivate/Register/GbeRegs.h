/** @file
  Register names for GbE device

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

@par Specification
**/
#ifndef _GBE_REGS_H_
#define _GBE_REGS_H_


#define N_GBE_CFG_DMI_CONFIG4_ADDR            17

#define R_GBE_CFG_CPCE                        0x00000080U      ///< CPCE
#define B_GBE_CFG_CPCE_HW_AUTONOM_EN          BIT5

#define R_GBE_CFG_CLCTL                       0x00000094U      ///< CLCTL

#define R_GBE_CFG_LANDISCTRL                  0x000000a0U      ///< LANDISCTRL
#define B_GBE_CFG_LANDISCTRL_DISABLE          BIT0

#define R_GBE_CFG_LOCKLANDIS                  0x000000a4U      ///< LOCKLANDIS
#define B_GBE_CFG_LOCKLANDIS_LOCK             BIT0

#define R_GBE_CFG_LTRC                        0x000000a8U      ///< LTRC

#define R_GBE_CFG_PM1                         0x000000ccU      ///< PM1
#define B_GBE_CFG_PM1_PWR_STATE               ( BIT1 | BIT0 )
#define V_GBE_CFG_PM1_PWR_STATE               0x00

//
// Gigabit Ethernet LAN Capabilities and Status Registers (Memory space)
//

#define R_GBE_MEM_CTRL                        0x00000000U      ///< CTRL
//
// LANPHYPC:
// Connects to the LCD DEVICE_OFF# signal in the
// LAN Connected Device
//
#define B_GBE_MEM_CTRL_LANPHYPC_OVRIDE        BIT16  // When set to 1 SW driver has the ability to control the LANPHYPC pin value.
#define B_GBE_MEM_CTRL_LANPHYPC_VAL           BIT17  // When set to 1 this bit will define the value of the LANPHYPC pin.
#define B_GBE_MEM_CTRL_MEHE                   BIT19

#define R_GBE_MEM_STATUS                      0x00000008U      ///< STATUS
#define B_GBE_MEM_STATUS_LAN_INIT_DONE        BIT9

#define R_GBE_MEM_FEXTNVM6                    0x00000010U      ///< FEXTNVM6
#define B_GBE_MEM_FEXTNVM6_K1_OFF_EN          BIT31

#define R_GBE_MEM_CTRL_EXT                    0x00000018U      ///< CTRL_EXT
#define B_GBE_MEM_CTRL_EXT_LPCD               BIT2  //LCD Power Cycle Done (LPCD). This bit indicates whether LCD power cycle is done
                                                    //- the bit is set 50/100mSec after LANPHYPC pin assertion.
#define B_GBE_MEM_CTRL_EXT_PHY_WOL            BIT11

#define R_GBE_MEM_MDIC                        0x00000020U      ///< MDIC
#define B_GBE_MEM_MDIC_R                      BIT28

#define R_GBE_MEM_FEXT                        0x0000002cU      ///< FEXT
#define B_GBE_MEM_FEXT_WOL_ENABLE             BIT30
#define B_GBE_MEM_FEXT_WOL_VALID              BIT31

#define R_GBE_MEM_PBECCSTS                    0x0000100cU      ///< PBECCSTS
#define B_GBE_MEM_PBECCSTS_ECC_EN             BIT16

#define R_GBE_MEM_EXTCNF_CTRL                 0x00000f00U      ///< EXTCNF_CTRL
#define B_GBE_MEM_EXTCNF_CTRL_SWFLAG          BIT5

#define R_GBE_MEM_RAL_0                       0x00005400U      ///< RAL_0

#define R_GBE_MEM_RAH_0                       0x00005404U      ///< RAH_0
#define B_GBE_MEM_RAH_0_RAH                   0x0000ffffU

#define R_GBE_MEM_WUC                         0x00005800U      ///< WUC
#define B_GBE_MEM_WUC_APME                    BIT0

#define R_GBE_MEM_FEXTNVM12                   0x00005bc0U      ///< FEXTNVM12
#define B_GBE_MEM_FEXTNVM12_LOCK_WRITE_TO_FLASH BIT13

#endif // _GBE_REGS_H_
