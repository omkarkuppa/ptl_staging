/** @file
  Initializes SD host controller located on SCS Iosf2Ocp bridge.
  This file contains routines for both eMMC and SD Card host controllers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Register/ScsSdRegs.h>
#include "PeiScsInitInternal.h"

/**
  Configures SD host controller DLL values.

  @param[in] MmioBase  MMIO base of the controller
  @param[in] DllValues Pointer to the structure holding DLL values of the controller.
**/
STATIC
VOID
ConfigureScsSdHostDll (
  IN UINTN       MmioBase,
  IN SCS_SD_DLL  *DllValues
  )
{
  if (DllValues == NULL) {
    ASSERT (FALSE);
    return;
  }

  MmioWrite32 (MmioBase + R_SCS_MEM_TX_CMD_DLL_CNTL, DllValues->TxCmdDelayControl);
  MmioWrite32 (MmioBase + R_SCS_MEM_TX_DATA_DLL_CNTL1, DllValues->TxDataDelayControl1);
  MmioWrite32 (MmioBase + R_SCS_MEM_TX_DATA_DLL_CNTL2, DllValues->TxDataDelayControl2);
  MmioWrite32 (MmioBase + R_SCS_MEM_RX_CMD_DATA_DLL_CNTL1, DllValues->RxCmdDataDelayControl1);
  MmioWrite32 (MmioBase + R_SCS_MEM_RX_CMD_DATA_DLL_CNTL2, DllValues->RxCmdDataDelayControl2);
  MmioWrite32 (MmioBase + R_SCS_MEM_RX_STROBE_DLL_CNTL, DllValues->RxStrobeDelayControl);

  DEBUG ((DEBUG_INFO, "Tx CMD Delay Control (820h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_TX_CMD_DLL_CNTL)));
  DEBUG ((DEBUG_INFO, "Tx Data Delay Control 1 (824h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_TX_DATA_DLL_CNTL1)));
  DEBUG ((DEBUG_INFO, "Tx Data Delay Control 2 (828h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_TX_DATA_DLL_CNTL2)));
  DEBUG ((DEBUG_INFO, "Rx CMD + Data Delay Control 1 (82Ch) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_RX_CMD_DATA_DLL_CNTL1)));
  DEBUG ((DEBUG_INFO, "Rx CMD + Data Delay Control 2 (834h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_RX_CMD_DATA_DLL_CNTL2)));
  DEBUG ((DEBUG_INFO, "Rx Strobe Delay Control (830h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_RX_STROBE_DLL_CNTL)));
}

/**
  Configures SD host controller capabilites.

  @param[in] MmioBase  MMIO base address of controller
  @param[in] SdCaps    Pointer to the structure containing controller capabilities
**/
STATIC
VOID
ConfigureScsSdHostCapabilities (
  IN UINTN        MmioBase,
  IN SCS_SD_CAPS  *SdCaps
  )
{
  if (SdCaps == NULL) {
    ASSERT (FALSE);
    return;
  }

  MmioWrite32 (MmioBase + R_SCS_MEM_CAP_BYPASS_REG1, SdCaps->CapReg1);
  MmioWrite32 (MmioBase + R_SCS_MEM_CAP_BYPASS_REG2, SdCaps->CapReg2);

  //
  // Enable Capabilities Bypass
  //
  MmioWrite32 (MmioBase + R_SCS_MEM_CAP_BYPASS_CONTROL, V_SCS_MEM_CAP_BYPASS_CONTROL_EN);
  DEBUG ((DEBUG_INFO, "Capabilities Bypass Control    (810h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_CAP_BYPASS_CONTROL)));
  DEBUG ((DEBUG_INFO, "Capabilities Bypass Register 1 (814h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_CAP_BYPASS_REG1)));
  DEBUG ((DEBUG_INFO, "Capabilities Bypass Register 2 (818h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_CAP_BYPASS_REG2)));
  DEBUG ((DEBUG_INFO, "Capabilities Register [31:0]  (40h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_CAP1)));
  DEBUG ((DEBUG_INFO, "Capabilities Register [63:32] (44h) = 0x%08x\n", MmioRead32 (MmioBase + R_SCS_MEM_CAP2)));
}

/**
  Configure eMMC controller HS400 mode.

  @param[in] MmioBase    MMIO base of the controller
  @param[in] EmmcConfig  eMMC configuration
**/
STATIC
VOID
ScsEmmcConfigureHostHs400 (
  IN UINTN            MmioBase,
  IN SCS_EMMC_CONFIG  *EmmcConfig
  )
{
  if (EmmcConfig->Hs400Supported) {
    MmioAndThenOr32 (
      MmioBase + R_SCS_MEM_CAP_BYPASS_REG1,
      (UINT32) ~0,
      B_SCS_MEM_CAP_BYPASS_REG1_HS400
      );
  } else {
    DEBUG ((DEBUG_INFO, "Disabling eMMC HS400 support\n"));
    MmioAndThenOr32 (
      MmioBase + R_SCS_MEM_CAP_BYPASS_REG1,
      (UINT32) ~B_SCS_MEM_CAP_BYPASS_REG1_HS400,
      0
      );
  }
}

/**
  Initialize eMMC MMIO space.

  @param[in] ScsEmmcHandle  Handle.
**/
STATIC
VOID
ScsEmmcInitMmioRegisters (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  )
{
  SCS_SD_DLL  *EmmcDlls;

  ScsControllerEnableMmio (ScsEmmcHandle->Controller.PciCfgBase, (UINTN) ScsEmmcHandle->Controller.MmioBase);

  ConfigureScsSdHostCapabilities (
    ScsEmmcHandle->Controller.MmioBase,
    &ScsEmmcHandle->SocConfig.Caps
    );

  if (ScsEmmcHandle->Config->UseCustomDlls) {
    EmmcDlls = &ScsEmmcHandle->Config->CustomDllValues;
  } else {
    EmmcDlls = &ScsEmmcHandle->SocConfig.DefaultDll;
  }

  ConfigureScsSdHostDll (
    ScsEmmcHandle->Controller.MmioBase,
    EmmcDlls
    );

  ScsEmmcConfigureHostHs400 ((UINTN) ScsEmmcHandle->Controller.MmioBase, ScsEmmcHandle->Config);

  ScsControllerDisableMmio (ScsEmmcHandle->Controller.PciCfgBase);
}

/**
  Configure SD card specific capabilities in SD_CAPS_BYPS register.

  @param[in] MmioBase        MMIO address to use during initialization
**/
STATIC
VOID
ScsSdCardConfigureSdCaps (
  IN UINTN  MmioBase
  )
{
  //
  // SD card controller by default advertises the UHS II features support
  // such as ADMA3 or VDD2 1.8 support. This is not true for our silicon so
  // we have to disable those features.
  //
  MmioWrite32 (MmioBase + R_SCS_MEM_SD_CAP_BYPS, 0x29214000);
}

/**
  Initialize SD Card MMIO space.

  @param[in] ScsSdCardHandle  Handle.
**/
STATIC
VOID
ScsSdCardInitMmioRegisters (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  )
{
  SCS_SD_DLL  *SdCardDlls;

  ScsControllerEnableMmio (ScsSdCardHandle->Controller.PciCfgBase, (UINTN) ScsSdCardHandle->Controller.MmioBase);

  ConfigureScsSdHostCapabilities (
    ScsSdCardHandle->Controller.MmioBase,
    &ScsSdCardHandle->SocConfig.Caps
    );
  ScsSdCardConfigureSdCaps (ScsSdCardHandle->Controller.MmioBase);

  if (ScsSdCardHandle->Config->UseCustomDlls) {
    SdCardDlls = &ScsSdCardHandle->Config->CustomDllValues;
  } else {
    SdCardDlls = &ScsSdCardHandle->SocConfig.DefaultDll;
  }

  ConfigureScsSdHostDll (
    (UINTN) ScsSdCardHandle->Controller.MmioBase,
    SdCardDlls
  );

  ScsControllerDisableMmio (ScsSdCardHandle->Controller.PciCfgBase);
}

/**
  Sets power gating configuration specific to SD controllers.

  @param[in] PciBaseAddress  PCI config space address
**/
STATIC
VOID
ScsSdEnablePowerGating (
  IN UINT64  PciBaseAddress
  )
{
  PciSegmentOr16 (
    PciBaseAddress + R_SCS_CFG_PG_CONFIG,
    (B_SCS_CFG_PG_CONFIG_SE| B_SCS_CFG_PG_CONFIG_PGE | B_SCS_CFG_PG_CONFIG_I3E)
    );
}

/**
  Enable eMMC controller.

  @param[in] ScsEmmcHandle  Handle.
**/
STATIC
VOID
ScsEmmcEnable (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  )
{
  DEBUG ((DEBUG_INFO, "Enabling eMMC controller\n"));

  Iosf2OcpDisableBar1 (ScsEmmcHandle->Controller.Iosf2OcpPort);
  ScsEmmcHandle->Callbacks.Bar1Disable (ScsEmmcHandle);
  Iosf2OcpUnlockEmmcDll (ScsEmmcHandle->Controller.Iosf2OcpPort);
  Iosf2OcpDisableTimeout(ScsEmmcHandle->Controller.Iosf2OcpPort);
  ScsEmmcInitMmioRegisters (
    ScsEmmcHandle
    );
  ScsEmmcHandle->Callbacks.PhyEnable (ScsEmmcHandle);
  Iosf2OcpConfigureInterrupts (
    ScsEmmcHandle->Controller.Iosf2OcpPort,
    ScsEmmcHandle->SocConfig.IntPin,
    ScsEmmcHandle->SocConfig.Irq
    );
}

/**
  Disable eMMC controller.

  @param[in] ScsEmmcHandle  Handle.
**/
STATIC
VOID
ScsEmmcDisable (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  )
{
  DEBUG ((DEBUG_INFO, "Disabling eMMC\n"));

  Iosf2OcpDisableBar1 (ScsEmmcHandle->Controller.Iosf2OcpPort);
  ScsEmmcHandle->Callbacks.Bar1Disable (ScsEmmcHandle);
  ScsSdEnablePowerGating (ScsEmmcHandle->Controller.PciCfgBase);
  ScsControllerPutToD3 (ScsEmmcHandle->Controller.PciCfgBase);
  Iosf2OcpDisableEmmc (ScsEmmcHandle->Controller.Iosf2OcpPort);
  ScsEmmcHandle->Callbacks.Disable (ScsEmmcHandle);
}

/**
  Enables or disables eMMC host controller.

  @param[in] ScsEmmcHandle  Handle.
**/
VOID
ScsEmmcInit (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  )
{
  DEBUG ((DEBUG_INFO, "SCS eMMC init start\n"));

  ScsEmmcHandle->EmmcInfo->Supported = TRUE;

  if (ScsEmmcHandle->Config->Enable) {
    ScsEmmcEnable (ScsEmmcHandle);
  } else {
    ScsEmmcDisable (ScsEmmcHandle);
  }

  DEBUG ((DEBUG_INFO, "SCS eMMC init finished\n"));
}

/**
  Enable SdCard controller.

  @param[in] ScsSdCardHandle  Handle.
**/
STATIC
VOID
ScsSdCardEnable (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  )
{
  DEBUG ((DEBUG_INFO, "Enabling SdCard\n"));

  Iosf2OcpDisableBar1 (ScsSdCardHandle->Controller.Iosf2OcpPort);
  ScsSdCardHandle->Callbacks.Bar1Disable (ScsSdCardHandle);
  Iosf2OcpSdCard1p8Enable (ScsSdCardHandle->Controller.Iosf2OcpPort, (BOOLEAN) ScsSdCardHandle->Config->PowerEnableActiveHigh);
  Iosf2OcpDisableTimeout(ScsSdCardHandle->Controller.Iosf2OcpPort);
  ScsSdCardInitMmioRegisters (
    ScsSdCardHandle
    );
  ScsSdCardHandle->Callbacks.PhyEnable (ScsSdCardHandle);
  Iosf2OcpConfigureInterrupts (
    ScsSdCardHandle->Controller.Iosf2OcpPort,
    ScsSdCardHandle->SocConfig.IntPin,
    ScsSdCardHandle->SocConfig.Irq
    );
}

/**
  Disable SdCard controller.

  @param[in] ScsSdCardHandle  Handle.
**/
STATIC
VOID
ScsSdCardDisable (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  )
{
  DEBUG ((DEBUG_INFO, "Disabling SdCard\n"));

  Iosf2OcpDisableBar1 (ScsSdCardHandle->Controller.Iosf2OcpPort);
  ScsSdCardHandle->Callbacks.Bar1Disable (ScsSdCardHandle);
  ScsSdEnablePowerGating (ScsSdCardHandle->Controller.PciCfgBase);
  ScsControllerPutToD3 (ScsSdCardHandle->Controller.PciCfgBase);
  Iosf2OcpDisableSdCard (ScsSdCardHandle->Controller.Iosf2OcpPort);
  ScsSdCardHandle->Callbacks.Disable (ScsSdCardHandle);
}

/**
  Configure SD card controller in SCS.

  @param[in] ScsSdCardHandle  Handle.
**/
VOID
ScsSdCardInit (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  )
{
  DEBUG ((DEBUG_INFO, "SCS SdCard init start\n"));
  ScsSdCardHandle->SdCardInfo->Supported = TRUE;

  if (ScsSdCardHandle->Config->Enable) {
    ScsSdCardEnable (ScsSdCardHandle);
  } else {
    ScsSdCardDisable (ScsSdCardHandle);
  }

  DEBUG ((DEBUG_INFO, "SCS SdCard init finished\n"));
}

