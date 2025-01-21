/** @file
  BPK Private library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Register/BpkRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <RegisterAccess.h>

/**
  Return BPK connection status

  @param[in]  *BpkPcrAccess       Instance of BPK sideband Register Access Protocol

  @retval TRUE   BPK is connected
  @retval FALSE  BPK is disconnected
**/
BOOLEAN
IsBpkConnected (
  REGISTER_ACCESS            *BpkPcrAccess
  )
{
  if (!IsSimicsEnvironment ()) {
    DEBUG ((DEBUG_INFO, "BPK [%x] = 0x%x\n", R_BPK_PCR_ARB_GRANT_STAT, BpkPcrAccess->Read32 (BpkPcrAccess, R_BPK_PCR_ARB_GRANT_STAT)));
    if (BpkPcrAccess->Read32 (BpkPcrAccess, R_BPK_PCR_ARB_GRANT_STAT) == 0xffffffff) {
      DEBUG ((DEBUG_ERROR, "BPK read arbitration grant failed\n"));
      return FALSE;
    }
    if (BpkPcrAccess->Read32 (BpkPcrAccess, R_BPK_PCR_ARB_GRANT_STAT) & B_BPK_PCR_ARB_GRANTED) {
      DEBUG ((DEBUG_INFO, "BPK is connected\n"));
      return TRUE;
    }
  }
  return FALSE;
}

