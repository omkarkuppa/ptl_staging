/** @file
  This file initializes PEI policies: MEMORY_CONFIG_NO_CRC and MEMORY_CONFIGURATION

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
#include <Library/CpuPlatformLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PeiMemPolicyLib.h>
#include "MrcInputBlock.h"
#include <CpuRegs.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/SiConfigBlockLib.h>

//
// Need minimum of 48MB during PEI phase for IAG and some buffer for boot.
//
#define  PEI_MIN_MEMORY_SIZE               (10 * 0x800000 + 0x10000000)    // 80MB + 256MB

static COMPONENT_BLOCK_ENTRY  mMrcIpBlocksPreMem[] = {
  { &gMemoryConfigGuid, sizeof (MEMORY_CONFIGURATION), MEMORY_CONFIGURATION_REVISION, LoadMemConfigDefault },
  { &gMemoryConfigNoCrcGuid, sizeof (MEMORY_CONFIG_NO_CRC), MEMORY_CONFIG_NO_CRC_REVISION, LoadMemConfigNoCrcDefault }
};

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemConfigNoCrcDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  MEMORY_CONFIG_NO_CRC                    *MemConfigNoCrc;

  MemConfigNoCrc = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "%aName = %g\n", "MemConfigNoCrc->Header.GuidHob.", &MemConfigNoCrc->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "%aHeader.HobLength = 0x%x\n", "MemConfigNoCrc->Header.GuidHob.", MemConfigNoCrc->Header.GuidHob.Header.HobLength));
  //
  // Allocating memory space for pointer structures inside MemConfigNoCrc
  //
  MemConfigNoCrc->SpdData = (SPD_DATA_BUFFER *) AllocateZeroPool (sizeof (SPD_DATA_BUFFER));
  ASSERT (MemConfigNoCrc->SpdData != NULL);
  if (MemConfigNoCrc->SpdData == NULL) {
    return;
  }

  MemConfigNoCrc->DqDqsMap = (MEMORY_DQDQS_MAPPING *) AllocateZeroPool (sizeof (MEMORY_DQDQS_MAPPING));
  ASSERT (MemConfigNoCrc->DqDqsMap != NULL);
  if (MemConfigNoCrc->DqDqsMap == NULL) {
    return;
  }

  MemConfigNoCrc->RcompData = (MEMORY_RCOMP *) AllocateZeroPool (sizeof (MEMORY_RCOMP));
  ASSERT (MemConfigNoCrc->RcompData != NULL);
  if (MemConfigNoCrc->RcompData == NULL) {
    return;
  }

  //
  // Set PlatformMemory Size
  //
  MemConfigNoCrc->PlatformMemorySize = PEI_MIN_MEMORY_SIZE;
  MemConfigNoCrc->SerialDebugLevel  = 3;  //< Enable MRC debug message

  MemConfigNoCrc->SafeLoadingBiosEnableState = 0;
  MemConfigNoCrc->PprRecoveryStatusEnable    = 0;
  MemConfigNoCrc->TsegMemoryTestStatus       = 1;
  MemConfigNoCrc->MrcPprStatus               = 1;
  MemConfigNoCrc->RetryCount                 = 0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadMemConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  MEMORY_CONFIGURATION  *MemConfig;
  UINT32                SimRecognitionReg;

  MemConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "%aName = %g\n", "MemConfig->Header.GuidHob.", &MemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "%aHeader.HobLength = 0x%x\n", "MemConfig->Header.GuidHob.", MemConfig->Header.GuidHob.Header.HobLength));
  // Initialize the Memory Configuration
  MemConfig->Header.Revision     = MEMORY_CONFIGURATION_REVISION;
  InitExternalInputsDefault (&MemConfig->ExternalInputs);

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  MemConfig->ExternalInputs.Force1Dpc = TRUE;
#endif

  // SAGV: 0 = Disabled, 1 = Enabled
  // SAGV WP Mask: 0x3 = Points0_1, 0x7 = Points0_1_2, 0xF = AllPoints0_1_2_3 (Only valid when SAGV is Enabled, otherwise ignored)
  if (IsSimicsEnvironment () || IsHfpgaEnvironment ()) {
    MemConfig->ExternalInputs.HsleFlag           = 0;
    MemConfig->ExternalInputs.SimicsFlag         = 1;
    SimRecognitionReg = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));
    if ((SimRecognitionReg & BIT16) != 0) {
      MemConfig->ExternalInputs.SimicsMinDbgMsg = 1;
    }
  } else {
    MemConfig->ExternalInputs.SimicsFlag         = 0;
    MemConfig->ExternalInputs.HsleFlag           = 0;
  }

  // Below default values are different from MiniBios.
  MemConfig->ExternalInputs.Lp5CccConfig         = 0; // BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].
  MemConfig->ExternalInputs.EccSupport           = 0;
  MemConfig->ExternalInputs.AllowOppRefBelowWriteThrehold = 1;
}

/**
  Get Mrc config block table total size.

  @retval      Size of Mrc config block table
**/
UINT16
EFIAPI
MrcGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize(&mMrcIpBlocksPreMem[0], sizeof(mMrcIpBlocksPreMem) / sizeof(COMPONENT_BLOCK_ENTRY));
}

/**
  MrcAddConfigBlocksPreMem add all Mrc PEI PreMem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add Mem config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MrcAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "MrcAddConfigBlocksPreMem: sizeof (MEMORY_CONFIGURATION): %u, sizeof (MEMORY_CONFIG_NO_CRC): %u\n", sizeof (MEMORY_CONFIGURATION), sizeof (MEMORY_CONFIG_NO_CRC)));
  Status = AddComponentConfigBlocks(ConfigBlockTableAddress, &mMrcIpBlocksPreMem[0], sizeof (mMrcIpBlocksPreMem) / sizeof (COMPONENT_BLOCK_ENTRY));

  return Status;
}

/**
  This function prints the PEI phase Mrc PreMem policy.

  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
MrcPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  DEBUG_CODE_BEGIN();
  EFI_STATUS                            Status;
  MEMORY_CONFIGURATION                  *MemConfig;
  MEMORY_CONFIG_NO_CRC                  *MemConfigNoCrc;
  UINT8                                 Index;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR(Status);

  DEBUG((DEBUG_INFO, "------------------------ MEMORY_CONFIGURATION -----------------\n"));
  DEBUG((DEBUG_INFO, " SpdProfileSelected: %d\n tCL: %d\n tRCDtRP: %d\n", MemConfig->ExternalInputs.MemoryProfile, MemConfig->ExternalInputs.tCL, MemConfig->ExternalInputs.tRCDtRP));
  DEBUG((DEBUG_INFO, " tRAS: %d\n tWR: %d\n tRFC: %d\n tRRD: %d\n", MemConfig->ExternalInputs.tRAS, MemConfig->ExternalInputs.tWR, MemConfig->ExternalInputs.tRFC, MemConfig->ExternalInputs.tRRD));
  DEBUG((DEBUG_INFO, " tWTR: %d\n tRTP: %d\n tFAW: %d\n tCWL: %d\n", MemConfig->ExternalInputs.tWTR, MemConfig->ExternalInputs.tRTP, MemConfig->ExternalInputs.tFAW, MemConfig->ExternalInputs.tCWL));
  DEBUG((DEBUG_INFO, " VddVoltage: %d\n VddqVoltage: %d\n VppVoltage: %d\n", MemConfig->ExternalInputs.VddVoltage, MemConfig->ExternalInputs.VddqVoltage, MemConfig->ExternalInputs.VppVoltage));

  DEBUG((DEBUG_INFO, "------------------------ MEMORY_CONFIG_NO_CRC -----------------\n"));
  DEBUG((DEBUG_INFO, " CleanMemory: %d\n MemTestOnWarmBoot: %d\n", MemConfigNoCrc->CleanMemory, MemConfigNoCrc->MemTestOnWarmBoot));
  DEBUG((DEBUG_INFO, " SerialDebugLevel: %d\n PlatformMemorySize: 0x%" PRIXMAX "\n", MemConfigNoCrc->SerialDebugLevel, MemConfigNoCrc->PlatformMemorySize));
  DEBUG((DEBUG_INFO, " SpdAddressTable[%d] :", MEM_CFG_MAX_SOCKETS));
  for (Index = 0; Index < MEM_CFG_MAX_SOCKETS; Index++) {
    DEBUG ((DEBUG_INFO, " 0x%x", MemConfigNoCrc->SpdAddressTable[Index]));
  }
  // DEBUG((DEBUG_INFO, "\n CkdAddressTable[%d] :", MEM_CFG_MAX_SOCKETS));
  // for (Index = 0; Index < MEM_CFG_MAX_SOCKETS; Index++) {
  //   DEBUG ((DEBUG_INFO, " 0x%x", MemConfigNoCrc->CkdAddressTable[Index]));
  // }
  DEBUG ((DEBUG_INFO, "\n MrcBdatTestType: %d\n", MemConfigNoCrc->MrcBdatTestType));
  DEBUG ((DEBUG_INFO, " MrcBdatEnable: %d\n", MemConfigNoCrc->MrcBdatEnable));

  DEBUG_CODE_END();
  return;
}
