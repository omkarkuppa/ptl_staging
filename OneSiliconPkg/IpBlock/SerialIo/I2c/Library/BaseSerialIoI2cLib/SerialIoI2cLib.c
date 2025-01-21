/** @file
  Basic I2C Librarary that allows I2C communication throguh Read/Write and Command Block operations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/PchPciBdfLib.h>
#include <Library/SerialIoI2cLib.h>
#include <Library/SerialIoI2cPrivateLib.h>
#include <Library/TimerLib.h>
#include <Library/PchInfoLib.h>

/**
  Execute an I2C write

  Addressing mode will defined based upon TargetAddress and mask:
  TargetAddress & 0x80000000 - if true will result in 10bit addressing
  Refer to: I2C_ADDRESSING_10_BIT

  @param[in]   PciCfgBase     Pci Config Base Address of specified I2C controller
                              @Note: If I2C Number is provided this parameter is ignored and can be left as 0
  @param[in]   I2cNumber      Optional: I2C controller applicable on bus 0 only
  @param[in]   TargetAddress  Address of the end point device
  @param[in]   WriteLength    Write length of the current transaction
  @param[in]   WriteBuffer    Pointer to data in write buffer
  @param[in]   TimeBudget     Time allotted to verify if I2C is enabled
  @param[in]   Frequency      Bus clock frequency (in kHz)
  @param[in]   TimingConfig   Optional: Pointer to struct passed down to override SDA Hold and SCL LCNT/HCNT settings
  @param[in]   Stop           Issues STOP on the last byte
  @param[in]   Restart        Indication if restart is needed, if TRUE will Initialize I2C controller.
                              @Note: Restart is required in first transaction.

  @retval   EFI_INVALID_PARAMETER   Invalid length / buffer parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in I2C disable/enable or wrong ENST value
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Write was successful
**/
EFI_STATUS
SerialIoI2cWrite (
  IN UINTN                 PciCfgBase,
  IN UINT8                 *I2cNumber  OPTIONAL,
  IN UINT32                TargetAddress,
  IN UINT32                WriteLength,
  IN UINT8                 *WriteBuffer,
  IN UINT64                TimeBudget,
  IN UINTN                 Frequency,
  IN SERIAL_IO_I2C_TIMING  *TimingConfig  OPTIONAL,
  IN BOOLEAN               Stop,
  IN BOOLEAN               Restart
  )
{
  EFI_STATUS  Status;
  UINT64      Timeout;
  UINTN       MmioBase;

  if (WriteLength == 0 ||(WriteLength != 0 && WriteBuffer == NULL)) {
    DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Invalid Parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  MmioBase = 0x0;

  //
  // If I2cNumber is provided, override PciCfgBase
  //
  if (I2cNumber) {
    if (*I2cNumber >= GetMaxI2cInterfacesNum ()) {
      return EFI_INVALID_PARAMETER;
    }
    PciCfgBase = (UINTN) LpssI2cPciCfgBase (*I2cNumber);
  }

  if (!SerialIoI2cPciEnable (PciCfgBase, &MmioBase)) {
    DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Device is still not enabled or there is no BAR assigned\n"));
    return EFI_DEVICE_ERROR;
  }
  if (Restart) {
    Status = SerialIoI2cInitializeWriteRead (MmioBase, TargetAddress, &Frequency, TimingConfig);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Error during initialization\n"));
      return Status;
    }
  }

  SerialIoI2cClearErrors (MmioBase);

  Timeout = TimeBudget;
  while (1) {
    if (SerialIoI2cIsRTxAbrt (MmioBase)) {
      DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Error during transmit\n"));
      SerialIoI2cDumpTxAbort (MmioBase);
      SerialIoI2cDisableController (MmioBase);
      SerialIoI2cInitializeWriteRead (MmioBase, TargetAddress, &Frequency, TimingConfig);
      return EFI_DEVICE_ERROR;
    }

    //
    // Add Data to TX FIFO
    // If TX FIFO is Full or at its limit decrease Timeout
    //
    if (SerialIoI2cPrivateWrite (&WriteLength, &WriteBuffer, MmioBase, Stop, Restart)) {
      Restart = FALSE;
    } else {
      MicroSecondDelay (1);
      Timeout--;
    }

    if (WriteLength == 0) {
      Status = SerialIoI2cWaitTxFifoEmptyIcNotActive (MmioBase);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Error Tx FIFO not empty or still active\n"));
        return Status;
      }
      return EFI_SUCCESS;
    }

    if (Timeout == 0) {
      SerialIoI2cDisableController (MmioBase);
      SerialIoI2cInitializeWriteRead (MmioBase, TargetAddress, &Frequency, TimingConfig);
      DEBUG ((DEBUG_WARN, "SerialIoI2cWrite Timeout\n"));
      return EFI_TIMEOUT;
    }
  }
}

/**
  Execute an I2C read command

  Addressing mode will defined based upon TargetAddress and mask:
  TargetAddress & 0x80000000 - if true will result in 10bit addressing
  Refer to: I2C_ADDRESSING_10_BIT

  @param[in]   PciCfgBase     Pci Config Base Address of specified I2C controller
                              @Note: If I2C Number is provided this parameter is ignored and can be left as 0
  @param[in]   I2cNumber      Optional: I2C controller applicable on bus 0 only
  @param[in]   TargetAddress  Address of the end point device
  @param[in]   ReadLength     Read length of the current transaction
  @param[in]   ReadBuffer     Pointer to data in read buffer
  @param[in]   TimeBudget     Time allotted to verify if I2C is enabled
  @param[in]   Frequency      Bus clock frequency (in kHz)
  @param[in]   TimingConfig   Optional: Pointer to struct passed down to override SDA Hold and SCL LCNT/HCNT settings
  @param[in]   Restart        Indication if restart is needed, if TRUE will Initialize I2C controller.
                              @Note: Restart is required in first transaction.

  @retval   EFI_INVALID_PARAMETER   Invalid length / buffer parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in I2C disable/enable or wrong ENST value
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Read was successful
**/
EFI_STATUS
SerialIoI2cRead (
  IN UINTN                 PciCfgBase,
  IN UINT8                 *I2cNumber  OPTIONAL,
  IN UINT32                TargetAddress,
  IN UINT32                ReadLength,
  IN UINT8                 *ReadBuffer,
  IN UINT64                TimeBudget,
  IN UINTN                 Frequency,
  IN SERIAL_IO_I2C_TIMING  *TimingConfig  OPTIONAL,
  IN BOOLEAN               Restart
  )
{
  EFI_STATUS  Status;
  UINT64      Timeout;
  UINTN       MmioBase;

  if (ReadLength == 0 || (ReadLength != 0 && ReadBuffer == NULL)) {
    DEBUG ((DEBUG_WARN, "SerialIoI2cRead Invalid Parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  MmioBase    = 0x0;

  //
  // If I2cNumber is provided, override PciCfgBase
  //
  if (I2cNumber) {
    if (*I2cNumber >= GetMaxI2cInterfacesNum ()) {
      return EFI_INVALID_PARAMETER;
    }
    PciCfgBase = (UINTN) LpssI2cPciCfgBase (*I2cNumber);
  }

  if (!SerialIoI2cPciEnable (PciCfgBase, &MmioBase)) {
    DEBUG ((DEBUG_WARN, "SerialIoI2cRead Device is still not enabled or there is no BAR assigned\n"));
    return EFI_DEVICE_ERROR;
  }
  if (Restart) {
    Status = SerialIoI2cInitializeWriteRead (MmioBase, TargetAddress, &Frequency, TimingConfig);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_WARN, "SerialIoI2cRead Error during initialization\n"));
      return Status;
    }
  }

  SerialIoI2cClearErrors (MmioBase);

  Timeout = TimeBudget;
  while (1) {
    if (SerialIoI2cIsRTxAbrt (MmioBase)) {
      DEBUG ((DEBUG_WARN, "SerialIoI2cRead Error during transmit\n"));
      SerialIoI2cDumpTxAbort (MmioBase);
      SerialIoI2cDisableController (MmioBase);
      return EFI_DEVICE_ERROR;
    }

    Status = SerialIoI2cPrivateRead (&ReadLength, &ReadBuffer, MmioBase, Restart);
    if (Status == EFI_TIMEOUT) {
      return Status;
    }
    //EFI_NOT_READY might be expected
    if (Status == EFI_SUCCESS && ReadLength == 0) {
      return EFI_SUCCESS;
    }

    Restart = FALSE;
    MicroSecondDelay (1);
    Timeout--;
    if (Timeout == 0) {
      SerialIoI2cDisableController (MmioBase);
      DEBUG ((DEBUG_WARN, "SerialIoI2cRead Timeout\n"));
      return EFI_TIMEOUT;
    }
  }
}

/**
  Executes a block of I2C read/write and wait commands. This is intended to enable or configure devices that require multiple data transactions through a I2C bus.
  The command structure consists of entries with a command id, input/output data pointer and data count.

  The following is an example of a command block:
  SERIALIO_I2C_BLOCK_COMMAND SampleCommandBlock[] = {
    { I2cBlockCommandWrite, &gWriteDataBuffer[0], sizeof (gWriteDataBuffer)},
    { I2cBlockCommandRead, &gReadDataBuffer[0], 1000},
    { I2cBlockCommandWait, NULL, 1000},
    { I2cBlockCommandWrite, &gWriteDataBuffer[100], 100}
  };

  Status = SerialIoI2cBlockWriteRead (
            PciCfgBase,
            &I2cNumber,
            0x10,
            SampleCommandBlock,
            sizeof(SampleCommandBlock)/sizeof(SERIALIO_I2C_BLOCK_COMMAND),
            Frequency,
            WAIT_1_SECOND,
            TRUE,
            &TimingConfig
            );

  @param[in]   PciCfgBase     Pci Config Base Address of specified I2C controller
                              @Note: If I2C Number is provided this parameter is ignored and can be left as 0
  @param[in]   I2cNumber                    Optional: The serial controller to be used
  @param[in]   TargetAddress                The address for the device
  @param[in]   I2cCommandBlock              Pointer to SERIALIO_I2C_BLOCK_COMMAND structure with all commands and data
  @param[in]   NumofBlocks                  Total amount of the command block entries - sizeof(SampleCommandBlock)/sizeof(SERIALIO_I2C_BLOCK_COMMAND)
  @param[in]   Frequency                    Bus clock frequency (in kHz)
  @param[in]   MaxTransactionTimeBudget     Maximum Time budget for a single transaction
  @param[in]   DisableI2cAfterTransaction   If TRUE, I2c is disabled after transaction is finished.
  @param[in]   TimingConfig                 Optional: Pointer to struct passed down to override SDA Hold and SCL LCNT/HCNT settings

  @retval   EFI_INVALID_PARAMETER   Signals that a command or data count was invalid
  @retval   EFI_BUFFER_TOO_SMALL    The last command could not be executed since the buffer ended before reading all parameters
  @retval   EFI_SUCCESS             The command were executed without incident
**/
EFI_STATUS
SerialIoI2cBlockWriteRead (
  IN UINTN                       PciCfgBase,
  IN UINT8                       *I2cNumber  OPTIONAL,
  IN UINT32                      TargetAddress,
  IN SERIALIO_I2C_BLOCK_COMMAND  *I2cCommandBlock,
  IN UINT32                      NumofBlocks,
  IN UINTN                       Frequency,
  IN UINT64                      MaxTransactionTimeBudget,
  IN BOOLEAN                     DisableI2cAfterTransaction,
  IN SERIAL_IO_I2C_TIMING        *TimingConfig  OPTIONAL
  )
{
  UINT32        BlockIndex;
  EFI_STATUS    Status;
  BOOLEAN       Stop;
  BOOLEAN       Restart;
  UINT8         NextOperationType;

  Status            = EFI_SUCCESS;
  BlockIndex        = 0;
  Stop              = TRUE;

  while (BlockIndex < NumofBlocks) {

    Restart = FALSE;
    if (BlockIndex == 0) {
      Restart = TRUE;
    }
    if (BlockIndex < (NumofBlocks - 1)) {
      NextOperationType = I2cCommandBlock[BlockIndex + 1].CmdId;
    } else {
      NextOperationType = 0xFF;
    }

    switch (I2cCommandBlock[BlockIndex].CmdId) {
      case I2cBlockCommandWait:
        MicroSecondDelay (I2cCommandBlock[BlockIndex].CommandData * 1000);
        break;

      case I2cBlockCommandRead:
        Status = SerialIoI2cRead (
                   PciCfgBase,
                   I2cNumber,
                   TargetAddress,
                   I2cCommandBlock[BlockIndex].CommandData,
                   I2cCommandBlock[BlockIndex].CommandPointer,
                   MaxTransactionTimeBudget,
                   Frequency,
                   TimingConfig,
                   Restart
                   );
        if (Status != EFI_SUCCESS) {
          DEBUG ((DEBUG_ERROR, "SerialIoI2cBlockWriteRead I2cBlockCommandRead Id: %d Read of: %d [bytes] failed with Status: %r\n", BlockIndex, I2cCommandBlock[BlockIndex].CommandData, Status));
          return Status;
        }
        break;

      case I2cBlockCommandWrite:
        Stop    = TRUE;
        // Stop when next operiation is not Read
        if (NextOperationType == I2cBlockCommandRead) {
          Stop = FALSE;
        }
        Status = SerialIoI2cWrite (
                   PciCfgBase,
                   I2cNumber,
                   TargetAddress,
                   I2cCommandBlock[BlockIndex].CommandData,
                   I2cCommandBlock[BlockIndex].CommandPointer,
                   MaxTransactionTimeBudget,
                   Frequency,
                   TimingConfig,
                   Stop,
                   Restart
                   );
        if (Status != EFI_SUCCESS) {
          DEBUG ((DEBUG_ERROR, "SerialIoI2cBlockWriteRead I2cBlockCommandWrite Id: %d Write of: %d [bytes] failed with Status: %r\n", BlockIndex, I2cCommandBlock[BlockIndex].CommandData, Status));
          return Status;
        }
        break;

      default:
        return EFI_INVALID_PARAMETER;
    }


    BlockIndex += 1;
  }

  if (DisableI2cAfterTransaction) {
    //
    // If I2cNumber is provided, override PciCfgBase
    //
    if (I2cNumber) {
      PciCfgBase = (UINTN) LpssI2cPciCfgBase (*I2cNumber);
    }
    SerialIoI2cPciDisable (PciCfgBase);
  }

  //
  // If the buffer index is not at the end of the buffer defined by size, there was a mismatch and potentially an error.
  //
  if (BlockIndex != NumofBlocks) {
    return EFI_BAD_BUFFER_SIZE;
  }
  return EFI_SUCCESS;
}
