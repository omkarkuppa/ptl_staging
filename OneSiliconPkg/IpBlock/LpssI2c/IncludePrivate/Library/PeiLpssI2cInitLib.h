/** @file
  Initialize I2C controllers.

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

#ifndef _PEI_LPSS_I2C_INIT_LIB_H_
#define _PEI_LPSS_I2C_INIT_LIB_H_

#include <LpssI2cHandle.h>

/**
  Configures Lpss I2c Controllers

  @param[in] I2cHandle        The Lpss I2c Handle instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       Usupported I2C controller mode
**/
EFI_STATUS
LpssI2cInit (
  IN LPSS_I2C_HANDLE    *I2cHandle
  );

/**
  Performs Early disable for Lpss I2C Controller

  @param[in] LpssAccess         The SerialIo I2C Register Access

**/
VOID
LpssI2cEarlyDisable (
  IN REGISTER_ACCESS   *LpssAccess
  );

/**
  Configures Lpss I2c Function 0 Disabled Controllers

  @param[in] I2cHandle        The Lpss UART Handle instance
**/
VOID
LpssI2cConfigureFun0Disable (
  IN LPSS_I2C_HANDLE   *I2cHandle,
  IN BOOLEAN           FunDisable
  );
#endif //_PEI_LPSS_I2C_INIT_LIB_H_
