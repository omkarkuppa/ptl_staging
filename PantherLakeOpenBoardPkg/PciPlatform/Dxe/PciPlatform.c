/**@file
  Registers onboard PCI ROMs with PCI.IO

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
#include "PciPlatform.h"
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Library/SmbusInfoLib.h>
#include <Library/PreSiliconEnvDetectLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PCI_PLATFORM_PROTOCOL mPciPlatform = {
  PhaseNotify,
  PlatformPrepController,
  GetPlatformPolicy,
  GetPciRom
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE mPciPlatformHandle = NULL;




EFI_STATUS
EFIAPI
PhaseNotify (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE  Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Platform Prep Controller

  This          - GC_TODO: add argument description
  HostBridge    - GC_TODO: add argument description
  RootBridge    - GC_TODO: add argument description
  PciAddress    - GC_TODO: add argument description
  Phase         - GC_TODO: add argument description
  ChipsetPhase  - GC_TODO: add argument description

  EFI_UNSUPPORTED - GC_TODO: Add description for return value
**/
EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get Platform Policy

  This      - GC_TODO: add argument description
  PciPolicy - GC_TODO: add argument description

  EFI_UNSUPPORTED - GC_TODO: Add description for return value
**/
EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN CONST EFI_PCI_PLATFORM_PROTOCOL               *This,
  OUT EFI_PCI_PLATFORM_POLICY                      *PciPolicy
  )
{
  if (IsSimicsEnvironment()) {
    *PciPolicy = EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_ALIAS;
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  Return a PCI ROM image for the onboard device represented by PciHandle

  This      - Protocol instance pointer.
  PciHandle - PCI device to return the ROM image for.
  RomImage  - PCI Rom Image for onboard device
  RomSize   - Size of RomImage in bytes

  EFI_SUCCESS   - RomImage is valid
  EFI_NOT_FOUND - No RomImage
**/
EFI_STATUS
EFIAPI
GetPciRom (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL             *This,
  IN  EFI_HANDLE                                  PciHandle,
  OUT VOID                                        **RomImage,
  OUT UINTN                                       *RomSize
  )
{
  return EFI_UNSUPPORTED;
}


/**
  PciPlatformDriverEntry

  EFI_STATUS
**/
EFI_STATUS
EFIAPI
PciPlatformDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;

  //
  // Install on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &mPciPlatformHandle,
                  &gEfiPciPlatformProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPciPlatform
                  );

  return Status;
}
