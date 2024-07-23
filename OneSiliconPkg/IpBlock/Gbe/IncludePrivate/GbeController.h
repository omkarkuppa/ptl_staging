/** @file
  GBE controller

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
#ifndef _GBE_CONTROLLER_H_
#define _GBE_CONTROLLER_H_

/**
  GBE controller structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions specific to controller.
**/
typedef struct {
  /**
    GBE controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    GBE controller PCI Segment
  **/
  UINT8                       Segment;
  /**
    GBE controller PCI Bus
  **/
  UINT8                       Bus;
  /**
    GBE controller PCI Device
  **/
  UINT8                       Device;
  /**
    GBE controller PCI Function
  **/
  UINT8                       Function;
} GBE_CONTROLLER;

#endif // _GBE_CONTROLLER_H_
