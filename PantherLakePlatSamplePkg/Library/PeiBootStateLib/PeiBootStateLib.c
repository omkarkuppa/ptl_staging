/** @file
  Description file for BootState Variable Information during PEI Phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <BootStateLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/ReadOnlyVariable2.h>

/**
 This function call checks if BootState variable is present.
 BootState variable is not present on the first boot after flashing image.
 BootState variable got set end of Dxe phase.

 @retval         FALSE                  It's the first boot after reflashing.
 @retval         TRUE                   It's not the first boot after reflashing.
**/
BOOLEAN
EFIAPI
IsBootStatePresent (
  VOID
)
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PeiVar;
  BOOLEAN                           BootState;
  UINTN                             DataSize;
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&PeiVar
             );
  ASSERT_EFI_ERROR(Status);

  ///
  /// Get last Boot State Variable From NVRAM
  ///
  DataSize = sizeof(BOOLEAN);
  Status = PeiVar->GetVariable (
                     PeiVar,
                     BOOT_STATE_VARIABLE_NAME,
                     &gBootStateGuid,
                     NULL,
                     &DataSize,
                     &BootState
                     );
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "BootState is present.\n"));
    return TRUE;
  } else if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "BootState is NOT present.\n"));
    return FALSE;
  } else {
    DEBUG ((DEBUG_ERROR, "Get variable failure.\n"));
    ASSERT_EFI_ERROR (Status);
    return FALSE;
  }
}
