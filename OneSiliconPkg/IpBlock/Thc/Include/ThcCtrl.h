/** @file
  THC controller handle
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
#ifndef _THC_CTRL_H_
#define _THC_CTRL_H_

#include <RegisterAccess.h>
#include <Library/P2SbSidebandAccessLib.h>

/**
  THC handle structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions sepecific to contorller.
**/
typedef struct {
  /**
    THC controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    THC controller PCI Segment
  **/
  UINT8                       Segment;
  /**
    THC controller PCI Bus
  **/
  UINT8                       Bus;
  /**
    THC controller PCI Device
  **/
  UINT8                       Device;
  /**
    THC controller PCI Function
  **/
  UINT8                       Function;
  /**
    THC controller index
  **/
  UINT8                       ThcCtrlIndex;
  /**
    THC P2SB
   **/
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbPcrAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbPciAccess;
  REGISTER_ACCESS                *ThcPcrAccess;
  REGISTER_ACCESS                *ThcPciSbAccess;
  P2SB_PID                       PortId;
} THC_CONTROLLER;

#endif // _THC_CTRL_H_
