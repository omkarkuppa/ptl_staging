/** @file
  Force power on YFL router

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

#ifndef _TBT_NVM_DRV_YFL_LIB_
#define _TBT_NVM_DRV_YFL_LIB_

#include <Library/TbtNvmDrvUtils.h>
#include <Protocol/PciIo.h>

//
// Device force power check define.
//
#define FORCE_PWR_REG_OFFSET        0xFC
#define FORCE_PWR_REG_BIT_INDEX     0x01
#define FW_READY_REG_OFFSET         0xC8
#define FW_READY_REG_BIT_INDEX      0x1F

#define TOTAL_WAIT_FOR_PWR_ON_MS    10000

/**
  TbtNvmDrvYflForcePwrFunc function to perform force power on YFL router
**/
TBT_STATUS
TbtNvmDrvYflForcePwrFunc (
  IN  EFI_PCI_IO_PROTOCOL *PciIoProto,
  IN  BOOLEAN             Enable,
  OUT BOOLEAN             *TBTControllerWasPowered
  );

#endif
