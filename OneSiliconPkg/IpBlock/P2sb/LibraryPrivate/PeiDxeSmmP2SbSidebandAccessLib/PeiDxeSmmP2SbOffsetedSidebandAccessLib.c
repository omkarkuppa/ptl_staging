/** @file
  P2SB offseted sideband access lib

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

#include <Uefi/UefiBaseType.h>
#include <Library/P2SbSidebandAccessLib.h>


/**
  Builds P2SB sideband access using offset to config space.
  This offset will be added to every register offset, when access callbacks are used.
  Some IPs, like for example DMI, implement multiple register banks in the same register space.
  For instance sideband register space can contain both PCR registers,
  starting from the beginning of the register space,
  and PCI config registers starting from the offset in the same register space.

  @param[in]  P2SbController        Pointer to P2SB controller implementing the sideband
  @param[in]  P2SbPid               Port id
  @param[in]  Fid                   Function id
  @param[in]  RegisterSpace         Target register space
  @param[in]  AccessMethod          Access method
  @param[in]  PostedWrites          If TRUE writes sent through sideband msg will be posted
  @param[in]  Offset                Register space offset that will be added to register offset
  @param[out] P2SbSidebandAccess    An initialized sideband access descriptor and register space offset

  @retval TRUE   Access initialized successfuly
  @retval FALSE  Failed to initialize access
**/
BOOLEAN
BuildP2SbSidebandAccessEx (
  IN P2SB_CONTROLLER                    *P2SbController,
  IN P2SB_PID                           P2SbPid,
  IN UINT16                             Fid,
  IN P2SB_REGISTER_SPACE                RegisterSpace,
  IN P2SB_SIDEBAND_ACCESS_METHOD        AccessMethod,
  IN BOOLEAN                            PostedWrites,
  IN UINT32                             Offset,
  OUT P2SB_SIDEBAND_REGISTER_ACCESS     *P2SbSidebandAccess
  )
{
  BuildP2SbSidebandAccess (
    P2SbController,
    P2SbPid,
    Fid,
    RegisterSpace,
    AccessMethod,
    PostedWrites,
    P2SbSidebandAccess
    );
  P2SbSidebandAccess->Offset = Offset;

  return TRUE;
}