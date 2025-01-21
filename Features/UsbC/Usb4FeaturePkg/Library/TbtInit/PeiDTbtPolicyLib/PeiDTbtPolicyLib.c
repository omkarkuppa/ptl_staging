/** @file
  This file is PeiDTbtPolicyLib library.

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

#include <Library/PeiServicesLib.h>
#include <PiPei.h>
#include <PeiDTbtPolicyLibrary.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PeiDTbtPolicy.h>

/**
  Print PEI DTBT Policy
**/
VOID
EFIAPI
DTbtPrintPeiPolicyConfig (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                       Status;
  UINT8                            Index;
  PEI_DTBT_POLICY                  *PeiDTbtConfig;

  PeiDTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "DTbtPrintPolicyConfig Start\n"));

  Status = PeiServicesLocatePpi (
             &gPeiDTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiDTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiDTbtPolicyPpiGuid Not installed!!!\n"));
    return;
  }

  //
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_INFO, "\n------------------------ DTBT Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", PEI_DTBT_POLICY_REVISION));
  for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].DTbtControllerEn = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].RpType       = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].RpType));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].PcieRpNumber = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].RpAcpiNumber = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].RpAcpiNumber));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].PcieRpBus    = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].PcieRpDev    = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpDev));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].PcieRpFunc   = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpFunc));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].BdfValid     = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].BdfValid));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].WakeGpioPin  = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].WakeGpioPin));
    DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtControllerConfig[%x].PcieRpMps    = %x\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpMps));
  }

  //
  // Print DTBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode = %x\n", PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode));
  DEBUG ((DEBUG_INFO, "\n------------------------ DTBT Policy (PEI) Print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  Install DTbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiDTbtPolicy (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_PPI_DESCRIPTOR        *PeiDTbtPolicyPpiDesc;
  PEI_DTBT_POLICY               *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "Install PEI DTBT Policy\n"));

  PeiDTbtConfig = NULL;

  //
  // Allocate memory for PeiDTbtPolicyPpiDesc
  //
  PeiDTbtPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (PeiDTbtPolicyPpiDesc != NULL);
  if (PeiDTbtPolicyPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate memory and initialize all default to zero for PeiDTbtPolicy
  //
  PeiDTbtConfig = (PEI_DTBT_POLICY *) AllocateZeroPool (sizeof (PEI_DTBT_POLICY));
  ASSERT (PeiDTbtConfig != NULL);
  if (PeiDTbtConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize PPI
  //
  PeiDTbtPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiDTbtPolicyPpiDesc->Guid = &gPeiDTbtPolicyPpiGuid;
  PeiDTbtPolicyPpiDesc->Ppi = PeiDTbtConfig;

  Status = PeiServicesInstallPpi (PeiDTbtPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install PEI DTBT Policy failed\n"));
  }
  return Status;
}

