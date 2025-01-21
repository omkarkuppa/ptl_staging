/** @file
  Header file for USB4 connection manager driver.

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

#ifndef _USB4_CM_DXE_H_
#define _USB4_CM_DXE_H_
#include <Uefi.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4PlatformInfo.h>

extern PUSB4_HR_INSTANCE   mUsb4HrInst[USB4_HR_SUPPORT_MAX];
extern UINT32              mUsb4HrCount;

//
// USB4 CM monitor timer interval.
// The unit is 100us, takes 100ms as interval.
//
#define USB4_CM_MONITOR_TIMER_INTERVAL     EFI_TIMER_PERIOD_MILLISECONDS(100)

/**
  USB4 CM execution for USB4 host routers in the platform.

  @param[in] ImageHandle      - ImageHandle of the loaded driver.
  @param[in] Usb4PlatformInfo - Pointer to the platform USB4 host router information.

  @retval EFI_SUCCESS           - Software CM execution success.
  @retval EFI_UNSUPPORTED       - General USB4 CM execution failure.
  @retval EFI_OUT_OUT_RESOURCES - Insufficient resources to execute CM.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CmExecute (
  IN EFI_HANDLE            ImageHandle,
  IN USB4_PLATFORM_INFO    *Usb4PlatformInfo
  );

/**
  Stop CM and release resources.
  The resources will be re-assigned in PCI enumeration phase.
**/
VOID
Usb4CmStop (
  VOID
  );

/**
  Destroy all CM resources and instances.
**/
VOID
Usb4CmDestroy (
  VOID
  );

/**
  Unpower USB4 host router in platform.
**/
VOID Usb4CmUnpowerHr (
  VOID
  );
#endif
