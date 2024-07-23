/** @file
  Register names for Serial IO UART Controllers

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

#ifndef _SERIAL_IO_UART_REGS_H_
#define _SERIAL_IO_UART_REGS_H_

//
//  MMIO
//
#define R_SERIAL_IO_UART_MEM_RBR             0x00      //< REG RBR Receive Buffer Register.
#define R_SERIAL_IO_UART_MEM_THR             0x00      //< REG THR Transmit Holding Register
#define R_SERIAL_IO_UART_MEM_DLL             0x00      //< REG DLL Divisor Latch (Low) Register.
#define R_SERIAL_IO_UART_MEM_DLH             0x04      //< REG DLH Divisor Latch (High) Register.
#define R_SERIAL_IO_UART_MEM_FCR             0x08      //< REG FCR FIFO Control Register.
#define R_SERIAL_IO_UART_MEM_IIR             0x08      //< REG IIR Interrupt Control Register.
#define R_SERIAL_IO_UART_MEM_LCR             0x0C      //< REG LCR Line Control Register.
#define R_SERIAL_IO_UART_MEM_MCR             0x10      //< REG MCR Modem Control Register.
#define R_SERIAL_IO_UART_MEM_LSR             0x14      //< REG LSR Line Status Register
#define R_SERIAL_IO_UART_MEM_USR             0x7C      //< REG USR UART Status Register.
#define R_SERIAL_IO_UART_MEM_CTR             0xFC      //< REG CTR Component Type Register.
#define UART_COMPONENT_IDENTIFICATION_CODE   0x44570110

//
//  MMIO Convergance Layer
//
#define R_SERIAL_IO_UART_MEM_RESETS          0x204      //< REG RESETS software reset
#define R_SERIAL_IO_UART_MEM_CLOCKS          0x200      //< REG CLOCKS Private Clock Configuraton
#define V_SERIAL_IO_MEM_PPR_CLK_M_DIV        0x25A
#define V_SERIAL_IO_MEM_PPR_CLK_N_DIV        0x7FFF


#define R_SERIAL_IO_UART_MEM_RESETS_8_BIT    0x81  //< Convert to 8 bit Offset
#define R_SERIAL_IO_UART_MEM_CLOCKS_8_BIT    0x80
//
//  General Purpose
//
#define R_SERIAL_IO_UART_PCR_GEN_REGRW7      0x618      //< General Purpose Register7

#endif // _SERIAL_IO_UART_REGS_H_
