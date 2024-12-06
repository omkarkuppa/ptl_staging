/** @file
  This is part of the implementation of an Intel IGpu drivers OpRegion /
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
**/

#include <Library/IGpuDxeOpRegionInitLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IGpuInfoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/IgdOpRegion.h>
#include <Protocol/IGpuNvsArea.h>
#include <Library/IGpuInfoLib.h>
#include <Protocol/IGpuPolicy.h>

GLOBAL_REMOVE_IF_UNREFERENCED IGD_OPREGION_PROTOCOL  mIgdOpRegion;

#define HEADER_OPREGION_VER 0x0302

/**
  Update IgdOpRegion Nvs Base address
  @param[in] IgdOpRegionProtocol -       Igd OpRegion Protocol
**/
VOID
IGpuUpdateOpRegionNvs (
  IN IGD_OPREGION_PROTOCOL  IgdOpRegionProtocol
  )
{
  EFI_STATUS              Status;
  IGPU_NVS_AREA_PROTOCOL  *IGpuNvsAreaProtocol;

  ///
  ///  Locate the IGPU NVS Protocol.
  ///
  Status = gBS->LocateProtocol (&gIGpuNvsAreaProtocolGuid, NULL, (VOID **)&IGpuNvsAreaProtocol);
  if (Status == EFI_SUCCESS) {
    IGpuNvsAreaProtocol->Area->IgdOpRegionAddress = (UINT32)(UINTN)(IgdOpRegionProtocol.OpRegion);
  } else {
    DEBUG ((DEBUG_INFO, " Failed to locate IGpuNvsAreaProtocol\n"));
  }
}

/**
  Get VBT data using SaPlaformPolicy

  @param[out] VbtFileBuffer    Pointer to VBT data buffer.

  @retval EFI_SUCCESS      VBT data was returned.
  @retval EFI_NOT_FOUND    VBT data not found.
  @exception EFI_UNSUPPORTED  Invalid signature in VBT data.
**/
EFI_STATUS
IGpuGetIntegratedIntelVbtPtr  (
  OUT VBIOS_VBT_STRUCTURE  **VbtFileBuffer
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  VbtAddress;
  UINT32                Size;
  IGPU_DXE_CONFIG       *IGpuDxeConfig;
  IGPU_POLICY_PROTOCOL  *IGpuPolicy;

  ///
  /// Get the IGPU policy.
  ///
  Status = gBS->LocateProtocol (
                  &gIGpuPolicyProtocolGuid,
                  NULL,
                  (VOID **)&IGpuPolicy
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  VbtAddress = IGpuDxeConfig->VbtAddress;
  Size       = IGpuDxeConfig->Size;

  if (VbtAddress == 0x00000000) {
    return EFI_NOT_FOUND;
  } else {
    ///
    /// Check VBT signature
    ///
    *VbtFileBuffer = NULL;
    *VbtFileBuffer = (VBIOS_VBT_STRUCTURE *)(UINTN)VbtAddress;
    if ((*((UINT32 *)((*VbtFileBuffer)->HeaderSignature))) != VBT_SIGNATURE) {
      FreePool (*VbtFileBuffer);
      *VbtFileBuffer = NULL;
      return EFI_UNSUPPORTED;
    }
  }

  if (Size == 0) {
    return EFI_NOT_FOUND;
  } else {
    ///
    /// Check VBT size
    ///
    if ((*VbtFileBuffer)->HeaderVbtSize > Size) {
      (*VbtFileBuffer)->HeaderVbtSize = (UINT16)Size;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function is to update GOP driver version into IGD OpRegion.

  @retval EFI_SUCCESS        - GOP driver version copied successfully into OpReion
  @exception EFI_NOT_FOUND   - gIGpuPolicyProtocolGuid is not located.
**/
EFI_STATUS
IGpuUpdateGopDriverVersion  (
  VOID
  )
{
  EFI_STATUS            Status;
  IGPU_DXE_CONFIG       *IGpuDxeConfig;
  IGPU_POLICY_PROTOCOL  *IGpuPolicy;

  IGpuPolicy    = NULL;
  IGpuDxeConfig = NULL;

  ///
  /// Get the IGPU policy protocol and Graphics DXE Config block.
  ///
  Status = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **)&IGpuPolicy);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Copy GOP driver version into Op-Region.
  ///
  CopyMem (mIgdOpRegion.OpRegion->Header.DVER, IGpuDxeConfig->GopVersion, sizeof (IGpuDxeConfig->GopVersion));
  DEBUG ((DEBUG_INFO, "GOP Driver version copied to IGD OpRegion.\n"));

  return EFI_SUCCESS;
}

/**
  Graphics OpRegion / Software SCI driver installation function.

  @param[in] void         - None
  @retval EFI_SUCCESS     - The driver installed without error.
  @retval EFI_ABORTED     - The driver encountered an error and could not complete
                            installation of the ACPI tables.
**/
EFI_STATUS
IGpuOpRegionInit (
  VOID
  )
{
  EFI_HANDLE            Handle;
  EFI_STATUS            Status;
  UINT32                DwordData;
  UINT64                IgdBaseAddress;
  IGPU_POLICY_PROTOCOL  *IGpuPolicy;
  IGPU_DXE_CONFIG       *IGpuDxeConfig;
  UINT8                 Index;
  VBIOS_VBT_STRUCTURE   *VbtFileBuffer;
  UINT16                ExtendedVbtSize;

  IGpuPolicy      = NULL;
  IGpuDxeConfig   = NULL;
  VbtFileBuffer   = NULL;
  ExtendedVbtSize = 0;

  DEBUG ((DEBUG_INFO, "IgdOpRegionInit Start\n"));

  ///
  /// Get the IGPU policy.
  ///
  Status = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **)&IGpuPolicy);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  IGpuGetIntegratedIntelVbtPtr (&VbtFileBuffer);

  ///
  /// Check if VBT size is >6KB then allocate an ACPI NVS memory buffer as the IGD OpRegion + extended VBT size,
  /// zero initialize it, and set the IGD OpRegion pointer in the Global NVS area structure.
  ///
  if ((VbtFileBuffer != NULL) && (VbtFileBuffer->HeaderVbtSize > 0x1800)) {
    ExtendedVbtSize = ((VbtFileBuffer->HeaderVbtSize) & (UINT32) ~(0x1FF)) + 0x200;
  }

  Status = (gBS->AllocatePool)(EfiACPIMemoryNVS, sizeof (IGD_OPREGION_STRUCTURE_VER_3_2) + ExtendedVbtSize, (VOID **)&mIgdOpRegion.OpRegion);
  ASSERT_EFI_ERROR (Status);
  SetMem (mIgdOpRegion.OpRegion, sizeof (IGD_OPREGION_STRUCTURE_VER_3_2) + ExtendedVbtSize, 0);
  ///
  /// Update IgdOpRegion Nvs Base address
  ///
  IGpuUpdateOpRegionNvs (mIgdOpRegion);

  ///
  /// If IGD is disabled return
  ///
  IgdBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, 0);
  if (PciSegmentRead32 (IgdBaseAddress + 0) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "Exiting IgdOpRegionInit () since IGPU is not present or supported.\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Initialize OpRegion Header
  ///
  CopyMem (mIgdOpRegion.OpRegion->Header.SIGN, HEADER_SIGNATURE, sizeof (HEADER_SIGNATURE));
  ///
  /// Set OpRegion Size in KBs
  ///
  mIgdOpRegion.OpRegion->Header.SIZE = HEADER_SIZE / 1024;
  mIgdOpRegion.OpRegion->Header.OVER = (UINT32)(LShiftU64 (HEADER_OPREGION_VER, 16) + LShiftU64 (HEADER_OPREGION_REV, 8));

  ///
  /// All Mailboxes are supported.
  ///
  mIgdOpRegion.OpRegion->Header.MBOX = HEADER_MBOX_SUPPORT;

  ///
  /// Initialize OpRegion Mailbox 1 (Public ACPI Methods).
  ///
  /// Note - The initial setting of mailbox 1 fields is implementation specific.
  /// Adjust them as needed many even coming from user setting in setup.
  ///
  mIgdOpRegion.OpRegion->Header.PCON = IGpuDxeConfig->PlatformConfig;
  mIgdOpRegion.OpRegion->Header.PCON = mIgdOpRegion.OpRegion->Header.PCON | 0x2;

  ///
  /// Initialize OpRegion Mailbox 2 (Brightness communication).
  ///
  ///
  /// Set Initial current Brightness
  ///
  mIgdOpRegion.OpRegion->MBox2.BCL1.Bits.Brightness = BACKLIGHT_BRIGHTNESS;
  mIgdOpRegion.OpRegion->MBox2.BCL2.Bits.Brightness = BACKLIGHT_BRIGHTNESS;
  mIgdOpRegion.OpRegion->MBox2.CBL1 = (INIT_BRIGHT_LEVEL | FIELD_VALID_BIT);
  mIgdOpRegion.OpRegion->MBox2.CBL2 = (INIT_BRIGHT_LEVEL | FIELD_VALID_BIT);
  ///
  /// Static Backlight Brightness Level Duty cycle Mapping Table
  ///
  for (Index = 0; Index < MAX_BCLM_ENTRIES; Index++) {
    mIgdOpRegion.OpRegion->MBox2.BCM1[Index] = IGpuDxeConfig->BCM1[Index];
    mIgdOpRegion.OpRegion->MBox2.BCM2[Index] = IGpuDxeConfig->BCM2[Index];
  }

  if (TRUE == IGpuIsDisplayPresent ()) {
    if (VbtFileBuffer != NULL) {
      DEBUG ((DEBUG_INFO, "VBT data found\n"));
      if (ExtendedVbtSize > 0) {
        // VBT > 6KB
        DEBUG ((DEBUG_INFO, "Extended VBT supported\n"));
        mIgdOpRegion.OpRegion->MBox3.RVDA = sizeof (IGD_OPREGION_STRUCTURE_VER_3_2);                      // Relative offset at the end of Op-region.
        mIgdOpRegion.OpRegion->MBox3.RVDS = ((VbtFileBuffer->HeaderVbtSize) & (UINT32) ~(0x1FF)) + 0x200; // Aligned VBT Data Size to 512 bytes.
        CopyMem ((CHAR8 *)(UINTN)(mIgdOpRegion.OpRegion) + sizeof (IGD_OPREGION_STRUCTURE_VER_3_2), VbtFileBuffer, mIgdOpRegion.OpRegion->MBox3.RVDS);
      } else {
        CopyMem (mIgdOpRegion.OpRegion->MBox4.RVBT, VbtFileBuffer, VbtFileBuffer->HeaderVbtSize);
      }
    }
  }

  ///
  /// Initialize hardware state:
  ///   Set ASLS Register to the OpRegion physical memory address.
  ///   Set SWSCI register bit 15 to a "1" to activate SCI interrupts.
  ///
  PciSegmentWrite32 (IgdBaseAddress + R_SA_IGD_ASLS_OFFSET, (UINT32)(UINTN)(mIgdOpRegion.OpRegion));
  PciSegmentAndThenOr16 (IgdBaseAddress + R_SA_IGD_SWSCI_OFFSET, (UINT16) ~(BIT0), BIT15);

  DwordData = PciSegmentRead32 (IgdBaseAddress + R_SA_IGD_ASLS_OFFSET);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN)PcdGet64 (PcdPciExpressBaseAddress) + (IgdBaseAddress + R_SA_IGD_ASLS_OFFSET),
    1,
    &DwordData
    );
  DwordData = PciSegmentRead32 (IgdBaseAddress + R_SA_IGD_SWSCI_OFFSET);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN)PcdGet64 (PcdPciExpressBaseAddress) + (IgdBaseAddress + R_SA_IGD_SWSCI_OFFSET),
    1,
    &DwordData
    );

  ///
  /// Install OpRegion / Software SCI protocol
  ///
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gIgdOpRegionProtocolGuid,
                  &mIgdOpRegion,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "IgdOpRegionInit End\n"));

  ///
  /// Return final status
  ///
  return EFI_SUCCESS;
}

/**
  Update Graphics OpRegion after PCI enumeration.

  @param[in] void         - None
  @retval EFI_SUCCESS     - The function completed successfully.
**/
EFI_STATUS
IGpuUpdateOpRegion (
  VOID
  )
{
  EFI_STATUS           Status;
  UINTN                HandleCount;
  EFI_HANDLE           *HandleBuffer;
  UINTN                Index;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           Pci;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;

  Bus      = 0;
  Device   = 0;
  Function = 0;

  DEBUG ((DEBUG_INFO, "UpdateIgdOpRegion Start\n"));

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (FALSE == IGpuIsSupported ()) {
    DEBUG ((DEBUG_INFO, "Returning from UpdateIgdOpRegion() since IGD is not supported or not enabled\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Update IGD OpRegion with GOP driver version.
  ///
  IGpuUpdateGopDriverVersion ();

  mIgdOpRegion.OpRegion->Header.PCON |= BIT8;            // Set External Gfx Adapter field is valid
  mIgdOpRegion.OpRegion->Header.PCON &= (UINT32)(~BIT7); // Assume No External Gfx Adapter

  ///
  /// Get all PCI IO protocols handles
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleCount; Index++) {
      ///
      /// Get the PCI IO Protocol Interface corresponding to each handle
      ///
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **)&PciIo
                      );

      if (!EFI_ERROR (Status)) {
        ///
        /// Read the PCI configuration space
        ///
        Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              0,
                              sizeof (Pci) / sizeof (UINT32),
                              &Pci
                              );

        ///
        /// Find the display controllers devices
        ///
        if (!EFI_ERROR (Status) && IS_PCI_DISPLAY (&Pci)) {
          Status = PciIo->GetLocation (
                            PciIo,
                            &Segment,
                            &Bus,
                            &Device,
                            &Function
                            );

          //
          // Assumption: Onboard devices will be sits on Bus no 0, while external devices will be sits on Bus no > 0
          //
          if (!EFI_ERROR (Status) && (Bus > 0)) {
            // External Gfx Adapter Detected and Available
            DEBUG ((DEBUG_INFO, "PCON - External Gfx Adapter Detected and Available\n"));
            mIgdOpRegion.OpRegion->Header.PCON |= BIT7;
            break;
          }
        }
      }
    }
  }

  ///
  /// Free any allocated buffers
  ///
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  DEBUG ((DEBUG_INFO, "UpdateIgdOpRegion End\n"));

  ///
  /// Return final status
  ///
  return Status;
}