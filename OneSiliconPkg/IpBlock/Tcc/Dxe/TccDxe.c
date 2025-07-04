/** @file
  DXE TCC Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <Base.h>
#include <CpuRegs.h>
#include <PiDxe.h>
#include <Register/Cpuid.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/PlatformNvsArea.h>

PLATFORM_NVS_AREA                        *mPlatformNvsArea = NULL;

/**
  Update Time Coordinated Compute related ACPI NVS settings.

  @retval EFI_SUCCESS             TCC ACPI parameters updated.
  @retval EFI_NOT_FOUND           Unable to find and upate ACPI parameters.
**/
EFI_STATUS
TccUpdateNvsArea (
  VOID
  )
{
  CPUID_EXTENDED_TIME_STAMP_COUNTER_EDX  Edx;
  CPUID_PROCESSOR_FREQUENCY_EBX          Ebx;

  if (mPlatformNvsArea == NULL) {
    return EFI_NOT_FOUND;
  }

  AsmCpuid (CPUID_TIME_STAMP_COUNTER, NULL, &Ebx.Uint32, NULL, NULL);
  AsmCpuid (CPUID_EXTENDED_TIME_STAMP_COUNTER, NULL, NULL, NULL, &Edx.Uint32);

  // if (Edx.Bits.InvariantTsc == 1 && Ebx.Uint32 != 0) {
  //   DEBUG ((DEBUG_INFO, "ATSC True\n"));
  //   mPlatformNvsArea->ATSC = 1;
  // } else {
  //   DEBUG ((DEBUG_INFO, "ATSC False\n"));
  //   mPlatformNvsArea->ATSC = 0;
  // }

  return EFI_SUCCESS;
}


/**
  Update the Core related information(APIC ID) for MHL reporting
  This is executed across all threads

@retval EFI_SUCCESS             The function completed successfully
@retval EFI_NOT_FOUND           Unable to find and upate ACPI parameters

**/
EFI_STATUS
UpdateArtTscLink (
  VOID
  )
{
  PLATFORM_NVS_AREA_PROTOCOL        *mPlatformNvsAreaProtocol;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (
                &gPlatformNvsAreaProtocolGuid,
                NULL,
                (VOID **)&mPlatformNvsAreaProtocol
                );

  if (EFI_ERROR (Status) || (mPlatformNvsAreaProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "Error: PlatformNvsArea protocol error. Unable to set TCC ACPI variables\n"));
    return Status;
  }

  mPlatformNvsArea = mPlatformNvsAreaProtocol->Area;
  if (mPlatformNvsArea != NULL) {
    Status = TccUpdateNvsArea ();
  } else {
    DEBUG ((DEBUG_ERROR, "Error: GlobalNvsArea->Area NULL.\n"));
    Status = EFI_NOT_FOUND;
  }
  return Status;
}

/**
  This function installs TCC ACPI RTCT table

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
TccDxeEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "[TCC] TccDxeEntry() - Start\n"));

  // Update ART Tracable to TSC Flag in GNVS
  Status = UpdateArtTscLink();

  DEBUG ((DEBUG_INFO, "[TCC] TccDxeEntry() - end\n"));
  return Status;
}
