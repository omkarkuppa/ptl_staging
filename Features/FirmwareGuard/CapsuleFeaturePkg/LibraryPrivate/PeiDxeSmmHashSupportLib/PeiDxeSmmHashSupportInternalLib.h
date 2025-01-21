/** @file
  Internal library for hash support.

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

#ifndef _HASH_SUPPORT_INTERNAL_DXE_LIB_H_
#define _HASH_SUPPORT_INTERNAL_DXE_LIB_H_

#include <Uefi.h>

#define BYTE_TO_BIT(Size)  (Size * 8)

typedef struct _HASH_SHA_ALG_CONTEXT HASH_SHA_ALG_CONTEXT;

/**
  Get the size of the context buffer required for SHA hash operations.

  @retval  Size of the context buffer in bytes.

**/
typedef
UINTN
(EFIAPI *HASH_SHA_GET_CONTEXT_SIZE)(
  VOID
  );

/**
  Initializes user-supplied memory pointed SHA context.

  @param[out]  ShaContext  Pointer of SHA context to be initialized.
                           If ShaContext is NULL, then return FALSE.

  @retval TRUE   SHA context initialization succeeded.
  @retval FALSE  SHA context initialization failed.

**/
typedef
BOOLEAN
(EFIAPI *HASH_SHA_INIT)(
  OUT VOID  *ShaContext
  );

/**
  Digests the input data and updates SHA context.

  @param[in, out]  ShaContext  Pointer to the SHA context.
                               If ShaContext is NULL, then return FALSE.
  @param[in]       Data        Pointer to the buffer containing the data
                               to be hashed.
  @param[in]       DataSize    Size of Data buffer in bytes.

  @retval  TRUE   SHA data digest succeeded.
  @retval  FALSE  SHA data digest failed.

**/
typedef
BOOLEAN
(EFIAPI *HASH_SHA_UPDATE)(
  IN OUT       VOID   *ShaContext,
  IN     CONST VOID   *Data,
  IN           UINTN  DataSize
  );

/**
  Completes computation of the SHA digest value.

  @param[in, out]  ShaContext  Pointer to the SHA context.
                               If ShaContext is NULL, then return FALSE.
  @param[out]      HashValue   Pointer to a buffer that receives
                               the SHA digest value.
                               If HashValue is NULL, then return FALSE.

  @retval TRUE   SHA digest computation succeeded.
  @retval FALSE  SHA digest computation failed.

**/
typedef
BOOLEAN
(EFIAPI *HASH_SHA_FINAL)(
  IN OUT VOID   *ShaContext,
     OUT UINT8  *HashValue
  );

//
// Private structure of SHA hash algorithm context to be used.
//
struct _HASH_SHA_ALG_CONTEXT {
  UINTN                        HashSize;
  HASH_SHA_GET_CONTEXT_SIZE    HashShaGetContextSize;
  HASH_SHA_INIT                HashShaInit;
  HASH_SHA_UPDATE              HashShaUpdate;
  HASH_SHA_FINAL               HashShaFinal;
};

/**
  Dump the hash digest of data.

  @param[in]  Data      Pointer of data to be dumped.
  @param[in]  DataSize  Size of the data to be dumped.

**/
VOID
DumpHashDigest (
  IN CONST UINT8  *Data,
  IN       UINTN  DataSize
  );

/**
  Check the specific size of SHA digest is supported.

  @param[in]  DigestSize  Size of SHA digest to be used.

  @retval  TRUE   Input size of SHA digest is supported.
  @retval  FALSE  Input size of SHA digest is not supported.

**/
BOOLEAN
IsHashShaDigestSupported (
  IN UINTN  DigestSize
  );

/**
  Get the correspond hash algorithm context by size of digest.

  @param[in]   DigestSize  Size of digest.
  @param[out]  Ctx         Double pointer to SHA hash algorithm context.
                           Set to NULL once there is not matched one.

  @retval  EFI_SUCCESS            Succeed to find the matched context.
  @retval  EFI_INVALID_PARAMETER  Ctx pointer is NULL.
  @retval  EFI_UNSUPPORTED        Expected size of digest is not found.
  @retval  EFI_NOT_FOUND          Failed to find the matched context.

**/
EFI_STATUS
GetHashShaAlgCtx (
  IN     UINTN                 DigestSize,
     OUT HASH_SHA_ALG_CONTEXT  **Ctx
  );

#endif
