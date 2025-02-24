/** @file
  Device IDs for THC Controllers

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#ifndef _PTL_THC_DEVICE_ID_H_
#define _PTL_THC_DEVICE_ID_H_

#define THC_PCD_H_SPI_DEVICE_ID_1    0xE349
#define THC_PCD_H_SPI_DEVICE_ID_2    0xE34B
#define THC_PCD_P_SPI_DEVICE_ID_1    0xE449
#define THC_PCD_P_SPI_DEVICE_ID_2    0xE44B

#define THC_WCL_PCD_SPI_DEVICE_ID_1  0x4D49
#define THC_WCL_PCD_SPI_DEVICE_ID_2  0x4D4B

#define THC_FCL_PCD_SPI_DEVICE_ID_1  0x67C9
#define THC_FCL_PCD_SPI_DEVICE_ID_2  0x67CB

#define PTL_PCD_H_I2C_DEVICE_ID_1   0xE348
#define PTL_PCD_H_I2C_DEVICE_ID_2   0xE34A
#define PTL_PCD_P_I2C_DEVICE_ID_1   0xE448
#define PTL_PCD_P_I2C_DEVICE_ID_2   0xE44A

#define WCL_PCD_I2C_DEVICE_ID_1     0x4D48
#define WCL_PCD_I2C_DEVICE_ID_2     0x4D4A

#define FCL_PCD_I2C_DEVICE_ID_1     0x67C8
#define FCL_PCD_I2C_DEVICE_ID_2     0x67CA

#endif //_THC_DEVICE_ID_PTL_H_
