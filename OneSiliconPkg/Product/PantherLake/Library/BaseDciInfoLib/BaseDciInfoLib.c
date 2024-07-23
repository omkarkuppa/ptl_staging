/** @file
  Base DCI Library

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

#include <Library/BaseDciInfoLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

/**
  Get Dci Mailbox Address

  @param[in] Index    Indicate which DCI mailbox

  @retval Address of DCI mailbox
**/
UINT64
GetDciMailboxAddr (
  IN DCI_MAILBOX_REG              Index
  )
{
  UINTN                          ExiMailboxIndexAddr;
  UINT32                          Offset = 0;
  P2SB_PORT_16_ID                 P2SBPid;

  switch (Index) {
    case Emb0:
      Offset = EXI_MAILBOX_REG_OFFSET (Emb0);
      break;
    case Emb1:
      Offset = EXI_MAILBOX_REG_OFFSET (Emb1);
      break;
    case Emb2:
      Offset = EXI_MAILBOX_REG_OFFSET (Emb2);
      break;
    case Emb3:
      Offset = EXI_MAILBOX_REG_OFFSET (Emb3);
      break;
    default:
      break;
  }

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  ExiMailboxIndexAddr = PCH_PCR_ADDRESS (P2SBPid.PortId.LocalPid, Offset);

  return ExiMailboxIndexAddr;
}

/**
  Return DCI host connection status

  @retval TRUE   DCI host is connected
  @retval FALSE  DCI host is not connected
**/
BOOLEAN
IsHostConnected (
  VOID
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);

  return !!(IsDciHostConnected ((REGISTER_ACCESS *) &DciPcrAccess));
}
