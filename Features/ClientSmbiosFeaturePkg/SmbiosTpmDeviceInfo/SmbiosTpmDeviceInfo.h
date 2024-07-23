/**@file

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

@par Specification
**/

#ifndef _SMBIOS_TPM_DEVICE_INFO_H_
#define _SMBIOS_TPM_DEVICE_INFO_H_

#include <IndustryStandard/SmBios.h>
#include <Guid/TpmInstance.h>

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/Tpm2CommandLib.h>
///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/Smbios.h>

#define EFI_SMBIOS_TYPE_TPM_DEVICE       SMBIOS_TYPE_TPM_DEVICE

#define SMBIOS_TYPE43_NUMBER_OF_STRINGS  1
#define NO_STRING_AVAILABLE              0
#define STRING_1                         1

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED                     0
#define TO_BE_FILLED_STRING              " "        ///< Initial value should not be NULL

#define ASCII_NULL_CHAR                  0x00
#define ASCII_SPACE_CHAR                 0x20

#pragma pack(1)
typedef struct {
  CHAR8 *Description;
} SMBIOS_TYPE43_STRING_ARRAY;
#pragma pack()

typedef enum {
  TpmDeviceNone  = 0,
  TpmDevice12TPM = 1,
  TpmDevice20TPM = 2,
} SUPPORTED_TPM_DEVICE_TYPES;

#endif // _SMBIOS_TPM_DEVICE_INFO_H_
