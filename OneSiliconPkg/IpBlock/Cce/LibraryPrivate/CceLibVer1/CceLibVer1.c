/** @file
  Initilize CCE in MCHBAR

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Library/CceLib.h>
#include <Library/CceInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/NguInfoLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/BaseLib.h>
#include <Library/MemorySubSystemInfoLib.h>
#include <CMrcInterface.h>

typedef union {
  struct {
    UINT64 KeyId              :1;
    UINT64 Reserved           :1;
    UINT64 HashEnabled        :1;
    UINT64 IrpInitialized     :1;
    UINT64 Reserved1          :24;
    UINT64 HashLsb            :3;
    UINT64 Reserved2          :1;
    UINT64 HashMask           :14;
    UINT64 Reserved3          :18;
  } Bits;
  UINT64 Data;
} TME_CONTROL_MCHBAR;

/**
  Set TME_CONTROL, REMAP_BASE, and REMAP_LIMIT in CCE MCHBAR register.

  @param[in] CceId           - CCE id.
  @param[in] RemapBase       - Remap Base Address
  @param[in] RemapLimit      - Remap Limit
  @param[in,out] TmeControl  - Tme Control data.

  @retval VOID - No value to return
**/
VOID
SetTmeControl (
  IN CCE_ID                 CceId,
  IN UINT32                 RemapBase,
  IN UINT32                 RemapLimit,
  IN OUT TME_CONTROL_MCHBAR *TmeControl
  )
{
  UINT64                      MchBar;
  UINT32                      CceOffset;
  REMAP_BASE_CCE_MEM_STRUCT   RemapBaseData;
  REMAP_LIMIT_CCE_MEM_STRUCT  RemapLimitData;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  switch (CceId) {
    case CCE0:
      CceOffset = R_SA_MCHBAR_CCE0_OFFSET;
      break;
    case CCE1:
      CceOffset = R_SA_MCHBAR_CCE1_OFFSET;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Unknown CCE number specified.\n"));
      return;
  }

  MmioWrite64 (MchBar + CceOffset + R_SA_MCHBAR_CCE_TME_CONTROL_OFFSET, TmeControl->Data);
  RemapBaseData.Data = GetCceRemapBase (CceId);
  RemapBaseData.Bits.remapbase  = RemapBase;
  MmioWrite64 (MchBar + CceOffset + R_SA_MCHBAR_CCE_REMAP_BASE_OFFSET, RemapBaseData.Data);
  RemapLimitData.Data = GetCceRemapLimit (CceId);
  RemapLimitData.Bits.remaplmt  = RemapLimit;
  MmioWrite64 (MchBar + CceOffset + R_SA_MCHBAR_CCE_REMAP_LIMIT_OFFSET, RemapLimitData.Data);
  TmeControl->Data = MmioRead64 (MchBar + CceOffset + R_SA_MCHBAR_CCE_TME_CONTROL_OFFSET);

  /// print TmeControl values
  DEBUG ((DEBUG_INFO, "CCE-TME-INIT: CCE%d TME_CONTROL_MCHBAR Value:\n", CceId));
  DEBUG ((DEBUG_INFO, " KeyId           = 0x%X\n", TmeControl->Bits.KeyId));
  DEBUG ((DEBUG_INFO, " HashEnabled     = 0x%X\n", TmeControl->Bits.HashEnabled));
  DEBUG ((DEBUG_INFO, " HashLsb         = 0x%X\n", TmeControl->Bits.HashLsb));
  DEBUG ((DEBUG_INFO, " HashMask        = 0x%X\n", TmeControl->Bits.HashMask));
  DEBUG ((DEBUG_INFO, " TmeControl Data = 0x%X\n", TmeControl->Data));

  /// print Remap ranges
  DEBUG ((DEBUG_INFO, " RemapBase Value = 0x%lx\n", GetCceRemapBase (CceId)));
  DEBUG ((DEBUG_INFO, " RemapLimit Value = 0x%lx\n", GetCceRemapLimit (CceId)));
}

/**
  Configure TME_CONTROL in CCE MCHBAR register prior to MRC_DONE event.

  @param[in] RemapBase       - Remap Base Address
  @param[in] RemapLimit      - Remap Limit

  @retval VOID - No value to return
**/
VOID
TmeControlInit (
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit
  )
{
  TME_CONTROL_MCHBAR          TmeControl;
  UINT32                      CceHashLsb;
  UINT32                      CceHashMask;
  BOOLEAN                     Cce0Enable;
  BOOLEAN                     Cce1Enable;

  /// zero out TmeControl variable
  ZeroMem (&TmeControl, sizeof (TME_CONTROL_MCHBAR));

  //check if CCE0 is enable
  Cce0Enable = IsCce0Enable ();
  DEBUG ((DEBUG_INFO, " Cce0Enable        = 0x%X\n", Cce0Enable));

  //check if CCE1 is enable
  Cce1Enable = IsCce1Enable ();
  DEBUG ((DEBUG_INFO, " Cce1Enable        = 0x%X\n", Cce1Enable));

  if (GetMcHashEnable ()) {
    TmeControl.Bits.HashEnabled = 1;

    /// HashLsb and HashMask are only set when HashEnabled is 1
    CceHashMask = GetMcHashMask ();
    CceHashLsb  = GetMcHashLsb ();
    DEBUG ((DEBUG_INFO, " CceHashMask     = 0x%X\n", CceHashMask));
    DEBUG ((DEBUG_INFO, " CceHashLsb      = 0x%X\n", CceHashLsb));

    TmeControl.Bits.HashLsb  = CceHashLsb;
    TmeControl.Bits.HashMask = CceHashMask;
    DEBUG ((DEBUG_INFO, " TmeControl Data = 0x%X\n", TmeControl.Data));
  }

  /// configure TME_CONTROL of CCE0 if enabled
  if (Cce0Enable) {
    SetTmeControl (CCE0, RemapBase, RemapLimit, &TmeControl);
  }

  /// configure TME_CONTROL of CCE1 if enabled
  if (Cce1Enable) {
    SetTmeControl (CCE1, RemapBase, RemapLimit, &TmeControl);
  }
}

/**
  Configure CCE in MCHBAR register prior to MRC_DONE event.

  @param[in] RemapBase       - Remap Base Address
  @param[in] RemapLimit      - Remap Limit

  @retval VOID - No value to return
**/
VOID
CceInit (
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit
  )
{
  /// initialize TME control in CCE
  TmeControlInit(RemapBase, RemapLimit);
}
