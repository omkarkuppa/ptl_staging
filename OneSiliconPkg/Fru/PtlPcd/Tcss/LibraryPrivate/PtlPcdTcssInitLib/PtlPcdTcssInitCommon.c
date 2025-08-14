/** @file
  TCSS Common Initialization for PTL FRU/SOC.

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

#include "PtlPcdTcssInitPrivate.h"

#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/RngLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/Ptl/PcdMinimalGpioNativeLib/PtlPcdMinimalGpioNativeLib.h>

#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/PcdLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
#include <UsbHandle.h>
#include <ITbtInfoHob.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/GpioHelpersLib.h>

#ifdef IPSIM_ENABLE
#include <stdio.h>
#include <string.h>
#include <IpSim.h>
#include <IpSimLib.h>
#include <IpSimRegMap.h>
#include <IpWrapper.h>
#endif

/**
  Configures GPIO pins for USB OverCurrent detection

  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  OvercurrentPin    Index of OverCurrent Pin to be enabled
  @param[in]  SbVwEnable        Flag to enable sideband messaging for physical OC pins in TCSS
**/
VOID
TcssUsbEnableOvercurrentPin (
  IP_USB3_INST *pInst,
  UINT8        OverCurrentPin,
  BOOLEAN      SbVwEnable
  )
{
  GPIOV2_SERVICES  *GpioServices;
  EFI_STATUS       Status;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    return;
  }

  PtlPcdGpioEnableUsbOverCurrent (GpioServices, OverCurrentPin, SbVwEnable);
}

/**
  Check if this chipset supports TCSS

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
EFIAPI
IsTcssSupported (
  VOID
  )
{
  EFI_STATUS                  Status;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
  TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig;

  SiPreMemPolicyPpi   = NULL;
  TcssPeiPreMemConfig = NULL;


  if (IsHfpgaEnvironment ()) {
    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **) &SiPreMemPolicyPpi
               );
    ASSERT_EFI_ERROR (Status);

    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    if (TcssPeiPreMemConfig->TcssEnable == 0) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Check if this chipset Enable TCSS

  @retval BOOLEAN  TRUE if IP enable, FALSE disable
**/
BOOLEAN
EFIAPI
IsItcssEnabled (
  VOID
  )
{
  return (IsTcssSupported () == TRUE);
}

/*
  Update TCSS DevEn data based on TCSS_USBTC_PEI_PERMEM_CONFIG

  @param[in, out] TcssConfig            Pointer to TCSS_CONFIG
  @param[in]      UsbTcPeiPreMemConfig  Pointer to TCSS_USBTC_PEI_PERMEM_CONFIG
*/
STATIC
VOID
TcssDevEnPolicyUpdate (
  TCSS_CONFIG                   *TcssConfig,
  TCSS_USBTC_PEI_PERMEM_CONFIG  *UsbTcPeiPreMemConfig
  )
{
  UINT32                         Index;

  if ((TcssConfig == NULL) || (UsbTcPeiPreMemConfig == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid Parameter!!\n", __FUNCTION__));
    return;
  }

  TcssConfig->Deven.TcssDevEn = 0;
  for (Index = 0; Index < GetPchMaxTypeCPortNum(); Index ++) {
    if (IS_TC_XHCI_NEEDED (UsbTcPeiPreMemConfig->PortIndex.CapPolicy[Index])) {
      //
      // The USBC Port supports native USB3 while
      // configuration is NoThunderbolt, NoPcie and FullFunction
      //
      TcssConfig->Deven.TcssDevEnBit.TcssXhciEn = 1;
    }
    if (IS_TC_PORT_TUNNELING_SUPPORTED (UsbTcPeiPreMemConfig->PortIndex.CapPolicy[Index])) {
      //
      // The USBC Port required DMA Controller enable while configuration is
      //    DpOnly    - DP tunneling
      //    NoPcie    - USB3/DP tunneling
      // FullFunction - USB3/PCIe/DP tunneling
      //
      TcssConfig->Deven.TcssDevEn |= TCSS_DEVEN_DMA_EN (Index);
      if (IS_TC_PORT_USB4_SUPPORTED (UsbTcPeiPreMemConfig->PortIndex.CapPolicy[Index])) {
        //
        // The USBC Port supports PCIe tunneling while configuration is FullFunction
        //
        TcssConfig->Deven.TcssDevEn |= TCSS_DEVEN_PCIE_EN (Index);
      }
    }
  }
}

/**
  TCSS Instance initialization for default values
**/
STATIC
VOID
TcssInstInitOnCreate (
  TCSS_INST                 *TcssInst,
  TCSS_PEI_PREMEM_CONFIG    *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG           *TcssPeiConfig,
  HOST_BRIDGE_PREMEM_CONFIG *HostBridgePreMemConfig
  )
{
  UINT8          Index;
  EFI_BOOT_MODE  BootMode;
  EFI_STATUS     Status;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  TcssInst->Callbacks->LsxPinsConfigure         = PtlPcdTcssLsxPinEnable;
  TcssInst->Callbacks->LsxOePinsConfigure       = PtlPcdTcssLsxOePinEnable;
  TcssInst->Callbacks->GetTcssDeven             = PtlIpIocTcssGetDevEn;
  TcssInst->Callbacks->ConvertUsbCToUsbA        = PmcTcssConvUsbA;
  TcssInst->Callbacks->PcieSetFunctionMapInSoc  = HostBridgeSetTcssPcieRpFuncMap;
  TcssInst->Callbacks->SetSvidSsid              = PtlSetSvidSsid;
  TcssInst->Callbacks->PmcTcssBiosInitDone      = PmcTcssInitDone;
  TcssInst->Callbacks->EnablePciePmeEvents      = PtlEnablePciePmeEvents;

  TcssInst->Config->PortEnData.PortEnData32 = TcssPeiPreMemConfig->UsbTcConfig.PortEnData32;
  TcssInst->Config->CridEnable       = HostBridgePreMemConfig->CridEnable;
  TcssInst->Config->MaxTcssUsb3Ports = MAX_TCSS_USB3_PORTS;
  TcssInst->Config->BootMode         = (UINT32) BootMode;

  if (TcssPeiConfig == NULL) {
    // Pre-Mem phase
    TcssDevEnPolicyUpdate (TcssInst->Config, &(TcssPeiPreMemConfig->UsbTcConfig));
  } else {
    // Post-Mem phase
    // DEVEN should be overiden by IOC value in Post-Mem
    TcssInst->Config->Deven.TcssDevEn = PtlIpIocTcssGetDevEn ();
  }

  if (TcssPeiConfig != NULL) {
    // Configuration for Post-Mem phase
    TcssInst->Config->PmcPdEnable = TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable;
    TcssInst->Config->VccSt = TcssPeiConfig->IomConfig.IomInterface.VccSt;
    TcssInst->Config->LsxOe = TcssPeiConfig->MiscConfig.LsxOe;
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      TcssInst->Config->TcssConvUsbA[Index].Data8 = TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Data8;
    }
  }
}

STATIC
VOID
IomInstOnCreate (
  IP_IOM_INST             *IomInst,
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG         *TcssPeiConfig
  )
{
  UINT32                        Index;
  IP_WR_REG_INFO                *RegInfo;
  P2SB_SIDEBAND_REGISTER_ACCESS *P2sbAccess;
  P2SB_PORT_16_ID               P2SBPid;

  RegInfo = (IP_WR_REG_INFO*)(UINTN)IomInst->RegCntxtIomSb;
  P2sbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS*)&RegInfo->RegType.P2Sb;

  RegInfo->AccessType = IpWrRegAccessSb;
  RegInfo->AccessSubType = IpWrRegAccessP2Sb;
  RegInfo->RegType.SbId = GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_IOM);

  P2SBPid.Pid16bit = PTL_SID_F3_PID_TC_IOM;
  PtlPcdGetP2SbController (P2sbAccess->P2SbCtrl, P2SBPid);

  BuildP2SbSidebandAccess (
    P2sbAccess->P2SbCtrl,
    GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_IOM),
    0,
    P2SbMemory,
    P2SbMsgAccess,
    TRUE,
    P2sbAccess
    );

#ifdef IPSIM_ENABLE
  // IpSim Override
  IomInst->RegCntxtIomSb = (IP_WR_REG_CNTXT)IpSimGetRegMap (MSGCR_IOM, 0, 0);
#endif

  IomInst->Callback.GetAuxOriGpioSettings      = PtlMinimalGpioIomAuxOriSetting;
  IomInst->Callback.SetTcssDevenRegisterMchbar = HostBridgeSetTcssDeven;
  IomInst->Callback.PmcTcssBiosInitDone        = PmcTcssInitDone;

  // Init to "all disabled". The value MUST be overriden later by TCSS configuration settings
  IomInst->IomConfig.DevEn.TcssDevEn = 0;
  IomInst->IomConfig.DevEn.TcssDevEnBit.Usb4HrEn     = 0x2;
  IomInst->IomConfig.DevEn.TcssDevEnBit.TcssPcieRpEn = 0x4;
  IomInst->IomConfig.DevEn.TcssDevEnBit.TcssXhciEn   = TRUE;
  IomInst->IomConfig.AuxBiasCount   = MAX_IOM_AUX_BIAS_COUNT;

  if (TcssPeiPreMemConfig != NULL) {
    // Pre-Mem phase
    if (TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[0] == IOM_EDP) {
      IomInst->IomConfig.FixConPort[0] = TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[0];
    }

    for (Index = 0; Index < MAX_IOM_AUX_BIAS_COUNT; Index++) {
      IomInst->IomConfig.IomAuxPortPad[Index].ReceptacleSbu1BiasCtrl = TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu1BiasCtrl;
      IomInst->IomConfig.IomAuxPortPad[Index].ReceptacleSbu2BiasCtrl = TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu2BiasCtrl;
      IomInst->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl = TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl;
    }

    IomInst->IomConfig.AuxOriOverride = TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri;
    IomInst->IomConfig.HslOriOverride = TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri;
  }

  if (TcssPeiConfig != NULL) {
    // Post-Mem phase
    IomInst->IomConfig.D3ColdEnable   = TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable;
    IomInst->IomConfig.TcStateLimit   = TcssPeiConfig->IomConfig.TcStateLimit;
    IomInst->IomConfig.VccSt          = TcssPeiConfig->IomConfig.IomInterface.VccSt;
    IomInst->IomConfig.TcNotifyIgd    = TcssPeiConfig->IomConfig.TcNotifyIgd;
  }
  if (IsHfpgaEnvironment ()) {
    IomInst->IomConfig.Environment = HfpgaEnv;
  } else if (IsSimicsEnvironment ()) {
    IomInst->IomConfig.Environment = SimicsEnv;
  } else {
    IomInst->IomConfig.Environment = SiliconEnv;
  }
  IomInst->TcssVersion = 0;
}

STATIC
VOID
Usb3InstOnCreate (
  IP_USB3_INST            *Usb3Inst,
  TCSS_PEI_CONFIG         *TcssPeiConfig
  )
{
  IP_WR_REG_INFO                *RegInfo;
  P2SB_SIDEBAND_REGISTER_ACCESS *P2sbAccess;
  P2SB_PORT_16_ID               P2SBPid;
  UINT32                        Index;

  //
  // Sideband registers access
  //
  RegInfo = (IP_WR_REG_INFO*)(UINTN)Usb3Inst->RegCntxtSbMmio;
  P2sbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS*)&RegInfo->RegType.P2Sb;

  RegInfo->AccessType = IpWrRegAccessSb;
  RegInfo->AccessSubType = IpWrRegAccessP2Sb;
  RegInfo->RegType.SbId = GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_XHCI);

  P2SBPid.Pid16bit = PTL_SID_F3_PID_TC_XHCI;
  PtlPcdGetP2SbController (P2sbAccess->P2SbCtrl, P2SBPid);

  BuildP2SbSidebandAccess (
    P2sbAccess->P2SbCtrl,
    GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_XHCI),
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    P2sbAccess
    );

  //
  // DCI Sideband registers access
  //
  RegInfo = (IP_WR_REG_INFO*)(UINTN)Usb3Inst->RegCntxtDciSbMmio;
  P2sbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS*)&RegInfo->RegType.P2Sb;

  RegInfo->AccessType = IpWrRegAccessSb;
  RegInfo->AccessSubType = IpWrRegAccessP2Sb;
  RegInfo->RegType.SbId = GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_XHCI);

  P2SBPid.Pid16bit = PTL_SID_F3_PID_TC_XHCI;
  PtlPcdGetP2SbController (P2sbAccess->P2SbCtrl, P2SBPid);

  BuildP2SbSidebandAccess (
    P2sbAccess->P2SbCtrl,
    GET_P2SB_LOCAL_PID (PTL_SID_F3_PID_TC_XHCI),
    0x68,
    P2SbMemory,
    P2SbMsgAccess,
    FALSE,
    P2sbAccess
    );
  P2sbAccess->Bar = 5;

  //
  // Primary PCIe Config space registers access
  //
  RegInfo = (IP_WR_REG_INFO*)(UINTN)Usb3Inst->RegCntxtPci;

  RegInfo->AccessType = IpWrRegAccessCfg;
  RegInfo->RegType.Pci.Seg = 0;
  RegInfo->RegType.Pci.Bus = XHCI_NORTH_BUS_NUM;
  RegInfo->RegType.Pci.Dev = XHCI_NORTH_DEV_NUM;
  RegInfo->RegType.Pci.Fun = XHCI_NORTH_FUNC_NUM;

  //
  // Primary MMIO space registers access
  //
  RegInfo = (IP_WR_REG_INFO*)(UINTN)Usb3Inst->RegCntxtMem;

  RegInfo->AccessType = IpWrRegAccessMem;
  RegInfo->RegType.MmioBase = PCH_TEMP_BASE_ADDRESS;

#ifdef IPSIM_ENABLE
// IpSim Override
  Usb3Inst->RegCntxtPci = (IP_WR_REG_CNTXT)IpSimGetRegMap (CFG_USB3, 0, 0);
  Usb3Inst->RegCntxtMem = (IP_WR_REG_CNTXT)IpSimGetRegMap (MEM_USB3, 0, 0);
  Usb3Inst->RegCntxtSbMmio = (IP_WR_REG_CNTXT)IpSimGetRegMap (MEM_USB3, 0, 0);
#endif

  Usb3Inst->Prefix.pName = "IP_USB3_TCSS";

  Usb3Inst->IpVersion = IpUsb3IpVersion19p3;
  Usb3Inst->Integration = IpUsb3IntegrationUsb4ss;
  Usb3Inst->MmioBaseAddress = PCH_TEMP_BASE_ADDRESS;

  Usb3Inst->Usb2PortCount = 0;
  if (IsWclPcd ()) {
    Usb3Inst->Usb3LanesCount = 2;
  } else {
    Usb3Inst->Usb3LanesCount = 4;
  }

  Usb3Inst->LtrEnable = FALSE;
  Usb3Inst->NdeSbEnable = FALSE;
  Usb3Inst->HsiiEnable = FALSE;
  Usb3Inst->FabricPllAdvanceWake = 0x19;
  Usb3Inst->LtrHighIdleTime = 0x00050002;
  Usb3Inst->LtrMedIdleTime = 0x00050002;
  Usb3Inst->LtrLowIdleTime = 0x00050002;
  Usb3Inst->LtvLimit = 0x17FF;
  Usb3Inst->MaxPayloadSize = 0x7;
  Usb3Inst->RequestBoundaryCrossingControl = 0x1;

  if (TcssPeiConfig != NULL) {
    // Post-Mem phase
    Usb3Inst->PdoProgramming        = !!TcssPeiConfig->UsbConfig.PdoProgramming;
    Usb3Inst->OverCurrentEnable     = !!TcssPeiConfig->UsbConfig.OverCurrentEnable;
    Usb3Inst->OverCurrentLock       = !!TcssPeiConfig->UsbConfig.XhciOcLock;
    Usb3Inst->UsbAudioOffloadEnable = !!TcssPeiConfig->UsbConfig.UaolEnable;
    Usb3Inst->Usb31SpeedAdvertise   = TcssPeiConfig->UsbConfig.Usb31Speed;

    for (Index = 0; Index < Usb3Inst->Usb3LanesCount; Index++) {
      Usb3Inst->Usb3PortConfig[Index].Enable = !!TcssPeiConfig->UsbConfig.PortUsb30[Index].Enable;
      Usb3Inst->Usb3PortConfig[Index].OverCurrentPin = (UINT8)TcssPeiConfig->UsbConfig.PortUsb30[Index].OverCurrentPin;
    }

    Usb3Inst->DisableHostController = !!TcssPeiConfig->UsbConfig.ControllerDisable;
  }

  // Callbacks
  Usb3Inst->cCalculateOcPin = TcssCalculateOcPin;
  Usb3Inst->cEnableOvercurrentPin = TcssUsbEnableOvercurrentPin;
}

STATIC
VOID
Usb4InstOnCreate (
  IP_USB4_INST            *Usb4Inst,
  PEI_ITBT_CONFIG         *PeiITbtConfig
  )
{
  UINT32          DmaIndex;
  IP_USB4_DMA     *DmaPort;
  IP_WR_REG_INFO  *RegInfo;
  UINT32          Index;

  //
  // Create DMA registers access context
  //
  DmaPort = Usb4Inst->DmaList;
  for (DmaIndex = 0; DmaPort != NULL; DmaPort = DmaPort->Next, DmaIndex++) {
    //
    // Primary DMA Config space registers access
    //
    RegInfo = (IP_WR_REG_INFO*)(UINTN)DmaPort->PrimCfgAccess;
    RegInfo->AccessType = IpWrRegAccessCfg;
    RegInfo->RegType.Pci.Seg = 0;
    RegInfo->RegType.Pci.Bus = HOST_DMA_BUS_NUM;
    RegInfo->RegType.Pci.Dev = HOST_DMA_DEV_NUM;
    RegInfo->RegType.Pci.Fun = (UINT8)(HOST_DMA0_FUN_NUM + DmaIndex);
#ifdef IPSIM_ENABLE
    DmaPort->PrimCfgAccess = (IP_WR_REG_CNTXT)IpSimGetRegMap (CFG_DMA, 0, DmaIndex);
#endif
  }

  //
  // USB4 configuration init
  //
  Usb4Inst->Usb4Config.DevEn.DevEn = 0; // Will be upated later based on TCSS config
  Usb4Inst->Usb4Config.ITbtToPcieReg = FixedPcdGet8 (PcdITbtToPcieRegister);
  Usb4Inst->Usb4Config.PcieToITbtReg = FixedPcdGet8 (PcdPcieToITbtRegister);
  Usb4Inst->BootMode = 0;

  if (PeiITbtConfig != NULL) {
    Usb4Inst->Usb4Config.ITbtConnectTopologyTimeoutInMs = PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs;
    for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index ++) {
      Usb4Inst->Usb4Config.ITbtDmaLtr[Index] = PeiITbtConfig->ITbtDmaLtr[Index];
    }
    Usb4Inst->Usb4Config.ITbtForcePowerOnTimeoutInMs = PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs;
    Usb4Inst->Usb4Config.ITbtPcieTunnelingForUsb4 = PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4;
    Usb4Inst->Usb4Config.ITbtSecurityLevel = PeiITbtConfig->ITbtGenericConfig.ITbtSecurityLevel;

    Usb4Inst->Usb4Config.Usb4CmMode = PeiITbtConfig->ITbtGenericConfig.Usb4CmMode;
  }

  GetRandomNumber64 (&Usb4Inst->RandomNum);
}

STATIC
VOID
PcieRpInstOnCreate (
  IP_TC_PCIE_RP_INST      *PcieInst,
  TCSS_PEI_CONFIG         *TcssPeiConfig
  )
{
  UINT32                        PcieRpIndex;
  UINT16                        Pid;
  IP_PCIE_ROOT_PORT             *RootPort;
  IP_WR_REG_INFO                *RegInfo;
  P2SB_SIDEBAND_REGISTER_ACCESS *P2sbAccess;
  P2SB_PORT_16_ID               P2SBPid;
  TCSS_PCIE_PORT_POLICY         *PcieRpConfig;
  EFI_STATUS                    Status;

  //
  // Create PCIE RP registers access context
  //
  RootPort = PcieInst->PciRootPortList;
  for (PcieRpIndex = 0; RootPort != NULL; RootPort = RootPort->Next, PcieRpIndex++) {
    //
    // Primary PCIe RP Config space registers access
    //
    RegInfo = (IP_WR_REG_INFO*)(UINTN)RootPort->PrimCfgAccess;
    RegInfo->AccessType = IpWrRegAccessCfg;
    RegInfo->RegType.Pci.Seg = 0;
    RegInfo->RegType.Pci.Bus = ITBT_PCIE_BUS_NUM;
    RegInfo->RegType.Pci.Dev = ITBT_PCIE_DEV_NUM;
    RegInfo->RegType.Pci.Fun = (UINT8)(ITBT_PCIE_0_DEFAULT_FUN_NUM + PcieRpIndex);
    //
    // Sideband PCIe RP registers
    //
    switch (PcieRpIndex) {
      case 0:
        Pid = PTL_SID_F3_PID_TC_PCIE0;
        break;
      case 1:
        Pid = PTL_SID_F3_PID_TC_PCIE1;
        break;
      case 2:
        Pid = PTL_SID_F3_PID_TC_PCIE2;
        break;
      case 3:
        Pid = PTL_SID_F3_PID_TC_PCIE3;
        break;
    }
    RegInfo = (IP_WR_REG_INFO*)(UINTN)RootPort->PcrAccess;
    P2sbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS*)&RegInfo->RegType.P2Sb;

    P2SBPid.Pid16bit = Pid;
    Status = PtlPcdGetP2SbController (P2sbAccess->P2SbCtrl, P2SBPid);
    if (Status == EFI_INVALID_PARAMETER) {
      DEBUG ((DEBUG_ERROR, "%a: SB access init failed!\n", __FUNCTION__));
      return;
    }

    RegInfo->AccessType    = IpWrRegAccessSb;
    RegInfo->AccessSubType = IpWrRegAccessP2Sb;
    RegInfo->RegType.SbId  = (UINT8) GET_P2SB_LOCAL_PID (Pid);

    BuildP2SbSidebandAccess (
      P2sbAccess->P2SbCtrl,
      (P2SB_PID) GET_P2SB_LOCAL_PID (Pid),
      0,
      P2SbPrivateConfig,
      P2SbMsgAccess,
      FALSE,
      P2sbAccess
      );
#ifdef IPSIM_ENABLE
    // IpSim Override
    RootPort->PcrAccess = (IP_WR_REG_CNTXT)IpSimGetRegMap (MSGCR_PCIE, 0, PcieRpIndex);
    RootPort->PrimCfgAccess = (IP_WR_REG_CNTXT)IpSimGetRegMap (CFG_PCIE, 0, PcieRpIndex);
#endif
  }

  PcieInst->DevEn.DevEn = 0;  // Will be later updated based on TCSS config
  PcieInst->DevEn.DevEnBit.TcssPcieRpEn = 0x4;
  PcieInst->Callbacks->PcieRpDownStreamConfig = PtlTcPcieRpDownStreamConfig;

  //
  // TC PCIe Root Ports configuration init
  //
  RootPort = PcieInst->PciRootPortList;
  for (PcieRpIndex = 0; RootPort != NULL; RootPort = RootPort->Next, PcieRpIndex++) {
    if (TcssPeiConfig != NULL) {
      PcieRpConfig = &(TcssPeiConfig->PciePolicy.PciePortPolicy[PcieRpIndex]);
      RootPort->PcieRpConfig.PtmEnabled                        = PcieRpConfig->PtmEnabled;
      RootPort->PcieRpConfig.AdvancedErrorReporting            = PcieRpConfig->AdvancedErrorReporting;
      RootPort->PcieRpConfig.SlotNumber                        = PcieRpConfig->SlotNumber;
      RootPort->PcieRpConfig.AcsEnabled                        = PcieRpConfig->AcsEnabled;
      RootPort->PcieRpConfig.LtrEnable                         = PcieRpConfig->LtrEnable;
      RootPort->PcieRpConfig.SnoopLatencyOverrideMode          = PcieRpConfig->SnoopLatencyOverrideMode;
      RootPort->PcieRpConfig.SnoopLatencyOverrideMultiplier    = PcieRpConfig->SnoopLatencyOverrideMultiplier;
      RootPort->PcieRpConfig.SnoopLatencyOverrideValue         = PcieRpConfig->SnoopLatencyOverrideValue;
      RootPort->PcieRpConfig.NonSnoopLatencyOverrideMode       = PcieRpConfig->NonSnoopLatencyOverrideMode;
      RootPort->PcieRpConfig.NonSnoopLatencyOverrideMultiplier = PcieRpConfig->NonSnoopLatencyOverrideMultiplier;
      RootPort->PcieRpConfig.NonSnoopLatencyOverrideValue      = PcieRpConfig->NonSnoopLatencyOverrideValue;
      RootPort->PcieRpConfig.ForceLtrOverride                  = PcieRpConfig->ForceLtrOverride;
      RootPort->PcieRpConfig.LtrConfigLock                     = PcieRpConfig->LtrConfigLock;
      RootPort->PcieRpConfig.LtrMaxSnoopLatency                = PcieRpConfig->LtrMaxSnoopLatency;
      RootPort->PcieRpConfig.LtrMaxNoSnoopLatency              = PcieRpConfig->LtrMaxNoSnoopLatency;
    }
  }
}

/**
  Initializes TCSS instance structure and Sub-IPs instance structures
  Note:
  Caller is responsible for the memory allocation
 **/
EFI_STATUS
PtlTcssInstInit (
  TCSS_INST               *TcssInst,
  TCSS_CALLBACKS          *TcssCallbacks,
  TCSS_CONFIG             *TcssOptions,
  TCSS_INFO               *TcssInfo,
  IP_USB3_INST            *Usb3Inst,
  IP_WR_REG_INFO          *Usb3RegInfoPciPrim,
  IP_WR_REG_INFO          *Usb3RegInfoMmio,
  IP_WR_REG_INFO          *Usb3RegInfoPcr,
  IP_WR_REG_INFO          *Usb3RegInfoDci,
  P2SB_CONTROLLER         *P2SbController,
  IP_USB4_INST            *Usb4Inst,
  IP_USB4_DMA             *Usb4Dma,
  IP_WR_REG_INFO          *Usb4DmaRegInfoPciPrim,
  IP_IOM_INST             *IomInst,
  IP_WR_REG_INFO          *IomRegInfoPcr,
  IP_TC_PCIE_RP_INST      *PcieRpInst,
  IP_PCIE_ROOT_PORT       *PcieRpPorts,
  IP_WR_REG_INFO          *PcieRpRegInfoPciPrim,
  IP_WR_REG_INFO          *PcieRpRegInfoPcr,
  IP_PCIE_CALLBACKS       *PcieCallbacks,
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG         *TcssPeiConfig,
  PEI_ITBT_CONFIG         *PeiITbtConfig,
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig
  )
{
  EFI_STATUS        Status;
  IP_CSI_STATUS     IpCsiStatus;
  UINT32            Index;
  IP_PCIE_ROOT_PORT *RootPort;
  IP_PCIE_ROOT_PORT **RpPtr;
  IP_WR_REG_INFO    *RegInfo;
  IP_USB4_DMA       *DmaDev;
  IP_USB4_DMA       **DmaPtr;

  //
  // Clear all structures
  //
  ZeroMem (TcssInst,              sizeof (TCSS_INST));
  ZeroMem (TcssCallbacks,         sizeof (TCSS_CALLBACKS));
  ZeroMem (TcssOptions,           sizeof (TCSS_CONFIG));
  ZeroMem (TcssInfo,              sizeof (TCSS_INFO));
  ZeroMem (IomInst,               sizeof (IP_IOM_INST));
  ZeroMem (PcieRpInst,            sizeof (IP_TC_PCIE_RP_INST));
  ZeroMem (PcieRpPorts,           sizeof (IP_PCIE_ROOT_PORT) * MAX_ITBT_PCIE_PORT);
  ZeroMem (Usb4Dma,               sizeof (IP_USB4_DMA) * MAX_HOST_ITBT_DMA_NUMBER);
  ZeroMem (Usb4DmaRegInfoPciPrim, sizeof (IP_WR_REG_INFO) * MAX_HOST_ITBT_DMA_NUMBER);
  ZeroMem (Usb3Inst,              sizeof (IP_USB3_INST));
  ZeroMem (Usb4Inst,              sizeof (IP_USB4_INST));
  ZeroMem (Usb3RegInfoPciPrim,    sizeof (IP_WR_REG_INFO));
  ZeroMem (Usb3RegInfoMmio,       sizeof (IP_WR_REG_INFO));
  ZeroMem (Usb3RegInfoPcr,        sizeof (IP_WR_REG_INFO));
  ZeroMem (Usb3RegInfoDci,        sizeof (IP_WR_REG_INFO));
  ZeroMem (IomRegInfoPcr,         sizeof (IP_WR_REG_INFO));
  ZeroMem (PcieRpRegInfoPciPrim,  sizeof (IP_WR_REG_INFO) * MAX_ITBT_PCIE_PORT);
  ZeroMem (PcieRpRegInfoPcr,      sizeof (IP_WR_REG_INFO) * MAX_ITBT_PCIE_PORT);
  ZeroMem (P2SbController,        sizeof (P2SB_CONTROLLER));
  ZeroMem (PcieCallbacks,         sizeof (IP_PCIE_CALLBACKS));

  //
  // TCSS Instance initialization
  //
  TcssInst->Callbacks = TcssCallbacks;
  TcssInst->Config    = TcssOptions;
  TcssInst->Info      = TcssInfo;
  TcssInstInitOnCreate (TcssInst, TcssPeiPreMemConfig, TcssPeiConfig, HostBridgePreMemConfig);

  //
  // PCIe RP Ports list initialization
  //
  PcieRpInst->Callbacks = PcieCallbacks;
  RpPtr = &PcieRpInst->PciRootPortList;
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    // Root Port
    RootPort = &PcieRpPorts[Index];
    // Primary access
    RegInfo = &PcieRpRegInfoPciPrim[Index];
    RootPort->PrimCfgAccess = (IP_WR_REG_CNTXT)(UINTN)RegInfo;
    // Sideband access
    RegInfo = &PcieRpRegInfoPcr[Index];
    RegInfo->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER*)P2SbController;
    RootPort->PcrAccess = (IP_WR_REG_CNTXT)(UINTN)RegInfo;
    RootPort->Next = NULL;
    // Add the Root Port to the list
    *RpPtr = RootPort;
    RpPtr = &RootPort->Next;
  }

  //
  // USB3 Instance Initialization
  //
  Usb3Inst->RegCntxtPci = (IP_WR_REG_CNTXT)(UINTN)Usb3RegInfoPciPrim;
  Usb3Inst->RegCntxtMem = (IP_WR_REG_CNTXT)(UINTN)Usb3RegInfoMmio;
  Usb3Inst->RegCntxtSbMmio = (IP_WR_REG_CNTXT)(UINTN)Usb3RegInfoPcr;
  Usb3RegInfoPcr->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER*)P2SbController;
  Usb3Inst->RegCntxtDciSbMmio = (IP_WR_REG_CNTXT)(UINTN)Usb3RegInfoDci;
  Usb3RegInfoDci->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER*)P2SbController;

  //
  // USB4 DMA list initialization
  //
  DmaPtr = &Usb4Inst->DmaList;
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    // DMA device
    DmaDev = &Usb4Dma[Index];
    // DMA Primary CFG access
    DmaDev->PrimCfgAccess = (IP_WR_REG_CNTXT)(UINTN)&Usb4DmaRegInfoPciPrim[Index];
    DmaDev->Next = NULL;
    // DMA device to the list
    *DmaPtr = DmaDev;
    DmaPtr = &DmaDev->Next;
  }

  //
  // IOM Instance construction
  //
  IomRegInfoPcr->RegType.P2Sb.P2SbCtrl = (IP_WR_P2SB_CONTROLLER*)P2SbController;
  IomInst->RegCntxtIomSb = (IP_WR_REG_CNTXT)(UINTN)IomRegInfoPcr;

  //
  // Include all Sub IPs instances to TCSS instance
  //
  TcssInst->Usb3Inst  = Usb3Inst;
  TcssInst->Usb4Inst  = Usb4Inst;
  TcssInst->IomInst   = IomInst;
  TcssInst->PcieInst  = PcieRpInst;

  //
  // All TCSS Sub-IPs instances initialization
  //
  Usb3InstOnCreate (TcssInst->Usb3Inst, TcssPeiConfig);
  Usb4InstOnCreate (Usb4Inst, PeiITbtConfig);
  PcieRpInstOnCreate (TcssInst->PcieInst, TcssPeiConfig);
  IomInstOnCreate (TcssInst->IomInst, TcssPeiPreMemConfig, TcssPeiConfig);

  //
  // Synchronize DEVEN values across all Sub-IPs based on TCSS config
  //
  TcssInst->PcieInst->DevEn.DevEn              = TcssInst->Config->Deven.TcssDevEn;
  TcssInst->Usb4Inst->Usb4Config.DevEn.DevEn   = TcssInst->Config->Deven.TcssDevEn;
  TcssInst->IomInst->IomConfig.DevEn.TcssDevEn = TcssInst->Config->Deven.TcssDevEn;

  //
  // Private TCSS Instance Init (initializes private data in the instance structure)
  //
  IpCsiStatus = TcssInstInit (TcssInst);

  if (IpCsiStatus != 0) {
    Status = EFI_DEVICE_ERROR;
    DEBUG((DEBUG_ERROR, "ERROR: TCSS Pre Memory initialization failed! IpCsiStatus 0x%x\n", IpCsiStatus));
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}
