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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Register/LpssSpiRegs.h>
#include <LpssSpiPrivateLib.h>
#include <Library/PchLimits.h>
#include <LpssSpiConfig.h>

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
LpssSpiChipSelectConfig (
  IN UINTN  SpiBaseAddress,
  IN UINT8  CsOutput,
  IN UINT8  CsMode,
  IN UINT8  CsState,
  IN UINT8  CsEnableBits,
  IN UINT8  CsIdlePolarityBits
  )
{
  UINT8                         Index;
  LPSS_SPI_CS_CONTROL_REG       SpiCsControl;

  SpiCsControl.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SPI_CS_CONTROL);

  //
  // Set CS mode
  //
  SpiCsControl.Fields.CsMode = CsMode;

  for (Index = 0; Index < PCH_MAX_SERIALIO_SPI_CHIP_SELECTS; Index++) {
    if (CsEnableBits & (BIT0 << Index)) {
      //
      // Set CS polarity
      //
      if (((CsIdlePolarityBits >> Index) & BIT0) == LpssSpiCsActiveLow) {
        SpiCsControl.Data32 &= (UINT32) ~(BIT0 << (N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY + Index));
      } else {
        SpiCsControl.Data32 |= (UINT32) (BIT0 << (N_LPSS_SPI_MEM_SPI_CS_CONTROL_CS0_POLARITY + Index));
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
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SPI_CS_CONTROL, SpiCsControl.Data32);
}

/**
  Execute an SPI write
  - Read data is ingnored

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
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
LpssSpiWrite (
  IN UINT64       PciCfgBase,
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
  UINTN                       SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  UINT32                      SpiData;

  SpiBaseAddress = 0x0;

  if (WriteBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Error write buffer is not initialized\n", __FUNCTION__));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (WriteLength == 0) {
    DEBUG ((DEBUG_ERROR, "%a () - Error WriteLength is 0\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = LpssSpiInitializeWriteRead (
             PciCfgBase,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Error during initialization\n", __FUNCTION__));
    return Status;
  }

  Timeout = TimeBudget;
  while ((WriteLength > 0) && (Timeout > 0)) {
    if (LpssSpiTxFifoNotFull (SpiBaseAddress)) {
      SpiData = *WriteBuffer;
      MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR, SpiData);
      WriteBuffer++;
      WriteLength--;
      MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR);
    }
    MicroSecondDelay (LpssSpiSingleOperationDelay);
    Timeout--;
  }
  LpssSpiWaitIdle (SpiBaseAddress);

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "%a () - Error Timeout\n", __FUNCTION__));
    Status = EFI_TIMEOUT;
  }

  LpssSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}

/**
  Execute an SPI read command
  - Write data is always 0x0

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
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
LpssSpiRead (
  IN UINT64       PciCfgBase,
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
  UINTN                       SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  UINT32                      SpiData;

  SpiBaseAddress = 0x0;

  if (ReadBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Error read buffer is not initialized.\n", __FUNCTION__));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (ReadLength == 0) {
    DEBUG ((DEBUG_ERROR, "%a () - Error ReadLength is 0.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = LpssSpiInitializeWriteRead (
             PciCfgBase,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Error during initialization.\n", __FUNCTION__));
    return Status;
  }

  Timeout = TimeBudget;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR, 0x0);
  while ((ReadLength > 0) && (Timeout > 0)) {
    if (LpssSpiRxFifoNotEmpty (SpiBaseAddress)) {
      SpiData = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR);
      *ReadBuffer = (UINT8) SpiData;
      ReadBuffer++;
      ReadLength--;
      //
      // Issue another write only when device responded with the data on the previous read
      //
      if (ReadLength > 0) {
        MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR, 0x0);
      }
    }
    MicroSecondDelay (LpssSpiSingleOperationDelay);
    Timeout--;
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "%a () -  Error Timeout\n", __FUNCTION__));
    Status = EFI_TIMEOUT;
  }

  LpssSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}

/**
  Execute an SPI write-read operation

  @param[in]   PciCfgBase        Pci Config Base Address of specified SPI controller
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
LpssSpiWriteRead (
  IN UINT64       PciCfgBase,
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
  UINTN                       SpiBaseAddress;
  EFI_STATUS                  Status;
  UINT64                      Timeout;
  UINT32                      SpiData;
  BOOLEAN                     RxDataIn;

  SpiBaseAddress = 0x0;

  if (WriteBuffer == NULL || ReadBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Error buffers are not initialized\n", __FUNCTION__));
    return EFI_BAD_BUFFER_SIZE;
  }

  if (TransactionLength == 0) {
    DEBUG ((DEBUG_ERROR, "%a () - Error TransactionLength is 0\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = LpssSpiInitializeWriteRead (
             PciCfgBase,
             &SpiBaseAddress,
             (UINT32) Frequency,
             ClockIdlePolarity,
             ClockPhase,
             CsOutputSelect,
             CsActivePolarity,
             AssertCs
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Error during initialization\n", __FUNCTION__));
    return Status;
  }

  Timeout = TimeBudget;
  RxDataIn = TRUE;
  while ((TransactionLength > 0) && (Timeout > 0)) {
    if ((RxDataIn == TRUE) &&  LpssSpiTxFifoNotFull (SpiBaseAddress)) {
      SpiData = *WriteBuffer;
      MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR, SpiData);
      WriteBuffer++;
      MicroSecondDelay (LpssSpiSingleOperationDelay);
      RxDataIn = FALSE;
    }
    if ((RxDataIn == FALSE) && LpssSpiRxFifoNotEmpty (SpiBaseAddress)) {
      SpiData = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSDR);
      *ReadBuffer = (UINT8) SpiData;
      RxDataIn = TRUE;
      ReadBuffer++;
      TransactionLength--;
    }
    Timeout--;
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "%a () - Error Timeout.\n", __FUNCTION__));
    Status = EFI_TIMEOUT;
  }

  LpssSpiDeactivate (SpiBaseAddress, CsOutputSelect, CsActivePolarity);

  return Status;
}
