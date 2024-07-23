/**@file
#  This PEIM driver request Nvme recovery seed to csme, and save it to hob
#  produce gCsmeSeedHobGuid instance for other driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "NvmeRecoveryPei.h"

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiMasterBootModePpiGuid,
    CsmeSeedRequest
  }
};

/**
  Bios send seed request to CSME before MRC initialization.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
EFIAPI
CsmeSeedRequest (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
  EFI_STATUS                   Status;
  SEED_PROPERTIES              SeedContent;
  GET_BIOS_SEED_RESPONSE       BiosSeedResp;

  ZeroMem (&SeedContent, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedResp, sizeof (GET_BIOS_SEED_RESPONSE));

  SeedContent.CsmeSvn = 1;
  SeedContent.BiosSvn = 1;
  SeedContent.SeedPropertiesBitMap.SeedDebugProperty = 1;    // 1 - seed is same in lock and unlock
  SeedContent.SeedPropertiesBitMap.SeedHwFpfProperty = 1;    // 1 - seed is same regardless source of FPF values (HW or UEP)
  SeedContent.SeedPropertiesBitMap.SeedBtgProperty   = 1;    // 1 - seed is same regardless of BTG state (enabled or disable)
  SeedContent.SeedPropertiesBitMap.SeedType          = 1;    // fixed/constant
  SeedContent.SeedPropertiesBitMap.SeedLength        = 1;    // 256 bits

  Status = PeiHeciGetBiosSeed (&SeedContent, 0, &BiosSeedResp.EphemeralKeyCounter, &BiosSeedResp.BiosSeedInfo, (UINT8 *)&(BiosSeedResp.BiosSeed));
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Seed request is failed. \n"));
    return Status;
  }

  //
  // Build HOB to transfer csme seed to NvmeRecovery Dxe driver
  //
  BuildGuidDataHob (
    &gCsmeSeedHobGuid,
    &BiosSeedResp,
    sizeof (GET_BIOS_SEED_RESPONSE)
    );

  //
  // Clear security data in infrequently mapped memory.
  //
  ZeroMem (&SeedContent, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedResp, sizeof (GET_BIOS_SEED_RESPONSE));

  return EFI_SUCCESS;
}

/**
  Check whether support NVMe Based Recovery feature.

  @retval TRUE   CSME Support NVMe Recovery
  @retval FALSE  CSME not Support NVMe Recovery

**/
UINT8
EFIAPI
IsNvmeBrandIdSupport (
  VOID
  )
{
  UINT32  MsrBrandIdentityValue;
  MsrBrandIdentityValue = 0;

  if (!EFI_ERROR (GetMsrBrandIdentityValue (&MsrBrandIdentityValue))) {
    if ((MsrBrandIdentityValue & NVME_BASED_BRAND_IDENTITY_BIT) != NVME_BASED_BRAND_IDENTITY_BIT) {
      DEBUG ((DEBUG_INFO, "NVMe Recovery Function is not supported. Reason Brand Identity bits not set.\n"));
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Init PcdNvmeRecoverySupported.

  NVME_RECOVERY_FUNCTION_DEFAULT_DISABLE_VALUE means not support NVMe Based Recovery Feature.
  Other value support NVMe Based Recovery Feature.

**/
VOID
InitNvmeRecoverySupportPcd (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiReadOnlyVarPpi;
  NVME_BASED_RECOVERY_HII_CONFIG   NvmeHii;
  UINTN                            NvmeHiiSize;
  NVME_BACKUP_INFO                 NvmeBackupInfo;
  UINTN                            NvmeBackupInfoSize;
  UINT8                            NvmeRecoverySupport;

  NvmeHiiSize         = sizeof (NVME_BASED_RECOVERY_HII_CONFIG);
  NvmeBackupInfoSize  = sizeof (NVME_BACKUP_INFO);
  NvmeRecoverySupport = NVME_RECOVERY_FUNCTION_DEFAULT_DISABLE_VALUE; // default disable

  if (!IsNvmeBrandIdSupport ()) {
    NvmeRecoverySupport = NVME_RECOVERY_FUNCTION_CSME_NOT_SUPPORT;
    goto Exit;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiReadOnlyVarPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate gEfiPeiReadOnlyVariable2PpiGuid\n"));
    ASSERT_EFI_ERROR (Status);
    goto Exit;
  }

  //
  // Get setup config about NVMe Based Recovery Feature
  //
  Status = PeiReadOnlyVarPpi->GetVariable (
                                PeiReadOnlyVarPpi,
                                NVME_BASED_RECOVERY_VARIABLE_NAME,
                                &gNvmeBasedRecoveryHiiGuid,
                                NULL,
                                &NvmeHiiSize,
                                &NvmeHii
                                );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "Failed to get the variable\n"));
      ASSERT_EFI_ERROR (Status);
    }
    goto Exit;
  }
  if (NvmeHii.NvmeRecoverySupported == NVME_RECOVERY_FUNCTION_ENABLE_VALUE) {
    //
    // Get Backup status
    //
    Status = PeiReadOnlyVarPpi->GetVariable (
                                  PeiReadOnlyVarPpi,
                                  NVME_BASED_RECOVERY_INFO_VARIABLE,
                                  &gNvmeBasedRecoveryVarGuid,
                                  NULL,
                                  &NvmeBackupInfoSize,
                                  &NvmeBackupInfo
                                  );
    if (!(EFI_ERROR (Status)) && (NvmeBackupInfo.BackupStatus == BackupSuccess)) {
      NvmeRecoverySupport = NVME_RECOVERY_FUNCTION_ENABLE_BACKUP_VALUE;
    } else {
      NvmeRecoverySupport = NVME_RECOVERY_FUNCTION_ENABLE_NO_BACKUP_VALUE;
    }
  }

Exit:
  PcdSet8S (PcdNvmeRecoverySupported, NvmeRecoverySupport);
  DEBUG ((DEBUG_INFO, "Set Dynamics PcdNvmeRecoverySupported = %d\n", NvmeRecoverySupport));
}

/**
  The user code starts with this function.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval Others                 Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
NvmeRecoveryPeimEntry (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  InitNvmeRecoverySupportPcd ();
  if (GetBiosResiliencyType () != SUPPORT_NVME_BASED_RECOVERY) {
    DEBUG ((DEBUG_INFO, "NVMe Recovery Function is not supported, skip csme seed request.\n"));
    return EFI_SUCCESS;
  }
  Status = PeiServicesNotifyPpi (mSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
