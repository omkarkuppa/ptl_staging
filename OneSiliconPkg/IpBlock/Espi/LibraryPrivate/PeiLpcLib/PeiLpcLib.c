/** @file
  PCH LPC Library

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

@par Specification Reference:
**/

#include <Uefi/UefiBaseType.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Register/EspiRegs.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <RegisterAccess.h>
#include <LpcConfig.h>

/**
  @param[in]  *LpcPcrAccess       Instance of LPC sideband Register Access Protocol

  Configure LPC power management.
**/
VOID
LpcConfigurePm (
  REGISTER_ACCESS            *LpcPcrAccess
  )
{
  EFI_STATUS              Status;
  SI_PREMEM_POLICY_PPI    *SiPreMemPolicy;
  PCH_LPC_PREMEM_CONFIG   *LpcPreMemConfig;
  UINT32                  AndData;
  UINT32                  OrData;


  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// BIOS need to set LPC PCR 0x341C[13,3:0] to all 1's and [8,9,10] = 0's
  ///
  AndData = (UINT32) ~(BIT10 | BIT9 | BIT8);
  OrData  = (BIT13 | BIT3 | BIT2 | BIT1 | BIT0);

  if (!LpcPreMemConfig->LpcPmHAE) {
    ///
    /// BIOS need to set LPC PCR 0x341C[3:0] to all 1's and [13,8,9,10] = 0's
    ///
    AndData &= (UINT32) ~(BIT13);
    OrData  &= (UINT32) ~(BIT13);
  }

  LpcPcrAccess->AndThenOr32 (LpcPcrAccess, R_LPC_PCR_PRC, AndData, OrData);
}

/**
  Configure LPC device on early PEI.
**/
VOID
LpcOnEarlyPeiConfigure (
  VOID
  )
{
  EFI_STATUS                            Status;

  DEBUG ((DEBUG_INFO, "LpcOnEarlyPeiConfigure() \n"));

  //
  // Enhance port 8xh LPC decoding.
  // The can be disable by policy EnhancePort8xhDecoding.
  //
  Status = PchLpcGenIoRangeSet (0x80, 0x10);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set LPC IO range\n"));
  }
}

/**
  Configure LPC device on Policy callback.

  @param[in] SiPreMemPolicyPpi         The SI PreMem Policy PPI instance
**/
VOID
LpcOnPolicyConfigure (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                            Status;
  PCH_LPC_PREMEM_CONFIG                 *LpcPreMemConfig;

  DEBUG ((DEBUG_INFO, "LpcOnPolicyConfigure()\n"));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // If EnhancePort8xhDecoding is disabled, clear LPC and DMI LPC general IO range register.
  //
  if (LpcPreMemConfig->EnhancePort8xhDecoding == 0) {
    DEBUG ((DEBUG_INFO, "Disable EnhancePort8xhDecoding\n"));
    Status = PchLpcGenIoRangeClear (0x80, 0x10);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to clear LPC IO range\n"));
    }
  }
}
