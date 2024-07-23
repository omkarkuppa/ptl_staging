/** @file
  This file load CpuInit Post memory defaults and config block.

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

@par Specification Reference:
**/
#include <CpuRegs.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SiConfigBlockLib.h>
#include <IntelRcStatusCode.h>
#include <Ppi/SiPolicy.h>

/**
  Print CPU_INIT_CONFIG and serial out.

  @param[in] CpuInitConfig   Pointer to a CPU_INIT_CONFIG
**/
VOID
CpuInitConfigPrint (
  IN CONST CPU_INIT_CONFIG   *CpuInitConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU_INIT_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " MicrocodePatchRegionSize : 0x%x\n", CpuInitConfig->MicrocodePatchRegionSize));
  DEBUG ((DEBUG_INFO, " MicrocodePatchAddress : 0x%x\n", CpuInitConfig->MicrocodePatchAddress));
  DEBUG ((DEBUG_INFO, " AesEnable : 0x%x\n", CpuInitConfig->AesEnable));
  DEBUG ((DEBUG_INFO, " TxtEnable : 0x%X\n", CpuInitConfig->TxtEnable));
  DEBUG ((DEBUG_INFO, " PpinSupport : 0x%x\n", CpuInitConfig->PpinSupport));
  DEBUG ((DEBUG_INFO, " AvxDisable : 0x%x\n", CpuInitConfig->AvxDisable));
  DEBUG ((DEBUG_INFO, " X2ApicEnable : 0x%x\n", CpuInitConfig->X2ApicEnable));

  DEBUG ((DEBUG_INFO, " PmonEnable : 0x%X\n", CpuInitConfig->PmonEnable));
  DEBUG ((DEBUG_INFO, " PmonAddress : 0x%X\n", CpuInitConfig->PmonAddress));
  DEBUG ((DEBUG_INFO, "------------------ CPU_INIT_CONFIG End ------------------\n"));
}

/**
  Print CPU_TEST_CONFIG and serial out.

  @param[in] CpuTestConfig   Pointer to a CPU_TEST_CONFIG
**/
VOID
CpuTestConfigPrint (
  IN CONST CPU_TEST_CONFIG   *CpuTestConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ CPU_TEST_CONFIG Begin ------------------\n"));
  DEBUG ((DEBUG_INFO, " MlcStreamerPrefetcher : 0x%X\n", CpuTestConfig->MlcStreamerPrefetcher));
  DEBUG ((DEBUG_INFO, " MlcSpatialPrefetcher : 0x%X\n", CpuTestConfig->MlcSpatialPrefetcher));
  DEBUG ((DEBUG_INFO, " MonitorMwaitEnable : 0x%X\n", CpuTestConfig->MonitorMwaitEnable));
  DEBUG ((DEBUG_INFO, " MachineCheckEnable : 0x%X\n", CpuTestConfig->MachineCheckEnable));
  DEBUG ((DEBUG_INFO, " ProcessorTraceOutputScheme : 0x%X\n", CpuTestConfig->ProcessorTraceOutputScheme));
  DEBUG ((DEBUG_INFO, " ProcessorTraceEnable : 0x%X\n", CpuTestConfig->ProcessorTraceEnable));
  DEBUG ((DEBUG_INFO, " ProcessorTraceBspOnly : 0x%X\n", CpuTestConfig->ProcessorTraceBspOnly));
  DEBUG ((DEBUG_INFO, " ProcessorTraceTimingPacket : 0x%X\n", CpuTestConfig->ProcessorTraceTimingPacket));
  DEBUG ((DEBUG_INFO, " ThreeStrikeCounter : 0x%X\n", CpuTestConfig->ThreeStrikeCounter));
  DEBUG ((DEBUG_INFO, " ProcessorTraceMemSize : 0x%X\n", CpuTestConfig->ProcessorTraceMemSize));
  DEBUG ((DEBUG_INFO, "------------------ CPU_TEST_CONFIG End ------------------\n"));
}

/**
  Print CPU Init Post Mem config blocks of SI_POLICY_PPI.

  @param[in] SiPolicyPpi The SI Policy PPI instance
**/
VOID
EFIAPI
CpuInitPrintPolicy (
  IN  SI_POLICY_PPI       *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                       Status;
  CPU_INIT_CONFIG                  *CpuInitConfig;
  CPU_TEST_CONFIG                  *CpuTestConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &CpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n ------------------------ CpuInit Print Begin in PostMem----------------- \n"));
  CpuInitConfigPrint (CpuInitConfig);
  CpuTestConfigPrint (CpuTestConfig);
  DEBUG ((DEBUG_INFO, "\n ------------------------ CpuInit Print End in PostMem ----------------- \n\n"));
DEBUG_CODE_END();
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuInitConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_INIT_CONFIG  *CpuInitConfig;
  CpuInitConfig  = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuInitConfig->Header.GuidHob.Name = %g\n", &CpuInitConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuInitConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuInitConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU configuration
  ********************************/
  CpuInitConfig->AesEnable             = CPU_FEATURE_ENABLE;
  CpuInitConfig->X2ApicEnable          = CPU_FEATURE_ENABLE;
  CpuInitConfig->PmonEnable            = CPU_FEATURE_DISABLE;
  CpuInitConfig->PmonAddress           = 0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadCpuTestConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CPU_TEST_CONFIG  *CpuTestConfig;
  CpuTestConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CpuTestConfig->Header.GuidHob.Name = %g\n", &CpuTestConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuTestConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuTestConfig->Header.GuidHob.Header.HobLength));

  /********************************
    CPU Test configuration
  ********************************/

  CpuTestConfig->MlcStreamerPrefetcher    = CPU_FEATURE_ENABLE;
  CpuTestConfig->MlcSpatialPrefetcher     = CPU_FEATURE_ENABLE;
  CpuTestConfig->MonitorMwaitEnable       = CPU_FEATURE_ENABLE;
  CpuTestConfig->MachineCheckEnable       = CPU_FEATURE_ENABLE;
  CpuTestConfig->ThreeStrikeCounter       = CPU_FEATURE_ENABLE;
  CpuTestConfig->ProcessorTraceMemSize    = 0xFF;
  CpuTestConfig->ProcessorTraceEnable     = 0;
  CpuTestConfig->ProcessorTraceBspOnly    = 0;
  CpuTestConfig->ProcessorTraceOutputScheme = 0;
  CpuTestConfig->ProcessorTraceTimingPacket = 0;
}

STATIC COMPONENT_BLOCK_ENTRY mCpuInitIpBlock [] = {
  {&gCpuInitConfigGuid,                 sizeof (CPU_INIT_CONFIG),                    CPU_INIT_CONFIG_REVISION,                   LoadCpuInitConfigDefault},
  {&gCpuTestConfigGuid,                 sizeof (CPU_TEST_CONFIG),                    CPU_TEST_CONFIG_REVISION,                   LoadCpuTestConfigDefault},
};

/**
  Get CPU Init block total size.

  @retval Size of config block
**/
UINT16
EFIAPI
CpuInitGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mCpuInitIpBlock[0], sizeof (mCpuInitIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add CPU Init ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CpuInitAddConfigBlocks (
  IN VOID *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "CPU Post-Mem Entry \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_POST_MEM_ENTRY); //PostCode (0xC00)

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mCpuInitIpBlock[0], sizeof (mCpuInitIpBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "Adding CpuPostMemConfigBlocks Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_ADD_POSTMEM_CONFIGBLOCKS_DONE); //PostCode (0xC0F)
  return Status;
}
