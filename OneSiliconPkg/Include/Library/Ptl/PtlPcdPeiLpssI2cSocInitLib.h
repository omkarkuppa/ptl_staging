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

#ifndef PTL_PCD_PEI_LPSS_I2C_SOC_INIT_LIB_H
#define PTL_PCD_PEI_LPSS_I2C_SOC_INIT_LIB_H

#include <LpssI2cHandle.h>

/**
  Build LPSS I2C Handle structures

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] I2cHandle        The Lpss I2C Handle instance
  @param[in]      I2cDeviceConfig  Lpss I2C Config Block
  @param[in]      I2cIndex         I2C Controller index

  @retval     EFI_SUCCESS          Completed successfully
              EFI_UNSUPPORTED      SOC is not supported
**/
EFI_STATUS
PtlPcdLpssI2cBuildHandle (
  IN SI_POLICY_PPI                  *SiPolicy,
  IN OUT LPSS_I2C_HANDLE            *I2cHandle,
  IN LPSS_I2C_CONTROLLER_CONFIG     *I2cDeviceConfig,
  IN UINT8                          I2cIndex
  );

/**
  Initialize the all I2C controllers integrated in the SOC's LPSS

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         I2C Config Block not found
**/
EFI_STATUS
PtlPcdLpssI2cSocInit (
  IN  SI_POLICY_PPI   *SiPolicy
  );

#endif //PTL_PCD_PEI_LPSS_I2C_SOC_INIT_LIB_H
