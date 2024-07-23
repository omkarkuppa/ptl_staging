/** @file
  HECI Transport protocol driver (PEI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciAccessPpi.h>

/**
  Get next HECI Transport

  @param[in]  This    Pointer to HECI Transport protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciTransport protocol instance
**/
HECI_TRANSPORT *
EFIAPI
GetNextHeciTransport (
  IN HECI_TRANSPORT   *This
  )
{
  return GetNextPpi (This, &gHeciTransportPpiGuid);
}

/**
  Get HECI_TRANSPORT instance for given PchIndex and HeciFunction in HeciAccess.

  @param[in]      HeciAccess            Heci Access that parameters HeciAccess has to match
  @param[in,out]  HeciCurrentInstance   Pointer to found instance id

  @retval HECI_TRANSPORT                Pointer to next HeciTransport instance
  @retval NULL                          NULL if no HECI_TRANSPORT_PRIVATE is found
**/
HECI_TRANSPORT *
GetHeciTransportForHeciAccess (
  IN      HECI_ACCESS   *HeciAccess,
  IN OUT  UINTN         *HeciCurrentInstance
  )
{
  EFI_STATUS                Status;
  HECI_TRANSPORT_PRIVATE    *HeciTransportPrivate;
  UINTN                     HeciAccessPciSegment;
  UINTN                     HeciAccessPciBus;
  UINTN                     HeciAccessPciDevice;
  UINTN                     HeciAccessHeciFunction;
  UINTN                     HeciInstance;

  Status = HeciAccess->PciIo.GetLocation (
                               &(HeciAccess->PciIo),
                               &HeciAccessPciSegment,
                               &HeciAccessPciBus,
                               &HeciAccessPciDevice,
                               &HeciAccessHeciFunction
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Status: %r\n", __FUNCTION__, Status));
    return NULL;
  }

  HeciInstance = 0;
  HeciTransportPrivate = (HECI_TRANSPORT_PRIVATE *) GetFirstHeciTransport ();

  while ((HeciTransportPrivate != NULL) &&
         ((HeciAccessPciSegment   != HeciTransportPrivate->PciSegment) ||
          (HeciAccessPciBus       != HeciTransportPrivate->PciBus)     ||
          (HeciAccessPciDevice    != HeciTransportPrivate->PciDevice)  ||
          (HeciAccessHeciFunction != HeciTransportPrivate->PciFunction))) {
    HeciInstance++;
    HeciTransportPrivate = (HECI_TRANSPORT_PRIVATE *) GetNextHeciTransport ((HECI_TRANSPORT *) HeciTransportPrivate);

    if (HeciTransportPrivate == NULL) {
      break;
    }
  }
  if (HeciCurrentInstance != NULL) {
    *HeciCurrentInstance = HeciInstance;
  }

  return (HECI_TRANSPORT *) HeciTransportPrivate;
}

/**
  Function returns HeciAccess for given HeciTransport
  Function implementation is deliver by parent module to HeciTransportCore library

  @param[in]  HeciTransport   HeciTransport for getting HeciAccess

  @retval HECI_ACCESS         On success valid pointer to linked HeciAccess is returned
  @retval NULL                If HeciAccess can't be found, NULL is returned
**/
HECI_ACCESS *
GetHeciAccess (
  IN  HECI_TRANSPORT_PRIVATE    *HeciTransport
 )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  VOID          *MemoryDiscoveredPpi;

  HeciAccess = NULL;

  if (HeciTransport == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - HeciTransport is NULL!\n", __FUNCTION__));
    return NULL;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             (VOID **) &MemoryDiscoveredPpi
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (
               &gHeciAccessPpiGuid,
               HeciTransport->HeciAccessIndex,
               NULL,
               (VOID **) &HeciAccess
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Could not find HeciAccess with given index!\n", __FUNCTION__));
      return NULL;
    }

    return HeciAccess;
  }

  return HeciTransport->HeciAccess;
}
