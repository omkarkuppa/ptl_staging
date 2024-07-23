/** @file
  TraceHub controller

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
#ifndef _TRACEHUB_CONTROLLER_H_
#define _TRACEHUB_CONTROLLER_H_

/**
  TraceHub controller structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions specific to controller.
**/
typedef struct {
  /**
    TraceHub controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  UINT8                       Segment;  ///< TraceHub controller PCI Segment
  UINT8                       Bus;      ///< TraceHub controller PCI Bus
  UINT8                       Device;   ///< TraceHub controller PCI Device
  UINT8                       Function; ///< TraceHub controller PCI Function
} TRACEHUB_CONTROLLER;

#endif // _TRACEHUB_CONTROLLER_H_
