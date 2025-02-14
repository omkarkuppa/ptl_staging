/** @file
  Library to get FSP information or perform specific action on platform.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
//
// Silicon Foundation.
//
#include <Register/PmcRegs.h>
#include <Library/PmcLib.h>
//
// Other AFPs Foundation.
//
#include <Library/FspInfoLib.h>
//
// AFP Foundation.
//
#include <Library/PlatformFspUpdateHookLib.h>

#define FSP_FMP_VERSION_STRING_FORMAT  L"Intel FSP XX.XX.XXXX.XXXX"
#define FSP_FMP_VERSION_STRING_SIZE    (sizeof (FSP_FMP_VERSION_STRING_FORMAT))

/**
  Clear the DISB - DRAM Initialization Scratchpad Bit.

**/
VOID
ClearDramInitScratchpad (
  VOID
  )
{
  //
  // Clear B_PMC_PWRM_GEN_PMCON_A_DISB.
  //
  MmioAnd8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2, (UINT8) ~((B_PMC_PWRM_GEN_PMCON_A_DISB) >> 16));
}

/**
  Get the FSP information HOB.

  @param[out]  FspInfoHobPtr  Pointer to the FSP information HOB.

  @retval  EFI_SUCCESS            Succeed to get the FSP information HOB.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          FSP information HOB is not exist.
  @retval  Others                 Failed to get the FSP information HOB.

**/
EFI_STATUS
GetFspInfoHob (
  OUT FSP_INFO_HOB  **FspInfoHobPtr
  )
{
  EFI_HOB_GUID_TYPE  *GuidHob;

  GuidHob = NULL;

  if (FspInfoHobPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&gFspInfoGuid);
  if (GuidHob == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  *FspInfoHobPtr = (FSP_INFO_HOB *)GET_GUID_HOB_DATA (GuidHob);

  return EFI_SUCCESS;
}

/**
  Get the version value of currently stored firmware image.

  @param[out] VersionPtr  A pointer to the version of currently stored firmware image.

  @retval  EFI_SUCCESS            Succeed to get the version value of currently
                                  stored firmware image.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported to get the version value of currently
                                  stored firmware image.
  @retval  Others                 Failed to get the version value of currently
                                  stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetFspVersion (
  OUT UINT32  *VersionPtr
  )
{
  EFI_STATUS    Status;
  FSP_INFO_HOB  *FspInfoHobPtr;
  UINT16        FspRevision;
  UINT16        FspBuild;

  FspInfoHobPtr = NULL;
  FspRevision   = 0;
  FspBuild      = 0;

  if (VersionPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetFspInfoHob (&FspInfoHobPtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FspRevision = FspInfoHobPtr->VersionRevision;
  FspBuild    = FspInfoHobPtr->VersionBuild;
  *VersionPtr = (FspRevision << 16) | (FspBuild);

  return EFI_SUCCESS;
}

/**
  Get the Null-terminated Unicode string used to fill in the VersionName field.

  @param[out]  VersionStringPtr  Double pointer to the version string retrieved of
                                 currently stored firmware image.

  @retval  EFI_SUCCESS            Succeed to get the version string of currently
                                  stored firmware image.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported to get the version sting of currently
                                  stored firmware image.
  @retval  Others                 Failed to get the version sting of currently
                                  stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetFspVersionString (
  OUT CHAR16  **VersionStringPtr
  )
{
  EFI_STATUS    Status;
  FSP_INFO_HOB  *FspInfoHobPtr;

  FspInfoHobPtr = NULL;

  if (VersionStringPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetFspInfoHob (&FspInfoHobPtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *VersionStringPtr = (CHAR16 *)AllocateZeroPool (FSP_FMP_VERSION_STRING_SIZE);
  if (*VersionStringPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UnicodeSPrint (
    *VersionStringPtr,
    FSP_FMP_VERSION_STRING_SIZE,
    L"Intel FSP %02X.%02X.%04X.%04X",
    FspInfoHobPtr->VersionMajor,
    FspInfoHobPtr->VersionMinor,
    FspInfoHobPtr->VersionRevision,
    FspInfoHobPtr->VersionBuild
    );

  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task before FSP update.

  @retval  EFI_SUCCESS  Succeed to perform platform required actions.
  @retval  Others       Failed to perform platform required actions.

**/
EFI_STATUS
EFIAPI
PlatformFmpFspUpdatePreHook (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  A function used to perform platform related task after FSP update.

  @param[in]  UpdateStatus  The status of BIOS update.

  @retval  EFI_SUCCESS  Succeed to perform platform required actions.
  @retval  Others       Failed to perform platform required actions.

**/
EFI_STATUS
EFIAPI
PlatformFmpFspUpdatePostHook (
  IN EFI_STATUS  UpdateStatus
  )
{
  if (!EFI_ERROR (UpdateStatus)) {
    //
    // Clear the DRAM DISB bit.
    //
    ClearDramInitScratchpad ();
  }

  return EFI_SUCCESS;
}
