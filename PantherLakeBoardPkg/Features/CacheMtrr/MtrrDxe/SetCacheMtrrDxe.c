/** @file
  Source code file for Set Cache Mtrr Dxe module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Protocol/Cpu.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IGpuInfoLib.h>

/**
  This function will update the GT Aperture Region to Write Combining.

  @retval EFI_SUCCESS             CreateProtocolNotifyEvent for PciEnumCompleteCallback Succesful
  @retval other                  Some error occurs when executing this function

**/
EFI_STATUS
UpdateGmAdrRegionToWriteCombine (
   VOID
  );

/**
  This function gets registered as a callback to perform CacheMtrrDxe  functions after PCI enumeration completes.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/

VOID
EFIAPI
PciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  Set Cache Mtrr Dxe module entry point

  @param[in] ImageHandle            The firmware allocated handle for the EFI image
  @param[in] SystemTable            A pointer to the EFI System Table

   @retval EFI_SUCCESS               The entry point is executed successfully
  @retval other                     Some error occurs when executing this entry point

**/
EFI_STATUS
EFIAPI
SetCacheMtrrDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "SetCacheMtrrDxeEntryPoint\n"));
  Status = UpdateGmAdrRegionToWriteCombine();
  return Status;
}


/**
  This function will update the GT Aperture Region to Write Combining.

  @retval EFI_SUCCESS             CreateProtocolNotifyEvent for PciEnumCompleteCallback Succesful
  @retval other                  Some error occurs when executing this function
.

**/
EFI_STATUS
UpdateGmAdrRegionToWriteCombine (
  VOID
  )
{
  VOID                      *Registration;

  DEBUG ((DEBUG_INFO, "Call backs to update GmAdr region as write combine\n"));

  ///
  /// Create PCI Enumeration Completed callback
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    PciEnumCompleteCallback,
    NULL,
    &Registration
    );

    return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform CacheMtrrDxe  functions after PCI enumeration completes.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS              Status;
  VOID                    *ProtocolPointer;
  UINT64                  GtApertureSize;
  UINT64                  GtApertureAdr;
  EFI_CPU_ARCH_PROTOCOL   *CpuArch;

  DEBUG ((DEBUG_INFO, "CacheMtrrDxe PciEnumCompleteCallback Start\n"));

  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  GtApertureAdr = 0;
  GtApertureSize = SIZE_256MB; // @todo : Read PCI BAR value instead of hard code.

  ///
  /// If device 0:2:0 (Internal Graphics Device, or GT) is not present, skip it.
  ///
  if (FALSE == IGpuIsSupported ()) {
    DEBUG((DEBUG_INFO, "IGD is not present, skip PciEnumCompleteCallback as part of UpdateGmAdrRegionToWriteCombine ()\n"));
    return;
  }

  ///
  /// If Internal Display Engine is not present, skip it.
  ///
  if (FALSE == IGpuIsDisplayPresent ()) {
    DEBUG ((DEBUG_INFO, "Exit PciEnumCompleteCallback as part of UpdateGmAdrRegionToWriteCombine() since Display Engine is not present.\n"));
    return;
  }

  GtApertureAdr = IGpuGetLMemBar ();

  Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **) &CpuArch);
  ASSERT_EFI_ERROR (Status);

  Status = CpuArch->SetMemoryAttributes (
                      CpuArch,
                      GtApertureAdr,
                      GtApertureSize,
                      EFI_MEMORY_WC
                      );
  if (EFI_SUCCESS != Status) {
    DEBUG((DEBUG_INFO, "Setting GmAdr Memory as WriteCombine failed! Cache the Flash area as UC to release MTRR. \n"));
    Status = CpuArch->SetMemoryAttributes (
                      CpuArch,
                      0xff000000,
                      0x01000000,
                      EFI_MEMORY_UC
                      );
    DEBUG((DEBUG_INFO, "Setting Flash Memory as UnCacheable %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    Status = CpuArch->SetMemoryAttributes (
                      CpuArch,
                      GtApertureAdr,
                      GtApertureSize,
                      EFI_MEMORY_WC
                      );
    DEBUG((DEBUG_INFO, "Setting GmAdr Memory as WriteCombine %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "CacheMtrrDxe Platform PciEnumCompleteCallback End\n"));

  return;
}
