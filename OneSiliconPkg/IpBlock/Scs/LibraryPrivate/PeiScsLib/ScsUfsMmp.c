/** @file
  Support routines for programming UFS MMP.

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

#include <Register/ScsUfsRegs.h>
#include "PeiScsInitInternal.h"
#include <RegisterAccess.h>

/**
  Returns the maximum number of lanes supported by MMP
**/
STATIC
UINT8
MmpGetMaxLanes (
  VOID
  )
{
  return 7;
}

/**
  Writes a 1 byte value to specified MMP Private register

  @param[in] PortId          MMP port id
  @param[in] MmpLaneIndex    MMP lane index
  @param[in] RegisterOffset  Offset of a private register
  @param[in] Value           Value to be written
**/
STATIC
VOID
MmpPrivateWrite8 (
  IN REGISTER_ACCESS *MmpAccess,
  IN UINT8           MmpLaneIndex,
  IN UINT16          RegisterOffset,
  IN UINT8           Value
  )
{
  UINT16  LaneBaseAddress;

  LaneBaseAddress = R_SCS_PCR_MMPUFS_DLANE1 * MmpLaneIndex;

  MmpAccess->Write8 (MmpAccess, LaneBaseAddress + RegisterOffset, Value);
  //
  // After writing to private register we have to trigger transaction
  // by writing to configuration update field in IMP22 private register.
  //
  MmpAccess->AndThenOr32 (
    MmpAccess,
    LaneBaseAddress + R_SCS_PCR_MMPUFS_DLANEX_IMP22,
    (UINT32) ~B_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD,
    V_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD << N_SCS_PCR_MMPUFS_DLANEX_IMP22_CNFGUPD
    );
}

/**
  Configures power gating on MMP lane

  @param[in] MmpAccess     MMP Register Access
  @param[in] MmpLaneIndex  Lane index of the MMP
**/
STATIC
VOID
MmpLanePowerGatingEnable (
  IN REGISTER_ACCESS *MmpAccess,
  IN UINT8           MmpLaneIndex
  )
{
  MmpPrivateWrite8 (MmpAccess, MmpLaneIndex, R_SCS_PCR_MMPUFS_DLANEX_IMP8_PWRGT, 0x1);
}

/**
  Initializes MMP lane

  @param[in] MmpAccess     MMP Register Access
  @param[in] MmpLaneIndex  Lane index of the MMP
**/
STATIC
VOID
MmpLaneInitialize (
  IN REGISTER_ACCESS *MmpAccess,
  IN UINT8           MmpLaneIndex
  )
{
  MmpPrivateWrite8 (MmpAccess, MmpLaneIndex, R_SCS_PCR_MMPUFS_DLANEX_TX3_SYNC_LEN, 0x4A);
  MmpPrivateWrite8 (MmpAccess, MmpLaneIndex, R_SCS_PCR_MMPUFS_DLANEX_IMP23_INTFILTER, 0x1);
}

/**
  Initializes MMP instance

  @param[in] MmpInstance        MMP instance
**/
VOID
MmpInit (
  IN SCS_UFS_MMP  *MmpInstance
  )
{
  UINT8         MmpLaneIndex;

  if (MmpInstance == NULL) {
    ASSERT (FALSE);
    return;
  }
  for (MmpLaneIndex = 0; MmpLaneIndex < MmpGetMaxLanes (); MmpLaneIndex++) {
    if (((MmpInstance->LaneMask >> MmpLaneIndex) & BIT0) != 0) {
      MmpLanePowerGatingEnable (MmpInstance->RegisterAccess, MmpLaneIndex);
      MmpLaneInitialize (MmpInstance->RegisterAccess, MmpLaneIndex);
    }
  }
}

/**
  Disables MMP lanes

  @param[in] MmpInstance  MMP instance
**/
VOID
MmpDisable (
  IN SCS_UFS_MMP  *MmpInstance
  )
{
  UINT8  MmpLaneIndex;

  if (MmpInstance == NULL) {
    ASSERT (FALSE);
    return;
  }
  for (MmpLaneIndex = 0; MmpLaneIndex < MmpGetMaxLanes (); MmpLaneIndex++) {
    if (((MmpInstance->LaneMask >> MmpLaneIndex) & BIT0) != 0) {
      MmpLanePowerGatingEnable (MmpInstance->RegisterAccess, MmpLaneIndex);
    }
  }
}

