/** @file
  Initilize FUSA device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Ppi/SiPolicy.h>
#include <FusaConfig.h>
#include <Library/PeiFusaLib.h>
#include <Library/PeiServicesLib.h>
#include <Core/Pei/PeiMain.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/FusaInfoLib.h>

/**
  Fusa Get Startup Pattern Addr

  @param[in] SI_POLICY_PPI      SiPolicy

  @retval UINT32          - FUSA Startup Pattern Addr
**/
UINT32
FusaStartupPatternAddr (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  FUSA_CONFIG                 *FusaConfig;
  EFI_STATUS                  Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);

  if (EFI_ERROR (Status)) {
    return 0;
  }

  return FusaConfig->FusaStartupPatternAddr;
}
