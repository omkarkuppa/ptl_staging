/** @file
  Do Platform Stage System Agent initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "PeiSaPolicyUpdate.h"
#include <Guid/MemoryTypeInformation.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiGetFvInfoLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/PlatformUsbConfigLib.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/GraphicsOutput.h>

#include <IndustryStandard/Bmp.h>
#include <Platform.h>
#include <PolicyUpdateMacro.h>
#if FixedPcdGetBool(PcdVmdEnable) == 1
#include <VmdPeiConfig.h>
#endif
#include <TcssPeiConfig.h>
#include <TcssPeiPreMemConfig.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <Ppi/IGpuPlatformPolicyPpi.h>
#include <FspsUpd.h>
#include <FspmUpd.h>
#endif
#include <TelemetryPeiConfig.h>

#include <PlatformBoardConfig.h>
#include <SiliconPolicyHob.h>
#include <Library/PeiLib.h>
#include <Library/PchInfoLib.h>

#if FixedPcdGet8(PcdFspModeSelection) == 1
EFI_STATUS
EFIAPI
PeiGraphicsPlatformPolicyNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;
  VOID                              *FspsUpd;
  PEI_IGPU_PLATFORM_POLICY_PPI      *IGpuPlatformPolicyPpi;
  LID_STATUS                        CurrentLidStatus;

  DEBUG ((DEBUG_INFO, "PeiGraphicsPlatformPolicyNotifyCallback Entry\n"));

  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
  ///
  /// Locate IGpuPlatformPolicyPpi
  ///
  Status = PeiServicesLocatePpi (&gPeiGraphicsPlatformPpiGuid, 0, NULL, (VOID *) &IGpuPlatformPolicyPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to locate IGpuPlatformPolicyPpi\n"));
    ASSERT_EFI_ERROR(Status);
  }

  Status = IGpuPlatformPolicyPpi->GetPlatformLidStatus (&CurrentLidStatus);
  ASSERT_EFI_ERROR (Status);
  ((FSPS_UPD *) FspsUpd)->FspsConfig.LidStatus = (UINT8) CurrentLidStatus;
  DEBUG ((DEBUG_INFO, "LidStatus from GetPlatformLidStatus is 0x%x\n", ((FSPS_UPD *) FspsUpd)->FspsConfig.LidStatus));
  DEBUG ((DEBUG_INFO, "PeiGraphicsPlatformPolicyNotifyCallback Exit\n"));

  return Status;
}

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mPeiGfxPlatformPolicyNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiGraphicsPlatformPpiGuid,
  PeiGraphicsPlatformPolicyNotifyCallback
};
#endif

/**
  Tcss Get PchUsb2 TypeA Port Number Mapped with TcssXhci Port Index

  @param[in] UINT8       TcssXhciPortIndex

  @retval UINT8          PchUsb2PortNumber (1-based)
**/
UINT8
EFIAPI
TcssGetPchUsb2PortNoMappedWithTypeATcssXhciPort (
  IN UINT8 TcssXhciPortIndex
  )
{
  USB_CONNECTOR_HOB_DATA      *UsbConnectorHobPtr;
  USB_CONNECTOR_BOARD_CONFIG  *UsbConnectorBoardConfig;
  UINT8                       ConnectorIndex;
  UINT8                       PchUsb2PortNum;

  PchUsb2PortNum          = 0xFF;
  UsbConnectorHobPtr      = GetUsbConnectorHobData ();
  UsbConnectorBoardConfig = UsbConnectorHobPtr->UsbConnectorBoardConfig;

  for (ConnectorIndex = 0; ConnectorIndex < UsbConnectorHobPtr->NumberOfUsbConnectors; ConnectorIndex++) {
    if (UsbConnectorBoardConfig[ConnectorIndex].Usb3Controller == TCSS_USB3 &&
        UsbConnectorBoardConfig[ConnectorIndex].Usb3PortNum == TcssXhciPortIndex) {
      if (UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable == CONNECTABLE &&
          UsbConnectorBoardConfig[ConnectorIndex].Usb2Controller == PCH_USB2 &&
          UsbConnectorBoardConfig[ConnectorIndex].ConnectorType == USB_TYPE_A) {
        PchUsb2PortNum = (UINT8) UsbConnectorBoardConfig[ConnectorIndex].Usb2PortNum + 1;
        DEBUG((DEBUG_INFO, "Found PchUsb2 Port#%d mapped with USBA TcssUsb3 Port%d.\n", PchUsb2PortNum - 1, TcssXhciPortIndex));
      } else {
        break;
      }
    }
  }
  return PchUsb2PortNum;
}

/**
  Tcss Get Type-C Port orientation setting.

  @param[in] UINT8       TcssXhciPortIndex

  @retval UINT8          Orientation flip setting.
**/
UINT8
EFIAPI
TcssGetPortOrientationSetting (
  IN UINT8 TcssXhciPortIndex
  )
{
  TCSS_COV_USBA_BOARD_CONFIG *TcssCovUsbA;
  TcssCovUsbA = PcdGetPtr (VpdPcdTcssCovUsbA);

  switch (TcssXhciPortIndex) {
    case 0:
      // Get TypeC Port 1 Orientation flip setting
      return (UINT8) TcssCovUsbA->Port1Orientation;
      break;
    case 1:
      // Get TypeC Port 2 Orientation flip setting
      return (UINT8) TcssCovUsbA->Port2Orientation;
      break;
    case 2:
      // Get TypeC Port 3 Orientation flip setting
      return (UINT8) TcssCovUsbA->Port3Orientation;
      break;
    case 3:
      // Get TypeC Port 4 Orientation flip setting
      return (UINT8) TcssCovUsbA->Port4Orientation;
      break;
    default:
      break;
  }
  return 0;
}

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance
  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_GUID                        BmpImageGuid;
  EFI_STATUS                      Status;
  EFI_STATUS                      Status2;
  SETUP_DATA                      SetupData;
  EFI_GUID                        FileGuid;
  SA_SETUP                        SaSetup;
  UINTN                           VarSize;
  VOID                            *Buffer;
  UINTN                           Size;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  EFI_BOOT_MODE                   BootMode;
  PCH_SETUP                       PchSetup;
  UINT8                           CapPolicy;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  VOID                            *FspmUpd;
#else
  NPU_PEI_CONFIG                  *NpuPeiConfig;
  HOST_BRIDGE_PEI_CONFIG          *HostBridgePeiConfig;
  TCSS_PEI_CONFIG                 *TcssPeiConfig;
  TELEMETRY_PEI_CONFIG            *TelemetryPeiConfig;
  TCSS_PEI_PREMEM_CONFIG          *TcssPeiPreMemConfig;
  IGPU_PEI_CONFIG                 *IGpuConfig;
#if FixedPcdGetBool(PcdVmdEnable) == 1
  VMD_PEI_CONFIG                  *VmdPeiConfig;
#endif
#endif //FSPMode Check
#if FixedPcdGetBool(PcdVmdEnable) == 1
  VOID                            *VmdVariablePtr;
  UINT8                           VmdDevIndex;
#endif
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *Blt;
  UINTN                           BltSize;
  UINTN                           Height;
  UINTN                           Width;
#if FixedPcdGetBool (PcdTcssSupport) == 1
  UINT8                           Index;
  UINT8                           PchUsb2PortNo;
  TCSS_TYPEC_CONV_USBA            TcssPortConvProperties;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiSaPolicyUpdate Pos-Mem Start\n"));

  Size  = 0;
  Blt   = NULL;
  BltSize = 0;
#if FixedPcdGetBool (PcdTcssSupport) == 1
  PchUsb2PortNo         = 0xFF;
  TcssPortConvProperties.Data8 = 0;
#endif

  Buffer     = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  NpuPeiConfig          = NULL;
  HostBridgePeiConfig   = NULL;
  TcssPeiConfig         = NULL;
  TelemetryPeiConfig    = NULL;
  TcssPeiPreMemConfig   = NULL;
  IGpuConfig            = NULL;
#if FixedPcdGetBool(PcdVmdEnable) == 1
  VmdPeiConfig          = NULL;
#endif

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHostBridgePeiConfigGuid, (VOID *) &HostBridgePeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gNpuPeiConfigGuid, (VOID *) &NpuPeiConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *)SiPolicyPpi, &gTelemetryPeiConfigGuid, (VOID *)&TelemetryPeiConfig);
  ASSERT_EFI_ERROR(Status);

#if FixedPcdGetBool(PcdVmdEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gVmdPeiConfigGuid, (VOID *) &VmdPeiConfig);
  ASSERT_EFI_ERROR(Status);
#endif
#endif //FSPMode Check

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGet8(PcdFspModeSelection) == 1
  DEBUG ((DEBUG_INFO, "Calling PeiGfxPlatformPolicyNotify\n"));
  Status = PeiServicesNotifyPpi (&mPeiGfxPlatformPolicyNotifyList);
#endif

  //
  // Locate system configuration variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchUsbOverCurrentEnable, TcssPeiConfig->UsbConfig.OverCurrentEnable, PchSetup.PchUsbOverCurrentEnable);
  }

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               PLATFORM_SETUP_VARIABLE_NAME,
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SetupData
                               );

  VarSize = sizeof (SA_SETUP);
  Status2 = VariableServices->GetVariable(
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &SaSetup
                                );
  ASSERT_EFI_ERROR(Status2);

#if FixedPcdGetBool (PcdTcssSupport) == 1
  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index ++) {
#if FixedPcdGet8(PcdFspModeSelection) == 0
    CapPolicy = TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[Index];
#else
    if (Index == 0) {
      CapPolicy = ((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort0;
    } else if (Index == 1) {
      CapPolicy = ((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort1;
    } else if (Index == 2) {
      CapPolicy = ((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort2;
    } else if (Index == 3) {
      CapPolicy = ((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort3;
    }
#endif
    if (!IS_TC_PORT_USB_SUPPORTED (CapPolicy)) {
      //
      // Update policy while USBC Port native USB3 unsupported
      //
#if FixedPcdGet8(PcdFspModeSelection) == 1
      ((FSPS_UPD *) FspsUpd)->FspsConfig.UsbTcPortEn &= ~(1 << Index);
#else
      TcssPeiConfig->UsbConfig.PortUsb30[Index].Enable = FALSE;
#endif
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.CpuUsb3OverCurrentPin[Index],TcssPeiConfig->UsbConfig.PortUsb30[Index].OverCurrentPin, USB_OC_SKIP);
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TcssCpuUsbPdoProgramming,    TcssPeiConfig->UsbConfig.PdoProgramming, TRUE);
    }
  }
#endif

  CopyMem (&BmpImageGuid, PcdGetPtr(PcdIntelGraphicsVbtFileGuid), sizeof(BmpImageGuid));

  if (!EFI_ERROR (Status)) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PavpEnable, IGpuConfig->PavpEnable, SaSetup.PavpEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PeiGraphicsPeimInit, IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit, SaSetup.PeiGraphicsPeimInit);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.RC1pGtFreqEnable, IGpuConfig->PeiGtConfig.RC1pGtFreqEnable, SaSetup.RC1pGtFreqEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.RC1pMediaFreqEnable, IGpuConfig->PeiMediaConfig.RC1pMediaFreqEnable, SaSetup.RC1pMediaFreqEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.RenderStandby, IGpuConfig->RenderStandby, SaSetup.EnableRenderStandby);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.MediaStandby, IGpuConfig->MediaStandby, SaSetup.EnableMediaStandby );
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ConfigureGT, IGpuConfig->PeiGtConfig.ConfigureGT, SaSetup.ConfigureGT);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.ConfigureMedia, IGpuConfig->PeiMediaConfig.ConfigureMedia, SaSetup.ConfigureMedia);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SkipFspGop, IGpuConfig->PeiDisplayConfig.SkipFspGop, FixedPcdGetBool(PcdFspPeiGopDisable));

    if (SaSetup.PeiGraphicsPeimInit == 1) {
      Buffer = NULL;

      CopyMem (&FileGuid, &BmpImageGuid, sizeof(FileGuid));
      PeiGetSectionFromAnyFv (&FileGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
      if (Buffer == NULL) {
        DEBUG((DEBUG_ERROR, "Could not locate VBT\n"));
      }

      if (BootMode == BOOT_ON_S3_RESUME) {
#if FixedPcdGet8(PcdFspModeSelection) == 1
        ((FSPS_UPD *) FspsUpd)->FspsConfig.GraphicsConfigPtr = (UINT64) NULL;
#else
        IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr = (VOID *) NULL;
#endif
      } else {
#if FixedPcdGet8(PcdFspModeSelection) == 1
        ((FSPS_UPD *) FspsUpd)->FspsConfig.GraphicsConfigPtr = (UINT64) Buffer;
        DEBUG ((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromFv is 0x%x\n", ((FSPS_UPD *) FspsUpd)->FspsConfig.GraphicsConfigPtr));
#else
        IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr = (VOID *) Buffer;
        DEBUG ((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromFv is 0x%x\n", IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr));
#endif
      }
      COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VbtSize, IGpuConfig->PeiDisplayConfig.VbtSize, (UINT32)Size);
      DEBUG ((DEBUG_INFO, "Vbt Size from PeiGetSectionFromFv is 0x%x\n", Size));

      Buffer = NULL;
      PeiGetSectionFromAnyFv (&gTianoLogoGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
      if (Buffer == NULL) {
        DEBUG ((DEBUG_WARN, "Could not locate Logo\n"));
      }

      DEBUG ((DEBUG_INFO, "Buffer from PeiGetSectionFromFv is 0x%x\n", Buffer));
      DEBUG ((DEBUG_INFO, "Size from PeiGetSectionFromFv is 0x%x\n", Size));

      //
      // OEM can modify the position of PEI logo, default is center.
      //
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.HorizontalResolution, IGpuConfig->PeiDisplayConfig.HorizontalResolution, 0);
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VerticalResolution, IGpuConfig->PeiDisplayConfig.VerticalResolution, 0);
    }

    if (Buffer != NULL) {
      Status = TranslateBmpToGopBlt (
                Buffer,
                Size,
                &Blt,
                &BltSize,
                &Height,
                &Width
                );

      if (Status == EFI_BUFFER_TOO_SMALL) {
        Blt = NULL;
        Status = TranslateBmpToGopBlt (
                  Buffer,
                  Size,
                  &Blt,
                  &BltSize,
                  &Height,
                  &Width
                  );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "TranslateBmpToGopBlt, Status = %r\n", Status));
          ASSERT_EFI_ERROR (Status);
          return Status;
        }
      }

      //
      // Initialize Blt, BltSize, LogoPixel Height and Width
      //
#if FixedPcdGet8(PcdFspModeSelection) == 1
      ((FSPS_UPD *) FspsUpd)->FspsConfig.BltBufferAddress = (UINTN) Blt;
#else
      IGpuConfig->PeiDisplayConfig.BltBufferAddress = Blt;
#endif
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelHeight,  IGpuConfig->PeiDisplayConfig.LogoPixelHeight, (UINT32) Height);
      UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelWidth,  IGpuConfig->PeiDisplayConfig.LogoPixelWidth,  (UINT32) Width);
    }

    // Initialize Misc SA Configuration

    //
    // Intel Platform Framework needs Camarillo device to be enabled.
    //
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.Device4Enable, HostBridgePeiConfig->Device4Enable, SetupData.IpfEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.NpuEnable, NpuPeiConfig->NpuEnable, SaSetup.NpuEnable);
  }

#if FixedPcdGetBool(PcdVmdEnable) == 1
  //
  // VMD related settings from setup variable
  //
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdEnable, VmdPeiConfig->VmdEnable,                       SaSetup.VmdEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdGlobalMapping, VmdPeiConfig->VmdGlobalMapping,                SaSetup.VmdGlobalMapping);

  for (VmdDevIndex = 0; VmdDevIndex < VMD_MAX_DEVICES; ++VmdDevIndex) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdPort[VmdDevIndex], VmdPeiConfig->VmdPortEnable[VmdDevIndex].RpEnable,   SaSetup.VmdPort[VmdDevIndex]);
    //Update dev and Fuc
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdPortDev[VmdDevIndex], VmdPeiConfig->VmdPortEnable[VmdDevIndex].RpDevice, SaSetup.VmdPortDev[VmdDevIndex]);
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdPortFunc[VmdDevIndex], VmdPeiConfig->VmdPortEnable[VmdDevIndex].RpFunction, SaSetup.VmdPortFunc[VmdDevIndex]);
  }
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdCfgBarBase, VmdPeiConfig->VmdCfgBarBase,  PcdGet32(PcdVmdCfgBarBase));
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdMemBar1Base, VmdPeiConfig->VmdMemBar1Base,PcdGet32(PcdVmdMemBar1Base));
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VmdMemBar2Base, VmdPeiConfig->VmdMemBar2Base,PcdGet32(PcdVmdMemBar2Base));

  // Read VmdVariable
  VarSize = 0;
  VmdVariablePtr = NULL;
  Status = VariableServices->GetVariable (
                              VariableServices,
                              EFI_VMD_OS_VARIABLE_NAME,
                              &gEfiVmdFeatureVariableGuid,
                              NULL,
                              &VarSize,
                              VmdVariablePtr
                              );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    VmdVariablePtr = AllocateZeroPool (VarSize);
    DEBUG ((DEBUG_VERBOSE, "VMD VARIABLE_BUFFER_TOO_SMALL\n"));
    ASSERT (VmdVariablePtr != NULL);

    Status = VariableServices->GetVariable (
                                  VariableServices,
                                  EFI_VMD_OS_VARIABLE_NAME,
                                  &gEfiVmdFeatureVariableGuid,
                                  NULL,
                                  &VarSize,
                                  VmdVariablePtr
                                  );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Vmd OS Variable not found Status is %r\n", Status));
    }
  }
#if FixedPcdGet8(PcdFspModeSelection) == 1
    ((FSPS_UPD *) FspsUpd)->FspsConfig.VmdVariablePtr = (UINT64) VmdVariablePtr;
    DEBUG ((DEBUG_INFO, "VmdVariablePtr from PeiGetSectionFromFv is 0x%x\n", ((FSPS_UPD *) FspsUpd)->FspsConfig.VmdVariablePtr));
#else
  VmdPeiConfig->VmdVariablePtr = VmdVariablePtr;
  DEBUG ((DEBUG_INFO, "VmdVariablePtr from PeiGetSectionFromFv is 0x%x\n", VmdPeiConfig->VmdVariablePtr));
#endif
#endif

#if FixedPcdGetBool (PcdTcssSupport) == 1
  //
  // BIOS-IOM Interaction policy update
  //
  if (SetupData.DeepestUSBSleepWakeCapability == 3) {
    //
    // Wake Capability is S3
    //
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VccSt, TcssPeiConfig->IomConfig.IomInterface.VccSt, 0);
  } else {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.VccSt,TcssPeiConfig->IomConfig.IomInterface.VccSt, SaSetup.TcssVccstStatus);
  }
  COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.D3ColdEnable, TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable, SaSetup.TcssD3ColdEnable);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TcCstateLimit, TcssPeiConfig->IomConfig.TcStateLimit, SaSetup.TcStateLimit);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.TcNotifyIgd, TcssPeiConfig->IomConfig.TcNotifyIgd, SaSetup.TcNotifyIgd);

  //
  // Itbt PCI Root Port Policy Initialization
  //
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.PtmEnabled[Index], TcssPeiConfig->PciePolicy.PciePortPolicy[Index].PtmEnabled, SaSetup.PtmEnabled[Index]);
    ///
    /// LTR Settings
    ///
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpLtrEnable[Index],                         TcssPeiConfig->PciePolicy.PciePortPolicy[Index].LtrEnable,                         SaSetup.SaPcieItbtLtrEnable[Index],                         Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideMode[Index],           TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideMode,          SaSetup.SaPcieItbtSnoopLatencyOverrideMode[Index],          Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideMultiplier[Index],     TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideMultiplier,    SaSetup.SaPcieItbtSnoopLatencyOverrideMultiplier[Index],    Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideValue[Index],         TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideValue,         SaSetup.SaPcieItbtSnoopLatencyOverrideValue[Index],         Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideMode[Index],       TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideMode,       SaSetup.SaPcieItbtNonSnoopLatencyOverrideMode[Index],       Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideMultiplier[Index], TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideMultiplier, SaSetup.SaPcieItbtNonSnoopLatencyOverrideMultiplier[Index], Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideValue[Index],      TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideValue,      SaSetup.SaPcieItbtNonSnoopLatencyOverrideValue[Index],      Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpForceLtrOverride[Index],                  TcssPeiConfig->PciePolicy.PciePortPolicy[Index].ForceLtrOverride,                  SaSetup.SaPcieItbtForceLtrOverride[Index],                  Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPS_UPD *)FspsUpd)->FspsConfig.SaPcieItbtRpLtrConfigLock[Index],                     TcssPeiConfig->PciePolicy.PciePortPolicy[Index].LtrConfigLock,                     SaSetup.SaPcieItbtLtrConfigLock[Index],                     Index);
  }
  //
  // BIOS-PMC Interaction policy update
  //
  if ((SetupData.UsbcBiosTcssHandshake == 1) && (PcdGetBool (VpdPcdTcssPmcPdEnable))) {
    COMPARE_AND_UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PmcPdEnable, TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable, SetupData.UsbcBiosTcssHandshake);
  } else {
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PmcPdEnable, TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable, 0);
  }

  //
  // TCSS XHCI Port x Convert to TypeA
  //
  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    //
    // TcssGetPchUsb2PortNoMappedWithTypeATcssXhciPort function is check
    // the PcdUsbCPort#Properties and get the mapping mapping PCH XHCI port
    // number to PchUsb2PortNo.
    // If the Port type isn't CPU Type-A, then the PchUsb2PortNo get '0'.
    //
    PchUsb2PortNo = TcssGetPchUsb2PortNoMappedWithTypeATcssXhciPort (Index);
    if ((PchUsb2PortNo > 0) && (PchUsb2PortNo <= GetPchXhciMaxUsb2PortNum())) {
      //
      // The PchUsb2PortNo is validate that means the PcdUsbCPort#Properties Port Type is to Type-A.
      // So that the Enable field set to '1'.
      //
      TcssPortConvProperties.Field.Enable = 1;
      TcssPortConvProperties.Field.Orientational = TcssGetPortOrientationSetting (Index);
      TcssPortConvProperties.Field.MappingPchXhciUsb2 = PchUsb2PortNo;

      COMPARE_UPDATE_POLICY_ARRAY (\
      ((FSPS_UPD *) FspsUpd)->FspsConfig.EnableTcssCovTypeA[Index],\
        TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Data8,\
        TcssPortConvProperties.Data8,\
        Index\
        );
    }
  }
#endif
  return EFI_SUCCESS;
}
