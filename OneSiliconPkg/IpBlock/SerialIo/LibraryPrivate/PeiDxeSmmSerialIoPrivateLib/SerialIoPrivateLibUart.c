/** @file
  Serial IO Private Lib implementation - UART part
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPcrLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchPcrRegsDeprecated.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoUartRegs.h>
#include <SerialIoPrivate.h>
#include <Library/PchLimits.h>

UINTN
GetReservedSerialIoMmio (
  VOID
  );

/**
  Checks if UART is Hidden, and its Pci Config space is available

  @param[in]      UartNumber     Selects Serial IO UART device

  @retval   TRUE             UART is in hidden mode
  @retval   FALSE            UART is not in hidden mode
**/
BOOLEAN
IsSerialIoUartHidden (
  IN UINT8               UartNumber
  )
{
  // @todo I1S changes required
  if (PchPcrRead32 (PID_SERIALIO_DEPRECATED, R_SERIAL_IO_UART_PCR_GEN_REGRW7) & (BIT0 << UartNumber)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Configures Serial IO Controller before control is passed to the OS

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config

**/
VOID
SerialIoUartBootHandler (
  IN UINT8                      UartNumber,
  IN SERIAL_IO_UART_CONFIG      *UartDeviceConfig
  )
{
  // @todo I1S changes required
  UINT64   PciCfgBase;
  BOOLEAN  TurnOff;

  TurnOff = FALSE;

  //
  // Even if Uart is Hidden and in D3 SerialIoUartLib is capable of setting D0 during each write/read.
  // In case it is required for Os Debug DBG2 must be set to TRUE.
  //
  if (UartDeviceConfig->Mode == SerialIoUartPci || UartDeviceConfig->Mode == SerialIoUartHidden) {
    TurnOff = TRUE;
  }

  //
  // Uart in Com mode will be placed in D3 depending on PG configuration through ACPI _PS3
  //

  if ((UartDeviceConfig->Mode == SerialIoUartDisabled) && (LpssUartFuncNumber (UartNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (LpssUartDevNumber (UartNumber))) {
      TurnOff = TRUE;
    }
  }

  if (UartDeviceConfig->DBG2 == TRUE) {
    TurnOff = FALSE;
  }

  if (TurnOff) {
    PciCfgBase = GetSerialIoUartPciCfg (UartNumber);
    SerialIoSetD3 (PciCfgBase);
    if ((UartDeviceConfig->Mode == SerialIoUartPci) || (UartDeviceConfig->Mode == SerialIoUartDisabled)) {
      SerialIoMmioDisable (PciCfgBase, TRUE);
    }
  }
}

/**
  Sets Pme Control Status and Command register values required for S3 Boot Script

  @param[in]     UartNumber         UART Number
  @param[in]     UartDeviceConfig   SerialIo UART Config
  @param[in/out] S3PciCfgBase       S3 Boot Script Pci Config Base
  @param[in/out] Command            Pci Command register data to save
  @param[in/out] Pme                Pci Pme Control register data to save

**/
VOID
SerialIoUartS3Handler (
  IN     UINT8                  UartNumber,
  IN     SERIAL_IO_UART_CONFIG  *UartDeviceConfig,
  IN OUT UINT64                 *S3PciCfgBase,
  IN OUT UINT32                 *Command,
  IN OUT UINT32                 *Pme
  )
{
  // @todo I1S changes required
  BOOLEAN  TurnOff;
  UINT64   PciCfgBase;

  *S3PciCfgBase = 0;
  TurnOff       = FALSE;

  if (UartDeviceConfig->Mode == SerialIoUartPci && UartDeviceConfig->PowerGating != SerialIoUartPgEnabled) {
    TurnOff = TRUE;
  }

  if ((UartDeviceConfig->Mode == SerialIoUartDisabled) && (LpssUartFuncNumber (UartNumber) == 0x0)) {
    if (SerialIoHigherFunctionsEnabled (LpssUartDevNumber (UartNumber))) {
      TurnOff = TRUE;
    }
  }

  if (TurnOff) {
    *S3PciCfgBase = GetSerialIoUartS3PciBase (UartNumber);
    PciCfgBase    = GetSerialIoUartPciCfg (UartNumber);
    if (PciCfgBase < GetReservedSerialIoMmio ()) {
      *Pme          = PciSegmentRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    } else {
      *Pme          = MmioRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    }
    *Pme          = *Pme | BIT0 | BIT1;

    if (PciCfgBase < GetReservedSerialIoMmio ()) {
      *Command      = PciSegmentRead32 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
    } else {
      *Command      = MmioRead32 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
    }
    *Command      = *Command & (UINT32)~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
}
