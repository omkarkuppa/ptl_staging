/** @file
  Initializes/updates PCH related HOBs in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include "PtlPcdInitPei.h"
#include <Library/GbeLib.h>
#include <Library/PeiPmcPrivateLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PchPciBdfLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/P2sbRegs.h>
#include <Ppi/PchPolicy.h>
#include <HdAudioConfig.h>
#include <LockDownConfig.h>
#include <GbeConfig.h>
#include <GbeConfigHob.h>
#include <LpssSpiConfigHob.h>
#include <EspiConfig.h>
#include <EspiConfigHob.h>
#include <UsbConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <ItssConfigHob.h>
#include <PmcConfigHob.h>
#include <SpiConfigHob.h>
#include <SmbusConfigHob.h>
#include <LpssUartConfigHob.h>
#include <PchConfigHobCommon.h>
#include <ThcConfigHob.h>
#include <LpssI3cConfigHob.h>
#include <HdaConfigHob.h>
#include <PchPcieRpConfigHob.h>
#include <LpssI2cConfigHob.h>
#include <SerialIoConfig.h>

/**
  The function update pch info hob in the end of PchInit.
**/
VOID
BuildPchInfoHob (
  VOID
  )
{
  EFI_STATUS  Status;
  PCH_INFO_HOB *PchInfoHob;
  BOOLEAN     CridSupport;
  UINT8       OrgRid;
  UINT8       NewRid;
  UINT8       Index;
  UINT8                       LpssUartDbg2ControllerIndex;
  SI_POLICY_PPI               *SiPolicy;
  SERIAL_IO_CONFIG            *SerialIoConfig;

  PchInfoHob = BuildGuidHob (&gPchInfoHobGuid, sizeof (PCH_INFO_HOB));
  ASSERT (PchInfoHob != 0);
  if (PchInfoHob == NULL) {
    return;
  }

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || (SiPolicy == NULL)) {
    return ;
  }
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);

  PchInfoHob->Revision = PCH_INFO_HOB_REVISION;

  BuildPcieInfoHob (PchInfoHob);

  PchInfoHob->Thc0Strap = PmcIsThcEnabled (0);

  PchInfoHob->HpetBusNum    = V_P2SB_CFG_HBDF_BUS;
  PchInfoHob->HpetDevNum    = V_P2SB_CFG_HBDF_DEV;
  PchInfoHob->HpetFuncNum   = V_P2SB_CFG_HBDF_FUNC;
  PchInfoHob->IoApicBusNum  = V_P2SB_CFG_IBDF_BUS;
  PchInfoHob->IoApicDevNum  = V_P2SB_CFG_IBDF_DEV;
  PchInfoHob->IoApicFuncNum = V_P2SB_CFG_IBDF_FUNC;

  //
  // This must be done before PMC hidden.
  //
  CridSupport = FALSE;
  OrgRid      = 0;
  NewRid      = 0;
  Status = PmcDetectCrid0 (PmcPciCfgBase (), PmcGetPwrmBase (), &CridSupport, &OrgRid, &NewRid);
  DEBUG ((DEBUG_INFO, "CridSupport = %x %x %x\n", CridSupport, OrgRid, NewRid));
  if (!EFI_ERROR (Status)) {
    PchInfoHob->CridSupport = CridSupport;
    PchInfoHob->CridOrgRid  = OrgRid;
    PchInfoHob->CridNewRid  = NewRid;
  }

  PchInfoHob->IshAvailable = PchIsIshSupported () && PmcIsIshSupported ();
  PchInfoHob->ThcSupport = IsPchThcSupported ();
  PchInfoHob->GbeSupport = IsPchGbeSupported ();
  PchInfoHob->TsnSupport = IsPchTsnSupported ();
  PchInfoHob->PSOnSupport = IsPchPSOnSupported ();
  PchInfoHob->XhciSupport = IsPchXhciSupported ();
  PchInfoHob->HdaLinkSupport = IsAudioInterfaceSupported (HdaLink, 0);
  for (Index = 0; Index < GetPchHdaMaxDmicLinkNum (); Index++) {
    PchInfoHob->DmicLinkSupport[Index] = IsAudioInterfaceSupported (HdaDmic, Index);
  }
  for (Index = 0; Index < GetPchHdaMaxSspLinkNum (); Index++) {
    PchInfoHob->SspLinkSupport[Index] = IsAudioInterfaceSupported (HdaSsp, Index);
  }
  for (Index = 0; Index < GetPchHdaMaxSndwLinkNum (); Index++) {
    PchInfoHob->SndwLinkSupport[Index] = IsAudioInterfaceSupported (HdaSndw, Index);
  }

  for (Index = 0; Index < GetPchHdaMaxSndwMultilaneNum (); Index++) {
    PchInfoHob->SndwMultilaneSupport[Index] = IsAudioInterfaceSupported (HdaSndwMultilane, Index);
  }

  LpssUartDbg2ControllerIndex = 0;
  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    if ((SerialIoConfig->UartDeviceConfig[Index].Mode == SerialIoUartHidden) && (SerialIoConfig->UartDeviceConfig[Index].DBG2 == TRUE)) {
      LpssUartDbg2ControllerIndex = Index + 1;
      break;
    }
  }
  PchInfoHob->OsDebugPort = LpssUartDbg2ControllerIndex;
}

/**
  The function update pch config hob in the end of PchInit.

  @param[in]      SiPolicy               The SI Policy PPI instance
**/
VOID
BuildPchConfigHobs (
  IN     SI_POLICY_PPI          *SiPolicy
  )
{
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicy;
  PCH_SMBUS_PREMEM_CONFIG      *SmbusPreMemConfig;
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  PCH_PCIE_CONFIG              *PchPcieConfig;
  HDAUDIO_CONFIG               *HdAudioConfig;
  HDAUDIO_PREMEM_CONFIG        *HdAudioPreMemConfig;
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  PCH_PM_CONFIG                *PmConfig;
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  THC_CONFIG                   *ThcConfig;
  GBE_CONFIG                   *GbeConfig;
  EFI_STATUS                   Status;
  PCH_ESPI_CONFIG              *EspiConfig;
  PCH_GENERAL_PREMEM_CONFIG    *PchGeneralPreMemConfig;
  PCH_PCIE_RP_PREMEM_CONFIG    *PchPcieRpPreMemConfig;
  USB_CONFIG                   *UsbConfig;
  LPSS_I3C_CONFIG              *I3cConfig;
  LPSS_UART_CONFIG             *LpssUartConfig;
  LPSS_SPI_CONFIG              *LpssSpiConfig;
  LPSS_I2C_CONFIG              *I2cConfig;
  PCH_P2SB_CONFIG              *P2sbConfig;
  ITSS_CONFIG_HOB              *ItssConfigHob;
  LPSS_UART_CONFIG_HOB         *LpssUartConfigHob;
  PMC_CONFIG_HOB               *PmcConfigHob;
  SPI_CONFIG_HOB               *SpiConfigHob;
  LOCK_DOWN_CONFIG_HOB         *LockDownConfigHob;
  VOID                         *HobPtr;
  PCH_GENERAL_CONFIG_HOB       *GeneralConfigHob;
  LPSS_I3C_CONFIG_HOB          *LpssI3cConfigHob;
  LPSS_I2C_CONFIG_HOB          *LpssI2cConfigHob;
  THC_CONFIG_HOB               *ThcConfigHob;
  ESPI_CONFIG_HOB              *EspiConfigHob;
  GBE_CONFIG_HOB               *GbeConfigHob;
  SMBUS_CONFIG_HOB             *SmbusConfigHob;
  PCIE_RP_CONFIG_HOB           *PcieRpConfigHob;
  HDAUDIO_CONFIG_HOB           *HdaConfigHob;
  LPSS_SPI_CONFIG_HOB          *LpssSpiConfigHob;

  UsbConfig = NULL;
  GbeConfig = NULL;
  ThcConfig = NULL;
  HdAudioConfig = NULL;
  HdAudioPreMemConfig = NULL;

  //
  // Get Policy settings through the SiPreMemPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    //
    // SI_PREMEM_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdAudioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PchPcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThcConfigGuid, (VOID *) &ThcConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI3cConfigGuid, (VOID *) &I3cConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssUartConfigGuid, (VOID *) &LpssUartConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssSpiConfigGuid, (VOID *) &LpssSpiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI2cConfigGuid, (VOID *) &I2cConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);

  // SMBUS HOB
  SmbusConfigHob = BuildGuidHob (&gPchSmbusConfigHobGuid, sizeof (SMBUS_CONFIG_HOB));
  if (SmbusConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (SmbusConfigHob, sizeof (SMBUS_CONFIG_HOB));
  SmbusConfigHob->NumRsvdSmbusAddresses = SmbusPreMemConfig->NumRsvdSmbusAddresses;
  CopyMem (
    SmbusConfigHob->RsvdSmbusAddressTable,
    SmbusPreMemConfig->RsvdSmbusAddressTable,
    sizeof (SmbusConfigHob->RsvdSmbusAddressTable)
    );

  // ITSS HOB
  ItssConfigHob = BuildGuidHob (&gPchItssConfigHobGuid, sizeof (ITSS_CONFIG_HOB));
  if (ItssConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (ItssConfigHob, sizeof (ITSS_CONFIG_HOB));
  ItssConfigHob->NumOfDevIntConfig = InterruptConfig->NumOfDevIntConfig;
  ItssConfigHob->GpioIrqRoute      = InterruptConfig->GpioIrqRoute;
  CopyMem (
    ItssConfigHob->DevIntConfig,
    InterruptConfig->DevIntConfig,
    sizeof (ItssConfigHob->DevIntConfig)
    );

  // LPSS SPI HOB
  LpssSpiConfigHob = BuildGuidHob (&gPchLpssSpiConfigHobGuid, sizeof (LPSS_SPI_CONFIG_HOB));
  if (LpssSpiConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (LpssSpiConfigHob, sizeof (LPSS_SPI_CONFIG_HOB));
  CopyMem (
    LpssSpiConfigHob->SpiDeviceConfig,
    LpssSpiConfig->SpiDeviceConfig,
    sizeof (LpssSpiConfigHob->SpiDeviceConfig)
    );
    
  // LPSS UART HOB
  LpssUartConfigHob = BuildGuidHob (&gPchLpssUartConfigHobGuid, sizeof (LPSS_UART_CONFIG_HOB));
  if (LpssUartConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (LpssUartConfigHob, sizeof (LPSS_UART_CONFIG_HOB));
  CopyMem (
    LpssUartConfigHob->UartDeviceConfig,
    LpssUartConfig->UartDeviceConfig,
    sizeof (LpssUartConfigHob->UartDeviceConfig)
    );

  // LPSS I2C HOB
  LpssI2cConfigHob = BuildGuidHob (&gPchLpssI2cConfigHobGuid, sizeof (LPSS_I2C_CONFIG_HOB));
  if (LpssI2cConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (LpssI2cConfigHob, sizeof (LPSS_I2C_CONFIG_HOB));
  CopyMem (
    LpssI2cConfigHob->I2cDeviceConfig,
    I2cConfig->I2cDeviceConfig,
    sizeof (LpssI2cConfigHob->I2cDeviceConfig)
    );
  
  // LPSS I3C HOB
  LpssI3cConfigHob = BuildGuidHob (&gPchLpssI3cConfigHobGuid, sizeof (LPSS_I3C_CONFIG_HOB));
  if (LpssI3cConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (LpssI3cConfigHob, sizeof (LPSS_I3C_CONFIG_HOB));
  CopyMem (
    LpssI3cConfigHob->I3cDeviceConfig,
    I3cConfig->I3cDeviceConfig,
    sizeof (LpssI3cConfigHob->I3cDeviceConfig)
    );

  // THC HOB
  ThcConfigHob = BuildGuidHob (&gPchThcConfigHobGuid, sizeof (THC_CONFIG_HOB));
  if (ThcConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (ThcConfigHob, sizeof (THC_CONFIG_HOB));
  CopyMem (
    ThcConfigHob->ThcPort,
    ThcConfig->ThcPort,
    sizeof (ThcConfigHob->ThcPort)
  );

  // PCIE RP HOB
  PcieRpConfigHob = BuildGuidHob (&gPchPcieRpConfigHobGuid, sizeof (PCIE_RP_CONFIG_HOB));
  if (PcieRpConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (PcieRpConfigHob, sizeof (PCIE_RP_CONFIG_HOB));
  CopyMem (
    PcieRpConfigHob->RootPort,
    PchPcieConfig->RootPort,
    sizeof (PcieRpConfigHob->RootPort)
    );
  CopyMem (
    PcieRpConfigHob->PcieClock,
    PchPcieRpPreMemConfig->PcieClock,
    sizeof (PcieRpConfigHob->PcieClock)
    );
  PcieRpConfigHob->ClkreqIpcCmdSupport = IsClkreqIpcCmdSupported ();

  // HDA
  HdaConfigHob = BuildGuidHob (&gPchHdaConfigHobGuid, sizeof (HDAUDIO_CONFIG_HOB));
  if (HdaConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (HdaConfigHob, sizeof (HDAUDIO_CONFIG_HOB));
  HdaConfigHob->DspEnable             = HdAudioPreMemConfig->DspEnable;
  HdaConfigHob->DspUaaCompliance      = HdAudioPreMemConfig->DspUaaCompliance;
  HdaConfigHob->AudioLinkSndw0        = HdAudioPreMemConfig->AudioLinkSndw[0].Enable;
  HdaConfigHob->AudioLinkSndw1        = HdAudioPreMemConfig->AudioLinkSndw[1].Enable;
  HdaConfigHob->AudioLinkSndw2        = HdAudioPreMemConfig->AudioLinkSndw[2].Enable || HdAudioPreMemConfig->SndwMultilane[0].Enable;
  HdaConfigHob->AudioLinkSndw3        = HdAudioPreMemConfig->AudioLinkSndw[3].Enable || HdAudioPreMemConfig->SndwMultilane[1].Enable;
  HdaConfigHob->AudioLinkSndw4        = HdAudioPreMemConfig->AudioLinkSndw[4].Enable;
  HdaConfigHob->AudioLinkSndwLanes2   = HdAudioPreMemConfig->SndwMultilane[0].Enable;
  HdaConfigHob->AudioLinkSndwLanes3   = HdAudioPreMemConfig->SndwMultilane[1].Enable;
  HdaConfigHob->CodecSxWakeCapability = HdAudioConfig->CodecSxWakeCapability;
  HdaConfigHob->Pme                   = HdAudioConfig->Pme;

  // ESPI HOB
  EspiConfigHob = BuildGuidHob (&gPchEspiConfigHobGuid, sizeof (ESPI_CONFIG_HOB));
  if (EspiConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (EspiConfigHob, sizeof (ESPI_CONFIG_HOB));
  EspiConfigHob->BmeHostDeviceEnabled = EspiConfig->BmeHostDeviceEnabled;

  // PMC HOB
  PmcConfigHob = BuildGuidHob (&gPchPmcConfigHobGuid, sizeof (PMC_CONFIG_HOB));
  if (PmcConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (PmcConfigHob, sizeof (PMC_CONFIG_HOB));
  PmcConfigHob->PsOnEnable            = PmConfig->PsOnEnable;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  PmcConfigHob->EnableTimedGpio0      = PmConfig->EnableTimedGpio0;
  PmcConfigHob->EnableTimedGpio1      = PmConfig->EnableTimedGpio1;
#endif

  // SPI HOB
  SpiConfigHob = BuildGuidHob (&gPchSpiConfigHobGuid, sizeof (SPI_CONFIG_HOB));
  if (SpiConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (SpiConfigHob, sizeof (SPI_CONFIG_HOB));
  CopyMem (
    SpiConfigHob->ProtectRange,
    FlashProtectionConfig->ProtectRange,
    sizeof (SpiConfigHob->ProtectRange)
    );

  // GBE HOB
  GbeConfigHob = BuildGuidHob (&gPchGbeConfigHobGuid, sizeof (GBE_CONFIG_HOB));
  if (GbeConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (GbeConfigHob, sizeof (GBE_CONFIG_HOB));
  GbeConfigHob->PchWOLFastSupport = GbeConfig->PchWOLFastSupport;

  // LOCK DOWN HOB
  LockDownConfigHob = BuildGuidHob (&gPchLockDownConfigHobGuid, sizeof (LOCK_DOWN_CONFIG_HOB));
  if (LockDownConfigHob == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (LockDownConfigHob, sizeof (LOCK_DOWN_CONFIG_HOB));
  LockDownConfigHob->GlobalSmi     = LockDownConfig->GlobalSmi;
  LockDownConfigHob->BiosInterface = LockDownConfig->BiosInterface;
  LockDownConfigHob->BiosLock      = LockDownConfig->BiosLock;
  LockDownConfigHob->SpiEiss       = LockDownConfig->SpiEiss;

  // Update of GENERAL HOB in PostMem
  HobPtr = GetFirstGuidHob (&gPchGeneralConfigHobGuid);
  GeneralConfigHob = (PCH_GENERAL_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);
  if (HobPtr == NULL) {
    GeneralConfigHob =  BuildGuidHob (&gPchGeneralConfigHobGuid, sizeof (PCH_GENERAL_CONFIG_HOB));
    if (GeneralConfigHob == NULL) {
      ASSERT (FALSE);
      return;
    }
    ZeroMem (GeneralConfigHob, sizeof (PCH_GENERAL_CONFIG_HOB));
  }
  GeneralConfigHob->Crid          = PchGeneralConfig->Crid;
  GeneralConfigHob->UaolEnable    = UsbConfig->UaolEnable;
  GeneralConfigHob->DisableResets = PchGeneralPreMemConfig->DisableResets;
    
}

/**
  The function update pch general config hob in the end of PchInit.

  @param[in]  SiPreMemPolicy    The SI Pre Meme Policy PPI instance
**/
VOID
BuildPchGeneralConfigHobPreMem (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicy
  )
{
  PCH_GENERAL_CONFIG_HOB       *GeneralConfigHobPreMem;
  EFI_STATUS                   Status;
  PCH_GENERAL_PREMEM_CONFIG    *PchGeneralPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  GeneralConfigHobPreMem = BuildGuidHob (&gPchGeneralConfigHobGuid, sizeof (PCH_GENERAL_CONFIG_HOB));
  if (GeneralConfigHobPreMem == NULL) {
    ASSERT (FALSE);
    return;
  }
  ZeroMem (GeneralConfigHobPreMem, sizeof (PCH_GENERAL_CONFIG_HOB));
  GeneralConfigHobPreMem->DisableResets = PchGeneralPreMemConfig->DisableResets;
}
