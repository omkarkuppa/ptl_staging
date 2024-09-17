/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <Library/ConfigBlockLib.h>
#include <FspmUpd.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <SpiInfoHob.h>
#include <PchPcieRpConfig.h>
#include <HdAudioConfig.h>
#include <IshConfig.h>
#include <CnviConfig.h>
#include <SpiConfig.h>
#include <SataConfig.h>
#include <RstConfig.h>
#include <SmbusConfig.h>
#include <DciConfig.h>
#include <HsioPcieConfig.h>
#include <HsioSataConfig.h>
#include <LpcConfig.h>
#include <WatchDogConfig.h>
#include <PchDmiConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <TraceHubConfig.h>
#include <PsfConfig.h>

/**
  Update TraceHub pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateTraceHubPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  TRACE_HUB_PREMEM_CONFIG         *TraceHubPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return;
  }
  TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode  = FspmUpd->FspmConfig.SocTraceHubMode;
  TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size = TRACEHUB_MEM_SIZE (FspmUpd->FspmConfig.SocTraceHubMemReg0Size);
  TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size = TRACEHUB_MEM_SIZE (FspmUpd->FspmConfig.SocTraceHubMemReg1Size);

}

/**
  Update Psf PreMem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdatePsfPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  PSF_CONFIG                      *PsfConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPsfConfigGuid, (VOID *) &PsfConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

}

/**
  Update Smbus pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateSmbusPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  UINT8                           NumRsvdSmbusTable;
  UINT8                           *RsvdSmbusTable;
  EFI_STATUS                      Status;
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  SmbusPreMemConfig->Enable                = FspmUpd->FspmConfig.SmbusEnable;
  SmbusPreMemConfig->ArpEnable             = FspmUpd->FspmConfig.SmbusArpEnable;
  SmbusPreMemConfig->DynamicPowerGating    = FspmUpd->FspmConfig.SmbusDynamicPowerGating;
  SmbusPreMemConfig->SpdWriteDisable       = FspmUpd->FspmConfig.SmbusSpdWriteDisable;
  SmbusPreMemConfig->SmbAlertEnable        = FspmUpd->FspmConfig.PchSmbAlertEnable;
  SmbusPreMemConfig->SmbusIoBase           = FspmUpd->FspmConfig.PchSmbusIoBase;
  SmbusPreMemConfig->NumRsvdSmbusAddresses = FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses;

  if (FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses != 0) {
    NumRsvdSmbusTable = FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses;
    RsvdSmbusTable = (UINT8 *)(UINTN) FspmUpd->FspmConfig.RsvdSmbusAddressTablePtr;
    ZeroMem (
      SmbusPreMemConfig->RsvdSmbusAddressTable,
      PCH_MAX_SMBUS_RESERVED_ADDRESS * sizeof (UINT8)
      );
    CopyMem (
      SmbusPreMemConfig->RsvdSmbusAddressTable,
      RsvdSmbusTable,
      NumRsvdSmbusTable * sizeof (UINT8)
      );
  }
}

/**
  Update Dci pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdatePeiDciPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  DciPreMemConfig->DciEn              = FspmUpd->FspmConfig.DciEn;
  DciPreMemConfig->DciClkEnable       = FspmUpd->FspmConfig.DciClkEnable;
  DciPreMemConfig->DciDbcMode         = FspmUpd->FspmConfig.DciDbcMode;
  DciPreMemConfig->KeepEarlyTrace     = FspmUpd->FspmConfig.KeepEarlyTrace;
}

/**
  Update PCIe Root Port pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdatePcieRpPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;
  UINTN                           MaxPcieRootPorts;
  UINTN                           Index;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  MaxPcieRootPorts = GetPchMaxPciePortNum ();
  PcieRpPreMemConfig->RpEnabledMask = FspmUpd->FspmConfig.PcieRpEnableMask & ((1 << MaxPcieRootPorts) - 1);
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index ++) {
    PcieRpPreMemConfig->PcieClock[Index].Usage = FspmUpd->FspmConfig.PcieClkSrcUsage[Index];
    PcieRpPreMemConfig->PcieClock[Index].ClkReq = FspmUpd->FspmConfig.PcieClkSrcClkReq[Index];
  }
  PcieRpPreMemConfig->PcieResizableBarSupport = FspmUpd->FspmConfig.PcieResizableBarSupport;
}

/**
  Update HsioPcie pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateHsioPciePreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  UINTN                           Index;
  EFI_STATUS                      Status;
  PCH_HSIO_PCIE_PREMEM_CONFIG     *HsioPciePreMemConfig;
  UINTN                           MaxPcieRootPorts;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPciePreMemConfigGuid, (VOID *) &HsioPciePreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  MaxPcieRootPorts = GetPchMaxPciePortNum ();
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable           = FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle                 = FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable        = FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmphEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph              = FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmph[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable     = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5Enable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5           = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable     = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0Enable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0           = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0[Index];
  }
}

/**
  Update Lpc pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateLpcPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
}

/**
  Update PCH General pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdatePchGeneralPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchGeneralPreMemConfig->Port80Route  = FspmUpd->FspmConfig.PchPort80Route;
  PchGeneralPreMemConfig->GpioOverride = FspmUpd->FspmConfig.GpioOverride;
  PchGeneralPreMemConfig->IoeDebugEn = FspmUpd->FspmConfig.IoeDebugEn;
  PchGeneralPreMemConfig->PmodeClkEn = FspmUpd->FspmConfig.PmodeClkEn;
  PchGeneralPreMemConfig->PmcPrivacyConsent = FspmUpd->FspmConfig.PmcPrivacyConsent;

}

/**
  Update WatchDog pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateWdtPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  PCH_WDT_PREMEM_CONFIG           *WdtPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  WdtPreMemConfig->DisableAndLock = FspmUpd->FspmConfig.WdtDisableAndLock;
}

/**
  Update HD Audio pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateHdaPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  HDAUDIO_PREMEM_CONFIG           *HdaPreMemConfig;
  UINTN                           Index;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  HdaPreMemConfig->Enable                = FspmUpd->FspmConfig.PchHdaEnable;
  HdaPreMemConfig->DspEnable             = FspmUpd->FspmConfig.PchHdaDspEnable;
  HdaPreMemConfig->DspUaaCompliance      = FspmUpd->FspmConfig.PchHdaDspUaaCompliance;
  HdaPreMemConfig->AudioLinkHda.Enable   = FspmUpd->FspmConfig.PchHdaAudioLinkHdaEnable;

  for (Index = 0; Index < PCH_MAX_HDA_SDI; Index++) {
    HdaPreMemConfig->AudioLinkHda.SdiEnable[Index] = FspmUpd->FspmConfig.PchHdaSdiEnable[Index];
  }

  for (Index = 0; Index < PCH_MAX_HDA_DMIC_LINK_NUM; Index++) {
    HdaPreMemConfig->AudioLinkDmic[Index].Enable      = FspmUpd->FspmConfig.PchHdaAudioLinkDmicEnable[Index];
    HdaPreMemConfig->AudioLinkDmic[Index].DmicClockSelect = FspmUpd->FspmConfig.PchHdaAudioLinkDmicClockSelect[Index];
    HdaPreMemConfig->AudioLinkDmic[Index].PinMux.ClkA = FspmUpd->FspmConfig.PchHdaAudioLinkDmicClkAPinMux[Index];
    HdaPreMemConfig->AudioLinkDmic[Index].PinMux.Data = FspmUpd->FspmConfig.PchHdaAudioLinkDmicDataPinMux[Index];
  }

  for (Index = 0; Index < PCH_MAX_HDA_SSP_LINK_NUM; Index++ ){
    HdaPreMemConfig->AudioLinkSsp[Index].Enable = FspmUpd->FspmConfig.PchHdaAudioLinkSspEnable[Index];
  }

  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    HdaPreMemConfig->AudioLinkSndw[Index].Enable = FspmUpd->FspmConfig.PchHdaAudioLinkSndwEnable[Index];
  }
  HdaPreMemConfig->IDispLinkFrequency    = FspmUpd->FspmConfig.PchHdaIDispLinkFrequency;
  HdaPreMemConfig->IDispLinkTmode        = FspmUpd->FspmConfig.PchHdaIDispLinkTmode;
  HdaPreMemConfig->IDispCodecDisconnect  = FspmUpd->FspmConfig.PchHdaIDispCodecDisconnect;
  HdaPreMemConfig->SubSystemIds          = FspmUpd->FspmConfig.PchHdaSubSystemIds;

  for (Index = 0; Index < PCH_MAX_HDA_SNDW_MULTILANE_NUM; Index++) {
    HdaPreMemConfig->SndwMultilane[Index].Enable        = FspmUpd->FspmConfig.PchHdAudioSndwMultilaneEnable[Index];
    HdaPreMemConfig->SndwMultilane[Index].SndwInterface = FspmUpd->FspmConfig.PchHdAudioSndwMultilaneSndwInterface[Index];
    HdaPreMemConfig->SndwMultilane[Index].PinMux.Clk    = FspmUpd->FspmConfig.PchHdaAudioLinkMultilaneClkPinMux[Index];
    HdaPreMemConfig->SndwMultilane[Index].PinMux.Data0  = FspmUpd->FspmConfig.PchHdaAudioLinkMultilaneData0PinMux[Index];
    HdaPreMemConfig->SndwMultilane[Index].PinMux.Data1  = FspmUpd->FspmConfig.PchHdaAudioLinkMultilaneData1PinMux[Index];
    HdaPreMemConfig->SndwMultilane[Index].PinMux.Data2  = FspmUpd->FspmConfig.PchHdaAudioLinkMultilaneData2PinMux[Index];
    HdaPreMemConfig->SndwMultilane[Index].PinMux.Data3  = FspmUpd->FspmConfig.PchHdaAudioLinkMultilaneData3PinMux[Index];
  }

  for (Index = 0; Index < PCH_MAX_HDA_SSP_LINK_NUM; Index++) {
    HdaPreMemConfig->AudioLinkSsp[Index].PinMux.Sclk = FspmUpd->FspmConfig.PchHdaAudioLinkSspSclkPinMux[Index];
    HdaPreMemConfig->AudioLinkSsp[Index].PinMux.Sfmr = FspmUpd->FspmConfig.PchHdaAudioLinkSspSfmrPinMux[Index];
    HdaPreMemConfig->AudioLinkSsp[Index].PinMux.Txd  = FspmUpd->FspmConfig.PchHdaAudioLinkSspTxdPinMux[Index];
    HdaPreMemConfig->AudioLinkSsp[Index].PinMux.Rxd  = FspmUpd->FspmConfig.PchHdaAudioLinkSspRxdPinMux[Index];
  }

  HdaPreMemConfig->ResetWaitTimer   = FspmUpd->FspmConfig.ResetWaitTimer;
  HdaPreMemConfig->PowerClockGating   = FspmUpd->FspmConfig.PchHdaTestPowerClockGating;
  HdaPreMemConfig->LowFreqLinkClkSrc  = FspmUpd->FspmConfig.PchHdaTestLowFreqLinkClkSrc;
  HdaPreMemConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled = FspmUpd->FspmConfig.PchHdaDiscBtOffEnabled;
  HdaPreMemConfig->HdaDiscBtOffload.HdaDiscBtOffSspLink = FspmUpd->FspmConfig.PchHdaDiscBtOffSspLink;
}


/**
  Update ISH pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateIshPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                      Status;
  ISH_PREMEM_CONFIG               *IshPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  IshPreMemConfig->Enable = FspmUpd->FspmConfig.PchIshEnable;
}

/**
  Update CNVi pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateCnviPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                   Status;
  CNVI_PREMEM_CONFIG           *CnviPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gCnviPreMemConfigGuid, (VOID *) &CnviPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  CnviPreMemConfig->DdrRfim = FspmUpd->FspmConfig.CnviDdrRfim;
}

/**
  Update SPI pre mem policies.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateSpiPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  EFI_STATUS                   Status;
  SPI_CONFIG                   *SpiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSpiConfigGuid, (VOID *) &SpiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  SpiPreMemConfig->ExtendedBiosDecodeRangeEnable = FspmUpd->FspmConfig.PchSpiExtendedBiosDecodeRangeEnable;
  SpiPreMemConfig->ExtendedBiosDecodeRangeBase = FspmUpd->FspmConfig.PchSpiExtendedBiosDecodeRangeBase;
  SpiPreMemConfig->ExtendedBiosDecodeRangeLimit = FspmUpd->FspmConfig.PchSpiExtendedBiosDecodeRangeLimit;
}

/**
  Update SPI pre mem HOB.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] FspsUpm        Pointer to FSPM_UPD
**/
STATIC
VOID
FspUpdateSpiPreMemHob (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  FSPM_UPD                 *FspmUpd
  )
{
  if (FspmUpd->FspmConfig.ExtendedBiosDecodeRange == 1) {
    SPI_INFO_HOB      *SpiInfoHob;

    SpiInfoHob = (SPI_INFO_HOB *) GetFirstGuidHob(&gSpiInfoHobGuid);

    if (SpiInfoHob != NULL) {
      SpiInfoHob->ExtendedBiosDecodeRangeEnable = FspmUpd->FspmConfig.ExtendedBiosDecodeRange;
      DEBUG ((DEBUG_INFO, "FSP ExtendedBiosDecodeRange = 0x%x\n", SpiInfoHob->ExtendedBiosDecodeRangeEnable));
    }
  }
}

/**
  This function performs PCH PEI Policy update in pre-memory.

  @param[in, out] SiPreMemPolicy  The SI PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN     FSPM_UPD                 *FspmUpd
  )
{
  FspUpdatePeiDciPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateTraceHubPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdatePsfPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateSmbusPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdatePcieRpPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateHsioPciePreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateLpcPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdatePchGeneralPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateWdtPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateHdaPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateIshPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateCnviPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateSpiPreMemPolicy (SiPreMemPolicy, FspmUpd);
  FspUpdateSpiPreMemHob (SiPreMemPolicy, FspmUpd);
  return EFI_SUCCESS;
}

/**
  This function performs PCH PEI Policy update in pre-memory.

  @param[in, out] PchPreMemPolicy The PCH PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAttachedPchPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *PchPreMemPolicy,
  IN     FSPM_UPD                 *FspmUpd
  )
{
  return EFI_SUCCESS;
}
