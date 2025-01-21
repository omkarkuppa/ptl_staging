/** @file
  This file contains code to get acces to GpioV2Controller in DXE phase

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

#include <Uefi/UefiBaseType.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <GpioV2ControllerInterface.h>
#include <Register/GpioV2ChipsetId.h>

#include <PiDxe.h>
#include <GpioV2Services.h>


/**
  Get Gpio Services instance for PCH

  @param[in]   Hid                  ACPI Hardware ID of the GPIO Controller, please refer to GpioV2PinsXXX.h (XXX - name of the SoC)
  @param[in]   Uid                  Unique Identifier for of GPIO particular type (please refer to GpioReadMeYYY.md (YYY- name of the Product)
  @param[out]  GpioServices         Pointer to pointer for GPIO GpioServices structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_NOT_FOUND         - GPIO access not found
              EFI_INVALID_PARAMETER - GpioServices or Hid is NULL
**/
EFI_STATUS
GpioV2GetAccess (
  IN  CHAR8               *Hid,
  IN  UINT32              Uid,
  OUT GPIOV2_SERVICES     **GpioServices
  )
{
  EFI_STATUS    Status;
  UINTN         HandleCount;
  EFI_HANDLE    *HandleBuffer;
  UINTN         HandleIndex;

  DEBUG ((DEBUG_VERBOSE, "[GPIOV2][DXE]: GpioV2GetAccess start\n"));

  if (GpioServices == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gGpioV2ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (HandleCount == 0)) {
    return EFI_NOT_FOUND;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2ProtocolGuid,
                    (VOID **)GpioServices);
    if (!EFI_ERROR (Status)) {
      if (CompareMem ((*GpioServices)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
         (*GpioServices)->Uid == Uid) {
        break;
      }
    }
  }

  if (HandleIndex == HandleCount) {
    Status = EFI_NOT_FOUND;
    *GpioServices = NULL;
  }

  gBS->FreePool (HandleBuffer);
  DEBUG ((DEBUG_VERBOSE, "[GPIOV2][DXE]: GpioV2GetAccess end (%r)\n", Status));

  return Status;
}

/**
  This function returns all instances of GPIO services.
  It can be called with GpioServices parameter as NULL to get the count of GPIO service instances.

  @param[in] GpioServices       Pointer to array of GPIO services
  @param[in] GpioServicesCount  Pointer to count of GPIO services

  @retval EFI_SUCCESS           Function completed successfuly
          EFI_NOT_FOUND         No GPIO services instance has been found
          EFI_INVALID_PARAMETER GpioServicesCount is NULL
**/
EFI_STATUS
GpioV2GetAllServices (
  OUT GPIOV2_SERVICES  **GpioServices,
  OUT UINT32           *GpioServicesCount
  )
{
  EFI_STATUS       Status;
  UINTN            HandleCount;
  EFI_HANDLE       *HandleBuffer;
  UINTN            HandleIndex;
  GPIOV2_SERVICES  *GpioServicesPtr;

  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: %a start\n", __FUNCTION__));

  if (GpioServicesCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gGpioV2ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  DEBUG ((DEBUG_INFO, "GpioServicesCount: 0x%x\n", HandleCount));
  if (EFI_ERROR (Status) || (HandleCount == 0)) {
    *GpioServicesCount = 0;
    return EFI_NOT_FOUND;
  }

  if (GpioServices == NULL) {
    *GpioServicesCount = (UINT32) HandleCount;
    gBS->FreePool (HandleBuffer);
    return EFI_SUCCESS;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2ProtocolGuid,
                    (VOID **)&GpioServicesPtr
                    );
    if (EFI_ERROR (Status)) {
      if (HandleIndex == 0) {
        *GpioServicesCount = 0;
        gBS->FreePool (HandleBuffer);
        return EFI_NOT_FOUND;
      } else {
        break;
      }
    } else {
      GpioServices[HandleIndex] = GpioServicesPtr;
    }
  }
  *GpioServicesCount = (UINT32) HandleCount;
  gBS->FreePool (HandleBuffer);
  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: %a end\n", __FUNCTION__));

  return EFI_SUCCESS;
}


/**
  Get Gpio PWM Services instance

  @param[in]   Hid                  ACPI Hardware ID of the GPIO Controller, please refer to GpioV2PinsXXX.h (XXX - name of the SoC)
  @param[in]   Uid                  Unique Identifier for of GPIO particular type (please refer to GpioReadMeYYY.md (YYY- name of the Product)
  @param[out]  Pwm                  Pointer to pointer for GPIO PWM structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_NOT_FOUND         - PWM access not found
              EFI_INVALID_PARAMETER - Pwm or Hid is NULL
**/
EFI_STATUS
GpioV2GetPwmAccess (
  IN  CHAR8              *Hid,
  IN  UINT32             Uid,
  OUT GPIOV2_PWM         **Pwm
  )
{
  EFI_STATUS          Status;
  UINTN               HandleCount;
  EFI_HANDLE          *HandleBuffer;
  UINTN               HandleIndex;

  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: GpioV2GetPwmAccess start\n"));

  if (Pwm == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  HandleBuffer = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gGpioV2PwmProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (HandleCount == 0)) {
    return EFI_NOT_FOUND;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2PwmProtocolGuid,
                    (VOID **)Pwm);
    if (!EFI_ERROR (Status)) {
      if (CompareMem ((*Pwm)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
         (*Pwm)->Uid == Uid) {
        break;
      }
    }
  }

  if (HandleIndex == HandleCount) {
    Status = EFI_NOT_FOUND;
    *Pwm = NULL;
  }
  if (HandleBuffer) {
    gBS->FreePool (HandleBuffer);
  }
  DEBUG ((DEBUG_INFO, "[GPIOV2][DXE]: GpioV2GetPwmAccess end (%r)\n", Status));

  return Status;
}
