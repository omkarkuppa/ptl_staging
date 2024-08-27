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
#include <Library/PcdLib.h>
#include <Library/PeiRtcPolicyLib.h>
#include <Library/PeiThermalPolicyLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/FspVPolicyLib.h>
#include <Library/PcdPeiIshPolicyLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSerialIoPolicyLib.h>
#include <Library/PeiLpssI3cPolicyLib.h>
#include <Library/PeiLpssUartPolicyLib.h>
#include <Library/PeiLpssSpiPolicyLib.h>
#include <Library/PeiLpssI2cPolicyLib.h>
#include <Library/PcdPolicyLib.h>

/**
  Get Si config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
SiGetConfigBlockTotalSize (
  VOID
  )
{
  return (UINT16) sizeof (SI_CONFIG);
}

EFI_STATUS
EFIAPI
LoadSiConfigBlockDefault (
  IN VOID *ConfigBlockPointer
  )
{
  SI_CONFIG                         *SiConfig;

  SiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SiConfig->Header.GuidHob.Name = %g\n", &SiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SiConfig->Header.GuidHob.Header.HobLength));

  SiConfig->Header.Revision = SI_CONFIG_REVISION;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SiAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  VOID                 *ConfigBlockPointer;
  EFI_STATUS           Status;
  CONFIG_BLOCK_HEADER  SiBlock;

  //
  // Initalize SiBlock
  //
  CopyMem (&(SiBlock.GuidHob.Name), &gSiConfigGuid, sizeof (EFI_GUID));
  SiBlock.GuidHob.Header.HobLength = sizeof (SI_CONFIG);
  SiBlock.Revision                 = SI_CONFIG_REVISION;
  //
  // Initialize ConfigBlockPointer
  //
  ConfigBlockPointer = (VOID *)&SiBlock;
  //
  // Add config block fro SiBlock
  //
  DEBUG ((DEBUG_INFO, "gSiConfigGuid = %g\n", &gSiConfigGuid));
  DEBUG ((DEBUG_INFO, "SiConfig->Header.GuidHob.Name = %g\n", &(SiBlock.GuidHob.Name)));
  Status = AddConfigBlock (ConfigBlockTableAddress, (VOID *) &ConfigBlockPointer);
  ASSERT_EFI_ERROR (Status);

  LoadSiConfigBlockDefault ((VOID *) ConfigBlockPointer);

  return Status;
}

/**
  SiCreateConfigBlocks creates the config blocksg of Silicon Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SiPolicyPpi         The pointer to get Silicon Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiCreateConfigBlocks (
  OUT  SI_POLICY_PPI **SiPolicyPpi
  )
{
  UINT16        TotalBlockSize;
  EFI_STATUS    Status;
  SI_POLICY_PPI *SiPolicy;
  UINT16        RequiredSize;

  SiPolicy = NULL;
  //
  // TotalBlockSize = Si, Pch, AMT, ME and CPU config block size.
  //
  TotalBlockSize  = SiGetConfigBlockTotalSize ();
  TotalBlockSize += PtlPcdGetConfigBlockTotalSize ();
  TotalBlockSize += AmtGetConfigBlockTotalSize ();
  TotalBlockSize += MeGetConfigBlockTotalSize ();

  //
  // Add size of IpBlock config blocks
  //
  TotalBlockSize += RtcGetConfigBlockTotalSize ();
  TotalBlockSize += ThermalGetConfigBlockTotalSize ();
  TotalBlockSize += CnviGetConfigBlockTotalSize ();
  TotalBlockSize += GbeGetConfigBlockTotalSize ();
  TotalBlockSize += HdaGetConfigBlockTotalSize ();
  TotalBlockSize += TsnGetConfigBlockTotalSize ();
  TotalBlockSize += PtlPcdIshGetConfigBlockTotalSize ();
  TotalBlockSize += ScsGetConfigBlockTotalSize ();
  TotalBlockSize += UsbGetConfigBlockTotalSize ();
  TotalBlockSize += ThcGetConfigBlockTotalSize ();
  TotalBlockSize += EspiGetConfigBlockTotalSize ();
  TotalBlockSize += IehGetConfigBlockTotalSize ();
  TotalBlockSize += GetInterruptConfigBlockTotalSize ();
  TotalBlockSize += GetIoApicConfigBlockTotalSize ();
  TotalBlockSize += P2sbGetConfigBlockTotalSize ();
  TotalBlockSize += PmcGetConfigBlockTotalSize ();
  TotalBlockSize += PtlPcdSerialIoGetConfigBlockTotalSize ();

#if FixedPcdGetBool (PcdTcssSupport) == 1
  TotalBlockSize += ITbtGetConfigBlockTotalSize ();
#endif

  TotalBlockSize += FusaGetConfigBlockTotalSize ();
  TotalBlockSize += IGpuGetConfigBlockTotalSize ();
  TotalBlockSize += TcssGetConfigBlockTotalSize ();
  TotalBlockSize += VmdGetPeiConfigBlockTotalSize ();
  TotalBlockSize += TelemetryGetPeiConfigBlockTotalSize ();
  TotalBlockSize += HostBridgeGetPeiConfigBlockTotalSize ();
  TotalBlockSize += NpuGetConfigBlockTotalSize ();
  TotalBlockSize += CpuInitGetConfigBlockTotalSize ();
  TotalBlockSize += CpuPowerManagementGetConfigBlockTotalSize ();
  TotalBlockSize += BiosGuardGetConfigBlockTotalSize ();
  TotalBlockSize += IaxGetConfigBlockTotalSize ();
  TotalBlockSize += LpssI3cGetConfigBlockTotalSize ();
  TotalBlockSize += LpssI2cGetConfigBlockTotalSize ();
  TotalBlockSize += LpssSpiGetConfigBlockTotalSize ();
  TotalBlockSize += LpssUartGetConfigBlockTotalSize ();
  TotalBlockSize += FspVPostMemGetConfigBlockTotalSize ();

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &SiPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  SiPolicy->TableHeader.Header.Revision = SI_POLICY_REVISION;
  //
  // Add config blocks.
  //
  Status = SiAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PtlPcdAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = AmtAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = MeAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // IpBlock config blocks
  //
  Status = RtcAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = ThermalAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = CnviAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = GbeAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = TsnAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = HdaAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PtlPcdIshAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = ScsAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = UsbAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = ThcAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = EspiAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = IehAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = InterruptAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = IoApicAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = P2sbAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PmcAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PtlPcdSerialIoAddConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
#if FixedPcdGetBool (PcdTcssSupport) == 1
  Status = ITbtAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
#endif
  Status = FusaAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = IGpuAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = TcssAddConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = VmdAddPeiConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = TelemetryAddPeiConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = HostBridgeAddPeiConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = NpuAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = CpuInitAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = BiosGuardInitAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = CpuPowerManagementAddConfigBlocks ((VOID*) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = FspVPostMemAddConfigBlock ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = IaxAddConfigBlocks ((VOID *) SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = LpssI3cAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = LpssUartAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = LpssSpiAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = LpssI2cAddConfigBlock (SiPolicy);
  ASSERT_EFI_ERROR (Status);

  if (SiPolicy->TableHeader.AvailableSize != 0) {
    DEBUG ((DEBUG_ERROR, "The size of allocated pool should be equal to config block size. Remain AvailableSize = 0x%x\n", SiPolicy->TableHeader.AvailableSize));
  }
  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *SiPolicyPpi = SiPolicy;
  return Status;
}

/**
  SiInstallPolicyPpi installs SiPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] SiPolicyPpi         The pointer to Silicon Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyPpi (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyPpiDesc;
  SI_CONFIG              *SiConfig;

  SiPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyPpiDesc->Guid  = &gSiPolicyPpiGuid;
  SiPolicyPpiDesc->Ppi   = SiPolicyPpi;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Install Silicon Policy PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  SiInstallPolicyReadyPpi installs SiPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPpiDesc;

  SiPolicyReadyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPpiDesc->Guid  = &gSiPolicyReadyPpiGuid;
  SiPolicyReadyPpiDesc->Ppi   = NULL;

  //
  // Install Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

