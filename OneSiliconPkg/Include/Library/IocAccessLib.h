/** @file
  Ioc library header for abstraction of Ioc registers access

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
#ifndef _IOC_ACCESS_LIB_H_
#define _IOC_ACCESS_LIB_H_

/**
  Check IOC BIOS interface is locked

  @retval TRUE  IOC BIOS interface is locked
          FALSE IOC BIOS interface is not locked
**/
BOOLEAN
IocIsBiosInterfaceLock (
  VOID
  );

/**
  Gets the extended BIOS region base and limit addresses

  @param[out] UINT64  Extended BIOS Region Range Base address
  @param[out] UINT64  Extended BIOS Region Range Limit address
**/
VOID
IocGetExtendedBiosRegionRange (
  OUT  UINT64   *RangeBase,
  OUT  UINT64   *RangeLimit
  );

/**
  Checks lock of extended BIOS Region Limit address

  @retval TRUE    Extended BIOS Region limit address is locked
  @retval FALSE   Extended BIOS Region limit address is unlocked
**/
BOOLEAN
IocIsExtendedBiosRegionLimitLocked (
  VOID
  );
#endif // _IOC_ACCESS_LIB_H_

