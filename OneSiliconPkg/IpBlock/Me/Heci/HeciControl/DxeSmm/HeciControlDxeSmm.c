/** @file
  HECI control protocol driver (Dxe, Smm)

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
#include <Guid/EventGroup.h>
#include <Guid/EventLegacyBios.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Register/HeciRegs.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/MeUtilsLib.h>

#define HECI_CONTROL_DEBUG   "[HECI Control]"

/**
  HECI driver entry point used to initialize support for the HECI device
  for DXE

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Protocol installed successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate protocol data
**/
EFI_STATUS
EFIAPI
HeciControlEntryPoint (
  IN EFI_HANDLE                  ImageHandle,
  IN EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS              Status;
  HECI_CONTROL_PRIVATE    *HeciControl;
  EFI_GUID                *SelectedHeciGuid;

  if (MeIsInSmm (NULL)) {
    SelectedHeciGuid = &gHeciControlSmmProtocolGuid;
  } else {
    SelectedHeciGuid = &gHeciControlProtocolGuid;
  }

  if (GetFirstHeciTransport () == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Control Protocol]  ERROR: Can't find HECI Transport\n"
      ));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
  DEBUG ((DEBUG_INFO, " %a\n", __FUNCTION__));

  HeciControl = HeciAllocateMemory (sizeof (HECI_CONTROL_PRIVATE));
  if (HeciControl != NULL) {
    SetHeciControlFunctions (HeciControl);
    HeciControlInitalizeAllCommunicationDevices (HeciControl);

    Status = InstallHeciProtocol (
               SelectedHeciGuid,
               (VOID*)HeciControl
               );
    if (EFI_ERROR (Status)) {
      HeciFreeMemory (HeciControl);
    }
  } else {
    Status = EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG));
  DEBUG ((DEBUG_INFO, " HECI Protocol Exit : %r.\n", Status));

  return Status;
}
