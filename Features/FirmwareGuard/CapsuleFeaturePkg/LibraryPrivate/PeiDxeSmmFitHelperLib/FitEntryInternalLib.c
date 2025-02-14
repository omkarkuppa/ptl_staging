/** @file
  Internal library to provide a series of APIs for FIT entry.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// EDK-II Platform Foundation.
//
#include <IndustryStandard/FirmwareInterfaceTable.h>
//
// Module Self Foundation.
//
#include "FitEntryInternalLib.h"

/**
  Check the type of the FIT entry is valid.

  @param[in]  FitEntryType  Type of the FIT entry.

  @retval  TRUE   Input type of the FIT entry is valid.
  @retval  FALSE  Input type of the FIT entry is invalid.

**/
BOOLEAN
IsFitEntryTypeValid (
  IN UINT8  FitEntryType
  )
{
  BOOLEAN  IsValid;

  IsValid = TRUE;

  switch (FitEntryType) {
    case FIT_TYPE_00_HEADER:
    case FIT_TYPE_01_MICROCODE:
    case FIT_TYPE_02_STARTUP_ACM:
    case FIT_TYPE_03_DIAGNOSTIC_ACM:
    case FIT_TYPE_04_PROT_BOOT_POLICY:
    case FIT_TYPE_05_MMC_FIRMWARE_IMAGE:
    case FIT_TYPE_06_FIT_RESET_STATE:
    case FIT_TYPE_07_BIOS_STARTUP_MODULE:
    case FIT_TYPE_08_TPM_POLICY:
    case FIT_TYPE_09_BIOS_POLICY:
    case FIT_TYPE_0A_TXT_POLICY:
    case FIT_TYPE_0B_KEY_MANIFEST:
    case FIT_TYPE_0C_BOOT_POLICY_MANIFEST:
    case FIT_TYPE_0D_FSP_BOOT_MANIFEST:
    case FIT_TYPE_10_CSE_SECURE_BOOT:
    case FIT_TYPE_1A_VAB_PROVISIONING_TABLE:
    case FIT_TYPE_1B_VAB_KEY_MANIFEST:
    case FIT_TYPE_1C_VAB_IMAGE_MANIFEST:
    case FIT_TYPE_1D_VAB_IMAGE_HASH_DESC:
    case FIT_TYPE_2C_SACM_DEBUG:
    case FIT_TYPE_2D_TXTSX_POLICY:
    case FIT_TYPE_2E_GRANULAR_SCRTM_ERROR:
    case FIT_TYPE_2F_JMP_DEBUG_POLICY:
      break;
    default:
      IsValid = FALSE;
  }

  return IsValid;
}

/**
  Get the size field data in FIT entry. (From byte-8 to byte-10)

  @param[in]   FitEntryPtr  Pointer for specific FIT entry.
  @param[out]  SizeValPtr   Pointer to store the size field value.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_SUCCESS            Succeed to get the size field value.

**/
EFI_STATUS
GetFitEntrySizeField (
  IN     CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr,
     OUT       UINT32                          *SizeValPtr
  )
{
  UINT32  SizeField;
  UINT32  Size0;
  UINT32  Size1;
  UINT32  Size2;

  if ((FitEntryPtr == NULL) || (SizeValPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Size0 = (UINT32)FitEntryPtr->Size[0];
  Size1 = (UINT32)FitEntryPtr->Size[1];
  Size2 = (UINT32)FitEntryPtr->Size[2];

  SizeField = ((Size2 << 16) | (Size1 << 8) | Size0) & FIT_ENTRY_SIZE_MASK_VAL;
  CopyMem ((VOID *)SizeValPtr, (VOID *)&SizeField, sizeof (UINT32));

  return EFI_SUCCESS;
}

/**
  Check the FIT entry type 00 is valid.

  @param[in]  FitEntryPtr  Pointer for specific FIT entry.

  @retval  TRUE   Input FIT entry type 00 is valid.
  @retval  FALSE  Input FIT entry type 00 is invalid.

**/
BOOLEAN
IsFitEntryType00Valid (
  IN CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr
  )
{
  if (FitEntryPtr == NULL) {
    return FALSE;
  }

  if (FitEntryPtr->Type != FIT_TYPE_00_HEADER) {
    DEBUG ((DEBUG_ERROR, "Unexpected FIT table header type (0x%x)\n", FitEntryPtr->Type));
    return FALSE;
  }

  if (FitEntryPtr->Address != FIT_TYPE_00_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Unexpected FIT table signature (0x%x)\n", FitEntryPtr->Address));
    return FALSE;
  }

  if (FitEntryPtr->Version != FIT_TYPE_VERSION) {
    DEBUG ((DEBUG_ERROR, "Unexpected FIT table version (0x%x)\n", FitEntryPtr->Version));
    return FALSE;
  }

  return TRUE;
}
