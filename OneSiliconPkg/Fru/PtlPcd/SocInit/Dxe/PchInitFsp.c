/** @file
  This is the FSP driver that initializes the Intel PCH.

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
#include <Library/DebugLib.h>
#include <Library/FspCommonLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PerformanceLib.h>
#include "PchInit.h"

EFI_STATUS
EFIAPI
PchOnPciEnumCompleteFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mPchOnPciEnumCompleteNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPciEnumerationCompleteProtocolGuid,
    PchOnPciEnumCompleteFsp
  }
};

EFI_STATUS
EFIAPI
PchOnEndOfSiInitFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mPchOnEndOfSiInitNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEndOfSiInitPpiGuid,
    PchOnEndOfSiInitFsp
  }
};

/**
  <b>FSP PchInit Module Entry Point for FSP</b>\n

  @param[in] FileHandle      PEIM's file handle
  @param[in] PeiServices     An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPointFsp (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "PchInitEntryPointFsp() Start\n"));

  Status = PeiServicesNotifyPpi (mPchOnEndOfSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  //
  // This PPI Notify will only be triggered in API mode.
  // In Dispatch mode, PchInitDxeFsp.c - PchOnPciEnumComplete() is used instead.
  //
  Status = PeiServicesNotifyPpi (mPchOnPciEnumCompleteNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PchInitEntryPointFsp() End\n"));

  return Status;
}

/**
  Fsp PCH initialization on PCI enumeration complete

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchOnPciEnumCompleteFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteFsp() Start\n"));

  PERF_INMODULE_BEGIN ("PchOnPciEnumCompCommon");
  PchOnPciEnumCompleteCommon ();
  PERF_INMODULE_END ("PchOnPciEnumCompCommon");

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteFsp() End\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PchOnEndOfSiInitFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "PchOnEndOfSiInitFsp() Start\n"));

  PERF_INMODULE_BEGIN ("PchOnEndOfSiInitFsp");
  PchInitEntryPointCommon ();
  PERF_INMODULE_END ("PchOnEndOfSiInitFsp");

  DEBUG ((DEBUG_INFO, "PchOnEndOfSiInitFsp() End\n"));

  return EFI_SUCCESS;
}
