/** @file
  Source code file for the Silicon Init DXE module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <SiInitDxe.h>
#include <Library/IpuLib.h>
#include <ImrConfigHob.h>
#include <PchConfigHobCommon.h>

//
// Module-wide global variables
//
VOID       *mRegistration;
EFI_EVENT  EndOfDxeEvent;

/**
  This function gets registered as a callback to perform

  @param[in]  Event     - A pointer to the Event that triggered the callback.
  @param[in]  Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SiInitDxeSmbiosCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  EFI_SMBIOS_PROTOCOL *SmbiosProtocol;

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &SmbiosProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Error locating gEfiSmbiosProtocolGuid at SiInitDxe. Status = %r\n", Status));
    return;
  }

  Status = SmbiosMemory (SmbiosProtocol);
  ASSERT_EFI_ERROR (Status);

  return;
}


/**
  This function handles PlatformInit task at the end of DXE

  @param[in]  Event
  @param[in]  *Context
**/
VOID
EFIAPI
SiInitEndOfDxe (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                 Status;
  DXE_SI_POLICY_PROTOCOL     *SiPolicyData;
  EFI_PEI_HOB_POINTERS       HobPtr;
  PCH_GENERAL_CONFIG_HOB     *GeneralConfigHob;

  Status = gBS->LocateProtocol (&gDxeSiPolicyProtocolGuid, NULL, (VOID **) &SiPolicyData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO,"Failed to locate DxeSiPolicyProtocolGuid Protocol\n"));
    return;
  }

  //
  // Update Smbios FVI OEM Type
  //
  HobPtr.Guid = GetFirstGuidHob (&gPchGeneralConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  GeneralConfigHob = (PCH_GENERAL_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
  Status = UpdateFviInfo (
             SiPolicyData->SmbiosOemTypeFirmwareVersionInfo,
             GeneralConfigHob->Crid
           );
  ASSERT_EFI_ERROR (Status);
  gBS->CloseEvent (Event);

#if FixedPcdGetBool(PcdTseEnable) == 1
  Status = PatchTseNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);
#endif // FixedPcdGetBool(PcdTseEnable) == 1
#if FixedPcdGetBool(PcdTcssSupport) == 1
  Status = PatchTcssNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);
#endif
  Status = PatchIpuNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);

  Status = IGpuPatchNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);

  Status = PatchHostBridgeNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  Status = PatchVmdNvsAreaAddress ();
  ASSERT_EFI_ERROR (Status);
#endif

  return;
}

/**
  This function handles PlatformInit task on Pci Enumeration completion

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SiInitOnPciEnumComplete (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;

  DEBUG ((DEBUG_INFO, "SiInitOnPciEnumComplete Start\n"));
  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  IGpuSaveBarForS3 ();
  SaveIpuBarForS3 ();

  UpdateHostBridgeNvs ();

  Status = IGpuInitOnPciEnumComplete ();
  ASSERT_EFI_ERROR (Status);

  Status = IGpuUpdateOpRegion ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInitOnPciEnumComplete End\n"));
}

/**
  Entry point for the driver.

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
SiInitDxe (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS           Status;
  EFI_EVENT            CallbackEvent;
  EFI_SMBIOS_PROTOCOL  *Smbios;
  VOID                 *Registration;
  IGPU_POLICY_PROTOCOL *IGpuPolicy;
  IMR_CONFIG_HOB       *ImrConfigHob;
  VOID                 *HobPtr;
#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  ClearVmdTempBars ();
#endif

  ///
  /// Get the platform setup policy.
  ///
  Status = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **) &IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install Memory Address Encode Decode protocol
  ///
  InstallMemoryAddressDecodeEncodeProtocol (ImageHandle);

  ///
  /// Install TSE NVS protocol
  ///
  InstallTseNvsProtocol (ImageHandle);
#if FixedPcdGetBool(PcdTcssSupport) == 1
  ///
  /// Install TCSS NVS protocol
  ///
  InstallTcssNvsProtocol (ImageHandle);
#endif
  ///
  /// Install IPU NVS protocol
  ///
  InstallIpuNvsProtocol (ImageHandle);
  ///
  /// Install IGPU NVS protocol
  ///
  IGpuInstallNvsProtocol (ImageHandle);
  ///
  /// Install Host Bridge Global NVS protocol
  ///
  InstallHostBridgeNvsProtocol (ImageHandle);

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  ///
  /// Install VMD Global NVS protocol
  ///
  InstallVmdNvsProtocol (ImageHandle);
  ///
  /// Vmd Initialization
  ///
  VmdNvsInit ();
#endif

  ///
  /// GtPostInit Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing GT ACPI tables\n"));
  IGpuDxeInit (IGpuPolicy);
  ///
  /// Vtd Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing VT-d ACPI tables\n"));
  VtdInit ();

  ///
  /// IgdOpRegion Install Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing IGD OpRegion\n"));
  IGpuOpRegionInit ();

  ///
  /// Install Intel Graphics SSDT
  ///
  Status = InstallSsdtAcpiTable (gGraphicsAcpiTableStorageGuid, SIGNATURE_64 ('I','g','f','x','S','s','d','t'));
  ASSERT_EFI_ERROR (Status);

  //
  // TSE Initialization
  //
  #if FixedPcdGetBool(PcdTseEnable) == 1
    Status = InstallSsdtAcpiTable (gTseAcpiTableStorageGuid, SIGNATURE_64 ('T','s','e','S','s','d','t',0));
    ASSERT_EFI_ERROR (Status);
    InitializeTseAcpi (ImageHandle);
  #endif // PcdTseEnable

  ///
  /// Tcss Initialization
  ///
#if FixedPcdGetBool(PcdTcssSupport) == 1
  DxeTcssInit ();
#endif
  ///
  /// Install IPU SSDT if IPU is present.
  ///
  if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetIpuBusNumber (), GetIpuDevNumber (), GetIpuFuncNumber (), 0)) != V_SA_DEVICE_ID_INVALID) {
    Status = InstallSsdtAcpiTable (gIpuAcpiTableStorageGuid, SIGNATURE_64 ('I','p','u','S','s','d','t',0));
    ASSERT_EFI_ERROR (Status);
    ///
    /// Update IPU NVS AREA tables
    ///
    UpdateIpuNvs();
  }

  DEBUG ((DEBUG_INFO, "Initializing Hybrid Graphics (Dxe)\n"));
  DxeHybridGraphicsInit();

  HobPtr = GetFirstGuidHob (&gImrConfigGuid);
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "No IMR_CONFIG HOB\n"));
    return EFI_INVALID_PARAMETER;
  }
  ImrConfigHob = (IMR_CONFIG_HOB*) GET_GUID_HOB_DATA (HobPtr);

  if (ImrConfigHob->ResizableBarSupport == TRUE) {
    PcdSetBoolS (PcdPcieResizableBarSupport, TRUE);
  }
  DEBUG ((DEBUG_INFO, "PcdPcieResizableBarSupport = %x\n", PcdGetBool (PcdPcieResizableBarSupport)));

  ///
  /// Performing SiInitOnPciEnumComplete after the gEfiPciEnumerationCompleteProtocolGuid is signaled
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    SiInitOnPciEnumComplete,
    NULL,
    &Registration
    );

  //
  // If EfiSmbiosProtocol already installed, invoke the callback directly.
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (!EFI_ERROR (Status)) {
    SiInitDxeSmbiosCallback (NULL,NULL);
  } else {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SiInitDxeSmbiosCallback,
                    NULL,
                    &CallbackEvent
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->RegisterProtocolNotify (
                    &gEfiSmbiosProtocolGuid,
                    CallbackEvent,
                    &mRegistration
                    );
    ASSERT_EFI_ERROR (Status);

  }

  ///
  /// Performing SiInitEndOfDxe after the gEfiEndOfDxeEventGroup is signaled.
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SiInitEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);


  IncompatiblePciDeviceSupport ();
  ASSERT_EFI_ERROR (Status);

  TopSwapControlSupport ();
  ASSERT_EFI_ERROR (Status);

  return Status;
}
