/** @file
  P2SB controller

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
#ifndef _P2SB_CONTROLLER_H_
#define _P2SB_CONTROLLER_H_

/**
  P2SB structure
  Stores information required to access to registers
  like base address, S:B:D:F etc.
  and definitions specific to P2SB.
**/
typedef struct {
  /**
    P2SB controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    P2SB controller MMIO base address
  **/
  UINT64                      Mmio;
  /**
    Is 20 bits mode supported
  **/
  BOOLEAN                     P2sb20bPcrSupported;
} P2SB_CONTROLLER;

#endif // _P2SB_CONTROLLER_H_
