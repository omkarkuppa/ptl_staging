/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.

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
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>
#include <TxtConfig.h>

/**
  This function performs Security PEI Policy initialization in Pre-memory.

  @param[in] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance
  @param[in] FspmUpd               The pointer of FspmUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSecurityPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi,
  IN     FSPM_UPD             *FspmUpd
  )
{
  EFI_STATUS                    Status;
  CPU_SECURITY_PREMEM_CONFIG    *CpuSecurityPreMemConfig;
  TXT_PREMEM_CONFIG             *TxtPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  ///Test RC Policies
  ///
  CpuSecurityPreMemConfig->SkipStopPbet = FspmUpd->FspmConfig.SkipStopPbet;

  ///
  ///Production Policies
  ///
  CpuSecurityPreMemConfig->ResetAux                  = FspmUpd->FspmConfig.ResetAux;
  CpuSecurityPreMemConfig->TseEnable                 = FspmUpd->FspmConfig.TseEnable;
  CpuSecurityPreMemConfig->BiosGuard                 = FspmUpd->FspmConfig.BiosGuard;
  CpuSecurityPreMemConfig->BiosGuardToolsInterface   = FspmUpd->FspmConfig.BiosGuardToolsInterface;
  CpuSecurityPreMemConfig->Txt                       = FspmUpd->FspmConfig.Txt;

  CpuSecurityPreMemConfig->BiosSize  = FspmUpd->FspmConfig.BiosSize;
  TxtPreMemConfig->SinitMemorySize   = FspmUpd->FspmConfig.SinitMemorySize;
  TxtPreMemConfig->TxtHeapMemorySize = FspmUpd->FspmConfig.TxtHeapMemorySize;
  TxtPreMemConfig->TxtDprMemoryBase  = FspmUpd->FspmConfig.TxtDprMemoryBase;
  TxtPreMemConfig->TxtDprMemorySize  = FspmUpd->FspmConfig.TxtDprMemorySize;
  TxtPreMemConfig->BiosAcmBase       = FspmUpd->FspmConfig.BiosAcmBase;
  TxtPreMemConfig->BiosAcmSize       = FspmUpd->FspmConfig.BiosAcmSize;
  TxtPreMemConfig->TgaSize           = FspmUpd->FspmConfig.TgaSize;
  TxtPreMemConfig->TxtLcpPdBase      = FspmUpd->FspmConfig.TxtLcpPdBase;
  TxtPreMemConfig->TxtLcpPdSize      = FspmUpd->FspmConfig.TxtLcpPdSize;
  TxtPreMemConfig->ApStartupBase     = FspmUpd->FspmConfig.ApStartupBase;
  TxtPreMemConfig->IsTPMPresence     = FspmUpd->FspmConfig.IsTPMPresence;
  TxtPreMemConfig->TxtImplemented    = 0; // Currently TXT is not ready for FSP Wrapper. TxtPreMemConfig->TxtImplemented = FspmUpd->TxtImplemented;
  TxtPreMemConfig->CmosTxtOffset     = FspmUpd->FspmConfig.CmosTxtOffset; // Update CMOS TXT Offset configuration 

  CpuSecurityPreMemConfig->TdxSeamldrSvn = FspmUpd->FspmConfig.TdxSeamldrSvn;
  CpuSecurityPreMemConfig->TmeExcludeBase = FspmUpd->FspmConfig.TmeExcludeBase;
  CpuSecurityPreMemConfig->TmeExcludeSize = FspmUpd->FspmConfig.TmeExcludeSize;
  CpuSecurityPreMemConfig->GenerateNewTmeKey =  FspmUpd->FspmConfig.GenerateNewTmeKey;

  #if FixedPcdGetBool (PcdTdxEnable) == 1
    CpuSecurityPreMemConfig->TdxEnable = FspmUpd->FspmConfig.TdxEnable;
    CpuSecurityPreMemConfig->TdxActmModuleAddr = FspmUpd->FspmConfig.TdxActmModuleAddr;
    CpuSecurityPreMemConfig->TdxActmModuleSize = FspmUpd->FspmConfig.TdxActmModuleSize;
  #endif


  return EFI_SUCCESS;
}
