/** @file
  Register names for Serial IO I2C Controllers

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
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _SERIAL_IO_I2C_REGS_H_
#define _SERIAL_IO_I2C_REGS_H_

//
// I2C Controller
// Registers accessed through BAR0 + offset
//
#define R_SERIAL_IO_I2C_MEM_IC_CON            0x00      //< REG IC_CON I2c Control
#define V_I2C_MEM_SPEED_STANDARD              0x02
#define V_I2C_MEM_SPEED_FAST                  0x04
#define V_I2C_MEM_SPEED_HIGH                  0x06

#define R_SERIAL_IO_I2C_MEM_IC_TAR            0x04      //< REG IC_TAR I2c Target Address

#define R_SERIAL_IO_I2C_MEM_IC_DATA_CMD       0x10      //< REG IC_DATA_CMD I2c Rx/Tx Data Buffer and Command

#define R_SERIAL_IO_I2C_MEM_IC_SS_SCL_HCNT    0x14      //< REG IC_SS_SCL_HCNT Standard Speed I2c Clock SCL High Count
#define R_SERIAL_IO_I2C_MEM_IC_SS_SCL_LCNT    0x18      //< REG IC_SS_SCL_LCNT Standard Speed I2c Clock SCL Low Count
#define R_SERIAL_IO_I2C_MEM_IC_FS_SCL_HCNT    0x1C      //< REG IC_FS_SCL_HCNT Full Speed I2c Clock SCL High Count
#define R_SERIAL_IO_I2C_MEM_IC_FS_SCL_LCNT    0x20      //< REG IC_FS_SCL_LCNT Full Speed I2c Clock SCL Low Count
#define R_SERIAL_IO_I2C_MEM_IC_HS_SCL_HCNT    0x24      //< REG IC_HS_SCL_HCNT High Speed I2c Clock SCL High Count
#define R_SERIAL_IO_I2C_MEM_IC_HS_SCL_LCNT    0x28      //< REG IC_HS_SCL_LCNT High Speed I2c Clock SCL Low Count
#define R_SERIAL_IO_I2C_MEM_IC_INTR_STAT      0x2C      //< REG IC_INTR_STAT I2c Inetrrupt Status
#define R_SERIAL_IO_I2C_MEM_IC_CLR_INTR       0x40      //< REG IC_CLR_INTR Clear Combined and Individual Interrupts
#define R_SERIAL_IO_I2C_MEM_IC_CLR_TX_ABRT    0x54      //< REG IC_CLR_TX_ABRT TX_ABRT interrupt
#define R_SERIAL_IO_I2C_MEM_IC_ENABLE         0x6C      //< REG IC_ENABLE I2c Enable

#define R_SERIAL_IO_I2C_MEM_IC_STATUS         0x70      //< REG IC_STATUS I2c Status

#define R_SERIAL_IO_I2C_MEM_IC_TXFLR          0x74      //< REG IC_TXFLR Transmit FIFO Level Register
#define R_SERIAL_IO_I2C_MEM_IC_RXFLR          0x78      //< REG IC_RXFLR Receive FIFO Level Register
#define R_SERIAL_IO_I2C_MEM_IC_SDA_HOLD       0x7C      //< REG IC_SDA_HOLD
#define V_SERIAL_IO_I2C_MEM_SDA_HOLD_VALUE    0x002C002C
#define R_SERIAL_IO_I2C_MEM_IC_TX_ABRT_SOURCE 0x80      //< REG IC_TX_ABRT_SOURCE I2c Transmit Abort Status Register

#define R_SERIAL_IO_I2C_MEM_IC_ENABLE_STATUS  0x9C      //< REG IC_ENABLE_STATUS I2c Enable Status Register

#define R_SERIAL_IO_I2C_MEM_GENERAL           0x208     //< REG GENERAL I2C Convergence Layer General

#endif // _SERIAL_IO_I2C_REGS_H_
