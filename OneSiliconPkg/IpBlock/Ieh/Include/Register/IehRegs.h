/** @file

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
#ifndef _IEH_REGS_H_
#define _IEH_REGS_H_

#define R_IEH_CFG_MISCCTL1               0xE4
#define B_IEH_CFG_MISCCTL1_IE            BIT31
#define B_IEH_CFG_MISCCTL1_IFD           BIT30
#define R_IEH_CFG_GCOERRMSK              0x230
#define R_IEH_CFG_GNFERRMSK              0x234
#define R_IEH_CFG_GFAERRMSK              0x238
#define R_IEH_CFG_GSYSEVTMSK             0x264
#define R_IEH_CFG_GSYSEVTMAP             0x268
#define R_IEH_CFG_IEHTYPEVER             0x26C
#define N_IEH_CFG_IEHTYPEVER_IEH_BUSNUM  8
#define B_IEH_CFG_IEHTYPEVER_IEH_BUSNUM  0xFF00
#define R_IEH_CFG_ERRPINCTRL             0x270
#define R_IEH_CFG_ERRPINDATA             0x278
#define R_IEH_CFG_BITMAP                 0x27C
#define R_IEH_CFG_LERRUNCMSK_N0          0x298
#define R_IEH_CFG_LERRUNCSEV_N0          0x2A0
#define R_IEH_CFG_LERRCORMSK_N0          0x2C0
#define R_IEH_CFG_LCORERRCNTSEL_N0       0x2C8

#endif
