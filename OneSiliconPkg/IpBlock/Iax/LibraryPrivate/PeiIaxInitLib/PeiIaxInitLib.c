/** @file
  PEIM Private Library to initialize for Iax device

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <IaxConfig.h>
#include <Library/DebugLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/IaxInfoLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/PeiServicesLib.h>

/**
  IaxInit: Initialize the Iax device

**/
VOID
EFIAPI
IaxInit (
  VOID
  )
{
  SI_POLICY_PPI               *SiPolicy;
  IAX_PEI_CONFIG              *IaxPeiConfig;
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  SiPolicy = NULL;
  IaxPeiConfig = NULL;

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPolicy == NULL)) {
    return ;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIaxPeiConfigGuid, (VOID *) &IaxPeiConfig);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "BIOS doens't have IAX ConfigBlock\n"));
    return;
  }

  //
  // Check if IAX is supported and enabled.
  //
  if (IsIaxSupported () && IaxPeiConfig->IaxEnable) {
    DEBUG ((DEBUG_INFO, "Enable IAX via DEVEN regiser\n"));
    UncoreDevEnWrite (Iax, 0, Enable);
  } else {
    DEBUG ((DEBUG_INFO, "Disable IAX via DEVEN regiser\n"));
    UncoreDevEnWrite (Iax, 0, Disable);
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}