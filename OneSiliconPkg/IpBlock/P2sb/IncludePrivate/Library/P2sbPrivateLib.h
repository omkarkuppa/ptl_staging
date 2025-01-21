/** @file
  Header file for P2sbPrivateLib.

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

#ifndef P2SB_PRIVATE_LIB_H
#define P2SB_PRIVATE_LIB_H

#include <PcrDefine.h>
#include <P2SbHandle.h>
#include <RegisterAccess.h>

/**
  The function check the P2SB hidden or not.

  @param[in] PciAccess             P2SB PCIe register access.

  @return TRUE                     P2SB device not hidden.
  @return FALSE                    P2SB device hidden.
**/
BOOLEAN
P2sbControllerNotExist (
  IN REGISTER_ACCESS *PciAccess
  );

/**
  Check SBREG readiness.

  @param[in] PciAccess             P2SB PCIe register access.

  @retval TRUE                SBREG is ready
  @retval FALSE               SBREG is not ready
**/
BOOLEAN
P2sbIsSbregReady (
  IN  REGISTER_ACCESS *PciAccess
  );

/**
  Internal function performing HPET init in early PEI phase

  @param[in] P2SbHandle            P2SB controller handle
  @param[in] HpetMmio              HPET MMIO
**/
VOID
P2sbHpetInit (
  IN  P2SB_HANDLE                       *P2SbHandle,
  IN  UINTN                             HpetMmio
  );

/**
  Early init P2SB configuration

  @param[in] PciAccess             P2SB PCIe register access.
**/
VOID
P2sbEarlyConfig (
  IN  REGISTER_ACCESS *PciAccess
  );

/**
  The function performs P2SB initialization.

  @param[in] P2SbHandle            P2SB controller handle
**/
VOID
P2sbConfigure (
  IN  P2SB_HANDLE             *P2SbHandle
  );

/**
  Hide P2SB device.

  @param[in] PciAccess             P2SB PCIe register access.
**/
VOID
P2sbHideDevice (
  IN  REGISTER_ACCESS *PciAccess
  );

/**
  Unhide P2SB device.

  @param[in] PciAccess             P2SB PCIe register access.
**/
VOID
P2sbUnhideDevice (
  IN  REGISTER_ACCESS *PciAccess
  );

/**
  Program to which endpoints P2SB can send posted writes when performing a sideband
  access via a P2SB BAR.

  @param[in] PciAccess                      P2SB PCIe register access.
  @param[in] P2SbPostedWriteEndpointsTable  Table of port ids for endpoints which can handle posted writes
  @param[in] TableLength                    Length of the table
**/
VOID
P2SbProgramPostedWriteMask (
  IN REGISTER_ACCESS  *PciAccess,
  IN P2SB_PID         *P2SbPostedWriteEndpointsTable,
  IN UINT32           TableLength
  );

/**
  The function performs P2SB lock programming.

  @param[in] P2SbHandle            P2SB controller handle
**/
VOID
P2sbLock (
  IN  P2SB_HANDLE             *P2SbHandle
  );

/**
  The function configures P2SB Regbar.

  @param[in] PciAccess                 P2SB PCIe register access.
  @param[in] P2sbMmio                  P2SB MMIO
**/
VOID
P2sbRegbarConfigure (
  IN  REGISTER_ACCESS     *PciAccess,
  IN  UINT64              P2sbMmio
  );

/**
  Configures the P2SB IoAPIC Control Register

  @param[in] PciAccess                 P2SB PCIe register access.
  @param[in] IoApicAddress             IoApic address

  @retval      None
**/
VOID
P2SbConfigureIoApic (
  IN  REGISTER_ACCESS     *PciAccess,
  IN  UINT32              IoApicAddress
  );

/**
  The function returns P2SB MMIO base for particular PID.

  Note: This function is used only for one specific case,
  should not be used for any other cases.

  @param[in]  P2SbController        Pointer to P2SB controller implementing the sideband
  @param[in]  P2SbPid               Port id

  @return                           P2SB MMIO base for particular PID.
**/
UINT64
P2sbGetMmioBase (
  IN P2SB_CONTROLLER                    *P2SbController,
  IN P2SB_PID                           P2SbPid
  );

#endif // P2SB_PRIVATE_LIB_H
