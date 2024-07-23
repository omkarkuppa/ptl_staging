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

//
// FIT Entry type definitions
//
#define FIT_TYPE_03_DIAGNOSTIC_ACM                  0x03
#define FIT_TYPE_06_FIT_RESET_STATE                 0x06
#define FIT_TYPE_1A_VENDOR_AUTH_BOOT_PROV_TBL       0x1A
#define FIT_TYPE_1B_VENDOR_AUTH_BOOT_KM             0x1B
#define FIT_TYPE_1C_VENDOR_AUTH_BOOT_IMG_MANIFEST   0x1C
#define FIT_TYPE_1D_VENDOR_AUTH_BOOT_IMG_HASH_DESC  0x1D
#define FIT_TYPE_2C_SACM_DEBUG                      0x2C
#define FIT_TYPE_2E_GRANULAR_SCRTM_ERROR            0x2E

#define FIT_ENTRY_SIZE_MASK_VAL       0xFFFFFF
#define FIT_ENTRY_SIZE_MULTIPLE_VAL   16
#define FIT_ENTRY_TYPE_2_VERSION_200  0x0200

#define CHECKSUM_DISABLED  0x0
#define CHECKSUM_ENABLED   0x1

typedef struct {
  UINT64    Address;
  UINT8     Model             : 4;
  UINT8     Family            : 4;
  UINT8     ProcessorType     : 4;
  UINT8     ExtModel          : 4;
  UINT8     ModelMask         : 4;
  UINT8     FamilyMask        : 4;
  UINT8     ProcessorTypeMask : 4;
  UINT8     ExtModelMask      : 4;
  UINT16    Version;
  UINT8     Type              : 7;
  UINT8     C_V               : 1;
  UINT8     ExtFamily         : 4;
  UINT8     ExtFamilyMask     : 4;
} FIT_TYPE2_VER_200_ENTRY;

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
