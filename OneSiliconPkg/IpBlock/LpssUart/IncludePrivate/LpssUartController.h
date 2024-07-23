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
#ifndef _LPSS_UART_CONTROLLER_H_
#define _LPSS_UART_CONTROLLER_H_

#include <RegisterAccess.h>

/**
  LPSS UART handle structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions sepecific to contorller.
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

  /**
    LPSS UART controller PCI config space address
    in Fixed Mmio representation.
  **/
  UINTN             FixedMmioPciCfgBaseAddr;

   /**
    LPSS UART controller MMIO address
    in Fixed Mmio representation.
  **/
  UINTN             FixedMmio;

  /**
    LPSS UART Temporary MMIO assigned
  **/
  BOOLEAN           TempBar;
} LPSS_UART_CONTROLLER;

#endif // _LPSS_UART_CONTROLLER_H_
