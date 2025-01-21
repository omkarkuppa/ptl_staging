/** @file

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

#include <PiPei.h>
#include <Ppi/DebugEventHandler.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
DebugEventHandler (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       *CallerId OPTIONAL,
  IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  );

DEBUG_EVENT_HANDLER_PPI mFspWrapperDebugEventHandlerPpi = {
  DebugEventHandler
};

EFI_PEI_PPI_DESCRIPTOR   mFspWrapperDebugEventHandlerPpiList = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gDebugEventHandlerPpiGuid,
    (VOID *) &mFspWrapperDebugEventHandlerPpi
};

EFI_STATUS
EFIAPI
DebugEventHandler (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       *CallerId OPTIONAL,
  IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  )
{
  CONST EFI_PEI_SERVICES     **PeiServices;
  EFI_STATUS                 Status;

  PeiServices = GetPeiServicesTablePointer ();
  Status = (*PeiServices)->ReportStatusCode (
                                 PeiServices,
                                 CodeType,
                                 Value,
                                 (UINT32) Instance,
                                 (EFI_GUID *) CallerId,
                                 (EFI_STATUS_CODE_DATA *) Data
                                 );
  return Status;
}

/**
  The Entry point of the Debug Event Handle PEIM

  @param[in]  FileHandle   Pointer to image file handle
  @param[in]  PeiServices  Pointer to PEI Services Table

  @return     EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
DebugEventHandlerPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                 Status;

  Status = PeiServicesInstallPpi (&mFspWrapperDebugEventHandlerPpiList);
  ASSERT_EFI_ERROR(Status);
  return EFI_SUCCESS;
}