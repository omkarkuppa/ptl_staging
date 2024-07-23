/** @file
  Nhlt library header file for getting configuration from PCD or EFI Variable

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

#ifndef _GET_NHLT_CONFIGURATION_LIB_H_
#define _GET_NHLT_CONFIGURATION_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <NhltConfiguration.h>


/**
  Functions provide Nhlt configuration based on user input stored in Efi Variable.

  @param[in][out]     *NhltConfiguration      Pointer to Nhlt configuration.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           NHLT user configuration is corrupted
**/
EFI_STATUS
GetNhltConfiguration (
  IN OUT NHLT_CONFIGURATION  *NhltConfiguration
  );

#endif // _GET_NHLT_CONFIGURATION_LIB_H_
