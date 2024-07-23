/** @file
  This file is PeiSiPolicyLib library for printing Policy settings.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
SiPrintPolicyPpi (
  IN  SI_POLICY_PPI          *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  SI_CONFIG     *SiConfig;
  EFI_STATUS    Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ Silicon Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " SkipSsidProgramming= %x\n", SiConfig->SkipSsidProgramming));
  DEBUG ((DEBUG_INFO, " CustomizedSvid= %x\n", SiConfig->CustomizedSvid));
  DEBUG ((DEBUG_INFO, " CustomizedSsid= %x\n", SiConfig->CustomizedSsid));
  DEBUG ((DEBUG_INFO, " SsidTablePtr= %x\n", SiConfig->SsidTablePtr));
  DEBUG ((DEBUG_INFO, " NumberOfSsidTableEntry= %x\n", SiConfig->NumberOfSsidTableEntry));
  DEBUG ((DEBUG_INFO, " SkipBiosDoneWhenFwUpdate= %x\n", SiConfig->SkipBiosDoneWhenFwUpdate));
  DEBUG ((DEBUG_INFO, "------------------------ Silicon Print Policy End --------------------------\n"));
  DEBUG_CODE_END ();
}
