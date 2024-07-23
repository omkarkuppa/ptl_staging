/** @file
  UART controller handle

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _SERIAL_IO_UART_HANDLE_H_
#define _SERIAL_IO_UART_HANDLE_H_

#include <RegisterAccess.h>
#include <Library/PsfLib.h>
#include <P2SbController.h>
#include <Library/P2SbSidebandAccessLib.h>

/**
  UART handle structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions sepecific to contorller.
**/
typedef struct {
  /**
    UART controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    UART controller PCI Segment
  **/
  UINT8                       Segment;
  /**
    UART controller PCI Bus
  **/
  UINT8                       Bus;
  /**
    UART controller PCI Device
  **/
  UINT8                       Device;
  /**
    UART controller PCI Function
  **/
  UINT8                       Function;
  /**
    UART controller PCI config space address
    in Fixed Mmio representation.
  **/
  UINTN                       FixedMmioPciCfgBaseAddr;
  /**
    UART controller index
  **/
  UINT8                       UartCtrlIndex;
  UINTN                       FixedMmio;
  UINT16                      PciCfgControlOffset;
  PSF_PORT                    PsfPort;
  /**
    P2SB
  **/
  P2SB_CONTROLLER                P2SbCtrl;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbAccess;
  REGISTER_ACCESS                *UartPcrAccess;
} SERIAL_IO_UART_CONTROLLER;

#endif // _SERIAL_IO_UART_HANDLE_H_
