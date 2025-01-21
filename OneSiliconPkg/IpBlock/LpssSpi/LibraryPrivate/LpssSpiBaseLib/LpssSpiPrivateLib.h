/** @file

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

#ifndef _LPSS_SPI_PRIVATE_LIB_H_
#define _LPSS_SPI_PRIVATE_LIB_H_

// uSec Timing values for Lpss Spi Lib
typedef enum {
  LpssSpiWaitIdleTimeout      = 32,
  LpssSpiClockUpdate          = 5,
  LpssSpiSingleOperationDelay = 1,
  LpssSpiNoIntCounter         = 16 // Number of cycles
} LPSS_SPI_TIMERS;

#define  LPSS_SPI_DEFAULT_CLOCK_FREQUENCY  100000000  // 100MHz

typedef enum {
  LpssSpiCsModeHardware  = 0,
  LpssSpiCsModeSoftware  = 1
} LPSS_SPI_CS_MODE;

typedef enum {
  LpssSpiCsStateAsserted    = 0,
  LpssSpiCsStateDeAsserted  = 1
} LPSS_SPI_CS_STATE;

/**
  Wait For LPSS SPI controller to be in Idle state by checking the Busy bit

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
VOID
LpssSpiWaitIdle (
  IN UINTN SpiBaseAddress
  );

/**
  Return LPSS SPI controller Tx Fifo Not Full status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
LpssSpiTxFifoNotFull (
  IN UINTN SpiBaseAddress
  );

/**
  Return LPSS SPI controller Rx Fifo Not Empty status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
LpssSpiRxFifoNotEmpty (
  IN UINTN SpiBaseAddress
  );

/**
  Deactivates SPI device
  - Revert CS to Idle state
  - Disable SSP

  @param[in] SpiBaseAddress  - Base Address of SPI Controller
  @param[in] CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in] CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
**/
VOID
LpssSpiDeactivate (
  IN UINTN  SpiBaseAddress,
  IN UINT8  CsOutputSelect,
  IN UINT8  CsActivePolarity
  );

/**
  Set up the SPI Controller for reading and/or writing

  @param[in]       PciCfgBase        Pci Config Base Address of specified SPI controller
  @param[in][out]  SpiBaseAddress    MMIO Base Address of specified I2C controller
  @param[in]       Frequency         Bus clock frequency (in kHz)
  @param[in]       ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]       ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                     1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]       CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in]       CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]       AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                     TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_TIMEOUT        Timeout occurred in SPI disable/enable
  @retval   EFI_SUCCESS        W/R successfully initialized
  @retval   EFI_DEVICE_ERROR   Lpss Spi Device is still not enabled or there is no BAR assigned
**/
EFI_STATUS
LpssSpiInitializeWriteRead (
  IN UINT64       PciCfgBase,
  IN UINTN        *SpiBaseAddress,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  );

#endif // _LPSS_SPI_PRIVATE_LIB_H_
