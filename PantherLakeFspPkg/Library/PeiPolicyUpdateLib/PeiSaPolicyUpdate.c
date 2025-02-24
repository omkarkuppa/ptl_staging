/** @file
  Do Platform Stage System Agent initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <FspEas.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <FspsUpd.h>
#include <TcssInfo.h>
#include <TcssPeiConfig.h>
#if (FixedPcdGetBool(PcdVmdEnable) == 1)
#include <VmdPeiConfig.h>
#endif
#include <TelemetryPeiConfig.h>

/**
  Update Fusa policies.

  @param[in out] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateFusaPolicy (
  IN OUT SI_POLICY_PPI  *SiPolicy,
  IN     FSPS_UPD       *FspsUpd
  )
{
#if FixedPcdGetBool(PcdEmbeddedEnable) == 1
  FUSA_CONFIG       *FusaConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  FusaConfig->DisplayFusaConfigEnable = FspsUpd->FspsConfig.DisplayFusaConfigEnable;
  FusaConfig->GraphicFusaConfigEnable = FspsUpd->FspsConfig.GraphicFusaConfigEnable;
  FusaConfig->OpioFusaConfigEnable    = FspsUpd->FspsConfig.OpioFusaConfigEnable;
#endif
}

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization

  @param[in out] SiPolicyPpi       SI_POLICY PPI
  @param[in]     FspsUpd           The pointer of FspsUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicy (
  IN OUT SI_POLICY_PPI       *SiPolicyPpi,
  IN     FSPS_UPD            *FspsUpd
  )
{
  EFI_STATUS                    Status;
  FSP_S_CONFIG                  *FspsConfig;
  HOST_BRIDGE_PEI_CONFIG        *HostBridgePeiConfig;
  TCSS_PEI_CONFIG               *TcssPeiConfig;
#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  VMD_PEI_CONFIG                *VmdPeiConfig;
#endif
  UINT8                         Index;
  TELEMETRY_PEI_CONFIG          *TelemetryPeiConfig;
  IAX_PEI_CONFIG                *IaxPeiConfig;
  NPU_PEI_CONFIG                *NpuPeiConfig;
  IGPU_PEI_CONFIG               *IGpuConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiSaPolicy\n"));

  // Use direct pointers below, to reduce debug build code size
  FspsConfig           = &FspsUpd->FspsConfig;

  IGpuConfig              = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR(Status);

  HostBridgePeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHostBridgePeiConfigGuid, (VOID *) &HostBridgePeiConfig);
  ASSERT_EFI_ERROR (Status);

  TcssPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR(Status);

  NpuPeiConfig          = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gNpuPeiConfigGuid, (VOID *) &NpuPeiConfig);
  ASSERT_EFI_ERROR(Status);

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  VmdPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gVmdPeiConfigGuid, (VOID *) &VmdPeiConfig);
  ASSERT_EFI_ERROR(Status);
#endif

  TelemetryPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *)SiPolicyPpi, &gTelemetryPeiConfigGuid, (VOID *)&TelemetryPeiConfig);
  ASSERT_EFI_ERROR(Status);

  IaxPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIaxPeiConfigGuid, (VOID *)&IaxPeiConfig);
  ASSERT_EFI_ERROR (Status);

  if (HostBridgePeiConfig != NULL) {
    HostBridgePeiConfig->Device4Enable    = FspsConfig->Device4Enable;
    HostBridgePeiConfig->SkipPamLock      = FspsConfig->SkipPamLock;
  }

  if (TelemetryPeiConfig != NULL) {
    TelemetryPeiConfig->CpuCrashLogEnable = FspsUpd->FspsConfig.CpuCrashLogEnable;
  }

  NpuPeiConfig->NpuEnable = ((FSPS_UPD *) FspsUpd)->FspsConfig.NpuEnable;

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  //
  // VMD configuration
  //
  if (VmdPeiConfig != NULL) {
    VmdPeiConfig->VmdEnable       = FspsConfig->VmdEnable;
    VmdPeiConfig->VmdGlobalMapping = FspsConfig->VmdGlobalMapping;
    for (Index = 0; Index < VMD_MAX_DEVICES; ++Index) {
      VmdPeiConfig->VmdPortEnable[Index].RpEnable    = FspsConfig->VmdPort[Index];
      VmdPeiConfig->VmdPortEnable[Index].RpBus       = FspsConfig->VmdPortBus[Index];
      VmdPeiConfig->VmdPortEnable[Index].RpDevice    = FspsConfig->VmdPortDev[Index];
      VmdPeiConfig->VmdPortEnable[Index].RpFunction  = FspsConfig->VmdPortFunc[Index];
    }
    VmdPeiConfig->VmdVariablePtr  = (EFI_VMD_OS_DATA*)(UINTN)FspsConfig->VmdVariablePtr;
    VmdPeiConfig->VmdCfgBarBase   = FspsConfig->VmdCfgBarBase;
    VmdPeiConfig->VmdMemBar1Base  = FspsConfig->VmdMemBar1Base;
    VmdPeiConfig->VmdMemBar2Base  = FspsConfig->VmdMemBar2Base;
  }
#endif
  if (TcssPeiConfig != NULL) {
    TcssPeiConfig->IomConfig.IomInterface.VccSt         = FspsConfig->VccSt;
    TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable  = FspsConfig->D3ColdEnable;
    TcssPeiConfig->IomConfig.TcNotifyIgd                = FspsConfig->TcNotifyIgd;
    TcssPeiConfig->IomConfig.TcStateLimit               = FspsConfig->TcCstateLimit;
    TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable   = FspsConfig->PmcPdEnable;
    TcssPeiConfig->MiscConfig.LsxOe  = FspsConfig->TcssLsxOe;
    TcssPeiConfig->UsbConfig.OverCurrentEnable = FspsConfig->PchUsbOverCurrentEnable;
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      TcssPeiConfig->UsbConfig.PortUsb30[Index].OverCurrentPin = FspsConfig->CpuUsb3OverCurrentPin[Index];
      TcssPeiConfig->UsbConfig.PortUsb30[Index].Enable = (FspsConfig->UsbTcPortEn >> Index) & 0x01;
      TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Data8 = FspsConfig->EnableTcssCovTypeA[Index];
    }

    //
    // Itbt Pcie Root port Policy update
    //
    for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
      if (TcssPeiConfig != NULL) {
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].PtmEnabled = FspsConfig->PtmEnabled[Index];
        ///
        /// LTR Settings
        ///
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].LtrEnable                         = FspsConfig->SaPcieItbtRpLtrEnable[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideMode          = FspsConfig->SaPcieItbtRpSnoopLatencyOverrideMode[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideMultiplier    = FspsConfig->SaPcieItbtRpSnoopLatencyOverrideMultiplier[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].SnoopLatencyOverrideValue         = FspsConfig->SaPcieItbtRpSnoopLatencyOverrideValue[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideMode       = FspsConfig->SaPcieItbtRpNonSnoopLatencyOverrideMode[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideMultiplier = FspsConfig->SaPcieItbtRpNonSnoopLatencyOverrideMultiplier[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].NonSnoopLatencyOverrideValue      = FspsConfig->SaPcieItbtRpNonSnoopLatencyOverrideValue[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].ForceLtrOverride                  = FspsConfig->SaPcieItbtRpForceLtrOverride[Index];
        TcssPeiConfig->PciePolicy.PciePortPolicy[Index].LtrConfigLock                     = FspsConfig->SaPcieItbtRpLtrConfigLock[Index];
      }
    }
  }

  IGpuConfig->PavpEnable = ((FSPS_UPD *) FspsUpd)->FspsConfig.PavpEnable;
  IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit = ((FSPS_UPD *) FspsUpd)->FspsConfig.PeiGraphicsPeimInit;
  IGpuConfig->PeiGtConfig.RC1pGtFreqEnable = ((FSPS_UPD *) FspsUpd)->FspsConfig.RC1pGtFreqEnable;
  IGpuConfig->PeiMediaConfig.RC1pMediaFreqEnable = ((FSPS_UPD *) FspsUpd)->FspsConfig.RC1pMediaFreqEnable;
  IGpuConfig->RenderStandby = ((FSPS_UPD *) FspsUpd)->FspsConfig.RenderStandby;
  IGpuConfig->MediaStandby = ((FSPS_UPD *) FspsUpd)->FspsConfig.MediaStandby;
  IGpuConfig->PeiGtConfig.ConfigureGT = ((FSPS_UPD *) FspsUpd)->FspsConfig.ConfigureGT;
  IGpuConfig->PeiMediaConfig.ConfigureMedia = ((FSPS_UPD *) FspsUpd)->FspsConfig.ConfigureMedia;
  IGpuConfig->PeiDisplayConfig.SkipFspGop = ((FSPS_UPD *) FspsUpd)->FspsConfig.SkipFspGop;
  IGpuConfig->PeiDisplayConfig.VbtSize = ((FSPS_UPD *) FspsUpd)->FspsConfig.VbtSize;
  IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr = (VOID *)(UINTN)((FSPS_UPD *) FspsUpd)->FspsConfig.GraphicsConfigPtr;
  IGpuConfig->PeiDisplayConfig.HorizontalResolution = ((FSPS_UPD *) FspsUpd)->FspsConfig.HorizontalResolution;
  IGpuConfig->PeiDisplayConfig.VerticalResolution = ((FSPS_UPD *) FspsUpd)->FspsConfig.VerticalResolution;
  IGpuConfig->PeiDisplayConfig.LidStatus = ((FSPS_UPD *) FspsUpd)->FspsConfig.LidStatus;
  IGpuConfig->PeiDisplayConfig.BltBufferAddress = (VOID *)(UINTN)((FSPS_UPD *) FspsUpd)->FspsConfig.BltBufferAddress;
  IGpuConfig->PeiDisplayConfig.LogoPixelHeight = ((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelHeight;
  IGpuConfig->PeiDisplayConfig.LogoPixelWidth = ((FSPS_UPD *) FspsUpd)->FspsConfig.LogoPixelWidth;
  //
  // IAX configuration
  //
  if (IaxPeiConfig != NULL) {
    IaxPeiConfig->IaxEnable = FspsConfig->IaxEnable;
  }
  //
  // Fusa configuration
  //
  FspUpdateFusaPolicy (SiPolicyPpi, FspsUpd);
  return EFI_SUCCESS;
}
