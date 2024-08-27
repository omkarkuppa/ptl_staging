/** @file
  HDA SoC library. Library allow to configure HDA controller for working on Ptl platforms.
  All function in this library is available for PEI.

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

#include <IndustryStandard/Pci30.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PcdLib.h>
#include <GpioV2Functionalities.h>
#include <Library/PeiHdaLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiAceInitLib.h>
#include <Library/HobLib.h>
#include <Register/PchRegs.h>
#include <Register/AceRegs.h>
#include <Ppi/SiPolicy.h>
#include <HdaConfigHob.h>
#include <Library/PchInfoHob.h>
#include <Defines/IgdDefines.h>
#include <Library/GpioV2AccessLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioHelpersLib.h>
#include <Library/PcdInfoLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <GpioV2Signals.h>

/*
  Configure HDA Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
*/
VOID
PtlPcdHdaConfigureHdaLink (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_BCLK, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: HDA_BCLK GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_RSTB, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: HDA_RSTB GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_SYNC, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: HDA_SYNC GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_SDO,  0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: HDA_SDO GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  if (HdaPreMemConfig->AudioLinkHda.SdiEnable[0]) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_SDI(0), 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HDA: HDA_SDI0 GPIO programming failed, Status = %r.\n", Status));
      return;
    }
  }

  if (HdaPreMemConfig->AudioLinkHda.SdiEnable[1]) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_HDA_SDI(1), 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HDA: HDA_SDI1 GPIO programming failed, Status = %r.\n", Status));
      return;
    }
  }

  DEBUG ((
    DEBUG_INFO, "HDA: HDA Audio Link - GPIOs programmed successfully, SDI0 = 0x%x, SDI1 = 0x%x\n",
    HdaPreMemConfig->AudioLinkHda.SdiEnable[0], HdaPreMemConfig->AudioLinkHda.SdiEnable[1]
    ));
}

/*
  Configure DMIC Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
  @param[in] DmicIndex            Dmic Link Index
*/
VOID
PtlPcdHdaConfigureDmicLinks (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN UINT32                  DmicIndex
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_DMIC_CLKA (DmicIndex), HdaPreMemConfig->AudioLinkDmic[DmicIndex].PinMux.ClkA);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: DMIC_CLKA#%d GPIO programming failed, Status = %r.\n", DmicIndex, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_DMIC_DATA (DmicIndex), HdaPreMemConfig->AudioLinkDmic[DmicIndex].PinMux.Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: DMIC_DATA#%d GPIO programming failed, Status = %r.\n", DmicIndex, Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "HDA: DMIC#%d - GPIOs programmed successfully\n", DmicIndex));
}

/*
  Configure SSP Audio Link GPIOs

  @param[in] SspIndex       SSP Link Index
*/
VOID
PtlPcdHdaConfigureSspLinks (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN UINT32                  SspIndex
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_I2S_SCLK (SspIndex), HdaPreMemConfig->AudioLinkSsp[SspIndex].PinMux.Sclk);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: I2S_SCLK GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_I2S_SFRM (SspIndex), HdaPreMemConfig->AudioLinkSsp[SspIndex].PinMux.Sfmr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: I2S_SFRM GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_I2S_TXD (SspIndex), HdaPreMemConfig->AudioLinkSsp[SspIndex].PinMux.Txd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: I2S_TXD GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_I2S_RXD (SspIndex), HdaPreMemConfig->AudioLinkSsp[SspIndex].PinMux.Rxd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: I2S_RXD GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "HDA: SSP#%d - GPIOs programmed successfully\n", SspIndex));
}

/*
  Configure Master Clock GPIOs for SSP Link

  @param[in] HdaPreMemConfig   Hda PreMem Configuration.
  @param[in] MasterClockIndex  SSP Master Clock Index
*/
VOID
PtlPcdHdaConfigureSspMasterClock (
  IN HDAUDIO_PREMEM_CONFIG     *HdaPreMemConfig,
  IN UINT32                    MasterClockIndex
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;
  UINT8              SspLinksEnabled;
  UINT8              Index;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

  SspLinksEnabled = 0;
  for (Index = 0; Index < GetPchHdaMaxSspLinkNum (); Index++) {
    if (HdaPreMemConfig->AudioLinkSsp[Index].Enable) {
      SspLinksEnabled++;
    }
  }
  if (SspLinksEnabled == 0 ||
     (HdaPreMemConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled && SspLinksEnabled == 1)) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_I2S_MCLK, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: I2S_MCLK GPIO programming failed, Status = %r.\n", Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "HDA: MasterClock#%d - GPIOs programmed successfully\n", MasterClockIndex));
}

/*
  Configure Sndw Audio Link GPIOs

  @param[in] SndwIndex       Sndw Link Index
*/
VOID
PtlPcdHdaConfigureSndwLinks (
  IN UINT32                  SndwIndex
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

  //
  // GPIO configuration for 5th Sndw controller should be skipped.
  // This controller is used for BT/CNVi and iDisp devices over virtual pins.
  //
  if (SndwIndex == 4) {
    return;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_CLK (SndwIndex), 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: SNDW_CLK#%d GPIO programming failed, Status = %r.\n", SndwIndex, Status));
    return;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_DATA0 (SndwIndex), 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: SNDW_DATA0#%d GPIO programming failed, Status = %r.\n", SndwIndex, Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "HDA: Sndw#%d - GPIOs programmed successfully\n", SndwIndex));
}

/*
  Function disables HDA Device on PMC level.
*/
VOID
PtlPcdHdaPmcDisableDevice (
  VOID
  )
{
}

/*
  Function disables HDA Device on PSF level.
*/
VOID
PtlPcdHdaPsfDisableDevice (
  VOID
  )
{
  PtlPcdPsfDisableAce ();
}

/*
  Function disables DSP Bar on PSF level.
*/
VOID
PtlPcdHdaPsfDisableDspBar (
  VOID
  )
{
  PtlPcdPsfDisableDspBar ();
}

/*
  Function base on type of link and number of link return if link is supported.

  @param[in] AudioLinkType   Link type support to be checked
  @param[in] AudioLinkIndex  Link number

  @retval    TRUE           Link supported
  @retval    FALSE          Link not supported
*/
BOOLEAN
PtlPcdHdaIsAudioInterfaceSupported (
  IN HDAUDIO_LINK_TYPE     AudioLinkType,
  IN UINT32                AudioLinkIndex
  )
{
  return IsAudioInterfaceSupported (AudioLinkType, AudioLinkIndex);
}

/*
  Function check if Tmode is supported for IDisp codec.

  @retval    TRUE           Tmode supported
  @retval    FALSE          Tmode not supported
*/
BOOLEAN
PtlPcdAceIsAudioIDispTmodeSupported (
  IN HDAUDIO_IDISP_TMODE Tmode
  )
{
  switch (Tmode) {
    case HdaIDispMode1T:
    case HdaIDispMode2T:
    case HdaIDispMode4T:
    case HdaIDispMode8T:
    case HdaIDispMode16T:
      return TRUE;
    default:
      return FALSE;
  }
}

/*
  Functions initialize IDisp audio codec in SA.
*/
EFI_STATUS
PtlPcdAceInitializeDisplayAudioCodec (
  VOID
  )
{
  return EFI_SUCCESS;
}

/*
  Function base on type of link and number of link return if link is supported.

  @param[in] SndwIndex             SoundWire Link Index
  @param[in] SndwMultilaneSelect   Multilane configuration for the given sndw interface
*/
EFI_STATUS
PtlPcdConfigureSndwMultilane (
  IN HDAUDIO_PREMEM_CONFIG         *HdaPreMemConfig,
  IN UINT8                         MultilaneIndex
  )
{
  HDAUDIO_SNDW_MULTILANE_SELECT    SndwMultilaneSelect;
  GPIOV2_SERVICES                  *GpioServices;
  EFI_STATUS                       Status;
  UINT32                           SndwInterface;

  SndwMultilaneSelect = HdaPreMemConfig->SndwMultilane[MultilaneIndex].Enable;
  SndwInterface       = HdaPreMemConfig->SndwMultilane[MultilaneIndex].SndwInterface;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }
   Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return Status;
  }

  if (SndwMultilaneSelect != HdaSndwMultilaneDisabled) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_CLK (SndwInterface), HdaPreMemConfig->SndwMultilane[MultilaneIndex].PinMux.Clk);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HDA: MULTILANE#%d SNDW_CLK#%d GPIO programming failed, Status = %r.\n", MultilaneIndex, SndwInterface, Status));
      return Status;
    }

    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_DATA0 (SndwInterface), HdaPreMemConfig->SndwMultilane[MultilaneIndex].PinMux.Data0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HDA: MULTILANE#%d SNDW_DATA0#%d GPIO programming failed, Status = %r.\n", MultilaneIndex, SndwInterface, Status));
      return Status;
    }

    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_DATA1 (SndwInterface), HdaPreMemConfig->SndwMultilane[MultilaneIndex].PinMux.Data1);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HDA: MULTILANE#%d SNDW_DATA1#%d GPIO programming failed, Status = %r.\n", MultilaneIndex, SndwInterface, Status));
      return Status;
    }

    if (SndwMultilaneSelect >= HdaSndwMultilaneThreeDataLanes) {
      Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_DATA2 (SndwInterface), HdaPreMemConfig->SndwMultilane[MultilaneIndex].PinMux.Data2);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "HDA: MULTILANE#%d SNDW_DATA2#%d GPIO programming failed, Status = %r.\n", MultilaneIndex, SndwInterface, Status));
        return Status;
      }
    }

    if (SndwMultilaneSelect == HdaSndwMultilaneFourDataLanes) {
      Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SNDW_DATA3 (SndwInterface), HdaPreMemConfig->SndwMultilane[MultilaneIndex].PinMux.Data3);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "HDA: MULTILANE#%d SNDW_DATA3#%d GPIO programming failed, Status = %r.\n", MultilaneIndex, SndwInterface, Status));
        return Status;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "HDA: Multilane#%d on Sndw#%d with config 0x%x - GPIOs programmed successfully\n", MultilaneIndex, SndwInterface, SndwMultilaneSelect));

  return EFI_SUCCESS;
}

/*
  Configure GPIO pins related to Microphone Privacy
*/
EFI_STATUS
PtlPcdConfigureMicrophonePrivacy (
  VOID
  )
{
  GPIOV2_SERVICES                  *GpioServices;
  EFI_STATUS                       Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

   Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return Status;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_MISC_MIC_MUTE, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: Microphone Privacy Mic Mute, Status = %r.\n", Status));
    return Status;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_MISC_MIC_MUTE_LED, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: Microphone Privacy Mic Mute Led, Status = %r.\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "HDA: Microphone Privacy - GPIOs programmed successfully\n"));

  return EFI_SUCCESS;
}

/*
  Create HDA controller instance.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in] HdaController        Pointer to Hda device structure
  @param[in] HdaCallbacks         Pointer to Hda callbacks structure
  @param[in] SiPolicy             Pointer to SiPolicy
  @param[in] SiPreMemPolicy       Pointer to SiPreMemPolicy
*/
EFI_STATUS
PtlPcdCreateHdaHandle (
  OUT  HDA_HANDLE                 *HdaHandle,
  IN   HDA_PRIVATE_CONFIG         *HdaPrivateConfig, OPTIONAL
  IN   HDA_CONTROLLER             *HdaController,    OPTIONAL
  IN   HDA_CALLBACK               *HdaCallback,      OPTIONAL
  IN   SI_POLICY_PPI              *SiPolicy,         OPTIONAL
  IN   SI_PREMEM_POLICY_PPI       *SiPreMemPolicy    OPTIONAL
  )
{
  EFI_STATUS                    Status;
  HDAUDIO_CONFIG                *HdaConfig;
  HDAUDIO_PREMEM_CONFIG         *HdaPreMemConfig;

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdaConfig);
    ASSERT_EFI_ERROR (Status);
    HdaHandle->HdaConfig = HdaConfig;
  } else {
    HdaHandle->HdaConfig = NULL;
  }

  if (SiPreMemPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    HdaHandle->HdaPreMemConfig = HdaPreMemConfig;
  } else {
    HdaHandle->HdaPreMemConfig = NULL;
  }

  if (HdaController != NULL) {
    HdaController->Segment        = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
    HdaController->Bus            = DEFAULT_PCI_BUS_NUMBER_PCH;
    HdaController->Device         = HdaDevNumber ();
    HdaController->Function       = HdaFuncNumber ();
    HdaController->PciCfgBaseAddr = HdaPciCfgBase ();
    HdaController->HdaMmioAddress = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
    HdaController->DspMmioAddress = PcdGet32 (PcdSiliconInitTempMemBaseAddr) + (UINT32) V_ACE_HDABAR_SIZE;
  }

  if (HdaCallback != NULL) {
    HdaCallback->HdaConfigureHdaLink                  = &PtlPcdHdaConfigureHdaLink;
    HdaCallback->HdaConfigureSndwLinks                = &PtlPcdHdaConfigureSndwLinks;
    HdaCallback->HdaConfigureDmicLinks                = &PtlPcdHdaConfigureDmicLinks;
    HdaCallback->HdaConfigureSspLinks                 = &PtlPcdHdaConfigureSspLinks;
    HdaCallback->HdaConfigureSspMasterClock           = &PtlPcdHdaConfigureSspMasterClock;
    HdaCallback->HdaPmcDisableDevice                  = &PtlPcdHdaPmcDisableDevice;
    HdaCallback->HdaPsfDisableDevice                  = &PtlPcdHdaPsfDisableDevice;
    HdaCallback->HdaPsfDisableDspBar                  = &PtlPcdHdaPsfDisableDspBar;
    HdaCallback->HdaIsAudioInterfaceSupported         = &PtlPcdHdaIsAudioInterfaceSupported;
    HdaCallback->HdaInitializeDisplayAudioCodec       = &PtlPcdAceInitializeDisplayAudioCodec;
    HdaCallback->HdaConfigureIDispAudioCodecFrequency = NULL;
    HdaCallback->HdaPcrPidDspAndThenOr32              = NULL;
    HdaCallback->HdaPcrPidDspRead32                   = NULL;
    HdaCallback->HdaIsAudioIDispTmodeSupported        = &PtlPcdAceIsAudioIDispTmodeSupported;
    HdaCallback->HdaConfigureSndwMultilane            = &PtlPcdConfigureSndwMultilane;
    HdaCallback->HdaConfigureMicrophonePrivacy        = &PtlPcdConfigureMicrophonePrivacy;
  }

  if (HdaPrivateConfig != NULL) {
    HdaPrivateConfig->HdaMaxSndwLinkNum      = GetPchHdaMaxSndwLinkNum ();
    HdaPrivateConfig->HdaMaxSndwMultilaneNum = GetPchHdaMaxSndwMultilaneNum ();
    HdaPrivateConfig->HdaMaxDmicLinkNum      = GetPchHdaMaxDmicLinkNum ();
    HdaPrivateConfig->HdaMaxSspLinkNum       = GetPchHdaMaxSspLinkNum ();
    HdaPrivateConfig->NodeId                 = 0x2;
    HdaPrivateConfig->SkipIGfxProgramming    = FALSE;
    if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
      HdaPrivateConfig->IGfxEnabled = FALSE;
    } else {
      HdaPrivateConfig->IGfxEnabled = TRUE;
    }
    HdaPrivateConfig->TrafficClassAssignmentUsb = 0xFF; // 0xFF does not overwrite TCUSB register value
    HdaPrivateConfig->MicPrivacySupported       = TRUE;
  }

  HdaHandle->HdaController     = HdaController;
  HdaHandle->HdaCallback       = HdaCallback;
  HdaHandle->HdaPrivateConfig  = HdaPrivateConfig;

  return EFI_SUCCESS;
}

/**
  Initialize the ACE (Audio & Context Subsystem). Program of FNCFG register before P2SB Lock

  @param[in] SiPreMemPolicyPpi       Pre-mem Policy

**/
VOID
PtlPcdAcePostMemInit (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_HOB_POINTERS          HobList;
  HDA_HANDLE                    HdaHandle;
  HDA_CONTROLLER                HdaController;
  HDA_CALLBACK                  HdaCallback;
  HDA_PRIVATE_CONFIG            HdaPrivateConfig;
  SI_POLICY_PPI                 *SiPolicyPpi;
  HDAUDIO_CONFIG_HOB            *HdaConfigHob;
  UINT32                        XtalValue;
  UINT8                         InterruptPin;
  VOID                          *HobPtr;
  PCH_INFO_HOB                  *PchInfoHob;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  XtalValue = 0;

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  PtlPcdCreateHdaHandle (&HdaHandle, &HdaPrivateConfig, &HdaController, &HdaCallback, SiPolicyPpi, SiPreMemPolicyPpi);
  PrintHdaHandle (&HdaHandle);

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return; //return is necessary because RELEASE build removes ASSERT
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);

  AceEnableHdaDspMmioAccess (&HdaController);

  //
  // Should be executed before disbaling DSP bar for ACE.
  //
  PchInfoHob->AudioDspFusedOut = AceIsDspFusedOut (&HdaController);

  InterruptPin = ItssGetDevIntPin (SiPolicyPpi, HdaController.Device, HdaController.Function);
  AceConfigureHdAudio (&HdaHandle, &XtalValue, InterruptPin);

  //
  // Get HDA HOB.
  //
  HobList.Guid = GetFirstGuidHob (&gPchHdaConfigHobGuid);
  ASSERT (HobList.Guid != NULL);
  HdaConfigHob = (HDAUDIO_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);
  HdaConfigHob->AudioXtal = XtalValue;

  AceConfigureHdAudioCodec (&HdaHandle);
  AceDisableHdaDspMmioAccess (&HdaController);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Initialize the ACE (Audio & Context Subsystem) on Pre-mem phase

  @param[in] SiPreMemPolicyPpi     The SI Pre-mem Policy PPI instance
**/
VOID
PtlPcdAcePreMemInit (
  IN  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                       Status;
  HDAUDIO_PREMEM_CONFIG            *HdaPreMemConfig;
  UINT64                           PciCfgBaseAddr;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  PciCfgBaseAddr = HdaPciCfgBase ();

  if (PciSegmentRead16 (PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "ACE device is not present!\n"));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (HdaPreMemConfig->Enable == FALSE) {
    DEBUG ((DEBUG_INFO, "%a () - End. ACE interface is disabled. \n", __FUNCTION__));
    return;
  }

  ///
  ///
  /// Enable GPIOs for HDA
  if (HdaPreMemConfig->AudioLinkHda.Enable && PtlPcdHdaIsAudioInterfaceSupported (HdaLink, 0)) {
    PtlPcdHdaConfigureHdaLink (HdaPreMemConfig);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}
