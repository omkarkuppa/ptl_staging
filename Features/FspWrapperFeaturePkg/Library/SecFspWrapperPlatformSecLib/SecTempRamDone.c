/** @file
  Provide SecTemporaryRamDone function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/TempRamExitPpi.h>
#include <Library/CacheAsRamLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/FspWrapperPlatformLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/PeiServicesTablePointerLib.h>

/**
  This interface disables temporary memory in SEC Phase.
  @todo move to inside FSP
**/
VOID
EFIAPI
SecPlatformDisableTemporaryMemory (
  VOID
  )
{
  DEBUG((DEBUG_INFO, "SecPlatformDisableTemporaryMemory enter\n"));

#if FixedPcdGet8(PcdFspModeSelection) == 1
  EFI_STATUS                Status;
  VOID                      *TempRamExitParam;

  TempRamExitParam = UpdateTempRamExitParam ();
  Status = CallTempRamExit (TempRamExitParam);
  DEBUG((DEBUG_INFO, "TempRamExit status: 0x%x\n", Status));
  ASSERT_EFI_ERROR(Status);
#else

  EFI_STATUS               Status;
  CONST EFI_PEI_SERVICES   **PeiServices;
  FSP_TEMP_RAM_EXIT_PPI    *TempRamExitPpi;

  PeiServices = GetPeiServicesTablePointer ();
  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gFspTempRamExitPpiGuid,
                           0,
                           NULL,
                           (VOID **) &TempRamExitPpi
                           );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  TempRamExitPpi->TempRamExit (NULL);
#endif

  return ;
}
