/**@file
    Unit test case Prerequisite of CrashLogDxeEntry.

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

#include "CrashLogDxeEntryMock.h"
#include "TestCase/InstallApeiBertTables/InstallApeiBertTablesMock.h"
#include "TestCase/GetPayloadInfoFromHob/GetPayloadInfoFromHobMock.h"

#include <PiDxe.h>
#include <Pi/PiHob.h>
#include <Library/HobLib.h>
#include <Guid/CrashLogHob.h>

extern VOID  *gHobList;

/**
  The Mock of LocateProtocol() function.

  Returns the first protocol instance that matches the given protocol.

  @param[in]  Protocol          Provides the protocol to search for.
  @param[in]  Registration      Optional registration key returned from
                                RegisterProtocolNotify().
  @param[out]  Interface        On return, a pointer to the first interface that matches Protocol and
                                Registration.

  @retval EFI_SUCCESS           A protocol instance matching Protocol was found and returned in
                                Interface.
  @retval EFI_NOT_FOUND         No protocol instances were found that match Protocol and
                                Registration.
  @retval EFI_INVALID_PARAMETER Interface is NULL.
                                Protocol is NULL.

**/
EFI_STATUS
EFIAPI
MockLocateProtocol (
  IN  EFI_GUID* Protocol,
  IN  VOID*     Registration OPTIONAL,
  OUT VOID**    Interface
  )
{
  EFI_STATUS    Status;

  Status = EFI_NOT_FOUND;

  if (CompareGuid (Protocol, &gEfiAcpiTableProtocolGuid) == TRUE) {
    *Interface = gAcpiTbl;
    Status = EFI_SUCCESS;
  }
  return Status;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreCrashLogDxeEntry (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PreInstallApeiBertTables (Context);

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = MockLocateProtocol;

  gHobList = MockCrashLogRecordHob ();
  return UNIT_TEST_PASSED;
}
