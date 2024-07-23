/**@file
    Unit test case mock header for AddFirmwareErrorEntry.

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

#ifndef _ADD_FIRMWARE_ERROR_ENTRY_MOCK_H_
#define _ADD_FIRMWARE_ERROR_ENTRY_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <IndustryStandard/Acpi.h>
#include <Guid/Cper.h>
#include <BertTable.h>

/**

  Add a new Firmware CrashLog Entry to existing Error Status Block.

  @param[in] ErrStsBlk                        BERT GENERIC_ERROR_STATUS instance.
  @param[in,out] FirmwareGenericErrorAddr     Pointer to this FirmwareGenericError entry address, updated on return.
  @param[in,out] FirmwareCrashLogPayloadAddr  Pointer to Firmware CrashLog Entry payload address, updated on return.
  @param[in] EntrySize                        Firmware CrashLog Entry payload size.
  @param[in] EntryVersion                     Firmware CrashLog Entry version.

**/
VOID
AddFirmwareErrorEntry (
  IN     EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk,
  IN OUT FIRMWARE_GENERIC_ERROR                       **FirmwareGenericErrorAddr,
  IN OUT UINT8                                        **FirmwareCrashLogPayloadAddr,
  IN     UINT32                                       EntrySize,
  IN     EFI_GUID                                     RecordIdGuid
  );

/**
  Unit test for AddFirmwareErrorEntry.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddFirmwareErrorEntry (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_ADD_FIRMWARE_ERROR_ENTRY_MOCK_H_*/
