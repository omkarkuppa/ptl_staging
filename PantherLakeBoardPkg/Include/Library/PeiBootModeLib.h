/** @file
Public header file for the Boot Mode Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _BOOT_MODE_LIB_H_
#define _BOOT_MODE_LIB_H_

#include <Ppi/ReadOnlyVariable2.h>

/**
  Update boot mode if it is in capsule update boot mode.

  @param[in,out] BootMode         Current System BootMode.

  @retval EFI_SUCCESS             Boot Mode is updated.
  @retval Others                  Some issues occur in updating Boot Mode.

**/
EFI_STATUS
EFIAPI
PreMemUpdateCapsuleBootMode (
  IN OUT EFI_BOOT_MODE          *BootMode
  );

/**
  Detect boot mode

  @retval EFI_BOOT_MODE
**/
EFI_BOOT_MODE
EFIAPI
DetectBootMode (
  VOID
  );

#endif
