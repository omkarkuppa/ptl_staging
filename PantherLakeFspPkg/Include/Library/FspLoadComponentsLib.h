/** @file
  Fsp loading FSP-M and BspPreMem

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef __FSP_LOAD_COMPONENTS_LIB_H__
#define __FSP_LOAD_COMPONENTS_LIB_H__

#include <Uefi.h>
#include <BspmDef.h>
#include <FbmDef.h>

/**
  Do Pei Core Te section relocation in Fv.

  @param[in]    FvHandle        Pointer to Fv.

  @retval  EFI_SUCCESS                    Relocation success.
           EFI_INVALID_PARAMETER          Relocation fail.

**/
EFI_STATUS
EFIAPI
RebasePeiCoreFfs (
  IN UINTN FvHandle
  );

/**
  Initialize NEM allocation for pre-memory components.
  This function consolidates all NEM allocation calls for pre-memory components
  including FSP-M and BspPreMem regions.

  @param[in]  BsssBaseAddress  Base address of the BSSS structure
  @param[in]  TopOfCar         Top of Cache-as-RAM

  @retval  EFI_SUCCESS             NEM allocation successful.
  @retval  EFI_OUT_OF_RESOURCES    Insufficient NEM resources available.
  @retval  EFI_INVALID_PARAMETER   Invalid parameters provided.

**/
EFI_STATUS
EFIAPI
InitializeNemForPreMemComponents (
  IN UINTN BsssBaseAddress,
  IN UINTN TopOfCar
  );

/**
  Fsp Load Components. When FSP Signing is supported, FBM should be verified as valid
  by ACM only in ACM verified boot flow, Bios should perform below tasks:
  1. Load and verify FSP-M as optional, and halt the system if any error occurs.
  2. Load and verify BSP-PreMem, and halt the system if any error occurs.

**/
VOID
EFIAPI
FspLoadComponents (
  IN UINTN TopOfCar
  );

#endif
