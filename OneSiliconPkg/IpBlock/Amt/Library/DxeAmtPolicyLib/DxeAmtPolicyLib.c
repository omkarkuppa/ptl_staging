/** @file
  This file is DxeAmtPolicyLib library.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/SiConfigBlockLib.h>
#include <AmtConfig.h>
#include <Protocol/AmtPolicy.h>
#include <MeBiosPayloadHob.h>

extern EFI_GUID gAmtPolicyHobGuid;

VOID
LoadAmtDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

STATIC GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY   mAmtDxeIpBlocks [] = {
  {&gAmtDxeConfigGuid,       sizeof (AMT_DXE_CONFIG),           AMT_DXE_CONFIG_REVISION,       LoadAmtDxeConfigDefault},
};

/**
  Load DXE Config block default for AMT

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadAmtDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  AMT_DXE_CONFIG                  *AmtDxeConfig;

  AmtDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "AmtDxeConfig->Header.GuidHob.Name = %g\n", &AmtDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "AmtDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", AmtDxeConfig->Header.GuidHob.Header.HobLength));
}


/**
  Dump values of AMT config block in DXE phase.

  @param[in] AmtDxeConfig                       The pointer to the config block
**/
VOID
EFIAPI
PrintAmtDxeConfig (
  IN  AMT_DXE_CONFIG                *AmtDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------------ AMT_DXE_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                 : 0x%x\n", AmtDxeConfig->Header.Revision));
  ASSERT (AmtDxeConfig->Header.Revision == AMT_DXE_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " CiraRequest              : 0x%x\n", AmtDxeConfig->CiraRequest));
  DEBUG ((DEBUG_INFO, " UnConfigureMe            : 0x%x\n", AmtDxeConfig->UnConfigureMe));
  DEBUG ((DEBUG_INFO, " UsbProvision             : 0x%x\n", AmtDxeConfig->UsbProvision));
}

/**
  This function prints the AMT DXE phase policy.

  @param[in] DxeAmtPolicy - AMT DXE Policy protocol
**/
VOID
AmtPrintPolicyProtocol (
  IN  AMT_POLICY_PROTOCOL     *DxeAmtPolicy
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  AMT_DXE_CONFIG                    *AmtDxeConfig;

  Status = GetConfigBlock ((VOID *) DxeAmtPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n------------------------ AmtPolicy Print Begin -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : 0x%x\n", DxeAmtPolicy->TableHeader.Header.Revision));
  ASSERT (DxeAmtPolicy->TableHeader.Header.Revision == AMT_POLICY_PROTOCOL_REVISION);

  PrintAmtDxeConfig (AmtDxeConfig);

  DEBUG ((DEBUG_INFO, "\n------------------------ AmtPolicy Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  CreateConfigBlocks generates the config blocksg of AMT Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeAmtPolicy          The pointer to get AMT Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AmtCreatePolicyDefaults (
  IN OUT  AMT_POLICY_PROTOCOL       **DxeAmtPolicy
  )
{
  ME_BIOS_PAYLOAD_HOB    *MbpHob;
  UINT16                 TotalBlockSize;
  EFI_STATUS             Status;
  AMT_POLICY_PROTOCOL    *AmtPolicyInit;
  UINT16                 RequiredSize;
  EFI_HOB_GUID_TYPE      *GuidHob;
  AMT_PEI_CONFIG         *AmtPeiConfig;

  DEBUG ((DEBUG_INFO, "AMT Create Dxe Config Blocks\n"));
  AmtPolicyInit  = NULL;
  MbpHob         = NULL;
  TotalBlockSize = 0;
  RequiredSize   = 0;
  Status         = EFI_UNSUPPORTED;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw) {
    DEBUG ((DEBUG_INFO, "Current ME FW is not Corporate SKU, Amt Policy protocol is not installed then.\n"));
    return EFI_UNSUPPORTED;
  }

  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (GuidHob);
  if (AmtPeiConfig->AmtEnabled == 0) {
    DEBUG ((DEBUG_INFO, "AMT feature is disabled from setup, Amt Policy protocol is not installed.\n"));
    return EFI_UNSUPPORTED;
  }

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mAmtDxeIpBlocks[0], sizeof (mAmtDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));
  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &AmtPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  AmtPolicyInit->TableHeader.Header.Revision = AMT_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) AmtPolicyInit, &mAmtDxeIpBlocks[0], sizeof (mAmtDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *DxeAmtPolicy = AmtPolicyInit;
  return Status;
}

/**
  AmtInstallPolicyProtocol installs Amt Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeAmtPolicy               The pointer to Amt Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
AmtInstallPolicyProtocol (
  IN  EFI_HANDLE                        ImageHandle,
  IN  AMT_POLICY_PROTOCOL               *DxeAmtPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Install protocol to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeAmtPolicyGuid,
                  DxeAmtPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Print Amt DXE Policy
  ///
  AmtPrintPolicyProtocol (DxeAmtPolicy);

  return Status;
}

