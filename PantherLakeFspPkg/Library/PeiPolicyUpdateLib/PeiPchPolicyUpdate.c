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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/Ptl/PcdPolicyLib/PtlPcdPolicyLib.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>
#include <GbeConfig.h>
#include <HdAudioConfig.h>
#include <CnviConfig.h>
#include <EspiConfig.h>
#include <ScsConfig.h>
#include <UsbConfig.h>
#include <Usb2PhyConfig.h>
#include <PchPcieRpConfig.h>
#include <SerialIoConfig.h>
#include <LpssI3cConfig.h>
#include <InterruptConfig.h>
#include <LockDownConfig.h>
#include <RtcConfig.h>
#include <ThcConfig.h>
#include <IehConfig.h>
#include <FlashProtectionConfig.h>
#include <IoApicConfig.h>
#include <P2sbConfig.h>
#include <PmConfig.h>
#include <AdrConfig.h>
#include <IshConfig.h>
#include <Usb3HsioConfig.h>
#include <ThermalConfig.h>
#include <FivrConfig.h>
#include <LpssI2cConfig.h>
#include <LpssUartConfig.h>
#include <LpssSpiConfig.h>
#include <HsioConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#if FixedPcdGet8 (PcdTsnSupport) == 0x1
#include <TsnConfig.h>
#endif

/**
  Update GBE policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateGbePolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  GBE_CONFIG        *GbeConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  GbeConfig->Enable                = FspsUpd->FspsConfig.PchLanEnable;
  GbeConfig->LtrEnable             = FspsUpd->FspsConfig.PchLanLtrEnable;
  GbeConfig->PchWOLFastSupport     = FspsUpd->FspsConfig.PchLanWOLFastSupport;
}

#if FixedPcdGet8 (PcdTsnSupport) == 0x1
/**
  Update TSN policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateTsnPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  TSN_CONFIG    *TsnConfig;
  EFI_STATUS    Status;
  UINT8         Index;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gTsnConfigGuid, (VOID *) &TsnConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < PCH_MAX_TSN_PORT; Index++) {
    TsnConfig->Enable[Index]    = FspsUpd->FspsConfig.PchTsnEnable[Index];
  }
  TsnConfig->MultiVcEnable      = FspsUpd->FspsConfig.PchTsnMultiVcEnable;

  TsnConfig->Port[0].MacAddr[1] = FspsUpd->FspsConfig.PchTsn1MacAddressHigh;
  TsnConfig->Port[0].MacAddr[0] = FspsUpd->FspsConfig.PchTsn1MacAddressLow;
  TsnConfig->Port[1].MacAddr[1] = FspsUpd->FspsConfig.PchTsn2MacAddressHigh;
  TsnConfig->Port[1].MacAddr[0] = FspsUpd->FspsConfig.PchTsn2MacAddressLow;
  TsnConfig->Port[2].MacAddr[1] = FspsUpd->FspsConfig.PchTsn3MacAddressHigh;
  TsnConfig->Port[2].MacAddr[0] = FspsUpd->FspsConfig.PchTsn3MacAddressLow;
  TsnConfig->Port[3].MacAddr[1] = FspsUpd->FspsConfig.PchTsn4MacAddressHigh;
  TsnConfig->Port[3].MacAddr[0] = FspsUpd->FspsConfig.PchTsn4MacAddressLow;
}
#endif

/**
  Update HD Audio policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateHdAudioPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  HDAUDIO_CONFIG    *HdAudioConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  HdAudioConfig->Pme                   = FspsUpd->FspsConfig.PchHdaPme;
  HdAudioConfig->HdAudioLinkFrequency  = FspsUpd->FspsConfig.PchHdaLinkFrequency;
  HdAudioConfig->CodecSxWakeCapability = FspsUpd->FspsConfig.PchHdaCodecSxWakeCapability;
  HdAudioConfig->VerbTableEntryNum     = FspsUpd->FspsConfig.PchHdaVerbTableEntryNum;
  HdAudioConfig->VerbTablePtr          = (UINTN) FspsUpd->FspsConfig.PchHdaVerbTablePtr;

  HdAudioConfig->MicPrivacy.Mode                            = FspsUpd->FspsConfig.PchHdaMicPrivacyMode;
  HdAudioConfig->MicPrivacy.Deglitch                        = FspsUpd->FspsConfig.PchHdaMicPrivacyDeglitch;
  HdAudioConfig->MicPrivacy.HwModeSoundWire0                = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeSoundWire0;
  HdAudioConfig->MicPrivacy.HwModeSoundWire1                = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeSoundWire1;
  HdAudioConfig->MicPrivacy.HwModeSoundWire2                = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeSoundWire2;
  HdAudioConfig->MicPrivacy.HwModeSoundWire3                = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeSoundWire3;
  HdAudioConfig->MicPrivacy.HwModeSoundWire4                = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeSoundWire4;
  HdAudioConfig->MicPrivacy.HwModeDmic                      = FspsUpd->FspsConfig.PchHdaMicPrivacyHwModeDmic;
  HdAudioConfig->MicPrivacy.Timeout                         = FspsUpd->FspsConfig.PchHdaMicPrivacyTimeout;
}

/**
  Update Hsio policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateHsioPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  HSIO_CONFIG      *HsioConfig;
  EFI_STATUS        Status;

  HsioConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHsioConfigGuid, (VOID *) &HsioConfig);

  if (EFI_ERROR (Status) || HsioConfig == NULL) {
    return;
  }

  //
  // Update Pch HSIO policies
  //
  HsioConfig->ChipsetInitBinPtr = (VOID *) (UINTN) (FspsUpd->FspsConfig.ChipsetInitBinPtr);
  HsioConfig->ChipsetInitBinLen = FspsUpd->FspsConfig.ChipsetInitBinLen;
  HsioConfig->NphyBinPtr        = (VOID *) (UINTN) (FspsUpd->FspsConfig.NphyBinPtr);
  HsioConfig->NphyBinLen        = FspsUpd->FspsConfig.NphyBinLen;
  HsioConfig->SynpsPhyBinPtr    = (VOID *) (UINTN) (FspsUpd->FspsConfig.SynpsPhyBinPtr);
  HsioConfig->SynpsPhyBinLen    = FspsUpd->FspsConfig.SynpsPhyBinLen;
}

/**
  Update Cnvi policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateCnviPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  CNVI_CONFIG       *CnviConfig;
  EFI_STATUS        Status;

  CnviConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &CnviConfig);

  if (EFI_ERROR (Status) || CnviConfig == NULL) {
    return;
  }

  CnviConfig->Mode           = FspsUpd->FspsConfig.CnviMode;
  CnviConfig->WifiCore       = FspsUpd->FspsConfig.CnviWifiCore;
  CnviConfig->BtCore         = FspsUpd->FspsConfig.CnviBtCore;
  CnviConfig->BtInterface    = FspsUpd->FspsConfig.CnviBtInterface;
  CnviConfig->BtAudioOffload = FspsUpd->FspsConfig.CnviBtAudioOffload;
  CnviConfig->BtAudioOffloadInterface = FspsUpd->FspsConfig.CnviBtAudioOffloadInterface;
  CnviConfig->PinMux.RfReset = FspsUpd->FspsConfig.CnviRfResetPinMux;
  CnviConfig->PinMux.Clkreq  = FspsUpd->FspsConfig.CnviClkreqPinMux;
}

/**
  Update Espi policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateEspiPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_ESPI_CONFIG   *EspiConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  EspiConfig->LgmrEnable = FspsUpd->FspsConfig.PchEspiLgmrEnable;
  EspiConfig->BmeHostDeviceEnabled = FspsUpd->FspsConfig.PchEspiBmeHostDeviceEnabled;
  EspiConfig->HostC10ReportEnable = FspsUpd->FspsConfig.PchEspiHostC10ReportEnable;
  EspiConfig->LockLinkConfiguration = FspsUpd->FspsConfig.PchEspiLockLinkConfiguration;
  EspiConfig->EspiPmHAE = FspsUpd->FspsConfig.PchEspiPmHAE;
  EspiConfig->HideNonFatalErrors = FspsUpd->FspsConfig.PchEspiHideNonFatalErrors;
  EspiConfig->NmiEnableCs1 = FspsUpd->FspsConfig.PchEspiNmiEnableCs1;
}

/**
  Update UFS policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateUfsPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  SCS_UFS_CONFIG  *UfsConfig;
  UINT8           UfsIndex;
  EFI_STATUS      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUfsConfigGuid, (VOID *) &UfsConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // FSPS UPD has hard allocated configuration space for only 2 UFS controllers. Be sure to change this loop termination
  // condition after adding more space in FSPS UPD.
  //
  for (UfsIndex = 0; (UfsIndex < PchGetMaxUfsNum ()) && (UfsIndex < 2); UfsIndex++) {
    UfsConfig->UfsControllerConfig[UfsIndex].Enable = FspsUpd->FspsConfig.UfsEnable[UfsIndex];
    UfsConfig->UfsControllerConfig[UfsIndex].InlineEncryption = FspsUpd->FspsConfig.UfsInlineEncryption[UfsIndex];
  }
}

/**
  Update Ish policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateIshPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN             Index;
  UINTN             CsIndex;
  ISH_CONFIG        *IshConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < GetPchMaxIshSpiControllersNum (); Index++) {
    if (Index < PCH_MAX_ISH_SPI_CONTROLLERS){
      IshConfig->Spi[Index].Enable                 = FspsUpd->FspsConfig.PchIshSpiEnable[Index];
      IshConfig->Spi[Index].CsEnable[0]            = FspsUpd->FspsConfig.PchIshSpiCs0Enable[Index];
      IshConfig->Spi[Index].PinConfig.Mosi.PinMux  = FspsUpd->FspsConfig.IshSpiMosiPinMuxing[Index];
      IshConfig->Spi[Index].PinConfig.Miso.PinMux  = FspsUpd->FspsConfig.IshSpiMisoPinMuxing[Index];
      IshConfig->Spi[Index].PinConfig.Clk.PinMux   = FspsUpd->FspsConfig.IshSpiClkPinMuxing[Index];

      IshConfig->Spi[Index].PinConfig.Mosi.PadTermination = FspsUpd->FspsConfig.IshSpiMosiPadTermination[Index];
      IshConfig->Spi[Index].PinConfig.Miso.PadTermination = FspsUpd->FspsConfig.IshSpiMisoPadTermination[Index];
      IshConfig->Spi[Index].PinConfig.Clk.PadTermination  = FspsUpd->FspsConfig.IshSpiClkPadTermination[Index];
      for (CsIndex = 0; CsIndex < PCH_MAX_ISH_SPI_CS_PINS; CsIndex++) {
        IshConfig->Spi[Index].CsEnable[CsIndex]            = FspsUpd->FspsConfig.PchIshSpiCsEnable[PCH_MAX_ISH_SPI_CS_PINS * Index + CsIndex];
        IshConfig->Spi[Index].PinConfig.Cs[CsIndex].PinMux = FspsUpd->FspsConfig.IshSpiCsPinMuxing[PCH_MAX_ISH_SPI_CS_PINS * Index + CsIndex];
        IshConfig->Spi[Index].PinConfig.Cs[CsIndex].PadTermination = FspsUpd->FspsConfig.IshSpiCsPadTermination[PCH_MAX_ISH_SPI_CS_PINS * Index + CsIndex];
      }
    }
  }
  for (Index = 0; Index < GetPchMaxIshUartControllersNum (); Index++) {
    IshConfig->Uart[Index].Enable               = FspsUpd->FspsConfig.PchIshUartEnable[Index];
    IshConfig->Uart[Index].PinConfig.Rx.PinMux  = FspsUpd->FspsConfig.IshUartRxPinMuxing[Index];
    IshConfig->Uart[Index].PinConfig.Tx.PinMux  = FspsUpd->FspsConfig.IshUartTxPinMuxing[Index];
    IshConfig->Uart[Index].PinConfig.Rts.PinMux = FspsUpd->FspsConfig.IshUartRtsPinMuxing[Index];
    IshConfig->Uart[Index].PinConfig.Cts.PinMux = FspsUpd->FspsConfig.IshUartCtsPinMuxing[Index];

    IshConfig->Uart[Index].PinConfig.Rx.PadTermination  = FspsUpd->FspsConfig.IshUartRxPadTermination[Index];
    IshConfig->Uart[Index].PinConfig.Tx.PadTermination  = FspsUpd->FspsConfig.IshUartTxPadTermination[Index];
    IshConfig->Uart[Index].PinConfig.Rts.PadTermination = FspsUpd->FspsConfig.IshUartRtsPadTermination[Index];
    IshConfig->Uart[Index].PinConfig.Cts.PadTermination = FspsUpd->FspsConfig.IshUartCtsPadTermination[Index];
  }
  for (Index = 0; Index < GetPchMaxIshI2cControllersNum (); Index++) {
    IshConfig->I2c[Index].Enable               = FspsUpd->FspsConfig.PchIshI2cEnable[Index];
    IshConfig->I2c[Index].PinConfig.Sda.PinMux = FspsUpd->FspsConfig.IshI2cSdaPinMuxing[Index];
    IshConfig->I2c[Index].PinConfig.Scl.PinMux = FspsUpd->FspsConfig.IshI2cSclPinMuxing[Index];

    IshConfig->I2c[Index].PinConfig.Sda.PadTermination = FspsUpd->FspsConfig.IshI2cSdaPadTermination[Index];
    IshConfig->I2c[Index].PinConfig.Scl.PadTermination = FspsUpd->FspsConfig.IshI2cSclPadTermination[Index];
  }

  for (Index = 0; Index < GetPchMaxIshGpNum (); Index++) {
    IshConfig->Gp[Index].Enable           = FspsUpd->FspsConfig.PchIshGpEnable[Index];
    IshConfig->Gp[Index].PinConfig.PinMux = FspsUpd->FspsConfig.IshGpGpioPinMuxing[Index];
    IshConfig->Gp[Index].PinConfig.PadTermination = FspsUpd->FspsConfig.IshGpGpioPadTermination[Index];
  }

  IshConfig->PdtUnlock = FspsUpd->FspsConfig.PchIshPdtUnlock;
  IshConfig->MsiInterrupt = FspsUpd->FspsConfig.PchIshMsiInterrupt;
}

/**
  Update Usb policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateUsbPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN             Index;
  USB_CONFIG        *UsbConfig;
  EFI_STATUS        Status;
  UINT8             Usb2PortCount;
  UINT8             Usb3PortCount;


  Usb2PortCount = PtlPcdGetUsb2MaxPhysicalPortNum ();
  Usb3PortCount = PtlPcdGetXhciMaxUsb3PortNum ();

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  UsbConfig->PdoProgramming         = FspsUpd->FspsConfig.UsbPdoProgramming;
  UsbConfig->OverCurrentEnable      = FspsUpd->FspsConfig.PchUsbOverCurrentEnable;

  UsbConfig->XhciOcLock             = FspsUpd->FspsConfig.PchXhciOcLock;
  UsbConfig->UaolEnable             = FspsUpd->FspsConfig.PchXhciUaolEnable;

  for (Index = 0; Index < Usb2PortCount; Index++) {
    UsbConfig->PortUsb20[Index].Enable         = FspsUpd->FspsConfig.PortUsb20Enable[Index];
    UsbConfig->PortUsb20[Index].OverCurrentPin = FspsUpd->FspsConfig.Usb2OverCurrentPin[Index];
    UsbConfig->PortUsb20[Index].PortResetMessageEnable = FspsUpd->FspsConfig.PortResetMessageEnable[Index];
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
    UsbConfig->PortUsb20[Index].SwDeviceModeEnable = FspsUpd->FspsConfig.PortUsb20SwDeviceModeEnable[Index];
#endif
  }

  for (Index = 0; Index < Usb3PortCount; Index++) {
    UsbConfig->PortUsb30[Index].Enable         = FspsUpd->FspsConfig.PortUsb30Enable[Index];
    UsbConfig->PortUsb30[Index].OverCurrentPin = FspsUpd->FspsConfig.Usb3OverCurrentPin[Index];
  }

  UsbConfig->LtrOverrideEnable           = FspsUpd->FspsConfig.PchUsbLtrOverrideEnable;
  if (UsbConfig->LtrOverrideEnable) {
    UsbConfig->LtrHighIdleTimeOverride   = FspsUpd->FspsConfig.PchUsbLtrHighIdleTimeOverride;
    UsbConfig->LtrMediumIdleTimeOverride = FspsUpd->FspsConfig.PchUsbLtrMediumIdleTimeOverride;
    UsbConfig->LtrLowIdleTimeOverride    = FspsUpd->FspsConfig.PchUsbLtrLowIdleTimeOverride;
  }

  UsbConfig->XdciConfig.Enable  = FspsUpd->FspsConfig.XdciEnable;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UsbConfig->Usb31Speed         = FspsUpd->FspsConfig.PortUsb31Speed;
#endif

}

/**
  Update Usb2Phy policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateUsb2PhyPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{

  UINTN Index;
  USB2_PHY_CONFIG   *Usb2PhyConfig;
  EFI_STATUS        Status;
  UINT8             PortCount;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PortCount = PtlPcdGetUsb2MaxPhysicalPortNum ();
  for (Index = 0; Index < PortCount; Index++) {
    Usb2PhyConfig->Port[Index].Petxiset   = FspsUpd->FspsConfig.Usb2PhyPetxiset[Index];
    Usb2PhyConfig->Port[Index].Txiset     = FspsUpd->FspsConfig.Usb2PhyTxiset[Index];
    Usb2PhyConfig->Port[Index].Predeemp   = FspsUpd->FspsConfig.Usb2PhyPredeemp[Index];
    Usb2PhyConfig->Port[Index].Pehalfbit  = FspsUpd->FspsConfig.Usb2PhyPehalfbit[Index];
  }
}

/**
  Update Usb3Hsio policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateUsb3HsioPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN             Index;
  USB3_HSIO_CONFIG  *Usb3HsioConfig;
  EFI_STATUS        Status;
  UINT8             PortCount;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb3HsioConfigGuid, (VOID *) &Usb3HsioConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PortCount = PtlPcdGetXhciMaxUsb3PortNum ();

  for (Index = 0; Index < PortCount; Index++) {
    Usb3HsioConfig->Port[Index].HsioTxDeEmphEnable             = FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxDeEmph                   = FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index];

    Usb3HsioConfig->Port[Index].HsioTxDownscaleAmpEnable       = FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmpEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxDownscaleAmp             = FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmp[Index];

    Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfgEnable   = FspsUpd->FspsConfig.PchUsb3HsioCtrlAdaptOffsetCfgEnable[Index];
    Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfg         = FspsUpd->FspsConfig.PchUsb3HsioCtrlAdaptOffsetCfg[Index];

    Usb3HsioConfig->Port[Index].HsioFilterSelPEnable           = FspsUpd->FspsConfig.PchUsb3HsioFilterSelPEnable[Index];
    Usb3HsioConfig->Port[Index].HsioFilterSelP                 = FspsUpd->FspsConfig.PchUsb3HsioFilterSelP[Index];

    Usb3HsioConfig->Port[Index].HsioFilterSelNEnable           = FspsUpd->FspsConfig.PchUsb3HsioFilterSelNEnable[Index];
    Usb3HsioConfig->Port[Index].HsioFilterSelN                 = FspsUpd->FspsConfig.PchUsb3HsioFilterSelN[Index];

    Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnResEnable = FspsUpd->FspsConfig.PchUsb3HsioOlfpsCfgPullUpDwnResEnable[Index];
    Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnRes       = FspsUpd->FspsConfig.PchUsb3HsioOlfpsCfgPullUpDwnRes[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate0UniqTranEnable      = FspsUpd->FspsConfig.Usb3HsioTxRate0UniqTranEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate0UniqTran            = FspsUpd->FspsConfig.Usb3HsioTxRate0UniqTran[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate1UniqTranEnable      = FspsUpd->FspsConfig.Usb3HsioTxRate1UniqTranEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate1UniqTran            = FspsUpd->FspsConfig.Usb3HsioTxRate1UniqTran[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate2UniqTranEnable      = FspsUpd->FspsConfig.Usb3HsioTxRate2UniqTranEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate2UniqTran            = FspsUpd->FspsConfig.Usb3HsioTxRate2UniqTran[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate3UniqTranEnable      = FspsUpd->FspsConfig.Usb3HsioTxRate3UniqTranEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxRate3UniqTran            = FspsUpd->FspsConfig.Usb3HsioTxRate3UniqTran[Index];
  }
}

/**
  Update PcieRp policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
  @param[in] MaxPcieRootPorts   Maximum Number of PCIe RootPorts
**/
STATIC
VOID
FspUpdatePcieRpPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN             Index;
  UINTN             MaxPcieRootPorts;
  PCH_PCIE_CONFIG   *PchPcieConfig;
  FIA_CONFIG        *FiaConfig;
  EFI_STATUS        Status;
  UINTN             RpIndex;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFiaConfigGuid, (VOID *) &FiaConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  MaxPcieRootPorts = GetPchMaxPciePortNum ();

  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HotPlug                  = FspsUpd->FspsConfig.PcieRpHotPlug[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci                    = FspsUpd->FspsConfig.PcieRpPmSci[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.TransmitterHalfSwing     = FspsUpd->FspsConfig.PcieRpTransmitterHalfSwing[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClkReqDetect             = FspsUpd->FspsConfig.PcieRpClkReqDetect[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AdvancedErrorReporting   = FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.UnsupportedRequestReport = FspsUpd->FspsConfig.PcieRpUnsupportedRequestReport[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FatalErrorReport         = FspsUpd->FspsConfig.PcieRpFatalErrorReport[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoFatalErrorReport       = FspsUpd->FspsConfig.PcieRpNoFatalErrorReport[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CorrectableErrorReport   = FspsUpd->FspsConfig.PcieRpCorrectableErrorReport[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnFatalError  = FspsUpd->FspsConfig.PcieRpSystemErrorOnFatalError[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnNonFatalError    = FspsUpd->FspsConfig.PcieRpSystemErrorOnNonFatalError[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnCorrectableError = FspsUpd->FspsConfig.PcieRpSystemErrorOnCorrectableError[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload               = FspsUpd->FspsConfig.PcieRpMaxPayload[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed                = FspsUpd->FspsConfig.PcieRpPcieSpeed[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber       = FspsUpd->FspsConfig.PcieRpPhysicalSlotNumber[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented          = FspsUpd->FspsConfig.PcieRpSlotImplemented[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CompletionTimeout        = FspsUpd->FspsConfig.PcieRpCompletionTimeout[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm                     = FspsUpd->FspsConfig.PcieRpAspm[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HostL0sTxDis             = FspsUpd->FspsConfig.HostL0sTxDis[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates              = FspsUpd->FspsConfig.PcieRpL1Substates[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable                = FspsUpd->FspsConfig.PcieRpLtrEnable[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock  = FspsUpd->FspsConfig.PcieRpLtrConfigLock[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled                       = FspsUpd->FspsConfig.PcieRpAcsEnabled[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnableCpm     = FspsUpd->FspsConfig.PcieRpEnableCpm[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs                  = FspsUpd->FspsConfig.PcieRpDetectTimeoutMs[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled = FspsUpd->FspsConfig.PciePtm[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency       = FspsUpd->FspsConfig.PcieRpLtrMaxSnoopLatency[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency     = FspsUpd->FspsConfig.PcieRpLtrMaxNoSnoopLatency[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode       = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMode[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMultiplier[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue      = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideValue[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode       = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMode[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMultiplier[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue      = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideValue[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitScale      = FspsUpd->FspsConfig.PcieRpSlotPowerLimitScale[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitValue      = FspsUpd->FspsConfig.PcieRpSlotPowerLimitValue[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnablePeerMemoryWrite    = FspsUpd->FspsConfig.PcieEnablePeerMemoryWrite[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LinkDownGpios            = FspsUpd->FspsConfig.PcieRpLinkDownGpios[Index];

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PowerGating              = FspsUpd->FspsConfig.PciePowerGating[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClockGating              = FspsUpd->FspsConfig.PcieClockGating[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AutoPowerGating          = FspsUpd->FspsConfig.PcieAutoPowerGating[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.VisaClockGating          = FspsUpd->FspsConfig.PcieVisaClockGating[Index];
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieFomsCp               = FspsUpd->FspsConfig.PcieFomsCp[Index];
	PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EqPhBypass               = FspsUpd->FspsConfig.PcieEqPhBypass[Index];
  }

  //
  // Update PCIE RP policies
  //
  PchPcieConfig->PcieCommonConfig.EnablePort8xhDecode           = FspsUpd->FspsConfig.PcieEnablePort8xhDecode;
  PchPcieConfig->PchPciePort8xhDecodePortIndex                  = FspsUpd->FspsConfig.PchPciePort8xhDecodePortIndex;
  PchPcieConfig->PcieCommonConfig.ComplianceTestMode            = FspsUpd->FspsConfig.PcieComplianceTestMode;
  PchPcieConfig->PcieCommonConfig.RpFunctionSwap                = FspsUpd->FspsConfig.PcieRpFunctionSwap;
  PchPcieConfig->PcieCommonConfig.PhyAutoPowerGating            = FspsUpd->FspsConfig.PciePhyAutoPowerGating;


  FiaConfig->FiaProgramming                                     = FspsUpd->FspsConfig.PcieFiaProgramming;

  for (RpIndex = 0; RpIndex < MaxPcieRootPorts; RpIndex++) {
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.AspmOptionalityCompliance                                           = FspsUpd->FspsConfig.PcieRpTestAspmOc[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.OverrideEqualizationDefaults                                        = FspsUpd->FspsConfig.PcieEqOverrideDefault[RpIndex];
    //
    // PCIe Gen3 EQ settings
    //
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMethod                     = FspsUpd->FspsConfig.PcieGen3EqMethod[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMode                       = FspsUpd->FspsConfig.PcieGen3EqMode[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.LocalTxOverrideEn                    = FspsUpd->FspsConfig.PcieGen3EqLocalTxOverrideEn[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff                 = FspsUpd->FspsConfig.PcieGen3EqPh3NoOfPresetOrCoeff[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor     = FspsUpd->FspsConfig.PcieGen3EqPh3PreCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor    = FspsUpd->FspsConfig.PcieGen3EqPh3PostCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[0]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[1]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[2]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[3]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[4]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[5]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[6]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[7]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[8]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[9]                     = FspsUpd->FspsConfig.PcieGen3EqPh3Preset9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[10]                    = FspsUpd->FspsConfig.PcieGen3EqPh3Preset10List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DpTxPreset                        = FspsUpd->FspsConfig.PcieGen3EqPh1DpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpTxPreset                        = FspsUpd->FspsConfig.PcieGen3EqPh1UpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph2LocalTxOverridePreset             = FspsUpd->FspsConfig.PcieGen3EqPh2LocalTxOverridePreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh3Bypass                          = FspsUpd->FspsConfig.PcieRpGen3EqPh3Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh23Bypass                         = FspsUpd->FspsConfig.PcieRpGen3EqPh23Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer                            = FspsUpd->FspsConfig.PcieGen3PcetTimer[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.TsLockTimer                          = FspsUpd->FspsConfig.PcieGen3TsLockTimer[RpIndex];
    //
    // PCIe Gen4 EQ settings
    //
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMethod                     = FspsUpd->FspsConfig.PcieGen4EqMethod[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMode                       = FspsUpd->FspsConfig.PcieGen4EqMode[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.LocalTxOverrideEn                    = FspsUpd->FspsConfig.PcieGen4EqLocalTxOverrideEn[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff                 = FspsUpd->FspsConfig.PcieGen4EqPh3NoOfPresetOrCoeff[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor     = FspsUpd->FspsConfig.PcieGen4EqPh3PreCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor    = FspsUpd->FspsConfig.PcieGen4EqPh3PostCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[0]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[1]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[2]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[3]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[4]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[5]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[6]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[7]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[8]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[9]                     = FspsUpd->FspsConfig.PcieGen4EqPh3Preset9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[10]                    = FspsUpd->FspsConfig.PcieGen4EqPh3Preset10List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DpTxPreset                        = FspsUpd->FspsConfig.PcieGen4EqPh1DpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpTxPreset                        = FspsUpd->FspsConfig.PcieGen4EqPh1UpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph2LocalTxOverridePreset             = FspsUpd->FspsConfig.PcieGen4EqPh2LocalTxOverridePreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh3Bypass                          = FspsUpd->FspsConfig.PcieRpGen4EqPh3Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh23Bypass                         = FspsUpd->FspsConfig.PcieRpGen4EqPh23Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer                            = FspsUpd->FspsConfig.PcieGen4PcetTimer[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.TsLockTimer                          = FspsUpd->FspsConfig.PcieGen4TsLockTimer[RpIndex];

    //
    // PCIe Gen5 EQ settings
    //
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMethod                     = FspsUpd->FspsConfig.PcieGen5EqMethod[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMode                       = FspsUpd->FspsConfig.PcieGen5EqMode[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.LocalTxOverrideEn                    = FspsUpd->FspsConfig.PcieGen5EqLocalTxOverrideEn[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff                 = FspsUpd->FspsConfig.PcieGen5EqPh3NoOfPresetOrCoeff[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor     = FspsUpd->FspsConfig.PcieGen5EqPh3PreCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor    = FspsUpd->FspsConfig.PcieGen5EqPh3PostCursor9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[0]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset0List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[1]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset1List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[2]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset2List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[3]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset3List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[4]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset4List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[5]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset5List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[6]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset6List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[7]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset7List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[8]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset8List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[9]                     = FspsUpd->FspsConfig.PcieGen5EqPh3Preset9List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[10]                    = FspsUpd->FspsConfig.PcieGen5EqPh3Preset10List[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DpTxPreset                        = FspsUpd->FspsConfig.PcieGen5EqPh1DpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpTxPreset                        = FspsUpd->FspsConfig.PcieGen5EqPh1UpTxPreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph2LocalTxOverridePreset             = FspsUpd->FspsConfig.PcieGen5EqPh2LocalTxOverridePreset[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh3Bypass                          = FspsUpd->FspsConfig.PcieRpGen5EqPh3Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh23Bypass                         = FspsUpd->FspsConfig.PcieRpGen5EqPh23Bypass[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer                            = FspsUpd->FspsConfig.PcieGen5PcetTimer[RpIndex];
    PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.TsLockTimer                          = FspsUpd->FspsConfig.PcieGen5TsLockTimer[RpIndex];
  }

}

/**
  Update Serial IO policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateSerialIoPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                 Index;
  SERIAL_IO_CONFIG      *SerialIoConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < 6; Index++) {
    SerialIoConfig->I2cDeviceConfig[Index].Mode           = FspsUpd->FspsConfig.SerialIoI2cMode[Index];
    SerialIoConfig->I2cDeviceConfig[Index].PinMux.Sda     = FspsUpd->FspsConfig.PchSerialIoI2cSdaPinMux[Index];
    SerialIoConfig->I2cDeviceConfig[Index].PinMux.Scl     = FspsUpd->FspsConfig.PchSerialIoI2cSclPinMux[Index];
    SerialIoConfig->I2cDeviceConfig[Index].PadTermination = FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[Index];
  }
}

/**
  Update LPSS SPI policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateLpssSpiPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                 Index;
  UINTN                 CsIndex;
  LPSS_SPI_CONFIG       *LpssSpiConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssSpiConfigGuid, (VOID *) &LpssSpiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
   for (Index = 0; Index < PCH_MAX_SERIALIO_SPI_CONTROLLERS; Index++) {
    LpssSpiConfig->SpiDeviceConfig[Index].Mode            = FspsUpd->FspsConfig.SerialIoLpssSpiMode[Index];
    LpssSpiConfig->SpiDeviceConfig[Index].CsMode          = FspsUpd->FspsConfig.SerialIoLpssSpiCsMode[Index];
    LpssSpiConfig->SpiDeviceConfig[Index].CsState         = FspsUpd->FspsConfig.SerialIoLpssSpiCsState[Index];
    LpssSpiConfig->SpiDeviceConfig[Index].PinMux.Clk      = FspsUpd->FspsConfig.SerialIoLpssSpiClkPinMux[Index];
    LpssSpiConfig->SpiDeviceConfig[Index].PinMux.Miso     = FspsUpd->FspsConfig.SerialIoLpssSpiMisoPinMux[Index];
    LpssSpiConfig->SpiDeviceConfig[Index].PinMux.Mosi     = FspsUpd->FspsConfig.SerialIoLpssSpiMosiPinMux[Index];
    for (CsIndex = 0; CsIndex < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; CsIndex++) {
      LpssSpiConfig->SpiDeviceConfig[Index].CsEnable[CsIndex]   = FspsUpd->FspsConfig.SerialIoLpssSpiCsEnable[Index * PCH_MAX_SERIALIO_SPI_CHIP_SELECTS + CsIndex];
      LpssSpiConfig->SpiDeviceConfig[Index].CsPolarity[CsIndex] = FspsUpd->FspsConfig.SerialIoLpssSpiCsPolarity[Index * PCH_MAX_SERIALIO_SPI_CHIP_SELECTS + CsIndex];
      LpssSpiConfig->SpiDeviceConfig[Index].PinMux.Cs[CsIndex]  = FspsUpd->FspsConfig.SerialIoLpssSpiCsPinMux[Index * PCH_MAX_SERIALIO_SPI_CHIP_SELECTS + CsIndex];
    }
  }
}

/**
  Update LPSS I3C policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateLpssI3cPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                 Index;
  LPSS_I3C_CONFIG       *I3cConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI3cConfigGuid, (VOID *) &I3cConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < 3; Index++) {
    I3cConfig->I3cDeviceConfig[Index].Mode                  = FspsUpd->FspsConfig.SerialIoI3cMode[Index];
    I3cConfig->I3cDeviceConfig[Index].Sda.PinMux            = FspsUpd->FspsConfig.SerialIoI3cSdaPinMux[Index];
    I3cConfig->I3cDeviceConfig[Index].Sda.PadTermination    = FspsUpd->FspsConfig.SerialIoI3cSdaPadTermination[Index];
    I3cConfig->I3cDeviceConfig[Index].Scl.PinMux            = FspsUpd->FspsConfig.SerialIoI3cSclPinMux[Index];
    I3cConfig->I3cDeviceConfig[Index].Scl.PadTermination    = FspsUpd->FspsConfig.SerialIoI3cSclPadTermination[Index];
    I3cConfig->I3cDeviceConfig[Index].SclFb.PinMux          = FspsUpd->FspsConfig.SerialIoI3cSclFbPinMux[Index];
    I3cConfig->I3cDeviceConfig[Index].SclFb.PadTermination  = FspsUpd->FspsConfig.SerialIoI3cSclFbPadTermination[Index];
  }
}

/**
  Update LPSS UART policies.
  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateLpssUartConfig (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                 Index;
  LPSS_UART_CONFIG      *LpssUartConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssUartConfigGuid, (VOID *) &LpssUartConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    LpssUartConfig->UartDeviceConfig[Index].Mode = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartMode[Index];
    LpssUartConfig->UartDeviceConfig[Index].Attributes.AutoFlow = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartAutoFlow[Index];
    LpssUartConfig->UartDeviceConfig[Index].PowerGating = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartPowerGating[Index];
    LpssUartConfig->UartDeviceConfig[Index].DmaEnable = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartDmaEnable[Index];
  }
  LpssUartConfig->UartDeviceConfig[1].PinMux.Rx = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartRxPinMuxPolicy[1];
  LpssUartConfig->UartDeviceConfig[1].PinMux.Tx = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartTxPinMuxPolicy[1];
  if (PtlIsPcdH () || PtlIsPcdP ()) {
    LpssUartConfig->UartDeviceConfig[1].PinMux.Rx = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartRxPinMuxPolicy[1];
    LpssUartConfig->UartDeviceConfig[1].PinMux.Tx = ((FSPS_UPD *) FspsUpd)->FspsConfig.SerialIoUartTxPinMuxPolicy[1];
  }
}

/**
  Update LPSS I2C policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateLpssI2cPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                 Index;
  LPSS_I2C_CONFIG       *I2cConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI2cConfigGuid, (VOID *) &I2cConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
 for (Index = 0; Index < 6; Index++) {
    I2cConfig->I2cDeviceConfig[Index].Mode                  = FspsUpd->FspsConfig.SerialIoI2cMode[Index];
    I2cConfig->I2cDeviceConfig[Index].PinMux.Sda            = FspsUpd->FspsConfig.PchSerialIoI2cSdaPinMux[Index];
    I2cConfig->I2cDeviceConfig[Index].PinMux.Scl            = FspsUpd->FspsConfig.PchSerialIoI2cSclPinMux[Index];
    I2cConfig->I2cDeviceConfig[Index].PadTermination        = FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[Index];
  }
}

/**
  Update Interrupt policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateInterruptPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINT8                        NumOfDevIntConfig;
  PCH_DEVICE_INTERRUPT_CONFIG  *DevIntConfg;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (FspsUpd->FspsConfig.NumOfDevIntConfig != 0) {
    DevIntConfg = (PCH_DEVICE_INTERRUPT_CONFIG *)(UINTN) FspsUpd->FspsConfig.DevIntConfigPtr;
    NumOfDevIntConfig = FspsUpd->FspsConfig.NumOfDevIntConfig;
    ASSERT (NumOfDevIntConfig <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);

    InterruptConfig->NumOfDevIntConfig = NumOfDevIntConfig;
    ZeroMem (
      InterruptConfig->DevIntConfig,
      PCH_MAX_DEVICE_INTERRUPT_CONFIG * sizeof (PCH_DEVICE_INTERRUPT_CONFIG)
      );
    CopyMem (
      InterruptConfig->DevIntConfig,
      DevIntConfg,
      NumOfDevIntConfig * sizeof (PCH_DEVICE_INTERRUPT_CONFIG)
      );
  }
  InterruptConfig->GpioIrqRoute = FspsUpd->FspsConfig.GpioIrqRoute;
  InterruptConfig->SciIrqSelect = FspsUpd->FspsConfig.SciIrqSelect;
  InterruptConfig->TcoIrqSelect = FspsUpd->FspsConfig.TcoIrqSelect;
  InterruptConfig->TcoIrqEnable = FspsUpd->FspsConfig.TcoIrqEnable;
}

/**
  Update LockDown policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateLockDownPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_LOCK_DOWN_CONFIG   *LockDownConfig;
  EFI_STATUS             Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  LockDownConfig->GlobalSmi       = FspsUpd->FspsConfig.PchLockDownGlobalSmi;
  LockDownConfig->BiosInterface   = FspsUpd->FspsConfig.PchLockDownBiosInterface;
  LockDownConfig->BiosLock        = FspsUpd->FspsConfig.PchLockDownBiosLock;

  LockDownConfig->UnlockGpioPads  = FspsUpd->FspsConfig.PchUnlockGpioPads;
}

/**
  Update Rtc policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateRtcPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  RTC_CONFIG        *RtcConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gRtcConfigGuid, (VOID *) &RtcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  RtcConfig->BiosInterfaceLock    = FspsUpd->FspsConfig.RtcBiosInterfaceLock;
  RtcConfig->MemoryLock           = FspsUpd->FspsConfig.RtcMemoryLock;
}

/**
  Update Thc policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateThcPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  THC_CONFIG    *ThcConfig;
  EFI_STATUS    Status;
  UINT8         ThcIndex;

  ThcIndex = 0;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThcConfigGuid, (VOID *) &ThcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (ThcIndex = 0; ThcIndex < PCH_MAX_THC_CONTROLLERS; ThcIndex++) {
    ThcConfig->ThcPort[ThcIndex].Assignment              = FspsUpd->FspsConfig.ThcAssignment[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].InterruptPinMuxing      = FspsUpd->FspsConfig.ThcInterruptPinMuxing[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].WakeOnTouch             = FspsUpd->FspsConfig.ThcWakeOnTouch[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].WotPinConfig.ActiveLevel = FspsUpd->FspsConfig.ThcWotActiveLevel[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].Mode                    = FspsUpd->FspsConfig.ThcMode[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].WotPinConfig.EdgeLevel  = FspsUpd->FspsConfig.ThcWotEdgeLevel[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].WotPinConfig.PinConfig  = FspsUpd->FspsConfig.ThcWotPinConfig[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].ThcSubSystemId.CustomizedSsid   = FspsUpd->FspsConfig.ThcCustomizedSsid[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].ThcSubSystemId.CustomizedSvid   = FspsUpd->FspsConfig.ThcCustomizedSvid[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].ActiveLtr               = FspsUpd->FspsConfig.ThcActiveLtr[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].IdleLtr                 = FspsUpd->FspsConfig.ThcIdleLtr[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].PerformanceLimitation   = FspsUpd->FspsConfig.ThcPerformanceLimitation[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].TimestampTimerMode      = FspsUpd->FspsConfig.TimestampTimerMode[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].DisplayFrameSyncPeriod  = FspsUpd->FspsConfig.DisplayFrameSyncPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].Reset.ResetPad                       = FspsUpd->FspsConfig.ThcResetPad[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].Reset.ResetPadTrigger                = FspsUpd->FspsConfig.ThcResetPadTrigger[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].Reset.ResetSequencingDelay           = FspsUpd->FspsConfig.ThcResetSequencingDelay[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].ThcDsyncPadEnable                    = FspsUpd->FspsConfig.ThcDsyncPad[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.Flags                     = FspsUpd->FspsConfig.ThcHidSpiFlags[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.Frequency                 = FspsUpd->FspsConfig.ThcHidSpiConnectionSpeed[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.InputReportBodyAddress    = FspsUpd->FspsConfig.ThcHidSpiInputReportBodyAddress[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.InputReportHeaderAddress  = FspsUpd->FspsConfig.ThcHidSpiInputReportHeaderAddress[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.OutputReportAddress       = FspsUpd->FspsConfig.ThcHidSpiOutputReportAddress[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.ReadOpcode                = FspsUpd->FspsConfig.ThcHidSpiReadOpcode[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.WriteOpcode               = FspsUpd->FspsConfig.ThcHidSpiWriteOpcode[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverSpi.LimitPacketSize           = FspsUpd->FspsConfig.ThcHidSpiLimitPacketSize[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.DeviceAddress                                     = FspsUpd->FspsConfig.ThcHidI2cDeviceAddress[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.ConnectionSpeed                                   = FspsUpd->FspsConfig.ThcHidI2cConnectionSpeed[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.AddressingMode                                    = FspsUpd->FspsConfig.ThcHidI2cAddressingMode[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.DeviceDescriptorAddress                           = FspsUpd->FspsConfig.ThcHidI2cDeviceDescriptorAddress[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialClockLineHighPeriod             = FspsUpd->FspsConfig.ThcHidI2cStandardModeSerialClockLineHighPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialClockLineLowPeriod              = FspsUpd->FspsConfig.ThcHidI2cStandardModeSerialClockLineLowPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialDataLineTransmitHoldPeriod      = FspsUpd->FspsConfig.ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.StandardModeSerialDataLineReceiveHoldPeriod       = FspsUpd->FspsConfig.ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialClockLineHighPeriod                 = FspsUpd->FspsConfig.ThcHidI2cFastModeSerialClockLineHighPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialClockLineLowPeriod                  = FspsUpd->FspsConfig.ThcHidI2cFastModeSerialClockLineLowPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialDataLineTransmitHoldPeriod          = FspsUpd->FspsConfig.ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModeSerialDataLineReceiveHoldPeriod           = FspsUpd->FspsConfig.ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.MaxSuppressedSpikesSMFMFMP                        = FspsUpd->FspsConfig.ThcHidI2cMaxSuppressedSpikesSMFMFMP[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialClockLineHighPeriod             = FspsUpd->FspsConfig.ThcHidI2cFastModePlusSerialClockLineHighPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialClockLineLowPeriod              = FspsUpd->FspsConfig.ThcHidI2cFastModePlusSerialClockLineLowPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialDataLineTransmitHoldPeriod      = FspsUpd->FspsConfig.ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.FastModePlusSerialDataLineReceiveHoldPeriod       = FspsUpd->FspsConfig.ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialClockLineHighPeriod        = FspsUpd->FspsConfig.ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialClockLineLowPeriod         = FspsUpd->FspsConfig.ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialDataLineTransmitHoldPeriod = FspsUpd->FspsConfig.ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.HighSpeedModePlusSerialDataLineReceiveHoldPeriod  = FspsUpd->FspsConfig.ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[ThcIndex];
    ThcConfig->ThcPort[ThcIndex].HidOverI2c.MaximumLengthOfSuppressedSpikesInHighSpeedMode    = FspsUpd->FspsConfig.ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[ThcIndex];
  }
}

/**
  Update IEH policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateIehPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  IEH_CONFIG        *IehConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  IehConfig->Mode = FspsUpd->FspsConfig.IehMode;
}


/**
  Update FlashProtection policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateFlashProtectionPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  UINTN                        Index;
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; Index ++) {
    FlashProtectionConfig->ProtectRange[Index].WriteProtectionEnable = FspsUpd->FspsConfig.PchWriteProtectionEnable[Index];
    FlashProtectionConfig->ProtectRange[Index].ReadProtectionEnable  = FspsUpd->FspsConfig.PchReadProtectionEnable[Index];
    FlashProtectionConfig->ProtectRange[Index].ProtectedRangeLimit   = FspsUpd->FspsConfig.PchProtectedRangeLimit[Index];
    FlashProtectionConfig->ProtectRange[Index].ProtectedRangeBase    = FspsUpd->FspsConfig.PchProtectedRangeBase[Index];
  }
}

/**
  Update IO APIC policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateIoApicPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_IOAPIC_CONFIG *IoApicConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  IoApicConfig->IoApicEntry24_119          = FspsUpd->FspsConfig.PchIoApicEntry24_119;
  IoApicConfig->Enable8254ClockGating      = FspsUpd->FspsConfig.Enable8254ClockGating;
  IoApicConfig->Enable8254ClockGatingOnS3  = FspsUpd->FspsConfig.Enable8254ClockGatingOnS3;
  IoApicConfig->IoApicId                   = FspsUpd->FspsConfig.PchIoApicId;
}

/**
  Update P2sb policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateP2sbPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_P2SB_CONFIG   *P2sbConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  P2sbConfig->SbAccessUnlock  = FspsUpd->FspsConfig.PchSbAccessUnlock;

}

/**
  Update PCH General policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdatePchGeneralPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_GENERAL_CONFIG *PchGeneralConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchGeneralConfig->Crid               = FspsUpd->FspsConfig.PchCrid;
  PchGeneralConfig->LegacyIoLowLatency = FspsUpd->FspsConfig.PchLegacyIoLowLatency;
}

/**
  Update Power Management policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdatePmPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_PM_CONFIG     *PmConfig;
  ADR_CONFIG        *PmcAdrConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PmConfig->WakeConfig.PmeB0S5Dis         = FspsUpd->FspsConfig.PchPmPmeB0S5Dis;
  PmConfig->WakeConfig.WolEnableOverride  = FspsUpd->FspsConfig.PchPmWolEnableOverride;
  PmConfig->WakeConfig.WoWlanEnable       = FspsUpd->FspsConfig.PchPmWoWlanEnable;

  PmConfig->PchSlpS3MinAssert             = FspsUpd->FspsConfig.PchPmSlpS3MinAssert;
  PmConfig->PchSlpS4MinAssert             = FspsUpd->FspsConfig.PchPmSlpS4MinAssert;
  PmConfig->PchSlpSusMinAssert            = FspsUpd->FspsConfig.PchPmSlpSusMinAssert;
  PmConfig->PchSlpAMinAssert              = FspsUpd->FspsConfig.PchPmSlpAMinAssert;
  PmConfig->SlpStrchSusUp                 = FspsUpd->FspsConfig.PchPmSlpStrchSusUp;
  PmConfig->SlpLanLowDc                   = FspsUpd->FspsConfig.PchPmSlpLanLowDc;
  PmConfig->PwrBtnOverridePeriod          = FspsUpd->FspsConfig.PchPmPwrBtnOverridePeriod;
  PmConfig->DisableEnergyReport           = FspsUpd->FspsConfig.PchPmDisableEnergyReport;
  PmConfig->DisableNativePowerButton      = FspsUpd->FspsConfig.PchPmDisableNativePowerButton;
  PmConfig->PowerButtonDebounce           = FspsUpd->FspsConfig.PmcPowerButtonDebounce;
  PmConfig->MeWakeSts                     = FspsUpd->FspsConfig.PchPmMeWakeSts;
  PmConfig->WolOvrWkSts                   = FspsUpd->FspsConfig.PchPmWolOvrWkSts;
  PmConfig->EnableTcoTimer                = FspsUpd->FspsConfig.EnableTcoTimer;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  PmConfig->EnableTimedGpio0              = FspsUpd->FspsConfig.EnableTimedGpio0;
  PmConfig->EnableTimedGpio1              = FspsUpd->FspsConfig.EnableTimedGpio1;
#endif
  PmConfig->VrAlert                       = FspsUpd->FspsConfig.PchPmVrAlert;
  PmConfig->PchPwrCycDur                  = FspsUpd->FspsConfig.PchPmPwrCycDur;
  PmConfig->PciePllSsc                    = FspsUpd->FspsConfig.PchPmPciePllSsc;
  PmConfig->PsOnEnable                    = FspsUpd->FspsConfig.PsOnEnable;
  PmConfig->CpuC10GatePinEnable           = FspsUpd->FspsConfig.PmcCpuC10GatePinEnable;
  PmConfig->ModPhySusPgEnable             = FspsUpd->FspsConfig.PmcModPhySusPgEnable;
  PmConfig->Usb2PhySusPgEnable            = FspsUpd->FspsConfig.PmcUsb2PhySusPgEnable;
  PmConfig->OsIdleEnable                  = FspsUpd->FspsConfig.PmcOsIdleEnable;
  PmConfig->LpmS0ixSubStateEnable.Val     = FspsUpd->FspsConfig.PmcLpmS0ixSubStateEnableMask;
  PmConfig->V1p05PhyExtFetControlEn       = FspsUpd->FspsConfig.PmcV1p05PhyExtFetControlEn;
  PmConfig->V1p05IsExtFetControlEn        = FspsUpd->FspsConfig.PmcV1p05IsExtFetControlEn;
  PmConfig->S0ixAutoDemotion              = FspsUpd->FspsConfig.PchS0ixAutoDemotion;
  PmConfig->LatchEventsC10Exit            = FspsUpd->FspsConfig.PchPmLatchEventsC10Exit;
  PmConfig->C10DynamicThresholdAdjustment = FspsUpd->FspsConfig.PmcC10DynamicThresholdAdjustment;
  PmConfig->PmErDebugMode                 = FspsUpd->FspsConfig.PchPmErDebugMode;
  PmConfig->PmcWdtTimerEn                 = FspsUpd->FspsConfig.PmcWdtTimerEn;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gAdrConfigGuid, (VOID *) &PmcAdrConfig);
  // ASSERT_EFI_ERROR (Status);
  // if (EFI_ERROR (Status)) {
  //   return;
  // }

  PmcAdrConfig->AdrEn                    = FspsUpd->FspsConfig.PmcAdrEn;
  PmcAdrConfig->AdrTimerEn               = FspsUpd->FspsConfig.PmcAdrTimerEn;
  PmcAdrConfig->AdrTimer1Val             = FspsUpd->FspsConfig.PmcAdrTimer1Val;
  PmcAdrConfig->AdrMultiplier1Val        = FspsUpd->FspsConfig.PmcAdrMultiplier1Val;
  PmcAdrConfig->AdrHostPartitionReset    = FspsUpd->FspsConfig.PmcAdrHostPartitionReset;

}
/**
  Update Thermal policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateThermalPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  THERMAL_CONFIG    *ThermalConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  ThermalConfig->PchHotEnable = FspsUpd->FspsConfig.PchHotEnable;

  ThermalConfig->TTLevels.T0Level            = FspsUpd->FspsConfig.PchT0Level;
  ThermalConfig->TTLevels.T1Level            = FspsUpd->FspsConfig.PchT1Level;
  ThermalConfig->TTLevels.T2Level            = FspsUpd->FspsConfig.PchT2Level;
  ThermalConfig->TTLevels.TTEnable           = FspsUpd->FspsConfig.PchTTEnable;
  ThermalConfig->TTLevels.TTState13Enable    = FspsUpd->FspsConfig.PchTTState13Enable;
  ThermalConfig->TTLevels.TTLock             = FspsUpd->FspsConfig.PchTTLock;
  ThermalConfig->TTLevels.SuggestedSetting   = FspsUpd->FspsConfig.TTSuggestedSetting;
  ThermalConfig->PchHotLevel = FspsUpd->FspsConfig.PchTemperatureHotLevel;
}

/**
  Update Fusa policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateFusaPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
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

  FusaConfig->PsfFusaConfigEnable = FspsUpd->FspsConfig.PsfFusaConfigEnable;
#endif
}

/**
  Update Fivr policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateFivrPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_FIVR_CONFIG   *FivrConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFivrConfigGuid, (VOID *) &FivrConfig);
  // ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  FivrConfig->ExtV1p05Rail.EnabledStates           = FspsUpd->FspsConfig.PchFivrExtV1p05RailEnabledStates;
  FivrConfig->ExtV1p05Rail.SupportedVoltageStates  = FspsUpd->FspsConfig.PchFivrExtV1p05RailSupportedVoltageStates;
  FivrConfig->ExtV1p05Rail.Voltage                 = FspsUpd->FspsConfig.PchFivrExtV1p05RailVoltage;
  if (FspsUpd->FspsConfig.PchFivrExtV1p05RailIccMaximum == 0 &&
      FspsUpd->FspsConfig.PchFivrExtV1p05RailIccMax != 0) {
    FivrConfig->ExtV1p05Rail.IccMaximum = FspsUpd->FspsConfig.PchFivrExtV1p05RailIccMax;
  } else {
    FivrConfig->ExtV1p05Rail.IccMaximum = FspsUpd->FspsConfig.PchFivrExtV1p05RailIccMaximum;
  }

  FivrConfig->ExtV1p05Rail.CtrlRampTmr   = FspsUpd->FspsConfig.PchFivrExtV1p05RailCtrlRampTmr;
  FivrConfig->ExtVnnRail.EnabledStates          = FspsUpd->FspsConfig.PchFivrExtVnnRailEnabledStates;
  FivrConfig->ExtVnnRail.SupportedVoltageStates = FspsUpd->FspsConfig.PchFivrExtVnnRailSupportedVoltageStates;
  FivrConfig->ExtVnnRail.Voltage                = FspsUpd->FspsConfig.PchFivrExtVnnRailVoltage;
  if (FspsUpd->FspsConfig.PchFivrExtVnnRailIccMaximum == 0 &&
      FspsUpd->FspsConfig.PchFivrExtVnnRailIccMax != 0) {
    FivrConfig->ExtVnnRail.IccMaximum = FspsUpd->FspsConfig.PchFivrExtVnnRailIccMax;
  } else {
    FivrConfig->ExtVnnRail.IccMaximum = FspsUpd->FspsConfig.PchFivrExtVnnRailIccMaximum;
  }

  FivrConfig->ExtVnnRail.CtrlRampTmr     = FspsUpd->FspsConfig.PchFivrExtVnnRailCtrlRampTmr;
  FivrConfig->ExtVnnRailSx.EnabledStates = FspsUpd->FspsConfig.PchFivrExtVnnRailSxEnabledStates;
  FivrConfig->ExtVnnRailSx.Voltage       = FspsUpd->FspsConfig.PchFivrExtVnnRailSxVoltage;
  if (FspsUpd->FspsConfig.PchFivrExtVnnRailSxIccMaximum == 0 &&
      FspsUpd->FspsConfig.PchFivrExtVnnRailSxIccMax != 0) {
    FivrConfig->ExtVnnRailSx.IccMaximum = FspsUpd->FspsConfig.PchFivrExtVnnRailSxIccMax;
  } else {
    FivrConfig->ExtVnnRailSx.IccMaximum = FspsUpd->FspsConfig.PchFivrExtVnnRailSxIccMaximum;
  }

  FivrConfig->VccinAux.LowToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxLowToHighCurModeVolTranTime;
  FivrConfig->VccinAux.RetToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxRetToHighCurModeVolTranTime;
  FivrConfig->VccinAux.RetToLowCurModeVolTranTime  = FspsUpd->FspsConfig.PchFivrVccinAuxRetToLowCurModeVolTranTime;
  FivrConfig->VccinAux.OffToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxOffToHighCurModeVolTranTime;
  FivrConfig->FivrDynPm                            = FspsUpd->FspsConfig.PchFivrDynPm;
}

/**
  This function performs PCH PEI Policy update.

  @param[in, out] SiPolicy       The SI Policy PPI instance
  @param[in]      FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicy (
  IN OUT SI_POLICY_PPI     *SiPolicy,
  IN     FSPS_UPD          *FspsUpd
  )
{
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));

  FspUpdateGbePolicy (SiPolicy, FspsUpd);
#if FixedPcdGet8 (PcdTsnSupport) == 0x1
  FspUpdateTsnPolicy (SiPolicy, FspsUpd);
#endif
  FspUpdateHdAudioPolicy (SiPolicy, FspsUpd);
  FspUpdateCnviPolicy (SiPolicy, FspsUpd);
  FspUpdateHsioPolicy (SiPolicy, FspsUpd);
  FspUpdateEspiPolicy (SiPolicy, FspsUpd);
  FspUpdateUfsPolicy (SiPolicy, FspsUpd);
  FspUpdateIshPolicy (SiPolicy, FspsUpd);
  FspUpdateUsbPolicy (SiPolicy, FspsUpd);
  FspUpdateUsb2PhyPolicy (SiPolicy, FspsUpd);
  FspUpdateUsb3HsioPolicy (SiPolicy, FspsUpd);
  FspUpdatePcieRpPolicy (SiPolicy, FspsUpd);
  FspUpdateLpssI3cPolicy (SiPolicy, FspsUpd);
  FspUpdateLpssUartConfig (SiPolicy, FspsUpd);
  FspUpdateLpssSpiPolicy (SiPolicy, FspsUpd);
  FspUpdateLpssI2cPolicy (SiPolicy, FspsUpd);
  FspUpdateSerialIoPolicy (SiPolicy, FspsUpd);

  FspUpdateInterruptPolicy (SiPolicy, FspsUpd);
  FspUpdateLockDownPolicy (SiPolicy, FspsUpd);
  FspUpdateRtcPolicy (SiPolicy, FspsUpd);
  FspUpdateThcPolicy (SiPolicy, FspsUpd);
  FspUpdateIehPolicy (SiPolicy, FspsUpd);
  FspUpdateFlashProtectionPolicy (SiPolicy, FspsUpd);
  FspUpdateIoApicPolicy (SiPolicy, FspsUpd);
  FspUpdateP2sbPolicy (SiPolicy, FspsUpd);
  FspUpdatePchGeneralPolicy (SiPolicy, FspsUpd);
  FspUpdatePmPolicy (SiPolicy, FspsUpd);
  FspUpdateThermalPolicy (SiPolicy, FspsUpd);
  FspUpdateFusaPolicy (SiPolicy, FspsUpd);
  FspUpdateFivrPolicy (SiPolicy, FspsUpd);

  return EFI_SUCCESS;
}

/**
  This function performs PCH PEI Policy update.

  @param[in, out] PchPolicy      The PCH Policy PPI instance
  @param[in]      FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAttachedPchPolicy (
  IN OUT SI_POLICY_PPI     *PchPolicy,
  IN     FSPS_UPD          *FspsUpd
  )
{
  return EFI_SUCCESS;
}
