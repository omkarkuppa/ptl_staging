/** @file
  This file provide services for DXE phase policy default initialization

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


#include <Library/DebugLib.h>
#include <Protocol/IGpuPolicy.h>
#include <Library/IGpuDxePolicyLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  CreateIGpuDxeConfigBlocks generates the config blocksg of SA DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] IGpuPolicy               The pointer to get SA  DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateIGpuDxeConfigBlocks (
  IN OUT  IGPU_POLICY_PROTOCOL      **IGpuPolicy
  )
{
  UINT16                TotalBlockSize;
  EFI_STATUS            Status;
  IGPU_POLICY_PROTOCOL  *IGpuInitPolicy;
  UINT16                RequiredSize;

  DEBUG ((DEBUG_INFO, "IGPU Create Dxe Config Blocks\n"));

  IGpuInitPolicy = NULL;

  TotalBlockSize = IGpuGetDxeConfigBlockTotalSize ();
  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &IGpuInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Policy Revision
  //
  IGpuInitPolicy->TableHeader.Header.Revision = IGPU_POLICY_PROTOCOL_REVISION;

  //
  // Add config blocks.
  //

  // IGPU
  Status = IGpuAddDxeConfigBlocks ((VOID *) IGpuInitPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning IGpuInitPolicy config block base address
  //
  *IGpuPolicy = IGpuInitPolicy;
  return Status;
}


/**
  IGpuInstallPolicyProtocol installs IGPU Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] IGpuPolicy                 The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
IGpuInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  IGPU_POLICY_PROTOCOL        *IGpuPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gIGpuPolicyProtocolGuid,
                  IGpuPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Print SA DXE Policy
  ///
  IGpuDxePolicyPrint (IGpuPolicy);

  return Status;
}

