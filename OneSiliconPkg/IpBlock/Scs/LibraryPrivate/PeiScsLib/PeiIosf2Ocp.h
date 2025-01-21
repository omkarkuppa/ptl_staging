/** @file
  Iosf2Ocp header.

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

#ifndef _PEI_IOSF2OCP_H_
#define _PEI_IOSF2OCP_H_

#include <Iosf2OcpPort.h>

/**
  This function disables BAR1 for a given controller on Iosf2Ocp brige.

  @param[in]  Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
**/
VOID
Iosf2OcpDisableBar1 (
  IN IOSF2OCP_PORT  *Iosf2OcpPort
  );

/**
  Configures interrupts in the Iosf2Ocp bridge.

  @param[in] Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
  @param[in] InterruptPin  Interrupt pin number
  @param[in] Irq           IRQ number
**/
VOID
Iosf2OcpConfigureInterrupts (
  IN IOSF2OCP_PORT  *Iosf2OcpPort,
  IN UINT8          InterruptPin,
  IN UINT8          Irq
  );

/**
  Disable OCP timeout on a IOSF2OCP bridge.

  @param[in] Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
**/
VOID
Iosf2OcpDisableTimeout (
  IN IOSF2OCP_PORT  *Iosf2OcpPort
  );

/**
  This function enables UFS on Iosf2Ocp bridge.
  It supports up to 2 UFS HC on a single bridge.

  @param[in] UfsPort       Pointer to IOSF2OCP_PORT structure describing UFS controller
  @param[in] UsedLanesNum  Number of lanes used by UFS controller
**/
VOID
Iosf2OcpEnableUfs (
  IN IOSF2OCP_PORT  *UfsPort,
  IN UINT32         UsedLanesNum
  );

/**
  Disables UFS controller on Iosf2Ocp bridge.

  @param[in] UfsPort  Structure describing UFS controller location on Iosf2Ocp bridge.
**/
VOID
Iosf2OcpDisableUfs (
  IN IOSF2OCP_PORT  *UfsPort
  );

/**
  Unlocks fields in eMMC DLL registers.

  @param[in] EmmcPort  Pointer to IOSF2OCP_PORT structure describing eMMC controller
**/
VOID
Iosf2OcpUnlockEmmcDll (
  IN IOSF2OCP_PORT  *EmmcPort
  );

/**
  Disables eMMC controller on Iosf2Ocp bridge.

  @param[in] EmmcPort  Structure describing eMMC controller location on Iosf2Ocp bridge
**/
VOID
Iosf2OcpDisableEmmc (
  IN IOSF2OCP_PORT  *EmmcPort
  );

/**
  Configures 1p8 voltage related registers in Iosf2Ocp bridge.

  @param[in] SdCardPort             Pointer to IOSf2OCP_PORT structure describing SdCard controller
  @param[in] PowerEnableActiveHigh  Flag to indicate if POWER_ENABLE# should be active high
**/
VOID
Iosf2OcpSdCard1p8Enable (
  IN IOSF2OCP_PORT  *SdCardPort,
  IN BOOLEAN        PowerEnableActiveHigh
  );

/**
  Disables SdCard controller on Iosf2Ocp bridge.

  @param[in] SdCardPort  Structure describing SdCard location on Iosf2Ocp bridge
**/
VOID
Iosf2OcpDisableSdCard (
  IN IOSF2OCP_PORT  *SdCardPort
  );

/**
  Enables UFS inline encryption feature for UFS controller.

  @param[in] UfsPort  Pointer to IOSF2OCP_PORT structure describing UFS controller
**/
VOID
Iosf2OcpEnableUfsInlineEncryption (
  IN IOSF2OCP_PORT  *UfsPort
  );

#endif

