/** @file
 Intel PEI CPU Policy update by board configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/Tpm2CommandLib.h>
#include <PolicyUpdateMacro.h>
#include <Library/EcMiscLib.h>

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
  EFI_STATUS                      Status;
  VOID                            *Sha384Context;
  VOID                            *Data;
  UINTN                           DataSize;
  UINT8                           HashValue[SHA384_DIGEST_SIZE];
  BIOSGUARD_HOB                   *BiosGuardHobPtr;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  VOID                            *FspmUpd;
#else
  BIOS_GUARD_CONFIG               *BiosGuardConfig;
  CPU_SECURITY_PREMEM_CONFIG      *CpuSecurityPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Updating CPU Policy by board config in Post-Mem\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);

  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  BiosGuardConfig         = NULL;
  CpuSecurityPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR(Status);

#if FixedPcdGetBool(PcdBiosGuardEnable) == 1
    Status = GetConfigBlock ((VOID *)SiPolicyPpi, &gBiosGuardConfigGuid, (VOID *) &BiosGuardConfig);
    ASSERT_EFI_ERROR(Status);
#endif
#endif // FspMode Check

  //
  // Make sure ReadOnlyVariablePpi is available
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR(Status);

#if FixedPcdGet8(PcdFspModeSelection) == 1
  if (((FSPM_UPD *) FspmUpd)->FspmConfig.BiosGuard == TRUE) {
#else
  if (CpuSecurityPreMemConfig->BiosGuard == TRUE) {
#endif
    BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
    if (BiosGuardHobPtr == NULL) {
      DEBUG ((DEBUG_ERROR, "BIOS Guard HOB not available\n"));
      return EFI_NOT_FOUND;
    }

    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EcProvisionEav, BiosGuardConfig->EcProvisionEav, (EFI_PHYSICAL_ADDRESS)(UINTN)EcProvisionEav);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.EcBiosGuardCmdLock, BiosGuardConfig->EcBiosGuardCmdLock, (EFI_PHYSICAL_ADDRESS)(UINTN)EcCmdLock);

    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.BiosGuardAttr, BiosGuardConfig->BiosGuardAttr, BiosGuardHobPtr->Bgpdt.BiosGuardAttr);
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.BiosGuardModulePtr, BiosGuardConfig->BiosGuardModulePtr, BiosGuardHobPtr->BiosGuardModulePtr);

    DataSize = Sha384GetContextSize ();
    Status   = PeiServicesAllocatePool (DataSize, &Sha384Context);
    ASSERT_EFI_ERROR (Status);
    DataSize = BiosGuardHobPtr->Bgpdt.BgpdtSize;
    Data     = (VOID *) &BiosGuardHobPtr->Bgpdt;
    Sha384Init   (Sha384Context);
    Sha384Update (Sha384Context, Data, DataSize);
    Sha384Final  (Sha384Context, HashValue);
    COPY_POLICY (&((FSPS_UPD *) FspsUpd)->FspsConfig.BgpdtHash[0], &BiosGuardConfig->BgpdtHash[0], &HashValue[0], 48);
  }

  return Status;
}
