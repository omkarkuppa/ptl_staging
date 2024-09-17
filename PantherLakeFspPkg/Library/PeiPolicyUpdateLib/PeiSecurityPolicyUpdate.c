/** @file
  This file is SampleCode of the library for Intel Security PEI Policy initialization.

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

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/DebugLib.h>
#include <Library/CpuPlatformLib.h>
#include <CpuRegs.h>
#include <Library/ConfigBlockLib.h>


/**
  This function performs Security PEI Policy initialization.

  @param[in] SiPolicyPpi           The SI Policy PPI instance
  @param[in] FspsUpd               The pointer of FspsUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSecurityPolicy (
  IN OUT SI_POLICY_PPI        *SiPolicyPpi,
  IN     FSPS_UPD             *FspsUpd
  )
{
  EFI_STATUS                       Status;
  BIOS_GUARD_CONFIG                *BiosGuardConfig;
  UINTN                            Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gBiosGuardConfigGuid, (VOID *) &BiosGuardConfig);
if (EFI_ERROR (Status)) {
  DEBUG ((DEBUG_ERROR, "skip BiosGuard assert\n"));
  ASSERT_EFI_ERROR (Status);
  return EFI_NOT_FOUND;
}

  ///
  /// Production RC Policies
  ///
  for (Index = 0; Index < BIOSGUARD_TOTAL_HASH_MSRS; Index++) {
    BiosGuardConfig->BgpdtHash[Index] = FspsUpd->FspsConfig.BgpdtHash[Index];
  }
  BiosGuardConfig->BiosGuardAttr      = FspsUpd->FspsConfig.BiosGuardAttr;
  BiosGuardConfig->BiosGuardModulePtr = FspsUpd->FspsConfig.BiosGuardModulePtr;
  BiosGuardConfig->EcProvisionEav     = FspsUpd->FspsConfig.EcProvisionEav;
  BiosGuardConfig->EcBiosGuardCmdLock = FspsUpd->FspsConfig.EcBiosGuardCmdLock;

  return EFI_SUCCESS;
}

