/** @file
  Initializes PCH CNVi device ACPI data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/CnviLib.h>
#include <CnviConfigHob.h>
#include <MeBiosPayloadHob.h>
#include "PchInit.h"
#include <PcrDefine.h>
#include <Library/PcdInfoLib.h>
#include <PcdSbPortIds.h>

/**
  Update ASL definitions for CNVi device.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
UpdateCnviAcpiData (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS HobPtr;
  CNVI_CONFIG_HOB      *CnviConfigHob;
  ME_BIOS_PAYLOAD_HOB  *MbpHob;
  P2SB_PORT_16_ID      P2SBPid;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // Get CNVi Config HOB.
  HobPtr.Guid = GetFirstGuidHob (&gCnviConfigHobGuid);
  if (HobPtr.Guid != NULL) {
    CnviConfigHob = (CNVI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);
    mPchNvsAreaProtocol.Area->CnviWifiCore                = (UINT8) CnviConfigHob->WifiCore;
    mPchNvsAreaProtocol.Area->CnviBtCore                  = (UINT8) CnviConfigHob->BtCore;
    mPchNvsAreaProtocol.Area->CnviBtAudioOffload          = (UINT8) CnviConfigHob->BtAudioOffload;
    mPchNvsAreaProtocol.Area->CnviBtAudioOffloadInterface = (UINT8) CnviConfigHob->BtAudioOffloadInterface;
    mPchNvsAreaProtocol.Area->CnviDdrRfim                 = (UINT8) CnviConfigHob->DdrRfim;
    mPchNvsAreaProtocol.Area->CnviCrfPresent              = (UINT8) CnviCrfModuleIsPresent ();
    mPchNvsAreaProtocol.Area->CnviBtInterface             = (UINT8) CnviConfigHob->BtInterface;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    if ((MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand == IntelAmtBrand) ||
        (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand == IntelStandardManageabilityBrand)) {
      mPchNvsAreaProtocol.Area->CnviVPro = TRUE;
    }
  }
  DEBUG ((DEBUG_INFO, "CnviVPro=%x\n", mPchNvsAreaProtocol.Area->CnviVPro));
  P2SBPid.Pid16bit = PTL_SID_F2_PID_CNVI;
  mPchNvsAreaProtocol.Area->CnviPortId = P2SBPid.PortId.LocalPid;
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));

  return EFI_SUCCESS;
}
