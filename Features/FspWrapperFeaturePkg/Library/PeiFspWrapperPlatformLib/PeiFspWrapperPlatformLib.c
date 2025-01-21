/** @file
  Provide FSP wrapper platform related function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/FspWrapperApiLib.h>
#include <FspEas.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/S3MemoryVariable.h>
#include <Library/OcWdtLib.h>
#include <Library/SpiLib.h>

/**
  This function overrides the default configurations in the FSP-M UPD data region.

  @note At this point, memory is NOT ready, PeiServices are available to use.

  @param[in,out] FspUpdRgnPtr   A pointer to the UPD data region data strcture.

**/
VOID
EFIAPI
UpdateFspmUpdData (
  IN OUT VOID        *FspUpdRgnPtr
  )
{
  EFI_STATUS         Status;

  Status = SpiServiceInit ();
  ASSERT_EFI_ERROR (Status);
}

/**
  This function overrides the default configurations in the FSP-S UPD data region.

  @param[in,out] FspUpdRgnPtr   A pointer to the UPD data region data strcture.

**/
VOID
EFIAPI
UpdateFspsUpdData (
  IN OUT VOID        *FspUpdRgnPtr
  )
{
}

/**
  Update TempRamExit parameter.

  @note At this point, memory is ready, PeiServices are available to use.

  @return TempRamExit parameter.
**/
VOID *
EFIAPI
UpdateTempRamExitParam (
  VOID
  )
{
  return NULL;
}

/**
  Get S3 PEI memory information.

  @note At this point, memory is ready, and PeiServices are available to use.
  Platform can get some data from SMRAM directly.

  @param[out] S3PeiMemSize  PEI memory size to be installed in S3 phase.
  @param[out] S3PeiMemBase  PEI memory base to be installed in S3 phase.

  @return If S3 PEI memory information is got successfully.
**/
EFI_STATUS
EFIAPI
GetS3MemoryInfo (
  OUT UINT64               *S3PeiMemSize,
  OUT EFI_PHYSICAL_ADDRESS *S3PeiMemBase
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  EFI_STATUS                      Status;
  S3_MEMORY_VARIABLE              S3MemVariable;

  *S3PeiMemBase = 0;
  *S3PeiMemSize = 0;

  //
  // Locate system configuration variable
  //
  Status = PeiServicesLocatePpi(
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    goto GetS3MemoryInfoExit;
  }


  //
  // Get S3 Memory Variable
  //
  VariableSize = sizeof (S3_MEMORY_VARIABLE);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               S3_MEMORY_VARIABLE_NAME,
                               &gS3MemoryVariableGuid,
                               NULL,
                               &VariableSize,
                               &S3MemVariable
                               );
  ASSERT(Status == EFI_SUCCESS);

  if (Status != EFI_SUCCESS) {
    goto GetS3MemoryInfoExit;
  }

  *S3PeiMemBase = S3MemVariable.AcpiReservedMemoryBase;
  *S3PeiMemSize = S3MemVariable.AcpiReservedMemorySize;

GetS3MemoryInfoExit:
  DEBUG ((DEBUG_INFO, "AcpiReservedMemoryBase = 0x%x\n", *S3PeiMemBase));
  DEBUG ((DEBUG_INFO, "AcpiReservedMemorySize = 0x%x\n", *S3PeiMemSize));
  DEBUG ((DEBUG_INFO, "System memory length from previous boot = 0x%x\n", S3MemVariable.SystemMemoryLength));

  return Status;
}