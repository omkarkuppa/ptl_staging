/** @file
  PTL SOC SerialIo Gpio Access library.
  All function in this library is available for SEC phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdGpioTopologyLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/P2SbSocLib.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Signals.h>
#include <GpioV2Pad.h>
#include <GpioV2Services.h>
#include <Register/GpioV2ChipsetId.h>
#include <SerialIoDevices.h>
#include <LpssI3cConfig.h>
#include <LpssI2cConfig.h>

/**
  This function looks for GPIO pad and its mode for a given Signal.
  Following fuction should be used in SEC phase only.

  @attention
  If the function doesn't find any gpio pad for a given signal or find more than
  one, then it returns with EFI_NOT_FOUND.

  @param[in]  Signal              Signal for which Native Pad is requested.
  @param[out] Pin Mux             GPIO Pin Mux. Please refer GpioV2PtlPcdPins.h

  @retval GPIOV2_PAD              Return Native Pad for Signal.
**/
GPIOV2_PAD
SecGetNativePadByFunction (
  IN      UINT32      Signal,
  IN      UINT32      PinMux
  )
{
  GPIOV2_SERVICES_PRIVATE    GpioInterface;
  UINT32                     CommunityIndex;
  UINT32                     GroupIndex;
  UINT32                     PadIndex;
  UINT32                     PadSignalIndex;
  UINT32                     FoundPadsCounter = 0;
  GPIOV2_PAD_MODE            PadMode = 0;
  GPIOV2_PAD                 GpioPad = 0;

  ZeroMem (&GpioInterface, sizeof (GPIOV2_SERVICES_PRIVATE));

  //
  // Only initialize required things here to get Pad_Mode and Native_Pad.
  // Remaining things can be initialized once we get above values.
    GpioInterface.ChipsetId                  = GPIOV2_PTL_PCD_CHIPSET_ID;
    GpioInterface.CommunitiesNum             = PtlPcdGpioGetCommunitiesNum ();
    GpioInterface.Communities                = PtlPcdGpioGetCommunities ();
  //
  // Iterate through all signals which are defined in *GpioV2PadData.c file
  //
  for (CommunityIndex = 0; CommunityIndex < GpioInterface.CommunitiesNum; CommunityIndex++) {
    for (GroupIndex = 0; GroupIndex < GpioInterface.Communities[CommunityIndex].GroupsNum; GroupIndex++) {
      //
      // check if PadDataArray is supported
      //
      if (GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray == NULL) {
        continue;
      }
      for (PadIndex = 0; PadIndex < GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].PadsNum; PadIndex++) {
        for (PadSignalIndex = 0; PadSignalIndex < GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignalsNum; PadSignalIndex++) {
          if (GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[PadSignalIndex].Signal.Value == Signal) {
            PadMode = GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[PadSignalIndex].PadMode;
            GpioPad = GpioInterface.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex];
            FoundPadsCounter++;
          }
        }
      }
    }
  }

  if (FoundPadsCounter == 0) {
    GpioPad = GPIOV2_PAD_NONE;
    PadMode = 0;
    return GpioPad;
  }

  if (GpioPad != GPIOV2_PAD_NONE && FoundPadsCounter == 1) {
    //
    // For situations where native signal can be enabled only on
    // a single pad then PinMux argument should be set to 0.
    //
    GpioPad = GPIOV2_PAD_SET_NATIVE_FUNCTION (GpioPad, ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK));
    ASSERT (PinMux == GPIOV2_PAD_NONE);
  } else {
    //
    // If native signal can be enabled on multiple pads
    // then PinMux argument needs to be configured to platform specific value
    GpioPad = PinMux;
  }
  return GpioPad;
}

/**
  This procedure does minimum Gpio Configuration required for Serial Io Devices
  in SEC phase.

  @param[in] GpioPad             GPIO pad
  @param[in] PadModeValue        GPIO pad mode value, please refer to GpioV2Pad.h
  @param[in] TerminationConfig   Termination configuration, please refer to GpioV2Pad.h
  @param[in] InputInversion      GpioV2InputInversionEnable or GpioV2InputInversionDisable, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Required structure not found.
**/
EFI_STATUS
EFIAPI
SecSerialIoGpioConfigure (
  IN  GPIOV2_PAD                  GpioPad,
  IN  GPIOV2_PAD_MODE             PadMode,
  IN  GPIOV2_PAD_INPUT_INVERSION  InputInversion,
  IN  GPIOV2_TERMINATION_CONFIG   TerminationConfig
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS   CommunityAccess;
  UINT32                          CommunityIndex;
  EFI_STATUS                      Status;
  UINT32                          RegisterOffset;
  UINT32                          AndValue;
  UINT32                          OrValue;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_PORT_16_ID                 Pid;

  if (GpioPad == 0) {
    ASSERT (GpioPad == GPIOV2_PAD_NONE);
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  ZeroMem (&CommunityAccess, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));
  Pid.Pid16bit = ((UINT16) (PtlPcdGpioGetCommunityPid (CommunityIndex)));
  Status = PtlPcdGetP2SbController (&P2SbController, Pid);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  BuildP2SbSidebandAccess (
    &(P2SbController),
    Pid.PortId.LocalPid,
    0,
    P2SbMemory,
    P2SbMmioAccess,
    TRUE,
    &CommunityAccess
  );

  //
  //
  //  Get DW0 Register Offset to program Pad Mode and Input Inversion
  //
  Status = PtlPcdGpioGetRegOffset (GpioPad, GpioV2Dw0Reg, &RegisterOffset);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //  Program Pad Mode
  //
  if (PadMode != GpioV2PadModeHardwareDefault) {
    AndValue = (UINT32)~(GPIOV2_PAD_MODE_MASK << GPIOV2_PAD_MODE_DW0_POS);
    OrValue = ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK) << GPIOV2_PAD_MODE_DW0_POS;

    CommunityAccess.Access.AndThenOr32(
      &(CommunityAccess.Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  //
  //  Program Input Inversion
  //
  if (InputInversion != GpioV2InputInversionHardwareDefault) {

    AndValue = (UINT32)~(GPIOV2_PAD_INPUT_INVERSION_MASK << GPIOV2_PAD_INPUT_INVERSION_DW0_POS);
    OrValue  = ((InputInversion >> 1) & GPIOV2_PAD_INPUT_INVERSION_MASK) << GPIOV2_PAD_INPUT_INVERSION_DW0_POS;

    CommunityAccess.Access.AndThenOr32 (
      &(CommunityAccess.Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  //
  //  Program Termination config
  //
  if (TerminationConfig != GpioV2TermDefault) {
    //
    //  Get DW1 Register Offset to program Pad Termination
    //
    Status = PtlPcdGpioGetRegOffset (GpioPad, GpioV2Dw1Reg, &RegisterOffset);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    AndValue = (UINT32)~(GPIOV2_PAD_TERMINATION_CONFIG_MASK << GPIOV2_PAD_TERMINATION_CONFIG_DW1_POS);
    OrValue  = ((TerminationConfig >> 1) & GPIOV2_PAD_TERMINATION_CONFIG_MASK) << GPIOV2_PAD_TERMINATION_CONFIG_DW1_POS;

    CommunityAccess.Access.AndThenOr32 (
      &(CommunityAccess.Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }
  return EFI_SUCCESS;
}

/**
  Configures GPIO for each Serial IO UART Controller

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config
**/
VOID
EFIAPI
SerialIoUartGpioEnable (
  IN UINT8                  UartNumber,
  IN SERIAL_IO_UART_CONFIG  *UartDeviceConfig
  )
{
  GPIOV2_PAD        GpioPad;

  //
  // Gpio Io Standby configuration is not required at boot stage.
  // To save space it is done as late as possible.
  //

  // RX
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_UART_RX (UartNumber), UartDeviceConfig->PinMux.Rx);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

  // TX
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_UART_TX (UartNumber), UartDeviceConfig->PinMux.Tx);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

  if (UartDeviceConfig->Attributes.AutoFlow) {
    // RTS
    GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_UART_RTS (UartNumber), UartDeviceConfig->PinMux.Rts);
    SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

    // CTS
    GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_UART_CTS (UartNumber), UartDeviceConfig->PinMux.Cts);
    SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);
  }

  return;
}

/**
  Configures GPIO for each Lpss I2C Controller

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   Lpss I2C Config
**/
VOID
EFIAPI
SerialIoI2cGpioEnable (
  IN UINT8                       I2cNumber,
  IN LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig
  )
{
  GPIOV2_PAD     GpioPad;

  //
  // Gpio Io Standby configuration is not required at boot stage.
  // To save space it is done as late as possible.
  //

  switch (I2cDeviceConfig->PadTermination) {
    case GpioV2TermDefault:
    case GpioV2TermNone:
    case GpioV2TermWpu1K:
    case GpioV2TermWpu5K:
    case GpioV2TermWpu20K:
      break;
    default:
      DEBUG ((DEBUG_WARN, "Gpio Pad Termination must be set as Default, None, None with 1v8 tolerance, 1k, 2k, 5k or 20k WPU\n"));
      ASSERT (FALSE);
  }

  // I2C SCL Pin
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_I2C_SCL (I2cNumber), I2cDeviceConfig->PinMux.Scl);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, I2cDeviceConfig->PadTermination);

  // I2C SDA Pin
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_I2C_SDA (I2cNumber), I2cDeviceConfig->PinMux.Sda);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, I2cDeviceConfig->PadTermination);
}

/**
  Configures GPIO for each Serial IO SPI Controller

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config
**/
VOID
EFIAPI
SerialIoSpiGpioEnable (
  IN UINT8                  SpiNumber,
  IN SERIAL_IO_SPI_CONFIG   *SpiDeviceConfig
  )
{
  GPIOV2_PAD    GpioPad;

  //
  // Gpio Io Standby configuration is not required at boot stage.
  // To save space it is done as late as possible.
  //

  // SPI MOSI Pin
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_SPI_MOSI (SpiNumber), SpiDeviceConfig->PinMux.Mosi);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

  // SPI MISO Pin
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_SPI_MISO (SpiNumber), SpiDeviceConfig->PinMux.Miso);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

  //SPI Clk Pin
  GpioPad = SecGetNativePadByFunction (GPIOV2_SIGNAL_LPSS_SPI_CLK (SpiNumber), SpiDeviceConfig->PinMux.Clk);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);

  //
  //  Serial Io SPI Supports only 1 Chip Select on PTL
  //
  GpioPad = SecGetNativePadByFunction ( GPIOV2_SIGNAL_LPSS_SPI_CS (SpiNumber), SpiDeviceConfig->PinMux.Cs[0]);
  SecSerialIoGpioConfigure (GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad), GpioV2InputInversionDisable, GpioV2TermDefault);
}

/**
  Configures GPIO for each Serial IO I3C Controller

  @param[in] I3cNumber         I3C Number
  @param[in] I3cDeviceConfig   SerialIo I3C Config
**/
VOID
SerialIoI3cGpioEnable (
  IN UINT8                     I3cNumber,
  IN LPSS_I3C_DEVICE_CONFIG   *I3cDeviceConfig
  )
{
  //
  // No I3C supported required in SEC phase.
  //
  return;
}
