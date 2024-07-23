/** @file
  PEIM to Memory Cache Init Library.

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

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuMailboxLib.h>
#include <Ppi/SiPolicy.h>
#include <HostBridgeConfig.h>
#include "MemoryCacheInitLib.h"


/**
  Memory cache initalization.

  @param[in]  SiPreMemPolicyPpi      The SI PreMem Policy PPI instance

  @retval     EFI_UNSUPPORTED        Unsupported
  @retval     EFI_INVALID_PARAMETER  Invalid input parameter
  @retval     EFI_SUCCES             Initialized successfully
**/
EFI_STATUS
PeiMemoryCacheInit (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{

  if (SiPreMemPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  return EFI_SUCCESS;
}