/** @file
  I2C Private Library header.

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

#ifndef _LPSS_I2C_PRIVATE_LIB_H_
#define _LPSS_I2C_PRIVATE_LIB_H_

#include <LpssI2cController.h>
#include <LpssI2cConfig.h>

/**
  Configures Lpss Controller before control is passd to the OS

  @param[in] I2cController     Instace of I2C Controller
**/
VOID
LpssI2cBootHandler (
  IN  LPSS_I2C_CONTROLLER   *I2cController
  );

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     I2cController  Instace of I2C Controller
  @param[in]     TurnOff        If PCIe function is disabled
  @param[in/out] Command        Pci Command register data to save
  @param[in/out] Pme            Pci Pme Control register data to save

**/
VOID
LpssI2cS3Handler (
  IN     LPSS_I2C_CONTROLLER  *I2cController,
  IN     BOOLEAN              TurnOff,
  IN OUT UINT32               *Command,
  IN OUT UINT32               *Pme
  );

#endif // _LPSS_I2C_PRIVATE_LIB_H_
