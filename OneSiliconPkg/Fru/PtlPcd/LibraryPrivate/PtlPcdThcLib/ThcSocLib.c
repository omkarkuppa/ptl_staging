/** @file
  Thc SoC library. Library allow to configure Thc controller
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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ThcSocLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiThcInitLib.h>
#include <Library/PeiItssLib.h>
#include <Register/PchRegs.h>
#include <Register/ThcRegs.h>
#include <Ppi/SiPolicy.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioV2AccessLib.h>
#include <GpioV2Services.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Signals.h>
#include <GpioV2Functionalities.h>
#include <Library/GpioHelpersLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>
#include <Library/PeiSiSsidLib.h>
#include <Library/HobLib.h>
#include <Library/PcdPeiThcSocInitLib.h>


/**
  Disables THC controller in power controller and fabric

  @param[in]  ThcController     Pointer to THC Controller structure
**/
STATIC
VOID
ThcControllerDisable (
  IN THC_CONTROLLER  *ThcController
  )
{
  if (ThcController == NULL) {
    DEBUG ((DEBUG_WARN, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }
  PciSegmentOr32 (ThcController->PciCfgBaseAddr + R_THC_CFG_PMD_PMCSRBSE_PMCSR, V_THC_CFG_PMD_PMCSRBSE_PMCSR_PWRST_D3);
  PciSegmentOr32 (ThcController->PciCfgBaseAddr + R_THC_CFG_UR_STS_CTL, B_THC_CFG_UR_STS_CTL_FD);
  PtlPcdPsfDisableThc (ThcController->ThcCtrlIndex);
}

#define THC_FLAGS_DUALSPI_MODE 0x4000
#define THC_FLAGS_QUADSPI_MODE 0x8000

/**
  Configures GPIO in Native Mode for THC

  @param[in] ThcIndex       THC Index
  @param[in] ThcPort        THC Port
**/
STATIC
VOID
ThcHidI2cGpioEnable (
  IN UINT32              ThcIndex,
  IN THC_PORT            *ThcPort
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

  DEBUG ((DEBUG_INFO, "ThcHidI2cGpioEnable () Entry ThcIndex - %X \n", ThcIndex));

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "THC: Failed to Configure THC Pins\n"));
    return;
  }

  Status = PtlPcdGpioEnableQuickI2c (GpioServices, ThcIndex);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't enable SCL or SDA GPIO pins for THC%d\n", __FUNCTION__, ThcIndex));
  }
  Status = PtlPcdGpioEnableThcI2cReset (GpioServices, ThcIndex, FALSE, ThcPort->Reset.ResetPad);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - cannot configure I2C reset GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
  }
  Status = PtlPcdGpioEnableThcI2cInt (GpioServices, ThcIndex, ThcPort->InterruptPinMuxing);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - cannot configure I2C interrupt GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
  }

  if (ThcPort->ThcDsyncPadEnable == 1) {
    Status = PtlPcdGpioEnableThcSpiDSync (GpioServices, ThcIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - cannot configure Display sync GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
    }
  }

  if (ThcPort->WakeOnTouch == 1) {
    Status = PtlPcdGpioConfigureThcWakeOnTouchPin (GpioServices, ThcIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - cannot configure Wake on Touch GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
    }
  }
}

/**
  Configures GPIO in Native Mode for THC

  @param[in] ThcIndex       THC Index
  @param[in] ThcPort        THC Port
**/
STATIC
VOID
ThcGpioEnable (
  IN UINT32              ThcIndex,
  IN THC_PORT            *ThcPort
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;
  UINT32             Index;
  UINT8              IoSignalNumber;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "THC: Failed to Configure THC Pins\n"));
    return;
  }

  Status = PtlPcdGpioEnableThcSpiClk (GpioServices, ThcIndex);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI clock GPIO pins for THC%d\n", __FUNCTION__, ThcIndex));
  }

  Status = PtlPcdGpioEnableThcSpiCs (GpioServices, ThcIndex);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI CS GPIO pins for THC%d\n", __FUNCTION__, ThcIndex));
  }

  Status = PtlPcdGpioEnableThcSpiReset (GpioServices, ThcIndex, FALSE, ThcPort->Reset.ResetPad);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI Reset GPIO pins for THC%d\n", __FUNCTION__, ThcIndex));
  }


  if ((ThcPort->HidOverSpi.Flags & THC_FLAGS_QUADSPI_MODE) != 0) {
    // Quad mode
    IoSignalNumber = 4;
  } else {
    // Dual mode or Single IO mode
    IoSignalNumber = 2;
  }

  DEBUG ((DEBUG_INFO, "%a - Enabling %d IO signals for THC%d\n", __FUNCTION__, IoSignalNumber, ThcIndex));
  for (Index = 0; Index < IoSignalNumber; Index++) {
    Status = PtlPcdGpioEnableThcSpiIo (GpioServices, ThcIndex, Index);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI IO pin#%d for THC%d\n", __FUNCTION__, Index, ThcIndex));
    }
  }

  Status = PtlPcdGpioSetNativePadByFunction (
    GpioServices,
    GPIOV2_SIGNAL_THC_SPI_INT (ThcIndex),
    ThcPort->InterruptPinMuxing
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI interrupt GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
  }

  Status = GpioServices->SetInputInversion (
                  GpioServices,
                  PtlPcdGpioGetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_THC_SPI_INT (ThcIndex)),
                  GpioV2InputInversionDisable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI interrupt GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_THC_CLK_LOOPBACK (ThcIndex), 0x0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - couldn't configure SPI clock loopback GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
  }

  if (ThcPort->ThcDsyncPadEnable == 1) {
    Status = PtlPcdGpioEnableThcSpiDSync (GpioServices, ThcIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - cannot configure Display sync GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
    }
  }

  if (ThcPort->WakeOnTouch == 1) {
    Status = PtlPcdGpioConfigureThcWakeOnTouchPin (GpioServices, ThcIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - cannot configure Wake on Touch GPIO pin for THC%d\n", __FUNCTION__, ThcIndex));
    }
  }
}

/**
  return THC Device ID for HID over I2C support

  @param[in] ThcIndex            THC0/THC1 0 or 1
  @retval new Device ID
**/
STATIC
UINT16
PtlPcdSocHidOverI2cDeviceId (
  IN UINT8          ThcIndex
  )
{
  switch (ThcIndex) {
    case 0:
      if (PtlIsPcdP ()) {
        return PTL_PCD_P_I2C_DEVICE_ID_1;
      } else {
        return PTL_PCD_H_I2C_DEVICE_ID_1;
      }
    default:
      if (PtlIsPcdP ()) {
        return PTL_PCD_P_I2C_DEVICE_ID_2;
      } else {
        return PTL_PCD_H_I2C_DEVICE_ID_2;
      }
  }
}

/**
  return THC Device ID for HID over SPI support

  @param[in] ThcIndex            THC0/THC1 0 or 1

  @retval new Device ID
**/
STATIC
UINT16
PtlPcdHidOverSpiDeviceId (
  IN UINT8          ThcIndex
  )
{
  switch (ThcIndex) {
    case 0:
      if (PtlIsPcdP ()) {
        return THC_PCD_P_SPI_DEVICE_ID_1;
      } else {
        return THC_PCD_H_SPI_DEVICE_ID_1;
      }
    default:
      if (PtlIsPcdP ()) {
        return THC_PCD_P_SPI_DEVICE_ID_2;
      } else {
        return THC_PCD_H_SPI_DEVICE_ID_2;
      }
  }
}

/**
  Performs Device Id update for Hid Over SPI support

  @param[in]  ThcHandle         The THC Handle instance
  @param[in]  ThcIndex          The THC Controiller index
**/
STATIC
VOID
ThcHidOverSpiEnable (
  IN THC_HANDLE  *ThcHandle,
  IN UINT8       ThcIndex
  )
{
  UINT32          Data32 = 0;
  THC_CONTROLLER  *ThcController;
  REGISTER_ACCESS *ThcPciSbAccess;

  if (ThcHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  ThcController = &ThcHandle->Controller[ThcIndex];
  ThcPciSbAccess = ThcController->ThcPciSbAccess;
  ThcHandle->PrivateConfig.HidDeviceid[ThcIndex] = PtlPcdHidOverSpiDeviceId (ThcIndex);

  Data32 = (PciSegmentRead32 (ThcController->PciCfgBaseAddr) & 0xFFFF) |
           (ThcHandle->PrivateConfig.HidDeviceid[ThcIndex] << 16);

  ThcPciSbAccess->Write32 (ThcPciSbAccess, 0, Data32);

  //
  // perform a read to avoid ordering and noncoherency problems
  //
  Data32 = ThcPciSbAccess->Read32 (ThcPciSbAccess, 0);

  DEBUG ((
    DEBUG_INFO,
    "ThcHidOverSpiEnable ThcPciSbAccess Data32  0x%X; Result DevId  0x%X \n",
    Data32,
    PciSegmentRead32 (ThcController->PciCfgBaseAddr)
    ));
}

/**
  Performs Device Id update for Hid Over I2C support

  @param[in]  ThcHandle         The THC Handle instance
  @param[in]  ThcIndex          The THC Controiller index
**/
STATIC
VOID
ThcHidOverI2cEnable (
  IN THC_HANDLE  *ThcHandle,
  IN UINT8       ThcIndex
  )
{
  UINT32          Data32 = 0;
  THC_CONTROLLER  *ThcController;
  REGISTER_ACCESS *ThcPciSbAccess;

  if (ThcHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  ThcController = &ThcHandle->Controller[ThcIndex];
  ThcPciSbAccess = ThcController->ThcPciSbAccess;
  ThcHandle->PrivateConfig.HidDeviceid[ThcIndex]  = PtlPcdSocHidOverI2cDeviceId (ThcIndex);

  Data32 = (PciSegmentRead32 (ThcController->PciCfgBaseAddr) & 0xFFFF) |
           (ThcHandle->PrivateConfig.HidDeviceid[ThcIndex] << 16);

  ThcPciSbAccess->Write32 (ThcPciSbAccess, 0, Data32);

  //
  // perform a read to avoid ordering and noncoherency problems
  //
  Data32 = ThcPciSbAccess->Read32 (ThcPciSbAccess, 0);

  DEBUG ((
    DEBUG_INFO,
    "ThcHidOverI2cEnable ThcPciSbAccess Data32  0x%X; Result DevId  0x%X \n",
    Data32,
    PciSegmentRead32 (ThcController->PciCfgBaseAddr)
    ));
}

/**
  Disables Resource control for THC

  @param[in] ThcPcrAccess       THC PCR Register Access
**/
STATIC
VOID
ThcResourceControlDisable (
  IN REGISTER_ACCESS     *ThcPcrAccess
  )
{
  ThcPcrAccess->AndThenOr32 (ThcPcrAccess, R_THC_PCR_RESOURCE_EN_CTL, ~((UINT32) V_THC_PCR_SB_RESOURCE_EN_CTL_DISABLE_0_9), 0);
}

/**
  Program THC Customized SubSystem ID only for valid ThcAssignment case

  @param[in] ThcPort          The THC Handle instance
  @param[in] SiPolicy         The SI Policy PPI instance
  @param[in] ThcIndex         THC INDEX
**/
STATIC
VOID
ThcProgramCustomizedSubSystemId (
  IN SI_POLICY_PPI       *SiPolicy,
  IN OUT THC_HANDLE      *ThcHandle,
  UINT8                  ThcIndex
  )
{
  THC_DATA_SSID_CONFIG_HOB    *ThcSsidHob;
  UINT16                      ThcSsid;
  UINT16                      ThcSvid;

  //
  // For Enable THC case only, program Sub System ID and Sub system Vendor ID
  //
  if (ThcHandle->ThcPortConfig[ThcIndex].Assignment > ThcAssignmentNone) {
    ThcSsid = (ThcHandle->ThcPortConfig[ThcIndex].Mode == ThcModeNone) ? THC_NONE_MODE_SSID
              : ThcHandle->ThcPortConfig[ThcIndex].ThcSubSystemId.CustomizedSsid;
    ThcSvid = (ThcHandle->ThcPortConfig[ThcIndex].Mode == ThcModeNone) ? DEFAULT_SSVID
              : ThcHandle->ThcPortConfig[ThcIndex].ThcSubSystemId.CustomizedSvid;

    ThcSsidHob = NULL;
    ThcSsidHob = (THC_DATA_SSID_CONFIG_HOB *) GetFirstGuidHob (&gThcSsidHobGuid);
    if (ThcSsidHob != NULL) {
      SiThcProgramSsid (SiPolicy, ThcSsidHob->ThcSsidTable, ThcIndex, ThcSvid, ThcSsid);
    }
  }
}

/**
  Update THC Handle with all Callbacks and Soc Private Config

  @param[in] ThcHandle        The THC Handle instance
  @param[in] SiPolicy         The SI Policy PPI instance
**/
STATIC
VOID
PtlPcdThcUpdateHandle (
  IN OUT THC_HANDLE             *ThcHandle,
  IN     SI_POLICY_PPI          *SiPolicy
  )
{
  UINT8  ThcIndex;
  //
  // Callbacks
  //
  ThcHandle->Callback->ThcControllerDisable      = ThcControllerDisable;
  ThcHandle->Callback->ThcGpioEnable             = ThcGpioEnable;
  ThcHandle->Callback->ThcI2cGpioEnable          = ThcHidI2cGpioEnable;
  ThcHandle->Callback->ThcHidOverSpiEnable       = ThcHidOverSpiEnable;
  ThcHandle->Callback->ThcHidOverI2cEnable       = ThcHidOverI2cEnable;
  ThcHandle->Callback->ThcResourceControlDisable = ThcResourceControlDisable;

  //
  // Private config
  //
  ThcHandle->PrivateConfig.D0i2EntryTimer           = V_THC_PCR_SB_PM_CTRL_D0I2_ENTRY_TIMER_10US;  // D0i2 Entry Timer value as 10us (0x3)
  ThcHandle->PrivateConfig.ThcResourceDisabled      = FALSE;
  ThcHandle->PrivateConfig.PgcbSourceClockFrequency = 2500000; //2.5 MHz
  ThcHandle->PrivateConfig.FrameCoalescingSupported = TRUE;

  for (ThcIndex = 0; ThcIndex < ThcHandle->ThcCount; ThcIndex++) {
    ThcHandle->PrivateConfig.InterruptPin[ThcIndex] = ItssGetDevIntPin (SiPolicy, ThcHandle->Controller[ThcIndex].Device, ThcHandle->Controller[ThcIndex].Function);
  }
  ThcHandle->Mmio = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
}

/**
  Initialize the Thc controller

  @param[in] SiPolicy                 The SI Policy PPI instance
**/
VOID
ThcSocInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  THC_HANDLE                       ThcHandle;
  EFI_STATUS                       Status;
  THC_CONFIG                       *ThcConfig;
  THC_CALLBACK                     ThcCallback;
  UINT8                            ThcIndex;
  P2SB_CONTROLLER                  P2SbCtrl;
  P2SB_PORT_16_ID                  ThcPortId[] = { { .Pid16bit = PTL_SID_F2_PID_THC0 }, { .Pid16bit = PTL_SID_F2_PID_THC1 } };

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (SiPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - invalid SiPolicy pointer provided.\n", __FUNCTION__));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThcConfigGuid, (VOID *) &ThcConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - cannot find THC config block. Aborting configuration...\n", __FUNCTION__));
    return;
  }

  ZeroMem (&ThcHandle, sizeof (THC_HANDLE));

  ThcHandle.ThcCount = GetPchMaxThcCount ();

  for (ThcIndex = 0; ThcIndex < ThcHandle.ThcCount; ThcIndex++) {
    CopyMem (&ThcHandle.ThcPortConfig[ThcIndex], &ThcConfig->ThcPort[ThcIndex], sizeof (THC_PORT));
    ThcHandle.Controller[ThcIndex].ThcCtrlIndex    = ThcIndex;
    ThcHandle.Controller[ThcIndex].PortId          = ThcPortId[ThcIndex].PortId.LocalPid;
    ThcHandle.Controller[ThcIndex].Segment         = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
    ThcHandle.Controller[ThcIndex].Bus             = DEFAULT_PCI_BUS_NUMBER_PCH;
    ThcHandle.Controller[ThcIndex].Device          = ThcDevNumber (ThcIndex);
    ThcHandle.Controller[ThcIndex].Function        = ThcFuncNumber (ThcIndex);
    ThcHandle.Controller[ThcIndex].PciCfgBaseAddr  = ThcPciCfgBase (ThcIndex);
    Status = PtlPcdGetP2SbController (&P2SbCtrl, ThcPortId[ThcIndex]);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - cannot locate P2SB controller for THC%d. Skipping.\n", __FUNCTION__, ThcIndex));
      continue;
    }
    BuildP2SbSidebandAccess (
      &P2SbCtrl,
      ThcPortId[ThcIndex].PortId.LocalPid,
      0,
      P2SbPrivateConfig,
      P2SbMmioAccess,
      TRUE,
      &ThcHandle.Controller[ThcIndex].P2SbPcrAccess
      );
    ThcHandle.Controller[ThcIndex].ThcPcrAccess = &ThcHandle.Controller[ThcIndex].P2SbPcrAccess.Access;
    BuildP2SbSidebandAccess (
      &P2SbCtrl,
      ThcPortId[ThcIndex].PortId.LocalPid,
      ((ThcHandle.Controller[ThcIndex].Device << 3) | ThcHandle.Controller[ThcIndex].Function),
      P2SbPciConfig,
      P2SbMsgAccess,
      FALSE,
      &ThcHandle.Controller[ThcIndex].P2SbPciAccess
      );
    ThcHandle.Controller[ThcIndex].ThcPciSbAccess = &ThcHandle.Controller[ThcIndex].P2SbPciAccess.Access;

    //
    // Program each THC instance with provided or default Sub system ID
    //
    ThcProgramCustomizedSubSystemId (SiPolicy, &ThcHandle, ThcIndex);
  }

  ThcHandle.Callback = &ThcCallback;
  PtlPcdThcUpdateHandle (&ThcHandle, SiPolicy);
  ThcInit (&ThcHandle);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}
