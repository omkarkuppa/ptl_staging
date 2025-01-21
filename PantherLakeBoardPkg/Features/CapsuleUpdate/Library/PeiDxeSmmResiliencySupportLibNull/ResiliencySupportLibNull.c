/** @file
  Null Library instance to support platform build PcdCapsuleEnable is FALSE.

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
#include <Library/ResiliencySupportLib.h>

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
  return NOT_SUPPORT_RECOVERY;
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
  return FALSE;
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
  return FALSE;
}