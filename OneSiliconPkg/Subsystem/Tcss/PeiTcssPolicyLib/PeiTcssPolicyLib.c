/** @file
  This file provides services for TCSS policy function

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <TcssPeiPreMemConfig.h>
#include <TcssPeiConfig.h>
#include <Register/CpuUsbRegs.h>
#include <TcssInfo.h>
#include <Library/PchInfoLib.h>

/**
  Print TCSS_PEI_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
TcssPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS             Status;
  TCSS_PEI_CONFIG        *TcssPeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR (Status);

  TcssConfigDump (TcssPeiConfig);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadTcssIomConfigDefault (
IN TCSS_IOM_PEI_CONFIG     *IomConfig
  )
{
  IomConfig->IomInterface.D3ColdEnable = 1;
  IomConfig->TcStateLimit              = 10;
  IomConfig->IomInterface.VccSt        = 0x1;
  IomConfig->IomInterface.D3ColdEnable = 1;
  IomConfig->TcNotifyIgd               = 0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadTcssPmcpdDefault (
  IN TCSS_IOM_PEI_CONFIG     *IomConfig
  )
{
  IomConfig->PmcInterface.PmcPdEnable = 1;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPeiPcieCommonConfigDefault(
  IN TCSS_PCIE_PORT_POLICY *SaPeiPciCommonConfig
  )
{
  SaPeiPciCommonConfig->AcsEnabled = 1;
  SaPeiPciCommonConfig->LtrEnable = 1;
  SaPeiPciCommonConfig->PtmEnabled = 1;
  SaPeiPciCommonConfig->Aspm = 1;
  SaPeiPciCommonConfig->SlotNumber = 0x0;
  SaPeiPciCommonConfig->SlotPowerLimitScale = 0;
  SaPeiPciCommonConfig->SlotPowerLimitValue = 0;
  SaPeiPciCommonConfig->AdvancedErrorReporting = 1;
  SaPeiPciCommonConfig->UnsupportedRequestReport = 0;
  SaPeiPciCommonConfig->FatalErrorReport = 0;
  SaPeiPciCommonConfig->NoFatalErrorReport = 0;
  SaPeiPciCommonConfig->CorrectableErrorReport = 0;

  SaPeiPciCommonConfig->SystemErrorOnFatalError = 0;
  SaPeiPciCommonConfig->SystemErrorOnNonFatalError = 0;
  SaPeiPciCommonConfig->SystemErrorOnCorrectableError = 0;

  ///
  /// LTR Settings Defaults
  ///
  SaPeiPciCommonConfig->LtrMaxSnoopLatency = 0x100F;
  SaPeiPciCommonConfig->LtrMaxNoSnoopLatency = 0x100F;
  SaPeiPciCommonConfig->SnoopLatencyOverrideMode = 1;
  SaPeiPciCommonConfig->SnoopLatencyOverrideMultiplier = 2;
  SaPeiPciCommonConfig->SnoopLatencyOverrideValue = 200;        /// Default is 200us
  SaPeiPciCommonConfig->NonSnoopLatencyOverrideMode = 1;
  SaPeiPciCommonConfig->NonSnoopLatencyOverrideMultiplier = 2;
  SaPeiPciCommonConfig->NonSnoopLatencyOverrideValue = 200;     /// Default is 200us
  SaPeiPciCommonConfig->ForceLtrOverride = 0;
  SaPeiPciCommonConfig->LtrConfigLock = 0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadTcssUsb3ConfigDefault (
  IN USB_CONFIG       *UsbConfig
  )
{
  UINTN           PortIndex;

  /********************************
    USB related configuration
  ********************************/
  for (PortIndex = 0; PortIndex < MAX_USB2_PORTS; PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Enable                 = FALSE;
    UsbConfig->PortUsb20[PortIndex].OverCurrentPin         = USB_OC_SKIP;
    UsbConfig->PortUsb20[PortIndex].PortResetMessageEnable = FALSE;
  }

  for (PortIndex = 0; PortIndex < MAX_USB3_PORTS; PortIndex++) {
    UsbConfig->PortUsb30[PortIndex].Enable         = FALSE;
    UsbConfig->PortUsb30[PortIndex].OverCurrentPin = USB_OC_SKIP;
  }
  UsbConfig->OverCurrentEnable = 0x1;

  //
  // BIOS should program PDO in PEI phase by default
  //
  UsbConfig->PdoProgramming = TRUE;
  UsbConfig->UaolEnable = FALSE;
  UsbConfig->XhciOcLock = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadTcssMiscConfigDefault (
  IN TCSS_MISC_PEI_CONFIG     *MiscConfig
  )
{
  UINTN        Index;

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    MiscConfig->TcssConvUsbA[Index].Data8 = 0;
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TcssLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TCSS_PEI_CONFIG   *TcssConfig;
  UINTN Index;

  TcssConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "TcssConfig->Header.GuidHob.Name = %g\n", &TcssConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TcssConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TcssConfig->Header.GuidHob.Header.HobLength));

  LoadTcssPmcpdDefault (&(TcssConfig->IomConfig));
  LoadTcssIomConfigDefault (&(TcssConfig->IomConfig));

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    LoadPeiPcieCommonConfigDefault (&(TcssConfig->PciePolicy.PciePortPolicy[Index]));
    (&(TcssConfig->PciePolicy.PciePortPolicy[Index]))->SlotNumber = GetPchMaxPciePortNum () + (UINT8)Index;
  }
  TcssConfig->PciePolicy.TcssTbtPerfBoost = 0x0F;

  LoadTcssUsb3ConfigDefault (&(TcssConfig->UsbConfig));
  LoadTcssMiscConfigDefault (&(TcssConfig->MiscConfig));
}

STATIC COMPONENT_BLOCK_ENTRY  mTcssBlocks = {
  &gTcssPeiConfigGuid,
  sizeof (TCSS_PEI_CONFIG),
  TCSS_PEI_CONFIG_REVISION,
  TcssLoadConfigDefault
};

/**
  Get TCSS config block table size.

  @retval      Size of config block
**/
UINT16
TcssGetConfigBlockTotalSize (
  VOID
  )
{
  return mTcssBlocks.Size;
}

/**
  Add TCSS ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TcssAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTcssBlocks, 1);
}
