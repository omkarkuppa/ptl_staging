/** @file
  Me FWSTS Lib implementation.

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

#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/HobLib.h>
#include <Register/HeciRegs.h>
#include <FwStsSmbiosTable.h>
#include <MeState.h>
#include <MeFwHob.h>

///
/// Global FwSts Device List
///
CONST MEI_DEV_STRING gFwStsDeviceList [MAX_HECI_FUNC] = {
  {HECI1, MEI1_FWSTS_STRING},
  {HECI2, MEI2_FWSTS_STRING},
  {HECI3, MEI3_FWSTS_STRING},
  {HECI4, MEI4_FWSTS_STRING}
};

///
/// Global FwSts Registers Table
///
CONST UINT32 gFwStsOffsetTable [MAX_FWSTS_REG] = {
  R_ME_CFG_HFS,
  R_ME_CFG_HFS_2,
  R_ME_CFG_HFS_3,
  R_ME_CFG_HFS_4,
  R_ME_CFG_HFS_5,
  R_ME_CFG_HFS_6
};

/**
  Get ME Firmware Status from FWSTS Hob by passed register

  @param[in]  FwStsReg     FWSTS register.

  @retval     UINT32       ME firmware status value.
                           In error case 0 returned
**/
UINT32
GetMeStatusFromFwstsHob (
  IN UINT32    FwStsReg
  )
{
  ME_FW_HOB   *MeFwHob;
  UINT32      Index;
  UINT32      MeFirmwareStatus;

  Index            = 0;
  MeFirmwareStatus = 0;
  MeFwHob          = GetFirstGuidHob (&gMeFwHobGuid);
  if (MeFwHob == NULL) {
    return 0;
  }

  for (Index = 0; Index < MAX_FWSTS_REG; Index++) {
    if (FwStsReg == gFwStsOffsetTable [Index]) {
      MeFirmwareStatus = MeFwHob->Group[HECI1].Reg[Index];
      break;
    }
  }

  return MeFirmwareStatus;
}

/**
  Get ME operation mode from FWSTS Hob.

  @retval UINT32             ME operation mode.
**/
UINT32
GetMeModeFromFwstsHob (
  VOID
  )
{
  HECI_FWS_REGISTER    MeFwSts;

  MeFwSts.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
  if (MeFwSts.ul == 0) {
    return ME_MODE_FAILED;
  }

  switch (MeFwSts.r.MeOperationMode) {
    case ME_OPERATION_MODE_NORMAL:
      return ME_MODE_NORMAL;

    case ME_OPERATION_MODE_DEBUG:
      return ME_MODE_DEBUG;

    case ME_OPERATION_MODE_SOFT_TEMP_DISABLE:
      return ME_MODE_TEMP_DISABLED;

    case ME_OPERATION_MODE_SECOVR_JMPR:
    case ME_OPERATION_MODE_SECOVR_HECI_MSG:
      return ME_MODE_SECOVER;

    default:
      return ME_MODE_FAILED;
  }
}
