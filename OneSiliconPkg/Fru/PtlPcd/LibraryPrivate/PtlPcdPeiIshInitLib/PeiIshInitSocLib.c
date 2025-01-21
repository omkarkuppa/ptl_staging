/** @file
  ISH Init Library - Ver6 specific functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/PcdLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Register/IshRegs.h>
#include <Library/PcdLib.h>
#include <Library/PeiIshInitLib.h>
#include <PchResetPlatformSpecific.h>
#include <GpioV2Functionalities.h>
#include <Library/PcdGpioNativeLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PchRegs.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <GpioV2Signals.h>
#include <PcdSbPortIds.h>

/**
  Set SPI GPIOs

  @param[in]  GpioServices  GPIOV2_SERVICES
  @param[in]  Spi           ISH_SPI
  @param[in]  SpiNum        UINT8
  @param[in]  CsNum         UINT8
**/
STATIC
VOID
IshSetGpioSpi (
  IN  GPIOV2_SERVICES *GpioServices,
  IN  ISH_SPI         *Spi,
  IN  UINT8           SpiNum,
  IN  UINT8           CsNum
  )
{
  UINT32   ControllerIndex;
  UINT32   CsIndex;

  for (ControllerIndex = 0; ControllerIndex < SpiNum; ControllerIndex++) {
    if (Spi[ControllerIndex].Enable) {
      DEBUG ((DEBUG_INFO, "ISH SPI_%d pins set to Native Mode\n", ControllerIndex));
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_SPI_MOSI, Spi[ControllerIndex].PinConfig.Mosi.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_SPI_MISO, Spi[ControllerIndex].PinConfig.Miso.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_SPI_CLK,  Spi[ControllerIndex].PinConfig.Clk.PinMux);
      for (CsIndex = 0; CsIndex < CsNum; CsIndex++) {
        if (Spi[ControllerIndex].CsEnable[CsIndex]) {
          PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_SPI_CS, Spi[ControllerIndex].PinConfig.Cs[CsIndex].PinMux);
        }
      }
    }
  }
}

/**
  Set UART GPIOs

  @param[in]  GpioServices  GPIOV2_SERVICES
  @param[in]  Uart          ISH_UART
  @param[in]  UartNum       UINT8
**/
STATIC
VOID
IshSetGpioUart (
  IN  GPIOV2_SERVICES *GpioServices,
  IN  ISH_UART        *Uart,
  IN  UINT8           UartNum
  )
{
  UINT32  Index;

  for (Index = 0; Index < UartNum; Index++) {
    if (Uart[Index].Enable) {
      DEBUG ((DEBUG_INFO, "ISH UART%d pins set to Native Mode\n", Index));
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_UART_RX (Index),  Uart[Index].PinConfig.Rx.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_UART_TX (Index),  Uart[Index].PinConfig.Tx.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_UART_RTS (Index), Uart[Index].PinConfig.Rts.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_UART_CTS (Index), Uart[Index].PinConfig.Cts.PinMux);
    }
  }
}

/**
  Set I2C GPIOs

  @param[in]  GpioServices  GPIOV2_SERVICES
  @param[in]  I2c           ISH_I2C
  @param[in]  I2cNum        UINT8
**/
STATIC
VOID
IshSetGpioI2c (
  IN  GPIOV2_SERVICES *GpioServices,
  IN  ISH_I2C         *I2c,
  IN  UINT8           I2cNum
  )
{
  UINT32  Index;

  for (Index = 0; Index < I2cNum; Index++) {
    if (I2c[Index].Enable) {
      DEBUG ((DEBUG_INFO, "ISH I2C%d pins set to Native Mode\n", Index));
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_I2C_SDA (Index), I2c[Index].PinConfig.Sda.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_I2C_SCL (Index), I2c[Index].PinConfig.Scl.PinMux);
    }
  }
}

/**
  Set GP GPIOs

  @param[in]  GpioServices  GPIOV2_SERVICES
  @param[in]  Gp            ISH_GP
  @param[in]  GpNum         UINT8
**/
STATIC
VOID
IshSetGpioGp (
  IN  GPIOV2_SERVICES *GpioServices,
  IN  ISH_GP          *Gp,
  IN  UINT8           GpNum
  )
{
  UINT32    Index;

  for (Index = 0; Index < GpNum; Index++) {
    if (Gp[Index].Enable) {
      DEBUG ((DEBUG_INFO, "ISH GP_%d pin set to Native Mode\n", Index));
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_GP (Index), Gp[Index].PinConfig.PinMux);
    }
  }
}

/**
  Set I3C GPIOs

  @param[in]  GpioServices  GPIOV2_SERVICES
  @param[in]  I3c           ISH_I3C
  @param[in]  I3cNum        UINT8
**/
STATIC
VOID
IshSetGpioI3c (
  IN  GPIOV2_SERVICES *GpioServices,
  IN  ISH_I3C         *I3c,
  IN  UINT8           I3cNum
  )
{
  UINT32 Index;

  for (Index = 0; Index < I3cNum; Index++) {
    if (I3c[Index].Enable) {
      DEBUG ((DEBUG_INFO, "ISH I3C%d pins set to Native Mode\n", Index));
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_I3C_SDA (Index), I3c[Index].PinConfig.Sda.PinMux);
      PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_ISH_I3C_SCL (Index), I3c[Index].PinConfig.Scl.PinMux);
    }
  }
}

STATIC
BOOLEAN
IshDisableResetForceEnable (
  VOID
  )
{
  EFI_STATUS Status;
  PCH_GENERAL_PREMEM_CONFIG   *PchGeneralPreMemConfig;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (PchGeneralPreMemConfig->DisableResets == FORCE_ENABLE) {
    DEBUG ((DEBUG_INFO, "DisableResets is enabled. Reset skipped.\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  Schedules Global Reset required to change ISH static power gating state.
**/
VOID
IshPerformGlobalReset (
  VOID
  )
{
  PCH_RESET_DATA             ResetData;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  if (IshDisableResetForceEnable ()) {
    return;
  }

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
  SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Enable MSI interrupts if enabled in Setup

  @param[in] IshHandle       ISH_HANDLE
**/
VOID
IshSocMsiInterruptsEnable (
  IN  ISH_HANDLE       *IshHandle
  )
{
  if (IshHandle->IshConfig->MsiInterrupt) {
    IshHandle->Access->SidebandMmio->And32 (
      IshHandle->Access->SidebandMmio,
      R_ISH_PCR_PCICFGCTR1,
      (UINT32) ~(B_ISH_PCR_PCICFGCTR1_DIS_MSI_CAP1)
      );
  } else {
    IshHandle->Access->SidebandMmio->AndThenOr32 (
      IshHandle->Access->SidebandMmio,
      R_ISH_PCR_PCICFGCTR1,
      (UINT32) ~0,
      B_ISH_PCR_PCICFGCTR1_DIS_MSI_CAP1
      );
  }
}

P2SB_CONTROLLER P2SbDevice = {
  PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_P2SB,
           PCI_FUNCTION_NUMBER_PCH_P2SB,
           0
           ),
  PCH_PCR_BASE_ADDRESS,
  TRUE
  };

P2SB_SIDEBAND_REGISTER_ACCESS IshMmioAccess = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID (PTL_SID_F2_PID_ISHBR),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDevice
  };

/**
  ISH Configure SOC interrupts

  @param[in] IshHandle       ISH_HANDLE
**/
VOID
IshSocConfigureInterrupts (
  IN  ISH_HANDLE      *IshHandle
  )
{
  IshConfigureInterrupts (IshHandle, IshHandle->Private.InterruptPin, IshHandle->Private.Irq);
}

/**
  Configures ISH-related GPIO pins to ISH native mode

  @param[in] IshHandle   ISH_HANDLE
 **/
VOID
IshSocSetGpio (
  IN  ISH_HANDLE   *IshHandle
  )
{
  GPIOV2_SERVICES  *GpioServices;
  EFI_STATUS       Status;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ISH: Failed to Configure ISH Pins\n"));
    return;
  }

  IshSetGpioSpi (GpioServices, IshHandle->IshConfig->Spi, GetPchMaxIshSpiControllersNum (), GetPchMaxIshSpiControllerCsPinsNum ());
  IshSetGpioUart (GpioServices, IshHandle->IshConfig->Uart, GetPchMaxIshUartControllersNum ());
  IshSetGpioI2c (GpioServices, IshHandle->IshConfig->I2c, GetPchMaxIshI2cControllersNum ());
  IshSetGpioGp (GpioServices, IshHandle->IshConfig->Gp, GetPchMaxIshGpNum ());
  IshSetGpioI3c (GpioServices, IshHandle->IshConfig->I3c, GetPchMaxIshI3cControllersNum ());

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Disable ISH BAR1 in PSF
**/
VOID
IshSocDisableDeviceBarInPsf (
  VOID
  )
{
  PtlPcdPsfDisableIshBar1 ();
}

/**
  Disabling ISH

  @param[in]  IshHandle  ISH_HANDLE
**/
VOID
IshSocDisable (
  ISH_HANDLE   *IshHandle
  )
{
  PtlPcdPsfDisableIsh ();

  //
  // Check if ISH is static power gated
  //
  if (PmcIsIshFunctionDisabled ()) {
    DEBUG ((DEBUG_INFO, "ISH found already static power gated\n"));
    return;
  }

  if (PmcStaticDisableIsh ()) {
    //
    // Perform reset for disabling to take effect
    //
    IshPerformGlobalReset ();
  } else {
    DEBUG ((DEBUG_INFO, "Can't disable ISH"));
  }
}

/**
  Function checks if ISH is PowerGated and if is, enable
  Power Management for ISH and issue reset

  @retval    TRUE       ISH Configuration should be omitted
  @retval    FALSE      ISH needs to be configured
**/
STATIC
BOOLEAN
IsIshPowerGated (
  VOID
  )
{
  //
  // Check if ISH is disabled by static PG
  //
  if (PmcIsIshFunctionDisabled ()) {
    DEBUG ((DEBUG_INFO, "ISH Static PG disabled - Enable and issue reset!\n"));

    //
    // Enable ISH controller by disabling static PG
    //
    PmcEnableIsh ();
    IshPerformGlobalReset ();

    return TRUE;
  }

  return FALSE;
}

/**
  Set ISH callback functions for given ISH_HANDLE instance
  @param[in, out] IshHandle       Pointer to ISH_HANDLE instance
  @param[in, out] IshCallbacks    Pointer to ISH_CALLBACKS instance
**/
STATIC
VOID
IshPrepareCallbacks (
  IN OUT  ISH_HANDLE       *IshHandle,
  IN OUT  ISH_CALLBACKS    *IshCallbacks
  )
{
  //
  // Populate callbacks pointers
  //
  IshCallbacks->ConfigureInterrupts = IshSocConfigureInterrupts;
  IshCallbacks->DisableDeviceBar = IshSocDisableDeviceBarInPsf;
  IshCallbacks->SetGpio = IshSocSetGpio;
  IshCallbacks->Disable = IshSocDisable;

  IshHandle->Callbacks = IshCallbacks;
}

/**
  Set PCIe and Sideband register access in given ISH_HANDLE instance
  @param[in, out] IshHandle         Pointer to ISH_HANDLE instance
  @param[in, out] IshAccess         Pointer to ISH_ACCESS instance
  @param[in, out] PcieConfigAccess  Pointer to register access to ISH PCIe config space
**/
STATIC
VOID
IshPrepareAccess (
  IN OUT  ISH_HANDLE               *IshHandle,
  IN OUT  ISH_ACCESS               *IshAccess,
  IN OUT  PCI_CONFIG_SPACE_ACCESS  *PcieConfigAccess
  )
{
  PciConfigAccessInit (
    DEFAULT_PCI_SEGMENT_NUMBER_PCH,
    DEFAULT_PCI_BUS_NUMBER_PCH,
    PCI_DEVICE_NUMBER_PCH_ISH,
    PCI_FUNCTION_NUMBER_PCH_ISH,
    PcieConfigAccess
    );

  IshAccess->PcieConfig = &PcieConfigAccess->RegAccess;
  IshAccess->SidebandMmio = &IshMmioAccess.Access;

  IshHandle->Access = IshAccess;
}

/**
  Initiates ISH_HANDLE structure

  @param[in, out] IshHandle            Abstract ISH structure instance to initialize
  @param[in, out] IshAccess            Register access structure instance
  @param[in, out] IshCallbacks         ISH_CALLBACKS instance
  @param[in, out] IshPcieConfigAccess  PCIe config space register access structure instance
  @param[in]      SiPolicy             Silicon Policy with ISH configuration data
**/
STATIC
VOID
IshHandleConstructor (
  IN OUT ISH_HANDLE               *IshHandle,
  IN OUT ISH_ACCESS               *IshAccess,
  IN OUT ISH_CALLBACKS            *IshCallbacks,
  IN OUT PCI_CONFIG_SPACE_ACCESS  *IshPcieConfigAccess,
  IN     SI_POLICY_PPI            *SiPolicy
  )
{
  EFI_STATUS  Status;

  ZeroMem (IshHandle, sizeof (ISH_HANDLE));
  ZeroMem (IshAccess, sizeof (ISH_ACCESS));
  ZeroMem (IshCallbacks, sizeof (ISH_CALLBACKS));

  IshPrepareAccess (IshHandle, IshAccess, IshPcieConfigAccess);
  IshPrepareCallbacks (IshHandle, IshCallbacks);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshHandle->IshConfig);
  ASSERT_EFI_ERROR (Status);

  ItssGetDevIntConfig (
    SiPolicy,
    IshDevNumber (),
    IshFuncNumber (),
    &IshHandle->Private.InterruptPin,
    &IshHandle->Private.Irq
    );
}

/**
  Performs PCH ISH Controller initialization

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PchIshConfigure (
  IN    SI_POLICY_PPI     *SiPolicy
  )
{
  ISH_HANDLE               IshHandle;
  ISH_ACCESS               IshAccess;
  ISH_CALLBACKS            IshCallbacks;
  PCI_CONFIG_SPACE_ACCESS  IshPcieConfigAccess;
  ISH_PREMEM_CONFIG        *IshPreMemConfig;
  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy;
  EFI_STATUS               Status;

  if (PmcIsIshSupported ()) {
    IshHandleConstructor (&IshHandle, &IshAccess, &IshCallbacks, &IshPcieConfigAccess, SiPolicy);
    IshSocMsiInterruptsEnable (&IshHandle);

    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **) &SiPreMemPolicy
               );
    ASSERT_EFI_ERROR (Status);

    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gIshPreMemConfigGuid, (VOID *) &IshPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    if (IshPreMemConfig->Enable) {
      if (IsIshPowerGated ()) {
        return;
      }
      IshConfigure (&IshHandle);
    } else {
      IshDisable (&IshHandle);
    }
  }
}