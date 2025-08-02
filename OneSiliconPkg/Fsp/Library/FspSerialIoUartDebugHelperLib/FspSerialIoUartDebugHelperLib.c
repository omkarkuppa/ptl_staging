/** @file
  Implementation of FspSerialIoUartDebugHelperLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <PiPei.h>
#include <FspEas.h>
#include <FsptUpd.h>
#include <FspmUpd.h>
#include <Library/FspCommonLib.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <Library/LpssUartLib.h>
#include <Library/SecPreMemLpssUartInitLib.h>
#include <Library/PrintLib.h>



/**
  Return FSPT UPD Location Ppi

**/
FSPT_UPD *
EFIAPI
FspLpssUartDebugGetFsptUpdLocPpi (
  VOID
  )
{
  CONST EFI_PEI_SERVICES    **PeiServices;
  IA32_DESCRIPTOR           Idtr;
  EFI_STATUS                Status;
  FSPT_UPD                  *FsptUpdLocationPpi;

  Status = EFI_SUCCESS;
  PeiServices = NULL;
  FsptUpdLocationPpi = NULL;

  if (PcdGetBool (PcdFspValidatePeiServiceTablePointer)) {  // New PCD to control do PeiServiceTablepointer check or not. For SEC, this PCD is configured as FALSE. Its default value is TRUE
    AsmReadIdtr (&Idtr);
    PeiServices = (CONST EFI_PEI_SERVICES **) (*(UINTN*)(Idtr.Base - sizeof (UINTN)));
  }
  if (PeiServices != NULL) {
    Status =  (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gFsptUpdLocationPpiGuid,
                                0,
                                NULL,
                                (VOID **) &FsptUpdLocationPpi
                                );
    if (EFI_ERROR (Status)) {
      FsptUpdLocationPpi = NULL;
    }
  }
  return FsptUpdLocationPpi;
}

/**
  Get FSPT Config

  @param[in] FsptConfig        FSPT Config

  @retval TRUE if FSPT exists
  @retval FALSE unable to locate FSP
**/
BOOLEAN
STATIC
GetFsptConfig (
  OUT FSP_T_CONFIG  **Fspt
  )
{
  FSPT_UPD        *FsptUpd;

  FsptUpd = FspLpssUartDebugGetFsptUpdLocPpi ();
  if (FsptUpd != NULL) {
    *Fspt = &FsptUpd->FsptConfig;
    return TRUE;
  }
  return FALSE;
}

/**
  Get FSPM Config

  @param[out] Fspm        FSPM Config

  @retval TRUE if FSPM exists
  @retval FALSE unable to locate FSP
**/
BOOLEAN
STATIC
GetFspmConfig (
  OUT FSP_M_CONFIG  **Fspm
  )
{
  FSP_GLOBAL_DATA *FspData;
  FSPM_UPD        *FspmUpd;

  FspData = GetFspGlobalDataPointer ();

  if (((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) ||
     (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    return FALSE;
  } else if ((FspData->FspMode == FSP_IN_API_MODE) && (FspData->MemoryInitUpdPtr != NULL)) { // FSP_MODE_CHECK
    FspmUpd = FspData->MemoryInitUpdPtr;
    *Fspm = &FspmUpd->FspmConfig;
    return TRUE;
  }
  return FALSE;
}

/**
  Returns UART attributes

  @param[out] UartDeviceConfig           A pointer to the SERIAL_IO_UART_CONFIG.
  @param[out] LpssUartNumber         The Number of Serial Io Uart.
**/
VOID
STATIC
FspLpssUartDebugGetDeviceConfig (
  OUT LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  OUT UINT8                    *UartNumber
  )
{
  FSP_M_CONFIG    *FspmConfig;
  FSP_T_CONFIG    *FsptConfig;

  FspmConfig = NULL;
  FsptConfig = NULL;

  if (GetFsptConfig (&FsptConfig)) {

    UartDeviceConfig->Attributes.BaudRate = FsptConfig->PcdLpssUartBaudRate;
    UartDeviceConfig->Attributes.Parity   = FsptConfig->PcdLpssUartParity;
    UartDeviceConfig->Attributes.DataBits = FsptConfig->PcdLpssUartDataBits;
    UartDeviceConfig->Attributes.StopBits = FsptConfig->PcdLpssUartStopBits;
    UartDeviceConfig->Attributes.AutoFlow = FsptConfig->PcdLpssUartAutoFlow;
    UartDeviceConfig->Mode                = FsptConfig->PcdLpssUartMode;
    UartDeviceConfig->PinMux.Rx           = FsptConfig->PcdLpssUartRxPinMux;
    UartDeviceConfig->PinMux.Tx           = FsptConfig->PcdLpssUartTxPinMux;
    UartDeviceConfig->PinMux.Rts          = FsptConfig->PcdLpssUartRtsPinMux;
    UartDeviceConfig->PinMux.Cts          = FsptConfig->PcdLpssUartCtsPinMux;
    UartDeviceConfig->PowerGating         = FsptConfig->PcdLpssUartPowerGating;
    *UartNumber =  FsptConfig->PcdLpssUartNumber;
  } else if (GetFspmConfig (&FspmConfig)) {
    UartDeviceConfig->Attributes.BaudRate = FspmConfig->SerialIoUartDebugBaudRate;
    UartDeviceConfig->Attributes.Parity   = FspmConfig->SerialIoUartDebugParity;
    UartDeviceConfig->Attributes.DataBits = FspmConfig->SerialIoUartDebugDataBits;
    UartDeviceConfig->Attributes.StopBits = FspmConfig->SerialIoUartDebugStopBits;
    UartDeviceConfig->Attributes.AutoFlow = FspmConfig->SerialIoUartDebugAutoFlow;
    UartDeviceConfig->Mode                = FspmConfig->SerialIoUartDebugMode;
    UartDeviceConfig->PinMux.Rx           = FspmConfig->SerialIoUartDebugRxPinMux;
    UartDeviceConfig->PinMux.Tx           = FspmConfig->SerialIoUartDebugTxPinMux;
    UartDeviceConfig->PinMux.Rts          = FspmConfig->SerialIoUartDebugRtsPinMux;
    UartDeviceConfig->PinMux.Cts          = FspmConfig->SerialIoUartDebugCtsPinMux;
    UartDeviceConfig->PowerGating         = FspmConfig->SerialIoUartPowerGating;
    *UartNumber =  FspmConfig->SerialIoUartDebugControllerNumber;
  } else {
    LpssUartDebugPcdGetDeviceConfig (UartDeviceConfig, UartNumber);
  }
  return;
}

/**
  Returns Serial Io UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
STATIC
FspLpssUartDebugGetControllerNumber (
  VOID
  )
{
  FSP_M_CONFIG    *FspmConfig;
  FSP_T_CONFIG    *FsptConfig;

  FspmConfig = NULL;
  FsptConfig = NULL;

  if (GetFsptConfig (&FsptConfig)) {
    return FsptConfig->PcdLpssUartNumber;
  } else if (GetFspmConfig (&FspmConfig)) {
    return FspmConfig->SerialIoUartDebugControllerNumber;
  } else {
    return LpssUartDebugPcdGetControllerNumber ();
  }
}

/**
  Returns Serial Io UART MMIO Base Address in Debug mode

  @retval  MMIO Base Address by default in PCI Mode
**/
UINTN
STATIC
FspLpssUartDebugGetPciDefaultMmioBase (
  VOID
  )
{
  FSP_M_CONFIG    *FspmConfig;
  FSP_T_CONFIG    *FsptConfig;
  UINTN           MmioBase;

  FspmConfig = NULL;
  FsptConfig = NULL;

  if (GetFsptConfig (&FsptConfig)) {
    MmioBase = FsptConfig->PcdLpssUartDebugMmioBase;
  } else if (GetFspmConfig (&FspmConfig)) {
    MmioBase = FspmConfig->SerialIoUartDebugMmioBase;
  } else {
    return LpssUartDebugPcdGetPciDefaultMmioBase ();
  }
  return MmioBase;
}

/**
  Initialize Lpss UART for debug.

**/
VOID
EFIAPI
SerialIoUartDebugInit (
  VOID
  )
{
  UINT8                     UartNumber;
  LPSS_UART_DEVICE_CONFIG   UartDeviceConfig;

  // Initalize the UartNumber
  UartNumber = 0xFF;

  FspLpssUartDebugGetDeviceConfig (&UartDeviceConfig, &UartNumber);

  if (UartDeviceConfig.Mode != LpssUartPci) {
    return;
  }

  UartDeviceConfig.DBG2        = FALSE;
  UartDeviceConfig.DmaEnable   = FALSE;

  LpssUartConfiguration (UartNumber, &UartDeviceConfig);

}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartDebugWrite (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  UINT8  UartIndex;

  if (FspLpssUartDebugGetPciDefaultMmioBase () == 0x0) {
    UartIndex = FspLpssUartDebugGetControllerNumber ();
    return LpssUartWrite ((UINTN) GetLpssUartFixedMmioAddress (UartIndex), Buffer, NumberOfBytes);
  }

  return LpssUartWrite ((UINTN) FspLpssUartDebugGetPciDefaultMmioBase (), Buffer, NumberOfBytes);
}
