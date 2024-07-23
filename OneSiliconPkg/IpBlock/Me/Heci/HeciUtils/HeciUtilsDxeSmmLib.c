/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PrintLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>

/**
  Function installs DXE or SMM protocol depending on mode we are running in

  @param[in] HeciGuidProtocol       Pointer to Dxe/Smm protocol guid to install
  @param[in] HeciProtocol           Pointer to HECI protocol to install

  @retval EFI_SUCCESS               Protocol installed successfully
  @retval EFI_INVALID_PARAMETER     Wrong parameters passed
**/
EFI_STATUS
InstallHeciProtocol (
  EFI_GUID    *HeciGuidProtocol,
  VOID        *HeciProtocol
  )
{
  EFI_STATUS                Status;
  EFI_SMM_SYSTEM_TABLE2     *Smst2;
  EFI_HANDLE                Handle;

  if ((HeciProtocol == NULL) || (HeciGuidProtocol == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Handle = NULL;
  MeIsInSmm (&Smst2);

  if (Smst2 != NULL) {
    Status = Smst2->SmmInstallProtocolInterface (
                      &Handle,
                      HeciGuidProtocol,
                      EFI_NATIVE_INTERFACE,
                      HeciProtocol
                      );
  } else {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    HeciGuidProtocol,
                    HeciProtocol,
                    NULL
                    );
  }

  return Status;
}

/**
  Get first HeciAccess protocol pointer

  @retval Pointer to first HeciAccess protocol
  @retval NULL pointer indicates error
**/
HECI_ACCESS *
GetFirstHeciAccess (
  VOID
  )
{
  EFI_SMM_SYSTEM_TABLE2  *Smst2;
  HECI_ACCESS            *ReturnHeciAccess;

  ReturnHeciAccess = NULL;
  MeIsInSmm (&Smst2);

  if (Smst2 != NULL) {
    Smst2->SmmLocateProtocol (&gHeciAccessSmmProtocolGuid, NULL, (VOID **) &ReturnHeciAccess);
  } else {
    gBS->LocateProtocol (&gHeciAccessProtocolGuid, NULL, (VOID **) &ReturnHeciAccess);
  }

  if (ReturnHeciAccess != NULL) {
    ReturnHeciAccess = ((HECI_ACCESS_PRIVATE *) ReturnHeciAccess)->HeciAccessRoot;
  }

  return ReturnHeciAccess;
}

/**
  Get first HeciTransport protocol pointer

  @retval Pointer to first HeciTransport protocol
  @retval NULL pointer indicates error
**/
HECI_TRANSPORT *
GetFirstHeciTransport (
  VOID
  )
{
  EFI_SMM_SYSTEM_TABLE2  *Smst2;
  HECI_TRANSPORT         *ReturnHeciTransport;

  ReturnHeciTransport = NULL;
  MeIsInSmm (&Smst2);

  if (Smst2 != NULL) {
    Smst2->SmmLocateProtocol (&gHeciTransportSmmProtocolGuid, NULL, (VOID **) &ReturnHeciTransport);
  } else {
    gBS->LocateProtocol (&gHeciTransportProtocolGuid, NULL, (VOID **) &ReturnHeciTransport);
  }

  if (ReturnHeciTransport != NULL) {
    ReturnHeciTransport = ((HECI_TRANSPORT_PRIVATE *) ReturnHeciTransport)->HeciTransportRoot;
  }

  return ReturnHeciTransport;
}