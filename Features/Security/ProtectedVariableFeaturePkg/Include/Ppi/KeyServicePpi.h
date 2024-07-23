/** @file
  Key Service Pei PPI

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

#ifndef _PEI_KEY_SERVICE_PPI_H
#define _PEI_KEY_SERVICE_PPI_H
///
/// KEY SERVICE PPI GUID
///
extern EFI_GUID gKeyServicePpiGuid;

/**
  Generate a new key.

  @param[in]   Salt                     Pointer to the salt(non-secret) value.
  @param[in]   SaltSize                 Salt size in bytes.
  @param[out]  NewKey                   Pointer to buffer to receive new key.
  @param[in]   NewKeySize               Size of new key bytes to generate.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
typedef
EFI_STATUS
(EFIAPI *KEY_SERVICE_GEN_KEY) (
  IN   UINT8        *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *NewKey,
  IN   UINTN        NewKeySize
  );

#define KEY_SERVICE_PPI_REVISION  1
#define ROOT_KEY_LEN              64
#define SALT_SIZE_MIN_LEN         64
#define KEY_SERVICE_KEY_NAME      L"KEY_SERVICE_KEY"

typedef struct {
  UINT8 RootKey[ROOT_KEY_LEN];
  UINT8 PreviousRootKey[ROOT_KEY_LEN];
} KEY_SERVICE_DATA;

typedef struct _KEY_SERVICE_PPI  KEY_SERVICE_PPI;

///
/// KEY SERVICE PPI
/// The interface functions are for Key Service in PEI Phase
///
struct _KEY_SERVICE_PPI {
  KEY_SERVICE_GEN_KEY    Generate256BitKey; /// Generate 256 Bit Key
  KEY_SERVICE_GEN_KEY    Generate512BitKey; /// Generate 512 Bit Key
};
#endif

