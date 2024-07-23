/**@file

  I2C Common driver file
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

@par Specification
**/

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>


/**
  Parses the Acpi File to return the current table pointer.

  @retval [I2CAcpiTable] I2c Acpi TabLE Pointer
**/
typedef
EFI_ACPI_DESCRIPTION_HEADER *
(EFIAPI *GET_TABLE_ADDR) (
  VOID
 );


struct _TABLE_POINTER_ADDR_PROTOCOL {
  GET_TABLE_ADDR    GetDevTablePointer;
};


typedef struct _TABLE_POINTER_ADDR_PROTOCOL TABLE_POINTER_ADDR_PROTOCOL;


extern EFI_GUID gI2cDeviceProtocolGuid;
