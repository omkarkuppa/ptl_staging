/** @file
  This file defines the PCH PCIE IoTrap Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _PCH_PCIE_IOTRAP_H_
#define _PCH_PCIE_IOTRAP_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                       gPchPcieIoTrapProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_PCIE_IOTRAP_PROTOCOL PCH_PCIE_IOTRAP_PROTOCOL;

///
/// Pcie Trap valid types
///
typedef enum {
  PchPciePmTrap,
  PcieTrapTypeMaximum
} PCH_PCIE_TRAP_TYPE;

/**
 This protocol is used to provide the IoTrap address to trigger PCH PCIE call back events
**/
struct _PCH_PCIE_IOTRAP_PROTOCOL {
  UINT16      PcieTrapAddress;
};

#endif
