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
#include <Uefi.h>

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
  )
{
}
