/** @file
  SCS PEI init library header file.

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

#ifndef _PEI_SCS_INTERNAL_H_
#define _PEI_SCS_INTERNAL_H_

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <ScsConfig.h>
#include <Register/ScsRegs.h>
#include <ScsUfsController.h>
#include <ScsSdController.h>
#include "PeiIosf2Ocp.h"

/**
  Programs passed MmioBase address into BAR register.

  @param[in] PciBaseAddress  Address of the PCI config space
  @param[in] MmioBase        Address to be used to access MMIO space
**/
VOID
ScsControllerEnableMmio (
  IN UINT64  PciBaseAddress,
  IN UINT64  MmioBase
  );

/**
  This function clears BAR address.

  @param[in] PciBaseAddress  Address of PCI config space
**/
VOID
ScsControllerDisableMmio (
  IN UINT64  PciBaseAddress
  );

/**
  This function puts SCS controller into D3 state

  @param[in] PciBaseAddress  Address of the PCI config space
**/
VOID
ScsControllerPutToD3 (
  IN UINT64  PciBaseAddress
  );

/**
  Initializes MMP instance

  @param[in] MmpInstance        MMP instance
**/
VOID
MmpInit (
  IN SCS_UFS_MMP   *MmpInstance
  );

/**
  Disables MMP lanes

  @param[in] MmpInstance  MMP instance
**/
VOID
MmpDisable (
  IN SCS_UFS_MMP  *MmpInstance
  );

#endif

