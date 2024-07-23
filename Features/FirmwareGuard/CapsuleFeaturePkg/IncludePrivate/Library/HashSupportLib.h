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

#ifndef _HASH_SUPPORT_LIB_H_
#define _HASH_SUPPORT_LIB_H_

#include <Uefi.h>

#define HASH_SHA256_DIGEST_SIZE  32
#define HASH_SHA384_DIGEST_SIZE  48
#define HASH_SHA512_DIGEST_SIZE  64

#define CAPSULE_HASH_DIGEST_SIZE      HASH_SHA384_DIGEST_SIZE
#define BACKUP_FILE_HASH_DIGEST_SIZE  HASH_SHA384_DIGEST_SIZE

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
  );

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
  );

#endif
