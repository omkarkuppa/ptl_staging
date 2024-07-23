/** @file
  Library to provide the generic function to operate on FIT table.

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

#ifndef _FIT_HELPER_LIB_H_
#define _FIT_HELPER_LIB_H_

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Register/Cpuid.h>
//
// EDK-II Platform Foundation.
//
#include <IndustryStandard/FirmwareInterfaceTable.h>

/**
  Check if FIT entry is resident in firmware address range.

  Note:
    (1) Input of address should be converted into memory range.
    (2) FIT table is in the range (4GB - 16MB) to (4GB - 40h).

  @param[in]  FitEntryAddress  FIT entry address mapping to memory.

  @retval  TRUE   FIT entry is resident in valid range.
  @retval  FALSE  FIT entry is not resident in valid range.

**/
BOOLEAN
EFIAPI
IsFitEntryAddressValid (
  IN EFI_PHYSICAL_ADDRESS  FitEntryAddress
  );

/**
  Get the FIT pointer address from input BIOS buffer.

  @param[in]   BiosBufferPtr   Pointer to BIOS image buffer in memory.
  @param[in]   BiosBufferSize  Size of BIOS image buffer.
  @param[out]  FitPtr          Double pointer to point to FIT table.

  @retval  EFI_SUCCESS            Succeed to get the FIT pointer from
                                  input BIOS buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetFitPointer (
  IN     CONST UINT8  *BiosBufferPtr,
  IN           UINTN  BiosBufferSize,
     OUT       VOID   **FitPtr
  );

/**
  Get the FIT entries buffer by type.

  As per the FIT specification, the order of the records should be in
  ascending order of their type attribute in FIT, i.e., the entries
  with same type are continuous.

  @param[in]       BiosBufferPtr   Pointer to BIOS image buffer in
                                   memory.
  @param[in]       BiosBufferSize  Size of BIOS image buffer.
  @param[in]       FitEntryType    Type of FIT entry.
  @param[in, out]  DataSizePtr     Pointer to size of buffer to place.
  @param[out]      DataBufferPtr   Double pointer to the specific
                                   address inside the BiosBufferPtr.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_BUFFER_TOO_SMALL   Caller provided the NULL pointer
                                  to store the address of first entry.
  @retval  EFI_NOT_FOUND          Assigned type of entry is not found.
  @retval  EFI_SUCCESS            Succeed to get the entries buffer by
                                  assigned type of entry.

**/
EFI_STATUS
EFIAPI
GetFitEntryBufferByType (
  IN     CONST UINT8  *BiosBufferPtr,
  IN           UINTN  BiosBufferSize,
  IN           UINT8  FitEntryType,
  IN OUT       UINTN  *DataSizePtr,
     OUT       VOID   **DataBufferPtr  OPTIONAL
  );

/**
  Get the processor FMS and FMS mask from type 2 Ver. 0x200 FIT entry.

  @param[in]   FitEntryPtr          Pointer for specific FIT entry.
  @param[out]  ProcessorFmsPtr      Pointer for processor FMS value.
  @param[out]  ProcessorFmsMaskPtr  Pointer for processor FMS mask value.

  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_SUCCESS            Succeed to get the both of FMS and
                                  FMS mask value.

**/
EFI_STATUS
EFIAPI
GetFitType2EntryVer200Info (
  IN     CONST FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntryPtr,
     OUT       CPUID_VERSION_INFO_EAX          *ProcessorFmsPtr,
     OUT       CPUID_VERSION_INFO_EAX          *ProcessorFmsMaskPtr
  );

#endif
