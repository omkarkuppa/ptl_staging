/** @file
  Incompatible PCI Device Support protocol allows the PCI bus driver to support
  resource allocation for some PCI devices that do not comply with the PCI Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <PiDxe.h>
#include <Protocol/IncompatiblePciDeviceSupport.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>

#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Acpi.h>
#include <Defines/HostBridgeDefines.h>
#include <Defines/IgdDefines.h>
#include <HostBridgeDataHob.h>

#include <Library/IpuLib.h>
#include <Library/HostBridgeInfoLib.h>

#define OLD_ALIGN   0xFFFFFFFFFFFFFFFFULL

/**
  Returns a list of ACPI resource descriptors that detail the special
  resource configuration requirements for an incompatible PCI device.

  @param  This                  Pointer to the EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL instance.
  @param  VendorId              A unique ID to identify the manufacturer of the PCI device.
  @param  DeviceId              A unique ID to identify the particular PCI device.
  @param  RevisionId            A PCI device-specific revision identifier.
  @param  SubsystemVendorId     Specifies the subsystem vendor ID.
  @param  SubsystemDeviceId     Specifies the subsystem device ID.
  @param  Configuration         A list of ACPI resource descriptors returned that detail
                                the configuration requirement.

  @retval EFI_SUCCESS           Successfully got ACPI resource for specified PCI device.
  @retval EFI_INVALID_PARAMETER Configuration is NULL.
  @retval EFI_OUT_OF_RESOURCES  No memory available.
  @retval EFI_UNSUPPORTED       The specified PCI device wasn't supported.

**/
EFI_STATUS
EFIAPI
PCheckDevice (
  IN  EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL  *This,
  IN  UINTN                                         VendorId,
  IN  UINTN                                         DeviceId,
  IN  UINTN                                         RevisionId,
  IN  UINTN                                         SubsystemVendorId,
  IN  UINTN                                         SubsystemDeviceId,
  OUT VOID                                          **Configuration
  );

//
// Handle onto which the Incompatible PCI Device List is installed
//
EFI_HANDLE                                    mIncompatiblePciDeviceSupportHandle = NULL;

//
// The Incompatible PCI Device Support Protocol instance produced by this driver
//
EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL  mIncompatiblePciDeviceSupport = {
  PCheckDevice
};

/**
  Entry point of the incompatible pci device support code. Setup an incompatible device list template
  and install EFI Incompatible PCI Device Support protocol.

  @param ImageHandle             A handle for the image that is initializing this driver.
  @param SystemTable             A pointer to the EFI system table.

  @retval EFI_SUCCESS            Installed EFI Incompatible PCI Device Support Protocol successfully.
  @retval others                 Failed to install protocol.

**/
EFI_STATUS
EFIAPI
IncompatiblePciDeviceSupport (
  VOID
  )
{
  EFI_STATUS                         Status;

  //
  // Install EFI Incompatible PCI Device Support Protocol on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &mIncompatiblePciDeviceSupportHandle,
                  &gEfiIncompatiblePciDeviceSupportProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mIncompatiblePciDeviceSupport
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Returns a list of ACPI resource descriptors that detail the special
  resource configuration requirements for an incompatible PCI device.

  @param  This                  Pointer to the EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL instance.
  @param  VendorId              A unique ID to identify the manufacturer of the PCI device.
  @param  DeviceId              A unique ID to identify the particular PCI device.
  @param  RevisionId            A PCI device-specific revision identifier.
  @param  SubsystemVendorId     Specifies the subsystem vendor ID.
  @param  SubsystemDeviceId     Specifies the subsystem device ID.
  @param  Configuration         A list of ACPI resource descriptors returned that detail
                                the configuration requirement.

  @retval EFI_SUCCESS           Successfully got ACPI resource for specified PCI device.
  @retval EFI_INVALID_PARAMETER Configuration is NULL.
  @retval EFI_OUT_OF_RESOURCES  No memory available.
  @retval EFI_UNSUPPORTED       The specified PCI device wasn't supported.

**/
EFI_STATUS
EFIAPI
PCheckDevice (
  IN  EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL  *This,
  IN  UINTN                                         VendorId,
  IN  UINTN                                         DeviceId,
  IN  UINTN                                         RevisionId,
  IN  UINTN                                         SubsystemVendorId,
  IN  UINTN                                         SubsystemDeviceId,
  OUT VOID                                          **Configuration
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *AcpiPtr;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *OldAcpiPtr;
  EFI_ACPI_END_TAG_DESCRIPTOR       *PtrEnd;
  BOOLEAN                           ResourceChanged;
  AcpiPtr = NULL;
  OldAcpiPtr = NULL;
  ResourceChanged = FALSE;

  //
  // Validate the parameters
  //
  if (Configuration == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Initialize the return value to NULL
  //
  * (VOID **) Configuration = NULL;

  if ((UINT16) DeviceId == PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, PCI_DEVICE_ID_OFFSET))) {
  //
  // Assign GMADR resource
  //
  if (IsAbove4GBMmioEnabled ()) {
    AcpiPtr = AllocateZeroPool (sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
    if (AcpiPtr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    OldAcpiPtr = AcpiPtr;
    ResourceChanged = TRUE;
    AcpiPtr->Desc = ACPI_ADDRESS_SPACE_DESCRIPTOR;
    //
    // AcpiPtr->Len should be length of this descriptor in bytes, not including the first two fields.
    // so need to - 3
    //
    AcpiPtr->Len = (UINT16) (sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3);
    AcpiPtr->ResType = ACPI_ADDRESS_SPACE_TYPE_MEM;
    AcpiPtr->GenFlag = 0;
    AcpiPtr->SpecificFlag = EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_CACHEABLE_PREFETCHABLE;
    AcpiPtr->AddrSpaceGranularity = 64;
    AcpiPtr->AddrRangeMin = 0;
    AcpiPtr->AddrRangeMax = SIZE_128GB - 1;
    AcpiPtr->AddrTranslationOffset = PCI_BAR_IDX1;
    AcpiPtr->AddrLen = SIZE_256MB;
    AcpiPtr++;
    }
  }

  if (!ResourceChanged) {
    return EFI_UNSUPPORTED;
  }

  //
  // Put the checksum
  //
  PtrEnd                    = (EFI_ACPI_END_TAG_DESCRIPTOR *) (AcpiPtr);
  PtrEnd->Desc              = ACPI_END_TAG_DESCRIPTOR;
  PtrEnd->Checksum          = 0;

  *(VOID **) Configuration  = OldAcpiPtr;

  return EFI_SUCCESS;
}

