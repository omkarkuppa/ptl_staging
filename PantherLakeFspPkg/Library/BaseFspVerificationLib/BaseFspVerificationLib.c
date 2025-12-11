/** @file
  This file contains functions to support FSP verification operations.

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

#include <IndustryStandard/Tpm20.h>
#include <PiPei.h>
#include <FspEas.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/FspVerificationLib.h>
#include <Library/FspMeasurementLib.h>
#include <Library/FspFbmSupportLib.h>
#include <Library/HobLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Library/IoLib.h>
#include <Txt.h>
#include <FbmDataHob.h>

/**
  DisableTxtCmos to clear the TXT enable bit in CMOS

  @param[in]   Bspm  BSPM element containing the CMOS offset.

**/
VOID
EFIAPI
DisableTxtCmos (
  IN UINT8 CmosTxtOffset
  )
{
  UINT8   CmosStatus;

  DEBUG ((DEBUG_INFO, "CmosTxtOffset=0x%x\n", CmosTxtOffset));

  if (CmosTxtOffset == 0) {
    CmosTxtOffset = FIT_REC_TXT_POLICY_TYPE_A;
  }
  IoWrite8 (CMOS_IO_ADDRESS, CmosTxtOffset);
  CmosStatus = IoRead8 (CMOS_IO_DATA);

  IoWrite8 (CMOS_IO_ADDRESS, CmosTxtOffset);
  IoWrite8 (CMOS_IO_DATA, CmosStatus & ~BIT4);  // Clear BIT4
}

BOOLEAN
Sha384Verify (
  IN VOID                     *HashCtx,
  IN SHA384_HASH_STRUCTURE    *Sha384DigestPtr,
  IN UINTN                    ImageBase,
  IN UINTN                    IbbSegmentCount,
  IN REGION_SEGMENT           *IbbSegments
  );

/**
  Detect the boot guard profile.

  @retval BootGuardProfile  Boot Guard Profile.

**/
UINT8
DetectBootGuardProfile (
  VOID
  )
{
  UINT8   BootGuardProfile = 0;
  UINT64  MsrValue;
  // Get FVME status from MSR 0x13A
  // F = Force Anchor Boot
  // V = Verified boot
  // M = Measured boot
  // E = Enforcement
  // BTG Profile detection
  // Profile  F  V  M  E
  //    0     0  0  0  0
  //    3     0  1  1  0
  //    4     1  1  0  1
  //    5     1  1  1  1
  // F (BIT[4]), M (BIT[5]), V (BIT[6]) bits should be fine to detect boot guard profile.
  //
  MsrValue = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
  //
  // Check condition from above table.
  //
  if (MsrValue & B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT) {
    if (MsrValue & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) {
      if (MsrValue & B_BOOT_GUARD_SACM_INFO_FORCE_ANCHOR_BOOT) {
        // Measured boot
        BootGuardProfile = BOOT_GUARD_PROFILE_5;
      } else {
        BootGuardProfile = BOOT_GUARD_PROFILE_3;
      }
    } else {
      if (MsrValue & B_BOOT_GUARD_SACM_INFO_FORCE_ANCHOR_BOOT) {
        // Verified boot
        BootGuardProfile = BOOT_GUARD_PROFILE_4;
      }
    }
  }
  return BootGuardProfile;
}

/**
  Verify CRTM Status and disable Txt Cmos
  Disable TXT when verification fail in BTG 0T/3T.

  @param[in]   CmosOffset  CMOS offset from BSPM.

**/
VOID
EFIAPI
VerifyCrtmStatusAndDisableTxtCmos (
  IN UINT8     CmosOffset
  )
{
  UINT64            AcmPolicyStatus;
  UINT8             BootGuardProfile;

  AcmPolicyStatus = *(UINT64 *) (UINTN) (MMIO_ACM_POLICY_STATUS);
  BootGuardProfile = DetectBootGuardProfile ();

  if ((BootGuardProfile < BOOT_GUARD_PROFILE_4) && (AcmPolicyStatus & (B_SCRTM_STATUS))) {
      DisableTxtCmos (CmosOffset);
      DEBUG ((DEBUG_INFO, "TXT disabled due to verification failure in BTG 0T/3T\n"));
  } else {
    CpuDeadLoop ();
  }
}

/**
  Check if FSP signing is supported.

  @retval TRUE   Signing is supported.
  @retval FALSE  Signing is not supported.

**/
UINT8
IsSigningSupported (
  VOID
  )
{
  UINT64         AcmPolicyStatus;
  UINT8          BootGuardProfile;
  IBB_ELEMENT    *BpmIbb;

  BpmIbb = LocateBpmIbbElement (); // Locate Bpm IBB Element
  if (BpmIbb == NULL) {
    return FALSE;
  }
  if (!(BpmIbb->Flags & IBB_FLAG_BOOT_COMPONENT_BIT)) {
    return FALSE;
  }

  AcmPolicyStatus = *(UINT64 *) (UINTN) (MMIO_ACM_POLICY_STATUS);
  BootGuardProfile = DetectBootGuardProfile ();

  if (AcmPolicyStatus & B_FBM_VALID_STATUS) {
    if ((BootGuardProfile >= BOOT_GUARD_PROFILE_4) ||
        ((BootGuardProfile < BOOT_GUARD_PROFILE_4) && (AcmPolicyStatus & (B_SCRTM_STATUS)))) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Find FSP header pointer.

  @param[in] FlashFvFspBase Flash address of FSP FV.

  @return FSP header pointer.
**/
FSP_INFO_HEADER *
EFIAPI
FspFindFspHeader (
  IN EFI_PHYSICAL_ADDRESS  FlashFvFspBase
  )
{
  UINT8  *CheckPointer;

  CheckPointer = (UINT8 *)(UINTN) FlashFvFspBase;

  if (((EFI_FIRMWARE_VOLUME_HEADER *) CheckPointer)->Signature != EFI_FVH_SIGNATURE) {
    return NULL;
  }

  if (((EFI_FIRMWARE_VOLUME_HEADER *) CheckPointer)->ExtHeaderOffset != 0) {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->ExtHeaderOffset;
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_EXT_HEADER *)CheckPointer)->ExtHeaderSize;
    CheckPointer = (UINT8 *)ALIGN_POINTER (CheckPointer, 8);
  } else {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->HeaderLength;
  }

  CheckPointer = CheckPointer + sizeof (EFI_FFS_FILE_HEADER);
  CheckPointer = CheckPointer + sizeof (EFI_RAW_SECTION);

  return (FSP_INFO_HEADER *)CheckPointer;
}

/**
  Get FSP Version information.

  @param[in]   Bspm        BSPM structure found in BPM.
  @param[out]  FspVersion  Holds the FSP version.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
GetFspVersion (
  IN BSPM_ELEMENT         *Bspm,
  OUT UINT8               FspVersion []
  )
{
  FSP_INFO_HEADER         *FspInfoHeader;
  UINTN                   FspmImageBase;

  if (Bspm->FspmLoadingPolicy & FSPM_COMPRESSED) {
    FspmImageBase = (UINTN) PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB;
  } else {
    FspmImageBase = (UINTN) Bspm->FspmBaseAddress;
  }

  FspInfoHeader = (FSP_INFO_HEADER *) FspFindFspHeader ((UINT32) FspmImageBase);
  if (FspInfoHeader == NULL) {
    DEBUG ((DEBUG_INFO, "FspInfoHeader is NULL.\n"));
    return EFI_INVALID_PARAMETER;
  }

  FspVersion[0] = FspInfoHeader->ImageRevision & 0xff;
  FspVersion[1] = FspInfoHeader->ExtendedImageRevision & 0xff;
  FspVersion[2] = (FspInfoHeader->ImageRevision & 0xff00) >> 8;
  FspVersion[3] = (FspInfoHeader->ExtendedImageRevision & 0xff00) >> 8;
  FspVersion[4] = (UINT8)((FspInfoHeader->ImageRevision & 0x00ff0000) >> 16);
  FspVersion[5] = (UINT8)((FspInfoHeader->ImageRevision & 0xff000000) >> 24);

  DEBUG ((DEBUG_INFO,
          "FSP Version : %02x.%02x.%04x.%04x\n",
          FspVersion[5],
          FspVersion[4],
          (FspVersion[3] << 8) | (FspVersion[2]),
          (FspVersion[1] << 8) | (FspVersion[0])
        ));

  return EFI_SUCCESS;
}

/**
  Verify FSP Version information in BSPM and FBM.
  FSP Version digest information is kept in FBM, FSP will only verify the SHA384 digest.

  @param[in]   Bspm      BSPM structure found in BPM.
  @param[in]   Fbm       FSP Boot Manifest which keeps FSP-M digest and IBB information.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyFspVersion (
  IN BSPM_ELEMENT                   *Bspm,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  )
{
  FSP_REGION_DIGEST       *FspDigest;
  VOID                    *HashCtx;
  UINT8                   FspVersion [6];
  REGION_SEGMENT          Segment;

  DEBUG ((DEBUG_INFO, "FSP Version Verification ...\n"));
  if (Bspm == NULL || Fbm == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;

  FspDigest  = VERSION_DIGEST_PTR (Fbm);
  if (FspDigest->ComponentID != FSP_REGION_TYPE_FSP_VERSION) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP Version Digest is Found!\n"));

  GetFspVersion (Bspm, FspVersion);

  Segment.Reserved = 0;
  Segment.Flags    = 0;
  Segment.Base     = 0;
  Segment.Size     = sizeof (FspVersion);

  if (FspDigest->ComponentDigests.Sha384Digest.Size != 0) {
    if (Sha384Verify (HashCtx, &FspDigest->ComponentDigests.Sha384Digest,
      (UINTN) &FspVersion, 1, &Segment) == TRUE)
    {
      DEBUG ((DEBUG_INFO, "FSP Version Verification Pass.\n"));
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "FSP Version Verification Fail!\n"));
  return EFI_ACCESS_DENIED;
}

/**
  Verify FSP-M with the information in BSPM and FBM.
  FSP-M digest information is kept in FBM, FSP will only verify the SHA384 digest.
  FSP-M IBB region information is kept in FBM, only hashed IBB (indicate by flag) should be taken
  into digest calculation. FSP-M IBB region in FBM are relative offset since FSP-M might be relocated
  in OEM bios integration. OEM should provide FSP-M image base information via BSPM.

  @param[in]   Bspm      BSPM structure found in BPM.
  @param[in]   Fbm       FSP Boot Manifest which keeps FSP-M digest and IBB information.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyAndExtendFspm (
  IN BSPM_ELEMENT                   *Bspm,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  )
{
  FSP_REGION_DIGEST       *FspDigest;
  FSP_REGION_STRUCTURE    *FspRegion;
  VOID                    *HashCtx;
  UINTN                   FspmImageBase;

  DEBUG ((DEBUG_INFO, "FSP-M Verification ...\n"));
  if (Bspm == NULL || Fbm == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;
  if (Bspm->FspmLoadingPolicy & FSPM_COMPRESSED) {
    FspmImageBase = (UINTN) PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB;
  } else {
    FspmImageBase = (UINTN) Bspm->FspmBaseAddress;
  }

  FspDigest  = COMPONENT_DIGEST0_PTR (Fbm);
  FspDigest += 1;    ///Skip Digest for FSP-O/T
  if (FspDigest->ComponentID != FSP_REGION_TYPE_FSPM) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-M Digest is Found!\n"));

  FspRegion = FSP_REGION0_PTR (Fbm);
  FspRegion = (FSP_REGION_STRUCTURE *) ((UINT8 *) FspRegion + sizeof (FSP_REGION_STRUCTURE) +
                FspRegion->SegmentCnt * sizeof (REGION_SEGMENT));

  if ((FspRegion->ComponentID != FSP_REGION_TYPE_FSPM) ||
       FspDigest->ComponentDigests.Sha384Digest.Size == 0) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-M Region is Found!\n"));

  if (Sha384Verify (HashCtx, &FspDigest->ComponentDigests.Sha384Digest,
    FspmImageBase, FspRegion->SegmentCnt, IBB_SEGMENTS_PTR (FspRegion)) == TRUE)
  {
    DEBUG ((DEBUG_INFO, "FSP-M Verification Pass.\n"));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "FSP-M Verification Fail!\n"));
  return EFI_ACCESS_DENIED;
}

/**
  Verify FSP-S with the information from FBM Data HOB.
  FSP-S digest information is retrieved from FBM Data HOB and will only verify the SHA384 digest.
  FSP-S IBB region information is retrieved from HOB, only hashed IBB (indicated by flag, following the same way
  to describe which regions should be calculated in digest or not) should be taken into digest calculation.
  FSP-S IBB region segments in HOB are relative offsets since FSP-S will be executed in memory. OEM should provide
  FSP-S image base in memory.

  @param[in]   FspsImageBase      FSP-S image base in memory.
  @param[in]   Buffer             Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_NOT_FOUND           FBM data HOB not found.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyAndLogEventFsps (
  IN UINTN                          FspsImageBase,
  IN VOID                           *Buffer
  )
{
  VOID                    *HashCtx;
  UINT8                   CmosOffset;
  SHA384_HASH_STRUCTURE   *FspsDigest;
  UINTN                   SegmentCount;
  REGION_SEGMENT          *Segments;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "FSP-S Verification Start ...\n"));

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Retrieve FBM data from HOB
  //
  Status = GetFbmDataFromHob (&FspsDigest, &SegmentCount, &Segments, &CmosOffset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HashCtx = (VOID *) Buffer;

  DEBUG ((DEBUG_INFO, "Using FSP-S digest\n"));
  DEBUG ((DEBUG_INFO, "FSP-S SegmentCount: %d\n", SegmentCount));

  if (Sha384Verify (HashCtx, FspsDigest, FspsImageBase, SegmentCount, Segments) == TRUE)
  {
    DEBUG ((DEBUG_INFO, "FSP-S Verification Pass!\n"));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_ERROR, "FSP-S Verification Fail!\n"));
  VerifyCrtmStatusAndDisableTxtCmos (CmosOffset);

  return EFI_ACCESS_DENIED;
}

/**
  Verify and extend BSP-PreMem with the information in BSPM.
  BSP-PreMem digest information is kept in BSPM, only one TPM required algorithm (SHA256, SHA384,
  SHA512, SM3) is needed for size consideration.

  BSP-PreMem region information is kept in BSPM, only hashed IBB (indicate by flag) should be taken
  into digest calculation.

  @param[in]   Bspm      Bspm structure found in BPM.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyBsp (
  IN BSPM_ELEMENT                   *Bspm,
  IN VOID                           *Buffer
  )
{
  VOID                   *HashCtx;
  REGION_SEGMENT         *IbbSegmentPtr;
  UINT32                 Index;
  SHAX_HASH_STRUCTURE    *DigestPtr;
  BOOLEAN                IsBspValid;

  DEBUG ((DEBUG_INFO, "BSP Verification Start ...\n"));

  if (Bspm == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;

  DEBUG_CODE_BEGIN ();
  IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bspm);
  for (Index = 0; Index < Bspm->BspSegmentCount; Index ++) {
    DEBUG ((DEBUG_INFO, "IbbBase - 0x%x    IbbSize - 0x%x    Flag - %d\n", IbbSegmentPtr->Base, IbbSegmentPtr->Size, IbbSegmentPtr->Flags));
    IbbSegmentPtr ++;
  }
  DEBUG_CODE_END ();

  DigestPtr = (SHAX_HASH_STRUCTURE *) ((UINT8 *) Bspm + sizeof (BSPM_ELEMENT) + Bspm->BspSegmentCount * sizeof (REGION_SEGMENT));
  DEBUG ((DEBUG_INFO, "BSP Digest is found: 0x%x!\n", (UINTN) (VOID *) DigestPtr));

  IsBspValid = FALSE;
  IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bspm);
  if (DigestPtr->HashAlg == TPM_ALG_SHA384) {
    IsBspValid = Sha384Verify (HashCtx, (SHA384_HASH_STRUCTURE *) DigestPtr, 0, Bspm->BspSegmentCount, IbbSegmentPtr);
  } else {
    DEBUG ((DEBUG_INFO, "The first digest must be Sha384 format!\n"));
  }

  if (IsBspValid) {
    DEBUG ((DEBUG_INFO, "BSP Verification Pass!\n"));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "BSP Verification Fail!\n"));
  return EFI_ACCESS_DENIED;
}

/**
  Retrieve FBM data from HOB.
  
  This function locates the FSP-S verification data HOB created earlier
  and returns pointers to the digest and region segments.
  
  @param[out] FspsDigest    Pointer to receive FSP-S SHA384 digest pointer
  @param[out] SegmentCount  Pointer to receive segment count
  @param[out] Segments      Pointer to receive segments array pointer
  @param[out] CmosOffset    Pointer to receive CMOS offset

  @retval EFI_SUCCESS      Data retrieved successfully from HOB
  @retval EFI_NOT_FOUND    FSP-S verification data HOB not found

**/
EFI_STATUS
EFIAPI
GetFbmDataFromHob (
  OUT SHA384_HASH_STRUCTURE  **FspsDigest,
  OUT UINTN                  *SegmentCount,
  OUT REGION_SEGMENT         **Segments,
  OUT UINT8                  *CmosOffset
  )
{
  EFI_PEI_HOB_POINTERS  Hob;
  FBM_DATA_HOB          *HobData;
  
  //
  // Locate FSP-S verification data HOB
  //
  Hob.Raw = GetFirstGuidHob (&gFbmDataHobGuid);
  
  if (Hob.Raw == NULL) {
    DEBUG ((DEBUG_ERROR, "FSP-S verification data HOB not found!\n"));
    DEBUG ((DEBUG_ERROR, "FSP Signing is not supported!\n"));
    return EFI_NOT_FOUND;
  }
  
  HobData = GET_GUID_HOB_DATA (Hob);
  
  //
  // Return pointers to data within HOB
  //
  *FspsDigest = &HobData->FspDigestSha384;
  *SegmentCount = HobData->SegmentCount;
  *Segments = &HobData->Segments[0];
  *CmosOffset = HobData->CmosOffset;

  return EFI_SUCCESS;
}
