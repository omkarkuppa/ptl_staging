/**@file
  The file implements NVMe Based Recovery HII functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _NVME_RECOVERY_HII_H_
#define _NVME_RECOVERY_HII_H_

#include <Protocol/HiiConfigAccess.h>
#include <Library/HiiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiHiiServicesLib.h>
#include "NvmeRecoveryDxe.h"

#pragma pack(1)

typedef struct {
    VENDOR_DEVICE_PATH             VendorDevicePath;
    EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

/**
  This function to initialize NvmeRecoverHii value.

  @retval  EFI_SUCCESS  Successfully initialize variable.
  @retval  Others       Failed initialize variable.
**/
EFI_STATUS
InitNvmeRecoveryHiiDefault (
  VOID
  );

/**
  Install the HII related resources.

  @retval  EFI_SUCCESS        Install all the resources success.
  @retval  other              Error occur when install the resources.
**/
EFI_STATUS
NvmeRecoveryHiiInstall (
  VOID
  );

#endif // _NVME_RECOVERY_HII_H_
