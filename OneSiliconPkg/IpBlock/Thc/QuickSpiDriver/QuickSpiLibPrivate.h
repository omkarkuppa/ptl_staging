/** @file
  Touch Host Controller QuickSpi Private Lib for Driver usage

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

#include "QuickSpiPrivate.h"

/**
  Sets TSSGO bit to start a cycle

  @param[in]  MmioBase         QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibGo (
  IN UINT64  MmioBase
  );

/**
  Sets Edge Interrupt mode

  @param[in]  MmioBase         QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetEdgeInterrupts (
  IN UINT64  MmioBase
  );

/**
  Returns last received interrupt of THC_INTERRUPT_TYPE

  @param[in]  MmioBase     QuickSpi MMIO BAR0

  @retval                  One of THC_INTERRUPT_TYPE
**/
UINT8
QuickSpiLibInterruptStatus (
  IN UINT64  MmioBase
  );

/**
  Waits for SPI_IO_RDY BIT after it was moved to D0

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval  EFI_SUCCESS       Device is ready
  @retval  EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibIoReady (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Start Quiesce to de-initialize THC

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for HW Sts bit to be 1

  @retval EFI_SUCCESS       Quiesce Enabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibStartQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Ends Quiesce

  @param[in] MmioBase       QuickSpi MMIO BAR0
  @param[in] Timeout        [MILLISECONDS] Indicates how long driver waits for HW STS response

  @retval EFI_SUCCESS       Quiesce Disabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickSpiLibEndQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Check if Quiesce is disabled

  @param[in]  MmioBase  QuickSpi MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
QuickSpiLibIsQuiesceDisabled (
  IN UINT64   MmioBase
  );

/**
  Sets Port Frequency

  @param[in] MmioBase          QuickSpi MMIO BAR0
  @param[in] Frequency         Read & Write Frequency
**/
VOID
QuickSpiLibSetFrequency (
  IN UINT64               MmioBase,
  IN UINT32               Frequency
  );

/**
  Sets Single IO mode for both read and write

  @param[in] MmioBase          QuickSpi MMIO BAR0
  @param[in] IoMode            Single/Dual/Quad

**/
VOID
QuickSpiLibSetIoMode (
  IN UINT64   MmioBase,
  IN UINT8    IoMode
  );

/**
  Power up device through RST GPIO

  @param[in] MmioBase           QuickSpi MMIO BAR0
  @param[in] Timeout            [MILLISECONDS] Indicates how long driver waits for device to power up
  @param[in] InstanceId         SPI instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
VOID
QuickSpiLibGetOutOfReset (
  IN UINT64                    MmioBase,
  IN UINT32                    Timeout,
  IN UINT8                     InstanceId,
  IN UINT32                    ResetPadTrigger
  );

/**
  Power up device through RST GPIO

  @param[in] MmioBase           QuickSpi MMIO BAR0
  @param[in] InstanceId         SPI instance id
  @param[in] ResetPadTrigger    Reset Pad Trigger

**/
VOID
QuickSpiLibSetReset (
  IN UINT64                    MmioBase,
  IN UINT8                     InstanceId,
  IN UINT32                    ResetPadTrigger
  );

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickSpiLibWaitForResetOccured (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Waits until cycle is completed

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          Cycle completed
  @retval EFI_DEVICE_ERROR     ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiLibWaitCycleCompleted (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Checks if no cycles are pending

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickSpiLibNoCyclesPending (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Prepares THC to return its configuration

  @param[in]  MmioBase          QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibPrepareReadDevRegCmd (
  IN UINT64   MmioBase
  );

/**
  Prepares THC to return its configuration

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickSpi MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  SpiAddressRegister     SPI Address Register
**/
VOID
QuickSpiLibPreparePioOperation (
  IN PIO_OP   Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT32   SpiAddressRegister
  );

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickSpi MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickSpiLibWaitForDeviceDescriptor (
  IN UINT64   MmioBase,
  IN UINT32   Timeout,
  IN OUT UINT32   *DeviceDescriptor
  );

/**
  Reads THC device registers

  @param[in]      MmioBase      QuickSpi MMIO BAR0
  @param[in/out]  ReturnData    Return Data Pointer

  @retval EFI_SUCCESS              Cycle completed
  @retval EFI_DEVICE_ERROR         Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickSpiLibReadComplete (
  IN     UINT64  MmioBase,
  IN OUT UINT32  *ReturnData
  );

/**
  Write to sequencing Data 1

  @param[in]  MmioBase         QuickSpi MMIO BAR0
  @param[in]  Data             Data

**/
VOID
QuickSpiLibWriteSeqData1 (
  IN UINT64  MmioBase,
  IN UINT32  Data
  );

/**
  Sets Prd Tables Adddress in THC MMIO

  @param[in]  MmioBase            QuickSpi MMIO BAR0
  @param[in]  WriteAddr           Write Prd Address
  @param[in]  Read2Addr           Read2 Prd Address

  @retval EFI_SUCCESS             Cycle completed
  @retval EFI_INVALID_PARAMETER   Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickSpiLibSetPrdTablesAddress (
  IN UINT64  MmioBase,
  IN UINT64  WriteAddr,
  IN UINT64  Read2Addr
  );

/**
  Sets PrdTable/CircullarBuffer length

  @param[in]  MmioBase        QuickSpi MMIO BAR0
  @param[in]  ReadEntryCount  Number Of Read Entries
  @param[in]  ReadCBDepth     Read Circullar Buffer Depth
  @param[in]  WriteEntryCount Number of Write Entries

**/
VOID
QuickSpiLibSetLengthInPrd (
  IN UINT64  MmioBase,
  IN UINT8   ReadEntryCount,
  IN UINT8   ReadCBDepth,
  IN UINT8   WriteEntryCount
  );

/**
  Sets Write DMA start bit

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetWriteStartBit (
  IN UINT64  MmioBase
  );

/**
  Polls until DMA Complete bit is set

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long polling lasts

  @retval  EFI_SUCCESS       DMA transfer is complete
  @retval  EFI_TIMEOUT       Timeout reached
**/
EFI_STATUS
QuickSpiLibPollDmaComplete (
  IN UINT64  MmioBase,
  IN UINT32  Timeout
  );

/**
  Sets Read Rx2 DMA start bit

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibSetReadRx2StartBit (
  IN UINT64  MmioBase
  );

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase       QuickSpi MMIO BAR0
  @param[in]  WritePointer   PRD CB Write Pointer

**/
VOID
QuickSpiLibSetReadRx2WritePointer (
  IN UINT64  MmioBase,
  IN UINT8   WritePointer
  );

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase           QuickSpi MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
UINT32
QuickSpiLibGetReadRx2Data (
  IN UINT64  MmioBase
  );

/**
  Prepares Touch Panel and THC for OS hand off

  @param[in]  MmioBase           QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibCleanUp (
  IN UINT64  MmioBase
  );

/**
  Read Int Cause Register in PIO mode.

  @param[in]  MmioBase          QuickSpi MMIO BAR0

**/
VOID
QuickSpiLibPioReadIntCause (
  IN UINT64   MmioBase
  );

/**
  Configures THC registers for QuickSpi support

  @param[in]  MmioBase         QuickSpi MMIO BAR0
**/
VOID
QuickSpiLibConfigure (
  IN UINT64  MmioBase,
  IN UINT32  InputReportHeaderAddress,
  IN UINT32  InputReportBodyAddress,
  IN UINT32  OutputReportAddres,
  IN UINT8   SioReadOpCode,
  IN UINT8   DioReadOpCode,
  IN UINT8   QioReadOpCode,
  IN UINT8   QpioReadOpCode,
  IN UINT8   SioWriteOpCode,
  IN UINT8   DioWriteOpCode,
  IN UINT8   QioWriteOpCode,
  IN UINT8   QpioWriteOpCode,
  IN UINT8   LimitPacketSize
  );

/**
  Gets Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickSpiLibGetReadRx2IntSts (
  IN UINT64  MmioBase
  );

/**
  Stops DMA traffic by clearing Start BIT and CB Pointer Reset for the 2nd RXDMA Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
QuickSpiLibStopDma (
  IN UINT64  MmioBase
  );