/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _PEI_LPSS_UART_INIT_LIB_H_
#define _PEI_LPSS_UART_INIT_LIB_H_

#include <LpssUartHandle.h>

/**
  Configures LPSS Uart Controller

  @param[in] LpssUartHandle        The Lpss UART Handle instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       Usupported UART controller mode
              EFI_ABORTED           Device not present or unsupported Uart Mode
              EFI_INVALID_PARAMETER Invalid parameter
**/
EFI_STATUS
LpssUartInit (
  IN LPSS_UART_HANDLE   *LpssUartHandle
  );

/**
  Configures LPSS Uart Function 0 Disabled Controllers

  @param[in] UartHandle           The LPSS UART Handle instance
**/
VOID
LpssUartFunction0Disable (
  IN LPSS_UART_HANDLE   *LpssUartHandle
  );

#endif
