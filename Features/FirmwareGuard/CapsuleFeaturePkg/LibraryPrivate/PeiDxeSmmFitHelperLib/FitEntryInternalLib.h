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

#ifndef _FIT_ENTRY_INTERNAL_H_
#define _FIT_ENTRY_INTERNAL_H_

//
// EDK-II Foundation.
//
#include <Uefi.h>
//
// EDK-II Platform Foundation.
//
#include <IndustryStandard/FirmwareInterfaceTable.h>

#define FIT_ENTRY_SIZE_MASK_VAL       0xFFFFFF
#define FIT_ENTRY_SIZE_MULTIPLE_VAL   16

#define CHECKSUM_DISABLED  0x0
#define CHECKSUM_ENABLED   0x1

/**
  Check the type of the FIT entry is valid.

  @param[in]  FitEntryType  Type of the FIT entry.

  @retval  TRUE   Input type of the FIT entry is valid.
  @retval  FALSE  Input type of the FIT entry is invalid.

**/
BOOLEAN
IsFitEntryTypeValid (
  IN UINT8  FitEntryType
  );

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
  );

/**
  Check the FIT entry type 00 is valid.

  @param[in]  FitEntryPtr  Pointer for specific FIT entry.

  @retval  TRUE   Input FIT entry type 00 is valid.
  @retval  FALSE  Input FIT entry type 00 is invalid.

**/
BOOLEAN
IsFitEntryType00Valid (
  IN CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr
  );

#endif
