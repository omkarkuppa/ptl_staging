/** @file
  USB4 Connection Manager Platform PEI driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4PeiLib.h>
#include "Usb4PlatformPei.h"
#include "Usb4PlatformDriver.h"

EFI_PEI_NOTIFY_DESCRIPTOR mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) Usb4PlatformEndOfPeiPpiNotifyCallback
};

/**
  The callback function for EndOfPeiPpi.
  - Query USB4 platform information
  - Allocate IOMMU DMA buffer for use in Dxe
  - Create USB4 platform hob to pass USB4 platform information to Dxe

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_UNSUPPORTED      - The function failed.
**/
EFI_STATUS
EFIAPI
Usb4PlatformEndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS            Status;
  USB4_PLATFORM_INFO    *Usb4PlatformInfo;

  DEBUG ((DEBUG_INFO, "Usb4PlatformEndOfPeiPpiNotifyCallback entry\n"));

  Status = EFI_SUCCESS;
  Usb4PlatformInfo = NULL;

  //
  // Allocate memory for receiving the host router information in the platform.
  //
  Usb4PlatformInfo = AllocateZeroPool (sizeof (USB4_PLATFORM_INFO));
  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Out of resources for querying USB4 platform information\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query USB4 platform information for CM support.
  //
  Status = Usb4PlatformInfoQuery (Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to query USB4 platform information for CM support, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // USB4 platform initialization for CM support
  //
  Status = Usb4PlatformInit (Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB4 platform initialization failure for CM support, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Create USB4 platform hob only if platform is ready for USB4 support.
  //
  Status = CreateUsb4InfoHob (Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to create USB4 Info Hob, %r\n", Status));
    goto Exit;
  }

Exit:
  if (Usb4PlatformInfo != NULL) {
    FreePool (Usb4PlatformInfo);
  }
  DEBUG ((DEBUG_INFO, "Usb4PlatformEndOfPeiPpiNotifyCallback exit\n"));
  return Status;
}

/**
  USB4 Platform PEI driver entry

  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS           - The function completes successfully
**/
EFI_STATUS
EFIAPI
Usb4PlatformPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "Usb4PlatformPeiEntry entry\n"));

  //
  // Register a notification for End of PEI to allocate IOMMU DMA buffer.
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Usb4PlatformPeiEntry: Failed to register a notification - %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "Usb4PlatformPeiEntry exit\n"));
  return Status;
}