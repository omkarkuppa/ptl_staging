/** @file
  This file provide services for IGpu DXE policy default initialization.

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

@par Specification Reference:
**/

#include <Library/IGpuDxePolicyLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>

/**
  This function prints the IGPU DXE phase policy.

  @param[in] IGpuPolicy - IGPU DXE Policy protocol
**/
VOID
IGpuDxePolicyPrint (
  IN  IGPU_POLICY_PROTOCOL  *IGpuPolicy
  )
{
  EFI_STATUS       Status;
  IGPU_DXE_CONFIG  *IGpuDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "IGPU Policy (DXE) print BEGIN\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", IGpuDxeConfig->Header.Revision));
  ASSERT (IGpuDxeConfig->Header.Revision == IGPU_DXE_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, "IGPU Policy (DXE) print END\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  This function Load default IGPU DXE policy.

  @param[in] ConfigBlockPointer    The pointer to add IGPU config block
**/
VOID
IGpuLoadDxeDefault (
  IN VOID  *ConfigBlockPointer
  )
{
  IGPU_DXE_CONFIG  *IGpuDxeConfig;
  UINT8            Index;

  IGpuDxeConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "IGpuDxeConfig->Header.GuidHob.Name = %g\n", &IGpuDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IGpuDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IGpuDxeConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Initialize the IGPU configuration
  ///
  IGpuDxeConfig->PlatformConfig = 1;
  IGpuDxeConfig->AlsEnable      = 2;
  ///
  /// <EXAMPLE> Create a static Backlight Brightness Level Duty cycle Mapping Table
  /// Possible 30 entries (example used 11), each 32 bits as follows:
  /// [31] = Field Valid bit, [30:23] = Reserved,  [22:16] = Level in Percentage (0-64h), [15:00] = Desired duty cycle (0 - FFFFh).
  ///
  IGpuDxeConfig->BCM1[0]  = (0x000000 + DWORD_FIELD_VALID_BIT); ///< 0%
  IGpuDxeConfig->BCM1[1]  = (0x0A0019 + DWORD_FIELD_VALID_BIT); ///< 10%
  IGpuDxeConfig->BCM1[2]  = (0x140033 + DWORD_FIELD_VALID_BIT); ///< 20%
  IGpuDxeConfig->BCM1[3]  = (0x1E004C + DWORD_FIELD_VALID_BIT); ///< 30%
  IGpuDxeConfig->BCM1[4]  = (0x280066 + DWORD_FIELD_VALID_BIT); ///< 40%
  IGpuDxeConfig->BCM1[5]  = (0x32007F + DWORD_FIELD_VALID_BIT); ///< 50%
  IGpuDxeConfig->BCM1[6]  = (0x3C0099 + DWORD_FIELD_VALID_BIT); ///< 60%
  IGpuDxeConfig->BCM1[7]  = (0x4600B2 + DWORD_FIELD_VALID_BIT); ///< 70%
  IGpuDxeConfig->BCM1[8]  = (0x5000CC + DWORD_FIELD_VALID_BIT); ///< 80%
  IGpuDxeConfig->BCM1[9]  = (0x5A00E5 + DWORD_FIELD_VALID_BIT); ///< 90%
  IGpuDxeConfig->BCM1[10] = (0x6400FF + DWORD_FIELD_VALID_BIT); ///< 100%
  for (Index = 0; Index < MAX_BCLM_ENTRIES; Index++) {
    IGpuDxeConfig->BCM2[Index] = IGpuDxeConfig->BCM1[Index];
  }
}

static COMPONENT_BLOCK_ENTRY  mIGpuDxeIpBlocks = {
  &gIGpuDxeConfigGuid, sizeof (IGPU_DXE_CONFIG), IGPU_DXE_CONFIG_REVISION, IGpuLoadDxeDefault
};

/**
  Get DXE IGPU config block table total size.

  @retval     Size of DXE IGPU config block table
**/
UINT16
EFIAPI
IGpuGetDxeConfigBlockTotalSize (
  VOID
  )
{
  return mIGpuDxeIpBlocks.Size;
}

/**
  IGpuAddDxeConfigBlocks add all DXE IGPU config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IGpuAddDxeConfigBlocks (
  IN VOID  *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mIGpuDxeIpBlocks, 1);
  return Status;
}
