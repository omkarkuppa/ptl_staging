/** @file
  Unit test case of the ExportPsrToUsbStorage function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Protocol/PsrProtocol.h>

/**
  Export PSR to selected Usb Storage.

  @param[in] *This              Pointer of PLATFORM_SERVICE_RECORD_PROTOCOL.
  @param[in] ExportHandle       BlockIO handle of the selected device.

  @retval    EFI_SUCCESS        Export PSR data to specific USB device successfully.
  @retval    Others             Fail to export PSR data to specific USB device.

**/
EFI_STATUS
EFIAPI
ExportPsrToUsbStorage (
  IN PLATFORM_SERVICE_RECORD_PROTOCOL *This,
  IN EFI_HANDLE                       *ExportHandle
  );


/**
  Unit Test case for ExportPsrToUsbStorage function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestExportPsrToUsbStorage (
  IN UNIT_TEST_CONTEXT  Context
  );
