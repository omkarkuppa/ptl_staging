/** @file
  This file is PeiSiPolicyLib library creates default settings of RC
  Policy and installs RC Policy PPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include "PeiSiPolicyLibrary.h"
#include <Base.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/BootGuardPolicyLib.h>
#include <Library/TxtPolicyLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/FspVPolicyLib.h>
#include <Library/MemorySubSystemPolicyLib.h>

/**
  Get Si PreMem config block table total size.

  @retval                               Size of SI PreMem config block table
**/
UINT16
SiGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return (UINT16) sizeof (SI_PREMEM_CONFIG);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
EFI_STATUS
LoadSiPreMemConfigBlockDefault (
  IN VOID              *ConfigBlockPointer
  )
{
  SI_PREMEM_CONFIG     *SiPreMemConfig;

  SiPreMemConfig     = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SiPreMemConfig->Header.GuidHob.Name = %g\n", &SiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SiPreMemConfig->Header.GuidHob.Header.HobLength));

  return EFI_SUCCESS;
}

/**
  Add the Si PreMem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
SiAddConfigBlocksPreMem (
  IN     VOID          *ConfigBlockTableAddress
  )
{
  VOID                 *ConfigBlockPointer;
  EFI_STATUS           Status;
  CONFIG_BLOCK_HEADER  SiPreMemBlock;

  //
  // Initalize SiPreMemBlock
  //
  CopyMem (&(SiPreMemBlock.GuidHob.Name), &gSiPreMemConfigGuid, sizeof (EFI_GUID));
  SiPreMemBlock.GuidHob.Header.HobLength = sizeof (SI_PREMEM_CONFIG);
  SiPreMemBlock.Revision                 = SI_PREMEM_CONFIG_REVISION;
  //
  // Initialize ConfigBlockPointer
  //
  ConfigBlockPointer = (VOID *)&SiPreMemBlock;
  //
  // Add SiPreMemBlock
  //
  DEBUG ((DEBUG_INFO, "gSiPreMemConfigGuid = %g\n", &gSiPreMemConfigGuid));
  DEBUG ((DEBUG_INFO, "SiPreMemBlock Name = %g\n", &(SiPreMemBlock.GuidHob.Name)));
  Status = AddConfigBlock (ConfigBlockTableAddress, (VOID *) &ConfigBlockPointer);
  ASSERT_EFI_ERROR (Status);

  LoadSiPreMemConfigBlockDefault ((VOID *) ConfigBlockPointer);

  return Status;
}

/**
  SiCreatePreMemConfigBlocks creates the config blocks of Silicon PREMEM Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SiPreMemPolicyPpi   The pointer to get Silicon Policy PreMem PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiCreatePreMemConfigBlocks (
  OUT  SI_PREMEM_POLICY_PPI **SiPreMemPolicyPpi
  )
{
  UINT16               TotalBlockSize;
  EFI_STATUS           Status;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicy;
  UINT16               RequiredSize;

  SiPreMemPolicy = NULL;
  //
  // TotalBlockSize = SI, Pch , SA, ME, ME Server, CPU + individual IPs' config block size
  //
  TotalBlockSize = SiGetConfigBlockTotalSizePreMem ();
  TotalBlockSize += PtlPcdGetPreMemConfigBlockTotalSize ();
  TotalBlockSize += MeGetConfigBlockTotalSizePreMem () +
                   MrcGetConfigBlockTotalSizePreMem ();
  // IPs'
  TotalBlockSize += HdaPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += CnviPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += PtlPcdIshPreMemGetConfigBlockTotalSize ();

  TotalBlockSize += TraceHubPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += SmbusPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += WdtPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += SpiGetConfigBlockTotalSize ();
  TotalBlockSize += TxtGetConfigBlockTotalSizePreMem ();
  TotalBlockSize += IpuGetConfigBlockTotalSizePreMem ();
  TotalBlockSize += DciPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += IGpuGetConfigBlockTotalSizePreMem ();
  TotalBlockSize += TcssGetConfigBlockTotalSizePreMem ();
  TotalBlockSize += VtdGetConfigBlockTotalSize ();
  TotalBlockSize += TelemetryGetPeiConfigBlockTotalSizePreMem ();
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  TotalBlockSize += OcPreMemGetConfigBlockTotalSize ();
#endif
  TotalBlockSize += HybridGraphicsGetConfigBlockTotalSize ();
  TotalBlockSize += HostBridgeGetPeiConfigBlockTotalSizePreMem ();
  TotalBlockSize += CpuInitPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += BootGuardGetConfigBlockTotalSize ();
  TotalBlockSize += GetCpuPowerMgmtVrConfigBlockTotalSize ();
  TotalBlockSize += CpuPowerDeliveryConfigBlockTotalSize ();
  TotalBlockSize += MemorySubSystemGetConfigBlockTotalSize ();

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  TotalBlockSize += FspVPreMemGetConfigBlockTotalSize ();

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  SiPreMemPolicy->TableHeader.Header.Revision = SI_PREMEM_POLICY_REVISION;
  //
  // Add config blocks.
  //
  Status = SiAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PtlPcdAddPreMemConfigBlocks ((VOID *) SiPreMemPolicy);
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  // OVERCLOCKING
  Status = OcPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
#endif
  ASSERT_EFI_ERROR (Status);
  Status = MeAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = MrcAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // IpBlock config blocks

  // HDA
  Status = HdaPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // ISH
  Status = PtlPcdIshPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // CNVi
  Status = CnviPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // Trace Hub
  Status = TraceHubPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // SMBUS
  Status = SmbusPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // WDT
  Status = WdtPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // SPI
  Status = SpiAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // Txt
  Status = TxtAddConfigBlockPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // IPU
  Status = IpuAddConfigBlockPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // DCI
  Status = DciPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // Gfx
  Status = IGpuAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // TCSS
  Status = TcssAddConfigBlockPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // VT-D
  Status = VtdAddConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // HostBridge
  Status = HostBridgeAddPeiConfigBlockPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // Telemetry
  Status = TelemetryAddPeiConfigBlockPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // Hybrid Graphics
  Status = HybridGraphicsAddConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // CpuInit
  Status = CpuInitPreMemAddConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // Voltage Regulator
  Status = AddCpuPowerMgmtVrConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // CpuPowerDelivery
  Status = AddCpuPowerDeliveryConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // BootGuard
  Status = BootGuardAddConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // FspV
  Status = FspVPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  // Memory SubSystem
  Status = MemorySubSystemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *SiPreMemPolicyPpi = SiPreMemPolicy;
  return Status;
}

/**
  Print SI PreMem Config block

  @param[in] SiPreMemPolicyPpi          The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
SiPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  SI_PREMEM_CONFIG                  *SiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a Start\n"));
  DEBUG ((DEBUG_INFO, "PlatformDebugOption = %x\n", SiPreMemConfig->PlatformDebugOption));
  DEBUG ((DEBUG_INFO, "SkipOverrideBootModeWhenFwUpdate = %x\n", SiPreMemConfig->SkipOverrideBootModeWhenFwUpdate));
  DEBUG ((DEBUG_INFO, "StaticContentSizeAt4Gb = %x\n", SiPreMemConfig->StaticContentSizeAt4Gb));
  DEBUG ((DEBUG_INFO, "%a End\n"));
  DEBUG_CODE_END ();
}

/**
  SiPreMemInstallPolicyPpi installs SiPreMemPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] SiPreMemPolicyPpi   The pointer to Silicon Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyPreMemPpiDesc;

  SiPolicyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyPreMemPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyPreMemPpiDesc->Guid  = &gSiPreMemPolicyPpiGuid;
  SiPolicyPreMemPpiDesc->Ppi   = SiPolicyPreMemPpi;

  //
  // Install Silicon Policy PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyPreMemPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  SiPreMemInstallPolicyReadyPpi installs SiPreMemPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPreMemPpiDesc;

  SiPolicyReadyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPreMemPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPreMemPpiDesc->Guid  = &gSiPreMemPolicyReadyPpiGuid;
  SiPolicyReadyPreMemPpiDesc->Ppi   = NULL;

  //
  // Install PreMem Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPreMemPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
