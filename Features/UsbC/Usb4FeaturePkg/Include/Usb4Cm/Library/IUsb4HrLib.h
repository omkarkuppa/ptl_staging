/** @file
  Header file for integrated USB4 host router initialization.

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

@par Specification
**/

#ifndef _I_USB4_HR_PEI_LIB_H_
#define _I_USB4_HR_PEI_LIB_H_

#include <Uefi.h>
#include <Library/Usb4PlatformInfo.h>

/**
  Query the integrated USB4 host router information in the platform.

  @param[out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Query the integrated USB4 host router information successfully.
  @retval EFI_NOT_FOUND         - No integrated USB4 host router found in platform.
  @retval EFI_UNSUPPORTED       - Fail to query the integrated USB4 host router information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4PlatformInfoQuery (
  OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  );

#endif
