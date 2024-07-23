/** @file
  Key Service header file.

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

#ifndef _KEY_SERVICE_H_
#define _KEY_SERVICE_H_

#include <PiPei.h>

#pragma pack(push, 1)

//
// Structure to hold Trusted Channel Key and its Ephermeral Key
//
typedef struct {
  UINT8  TrustedKey[64];
  UINT32 EphemeralKey;
} KEY_SERVICE_TRUSTED_DATA;

///
/// Function Declarations
///

/**
  Generate a 256 bit key.

  @param[in]   Salt                     Pointer to the salt (non-secret) value.
  @param[in]   SaltSize                 Salt size in bytes.  Must be less than 384-bits.
  @param[out]  NewKey                   Pointer to buffer to receive new key.
  @param[in]   NewKeySize               Size of new key bytes to generate.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
EFI_STATUS
EFIAPI
Generate256BitKey (
  IN   UINT8        *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *NewKey,
  IN   UINTN        NewKeySize
  );


/**
  Generate a 512 bit key.

  @param[in]   Salt                     Pointer to the salt (non-secret) value.
  @param[in]   SaltSize                 Salt size in bytes.  Must be less than 384-bits.
  @param[out]  NewKey                   Pointer to buffer to receive new key.
  @param[in]   NewKeySize               Size of new key bytes to generate.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
EFI_STATUS
EFIAPI
Generate512BitKey (
  IN   UINT8        *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *NewKey,
  IN   UINTN        NewKeySize
  );

#pragma pack(pop)

#endif
