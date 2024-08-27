/** @file
  This library gives information about LPSS Controllers

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegs.h>
#include <Register/LpssPciConfigControlRegs.h>
#include <LpssI2cController.h>
#include <LpssI3cController.h>
#include <LpssSpiController.h>
#include <LpssUartController.h>
#include <Defines/PchReservedResources.h>
#include <Library/SerialIoUartSocLib.h>

/**
  Get I2C Controller device instance
  Note:
  This function allocates all the memory required by I2C controller structures

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
  )
{
  LPSS_I2C_CONTROLLER           *I2cCtrl;
  PCI_CONFIG_SPACE_ACCESS       *CfgAccess;

  if (I2cControllerIndex >= GetMaxI2cInterfacesNum ()) {
    return EFI_INVALID_PARAMETER;
  }

  I2cCtrl = AllocateZeroPool (sizeof (LPSS_I2C_CONTROLLER));
  CfgAccess = AllocateZeroPool (sizeof (PCI_CONFIG_SPACE_ACCESS));

  if ((CfgAccess == NULL) || (I2cCtrl == NULL)) {
    DEBUG ((DEBUG_WARN, "%a: ERROR: Memory Allocation failed for I2C Controller!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // MMIO Base Address
  //
  I2cCtrl->MmioTmpBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  //
  // I2C Controller - PCIe Configuration Space registers access
  //
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                        DEFAULT_PCI_BUS_NUMBER_PCH,
                        LpssI2cDevNumber (I2cControllerIndex),
                        LpssI2cFuncNumber (I2cControllerIndex),
                        CfgAccess
                        );
  I2cCtrl->PcieCfgAccess = (REGISTER_ACCESS *) CfgAccess;

  *I2cController = I2cCtrl;

  return EFI_SUCCESS;
}

/**
  Frees I2C PCI controller memory

  @param[out] I2cController   Pointer to I2C Controller structure
**/
VOID
PtlPcdLpssI2cControllerFree (
  IN LPSS_I2C_CONTROLLER *I2cController
  )
{
  FreePool (I2cController->PcieCfgAccess);
  FreePool (I2cController);
}

/**
  Get I3C Controller device instance
  Note:
  This function allocates all the memory required to I3C controller structures

  @param[in]  I3cControllerIndex       I3C controller index
  @param[out] I3cController            Pointer to I3c Controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - Controller index out of the range
              EFI_OUT_OF_RESOURCES  - Out of memory
**/
EFI_STATUS
PtlPcdLpssI3cGetController (
  IN  UINT8               I3cControllerIndex,
  OUT LPSS_I3C_CONTROLLER **I3cController
  )
{
  LPSS_I3C_CONTROLLER           *I3cCtrl;
  PCI_CONFIG_SPACE_ACCESS       *CfgAccess;

  if (I3cControllerIndex >= GetMaxI3cInterfacesNum ()) {
    return EFI_INVALID_PARAMETER;
  }

  I3cCtrl = AllocateZeroPool (sizeof (LPSS_I3C_CONTROLLER));
  CfgAccess = AllocateZeroPool (sizeof (PCI_CONFIG_SPACE_ACCESS));

  if ((CfgAccess == NULL) || (I3cCtrl == NULL)) {
    DEBUG ((DEBUG_WARN, "%a: ERROR: Memory Allocation failed for I3C Controller!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // MMIO Base Address
  //
  I3cCtrl->MmioTmpBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  //
  // I3C Controller - PCIe Configuration Space registers access
  //
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                        DEFAULT_PCI_BUS_NUMBER_PCH,
                        LpssI3cDevNumber (I3cControllerIndex),
                        LpssI3cFuncNumber (I3cControllerIndex),
                        CfgAccess
                        );
  I3cCtrl->PcieCfgAccess = (REGISTER_ACCESS *) CfgAccess;

  *I3cController = I3cCtrl;

  return EFI_SUCCESS;
}

/**
  Frees I3C PCI controller memory

  @param[out] I3cController     Pointer to I3C Controller structure
**/
VOID
PtlPcdLpssI3cControllerFree (
  IN LPSS_I3C_CONTROLLER *I3cController
  )
{
  FreePool (I3cController->PcieCfgAccess);
  FreePool (I3cController);
}

/**
  Get SPI Controller device instance
  Note:
  This function allocates all the memory required by SPI controller structures

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
  )
{
  LPSS_SPI_CONTROLLER           *SpiCtrl;
  PCI_CONFIG_SPACE_ACCESS       *CfgAccess;

  if (SpiControllerIndex >= GetMaxSpiInterfacesNum ()) {
    return EFI_INVALID_PARAMETER;
  }

  SpiCtrl = AllocateZeroPool (sizeof (LPSS_SPI_CONTROLLER));
  CfgAccess = AllocateZeroPool (sizeof (PCI_CONFIG_SPACE_ACCESS));

  if ((CfgAccess == NULL) || (SpiCtrl == NULL)) {
    DEBUG ((DEBUG_WARN, "%a: ERROR: Memory Allocation failed for SPI Controller!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // MMIO Base Address
  //
  SpiCtrl->MmioTmpBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  //
  // SPI Controller - PCIe Configuration Space registers access
  //
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                        DEFAULT_PCI_BUS_NUMBER_PCH,
                        LpssSpiDevNumber (SpiControllerIndex),
                        LpssSpiFuncNumber (SpiControllerIndex),
                        CfgAccess
                        );
  SpiCtrl->PcieCfgAccess = (REGISTER_ACCESS *) CfgAccess;

  *SpiController = SpiCtrl;

  return EFI_SUCCESS;
}

/**
  Frees SPI PCI controller memory

  @param[out] SpiController   Pointer to SPI Controller structure
**/
VOID
PtlPcdLpssSpiControllerFree (
  IN LPSS_SPI_CONTROLLER *SpiController
  )
{
  FreePool (SpiController->PcieCfgAccess);
  FreePool (SpiController);
}
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
  )
{
  LPSS_UART_CONTROLLER      *UartCtrl;
  PCI_CONFIG_SPACE_ACCESS   *CfgAccess;

  if (UartControllerIndex >= GetMaxUartInterfacesNum ()) {
    return EFI_INVALID_PARAMETER;
  }

  UartCtrl = AllocateZeroPool (sizeof (LPSS_UART_CONTROLLER));
  CfgAccess = AllocateZeroPool (sizeof (PCI_CONFIG_SPACE_ACCESS));

  if ((CfgAccess == NULL) || (UartCtrl == NULL)) {
    DEBUG ((DEBUG_WARN, "%a: ERROR: Memory Allocation failed for UART Controller!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // MMIO Base Address
  //
  UartCtrl->MmioTmpBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  UartCtrl->FixedMmioPciCfgBaseAddr = GetLpssUartFixedBar1 (UartControllerIndex);
  UartCtrl->FixedMmio = GetLpssUartFixedBar0 (UartControllerIndex);

  //
  // UART Controller - PCIe Configuration Space registers access
  //
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                        DEFAULT_PCI_BUS_NUMBER_PCH,
                        LpssUartDevNumber (UartControllerIndex),
                        LpssUartFuncNumber (UartControllerIndex),
                        CfgAccess
                        );
  UartCtrl->PcieCfgAccess = (REGISTER_ACCESS *) CfgAccess;

  *UartController = UartCtrl;

  return EFI_SUCCESS;
}

/**
  Frees UART PCI controller memory

  @param[out] UartController   Pointer to UART Controller structure
**/
VOID
PtlPcdLpssUartControllerFree (
  IN LPSS_UART_CONTROLLER *UartController
  )
{
  if (UartController != NULL) {
    if (UartController->PcieCfgAccess != NULL) {
      FreePool (UartController->PcieCfgAccess);
      UartController->PcieCfgAccess = NULL;
    }
    FreePool (UartController);
    UartController = NULL;
  }
}
