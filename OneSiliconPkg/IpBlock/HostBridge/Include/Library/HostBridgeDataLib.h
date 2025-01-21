/** @file
  The GUID definition for Host Bridge Data Library

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

@par Specification Reference:
**/
#ifndef _HOST_BRIDGE_DATA_LIB_H_
#define _HOST_BRIDGE_DATA_LIB_H_

/**
  This function will get 16-bit value of REG BAR from Device 0.

  @return The 16-bit value of REG BAR.
**/
UINT16
EFIAPI
GetRegBar (
  VOID
  );

/**
  This function will return the 16-bit Device Id value of Device 0.

  @return the 16-bit value of DID.
**/
UINT16
EFIAPI
GetHostBridgeDeviceId (
  VOID
  );

  /**
  Print HostBridege PCI space in Debug log.

**/
VOID
HostBridgePciPrint (
  VOID
  );
#endif
