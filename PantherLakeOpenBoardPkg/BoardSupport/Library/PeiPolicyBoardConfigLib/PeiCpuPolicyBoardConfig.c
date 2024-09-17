/** @file
 Intel PEI CPU Policy update by board configuration

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
#include <FspsUpd.h>

/**
  This function performs PEI CPU Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  CPU_INIT_CONFIG                  *CpuInitConfig;
  #if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd; 
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
  #endif

  Status                      = EFI_SUCCESS;
  CpuInitConfig               = NULL;

  DEBUG ((DEBUG_INFO, "Updating CPU Policy by board config in Post-Mem\n"));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Directly assign the microcode flash location to FSPS UPD (API mode) or CpuInitConfig (Dispatch mode)
  // The microcode copy from flash to memory will be done just before microcode reload.
  //
  UPDATE_POLICY_V2 (
    ((FSPS_UPD *)FspsUpd)->FspsConfig.MicrocodeRegionBase,CpuInitConfig->MicrocodePatchAddress,
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );
  UPDATE_POLICY_V2 (
    ((FSPS_UPD *)FspsUpd)->FspsConfig.MicrocodeRegionSize,CpuInitConfig->MicrocodePatchRegionSize,
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv)
    );

  return Status;
}
