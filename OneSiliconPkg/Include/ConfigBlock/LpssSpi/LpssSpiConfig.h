/** @file
  LPSS SPI config defines.

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

#ifndef _LPSS_SPI_CONFIG_H_
#define _LPSS_SPI_CONFIG_H_

#define LPSS_SPI_CONFIG_REVISION 1

#define PCH_MAX_LPSS_SPI_CONTROLLERS      4
#define PCH_MAX_LPSS_SPI_CHIP_SELECTS     2

extern EFI_GUID gLpssSpiConfigGuid;
#pragma pack (push,1)

#include <ConfigBlock.h>

/**
  Available working modes for LPSS SPI Host Controller

       0: LpssSpiDisabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - PSF:
          !important! If given device is Function 0 and other higher functions on given device
                      are enabled, PSF disabling is skipped. PSF default will remain and device PCI CFG Space will still be visible.
                      This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
    <b>1: LpssSpiPci;</b>
          - Gpio pin configuration in native mode for each assigned pin
          - Device will be enabled in PSF
          - Only BAR0 will be enabled
**/
typedef enum {
  LpssSpiDisabled,
  LpssSpiPci
} LPSS_SPI_MODE;

/**
  Used to set Inactive/Idle polarity of Spi Chip Select
**/
typedef enum {
  LpssSpiCsActiveLow  = 0,
  LpssSpiCsActiveHigh = 1
} LPSS_CS_POLARITY;

/**
  SPI signals pin muxing settings. If signal can be enable only on a single pin
  then this parameter is ignored by RC. Refer to GPIO_*_MUXING_SERIALIO_SPIx_* in GpioPins*.h
  for supported settings on a given platform
**/
typedef struct {
  UINT32 Cs[PCH_MAX_LPSS_SPI_CHIP_SELECTS];      ///< CS Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CS_*
  UINT32 Clk;                                    ///< CLK Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CLK_*
  UINT32 Miso;                                   ///< MISO Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_SPIx_MISO_*
  UINT32 Mosi;                                   ///< MOSI Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_SPIx_MOSI_*
} LPSS_SPI_PIN_MUX;

/**
  The LPSS_SPI_CONFIG provides the configurations to set the Serial IO SPI controller
**/
typedef struct {
  UINT8            Mode;                                          ///< <b>LpssSpiPci </b> see LPSS_SPI_MODE
  UINT8            DefaultCsOutput;                               ///< <b>0 = CS0</b> CS1, CS2, CS3. Default CS used by the SPI HC
  UINT8            CsPolarity[PCH_MAX_LPSS_SPI_CHIP_SELECTS];     ///< Selects SPI ChipSelect signal polarity, 0 = low <b>1 = High</b>
  UINT8            CsEnable[PCH_MAX_LPSS_SPI_CHIP_SELECTS];       ///< <b>0 = Enable</b> 1 = Disable. Based on this setting GPIO for given SPIx CSx will be configured in Native mode
  UINT8            CsMode;                                        ///< <b>0 = HW Control</b> 1 = SW Control. Sets Chip Select Control mode Hardware or Software.
  UINT8            CsState;                                       ///< <b>0 = CS is set to low</b> 1 = CS is set to high
  LPSS_SPI_PIN_MUX PinMux;                                        ///< SPI Pinmux configuration
} LPSS_SPI_DEVICE_CONFIG;

/**
  The LPSS_SPI_CONFIG block provides the configurations to set the LPSS SPI controllers
  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER         Header;                                                ///< Config Block Header
  LPSS_SPI_DEVICE_CONFIG      SpiDeviceConfig[PCH_MAX_LPSS_SPI_CONTROLLERS];    ///< LPSS SPI Device Configuration
} LPSS_SPI_CONFIG;

#pragma pack (pop)

#endif // _LPSS_SPI_CONFIG_H_
