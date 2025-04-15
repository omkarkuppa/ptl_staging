/** @file
  This file contains functions to support FSP verification operations.

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

#ifndef __FSP_VERIFICATION_LIB_H__
#define __FSP_VERIFICATION_LIB_H__

#include <Uefi.h>
#include <BspmDef.h>
#include <FbmDef.h>

#define MSR_BOOT_GUARD_SACM_INFO             0x0000013A
#define B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS   BIT3
#define HASH_CTX_LEN_MAX                     256
#define BOOT_GUARD_PROFILE_3                 3
#define BOOT_GUARD_PROFILE_4                 4
#define BOOT_GUARD_PROFILE_5                 5

/**
  Find FSP header pointer.

  @param[in] FlashFvFspBase Flash address of FSP FV.

  @return FSP header pointer.
**/
FSP_INFO_HEADER *
EFIAPI
FspFindFspHeader (
  IN EFI_PHYSICAL_ADDRESS  FlashFvFspBase
  );

/**
  Get FSP Version information.

  @param[in]   Bspm        BSPM structure found in BPM.
  @param[out]  FspVersion  Holds the FSP version.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
GetFspVersion (
  IN BSPM_ELEMENT         *Bspm,
  OUT UINT8               FspVersion []
  );

/**
  Detect the boot guard profile.

  @retval BootGuardProfile  Boot Guard Profile.

**/
UINT8
DetectBootGuardProfile (
  VOID
  );

/**
  Check if FSP signing is supported.

  @param[in]   Fbm    FSP Boot Manifest which keeps FSP-M digest and IBB information.

  @retval TRUE   Signing is supported.
  @retval FALSE  Signing is not supported.

**/
UINT8
IsSigningSupported (
  IN FSP_BOOT_MANIFEST_STRUCTURE  *Fbm
  );

/**
  Verify FSP Version information in BSSS(BSPM) and FBM.
  FSP Version digest information is kept in FBM, FSP will only verify the SHA384 digest.


  @param[in]   Bsss      BSSS(BSPM) structure found in BPM.
  @param[in]   Fbm       FSP Boot Manifest which keeps FSP-M digest and IBB information.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyFspVersion (
  IN BSPM_ELEMENT                   *Bsss,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

/**
  Verify FSP-M with the information in BSSS(BSPM) and FBM.
  FSP-M digest information is kept in FBM, FSP will only verify the SHA384 digest.
  FSP-M IBB region information is kept in FBM, only hashed IBB (indicate by flag) should be taken
  into digest calculation. FSP-M IBB region in FBM are relative offset since FSP-M might be relocated
  in OEM bios integration. OEM should provide FSP-M image base information via BSSS(BSPM).

  @param[in]   Bsss      BSSS(BSPM) structure found in BPM.
  @param[in]   Fbm       FSP Boot Manifest which keeps FSP-M digest and IBB information.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyAndExtendFspm (
  IN BSPM_ELEMENT                   *Bsss,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

/**
  Verify FSP-S with the information in FBM.
  FSP-S digest information is kept in FBM, FSP will only verify the SHA384 digest.
  FSP-S IBB region information is kept in FBM, only hashed IBB (indicate by flag, follow the same way
  to describe region should be calculated in digest or not) should be taken into digest calculation.
  FSP-S IBB region in FBM are relative offset since FSP-S will be executed in memory. OEM should provide
  FSP-S image base in memory.

  @param[in]   FspsImageBase      FSP-S image base in memory.
  @param[in]   Fbm                FSP Boot Manifest which keeps FSP-S digest and IBB information.
  @param[in]   Buffer             Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyAndLogEventFsps (
  IN UINTN                          FspsImageBase,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

/**
  Verify and extend BSP-PreMem with the information in BSPM.
  BSP-PreMem digest information is kept in BSPM, only one TPM required algorithm (SHA256, SHA384,
  SHA512, SM3) is needed for size consideration.

  BSP-PreMem region information is kept in BSPM, only hashed IBB (indicate by flag) should be taken
  into digest calculation.

  @param[in]   Bspm      BSPM structure found in BPM.
  @param[in]   Buffer    Memory buffer for hash verification.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifyBsp (
  IN BSPM_ELEMENT                   *Bspm,
  IN VOID                           *Buffer
  );

//
// FSP_VERIFY_API_WRAPPER is defined to allow crypto service share in FSP binaries,
// which helps reduce FSP size a great deal.
// Only one driver should link to the real crypto service and other drivers can define
// FSP_VERIFY_API_WRAPPER structure and patch function pointers in FSP post build. The
// tool can scan FSP binary to get the real crypto service entry via specific signature.
//
#define FSP_VERIFY_API_WRAPPER_STRUCTURE_ID  "__FVA__"
typedef struct _FSP_VERIFY_API_WRAPPER    FSP_VERIFY_API_WRAPPER;

typedef
EFI_STATUS
(EFIAPI *VERIFY_FSPM_API_WRAPPER) (
  IN BSPM_ELEMENT                   *Bsss,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *VERIFY_FSPS_API_WRAPPER) (
  IN UINTN                          FspsImageBase,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *VERIFY_BSP_API_WRAPPER) (
  IN BSPM_ELEMENT                   *Bsss,
  IN VOID                           *Buffer
  );

typedef
EFI_STATUS
(EFIAPI *VERIFY_FSPVERSION_API_WRAPPER) (
  IN BSPM_ELEMENT                   *Bsss,
  IN FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN VOID                           *Buffer
  );

struct _FSP_VERIFY_API_WRAPPER {
  CHAR8                              Signature[8];  // Must be FSP Verification API Signature "__FVA__"
  VERIFY_FSPM_API_WRAPPER            VerifyFspmApiWrapper;
  VERIFY_FSPS_API_WRAPPER            VerifyFspsApiWrapper;
  VERIFY_BSP_API_WRAPPER             VerifyBspApiWrapper;
  VERIFY_FSPVERSION_API_WRAPPER      VerifyFspVersionApiWrapper;
};

#endif
