/**@file
    Defines macros required to get SOC Segment and Device ID

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef __TEST_CASE_GET_SOC_SERIES_H__
#define __TEST_CASE_GET_SOC_SERIES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <Uefi.h>
#include <Library/UnitTestLib.h>

typedef UINT8 SOC_SERIES;

#define SOC_PTL_PCD_P                                0x0
#define SOC_PTL_PCD_H                                0x1
#define SOC_UNKNOWN_SERIES                           0xFF

#define ESPI_CFG_DID_PTL_PCD_H                       0xE300
#define ESPI_CFG_DID_PTL_PCD_P                       0xE400

///
/// The default PCH PCI bus number
///
#define DEFAULT_PCI_BUS_NUMBER_PCH                   0
#define DEFAULT_PCI_SEGMENT_NUMBER_PCH               0
#define LPC_CFG_DID                                  0xFFE0

//
// LPC Controller (D31:F0)
//
#define PCI_DEVICE_NUMBER_PCH_LPC                     31
#define PCI_FUNCTION_NUMBER_PCH_LPC                   0

#define SERIAL_IO_ERROR_DEVID                        0xFFFF

//
//  LPSS I2C0 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID        0xE478
#define PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID        0xE378
//
//  LPSS I2C1 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C1_DEVICE_ID        0xE479
#define PTL_PCD_H_LPSS_I2C_CFG_I2C1_DEVICE_ID        0xE379
//
//  LPSS I2C2 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C2_DEVICE_ID        0xE47A
#define PTL_PCD_H_LPSS_I2C_CFG_I2C2_DEVICE_ID        0xE37A
//
//  LPSS I2C3 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C3_DEVICE_ID        0xE47B
#define PTL_PCD_H_LPSS_I2C_CFG_I2C3_DEVICE_ID        0xE37B
//
//  LPSS I2C4 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C4_DEVICE_ID        0xE450
#define PTL_PCD_H_LPSS_I2C_CFG_I2C4_DEVICE_ID        0xE350
//
//  LPSS I2C5 Controller DevId
//
#define PTL_PCD_P_LPSS_I2C_CFG_I2C5_DEVICE_ID        0xE451
#define PTL_PCD_H_LPSS_I2C_CFG_I2C5_DEVICE_ID        0xE351

/**
  Get SOC Segment
  @retval SOC Segment
**/

SOC_SERIES
GetSocSeries (
  VOID);

/**
  Gets I2cs Device Id

  @param[in] I2cNumbe               Lpss device I2C number

  @retval                            Device Id
**/
UINT16
GetLpssI2cDeviceId (
  IN UINT8       I2cNumber
  );

#ifdef __cplusplus
}
#endif

#endif
