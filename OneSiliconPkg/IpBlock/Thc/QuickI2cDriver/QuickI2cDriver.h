/** @file
  Touch Host Controller QuickI2c Driver API

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
#include <ThcConfig.h>


/**
  Initialize QuickI2c according to procedure described in EDS

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidOverI2c       Hid Over I2C policy data
  @param[in]  Reset            Reset data

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cInitialize (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c,
  IN THC_RESET                    *Reset
  );

/**
  Configures THC registers with default value for QuickI2c support

  @param[in]  HidOverI2c         QuickI2c HidOverI2c struct pointer
**/
VOID
QuickI2cLibConfigureDefaultPolicy (
  IN THC_HID_OVER_I2C  *HidOverI2c
  );

/**
  Perform SubIP reset process after initialization

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidOverI2c       Hid Over I2C policy data
  @param[in]  Reset            Reset data

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cTakeOutOfReset (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c,
  IN THC_RESET                    *Reset
  );

/**
  Perform SubIP reset process after initialization

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cPerformTouchIcReset (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  THC_RESET              *Reset
  );

/**
  Power up device through RST GPIO

  @param[in] Timeout            [MILLISECONDS] Indicates how long driver waits for device to power up
  @param[in] InstanceId         instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
VOID
QuickI2cLibGpioGetOutOfReset (
  IN UINT32                    Timeout,
  IN UINT8                     InstanceId,
  IN UINT32                    ResetPadTrigger
  );

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidOverI2c       Hid Over I2c Context

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cReadDeviceDescriptor (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c
  );

/**
  Prints end Device Configuration

  @param[in]  QuickI2cDev      Context of QuickI2c device
**/
VOID
QuickI2cPrintDeviceDescriptor (
  IN  QUICK_I2C_DEV    *QuickI2cDev
  );

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  QuickI2cDev        Context of QuickI2c device

  @retval EFI_SUCCESS            QuickI2c Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickI2cDmaCreatePrdCommonBuffers (
  IN  QUICK_I2C_DEV                *QuickI2cDev
  );

/**
  Fills single Prd Table with the defualt values

  @param[in]  QuickI2cDma      Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table
**/
VOID
QuickI2cDmaFillSinglePrdTable (
  IN THC_DMA    *QuickI2cDma,
  IN UINT32     PrdTableIndex
  );

/**
  Fills every Prd Table with the defualt values

  @param[in]  QuickI2cDma           Dma that stores Driver PRD Table
**/
VOID
QuickI2cDmaFillPrdTables (
  THC_DMA                        *QuickI2cDma
  );

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  QuickI2cDma      Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            QuickI2c Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickI2cDmaCreatePrds (
  IN QUICK_I2C_DEV                  *QuickI2cDev,
  IN THC_DMA                        *QuickI2cDma,
  IN EFI_PCI_IO_PROTOCOL_OPERATION  PciIoOperation
  );

/**
  Initialize QuickI2c DMA

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS     QuickI2c Dma Initialized successfuly
  @retval other           error during initialization
**/
EFI_STATUS
QuickI2cDmaInitialize (
  IN QUICK_I2C_DEV                *QuickI2cDev
  );

/**
  Calculates the length of the current message

  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
QuickI2cDmaGetMessageLength (
  IN THC_DMA                        *QuickI2cDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  );

/**
  Reads first PRD Table Entry and checks if header data type
  equals to TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickI2c Context.

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller then its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller then one present in Prd
**/
EFI_STATUS
QuickI2cDmaReadSingleReport (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN THC_DMA                *QuickI2cDma,
  IN UINT16                 CurrentPrdTable
  );

/**
  Reads first PRD Table Entry and checks if header data type
  equals TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickI2c Context.

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller than its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller the one present in Prd
**/
EFI_STATUS
QuickI2cSwDmaReadSingleReport (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN THC_DMA                *QuickI2cDma,
  IN UINT16                 CurrentPrdTable
  );

/**
  Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  QuickI2cDev               Context of QuickI2c device
  @param[in]  CurrentCBReadPointer      Circullar Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circullar Buffer Write Pointer

**/
VOID
QuickI2cDmaReadRx2 (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  );

/**
  Clear RxDma2 interrupt status once read is complete

  @param[in]  QuickI2cDev               Context of QuickI2c device
  @param[in]  CurrentCBReadPointer      Circullar Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circullar Buffer Write Pointer

**/
VOID
QuickI2cLibClearReadRx2IntSts (
  IN UINT64  MmioBase
  );

/**
  Clear SwDma interrupt status once read is complete

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
VOID
QuickI2cLibClearSwDmaReadIntSts (
  IN UINT64  MmioBase
  );

/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  Buffer             Data Container that will be sent to external device
  @param[in]  DataHeader         Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickI2cDmaWriteTx (
  IN QUICK_I2C_DEV                      *QuickI2cDev,
  IN QUICK_I2C_OUTPUT_REPORT_HEADER     OutputHeader,
  IN UINT8                              *DataBuffer
  );

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  QuickI2cDev             Context of QuickI2c device
  @param[in]  QuickI2cDma             Dma that stores Driver PRD Table

**/
VOID
QuickI2cDmaFreePrds (
  IN QUICK_I2C_DEV                  *QuickI2cDev,
  IN THC_DMA                        *QuickI2cDma
  );

/**
  Stop Touch Panel, brings it back to reset state
  Frees all allocated memory and QuickI2c device context

  @param[in]  QuickI2cDev         Context of QuickI2c device
  @param[in]  HidOverI2c          Hid Over I2C policy data
  @param[in]  Reset               Reset data

**/
VOID
EFIAPI
QuickI2cStop (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c,
  IN THC_RESET                    *Reset
  );

/**
  Polls on QuickI2c Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is triggered

  @param[in]  Event       Polling event
  @param[in]  Context     Context of QuickI2c device

**/
VOID
EFIAPI
QuickI2cPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Polls on QuickI2c Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is triggered

  @param[in]  Event       Polling event
  @param[in]  Context     Context of QuickI2c device

**/
VOID
EFIAPI
QuickI2cPollingSwDma (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );


/**
  Enables end point device.
 - TBD

  @param[in]  QuickI2cDev           Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cEnableAfterReset (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  );

/**
  Reads HID Descriptor

  @param[in]  QuickI2cDev           Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond to TCTLCMD
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cReadHidDescriptor (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  );

/**
  Touch Enable

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cTouchEnable (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  );

/**
  Get feature via SwDma

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device Did not respond
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cGetFeatureViaSwDma (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  );

/**
  Reads data from each Read Sw DMA PRD Table Entry if write pointer is equal to read pointer

  @param[in]  QuickI2cDev               Context of QuickI2c device

**/
VOID
QuickI2cReadSwRxDma (
  IN QUICK_I2C_DEV          *QuickI2cDev
  );