/** @file
  PeiProtectedVariableKeyLib implementation.

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


#include <PiPei.h>

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>

#include <Ppi/KeyServicePpi.h>

#define VAR_KEY_SALT       L"Key for RPMC Variable"
#define VAR_KEY_SALT_SIZE  sizeof (VAR_KEY_SALT)
#define KEY_SIZE_256       (256/8)
#define SALT_SIZE_256      (256/8)

/**
  Retrieves the 256-bit key for integrity and/or confidentiality of variables.

  @param[out]   VariableKey         A pointer to the caller allocated variable key buffer.
  @param[in]    VariableKeySize     The size in bytes of the variable key.

  @retval       EFI_SUCCESS             The VariableKey was returned.
  @retval       EFI_INVALID_PARAMETER   The VariableKey pointer is null.
  @retval       EFI_BUFFER_TOO_SMALL    The VariableKeySize is incorrect.
  @retval       EFI_UNSUPPORTED         The variable key is not supported in the current boot configuration.
  @retval       OTHER                   The function completed with failure.
**/
EFI_STATUS
EFIAPI
ProtectedVariableGet256Key (
  OUT UINT8  *VariableKey,
  IN  UINTN  VariableKeySize
  )
{
  EFI_STATUS       Status;
  KEY_SERVICE_PPI  *KeyService;

  if (VariableKey == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (VariableKeySize != KEY_SIZE_256) {
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = PeiServicesLocatePpi (
             &gKeyServicePpiGuid,
             0,
             NULL,
             (void **)&KeyService
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = KeyService->Generate256BitKey (
                         (UINT8 *)VAR_KEY_SALT,
                         VAR_KEY_SALT_SIZE,
                         VariableKey,
                         VariableKeySize
                         );

  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Unable to generate variable key\n", __FUNCTION__));
  }

  return Status;
}

/**
  Generate Random 256-bit Salt.

  @param[in]    Seed         Pointer to seed value.
  @param[in]    SeedSize     Size of seed value.
  @param[out]   Salt         A pointer to caller allocated salt buffer.
  @param[in]    SaltSize     The size in bytes of the salt.

  @retval       EFI_SUCCESS             The Salt was returned.
  @retval       EFI_INVALID_PARAMETER   The Seed or Salt pointers is null.
  @retval       EFI_BUFFER_TOO_SMALL    The Seed or Salt Size is incorrect.
  @retval       EFI_UNSUPPORTED         The variable key is not supported in the current boot configuration.
  @retval       OTHER                   The function completed with failure.
**/
EFI_STATUS
EFIAPI
ProtectedVariableGenerate256Salt (
  IN  UINT8  *Seed,
  IN  UINTN  SeedSize,
  OUT UINT8  *Salt,
  IN  UINTN  SaltSize
  )
{
  EFI_STATUS       Status;
  KEY_SERVICE_PPI  *KeyService;

  if ((Seed == NULL) || (Salt == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((SeedSize == 0) || (SaltSize != SALT_SIZE_256)) {
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = PeiServicesLocatePpi (
             &gKeyServicePpiGuid,
             0,
             NULL,
             (void **)&KeyService
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = KeyService->Generate256BitKey (
                         Seed,
                         SeedSize,
                         Salt,
                         SaltSize
                         );
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Unable to generate salt\n", __FUNCTION__));
  }

  return Status;
}
