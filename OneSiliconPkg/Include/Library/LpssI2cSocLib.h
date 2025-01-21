/** @file
  Lpss I2C SoC library. Library allow to configure I2C controller
  All function in this library is available for PEI.

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

#ifndef _LPSS_I2C_SOC_LIB_H_
#define _LPSS_I2C_SOC_LIB_H_

/**
  Gets I2cs Device Id

  @param[in] I2cNumbe               Lpss device I2C number

  @retval                            Device Id
**/
UINT16
GetLpssI2cDeviceId (
  IN UINT8       I2cNumber
  );

/**
  Checks if higher PCIe functions are enabled.
  Used for Function 0 Lpss Device disabling

  @param[in] Seg    PCIe Segment
  @param[in] Bus    PCIe Bus
  @param[in] Dev    PCIe Devie

  @retval TRUE            At least one higher function device is enabled
          FALSE           Higher functions are disabled
**/
BOOLEAN
LpssI2cHigherFunctionsEnabled (
  IN  UINT8   Seg,
  IN  UINT8   Bus,
  IN  UINT8   Dev
  );

#endif //_LPSS_I2C_SOC_LIB_H_
