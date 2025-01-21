/** @file
  Header file for SPI IP locking library

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
#ifndef _SPI_LOCK_LIB_H_
#define _SPI_LOCK_LIB_H_

#include <SpiHandle.h>

/**
  Lock SPI register before boot

  @param[in]  SpiHandle           SPI controller handle
**/
VOID
LockSpiConfiguration (
  IN  SPI_HANDLE        *SpiHandle
  );

/**
  Enables Extended BIOS Range decoding in SPI controller by programming SPI
  BAR1 and enable bits

  @param[in] SpiHandle       SPI controller handle

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
SpiEnableExtendedBiosRangeSupport (
  SPI_HANDLE  *SpiHandle
  );

/**
  Disable EISS (Enable InSMM.STS)

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiDisableEiss (
  IN SPI_HANDLE       *SpiHandle
  );

/**
  Disable Unsupport Request Reporting Enable

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiDisableUrre (
  IN SPI_HANDLE       *SpiHandle
  );

/**
  Detect EDAF mode

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
BOOLEAN
SpiDetectEdafMode (
  IN SPI_HANDLE       *SpiHandle
  );

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
  @param[in]  BiosLockEnable      Policy for BiosLockEnable bit programming
  @param[in]  BiosInterfaceLock   Policy for BiosInterfaceLock bit programming
  @param[in]  EissEnable          Policy for EISS bit programming
**/
VOID
SpiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN SPI_HANDLE       *SpiHandle,
  IN BOOLEAN          BiosLockEnable,
  IN BOOLEAN          BiosInterfaceLock,
  IN BOOLEAN          EissEnable
  );

/**
  Lock SPI BIOS decode enable range from change. Saving lock configuration with use of S3BootScript.

  @param[in] SpiHandle          Instance of SPI_HANDLE describing SPI Flash Controller

**/
VOID
SpiBiosDecodeEnableLockWithS3BootScript (
  IN SPI_HANDLE   *SpiHandle
  );

/**
  Clears BIOS Write Protect Disable bit

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiClearBiosWriteProtectDisable (
  IN SPI_HANDLE       *SpiHandle
  );

/**
  Install the SPI Info Hob
**/
VOID
InstallSpiInfoHob(
  VOID
);

#endif
