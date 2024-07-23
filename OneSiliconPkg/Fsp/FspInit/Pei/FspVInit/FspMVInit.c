/** @file
  PEI Module to validate FSPM

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

#include "FspMVInit.h"

/**
  FSP-N test case callback based on return codes
  @param[in]  PeiServices         An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in]  CodeType            Indicates the type of status code being reported.
  @param[in]  Value               Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is used to
                                  classify the entity as well as an operation.
  @param[in]  Instance            The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in]  CallerId            This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different rules to
                                  different callers.
  @param[in]  Data                This optional parameter may be used to pass additional data.
  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.
**/
EFI_STATUS
EFIAPI
FspMVTestCasesLoadCallback (
  IN CONST  EFI_PEI_SERVICES       **PeiServices,
  IN        EFI_STATUS_CODE_TYPE   CodeType,
  IN        EFI_STATUS_CODE_VALUE  Value,
  IN        UINT32                 Instance,
  IN CONST  EFI_GUID               *CallerId,
  IN CONST  EFI_STATUS_CODE_DATA   *Data
  )
{
  FSPV_PREMEM_CONFIG    *FspVPreMemConfig;
  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi;
  EFI_STATUS            Status;
  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_PROGRESS_CODE) ||
      (Value != (INTEL_RC_STATUS_CODE_ME_MEDEV_INIT_ENTRY)))
  {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "FSP-M validation callback begin\n"));

  //Configure FSPM test cases based on FspmValidation value.
  Status = PeiServicesLocatePpi (
                 &gSiPreMemPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPreMemPolicyPpi
                 );
  ASSERT_EFI_ERROR (Status);

  if ((Status == EFI_SUCCESS) && (SiPreMemPolicyPpi != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gFspVPreMemConfigGuid, (VOID *) &FspVPreMemConfig);
    ASSERT_EFI_ERROR (Status);
  }
  if (EFI_ERROR ((Status))) {
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "FspmValidation configuration value = 0x%x\n", FspVPreMemConfig->TestId));

  // Execute FSP-M test cases based on TestId value.
  switch (FspVPreMemConfig->TestId) {
  case RunNoFspmTest:
    //Placeholder for FSPM test cases. Test cases will be added in future.
    break;

  case RunAllFspmTest:
  default:
    //Placeholder for FSPM test cases. Test cases will be added in future.
    break;
  }

  return EFI_SUCCESS;
}

/**
  FSP Validation entry point
  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.
  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
FspMVInitEntryPoint (
  IN  EFI_PEI_FILE_HANDLE     FileHandle,
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS               Status;
  EFI_PEI_RSC_HANDLER_PPI  *RscHandler;
  //
  // Register notify list that will get invoked later
  //
  DEBUG ((DEBUG_INFO, "Register RSC callback for FSP-M validation\n"));
  Status = PeiServicesLocatePpi (
               &gEfiPeiRscHandlerPpiGuid,
               0,
               NULL,
               (VOID **)&RscHandler
               );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = RscHandler->Register (FspMVTestCasesLoadCallback);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
