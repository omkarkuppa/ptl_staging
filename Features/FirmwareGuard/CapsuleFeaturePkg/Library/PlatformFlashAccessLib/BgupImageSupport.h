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
#ifndef _BGUP_IMAGE_SUPPORT_H_
#define _BGUP_IMAGE_SUPPORT_H_

#include <Uefi.h>
#include <BiosGuard.h>

#define BGUPC_RESERVED_DEFAULT_VALUE  0x0000

#define BGUPC_HDR_VER_1                    0x0001
//
// PKCS1 1.5 digital signature (SHA-256 hash, RSA 2048 key)
//
#define BGUPC_ALG_PKCS1_15_SHA256_RSA2048  0x00000001
//
// PKCS1 v2.1 EMSA/PSS digital signature (SHA-256 hash, RSA 2048 key)
//
#define BGUPC_ALG_PKCS1_21_SHA256_RSA2048  0x00000002

#define BGUPC_HDR_VER_2                    0x0002
//
// PKCS1 1.5 digital signature (SHA-256 hash, RSA 3072 key)
//
#define BGUPC_ALG_PKCS1_15_SHA256_RSA3072  0x00000003
//
// PKCS1 v2.1 EMSA/PSS digital signature (SHA-256 hash, RSA 3072 key)
//
#define BGUPC_ALG_PKCS1_21_SHA256_RSA3072  0x00000004

#define BGUPC_HDR_VER_3                    0x0003
//
// PKCS1 1.5 digital signature (SHA-384 hash, RSA 3072 key)
//
#define BGUPC_ALG_PKCS1_15_SHA384_RSA3072  0x00000005
//
// PKCS1 v2.1 EMSA/PSS digital signature (SHA-384 hash, RSA 3072 key)
//
#define BGUPC_ALG_PKCS1_21_SHA384_RSA3072  0x00000006

typedef struct {
  UINT16  Version;
  UINT16  Reserved;
  UINT32  Algorithm;
} BGUPC_HEADER;

typedef struct {
  BGUPC_HEADER  BgupCHdr;
  UINT64        LenOfCert;
} BGUPC_CERT_INFO;

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
  );

#endif
