/** @file
  I3C Controller Handle Defines.

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

#ifndef __I3C_CONTROLLER_H_
#define __I3C_CONTROLLER_H_

#include <RegisterAccess.h>

/**
  LPSS I3C controller structure
  Stores information required to access the controller registers
  like base address, S:B:D:F, Port ID, access definition etc.
  Can also contain information sepecific to the controller
  that cannot be obtained from the hardware like number of active ports.
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
} LPSS_I3C_CONTROLLER;

#endif // __I3C_CONTROLLER_H_
