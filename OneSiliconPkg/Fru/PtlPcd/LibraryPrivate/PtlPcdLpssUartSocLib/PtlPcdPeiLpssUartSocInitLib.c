/** @file
  LPSS Private Uart Lib implementation PantherLake specific.
  All functions in this library are available in PEI,
  But do not support UEFI RUNTIME environment call.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiItssLib.h>
#include <Ppi/SiPolicy.h>
#include <LpssUartConfig.h>
#include <Library/PeiLpssUartInitLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/LpssControllerInfoLib.h>

/**
  Build LPSS UART Handle

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] UartHandle        The Lpss UART Handle instance
  @param[in]      UartDeviceConfig  Lpss UART Config Block
  @param[in]      UartIndex         UART Controller index

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
PtlPcdLpssUartBuildHandle (
  IN SI_POLICY_PPI                *SiPolicy,
  IN OUT LPSS_UART_HANDLE         *UartHandle,
  IN     LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  IN     UINT8                    UartIndex
  )
{
  EFI_STATUS                Status;
  PCI_CONFIG_SPACE_ACCESS   *CfgAccess;

  if (UartIndex >= GetMaxUartInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  UartHandle->ControllerId = UartIndex;

  Status = PtlPcdLpssUartGetController (UartIndex, &UartHandle->Controller);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // UART Config Block
  //
  UartHandle->UartConfig = UartDeviceConfig;

  //
  // Private configuration options
  //
  CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)UartHandle->Controller->PcieCfgAccess;
  ItssGetDevIntConfig (
                    SiPolicy,
                    CfgAccess->Dev,
                    CfgAccess->Fun,
                    &UartHandle->PrivateConfig.InterruptPin,
                    &UartHandle->PrivateConfig.Irq);

  return EFI_SUCCESS;
}

/**
  Initialize the LPSS UART controller

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         UART Config Block not found
**/
EFI_STATUS
PtlPcdLpssUartSocInit (
  IN  SI_POLICY_PPI *SiPolicy
  )
{
  EFI_STATUS        Status;
  BOOLEAN           ErrorFlag;
  UINT8             UartIndex;
  LPSS_UART_CONFIG  *LpssUartConfig;
  LPSS_UART_HANDLE  LpssUartHandle;

  DEBUG ((DEBUG_INFO, "%a - Start.\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssUartConfigGuid, (VOID *) &LpssUartConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get Uart config block!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  ErrorFlag = FALSE;
  for (UartIndex = 0; UartIndex < GetMaxUartInterfacesNum (); UartIndex++) {
    Status = PtlPcdLpssUartBuildHandle (SiPolicy, &LpssUartHandle, &(LpssUartConfig->UartDeviceConfig[UartIndex]), UartIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot build Uart handle structure for Ctrl index %d (Status %r)!\n", __FUNCTION__, UartIndex, Status));
      DEBUG ((DEBUG_ERROR, "Skip UART Initialization for controller index %d!\n", UartIndex));
      ErrorFlag = TRUE;
      PtlPcdLpssUartControllerFree (LpssUartHandle.Controller);
      continue;
    }
    Status = LpssUartInit (&LpssUartHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Failed to initialize UART controller #%d!\n", __FUNCTION__, UartIndex));
      ErrorFlag = TRUE;
    }
    PtlPcdLpssUartControllerFree (LpssUartHandle.Controller);
  }

  if (ErrorFlag == TRUE) {
    if (EFI_ERROR (Status)) {
      return Status;
    } else {
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_SUCCESS;
}
