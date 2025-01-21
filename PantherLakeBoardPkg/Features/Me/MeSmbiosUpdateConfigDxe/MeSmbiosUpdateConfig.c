/** @file
  This is the driver which installs protocol for updating ME SMBIOS tables.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <IndustryStandard/Pci30.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GbeLib.h>
#include <Library/HobLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Protocol/MeSmbiosUpdateProtocol.h>
#include <Protocol/OneClickRecoveryProtocol.h>
#include <Protocol/MebxConfigProtocol.h>
#include <Protocol/PciIo.h>
#include <Register/PchRegs.h>
#include <Register/Msr.h>
#include <Register/Cpuid.h>
#include <MeBiosPayloadHob.h>
#include <Uefi.h>
#include <MebxData.h>
#include <MeSmbios.h>
#include <SetupVariable.h>
#include <OemSetup.h>
#include <PsParameter.h>

EFI_STATUS
EFIAPI
UpdateSmbios130 (
  VOID
  );

EFI_STATUS
EFIAPI
UpdateSmbios131 (
  VOID
  );

ME_SMBIOS_TABLES_UPDATE_PROTOCOL  mMeSmbiosTablesUpdate = {
  UpdateSmbios130,
  UpdateSmbios131,
};

/**
  Returns the SMBIOS table with the specified type.

  @param[in]        Type            The type of the SMBIOS table which is searched.
  @param[out]       Record          If the functions finishes with success, then it will point the found SMBIOS Record.

  @retval EFI_SUCCESS               Record was found.
  @retval EFI_INVALID_PARAMETER     Table pointer is NULL.
  @retval EFI_NOT_FOUND             Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN     SMBIOS_TYPE              Type,
     OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  EFI_SMBIOS_PROTOCOL        *Smbios;
  EFI_SMBIOS_TABLE_HEADER    *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  This function updates SMBIOS type 130 OemCapabilities1 field.

  @param[out] SmbiosTableType130Data   SMBIOS type 130 data
  @param[in]  MbpHob                   ME BIOS Payload Hob
**/
VOID
UpdateManageabilityData (
  OUT SMBIOS_TABLE_TYPE130 *SmbiosTableType130Data,
  IN  ME_BIOS_PAYLOAD_HOB  *MbpHob
  )
{
  EFI_STATUS               Status;
  UINTN                    MebxDataSize;
  MEBX_DATA                MebxData;
  MEBX_CONFIG_PROTOCOL     *MebxConfigProtocol;

  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: No MBP HOB available\n", __FUNCTION__));
    return;
  }

  ZeroMem (&MebxData, sizeof (MEBX_DATA));
  Status = gBS->LocateProtocol (&gMebxConfigProtocolGuid, NULL, (VOID**) &MebxConfigProtocol);
  if (!EFI_ERROR (Status)) {
    MebxDataSize = sizeof (MEBX_DATA);
    Status = MebxConfigProtocol->GetMebxConfig (&MebxDataSize, (VOID*) &MebxData);
    if (EFI_ERROR (Status)) {
      ZeroMem (&MebxData, sizeof (MEBX_DATA));
    }
  }

  SmbiosTableType130Data->AmtSupported = (UINT8) MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt;

  if (SmbiosTableType130Data->AmtSupported) {
    SmbiosTableType130Data->AmtEnabled = (UINT8) MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.Amt;
  }

  if (SmbiosTableType130Data->AmtSupported) {
    //
    // if vPro or ISM supported, LAN is always enabled and SRoU/Sol depend on enabled/disabled state in MEBX
    //
    SmbiosTableType130Data->NetworkEnabled = 1;
    SmbiosTableType130Data->SrouEnabled    = MebxData.AmtSrou;
    SmbiosTableType130Data->SolEnabled     = MebxData.AmtSol;
  }

  if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.KVM) {
    SmbiosTableType130Data->KvmEnabled = (UINT8) MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.KVM;
  }
}

/**
  This function updates SMBIOS type 130 OemCapabilities1 field.

  @param[out] OemCapabilities1  OemCapabilities1
  @param[in]  MeSetup           ME Setup Configuration
**/
VOID
UpdateOemCapabilities1 (
  OUT UINT8    *OemCapabilities1,
  IN  ME_SETUP MeSetup
  )
{
  *OemCapabilities1 |= (((!!MeSetup.BiosReflash) << OEM_CAPS_1_BIOS_REFLASH_BIT_OFFSET) |
                        ((!!MeSetup.BiosSetup)   << OEM_CAPS_1_BIOS_SETUP_BIT_OFFSET  ) |
                        ((!!MeSetup.BiosPause)   << OEM_CAPS_1_BIOS_PAUSE_BIT_OFFSET  )
                       );
}

/**
  This function updates SMBIOS type 130 OemCapabilities2 field.

  @param[out] OemCapabilities2  OemCapabilities2
**/
VOID
UpdateOemCapabilities2 (
  OUT UINT8    *OemCapabilities2
  )
{
  switch (PcdGet8 (PcdSolTerminalType)) {
    case OEM_CAPS_2_TERMINAL_EMULATION_PCANSI:
      *OemCapabilities2 |= OEM_CAPS_2_TERMINAL_EMULATION_PCANSI;
      break;

    case OEM_CAPS_2_TERMINAL_EMULATION_VTUTF8:
    default:
      //
      // Set recommended type
      //
      *OemCapabilities2 |= OEM_CAPS_2_TERMINAL_EMULATION_VTUTF8;
      break;
  }
}

/**
  This function updates SMBIOS type 130 OemCapabilities3 field.

  @param[out] OemCapabilities3  OemCapabilities3
  @param[in]  MeSetup           ME Setup Configuration
  @param[in]  MbpHob            ME BIOS Payload Hob
**/
VOID
UpdateOemCapabilities3 (
  OUT UINT8                *OemCapabilities3,
  IN  ME_SETUP             MeSetup,
  IN  ME_BIOS_PAYLOAD_HOB  *MbpHob
  )
{
  *OemCapabilities3 |= (!!MeSetup.SecureBootExposureToFw) << OEM_CAPS_3_BIOS_SECURE_BOOT_BIT_OFFSET;
}

/**
  This function updates SMBIOS type 130 OemCapabilities4 field.

  @param[out] OemCapabilities4  OemCapabilities4
  @param[in]  MeSetup           ME Setup Configuration
  @param[in]  MbpHob            ME BIOS Payload Hob
**/
VOID
UpdateOemCapabilities4 (
  OUT UINT8                *OemCapabilities4,
  IN  ME_SETUP             MeSetup,
  IN  ME_BIOS_PAYLOAD_HOB  *MbpHob
  )
{
  EFI_STATUS                  Status;
  ONE_CLICK_RECOVERY_PROTOCOL *OneClickRecovery;
  ONE_CLICK_RECOVERY_CAP      OcrCap;

  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: No MBP HOB available\n", __FUNCTION__));
    return;
  }

  //
  // Supported if FW capability is enabled for TBT Dock
  //
  if (!!MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.TbtDock) {
    *OemCapabilities4 |= (!!MeSetup.vProTbtDock) << OEM_CAPS_4_VPRO_TBT_DOCK_BIT_OFFSET;
  }

  //
  // Supported in full manageability
  //
  Status = gBS->LocateProtocol (&gOneClickRecoveryProtocolGuid, NULL, (VOID **) &OneClickRecovery);
  if (EFI_ERROR (Status)) {
    return;
  }

  OcrCap = OneClickRecovery->OcrCap ();

  *OemCapabilities4 |= (((!!OcrCap.Bits.OcrBootHttps)     << OEM_CAPS_4_OCR_HTTPS_BOOT_BIT_OFFSET      ) |
                        ((!!OcrCap.Bits.OcrBootPba)       << OEM_CAPS_4_OCR_PBA_BOOT_BIT_OFFSET        ) |
                        ((!!OcrCap.Bits.OcrBootWinRe)     << OEM_CAPS_4_OCR_WINRE_BOOT_BIT_OFFSET      ) |
                        ((!!OcrCap.Bits.OcrAmtDisSecBoot) << OEM_CAPS_4_OCR_AMT_DIS_SEC_BOOT_BIT_OFFSET) |
                        ((!!OcrCap.Bits.OcrWifiProfile)   << OEM_CAPS_4_OCR_WIFI_PROFILE_BIT_OFFSET    )
                        );
}

/**
  Updates SMBIOS 130 table with Remote Platform Erase Capabilities

  @param[out] RpeCapabilities  RpeCapabilities
  @param[in]  MbpHob           ME BIOS Payload Hob
**/
VOID
UpdateRpeCapabilities (
  OUT UINT32               *RpeCapabilities,
  IN  ME_BIOS_PAYLOAD_HOB  *MbpHob
  )
{
  #if FixedPcdGetBool (PcdPlatformSanitizeRpeSupport) == 1
    if ((MbpHob != NULL) && (MbpHob->MeBiosPayload.FwCapsSku.Available == TRUE)) {
      if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.RPE == 1) {
        *RpeCapabilities |= FixedPcdGet32 (PcdPsSupportCapabilities) & PS_CAPABILITY_MASK;
      }
    }
  #endif
}

/**
  This function makes boot time changes to the content of the SMBIOS type 130.

  @retval EFI_SUCCESS           Record was updated successfully.
  @retval EFI_NOT_FOUND         HECI protocol or SMBIOS type 130 record cannot be found.
**/
EFI_STATUS
EFIAPI
UpdateSmbios130 (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_SMBIOS_TABLE_HEADER     *Record;
  ME_SETUP                    MeSetup;
  UINTN                       VariableSize;
  SMBIOS_TABLE_TYPE130        *SmbiosTableType130Data;
  ME_BIOS_PAYLOAD_HOB         *MbpHob;

  DEBUG ((DEBUG_INFO, "%a() entry \n", __FUNCTION__));

  Status = GetSmbiosTable (MESMBIOS_TYPE_130, &Record);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  SmbiosTableType130Data = (SMBIOS_TABLE_TYPE130*) Record;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  VariableSize = sizeof (ME_SETUP);
  Status = gRT->GetVariable (
                  L"MeSetup",
                  &gMeSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &MeSetup
                  );
  if (EFI_ERROR (Status)) {
    ZeroMem (&MeSetup, sizeof (ME_SETUP));
  }

  UpdateManageabilityData (SmbiosTableType130Data, MbpHob);
  UpdateOemCapabilities1 (&SmbiosTableType130Data->OemCapabilities1, MeSetup);
  UpdateOemCapabilities2 (&SmbiosTableType130Data->OemCapabilities2);
  UpdateOemCapabilities3 (&SmbiosTableType130Data->OemCapabilities3, MeSetup, MbpHob);
  UpdateOemCapabilities4 (&SmbiosTableType130Data->OemCapabilities4, MeSetup, MbpHob);
  UpdateRpeCapabilities (&SmbiosTableType130Data->RpeCapabilities, MbpHob);

  return EFI_SUCCESS;
}

/**
  Updates CPU capabilities.

  @param[out] CpuCapabilities       Cpu Capabilities
**/
VOID
UpdateCpuCapabilities (
  OUT CPU_CAP *CpuCapabilities
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER Ia32FeatureControlMsr;
  CPUID_VERSION_INFO_ECX            Ecx;

  Ia32FeatureControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  CpuCapabilities->VMXState     = Ia32FeatureControlMsr.Bits.EnableVmxOutsideSmx;
  CpuCapabilities->VTxEnabled   = CpuCapabilities->VMXState;
  CpuCapabilities->SMXState     = Ia32FeatureControlMsr.Bits.EnableVmxInsideSmx;
  CpuCapabilities->LtTxtEnabled = CpuCapabilities->SMXState;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  CpuCapabilities->LtTxtCap     = Ecx.Bits.SMX;
  CpuCapabilities->VTxCap       = Ecx.Bits.VMX;
}

/**
  Updates PCH capabilities.

  @param[out] PchCapabilities       PCH Capabilities
**/
VOID
UpdatePchCapabilities (
  OUT PCH_CAP *PchCapabilities
  )
{
  PchCapabilities->FunctionNumber = LpcFuncNumber ();
  PchCapabilities->DeviceNumber   = LpcDevNumber ();
  PchCapabilities->BusNumber      = DEFAULT_PCI_BUS_NUMBER_PCH;
  PchCapabilities->DeviceID       = PciSegmentRead16 (LpcPciCfgBase () + PCI_DEVICE_ID_OFFSET);
}

/**
  Updates ME platform capabilites from the MBP.

  @param[out] MeCapabilities       ME Capabilities
**/
VOID
UpdateMeCapabilities (
  OUT ME_CAP *MeCapabilities
  )
{
  ME_BIOS_PAYLOAD_HOB   *MbpHob;

  MbpHob = NULL;
  //
  // Get Mbp Data HOB
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "HeciGetMeFwInfo: No MBP Data Protocol available\n"));
    return;
  }

  MeCapabilities->MeEnabled       = 1;
  MeCapabilities->IntelAmtEnabled = MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.FullMng;
  MeCapabilities->IntelKVM        = MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.KVM;
  MeCapabilities->IntelKVMEnabled = MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.KVM;

  switch (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand) {
    case IntelAmtBrand:
      MeCapabilities->IntelAmtFw        = 1;
      MeCapabilities->LocalWakeupTimer  = 1;
      break;

    case IntelStandardManageabilityBrand:
      MeCapabilities->IntelAmtFwStandard = 1;
      break;
  }

  MeCapabilities->MeMajorVer  = MbpHob->MeBiosPayload.FwVersionName.MajorVersion;
  MeCapabilities->MeMinorVer  = MbpHob->MeBiosPayload.FwVersionName.MinorVersion;
  MeCapabilities->MeBuildNo   = MbpHob->MeBiosPayload.FwVersionName.BuildVersion;
  MeCapabilities->MeHotFixNo  = MbpHob->MeBiosPayload.FwVersionName.HotfixVersion;
}

/**
  Updates Network device.

  @param[out] NetworkDevice       Network Device
**/
VOID
UpdateNetworkDevice (
  OUT NETWORK_DEV *NetworkDevice
  )
{
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         PciSegment;
  UINTN                         PciBusNum;
  UINTN                         PciDevNum;
  UINTN                         PciFuncNum;
  EFI_STATUS                    Status;
  PCI_ROOT_BRIDGE*              PciRootBridge;
  UINTN                         PciRootBridgeCount;
  UINTN                         PciRootBridgeIndex;
  UINT8                         BaseCode;
  UINT8                         SubClassCode;
  UINT16                        PciDeviceID;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );

  if (EFI_ERROR (Status) || (PciIoHandleBuffer == NULL) || (PciIoHandleCount == 0)) {
    return;
  }

  PciRootBridge = PciHostBridgeGetRootBridges (&PciRootBridgeCount);
  if ((PciRootBridge == NULL) || (PciRootBridgeCount == 0)) {
    gBS->FreePool (PciIoHandleBuffer);
    return;
  }

  for (Index = 0; Index < PciIoHandleCount; Index++) {
    Status = gBS->HandleProtocol (PciIoHandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
    ASSERT_EFI_ERROR (Status);
    Status = PciIo->GetLocation (PciIo, &PciSegment, &PciBusNum, &PciDevNum, &PciFuncNum);
    if (!EFI_ERROR (Status)) {
      for (PciRootBridgeIndex = 0; PciRootBridgeIndex < PciRootBridgeCount; PciRootBridgeIndex++) {
        if (PciBusNum == PciRootBridge[PciRootBridgeIndex].Bus.Base) {
          Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, PCI_CLASSCODE_OFFSET + 1, 1, &SubClassCode);
          if (EFI_ERROR (Status)) {
            continue;
          }
          Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, PCI_CLASSCODE_OFFSET + 2, 1, &BaseCode);
          if (EFI_ERROR (Status)) {
            continue;
          }
          if ((BaseCode == PCI_CLASS_NETWORK) && (SubClassCode == PCI_CLASS_NETWORK_ETHERNET)) {
            NetworkDevice->FunctionNumber   = (UINT32) PciFuncNum;             // [2:0] PCI Device Function Number of Wired LAN
            NetworkDevice->DeviceNumber     = (UINT32) PciDevNum;              // [7:3] PCI Device Device Number of Wired LAN
            NetworkDevice->BusNumber        = (UINT32) PciBusNum;              // [15:8] PCI Device Bus Number of Wired LAN
            Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &PciDeviceID);
            if (!EFI_ERROR (Status)) {
              NetworkDevice->DeviceID       = PciDeviceID;
            }
            DEBUG ((DEBUG_INFO, "Integrated Gbe controller found on [S:B:D:F] = [%x:%x:%x:%x]\n", PciSegment, PciBusNum, PciDevNum, PciFuncNum));
            PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
            gBS->FreePool (PciIoHandleBuffer);
            return;
          }
        }
      }
    }
  }

  PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
  gBS->FreePool (PciIoHandleBuffer);
  return;
}

/**
  Updates BIOS capabilities.

  @param[out] BiosCapabilities       BIOS Capabilities
**/
VOID
UpdateBiosCapabilities (
  OUT BIOS_CAP *BiosCapabilities
  )
{
  EFI_STATUS                        Status;
  SETUP_CPU_FEATURES                SetupCpuFeatures;
  SETUP_VOLATILE_DATA               SetupVolatileData;
  UINTN                             VariableSize;

  VariableSize = sizeof (SETUP_CPU_FEATURES);
  Status = gRT->GetVariable (
                  L"SetupCpuFeatures",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupCpuFeatures
                  );

  if (!EFI_ERROR (Status)) {
    BiosCapabilities->VTxSupport = !!SetupCpuFeatures.VTAvailable;
    BiosCapabilities->TxtSupport = !!SetupCpuFeatures.TXTAvailable;
  }

  VariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupVolatileData
                  );
  if (!EFI_ERROR (Status)) {
    BiosCapabilities->VTdSupport = !!SetupVolatileData.VTdAvailable;
  }
}

/**
  This function makes boot time changes to the content of the SMBIOS type 131.

  @retval EFI_SUCCESS           Record was updated successfully.
  @retval EFI_NOT_FOUND         HECI protocol or SMBIOS type 131 record cannot be found.
**/
EFI_STATUS
EFIAPI
UpdateSmbios131 (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_SMBIOS_TABLE_HEADER           *Record;
  SMBIOS_TABLE_TYPE131              *SmbiosTableType131Data;

  DEBUG ((DEBUG_INFO, "%a() entry \n", __FUNCTION__));

  Status = GetSmbiosTable (MESMBIOS_TYPE_131, &Record);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  SmbiosTableType131Data = (SMBIOS_TABLE_TYPE131*) Record;

  UpdateCpuCapabilities  (&SmbiosTableType131Data->CpuCapabilities);
  UpdatePchCapabilities  (&SmbiosTableType131Data->PchCapabilities);
  UpdateMeCapabilities   (&SmbiosTableType131Data->MeCapabilities);
  UpdateNetworkDevice    (&SmbiosTableType131Data->NetworkDevice);
  UpdateBiosCapabilities (&SmbiosTableType131Data->BiosCapabilities);

  return EFI_SUCCESS;
}

/**
  This is the standard EFI driver entrypoint to install protocol to update ME SMBIOS tables from silicon package.

  @param[in] ImageHandle         Handle for the image of this driver
  @param[in] SystemTable         Pointer to the EFI System Table

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle or Protocol is NULL.
**/
EFI_STATUS
EFIAPI
MeSmbiosUpdateConfigEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS        Status;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gMeSmbiosTablesUpdateProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mMeSmbiosTablesUpdate
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
