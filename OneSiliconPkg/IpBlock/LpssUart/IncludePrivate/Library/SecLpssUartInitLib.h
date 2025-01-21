/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _SEC_LPSS_UART_INIT_LIB_H_
#define _SEC_LPSS_UART_INIT_LIB_H_

#include <LpssUartHandle.h>

  /**
  Configures LPSS Controller with minimum configuration in ACPI mode

  @param[in] UartNumber        UART Number
  @param[in] UartDeviceConfig  LPSS UART Config
**/
VOID
SecLpssUartConfiguration (
  IN UINT8                        UartNumber,
  IN LPSS_UART_DEVICE_CONFIG      *UartDeviceConfig                     
  );

/**
  Gets Pci Config control offset

  @param[in] UartNumber              Lpss device UART number

  @retval                            Config control offset
**/
UINT16
GetLpssUartConfigControlOffset (
  IN UINT8       UartNumber
  );
  
/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetLpssUartFixedMmioAddress (
  IN UINT8       UartNumber
  );


  
#endif
