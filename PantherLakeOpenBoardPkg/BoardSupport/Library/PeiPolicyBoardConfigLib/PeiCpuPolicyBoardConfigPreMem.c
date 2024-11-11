/** @file
 Intel PEI CPU Pre-Memory Policy update by board configuration

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

#include "PeiPolicyBoardConfig.h"
#include <Register/GenerationMsr.h>

/**
  This function performs PEI CPU Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi         The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspmUpd;
#else
  CPU_INIT_PREMEM_CONFIG          *CpuInitPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Updating CPU Policy by board config in Pre-Mem\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  CpuInitPreMemConfig     = NULL;
  CpuSecurityPreMemConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR(Status);
#endif
  //
  //  Cpu Config Lib policies
  //
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.CpuRatio, CpuInitPreMemConfig->CpuRatio, PcdGet8 (PcdCpuRatio));
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuard, CpuSecurityPreMemConfig->BiosGuard, PcdGet8 (PcdBiosGuard));
  UPDATE_POLICY_V2 (((FSPM_UPD *) FspmUpd)->FspmConfig.TsegSize, CpuInitPreMemConfig->TsegSize, PcdGet32 (PcdTsegSize));

  return EFI_SUCCESS;
}
