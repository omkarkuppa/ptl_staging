/** @file
  LPSS UART config defines.

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

#ifndef _LPSS_UART_CONFIG_H_
#define _LPSS_UART_CONFIG_H_

#define LPSS_UART_CONFIG_REVISION 1

#define PCH_MAX_LPSS_UART_CONTROLLERS     4

extern EFI_GUID gLpssUartConfigGuid;
#pragma pack (push,1)

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

/**
  Available working modes for Lpss UART Host Controller

       0: LpssUartDisabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - PSF:
          !important! If given device is Function 0 and other higher functions on given device
                      are enabled, PSF disabling is skipped. PSF default will remain and device PCI CFG Space will still be visible.
                      This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
    <b>1: LpssUartPci;</b>
          - Designated for Serial IO UART OS driver usage
          - Gpio pin configuration in native mode for each assigned pin
          - Device will be enabled in PSF
          - Only BAR0 will be enabled
       2: LpssUartHidden;
          - Designated for BIOS and/or DBG2 OS kernel debug
          - Gpio pin configuration in native mode for each assigned pin
          - Device disabled in the PSF
          - Both BARs are enabled, BAR1 becomes devices Pci Config Space
          - BAR0 assigned from the global PCH reserved memory range, reported as motherboard resource by SIRC
       @note
           If this controller is located at function 0 and it's mode is set to hidden it will not be visible in the PCI space.
       3: LpssUartCom;
          - Designated for 16550/PNP0501 compatible COM device
          - Gpio pin configuration in native mode for each assigned pin
          - Device disabled in the PSF
          - Both BARs are enabled, BAR1 becomes devices Pci Config Space
          - BAR0 assigned from the global PCH reserved memory range, reported as motherboard resource by SIRC
       4: LpssUartSkipInit;
          - Gpio configuration is skipped
          - PSF configuration is skipped
          - BAR assignemnt is skipped
          - D-satate setting is skipped

**/
typedef enum {
  LpssUartDisabled,
  LpssUartPci,
  LpssUartHidden,
  LpssUartCom,
  LpssUartSkipInit
} LPSS_UART_MODE;

typedef enum {
  LpssUartPgDisabled,  ///< No _PS0/_PS3 support, device left in D0, after initialization
/**
  In mode: LpssUartCom;
           _PS0/_PS3  that supports getting device out of reset
  In mode: LpssUartPci
           Keeps UART in D0 and assigns Fixed MMIO for SEC/PEI usage only
**/
  LpssUartPgEnabled,
  LpssUartPgAuto       ///< _PS0 and _PS3, detection through ACPI if device was initialized prior to first PG. If it was used (DBG2) PG is disabled,
} LPSS_UART_PG;

/**
  UART Settings
**/
typedef struct {
  UINT32      BaudRate;    ///< <b> 115200 </b> Max 6000000  MdePkg.dec  PcdUartDefaultBaudRate
  UINT8       Parity;      ///< <b> 1 - No Parity</b> see EFI_PARITY_TYPE MdePkg.dec PcdUartDefaultParity
  UINT8       DataBits;    ///< <b>8</b> MdePkg.dec PcdUartDefaultDataBits
  UINT8       StopBits;    ///< <b>1 - One Stop Bit</b> see EFI_STOP_BITS_TYPE  MdePkg.dec  PcdUartDefaultStopBits
  UINT8       AutoFlow;    ///< <b>FALSE</b>  IntelFrameworkModulePkg.dsc PcdIsaBusSerialUseHalfHandshake
} LPSS_UART_ATTRIBUTES;

/**
  UART signals pin muxing settings. If signal can be enable only on a single pin
  then this parameter is ignored by RC. Refer to GPIO_*_MUXING_SERIALIO_UARTx_* in GpioPins*.h
  for supported settings on a given platform
**/
typedef struct {
  UINT32 Rx;  ///< RXD Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RXD_*
  UINT32 Tx;  ///< TXD Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_UARTx_TXD_*
  UINT32 Rts; ///< RTS Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS_*
  UINT32 Cts; ///< CTS Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_UARTx_CTS_*
} LPSS_UART_PIN_MUX;

/**
  Lpss UART Controller Configuration
**/
typedef struct {
  LPSS_UART_ATTRIBUTES      Attributes;  ///< see LPSS_UART_ATTRIBUTES
  LPSS_UART_PIN_MUX         PinMux;      ///< UART pin mux configuration
  UINT8                     Mode;        ///< <b> LpssUartPci </b> see LPSS_UART_MODE
  UINT8                     DBG2;        ///< <b> FALSE </b> If TRUE adds UART to DBG2 table and overrides UartPg to LpssUartPgDisabled
  UINT8                     PowerGating; ///< <b> LpssUartPgAuto </b> Applies to Hidden/COM/SkipInit see LPSS_UART_PG
  UINT8                     DebugPowerGating; ///< <b> Applies to ACPI DBG2 specific case
  UINT8                     DmaEnable;   ///< <b> TRUE </b> Applies to LpssUartPci only. Informs OS driver to use DMA, if false it will run in PIO mode
  UINT8                     RsvdByte125[3];
} LPSS_UART_DEVICE_CONFIG;

/**
  The LPSS_UART_CONFIG block provides the configurations to set the LPSS UART controllers
  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER         Header;                                                ///< Config Block Header
  LPSS_UART_DEVICE_CONFIG     UartDeviceConfig[PCH_MAX_LPSS_UART_CONTROLLERS];   ///< LPSS UART Device Configuration
} LPSS_UART_CONFIG;

#pragma pack (pop)

#endif // _LPSS_UART_CONFIG_H_
