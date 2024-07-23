/** @file
  Initializes PTL CAN

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include "PtlPcdInitPei.h"
#include <Library/BaseLib.h>
#include <Library/CanSsInitLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PchRegs.h>
#include <Register/CanRegs.h>
#include <PtlPcdSbPortIds.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mCanPciCfgCtrOffset [] = {
  R_CAN_PCR_PCICFGCTR1,  // PCICFGCTRL_CAN1
  R_CAN_PCR_PCICFGCTR2   // PCICFGCTRL_CAN2
};

/**
  Add element at the end of the list

  @param ListHead    CAN controllers list head
  @return            Pointer to added device
 */
STATIC
CAN_CTRL *
CanAddTailList (
  CAN_CTRL  **ListHead
  )
{
  CAN_CTRL  *ListPtr;
  CAN_CTRL  *CanCtrl;

  CanCtrl = AllocateZeroPool (sizeof (CAN_CTRL));
  if (CanCtrl == NULL) {
    return NULL;
  }

  if (*ListHead == NULL) {
    *ListHead = CanCtrl;
    return CanCtrl;
  }

  ListPtr = *ListHead;
  while (ListPtr->Next != NULL) {
    ListPtr = ListPtr->Next;
  }

  ListPtr->Next = CanCtrl;

  return CanCtrl;
}

/**
  Initialize CAN Subsystem for PTL SOC

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdCanInit (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
  CAN_SUBSYSTEM                 CanSubsystem;
  CAN_CTRL                      *CanCtrl;
  P2SB_CONTROLLER               P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS CanPcrAccess;
  P2SB_PORT_16_ID               P2SBPid;
  UINT8                         InterruptPin;
  UINT8                         CtrlIndex;

  ZeroMem (&CanSubsystem, sizeof (CAN_SUBSYSTEM));

  //
  // CAN Subsystem - Sideband PCR Space registers access
  //
  P2SBPid.Pid16bit = PTL_A_SID_F2_PID_CANFD;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &CanPcrAccess);
  CanSubsystem.PcrAccess = (REGISTER_ACCESS *) &CanPcrAccess;

  //
  // CAN Controllers list
  //
  for (CtrlIndex = 0; CtrlIndex < PtlPcdGetCanCtrlNum (); CtrlIndex++) {
    ASSERT (CtrlIndex < ARRAY_SIZE (mCanPciCfgCtrOffset));
    CanCtrl = CanAddTailList (&CanSubsystem.Controllers);
    CanCtrl->CtrlIndex = CtrlIndex;
    CanCtrl->PciCfgCtrlOffset = mCanPciCfgCtrOffset[CtrlIndex];
    ItssGetDevIntConfig (
      SiPolicyPpi,
      CanDevNumber (CtrlIndex),
      CanFuncNumber (CtrlIndex),
      &InterruptPin,
      &CanCtrl->IrqNumber
      );
  }

  CanSubsystemInit (&CanSubsystem);
}
