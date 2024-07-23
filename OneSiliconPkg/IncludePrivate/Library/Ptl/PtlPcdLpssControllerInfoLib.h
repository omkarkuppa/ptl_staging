/** @file
  This library gives PCI information about LPSS Controller

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

#ifndef PTL_PCD_LPSS_CTRL_INFO_LIB_H
#define PTL_PCD_LPSS_CTRL_INFO_LIB_H

#include <LpssI3cController.h>
#include <LpssI2cController.h>
#include <LpssSpiController.h>
#include <LpssUartController.h>

/**
  Get I2C Controller device instance
  Note:
  This function allocates all the memory required to I2C controller structures

  @param[in]  I2cControllerIndex       I2c controller index
  @param[out] I2cController            Pointer to I2c Controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - Controller index out of the range
              EFI_OUT_OF_RESOURCES  - Out of memory
**/
EFI_STATUS
PtlPcdLpssI2cGetController (
  IN  UINT8               I2cControllerIndex,
  OUT LPSS_I2C_CONTROLLER **I2cController
  );

/**
  Frees I2C PCI controller memory

  @param[out] I2cController                Pointer to I2C Controller structure
**/
VOID
PtlPcdLpssI2cControllerFree (
  IN LPSS_I2C_CONTROLLER *I2cController
  );

/**
  Get I3C Controller device instance
  Note:
  This function allocates all the memory required to I3C controller structures

  @param[in]  I2cControllerIndex       I2c controller index
  @param[out] I2cController            Pointer to I2c Controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - Controller index out of the range
              EFI_OUT_OF_RESOURCES  - Out of memory
**/
EFI_STATUS
PtlPcdLpssI3cGetController (
  IN  UINT8               I3cControllerIndex,
  OUT LPSS_I3C_CONTROLLER **I3cController
  );

/**
  Frees I3C PCI controller memory

  @param[out] I3cController     Pointer to I3C Controller structure
**/
VOID
PtlPcdLpssI3cControllerFree (
  IN LPSS_I3C_CONTROLLER *I3cController
  );

/**
  Get SPI Controller device instance
  Note:
  This function allocates all the memory required to SPI controller structures

  @param[in]  SpiControllerIndex       Spi controller index
  @param[out] SpiController            Pointer to Spi Controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - Controller index out of the range
              EFI_OUT_OF_RESOURCES  - Out of memory
**/
EFI_STATUS
PtlPcdLpssSpiGetController (
  IN  UINT8               SpiControllerIndex,
  OUT LPSS_SPI_CONTROLLER **SpiController
  );

/**
  Frees SPI PCI controller memory

  @param[out] SpiController                Pointer to SPI Controller structure
**/
VOID
PtlPcdLpssSpiControllerFree (
  IN LPSS_SPI_CONTROLLER *SpiController
  );

/**
  Get UART Controller device instance
  Note:
  This function allocates all the memory required by UART controller structures

  @param[in]  UartControllerIndex       Uart controller index
  @param[out] UartController            Pointer to Uart Controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - Controller index out of the range
              EFI_OUT_OF_RESOURCES  - Out of memory
**/
EFI_STATUS
PtlPcdLpssUartGetController (
  IN  UINT8                 UartControllerIndex,
  OUT LPSS_UART_CONTROLLER  **UartController
  );

/**
  Frees UART PCI controller memory

  @param[out] UartController   Pointer to UART Controller structure
**/
VOID
PtlPcdLpssUartControllerFree (
  IN LPSS_UART_CONTROLLER *UartController
  );

#endif //PTL_PCD_LPSS_CTRL_INFO_LIB_H
