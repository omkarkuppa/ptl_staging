/**@file
  Unit test case of the Cnv Dxe driver.

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

#include "BootServicesMock.h"
#include "TestCaseCnvNvsAreaInit.h"
#include <Protocol/CnvNvsArea.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UnitTestHostBaseLib.h>
#include <Library/DebugLib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

extern EFI_BOOT_SERVICES  *gBS;

/**
  CNV NVS Area Initialize

  @param[in] mCnvNvsAreaPtr      Pointer to CNV NVS Area.
**/
extern
VOID
CnvNvsAreaInit (
  IN  VOID  **mCnvNvsAreaPtr
  );

/**
  Unit-Test Function for CnvNvsAreaInit.

  CNV NVS Area Initialize when AllocatePages.

  @param[in]  Context                            An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCnvNvsAreaInit (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                  Pages;
  EFI_PHYSICAL_ADDRESS   CmpAddress;
  EFI_PHYSICAL_ADDRESS   Address;
  CNV_NVS_AREA_PROTOCOL  *CnvNvsAreaProtocol;

  Pages      = EFI_SIZE_TO_PAGES (sizeof (CNV_PKG_NVS_AREA));
  Address    =  (UINTN)AllocatePages (Pages);
  CmpAddress = (UINTN)AllocatePages (Pages);
  SetMem ((VOID *)Address, sizeof (CNV_PKG_NVS_AREA), 1);
  SetMem ((VOID *)CmpAddress, sizeof (CNV_PKG_NVS_AREA), 0);

  //
  // Mock CnvNvsAreaProtocol
  //
  CnvNvsAreaProtocol = AllocateZeroPool (sizeof (CNV_NVS_AREA_PROTOCOL));

  //
  // Mock boot service AllocatePages
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->AllocatePages = MockBootServiceAllocatePages;

  will_return (MockBootServiceAllocatePages, Address);      // Mock gBS->AllocatePages to get address
  will_return (MockBootServiceAllocatePages, Pages);
  will_return (MockBootServiceAllocatePages, EFI_SUCCESS);  // Mock return Status = EFI_SUCCESS

  //
  // Execute test function with above mock environment
  //
  CnvNvsAreaInit ((VOID **)&CnvNvsAreaProtocol->Area);

  //
  // Test case
  //
  UT_ASSERT_EQUAL (CompareMem (CnvNvsAreaProtocol->Area, (VOID *)CmpAddress, sizeof (CNV_PKG_NVS_AREA)), 0);
  if (CompareMem (CnvNvsAreaProtocol->Area, (VOID *)CmpAddress, sizeof (CNV_PKG_NVS_AREA)), 0) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  FreePages ((VOID *)Address, EFI_SIZE_TO_PAGES (Pages));
  FreePages ((VOID *)CmpAddress, EFI_SIZE_TO_PAGES (Pages));
  FreePool (gBS);
  FreePool (CnvNvsAreaProtocol);

  return UNIT_TEST_PASSED;
}
