/** @file
  This file load CpuInit Pre memory defaults and config block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/CpuInitPolicyLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <IntelRcStatusCode.h>
#include <Library/SiConfigBlockLib.h>
#include <Register/Cpuid.h>
#include <Library/PeiServicesLib.h>

/**
  Print CPU_INIT_PREMEM_CONFIG and serial out.

  @param[in] CpuInitPreMemConfig     Pointer to a CPU_INIT_PREMEM_CONFIG

**/
VOID
CpuInitPreMemConfigPrint (
  IN CONST CPU_INIT_PREMEM_CONFIG        *CpuInitPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "BootMaxFrequency = 0x%x\n", CpuInitPreMemConfig->BootMaxFrequency));
  DEBUG ((DEBUG_INFO, "BistOnReset = 0x%x\n", CpuInitPreMemConfig->BistOnReset));
  DEBUG ((DEBUG_INFO, "VmxEnable = 0x%x\n", CpuInitPreMemConfig->VmxEnable));
  DEBUG ((DEBUG_INFO, "CrashLogEnable = 0x%x\n", CpuInitPreMemConfig->CrashLogEnable));
  DEBUG ((DEBUG_INFO, "TmeEnable = 0x%x\n", CpuInitPreMemConfig->TmeEnable));
  DEBUG ((DEBUG_INFO, "Tme By_pass Capability = 0x%x\n", CpuInitPreMemConfig->TmeBypassCapability));
  DEBUG ((DEBUG_INFO, "DebugInterfaceEnable = 0x%X\n", CpuInitPreMemConfig->DebugInterfaceEnable));
  DEBUG ((DEBUG_INFO, "DebugInterfaceLockEnable = 0x%X\n", CpuInitPreMemConfig->DebugInterfaceLockEnable));
  DEBUG ((DEBUG_INFO, "ActiveCoreCount = 0x%x\n", CpuInitPreMemConfig->ActiveCoreCount));
  DEBUG ((DEBUG_INFO, "PeciSxReset = 0x%x\n", CpuInitPreMemConfig->PeciSxReset));
  DEBUG ((DEBUG_INFO, "ActiveSmallCoreCount = 0x%x\n", CpuInitPreMemConfig->ActiveSmallCoreCount));
  DEBUG ((DEBUG_INFO, "CrashLogGprs = 0x%x\n", CpuInitPreMemConfig->CrashLogGprs));
  DEBUG ((DEBUG_INFO, "CpuRatio = 0x%x\n", CpuInitPreMemConfig->CpuRatio));
  DEBUG ((DEBUG_INFO, "ActiveLpAtomCoreCount = 0x%x\n", CpuInitPreMemConfig->ActiveLpAtomCoreCount));
  DEBUG ((DEBUG_INFO, "DfdEnable = 0x%x\n", CpuInitPreMemConfig->DfdEnable));
  DEBUG ((DEBUG_INFO, "PrmrrSize = 0x%x\n", CpuInitPreMemConfig->PrmrrSize));
  DEBUG ((DEBUG_INFO, "OcLock = 0x%x\n", CpuInitPreMemConfig->OcLock));
  DEBUG ((DEBUG_INFO, "TsegSize: 0x%x\n", CpuInitPreMemConfig->TsegSize));
  DEBUG ((DEBUG_INFO, "SmmRelocationEnable: 0x%x\n", CpuInitPreMemConfig->SmmRelocationEnable));
  DEBUG ((DEBUG_INFO, "ReduceXecores: 0x%x\n", CpuInitPreMemConfig->ReduceXecores));
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}


/**
  Print whole CPU Config blocks of SI_PREMEM_POLICY_PPI and serial out in PreMem.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
**/
VOID
EFIAPI
CpuInitPreMemPrintPolicy (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                    Status;
  CPU_INIT_PREMEM_CONFIG        *CpuInitPreMemConfig;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "Revision= %x\n", SiPreMemPolicyPpi->TableHeader.Header.Revision));
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  CpuInitPreMemConfigPrint(CpuInitPreMemConfig);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
DEBUG_CODE_END();
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuInitPreMemConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_INIT_PREMEM_CONFIG *CpuInitPreMemConfig;
  CPU_SKU CpuSku;
  UINT8   MaxNonTurboLimRatio;

  CpuInitPreMemConfig = ConfigBlockPointer;
  CpuSku              = GetCpuSku ();

  DEBUG ((DEBUG_INFO, "CpuInitPreMemConfig Name = %g HobLength = 0x%x\n", &CpuInitPreMemConfig->Header.GuidHob.Name, CpuInitPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Config Lib PreMem configuration
  ********************************/
  CpuInitPreMemConfig->BistOnReset                 = CPU_FEATURE_DISABLE;
  CpuInitPreMemConfig->VmxEnable                   = CPU_FEATURE_ENABLE;
  CpuInitPreMemConfig->CrashLogEnable              = CPU_FEATURE_ENABLE;
  GetBusRatio (&MaxNonTurboLimRatio, NULL);
  CpuInitPreMemConfig->CpuRatio                    = MaxNonTurboLimRatio;
  CpuInitPreMemConfig->BootMaxFrequency            = CPU_FEATURE_ENABLE;
  CpuInitPreMemConfig->OcLock                      = CPU_FEATURE_ENABLE;
  CpuInitPreMemConfig->ReduceXecores               = CPU_FEATURE_ENABLE;

  ///
  /// DT/Halo FCLK = 1GHz
  /// Ulx/Ult FCLK = 800MHz
  ///
  if (CpuSku == EnumCpuTrad) {
    CpuInitPreMemConfig->FClkFrequency = 1;
  } else {
    CpuInitPreMemConfig->FClkFrequency = 0;
  }

  CpuInitPreMemConfig->DebugInterfaceEnable     = 2; // No Change, comply with HW value.
  CpuInitPreMemConfig->DebugInterfaceLockEnable = CPU_FEATURE_ENABLE;
  CpuInitPreMemConfig->TmeEnable                = PeiCpuTmeDefaultSetting ();
  CpuInitPreMemConfig->TmeBypassCapability      = CPU_FEATURE_DISABLE;
  CpuInitPreMemConfig->TsegSize                 = PcdGet32 (PcdTsegSize);

  CpuInitPreMemConfig->SmmRelocationEnable      = CPU_FEATURE_ENABLE;
}


static COMPONENT_BLOCK_ENTRY  mCpuIpBlocksPreMem [] = {
  {&gCpuInitPreMemConfigGuid,        sizeof (CPU_INIT_PREMEM_CONFIG),        CPU_INIT_PREMEM_CONFIG_REVISION,        LoadCpuInitPreMemConfigDefault}
};

/**
  Get CPU PREMEM config block table total size.

  @retval Size of CPU PREMEM config block table
**/
UINT16
EFIAPI
CpuInitPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mCpuIpBlocksPreMem[0], sizeof (mCpuIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  CpuAddPreMemConfigBlocks add all CPU PREMEM config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add CPU PREMEM config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuInitPreMemAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;
  DEBUG ((DEBUG_INFO, "CPU Pre-Mem Entry\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PRE_MEM_ENTRY); //PostCode (0xC00)

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCpuIpBlocksPreMem[0], sizeof (mCpuIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_ADD_PREMEM_CONFIGBLOCKS_DONE); //PostCode (0xC0F)

  return Status;
}
