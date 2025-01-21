/** @file
  This file is SampleCode of the library for Intel FspV Post-Mem
  Platform Policy Update.

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <FspsUpd.h>
#include <Library/ConfigBlockLib.h>
#include <FspVConfig.h>

/**
  This function performs FspV PEI Policy update in post-memory.

  @param[in out] SiPolicy    SI_POLICY_PPI
  @param[in]     FspsUpd     The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiFspVPolicy (
  IN OUT SI_POLICY_PPI  *SiPolicy,
  IN     FSPS_UPD       *FspsUpd
  )
{
  EFI_STATUS           Status;
  FSPV_POSTMEM_CONFIG  *FspVPostMemConfig;

  DEBUG ((DEBUG_INFO, "FspUpdatePeiFspVPolicyPostMem\n"));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFspVConfigGuid, (VOID *) &FspVPostMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR ((Status))) {
    return EFI_NOT_FOUND;
  }

  FspVPostMemConfig = (FSPV_POSTMEM_CONFIG *) (UINTN) FspsUpd->FspsConfig.FspsValidationPtr;

  return EFI_SUCCESS;
}

