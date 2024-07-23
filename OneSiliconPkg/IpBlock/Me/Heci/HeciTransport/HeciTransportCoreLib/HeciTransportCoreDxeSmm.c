 /** @file
  HECI Transport Protocol DXE SMM driver

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciAccessProtocol.h>

/**
  Get next HECI Transport

  @param[in]  This    Pointer to HECI Transport protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciTransport protocol instance
**/
HECI_TRANSPORT *
EFIAPI
GetNextHeciTransport (
  IN  HECI_TRANSPORT    *This
  )
{
  if (This == NULL) {
    return NULL;
  }

  return ((HECI_TRANSPORT_PRIVATE *) This)->HeciTransportNext;
}

/**
  Function returns HeciAccess for given HeciTransport

  @param[in]  HeciTransport   HeciTransport for getting HeciAccess

  @retval On success valid pointer to linked HeciAccess
  @retval NULL if HeciAccess couldn't be found
**/
HECI_ACCESS *
GetHeciAccess (
  HECI_TRANSPORT_PRIVATE    *HeciTransport
  )
{
  if (HeciTransport == NULL) {
    return NULL;
  }

  return HeciTransport->HeciAccess;
}