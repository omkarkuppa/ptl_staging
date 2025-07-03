/** @file
  Acpi Gnvs Init Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PlatformNvsArea.h>
#include <Library/DxeAcpiGnvsInitLib.h>
#include <Protocol/MpService.h>

/**
@brief
  Global NVS initialize.

  @param[in] PlatformNvsAreaPtr    - Pointer of global NVS area

  @retval EFI_SUCCESS              - Allocate Global NVS completed.
  @retval EFI_OUT_OF_RESOURCES     - Failed to allocate required page for PlatformNvsAreaProtocol.
**/
EFI_STATUS
EFIAPI
AcpiGnvsInit (
  IN OUT VOID               **PlatformNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;
  PLATFORM_NVS_AREA_PROTOCOL    *PlatformNvsAreaProtocol;
  EFI_MP_SERVICES_PROTOCOL      *MpService;
  UINTN                         NumberOfCPUs;
  UINTN                         NumberOfEnabledCPUs;

  Pages = EFI_SIZE_TO_PAGES (sizeof (PLATFORM_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Locate the MP services protocol
  // Find the MP Protocol. This is an MP platform, so MP protocol must be there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpService
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
              MpService,
              &NumberOfCPUs,
              &NumberOfEnabledCPUs
              );

  *PlatformNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*PlatformNvsAreaPtr, sizeof (PLATFORM_NVS_AREA), 0);

  //
  // PlatformNvsAreaProtocol default value init here...
  //
  PlatformNvsAreaProtocol = (PLATFORM_NVS_AREA_PROTOCOL *) &Address;

  PlatformNvsAreaProtocol->Area->ThreadCount = (UINT8)NumberOfEnabledCPUs;

  //
  // Thermal trip points
  //
  PlatformNvsAreaProtocol->Area->Ac0TripPoint                = 71;
  PlatformNvsAreaProtocol->Area->Ac1TripPoint                = 55;
  PlatformNvsAreaProtocol->Area->Ac0FanSpeed                 = 100;
  PlatformNvsAreaProtocol->Area->Ac1FanSpeed                 = 75;
  PlatformNvsAreaProtocol->Area->PassiveThermalTripPoint     = 95;
  PlatformNvsAreaProtocol->Area->PassiveTc1Value             = 1;
  PlatformNvsAreaProtocol->Area->PassiveTc2Value             = 5;
  PlatformNvsAreaProtocol->Area->PassiveTspValue             = 10;
  PlatformNvsAreaProtocol->Area->CriticalThermalTripPoint    = 119;

  //
  // Miscellaneous
  //
  PlatformNvsAreaProtocol->Area->ConfigTdpBios = 0;
  PlatformNvsAreaProtocol->Area->PL1LimitCS = 0;
  PlatformNvsAreaProtocol->Area->PL1LimitCSValue = 4500;

  return EFI_SUCCESS;
}

