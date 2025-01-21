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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/BaseCryptLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
//
// Advanced Feature Package Foundation.
//
#include <Library/HashSupportLib.h>
//
// Module Self Foundation.
//
#include "PeiDxeSmmHashSupportInternalLib.h"

//
// Table of SHA hash algorithm context.
//
HASH_SHA_ALG_CONTEXT  mHashShaAlgTable[] = {
#if (FixedPcdGetBool (PcdSha256DigestSupport) == 1)
  { HASH_SHA256_DIGEST_SIZE, Sha256GetContextSize, Sha256Init, Sha256Update, Sha256Final },
#endif
  { HASH_SHA384_DIGEST_SIZE, Sha384GetContextSize, Sha384Init, Sha384Update, Sha384Final },
#if (FixedPcdGetBool (PcdSha512DigestSupport) == 1)
  { HASH_SHA512_DIGEST_SIZE, Sha512GetContextSize, Sha512Init, Sha512Update, Sha512Final },
#endif
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
  )
{
  DEBUG_CODE_BEGIN ();

  UINTN  Idx;

  Idx = 0;

  for (Idx = 0; Idx < DataSize; Idx++) {
    if (Idx % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08X:", Idx));
    }

    DEBUG ((DEBUG_INFO, " %02X", *Data++));
  }

  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG_CODE_END ();
}

/**
  Check the specific size of SHA digest is supported.

  @param[in]  DigestSize  Size of SHA digest to be used.

  @retval  TRUE   Input size of SHA digest is supported.
  @retval  FALSE  Input size of SHA digest is not supported.

**/
BOOLEAN
IsHashShaDigestSupported (
  IN UINTN  DigestSize
  )
{
  UINTN  Idx;

  Idx = 0;

  for (Idx = 0; Idx < (sizeof (mHashShaAlgTable) / sizeof (HASH_SHA_ALG_CONTEXT)); Idx++) {
    if (mHashShaAlgTable[Idx].HashSize == DigestSize) {
      return TRUE;
    }
  }

  return FALSE;
}

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
  )
{
  UINTN  Idx;

  Idx = 0;

  if (Ctx == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsHashShaDigestSupported (DigestSize)) {
    return EFI_UNSUPPORTED;
  }

  *Ctx = NULL;

  for (Idx = 0; Idx < (sizeof (mHashShaAlgTable) / sizeof (HASH_SHA_ALG_CONTEXT)); Idx++) {
    if (mHashShaAlgTable[Idx].HashSize == DigestSize) {
      *Ctx = &(mHashShaAlgTable[Idx]);
      DEBUG ((DEBUG_INFO, "Get the SHA-%d algorithm in index [%d]\n", BYTE_TO_BIT (DigestSize), Idx));
      return EFI_SUCCESS;
    }
  }

  //
  // Expected we should not touch here.
  //
  return EFI_NOT_FOUND;
}
