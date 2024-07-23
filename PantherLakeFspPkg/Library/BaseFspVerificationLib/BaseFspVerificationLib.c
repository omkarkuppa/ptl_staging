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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/FspVerificationLib.h>

BOOLEAN
Sha384Verify (
  IN VOID                     *HashCtx,
  IN SHA384_HASH_STRUCTURE    *Sha384DigestPtr,
  IN UINTN                    ImageBase,
  IN UINTN                    IbbSegmentCount,
  IN REGION_SEGMENT              *IbbSegments
  );

/**
  Verify FSP-M with the information in BSSS(BSPM) and FBM.
  FSP-M digest information is kept in FBM, FSP will only verify the SHA384 digest.
  FSP-M IBB region information is kept in FBM, only hashed IBB (indicate by flag) should be taken
  into digest calculation. FSP-M IBB region in FBM are relative offset since FSP-M might be relocated
  in OEM bios integration. OEM should provide FSP-M image base information via BSSS(BSPM).

  @param[in]   Bsss      BSSS(BSPM) structure found in BPM.
  @param[in]   Fbm       FSP Boot Manifest which keeps FSP-M digest and IBB information.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyFspm (
  IN BSPM_ELEMENT                   *Bsss,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  )
{
  FSP_REGION_DIGEST       *FspDigest;
  FSP_REGION_STRUCTURE    *FspRegion;
  VOID                    *HashCtx;
  UINTN                   FspmImageBase;

  DEBUG ((DEBUG_INFO, "FSP-M Verification ...\n"));
  if (Bsss == NULL || Fbm == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;
  FspmImageBase = (UINTN) Bsss->FspmBaseAddress;

  FspDigest  = COMPONENT_DIGEST0_PTR (Fbm);
  FspDigest += 1;    ///Skip Digest for FSP-O/T
  if (FspDigest->ComponentID != FSP_REGION_TYPE_FSPM) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-M Digest is Found!\n"));

  FspRegion = FSP_REGION0_PTR (Fbm);
  FspRegion = (FSP_REGION_STRUCTURE *) ((UINT8 *) FspRegion + sizeof (FSP_REGION_STRUCTURE) +
                FspRegion->SegmentCnt * sizeof (REGION_SEGMENT));
  if (FspRegion->ComponentID != FSP_REGION_TYPE_FSPM) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-M Region is Found!\n"));

  if (FspDigest->ComponentDigests.Sha384Digest.Size != 0) {
    if (Sha384Verify (HashCtx, &FspDigest->ComponentDigests.Sha384Digest,
      FspmImageBase, FspRegion->SegmentCnt, IBB_SEGMENTS_PTR (FspRegion)) == TRUE)
    {
      DEBUG ((DEBUG_INFO, "FSP-M Verification Pass.\n"));
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "FSP-M Verification Fail!\n"));
  return EFI_ACCESS_DENIED;
}

/**
  Verify FSP-S with the information in FBM.
  FSP-S digest information is kept in FBM, FSP will only verify the SHA384 digest.
  FSP-S IBB region information is kept in FBM, only hashed IBB (indicate by flag, follow the same way
  to describe region should be calculated in digest or not) should be taken into digest calculation.
  FSP-S IBB region in FBM are relative offset since FSP-S will be executed in memory. OEM should provide
  FSP-S image base in memory.

  @param[in]   FspsImageBase      FSP-S image base in memory.
  @param[in]   Fbm                FSP Boot Manifest which keeps FSP-S digest and IBB information.
  @param[in]   Buffer             Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyFsps (
  IN UINTN                          FspsImageBase,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  )
{
  FSP_REGION_DIGEST       *FspDigest;
  FSP_REGION_STRUCTURE    *FspRegion;
  VOID                    *HashCtx;

  DEBUG ((DEBUG_INFO, "FSP-S Verification Start ...\n"));
  if (Fbm == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;

  FspDigest  = COMPONENT_DIGEST0_PTR (Fbm);
  FspDigest += 2;    ///Skip Digest for FSP-O/T and FSP-M
  if (FspDigest->ComponentID != FSP_REGION_TYPE_FSPS) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-S Digest is Found!\n"));

  FspRegion  = FSP_REGION0_PTR (Fbm);
  FspRegion += 2;
  if (FspRegion->ComponentID != FSP_REGION_TYPE_FSPS) {
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((DEBUG_INFO, "FSP-S Region is Found!\n"));

  if (FspDigest->ComponentDigests.Sha384Digest.Size != 0) {
    if (Sha384Verify (HashCtx, &FspDigest->ComponentDigests.Sha384Digest,
      FspsImageBase, FspRegion->SegmentCnt, IBB_SEGMENTS_PTR (FspRegion)) == TRUE)
    {
      DEBUG ((DEBUG_INFO, "FSP-S Verification Pass!\n"));
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_INFO, "FSP-S Verification Fail!\n"));
  return EFI_ACCESS_DENIED;
}

/**
  Verify BSP-PreMem with the information in BSSS(BSPM).
  BSP-PreMem digest information is kept in BSSS(BSPM), only one TPM required algorithm (SHA256, SHA384,
  SHA512, SM3) is needed for size consideration.

  BSP-PreMem region information is kept in BSSS(BSPM), only hashed IBB (indicate by flag) should be taken
  into digest calculation.

  @param[in]   Bsss      BSSS(BSPM) structure found in BPM.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyBsp (
  IN BSPM_ELEMENT                   *Bsss,
  IN VOID                           *Buffer
  )
{
  VOID                   *HashCtx;
  REGION_SEGMENT         *IbbSegmentPtr;
  UINTN                  Index;
  SHAX_HASH_STRUCTURE    *DigestPtr;
  BOOLEAN                IsBspValid;

  DEBUG ((DEBUG_INFO, "BSP Verification Start ...\n"));

  if (Bsss == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HashCtx = (VOID *) Buffer;

  DEBUG_CODE_BEGIN ();
  IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bsss);
  for (Index = 0; Index < Bsss->BspSegmentCount; Index ++) {
    DEBUG ((DEBUG_INFO, "IbbBase - 0x%x    IbbSize - 0x%x    Flag - %d\n", IbbSegmentPtr->Base, IbbSegmentPtr->Size, IbbSegmentPtr->Flags));
    IbbSegmentPtr ++;
  }
  DEBUG_CODE_END ();

  DigestPtr = (SHAX_HASH_STRUCTURE *) ((UINT8 *) Bsss + sizeof (BSPM_ELEMENT) + Bsss->BspSegmentCount * sizeof (REGION_SEGMENT));
  DEBUG ((DEBUG_INFO, "BSP Digest is found: 0x%x!\n", (UINTN) (VOID *) DigestPtr));

  IsBspValid = FALSE;
  IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bsss);
  if (DigestPtr->HashAlg == TPM_ALG_SHA384) {
    IsBspValid = Sha384Verify (HashCtx, (SHA384_HASH_STRUCTURE *) DigestPtr, 0, Bsss->BspSegmentCount, IbbSegmentPtr);
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