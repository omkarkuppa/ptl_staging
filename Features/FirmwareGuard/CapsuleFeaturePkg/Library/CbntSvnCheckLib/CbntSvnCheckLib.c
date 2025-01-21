/**@file
  The file check if S-ACM/BPM/KM from image is greater than or equal to ARBH SVN.

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

#include "CbntSvnCheckLibInternal.h"

#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Register/Cpuid.h>
#include <Library/CbntSvnCheckLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeMeLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/BootGuardLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/FitHelperLib.h>

//
// Use to display progress of sending ME FW
//
ARB_SVN_INFO_ENTRY         mArbSvnInfoTable[MFT_KEY_USAGE_INDEX_UNDEFINED_MANIFEST] = {0};
UINT32                     mNumOfArbSvnEntries = 0;

/**
  Check if need to bypass the hardware anti-replay SVN check.

  This function would check,
    (1) Execute within Simics environment.
    (2) BootGuard is not enabled.
    (3) Flash Descriptor Override(FDO) hardware strap get set.

  If one of above condition stands, bypass the check and return TRUE.
  Otherwise, return FALSE.

  @retval TRUE   Bypass the hardware anti-replay SVN check.
  @retval FALSE  Require the hardware anti-replay SVN check.
**/
BOOLEAN
IsBypassArbhSvnCheck (
  VOID
  )
{
  if (IsSimicsEnvironment ()) {
    DEBUG ((DEBUG_INFO, "Simics is not supported anti-replay SVN information, bypass the ARBH SVN check.\n"));
    return TRUE;
  }

  if (!IsBootGuardSupported ()) {
    DEBUG ((DEBUG_INFO, "BootGuard is not enabled, bypass the ARBH SVN check.\n"));
    return TRUE;
  }

  if (SpiIsFlashDescriptorOverrideEnabled ()) {
    DEBUG ((DEBUG_INFO, "Flash Descriptor Override is enabled, bypass the ARBH SVN check.\n"));
    return TRUE;
  }

  return FALSE;
}

/**
  Get ARBH SVN information

  @param[in] UsageId                Key usage number

  @retval ARB_SVN_INFO_ENTRY        Pointer to requested ARBH SVN Info. NULL means not found.

**/
ARB_SVN_INFO_ENTRY *
GetArbhSvnInfo (
  IN  UINT8          UsageId
  )
{
  EFI_STATUS              Status;
  ARB_SVN_INFO_ENTRY      *ArbSvnInfo;
  UINT32                  Index;

  ArbSvnInfo = NULL;

  if (mNumOfArbSvnEntries == 0) {
    mNumOfArbSvnEntries = MFT_KEY_USAGE_INDEX_UNDEFINED_MANIFEST;
    Status = HeciArbSvnGetInfoMsg (&mNumOfArbSvnEntries, mArbSvnInfoTable);

    if (Status == EFI_SUCCESS) {
      DEBUG_CODE_BEGIN ();
      DEBUG ((DEBUG_INFO, "Get ARB SVN info with %d entries\n", mNumOfArbSvnEntries));
      for (Index = 0; Index < mNumOfArbSvnEntries; Index++) {
        DEBUG ((DEBUG_INFO, "mArbSvnInfoTable[%d]\n",     Index));
        DEBUG ((DEBUG_INFO, "  UsageId         = %d\n",   mArbSvnInfoTable[Index].UsageId));
        DEBUG ((DEBUG_INFO, "  BootPartitionId = 0x%x\n", mArbSvnInfoTable[Index].BootPartitionId));
        DEBUG ((DEBUG_INFO, "  ExecutingSvn    = %d\n",   mArbSvnInfoTable[Index].ExecutingSvn));
        DEBUG ((DEBUG_INFO, "  MinAllowedSvn   = %d\n",   mArbSvnInfoTable[Index].MinAllowedSvn));
      }
      DEBUG_CODE_END ();
    } else {
      DEBUG ((DEBUG_ERROR, "Failed to get ARB SVN info (%r)\n", Status));
      mNumOfArbSvnEntries = 0;
      return NULL;
    }
  }

  for (Index = 0; Index < mNumOfArbSvnEntries; Index++) {
    if (mArbSvnInfoTable[Index].UsageId == UsageId) {
      ArbSvnInfo = &(mArbSvnInfoTable[Index]);
      DEBUG ((DEBUG_INFO, "Found matched ARB SVN info\n"));
      DEBUG ((DEBUG_INFO, "  BootPartitionId = 0x%x\n", ArbSvnInfo->BootPartitionId));
      DEBUG ((DEBUG_INFO, "  ExecutingSvn    = %d\n",   ArbSvnInfo->ExecutingSvn));
      DEBUG ((DEBUG_INFO, "  MinAllowedSvn   = %d\n",   ArbSvnInfo->MinAllowedSvn));
      break;
    }
  }

  if (ArbSvnInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Not found matched ARB SVN info\n"));
  }

  return ArbSvnInfo;
}

/**
  Check if input SVN is greater than or equal to ARBH SVN get by UsageId

  @param[in] UsageId                Key usage number
  @param[in] Svn                    The Svn value to be examined.

  @retval EFI_SUCCESS                 SVN is greater than or equal to ARBH SVN
  @retval EFI_INCOMPATIBLE_VERSION    SVN is smaller than ARBH SVN
  @retval Others                      Failed to check SVN

**/
EFI_STATUS
VerifySvn (
  IN UINT8   UsageId,
  IN UINT16  Svn
  )
{
  ARB_SVN_INFO_ENTRY               *ArbSvnInfo;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS  PreviousUpdateProgress;

  ArbSvnInfo = NULL;

  DEBUG ((DEBUG_INFO, "Usage ID - (%d), Trialed SVN - (%d)\n", UsageId, Svn));

  if (IsBypassArbhSvnCheck ()) {
    return EFI_SUCCESS;
  }

  ArbSvnInfo = GetArbhSvnInfo (UsageId);
  if (ArbSvnInfo == NULL) {
    if (IsPreviousUpdateUnfinished (&PreviousUpdateProgress)) {
      DEBUG ((DEBUG_INFO, "Continue unfinished update, bypass the ARBH SVN check.\n"));
      return EFI_SUCCESS;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  if (Svn < ArbSvnInfo->MinAllowedSvn) {
    DEBUG ((DEBUG_ERROR, "SVN is smaller than MinAllowed ARBH SVN (%d).\n", ArbSvnInfo->MinAllowedSvn));
    return EFI_INCOMPATIBLE_VERSION;
  }

  return EFI_SUCCESS;
}

/**
  Check the FIT type 2 entry is for current platform.

  @param[in]  FitEntryPtr  Pointer for specific FIT entry.

  @retval  TRUE   Assigned FIT type 2 entry is for current platform.
  @retval  FALSE  Assigned FIT type 2 entry is not for current platform.

**/
BOOLEAN
IsType2EntryForCurrPlatform (
  IN CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr
  )
{
  EFI_STATUS              Status;
  CPUID_VERSION_INFO_EAX  CpuSignature;
  CPUID_VERSION_INFO_EAX  CpuFmsVal;
  CPUID_VERSION_INFO_EAX  CpuFmsMaskVal;

  if (FitEntryPtr->Version == FIT_TYPE_VERSION) {
    return FALSE;
  }

  AsmCpuid (CPUID_VERSION_INFO, &CpuSignature.Uint32, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "CpuSignature: 0x%x\n", CpuSignature.Uint32));

  Status = GetFitType2EntryVer200Info (FitEntryPtr, &CpuFmsVal, &CpuFmsMaskVal);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to decode the FMS and FMS mask value from FIT entry - %r.\n", Status));
    return FALSE;
  }

  if ((CpuSignature.Uint32 & CpuFmsMaskVal.Uint32) != CpuFmsVal.Uint32) {
    DEBUG ((DEBUG_ERROR, "Type 2 entry is not for current platform.\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check the FIT entry is for current platform.

  @param[in]  FitEntryPtr  Pointer for specific FIT entry.

  @retval  TRUE   Assigned FIT entry is for current platform.
  @retval  FALSE  Assigned FIT entry is not for current platform.

**/
BOOLEAN
IsFitEntryForCurrPlatform (
  IN CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr
  )
{
  BOOLEAN  IsValid;

  switch (FitEntryPtr->Type) {
    case FIT_TYPE_02_STARTUP_ACM:
      IsValid = IsType2EntryForCurrPlatform (FitEntryPtr);
      break;

    default:
      IsValid = TRUE;
      break;
  }

  return IsValid;
}

/**
  Find FIT Entry by requested type in given BIOS image

  @param[in] Type            Requested FIT Entry type
  @param[in] BiosImage       Pointer to BiosImage buffer
  @param[in] BiosImageSize   Size of BiosImage buffer

  @return Pointer to FIT entry with given Type in BiosImage. NULL means not found.

**/
UINT8 *
FindFitEntryFromBiosImage (
  IN  UINT8         Type,
  IN  UINT8         *BiosImage,
  IN  UINTN         BiosImageSize
  )
{
  EFI_STATUS                      Status;
  UINTN                           BiosAreaBaseAddress;
  UINTN                           BiosSize;
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr;
  UINTN                           FitEntryDataSize;
  UINTN                           FitEntryNum;
  UINT32                          Index;
  EFI_PHYSICAL_ADDRESS            Address;

  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));

  if ((BiosImage == NULL) || (BiosImageSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Invalid parameters\n"));
    return NULL;
  }

  if ((Type == FIT_TYPE_00_HEADER) || (Type == FIT_TYPE_0A_TXT_POLICY)) {
    DEBUG ((DEBUG_ERROR, "Unsupported type (%d)\n", Type));
    return NULL;
  }

#if FixedPcdGetBool(PcdBiosExtenedRegionEnable) == 0
  BiosSize            = (UINTN) PcdGet32 (PcdBiosSize);
  BiosAreaBaseAddress = (UINTN) PcdGet32 (PcdBiosAreaBaseAddress);
#else
  BiosSize            = FixedPcdGet32 (PcdFlashExtendSize) + 0x1000000;
  BiosAreaBaseAddress = (UINTN) PcdGet32 (PcdBiosAreaBaseAddress) + (UINTN) PcdGet32 (PcdFlashExtendOffset);
#endif

  if (BiosImageSize < BiosSize) {
    DEBUG ((DEBUG_ERROR, "Unexpected BIOS image size (0x%x)\n", BiosImageSize));
    return NULL;
  }

  //
  // Get the number of entries with specific type of FIT entry.
  //
  FitEntryDataSize = 0;
  Status           = GetFitEntryBufferByType (
                       BiosImage,
                       BiosImageSize,
                       Type,
                       &FitEntryDataSize,
                       NULL
                       );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_ERROR, "Failed to get the size of FIT entry data - %r.\n", Status));
    return NULL;
  }

  //
  // Get the matched type of entry buffer.
  //
  Status = GetFitEntryBufferByType (
             BiosImage,
             BiosImageSize,
             Type,
             &FitEntryDataSize,
             (VOID **)&FitEntryPtr
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get type %X FIT entry in FIT table - %r.\n", Type, Status));
    return NULL;
  }

  //
  // Iterate all the matched entries.
  //
  FitEntryNum = FitEntryDataSize / sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY);
  for (Index = 0; Index < FitEntryNum; Index++) {
    Address = (EFI_PHYSICAL_ADDRESS)FitEntryPtr[Index].Address;
    DEBUG ((DEBUG_INFO, "Matched type found (0x%x), address = 0x%x\n", Type, Address));

    if (!IsFitEntryForCurrPlatform (&FitEntryPtr[Index])) {
      DEBUG ((DEBUG_ERROR, "FIT entry is not for current platform.\n"));
      continue;
    }

    if ((Address < BiosAreaBaseAddress) || (Address > (BiosAreaBaseAddress + BiosSize))) {
      DEBUG ((DEBUG_ERROR, "Unexpected FIT entry data address (0x%X)\n", Address));
      continue;
    }

    return (BiosImage + (FitEntryPtr[Index].Address - BiosAreaBaseAddress));
  }

  return NULL;
}

/**
  Check if S-ACM SVN from input S-ACM image buffer is greater than or equal to ARBH SVN

  @param[in] SAcmImage                Pointer to S-ACM image buffer.
  @param[in] SAcmImageSize            The size of SAcmImageSize

  @retval EFI_SUCCESS                 S-ACM SVN from SAcmImage is greater than or equal to ARBH SVN
  @retval EFI_INCOMPATIBLE_VERSION    S-ACM SVN from SAcmImage is smaller than ARBH SVN
  @retval Others                      Fail to check S-ACM SVN from SAcmImage

**/
EFI_STATUS
EFIAPI
CheckSvnFromSAcmImage (
  IN  UINT8         *SAcmImage,
  IN  UINTN         SAcmImageSize
  )
{
  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));
  return VerifySvn (MFT_KEY_USAGE_INDEX_OEM_ACM_MANIFEST, ((ACM_HEADER *)SAcmImage)->AcmSvn);
}

/**
  Check if S-ACM/BPM/KM SVN from input BIOS image buffer is greater than or equal to ARBH SVN

  @param[in] BiosImage                Pointer to BIOS image buffer.
  @param[in] BiosImageSize            The size of BiosImage

  @retval EFI_SUCCESS                 SVN from BiosImage is greater than or equal to ARBH SVN
  @retval EFI_INCOMPATIBLE_VERSION    SVN from BiosImage is smaller than ARBH SVN
  @retval Others                      Fail to check SVN from BiosImage

**/
EFI_STATUS
EFIAPI
CheckSvnFromBiosImage (
  IN  UINT8         *BiosImage,
  IN  UINTN         BiosImageSize
  )
{
  EFI_STATUS                          Status;
  ACM_HEADER                          *SAcmHeader;
  BOOT_POLICY_MANIFEST_HEADER         *BpmHeader;
  KEY_MANIFEST_STRUCTURE              *KmHeader;

  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));

  SAcmHeader = (ACM_HEADER *) FindFitEntryFromBiosImage (FIT_TYPE_02_STARTUP_ACM, BiosImage, BiosImageSize);
  BpmHeader  = (BOOT_POLICY_MANIFEST_HEADER *) FindFitEntryFromBiosImage (FIT_TYPE_0C_BOOT_POLICY_MANIFEST, BiosImage, BiosImageSize);
  KmHeader   = (KEY_MANIFEST_STRUCTURE *) FindFitEntryFromBiosImage (FIT_TYPE_0B_KEY_MANIFEST, BiosImage, BiosImageSize);

  if ((SAcmHeader == NULL) || \
      (BpmHeader  == NULL) || \
      (KmHeader   == NULL)) {
    DEBUG ((DEBUG_ERROR, "ACM/BPM/KM header not found in given BiosImage\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  if ((*(UINT64 *)(BpmHeader->StructureId) != BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID) ||
      (*(UINT64 *)(KmHeader->StructureId) != KEY_MANIFEST_STRUCTURE_ID)) {
    DEBUG ((DEBUG_ERROR, "BPM/KM signature not found in given BiosImage\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Check ACM SVN
  //
  Status = VerifySvn (MFT_KEY_USAGE_INDEX_OEM_ACM_MANIFEST, SAcmHeader->AcmSvn);
  DEBUG ((DEBUG_INFO, "Check ACM SVN: %r\n", Status));

  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Check BPM SVN
  //
  Status = VerifySvn (MFT_KEY_USAGE_INDEX_BOOT_POLICY_MANIFEST, BpmHeader->BpmRevocation);
  DEBUG ((DEBUG_INFO, "Check BPM SVN: %r\n", Status));

  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Check KM SVN
  //
  Status = VerifySvn (MFT_KEY_USAGE_INDEX_OEM_KEY_MANIFEST, KmHeader->KmSvn);
  DEBUG ((DEBUG_INFO, "Check KM SVN: %r\n", Status));

Exit:
  DEBUG ((DEBUG_INFO, "%a - End (%r)\n", __FUNCTION__, Status));
  return Status;
}
