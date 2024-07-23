/**@file
  WdtApp Pei driver implementation.

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

@par Specification
**/

#include <PiPei.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PeiWdtAppLib.h>

/**
  Run this function after Wdt PPI to be installed.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
WdtAppPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mWdtPpiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gWdtPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) WdtAppPpiNotifyCallback
};

/**
  Run this function after Wdt PPI to be installed.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
WdtAppPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                    Status;
  WDT_PPI                       *WdtPei;

  WdtPei = (WDT_PPI *) Ppi;

  DEBUG ((DEBUG_INFO, "(WdtAppPei) %a Start\n", __FUNCTION__));
  Status = PeiWdtApp (WdtPei);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "(WdtAppPei) %a End\n", __FUNCTION__));

  return Status;
}

/**
  Main entry for WdtApp PEIM.
  This routine is to .

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  General purpose services available to every PEIM.

  @retval    EFI_SUCCESS            The function completed successfully.
  @retval    EFI_INVALID_PARAMETER  The PpiList pointer is NULL.
  @retval    EFI_OUT_OF_RESOURCES   There is no additional space in the PPI database.
**/
EFI_STATUS
EFIAPI
WdtAppPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                   Status;

  //
  // Install Wdt Notification
  //
  DEBUG ((DEBUG_INFO, "(WdtAppPei) %a Start\n", __FUNCTION__));
  Status = PeiServicesNotifyPpi (&mWdtPpiNotifyList);
  DEBUG ((DEBUG_INFO, "(WdtAppPei) %a End\n", __FUNCTION__));

  ASSERT_EFI_ERROR (Status);

  return Status;
}
