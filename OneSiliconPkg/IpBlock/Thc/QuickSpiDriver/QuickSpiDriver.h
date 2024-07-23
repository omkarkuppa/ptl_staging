/** @file
  Touch Host Controller QuickSpi Driver API

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
#include <ThcConfig.h>

/**
  Initialize QuickSpi according to procedure described in EDS

  @param[in]  QuickSpiDev      Context of QuickSpi device
  @param[in]  HidOverSpi       Hid Over Spi policy data
  @param[in]  Reset            Reset data

  @retval EFI_SUCCESS     QuickSpi initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiInitialize (
  IN QUICK_SPI_DEV                *QuickSpiDev,
  IN THC_HID_OVER_SPI             *HidOverSpi,
  IN THC_RESET                    *Reset
  );

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS     QuickSpi initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickSpiReadDeviceDescriptor (
  IN QUICK_SPI_DEV                *QuickSpiDev
  );

/**
  Prints end Device Configuration

  @param[in]  QuickSpiDev      Context of QuickSpi device
**/
VOID
QuickSpiPrintDeviceDescriptor (
  IN  QUICK_SPI_DEV    *QuickSpiDev
  );

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  QuickSpiDev        Context of QuickSpi device

  @retval EFI_SUCCESS            QuickSpi Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickSpiDmaCreatePrdCommonBuffers (
  IN  QUICK_SPI_DEV                *QuickSpiDev
  );

/**
  Fills single Prd Table with the defualt values

  @param[in]  QuickSpiDma      Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table
**/
VOID
QuickSpiDmaFillSinglePrdTable (
  IN THC_DMA    *QuickSpiDma,
  IN UINT16     PrdTableIndex
  );

/**
  Fills every Prd Table with the defualt values

  @param[in]  QuickSpiDma           Dma that stores Driver PRD Table
**/
VOID
QuickSpiDmaFillPrdTables (
  THC_DMA                        *QuickSpiDma
  );

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  QuickSpiDev      Context of QuickSpi device
  @param[in]  QuickSpiDma      Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            QuickSpi Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickSpiDmaCreatePrds (
  IN QUICK_SPI_DEV                  *QuickSpiDev,
  IN THC_DMA                        *QuickSpiDma,
  IN EFI_PCI_IO_PROTOCOL_OPERATION  PciIoOperation
  );

/**
  Initialize QuickSpi DMA

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS     QuickSpi Dma Initialized successfuly
  @retval other           error during initialization
**/
EFI_STATUS
QuickSpiDmaInitialize (
  IN QUICK_SPI_DEV                *QuickSpiDev
  );

/**
  Calculates the length of the current message

  @param[in]  QuickSpiDma        Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
QuickSpiDmaGetMessageLength (
  IN THC_DMA                        *QuickSpiDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  );

/**
  Reads first PRD Table Entry and checks if header data type
  equals to TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickSpi Context.

  @param[in]  QuickSpiDev        Context of QuickSpi device
  @param[in]  QuickSpiDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller then its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller then one present in Prd
**/
EFI_STATUS
QuickSpiDmaReadSingleReport (
  IN QUICK_SPI_DEV          *QuickSpiDev,
  IN THC_DMA                *QuickSpiDma,
  IN UINT16                 CurrentPrdTable
  );

/**
  Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  QuickSpiDev               Context of QuickSpi device
  @param[in]  CurrentCBReadPointer      Circullar Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circullar Buffer Write Pointer

**/
VOID
QuickSpiDmaReadRx2 (
  IN QUICK_SPI_DEV          *QuickSpiDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  );

/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  QuickSpiDev        Context of QuickSpi device
  @param[in]  Buffer             Data Container that will be sent to external device
  @param[in]  DataHeader         Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickSpiDmaWriteTx (
  IN QUICK_SPI_DEV                      *QuickSpiDev,
  IN QUICK_SPI_OUTPUT_REPORT_HEADER     OutputHeader,
  IN UINT8                              *DataBuffer
  );

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  QuickSpiDev             Context of QuickSpi device
  @param[in]  QuickSpiDma             Dma that stores Driver PRD Table

**/
VOID
QuickSpiDmaFreePrds (
  IN QUICK_SPI_DEV                  *QuickSpiDev,
  IN THC_DMA                        *QuickSpiDma
  );

/**
  Stop Touch Panel, brings it back to reset state
  Frees all allocated memory and QuickSpi device context

  @param[in]  QuickSpiDev         Context of QuickSpi device
  @param[in]  HidOverSpi          Hid Over Spi policy data
  @param[in]  Reset               Reset data

**/
VOID
EFIAPI
QuickSpiStop (
  IN QUICK_SPI_DEV                *QuickSpiDev,
  IN THC_HID_OVER_SPI             *HidOverSpi,
  IN THC_RESET                    *Reset
  );

/**
  Polls on QuickSpi Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is triggered

  @param[in]  Event       Polling event
  @param[in]  Context     Context of QuickSpi device

**/
VOID
EFIAPI
QuickSpiPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Enables end point device.
 - TBD

  @param[in]  QuickSpiDev           Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiEnableAfterReset (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  );

/**
  Reads HID Descriptor

  @param[in]  QuickSpiDev           Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond to TCTLCMD
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickSpi HID support
**/
EFI_STATUS
QuickSpiReadHidDescriptor (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  );

/**
  Touch Enable

  @param[in]  QuickSpiDev      Context of QuickSpi device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickSpi HID support
**/
EFI_STATUS
QuickSpiTouchEnable (
  IN  QUICK_SPI_DEV              *QuickSpiDev
  );
