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

#ifndef _PTL_PCD_LPSS_UART_LIB_H_
#define _PTL_PCD_LPSS_UART_LIB_H_

#include <Ppi/SiPolicy.h>
#include <LpssUartHandle.h>

/**
  Initialize the LPSS UART controller

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         I2C Config Block not found
**/
EFI_STATUS
PtlPcdLpssUartSocInit (
  IN     SI_POLICY_PPI           *SiPolicy
  );

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
  );

#endif /*_PTL_PCD_LPSS_UART_LIB_H_*/
