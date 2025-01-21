/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>

#include <Library/ReportStatusCodeLib.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/PostCodeLib.h>

#define VARIABLE_INTEGRITY_RECOVERY_CODE    0x00F1
/**
  Report status code handler for PEI. This is used to detect protected variable integrity failure.

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
StatusCodeHandlerPei (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN        EFI_STATUS_CODE_TYPE    CodeType,
  IN        EFI_STATUS_CODE_VALUE   Value,
  IN        UINT32                  Instance,
  IN CONST  EFI_GUID                *CallerId,
  IN CONST  EFI_STATUS_CODE_DATA    *Data
  )
{
  EFI_STATUS  Status;

  ///
  /// Detect protected variable integrity check failure
  ///
  if ((CodeType == (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED)) &&
      (Value == (PcdGet32 (PcdStatusCodeVariableIntegrity) | (EFI_SW_PEI_PC_RECOVERY_BEGIN)))) {
    ///
    /// Protected variable integrity check failed
    /// promote boot mode to Recovery
    ///
    DEBUG ((DEBUG_ERROR, "Promote BootMode to Recovery\n"));
    Status = PeiServicesSetBootMode (BOOT_IN_RECOVERY_MODE);
    PostCode (VARIABLE_INTEGRITY_RECOVERY_CODE);
    return Status;
  }

  return EFI_UNSUPPORTED;
}

/**
  Init routine for PEI Protected Variable Recovery.

  @param[in] FfsHeader            FfsHeader pointer.
  @param[in] PeiServices          PeiServices pointer.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
PeiProtectedVariableRecoveryInit (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS               Status;
  EFI_PEI_RSC_HANDLER_PPI  *RscHandler;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandler
             );
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to locate PPI: %g\n", __FUNCTION__, gEfiPeiRscHandlerPpiGuid));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Register recovery handler for protected variable failure
  //
  Status = RscHandler->Register (StatusCodeHandlerPei);
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to register status code handler\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "%a: Registered Protected Variable Recovery handler\n", __FUNCTION__));

  return Status;
}
