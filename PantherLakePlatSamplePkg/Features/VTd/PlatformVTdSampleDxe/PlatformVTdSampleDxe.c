/** @file
  Platform VTd Sample driver.

  Note: This module should only be used for dev/debug purposes.
        It MUST never be used for production builds.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <IndustryStandard/Vtd.h>
#include <Protocol/PlatformVtdPolicy.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>

#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>

#include <IndustryStandard/DmaRemappingReportingTable.h>


#pragma pack(1)

typedef struct {
  EDKII_PLATFORM_VTD_EXCEPTION_DEVICE_INFO     ExceptionDeviceInfo;
  EDKII_PLATFORM_VTD_DEVICE_SCOPE              DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       PciBridge;
} PLATFORM_EXCEPTION_DEVICE_SCOPE_STRUCT;

typedef struct {
  EDKII_PLATFORM_VTD_EXCEPTION_DEVICE_INFO     ExceptionDeviceInfo;
  EDKII_PLATFORM_VTD_PCI_DEVICE_ID             PciDeviceId;
} PLATFORM_EXCEPTION_PCI_DEVICE_ID_STRUCT;

#pragma pack()


/**
  Get the VTD SourceId from the device handler.
  This function is required for non PCI device handler.

  Pseudo-algo in Intel VTd driver:
    Status = PlatformGetVTdDeviceId ();
    if (EFI_ERROR(Status)) {
      if (DeviceHandle is PCI) {
        Get SourceId from Bus/Device/Function
      } else {
        return EFI_UNSUPPORTED
      }
    }
    Get VTd engine by Segment/Bus/Device/Function.

  @param[in]  This                  The protocol instance pointer.
  @param[in]  DeviceHandle          Device Identifier in UEFI.
  @param[out] DeviceInfo            DeviceInfo for indentify the VTd engine in ACPI Table
                                    and the VTd page entry.

  @retval EFI_SUCCESS           The VtdIndex and SourceId are returned.
  @retval EFI_INVALID_PARAMETER DeviceHandle is not a valid handler.
  @retval EFI_INVALID_PARAMETER DeviceInfo is NULL.
  @retval EFI_NOT_FOUND         The Segment or SourceId information is NOT found.
  @retval EFI_UNSUPPORTED       This function is not supported.

**/
EFI_STATUS
EFIAPI
PlatformVTdGetDeviceId (
  IN  EDKII_PLATFORM_VTD_POLICY_PROTOCOL       *This,
  IN  EFI_HANDLE                               DeviceHandle,
  OUT EDKII_PLATFORM_VTD_DEVICE_INFO           *DeviceInfo
  )
{
  EFI_PCI_IO_PROTOCOL       *PciIo;
  UINTN                     Seg;
  UINTN                     Bus;
  UINTN                     Dev;
  UINTN                     Func;
  EFI_STATUS                Status;

  DEBUG ((DEBUG_INFO, "PlatformVTdGetDeviceId\n"));

  if (DeviceInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DeviceHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Handle PCI device
  //
  Status = gBS->HandleProtocol (DeviceHandle, &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
  if (!EFI_ERROR(Status)) {
    Status = PciIo->GetLocation (PciIo, &Seg, &Bus, &Dev, &Func);
    if (EFI_ERROR(Status)) {
      return EFI_UNSUPPORTED;
    }
    DeviceInfo->Segment = (UINT16)Seg;
    DeviceInfo->SourceId.Bits.Bus = (UINT8)Bus;
    DeviceInfo->SourceId.Bits.Device = (UINT8)Dev;
    DeviceInfo->SourceId.Bits.Function = (UINT8)Func;

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Get a list of the exception devices.

  The VTd driver should always set ALLOW for the device in this list.

  @param[in]  This                  The protocol instance pointer.
  @param[out] DeviceInfoCount       The count of the list of DeviceInfo.
  @param[out] DeviceInfo            A callee allocated buffer to hold a list of DeviceInfo.
                                    Each DeviceInfo pointer points to EDKII_PLATFORM_VTD_EXCEPTION_DEVICE_INFO.

  @retval EFI_SUCCESS           The DeviceInfoCount and DeviceInfo are returned.
  @retval EFI_INVALID_PARAMETER DeviceInfoCount is NULL, or DeviceInfo is NULL.
  @retval EFI_UNSUPPORTED       This function is not supported.

**/
EFI_STATUS
EFIAPI
PlatformVTdGetExceptionDeviceList (
  IN  EDKII_PLATFORM_VTD_POLICY_PROTOCOL       *This,
  OUT UINTN                                    *DeviceInfoCount,
  OUT VOID                                     **DeviceInfo
  )
{
  DEBUG ((DEBUG_INFO, "PlatformVTdGetExceptionDeviceList\n"));

  if (DeviceInfoCount == NULL || DeviceInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *DeviceInfoCount = 0;


  if (*DeviceInfoCount == 0) {
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }
}

EDKII_PLATFORM_VTD_POLICY_PROTOCOL  mPlatformVTdSample = {
  EDKII_PLATFORM_VTD_POLICY_PROTOCOL_REVISION,
  PlatformVTdGetDeviceId,
  PlatformVTdGetExceptionDeviceList,
};


/**
  Platform VTd sample driver.

  @param[in]  ImageHandle  ImageHandle of the loaded driver
  @param[in]  SystemTable  Pointer to the System Table

  @retval  EFI_SUCCESS           The Protocol is installed.
  @retval  EFI_OUT_OF_RESOURCES  Not enough resources available to initialize driver.
  @retval  EFI_DEVICE_ERROR      A device error occurred attempting to initialize the driver.

**/
EFI_STATUS
EFIAPI
PlatformVTdSampleInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  return Status;
}
