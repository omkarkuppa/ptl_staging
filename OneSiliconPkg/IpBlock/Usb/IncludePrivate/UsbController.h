/** @file
  USB controller definition- part of USB IP Block API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _USB_CONTROLLER_H_
#define _USB_CONTROLLER_H_

#include <RegisterAccess.h>

// Enumeration for controller types
typedef enum {
  HostControllerType,
  DeviceControllerType,
  Usb2ControllerType
} CONTROLLER_TYPE;

/**
  USB Controller structure
  Stores information required to access to the controller registers:
  - PCIe Config Space,
  - Memory Mapped IO (MMIO),
  - Private Space Configuration Registers (PCR)
  - Reserved MMIO base address
  Defines capabilities of the controller that cannot be obtained
  from the hardware like number of ports implemented in the USB controller.
**/
typedef struct {
  /**
    Controller Private Configuration Registers (PCR) space access using P2SB MMIO
  **/
  REGISTER_ACCESS       *SbMmioAccess;
  /**
    PCIe Configuration space access
  **/
  REGISTER_ACCESS       *PcieConfigAccess;
  /**
    Memory mapped IO (MMIO) access
  **/
  REGISTER_ACCESS       *MmioAccess;

  /**
    Reserved temporary MMIO basse address will be used for USB programming
  **/
  UINT64                ReservedMmio;

  /**
    Count of USB2 port
  **/
  UINT8   Usb2PortCount;
  /**
    Count of USB2 ports used as USB-R
  **/
  UINT8   UsbrPortCount;
  /**
    Number of all available USB3 lanes
  **/
  UINT8   Usb3LanesCount;

} USB_CONTROLLER;

#endif // _USB_CONTROLLER_H_
