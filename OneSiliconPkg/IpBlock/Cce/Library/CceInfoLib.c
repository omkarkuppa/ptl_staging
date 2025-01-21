/** @file
  Initilize CCE in MCHBAR

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/MemorySubSystemInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CceInfoLib.h>

/**
  This function returns the specified CCE Remap Base value

  @param[in] CceId      - Id of CCE to retrieve value.

  @retval Remap Base
**/
UINT64
EFIAPI
GetCceRemapBase (
  IN CCE_ID CceId
  )
{
  UINT64                                      MchBar;
  R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_STRUCT  MemSsPmaCrMemConfig;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemSsPmaCrMemConfig.Data = MmioRead32 ((UINTN) (MchBar + R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_REG));


  switch (CceId) {
  case CCE0:
    if (MemSsPmaCrMemConfig.Bits.cce0_en) {
      return MmioRead64 (MchBar + R_SA_MCHBAR_CCE0_OFFSET + R_SA_MCHBAR_CCE_REMAP_BASE_OFFSET);
    }
    break;
  case CCE1:
    if (MemSsPmaCrMemConfig.Bits.cce1_en) {
      return MmioRead64 (MchBar + R_SA_MCHBAR_CCE1_OFFSET + R_SA_MCHBAR_CCE_REMAP_BASE_OFFSET);
    }
    break;
  default:
    DEBUG ((DEBUG_ERROR, "Unknown CCE number specified.\n"));
    break;
  }

  return 0;
}

/**
  This function returns the specified CCE Remap Limit value

  @param[in] CceId      - Id of CCE to retrieve value.

  @retval Remap Limit
**/
UINT64
EFIAPI
GetCceRemapLimit (
  IN CCE_ID CceId
  )
{
  UINT64                                      MchBar;

  R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_STRUCT  MemSsPmaCrMemConfig;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemSsPmaCrMemConfig.Data = MmioRead32 ((UINTN) (MchBar + R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_REG));

  switch (CceId) {
  case CCE0:
    if (MemSsPmaCrMemConfig.Bits.cce0_en) {
      return MmioRead64 (MchBar + R_SA_MCHBAR_CCE0_OFFSET + R_SA_MCHBAR_CCE_REMAP_LIMIT_OFFSET);
    }
    break;
  case CCE1:
    if (MemSsPmaCrMemConfig.Bits.cce1_en) {
      return MmioRead64 (MchBar + R_SA_MCHBAR_CCE1_OFFSET + R_SA_MCHBAR_CCE_REMAP_LIMIT_OFFSET);
    }
    break;
  default:
    DEBUG ((DEBUG_ERROR, "Unknown CCE number specified.\n"));
    break;
  }

  return 0;
}
