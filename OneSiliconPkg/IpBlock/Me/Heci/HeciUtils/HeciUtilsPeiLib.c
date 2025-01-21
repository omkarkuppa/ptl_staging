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
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/HeciTransportPpi.h>

/**
  Get instance index of given PPI for given GUID.

  @param[in] InputPpi         PPI instance pointer to look for
  @param[in] InputPpiGuid     PPI Guid to search for

  @retval PPI instance index if PPI instance found
  @retval MAX_UINT32 if PPI instance not found
**/
UINT32
GetPpiInstanceIndex (
  IN VOID       *InputPpi,
  IN EFI_GUID   *InputPpiGuid
  )
{
  EFI_STATUS    Status;
  UINTN         PpiInstance;
  VOID          *ReturnPpi;

  if ((InputPpi == NULL) || (InputPpiGuid == NULL)) {
    return MAX_UINT32;
  }

  PpiInstance = 0;
  ReturnPpi = NULL;

  PeiServicesLocatePpi (
    InputPpiGuid,
    PpiInstance,
    NULL,
    (VOID **) &ReturnPpi
    );

  while (InputPpi != ReturnPpi) {
    ReturnPpi = NULL;
    PpiInstance++;
    Status = PeiServicesLocatePpi (
               InputPpiGuid,
               PpiInstance,
               NULL,
               (VOID **) &ReturnPpi
               );
    if (EFI_ERROR (Status) || (ReturnPpi == NULL)) {
      break;
    }
  }

  if (InputPpi != ReturnPpi) {
    PpiInstance = MAX_UINT32;
  }

  return (UINT32) PpiInstance;
}

/**
  Get next instance of given PPI for given GUID.
  If NULL passed as an InputPpi, returns first PPI instance.

  @param[in] InputPpi         PPI instance pointer to look for next instance
  @param[in] InputPpiGuid     PPI Guid to search for

  @retval Pointer to next PPI instance
  @retval NULL if there's no more PPI instances
**/
VOID *
GetNextPpi (
  IN VOID       *InputPpi,
  IN EFI_GUID   *InputPpiGuid
  )
{
  UINTN   PpiInstance;
  VOID    *ReturnPpi;

  ReturnPpi = NULL;

  if (InputPpi == NULL) {
    PpiInstance = 0;
  } else {
    PpiInstance = GetPpiInstanceIndex (InputPpi, InputPpiGuid);
  }

  if (PpiInstance == MAX_UINT32) {
    return NULL;
  }

  if (InputPpi != NULL) {
    PpiInstance++;
  }

  PeiServicesLocatePpi (
    InputPpiGuid,
    PpiInstance,
    NULL,
    (VOID **) &ReturnPpi
    );

  return ReturnPpi;
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
  HECI_TRANSPORT    *HeciTransport;

  HeciTransport = NULL;

  PeiServicesLocatePpi (
    &gHeciTransportPpiGuid,
    0,
    NULL,
    (VOID **) &HeciTransport
    );

  return HeciTransport;
}
