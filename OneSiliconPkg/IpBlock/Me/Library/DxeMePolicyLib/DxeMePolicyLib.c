/** @file
  This file is DxeMePolicyLib library.

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
#include "DxeMePolicyLibrary.h"

/**
  Load default settings for ME config block in DXE phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMeDxeDefault (
  IN VOID                           *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mMeIpBlocks [] = {
  {&gMeDxeConfigGuid,        sizeof (ME_DXE_CONFIG),            ME_DXE_CONFIG_REVISION,        LoadMeDxeDefault},
};

/**
  Get total size of IP list for the config block.

  @param[in] IpBlocks                   The pointer to the IP list
  @param[in] TotalIpSize                The size of IP list

  @retval                               Size of config block table
**/
STATIC
UINT16
EFIAPI
GetConfigBlockTotalSize (
  IN COMPONENT_BLOCK_ENTRY  IpBlocks [],
  IN UINTN                  TotalIpSize
  )
{
  UINT16            TotalBlockCount;
  UINT16            TotalBlockSize;
  UINT16            ConfigBlockHdrSize;
  UINT16            BlockCount;

  TotalBlockCount = (UINT16) (TotalIpSize / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize = 0;
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    TotalBlockSize += (UINT32) IpBlocks[BlockCount].Size;
    DEBUG ((DEBUG_INFO, "TotalBlockSize after adding Block[0x%x]= 0x%x\n", BlockCount, TotalBlockSize));
  }
  ConfigBlockHdrSize = sizeof (CONFIG_BLOCK_TABLE_HEADER);

  return ConfigBlockHdrSize + TotalBlockSize;
}

/**
  CreateConfigBlocksByIpBlocks generates the config blocks for IP list.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in] IpBlocks                     The pointer to the IP list
  @param[in] TotalIpSize                  The size of IP list
  @param[in, out] ConfigBlockTableHeader  The pointer to the config block

  @retval EFI_SUCCESS                     The config block for IP list is created and initialized.
  @retval EFI_OUT_OF_RESOURCES            Insufficient resources to create buffer
**/
STATIC
EFI_STATUS
EFIAPI
CreateConfigBlocksByIpBlocks (
  IN COMPONENT_BLOCK_ENTRY          IpBlocks [],
  IN UINTN                          TotalIpSize,
  IN OUT CONFIG_BLOCK_TABLE_HEADER  **ConfigBlockTableHeader
  )
{
  UINT16            TotalBlockCount;
  UINT16            BlockCount;
  VOID              *ConfigBlockPointer;
  CONFIG_BLOCK      ConfigBlockBuf;
  EFI_STATUS        Status;
  UINT16            RequiredSize;
  CONFIG_BLOCK_TABLE_HEADER      *TableHeader;

  DEBUG ((DEBUG_INFO, "CreateConfigBlocksByIpBlocks\n"));

  TotalBlockCount = (UINT16) (TotalIpSize / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "TotalBlockCount = 0x%x\n", TotalBlockCount));

  RequiredSize = GetConfigBlockTotalSize (IpBlocks, TotalIpSize);

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&TableHeader);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    //
    // Initialize ConfigBlockPointer to NULL
    //
    ConfigBlockPointer = NULL;
    //
    // Loop to identify each config block from IpBlocks[] Table and add each of them
    //
    for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
      CopyMem (&(ConfigBlockBuf.Header.GuidHob.Name), IpBlocks[BlockCount].Guid, sizeof (EFI_GUID));
      ConfigBlockBuf.Header.GuidHob.Header.HobLength = IpBlocks[BlockCount].Size;
      ConfigBlockBuf.Header.Revision        = IpBlocks[BlockCount].Revision;
      ConfigBlockPointer = (VOID *)&ConfigBlockBuf;
      Status = AddConfigBlock ((VOID *) TableHeader, (VOID *)&ConfigBlockPointer);
      ASSERT_EFI_ERROR (Status);
      if (IpBlocks[BlockCount].LoadDefault != NULL) {
        IpBlocks[BlockCount].LoadDefault (ConfigBlockPointer);
      }
    }
    //
    // Assignment for returning config block base address
    //
    *ConfigBlockTableHeader = TableHeader;
    return EFI_SUCCESS;
  }

  return Status;
}

/**
  Load default settings for ME config block in DXE phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMeDxeDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_DXE_CONFIG         *MeDxeConfig;

  MeDxeConfig = ConfigBlockPointer;

  MeDxeConfig->CoreBiosDoneEnabled        = 1;
}

/**
  Dump values of ME config block in DXE phase.

  @param[in] MeDxeConfig                        The pointer to the config block
**/
VOID
EFIAPI
PrintMeDxeConfig (
  IN ME_DXE_CONFIG                  *MeDxeConfig
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "------------------------ ME_DXE_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision             : 0x%x\n", MeDxeConfig->Header.Revision));
  ASSERT (MeDxeConfig->Header.Revision == ME_DXE_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " MeFwDowngrade        : 0x%x\n", MeDxeConfig->MeFwDowngrade));
  DEBUG ((DEBUG_INFO, " CoreBiosDoneEnabled  : 0x%x\n", MeDxeConfig->CoreBiosDoneEnabled));
  DEBUG_CODE_END ();
}

/**
  This function prints the ME DXE phase policy.

  @param[in] DxeMePolicy - ME DXE Policy protocol
**/
VOID
EFIAPI
MePrintPolicyProtocol (
  IN  ME_POLICY_PROTOCOL            *DxeMePolicy
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_DXE_CONFIG                     *MeDxeConfig;

  Status = GetConfigBlock ((VOID *) DxeMePolicy, &gMeDxeConfigGuid, (VOID *) &MeDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- DxeMePolicy Print Begin -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : 0x%x\n", DxeMePolicy->TableHeader.Header.Revision));
  ASSERT (DxeMePolicy->TableHeader.Header.Revision == ME_POLICY_PROTOCOL_REVISION);

  PrintMeDxeConfig (MeDxeConfig);

  DEBUG ((DEBUG_INFO, "\n---------------------- DxeMePolicy Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  CreateConfigBlocks generates the config blocksg of ME Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeMePolicy           The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeDxeCreateConfigBlocks (
  IN OUT  ME_POLICY_PROTOCOL            **DxeMePolicy
  )
{
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "MeDxeCreateConfigBlocks\n"));
  Status = EFI_NOT_FOUND;

  Status = CreateConfigBlocksByIpBlocks (mMeIpBlocks, sizeof (mMeIpBlocks), (CONFIG_BLOCK_TABLE_HEADER **) DxeMePolicy);
  if (!EFI_ERROR (Status)) {
    //
    // Initialize Policy Revision
    //
    (*DxeMePolicy)->TableHeader.Header.Revision = ME_POLICY_PROTOCOL_REVISION;
  }

  DEBUG ((DEBUG_INFO, "MeDxeCreateConfigBlocks done, Status: %r\n", Status));
  return Status;
}

/**
  MeCreatePolicyDefaults creates the default setting of ME Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeMePolicy           The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
MeCreatePolicyDefaults (
  IN OUT  ME_POLICY_PROTOCOL            **DxeMePolicy
  )
{
  return MeDxeCreateConfigBlocks (DxeMePolicy);
}

/**
  MeInstallPolicyProtocol installs ME Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeMePolicy                The pointer to ME Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
MeInstallPolicyProtocol (
  IN  EFI_HANDLE                    ImageHandle,
  IN  ME_POLICY_PROTOCOL            *DxeMePolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Install protocol to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeMePolicyGuid,
                  DxeMePolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Print ME DXE Policy
  ///
  MePrintPolicyProtocol (DxeMePolicy);

  return Status;
}

