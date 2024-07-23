/** @file
 Serial IO I2C Private Registers

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

//
// SDA Hold Defines
//
#define V_SERIAL_IO_I2C_MEM_SDA_TX_100_1000_KHZ    0x10
#define V_SERIAL_IO_I2C_MEM_SDA_RX_100_1000_KHZ    0x20
#define V_SERIAL_IO_I2C_MEM_SDA_TX_1000_3400_KHZ   0x20
#define V_SERIAL_IO_I2C_MEM_SDA_RX_1000_3400_KHZ   0x0
#define V_SERIAL_IO_I2C_MEM_SDA_TX_3400_INF_KHZ    0x10
#define V_SERIAL_IO_I2C_MEM_SDA_RX_3400_INF_KHZ    0x0

//
// I2C Clock SCL Defines
//
#define V_SERIAL_IO_I2C_MEM_SS_SCL_HCNT_20          2045
#define V_SERIAL_IO_I2C_MEM_SS_SCL_LCNT_20          2880
#define V_SERIAL_IO_I2C_MEM_SS_SCL_HCNT_50          818
#define V_SERIAL_IO_I2C_MEM_SS_SCL_LCNT_50          1152
#define V_SERIAL_IO_I2C_MEM_SS_SCL_HCNT_70          706
#define V_SERIAL_IO_I2C_MEM_SS_SCL_LCNT_70          612
#define V_SERIAL_IO_I2C_MEM_SS_SCL_HCNT             409
#define V_SERIAL_IO_I2C_MEM_SS_SCL_LCNT             576
#define V_SERIAL_IO_I2C_MEM_SS_FCL_HCNT_400_1000    63
#define V_SERIAL_IO_I2C_MEM_SS_FCL_LCNT_400_1000    179
#define V_SERIAL_IO_I2C_MEM_SS_FCL_HCNT_1000_3400   28
#define V_SERIAL_IO_I2C_MEM_SS_FCL_LCNT_1000_3400   65
#define V_SERIAL_IO_I2C_MEM_HS_FCL_HCNT             16
#define V_SERIAL_IO_I2C_MEM_HS_FCL_LCNT             34

