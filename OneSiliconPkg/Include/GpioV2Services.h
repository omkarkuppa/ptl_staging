/** @file
  Header file for GPIO Ppi and Protocol definitions.

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
#ifndef _GPIOV2_PROTOCOL_H_
#define _GPIOV2_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>
#include <Base.h>
#include <GpioV2Pad.h>
#include <GpioV2Config.h>

#define GPIOV2_CONTROLLER_HID_LENGTH  8
#define GPIOV2_NAME_LENGTH_MAX        32

typedef struct _GPIOV2_PROTOCOL            GPIOV2_SERVICES;
typedef union  _GPIOV2_SIGNAL              GPIOV2_SIGNAL;
typedef struct _GPIOV2_NATIVE_SIGNAL_DATA  GPIOV2_NATIVE_SIGNAL_DATA;
typedef struct _GPIOV2_PAD_SIGNAL          GPIOV2_PAD_SIGNAL;
typedef struct _GPIOV2_PAD_DATA            GPIOV2_PAD_DATA;

/**
  This procedure will set GPIO mode

  @param[in] GPIOV2_SERVICES      Gpio Services
  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] PadModeValue         GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PADMODE) (
  IN GPIOV2_SERVICES             *This,
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_MODE             PadMode
  );

/**
  This procedure reads current GPIO Pad Mode

  @param[in] GPIOV2_SERVICES     Gpio Services
  @param[in] GpioPad             GPIO Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] PadMode             Pointer to a buffer for GPIO Pad Mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PADMODE) (
  IN GPIOV2_SERVICES             *This,
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_MODE             *PadMode
  );

/**
  This procedure will set GPIO enable or disable input inversion on rquested pad

  @param[in] GPIOV2_SERVICES     Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] InputInversion      GpioV2InputInversionEnable or GpioV2InputInversionDisable, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_INPUT_INVERSION) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_INPUT_INVERSION   InputInversion
  );

/**
  This procedure will Get GPIO enable or disable input inversion on rquested pad

  @param[in]  GPIOV2_SERVICES     Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[Out] Data                Enabled / Disabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_INPUT_INVERSION) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  OUT BOOLEAN                     *Inverted
  );

/**
  This procedure will set GPIO Lock (register PADCFGLOCK)

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] Lock                GpioV2Unlock - Unlock pad, GpioV2Lock - Lock pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_LOCK) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_LOCK              Lock
  );

/**
  This procedure will get GPIO Lock (register PADCFGLOCK)

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] Lock                GpioV2Unlock - Unlock pad, GpioV2Lock - Lock pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_LOCK) (
  IN  GPIOV2_SERVICES              *This,
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_PAD_LOCK              *Lock
  );

/**
  This procedure will set GPIO Lock Tx (register PADCFGLOCKTX)

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] LockTx              GpioV2Unlock - Unlock output state of Gpio Pad, GpioV2Lock - Lock output state of Gpio Pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_LOCK_TX) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_LOCK              LockTx
  );

/**
  This procedure will get GPIO Lock Tx (register PADCFGLOCKTX)

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] LockTx              GpioV2Unlock - Unlock output state of Gpio Pad, GpioV2Lock - Lock output state of Gpio Pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_LOCK_TX) (
  IN  GPIOV2_SERVICES              *This,
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_PAD_LOCK              *LockTx
  );

/**
  This procedure sets Gpio Pad output state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         GpioV2StateLow - output state low, GpioV2StateHigh - output state high

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_TX) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             OutputState
  );

/**
  This procedure reads current Gpio Pad output state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         Pointer to a buffer for output state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_TX) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             *OutputState
  );

/**
  This procedure reads current Gpio Pad input state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] InputState          Pointer to a buffer for input state

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
  @retval EFI_ACCESS_DENIED      Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_RX) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_STATE             *InputState
  );

/**
  This procedure reads if TX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
  @retval EFI_ACCESS_DENIED      Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_TX_DISABLE) (
  IN GPIOV2_SERVICES            *This,
  IN GPIOV2_PAD                 GpioPad,
  IN BOOLEAN                    *TxDisabled
  );

/**
  This procedure sets TX buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] TxDisabled           True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_TX_DISABLE) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      TxDisabled
  );

/**
  This procedure reads if RX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] RxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_RX_DISABLE) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      *RxDisabled
  );

/**
  This procedure sets Rx buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] TxDisabled           True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_RX_DISABLE) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN BOOLEAN                      RxDisabled
  );

/**
  This procedure sets Reset Configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] ResetConfig          Reset Configuration - please refer to GpioV2Pad.h (GPIOV2_RESET_CONFIG)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_RESET_CONFIG) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RESET_CONFIG          ResetConfig
  );

/**
  This procedure reads current Reset Configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] ResetConfig          Pointer to a buffer for Reset Configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_RESET_CONFIG) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RESET_CONFIG          *ResetConfig
  );

/**
  This procedure sets termination configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] TerminationConfig    Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_TERMINATION_CONFIG) (
  IN GPIOV2_SERVICES               *This,
  IN GPIOV2_PAD                    GpioPad,
  IN GPIOV2_TERMINATION_CONFIG     ElectricalConfig
  );

/**
  This procedure reads current termination configuration for requested Gpio Pad

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TerminationConfig   Pointer to a buffer for Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_TERMINATION_CONFIG) (
  IN GPIOV2_SERVICES               *This,
  IN GPIOV2_PAD                    GpioPad,
  IN GPIOV2_TERMINATION_CONFIG     *ElectricalConfig
  );

/**
  This procedure sets IO Standby configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_IOSTANDBY_CONFIG) (
  IN GPIOV2_SERVICES                *This,
  IN GPIOV2_PAD                     GpioPad,
  IN GPIOV2_IOSTANDBY_STATE         IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM          IosTerminationConfig
  );

/**
  This procedure reads current IO Standby configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       Pointer to a buffer for IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig Pointer to a buffer for IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_IOSTANDBY_CONFIG) (
  IN GPIOV2_SERVICES                *This,
  IN GPIOV2_PAD                     GpioPad,
  IN GPIOV2_IOSTANDBY_STATE         *IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM          *IosTerminationConfig
  );

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
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_DEBOUNCE) (
  IN GPIOV2_SERVICES            *This,
  IN GPIOV2_PAD                   GpioPad,
  IN UINT32                       DebounceTime
  );

/**
  This procedure reads current ownership configuration for requested Gpio Pad
  @param[in] GpioServices       Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h
  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_OWNERSHIP) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_PAD_OWN              *Ownership
  );

/**
  This procedure sets host ownership configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PAD_HOST_OWNERSHIP) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_HOSTSW_OWN            HostOwnership
  );

/**
  This procedure reads current host ownership configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_HOST_OWNERSHIP) (
  IN  GPIOV2_SERVICES            *This,
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_HOSTSW_OWN            *HostOwnership
  );

/**
  This procedure sets Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                GPI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PAD_GPI_IE) (
  IN GPIOV2_SERVICES                *This,
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        GpiIe
  );

/**
  This procedure reads current Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                Pointer to a buffer for GPI Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_GPI_IE) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiIe
  );

/**
  This procedure reads current Gpi Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIs                Pointer to a buffer for GPI Status

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_GPI_IS) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  OUT BOOLEAN                     *GpiIs
  );

/**
  This procedure sets GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiGpeEn             GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PAD_GPI_GPE_EN) (
  IN GPIOV2_SERVICES                *This,
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        GpiGpeEn
  );

/**
  This procedure reads current GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiGpeEn             GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_GPI_GPE_EN) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiGpeEn
  );

/**
  This procedure reads current GPI GPE Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] GpiGpeSts           Pointer to a buffer for GPI GPE Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_GPI_GPE_STS) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *GpiGpeSts
  );

/**
  This procedure sets NMI Enable for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] NmiEn                NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PAD_NMI_EN) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        NmiEn
  );

/**
  This procedure reads current NMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] NmiEn               Pointer to a buffer for NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_NMI_EN) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *NmiEn
  );

/**
  This procedure reads current NMI Status configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] NmiSts              Pointer to a buffer for NMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_NMI_STS) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *NmiSts
  );

/**
  This procedure sets SMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] SmiSts               SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_PAD_SMI_EN) (
  IN GPIOV2_SERVICES                *This,
  IN GPIOV2_PAD                     GpioPad,
  IN BOOLEAN                        SmiEn
  );

/**
  This procedure reads current SMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] SmiSts              Pointer to a buffer for SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_SMI_EN) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *SmiEn
  );

/**
  This procedure reads current SMI Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] SmiSts              Pointer to a buffer for SMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_SMI_STS) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *SmiSts
  );

/**
  This procedure clears current SMI Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_CLR_PAD_SMI_STS) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad
  );

/**
  This procedure verifies if requested GpioGroup definition is ment for platform that it is used on

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioGroup           GPIO pad
  @param[out] IsGroupValid        Pointer to a buffer for boolean information, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_IS_GROUP_VALID) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD_GROUP              GpioGroup,
  OUT BOOLEAN                       *IsGroupValid
  );

/**
  This procedure verifies if requested GpioPad definition is ment for platform that it is used on

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] IsPadValid          Pointer to a buffer for Is Pad Valid information, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_IS_PAD_VALID) (
  IN  GPIOV2_SERVICES               *This,
  IN  GPIOV2_PAD                    GpioPad,
  OUT BOOLEAN                       *IsPadValid
  );

/**
  This procedure set Lock and LockTx registers as enabled for all GpioPads

  @param[in] GpioServices         Gpio Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_LOCK_ALL) (
  IN  GPIOV2_SERVICES           *This
  );

/**
  This procedure gets RxEv configuration for requested Gpio Pad

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] RxEvCfg             RxEv configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_RX_EV_CFG) (
  IN  GPIOV2_SERVICES              *GpioAccess,
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_RXEVCFG               *RxEvCfg
  );

/**
  This procedure sets RxEv configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] RxEvCfg              RxEv configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_RX_EV_CFG) (
  IN  GPIOV2_SERVICES             *GpioAccess,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_RXEVCFG               RxEvCfg
  );

/**
  This procedure retrieves register offset

  @param[in]  GPIOV2_SERVICES     Gpio Services
  @param[in]  Register            Register for which user want to retrieve offset. Please refer to GpioV2Pad.h
  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_REGISTER_OFFSET) (
  IN  GPIOV2_SERVICES            *This,
  IN  GPIOV2_REGISTER             Register,
  IN  GPIOV2_PAD                  GpioPad,
  OUT UINT32                      *RegisterOffset
  );

/**
  This procedure sets GPIROUTNMI bit (17th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 17 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_ROUTE_NMI) (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  );

/**
  This procedure sets GPIROUTSMI bit (18th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 18 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_ROUTE_SMI) (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  );

/**
  This procedure sets GPIROUTSCI bit (19th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 19 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_ROUTE_SCI) (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  );

/**
  This procedure sets GPIROUTIOXAPIC bit (20th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 20th bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_ROUTE_IOXAPIC) (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  );

/**
  This procedure retrieves name of requested Gpio Pad

  @param[in] GPIOV2_SERVICES      Gpio Services
  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] NameBufferLength     Maximum number of characters to be stored in NameBuffer
  @param[out] NameBuffer          Pointer to a buffer for Gpio Pad name

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_NAME) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  IN  UINT32                      NameBufferLength,
  OUT CHAR8                       *NameBuffer
  );

/**
  This procedure will set GPIO PAD configuration
  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_CONFIGURE_PAD) (
  IN GPIOV2_SERVICES          *This,
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_CONFIG            *PadConfig
  );

/**
  This procedure will get actual GPIO PAD configuration
  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_CONFIGURATION) (
  IN GPIOV2_SERVICES          *This,
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_CONFIG            *PadConfig
  );

/**
  This procedure will configure all GPIO pads given in PadInitConfigTable

  @param[in] GpioServices             Gpio Services
  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_CONFIGURE_PADS) (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
);

/**
  This procedure will create HOB for Conflict Check Data.

  @param[in] GpioServices             Gpio Services
  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_CONFLICT_CHECK_DATA) (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
);

/**
  This procedure will check if there are any discrepancies between
  data saved by GPIO_SET_CONFLICT_CHECK_DATA method and actual
  GPIOs configuration

  @param[in] GpioServices             Gpio Services

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_CONFLICT_CHECK) (
  IN GPIOV2_SERVICES          *GpioServices
);

/**
  This procedure gets Smi Gpi Number based on GPIOV2_PAD

  @param[in]  GpioServices           Gpio Services
  @param[in]  GpioPad                Gpio Pad
  @param[out] GpiNum                 Smi Gpi Number

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_SMI_GPI_NUM) (
  IN  GPIOV2_SERVICES     *GpioServices,
  IN  GPIOV2_PAD          GpioPad,
  OUT UINTN               *GpiNum
  );

/**
  This procedure gets GPIOV2_PAD based on Smi Gpi Number

  @param[in]   GpioServices           Gpio Services
  @param[in]   GpiNum                 Smi Gpi Number
  @param[out]  GpioPad                Gpio Pad

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_SMI_GPI_NUM_PAD) (
  IN   GPIOV2_SERVICES     *GpioServices,
  IN   UINTN               GpiNum,
  OUT  GPIOV2_PAD          *GpioPad
  );

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  GpioServices            Gpio Services
  @param[in]  Index                   GPI SMI number
  @param[out] GpioPad                 GPIO pad
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval Status
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_SMI_REGS) (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  UINT32             Index,
  OUT GPIOV2_PAD         *GpioPad,
  OUT UINT8              *GpiSmiBitOffset,
  OUT UINT64             *GpiHostSwOwnRegAddress,
  OUT UINT64             *GpiSmiStsRegAddress
  );

/**
  This procedure will get GPIO IOxAPIC interrupt number

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] IrqNum              IRQ number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_IOAPIC_IRQ_NUM) (
  IN GPIOV2_SERVICES  *GpioServices,
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *IrqNum
  );

typedef
EFI_STATUS
(EFIAPI *GPIO_SET_MISCCFG) (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           CommunityIndex,
  IN UINT32           MiscCfgValue
  );

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_MISCCFG) (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           CommunityIndex,
  IN UINT32           *MiscCfgValue
  );

/**
  This procedure sets Gpio Group to GPE mapping

  @param[in] This                 Gpio Services
  @param[in] GpioGroup0           GPIO Group 0
  @param[in] GpioGroup1           GPIO Group 1
  @param[in] GpioGroup2           GPIO Group 2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_SET_GROUP_TO_GPE_MAPPING) (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD_GROUP              GpioGroup0,
  IN GPIOV2_PAD_GROUP              GpioGroup1,
  IN GPIOV2_PAD_GROUP              GpioGroup2
  );

/**
  This procedure retrieves GPIOV2_PAD_GROUP based on GPIO/COM0/MISCCFG DwX values.

  @param[in]  This                Gpio Services
  @param[out] GpioGroup0          GPIO Group 0
  @param[out] GpioGroup1          GPIO Group 1
  @param[out] GpioGroup2          GPIO Group 2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_GET_GROUP_TO_GPE_MAPPING) (
  IN  GPIOV2_SERVICES             *This,
  OUT GPIOV2_PAD_GROUP            *GpioGroup0,
  OUT GPIOV2_PAD_GROUP            *GpioGroup1,
  OUT GPIOV2_PAD_GROUP            *GpioGroup2
  );

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_GPE_NUMBER) (
  IN GPIOV2_SERVICES  *This,
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *GpeNumber
  );

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_SIGNAL_INFO_BY_SIGNAL) (
  IN  GPIOV2_SERVICES             *This,
  IN  UINT32                      Signal,
  OUT GPIOV2_NATIVE_SIGNAL_DATA   *SignalData
);

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_NATIVE_PAD_BY_FUNCTION) (
  IN  GPIOV2_SERVICES             *This,
  IN  UINT32                      Signal,
  OUT GPIOV2_PAD                  *GpioPad,
  OUT GPIOV2_PAD_MODE             *PadMode
);

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_DATA_FOR_PAD) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  OUT GPIOV2_PAD_DATA             **PadData
);

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_PAD_MODE_FOR_PAD) (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  IN  UINT32                      Signal,
  OUT GPIOV2_PAD_MODE             *PadMode
);

typedef
EFI_STATUS
(EFIAPI *GPIO_RESTRICT_WRITE_ACCESS) (
  IN  GPIOV2_SERVICES             *This,
  IN  UINT32                      CommunityIndex
);

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_GLOBAL_GROUP_INDEX) (
  IN   GPIOV2_SERVICES             *This,
  IN   GPIOV2_PAD_GROUP            GpioGroup,
  OUT  UINT32                      *GlobalGroupIndex
);

typedef
EFI_STATUS
(EFIAPI *GPIO_GET_NEXT_PAD) (
  IN  GPIOV2_SERVICES              *GpioServices,
  IN  GPIOV2_PAD                   CurrentPad,
  OUT GPIOV2_PAD                   *NextPad
);

/**
  This procedure clears the GPI_IE registers for all GpioPads.

  @param[in] GpioServices         Gpio Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
typedef
EFI_STATUS
(EFIAPI *GPIO_DISABLE_ALL_GPI_IE) (
  IN  GPIOV2_SERVICES *This
  );


#define GPIOV2_CONTROLLER_REVISION  2
/**
  <b>Revision 0:</b>
  - Inital version.
  <b>Revision 1:</b>
  - Added functions: GetLock, GetLockTx, GetRxEvCfg.
  <b>Revision 2:</b>
  - Added functions: DisableAllGpiIe.
**/

struct _GPIOV2_PROTOCOL {
  ///
  /// The revision to which the interface adheres.
  /// All future revisions must be backwards compatible.
  ///
  UINT32                              Revision;

  UINT8                               Hid[GPIOV2_CONTROLLER_HID_LENGTH];
  UINT32                              Uid;
  UINT32                              UnlockAllPads;
  GPIO_GET_REGISTER_OFFSET            GetRegisterOffset;

  //
  // Basic settings
  //
  GPIO_SET_PADMODE                    SetPadMode;
  GPIO_GET_PADMODE                    GetPadMode;
  GPIO_SET_INPUT_INVERSION            SetInputInversion;
  GPIO_GET_INPUT_INVERSION            GetInputInversion;
  GPIO_SET_LOCK                       SetLock;
  GPIO_GET_LOCK                       GetLock;
  GPIO_SET_LOCK_TX                    SetLockTx;
  GPIO_GET_LOCK_TX                    GetLockTx;
  GPIO_SET_TX                         SetTx;
  GPIO_GET_TX                         GetTx;
  GPIO_GET_RX                         GetRx;
  GPIO_SET_TX_DISABLE                 SetTxDisable;
  GPIO_GET_TX_DISABLE                 GetTxDisable;
  GPIO_SET_RX_DISABLE                 SetRxDisable;
  GPIO_GET_RX_DISABLE                 GetRxDisable;
  GPIO_SET_RESET_CONFIG               SetResetConfig;
  GPIO_GET_RESET_CONFIG               GetResetConfig;
  GPIO_SET_TERMINATION_CONFIG         SetTerminationConfig;
  GPIO_GET_TERMINATION_CONFIG         GetTerminationConfig;
  GPIO_SET_IOSTANDBY_CONFIG           SetIostandbyConfig;
  GPIO_GET_IOSTANDBY_CONFIG           GetIostandbyConfig;
  GPIO_SET_DEBOUNCE                   SetDebounce;
  GPIO_GET_PAD_OWNERSHIP              GetOwnership;
  GPIO_SET_PAD_HOST_OWNERSHIP         SetHostOwnership;
  GPIO_GET_PAD_HOST_OWNERSHIP         GetHostOwnership;
  GPIO_SET_PAD_GPI_IE                 SetGpiIe;
  GPIO_GET_PAD_GPI_IE                 GetGpiIe;
  GPIO_GET_PAD_GPI_IS                 GetGpiIs;
  GPIO_SET_PAD_GPI_GPE_EN             SetGpiGpeEn;
  GPIO_GET_PAD_GPI_GPE_EN             GetGpiGpeEn;
  GPIO_GET_PAD_GPI_GPE_STS            GetGpiGpeSts;
  GPIO_SET_PAD_NMI_EN                 SetNmiEn;
  GPIO_GET_PAD_NMI_EN                 GetNmiEn;
  GPIO_GET_PAD_NMI_STS                GetNmiSts;
  GPIO_SET_PAD_SMI_EN                 SetSmiEn;
  GPIO_GET_PAD_SMI_EN                 GetSmiEn;
  GPIO_GET_PAD_SMI_STS                GetSmiSts;
  GPIO_CLR_PAD_SMI_STS                ClearSmiSts;
  GPIO_GET_PAD_SMI_GPI_NUM            GetGpiSmiNumFromPad;
  GPIO_GET_SMI_GPI_NUM_PAD            GetPadFromGpiSmiNum;
  GPIO_GET_PAD_SMI_REGS               GetPadAndSmiRegs;
  GPIO_GET_RX_EV_CFG                  GetRxEvCfg;
  GPIO_SET_RX_EV_CFG                  SetRxEvCfg;
  GPIO_IS_GROUP_VALID                 IsGroupValid;
  GPIO_IS_PAD_VALID                   IsPadValid;
  GPIO_SET_ROUTE_NMI                  SetRouteNmi;
  GPIO_SET_ROUTE_SMI                  SetRouteSmi;
  GPIO_SET_ROUTE_SCI                  SetRouteSci;
  GPIO_SET_ROUTE_IOXAPIC              SetRouteIoxApic;
  GPIO_LOCK_ALL                       LockAll;
  GPIO_GET_PAD_NAME                   GetPadName;
  GPIO_CONFIGURE_PAD                  ConfigurePad;
  GPIO_CONFIGURE_PADS                 ConfigurePads;
  GPIO_GET_PAD_CONFIGURATION          GetPadConfiguration;
  GPIO_GET_PAD_IOAPIC_IRQ_NUM         GetPadIoApicIrqNum;
  GPIO_SET_MISCCFG                    SetMiscCfg;
  GPIO_GET_MISCCFG                    GetMiscCfg;
  GPIO_SET_GROUP_TO_GPE_MAPPING       SetGroupToGpeMapping;
  GPIO_GET_GROUP_TO_GPE_MAPPING       GetGroupToGpeMapping;
  GPIO_GET_GPE_NUMBER                 GetGpeNumber;
  GPIO_SET_CONFLICT_CHECK_DATA        SetConflictCheckData;
  GPIO_CONFLICT_CHECK                 ConflictCheck;
  GPIO_GET_SIGNAL_INFO_BY_SIGNAL      GetSignalInfoBySignal;
  GPIO_GET_PAD_DATA_FOR_PAD           GetPadDataForPad;
  GPIO_GET_NATIVE_PAD_BY_FUNCTION     GetNativePadByFunction;
  GPIO_GET_PAD_MODE_FOR_PAD           GetPadModeForPad;
  GPIO_RESTRICT_WRITE_ACCESS          RestrictWriteAccess;
  GPIO_GET_GLOBAL_GROUP_INDEX         GetGlobalGroupIndex;
  GPIO_GET_NEXT_PAD                   GetNextPad;
  GPIO_DISABLE_ALL_GPI_IE             DisableAllGpiIe;
};

#endif // _GPIOV2_PROTOCOL_H_
