/** @file
  This contains code necessary to build the Domain IGpu Library.

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

@par Specification
**/

#include <IpWrapperCntxtInfoClient.h>
#include <Library/DebugLib.h>
#include <Defines/IgdDefines.h>
#include <IpIGpu.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/SiPolicy.h>
#include <IGpuConfig.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiImrInitLib.h>
#include <Uefi/UefiBaseType.h>
#include <Pi/PiPeiCis.h>
#include <IGpuInit.h>
#include <Library/IGpuInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/MtrrLib.h>
#include <Library/DomainIGpuInit.h>
#include <MemInfoHob.h>
#include <Library/VtdInfoLib.h>
#include <HostBridgeConfig.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/MeInitLib.h>
#include <MeHandle.h>
#include <MeDefines.h>
#include <PciSbdf.h>
#include <IGpuDataHob.h>
#include <Register/HostBridgeRegs.h>
#include <Library/PeiImrInitLib.h>
#include <Library/MeInitLib.h>
#include <Library/IGpuPeiPolicyLib.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>

EFI_PHYSICAL_ADDRESS
NaturalAlignment (
  IN  EFI_PHYSICAL_ADDRESS  MemBaseAddr,
  IN  UINT64                MemSize
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mIGpuEndOfPeiNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    IGpuEndOfPeiCallback
  }
};

/**
  Update Reg Context for IGpu

  @param[in] pInst             Pointer to IP inst
  @param[in] IGpuPreMemConfig   Graphics PreMem Config
**/
STATIC
IP_CSI_STATUS
IGpuUpdateRegCntxt (
  IP_IGPU_INST            *pInst,
  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  IP_WR_REG_INFO  *RegInfo;

  if (pInst == NULL) {
    DEBUG ((DEBUG_INFO, "IGpu pInst == NULL\n"));
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->Prefix.pName == NULL) {
    // force valid name for debug printing
    pInst->Prefix.pName = "IGpuAgent";
  }

  pInst->PcieCfgAccess = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->PcieCfgAccess == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return IpCsiStsErrorInsufficientResources;
  }

  //
  // Update IGpu Config access
  //
  RegInfo = (IP_WR_REG_INFO *)(UINTN)pInst->PcieCfgAccess;

  RegInfo->AccessType      = IpWrRegAccessCfg;
  RegInfo->AccessSubType   = IpWrRegSubTypeNone;
  RegInfo->RegType.Pci.Seg = 0x0;
  RegInfo->RegType.Pci.Bus = IGD_BUS_NUM;
  RegInfo->RegType.Pci.Dev = IGD_DEV_NUM;
  RegInfo->RegType.Pci.Fun = IGD_FUN_NUM;

  //
  // Update IGpu Register MMIO access
  //
  pInst->MmioAccess = (IP_WR_REG_CNTXT)(UINTN)AllocateZeroPool (sizeof (IP_WR_REG_INFO));
  if (pInst->MmioAccess == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return IpCsiStsErrorInsufficientResources;
  }

  RegInfo = (IP_WR_REG_INFO *)(UINTN)pInst->MmioAccess;

  RegInfo->AccessType         = IpWrRegAccessMem;
  RegInfo->AccessSubType      = IpWrRegSubTypeNone;
  RegInfo->OverrideAccessType = IpWrRegAccessDefault;
  RegInfo->RegType.MmioBase   = IGpuPreMemConfig->GttMmAdr;

  //
  // Init print Context
  //
  pInst->PrintError   = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoError;
  pInst->PrintWarning = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoWarning;
  pInst->PrintLevel1  = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoLevel1;

  pInst->Signature = IP_IGPU_SIGNATURE;

  return IpCsiStsSuccess;
}

/**
  Update Reg Post Mem Context for IGpu

  @param[in] pInst             Pointer to IP inst
  @param[in] IGpuPreMemConfig   Graphics PreMem Config
**/
STATIC
IP_CSI_STATUS
IGpuUpdateRegPostMemCntxt (
  IP_IGPU_INST            *pInst,
  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  IP_WR_REG_INFO  *RegInfo;

  if (pInst == NULL) {
    DEBUG ((DEBUG_INFO, "IGpu pInst == NULL\n"));
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->Prefix.pName == NULL) {
    // force valid name for debug printing
    pInst->Prefix.pName = "IGpuAgent";
  }

  //
  // Allocate Pages so that memory is available though out the boot phase
  //
  pInst->PcieCfgAccess = (IP_WR_REG_CNTXT)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (sizeof (IP_WR_REG_INFO)));
  if (pInst->PcieCfgAccess == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return IpCsiStsErrorInsufficientResources;
  }

  //
  // Update IGpu Config access
  //
  RegInfo = (IP_WR_REG_INFO *)(UINTN)pInst->PcieCfgAccess;
  ZeroMem (RegInfo, sizeof (IP_WR_REG_INFO));

  RegInfo->AccessType         = IpWrRegAccessCfg;
  RegInfo->AccessSubType      = IpWrRegSubTypeNone;
  RegInfo->OverrideAccessType = IpWrRegAccessDefault;
  RegInfo->RegType.Pci.Seg    = 0x0;
  RegInfo->RegType.Pci.Bus    = IGD_BUS_NUM;
  RegInfo->RegType.Pci.Dev    = IGD_DEV_NUM;
  RegInfo->RegType.Pci.Fun    = IGD_FUN_NUM;

  //
  // Allocate Pages so that memory is available though out the boot phase
  //
  pInst->MmioAccess = (IP_WR_REG_CNTXT)(UINTN)AllocatePages (EFI_SIZE_TO_PAGES (sizeof (IP_WR_REG_INFO)));
  if (pInst->MmioAccess == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return IpCsiStsErrorInsufficientResources;
  }

  RegInfo = (IP_WR_REG_INFO *)(UINTN)pInst->MmioAccess;
  ZeroMem (RegInfo, sizeof (IP_WR_REG_INFO));

  RegInfo->AccessType         = IpWrRegAccessMem;
  RegInfo->AccessSubType      = IpWrRegSubTypeNone;
  RegInfo->OverrideAccessType = IpWrRegAccessDefault;
  RegInfo->RegType.MmioBase   = IGpuPreMemConfig->GttMmAdr;

  //
  // Init print Context
  //
  pInst->PrintError   = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoError;
  pInst->PrintWarning = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoWarning;
  pInst->PrintLevel1  = (IP_WR_PRINT_CNTXT)(UINTN)&mPrintInfoLevel1;

  pInst->Signature = IP_IGPU_SIGNATURE;

  return IpCsiStsSuccess;
}

/**
  Update Reg Context for IGpu

  @param[in] pInst             Pointer to IP inst
  @param[in] IGpuPreMemConfig  Graphics PreMem Config
**/
STATIC
VOID
IGpuUpdatePreMemConfig (
  IP_IGPU_INST            *pInst,
  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  pInst->IGpuPreMemConfig.PrimaryDisplay                  = IGpuPreMemConfig->PrimaryDisplay;
  pInst->IGpuPreMemConfig.InternalGraphics                = IGpuPreMemConfig->InternalGraphics;
  pInst->IGpuPreMemConfig.IgdDvmt50PreAlloc               = IGpuPreMemConfig->IgdDvmt50PreAlloc;
  pInst->IGpuPreMemConfig.PanelPowerEnable                = IGpuPreMemConfig->PanelPowerEnable;
  pInst->IGpuPreMemConfig.MemoryBandwidthCompression      = IGpuPreMemConfig->MemoryBandwidthCompression;
  pInst->IGpuPreMemConfig.DeltaT12PowerCycleDelay         = IGpuPreMemConfig->DeltaT12PowerCycleDelay;
  pInst->IGpuPreMemConfig.GttMmAdr                        = IGpuPreMemConfig->GttMmAdr;
  pInst->IGpuPreMemConfig.LMemBar                         = IGpuPreMemConfig->LMemBar;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortAConfig = IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortBConfig = IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortAHpd    = IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortBHpd    = IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortCHpd    = IGpuPreMemConfig->DdiConfiguration.DdiPortCHpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort1Hpd    = IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort2Hpd    = IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort3Hpd    = IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort4Hpd    = IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortADdc    = IGpuPreMemConfig->DdiConfiguration.DdiPortADdc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortBDdc    = IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortCDdc    = IGpuPreMemConfig->DdiConfiguration.DdiPortCDdc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort1Ddc    = IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort2Ddc    = IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort3Ddc    = IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc;
  pInst->IGpuPreMemConfig.DdiConfiguration.DdiPort4Ddc    = IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc;
  pInst->IGpuPreMemConfig.OemT12DelayOverride             = IGpuPreMemConfig->OemT12DelayOverride;
}

/**
  Update PostMem Reg Context for IGpu

  @param[in] pInst       Pointer to IP inst
  @param[in] IGpuConfig   Graphics Config
**/
STATIC
VOID
IGpuUpdatePostMemConfig (
  IP_IGPU_INST     *pInst,
  IGPU_PEI_CONFIG  *IGpuConfig
  )
{
  pInst->IGpuConfig.PeiDisplayConfig.GraphicsConfigPtr    = (UINT64)(UINTN)IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr;
  pInst->IGpuConfig.PeiDisplayConfig.BltBufferAddress     = (UINT64)(UINTN)IGpuConfig->PeiDisplayConfig.BltBufferAddress;
  pInst->IGpuConfig.PeiDisplayConfig.LogoPixelHeight      = IGpuConfig->PeiDisplayConfig.LogoPixelHeight;
  pInst->IGpuConfig.PeiDisplayConfig.LogoPixelWidth       = IGpuConfig->PeiDisplayConfig.LogoPixelWidth;
  pInst->IGpuConfig.PeiDisplayConfig.VerticalResolution   = IGpuConfig->PeiDisplayConfig.VerticalResolution;
  pInst->IGpuConfig.PeiDisplayConfig.HorizontalResolution = IGpuConfig->PeiDisplayConfig.HorizontalResolution;
  pInst->IGpuConfig.PeiDisplayConfig.PeiGraphicsPeimInit  = IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit;
  pInst->IGpuConfig.PeiDisplayConfig.SkipFspGop           = IGpuConfig->PeiDisplayConfig.SkipFspGop;

  pInst->IGpuConfig.PeiMediaConfig.ConfigureMedia      = IGpuConfig->PeiMediaConfig.ConfigureMedia;
  pInst->IGpuConfig.PeiMediaConfig.RC1pMediaFreqEnable = IGpuConfig->PeiMediaConfig.RC1pMediaFreqEnable;

  pInst->IGpuConfig.PeiGtConfig.ConfigureGT      = IGpuConfig->PeiGtConfig.ConfigureGT;
  pInst->IGpuConfig.PeiGtConfig.RC1pGtFreqEnable = IGpuConfig->PeiGtConfig.RC1pGtFreqEnable;

  pInst->IGpuConfig.RenderStandby = IGpuConfig->RenderStandby;
  pInst->IGpuConfig.PavpEnable    = IGpuConfig->PavpEnable;
  pInst->IGpuConfig.MediaStandby  = IGpuConfig->MediaStandby;

}

/**
  IGpuPreMemInit for Initilizing IGpu IP Instances

  @retval       EFI_SUCCESS on success
**/
EFI_STATUS
IGpuPreMemInit (
  IN  IGPU_XE_VERSION  IGpuXeVersion
  )
{
  IP_IGPU_INST            *IGpuInst;
  EFI_STATUS              Status;
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;
  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  IGpuInst = BuildGuidHob (&gIGpuInstHobGuid, sizeof (IP_IGPU_INST));

  if (IGpuInst == NULL) {
    DEBUG ((DEBUG_WARN, "Failed to build IGpuInstHob\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (IGpuInst, sizeof (IP_IGPU_INST));

  Status = PeiServicesLocatePpi (
                                 &gSiPreMemPolicyPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&SiPreMemPolicyPpi
                                 );
  ASSERT_EFI_ERROR (Status);

  IGpuPreMemConfig = NULL;
  Status           = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *)&IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update Register Context
  //
  IGpuUpdateRegCntxt (IGpuInst, IGpuPreMemConfig);

  //
  // Update the XeVersion
  //
  IGpuInst->XeVersion = (IP_IGPU_XE_VERSION)IGpuXeVersion;

  DEBUG ((DEBUG_INFO, "IGpu Xe Version = %d\n", IGpuInst->XeVersion));

  IGpuInst->IGpuPrivateConfig.IsEom = IsEom ();

  //
  // Update configblock values to IGPU Inst
  //
  IGpuUpdatePreMemConfig (IGpuInst, IGpuPreMemConfig);

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  IGpuPostMemInit for Initilizing Post Mem IGpu IP Instances

  @retval       EFI_SUCCESS on success
**/
EFI_STATUS
IGpuPostMemInit (
  VOID
  )
{
  IP_IGPU_INST            *IGpuInst;
  EFI_STATUS              Status;
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;
  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig;
  SI_POLICY_PPI           *SiPolicy;
  IGPU_PEI_CONFIG         *IGpuConfig;
  MEMORY_INFO_DATA_HOB    *MemInfo;
  EFI_HOB_GUID_TYPE       *GuidHob;
  IGPU_DATA_HOB           *IGpuDataHob;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  IGpuPreMemConfig = NULL;
  IGpuConfig       = NULL;
  MemInfo          = NULL;
  GuidHob          = NULL;
  IGpuDataHob      = NULL;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (IGpuInst->Signature != IP_IGPU_SIGNATURE) {
    CpuDeadLoop ();
  }

  IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "IGPU Hob Not Found\n"));
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  Status = PeiServicesLocatePpi (
                                 &gSiPreMemPolicyPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&SiPreMemPolicyPpi
                                 );
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (
                                 &gSiPolicyPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&SiPolicy
                                 );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *)&IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicy, &gGraphicsPeiConfigGuid, (VOID *)&IGpuConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update Register Context
  //
  IGpuUpdateRegPostMemCntxt (IGpuInst, IGpuPreMemConfig);

  //
  // Update PreMem configblock values to IGPU Inst
  //
  IGpuUpdatePreMemConfig (IGpuInst, IGpuPreMemConfig);

  //
  // Update PostMem configblock values to IGPU Inst
  //
  IGpuUpdatePostMemConfig (IGpuInst, IGpuConfig);

  //
  // Update Private Config
  // Update Memory Data
  //
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  if (GuidHob != NULL) {
    MemInfo = (MEMORY_INFO_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    return EFI_NOT_FOUND;
  }

  IGpuInst->IGpuPrivateConfig.MemoryInfoData.MemoryType                          = MemInfo->MemoryType;
  IGpuInst->IGpuPrivateConfig.MemoryInfoData.NumPopulatedChannels                = MemInfo->NumPopulatedChannels;
  IGpuInst->IGpuPrivateConfig.MemoryInfoData.SagvConfigInfo.NumSaGvPointsEnabled = MemInfo->SagvConfigInfo.NumSaGvPointsEnabled;
  CopyMem (&IGpuInst->IGpuPrivateConfig.MemoryInfoData.SagvConfigInfo, &MemInfo->SagvConfigInfo, sizeof (IP_HOB_SAGV_INFO));
  //
  // To make sure MRC and IpIGpu strucures are aligned to each other
  //
  ASSERT (sizeof (IP_HOB_SAGV_INFO) == sizeof (HOB_SAGV_INFO));

  //
  // Update FlatCss
  //
  IGpuInst->IGpuPrivateConfig.FlatCcsBaseAddr = IGpuDataHob->FlatCcsBaseAddr;
  IGpuInst->IGpuPrivateConfig.FlatCcsMemSize  = IGpuDataHob->FlatCcsMemSize;

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Create and Init IGPU Data Hob

  @param[in]  IGpuPreMemConfig     - Instance of IGPU_PEI_PREMEM_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
CreateIGpuDataHob (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  EFI_STATUS     Status;
  IGPU_DATA_HOB  *IGpuDataHob;

  IGpuDataHob = NULL;
  ///
  /// Create HOB for Graphics Data
  ///
  Status = PeiServicesCreateHob (
                                 EFI_HOB_TYPE_GUID_EXTENSION,
                                 sizeof (IGPU_DATA_HOB),
                                 (VOID **)&IGpuDataHob
                                 );
  ASSERT_EFI_ERROR (Status);
  if (IGpuDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "IGPU Data Hob not Created\n"));
    return EFI_BUFFER_TOO_SMALL;
  }

  ///
  /// Initialize default HOB data
  ///
  CopyGuid (&IGpuDataHob->EfiHobGuidType.Name, &gIGpuDataHobGuid);
  DEBUG ((DEBUG_INFO, "IGpuDataHob->EfiHobGuidType.Name: %g\n", &IGpuDataHob->EfiHobGuidType.Name));
  ZeroMem (&(IGpuDataHob->FlatCcsBaseAddr), sizeof (IGPU_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));

  IGpuDataHob->PrimaryDisplayDetection = DISPLAY_IGD;

  DEBUG ((DEBUG_INFO, "IGpuDataHob @ %X\n", IGpuDataHob));
  DEBUG ((DEBUG_INFO, "IGpuDataHobSize - HobHeader: %X\n", sizeof (IGPU_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "IGpuDataHobSize: %X\n", sizeof (IGPU_DATA_HOB)));
  DEBUG ((DEBUG_INFO, "PrimaryDisplayDetection: 0x%x\n", IGpuDataHob->PrimaryDisplayDetection));

  DEBUG ((DEBUG_INFO, "IGPU HOBs installed\n"));

  return EFI_SUCCESS;
}

/**
  Additional steps for disabling IGPU.

  @param[in]  pInst       The instance
**/
VOID
IGpuAdditionalStepsForDisabling (
  IN IP_IGPU_INST  *pInst
  )
{
  DEBUG ((DEBUG_INFO, "Disable IGD Device.\n"));
  ///
  /// Disable IGD device
  ///
  ///
  /// Set Register D0:F0 Offset 50h [15:8] = '00000000b'.
  /// This prevents UMA memory from being pre-allocated to IGD.
  /// Set D0:F0 Offset 50h [7:6] = '00b'.
  /// GTT Graphics Memory Size to 0
  /// Set VGA Disable (IVD) in D0:F0 Offset 50h [1] = '1b'.
  ///
  IpIGpuAdditionalStepToDisable (pInst);

  //
  // Clear GMS, GGMS and Set IVD, VEMEN
  //
  SetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcClearGmsSetIvdVamen, 0);

  ///
  /// Disable IGD. D0.F0.R 054h [4] = '0b'.
  ///
  UncoreDevEnWrite (Graphics, 0, Disable);

  //
  // Inform the Display PHY that IGPU has been disabled
  //
  InformIGpuDisableStatus ();
}

/**
  IGpuInit: Initialize the IGD if no other external graphics is present

  @param[in] IGpuPreMemConfig      - IGpuPreMemConfig to access the IGpuPreMemConfig related information
  @param[in] PrimaryDisplay       - Primary Display - default is IGD
  @param[in] IGpuMmioLength       - Total IGPU MMIO length

**/
VOID
IGpuInit (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN  DISPLAY_DEVICE          *PrimaryDisplay,
  IN OUT  UINT32              *IGpuMmioLength
  )
{
  BOOLEAN            IGpuSupported;
  EFI_HOB_GUID_TYPE  *GuidHob;
  IP_IGPU_INST       *IGpuInst;
  UINT64             IGpuTotalBarsLength;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  *IGpuMmioLength = 0;

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return;
  }

  IGpuSupported = IpIGpuSupported (IGpuInst);

  if (!IGpuSupported) {
    DEBUG ((DEBUG_INFO, "IGPU is unsupported or disabled!\n"));
    IGpuAdditionalStepsForDisabling (IGpuInst);
    return;
  }

  if (IGpuPreMemConfig->GttMmAdr == 0) {
    DEBUG ((DEBUG_INFO, "Temporary GttMmAdr Bar is not initialized. Returning from GraphicsInit().\n"));
    return;
  }

  ///
  /// Program GttMmAdr
  ///
  IpIGpuProgramGttMmAdr (IGpuInst);

  ///
  /// Program LMemBar
  ///
  IpIGpuProgramLMemBar (IGpuInst);

  ///
  /// Enable Bus Initiator and Memory access on IGPU
  ///
  IpIGpuEnableCmdReg (IGpuInst);

  ///
  /// If primary display device is IGD or no other display detected then enable IGD
  ///
  if (IGpuSupported &&
      (
       (
        ((*PrimaryDisplay == DISPLAY_IGD) || (IGpuPreMemConfig->PrimaryDisplay == DISPLAY_IGD)) &&
        (IGpuPreMemConfig->InternalGraphics != DISABLED)
       ) || (IGpuPreMemConfig->InternalGraphics == ENABLED)
      )
      )
  {
    DEBUG ((DEBUG_INFO, "IGPU enabled.\n"));

    ///
    /// Description of GMS D0:F0:R50h[15:8]
    ///
    SetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcGmsWriteData, (UINT8)(IGpuPreMemConfig->IgdDvmt50PreAlloc));

    ///
    /// Enable IGD VGA Decode.  This is needed so the Class Code will
    /// be correct for the IGD Device when determining which device
    /// should be primary.  If disabled, IGD will show up as a non VGA device.
    ///
    if ((IGpuPreMemConfig->PrimaryDisplay != DISPLAY_IGD) && (*PrimaryDisplay != DISPLAY_IGD)) {
      ///
      /// If IGD is forced to be enabled, but is a secondary display, disable IGD VGA Decode
      ///
      SetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcCfgSetIvdVamen, TRUE);
      DEBUG ((DEBUG_INFO, "IGD VGA Decode is disabled because it's not a primary display.\n"));
    } else {
      SetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcCfgClearIvdVamen, FALSE);
    }

    ///
    /// Save Media, Display, GT status (enable/disable) from IGPU MMIO register
    ///
    IpIGpuSaveGMDStatus (IGpuInst);

    IGpuTotalBarsLength  = IpIGpuGetGttMmAdrSize (IGpuInst);
    IGpuTotalBarsLength += IpIGpuGetLMemBarSize (IGpuInst);
    *IGpuMmioLength      = (UINT32)IGpuTotalBarsLength;
  } else {
    IGpuAdditionalStepsForDisabling (IGpuInst);
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Function to handle Graphics at end of PEI

  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDesc       Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Ppi              Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EFIAPI
IGpuEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS         Status;
  UINT64             GtApertureSize;
  IP_IGPU_INST       *IGpuInst;
  EFI_BOOT_MODE      BootMode;
  EFI_HOB_GUID_TYPE  *GuidHob;

  Status = PeiServicesGetBootMode (&BootMode);

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    return EFI_UNSUPPORTED;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  if ((Status == EFI_SUCCESS) && (BootMode != BOOT_ON_S3_RESUME)) {
    ///
    /// Clear IGD GttMmAdr, Aperture BAR and disable Bus Initiator and Memory Access for 0/2/0
    ///
    DEBUG ((DEBUG_INFO, "Clear Temp IGPU BARs at End Of PEI.\n"));
    IpIGpuClearTempBar (IGpuInst);
  }

  GtApertureSize = IpIGpuGetLMemBarSize (IGpuInst);

  DEBUG ((DEBUG_INFO, "GtApertureAdr = %lx GtApertureSize = %lx \n", IGpuInst->IGpuPreMemConfig.LMemBar, GtApertureSize));
  DEBUG ((DEBUG_INFO, "Restore Framebuffer memory to Original at EndofPei.\n"));
  Status = MtrrSetMemoryAttributeInMtrrSettings (
                                                 NULL,
                                                 IGpuInst->IGpuPreMemConfig.LMemBar,
                                                 GtApertureSize,
                                                 CacheUncacheable
                                                 );

  return EFI_SUCCESS;
}

/**
  Graphics function called end of PEI

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EFIAPI
IGpuEndOfPei (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = PeiServicesNotifyPpi (mIGpuEndOfPeiNotifyList);
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
}

/**
Initialize GT Power management
  @param[in] IGPU_PEI_PREMEM_CONFIG          IGpuPreMemConfig
  @param[in] IGPU_PEI_CONFIG                 IGpuConfig

  @retval EFI_SUCCESS          - GT Power management initialization complete
**/
EFI_STATUS
IGpuPmInit (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN  IGPU_PEI_CONFIG         *IGpuConfig
  )
{
  IP_IGPU_INST          *IGpuInst;
  MEMORY_INFO_DATA_HOB  *MemInfo;
  EFI_HOB_GUID_TYPE     *GuidHob;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  GuidHob = NULL;
  MemInfo = NULL;

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (IpIGpuSupported (IGpuInst) == FALSE) {
    return EFI_SUCCESS;
  }

  ///
  /// Program GttMmAdr
  ///
  IpIGpuProgramGttMmAdr (IGpuInst);

  ///
  /// Program LMemBar
  ///
  IpIGpuProgramLMemBar (IGpuInst);

  if (!IpIGpuCmdRegEnabled (IGpuInst)) {
    ///
    /// Enable Bus Initiator and Memory access on 0:2:0
    ///
    IpIGpuEnableCmdReg (IGpuInst);
  }

  //
  // Update IGPU Private Config
  //
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  if (GuidHob != NULL) {
    MemInfo                                    = (MEMORY_INFO_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
    IGpuInst->IGpuPrivateConfig.IsIbeccEnabled = MemInfo->IsIbeccEnabled;
  }

  IGpuInst->IGpuPrivateConfig.IsIGpuVtdEngineEnabled = IsVtdEngineEnabled (IGD_VTD);

  ///
  /// GtInit Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing GT\n"));
  IGpuGtInit (IGpuInst);

  ///
  /// MediaInit Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing Media\n"));
  IGpuMediaInit (IGpuInst);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function is to set IGPU Memory map (RC6 base, Doorbell base, Dfdbase etc).

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuSetMemMap (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  EFI_STATUS               Status;
  IP_IGPU_INST             *IGpuInst;
  EFI_HOB_GUID_TYPE        *GuidHob;
  ME_HANDLE                *MeHandle;
  PCI_SBDF                 HeciSbdf;
  EFI_PREMEM_GRAPHICS_PPI  *GraphicsPreMemPpi;
  IGPU_PEI_PREMEM_CONFIG   *IGpuPreMemConfig;
  UINT64                   StartTime;

  IGpuPreMemConfig  = NULL;
  GraphicsPreMemPpi = NULL;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
  }

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *)&IGpuPreMemConfig);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gGraphicsPeiPreMemConfigGuid block\n"));
    ASSERT_EFI_ERROR (Status);
  }

  if (IGpuInst->XeVersion <= IpIGpuXe3) {
    Status = GetMeHandle (&MeHandle);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    HeciSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (HECI1);

    if (IsSimicsEnvironment ()) {
      IGpuInst->IGpuPrivateConfig.CsmeBusNum = DEFAULT_PCI_BUS_NUMBER_PCH;
    } else {
      IGpuInst->IGpuPrivateConfig.CsmeBusNum = (UINT32)HeciSbdf.Fields.Bus;
    }

    IGpuInst->IGpuPrivateConfig.CsmeDevNum  = (UINT32)HeciSbdf.Fields.Dev;
    IGpuInst->IGpuPrivateConfig.CsmeFuncNum = (UINT32)HeciSbdf.Fields.Func;
  }

  IGpuInst->IGpuPrivateConfig.BdsmBase = GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmFullData);
  IGpuInst->IGpuPrivateConfig.DsmSize  = IGpuGetDsmSizeInBytes ();
  IGpuInst->IGpuPrivateConfig.BgsmBase = GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmFullData);
  IGpuInst->IGpuPrivateConfig.GgcBase  = (UINT16)GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcFullData);

  IpIGpuSetMemMap (IGpuInst);

  //
  // Clear VGA Display
  //
  if (IpIGpuSupported (IGpuInst) == TRUE) {
    if (IGpuPreMemConfig != NULL) {
      if (IGpuPreMemConfig->VgaInitControl == TRUE) {
        Status = PeiServicesLocatePpi (&gIntelPeiPreMemGraphicsPpiGuid, 0, NULL, (VOID **)&GraphicsPreMemPpi);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Unable to locate the GraphicsPreMemPpi\n"));
          return Status;
        }

        if (GraphicsPreMemPpi != NULL) {
          //
          // Log the uGOP Exit Timings
          //
          StartTime = GetFspCurrentTime ();

          //
          // Exit VGA
          //
          Status = GraphicsPreMemPpi->GraphicsPreMemPpiExit ();
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_INFO, "GraphicsPreMemPpiExit Failed"));
            return Status;
          }

          //
          // Log uGOP Exit timing in FSP
          //
          LogFspPerformanceData (FspuGopExitPerf, StartTime);

          //
          // disable IO Bar on 0:2:0
          //
          IpIGpuDisableIoCmdReg (IGpuInst);
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Reserve IGPU Stolen Memory.

  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] Touud                      - Size leftover pointer
  @param[in] ResourceAttributeTested    - Resource attribute flag
**/
VOID
EFIAPI
IGpuMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  )
{
  EFI_STATUS                   Status;
  UINT64                       FlatCcsBaseAddr;
  UINT32                       FlatCcsSizeInBytes;
  UINT32                       FlatCcsSizeInMb;
  EFI_RESOURCE_TYPE            ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;
  IGPU_DATA_HOB                *IGpuDataHob;
  IGPU_PEI_PREMEM_CONFIG       *IGpuPreMemConfig;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;

  IGpuPreMemConfig = NULL;
  IGpuDataHob      = NULL;

  if (IGpuIsSupported () == FALSE) {
    return;
  }

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **)&SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    return;
  }

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *)&IGpuPreMemConfig);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gGraphicsPeiPreMemConfigGuid block\n"));
    ASSERT_EFI_ERROR (Status);
  }

  if ((IGpuPreMemConfig != NULL) && (IGpuPreMemConfig->MemoryBandwidthCompression == 1)) {
    FlatCcsBaseAddr = *TopUseableMemAddr;
    //
    // Check if CCS Base is 1M aligned or not. If not, it should be MB aligned due to IMR requirement.
    //
    if ((FlatCcsBaseAddr & (SIZE_1MB - 1)) != 0x0) {
      FlatCcsBaseAddr = (FlatCcsBaseAddr & ((UINT64) ~(SIZE_1MB - 1))) + SIZE_1MB;
    }

    FlatCcsSizeInMb    = IGpuGetFlatCcsSizeInMb ();
    FlatCcsSizeInBytes = FlatCcsSizeInMb << 20;
    DEBUG ((DEBUG_INFO, "Flat CCS memory base = 0x%016lX Size = 0x%x\n", FlatCcsBaseAddr, FlatCcsSizeInBytes));

    // Fill IGpuDataHob with Flat Ccs Base and Size
    IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
    if (IGpuDataHob != NULL) {
      IGpuDataHob->FlatCcsBaseAddr = FlatCcsBaseAddr;
      IGpuDataHob->FlatCcsMemSize  = FlatCcsSizeInMb;
    }

    ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
    ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                        ResourceAttributeTested |
                        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;

    BuildResourceDescriptorWithOwnerHob (
                                         ResourceType,       // MemoryType,
                                         ResourceAttribute,  // MemoryAttribute
                                         FlatCcsBaseAddr,    // MemoryBegin
                                         FlatCcsSizeInBytes, // MemoryLength
                                         &gIGpuFlatCcsMemoryResourceHobGuid
                                         );

    BuildMemoryAllocationHob (
                              FlatCcsBaseAddr,
                              FlatCcsSizeInBytes,
                              EfiReservedMemoryType
                              );
    //
    // Program Flat CCS IMR.
    //
    Status = SetImr (IMR_1M_CCS, FlatCcsBaseAddr, FlatCcsSizeInBytes);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Fail to program IGPU Flat CCS IMR\n"));
    }

    *TopUseableMemAddr = FlatCcsBaseAddr + FlatCcsSizeInBytes;
    *Touud            -= FlatCcsSizeInBytes;
  }
}

/**
  Reserve IGPU Stolen Memory.

  @param[in] SiPreMemPolicyPpi        - Pointer to SiPreMemPolicyPpi
  @param[in] TopUseableMemAddr        - Moving address pointer
  @param[in] Touud                    - Size leftover pointer
  @param[in] ResourceAttributeTested  - Resource attribute flag
**/
VOID
EFIAPI
IGpuGsm2Allocation (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi,
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  )
{
  IGPU_PEI_PREMEM_CONFIG       *IGpuPreMemConfig;
  UINT64                       GfxGsm2BaseAddress;
  UINT64                       GfxGsm2TempBaseAddress;
  UINT64                       GfxGsm2Size;
  UINT64                       TempSize;
  IP_IGPU_INST                 *IGpuInst;
  IGPU_DATA_HOB                *IGpuDataHob;
  EFI_STATUS                   Status;
  EFI_RESOURCE_TYPE            ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;
  EFI_HOB_GUID_TYPE            *GuidHob;

  IGpuPreMemConfig       = NULL;
  GuidHob                = NULL;
  IGpuDataHob            = NULL;
  GfxGsm2BaseAddress     = 0;
  GfxGsm2TempBaseAddress = 0;
  GfxGsm2Size            = 0;
  TempSize               = 0;

  if (IGpuIsSupported () == FALSE) {
    DEBUG ((DEBUG_INFO, "Returning from %a since IGD is not present\n", __FUNCTION__));
    return;
  }

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *)&IGpuPreMemConfig);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gGraphicsPeiPreMemConfigGuid block\n"));
    ASSERT_EFI_ERROR (Status);
  }

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  if ((IGpuPreMemConfig != NULL) && (IGpuPreMemConfig->IGpuGsm2Size <= 15)) {
    GfxGsm2TempBaseAddress = *TopUseableMemAddr;
    GfxGsm2BaseAddress     = NaturalAlignment (GfxGsm2TempBaseAddress, SIZE_32MB);
    if (IGpuPreMemConfig->IGpuGsm2Size == 0) {
      GfxGsm2Size = SIZE_2GB;
    } else {
      TempSize    = MultU64x64 (IGpuPreMemConfig->IGpuGsm2Size, 2);
      GfxGsm2Size = MultU64x64 (TempSize, SIZE_1GB) + SIZE_2GB;
    }

    //
    // Check if the GSM2 Size is available or not
    //
    if (*Touud > GfxGsm2Size) {
      ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
      ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                          ResourceAttributeTested |
                          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;
      if (GfxGsm2TempBaseAddress != GfxGsm2BaseAddress) {
        BuildResourceDescriptorHob (
                                    ResourceType,                                 // MemoryType,
                                    ResourceAttribute,                            // MemoryAttribute
                                    GfxGsm2TempBaseAddress,                       // MemoryBegin
                                    (GfxGsm2BaseAddress - GfxGsm2TempBaseAddress) // MemoryLength
                                    );
        BuildMemoryAllocationHob (
                                  GfxGsm2TempBaseAddress,
                                  (GfxGsm2BaseAddress - GfxGsm2TempBaseAddress),
                                  EfiReservedMemoryType
                                  );
      }

      BuildResourceDescriptorHob (
                                  ResourceType,       // MemoryType,
                                  ResourceAttribute,  // MemoryAttribute
                                  GfxGsm2BaseAddress, // MemoryBegin
                                  GfxGsm2Size         // MemoryLength
                                  );
      BuildMemoryAllocationHob (
                                GfxGsm2BaseAddress,
                                GfxGsm2Size,
                                EfiReservedMemoryType
                                );
      IpIGpuGsm2Allocation (IGpuInst, GfxGsm2BaseAddress, IGpuPreMemConfig->IGpuGsm2Size);
      *TopUseableMemAddr = GfxGsm2BaseAddress + GfxGsm2Size;
      *Touud            -= (GfxGsm2Size + (GfxGsm2BaseAddress - GfxGsm2TempBaseAddress));

      IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
      if (IGpuDataHob != NULL) {
        IGpuDataHob->Gsm2BaseAddress = GfxGsm2BaseAddress;
        IGpuDataHob->Gsm2Size        = GfxGsm2Size;
        DEBUG ((DEBUG_INFO, "Gsm2BaseAddress = 0x%lx Size = 0x%lx\n", IGpuDataHob->Gsm2BaseAddress, IGpuDataHob->Gsm2Size));
      }
    } else {
      IpIGpuGsm2Allocation (IGpuInst, 0x0, 0x0);
    }
  }
}
