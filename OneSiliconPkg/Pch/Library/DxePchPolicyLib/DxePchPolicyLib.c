/** @file
  This file provide services for DXE phase policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ScsConfig.h>
#include <Library/DxeGpioPolicyLib.h>
#include <Library/DxeHdaPolicyLib.h>
#include <Library/DxePchPcieRpPolicyLib.h>

/**
  Load DXE Config block default for eMMC

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadEmmcDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig;
  EmmcDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Name = %g\n", &EmmcDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EmmcDxeConfig->Header.GuidHob.Header.HobLength));

  EmmcDxeConfig->DriverStrength = DriverStrength40Ohm;
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchDxeIpBlocks [] = {
  {&gEmmcDxeConfigGuid,    sizeof (SCS_EMMC_DXE_CONFIG), SCS_EMMC_DXE_CONFIG_REVISION, LoadEmmcDxeConfigDefault}
};

/**
  Print SCS_EMMC_DXE_CONFIG.

  @param[in] EmmcDxeConfig         Pointer to a SCS_EMMC_DXE_CONFIG that provides the eMMC settings
**/
VOID
PchPrintEmmcDxeConfig (
  IN CONST SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH eMMC DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DriverStrength : %d\n", EmmcDxeConfig->DriverStrength));
  DEBUG ((DEBUG_INFO, " EnableSoftwareHs400Tuning: %d\n", EmmcDxeConfig->EnableSoftwareHs400Tuning));
  DEBUG ((DEBUG_INFO, " TuningLba : %X\n", EmmcDxeConfig->TuningLba));
  DEBUG ((DEBUG_INFO, " Previous tuning success : %d\n", EmmcDxeConfig->PreviousTuningResults.TuningSuccessful));
  if (EmmcDxeConfig->PreviousTuningResults.TuningSuccessful) {
    DEBUG ((DEBUG_INFO, "  Hs400 Rx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400RxValue));
    DEBUG ((DEBUG_INFO, "  Hs400 Tx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400TxValue));
  }
}

/**
  This function prints the PCH DXE phase policy.

  @param[in] PchPolicy - PCH DXE Policy protocol
**/
VOID
PchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL      *PchPolicy
  )
{
  DEBUG_CODE_BEGIN();
  EFI_STATUS                 Status;
  SCS_EMMC_DXE_CONFIG        *EmmcDxeConfig;
  VOID                       *Ptr;
  BOOLEAN                    AttachedPch = FALSE;

  Status = gBS->LocateProtocol (&gAttachedPchPolicyProtocolGuid, NULL, (VOID **) &Ptr);
  if (Status == EFI_SUCCESS) {
    AttachedPch = TRUE;
  }

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) PchPolicy, &gEmmcDxeConfigGuid, (VOID *)&EmmcDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %x\n", PchPolicy->TableHeader.Header.Revision));

  PchPrintEmmcDxeConfig (EmmcDxeConfig);
  GpioDxePrintConfig (PchPolicy);
  if (!AttachedPch) {
    HdaDxePrintConfig (PchPolicy);
  }
  PchPcieRpDxePrintConfig (PchPolicy);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print End --------------------------\n"));
  DEBUG_CODE_END();
}

/**
  This function prints the PCH DXE phase policy.

  @param[in] PchPolicy - PCH DXE Policy protocol
**/
VOID
AttachedPchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL      *PchPolicy
  )
{
  DEBUG_CODE_BEGIN();

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %x\n", PchPolicy->TableHeader.Header.Revision));

  GpioDxePrintConfig (PchPolicy);
  HdaDxePrintConfig (PchPolicy);
  PchPcieRpDxePrintConfig (PchPolicy);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print End --------------------------\n"));
  DEBUG_CODE_END();
}

/**
  CreatePchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] DxePchPolicy              The pointer to get PCH DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks (
  IN OUT  PCH_POLICY_PROTOCOL      **DxePchPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  PCH_POLICY_PROTOCOL *PchPolicyInit;
  UINT16              RequiredSize;
  VOID                *Ptr;
  BOOLEAN             AttachedPch = FALSE;

  Status = gBS->LocateProtocol (&gAttachedPchPolicyProtocolGuid, NULL, (VOID **) &Ptr);
  if (Status == EFI_SUCCESS) {
    AttachedPch = TRUE;
  }

  DEBUG ((DEBUG_INFO, "PCH Create Dxe Config Blocks\n"));

  PchPolicyInit = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize += GpioDxeGetConfigBlockTotalSize();
  if (!AttachedPch) {
    TotalBlockSize += HdaDxeGetConfigBlockTotalSize();
  }
  TotalBlockSize += PchPcieRpDxeGetConfigBlockTotalSize();

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  PchPolicyInit->TableHeader.Header.Revision = PCH_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) PchPolicyInit, &mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  Status =  GpioDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  if (!AttachedPch) {
    Status = HdaDxeAddConfigBlock ((VOID *) PchPolicyInit);
    ASSERT_EFI_ERROR (Status);
  }

  Status = PchPcieRpDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *DxePchPolicy = PchPolicyInit;
  return Status;
}

/**
  CreateAttachedPchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] DxePchPolicy              The pointer to get PCH DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateAttachedPchDxeConfigBlocks (
  IN OUT  PCH_POLICY_PROTOCOL      **DxePchPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  PCH_POLICY_PROTOCOL *PchPolicyInit;
  UINT16              RequiredSize;

  DEBUG ((DEBUG_INFO, "PCH Create Dxe Config Blocks\n"));

  PchPolicyInit = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize += HdaDxeGetConfigBlockTotalSize();
  TotalBlockSize += PchPcieRpDxeGetConfigBlockTotalSize();
  TotalBlockSize += GpioDxeGetConfigBlockTotalSize();

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  PchPolicyInit->TableHeader.Header.Revision = PCH_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status = AddComponentConfigBlocks ((VOID *) PchPolicyInit, &mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  Status = HdaDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  Status = PchPcieRpDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  Status = GpioDxeAddConfigBlock ((VOID *) PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *DxePchPolicy = PchPolicyInit;
  return Status;
}

/**
  PchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] PchPolicy                  The pointer to PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  )
{

  EFI_STATUS            Status;

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchPolicyProtocolGuid,
                  PchPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Print PCH DXE Policy
  ///
  PchPrintPolicyProtocol (PchPolicy);

  return Status;
}

/**
  PchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] PchPolicy                  The pointer to PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
AttachedPchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  )
{

  EFI_STATUS            Status;

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gAttachedPchPolicyProtocolGuid,
                  PchPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Print PCH DXE Policy
  ///
  AttachedPchPrintPolicyProtocol (PchPolicy);

  return Status;
}
