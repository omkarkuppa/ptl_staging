/** @file
  This is part of the implementation of DG OpRegion initialization.
  Software SCI interface between system BIOS, ASL code, and Graphics drivers.
  The code in this file will load the driver and initialize the interface

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
  - OpRegion / Software SCI SPEC
**/
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/PciIo.h>
#include <Library/PciSegmentLib.h>
#include <Library/CpuPlatformLib.h>
#include <PcieRegs.h>
#include "DgOpRegion.h"
#include <Defines/HostBridgeDefines.h>
#include "HybridGraphicsInfo.h"
#include <Protocol/PlatformNvsArea.h>
#include <SetupVariable.h>

GLOBAL_REMOVE_IF_UNREFERENCED DG_OPREGION_PROTOCOL  DgOpRegion;

/**
  Check if the device id is DG

  @param[in] UINT16      DeviceId

  @retval    BOOLEAN     TRUE if DG DeviceId , FALSE otherwise.

**/
BOOLEAN
IsDgDevice (
  IN UINT16  DeviceId
)
{
  switch (DeviceId) {
    case DG2_DEVICE_ID_1:
    case DG2_DEVICE_ID_2:
    case DG2_DEVICE_ID_3:
    case DG2_DEVICE_ID_4:
    case DG2_DEVICE_ID_5:
    case DG2_DEVICE_ID_6:
    case DG2_DEVICE_ID_7:
    case DG2_DEVICE_ID_8:
    case DG2_DEVICE_ID_9:
    case DG2_DEVICE_ID_19:
    case DG2_DEVICE_ID_20:
    case DG2_DEVICE_ID_21:
      return TRUE;
    default:
      return FALSE;
  }
}

/**
  This function gets registered as a callback to Update DG Graphics OpRegion after PCI enumeration for VRAMSR support

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateDgOnPciEnumComplete (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                     Status;
  UINTN                          HandleCount;
  PCI_TYPE00                     Pci;
  UINTN                          Index;
  UINTN                          Segment;
  UINTN                          Bus;
  UINTN                          Device;
  UINTN                          Function;
  EFI_HANDLE                     *HandleBuffer;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  PCI_DATA_STRUCTURE             *PcirBlockPtr;
  PCI_EXPANSION_ROM_HEADER       *LocalRomImage;
  PLATFORM_NVS_AREA_PROTOCOL     *PlatformNvsAreaProtocol;
  UINT64                         DgBaseAddress;
  UINT64                         ConfigDataSize;
  UINTN                          VariableSize;
  SETUP_DATA                     SetupVariables;
  BOOLEAN                        DgDeviceFound;
  VOID                           *ProtocolPointer;

  Bus              = 0;
  Device           = 0;
  Function         = 0;
  DgDeviceFound    = FALSE;

  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete Start\n"));

  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return;
  }

  ///
  ///  Get the SA policy and the required Dg config block.
  ///
  VariableSize = sizeof (SETUP_DATA);
  ZeroMem (&SetupVariables, VariableSize);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupVariables
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  ///  Locate the SA Global NVS Protocol.
  ///
  Status = gBS->LocateProtocol (&gPlatformNvsAreaProtocolGuid, NULL, (VOID **) &PlatformNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - Not able to locate NVS protocol 0x%x \n", Status));
    return;
  }

  ///
  ///  Get all PCI IO protocols handles
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete -Not able to get PCI IO handles 0x%x \n", Status));
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    ///
    ///  Get the PCI IO Protocol Interface corresponding to each handle
    ///
    PciIo = NULL;

    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );

    if (EFI_ERROR (Status)) {
      ///
      /// Not able to locate the pci io protocol, skip to the next device,
      ///
      continue;
    }

    Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,
                              sizeof (Pci) / sizeof (UINT32),
                              &Pci
                              );

    if (EFI_ERROR (Status) || (NULL == PciIo->RomImage)) {
      ///
      ///  If this PCI device doesn't have a ROM image, skip to the next device,
      ///
      continue;
    }

    LocalRomImage = PciIo->RomImage;
    PcirBlockPtr = (PCI_DATA_STRUCTURE *) ((UINTN) LocalRomImage + LocalRomImage->PcirOffset);
    if ((OPTION_ROM_SIGNATURE != LocalRomImage->Signature) || (NULL == PcirBlockPtr)) {
      ///
      ///  If this PCI device doesn't have a valid Option rom image or pcir block,
      ///  skip to the next device,
      ///
      continue;
    }

    ///
    /// If DG device is found then get the OpRegion start address, OpRegion offset and size
    ///
    if ((Pci.Hdr.VendorId == 0x8086) && (IsDgDevice (Pci.Hdr.DeviceId))) {
      DgDeviceFound  = TRUE;
      break;
    }
  }

  if (DgDeviceFound == FALSE) {
    DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete -DG Device not found\n"));
    FreePool (HandleBuffer);
    return;
  }

  ///
  /// Find the b.d.f of the DG device
  ///
  Status = PciIo->GetLocation (
                      PciIo,
                      &Segment,
                      &Bus,
                      &Device,
                      &Function
                      );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - Not able to locate the b.d.f of DG device 0x%x \n", Status));
    FreePool (HandleBuffer);
    return;
  }

  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - DG device b.d.f 0x%x : 0x%x : 0x%x \n", Bus, Device, Function));
  DgBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, Bus, Device, Function, 0);
  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - DgBaseAddress : 0x%x \n", DgBaseAddress));
  PlatformNvsAreaProtocol->Area->DgBaseAddress = DgBaseAddress;
  PlatformNvsAreaProtocol->Area->DgBrightnessPercentage = 100;

  ///
  ///  Allocate NVS memory of OpRegion Size + 16KB of buffer for VBT which Gfx driver will copy into.
  ///
  ConfigDataSize = sizeof (DG_OPREGION_STRUCTURE) + SIZE_16KB;

  ///
  ///  Allocate an ACPI NVS memory buffer as the DG NVS OpRegion.
  ///
  Status  = (gBS->AllocatePool) (EfiACPIMemoryNVS, ConfigDataSize, (VOID **) &DgOpRegion.OpRegion);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete allocated Nvs OpRegion 0x%x \n", (UINT32) (UINTN) (DgOpRegion.OpRegion)));

  SetMem (DgOpRegion.OpRegion, ConfigDataSize, 0);
  PlatformNvsAreaProtocol->Area->DgOpRegionAddress = (UINT32) (UINTN) (DgOpRegion.OpRegion);

  ///
  /// Initialize OpRegion Header
  ///
  CopyMem (DgOpRegion.OpRegion->Header.SIGN, DG_HEADER_SIGNATURE, sizeof (DG_HEADER_SIGNATURE));
  ///
  /// Set OpRegion Size in KBs
  ///
  DgOpRegion.OpRegion->Header.SIZE = HEADER_SIZE / 1024;
  DgOpRegion.OpRegion->Header.OVER = (UINT32) (LShiftU64 (HEADER_OPREGION_VER, 16) + LShiftU64 (HEADER_OPREGION_REV, 8));

  ///
  /// All Mailboxes are supported.
  ///
  DgOpRegion.OpRegion->Header.MBOX = HEADER_MBOX_SUPPORT;

  ///
  /// Update DG Opregion fields
  ///
  if (SetupVariables.DgPlatformSupport == 1) {
    //
    // Set PCON BIT9 & BIT10 if External Gfx adapter is in MB-down config
    //
    DgOpRegion.OpRegion->Header.PCON |= (BIT9 | BIT10);
  }

  // For DG MBD RVP, BIT 12 & BIT 11 of PCON variable in DG Op-region are required to be set to support VRAM Self Refresh feature
  // By default enabling VRAM SR, oem can override this
  DgOpRegion.OpRegion->Header.PCON |= (BIT12 | BIT11);

  //
  // Set BIT0 & BIT1 if Platform is Connected Standby capable & it's capability field is valid respectively.
  //
  if (SetupVariables.LowPowerS0Idle == 0) {
    DgOpRegion.OpRegion->Header.PCON |= (BIT1 | BIT0);
  } else {
    DgOpRegion.OpRegion->Header.PCON &= (UINT32)(~BIT0);
  }

  DgOpRegion.OpRegion->MBox3.BCLP = BACKLIGHT_BRIGHTNESS;
  DgOpRegion.OpRegion->MBox3.CBLV = (INIT_BRIGHT_LEVEL | FIELD_VALID_BIT);
  DgOpRegion.OpRegion->MBox3.BCLM[0] = (0x0000 + WORD_FIELD_VALID_BIT);  ///< 0%
  DgOpRegion.OpRegion->MBox3.BCLM[1] = (0x0A19 + WORD_FIELD_VALID_BIT);  ///< 10%
  DgOpRegion.OpRegion->MBox3.BCLM[2] = (0x1433 + WORD_FIELD_VALID_BIT);  ///< 20%
  DgOpRegion.OpRegion->MBox3.BCLM[3] = (0x1E4C + WORD_FIELD_VALID_BIT);  ///< 30%
  DgOpRegion.OpRegion->MBox3.BCLM[4] = (0x2866 + WORD_FIELD_VALID_BIT);  ///< 40%
  DgOpRegion.OpRegion->MBox3.BCLM[5] = (0x327F + WORD_FIELD_VALID_BIT);  ///< 50%
  DgOpRegion.OpRegion->MBox3.BCLM[6] = (0x3C99 + WORD_FIELD_VALID_BIT);  ///< 60%
  DgOpRegion.OpRegion->MBox3.BCLM[7] = (0x46B2 + WORD_FIELD_VALID_BIT);  ///< 70%
  DgOpRegion.OpRegion->MBox3.BCLM[8] = (0x50CC + WORD_FIELD_VALID_BIT);  ///< 80%
  DgOpRegion.OpRegion->MBox3.BCLM[9] = (0x5AE5 + WORD_FIELD_VALID_BIT);  ///< 90%
  DgOpRegion.OpRegion->MBox3.BCLM[10] = (0x64FF + WORD_FIELD_VALID_BIT);  ///< 100%

  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - PCON 0x%x Nvs OpRegion 0x%x \n", DgOpRegion.OpRegion->Header.PCON, PlatformNvsAreaProtocol->Area->DgOpRegionAddress));
  ///
  ///  Set ASLS Register to the OpRegion physical memory address..
  ///
  PciSegmentWrite32 (DgBaseAddress + R_SA_DG_ASLS_OFFSET, (UINT32) (UINTN) (DgOpRegion.OpRegion));
  DEBUG ((DEBUG_INFO, "UpdateDgOnPciEnumComplete - Value at R_SA_DG_ASLS_OFFSET 0x%x \n", (PciSegmentRead32 (DgBaseAddress + R_SA_DG_ASLS_OFFSET))));
  FreePool (HandleBuffer);
}

/**
  DG Op-Region Initialization for VRAM SR support

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
DxeDgOpregionInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  VOID        *Registration;

  if (PcdGetBool (PcdDashGEnable) == FALSE) {
    DEBUG ((DEBUG_INFO, "DxeDgOpregionInit - DashG is not enabled\n"));
    return EFI_UNSUPPORTED;
  }

  ///
  /// Create PCI Enumeration Completed callback for DG
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    UpdateDgOnPciEnumComplete,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}
