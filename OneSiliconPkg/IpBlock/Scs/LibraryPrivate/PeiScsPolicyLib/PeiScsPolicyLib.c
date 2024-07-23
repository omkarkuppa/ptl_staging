/** @file
  This file provides services for Scs policy function

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
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <ScsConfig.h>

/**
  Print SCS_SDCARD_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
SdCardPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  SCS_SDCARD_CONFIG  *SdCardConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSdCardConfigGuid, (VOID *) &SdCardConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ SCS SdCard Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable = %d\n", SdCardConfig->Enable));
  DEBUG ((DEBUG_INFO, " PowerEnableActiveHigh = %d\n", SdCardConfig->PowerEnableActiveHigh));
  DEBUG ((DEBUG_INFO, " Use tuned DLLs = %d\n", SdCardConfig->UseCustomDlls));
  if (SdCardConfig->UseCustomDlls) {
    DEBUG ((DEBUG_INFO, "  TxCmdDelayControl value = %X\n", SdCardConfig->CustomDllValues.TxCmdDelayControl));
    DEBUG ((DEBUG_INFO, "  TxDataDelayControl1 value = %X\n", SdCardConfig->CustomDllValues.TxDataDelayControl1));
    DEBUG ((DEBUG_INFO, "  TxDataDelayControl2 value = %X\n", SdCardConfig->CustomDllValues.TxDataDelayControl2));
    DEBUG ((DEBUG_INFO, "  RxCmdDataDelayControl1 value = %X\n", SdCardConfig->CustomDllValues.RxCmdDataDelayControl1));
    DEBUG ((DEBUG_INFO, "  RxCmdDataDelayControl2 value = %X\n", SdCardConfig->CustomDllValues.RxCmdDataDelayControl2));
  }
}

/**
  Print SCS_EMMC_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
EmmcPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  SCS_EMMC_CONFIG  *EmmcConfig;
  EFI_STATUS       Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gEmmcConfigGuid, (VOID *) &EmmcConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ SCS eMMC Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Enable = %d\n", EmmcConfig->Enable));
  DEBUG ((DEBUG_INFO, " HS400 Supported = %d\n", EmmcConfig->Hs400Supported));
  DEBUG ((DEBUG_INFO, " Use tuned DLLs = %d\n", EmmcConfig->UseCustomDlls));
  if (EmmcConfig->UseCustomDlls) {
    DEBUG ((DEBUG_INFO, "  TxCmdDelayControl value = %X\n", EmmcConfig->CustomDllValues.TxCmdDelayControl));
    DEBUG ((DEBUG_INFO, "  TxDataDelayControl1 value = %X\n", EmmcConfig->CustomDllValues.TxDataDelayControl1));
    DEBUG ((DEBUG_INFO, "  TxDataDelayControl2 value = %X\n", EmmcConfig->CustomDllValues.TxDataDelayControl2));
    DEBUG ((DEBUG_INFO, "  RxCmdDataDelayControl1 value = %X\n", EmmcConfig->CustomDllValues.RxCmdDataDelayControl1));
    DEBUG ((DEBUG_INFO, "  RxCmdDataDelayControl2 value = %X\n", EmmcConfig->CustomDllValues.RxCmdDataDelayControl2));
    DEBUG ((DEBUG_INFO, "  RxStrobeDelayControl value = %X\n", EmmcConfig->CustomDllValues.RxStrobeDelayControl));
  }
}

/**
  Print SCS_UFS_CONFIG to serial.

  @param[in] SiPolicyPpi  Pointer to SI_POLICY_PPI
**/
VOID
UfsPrintConfig (
  IN SI_POLICY_PPI  *SiPolicyPpi
  )
{
  SCS_UFS_CONFIG  *UfsConfig;
  EFI_STATUS      Status;
  UINT8           UfsIndex;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gUfsConfigGuid, (VOID *) &UfsConfig);
  ASSERT_EFI_ERROR (Status);

  for (UfsIndex = 0; UfsIndex < PchGetMaxUfsNum (); UfsIndex++) {
    DEBUG ((DEBUG_INFO, "------------------ SCS UFS%d Config ------------------\n", UfsIndex));
    DEBUG ((DEBUG_INFO, " Enable = %d\n", UfsConfig->UfsControllerConfig[UfsIndex].Enable));
    DEBUG ((DEBUG_INFO, " InlineEncryption = %d\n", UfsConfig->UfsControllerConfig[UfsIndex].InlineEncryption));
  }
}

/**
  Print Scs configs and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
ScsPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  SdCardPrintConfig (SiPolicyPpi);
  EmmcPrintConfig (SiPolicyPpi);
  UfsPrintConfig (SiPolicyPpi);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
SdCardLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_SDCARD_CONFIG  *SdCardConfig;

  SdCardConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "SdCardConfig->Header.GuidHob.Name = %g\n", &SdCardConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SdCardConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SdCardConfig->Header.GuidHob.Header.HobLength));

  SdCardConfig->Enable = IsPchSdCardSupported ();
  SdCardConfig->PowerEnableActiveHigh = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
EmmcLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_EMMC_CONFIG  *EmmcConfig;

  EmmcConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "EmmcConfig->Header.GuidHob.Name = %g\n", &EmmcConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EmmcConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EmmcConfig->Header.GuidHob.Header.HobLength));
  EmmcConfig->Enable = IsPchEmmcSupported ();
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
UfsLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_UFS_CONFIG  *UfsConfig;
  UINT8           UfsIndex;

  UfsConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "UfsConfig->Header.GuidHob.Name = %g\n", &UfsConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "UfsConfig->Header.GuidHob.Header.HobLength = 0x%x\n", UfsConfig->Header.GuidHob.Header.HobLength));

  for (UfsIndex = 0; UfsIndex < PchGetMaxUfsNum (); UfsIndex++) {
    UfsConfig->UfsControllerConfig[UfsIndex].Enable = TRUE;
    if (IsMtlSoc ()) {
      UfsConfig->UfsControllerConfig[UfsIndex].InlineEncryption = TRUE;
    }
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mScsBlocks [] = {
  {&gSdCardConfigGuid,  sizeof (SCS_SDCARD_CONFIG), SCS_SDCARD_CONFIG_REVISION, SdCardLoadConfigDefault},
  {&gEmmcConfigGuid,    sizeof (SCS_EMMC_CONFIG),   SCS_EMMC_CONFIG_REVISION,   EmmcLoadConfigDefault},
  {&gUfsConfigGuid,     sizeof (SCS_UFS_CONFIG),    SCS_UFS_CONFIG_REVISION,    UfsLoadConfigDefault}
};

/**
  Get Scs config block table size.

  @retval      Size of config block
**/
UINT16
ScsGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mScsBlocks[0], sizeof (mScsBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add Scs ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ScsAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mScsBlocks[0], sizeof (mScsBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
