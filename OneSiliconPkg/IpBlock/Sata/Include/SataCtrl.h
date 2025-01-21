/** @file
  SATA controller handle

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
#ifndef _SATA_HANDLE_H_
#define _SATA_HANDLE_H_

/**
  SATA handle structure
  Stores information required to access to controller registers
  like base address, S:B:D:F etc.
  and definitions sepecific to contorller.
**/
typedef struct {
  /**
    SATA controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    SATA controller PCI Segment
  **/
  UINT8                       Segment;
  /**
    SATA controller PCI Bus
  **/
  UINT8                       Bus;
  /**
    SATA controller PCI Device
  **/
  UINT8                       Device;
  /**
    SATA controller PCI Function
  **/
  UINT8                       Function;
  /**
    SATA controller index
  **/
  UINT32                      SataCtrlIndex;
  /**
    SATA controller total number of ports
  **/
  UINT32                      TotalCtrlPortNum;
} SATA_CONTROLLER;

#endif // _SATA_HANDLE_H_
