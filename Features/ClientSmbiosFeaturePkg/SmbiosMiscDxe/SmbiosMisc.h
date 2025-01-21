/** @file
  Header file for the SmbiosMisc Driver.

  @par Revision Reference:
  SMBIOS Specification from DMTF: http://www.dmtf.org/standards/smbios
  Intel Framework Specifications, all available at: http://www.intel.com/technology/framework/spec.htm
  Unified Extensible Firmware Interface (UEFI) Specifications: http://www.uefi.org/specs/

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _SMBIOS_MISC_DRIVER_H
#define _SMBIOS_MISC_DRIVER_H

#include <Uefi.h>
#include <Protocol/Smbios.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>

#define MISC_MAX_SMBIOS_TABLE_STRINGS   10
#define MAX_ADDRESS_32BIT               0xFFFFFFFF
#define T14_FVI_STRING                  "Firmware Version Info"

//
// Hardware Security - SMBIOS Type 24
//
typedef enum {
  EfiHardwareSecurityStatusDisabled       = 0,
  EfiHardwareSecurityStatusEnabled        = 1,
  EfiHardwareSecurityStatusNotImplemented = 2,
  EfiHardwareSecurityStatusUnknown        = 3
} EFI_MISC_HARDWARE_SECURITY_STATUS;

/**
  The Generic Smbios Function to handle all SMBIOS records.

  @param[in]  *RecordData               Pointer to copy of RecordData from the Data Table.
  @param[in]  *RecordStrings            Pointer to copy of RecordStrings from the Data Table.
  @param[in]  StringCount               The counter of String.
  @param[in]  NeedToFreePoolMem         Check the Pool if need to free.
  @param[in]  MaxInstances              Number of Instance.
  @param[in]  Smbios                    Pointer to SmbiosProtocol.

  @retval     EFI_SUCCESS               All parameters were valid.
  @retval     EFI_UNSUPPORTED           Unexpected RecordType value.
  @retval     EFI_INVALID_PARAMETER     Invalid parameter was found.

**/
EFI_STATUS
MiscCommonSmbiosFunction (
  IN  VOID                    *RecordData,
  IN  VOID                    *RecordStrings,
  IN  UINTN                   StringCount,
  IN  BOOLEAN                 NeedToFreePoolMem,
  IN  UINTN                   MaxInstances,
  IN  EFI_SMBIOS_PROTOCOL     *Smbios
  );

/**
  Add an SMBIOS record.

  @param[in]  Smbios                    The EFI_SMBIOS_PROTOCOL instance.
  @param[out] SmbiosHandle              A unique handle will be assigned to the SMBIOS record.
  @param[in]  Record                    The data for the fixed portion of the SMBIOS record. The format of the record is
                                        determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined
                                        by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or
                                        a set of null terminated strings and a null.

  @retval EFI_SUCCESS                   Record was added.
  @retval EFI_OUT_OF_RESOURCES          Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN  EFI_SMBIOS_PROTOCOL     *Smbios,
  OUT EFI_SMBIOS_HANDLE       *SmbiosHandle,
  IN  EFI_SMBIOS_TABLE_HEADER *Record
  );

/**
  Initialize SMBIOS table strings.

  @param[out] **Destination             The pointer for the destination.
  @param[in]  *Source                   The pointer for the source date.
  @param[out] **StrBuffer               The pointer for string buffer.
  @param[out] *Count                    The accumulated number of strings.
  @param[out] *SizeOfStrings            The accumulated number of strings length.
  @param[in, out]  *StrBuffSize         Size of StrBuffer.

  @retval     EFI_SUCCESS               Successful.
  @retval     EFI_INVALID_PARAMETER     Distination pointer is not NULL.
  @retval     EFI_BUFFER_TOO_SMALL      StrBuffSize is zero.

**/
EFI_STATUS
SmbiosStrInit (
  OUT CHAR16        **Destination,
  IN  CHAR16  CONST *Source,
  OUT CHAR16        **StrBuffer,
  OUT UINTN         *Count,
  OUT UINTN         *SizeOfStrings,
  IN  OUT  UINTN    *StrBuffSize
  );

/**
  Update SMBIOS strings.

  @param[in]  **StringArray             The strings to be updated.
  @param[in]  *StarAddr                 The pointer of strat location.
  @param[in]  NumOfStrings              The number of strings.
  @param[in]  BufferSize                The usage size from start location, *StarAddr.

**/
VOID
SmbiosStringsUpdate (
  IN  CHAR16            **StringArray,
  IN  UINT8             *StartAddr,
  IN  UINTN             NumOfStrings,
  IN  UINTN             BufferSize
  );

/**
  Create Dynamic SMBIOS Table and store data.

  @param[in] Event                      Event whose notification function is being invoked (Ready To Boot).
  @param[in] Context                    Pointer to the notification functions context, which is implementation dependent.

**/
VOID
EFIAPI
CreateDynamicSmbiosTable (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  );

#endif
