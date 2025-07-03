/** @file
  Cnvi SoC library. Library allow to configure Cnvi controller for working on Adl platforms.
  All function in this library is available for PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <CnviConfigHob.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CnviLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiItssLib.h>
#include <Library/HobLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PmcLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiCnviInitLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/CnviRegs.h>
#include <Register/UsbHostRegs.h>
#include <Library/GpioV2AccessLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <GpioV2Pad.h>
#include <GpioV2Signals.h>
#include <Defines/PchReservedResources.h>
#include <PcdSbPortIds.h>
#include <Library/GpioHelpersLib.h>

/**
  Clear u2ub_survivability_reg1[0]
**/
VOID
PtlPcdClearXhciSurvReg1 (
  VOID
  )
{
  UINT32                XhciMmioBase;
  UINT64                XhciPciBase;

  XhciMmioBase = PCH_TEMP_BASE_ADDRESS;
  XhciPciBase = PchXhciPciCfgBase ();

  //
  // Assign memory resources
  //
  PciSegmentWrite32 (XhciPciBase + PCI_BASE_ADDRESSREG_OFFSET, XhciMmioBase);
  if (PciSegmentRead32 (XhciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (XhciPciBase + (PCI_BASE_ADDRESSREG_OFFSET + 4), (UINT32) RShiftU64 (XhciMmioBase, 32));
  }

  PciSegmentOr16 (
    XhciPciBase + PCI_COMMAND_OFFSET,
    (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  //
  // Clear u2ub_survivability_reg1[0]
  //
  MmioAnd32 (
    XhciMmioBase + R_XHCI_MEM_U2UB_SURVIVABILITY_REG1,
    (UINT32)~BIT0
    );

  //
  // Clear memory resources
  //
  PciSegmentAnd16 (
    XhciPciBase + PCI_COMMAND_OFFSET,
    (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  PciSegmentWrite32 (XhciPciBase + PCI_BASE_ADDRESSREG_OFFSET, 0);
  if (PciSegmentRead32 (XhciPciBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (XhciPciBase + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  }
}

/**
  Check if CNVi Device is supported

  @retval BOOLEAN        True if CNVi Device supported or False is not.
**/
BOOLEAN
PtlPcdCnviIsDeviceSupported (
  VOID
  )
{
  return PmcIsCnviSupported ();
}

/**
  Set CNVi Function Enable/Disable

  @param[in] State               Target CNVi Function State
**/
VOID
PtlPcdCnviSetFunction (
  IN CNVI_STATE                  State
  )
{
  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  if (State == FunctionEnable) {
    DEBUG ((DEBUG_INFO, "CNVi WiFi found disabled and requested to be enabled - clear PG bit in PMC and issue reset!\n"));
    //
    // CNVi PMC function enable
    //
    PmcEnableCnvi ();
  } else {
    //
    // Perform PSF function disable
    //
    DEBUG ((DEBUG_INFO, "Disable Cnvi Wifi and Bluetooth\n"));
    PtlPcdPsfDisableCnvi ();
    PtlPcdPsfDisableCnviBt ();

    //
    // CNVi PMC function disable
    //
    DEBUG ((DEBUG_INFO, "CNVi WiFi found enabled and requested to be disabled - set PG bit and issue reset!\n"));
    PmcStaticDisableCnvi ();
  }
}

/**
  Configure CNVi GPIO Pin Function

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
  @param[in] PinFunction         Target Pin Function
**/
VOID
PtlPcdCnviConfigGpioPin (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_PIN_FUNCTION           PinFunction
  )
{
  CNVI_CONFIG                    *CnviConfig;
  EFI_STATUS                     Status;
  GPIOV2_SERVICES                *GpioServices;

  DEBUG ((DEBUG_INFO, "%a () - Start, Pin function is %d\n", __FUNCTION__, PinFunction));

  CnviConfig = CnviHandle->Config;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GpioServices don't get successfully\n"));
    return;
  }

  switch (PinFunction) {
    case ConfigCrfPin:
      //
      // CNVi detected. Configure CNVi components.
      //
      PtlPcdGpioConfigureCnviCrfConnection (GpioServices);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_CNVI_RF_RESET, CnviConfig->PinMux.RfReset);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_CNVI_CLKREQ, CnviConfig->PinMux.Clkreq);
      break;

    case ConfigBtI2sPin:
      //
      // Configure BT I2S pins for cAVS connection
      //
      if (CnviConfig->BtAudioOffload && CnviConfig->BtAudioOffloadInterface == CnviBtAudioOffloadI2s) {
        PtlPcdGpioConfigureCnviBtI2sConnection (GpioServices, GpioV2CnviBtI2sToSsp2, 0);
        DEBUG ((DEBUG_INFO, "Configure BT I2S to SSP2\n"));
      } else {
        PtlPcdGpioConfigureCnviBtI2sConnection (GpioServices, GpioV2CnviBtI2sNotConnected, 0);
      }
      break;

    case ConfigBtDisPin:
      //
      // Configure BT related pins to not connected
      //
      PtlPcdGpioConfigureCnviBtI2sConnection (GpioServices, GpioV2CnviBtI2sNotConnected, 0);
      break;

    default:
      DEBUG ((DEBUG_INFO, "Not supported pin function\n"));
      break;
  }
}

/**
  This function sets CNVi WiFi/Bluetooth Core Enable/Disable State

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
  @param[in] State               Target CNVi Function State
**/
VOID
PtlPcdCnviWifiCore (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_STATE                  State
  )
{
  if (State == FunctionDisable) {
    DEBUG ((DEBUG_INFO, "Disable "));
    PtlPcdPsfDisableCnvi ();
  } else {
    DEBUG ((DEBUG_INFO, "Enable "));
    //
    // Do nothing
    //
  }
  DEBUG ((DEBUG_INFO, "Cnvi Wifi on PSF\n"));
}

VOID
ConfigureGpioForBtCore (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_STATE                  State
)
{
  CNVI_CONFIG                    *CnviConfig;
  EFI_STATUS                     Status;
  GPIOV2_SERVICES                *GpioServices;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return;
  }

  CnviConfig = CnviHandle->Config;
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GpioServices don't get successfully\n"));
    return;
  }

  if (State == FunctionEnable) {
    if (CnviConfig->BtInterface == CnviBtIfUsb) {
      DEBUG ((DEBUG_INFO, "Enable BT core\n"));
      PtlPcdGpioSetCnviBtEnState (GpioServices, 1);
    }
  } else {
    DEBUG ((DEBUG_INFO, "Disable BT core\n"));
    //
    // Clear BT_EN and BT_IF_SELECT to disable UTIM2UTIM bridge if CNVi is disabled.
    //
    PtlPcdGpioSetCnviBtEnState (GpioServices, 0);
    PtlPcdGpioEnableCnviBtInterface (GpioServices, GpioV2CnviBtIfPci);
  }
  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  This function sets CNVi WiFi/Bluetooth Core Enable/Disable State

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
  @param[in] State               Target CNVi Function State
**/
VOID
PtlPcdCnviBtCore (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_STATE                  State
  )
{
  CNVI_CONFIG                    *CnviConfig;

  CnviConfig = CnviHandle->Config;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  ConfigureGpioForBtCore (CnviHandle, State);

  if (State == FunctionEnable) {
    if (CnviConfig->BtInterface == CnviBtIfUsb) {
      DEBUG ((DEBUG_INFO, "Enable BT core\n"));
      PtlPcdClearXhciSurvReg1 ();
    } else {
      //
      // The selection of IOSF interface will bring the interface out of reset
      // and perform all relevant IOSF interface readiness preparations.
      // BIOS shall not need to set BT_EN.
      //
    }
  } else {
    DEBUG ((DEBUG_INFO, "Disable BT core\n"));
    PtlPcdPsfDisableCnviBt ();
  }
  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  This function sets CNVi Bluetooth Interface

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
PtlPcdCnviBtSetInterface (
  IN CNVI_HANDLE                 *CnviHandle
  )
{
  CNVI_CONFIG                    *CnviConfig;
  REGISTER_ACCESS                *CnviPcrAccess;
  UINT16                         Timeout;
  UINT32                         Data32;
  EFI_STATUS                     Status;
  GPIOV2_SERVICES                *GpioServices;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GpioServices don't get successfully\n"));
    return;
  }

  CnviConfig    = CnviHandle->Config;
  CnviPcrAccess = CnviHandle->SbAccess;

  switch (CnviConfig->BtInterface) {
    case CnviBtIfPci: // BT over PCI
      DEBUG ((DEBUG_INFO, "Enable BT Interface: PCI\n"));
      //
      // If the IOSF interface (PCIe) is selected, BIOS sets the BT_IF_SELECT register bit
      // BT_EN (vGPIO_0) shall remain at the default, zero value.
      // BT_IF_SELECT (vGPIO_5) shall set to ZERO value to disable UTIM2UTIM bridge
      // PCIe enumeration should not happen until OTP read done indication is set, for the IOSF interface selection.
      //
      CnviPcrAccess->Write32 (CnviPcrAccess, R_CNVI_PCR_BT_PCIE_IF, B_CNVI_PCR_BT_PCIE_IF_BT_PCIE_IF_SEL);
      Timeout = 10;
      do {
        MicroSecondDelay (1000);
        Data32 = CnviPcrAccess->Read32 (CnviPcrAccess, R_CNVI_PCR_BT_OTP_RD_DONE) & B_CNVI_PCR_BT_OTP_RD_DONE_BT_OTP_READ_DONE;
        if ((Data32 & B_CNVI_PCR_BT_OTP_RD_DONE_BT_OTP_READ_DONE) == B_CNVI_PCR_BT_OTP_RD_DONE_BT_OTP_READ_DONE) {
          break;
        }
        Timeout--;
      } while (Timeout > 0);
      if (Data32 == 0) {
        DEBUG ((DEBUG_INFO, "OTP_READ_DONE is not set by the hardware\n"));
      }
      PtlPcdGpioEnableCnviBtInterface (GpioServices, GpioV2CnviBtIfPci);
      break;

    case CnviBtIfUsb: // BT over USB
    default:
      DEBUG ((DEBUG_INFO, "Enable BT Interface: USB\n"));
      PtlPcdGpioEnableCnviBtInterface (GpioServices, GpioV2CnviBtIfUsb);
      break;
  }
}

/**
  This function performs CNVi Bluetooth prerequisite setting before initialization

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
VOID
PtlPcdCnviBtPreInit (
  IN CNVI_HANDLE                 *CnviHandle
  )
{
  PMC_SLEEP_STATE    SleepType;
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GpioServices don't get successfully\n"));
    return;
  }

  //
  // 1. For some steppings, the BT_EN default value is 1. It needs a 0->1 transition to enable BT core.
  //    Set BT_EN 0 here, and configure it in CnviConfigureBt ().
  // 2. Skip setting BT_EN 0 in S3/S4/S5 to prevent BT entering D3 state, reloading FW and causing the connection lost.
  //
  SleepType = PmcGetSleepTypeAfterWake ();
  if ((SleepType != PmcS3SleepState) &&
      (SleepType != PmcS4SleepState) &&
      (SleepType != PmcS5SleepState)) {
      PtlPcdGpioSetCnviBtEnState (GpioServices, 0);
  }
}

/**
  Initialize CNVi structures
  This should be done according CNVi IP integration in SoC

  @param[in]      SiPolicy        The Silicon Policy PPI instance
  @param[in, out] CnviHandle      Pointer to CNVi handle structure
  @param[in]      Config          Pointer to CNVi configuration structure
  @param[in]      PrivateConfig   Pointer to CNVi privare configuration structure
  @param[in]      Controller      Pointer to CNVi controller structure
  @param[in]      Callbacks       Pointer to CNVi callback structure
  @param[in]      P2SbCtrl        Pointer to P2SB controller
  @param[in]      P2SbAccess      Pointer to P2SB side band access
**/
VOID
PtlPcdCnviHandleInit (
  IN     SI_POLICY_PPI                  *SiPolicy,
  IN OUT CNVI_HANDLE                    *CnviHandle,
  IN     CNVI_CONFIG                    *Config,
  IN     CNVI_PRIVATE_CONFIG            *PrivateConfig,
  IN     CNVI_CONTROLLER                *Controller,
  IN     CNVI_CALLBACKS                 *Callbacks,
  IN     P2SB_CONTROLLER                *P2SbCtrl,
  IN     P2SB_SIDEBAND_REGISTER_ACCESS  *P2SbAccess
  )
{
  EFI_STATUS       Status;
  UINT8            WifiInterruptPin;
  UINT8            WifiIrq;
  P2SB_PORT_16_ID  P2SBPid;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ZeroMem (CnviHandle,    sizeof (CNVI_HANDLE));
  ZeroMem (Config,        sizeof (CNVI_CONFIG));
  ZeroMem (PrivateConfig, sizeof (CNVI_PRIVATE_CONFIG));
  ZeroMem (Controller,    sizeof (CNVI_CONTROLLER));
  ZeroMem (Callbacks,     sizeof (CNVI_CALLBACKS));
  ZeroMem (P2SbCtrl,      sizeof (P2SB_CONTROLLER));
  ZeroMem (P2SbAccess,    sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));
  WifiInterruptPin = 0;
  WifiIrq          = 0;

  //
  // Configure CNVi config block
  //
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &Config);
  ASSERT_EFI_ERROR (Status);
  CnviHandle->Config                 = Config;

  //
  // Configure CNVi private config
  //
  PrivateConfig->IsFunctionDisabled  = PmcIsCnviFunctionDisabled ();
  PrivateConfig->IsCrfModulePresent  = CnviCrfModuleIsPresent ();

  ItssGetDevIntConfig (SiPolicy, CnviWifiDevNumber (), CnviWifiFuncNumber (), &WifiInterruptPin, &WifiIrq);
  PrivateConfig->WifiInterruptPin    = WifiInterruptPin;
  PrivateConfig->WifiIrq             = WifiIrq;
  CnviHandle->PrivateConfig          = PrivateConfig;

  //
  // Configure CNVi callbacks
  //
  Callbacks->SetFunction             = PtlPcdCnviSetFunction;
  Callbacks->ConfigPinFunction       = PtlPcdCnviConfigGpioPin;
  Callbacks->WifiSetCore             = PtlPcdCnviWifiCore;
  Callbacks->BtSetCore               = PtlPcdCnviBtCore;
  Callbacks->BtSetInterface          = PtlPcdCnviBtSetInterface;
  Callbacks->BtPreInit               = PtlPcdCnviBtPreInit;
  CnviHandle->Callbacks              = Callbacks;

  //
  // Configure CNVi controller
  //
  Controller->WifiPciCfgBaseAddr     = CnviWifiPciCfgBase ();
  Controller->BtPciCfgBaseAddr       = CnviBtPciCfgBase ();
  CnviHandle->Controller             = Controller;

  //
  // Configure CNVi P2Sb access
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_CNVI;
  Status = PtlPcdGetP2SbController (P2SbCtrl, P2SBPid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  BuildP2SbSidebandAccess (
    P2SbCtrl,
    P2SBPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    P2SbAccess
    );
  CnviHandle->SbAccess               = &P2SbAccess->Access;

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}


/**
  Initialize the CNVi controller on Post-Mem phase.

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PtlPcdCnviPostMemInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  CNVI_HANDLE                      CnviHandle;
  CNVI_CONFIG                      Config;
  CNVI_PRIVATE_CONFIG              PrivateConfig;
  CNVI_CONTROLLER                  Controller;
  CNVI_CALLBACKS                   Callbacks;
  P2SB_CONTROLLER                  P2SbCtrl;
  P2SB_SIDEBAND_REGISTER_ACCESS    P2SbAccess;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (!PtlPcdCnviIsDeviceSupported ()) {
    //
    // Function Disable already set in PchEarlyDisabledDeviceHandling()
    //
    DEBUG ((DEBUG_INFO, "CNVi has been fused out\n"));
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return;
  }

  PtlPcdCnviHandleInit (SiPolicy, &CnviHandle, &Config, &PrivateConfig, &Controller, &Callbacks, &P2SbCtrl, &P2SbAccess);
  CnviInit (&CnviHandle);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Initialize the CNVi controller on Pre-Mem phase.

  @param[in] SiPreMemPolicyPpi         The SI PreMem Policy PPI instance
**/
VOID
PtlPcdCnviPreMemInit (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                       Status;
  UINT64                           MchBar;
  CNVI_PREMEM_CONFIG               *CnviPreMemConfig;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCnviPreMemConfigGuid, (VOID *) &CnviPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (CnviPreMemConfig->DdrRfim == 0) {
    MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    //
    // Program Mchbar 0x5A40 (DDR_DVFS_RFI_CONFIG_PCU_REG) RFI_DISABLE bit
    //
    MmioOr32 ((UINTN) MchBar + 0x5A40, BIT0);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}


/**
  Configure CNVi Bluetooth on Post-Mem phase.

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PtlPcdCnviConfigureBt (
  IN SI_POLICY_PPI *SiPolicy
  )
{
  CNVI_HANDLE                      CnviHandle;
  CNVI_CONFIG                      Config;
  CNVI_PRIVATE_CONFIG              PrivateConfig;
  CNVI_CONTROLLER                  Controller;
  CNVI_CALLBACKS                   Callbacks;
  P2SB_CONTROLLER                  P2SbCtrl;
  P2SB_SIDEBAND_REGISTER_ACCESS    P2SbAccess;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (!PtlPcdCnviIsDeviceSupported ()) {
    //
    // Function Disable already set in PchEarlyDisabledDeviceHandling()
    //
    DEBUG ((DEBUG_INFO, "CNVi has been fused out\n"));
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return;
  }

  PtlPcdCnviHandleInit (SiPolicy, &CnviHandle, &Config, &PrivateConfig, &Controller, &Callbacks, &P2SbCtrl, &P2SbAccess);
  CnviConfigureBt (&CnviHandle);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}
