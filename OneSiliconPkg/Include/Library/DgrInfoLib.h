/** @file
  Header file for DGR Info Lib implementation.

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

#ifndef _DGR_INFO_LIB_H_
#define _DGR_INFO_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#define NO_NR_PPAM_SUPPORT                    0
#define NR_PPAM_11_SUPPORT                    11
#define NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND 12
#define NR_BRAND_IDENTITY_BITS                (BIT0 | BIT1 | BIT2)
#define LEDGER_ISLAND_BRAND_IDENTITY_BITS     BIT4

/**
  Returns PPAM version details to support Nifty Rock feature based on SKU.

  @retval 0    Nifty Rock feature is not supported, No need to load PPAM.
  @retval 11   Nifty Rock feature is supported with PPAM Version 1.1.
  @retval 12   Nifty Rock feature is supported with PPAM Version 1.1 with Ledger Island PTL-Comment:RestrictedContent
**/
UINT8
EFIAPI
NiftyRockSupportLevel (
  VOID
  );
#endif
