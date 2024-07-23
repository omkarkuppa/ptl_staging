/**@file
  Platform Sanitize Setup Header File.

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

#ifndef _PLATFORM_SANITIZE_SETUP_H_
#define _PLATFORM_SANITIZE_SETUP_H_

#include <Guid/MdeModuleHii.h>
#include <Protocol/StorageSanitizeNotifyProtocol.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/HiiConfigAccess.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <PsParameter.h>
#include <Library/PlatformSanitizeServices.h>
#include <Library/PsCsmeAccessRpeLib.h>
#include "SetupForm.h"
#include <Library/DxeAmtSupportLib.h>
//
// Private structure of driver
//
#define PLATFORM_ERASE_SETUP_DRIVER_FROM_THIS(a)  BASE_CR (a, PLATFORM_ERASE_SETUP_DRIVER, HiiConfigAccess)

typedef struct {
  EFI_HANDLE                        Handle;                   // Driver image handle
  EFI_HII_CONFIG_ACCESS_PROTOCOL    HiiConfigAccess;
  EFI_HII_HANDLE                    HiiHandle;                // Handle to the list of HII packages (forms and strings) for this driver
  HII_CONFIG                        HiiCfgData;
  ERASE_DEVICE_LIST                 *DeviceList;
} PLATFORM_ERASE_SETUP_DRIVER;

//
// The device path containing the VENDOR_DEVICE_PATH and EFI_DEVICE_PATH_PROTOCOL
//
typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

//
// This is the generated IFR binary Data for each formset defined in VFR.
// This Data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8  PlatformSanitizeSetupVfrBin[];
extern UINT8  PlatformSanitizeSetupDxeStrings[];

#define HII_CFG_NAME    L"PlatformSanitizeHiiCfg"
#endif /*_PLATFORM_SANITIZE_SETUP_H_*/
