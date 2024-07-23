/** @file
  This file is PeiSiPolicyLib library creates default settings of RC
  Policy and installs RC Policy PPI.

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

#include "PeiSiPolicyLibrary.h"

/**
  SiInstallPolicyReadyPpi installs SiPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyReadyPpiDesc;

  SiPolicyReadyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyReadyPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyReadyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyReadyPpiDesc->Guid  = &gSiPolicyReadyPpiGuid;
  SiPolicyReadyPpiDesc->Ppi   = NULL;

  //
  // Install Silicon Policy Ready PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyReadyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
