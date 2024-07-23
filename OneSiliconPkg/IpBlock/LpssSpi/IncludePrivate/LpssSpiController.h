/** @file
  LPSS I2C controller definition

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
#ifndef _LPSS_SPI_CONTROLLER_H_
#define _LPSS_SPI_CONTROLLER_H_

#include <RegisterAccess.h>

/**
  LPSS SPI handle structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions sepecific to controller.
**/
typedef struct {
  /**
    PCIe Configuration Space asccess protocol
   */
  REGISTER_ACCESS   *PcieCfgAccess;

  /**
     Temporary MMIO Base Address
   */
  UINT64            MmioTmpBar;
} LPSS_SPI_CONTROLLER;

#endif // _LPSS_SPI_CONTROLLER_H_
