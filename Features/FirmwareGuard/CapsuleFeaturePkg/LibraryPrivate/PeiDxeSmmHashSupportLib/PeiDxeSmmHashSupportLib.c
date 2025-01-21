/** @file
  Library for hash support.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
//
// Advanced Feature Package Foundation.
//
#include <Library/HashSupportLib.h>
//
// Module Self Foundation.
//
#include "PeiDxeSmmHashSupportInternalLib.h"

/**
  Wrapper function to create the SHA digest.

  @param[in]   Data        Pointer of data to create SHA digest.
  @param[in]   DataSize    Size of data to create SHA digest.
  @param[out]  Digest      Double pointer to stored the SHA digest.
  @param[in]   DigestSize  Size of digest to be calculated.

  @retval  EFI_SUCCESS            Succeed to create SHA digest.
  @retval  EFI_INVALID_PARAMETER  Input Data is invalid.
                                  Input Digest is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Failed to allocate the memory.
  @retval  EFI_UNSUPPORTED        Failed to create SHA digest.

**/
EFI_STATUS
EFIAPI
CreateHashShaDigest (
  IN     CONST UINT8  *Data,
  IN           UINTN  DataSize,
     OUT       UINT8  **Digest,
  IN           UINTN  DigestSize
  )
{
  EFI_STATUS            Status;
  BOOLEAN               IsSucceed;
  HASH_SHA_ALG_CONTEXT  *HashAlgCtx;
  UINTN                 CtxSize;
  VOID                  *HashCtx;

  Status      = EFI_SUCCESS;
  IsSucceed   = FALSE;
  HashAlgCtx  = NULL;
  CtxSize     = 0;
  HashCtx     = NULL;

  if ((Data == NULL) || (DataSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Digest == NULL) || (DigestSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetHashShaAlgCtx (DigestSize, &HashAlgCtx);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the specific SHA context size.
  //
  CtxSize = HashAlgCtx->HashShaGetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  if (HashCtx == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initializes SHA context.
  //
  IsSucceed = HashAlgCtx->HashShaInit (HashCtx);
  if (!IsSucceed) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed on initial phase.\n"));
    goto CleanUp;
  }

  //
  // Updates SHA context.
  //
  IsSucceed = HashAlgCtx->HashShaUpdate (HashCtx, Data, DataSize);
  if (!IsSucceed) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed on update phase.\n"));
    goto CleanUp;
  }

  //
  // Completes computation of the SHA digest value.
  //
  IsSucceed = HashAlgCtx->HashShaFinal (HashCtx, *Digest);
  if (!IsSucceed) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed on final phase.\n"));
    goto CleanUp;
  }

  DEBUG ((DEBUG_INFO, "Calculated Digest:\n"));
  DumpHashDigest (*Digest, DigestSize);

CleanUp:
  FreePool (HashCtx);

  return Status;
}

/**
  Wrapper function to examine the SHA digest.

  @param[in]   Data            Pointer of data to create SHA digest.
  @param[in]   DataSize        Size of data to create SHA digest.
  @param[out]  ExpectedDigest  Pointer of SHA digest to be examined.
  @param[in]   DigestSize      Size of digest to be examined.

  @retval  EFI_SUCCESS            Succeed to create SHA digest.
  @retval  EFI_CRC_ERROR          SHA digest is not matched.
  @retval  EFI_INVALID_PARAMETER  Input Data is invalid.
                                  Input ExpectedDigest is invalid.
                                  Input DigestSize is invalid.
  @retval  EFI_OUT_OF_RESOURCES   Failed to allocate the memory.
  @retval  EFI_UNSUPPORTED        Unsupported size of SHA digest.

**/
EFI_STATUS
EFIAPI
ExamineHashShaDigest (
  IN CONST UINT8    *Data,
  IN       UINTN    DataSize,
  IN CONST UINT8    *ExpectedDigest,
  IN       UINTN    DigestSize
  )
{
  EFI_STATUS  Status;
  UINT8       *InputDataDigest;

  if ((Data == NULL) || (DataSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((ExpectedDigest == NULL) || (DigestSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHashShaDigestSupported (DigestSize)) {
    return EFI_UNSUPPORTED;
  }

  Status          = EFI_SUCCESS;
  InputDataDigest = NULL;

  DEBUG ((DEBUG_INFO, "Expected Digest:\n"));
  DumpHashDigest (ExpectedDigest, DigestSize);

  //
  // Allocate memory for digest.
  //
  InputDataDigest = AllocatePool (DigestSize);
  if (InputDataDigest == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (InputDataDigest, DigestSize);

  //
  // Calculate the hash digest.
  //
  Status = CreateHashShaDigest (Data, DataSize, &InputDataDigest, DigestSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to create the hash digest.\n."));
    goto CleanUp;
  }

  //
  // Check if digest is matched.
  //
  if (CompareMem (InputDataDigest, ExpectedDigest, DigestSize) != 0) {
    Status = EFI_CRC_ERROR;
    DEBUG ((DEBUG_ERROR, "Digest is not matched.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Digest check passed.\n"));
  }

CleanUp:
  FreePool (InputDataDigest);

  return Status;
}
