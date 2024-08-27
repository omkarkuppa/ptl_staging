/** @file
  Initializes Lpss Controllers

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

#include <Ppi/SiPolicy.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PmcLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiItssLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/LpssPciConfigControlRegs.h>
#include <PchResetPlatformSpecific.h>
#include <Library/PcdPeiLpssI2cSocInitLib.h>
#include <Library/PcdPeiLpssSpiSocInitLib.h>
#include <Library/PcdPeiLpssUartSocInitLib.h>
#include <LpssI3cConfig.h>
#include <LpssUartConfig.h>
#include <LpssI2cConfig.h>
#include <LpssSpiHandle.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/LpssInitLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/PcdSerialIoGpioAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Signals.h>
#include "PtlPcdInitPei.h"
#include <PcdSbPortIds.h>

/**
  Configures GPIO for each LPSS SPI Controller

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config
**/
VOID
EFIAPI
PtlPcdLpssSpiGpioConfigure (
  IN UINT8                    SpiNumber,
  IN LPSS_SPI_DEVICE_CONFIG   *SpiDeviceConfig
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HDA: Failed to Configure HDA Pins\n"));
    return;
  }

  //
  // Mosi
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MOSI (SpiNumber), SpiDeviceConfig->PinMux.Mosi);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MOSI (SpiNumber), SpiDeviceConfig->PinMux.Mosi), GpioV2InputInversionDisable);

  //
  // Miso
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MISO (SpiNumber), SpiDeviceConfig->PinMux.Miso);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_MISO (SpiNumber), SpiDeviceConfig->PinMux.Miso), GpioV2InputInversionDisable);

  //
  // Clk
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CLK (SpiNumber), SpiDeviceConfig->PinMux.Clk);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CLK (SpiNumber), SpiDeviceConfig->PinMux.Clk), GpioV2InputInversionDisable);

  //
  //  Lpss SPI on PTL Supports only 1 Chip Selects
  //
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CS (SpiNumber), SpiDeviceConfig->PinMux.Cs[0]);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_SPI_CS (SpiNumber), SpiDeviceConfig->PinMux.Cs[0]), GpioV2InputInversionDisable);
}

/**
  Configures GPIO for each LPSS UART Controller

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   LPSS UART Config
**/
VOID
EFIAPI
PtlPcdLpssUartGpioConfigure (
  IN UINT8                         UartNumber,
  IN LPSS_UART_DEVICE_CONFIG       *UartDeviceConfig
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to Configure Lpss Uart pins.\n"));
    return;
  }

  // RX
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RX (UartNumber), UartDeviceConfig->PinMux.Rx);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RX (UartNumber), UartDeviceConfig->PinMux.Rx), GpioV2InputInversionDisable);

  // TX
  PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_TX (UartNumber), UartDeviceConfig->PinMux.Tx);
  GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_TX (UartNumber), UartDeviceConfig->PinMux.Tx), GpioV2InputInversionDisable);

  if (UartDeviceConfig->Attributes.AutoFlow) {
    // RTS
    PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RTS (UartNumber), UartDeviceConfig->PinMux.Rts);
    GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_RTS (UartNumber), UartDeviceConfig->PinMux.Rts), GpioV2InputInversionDisable);
    // CTS
    PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_LPSS_UART_CTS (UartNumber), UartDeviceConfig->PinMux.Cts);
    GpioServices->SetInputInversion (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_LPSS_UART_CTS (UartNumber), UartDeviceConfig->PinMux.Cts), GpioV2InputInversionDisable);
  }

  // //
  // // Uart Gpio Io Standby configuration is not required at boot stage.
  // // To save spece it is done as late as possible.
  // //
  // if ((LpssUartDeviceConfig->Mode != LpssUartDisabled) && (LpssUartCallback->LpssUartGpioIoStandbyConfigure != NULL)) {
  //   LpssUartCallback->LpssUartGpioIoStandbyConfigure (LpssUartController->UartCtrlIndex, LpssUartDeviceConfig);
  // }
}

/**
  Add element and the en of the list

  @param ListHead    LPSS devices list head
  @return            Pointer to added device
 */
STATIC
LPSS_DEVICE *
LpssAddTailList (
  LPSS_DEVICE  **ListHead
  )
{
  LPSS_DEVICE  *ListPtr;
  LPSS_DEVICE  *LpssDev;

  LpssDev = AllocateZeroPool (sizeof (LPSS_DEVICE));
  if (LpssDev == NULL) {
    return NULL;
  }

  if (*ListHead == NULL) {
    *ListHead = LpssDev;
    return LpssDev;
  }

  ListPtr = *ListHead;
  while (ListPtr->Next != NULL) {
    ListPtr = ListPtr->Next;
  }

  ListPtr->Next = LpssDev;

  return LpssDev;
}

#define MAX_LPSS_DEV_NAME 30

CHAR8 *
LpssGetDevName (
  const CHAR8  *DevName,
  UINT8         Index,
  BOOLEAN       DoubleSlice
  )
{
  CHAR8  *Name;

  Name = AllocateZeroPool (MAX_LPSS_DEV_NAME);
  if (Name == NULL) {
    return NULL;
  }

  if (DoubleSlice == TRUE) {
    AsciiSPrint (Name, MAX_LPSS_DEV_NAME, "LPSS %a Slice %d/%d", DevName, Index, Index+1);
  } else {
    AsciiSPrint (Name, MAX_LPSS_DEV_NAME, "LPSS %a %d", DevName, Index);
  }

  return Name;
}

/**
  Creates LPSS Subsystem structure
  - Allocates memory for the stuctures
  - Creates PCR Registers Access protocol
  - Initializes devices list as empty (NULL)

  @param[in]  LpssSubsystem         LPSS Subsystem instance

  @retval     EFI_SUCCESS           Completed successfully
              EFI_OUT_OF_RESOURCES  Memory allocation failed
**/
EFI_STATUS
GetLpssSubsystemInstance (
  LPSS_SUBSYSTEM  *LpssSubsystem
  )
{
  P2SB_CONTROLLER               *P2SbCtrl;
  P2SB_SIDEBAND_REGISTER_ACCESS *PcrAccess;
  P2SB_PORT_16_ID               P2SBPid;

  P2SbCtrl = AllocateZeroPool (sizeof (P2SB_CONTROLLER));
  PcrAccess = AllocateZeroPool (sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));

  if ((P2SbCtrl == NULL) || (PcrAccess == NULL)) {
    DEBUG ((DEBUG_WARN, "%a: ERROR: Memory Allocation failed for LPSS Subsystem!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // PSS Subsystem - Register Access - PCR
  P2SBPid.Pid16bit = PTL_SID_F2_PID_LPSS;
  PtlPcdGetP2SbController (P2SbCtrl, P2SBPid);
  BuildP2SbSidebandAccess (
    P2SbCtrl,
    P2SBPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    TRUE,
    PcrAccess
    );
  LpssSubsystem->PcrAccess = (REGISTER_ACCESS *)PcrAccess;

  LpssSubsystem->Devices = NULL;

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mI2cPciCfgCtrOffset [] = {
  R_LPSS_PCR_PCICFGCTR1,  // PCICFGCTRL_I2C0
  R_LPSS_PCR_PCICFGCTR2,  // PCICFGCTRL_I2C1
  R_LPSS_PCR_PCICFGCTR3,  // PCICFGCTRL_I2C2
  R_LPSS_PCR_PCICFGCTR4,  // PCICFGCTRL_I2C3
  R_LPSS_PCR_PCICFGCTR5,  // PCICFGCTRL_I2C4
  R_LPSS_PCR_PCICFGCTR6   // PCICFGCTRL_I2C5
};

/**
  Adds I2C devices to LPSS Subsystem device list

  @param[in] SiPolicy        The SiPolicy instance
  @param[in] LpssI2cConfig   I2C Config Block
  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
VOID
STATIC
AddI2cDevices (
  IN SI_POLICY_PPI    *SiPolicy,
  LPSS_I2C_CONFIG     *LpssI2cConfig,
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  )
{
  UINT8         Index;
  LPSS_DEVICE   *LpssDev;

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    ASSERT (Index < ARRAY_SIZE (mI2cPciCfgCtrOffset));
    LpssDev = LpssAddTailList (&LpssSubsystem->Devices);
    LpssDev->Name = LpssGetDevName ("I2C", Index, FALSE);
    LpssDev->InterfaceIndex = Index;
    LpssDev->PciCfgCtrlOffset = mI2cPciCfgCtrOffset[Index];
    LpssDev->PciDev = LpssI2cDevNumber (Index);
    LpssDev->PciFun = LpssI2cFuncNumber (Index);
    LpssDev->PsfDisable = PtlPcdPsfDisableLpssI2c;
    LpssDev->PsfPciCfgHide = PtlPcdPsfHideLpssI2c;
    LpssDev->PsfBar1Enable = NULL;
    switch (LpssI2cConfig->I2cDeviceConfig[Index].Mode) {
      case LpssI2cPci:
        LpssDev->Mode = ModePci;
        break;
      case LpssI2cDisabled:
      default:
        LpssDev->Mode = ModeDisabled;
        break;
    }
    ItssGetDevIntConfig (
                    SiPolicy,
                    LpssI2cDevNumber (Index),
                    LpssI2cFuncNumber (Index),
                    &LpssDev->InterruptPin,
                    &LpssDev->Irq);
  }
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mI3cPciCfgCtrOffset[] = {
  R_LPSS_PCR_PCICFGCTR13,  // Slice I3C0/I3C1
  R_LPSS_PCR_PCICFGCTR14   // Slice I3C2/I3C3
};

/**
  Adds I3C devices to LPSS Subsystem device list

  @param[in] SiPolicy        The SiPolicy instance
  @param[in] LpssI3cConfig   I3C Config Block
  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
STATIC
VOID
AddI3cDevices (
  IN SI_POLICY_PPI    *SiPolicy,
  LPSS_I3C_CONFIG     *LpssI3cConfig,
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  )
{
  UINT8         Index;
  LPSS_DEVICE   *LpssDev;

  //
  // Note:
  // I3C devices are groupped two per Slice
  // so we have followig configuration:
  // Slice 0: I3C0 and I3C1
  // Slice 1: I3C1 and I3C2
  // Be carefull when iterating in below loop
  for (Index = 0; Index < GetMaxI3cInterfacesNum ();) {
    ASSERT (Index/2 < ARRAY_SIZE (mI3cPciCfgCtrOffset));
    LpssDev = LpssAddTailList (&LpssSubsystem->Devices);
    LpssDev->Name = LpssGetDevName ("I3C", Index, TRUE);
    LpssDev->InterfaceIndex = Index;
    LpssDev->PciCfgCtrlOffset = mI3cPciCfgCtrOffset[Index/2];
    LpssDev->PciDev = LpssI3cDevNumber (Index);
    LpssDev->PciFun = LpssI3cFuncNumber (Index);
    LpssDev->PsfDisable = PtlPcdPsfDisableLpssI3c;
    LpssDev->PsfPciCfgHide = PtlPcdPsfHideLpssI3c;
    LpssDev->PsfBar1Enable = NULL;
    switch (LpssI3cConfig->I3cDeviceConfig[Index].Mode) {
      case I3cPci:
        LpssDev->Mode = ModePci;
        break;
      case I3cPhantom:
        switch (LpssI3cConfig->I3cDeviceConfig[Index-1].Mode) {
          case I3cPci:
            LpssDev->Mode = ModePci;
            break;
          case I3cDisabled:
          default:
            LpssDev->Mode = ModeDisabled;
            break;
        }
        break;
      case I3cDisabled:
      default:
        LpssDev->Mode = ModeDisabled;
        break;
    }
    ItssGetDevIntConfig (
                    SiPolicy,
                    LpssI3cDevNumber (Index),
                    LpssI3cFuncNumber (Index),
                    &LpssDev->InterruptPin,
                    &LpssDev->Irq);
    Index += 2;
  }

  LpssSubsystem->Flags = LpssFlagForceClockOn;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mSpiPciCfgCtrOffset [] = {
  R_LPSS_PCR_PCICFGCTR10,  // PCICFGCTRL_SPI0
  R_LPSS_PCR_PCICFGCTR11,  // PCICFGCTRL_SPI1
  R_LPSS_PCR_PCICFGCTR12,  // PCICFGCTRL_SPI2
};

/**
  Adds SPI devices to LPSS Subsystem device list

  @param[in] SiPolicy        The SiPolicy instance
  @param[in] LpssSpiConfig   SPI Config Block
  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
STATIC
VOID
AddSpiDevices (
  IN SI_POLICY_PPI    *SiPolicy,
  LPSS_SPI_CONFIG     *LpssSpiConfig,
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  )
{
  UINT8         Index;
  LPSS_DEVICE   *LpssDev;

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    ASSERT (Index < ARRAY_SIZE (mSpiPciCfgCtrOffset));
    LpssDev = LpssAddTailList (&LpssSubsystem->Devices);
    LpssDev->Name = LpssGetDevName ("SPI", Index, FALSE);
    LpssDev->InterfaceIndex = Index;
    LpssDev->PciCfgCtrlOffset = mSpiPciCfgCtrOffset[Index];
    LpssDev->PciDev = LpssSpiDevNumber (Index);
    LpssDev->PciFun = LpssSpiFuncNumber (Index);
    LpssDev->PsfDisable = PtlPcdPsfDisableLpssSpi;
    LpssDev->PsfPciCfgHide = PtlPcdPsfHideLpssSpi;
    LpssDev->PsfBar1Enable = NULL;
    switch (LpssSpiConfig->SpiDeviceConfig[Index].Mode) {
      case LpssSpiPci:
        LpssDev->Mode = ModePci;
        break;
      case LpssSpiDisabled:
      default:
        LpssDev->Mode = ModeDisabled;
        break;
    }
    ItssGetDevIntConfig (
                    SiPolicy,
                    LpssSpiDevNumber (Index),
                    LpssSpiFuncNumber (Index),
                    &LpssDev->InterruptPin,
                    &LpssDev->Irq);
  }
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mUartPciCfgCtrOffset [] = {
  R_LPSS_PCR_PCICFGCTR7,  // PCICFGCTRL_UART0
  R_LPSS_PCR_PCICFGCTR8,  // PCICFGCTRL_UART1
  R_LPSS_PCR_PCICFGCTR9   // PCICFGCTRL_UART2
};

/**
  Adds UART devices to LPSS Subsystem device list

  @param[in] SiPolicy              The SiPolicy instance
  @param[in] LpssUartConfig  UART Config Block
  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
STATIC
VOID
AddUartDevices (
  IN SI_POLICY_PPI    *SiPolicy,
  LPSS_UART_CONFIG    *LpssUartConfig,
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  )
{
  UINT8         Index;
  LPSS_DEVICE   *LpssDev;

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    ASSERT (Index < ARRAY_SIZE (mUartPciCfgCtrOffset));
    LpssDev = LpssAddTailList (&LpssSubsystem->Devices);
    LpssDev->Name = LpssGetDevName ("UART", Index, FALSE);
    LpssDev->InterfaceIndex = Index;
    LpssDev->PciCfgCtrlOffset = mUartPciCfgCtrOffset[Index];
    LpssDev->PciDev = LpssUartDevNumber (Index);
    LpssDev->PciFun = LpssUartFuncNumber (Index);
    LpssDev->PsfDisable = PtlPcdPsfDisableLpssUart;
    LpssDev->PsfPciCfgHide = PtlPcdPsfHideLpssUart;
    LpssDev->PsfBar1Enable = PtlPcdPsfEnableLpssUartBar1;
    switch (LpssUartConfig->UartDeviceConfig[Index].Mode) {
      case LpssUartPci:
        LpssDev->Mode = ModePci;
        break;
      case LpssUartHidden:
        LpssDev->Mode = ModeAcpi;
        break;
      case LpssUartCom:
        LpssDev->Mode = ModeCom;
        break;
      case LpssUartSkipInit:
        LpssDev->Mode = ModeSkipInit;
        break;
      case LpssUartDisabled:
      default:
        LpssDev->Mode = ModeDisabled;
        break;
    }
    ItssGetDevIntConfig (
                    SiPolicy,
                    LpssUartDevNumber (Index),
                    LpssUartFuncNumber (Index),
                    &LpssDev->InterruptPin,
                    &LpssDev->Irq);
  }
}

/**
  Configures GPIO ping for I2C devices

  @param[in] LpssI2cConfig   I2C Config Block
**/
STATIC
VOID
GpioI2cInit (
  LPSS_I2C_CONFIG    *LpssI2cConfig
  )
{
  UINT8                       Index;
  LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    I2cDeviceConfig = &LpssI2cConfig->I2cDeviceConfig[Index];
    if (I2cDeviceConfig->Mode != LpssI2cDisabled) {
      SerialIoI2cGpioEnable (Index, I2cDeviceConfig);
    }
  }
}

/**
  Configures GPIO ping for I3C devices

  @param[in] LpssI3cConfig   I3C Config Block
**/
STATIC
VOID
GpioI3cInit (
  LPSS_I3C_CONFIG    *LpssI3cConfig
  )
{
  UINT8                   Index;
  LPSS_I3C_DEVICE_CONFIG  *I3cDeviceConfig;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    I3cDeviceConfig = &LpssI3cConfig->I3cDeviceConfig[Index];
    if (I3cDeviceConfig->Mode != I3cDisabled) {
      SerialIoI3cGpioEnable (Index, I3cDeviceConfig);
    }
  }
}

/**
  Configures GPIO ping for SPI devices

  @param[in] LpssSpiConfig   SPI Config Block
**/
STATIC
VOID
GpioSpiInit (
  LPSS_SPI_CONFIG    *LpssSpiConfig
  )
{
  UINT8                   Index;
  LPSS_SPI_DEVICE_CONFIG  *SpiDeviceConfig;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    SpiDeviceConfig = &LpssSpiConfig->SpiDeviceConfig[Index];
    if (SpiDeviceConfig->Mode != LpssSpiDisabled) {
      PtlPcdLpssSpiGpioConfigure (Index, SpiDeviceConfig);
    }
  }
}

/**
  Configures GPIO ping for UART devices

  @param[in] LpssUartConfig   UART Config Block
**/
STATIC
VOID
GpioUartInit (
  LPSS_UART_CONFIG    *LpssUartConfig
  )
{
  UINT8                            Index;
  LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    UartDeviceConfig = &LpssUartConfig->UartDeviceConfig[Index];
    if (UartDeviceConfig->Mode != LpssUartDisabled) {
      PtlPcdLpssUartGpioConfigure (Index, UartDeviceConfig);
    }
  }
}

/**
  Gather all the date required for LPSS initialization

  @param[in] SiPolicy         The SiPolicy instance
  @param[in] LpssSubsystem    Subsystem structure
  @param[in] LpssUartConfig   UART Bconfig Block
  @param[in] LpssSpiConfig    SPI Config Block
  @param[in] LpssI3cConfig    I3C Config Block
  @param[in] LpssI2cConfig    I2C Config Block
**/
VOID
CreateLpssStructAndDevList (
  SI_POLICY_PPI     *SiPolicy,
  LPSS_SUBSYSTEM    *LpssSubsystem,
  LPSS_UART_CONFIG  **LpssUartConfig,
  LPSS_SPI_CONFIG   **LpssSpiConfig,
  LPSS_I3C_CONFIG   **LpssI3cConfig,
  LPSS_I2C_CONFIG   **LpssI2cConfig
  )
{
  EFI_STATUS    Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssUartConfigGuid, (VOID *) LpssUartConfig);
  if (EFI_ERROR ((Status))) {
    DEBUG ((DEBUG_ERROR, "ERROR: Cannot get LpssUart config block! (Status: %r)\n", Status));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssSpiConfigGuid, (VOID *) LpssSpiConfig);
  if (EFI_ERROR ((Status))) {
    DEBUG ((DEBUG_ERROR, "ERROR: Cannot get LpssSpi config block! (Status: %r)\n", Status));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI3cConfigGuid, (VOID *) LpssI3cConfig);
  if (EFI_ERROR ((Status))) {
    DEBUG ((DEBUG_ERROR, "ERROR: Cannot get LpssI3c config block! (Status: %r)\n", Status));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI2cConfigGuid, (VOID *) LpssI2cConfig);
  if (EFI_ERROR ((Status))) {
    DEBUG ((DEBUG_ERROR, "ERROR: Cannot get LpssI2c config block! (Status: %r)\n", Status));
    return;
  }

  // Create Subsystem structure
  Status = GetLpssSubsystemInstance (LpssSubsystem);
  if (EFI_ERROR ((Status))) {
    DEBUG ((DEBUG_ERROR, "ERROR: Cannot get Lpss Instance structure! (Status: %r)\n", Status));
    return;
  }

  // Create devices list integrated in the subsystem
  AddI2cDevices (SiPolicy, *LpssI2cConfig, LpssSubsystem);
  AddI3cDevices (SiPolicy, *LpssI3cConfig, LpssSubsystem);
  AddSpiDevices (SiPolicy, *LpssSpiConfig, LpssSubsystem);
  AddUartDevices (SiPolicy, *LpssUartConfig, LpssSubsystem);
}

/**
  Configures LPSS Controllers

  @param[in] SiPolicy              The SiPolicy instance
**/
VOID
PtlPcdLpssInit (
  IN SI_POLICY_PPI                 *SiPolicy
  )
{
  LPSS_SUBSYSTEM    LpssSubsystem;
  LPSS_UART_CONFIG  *LpssUartConfig;
  LPSS_SPI_CONFIG   *LpssSpiConfig;
  LPSS_I3C_CONFIG   *LpssI3cConfig;
  LPSS_I2C_CONFIG   *LpssI2cConfig;
  UINT8             Index;
  PCH_RESET_DATA    ResetData;
  BOOLEAN           IsLpssDisabled;
  BOOLEAN           DisableLpss;

  DEBUG ((DEBUG_INFO, "%a - Start.\n", __FUNCTION__));

  if (!PmcIsSerialIoSupported (PmcGetPwrmBase ())) {

    DEBUG((DEBUG_INFO, "LPSS disabled in straps\n"));

    if (!PmcIsSerialIoStaticallyDisabled (PmcGetPwrmBase ())) {
      //
      // If Lpss not statically disabled in PMC, set the function disable bit
      //
      PmcStaticDisableSerialIo (PmcGetPwrmBase ());
    }
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return;
  }

  CreateLpssStructAndDevList (SiPolicy, &LpssSubsystem, &LpssUartConfig, &LpssSpiConfig, &LpssI3cConfig, &LpssI2cConfig);

  IsLpssDisabled = PmcIsSerialIoStaticallyDisabled (PmcGetPwrmBase ());

  if (!IsLpssDisabled) {
    // GPIO configurtation for all LPSS Sub-IPs
    GpioI2cInit (LpssI2cConfig);
    GpioI3cInit (LpssI3cConfig);
    GpioSpiInit (LpssSpiConfig);
    GpioUartInit (LpssUartConfig);

    // Initialization at IP layer (all Sub-IPs integrated in LPSS)
    PtlPcdLpssI2cSocInit (SiPolicy);
    PtlPcdLpssSpiSocInit (SiPolicy);
    PtlPcdLpssUartSocInit (SiPolicy);
    PtlPcdLpssI3cSocInit (SiPolicy);

    // Initialization at Subsystem layer
    LpssInit (&LpssSubsystem);
  }

  //
  // Check if all Lpss controllers should be disabled in PMC
  //
  DisableLpss = TRUE;

  for (Index = 0; Index < GetMaxSpiInterfacesNum (); Index++) {
    if (LpssSpiConfig->SpiDeviceConfig[Index].Mode != LpssSpiDisabled) {
      DisableLpss = FALSE;
    }
  }

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    if (LpssI3cConfig->I3cDeviceConfig[Index].Mode != I3cDisabled) {
      DisableLpss = FALSE;
    }
  }

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    if (LpssI2cConfig->I2cDeviceConfig[Index].Mode != LpssI2cDisabled) {
      DisableLpss = FALSE;
    }
  }

  for (Index = 0; Index < GetMaxUartInterfacesNum (); Index++) {
    if (LpssUartConfig->UartDeviceConfig[Index].Mode != LpssUartDisabled) {
      DisableLpss = FALSE;
    }
  }

  if (DisableLpss && !IsLpssDisabled) {
    //
    // If all Lpss controllers are disabled do static power gating in PMC
    //
    PmcStaticDisableSerialIo (PmcGetPwrmBase ());
  } else if (!DisableLpss && IsLpssDisabled) {
    //
    // If at least one Lpss controller is to be used remove static power gating in PMC
    //
    PmcEnableSerialIo (PmcGetPwrmBase ());
  }

  //
  // Trigger reset if Lpss static power gating state has to be changed
  //
  if (IsLpssDisabled != DisableLpss) {
    DEBUG ((DEBUG_INFO, "%a () - Reset due to changes in Lpss FunctionDisable.\n", __FUNCTION__));
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);
  }

  DEBUG ((DEBUG_INFO, "%a - End.\n", __FUNCTION__));
}

/**
  Configures LPSS Function 0 Disabled Controllers
  Note:
    This function shall be executed after all other PCH devices (non LPSS) are configured
    because non LPSS devices may share device PCI number with function zero of LPSS device

  @param[in] SiPolicy              The SiPolicy instance
**/
VOID
PtlPcdLpssFunction0Disable (
  IN SI_POLICY_PPI                 *SiPolicy
  )
{
  LPSS_SUBSYSTEM    LpssSubsystem;
  LPSS_UART_CONFIG  *LpssUartConfig;
  LPSS_SPI_CONFIG   *LpssSpiConfig;
  LPSS_I3C_CONFIG   *LpssI3cConfig;
  LPSS_I2C_CONFIG   *LpssI2cConfig;

  DEBUG ((DEBUG_INFO, "%a - Start\n", __FUNCTION__));

  CreateLpssStructAndDevList (SiPolicy, &LpssSubsystem, &LpssUartConfig, &LpssSpiConfig, &LpssI3cConfig, &LpssI2cConfig);

  LpssFabricFunctionDisable (&LpssSubsystem, DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
}
