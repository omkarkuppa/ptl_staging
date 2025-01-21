/** @file
  DXE eSPI private library

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

#ifndef _DXE_ESPI_PRIVATE_LIB_H_
#define _DXE_ESPI_PRIVATE_LIB_H_

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in] PciCfgBase         eSPI controller address in pcicfg space
  @param[in] BiosLockEnable     Policy for BiosLockEnable bit programming
  @param[in] BiosInterfaceLock  Policy for BiosInterfaceLock bit programming
  @param[in] EissEnable         Policy for EISS bit programming

**/
VOID
EspiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN UINT64   PciCfgBase,
  IN BOOLEAN  BiosLockEnable,
  IN BOOLEAN  BiosInterfaceLock,
  IN BOOLEAN  EissEnable
  );

/**
  Lock SPI BIOS decode enable range from change. Saving lock configuration with use of S3BootScript.

  @param[in] PciCfgBase          eSPI controller address in pcicfg space

**/
VOID
EspiBiosDecodeEnableLockWithS3BootScript (
  IN UINT64 PciCfgBase
  );

#endif // _DXE_ESPI_PRIVATE_LIB_H_
