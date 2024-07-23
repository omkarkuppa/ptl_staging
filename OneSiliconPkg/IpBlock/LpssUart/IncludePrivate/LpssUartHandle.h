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

#ifndef _LPSS_UART_HANDLE_H_
#define _LPSS_UART_HANDLE_H_

#include <LpssUartConfig.h>
#include <LpssUartController.h>

/**
  Internal UART policy options
**/
typedef struct {
  /**
    LPSS UART Interrupt Number
   **/
  UINT8                         InterruptPin;

  /**
    LPSS UART Interrupt Number
   **/
  UINT8                         Irq;
} LPSS_UART_PRIVATE_CONFIG;


/**
  LPSS UART Handle structure
  Stores all data necessary to initialize LPSS UART IP block
**/
typedef struct {
  LPSS_UART_CONTROLLER              *Controller;
  LPSS_UART_DEVICE_CONFIG           *UartConfig;
  LPSS_UART_PRIVATE_CONFIG          PrivateConfig;
  UINT8                             ControllerId;
} LPSS_UART_HANDLE;

#endif
