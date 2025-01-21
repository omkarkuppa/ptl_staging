/** @file
  Header file for PeiScsLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PEI_SCS_LIB_H_
#define _PEI_SCS_LIB_H_

#include <ScsSdController.h>
#include <ScsUfsController.h>

/**
  Configure SD card controller in SCS.

  @param[in] ScsSdCardHandle  Handle.
**/
VOID
ScsSdCardInit (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

/**
  Configure eMMC controller in SCS.

  @param[in] ScsEmmcHandle  Handle.
**/
VOID
ScsEmmcInit (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

/**
  Configure UFS controller in SCS.

  @param[in] ScsUfsHandle  Handle.
**/
VOID
ScsUfsInit (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  );

/**
  This function configures power and clock gating policy in Iosf2Ocp bridge
  based on value passed by the caller.

  @param[in] BridgeAccess              Bridge Register Access handle
  @param[in] PowerAndClockGatingValue  Value to be written to power and clock gating register
**/
VOID
Iosf2OcpConfigurePowerAndClockGating (
  IN REGISTER_ACCESS *BridgeAccess,
  IN UINT32          PowerAndClockGatingValue
  );

/**
  Enables Iosf2Ocp bridge to send snooped transactions on
  IOSF fabric.

  @param[in] BridgeAccess              Bridge Register Access handle
**/
VOID
Iosf2OcpEnableSnoopedTransactions (
  IN REGISTER_ACCESS *BridgeAccess
  );

#endif
