/** @file
  Cpu driver, which initializes ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "CpuInitDxe.h"
#include <PowerMgmtNvsStruct.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <CpuPowerManagementDataHob.h>
#include <CpuPowerMgmt.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Protocol/BiosGuardNvsArea.h>
#include <Register/GenerationMsr.h>

GLOBAL_REMOVE_IF_UNREFERENCED BIOSGUARD_NVS_AREA_PROTOCOL mBiosGuardNvsAreaProtocol;

/**
  Initialize BIOS Guard Nvs Area Protocol

  @param[in] ImageHandle - Image handle of the loaded driver

  @retval EFI_SUCCESS           - thread can be successfully created
**/
EFI_STATUS
BiosGuardAcpiInit (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                    Status;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;

  ///
  /// Allocated ACPI NVS type memory for BiosGuard Nvs Configuration.
  ///
  Pages = EFI_SIZE_TO_PAGES (sizeof (BIOSGUARD_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  Pages,
                  &Address
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "EfiACPIMemoryNVS pages for BIOS Guard NVS not Allocated\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mBiosGuardNvsAreaProtocol.Area = (BIOSGUARD_NVS_AREA *)(UINTN) Address;
  ZeroMem ((VOID *)mBiosGuardNvsAreaProtocol.Area, sizeof (BIOSGUARD_NVS_AREA));

  ///
  /// Install BIOS Guard NVS Area protocol
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gBiosGuardNvsAreaProtocolGuid,
                  &mBiosGuardNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
