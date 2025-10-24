/** @file
  This file provides services for Usb policy function

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <Usb2PhyConfig.h>
#include <Usb3HsioConfig.h>
#include <UsbConfig.h>

/**
  Print USB_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
UsbConfPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  USB_CONFIG    *UsbConfig;
  EFI_STATUS    Status;
  UINT32        Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ USB Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " PdoProgramming        = %x\n", UsbConfig->PdoProgramming));
  DEBUG ((DEBUG_INFO, " OverCurrentEnable     = %x\n", UsbConfig->OverCurrentEnable));
  DEBUG ((DEBUG_INFO, " XhciOcLock            = %x\n", UsbConfig->XhciOcLock));
  DEBUG ((DEBUG_INFO, " UaolEnable            = %x\n", UsbConfig->UaolEnable));
  DEBUG ((DEBUG_INFO, " HsiiEnable            = %x\n", UsbConfig->HsiiEnable));
  DEBUG ((DEBUG_INFO, " DwbEnable             = %x\n", UsbConfig->DwbEnable));

  for (Index = 0; Index < MAX_USB2_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Enabled                = %x\n", Index, UsbConfig->PortUsb20[Index].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].OverCurrentPin         = OC%d\n", Index, UsbConfig->PortUsb20[Index].OverCurrentPin));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].PortResetMessageEnable = %x\n", Index, UsbConfig->PortUsb20[Index].PortResetMessageEnable));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].SwDeviceModeEnable     = %x\n", Index, UsbConfig->PortUsb20[Index].SwDeviceModeEnable));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Usb3CompatiblePort     = %x\n", Index, UsbConfig->PortUsb20[Index].Usb3CompatiblePort));
    DEBUG ((DEBUG_INFO, " PortUsb20[%d].Usb3CompatiblePortNum  = %x\n", Index, UsbConfig->PortUsb20[Index].Usb3CompatiblePortNum));
  }

  for (Index = 0; Index < MAX_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, " PortUsb30[%d] Enabled        = %x\n", Index, UsbConfig->PortUsb30[Index].Enable));
    DEBUG ((DEBUG_INFO, " PortUsb30[%d].OverCurrentPin = OC%d\n", Index, UsbConfig->PortUsb30[Index].OverCurrentPin));
  }

  DEBUG ((DEBUG_INFO, " LtrOverrideEnable             = %x\n", UsbConfig->LtrOverrideEnable));
  if (UsbConfig->LtrOverrideEnable) {
    DEBUG ((DEBUG_INFO, " LtrHighIdleTimeOverride       = 0x%4X\n", UsbConfig->LtrHighIdleTimeOverride));
    DEBUG ((DEBUG_INFO, " LtrMediumIdleTimeOverride     = 0x%4X\n", UsbConfig->LtrMediumIdleTimeOverride));
    DEBUG ((DEBUG_INFO, " LtrLowIdleTimeOverride        = 0x%4X\n", UsbConfig->LtrLowIdleTimeOverride));
  }

  DEBUG ((DEBUG_INFO, " XdciConfig.Enable= %x\n", UsbConfig->XdciConfig.Enable));

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  DEBUG ((DEBUG_INFO, " Index=%d PortUsb31Speed = %d\n", Index, UsbConfig->Usb31Speed));
#endif
  DEBUG ((DEBUG_INFO, " XhciControllerDisable = %x\n", UsbConfig->ControllerDisable));
}

/**
  Print USB2_PHY_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
Usb2PhyPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  USB2_PHY_CONFIG   *Usb2PhyConfig;
  EFI_STATUS        Status;
  UINT32            Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ USB2 PHY Config ------------------\n"));

  for (Index = 0; Index < MAX_USB2_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, " Port[%d].Petxiset           = %x\n", Index, Usb2PhyConfig->Port[Index].Petxiset));
    DEBUG ((DEBUG_INFO, " Port[%d].Txiset             = %x\n", Index, Usb2PhyConfig->Port[Index].Txiset));
    DEBUG ((DEBUG_INFO, " Port[%d].Predeemp           = %x\n", Index, Usb2PhyConfig->Port[Index].Predeemp));
    DEBUG ((DEBUG_INFO, " Port[%d].Pehalfbit          = %x\n", Index, Usb2PhyConfig->Port[Index].Pehalfbit));
    DEBUG ((DEBUG_INFO, " Port[%d].PredeempSemiflexEn = %x\n", Index, Usb2PhyConfig->Port[Index].PredeempSemiflexEn));
  }
}

/**
  Print USB3_HSIO_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
Usb3HsioPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  USB3_HSIO_CONFIG  *Usb3HsioConfig;
  EFI_STATUS        Status;
  UINT32            Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUsb3HsioConfigGuid, (VOID *) &Usb3HsioConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ USB3 HSIO Config ------------------\n"));

  for (Index = 0; Index < MAX_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxDeEmphEnable       = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxDeEmphEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxDeEmph             = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxDeEmph));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxDownscaleAmpEnable = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxDownscaleAmpEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxDownscaleAmp       = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxDownscaleAmp));

    DEBUG ((DEBUG_INFO, " Port[%d].HsioCtrlAdaptOffsetCfgEnable   = %x\n", Index, Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfgEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioCtrlAdaptOffsetCfg         = %x\n", Index, Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfg));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioFilterSelNEnable           = %x\n", Index, Usb3HsioConfig->Port[Index].HsioFilterSelNEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioFilterSelN                 = %x\n", Index, Usb3HsioConfig->Port[Index].HsioFilterSelN));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioFilterSelPEnable           = %x\n", Index, Usb3HsioConfig->Port[Index].HsioFilterSelPEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioFilterSelP                 = %x\n", Index, Usb3HsioConfig->Port[Index].HsioFilterSelP));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioOlfpsCfgPullUpDwnResEnable = %x\n", Index, Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnResEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioOlfpsCfgPullUpDwnRes       = %x\n", Index, Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnRes));

    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate0UniqTranEnable      = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate0UniqTranEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate0UniqTran            = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate0UniqTran));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate1UniqTranEnable      = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate1UniqTranEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate1UniqTran            = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate1UniqTran));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate2UniqTranEnable      = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate2UniqTranEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate2UniqTran            = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate2UniqTran));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate3UniqTranEnable      = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate3UniqTranEnable));
    DEBUG ((DEBUG_INFO, " Port[%d].HsioTxRate3UniqTran            = %x\n", Index, Usb3HsioConfig->Port[Index].HsioTxRate3UniqTran));
  }
}

/**
  Print Usb configs and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
UsbPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  UsbConfPrintConfig (SiPolicyPpi);
  Usb2PhyPrintConfig (SiPolicyPpi);
  Usb3HsioPrintConfig (SiPolicyPpi);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
UsbConfLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN           PortIndex;
  USB_CONFIG      *UsbConfig;
  UsbConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "UsbConfig->Header.GuidHob.Name = %g\n", &UsbConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "UsbConfig->Header.GuidHob.Header.HobLength = 0x%x\n", UsbConfig->Header.GuidHob.Header.HobLength));

  /********************************
    USB related configuration
  ********************************/
  for (PortIndex = 0; PortIndex < MAX_USB2_PORTS; PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Enable                 = TRUE;
    UsbConfig->PortUsb20[PortIndex].OverCurrentPin         = USB_OC_SKIP;
    UsbConfig->PortUsb20[PortIndex].PortResetMessageEnable = FALSE;
    UsbConfig->PortUsb20[PortIndex].SwDeviceModeEnable     = FALSE;
    UsbConfig->PortUsb20[PortIndex].Usb3CompatiblePort     = FALSE;
    UsbConfig->PortUsb20[PortIndex].Usb3CompatiblePortNum  = 0;
  }

  for (PortIndex = 0; PortIndex < MAX_USB3_PORTS; PortIndex++) {
    UsbConfig->PortUsb30[PortIndex].Enable         = TRUE;
    UsbConfig->PortUsb30[PortIndex].OverCurrentPin = USB_OC_SKIP;
  }

  //
  // BIOS should program PDO in PEI phase by default
  //
  UsbConfig->PdoProgramming = TRUE;

  UsbConfig->XhciOcLock = TRUE;
  UsbConfig->UaolEnable = TRUE;
  UsbConfig->HsiiEnable = TRUE;
    UsbConfig->DwbEnable = FALSE;

  //
  // xDCI configuration
  //
  UsbConfig->XdciConfig.Enable = FALSE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
Usb2PhyLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN             PortIndex;
  USB2_PHY_CONFIG   *Usb2PhyConfig;
  Usb2PhyConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "Usb2PhyConfig->Header.GuidHob.Name = %g\n", &Usb2PhyConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Usb2PhyConfig->Header.GuidHob.Header.HobLength = 0x%x\n", Usb2PhyConfig->Header.GuidHob.Header.HobLength));

  /********************************
    USB2 PHY related configuration
  ********************************/
  for (PortIndex = 0; PortIndex < MAX_USB2_PORTS; PortIndex++) {
    Usb2PhyConfig->Port[PortIndex].Pehalfbit = 1;
    Usb2PhyConfig->Port[PortIndex].Petxiset  = 6;
    Usb2PhyConfig->Port[PortIndex].Txiset    = 5;
    Usb2PhyConfig->Port[PortIndex].Predeemp  = 3;
    Usb2PhyConfig->Port[PortIndex].PredeempSemiflexEn = 1;
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
Usb3HsioLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN             PortIndex;
  USB3_HSIO_CONFIG  *Usb3HsioConfig;
  Usb3HsioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "Usb3HsioConfig->Header.GuidHob.Name = %g\n", &Usb3HsioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "Usb3HsioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", Usb3HsioConfig->Header.GuidHob.Header.HobLength));

  for (PortIndex = 0; PortIndex < MAX_USB3_PORTS; PortIndex++) {
    Usb3HsioConfig->Port[PortIndex].HsioOlfpsCfgPullUpDwnRes = 3;
    Usb3HsioConfig->Port[PortIndex].HsioTxRate3UniqTran      = 0x4C;
    Usb3HsioConfig->Port[PortIndex].HsioTxRate2UniqTran      = 0x4C;
    Usb3HsioConfig->Port[PortIndex].HsioTxRate1UniqTran      = 0x4C;
    Usb3HsioConfig->Port[PortIndex].HsioTxRate0UniqTran      = 0x4C;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mUsbBlocks [] = {
  {&gUsbConfigGuid,         sizeof (USB_CONFIG),        USB_CONFIG_REVISION,        UsbConfLoadConfigDefault},
  {&gUsb2PhyConfigGuid,     sizeof (USB2_PHY_CONFIG),   USB2_PHY_CONFIG_REVISION,   Usb2PhyLoadConfigDefault},
  {&gUsb3HsioConfigGuid,    sizeof (USB3_HSIO_CONFIG),  USB3_HSIO_CONFIG_REVISION,  Usb3HsioLoadConfigDefault}
};

/**
  Get Usb config block table size.

  @retval      Size of config block
**/
UINT16
UsbGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mUsbBlocks[0], sizeof (mUsbBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add Usb ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
UsbAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mUsbBlocks[0], sizeof (mUsbBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
