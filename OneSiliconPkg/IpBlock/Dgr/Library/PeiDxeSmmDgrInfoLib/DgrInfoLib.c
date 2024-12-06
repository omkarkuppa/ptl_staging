/** @file
  Source file for DGR Info Lib.

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
#include <Library/DgrInfoLib.h>

/**
  Returns PPAM version details to support DGR with Nifty Rock feature based on SKU.

  @retval 0    DGR with Nifty Rock feature is not supported, No need to load PPAM.
  @retval 11   DGR with Nifty Rock feature is supported with PPAM Version 1.1.
**/
UINT8
EFIAPI
NiftyRockSupportLevel (
  VOID
  )
{
  MSR_PLATFORM_INFO_REGISTER     PlatformInfoMsr;
  CPUID_VERSION_INFO_ECX         Ecx;
  UINT32                         MsrBrandIdentityValue;

  MsrBrandIdentityValue = 0;
  //
  // DGR with NiftyRock supports only if below conditions are met.
  // 1. Brand Identity support shall be present and Brand Identity MSR (address 0xAE) Bits 0,1 and 2 shall be set.
  // 2. Platform Info MSR bit59 (SMM_SUPOVR_STATE_LOCK_ENABLE) shall be Set.
  // 3. SMX shall be supported.
  //
  if (!EFI_ERROR (GetMsrBrandIdentityValue (&MsrBrandIdentityValue))) {
    if ((MsrBrandIdentityValue & NR_BRAND_IDENTITY_BITS) != NR_BRAND_IDENTITY_BITS) {
      DEBUG ((DEBUG_INFO, "NiftyRock not supported. Reason-Brand Identity bits not set.\n"));
      return NO_NR_PPAM_SUPPORT;
    }
  } else {
    DEBUG ((DEBUG_INFO, "NiftyRock not supported. Reason-Brand Identity not supported.\n"));
    return NO_NR_PPAM_SUPPORT;
  }

  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  if (!PlatformInfoMsr.Bits.SmmSupovrStateLockEnable) {
    DEBUG ((DEBUG_INFO, "NiftyRock not supported. Reason-SMM_SUPOVR_STATE_LOCK MSR is not supported.\n"));
    return NO_NR_PPAM_SUPPORT;
  }

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  if (!(Ecx.Bits.SMX == 1)) {
    DEBUG ((DEBUG_INFO, "NiftyRock not supported. Reason-SMX is not supported.\n"));
    return NO_NR_PPAM_SUPPORT;
  }

  return NR_PPAM_11_SUPPORT;
}
