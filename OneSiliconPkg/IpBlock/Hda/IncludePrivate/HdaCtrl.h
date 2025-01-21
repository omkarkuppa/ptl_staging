/** @file
  Header file for Hda Controller.

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

@par Specification Reference:
**/

#ifndef _HDA_CTRL_H_
#define _HDA_CTRL_H_

/**
  HDA controller structure
  Stores information required to access to controller registers
  like PCI config space address, BARs and S:B:D:F
**/
typedef struct {
  UINT64    PciCfgBaseAddr;
  UINT16     Segment;
  UINT8     Bus;
  UINT8     Device;
  UINT8     Function;
  UINTN     HdaMmioAddress;
  UINTN     DspMmioAddress;
} HDA_CONTROLLER;

#endif
