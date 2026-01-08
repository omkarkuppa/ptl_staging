/** @file
  Source code file for Silicon Init Pre Memory module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "SiInitPreMem.h"
#include <Library/PerformanceLib.h>
#include <Library/PeiSiPolicyOverrideLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/PeiPreMemSiDefaultPolicy.h>
#include <IntelRcStatusCode.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <DprInfoHob.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiVtdInitLib.h>
#include <Library/TelemetryPrivateLib.h>
#include <Library/PeiPcdInitLib.h>
#if FixedPcdGetBool(PcdOverclockEnable) == 1
#include <Library/PeiOcLib.h>
#endif
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/HostBridgeInfoLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <Ppi/PeiPreMemSiPolicyPrint.h>
#include <Library/CpuInitPreMem.h>
#include <Register/GenerationMsr.h>
#include <Library/PeiTseLib.h>
#include <Library/TxtPeiLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/IpuInitLib.h>
#include <Library/CnviLib.h>
#include <Library/PeiHdaLib.h>
#include <ImrConfigHob.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiVrLib.h>
#include <Library/SpiInitLib.h>
#include <Library/PeiExtGfxCardDetectLib.h>
#include <Library/PeiTelemetryFruLib.h>
#include <Library/FspCommonLib.h>
#include <FspStatusCode.h>
#if FixedPcdGet8(PcdFusaSupport) == 0x1
#include <Library/PeiFusaE2eCtcLib.h>
#endif
#include <Library/PmcPrivateLib.h>
#include <Library/CpuPowerManagementLib.h>
#include <Library/DomainIGpuInit.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <PeiITbtConfig.h>
#include <Library/PcdTcssInitLib.h>
#include <Ppi/Reset2.h>
#include <Library/CpuPmDataLib.h>
#include <Library/PeiPcieSipInitLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <P2SbController.h>
#include <FsptUpd.h>
#include <FspGlobalData.h>
#include <ConfigBlock/Ish/IshConfig.h>
#include <Library/PchInitLib.h>
#include <Library/FspPerformanceLib.h>
#include <Library/PtlCpuInfoLib.h>
#if FixedPcdGet8(PcdTccSupport) == 0x1
#include <Library/PeiTccInitLib.h>
#include <TccConfig.h>
#endif

/**
  PPV dummy reset handler.
**/
VOID
EFIAPI
PpvResetSystem2 (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "Skip reset since DisableReset is enabled.\n"));
}

STATIC EFI_PEI_RESET2_PPI mPpvPpiReset2 = {
  PpvResetSystem2
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpvPpiReset2List = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReset2PpiGuid,
  &mPpvPpiReset2
};

/**
  This function will re-install a null Reset2Ppi service if disable reset is enabled.

  @param[in] SiPreMemPolicyPpi    PreMem policy
**/
STATIC
VOID
PpvInstallReset2Ppi (
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                 Status;
  VOID                       *OldPpi;
  EFI_PEI_PPI_DESCRIPTOR     *OldPpiDescriptor;
  PCH_GENERAL_PREMEM_CONFIG  *PchGeneralPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (PchGeneralPreMemConfig->DisableResets == FORCE_ENABLE) {
    DEBUG ((DEBUG_INFO, "DisableResets is enabled. Install NULL Reset2Ppi service.\n"));

    Status = PeiServicesLocatePpi (&gEfiPeiReset2PpiGuid, 0, &OldPpiDescriptor, (VOID **) &OldPpi);
    if (EFI_ERROR (Status)) {
      PeiServicesInstallPpi (&mPpvPpiReset2List);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Install PPV Reset2Ppi failure.\n"));
      }
    } else {
      DEBUG ((DEBUG_INFO, "Re-install PPV Reset2Ppi.\n"));
      Status = PeiServicesReInstallPpi (OldPpiDescriptor, &mPpvPpiReset2List);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Re-install PPV Reset2Ppi failure.\n"));
      }
    }
  }
}

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiPreMemPolicyReadyPpiGuid,
    SiInitPreMemOnPolicy
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mMigrationMemoryListOneSilicon[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMemoryDiscoveredPpiGuid,
    ResetMemoryAddressOneSilicon
  }
};

FSP_TEMP_RAM_EXIT_PPI mTempRamExitPpi = {
  TempRamExit
};

static EFI_PEI_PPI_DESCRIPTOR  mTempRamExitPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gFspTempRamExitPpiGuid,
    &mTempRamExitPpi
  }
};

STATIC PEI_PREMEM_SI_POLICY_PRINT_PPI mPeiPreMemSiPolicyPrintPpi = {
  PeiPreMemSiPolicyPrint
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPeiPreMemSiPolicyPrintPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gSiPreMemPolicyPrintPpiGuid,
  &mPeiPreMemSiPolicyPrintPpi
};

/**
  Check some SA policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicyPpi    The SI PreMem Policy instance

**/
VOID
SaValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  ASSERT (SiPreMemPolicyPpi->TableHeader.Header.Revision == SI_PREMEM_POLICY_REVISION);
}


/**
  Check if Hd Audio is enabled

  @retval TRUE                    HDA is enabled
  @retval FALSE                   HDA PCI device not found - fuse or policy disabled
**/
STATIC
BOOLEAN
PtlPcdIsHdAudioEnabled (
  VOID
  )
{
  EFI_STATUS                Status;
  HDAUDIO_PREMEM_CONFIG     *HdaPreMemConfig;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  UINT64                    PciBase;

  PciBase = HdaPciCfgBase ();

  if (PciSegmentRead16 (PciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "HDA: PCI Device not found!\n"));
    return FALSE;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (HdaPreMemConfig->Enable) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if ISH is enabled

  @retval TRUE                    ISH is enabled
  @retval FALSE                   ISH device either fuse or policy disabled
**/
STATIC
BOOLEAN
PtlPcdIsIshEnabled (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  ISH_PREMEM_CONFIG         *IshPreMemConfig;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  return (BOOLEAN) IshPreMemConfig->Enable;
}

/**
  Install IMR config
**/
VOID
InstallImrConfigHob (
  VOID
  )
{
  IMR_CONFIG_HOB *ImrConfigHob;

  ImrConfigHob = BuildGuidHob (
                   &gImrConfigGuid,
                   sizeof (IMR_CONFIG_HOB)
                   );

  if (ImrConfigHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate IMR CONFIG HOB\n"));
    return;
  }

  ImrConfigHob->BiosImrDisableMask0 = 0xFFFFFFFF;
  ImrConfigHob->BiosImrDisableMask1 = 0xFFFFFFFF;
  ImrConfigHob->PcieImrSize = 0;

  ImrConfigHob->BiosImrDisableMask0 &= (UINT32) ~(BIT5);    // CSME

  if (IsSamEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32) ~(BIT4);  // SAM
  }

  if (PtlPcdIsIshEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32) ~(BIT0);  // ISH
  }

  if (PtlPcdIsHdAudioEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32) ~(BIT10); // Audio
  }

  if (IsIpuEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32)~(BIT1);  //IPU Camera
  }

  //
  // TCSS IMR need to be always enabled since common b/w iTBT & IOM
  //
  if (IsItcssEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32)~(BIT14);  // TCSS(iTBT+IOM)
    DEBUG ((DEBUG_INFO, "IMR INFO: TCSS(iTBT+IOM) Disable BIT Mask in BiosImrDisableMask0 is Clear\n"));
  }

  if (IsCnviWifiEnabled () || IsCnviBtEnabled ()) {
    ImrConfigHob->BiosImrDisableMask0 &= (UINT32)~(BIT12); // CNVi
  }
}

/**
  Slicon Initializes after Policy PPI produced, All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             Succeeds.
**/
EFI_STATUS
EFIAPI
SiInitPreMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  EFI_STATUS                    Status;
  SI_POLICY_PPI                 *SiPolicyPpi;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi;
  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig;
  HOST_BRIDGE_DATA_HOB          *HostBridgeDataHob;
  DPR_INFO_HOB                  *DprInfoHob;
  TCSS_PEI_PREMEM_CONFIG        *TcssPeiPreMemConfig;
#if FixedPcdGetBool(PcdHgEnable) == 1
  HYBRID_GRAPHICS_CONFIG        *HgGpioData;
#endif
  IGPU_PEI_PREMEM_CONFIG        *IGpuPreMemConfig;
  DISPLAY_DEVICE                PrimaryDisplay;
  UINT32                        IGpuMmioLength;
  VTD_CONFIG                    *VtdConfig;
  MSR_FLEX_RATIO_REGISTER       MsrFlexRatio;
  TELEMETRY_PEI_PREMEM_CONFIG   *TelemetryPreMemConfig;
  UINT32                        PchPcieMmioLength;
  UINT32                        PegMmioLength;
  CPU_INIT_PREMEM_CONFIG        *CpuInitPreMemConfig;
  CPU_POWER_DELIVERY_CONFIG     *CpuPowerDeliveryConfig;
  CPU_POWER_MGMT_VR_CONFIG      *CpuPowerMgmtVrConfig;
  CPU_SECURITY_PREMEM_CONFIG    *CpuSecurityPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG     *PcieRpPreMemConfig;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  //
  // Prevent from potential execution in PostMem phase
  //
  if (Status == EFI_SUCCESS) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = PeiPreMemSiPolicyPrint ();
  ASSERT_EFI_ERROR (Status);

  HostBridgePreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  TcssPeiPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool(PcdHgEnable) == 1
  HgGpioData = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHybridGraphicsConfigGuid, (VOID *) &HgGpioData);
  ASSERT_EFI_ERROR (Status);
#endif
  IGpuPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  VtdConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  ASSERT_EFI_ERROR (Status);

  TelemetryPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  CpuInitPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  CpuPowerDeliveryConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);

  CpuPowerMgmtVrConfig = NULL;
  Status = GetConfigBlock ((VOID*) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID*) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);

  CpuSecurityPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  PcieRpPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = CreateCpuPmDataHob (CpuPowerMgmtVrConfig->VrPowerDeliveryDesign);
  DEBUG ((DEBUG_INFO, "CreateCpuPmDataHob return status = 0x%x\n", Status));

  PrimaryDisplay = DISPLAY_IGD;
  IGpuMmioLength = 0;
  PchPcieMmioLength = 0;
  PegMmioLength = 0;

  PpvInstallReset2Ppi (SiPreMemPolicyPpi);

  //
  // Cross IP Policy override for specific feature enabling
  //
  PeiSiPolicyOverridePreMem (SiPreMemPolicyPpi);

#ifndef MDEPKG_NDEBUG
  //
  // Validate PCH policies
  //
  PERF_INMODULE_BEGIN ("PchValidatePolicy");
  PtlPcdValidatePolicy (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("PchValidatePolicy");

  //
  // Validate CPU policies
  //
  PERF_INMODULE_BEGIN ("CpuValidatePolicy");
  CpuValidatePolicy (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("CpuValidatePolicy");

  //
  // Validate ME policies
  //
  PERF_INMODULE_BEGIN ("MeValidatePolicy");
  MeValidatePolicy (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("MeValidatePolicy");

  //
  // Validate SA policies
  //
  PERF_INMODULE_BEGIN ("SaValidatePolicy");
  SaValidatePolicy (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("SaValidatePolicy");

#endif // MDEPKG_NDEBUG
  //
  // Dfd power and clock configuration
  //
  DfdConfiguration (SiPreMemPolicyPpi);
  //
  // Initialize PCH after Policy PPI produced
  //
  PERF_INMODULE_BEGIN ("PchOnPolicyInstalled");
  PtlPcdOnPolicyInstalled (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("PchOnPolicyInstalled");
  //
  // Initialize PSMI handler/trace sizes before MRC
  //
  PsmiInit (SiPreMemPolicyPpi);
  //
  // Initialize CPU after Policy PPI produced
  //
  PERF_INMODULE_BEGIN ("CpuOnPolicyInstalled");
  CpuOnPolicyInstalled (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("CpuOnPolicyInstalled");

  ///
  /// Program the OverClocking Lock Bit before 2nd patch load
  ///
  MsrFlexRatio.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  MsrFlexRatio.Bits.OcLock = CpuInitPreMemConfig->OcLock;
  AsmWriteMsr64 (MSR_FLEX_RATIO, MsrFlexRatio.Uint64);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  ///
  /// Init Overclocking
  ///
  PERF_INMODULE_BEGIN ("CpuOcInitPreMem");
  CpuOcInitPreMem (SiPreMemPolicyPpi);
  PERF_INMODULE_END ("CpuOcInitPreMem");
#endif

  ///
  /// Program platform power and SVID VR's
  ///
  ConfigureSvidVrs (SiPreMemPolicyPpi);

  ///
  /// Initializes Cpu Power Delivery policy at PreMem phase
  ///
  CpuPowerDeliveryInit (CpuPowerDeliveryConfig);

  ///
  /// Send Early PM Configuration done through mailbox
  ///
  SendPowerDeliveryDone ();

  ///
  /// Initializes TSE after Policy PPI produced
  ///
  TseInit (CpuSecurityPreMemConfig);

  ///
  ///Enable all DLVR's PHASE_SSC feature
  ///
  SetDlvrPhaseSsc (CpuPowerMgmtVrConfig);

  ///
  /// Initializes TXT after Policy PPI produced
  ///
  DEBUG ((DEBUG_INFO, "TxtInit Start \n"));
  TxtInit ();

  //
  // Lock pmc set strap.
  //
  PmcLockSetStrapMsgInterface ();

  //
  // Initialize ME after Policy PPI produced
  //
  PERF_INMODULE_BEGIN ("MeOnPolicyInstalled");
  MeOnPolicyInstalled ();
  PERF_INMODULE_END ("MeOnPolicyInstalled");

  //
  // Initialize SA after Policy PPI produced
  //
  PERF_INMODULE_BEGIN ("SaOnPolicyPpiProduced");

  ///
  /// Program Host Bridge Bar Registers
  ///
  DEBUG ((DEBUG_INFO, "Programming Host Bridge Bars\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_PROGRAM_SA_BARS); //PostCode (0xA06)
  ProgramHostBridgeBars ();
  ///
  /// Install Host Bridge Data HOB
  ///
  InstallHostBridgeDataHob (HostBridgePreMemConfig, &HostBridgeDataHob);
  ///
  /// Enable VLW's
  ///
  VlwEnable (HostBridgePreMemConfig);
  ///
  /// Install DPR Info HOB
  ///
  DEBUG ((DEBUG_INFO, "Install DPR Info HOB\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_INSTALL_SA_HOBS); //PostCode (0xA08)
  InstallDprInfoHob (&DprInfoHob);
  ///
  /// Initialize TCSS PreMem
  ///
  if (IsItcssEnabled ()) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_TCSS_INIT_PREMEM); //PostCode (0xA09)
    PtlTcssInitPreMem (TcssPeiPreMemConfig, HostBridgePreMemConfig);
  }

#if FixedPcdGet8(PcdFusaSupport) == 0x1
  ///
  /// Install Fusa Info Hob if diagnostic mode is enabled
  ///
  DEBUG ((DEBUG_INFO, "Initializing Fusa Hob\n"));
  InstallFusaInfoHob();
#endif

  DEBUG ((DEBUG_INFO, "Create IGPU Data Hob\n"));
  CreateIGpuDataHob (IGpuPreMemConfig);

  //
  // IGPU PreMem Init for Policy
  //
  IGpuPreMemInit (IGpuXe3);

  InstallImrConfigHob ();

  ///
  /// Graphics Detection (IGD/PEG/PCH)
  ///
  DEBUG ((DEBUG_INFO, "Graphics detection\n"));
  if (PcieRpPreMemConfig->SkipExtGfxScan == DISABLED) {
    DEBUG ((DEBUG_INFO, "CheckOffboardPcieVga\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_CHK_OFFBOARD_PCIEVGA); //PostCode (0xA42)

    CheckOffboardPcieVga (
      &PchPcieMmioLength,
      &PrimaryDisplay,
      PcieRpPreMemConfig->PcieResizableBarSupport
      );
  }

  ///
  /// Initialize Graphics (IGD)
  ///
  DEBUG ((DEBUG_INFO, "Initializing IGPU\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_GFX_INIT); //PostCode (0xA50)

  IGpuInit (IGpuPreMemConfig, &PrimaryDisplay, &IGpuMmioLength);

  DEBUG ((DEBUG_INFO, "Initializing Pre-Memory Display \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_DISPLAY_INIT_PREMEM); //PostCode (0xA0B)

  IGpuDisplayInitPreMem (IGpuPreMemConfig);
  ///
  /// Initialize VT-d
  ///
  DEBUG ((DEBUG_INFO, "Initializing Vtd\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_VTD_INIT); //PostCode (0xA51)

  VtdInit (VtdConfig);
  ///
  /// Calculate required MMIO size
  ///
  MmioSizeCalculation (PchPcieMmioLength, PegMmioLength, IGpuMmioLength);

  PERF_INMODULE_END ("SaOnPolicyPpiProduced");

  //
  // Register MRC initialization callback when gEfiPeiMasterBootModePpiGuid is installed.
  //
  PERF_INMODULE_BEGIN ("InstallMrcCallback");
  InstallMrcCallback ();
  PERF_INMODULE_END ("InstallMrcCallback");

  PERF_INMODULE_BEGIN ("PtlPcdOnMemoryDiscovered");
  PtlPcdOnMemoryDiscovered();
  PERF_INMODULE_END ("PtlPcdOnMemoryDiscovered");

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  PPI function to install default ConfigBlock Policy PPI.

  @retval EFI_STATUS       - Status from each sub function.
**/
EFI_STATUS
EFIAPI
PeiPreMemSiDefaultPolicyInit (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_PPI_DESCRIPTOR                *SiPolicyPreMemPpiDesc;
  SI_PREMEM_POLICY_PPI                  *SiPreMemPolicyPpi;

  SiPreMemPolicyPpi = NULL;
  Status = SiCreatePreMemConfigBlocks (&SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (SiPreMemPolicyPpi != NULL) {

    SiPolicyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (SiPolicyPreMemPpiDesc == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    SiPolicyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    SiPolicyPreMemPpiDesc->Guid  = &gSiPreMemPolicyPpiGuid;
    SiPolicyPreMemPpiDesc->Ppi   = SiPreMemPolicyPpi;
    //
    // Install Silicon Policy PPI
    //
    Status = PeiServicesInstallPpi (SiPolicyPreMemPpiDesc);
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

PEI_PREMEM_SI_DEFAULT_POLICY_INIT_PPI mPeiPreMemSiDefaultPolicyInitPpi = {
  PeiPreMemSiDefaultPolicyInit
};

static EFI_PEI_PPI_DESCRIPTOR  mPeiPreMemSiDefaultPolicyInitPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiPreMemDefaultPolicyInitPpiGuid,
    &mPeiPreMemSiDefaultPolicyInitPpi
  }
};

typedef struct {
  PCIE_ROOT_PORT_DEV             RpDev;
  PCI_CONFIG_SPACE_ACCESS        PcieConfigSpaceAccess;
  PCI_CONFIG_SPACE_ACCESS        EpPciRegAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbMsgCfgAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbMsgMemAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbPcrAccess;
  P2SB_CONTROLLER                P2SbController;
  UINT32                         RpIndex;
} PCIE_ROOT_PORT_DEV_PRIVATE;

typedef struct {
  PCIE_ROOT_PORT_LIST         RpList;
  UINT32                      CurrentIndex;
  UINT32                      NumOfElements;
  PCIE_ROOT_PORT_DEV_PRIVATE  RootPorts[PCH_MAX_PCIE_ROOT_PORTS];
} PCIE_ROOT_PORT_LIST_PRIVATE;

typedef struct {
  UINT32    FvOrgBase;         // original FV address
  UINT32    FvNewBase;         // new FV address
  UINT32    FvDataBase;        // original FV data
  UINT32    FvLength;          // Fv Length
} EDKII_MIGRATED_FV_INFO;

/**
  Migrate the pointer between flash address to memory address
  Leverage the original HOB created by Core.
  @param[in, out] FlashPointer Pointer to flash address will result page fault
**/
STATIC
VOID
MigrateFlashPointer (
  IN OUT UINTN  *FlashPointer
  )
{
  VOID                    *Hob;
  EDKII_MIGRATED_FV_INFO  *MigratedFvInfo;

  Hob  = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);

  while (Hob != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);

    if ((*FlashPointer > MigratedFvInfo->FvOrgBase) &&
        (*FlashPointer < (MigratedFvInfo->FvOrgBase + MigratedFvInfo->FvLength - 1))) {
      // Migrate the pointer
      if (MigratedFvInfo->FvOrgBase >= MigratedFvInfo->FvNewBase) {
        *FlashPointer = (UINT32) *FlashPointer + (MigratedFvInfo->FvNewBase - MigratedFvInfo->FvOrgBase);
      } else {
        *FlashPointer = (UINT32) *FlashPointer - (MigratedFvInfo->FvOrgBase - MigratedFvInfo->FvNewBase);
      }

      // break after migrate successfully
      break;
    }

    Hob = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, GET_NEXT_HOB (Hob));
  }
}

/**
  Check the FV to cover target flash address is already migrated or not.

  @param[in, out] FlashBaseAddress  The FV for target flash address to check.

  @retval TRUE             The region pointed by flash pointer is already migraetd.
  @retval FALSE            The region pointed by flash pointer is NOT migraetd.
**/
BOOLEAN
IsFlashAddressMigrated (
  IN EFI_PHYSICAL_ADDRESS    FlashBaseAddress
  )
{
  VOID                    *Hob;
  EDKII_MIGRATED_FV_INFO  *MigratedFvInfo;

  Hob  = GetFirstGuidHob (&gEdkiiMigratedFvInfoGuid);
  while (Hob != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);

    if ((FlashBaseAddress >= MigratedFvInfo->FvOrgBase) &&
        (FlashBaseAddress < (MigratedFvInfo->FvOrgBase + MigratedFvInfo->FvLength - 1))) {
      return TRUE;
    }
    Hob = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, GET_NEXT_HOB (Hob));
  }

  return FALSE;
}

/**
  TOC/TOU migration issues are maintained by this function for OneSiliconPkg.
  Find the memory address or pointer then call MigrateFlashPointer() for processing.
  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  PPI.
**/
EFI_STATUS
EFIAPI
ResetMemoryAddressOneSilicon (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  EFI_STATUS                     Status;
  PCIE_ROOT_PORT_LIST_PRIVATE    *RpListPrivate;
  PCIE_ROOT_PORT_LIST            *RpList;
  EFI_GUID  gPtlPcdRpListPpiGuid = {0x55F621C1, 0xAEFE, 0x4712, {0x97, 0x96, 0xE3, 0xDC, 0x4D, 0xE4, 0x17, 0xC9}};

  VOID                           *FspGlobalDataInitPpi;
  FSP_GLOBAL_DATA                *FspData;
  EFI_PHYSICAL_ADDRESS           FsptUpdAddress;

  PERF_INMODULE_BEGIN ("ResetMemAddressOneSi");
  FspData = NULL;

  DEBUG ((DEBUG_INFO, "Reset RpList\n"));
  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (!EFI_ERROR (Status)) {
    RpList = &RpListPrivate->RpList;
    MigrateFlashPointer ((UINTN*)&RpList->ResetToFirst);
    MigrateFlashPointer ((UINTN*)&RpList->GetNextRootPort);
    MigrateFlashPointer ((UINTN*)&RpList->GetThisController);
    MigrateFlashPointer ((UINTN*)&RpList->GetRootPortOnNextDevice);
    MigrateFlashPointer ((UINTN*)&RpList->GetRootPortOnNextFunction);
    MigrateFlashPointer ((UINTN*)&RpList->GetNextRootPortInThisController);
    MigrateFlashPointer ((UINTN*)&RpList->GetNextController);
  }

  Status = PeiServicesLocatePpi (
             &gFspGlobalDataInitPpiGuid,
             0,
             NULL,
             (VOID **) &FspGlobalDataInitPpi
             );
  if (!EFI_ERROR (Status)) {
    FspData = (FSP_GLOBAL_DATA *) FspGlobalDataInitPpi;
    //
    // If TempRamInitUpd is already migrated, reset the pointer in FspData
    //
    if (IsFlashAddressMigrated ((EFI_PHYSICAL_ADDRESS)(UINTN) FspData->TempRamInitUpdPtr)) {
      DEBUG ((DEBUG_INFO, "AllocatePages for FsptUpdAddress\n"));
      Status = PeiServicesAllocatePages (
                 EfiBootServicesData,
                 EFI_SIZE_TO_PAGES (sizeof (FSPT_UPD)),
                 &FsptUpdAddress
                 );
      if (!EFI_ERROR (Status)) {
        CopyMem ((VOID *)(UINTN) FsptUpdAddress, FspData->TempRamInitUpdPtr, sizeof (FSPT_UPD));
        FspData->TempRamInitUpdPtr = (FSPT_UPD *)(UINTN) FsptUpdAddress;
        SetFspGlobalDataPointer (FspData);
      }
    }
  }

  PERF_INMODULE_END ("ResetMemAddressOneSi");
  return EFI_SUCCESS;
}

/**
  Silicon Initializes before Policy PPI produced

  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
SiInitPrePolicy (
  IN  EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                            Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  //MemoryInit Phase Postcode set
  SetPhaseStatusCode(FSP_STATUS_CODE_MEMORY_INIT);

  //
  // Create FSP Performance Hob
  //
  Status = CreateFspPerformanceHob();
  ASSERT_EFI_ERROR (Status);

  Status = PtlPcdInstallDieInfo ();
  ASSERT_EFI_ERROR (Status);

  Status = CpuInstallDieInfo ();
  ASSERT_EFI_ERROR (Status);

  //
  // Install SPI Info HOB
  //
  InstallSpiInfoHob ();

  //
  // Initializes PCH before Policy initialized
  //
  PERF_START_EX (&gPerfPchPrePolicyGuid, NULL, NULL, AsmReadTsc (), 0x50F0);
  PtlPcdInitPrePolicy ();
  PERF_END_EX (&gPerfPchPrePolicyGuid, NULL, NULL, AsmReadTsc (), 0x50F1);

  //
  // Register Silicon init call back after PlatformPolicy PPI produced
  //
  Status = PeiServicesNotifyPpi (mSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  //
  // Migration Silicon memory address
  //
  if (PcdGetBool (PcdMigrateTemporaryRamFirmwareVolumes)) {
    Status = PeiServicesNotifyPpi (mMigrationMemoryListOneSilicon);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Install Silicon ProgramMtrr PPI
  //
  Status = PeiServicesInstallPpi (mTempRamExitPpiList);
  ASSERT_EFI_ERROR (Status);

  //
  // Install a Default Policy initialization PPI
  //
  Status = PeiServicesInstallPpi (mPeiPreMemSiDefaultPolicyInitPpiList);
  ASSERT_EFI_ERROR (Status);

  //
  // Install the Si PreMem Policy print PPI
  //
  Status = PeiServicesInstallPpi (&mPeiPreMemSiPolicyPrintPpiList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

