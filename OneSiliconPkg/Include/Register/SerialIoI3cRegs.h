/** @file
  Register names for SERIAL_IO_I3C

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
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _SERIALIOI3CREGS_H_
#define _SERIALIOI3CREGS_H_

//
// I3C0 registers
//
#define R_SERIAL_IO_I3C0_MEM_QUEUE_THLD_CTRL              0xD0            ///< REG QUEUE_THLD_CTRL
#define R_SERIAL_IO_I3C0_MEM_DATA_BUFFER_THLD_CTRL        0xD4            ///< REG DATA_BUFFER_THLD_CTRL
#define R_SERIAL_IO_I3C0_MEM_SCL_I3C_OD_TIMING            0x214           ///< REG SCL_I3C_OD_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_I3C_PP_TIMING            0x218           ///< REG SCL_I3C_PP_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_I2C_FM_TIMING            0x21C           ///< REG SCL_I2C_FM_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_I2C_FMP_TIMING           0x220           ///< REG SCL_I2C_FMP_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_I2C_SS_TIMING            0x224           ///< REG SCL_I2C_SS_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_EXT_LCNT_TIMING          0x228           ///< REG SCL_EXT_LCNT_TIMING
#define R_SERIAL_IO_I3C0_MEM_SCL_EXT_TERMN_LCNT_TIMING    0x22C           ///< REG SCL_EXT_TERMN_LCNT_TIMING
#define R_SERIAL_IO_I3C0_MEM_SDA_HOLD_SWITCH_DLY_TIMING   0x230           ///< REG SDA_HOLD_SWITCH_DLY_TIMING
#define R_SERIAL_IO_I3C0_MEM_BUS_FREE_TIMING              0x234           ///< REG BUS_FREE_TIMING

//
// I3C1 registers
//
#define R_SERIAL_IO_I3C1_MEM_QUEUE_THLD_CTRL              0x4D0           ///< REG QUEUE_THLD_CTRL
#define R_SERIAL_IO_I3C1_MEM_DATA_BUFFER_THLD_CTRL        0x4D4           ///< REG DATA_BUFFER_THLD_CTRL
#define R_SERIAL_IO_I3C1_MEM_SCL_I3C_OD_TIMING            0x614           ///< REG SCL_I3C_OD_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_I3C_PP_TIMING            0x618           ///< REG SCL_I3C_PP_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_I2C_FM_TIMING            0x61C           ///< REG SCL_I2C_FM_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_I2C_FMP_TIMING           0x620           ///< REG SCL_I2C_FMP_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_I2C_SS_TIMING            0x624           ///< REG SCL_I2C_SS_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_EXT_LCNT_TIMING          0x628           ///< REG SCL_EXT_LCNT_TIMING
#define R_SERIAL_IO_I3C1_MEM_SCL_EXT_TERMN_LCNT_TIMING    0x62C           ///< REG SCL_EXT_TERMN_LCNT_TIMING
#define R_SERIAL_IO_I3C1_MEM_SDA_HOLD_SWITCH_DLY_TIMING   0x630           ///< REG SDA_HOLD_SWITCH_DLY_TIMING
#define R_SERIAL_IO_I3C1_MEM_BUS_FREE_TIMING              0x634           ///< REG BUS_FREE_TIMING

//
// I3C2 registers
//
#define R_SERIAL_IO_I3C2_MEM_QUEUE_THLD_CTRL              0xD0            ///< REG QUEUE_THLD_CTRL
#define R_SERIAL_IO_I3C2_MEM_DATA_BUFFER_THLD_CTRL        0xD4            ///< REG DATA_BUFFER_THLD_CTRL
#define R_SERIAL_IO_I3C2_MEM_SCL_I3C_OD_TIMING            0x214           ///< REG SCL_I3C_OD_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_I3C_PP_TIMING            0x218           ///< REG SCL_I3C_PP_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_I2C_FM_TIMING            0x21C           ///< REG SCL_I2C_FM_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_I2C_FMP_TIMING           0x220           ///< REG SCL_I2C_FMP_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_I2C_SS_TIMING            0x224           ///< REG SCL_I2C_SS_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_EXT_LCNT_TIMING          0x228           ///< REG SCL_EXT_LCNT_TIMING
#define R_SERIAL_IO_I3C2_MEM_SCL_EXT_TERMN_LCNT_TIMING    0x22C           ///< REG SCL_EXT_TERMN_LCNT_TIMING
#define R_SERIAL_IO_I3C2_MEM_SDA_HOLD_SWITCH_DLY_TIMING   0x230           ///< REG SDA_HOLD_SWITCH_DLY_TIMING
#define R_SERIAL_IO_I3C2_MEM_BUS_FREE_TIMING              0x234           ///< REG BUS_FREE_TIMING

#define V_SERIAL_IO_I3C_MEM_QUEUE_THLD_CTRL               0x200002
#define V_SERIAL_IO_I3C_MEM_DATA_BUFFER_THLD_CTRL         0x1010404
#define V_SERIAL_IO_I3C_MEM_SCL_I3C_OD_TIMING             0xFA00FA
#define V_SERIAL_IO_I3C_MEM_SCL_I3C_PP_TIMING             0x80008
#define V_SERIAL_IO_I3C_MEM_SCL_I2C_FM_TIMING             0xFA00FA
#define V_SERIAL_IO_I3C_MEM_SCL_I2C_FMP_TIMING            0x640064
#define V_SERIAL_IO_I3C_MEM_SCL_I2C_SS_TIMING             0x32003AC
#define V_SERIAL_IO_I3C_MEM_SCL_EXT_LCNT_TIMING           0x20202020
#define V_SERIAL_IO_I3C_MEM_SCL_EXT_TERMN_LCNT_TIMING     0x30000
#define V_SERIAL_IO_I3C_MEM_SDA_HOLD_SWITCH_DLY_TIMING    0x10000
#define V_SERIAL_IO_I3C_MEM_BUS_FREE_TIMING               0x8


#define R_SERIAL_IO_I3C_MEM_ACTIVELTR                     0x2BC           ///< REG ACTIVELTR
#define V_SERIAL_IO_I3C_MEM_ACTIVELTR                     0x800

#define R_SERIAL_IO_I3C_MEM_IDLELTR                       0x2C0           ///< REG IDLELTR
#define V_SERIAL_IO_I3C_MEM_IDLELTR                       0x800

#define R_SERIAL_IO_I3C_MEM_CLOCK_GATE                    0x2C8           ///< REG CLOCK_GATE
#define V_SERIAL_IO_I3C_MEM_CLOCK_GATE                    0x0

#define R_SERIAL_IO_I3C_MEM_DEVIDLE_CONTROL               0x2CC           ///< REG DEVIDLE_CONTROL
#define V_SERIAL_IO_I3C_MEM_DEVIDLE_CONTROL               0x8

#define R_SERIAL_IO_I3C_MEM_HCI_COMP_VER_OVRD             0x2D0           ///< REG HCI_COMP_VER_OVRD
#define V_SERIAL_IO_I3C_MEM_HCI_COMP_VER_OVRD             0x00000052


#define R_SERIAL_IO_I3C_MEM_I3C_THRESHOLD                 0x2D4           ///< REG I3C_Threshold
#define V_SERIAL_IO_I3C_MEM_I3C_THRESHOLD                 0x20442044

#endif      // _SERIALIOI3CREGS_H_

