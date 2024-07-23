/** @file
  PTL SoC SSE init.

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

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>

/**
  Initializes PTL SoC SSE/SSE++ subsystem.
  All SSE/SSE++ devices should be hidden and disabled.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdSseInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  ME_PEI_CONFIG                   *MePeiConfig;
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  if (MePeiConfig->SseCommunication == FORCE_ENABLE) {
    return;
  } else if (MePeiConfig->SseCommunication == FORCE_DISABLE) {
    PtlPcdPsfDisableSse (1);
  }

  PtlPcdPsfDisableSse (2);
  PtlPcdPsfDisableSse (3);

  DEBUG ((DEBUG_INFO, "%a() exit\n", __FUNCTION__));
}