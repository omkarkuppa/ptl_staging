/** @file
  BiosGuard Update Package support.

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
#include <Uefi.h>
#include <Library/DebugLib.h>

#include "BgupImageSupport.h"

//
// BiosGuard Update Package Certificate information table.
//
// LenOfCert = RSA Verification key (Modules) + RSA Verification key (Exponent) + Signature
// e.g. (1) RSA2048: LenOfCert = 256 + 4 + 256 = 516
//      (2) RSA3072: LenOfCert = 384 + 4 + 384 = 772
// For more details, please refer the BiosGuard SAS.
//
BGUPC_CERT_INFO  mBgupCertInfoTable[] = {
  {{BGUPC_HDR_VER_1, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_15_SHA256_RSA2048}, 516},
  {{BGUPC_HDR_VER_1, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_21_SHA256_RSA2048}, 516},
  {{BGUPC_HDR_VER_2, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_15_SHA256_RSA3072}, 772},
  {{BGUPC_HDR_VER_2, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_21_SHA256_RSA3072}, 772},
  {{BGUPC_HDR_VER_3, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_15_SHA384_RSA3072}, 772},
  {{BGUPC_HDR_VER_3, BGUPC_RESERVED_DEFAULT_VALUE, BGUPC_ALG_PKCS1_21_SHA384_RSA3072}, 772},
  };

/**
  Check BiosGuard Update Package Certificate buffer is valid.

  @param[in] BgupCHeaderPtr   Pointer point to the input BGUPC buffer.
                              Buffer is included the header and body parts.
  @param[in] BgupCBufferSize  Size of the BGUPC buffer.

  @retval TRUE   Input BGUPC buffer is valid.
  @retval FALSE  Input BGUPC buffer is not valid.

**/
BOOLEAN
IsBgupCValid (
  IN BGUPC_HEADER  *BgupCHeaderPtr,
  IN UINTN         BgupCBufferSize
  )
{
  UINTN    Idx;
  BOOLEAN  IsFound;

  Idx     = 0;
  IsFound = FALSE;

  if (BgupCBufferSize < sizeof (BGUPC_HEADER)) {
    DEBUG ((DEBUG_ERROR, "BGUPC buffer size smaller than BGUPC header size.\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "BGUPC header Version  :0x%x\n", BgupCHeaderPtr->Version));
  DEBUG ((DEBUG_INFO, "BGUPC header Reserved :0x%x\n", BgupCHeaderPtr->Reserved));
  DEBUG ((DEBUG_INFO, "BGUPC header Algorithm:0x%x\n", BgupCHeaderPtr->Algorithm));

  //
  // Iteration to find the correspond BGUPC information based on BGUPC header.
  //
  for (Idx = 0; Idx < (sizeof (mBgupCertInfoTable) / sizeof (BGUPC_CERT_INFO)); Idx++) {
    if ((mBgupCertInfoTable[Idx].BgupCHdr.Version) != (BgupCHeaderPtr->Version)) {
      continue;
    }

    if ((mBgupCertInfoTable[Idx].BgupCHdr.Reserved) != (BgupCHeaderPtr->Reserved)) {
      continue;
    }

    if ((mBgupCertInfoTable[Idx].BgupCHdr.Algorithm) != (BgupCHeaderPtr->Algorithm)) {
      continue;
    }

    IsFound = TRUE;
    DEBUG ((DEBUG_INFO, "Find the matched BGUPC info in table on index: [%d]\n", Idx));
    break;
  }

  if (!IsFound) {
    DEBUG ((DEBUG_ERROR, "Not found the correspond BGUPC information.\n"));
    return FALSE;
  }

  //
  // Post-Check the BGUPC buffer size is valid.
  //
  if ((sizeof (BGUPC_HEADER) + mBgupCertInfoTable[Idx].LenOfCert) != BgupCBufferSize) {
    DEBUG ((
      DEBUG_ERROR,
      "BGUPC buffer size is not matched. (Expected: 0x%x - Actual: 0x%x)\n",
      (sizeof (BGUPC_HEADER) + mBgupCertInfoTable[Idx].LenOfCert),
      BgupCBufferSize
      ));
    return FALSE;
  }

  return TRUE;
}

/**
  Decompose the BGUP image delivered within the buffer.

  Note:
  - BGUP image = BGUP (Header+Body) and BGUPC (Header+Body).
  +--------------+
  | BGUP  Header |
  +--------------+
  |  BGUP  Body  |
  +--------------+
  | BGUPC Header |
  + -------------+
  |  BGUPC Body  |
  +--------------+

  @param[in]      BgupImagePtr     Pointer point to BGUP image buffer.
  @param[in]      BgupImageSize    Size of full BGUP image.
  @param[in, out] BgupHeaderPtr    Pointer point to BGUP buffer.
  @param[in, out] BgupBufferSize   Size of BGUP buffer part.
  @param[in, out] BgupCHeaderPtr   Pinter point to BGUPC buffer.
  @param[in, out] BgupCBufferSize  Size of BGUPC buffer part.

  @retval EFI_SUCCESS            Succeed to decompose the BGUP image buffer.
  @retval EFI_INVALID_PARAMETER  Failed to decompose the BGUP image buffer.

**/
EFI_STATUS
DecomposeBgupImage (
  IN     UINT8         *BgupImagePtr,
  IN     UINTN         BgupImageSize,
  IN OUT BGUP_HEADER   **BgupHeaderPtr,  OPTIONAL
  IN OUT UINTN         *BgupBufferSize,  OPTIONAL
  IN OUT BGUPC_HEADER  **BgupCHeaderPtr, OPTIONAL
  IN OUT UINTN         *BgupCBufferSize  OPTIONAL
  )
{
  BGUP_HEADER   *BgupPtr;
  UINTN         BgslSize;
  UINTN         BgupSize;
  BGUPC_HEADER  *BgupCPtr;
  UINTN         BgupCSize;
  BOOLEAN       IsValid;

  BgupPtr   = NULL;
  BgslSize  = 0;
  BgupSize  = 0;
  BgupCPtr  = NULL;
  BgupCSize = 0;
  IsValid   = FALSE;

  if (BgupImagePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (BgupImageSize < sizeof (BGUP_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the BiosGuard Update Package information.
  //
  BgupPtr  = (BGUP_HEADER *) BgupImagePtr;
  BgslSize = (UINTN) (BgupPtr->ScriptSectionSize);
  BgupSize = sizeof (BGUP_HEADER) + BgslSize;
  if (BgupSize >= BgupImageSize) {
    //
    // Assume the BGUPC must exist, if not exist consider the buffer is invalid.
    // Return as EFI_INVALID_PARAMETER.
    //
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the BiosGuard Update Package Certificate information.
  //
  BgupCPtr  = (BGUPC_HEADER *) (BgupImagePtr + BgupSize);
  BgupCSize = BgupImageSize - BgupSize;
  IsValid   = IsBgupCValid (BgupCPtr, BgupCSize);
  if (!IsValid) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Set the BGUP and BGUPC pointer and size information.
  //
  if (BgupHeaderPtr != NULL) {
    *BgupHeaderPtr   = BgupPtr;
  }
  if (BgupBufferSize != NULL) {
    *BgupBufferSize  = BgupSize;
  }
  if (BgupCHeaderPtr != NULL) {
    *BgupCHeaderPtr  = BgupCPtr;
  }
  if (BgupCBufferSize != NULL) {
    *BgupCBufferSize = BgupCSize;
  }

  return EFI_SUCCESS;
}
