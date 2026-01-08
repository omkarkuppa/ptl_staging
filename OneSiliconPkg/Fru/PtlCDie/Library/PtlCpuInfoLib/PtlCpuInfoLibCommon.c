/** @file
  Information library for PantherLake CPU die.

  This library is available for PEI, DXE and SMM.
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/PtlCpuInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/CpuLib.h>
#include <Protocol/IntelDieInfo.h>

EFI_STATUS
CpuInstallDieInfoInternal (
  IN  EFI_GUID                         Type,
  IN  UINT32                           DieIndex,
  IN  UINT64                           DieId,
  IN  UINT32                           SteppingId,
  IN  EDKII_INTEL_DIE_INFO_GET_DIE_NAME_STR  GetNameStr,
  IN  EDKII_INTEL_DIE_INFO_GET_STEPPING_STR  GetSteppingStr,
  IN  EDKII_INTEL_DIE_INFO_GET_SKU_STR       GetSkuStr
  );

/**
  This function returns pointer to the const CPU name string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
CpuDieGetNameStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return GetGenerationString ();
  } else {
    return NULL;
  }
}

/**
  This function returns pointer to the const CPU stepping string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
CpuDieGetSteppingStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return GetRevisionTable ();
  } else {
    return NULL;
  }
}

/**
  This function returns pointer to the const CPU SKU string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
CpuDieGetSkuStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return GetSkuString ();
  } else {
    return NULL;
  }
}

/**
  This function installs DieInfo instance for CPU die.

  @retval EFI_SUCCESS if instance has been installed successfully.
**/
EFI_STATUS
CpuInstallDieInfo (
  VOID
  )
{
  return CpuInstallDieInfoInternal (
          gIntelDieInfoCpuGuid,
          1,
          GetCpuFamilyModel (),
          GetCpuSteppingId (),
          CpuDieGetNameStr,
          CpuDieGetSteppingStr,
          CpuDieGetSkuStr
          );
}