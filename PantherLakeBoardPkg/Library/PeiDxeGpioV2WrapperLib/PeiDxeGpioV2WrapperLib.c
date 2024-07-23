/** @file
  A library instance for PEI/DXE phase GpioV2WrapperLib

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
#include <Uefi/UefiBaseType.h>
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/GpioV2AccessLib.h>

#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioV2PcdPins.h>
#include <Register/GpioAcpiDefines.h>

EFI_STATUS
GetGpioV2ServicesFromPad (
  IN  GPIOV2_PAD                GpioPad,
  OUT GPIOV2_SERVICES           **GpioServices
  )
{
  EFI_STATUS              Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, GpioServices);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    DEBUG((DEBUG_ERROR, "Failed to find Gpio Services for GpioPad 0x%x\n", GpioPad));
  }

  return Status;
}
/**
  This procedure will set GPIO mode

  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. PTL, EBG, ...)
  @param[in] PadModeValue         GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetPadMode (
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_MODE             PadMode
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetPadMode (GpioServices, GpioPad, PadMode);
}

/**
  This procedure reads current GPIO Pad Mode

  @param[in] GpioPad             GPIO Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. PTL, EBG, ...)
  @param[in] PadMode             Pointer to a buffer for GPIO Pad Mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetPadMode (
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_MODE             *PadMode
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetPadMode (GpioServices, GpioPad, PadMode);
}

/**
  This procedure will set GPIO enable or disable input inversion on rquested pad

  @param[in] GpioPad             GPIO pad
  @param[in] InputInversion      GpioV2InputInversionEnable or GpioV2InputInversionDisable, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetInputInversion (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_INPUT_INVERSION   InputInversion
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetInputInversion (GpioServices, GpioPad, InputInversion);
}

/**
  This procedure will Get GPIO enable or disable input inversion on rquested pad

  @param[in]  GpioPad             GPIO pad
  @param[Out] Inverted            Enabled / Disabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetInputInversion (
  IN  GPIOV2_PAD                  GpioPad,
  OUT BOOLEAN                     *Inverted
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetInputInversion (GpioServices, GpioPad, Inverted);
}

/**
  This procedure will set GPIO Lock (register PADCFGLOCK)

  @param[in] GpioPad             GPIO pad
  @param[in] Lock                GpioV2Unlock - Unlock pad, GpioV2Lock - Lock pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetLock (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_LOCK              Lock
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetLock (GpioServices, GpioPad, Lock);
}

/**
  This procedure will set GPIO Lock Tx (register PADCFGLOCKTX)

  @param[in] GpioPad             GPIO pad
  @param[in] LockTx              GpioV2Unlock - Unlock output state of Gpio Pad, GpioV2Lock - Lock output state of Gpio Pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetLockTx (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_LOCK              LockTx
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetLockTx (GpioServices, GpioPad, LockTx);
}

/**
  This procedure sets Gpio Pad output state

  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         GpioV2StateLow - output state low, GpioV2StateHigh - output state high

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetTx (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             OutputState
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetTx (GpioServices, GpioPad, OutputState);
}

/**
  This procedure reads current Gpio Pad output state

  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         Pointer to a buffer for output state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetTx (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             *OutputState
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetTx (GpioServices, GpioPad, OutputState);
}

/**
  This procedure reads current Gpio Pad input state

  @param[in] GpioPad             GPIO pad
  @param[in] InputState          Pointer to a buffer for input state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetRx (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             *InputState
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetRx (GpioServices, GpioPad, InputState);
}

/**
  This procedure reads if TX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetTxDisable (
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      *TxDisabled
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetTxDisable (GpioServices, GpioPad, TxDisabled);
}

/**
  This procedure sets TX buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetTxDisable (
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      TxDisabled
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetTxDisable (GpioServices, GpioPad, TxDisabled);
}

/**
  This procedure reads if RX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioPad             GPIO pad
  @param[in] RxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetRxDisable (
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      *RxDisabled
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetRxDisable (GpioServices, GpioPad, RxDisabled);
}

/**
  This procedure sets Rx buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRxDisable (
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      RxDisabled
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRxDisable (GpioServices, GpioPad, RxDisabled);
}

/**
  This procedure sets Reset Configuration for requested Gpio Pad

  @param[in] GpioPad             GPIO pad
  @param[in] ResetConfig         Reset Configuration - please refer to GpioV2Pad.h (GPIOV2_RESET_CONFIG)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetResetConfig (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RESET_CONFIG          ResetConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetResetConfig (GpioServices, GpioPad, ResetConfig);
}

/**
  This procedure reads current Reset Configuration for requested Gpio Pad

  @param[in] GpioPad             GPIO pad
  @param[in] ResetConfig         Pointer to a buffer for Reset Configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetResetConfig (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RESET_CONFIG          *ResetConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetResetConfig (GpioServices, GpioPad, ResetConfig);
}

/**
  This procedure sets termination configuration for requested Gpio Pad

  @param[in] GpioPad             GPIO pad
  @param[in] TerminationConfig   Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetTerminationConfig (
  IN GPIOV2_PAD                    GpioPad,
  IN GPIOV2_TERMINATION_CONFIG     ElectricalConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetTerminationConfig (GpioServices, GpioPad, ElectricalConfig);
}

/**
  This procedure reads current termination configuration for requested Gpio Pad

  @param[in] GpioPad             GPIO pad
  @param[in] TerminationConfig   Pointer to a buffer for Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetTerminationConfig (
  IN GPIOV2_PAD                    GpioPad,
  IN GPIOV2_TERMINATION_CONFIG     *ElectricalConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetTerminationConfig (GpioServices, GpioPad, ElectricalConfig);
}

/**
  This procedure sets IO Standby configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetIostandbyConfig (
  IN GPIOV2_PAD                     GpioPad,
  IN GPIOV2_IOSTANDBY_STATE         IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM          IosTerminationConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetIostandbyConfig (GpioServices, GpioPad, IosStateConfig, IosTerminationConfig);
}

/**
  This procedure reads current IO Standby configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       Pointer to a buffer for IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig Pointer to a buffer for IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetIostandbyConfig (
  IN GPIOV2_PAD                     GpioPad,
  IN GPIOV2_IOSTANDBY_STATE         *IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM          *IosTerminationConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetIostandbyConfig (GpioServices, GpioPad, IosStateConfig, IosTerminationConfig);
}

/**
  This procedure enables debounce feature on a selected pad configured in input mode
  Debounce time can be specified in microseconds. GPIO HW supports only certain values
  according to below formula:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by GPIO HW, function will round down
  to closest supported

  @param[in] GpioPad              GPIO pad
  @param[in] DebounceTime         Debounce Time in microseconds
                                  If Debounce Time = 0, Debouncer feature will be disabled
                                  Function will set DebounceTime argument to rounded supported value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad or unsupported DebounceDuration value
  @retval EFI_UNSUPPORTED         GpioPad is not owned by host
**/
EFI_STATUS
GpioV2SetDebounce (
  IN GPIOV2_PAD                   GpioPad,
  IN UINT32                       DebounceTime
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetDebounce (GpioServices, GpioPad, DebounceTime);
}

/**
  This procedure reads current ownership configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetOwnership (
  IN GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_PAD_OWN              *Ownership
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetOwnership (GpioServices, GpioPad, Ownership);
}

/**
  This procedure sets host ownership configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetHostOwnership (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_HOSTSW_OWN            HostOwnership
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetHostOwnership (GpioServices, GpioPad, HostOwnership);
}

/**
  This procedure reads current host ownership configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetHostOwnership (
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_HOSTSW_OWN            *HostOwnership
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetHostOwnership (GpioServices, GpioPad, HostOwnership);
}

/**
  This procedure sets Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                GPI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetGpiIe (
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        GpiIe
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetGpiIe (GpioServices, GpioPad, GpiIe);
}

/**
  This procedure reads current Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                Pointer to a buffer for GPI Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGpiIe (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiIe
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpiIe (GpioServices, GpioPad, GpiIe);
}

/**
  This procedure reads current Gpi Status for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] GpiIs                Pointer to a buffer for GPI Status

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGpiIs (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiIs
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpiIs (GpioServices, GpioPad, GpiIs);
}

/**
  This procedure sets GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] GpiGpeEn             GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetGpiGpeEn (
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        GpiGpeEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetGpiGpeEn (GpioServices, GpioPad, GpiGpeEn);
}

/**
  This procedure reads current GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] GpiGpeEn             GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGpiGpeEn (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiGpeEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpiGpeEn (GpioServices, GpioPad, GpiGpeEn);
}

/**
  This procedure reads current GPI GPE Status for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[out] GpiGpeSts            Pointer to a buffer for GPI GPE Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGpiGpeSts (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiGpeSts
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpiGpeSts (GpioServices, GpioPad, GpiGpeSts);
}

/**
  This procedure sets NMI Enable for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] NmiEn                NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetNmiEn (
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        NmiEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetNmiEn (GpioServices, GpioPad, NmiEn);
}

/**
  This procedure reads current NMI Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[out] NmiEn                Pointer to a buffer for NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetNmiEn (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *NmiEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetNmiEn (GpioServices, GpioPad, NmiEn);
}

/**
  This procedure reads current NMI Status configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[out] NmiSts               Pointer to a buffer for NMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetNmiSts (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *NmiSts
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetNmiSts (GpioServices, GpioPad, NmiSts);
}

/**
  This procedure sets SMI Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] SmiSts               SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetSmiEn (
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        SmiEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetSmiEn (GpioServices, GpioPad, SmiEn);
}

/**
  This procedure reads current SMI Enable configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[out] SmiSts               Pointer to a buffer for SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetSmiEn (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *SmiEn
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetSmiEn (GpioServices, GpioPad, SmiEn);
}

/**
  This procedure reads current SMI Status for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[out] SmiSts              Pointer to a buffer for SMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetSmiSts (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *SmiSts
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetSmiEn (GpioServices, GpioPad, SmiSts);
}

/**
  This procedure clears current SMI Status for requested Gpio Pad

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2ClearSmiSts (
  IN  GPIOV2_PAD                    GpioPad
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->ClearSmiSts (GpioServices, GpioPad);
}

/**
  This procedure verifies if requested GpioPad definition is ment for platform that it is used on

  @param[in] GpioPad              GPIO pad
  @param[out] IsPadValid          Pointer to a buffer for Is Pad Valid information, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2IsPadValid (
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *IsPadValid
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->IsPadValid (GpioServices, GpioPad, IsPadValid);
}

/**
  This procedure set Lock and LockTx registers as enabled for all GpioPads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2LockAll (
  VOID
  )
{
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;

  //
  // PTL P
  //
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO Services with HID %a\n", GPIO_HID_PTL_PCD_P));
  } else {
    Status = GpioServices->LockAll (GpioServices);
  }

  return Status;
}

/**
  This procedure sets RxEv configuration for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] RxEvCfg              RxEv configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRxEvCfg (
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RXEVCFG               RxEvCfg
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRxEvCfg (GpioServices, GpioPad, RxEvCfg);
}

/**
  This procedure sets GPIROUTNMI bit (17th bit in DW0) for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 17 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRouteNmi (
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRouteNmi (GpioServices, GpioPad, Enable);
}

/**
  This procedure sets GPIROUTSMI bit (18th bit in DW0) for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 18 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRouteSmi (
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRouteSmi (GpioServices, GpioPad, Enable);
}

/**
  This procedure sets GPIROUTSCI bit (19th bit in DW0) for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 19 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRouteSci (
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRouteSci (GpioServices, GpioPad, Enable);
}

/**
  This procedure sets GPIROUTIOXAPIC bit (20th bit in DW0) for requested Gpio Pad

  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 20th bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetRouteIoxApic (
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetRouteIoxApic (GpioServices, GpioPad, Enable);
}

/**
  This procedure retrieves name of requested Gpio Pad

  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. PTL, EBG, ...)
  @param[in] NameBufferLength     Maximum number of characters to be stored in NameBuffer
  @param[out] NameBuffer          Pointer to a buffer for Gpio Pad name

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetPadName (
  IN  GPIOV2_PAD                  GpioPad,
  IN  UINT32                      NameBufferLength,
  OUT CHAR8                       *NameBuffer
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetPadName (GpioServices, GpioPad, NameBufferLength, NameBuffer);
}

/**
  This procedure will set GPIO PAD configuration

  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_INVALID_PARAMETER    Invalid group or pad number
**/
EFI_STATUS
GpioV2ConfigurePad (
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_CONFIG            *PadConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->ConfigurePad (GpioServices, GpioPad, PadConfig);
}

/**
  This procedure will get actual GPIO PAD configuration

  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_INVALID_PARAMETER    Invalid group or pad number
**/
EFI_STATUS
GpioV2GetPadConfiguration (
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_CONFIG            *PadConfig
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetPadConfiguration (GpioServices, GpioPad, PadConfig);
}

/**
  This procedure will configure all GPIO pads given in PadInitConfigTable
  All pads in PadInitConfigTable should belong to the same ChipsetId.

  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
GpioV2ConfigurePads (
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  if ((GpioPadsConfigTable == NULL) || (GpioPadsConfigTableSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // All pads in PadInitConfigTable should belong to the same ChipsetId.
  //
  Status = GetGpioV2ServicesFromPad (GpioPadsConfigTable[0].GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->ConfigurePads (GpioServices, GpioPadsConfigTable, GpioPadsConfigTableSize);

}

/**
  This procedure will create HOB for Conflict Check Data.
  All pads in PadInitConfigTable should belong to the same ChipsetId.

  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
GpioV2SetConflictCheckData (
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  if ((GpioPadsConfigTable == NULL) || (GpioPadsConfigTableSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // All pads in PadInitConfigTable should belong to the same ChipsetId.
  //
  Status = GetGpioV2ServicesFromPad (GpioPadsConfigTable[0].GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->SetConflictCheckData (GpioServices, GpioPadsConfigTable, GpioPadsConfigTableSize);

}

/**
  This procedure will check if there are any discrepancies between
  data saved by GPIO_SET_CONFLICT_CHECK_DATA method and actual
  GPIOs configuration

  @retval Status
**/
EFI_STATUS
GpioV2ConflictCheck (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This procedure gets Smi Gpi Number based on GPIOV2_PAD

  @param[in]  GpioPad                Gpio Pad
  @param[out] GpiNum                 Smi Gpi Number

  @retval Status
**/
EFI_STATUS
GpioV2GetGpiSmiNumFromPad (
  IN  GPIOV2_PAD          GpioPad,
  OUT UINTN               *GpiNum
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpiSmiNumFromPad (GpioServices, GpioPad, GpiNum);
}

/**
  This procedure gets GPIOV2_PAD based on Smi Gpi Number

  @param[in]   GpiNum                 Smi Gpi Number
  @param[out]  GpioPad                Gpio Pad

  @retval Status
**/
EFI_STATUS
GpioV2GetPadFromGpiSmiNum (
  IN   UINTN             GpiNum,
  OUT  GPIOV2_PAD        *GpioPad
  )
{
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO Services with HID %a\n", GPIO_HID_PTL_PCD_P));
  } else {
    Status = GpioServices->GetPadFromGpiSmiNum (GpioServices, GpiNum, GpioPad);
  }

  return Status;
}

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  Index                   GPI SMI number
  @param[out] GpioPad                 GPIO pad
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval Status
**/
EFI_STATUS
GpioV2GetPadAndSmiRegs (
  IN  UINT32             Index,
  OUT GPIOV2_PAD         *GpioPad,
  OUT UINT8              *GpiSmiBitOffset,
  OUT UINT32             *GpiHostSwOwnRegAddress,
  OUT UINT32             *GpiSmiStsRegAddress
  )
{
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO Services with HID %a\n", GPIO_HID_PTL_PCD_P));
  } else {
    Status = GpioServices->GetPadAndSmiRegs (
                             GpioServices,
                             Index,
                             GpioPad,
                             GpiSmiBitOffset,
                             GpiHostSwOwnRegAddress,
                             GpiSmiStsRegAddress
                             );
  }

  return Status;
}

EFI_STATUS
GpioV2GetPadIoApicIrqNum (
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *IrqNum
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetPadIoApicIrqNum (GpioServices, GpioPad, IrqNum);
}

EFI_STATUS
GpioV2SetMiscCfg (
  IN UINT32           CommunityIndex,
  IN UINT32           MiscCfgValue
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
GpioV2GetMiscCfg (
  IN UINT32           CommunityIndex,
  IN UINT32           *MiscCfgValue
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This procedure sets Gpio Group to GPE mapping

  @param[in] GpioGroup0           GPIO Group 0
  @param[in] GpioGroup2           GPIO Group 2
  @param[in] GpioGroup1           GPIO Group 1

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2SetGroupToGpeMapping (
  IN GPIOV2_PAD_GROUP              GpioGroup0,
  IN GPIOV2_PAD_GROUP              GpioGroup1,
  IN GPIOV2_PAD_GROUP              GpioGroup2
  )
{
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;

  //
  // PTL P
  //
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO Services\n"));
    return Status;
  }

  return GpioServices->SetGroupToGpeMapping (GpioServices, GpioGroup0, GpioGroup1, GpioGroup2);
}

/**
  This procedure gets Gpio Group to GPE mapping

  @param[in] GpioGroup0           GPIO Group 0
  @param[in] GpioGroup2           GPIO Group 2
  @param[in] GpioGroup1           GPIO Group 1

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGroupToGpeMapping (
  IN GPIOV2_PAD_GROUP             *GpioGroup0,
  IN GPIOV2_PAD_GROUP             *GpioGroup1,
  IN GPIOV2_PAD_GROUP             *GpioGroup2
  )
{
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;

  //
  // PTL P
  //
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GPIO Services\n"));
    return Status;
  }

  return GpioServices->GetGroupToGpeMapping (GpioServices, GpioGroup0, GpioGroup1, GpioGroup2);
}

/**
  This procedure gets Gpio Pad GPE number

  @param[in]  GpioPad             GPIO Pad
  @param[out] GpeNumber           GPE Number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioV2GetGpeNumber (
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *GpeNumber
  )
{
  EFI_STATUS                Status;
  GPIOV2_SERVICES           *GpioServices;

  Status = GetGpioV2ServicesFromPad (GpioPad, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return GpioServices->GetGpeNumber (GpioServices, GpioPad, GpeNumber);
}
