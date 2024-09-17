/** @file
  PeiBootGuardLibrary implementation.

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

@par Specification
**/
#include <Library/BootGuardPolicyLib.h>
#include <Library/ConfigBlockLib.h>
#include <CpuRegs.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <CpuSecurityPreMemConfig.h>

/**
  Print CPU_SECURITY_PREMEM_CONFIG and serial out.

  @param[in] CpuSecurityPreMemConfig     Pointer to a CPU_SECURITY_PREMEM_CONFIG
**/
VOID
CpuSecurityPreMemConfigPrint (
  IN CONST CPU_SECURITY_PREMEM_CONFIG        *CpuSecurityPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU_SECURITY_PREMEM_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " BiosSize = 0x%x\n", CpuSecurityPreMemConfig->BiosSize));
  DEBUG ((DEBUG_INFO, " BiosGuard = 0x%x\n", CpuSecurityPreMemConfig->BiosGuard));
  DEBUG ((DEBUG_INFO, " BiosGuardToolsInterface = 0x%x\n", CpuSecurityPreMemConfig->BiosGuardToolsInterface));
  DEBUG ((DEBUG_INFO, " Txt = 0x%x\n", CpuSecurityPreMemConfig->Txt));
  DEBUG ((DEBUG_INFO, " SkipStopPbet = 0x%x\n", CpuSecurityPreMemConfig->SkipStopPbet));
  DEBUG ((DEBUG_INFO, " ResetAux = 0x%x\n", CpuSecurityPreMemConfig->ResetAux));
  DEBUG ((DEBUG_INFO, " TxtAcheckRequest = 0x%x\n", CpuSecurityPreMemConfig->TxtAcheckRequest));
  DEBUG ((DEBUG_INFO, " TxtPowerdownRequest = 0x%x\n", CpuSecurityPreMemConfig->TxtPowerdownRequest));
  DEBUG ((DEBUG_INFO, " TseEnable = 0x%x\n", CpuSecurityPreMemConfig->TseEnable));
  DEBUG ((DEBUG_INFO, " TdxActmModuleAddr = 0x%x\n", CpuSecurityPreMemConfig->TdxActmModuleAddr));
  DEBUG ((DEBUG_INFO, " TdxActmModuleSize = 0x%x\n", CpuSecurityPreMemConfig->TdxActmModuleSize));

  DEBUG ((DEBUG_INFO, "------------------ CPU_SECURITY_PREMEM_CONFIG End ------------------\n"));
}

/**
  Print whole CPU Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
EFIAPI
BootGuardPrintPolicy (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                    Status;
  CPU_SECURITY_PREMEM_CONFIG    *CpuSecurityPreMemConfig;

  DEBUG ((DEBUG_INFO, "\n------------------------ BootGuard Print Begin in PreMem -----------------\n"));

  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  CpuSecurityPreMemConfigPrint(CpuSecurityPreMemConfig);

  DEBUG ((DEBUG_INFO, "\n------------------------ BootGuard Print End -----------------\n\n"));
DEBUG_CODE_END();
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuSecurityPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_SECURITY_PREMEM_CONFIG  *CpuSecurityPreMemConfig;
  CpuSecurityPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuSecurityPreMemConfig->Header.GuidHob.Name = %g\n", &CpuSecurityPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuSecurityPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuSecurityPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Security PreMem configuration
  ********************************/
  CpuSecurityPreMemConfig->BiosSize               = (10 * SIZE_1KB);      /// BIOS Size in KB
#if FixedPcdGetBool (PcdBiosGuardEnable) == 1
  CpuSecurityPreMemConfig->BiosGuard              = CPU_FEATURE_ENABLE;
#else
  CpuSecurityPreMemConfig->BiosGuard              = CPU_FEATURE_DISABLE;
#endif
  CpuSecurityPreMemConfig->BiosGuardToolsInterface = TRUE;
  CpuSecurityPreMemConfig->TmeExcludeBase = 0;
  CpuSecurityPreMemConfig->TmeExcludeSize = 0;
  CpuSecurityPreMemConfig->GenerateNewTmeKey = CPU_FEATURE_DISABLE;
}

static COMPONENT_BLOCK_ENTRY  mBootGuardIpBlocksPreMem [] = {
  {&gCpuSecurityPreMemConfigGuid,    sizeof (CPU_SECURITY_PREMEM_CONFIG),    CPU_SECURITY_PREMEM_CONFIG_REVISION,    LoadCpuSecurityPreMemConfigDefault},
};

/**
  Get CPU PREMEM config block table total size.

  @retval Size of CPU PREMEM config block table
**/
UINT16
EFIAPI
BootGuardGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mBootGuardIpBlocksPreMem[0], sizeof (mBootGuardIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  BootGuardAddConfigBlocks add BootGuard config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add BootGuard config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
BootGuardAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "PeiBootGuardPolicy Pre-Mem Entry \n"));
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mBootGuardIpBlocksPreMem[0], sizeof (mBootGuardIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "PeiBootGuardPolicy Done \n"));

  return Status;
}
