/** @file
  This file provides services for Lpss Uart policy function

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

#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <Library/PchLimits.h>
#include <LpssSpiConfig.h>
#include <LpssUartConfig.h>

/**
  Print LPSS_UART_CONFIG.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
LpssUartPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS         Status;
  LPSS_UART_CONFIG   *UartConfig;
  UINT8              Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLpssUartConfigGuid, (VOID *) &UartConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ LPSS UART Config ------------------\n"));

  for (Index = 0; Index < PCH_MAX_LPSS_UART_CONTROLLERS; Index++) {
    DEBUG ((DEBUG_INFO, " UART[%d].Mode                = %d\n", Index, UartConfig->UartDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.BaudRate = %d\n", Index, UartConfig->UartDeviceConfig[Index].Attributes.BaudRate));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.Parity   = %d\n", Index, UartConfig->UartDeviceConfig[Index].Attributes.Parity));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.DataBits = %d\n", Index, UartConfig->UartDeviceConfig[Index].Attributes.DataBits));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.StopBits = %d\n", Index, UartConfig->UartDeviceConfig[Index].Attributes.StopBits));
    DEBUG ((DEBUG_INFO, " UART[%d].Attributes.AutoFlow = %d\n", Index, UartConfig->UartDeviceConfig[Index].Attributes.AutoFlow));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Rx           = 0x%x\n", Index, UartConfig->UartDeviceConfig[Index].PinMux.Rx));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Tx           = 0x%x\n", Index, UartConfig->UartDeviceConfig[Index].PinMux.Tx));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Rts          = 0x%x\n", Index, UartConfig->UartDeviceConfig[Index].PinMux.Rts));
    DEBUG ((DEBUG_INFO, " UART[%d].PinMux.Cts          = 0x%x\n", Index, UartConfig->UartDeviceConfig[Index].PinMux.Cts));
    DEBUG ((DEBUG_INFO, " UART[%d].DBG2                = %d\n", Index, UartConfig->UartDeviceConfig[Index].DBG2));
    DEBUG ((DEBUG_INFO, " UART[%d].PowerGating         = %d\n", Index, UartConfig->UartDeviceConfig[Index].PowerGating));
    DEBUG ((DEBUG_INFO, " UART[%d].DmaEnable           = %d\n", Index, UartConfig->UartDeviceConfig[Index].DmaEnable));
  }
  DEBUG ((DEBUG_INFO, "------------------------------------------------\n"));
}

/**
  LPss Uart load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LpssUartLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                       Index;
  LPSS_UART_CONFIG*           UartConfig;
  UINT8                       DebugUartNumber;
  LPSS_UART_DEVICE_CONFIG     DebugUartDeviceConfig;

  UartConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "UartConfig->Header.GuidHob.Name = %g\n", &UartConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "UartConfig->Header.GuidHob.Header.HobLength = 0x%x\n", UartConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < PCH_MAX_SERIALIO_UART_CONTROLLERS; Index++) {
    if (Index == 0) {
      UartConfig->UartDeviceConfig[Index].Mode                = LpssUartHidden;
    } else {
      UartConfig->UartDeviceConfig[Index].Mode                = LpssUartDisabled;
    }
    UartConfig->UartDeviceConfig[Index].DBG2                = FALSE;
    UartConfig->UartDeviceConfig[Index].DmaEnable           = TRUE;
    UartConfig->UartDeviceConfig[Index].Attributes.BaudRate = 115200;
    UartConfig->UartDeviceConfig[Index].Attributes.Parity   = 1;
    UartConfig->UartDeviceConfig[Index].Attributes.DataBits = 8;
    UartConfig->UartDeviceConfig[Index].Attributes.StopBits = 1;
    UartConfig->UartDeviceConfig[Index].Attributes.AutoFlow = 0;
    UartConfig->UartDeviceConfig[Index].PowerGating         = LpssUartPgAuto;
  }

  if (LpssUartDebugPcdGetDebugEnable () > 0) {
    LpssUartDebugPcdGetDeviceConfig (&DebugUartDeviceConfig, &DebugUartNumber);
    UartConfig->UartDeviceConfig[DebugUartNumber].Mode                = DebugUartDeviceConfig.Mode;
    UartConfig->UartDeviceConfig[DebugUartNumber].Attributes.BaudRate = DebugUartDeviceConfig.Attributes.BaudRate;
    UartConfig->UartDeviceConfig[DebugUartNumber].Attributes.Parity   = DebugUartDeviceConfig.Attributes.Parity;
    UartConfig->UartDeviceConfig[DebugUartNumber].Attributes.DataBits = DebugUartDeviceConfig.Attributes.DataBits;
    UartConfig->UartDeviceConfig[DebugUartNumber].Attributes.StopBits = DebugUartDeviceConfig.Attributes.StopBits;
    UartConfig->UartDeviceConfig[DebugUartNumber].Attributes.AutoFlow = DebugUartDeviceConfig.Attributes.AutoFlow;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mLpssUartBlocks = {
  &gLpssUartConfigGuid,
  sizeof (LPSS_UART_CONFIG),
  LPSS_UART_CONFIG_REVISION,
  LpssUartLoadConfigDefault
};

/**
  Get LPss Uart config block table size.

  @retval      Size of config block
**/
UINT16
LpssUartGetConfigBlockTotalSize (
  VOID
  )
{
  return mLpssUartBlocks.Size;
}

/**
  Add Lpss Uart ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval    EFI_SUCCESS                   The policy default is initialized.
  @retval    EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
LpssUartAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mLpssUartBlocks, 1);
}