/** @file
  Register names for PCH private chipset register

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

#ifndef _PCH_PCR_REGS_DEPRECATED_H_
#define _PCH_PCR_REGS_DEPRECATED_H_

/**
 * IMPORTANT NOTE:
 *
 * This file is deprecated!
 * There were Sideband Port ID definitions in the file but
 * original content of the file has been moved to coresponding SbPortIds.h in FRU/SoC
 * General assumption is the Sideband Port ID definiton should be specific per SoC/FRU
 * and used only by SoC/FRU code.
 * Because some IP Blocks haven't been already refactored to meet I1S compliance
 * and some IP Block code use directly Sideband Por IDs definitions this file must be
 * temporarly be maintained until full I1S compliance will be achieved by PCH code.
 * Finally the file shall be removed.
 **/

#define PID_ICLK_DEPRECATED       0x72
#define PID_DMI_DEPRECATED        0x2F // SoC-S only - not valid
#define PID_ESPISPI_DEPRECATED    0x6D
#define PID_SERIALIO_DEPRECATED   0x65
#define PID_PSTH_DEPRECATED       0x6A
#define PID_ITSS_DEPRECATED       0x00 // ITSS

#endif // _PCH_PCR_REGS_DEPRECATED_H_
