/** @file
  PEI Module to validate FSPS

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

#include "FspSVInit.h"

/**
  FSP-V test case callback based on the return codes
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
FspSVTestCasesCallback (
  IN CONST  EFI_PEI_SERVICES       **PeiServices,
  IN        EFI_STATUS_CODE_TYPE   CodeType,
  IN        EFI_STATUS_CODE_VALUE  Value,
  IN        UINT32                 Instance,
  IN CONST  EFI_GUID               *CallerId,
  IN CONST  EFI_STATUS_CODE_DATA   *Data
  )
{
  FSPV_POSTMEM_CONFIG  *FspVPostMemConfig;
  SI_POLICY_PPI        *SiPolicyPpi;
  EFI_STATUS           Status;
  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_PROGRESS_CODE) ||
      (Value != (INTEL_RC_STATUS_CODE_CPU_INIT_START_POSTMEM)))
  {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "FSP-S validation callback begin\n"));

  //Configure FSPM test cases based on FspmValidation value.
  Status = PeiServicesLocatePpi (
                 &gSiPolicyPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SiPolicyPpi
                 );
  ASSERT_EFI_ERROR (Status);

  if ((Status == EFI_SUCCESS) && (SiPolicyPpi != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFspVConfigGuid, (VOID *) &FspVPostMemConfig);
    ASSERT_EFI_ERROR (Status);
  }
  if (EFI_ERROR ((Status))) {
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "FspsValidation configuration value = 0x%x\n", FspVPostMemConfig->TestId));

  // Execute FSP-S test cases based on TestId value.
  switch (FspVPostMemConfig->TestId) {
  case RunNoFspsTest:
    break;

  case RunAllFspsTest:
  default:
    ValidateMicrocodeVersion (0x00); // Expected microcode version to be passed.
    ValidateMicrocodeVersion (0x01); // Expected microcode version to be passed
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
FspSVInitEntryPoint (
  IN  EFI_PEI_FILE_HANDLE     FileHandle,
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS               Status;
  EFI_PEI_RSC_HANDLER_PPI  *RscHandler;
  //
  // Register notify list that will get invoked later
  //
  DEBUG ((DEBUG_INFO, "Register RSC callback for FSP-S validation\n"));
  Status = PeiServicesLocatePpi (
               &gEfiPeiRscHandlerPpiGuid,
               0,
               NULL,
               (VOID **)&RscHandler
               );
  ASSERT_EFI_ERROR (Status);
  Status = RscHandler->Register (FspSVTestCasesCallback);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
