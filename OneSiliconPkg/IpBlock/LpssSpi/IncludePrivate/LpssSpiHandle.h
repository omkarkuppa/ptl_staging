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

#ifndef _LPSS_SPI_HANDLE_H_
#define _LPSS_SPI_HANDLE_H_

#include <LpssSpiController.h>
#include <LpssSpiConfig.h>

typedef struct {
  /**
    LPSS SPI Interrupt Number
   **/
  UINT8                         InterruptPin;
  /**
    LPSS SPI Interrupt Number
   **/
  UINT8                         Irq;
} LPSS_SPI_PRIVATE_CONFIG;

/**
  LPSS SPI Handle structure
  Stores all data necessary to initialize LPSS SPI IP block
**/
typedef struct {
  LPSS_SPI_CONTROLLER       *Controller;
  LPSS_SPI_DEVICE_CONFIG    *SpiConfig;
  LPSS_SPI_PRIVATE_CONFIG   PrivateConfig;
  UINT8                     ControllerId;
} LPSS_SPI_HANDLE;

#endif
