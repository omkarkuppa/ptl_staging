/** @file
  Prototype of the NhltLib library.

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

@par Specification Reference:
**/

#ifndef _INSTALL_BT_CONFIGURATION_VARIABLE_H_
#define _INSTALL_BT_CONFIGURATION_VARIABLE_H_

#include <Library/PcdLib.h>
#include <Uefi/UefiBaseType.h>
#include <NhltConfiguration.h>
#include <NhltBtConfigurationVariable.h>

/**
  Install NHLT BT UEFI variable
  @param[in]                 NhltBtActiveConfig   Value of active BT configuration
  @retval EFI_SUCCESS        The function completed successfully
  @retval Others             Failed to install UEFI variable
**/
EFI_STATUS
InstallNhltBtVariable (
  IN UINT8                   NhltBtConfigurationValue
  );

#endif /** _INSTALL_BT_CONFIGURATION_VARIABLE_H_ **/
