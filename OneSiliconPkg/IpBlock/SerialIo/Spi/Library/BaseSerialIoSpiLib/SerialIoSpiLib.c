/** @file
  Basic SPI Library that allows SPI communication through Read/Write/WriteRead operations.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoSpiRegs.h>
#include <SerialIoSpiPrivateLib.h>
#include <Library/PchLimits.h>
#include <SerialIoDevices.h>

/**
  Configure Initial Chip Select settings

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
  @param[in]  CsOutput       - Default CS used by the SPI HC. 0 = CS0, 1 = CS1, CS2, CS3.
  @param[in]  CsMode         - Sets Chip Select Control mode Hardware or Software. 0 = HW Control, 1 = SW Control
  @param[in]  CsState        - 0 = CS is set to low; 1 = CS is set to high
  @param[in]  CsEnableBits         Bit based Cs Enable setting (BIT0 -> Enable CS0, BIT1 -> Enable CS1 etc.)
  @param[in]  CsIdlePolarityBits   Bit based Cs Polarity - Sets Inactive/Idle polarity of each CS
**/
VOID
SerialIoSpiChipSelectConfig (
  IN UINT64 SpiBaseAddress,
  IN UINT8  CsOutput,
  IN UINT8  CsMode,
  IN UINT8  CsState,
  IN UINT8  CsEnableBits,
  IN UINT8  CsIdlePolarityBits
  )
{
  UINT8                         Index;
  SERIAL_IO_SPI_CS_CONTROL_REG  SpiCsControl;

  SpiCsControl.Data32 = MmioRead32 ((UINTN) (SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL));

  //
  // Set CS mode
  //
  SpiCsControl.Fields.CsMode = CsMode;

  for (Index = 0; Index < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; Index++) {
    if (CsEnableBits & (BIT0 << Index)) {
      //
      // Set CS Polarity
      //
      if (((CsIdlePolarityBits >> Index) & BIT0) == SerialIoSpiCsActiveLow) {
        SpiCsControl.Data32 &= (UINT32) ~(BIT0 << (N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY + Index));
      } else {
        SpiCsControl.Data32 |= (UINT32) (BIT0 << (N_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY + Index));
      }
    }
  }
  //
  // Set CS Output Select
  //
  SpiCsControl.Fields.CsOutputSelect = CsOutput;
  //
  // Set CS state
  //
  SpiCsControl.Fields.CsState = CsState;
  MmioWrite32 ((UINTN) (SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SPI_CS_CONTROL), SpiCsControl.Data32);
}

/**
  Execute an SPI write
  - Read data is ingnored

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
                                 @Note: If SPI Number is provided this parameter is ignored and can be left as 0
  @param[in]   SpiNumber         Optional: SPI controller applicable on bus 0 only
  @param[in]   WriteLength       Write length of the current transaction
  @param[in]   WriteBuffer       Pointer to data in write buffer
  @param[in]   TimeBudget        Time allotted to verify if SPI is enabled
  @param[in]   Frequency         Bus clock frequency (in Hz)
  @param[in]   ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]   ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                 1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]   CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1 = CS1 etc.)
  @param[in]   CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]   AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                 TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_INVALID_PARAMETER   Invalid length parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in SPI disable/enable or transaction timeout
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Write was successful
  @retval   EFI_BAD_BUFFER_SIZE     Write buffer is NULL
**/
EFI_STATUS
SerialIoSpiWrite (
  IN UINTN        PciCfgBase,
  IN UINT8        *SpiNumber  OPTIONAL,
  IN UINT32       WriteLength,
  IN UINT8        *WriteBuffer,
  IN UINT64       TimeBudget,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  )
{
  UINT64                      SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  SERIAL_IO_SPI_DATA_REG      SpiData;

  SpiBaseAddress = 0x0;

  if (WriteBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWrite Error write buffer is not initialized\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (WriteLength == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWrite Error WriteLength is 0\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = SerialIoSpiInitializeWriteRead (
             PciCfgBase,
             SpiNumber,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWrite Error during initialization\n"));
    return Status;
  }

  Timeout = TimeBudget;
  while ((WriteLength > 0) && (Timeout > 0)) {
    if (SerialIoSpiTxFifoNotFull (SpiBaseAddress)) {
      SpiData.Data32 = *WriteBuffer;
      MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR, SpiData.Data32);
      WriteBuffer++;
      WriteLength--;
      MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR);
    }
    MicroSecondDelay (SpiSingleOperationDelay);
    Timeout--;
  }
  SerialIoSpiWaitIdle (SpiBaseAddress);

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWrite Error Timeout\n"));
    Status = EFI_TIMEOUT;
  }

  SerialIoSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}

/**
  Execute an SPI read command
  - Write data is always 0x0

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
                                 @Note: If SPI Number is provided this parameter is ignored and can be left as 0
  @param[in]   SpiNumber         Optional: SPI controller applicable on bus 0 only
  @param[in]   ReadLength        Read length of the current transaction
  @param[in]   ReadBuffer        Pointer to data in read buffer
  @param[in]   TimeBudget        Time allotted to verify if SPI is enabled
  @param[in]   Frequency         Bus clock frequency (in Hz)
  @param[in]   ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]   ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                 1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]   CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1 = CS1 etc.)
  @param[in]   CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]   AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                 TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_INVALID_PARAMETER   Invalid length parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in SPI disable/enable or transaction timeout
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Read was successful
  @retval   EFI_BAD_BUFFER_SIZE     Read buffer is NULL
**/
EFI_STATUS
SerialIoSpiRead (
  IN UINTN        PciCfgBase,
  IN UINT8        *SpiNumber  OPTIONAL,
  IN UINT32       ReadLength,
  IN UINT8        *ReadBuffer,
  IN UINT64       TimeBudget,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  )
{
  UINT64                      SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  SERIAL_IO_SPI_DATA_REG      SpiData;

  SpiBaseAddress = 0x0;

  if (ReadBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiRead Error read buffer is not initialized\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (ReadLength == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiRead Error ReadLength is 0\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = SerialIoSpiInitializeWriteRead (
             PciCfgBase,
             SpiNumber,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiRead Error during initialization\n"));
    return Status;
  }

  Timeout = TimeBudget;
  MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR, 0x0);
  while ((ReadLength > 0) && (Timeout > 0)) {
    if (SerialIoSpiRxFifoNotEmpty (SpiBaseAddress)) {
      SpiData.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR);
      *ReadBuffer = (UINT8) SpiData.Data32;
      ReadBuffer++;
      ReadLength--;
      //
      // Issue another write only when device responded with the data on the previous read
      //
      if (ReadLength > 0) {
        MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR, 0x0);
      }
    }
    MicroSecondDelay (SpiSingleOperationDelay);
    Timeout--;
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiRead Error Timeout\n"));
    Status = EFI_TIMEOUT;
  }

  SerialIoSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}

/**
  Execute an SPI write-read operation

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
                                 @Note: If SPI Number is provided this parameter is ignored and can be left as 0
  @param[in]   SpiNumber         Optional: SPI controller applicable on bus 0 only
  @param[in]   TransactionLength Read length of the current transaction
  @param[in]   WriteBuffer       Pointer to data in write buffer
  @param[in]   ReadBuffer        Pointer to data in read buffer
  @param[in]   TimeBudget        Time allotted to verify if SPI is enabled
  @param[in]   Frequency         Bus clock frequency (in Hz)
  @param[in]   ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]   ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                 1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]   CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1 = CS1 etc.)
  @param[in]   CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]   AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                 TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_INVALID_PARAMETER   Invalid length parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in SPI disable/enable or transaction timeout
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             WriteRead was successful
  @retval   EFI_BAD_BUFFER_SIZE     One of the buffers is NULL
**/
EFI_STATUS
SerialIoSpiWriteRead (
  IN UINTN        PciCfgBase,
  IN UINT8        *SpiNumber  OPTIONAL,
  IN UINT32       TransactionLength,
  IN UINT8        *WriteBuffer,
  IN UINT8        *ReadBuffer,
  IN UINT64       TimeBudget,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  )
{
  UINT64                      SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  SERIAL_IO_SPI_DATA_REG      SpiData;
  BOOLEAN                     RxDataIn;

  SpiBaseAddress = 0x0;

  if (WriteBuffer == NULL || ReadBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWriteRead Error buffers are not initialized\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (TransactionLength == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWriteRead Error TransactionLength is 0\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = SerialIoSpiInitializeWriteRead (
             PciCfgBase,
             SpiNumber,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWriteRead Error during initialization\n"));
    return Status;
  }

  Timeout = TimeBudget;
  RxDataIn = TRUE;
  while ((TransactionLength > 0) && (Timeout > 0)) {
    if ((RxDataIn == TRUE) &&  SerialIoSpiTxFifoNotFull (SpiBaseAddress)) {
      SpiData.Data32 = *WriteBuffer;
      MmioWrite32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR, SpiData.Data32);
      WriteBuffer++;
      MicroSecondDelay (SpiSingleOperationDelay);
      RxDataIn = FALSE;
      TransactionLength--;
    }
    if (SerialIoSpiRxFifoNotEmpty (SpiBaseAddress)) {
      SpiData.Data32 = MmioRead32 ((UINTN) SpiBaseAddress + R_SERIAL_IO_SPI_MEM_SSDR);
      *ReadBuffer = (UINT8) SpiData.Data32;
      RxDataIn = TRUE;
      ReadBuffer++;
    }
    Timeout--;
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "SerialIoSpiWriteRead Error Timeout\n"));
    Status = EFI_TIMEOUT;
  }

  SerialIoSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}

