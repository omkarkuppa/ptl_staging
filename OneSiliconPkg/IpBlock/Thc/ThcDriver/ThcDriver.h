/** @file
  Touch Host Controller Driver API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

/**
  Sets Frequency based on Touch Capabilities MAX_SPI_FREQUENCY_SUPPORTED accordingly to EDS

  @param[in]  ThcDev          Context of Thc device

  @retval EFI_SUCCESS         Thc frequency set successfuly
  @retval EFI_DEVICE_ERROR    Error during frequency mapping with EDS
**/
EFI_STATUS
ThcSetFrequency (
  IN THC_DEV                *ThcDev
  );

/**
  Initialize Thc according to procedure described in EDS

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
ThcInitialize (
  IN THC_DEV                *ThcDev
  );

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
ThcReadConfig (
  IN THC_DEV                *ThcDev
  );

/**
  Prints end Device Configuration

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc initialized successfully
  @retval other           Error during initialization
**/
VOID
ThcPrintConfig (
  IN  THC_DEV                *ThcDev
  );

/**
  Enables Touch Panel connected to the Touch Host Controller

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Touch Panel Enabled
  @retval other                  Error during device enabling process
**/
EFI_STATUS
ThcEnableEndDevice (
  IN  THC_DEV              *ThcDev
  );

/**
  Disables Touch Panel connected to the Touch Host Controller

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Touch Panel Disabled
  @retval other                  Error during device enabling process
**/
EFI_STATUS
ThcDisableEndDevice (
  IN THC_DEV                *ThcDev
  );

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  ThcDev             Context of Thc device

  @retval EFI_SUCCESS            Thc Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
ThcDmaCreatePrdCommonBuffers (
  IN  THC_DEV                *ThcDev
  );

/**
  Fills single Prd Table with the defualt values

  @param[in]  ThcDma           Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table

**/
VOID
ThcDmaFillSinglePrdTable (
  THC_DMA                        *ThcDma,
  UINT16                         PrdTableIndex
  );

/**
  Fills every Prd Table with the defualt values

  @param[in]  ThcDma           Dma that stores Driver PRD Table

**/
VOID
ThcDmaFillPrdTables (
  THC_DMA                        *ThcDma
  );

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  ThcDev           Context of Thc device
  @param[in]  ThcDma           Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            Thc Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
ThcDmaCreatePrds (
  IN THC_DEV                        *ThcDev,
  IN THC_DMA                        *ThcDma,
  IN EFI_PCI_IO_PROTOCOL_OPERATION  PciIoOperation
  );

/**
  Initialize Thc DMA

  @param[in]  ThcDev      Context of Thc device

  @retval EFI_SUCCESS     Thc Dma Initialized successfuly
  @retval other           error during initialization
**/
EFI_STATUS
ThcDmaInitialize (
  IN THC_DEV                *ThcDev
  );

/**
  Calculates the length of the current message

  @param[in]  ThcDma             Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
ThcDmaGetMessageLength (
  IN THC_DMA                        *ThcDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  );

/**
  Reads first PRD Table Entry and checks if header data type
  equals to TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the Thc Context.

  @param[in]  ThcDev             Context of Thc device
  @param[in]  ThcDma             Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller then its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller then one present in Prd
**/
EFI_STATUS
ThcDmaReadSingleReport (
  IN THC_DEV                *ThcDev,
  IN THC_DMA                *ThcDma,
  IN UINT16                 CurrentPrdTable
  );

/**
  Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  ThcDev                    Context of Thc device
  @param[in]  CurrentCBReadPointer      Circullar Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circullar Buffer Write Pointer

**/
VOID
ThcDmaReadRx2 (
  IN THC_DEV                *ThcDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  );

/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  ThcDev             Context of Thc device
  @param[in]  Buffer             Data Container that will be sent to external device
  @param[in]  DataHeader         Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
ThcDmaWriteTx (
  IN THC_DEV                *ThcDev,
  IN UINT8                  *Buffer,
  IN THC_WRITE_DATA_HDR     *DataHeader
  );

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  ThcDev             Context of Thc device
  @param[in]  ThcDma             Dma that stores Driver PRD Table

**/
VOID
ThcDmaFreePrds (
  IN THC_DEV                        *ThcDev,
  IN THC_DMA                        *ThcDma
  );

/**
  Stop Touch Panel, brings it back to reset state
  Freees all allocated mem

  @param[in]  ThcDev         Context of Thc device

**/
VOID
EFIAPI
ThcStop (
  IN THC_DEV                *ThcDev
  );

/**
  Polls on Thc Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is trigerred

  @param[in]  Event       Polling event
  @param[in]  Context     Context of Thc device

**/
VOID
EFIAPI
ThcPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Enables end point device.
  - Reads all Touch Panels devices registers
  - Sets TouchEnable

  @param[in]  ThcDev           Context of Thc device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
ThcEnableAfterReset (
  IN  THC_DEV              *ThcDev
  );

/**
  Reads HID Descriptor for non EDS complaint devices that do not support Single Finger Report.
  Flow:
  - send TCTL_REQUEST with Get Hid desc command
  - waits for response
  - Run Descriptor parser and populate all known Report Ids in Thc HID_INPUT_REPORT_TABLE

  @param[in]  ThcDev           Context of Thc device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond to TCTLCMD
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for Thc HID support
**/
EFI_STATUS
ThcReadHidDescriptor (
  IN  THC_DEV              *ThcDev
  );
