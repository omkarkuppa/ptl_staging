/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

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

#include "PeiPchPolicyUpdate.h"
#include <Guid/GlobalVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CnviLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/PcdInfoLib.h>
#include <PolicyUpdateMacro.h>
#include <SpiInfoHob.h>
#include <CnvVfrSetupMenuHii.h>
#include <SetupVariable.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif
#include <Pins/GpioPinsVer2Lp.h>
#include <Library/GpioV2AccessLib.h>
#include <Register/GpioV2ChipsetId.h>
#include <GpioV2Pad.h>
#include <Register/GpioV2PcdRegs.h>
#include <Register/GpioV2PcdPins.h>
#include <CnviConfig.h>
#include <SpiConfig.h>
#include <IshConfig.h>
#include <HdAudioConfig.h>
#include <PchPcieRpConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <LpcConfig.h>
#include <DciConfig.h>
#include <WatchDogConfig.h>
#include <SmbusConfig.h>

//
// Sawtooth Peak
// Single SPD EEPROM at 0xA2 serves both C0D0 and C1D0 (LPDDR is 1DPC only)
//
#define DIMM_SMB_SPD_P0C0D0_STP 0xA2
#define DIMM_SMB_SPD_P0C0D1_STP 0xA0
#define DIMM_SMB_SPD_P0C1D0_STP 0xA2
#define DIMM_SMB_SPD_P0C1D1_STP 0xA0

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusSTPRsvdAddresses[] = {
  DIMM_SMB_SPD_P0C0D0_STP,
  DIMM_SMB_SPD_P0C0D1_STP,
  DIMM_SMB_SPD_P0C1D0_STP,
  DIMM_SMB_SPD_P0C1D1_STP
};

/**
  Update PCH General pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
  @param[in] SetupData      Pointer to SETUP_DATA
**/
STATIC
VOID
UpdatePchGeneralPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup,
  IN  SETUP_DATA               *SetupData
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;
  EFI_STATUS                      Status;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchPort80Route, PchGeneralPreMemConfig->Port80Route, PchSetup->IchPort80Route);
  UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.GpioOverride, PchGeneralPreMemConfig->GpioOverride, 0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IoeDebugEn, PchGeneralPreMemConfig->IoeDebugEn, PchSetup->IoeDebugEn);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PmodeClkEn, PchGeneralPreMemConfig->PmodeClkEn, PchSetup->PmodeClkEn);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PmcPrivacyConsent, PchGeneralPreMemConfig->PmcPrivacyConsent, PchSetup->PmcPrivacyConsent);

}

/**
  Update DCI pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateDciPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DciEn, DciPreMemConfig->DciEn, PchSetup->DciEn);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DciClkEnable, DciPreMemConfig->DciClkEnable, PchSetup->DciClkEnable);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DciDbcMode, DciPreMemConfig->DciDbcMode, PchSetup->DciDbcMode);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.KeepEarlyTrace, DciPreMemConfig->KeepEarlyTrace, PchSetup->KeepEarlyTrace);
}

/**
  Update TraceHub pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateTraceHubPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  TRACE_HUB_PREMEM_CONFIG         *TraceHubPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
  ((FSPM_UPD *) FspmUpd)->FspmConfig.SocTraceHubMemReg0Size = PchSetup->SocTraceHubMemReg0Size;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.SocTraceHubMemReg1Size = PchSetup->SocTraceHubMemReg1Size;
#else
  TraceHubPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size = TRACEHUB_MEM_SIZE (PchSetup->SocTraceHubMemReg0Size);
  TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size = TRACEHUB_MEM_SIZE (PchSetup->SocTraceHubMemReg1Size);
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SocTraceHubMode, TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode, PchSetup->SocTraceHubMode);
}

/**
  Update Smbus pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateSmbusPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
  UINT8                           *SmBusReservedTable;
  UINT8                           SmBusReservedNum;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SmbusEnable, SmbusPreMemConfig->Enable, TRUE);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchSmbAlertEnable, SmbusPreMemConfig->SmbAlertEnable, PcdGetBool (PcdSmbusAlertEnable));

  SmBusReservedTable = NULL;
  SmBusReservedNum   = 0;
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchSmbusIoBase, SmbusPreMemConfig->SmbusIoBase, PcdGet16 (PcdSmbusBaseAddress));
  SmBusReservedTable = mSmbusSTPRsvdAddresses;
  SmBusReservedNum   = sizeof (mSmbusSTPRsvdAddresses);

  if (SmBusReservedTable != NULL) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchNumRsvdSmbusAddresses, SmbusPreMemConfig->NumRsvdSmbusAddresses, SmBusReservedNum);
#if FixedPcdGet8(PcdFspModeSelection) == 1
    ((FSPM_UPD *) FspmUpd)->FspmConfig.RsvdSmbusAddressTablePtr = (UINTN) SmBusReservedTable; //To fix Simics pause on API build
#else
    CopyMem (SmbusPreMemConfig->RsvdSmbusAddressTable, SmBusReservedTable, SmBusReservedNum);
#endif
  }
}

/**
  Update Lpc pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateLpcPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchLpcEnhancePort8xhDecoding, LpcPreMemConfig->EnhancePort8xhDecoding, PchSetup->EnhancePort8xhDecoding);
}

/**
  Update WatchDog pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateWatchDogPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  PCH_WDT_PREMEM_CONFIG           *WatchDogPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
 #else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gWatchDogPreMemConfigGuid, (VOID *) &WatchDogPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WdtDisableAndLock, WatchDogPreMemConfig->DisableAndLock, (UINTN)(!PchSetup->WdtEnable));
}

VOID
UpdatePcieClockInfo (
  PCH_PCIE_RP_PREMEM_CONFIG  *PcieRpPreMemConfig,
  UINTN                      Index,
  UINT64                     Data
  )
{
  PCD64_BLOB Pcd64;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#endif

  Pcd64.Blob = Data;
  DEBUG ((DEBUG_INFO, "UpdatePcieClockInfo ClkIndex %x ClkUsage %x, Supported %x\n", Index, Pcd64.PcieClock.ClockUsage, Pcd64.PcieClock.ClkReqSupported));

  if(Index < GetPchMaxPcieClockNum()) {
    UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcUsage[Index], PcieRpPreMemConfig->PcieClock[Index].Usage, (UINT8)Pcd64.PcieClock.ClockUsage);
    UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcClkReq[Index], PcieRpPreMemConfig->PcieClock[Index].ClkReq, Pcd64.PcieClock.ClkReqSupported ? (UINT8)Index : 0xFF);
  }
}
/**
  Update PcieRp pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePcieRpPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
  UINT32                          RpIndex;
  UINT32                          RpEnabledMask;
  SA_SETUP                        SaSetup;
  UINTN                           VarSize;

  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#endif
  EFI_STATUS                      Status;
  PcieRpPreMemConfig = NULL;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  GET_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PcieRpEnableMask,PcieRpPreMemConfig->RpEnabledMask, RpEnabledMask);

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex ++) {
    if (PchSetup->PcieRootPortEn[RpIndex]) {
      RpEnabledMask |=  (UINT32) (1 << RpIndex);
    } else {
      RpEnabledMask &= ~(UINT32) (1 << RpIndex);
    }
  }
  // RpEnabledMask value is related with Setup value, Need to check Policy Default
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PcieRpEnableMask, PcieRpPreMemConfig->RpEnabledMask, RpEnabledMask);

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SaSetup",
                               &gSaSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SaSetup
                               );
  ASSERT_EFI_ERROR (Status);
  //
  // SA:GfxRemovePostPOBegin
  //
  //
  // External Graphics card scan option.
  //
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SkipExtGfxScan, PcieRpPreMemConfig->SkipExtGfxScan, SaSetup.SkipExtGfxScan);
  //
  // SA:GfxRemovePostPOEnd
  //
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PcieResizableBarSupport, PcieRpPreMemConfig->PcieResizableBarSupport, PchSetup->PcieResizableBarSupport);

  UpdatePcieClockInfo (PcieRpPreMemConfig,  0, PcdGet64(PcdPcieClock0));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  1, PcdGet64(PcdPcieClock1));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  2, PcdGet64(PcdPcieClock2));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  3, PcdGet64(PcdPcieClock3));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  4, PcdGet64(PcdPcieClock4));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  5, PcdGet64(PcdPcieClock5));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  6, PcdGet64(PcdPcieClock6));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  7, PcdGet64(PcdPcieClock7));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  8, PcdGet64(PcdPcieClock8));
  UpdatePcieClockInfo (PcieRpPreMemConfig,  9, PcdGet64(PcdPcieClock9));

  for (RpIndex = 0; RpIndex < GetPchMaxPcieClockNum(); RpIndex++) {
    if (PchSetup->PchPcieClkReqSupport[RpIndex] == 0xFF) {
      UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcClkReq[RpIndex], PcieRpPreMemConfig->PcieClock[RpIndex].ClkReq, (UINT8)PCH_PCIE_NO_SUCH_CLOCK);
    }
    if (PchSetup->PchPcieClockUsageOverride[RpIndex] == 1) {
      UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcUsage[RpIndex], PcieRpPreMemConfig->PcieClock[RpIndex].Usage, PchClockUsageUnspecified);
    } else if (PchSetup->PchPcieClockUsageOverride[RpIndex] == 2) {
      UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.PcieClkSrcUsage[RpIndex], PcieRpPreMemConfig->PcieClock[RpIndex].Usage, PchClockUsageNotUsed);
    }
  }

}
/**
  Update HD Audio pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateHdaPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
  UINTN                           Index;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  HDAUDIO_PREMEM_CONFIG           *HdaPreMemConfig;
  EFI_STATUS                      Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  for (Index = 0; Index < PCH_MAX_HDA_SDI; Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaSdiEnable[Index], HdaPreMemConfig->AudioLinkHda.SdiEnable[Index], PchSetup->PchHdAudioSdiLaneEnable[Index], Index);
  }
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaEnable,               HdaPreMemConfig->Enable,               PchSetup->PchHdAudio                   );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaDspEnable,            HdaPreMemConfig->DspEnable,            PchSetup->PchHdAudioDsp                );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaIDispLinkFrequency,   HdaPreMemConfig->IDispLinkFrequency,   PchSetup->PchHdaIDisplayLinkFreq       );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaIDispLinkTmode,       HdaPreMemConfig->IDispLinkTmode,       PchSetup->PchHdaIDisplayLinkTmode      );
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaIDispCodecDisconnect, HdaPreMemConfig->IDispCodecDisconnect, PchSetup->PchHdaIDisplayCodecDisconnect);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaDspUaaCompliance,     HdaPreMemConfig->DspUaaCompliance,     PchSetup->PchHdAudioDspUaaCompliance   );

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkHdaEnable,   HdaPreMemConfig->AudioLinkHda.Enable,  PchSetup->PchHdAudioHdaLinkEnable);
  for (Index = 0; Index < GetPchHdaMaxDmicLinkNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicEnable[Index],      HdaPreMemConfig->AudioLinkDmic[Index].Enable,          PchSetup->PchHdAudioDmicLinkEnable[Index],  Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicClockSelect[Index], HdaPreMemConfig->AudioLinkDmic[Index].DmicClockSelect, HdaDmicClockSelectClkA, Index);
  }

  for (Index = 0; Index < GetPchHdaMaxSndwMultilaneNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdAudioSndwMultilaneEnable[Index], HdaPreMemConfig->SndwMultilane[Index].Enable,        PchSetup->PchHdAudioSndwMultilaneEnable[Index], Index);
  }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicClkAPinMux[0], HdaPreMemConfig->AudioLinkDmic[0].PinMux.ClkA, GPIOV2_PTL_PCD_MUXING__XXGPP_S_4__DMIC_CLK_A_0);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicDataPinMux[0], HdaPreMemConfig->AudioLinkDmic[0].PinMux.Data, GPIOV2_PTL_PCD_MUXING__XXGPP_S_5__DMIC_DATA_0);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicClkAPinMux[1], HdaPreMemConfig->AudioLinkDmic[1].PinMux.ClkA, GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__DMIC_CLK_A_1);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicDataPinMux[1], HdaPreMemConfig->AudioLinkDmic[1].PinMux.Data, GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__DMIC_DATA_1);

  for (Index = 0; Index < GetPchHdaMaxSndwMultilaneNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneClkPinMux[Index], HdaPreMemConfig->SndwMultilane[Index].PinMux.Clk,    0, Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneData0PinMux[Index], HdaPreMemConfig->SndwMultilane[Index].PinMux.Data0,  0, Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneData2PinMux[Index], HdaPreMemConfig->SndwMultilane[Index].PinMux.Data2,  0, Index);
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneData3PinMux[Index], HdaPreMemConfig->SndwMultilane[Index].PinMux.Data3,  0, Index);
  }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneData1PinMux[0], HdaPreMemConfig->SndwMultilane[0].PinMux.Data1, GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__SNDW2_DATA1);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkMultilaneData1PinMux[1], HdaPreMemConfig->SndwMultilane[1].PinMux.Data1, 0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdAudioSndwMultilaneSndwInterface[0], HdaPreMemConfig->SndwMultilane[0].SndwInterface, 2);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdAudioSndwMultilaneSndwInterface[1], HdaPreMemConfig->SndwMultilane[1].SndwInterface, 3);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSclkPinMux[0], HdaPreMemConfig->AudioLinkSsp[0].PinMux.Sclk, GPIOV2_PTL_PCD_MUXING__XXGPP_D_10__I2S0_SCLK);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSfmrPinMux[0], HdaPreMemConfig->AudioLinkSsp[0].PinMux.Sfmr, GPIOV2_PTL_PCD_MUXING__XXGPP_D_11__I2S0_SFRM);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspTxdPinMux[0], HdaPreMemConfig->AudioLinkSsp[0].PinMux.Txd,  GPIOV2_PTL_PCD_MUXING__XXGPP_D_12__I2S0_TXD);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspRxdPinMux[0], HdaPreMemConfig->AudioLinkSsp[0].PinMux.Rxd,  GPIOV2_PTL_PCD_MUXING__XXGPP_D_13__I2S0_RXD);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSclkPinMux[1], HdaPreMemConfig->AudioLinkSsp[1].PinMux.Sclk, GPIOV2_PTL_PCD_MUXING__XXGPP_S_2__I2S1_SCLK);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSfmrPinMux[1], HdaPreMemConfig->AudioLinkSsp[1].PinMux.Sfmr, GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__I2S1_SFRM);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspTxdPinMux[1], HdaPreMemConfig->AudioLinkSsp[1].PinMux.Txd,  GPIOV2_PTL_PCD_MUXING__XXGPP_S_0__I2S1_TXD);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspRxdPinMux[1], HdaPreMemConfig->AudioLinkSsp[1].PinMux.Rxd,  GPIOV2_PTL_PCD_MUXING__XXGPP_S_1__I2S1_RXD);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSclkPinMux[2], HdaPreMemConfig->AudioLinkSsp[2].PinMux.Sclk, GPIOV2_PTL_PCD_MUXING__XXGPP_S_4__I2S2_SCLK);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspSfmrPinMux[2], HdaPreMemConfig->AudioLinkSsp[2].PinMux.Sfmr, GPIOV2_PTL_PCD_MUXING__XXGPP_S_5__I2S2_SFRM);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspTxdPinMux[2], HdaPreMemConfig->AudioLinkSsp[2].PinMux.Txd,  GPIOV2_PTL_PCD_MUXING__XXGPP_S_6__I2S2_TXD);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspRxdPinMux[2], HdaPreMemConfig->AudioLinkSsp[2].PinMux.Rxd,  GPIOV2_PTL_PCD_MUXING__XXGPP_S_7__I2S2_RXD);

  for (Index = 0; Index < GetPchHdaMaxSspLinkNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspEnable[Index], HdaPreMemConfig->AudioLinkSsp[Index].Enable,  PchSetup->PchHdAudioSspLinkEnable[Index],  Index);
  }

  if (PcdGetBool (VpdPcdAepConfig) == TRUE){
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicDataPinMux[1], HdaPreMemConfig->AudioLinkDmic[1].PinMux.Data, GPIOV2_PTL_PCD_MUXING__XXGPP_D_17__DMIC_DATA_1);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicClkAPinMux[0], HdaPreMemConfig->AudioLinkDmic[0].PinMux.ClkA, GPIOV2_PTL_PCD_MUXING__XXGPP_D_16__DMIC_CLK_A_1);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkDmicDataPinMux[0], HdaPreMemConfig->AudioLinkDmic[0].PinMux.Data, GPIOV2_PTL_PCD_MUXING__XXGPP_S_3__DMIC_DATA_0);
  }

  //
  // This is to avoid SSP pins program when CNVi and Discrete BT Offload is enabled, since SSP pins need to be at HighZ when CRF is detected.
  //
  if (CnviCrfModuleIsPresent()) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSspEnable[PchSetup->HdaDiscBtOffSspLink], HdaPreMemConfig->AudioLinkSsp[PchSetup->HdaDiscBtOffSspLink].Enable,  0);
  }
  for (Index = 0; Index < GetPchHdaMaxSndwLinkNum (); Index++) {
    COMPARE_UPDATE_POLICY_ARRAY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaAudioLinkSndwEnable[Index], HdaPreMemConfig->AudioLinkSndw[Index].Enable, PchSetup->PchHdAudioSndwLinkEnable[Index], Index);
  }
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaDiscBtOffEnabled, HdaPreMemConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled, PchSetup->HdaDiscBtOffEnabled);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaDiscBtOffSspLink, HdaPreMemConfig->HdaDiscBtOffload.HdaDiscBtOffSspLink, PchSetup->HdaDiscBtOffSspLink);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchHdaSndwClockSourceSelect, HdaPreMemConfig->SoundWireClockSelect, PchSetup->PchHdaSndwClockSrcSelect);

}

/**
  Update ISH pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateIshPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  ISH_PREMEM_CONFIG               *IshPreMemConfig;
  EFI_STATUS                      Status;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchIshEnable, IshPreMemConfig->Enable, PchSetup->PchIshEnable);
}

#if FixedPcdGetBool (PcdCnvIntegratedSupport) == 1
/**
  Update CNVi pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] SaSetup        Pointer to Sa_SETUP
**/
STATIC
VOID
UpdateCnviPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  SA_SETUP                 *SaSetup
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                         *FspmUpd;
#else
  CNVI_PREMEM_CONFIG           *CnviPreMemConfig;
  EFI_STATUS                   Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCnviPreMemConfigGuid, (VOID *) &CnviPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CnviDdrRfim, CnviPreMemConfig->DdrRfim, SaSetup->CnviDdrRfim);
}
#endif

/**
  Update SPI pre mem policies.

  @param[in] SiPreMemPolicy       Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateSpiPreMemPolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  IN PCH_SETUP                 *PchSetup
  )
{

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  SPI_CONFIG                   *SpiPreMemConfig;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  EFI_STATUS                   Status;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSpiConfigGuid, (VOID *) &SpiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PchSpiExtendedBiosDecodeRangeEnable, SpiPreMemConfig->ExtendedBiosDecodeRangeEnable, PchSetup->ExtendedBiosDecodeRangeEnable);
}

/**
  this function performs SPI HOB Update

**/
VOID
UpdateSpiPreMemHob (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  PCH_SETUP                *PchSetup
  )
{
  VOID                        *SpiInfoHob;
  SPI_INFO_HOB                *SpiInfoHobData;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
#endif

  SpiInfoHob = GetFirstGuidHob (&gSpiInfoHobGuid);

  if (SpiInfoHob != NULL) {
    SpiInfoHobData =  GET_GUID_HOB_DATA (SpiInfoHob);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ExtendedBiosDecodeRange, SpiInfoHobData->ExtendedBiosDecodeRangeEnable, FixedPcdGetBool(PcdExtendedBiosRegionSupport));
    DEBUG ((DEBUG_INFO, "ExtendedBiosDecodeRange = 0x%x\n", SpiInfoHobData->ExtendedBiosDecodeRangeEnable));
  } else {
    DEBUG ((DEBUG_INFO, "SPI Info Hob not found\n"));
  }
}

/**
  This function performs PCH PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  PCH_SETUP                       PchSetup;
  SETUP_DATA                      SetupVariables;
  SA_SETUP                        SaSetup;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyUpdate Pre-Mem Start\n"));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupVariables
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SaSetup",
                               &gSaSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SaSetup
                               );
  ASSERT_EFI_ERROR (Status);
  UpdatePchGeneralPreMemPolicy (SiPreMemPolicyPpi, &PchSetup, &SetupVariables);
  UpdateDciPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateTraceHubPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateSmbusPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateLpcPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateWatchDogPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdatePcieRpPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateHdaPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateIshPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
#if FixedPcdGetBool (PcdCnvIntegratedSupport) == 1
  UpdateCnviPreMemPolicy (SiPreMemPolicyPpi, &SaSetup);
#endif
  UpdateSpiPreMemPolicy (SiPreMemPolicyPpi, &PchSetup);
  UpdateSpiPreMemHob (SiPreMemPolicyPpi, &PchSetup);
  return EFI_SUCCESS;
}
