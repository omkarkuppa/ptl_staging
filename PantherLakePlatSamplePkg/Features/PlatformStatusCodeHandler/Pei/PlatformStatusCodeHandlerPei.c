/** @file
  Platform status code implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include "PlatformStatusCodeHandlerPei.h"
#include <Library/FspErrorInfoLib.h>

/**
  ReportStatusCode worker for FSP Error Information. (PEI wrapper)

  @param  CodeType         Always (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED)
  @param  Value            Always 0
  @param  Instance         Always 0
  @param  CallerId         This optional parameter may be used to identify the caller.
                           It may be used to identify which internal component of the FSP
                           was executing at the time of the error.
  @param  Data             This data contains FSP error type and status code.

  @retval EFI_SUCCESS      Show error status sent by FSP successfully.
  @retval RETURN_ABORTED   Function skipped as unrelated.

**/
EFI_STATUS
EFIAPI
FspErrorStatusCodeReportWorkerPei (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  return (FspErrorStatusCodeReportWorker (CodeType, Value, Instance, CallerId, Data));
}

/**
  A notify function to initialize the supported status code devices.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PlatformStatusCodeDevicesInit(
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiRscHandlerPpiGuid,
  PlatformStatusCodeDevicesInit
};

/**
  Entry point of Status Code PEIM.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCESS  The entry point of DXE IPL PEIM executes successfully.

**/
EFI_STATUS
EFIAPI
StatusCodeHandlerPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
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
  if (EFI_ERROR (Status)) {
    PeiServicesNotifyPpi (&mPpiList);
  } else {
    PlatformStatusCodeDevicesInit (NULL, NULL, NULL);
  }

  return EFI_SUCCESS;
}

/**
  A notify function to initialize the supported status code devices.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PlatformStatusCodeDevicesInit(
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_RSC_HANDLER_PPI     *RscHandlerPpi;
  UINT8                       StatusCodeFlags;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );
  ASSERT_EFI_ERROR (Status);

  StatusCodeFlags = GetDebugInterface ();

  if (PcdGetBool (PcdStatusCodeUseUsb3)) {
    if (StatusCodeFlags & STATUS_CODE_USE_USB3) {
      Status = Usb3DebugPortInitialize ();
      ASSERT_EFI_ERROR (Status);
    }
    Status = RscHandlerPpi->Register (Usb3StatusCodeReportWorker);
    ASSERT (!EFI_ERROR (Status) || Status == EFI_ALREADY_STARTED);
  }

  if (PcdGetBool (PcdStatusCodeUseSerialIoUart)) {
    if (StatusCodeFlags & STATUS_CODE_USE_SERIALIO) {
      SerialIoUartDebugInit ();
    }
    Status = RscHandlerPpi->Register (SerialIoUartStatusCodeReportWorker);
    ASSERT (!EFI_ERROR (Status) || Status == EFI_ALREADY_STARTED);
  }

  if (PcdGet8 (PcdFspModeSelection) == 0) {
    Status = RscHandlerPpi->Register (FspErrorStatusCodeReportWorkerPei);
    ASSERT (!EFI_ERROR (Status) || Status == EFI_ALREADY_STARTED);
  }

  return EFI_SUCCESS;
}
