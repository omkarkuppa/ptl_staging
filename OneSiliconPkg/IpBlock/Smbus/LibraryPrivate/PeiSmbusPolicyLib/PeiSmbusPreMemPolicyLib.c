/** @file
  This file provides services for Smbus policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <SmbusConfig.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mSmbusRsvdAddresses[] = {
  0xA0,
  0xA2,
  0xA4,
  0xA6
};

/**
  Print PCH_SMBUS_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI

**/
VOID
SmbusPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                Status;
  PCH_SMBUS_PREMEM_CONFIG   *SmbusPreMemConfig;
  UINT32                    Index;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ SMBUS PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable= %x\n", SmbusPreMemConfig->Enable));
  DEBUG ((DEBUG_INFO, " ArpEnable= %x\n", SmbusPreMemConfig->ArpEnable));
  DEBUG ((DEBUG_INFO, " DynamicPowerGating= %x\n", SmbusPreMemConfig->DynamicPowerGating));
  DEBUG ((DEBUG_INFO, " SpdWriteDisable= %x\n", SmbusPreMemConfig->SpdWriteDisable));
  DEBUG ((DEBUG_INFO, " SmbAlertEnable= %x\n", SmbusPreMemConfig->SmbAlertEnable));
  DEBUG ((DEBUG_INFO, " SmbusIoBase= %x\n", SmbusPreMemConfig->SmbusIoBase));
  DEBUG ((DEBUG_INFO, " NumRsvdSmbusAddresses= %x\n", SmbusPreMemConfig->NumRsvdSmbusAddresses));
  DEBUG ((DEBUG_INFO, " RsvdSmbusAddressTable= {"));
  for (Index = 0; Index < SmbusPreMemConfig->NumRsvdSmbusAddresses; ++Index) {
    DEBUG ((DEBUG_INFO, " %02xh", SmbusPreMemConfig->RsvdSmbusAddressTable[Index]));
  }
  DEBUG ((DEBUG_INFO, " }\n"));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
SmbusPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_SMBUS_PREMEM_CONFIG  *SmbusPreMemConfig;
  SmbusPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SmbusPreMemConfig->Header.GuidHob.Name = %g\n", &SmbusPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SmbusPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SmbusPreMemConfig->Header.GuidHob.Header.HobLength));

  /********************************
    SMBus configuration
  ********************************/
  SmbusPreMemConfig->Enable                = TRUE;
  SmbusPreMemConfig->DynamicPowerGating    = TRUE;
  SmbusPreMemConfig->SpdWriteDisable       = TRUE;
  SmbusPreMemConfig->SmbusIoBase           = PcdGet16 (PcdSmbusBaseAddress);
  ASSERT (sizeof (mSmbusRsvdAddresses) <= PCH_MAX_SMBUS_RESERVED_ADDRESS);
  SmbusPreMemConfig->NumRsvdSmbusAddresses = sizeof (mSmbusRsvdAddresses);
  CopyMem (
    SmbusPreMemConfig->RsvdSmbusAddressTable,
    mSmbusRsvdAddresses,
    sizeof (mSmbusRsvdAddresses)
    );
}

STATIC COMPONENT_BLOCK_ENTRY  mSmbusPreMemBlocks = {
  &gSmbusPreMemConfigGuid,
  sizeof (PCH_SMBUS_PREMEM_CONFIG),
  PCH_SMBUS_PREMEM_CONFIG_REVISION,
  SmbusPreMemLoadConfigDefault
};

/**
  Get Smbus config block table size.

  @retval      Size of config block
**/
UINT16
SmbusPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return mSmbusPreMemBlocks.Size;
}

/**
  Add Smbus ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
SmbusPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSmbusPreMemBlocks, 1);
}
