/** @file
  TSE driver for initializing ACPI tables

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

/**
   This function patches and installs ACPI tables used for handling TSE

   @param[in] ImageHandle  The firmware allocated handle for the EFI image.

   @retval EFI_SUCCESS     The entry point is executed successfully.
   @retval Others          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
InitializeTseAcpi (
  IN EFI_HANDLE        ImageHandle
  )
{
  return EFI_UNSUPPORTED;
}

/**
   Install TSE Global NVS protocol

   @param[in]  ImageHandle  Image Handle.

   @retval   EFI_SUCCESS     The TSE ACPI table is published successfully.
   @retval   Others          The TSE ACPI table is not published.
**/
EFI_STATUS
InstallTseNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  return EFI_UNSUPPORTED;
}

/**
   Patch TSE Nvs Area region.

   @retval EFI_SUCCESS    initialized successfully
   @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchTseNvsAreaAddress (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}
