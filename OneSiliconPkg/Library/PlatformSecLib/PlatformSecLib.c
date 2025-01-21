/** @file
  Instance of SEC Platform Hook Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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

///
/// The package level header files this module uses
///
#include "PlatformSecLibrary.h"

GLOBAL_REMOVE_IF_UNREFERENCED PEI_SEC_PERFORMANCE_PPI  mSecPerformancePpi = {
  SecGetPerformance
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR  mPeiSecPlatformPpi[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiSecPerformancePpiGuid,
    &mSecPerformancePpi
  }
};

/**
  Perform those platform specific operations that are requried to be executed as early as possibile.

  @retval EFI_SUCCESS    Always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SecPlatformLibConstructor (
  VOID
  )
{
  BOOLEAN                       RngSuccess;
  UINT16                        RandomNumber;
  UINT32                        Index;
  ///
  /// Init Apic Timer for Performance collection.
  /// Use EXCEPT_IA32_BOUND as interrupte type.
  ///
  PERF_CODE (
    InitializeApicTimer (0, (UINT32) -1, TRUE, 5);
  );

  //
  // Skylake and Kaby Lake Processor Family Core and Uncore BIOS Specification 2.0.7 March 2016 Section 1.5.1
  // Initialize RNG HW early in the POST to ensure random numbers are generated correctly BIOS must
  //  implement a time-out value of 0x800000.
  //
  RngSuccess = FALSE;
  for (Index = 0; Index < CPU_DRNG_RETRY_LIMIT; Index++) {
    RngSuccess = GetRandomNumber16 ((UINT16 *)&RandomNumber);
    if (RngSuccess) {
      break;
    }
  }

  if (!RngSuccess) {
    DEBUG ((DEBUG_INFO, "Fail to get a 16-bits random number!!!\n"));
  }

  return EFI_SUCCESS;
}

/**
  This interface conveys performance information out of the Security (SEC) phase into PEI.

  This service is published by the SEC phase. The SEC phase handoff has an optional
  EFI_PEI_PPI_DESCRIPTOR list as its final argument when control is passed from SEC into the
  PEI Foundation. As such, if the platform supports collecting performance data in SEC,
  this information is encapsulated into the data structure abstracted by this service.
  This information is collected for the boot-strap processor (BSP) on IA-32.

  @param[in]  PeiServices  The pointer to the PEI Services Table.
  @param[in]  This         The pointer to this instance of the PEI_SEC_PERFORMANCE_PPI.
  @param[out] Performance  The pointer to performance data collected in SEC phase.

  @retval     EFI_SUCCESS  The data was successfully returned.
**/
EFI_STATUS
EFIAPI
SecGetPerformance (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PEI_SEC_PERFORMANCE_PPI   *This,
  OUT      FIRMWARE_SEC_PERFORMANCE  *Performance
  )
{
  return EFI_SUCCESS;
}


/**
  A developer supplied function to perform platform specific operations.

  It's a developer supplied function to perform any operations appropriate to a
  given platform. It's invoked just before passing control to PEI core by SEC
  core. Platform developer may modify the SecCoreData and PPI list that is
  passed to PEI Core.

  @param[in][out] SecCoreData  The same parameter as passing to PEI core. It
                               could be overridden by this function.

  @retval         PpiList      The platform specific PPI list to be passed to PEI core or
                               NULL if there is no need of such platform specific PPI list.
**/
EFI_PEI_PPI_DESCRIPTOR *
EFIAPI
SecPlatformMain (
  IN OUT   EFI_SEC_PEI_HAND_OFF        *SecCoreData
  )
{
  EFI_PEI_PPI_DESCRIPTOR      *PpiList;

  PpiList = &mPeiSecPlatformPpi[0];

  return PpiList;
}
