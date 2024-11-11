/** @file
  Touch Host Controller QuickI2c Private Lib for Driver usage

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

#include "QuickI2cPrivate.h"

/**
  Sets TSSGO bit to start a cycle

  @param[in]  MmioBase         QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibGo (
  IN UINT64  MmioBase
  );

/**
  Sets Edge Interrupt mode

  @param[in]  MmioBase         QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetEdgeInterrupts (
  IN UINT64  MmioBase
  );

/**
  Returns last received interrupt of THC_INTERRUPT_TYPE

  @param[in]  MmioBase     QuickI2c MMIO BAR0

  @retval                  One of THC_INTERRUPT_TYPE
**/
UINT8
QuickI2cLibInterruptStatus (
  IN UINT64  MmioBase
  );

/**
  Start Quiesce to de-initialize THC

  @param[in]  MmioBase      QuickI2c MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for HW Sts bit to be 1

  @retval EFI_SUCCESS       Quiesce Enabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickI2cLibStartQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Ends Quiesce

  @param[in] MmioBase       QuickI2c MMIO BAR0
  @param[in] Timeout        [MILLISECONDS] Indicates how long driver waits for HW STS response

  @retval EFI_SUCCESS       Quiesce Disabled
  @retval EFI_DEVICE_ERROR  Timeout reached - error during initialization
**/
EFI_STATUS
QuickI2cLibEndQuiesce (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Check if Quiesce is disabled

  @param[in]  MmioBase  QuickI2c MMIO BAR0

  @retval  TRUE         Quiesce is Disabled
  @retval  FALSE        Quiesce is Enabled
**/
BOOLEAN
QuickI2cLibIsQuiesceDisabled (
  IN UINT64   MmioBase
  );

/**
  Check if Quiesce is Enabled

  @param[in]  MmioBase  QuickI2c MMIO BAR0

  @retval  TRUE         Quiesce is Enabled
  @retval  FALSE        Quiesce is Disabled
**/
BOOLEAN
QuickI2cLibIsQuiesceEnabled (
  IN UINT64   MmioBase
  );

/**
  Waits for TOUCH_INT_CAUSE to return ResetOccurred

  @param[in]  MmioBase      QuickI2c MMIO BAR0
  @param[in]  Timeout       [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS       Reset was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibWaitForResetOccured (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Waits until cycle is completed

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          Cycle completed
  @retval EFI_DEVICE_ERROR     ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cLibWaitCycleCompleted (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Checks if no cycles are pending

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Timeout          [MILLISECONDS] Indicates how long driver waits for device to become ready

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cLibNoCyclesPending (
  IN UINT64   MmioBase,
  IN UINT32   Timeout
  );

/**
  Prepares THC to return its configuration

  @param[in]  MmioBase          QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibPrepareReadDevRegCmd (
  IN UINT64   MmioBase
  );

/**
  Prepares THC to return its configuration

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickI2c MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  I2CAddressRegister     I2C Address Register
**/
VOID
QuickI2cLibPreparePioOperation (
  IN THC_I2C_PIO_OP    Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT32   I2CAddressRegister
  );

/**
  Prepares THC to return its configuration

  @param[in]  Operation              Operation Command
  @param[in]  MmioBase               QuickI2c MMIO BAR0
  @param[in]  ByteCount              Number of bytes
  @param[in]  I2CAddressRegister     I2C Address Register
**/
VOID
QuickI2cLibPrepareRwPioOperation (
  IN THC_I2C_PIO_OP Operation,
  IN UINT64   MmioBase,
  IN UINT16   ByteCount,
  IN UINT16   WriteCount,
  IN UINT32   I2CAddressRegister
  );

/**
  Reads THC device registers

  @param[in]      MmioBase      QuickI2c MMIO BAR0
  @param[in/out]  ReturnData    Return Data Pointer

  @retval EFI_SUCCESS              Cycle completed
  @retval EFI_DEVICE_ERROR         Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickI2cLibReadComplete (
  IN     UINT64  MmioBase,
  IN OUT UINT32  *ReturnData
  );

/**
  Write to sequencing Data 1

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  Data             Data

**/
VOID
QuickI2cLibWriteSeqData1 (
  IN UINT64  MmioBase,
  IN UINT32  Data
  );

/**
  Sets Prd Tables Adddress in THC MMIO

  @param[in]  MmioBase            QuickI2c MMIO BAR0
  @param[in]  WriteAddr           Write Prd Address
  @param[in]  Read2Addr           Read2 Prd Address

  @retval EFI_SUCCESS             Cycle completed
  @retval EFI_INVALID_PARAMETER   Returned Byte Count is too big or empty
**/
EFI_STATUS
QuickI2cLibSetPrdTablesAddress (
  IN UINT64  MmioBase,
  IN UINT64  WriteAddr,
  IN UINT64  Read2Addr,
  IN UINT64  SwDmaReadAddr
  );

/**
  Sets PrdTable/CircullarBuffer length

  @param[in]  MmioBase              QuickI2c MMIO BAR0
  @param[in]  ReadEntryCount        Number Of Read Entries
  @param[in]  ReadCBDepth           Read Circullar Buffer Depth
  @param[in]  WriteEntryCount       Number of Write Entries
  @param[in]  SwDmaReadEntryCount   Number Of Read Entries
  @param[in]  SwDmaReadCBDepth      Read Circullar Buffer Depth

**/
VOID
QuickI2cLibSetLengthInPrd (
  IN UINT64  MmioBase,
  IN UINT8   ReadEntryCount,
  IN UINT8   ReadCBDepth,
  IN UINT8   WriteEntryCount,
  IN UINT8   SwDmaReadEntryCount,
  IN UINT8   SwDmaReadCBDepth
  );

/**
  Sets Write DMA start bit

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetWriteStartBit (
  IN UINT64  MmioBase
  );

/**
  Sets Read SwDMA start bit

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSwDmaSetReadStartBit (
  IN UINT64  MmioBase
  );

/**
  Check Write DMA status

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibCheckWriteDmaStatus (
  IN UINT64  MmioBase
  );

/**
  Polls until DMA Complete bit is set

  @param[in]  MmioBase       QuickI2c MMIO BAR0
  @param[in]  Timeout        [MILLISECONDS] Indicates how long polling lasts

  @retval  EFI_SUCCESS       DMA transfer is complete
  @retval  EFI_TIMEOUT       Timeout reached
**/
EFI_STATUS
QuickI2cLibPollDmaComplete (
  IN UINT64  MmioBase,
  IN UINT32  Timeout
  );

/**
  Sets Read Rx2 DMA start bit

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibSetReadRx2StartBit (
  IN UINT64  MmioBase
  );

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase       QuickI2c MMIO BAR0
  @param[in]  WritePointer   PRD CB Write Pointer

**/
VOID
QuickI2cLibSetReadRx2WritePointer (
  IN UINT64  MmioBase,
  IN UINT8   WritePointer
  );

/**
  Sets Read Rx2 DMA write pointer

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
UINT32
QuickI2cLibGetReadRx2Data (
  IN UINT64  MmioBase
  );

/**
  Prepares Touch Panel and THC for OS hand off

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cLibCleanUp (
  IN UINT64  MmioBase
  );

/**
  Gets Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickI2cLibGetReadRx2IntSts (
  IN UINT64  MmioBase
  );

/**
  Sets Read SwDMA Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

  @retval     Contents of Dma Interrupt Status 2 register
**/
UINT32
QuickI2cLibGetReadSwDmaRx2IntSts (
  IN UINT64  MmioBase
  );

/**
  Stops DMA traffic by clearing Start BIT and CB Pointer Reset for the 2nd RXDMA Read Rx2 DMA Interrupt Status

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
QuickI2cLibStopDma (
  IN UINT64  MmioBase
  );

/**
  Sets THC to work in I2C mode

  @param[in]  MmioBase           Thc MMIO BAR0

**/
VOID
QuickI2cLibSetI2cPort (
  IN UINT64  MmioBase
  );

/**
  Reads I2C Ip register

  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in/out] Data           I2C register output data

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibReadSubIpRegister (
  IN UINT64   MmioBase,
  IN UINT32   RegisterAddress,
  IN OUT UINT32 *Data
  );

/**
  Perfomrs PIO read operation
  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in]  DataSize          Data Size
  @param[out] Data              I2C register output data
  @retval EFI_SUCCESS       Read was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibPerformPioRead (
  IN UINT64       MmioBase,
  IN UINT32       RegisterAddress,
  IN UINT16       DataSize,
  IN OUT UINT32   *Data
  );

/**
  Writes to I2C Ip register

  @param[in]  MmioBase          QuickI2c MMIO BAR0
  @param[in]  RegisterAddress   I2C register address
  @param[in]  Data              I2C register input data

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_TIMEOUT       Timeout reached - might be expected
**/
EFI_STATUS
QuickI2cLibWriteSubIpRegister (
  IN UINT64   MmioBase,
  IN UINT32   RegisterAddress,
  IN UINT32   Data
  );

  /**
  For serial debugger used, it will show the buffer message line by line to serial console.

  @param[in] Message              The address point of buffer message
  @param[in] Length               Message length

**/
VOID
ShowBuffer (
  IN  UINT8                       *Message,
  IN  UINT32                      Length
  );

  /**
  Wait for RxDma2 status for Pause

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
EFI_STATUS
QuickI2cWaitForRxDMA2Pause(
  IN UINT64  MmioBase
);

/**
  Reset DMA settings

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
VOID
QuickI2cResetDmaSettings (
  IN UINT64  MmioBase
  );

/**
  Writes data to a QuickI2c Sw DMA PRD Table

  @param[in]  QuickI2cDev                     Context of QuickI2c device
  @param[in]  DataBuffer                      Write Data Header
  @param[in]  RxDlenEn                        Context of QuickI2c device
  @param[in]  Wbc                             Data Container that will be sent to external device
  @param[in]  WriteDataHeader                 Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickI2cSwDma (
  IN QUICK_I2C_DEV                      *QuickI2cDev,
  IN UINT32                             *DataBuffer,
  IN UINT32                             RxDlenEn,
  IN UINT32                             Wbc,
  IN UINT32                             WriteDataHeader
  );

  /**
  Reset SwDMA settings

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
void QuickI2cResetSwdmaSettings (
  IN UINT64  MmioBase
  );

/**
  Start SwDma Operation

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cStartWriteSwdma (
  IN UINT64   MmioBase,
  IN UINT32   Address,
  IN UINT32   Data0
  );

  /**
  Confure SwDMA and program PRD table

  @param[in]  MmioBase           QuickI2c MMIO BAR0

  @retval     Contents of Dma Control 2 register
**/
EFI_STATUS
QuickI2cSwDmaConfigure (
  IN QUICK_I2C_DEV                      *QuickI2cDev
  );

/**
  Complete SwDma Operation

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cCompleteSwdma (
    IN QUICK_I2C_DEV                      *QuickI2cDev
  );

/**
  Wait For Sw RxDma Pause

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
EFI_STATUS
QuickI2cWaitForSwRxDMAPause (
  IN  UINT64 MmioBase
  );

/**
  Program SwDma read length

  @param[in]  MmioBase           QuickI2c MMIO BAR0

**/
VOID
QuickI2cSwDmaProgramReadLen(
  IN  UINT64 MmioBase,
  IN  UINT32 RxDlenEn,
  IN  UINT32 Wbc,
  IN  UINT32 wReportDescLength
  );

/**
  Send Power request to SubIP

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  PowerState       PowerState

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendSetPowerRequest(
  IN  QUICK_I2C_DEV    *QuickI2cDev,
  IN  UINT8            PowerState
  );

  /**
  Send Reset request to SubIP

  @param[in]  MmioBase         QuickI2c MMIO BAR0

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendResetRequest(
  IN QUICK_I2C_DEV      *QuickI2cDev
  );