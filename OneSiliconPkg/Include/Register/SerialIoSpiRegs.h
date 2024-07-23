/** @file
  Register names for Serial IO SPI Controllers

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
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _SERIAL_IO_SPI_REGS_H_
#define _SERIAL_IO_SPI_REGS_H_

//
//  MMIO
//
#define R_SERIAL_IO_SPI_MEM_SSCR0          0x00      //< REG SSCR0
#define V_SERIAL_IO_MEM_SPI_SSCR0_DSS_8    0x07
#define R_SERIAL_IO_SPI_MEM_SSCR1          0x04      //< REG SSCR1
#define R_SERIAL_IO_SPI_MEM_SSSR           0x08      //< REG SSSR
#define R_SERIAL_IO_SPI_MEM_SSITR          0x0C      //< REG SSITR
#define R_SERIAL_IO_SPI_MEM_SSDR           0x10      //< REG SSDR
#define R_SERIAL_IO_SPI_MEM_SIRF           0x48      //< REG SIRF

//
//  MMIO Convergence Layer
//
#define R_SERIAL_IO_SPI_MEM_CLOCKS         0x200      //< REG CLOCKS
#define R_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL 0x224      //< REG SPI_CS_CONTROL
#define N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY 12
#define N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS1_POLARITY 13
#define N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS2_POLARITY 14
#define N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS3_POLARITY 15

#endif // _SERIAL_IO_SPI_REGS_H_
