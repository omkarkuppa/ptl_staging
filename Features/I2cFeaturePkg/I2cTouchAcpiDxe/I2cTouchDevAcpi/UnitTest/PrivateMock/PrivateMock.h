/**@file
  Private mock for the I2c Touch Device Acpi Driver

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

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <Uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <I2cDevices.h>
#include <I2cCommon.h>

extern EFI_ACPI_DESCRIPTION_HEADER  *gI2cRtd3AcpiTable;
extern I2C_ACPI_DEV                 gMockI2cControllerInfo[MAX_I2C_ROOTPORT_ENTRY_SUPPORT];
extern EFI_ACPI_DESCRIPTION_HEADER  *gI2cDevicesAcpiTable;
extern I2C_DEVICES                  *gMockI2cTPTD;

/**
  Mock for GetDevTablePointer.

  @retval [I2CAcpiTable] I2c Acpi TabLE Pointer
**/
EFI_ACPI_DESCRIPTION_HEADER *
EFIAPI
MockGetDevTablePointer (
  VOID
  );

/**
  Get ACPI Table raw data by file path.

  @param[out] FileData          ACPI Table RAW data
  @param[out] FileSize          ACPI Table size

  @retval EFI_SUCCESS           Get ACPI table successfully.
  @retval EFI_OUT_OF_RESOURCES  Don't have any resource to allocate memory.
  @retval EFI_NOT_FOUND         File path not found.

**/
EFI_STATUS
EFIAPI
MockI2cAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  );

#endif  // _PRIVATE_MOCK_H_
