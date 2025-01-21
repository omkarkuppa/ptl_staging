 /** @file
  HECI Transport Protocol DXE SMM driver

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>

#define HECI_TRANSPORT_DEBUG    "[HECI Transport]"

/**
  HECI driver entry point used to initialize support for the HECI device
  for DXE/SMM

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Returned for succeed execution
  @retval EFI_OUT_OF_RESOURCES    Can't allocate protocol data
  @retval EFI_UNSUPPORTED         Not supported when driver is called in non SMM context
  @retval EFI_DEVICE_ERROR        No HeciAccess protocol has been found
**/
EFI_STATUS
EFIAPI
HeciTransportEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                Status;
  HECI_TRANSPORT_PRIVATE    *HeciTransport;
  HECI_ACCESS               *HeciAccess;
  UINTN                     HeciTransportCounter;
  HECI_TRANSPORT_PRIVATE    *HeciTransportPrevious;
  HECI_TRANSPORT_PRIVATE    *HeciTransportRoot;
  EFI_GUID                  *SelectedHeciGuid;

  HeciTransportRoot = NULL;
  HeciTransportPrevious = NULL;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG));
  DEBUG ((DEBUG_INFO, " %a\n", __FUNCTION__));

  if (MeIsInSmm (NULL)) {
    SelectedHeciGuid = &gHeciTransportSmmProtocolGuid;
  } else {
    SelectedHeciGuid = &gHeciTransportProtocolGuid;
  }

  HeciTransportCounter = 0;
  HeciAccess = (HECI_ACCESS *) GetFirstHeciAccess ();

  while (HeciAccess != NULL) {
    HeciTransport = HeciAllocateMemory (sizeof (HECI_TRANSPORT_PRIVATE));

    if (HeciTransport == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }
    Status = HeciInitializeNewTransportInstance (
               HeciTransport,
               HeciAccess
               );
    if (!EFI_ERROR (Status)) {
      Status = InstallHeciProtocol (
                 SelectedHeciGuid,
                 (VOID *) HeciTransport
                 );
     }
    if (!EFI_ERROR (Status)) {
      // for first element get a root
      if (HeciTransportRoot == NULL) {
        HeciTransportRoot = HeciTransport;
      }
      // Link to next
      HeciTransport->HeciTransportRoot = HeciTransportRoot;
      if (HeciTransportPrevious != NULL) {
        HeciTransportPrevious->HeciTransportNext = HeciTransport;
      }
      HeciTransportPrevious = HeciTransport;
    } else {
      HeciFreeMemory (HeciTransport);
    }

    HeciTransportCounter++;
    HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess);
  }

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG));
  DEBUG ((
    DEBUG_INFO,
    " HECI Transport Protocol Exit : %r. (installed %d transport(s))\n",
    Status,
    HeciTransportCounter
    ));

  return Status;
}
