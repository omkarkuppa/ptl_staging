/** @file
  Initialize TDX in PEI

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

#include <Uefi.h>
#include <Ppi/SiPolicy.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/TdxDataHob.h>

/**
  Reports if TDX is Supported or not

  @retval TRUE             If TDX is Supported
  @retval FALSE            If TDX is not Supported
**/
BOOLEAN
IsTdxSupported (
  VOID
  )
{
  if (IsSeamModeSupported () == FALSE) {
    return FALSE;
  }
  return TRUE;
}

/**
  Reports if TDX is Enabled or not
  @retval TRUE             If TDX is Enabled
  @retval FALSE            If TDX is not Enabled
**/
BOOLEAN
IsTdxEnabled (
  VOID
  )
{
  TDX_DATA_HOB *TdxDataHobPtr;
  TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);

  if (TdxDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TdxInfoLib: Error getting TDX Data HOB!\n"));
    return FALSE;
  }

  return (TdxDataHobPtr->TdxEnabled != 0);
}

/**
  Reports seamrr size in MB

  @param[out] SeamrrSize

  @retval     EFI_INVALID_PARAMETER - Bad inputs provided
  @retval     EFI_UNSUPPORTED       - System is not TDX capable
  @retval     EFI_NOT_READY         - Cannot retrieve seamrr size
  @retval     EFI_SUCCESS
**/
EFI_STATUS
GetSeamrrSize (
  OUT UINT32 *SeamrrSize
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  TDX_DATA_HOB *TdxDataHobPtr = NULL;

  if (SeamrrSize == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (IsTdxSupported () == FALSE) {
    DEBUG ((DEBUG_ERROR, "TDX is not supported in current system!\n"));
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (TdxDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TdxInfoLib: Error getting TDX Data HOB!\n"));
    Status = EFI_NOT_READY;
    return Status;
  }

  *SeamrrSize = TdxDataHobPtr->SeamrrSize;

  return Status;
}
