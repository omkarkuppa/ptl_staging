/** @file
  Source code file for Silicon Init Post Memory module.

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

#include "SiInit.h"
#include <Library/PerformanceLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiSiPolicyOverrideLib.h>
#include <IntelRcStatusCode.h>
#include <Library/IpuInitLib.h>
#include <Library/PeiSiSsidLib.h>
#include <Register/VoltageRegulatorRegs.h>
#include <Library/PeiVrPolicyLib.h>
#include <Library/PeiVrLib.h>
#include <Register/B2pMailbox.h>
#include <Defines/IgdDefines.h>
#include <Library/FspCommonLib.h>
#include <FspStatusCode.h>
#include <Library/SiPolicyLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/PeiSiDefaultPolicy.h>
#include <Ppi/MpServices2.h>
#include <Library/PciSegmentLib.h>
#include <PcieRegs.h>
#include <VtdDataHob.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiExtGfxCardDetectLib.h>
#include <Library/PeiVtdInitLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/ConfigBlockLib.h>
#include <GpioV2Functionalities.h>
#include <Library/PeiVmdInitLib.h>
#include <Library/TelemetryPrivateLib.h>
#include <TelemetryPeiConfig.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <Library/MsrFruLib.h>
#include <Library/PeiNpuInitLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiIaxInitLib.h>
#if FixedPcdGetBool(PcdOverclockEnable) == 1
#include <Library/PeiOcLib.h>
#endif
#include <Library/PeiNguInitLib.h>
#include <Library/PeiTprInitLib.h>

#include <Library/CpuInitPostMem.h>
#include <Ppi/PeiSiPolicyPrint.h>
#include <Library/CpuPowerManagementLib.h>
#include <Library/BiosGuardInit.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/PeiDmiInitLib.h>
#include <Library/SiFviLib.h>
#include <Library/PmonLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Library/MtrrLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/PeiTelemetryFruLib.h>
#include <Library/DomainIGpuInit.h>
#include <Library/IGpuInfoLib.h>
#include <PeiITbtConfig.h>
#include <Library/ThcInitSsidHob.h>
#include <Library/PcdTcssInitLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Library/PeiPcdInitLib.h>
#include <Library/PeiImrInitLib.h>
#include <Library/VmdInfoLib.h>
#include <Register/CpuGenInfoRegs.h>
#include <Library/CpuLib.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <TraceHubDataHob.h>
#include <Library/DomainPcie.h>
#include <IpCpcie.h>
#include <IpPcieRegs.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Library/PeiVmdInitFruLib.h>

extern EFI_GUID gIpPcieInstHobGuid;

EFI_PEI_PPI_DESCRIPTOR mEndOfSiInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEndOfSiInitPpiGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR  mEnablePeiGraphicsPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEnablePeiGraphicsPpiGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR  mEnableFspPeiGraphicsPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEnableFspPeiGopPpiGuid,
  NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    SiInitOnEndOfPei
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitPostMemNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiPolicyReadyPpiGuid,
    SiInitPostMemOnPolicy
  }
};

STATIC PEI_SI_POLICY_PRINT_PPI mPeiSiPolicyPrintPpi = {
  PeiSiPolicyPrint
};

STATIC EFI_PEI_PPI_DESCRIPTOR  mPeiSiPolicyPrintPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gSiPolicyPrintPpiGuid,
  &mPeiSiPolicyPrintPpi
};

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mOnPciEnumerationCompleteNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPciEnumerationCompleteProtocolGuid,
    SiOnPciEnumerationComplete
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfFirmwareNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gFspEventEndOfFirmwareGuid,
    SiOnEndOfFirmware
  }
};

/**
  PPI function to install default ConfigBlock Policy PPI.

  @retval EFI_STATUS       - Status from each sub function.
**/
EFI_STATUS
EFIAPI
PeiSiDefaultPolicyInit (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_PPI_DESCRIPTOR                *SiPolicyPpiDesc;
  SI_POLICY_PPI                         *SiPolicyPpi;

  SiPolicyPpi = NULL;
  Status = SiCreateConfigBlocks (&SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (SiPolicyPpi != NULL) {
    SiPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (SiPolicyPpiDesc == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    SiPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    SiPolicyPpiDesc->Guid  = &gSiPolicyPpiGuid;
    SiPolicyPpiDesc->Ppi   = SiPolicyPpi;
    //
    // Install Silicon Policy PPI
    //
    Status = PeiServicesInstallPpi (SiPolicyPpiDesc);
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

PEI_SI_DEFAULT_POLICY_INIT_PPI mPeiSiDefaultPolicyInitPpi = {
  PeiSiDefaultPolicyInit
};

static EFI_PEI_PPI_DESCRIPTOR  mPeiSiDefaultPolicyInitPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSiDefaultPolicyInitPpiGuid,
    &mPeiSiDefaultPolicyInitPpi
  }
};

EFI_PEI_PPI_DESCRIPTOR      mIDispCodecReadyPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gIDispCodecReadyPpiGuid,
  NULL
};

/**
  Silicon Init End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.

  @param[in] PeiServices   - Pointer to PEI Services Table.
  @param[in] NotifyDesc    - Pointer to the descriptor for the Notification event that
                             caused this function to execute.
  @param[in] Ppi           - Pointer to the PPI data associated with this function.

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
SiInitOnEndOfPei (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS                  Status;
  SI_POLICY_PPI               *SiPolicy;
  SI_CONFIG                   *SiConfig;
  CPU_INIT_CONFIG             *CpuInitConfig;
  EFI_BOOT_MODE               BootMode;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicy;
  HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig;
  IGPU_PEI_PREMEM_CONFIG      *IGpuPreMemConfig;
  IGPU_PEI_CONFIG             *IGpuConfig;
  CPU_POWER_DELIVERY_CONFIG   *CpuPowerDeliveryConfig;

  //
  // Get Policy settings through the SiPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if ((Status != EFI_SUCCESS) || (SiPolicy == NULL)) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  //
  // Get Si PreMem Policy settings through the SiPreMemPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    ASSERT (FALSE);
    return EFI_SUCCESS;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gCpuInitConfigGuid, (VOID *)&CpuInitConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHostBridgePeiConfigGuid, (VOID *) &HostBridgePeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInitOnEndOfPei - Start\n"));

  //
  // Initializes PCH after End of Pei
  //
  PtlPcdOnEndOfPei ();

  DEBUG ((DEBUG_INFO, "SubsystemID programming on Pch Pcie rootports\n"));
  SiPcieProgramSsid (SiConfig);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Set WAC and CP for IMRGLOBAL_BM\n"));
  SetImrGlobalSai ();

  if ((BootMode == BOOT_ON_FLASH_UPDATE) && (SiConfig->SkipBiosDoneWhenFwUpdate)) {
    DEBUG ((DEBUG_INFO, "Skip BIOS_DONE when updating flash.\n"));
  } else {
    //
    // Set BIOS DONE MSR on all Cores
    //
      SetBiosDone ();
  }

  CpuInitAtEndOfPei ();

  //
  // Set BIOS_RESET_CPL to indicate BIOS initialization completed
  //
  PERF_START_EX (&gPerfSaResetPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4090);

  ///
  /// Set BIOS_RESET_CPL
  ///
  SetBiosResetCpl ();

  ///
  /// Graphics PM initialization after BIOS_RESET_CPL
  ///
  DEBUG ((DEBUG_INFO, "IGpuPmInit Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_IGPU_PM_INIT_START); //PostCode (0xA63)
  IGpuPmInit (IGpuPreMemConfig, IGpuConfig);

  ///
  /// Enable DMA buffer for IGD VT-d
  ///
  DEBUG ((DEBUG_INFO, "Enabling DMA buffer for IGD VT-d\n"));
  VtdEnableDmaBuffer (GFX_VTD);

  ///
  /// Initialize PEI Display
  ///
  DEBUG ((DEBUG_INFO, "Initializing Pei Display\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_PEI_DISPLAY_INIT); //PostCode (0xA03)
  IGpuPeiDisplayInit (IGpuPreMemConfig, IGpuConfig);
  PeiServicesInstallPpi (&mIDispCodecReadyPpi);

  ///
  /// Print HostBridge PCI space in Debug log.
  ///
  DEBUG ((DEBUG_INFO, "PciPrint Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_SAPCIPRINT_START); //PostCode (0xA64)
  HostBridgePciPrint ();
  ///
  /// Print Graphics PCI space in Debug log.
  ///
  IGpuPrintPciConfig ();

  PERF_END_EX (&gPerfSaResetPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4091);

  //
  // Initialize power management after RESET_CPL at post-memory phase.
  //
  PERF_START_EX (&gPerfCpuPowerMgmtGuid, NULL, NULL, AsmReadTsc (), 0x40A0);
  CpuPowerMgmtInit (CpuPowerDeliveryConfig);
  PERF_END_EX (&gPerfCpuPowerMgmtGuid, NULL, NULL, AsmReadTsc (), 0x40A1);

  //
  // Lock PAM register at End Of PEI.
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_LOCK_PAM_ENDOFPEI); //PostCode (0xA70)
  //
  // Clear Graphics Temp BARs at End Of PEI
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_S3_RESUME_CALLBACK_EXIT); //PostCode (0xA7F)
  IGpuEndOfPei ();

  //
  // PMON populate discovery table
  //
  PmonPopulateDiscoveryTable();

  //
  // Build FVI Info HOB in normal boot
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    BuildFviInfoHob ();
  }

  InitializeSmbiosCpuHobs ();

  Status = PeiServicesNotifyPpi (mOnPciEnumerationCompleteNotifyList);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesNotifyPpi (mEndOfFirmwareNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInitOnEndOfPei - End\n"));

#if(FixedPcdGetBool(PcdFspConfigHobsEnable))
  DEBUG ((DEBUG_INFO, "SiPolicy       Address: is %8x\n", SiPolicy));
  DEBUG ((DEBUG_INFO, "SiPreMemPolicy Address: is %8x\n", SiPreMemPolicy));
  BuildGuidDataHob (
    &gSiPolicyHobGuid,
    (VOID *) SiPolicy,
    SiPolicy->TableHeader.Header.GuidHob.Header.HobLength
    );

  BuildGuidDataHob (
    &gSiPreMemPolicyHobGuid,
    (VOID *) SiPreMemPolicy,
    SiPreMemPolicy->TableHeader.Header.GuidHob.Header.HobLength
    );
  DEBUG ((DEBUG_INFO, "FSP Configuration HOBs are created\n"));
#endif

  PlatformLateInit ();
  return EFI_SUCCESS;
}

/**
  Get HybridGraphics subsystem ID.

  @retval  HybridGraphics subsystem ID
**/
STATIC
UINT16
GetHgSsid (
  VOID
  )
{
  EFI_STATUS                    Status;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi;
  HYBRID_GRAPHICS_CONFIG        *HgGpioData;
  UINT16                        HgSubSystemId;

  SiPreMemPolicyPpi = NULL;
  HgGpioData = NULL;
  HgSubSystemId = 0;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );

  if (!EFI_ERROR (Status) && (SiPreMemPolicyPpi != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHybridGraphicsConfigGuid, (VOID *) &HgGpioData);
    if (HgGpioData != NULL) {
      HgSubSystemId = HgGpioData->HgSubSystemId;
    }
  }
  return HgSubSystemId;
}

/**
  Override HG SVID and SSID

  @param[in]      PciDevNum       Pci device number
  @param[in]      PciFuncNum      Pci function number
  @param[in,out]  Svid            Svid value
  @param[in,out]  Ssid            Ssid value

  @retval         TRUE            Silicon overrides the SSID
  @retval         FALSE           Silicon doesn't override the SSID
**/
STATIC
BOOLEAN
HgSsidOverride (
  UINT32                 PciDevNum,
  UINT32                 PciFuncNum,
  UINT16                 *Svid,
  UINT16                 *Ssid
  )
{
  UINT16    HgSsid;

  if ((PciDevNum == IGpuGetDevNumber ()) && (PciFuncNum == IGpuGetFuncNumber ())) {
    HgSsid = GetHgSsid ();
    if (HgSsid != 0) {
      *Ssid = HgSsid;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Initializes TraceHub in PostMem

  @param[in] SiPreMemPolicyPpi        The SI PreMem Policy PPI instance
**/
VOID
TraceHubConfigPostMem (
  IN  SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi
  )
{
  TRACEHUB_HANDLE                    TraceHubHandle;
  TRACEHUB_CONTROLLER                TraceHubController;
  TRACEHUB_PRIVATE_CONFIG            PrivateConfig;
  TRACEHUB_CALLBACK                  Callback;
  P2SB_CONTROLLER                    P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS      TraceHubSbAccessMmio;
  UINT64                             TraceHubMemBase;
  TRACEHUB_DATA_HOB                  *TraceHubDataHob;

  TraceHubMemBase = 0;
  TraceHubDataHob = NULL;

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);

  if (TraceHubDataHob != NULL) {
    DEBUG ((DEBUG_INFO, "TraceHub memory base = 0x%lx Size = 0x%lx\n", TraceHubDataHob->TraceHubMemBase, TraceHubDataHob->TraceHubMemSize));
    if ((TraceHubDataHob->TraceHubMemBase > 0) && (TraceHubDataHob->TraceHubMemSize > 0)) {
      TraceHubMemBase = TraceHubDataHob->TraceHubMemBase;
    }
  }

  TraceHubInitSocHandle (
    SiPreMemPolicyPpi,
    &TraceHubHandle,
    &TraceHubController,
    &PrivateConfig,
    &Callback,
    &P2SbController,
    &TraceHubSbAccessMmio
    );
  TraceHubInitPostMem (&TraceHubHandle, &TraceHubMemBase); //@todo: Need implement PchTraceHubHandle
}

/**
  Slicon Initializes after PostMem phase Policy PPI produced,
  All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  SiPolicy PPI.

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_UNSUPPORTED         The function failed to locate SiPolicy
**/
EFI_STATUS
EFIAPI
SiInitPostMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  SI_POLICY_PPI               *SiPolicy;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
  VOID                        *HobPtr;
  EFI_STATUS                  Status;
  CPU_INIT_CONFIG             *CpuInitConfig;
  SI_CONFIG                   *SiConfig;
  HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig;
  IGPU_PEI_PREMEM_CONFIG      *IGpuPreMemConfig;
  IGPU_PEI_CONFIG             *IGpuConfig;
  VTD_CONFIG                  *Vtd;
  MEMORY_CONFIGURATION        *MemConfig;
  TELEMETRY_PEI_CONFIG        *TelemetryPeiConfig;
  NPU_PEI_CONFIG              *NpuPeiConfig;
  VMD_PEI_CONFIG              *VmdPeiConfig;

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OVERCLOCKING_PREMEM_CONFIG  *OverClockingConfig;
#endif
  EDKII_PEI_MP_SERVICES2_PPI  *MpServices2Ppi;
  CPU_POWER_DELIVERY_CONFIG   *CpuPowerDeliveryConfig;
  TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig;
  TCSS_PEI_CONFIG             *TcssPeiConfig;
  PEI_ITBT_CONFIG             *PeiITbtConfig;
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig;
  IP_PCIE_INST                *pInst;
  EFI_HOB_GUID_TYPE           *GuidHob;
  UINT8                       MaxLinkSpeed;
  IP_WR_REG_INFO              *RegInfo;
  UINT64                      RpBase;
  LCAP_PCIE_CFG_STRUCT        Lcap;
  PCIE_DEV_INFO               DevInfo;

  DEBUG ((DEBUG_INFO, "SiInit () - Start\n"));

  SiPreMemPolicyPpi      = NULL;
  SiPolicy               = NULL;
  VmdPeiConfig           = NULL;
  HostBridgePeiConfig    = NULL;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OverClockingConfig     = NULL;
#endif
  MpServices2Ppi         = NULL;
  CpuPowerDeliveryConfig = NULL;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPolicy == NULL)) {
    return EFI_UNSUPPORTED;
  }

  Status = PeiSiPolicyPrint ();
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR(Status);

  ///
  /// Locate CpuMpCpu MpService Ppi
  ///
  Status = PeiServicesLocatePpi (
              &gEdkiiPeiMpServices2PpiGuid,
              0,
              NULL,
              (VOID **) &MpServices2Ppi
              );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHostBridgePeiConfigGuid, (VOID *) &HostBridgePeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &Vtd);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gGraphicsPeiConfigGuid, (VOID *) &IGpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  Status = GetConfigBlock ((VOID *) SiPolicy, &gVmdPeiConfigGuid, (VOID *) &VmdPeiConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  Status = GetConfigBlock ((VOID *) SiPolicy, &gTelemetryPeiConfigGuid, (VOID *) &TelemetryPeiConfig);
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OverClockingConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  Status = GetConfigBlock ((VOID *) SiPolicy, &gNpuPeiConfigGuid, (VOID *) &NpuPeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Initilizing Post Mem IGPU IP Instances
  //
  IGpuPostMemInit ();

  //
  // Program PMON address,
  //
  PmonMemoryAddressProgram (SiPolicy);

  //
  // PSMI Configuration
  //
  ConfigurePsmi ();
  //
  // Install Mbp HOB before policy override to correctly config policy
  //
  InstallMbpHob ();
  //
  // Cross IP Policy override for specific feature enabling
  //
  PeiSiPolicyOverride (SiPreMemPolicyPpi, SiPolicy);
  HobPtr = BuildGuidDataHob (&gSiConfigHobGuid, SiConfig, sizeof (SI_CONFIG));
  ASSERT (HobPtr != 0);

  //
  // Initialize THC SSID hob
  //
  ThcInstallSubSystemIdHob (SiPolicy);

  ///
  /// SubsystemID programming on Internal Devices.
  ///
  DEBUG ((DEBUG_INFO, "SubsystemID programming on Internal Devices\n"));
  SiProgramSsid (SiPolicy, HgSsidOverride, 0, 0);

  //
  // Perform ME post mem init
  // Call before PchInit to have MbpHob data ready.
  //
  PERF_START_EX (&gPerfMePostMemGuid, NULL, NULL, AsmReadTsc (), 0x40B0);
  //
  // Configure MCTP
  //
  if (IsMctpConfigurationSupported (SiPolicy)) {
    PtlPcdPsfMctpConfigure ();
  }
  MePostMemInit (SiPolicy);
  PERF_END_EX (&gPerfMePostMemGuid, NULL, NULL, AsmReadTsc (), 0x40B1);

  //
  // Initializes PCH after memory services initialized
  //
  PERF_START_EX (&gPerfPchPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4020);
  PtlPcdInit (SiPolicy);
  PERF_END_EX (&gPerfPchPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4021);

  //
  // SA Post Mem initialization
  //
  PERF_START_EX (&gPerfSaPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4030);

  ///
  /// SA device configuration
  ///
  DEBUG ((DEBUG_INFO, "DeviceConfigure Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_DEVICE_CONFIG_START); //PostCode (0xA01)
  DeviceConfigure (HostBridgePeiConfig);

  ///
  /// Install PPI to enable IGPU PEIM
  ///
  if ((IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit == 1) && (IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr != NULL)) {
    Status = PeiServicesInstallPpi (&mEnablePeiGraphicsPpi);
    ASSERT_EFI_ERROR (Status);
    ///
    /// Install PPI to use FSP provide PEIM IGPU
    ///
    if (IGpuConfig->PeiDisplayConfig.SkipFspGop == 0) {
      Status = PeiServicesInstallPpi (&mEnableFspPeiGraphicsPpi);
      ASSERT_EFI_ERROR (Status);
    }
  }
  /// Display Post Mem Init
  ///
  DEBUG ((DEBUG_INFO, "Initializing Display Post Mem\n"));
  IGpuDisplayInitPostMem ();

  ///
  /// Initializing TCSS Devices
  ///
  if (IsItcssEnabled ()) {
    DEBUG ((DEBUG_INFO, "Initializing TCSS\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_TCSS_INIT); //PostCode (0xA31)

    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock ((VOID *) SiPolicy, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
    ASSERT_EFI_ERROR(Status);
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
    ASSERT_EFI_ERROR(Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - error while getting TCSS config block. Aborting TCSS init.\n", __FUNCTION__));
    } else {
      PtlTcssInitPostMem (SiPolicy, TcssPeiPreMemConfig, TcssPeiConfig, PeiITbtConfig, HostBridgePreMemConfig);
    }
  }

  ///
  /// Update HostBridge Hob in PostMem
  ///
  UpdateHostBridgeHobPostMem (HostBridgePeiConfig);

  GuidHob = GetFirstGuidHob (&gIpPcieInstHobGuid);
  while(GuidHob != NULL) {
    pInst = (IP_PCIE_INST *) GET_GUID_HOB_DATA (GuidHob);

    if (pInst == NULL) {
      DEBUG ((DEBUG_WARN, "%a: Invalid pInst\n", __FUNCTION__));
      ASSERT (FALSE);
    }

    //
    // 6.15 step 2 if LCTL.CLS = Lowest (LCAP.MLS, EndPointMaxSpeed), the flow has completed
    // 6.15 step 4 skip all subsequence steps if LCTS.TLS is Gen2
    //
    if (!pInst->PrivateConfig.RootPortDisable && pInst->PrivateConfig.LinkRetrainInProgress) {
      RegInfo = (IP_WR_REG_INFO*) pInst->RegCntxt_Cfg_Pri;
      RpBase  = PCI_SEGMENT_LIB_ADDRESS (RegInfo->RegType.Pci.Seg, RegInfo->RegType.Pci.Bus, RegInfo->RegType.Pci.Dev, RegInfo->RegType.Pci.Fun, 0);

      Lcap.Data = PciSegmentRead32 (RpBase + LCAP_PCIE_CFG_REG);
      PcieGetDeviceInfo (pInst, pInst->PrivateConfig.BusMin, &DevInfo);
      MaxLinkSpeed = (UINT8)MIN (Lcap.Bits.mls, DevInfo.MaxLinkSpeed);
      if (pInst->PcieRpCommonConfig.EnableDtr) {
        MaxLinkSpeed = PcieDtrSpeedDetermine (pInst, MaxLinkSpeed);
      }
      IpPcieRpSpeedChangeEnd (pInst, MaxLinkSpeed, PcieGetTimeoutValue ());
    }
    SipLockCapRegisters (pInst);
    GuidHob = GetNextGuidHob (&gIpPcieInstHobGuid, GET_NEXT_HOB(GuidHob));
  }

  ///
  /// VMD Initializations if the VMD IP is supported. Disable it if the feature PCD is disabled.
  ///
  DEBUG ((DEBUG_INFO, "Initializing VMD\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_VMD_INIT); //PostCode (0xA33)
  PERF_INMODULE_BEGIN ("VmdInit");
  VmdInit(VmdPeiConfig);
  PERF_INMODULE_END ("VmdInit");

  /// PAVP Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing Pavp\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_PAVP_INIT); //PostCode (0xA32)
  IGpuPavpInit (
    IGpuConfig,
    IGpuPreMemConfig);

  ///
  /// Setup StreamTracer
  ///
  if (IsTelemetrySupported ()) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_STREAMTRACER_INIT); //PostCode (0xA13)
    SetStreamTracerMemoryBuffer (SiPreMemPolicyPpi);
  }

  PERF_END_EX (&gPerfSaPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4031);

  //
  //  IPU Initilization
  //
  DEBUG ((DEBUG_INFO, "Initializing IPU device\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_IPUINIT_BEGIN); //PostCode (0xA14)
  IpuInit();

  //
  //  NPU Initilization
  //
  DEBUG ((DEBUG_INFO, "Initializing NPU device\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_NPUINIT_BEGIN); //PostCode (0xA15)
  NpuInit (NpuPeiConfig);

  //
  //  IAX Initilization
  //
  DEBUG ((DEBUG_INFO, "Initializing IAX device\n"));
  IaxInit ();

  //
  // Install TPR Info HOB
  //
  InstallTprInfoHob ();

  //
  // Overclocking Post memory Initialize.
  //
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  PERF_START_EX (&gPerfCpuPostMemGuid, NULL, NULL, AsmReadTsc (), 0x407E);
  Status = CpuOcInitPostMem (OverClockingConfig, MpServices2Ppi);
  ASSERT_EFI_ERROR (Status);
  PERF_END_EX (&gPerfCpuPostMemGuid, NULL, NULL, AsmReadTsc (), 0x407F);
#endif
  //
  // Initialize processor features, performance and power management features
  // before RESET_CPL at post-memory phase.
  //
  PERF_START_EX (&gPerfCpuPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4080);
  CpuInit (SiPolicy);
  PERF_END_EX (&gPerfCpuPostMemGuid, NULL, NULL, AsmReadTsc (), 0x4081);

  //
  // BIOS Guard Initialization
  //
  BiosGuardInit (SiPolicy);

  //
  // Initialize performance and power management features before RESET_CPL at Post-memory phase.
  //
  CpuPowerMgmtInitPreResetCpl (SiPolicy, CpuPowerDeliveryConfig);

  //
  // Configure Isys control feature
  //
  ConfigureIsysControl (CpuPowerDeliveryConfig);

  //
  // Configures SoC firmware based skin temperature control MMIO register
  //
  ConfigureSkinTempControl (CpuPowerDeliveryConfig);

  //
  // Initialize Late TPR
  //
#if FixedPcdGetBool(PcdDtprEnable) == 1
  DEBUG ((DEBUG_INFO, "Initializing TprInit\n"));
  TprInit ();
#endif

  //
  // VT-d post mem initialization
  //
  VtdLateInit (Vtd);

  ///
  /// Enable DMA buffer for non-Gfx VT-d
  ///
  DEBUG ((DEBUG_INFO, "Enabling DMA buffer for non-Gfx VT-d\n"));
  VtdEnableDmaBuffer (NON_GFX_VTD);

  ///
  /// Update Vtd Hob in PostMem
  ///
  UpdateVtdHobPostMem (Vtd);

  //
  // Initialize TraceHub in PostMem
  //
  TraceHubConfigPostMem (SiPreMemPolicyPpi);

  //
  // Configure root port function number mapping
  // This has to be done before PCI enumeration and after RST/VMD remapping
  //
  PtlPcdPcieRpConfigureRpfnMapping (SiPolicy);

  //
  // Install EndOfPei callback function.
  //
  //
  // gEfiEndOfPeiSignal2PpiGuid will be notified at the end of FSP S.
  // Below Code Replace gEfiEndOfPeiSignalPpiGuid with gEfiEndOfPeiSignal2PpiGuid
  // All the gEfiEndOfPeiSignalPpiGuid calls registered as part of FSP will be executed
  // for gEfiEndOfPeiSignal2PpiGuid Notify.
  // Some of the drivers from Open Source may use only gEfiEndOfPeiSignalPpiGuid and this change
  // will help to override in those cases.
  //
  CopyGuid (mSiInitNotifyList->Guid, &gEfiEndOfPeiSignal2PpiGuid);
  //mSiInitNotifyList->Guid = gEfiEndOfPeiSignal2PpiGuid;

  Status = PeiServicesNotifyPpi (mSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  //
  // End of SiInit notification event
  //
  Status = PeiServicesInstallPpi (&mEndOfSiInit);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "SiInit () - End\n"));

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to Silicon Init on end of firmware.

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SiOnEndOfFirmware (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "SiOnEndOfFirmware () - Start\n"));

  PamLockEndOfFirmware ();

  DEBUG ((DEBUG_INFO, "SiOnEndOfFirmware () - End\n"));

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to Silicon Init on PCI enumeration complete.

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SiOnPciEnumerationComplete (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  return EFI_SUCCESS;
}

/**
  Silicon Initializes after memory services initialized

  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
SiInit (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                Status;

  //
  // Install PostMem phase OnPolicyInstalled callback function.
  //
  Status = PeiServicesNotifyPpi (mSiInitPostMemNotifyList);
  ASSERT_EFI_ERROR (Status);

  //Silicon Init Phase Postcode set
  SetPhaseStatusCode(FSP_STATUS_CODE_SILICON_INIT);
  //
  // Install a Default Policy initialization PPI
  //
  Status = PeiServicesInstallPpi (mPeiSiDefaultPolicyInitPpiList);
  ASSERT_EFI_ERROR (Status);

  //
  // Install the Pei Silicon Policy print PPI
  //
  Status = PeiServicesInstallPpi (&mPeiSiPolicyPrintPpiList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
