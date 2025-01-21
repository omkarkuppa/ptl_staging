/** @file
  This file contains functions that implement PCIe configuration space access protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _PCI_CONFIG_ACCESS_LIB_H_
#define _PCI_CONFIG_ACCESS_LIB_H_

#include <RegisterAccess.h>

/**
  Definition of PCIe Configuration Space access protocol
**/
typedef struct {
  REGISTER_ACCESS   RegAccess;
  UINT8             Seg;
  UINT8             Bus;
  UINT8             Dev;
  UINT8             Fun;
} PCI_CONFIG_SPACE_ACCESS;

/**
  Initializes PCI_CONFIG_SPACE_ACCESS based on Segment/Bus/Device/Function.

  @param[in]  Seg                     PCIe Segment
  @param[in]  Bus                     PCIe Bus
  @param[in]  Dev                     PCIe Device
  @param[in]  Fun                     PCIe Function
  @param[out] PcieConfigSpaceAccess   Structure to be initialized

  @retval TRUE   Initialization successful
  @retval FALSE  Failed to initialize
**/
BOOLEAN
PciConfigAccessInit (
  IN UINT8                      Seg,
  IN UINT8                      Bus,
  IN UINT8                      Dev,
  IN UINT8                      Fun,
  OUT PCI_CONFIG_SPACE_ACCESS   *PcieConfigSpaceAccess
  );

#endif // _PCI_CONFIG_ACCESS_LIB_H_
