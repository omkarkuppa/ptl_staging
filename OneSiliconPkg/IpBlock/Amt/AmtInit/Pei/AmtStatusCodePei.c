/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <AmtPetQueueHob.h>

/**
  This routine puts PET message to MessageQueue, which will be sent later.

  @param[in] Type                 StatusCode message type.
  @param[in] Value                StatusCode message value.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
AmtQueuePetMessage (
  IN  EFI_STATUS_CODE_TYPE    Type,
  IN  EFI_STATUS_CODE_VALUE   Value
  )
{
  AMT_PET_QUEUE_HOB     *PetQueueHob;
  EFI_STATUS            Status;

  //
  // Create PET queue hob
  //
  Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, sizeof(AMT_PET_QUEUE_HOB), (VOID **) &PetQueueHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  PetQueueHob->EfiHobGuidType.Name = gAmtPetQueueHobGuid;
  PetQueueHob->Type = Type;
  PetQueueHob->Value = Value;

  return EFI_SUCCESS;
}

/**
  Provides an interface that a software module can call to report an ASF PEI status code.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] Type                 Indicates the type of status code being reported.
  @param[in] Value                Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is
                                  used to classify the entity as well as an operation.
  @param[in] Instance             The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in] CallerId             This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different
                                  rules to different callers.
  @param[in] Data                 This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
AmtStatusCodeWorker (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_STATUS_CODE_TYPE        Type,
  IN EFI_STATUS_CODE_VALUE       Value,
  IN UINT32                      Instance,
  IN EFI_GUID                    *CallerId,
  IN EFI_STATUS_CODE_DATA        *Data
  )
{
  if (((Type & EFI_STATUS_CODE_TYPE_MASK) != EFI_PROGRESS_CODE) &&
      ((Type & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE)) {
    return EFI_SUCCESS;
  }

  //
  // Create PET queue hob
  //
  return AmtQueuePetMessage (Type, Value);
}

/**
  Init routine for PEI ASF StatusCode.

  @param[in] FfsHeader            FfsHeader pointer.
  @param[in] PeiServices          PeiServices pointer.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
AmtStatusInit (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = RscHandlerPpi->Register ((EFI_PEI_RSC_HANDLER_CALLBACK) AmtStatusCodeWorker);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
