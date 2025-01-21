/** @file
  This file contains functions for hash calculation.

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

#include <BpmDef.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseCryptLib.h>

#define  BIT_HASHED_IBB           0x00000001    //0: Hashed IBB    1:Non-Hashed IBB
#define  IBB_SEGMENT_COUNT_MAX    8

/**
  Calculate the SHA-384 message digest for one or more Ibb trunks, and compare with the input digest
  value.

  @param[in]   HashCtx            Memory buffer to conatin hash algorithm context, must not be NULL.
  @param[in]   Sha384DigestPtr    Sha384 hash structure which contains the digest to compare.
  @param[in]   ImageBase          An optional field to indicate the base of Ibb segments, should
                                  be 0 if IbbSegments are already described with absolute addresses.
  @param[in]   IbbSegmentCount    A positive number to indicate how many segments are described in
                                  Ibb segments array.
  @param[in]   IbbSegments        Ibb segments to describe Base and Size information for hash calculation.
                                  Only hashed IBB regions indicated by flag will be calculated.

  @retval TRUE   SHA-384 digest calcuated at runtime matches to the one from input.
  @retval FALSE  SHA-384 digest calcuated at runtime NOT matches to the one from input or invalid parameters.

**/
BOOLEAN
Sha384Verify (
  IN VOID                     *HashCtx,
  IN SHA384_HASH_STRUCTURE    *Sha384DigestPtr,
  IN UINTN                    ImageBase,
  IN UINTN                    IbbSegmentCount,
  IN REGION_SEGMENT           *IbbSegments
  ) {

  BOOLEAN                  CryptoStatus;
  UINTN                    Base;
  UINTN                    Size;
  UINTN                    Index;
  SHA384_HASH_STRUCTURE    Sha384DigestRuntime;

 if (HashCtx == NULL || Sha384DigestPtr == NULL) {
    ASSERT (FALSE);
    return FALSE;
  }

  if (IbbSegmentCount == 0 || IbbSegmentCount > IBB_SEGMENT_COUNT_MAX || IbbSegments == NULL) {
    ASSERT (FALSE);
    return FALSE;
  }

  CryptoStatus = Sha384Init (HashCtx);
  if (!CryptoStatus) {
    return FALSE;
  }

  for (Index = 0; Index < IbbSegmentCount; Index ++) {

    if (IbbSegments[Index].Size != 0 && (IbbSegments[Index].Flags & BIT_HASHED_IBB) == 0) {
      Base = ImageBase + IbbSegments[Index].Base;
      Size = IbbSegments[Index].Size;
      DEBUG ((DEBUG_INFO, "[%d] Base: %x\n", Index, Base));
      DEBUG ((DEBUG_INFO, "[%d] Size: %x\n", Index, Size));
      CryptoStatus = Sha384Update (
                       HashCtx,
                       (CONST VOID*) Base,
                       Size
                       );
      if (!CryptoStatus) {
        return FALSE;
      }
    } else if (IbbSegments[Index].Size == 0) {
      break;
    }
  }

  CryptoStatus = Sha384Final (
                  HashCtx,
                  Sha384DigestRuntime.HashBuffer.digest8
                  );
  if (!CryptoStatus) {
    return FALSE;
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "Digest (Manifest):\n"));
  for (Index = 0; Index < SHA384_DIGEST_SIZE; Index ++) {
    DEBUG ((DEBUG_INFO, "%x ", Sha384DigestPtr->HashBuffer.digest8[Index]));
    if ((Index + 1) % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }

  DEBUG ((DEBUG_INFO, "Digest (RT):\n"));
  for (Index = 0; Index < SHA384_DIGEST_SIZE; Index ++) {
    DEBUG ((DEBUG_INFO, "%x ", Sha384DigestRuntime.HashBuffer.digest8[Index]));
    if ((Index + 1) % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }
  DEBUG_CODE_END ();

  if (CompareMem (Sha384DigestRuntime.HashBuffer.digest8, Sha384DigestPtr->HashBuffer.digest8, SHA384_DIGEST_SIZE) == 0) {
    return TRUE;
  }

  return FALSE;
}
