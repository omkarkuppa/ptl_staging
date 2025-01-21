/** @file
  Library instance to support Resiliency Feature Functions.

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
#include <Guid/NvmeRecoveryCommon.h>
#include <Library/ResiliencySupportLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>

/**
  Get bios resiliency type.

  @retval SUPPORT_NVME_BASED_RECOVERY       Support NVME Recovery.
  @retval SUPPORT_BIOS_RESILIENCY_RECOVERY  Support Resiliency Recovery.
  @retval NOT_SUPPORT_RECOVERY              It isn't support recovery.

**/
UINT8
EFIAPI
GetBiosResiliencyType (
  VOID
  )
{
  UINT8  BiosResiliencyType;

  BiosResiliencyType = NOT_SUPPORT_RECOVERY;

#if (FixedPcdGetBool (PcdBiosResiliencyEnable) == 1)
  BiosResiliencyType = SUPPORT_BIOS_RESILIENCY_RECOVERY;
#elif (FixedPcdGetBool (PcdNvmeRecoveryEnable) == 1)
  if ((PcdGet8 (PcdNvmeRecoverySupported) == NVME_RECOVERY_FUNCTION_ENABLE_BACKUP_VALUE) ||
    (PcdGet8 (PcdNvmeRecoverySupported) == NVME_RECOVERY_FUNCTION_ENABLE_NO_BACKUP_VALUE)) {
    BiosResiliencyType = SUPPORT_NVME_BASED_RECOVERY;
  }
#endif
  return BiosResiliencyType;
}

/**
  Check whether support csme resiliency function.

  @retval TRUE   Support csme resiliency.
  @retval FALSE  Not support csme resiliency.

**/
BOOLEAN
IsMeResiliencySupported (
  VOID
)
{
#if (FixedPcdGetBool (PcdMeResiliencyEnable) == 1)
  return TRUE;
#else
  return FALSE;
#endif
}

/**
  Check whether there is valid backup content.

  @retval TRUE   There is valid backup content.
  @retval FALSE  No valid backup content.

**/
BOOLEAN
EFIAPI
IsBiosBackupValid (
  VOID
  )
{
  UINT8  BiosResiliencyType;

  BiosResiliencyType = GetBiosResiliencyType ();

  if (BiosResiliencyType == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    return TRUE;
  } else if (BiosResiliencyType == SUPPORT_NVME_BASED_RECOVERY) {
    if (PcdGet8 (PcdNvmeRecoverySupported) == NVME_RECOVERY_FUNCTION_ENABLE_BACKUP_VALUE) {
      return TRUE;
    }
  }
  return FALSE;
}