/** @file
  DXE Library for Initializing IGpu ACPI table initialization.

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

#include <Library/IGpuDxeInitLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Protocol/IGpuNvsArea.h>
#include <Protocol/IGpuPolicy.h>
#include <Library/AslUpdateLib.h>
#include <Library/IGpuInfoLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED IGPU_NVS_AREA_PROTOCOL  mIGpuNvsAreaProtocol;

/**
  Update Igd NVS Area
  @param[in] IGpuPolicy -        IGPU DXE Policy protocol
**/
VOID
IGpuUpdateNvsArea (
  IN IGPU_POLICY_PROTOCOL  *IGpuPolicy
  )
{
  EFI_STATUS              Status;
  IGPU_DXE_CONFIG         *IGpuDxeConfig;
  IGPU_NVS_AREA_PROTOCOL  *IGpuNvsAreaProtocol;
  BOOLEAN                 IgdDevenStatus;

  Status = EFI_SUCCESS;

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Update IGD SA Global NVS
  ///
  DEBUG ((DEBUG_INFO, " Update Igd NVS Area.\n"));
  ///
  ///  Locate the IGPU NVS Protocol.
  ///
  Status = gBS->LocateProtocol (&gIGpuNvsAreaProtocolGuid, NULL, (VOID **)&IGpuNvsAreaProtocol);

  if (Status == EFI_SUCCESS) {
    IGpuNvsAreaProtocol->Area->AlsEnable = IGpuDxeConfig->AlsEnable;
    ///
    /// Initialize IGD state by checking if IGD Device 2 Function 0 is enabled in the chipset
    ///
    IgdDevenStatus = UncoreDevEnRead (Graphics, 0);
    if (IgdDevenStatus == TRUE) {
      IGpuNvsAreaProtocol->Area->IgdState = 1;
    } else {
      IGpuNvsAreaProtocol->Area->IgdState = 0;
    }

    IGpuNvsAreaProtocol->Area->BrightnessPercentage = 100;
    IGpuNvsAreaProtocol->Area->EdpValid             = 0;
  } else {
    DEBUG ((DEBUG_INFO, " Failed to locate IGpuNvsAreaProtocol\n"));
  }
}

/**
  Initialize IGPU Nvs Area operation region.
  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
IGpuPatchNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT32                  Address;
  UINT16                  Length;
  IGPU_NVS_AREA_PROTOCOL  *IGpuNvsAreaProtocol;

  ///
  ///  Locate the IGPU NVS Protocol.
  ///
  Status = gBS->LocateProtocol (&gIGpuNvsAreaProtocolGuid, NULL, (VOID **)&IGpuNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);

  Address = (UINT32)(UINTN)IGpuNvsAreaProtocol->Area;
  Length  = (UINT16)sizeof (IGPU_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch IGPU NvsAreaAddress: IGPU NVS Address %x Length %x\n", Address, Length));

  Status = UpdateNameAslCode (SIGNATURE_32 ('I', 'G', 'N', 'B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status = UpdateNameAslCode (SIGNATURE_32 ('I', 'G', 'N', 'L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install IGPU NVS protocol
**/
VOID
IGpuInstallNvsProtocol (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Install IGPU NVS protocol\n"));
  Status = (gBS->AllocatePool)(EfiACPIMemoryNVS, sizeof (IGPU_NVS_AREA), (VOID **)&mIGpuNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *)mIGpuNvsAreaProtocol.Area, sizeof (IGPU_NVS_AREA));
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gIGpuNvsAreaProtocolGuid,
                  &mIGpuNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Graphics initialization on PCIe Enum complete like GopDriver version init etc.

  @retval EFI_SUCCESS          Either IGD is not present or function executed successfully.
**/
EFI_STATUS
IGpuInitOnPciEnumComplete (
  VOID
  )
{
  CHAR16                        *DriverVersion;
  UINTN                         Index;
  EFI_STATUS                    Status;
  IGPU_DXE_CONFIG               *IGpuDxeConfig;
  IGPU_POLICY_PROTOCOL          *IGpuPolicy;
  GOP_COMPONENT_NAME2_PROTOCOL  *GopComponentName2Protocol;

  GopComponentName2Protocol = NULL;

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (!IGpuIsSupported ()) {
    DEBUG ((DEBUG_INFO, "Returning from GraphicsInitOnPciEnumComplete() since IGD is not supported or not enabled\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Get the platform setup policy.
  ///
  DriverVersion = NULL;
  Status        = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **)&IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gGopComponentName2ProtocolGuid, NULL, (VOID **)&GopComponentName2Protocol);
  if (!EFI_ERROR (Status)) {
    Status = GopComponentName2Protocol->GetDriverVersion (
                                          GopComponentName2Protocol,
                                          "en-US",
                                          &DriverVersion
                                          );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; (DriverVersion[Index] != '\0'); Index++) {
      }

      Index = (Index+1)*2;
      CopyMem (IGpuDxeConfig->GopVersion, DriverVersion, Index);
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize GT ACPI tables

  @param[in] IGpuPolicy -        IGPU DXE Policy protocol

  @retval EFI_SUCCESS          - GT ACPI initialization complete
**/
EFI_STATUS
IGpuDxeInit (
  IN IGPU_POLICY_PROTOCOL  *IGpuPolicy
  )
{
  ///
  ///  Update Igd Nvs Area
  ///
  IGpuUpdateNvsArea (IGpuPolicy);

  return EFI_SUCCESS;
}

/**
  Save IGPU BAR for S3

**/
VOID
IGpuSaveBarForS3 (
  VOID
  )
{
  UINT64  McD2BaseAddress;
  UINT32  Data32;

  McD2BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, 0);
  Data32          = (PciSegmentRead32 (McD2BaseAddress + IGpuGetGttMmAdrOffset ())) & 0xFFFFFFF0;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              IGpuGetGttMmAdrOffset ()
                                              ),
    1,
    &Data32
    );

  Data32 = PciSegmentRead32 (McD2BaseAddress + IGpuGetGttMmAdrOffset () + 4);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              IGpuGetGttMmAdrOffset () + 4
                                              ),
    1,
    &Data32
    );

  Data32 = PciSegmentRead32 (McD2BaseAddress + IGpuGetLMemBarOffset ());
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              IGpuGetLMemBarOffset ()
                                              ),
    1,
    &Data32
    );

  Data32 = PciSegmentRead32 (McD2BaseAddress + IGpuGetLMemBarOffset () + 4);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              IGpuGetLMemBarOffset () + 4
                                              ),
    1,
    &Data32
    );

  Data32 = PciSegmentRead32 (McD2BaseAddress + R_SA_IGD_IOBAR);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              R_SA_IGD_IOBAR
                                              ),
    1,
    &Data32
    );

  Data32 = PciSegmentRead32 (McD2BaseAddress + PCI_COMMAND_OFFSET);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              IGD_BUS_NUM,
                                              IGD_DEV_NUM,
                                              IGD_FUN_NUM,
                                              PCI_COMMAND_OFFSET
                                              ),
    1,
    &Data32
    );
}
