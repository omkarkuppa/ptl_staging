/** @file
  This file provides service for PEI phase policy printing

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
#include <PeiITbtPolicyLibrary.h>

/**
  This function prints the PEI phase PreMem policy.

  @param[in] SiPolicyPpi                Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
ITbtPrintPolicy (
  IN  SI_POLICY_PPI                     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                            Status;
  PEI_ITBT_CONFIG                       *PeiITbtConfig;

  PeiITbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "[TBT] TbtPrintPolicyConfig Start\n"));

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n------------------------ ITBT Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", SiPolicyPpi->TableHeader.Header.Revision));

  DEBUG ((DEBUG_INFO, " Revision : %d\n", PeiITbtConfig->Header.Revision));
  //
  // Print ITBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs = %x\n", PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs));
  DEBUG ((DEBUG_INFO, "PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs = %x\n", PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs));
  DEBUG ((DEBUG_INFO, "PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4 = %x\n", PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4));
  DEBUG ((DEBUG_INFO, "PeiITbtConfig->ITbtGenericConfig.Usb4CmMode = %x\n", PeiITbtConfig->ITbtGenericConfig.Usb4CmMode));

  DEBUG ((DEBUG_INFO, "\n------------------------ ITBT Policy (PEI) Print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}