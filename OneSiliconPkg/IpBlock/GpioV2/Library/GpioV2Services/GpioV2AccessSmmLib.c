/** @file
  This file contains code to get acces to GpioV2Controller in DXE phase

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/GpioV2SmmAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <GpioV2ControllerInterface.h>
#include <Register/GpioV2ChipsetId.h>

#include <Pi/PiSmmCis.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/MmServicesTableLib.h>
#include <GpioV2Services.h>


/**
  Get Gpio Services instance for PCH in SMM

  @param[in]   Hid                  ACPI Hardware ID of the GPIO Controller, please refer to GpioV2PinsXXX.h (XXX - name of the SoC)
  @param[in]   Uid                  Unique Identifier for of GPIO particular type (please refer to GpioReadMeYYY.md (YYY- name of the Product)
  @param[out]  GpioServices         Pointer to pointer for GPIO GpioServices structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_NOT_FOUND         - GPIO access not found
              EFI_INVALID_PARAMETER - GpioServices or Hid is NULL
              EFI_OUT_OF_RESOURCES  - Out of resources
**/

EFI_STATUS
SmmGetGpioAccess (
  IN  CHAR8               *Hid,
  IN  UINT32              Uid,
  OUT GPIOV2_SERVICES     **GpioServices
  )
{
  EFI_STATUS    Status;
  UINTN         HandleCount;
  EFI_HANDLE    *HandleBuffer;
  UINTN         HandleIndex;
  UINTN         HandleBufferSize;

  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: GetSmmGpioAccess start\n"));

  if (GpioServices == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HandleBuffer     = NULL;
  HandleCount      = 0;
  HandleBufferSize = 0;

  /*
    SmmLocateHandleBuffer is not a member of SmmServiveTable hence we need
    to work here with SmmLocateHandle and imitate SmmLocateHandleBuffer
    functionality in order to locate all SMM protocol instance for GPIOV2.
  */

  Status = gMmst->MmLocateHandle (
                  ByProtocol,
                  &gGpioV2SmmProtocolGuid,
                  NULL,
                  &HandleBufferSize,
                  HandleBuffer
                  );
  if (EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL) {
    if (Status != EFI_INVALID_PARAMETER) {
      Status = EFI_NOT_FOUND;
    }
    return Status;
  }

  Status = gMmst->MmAllocatePool (EfiRuntimeServicesData, HandleBufferSize, (VOID**) &HandleBuffer);
  if (HandleBuffer == (EFI_HANDLE*) NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gMmst->MmLocateHandle (
                  ByProtocol,
                  &gGpioV2SmmProtocolGuid,
                  NULL,
                  &HandleBufferSize,
                  HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    gMmst->MmFreePool (HandleBuffer);
    return Status;
  }

  HandleCount = HandleBufferSize / sizeof(EFI_HANDLE);

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gMmst->MmHandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2SmmProtocolGuid,
                    (VOID **)GpioServices
                    );
    if (!EFI_ERROR (Status)) {
      if (CompareMem ((*GpioServices)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
         (*GpioServices)->Uid == Uid) {
        gMmst->MmFreePool (HandleBuffer);
        return EFI_SUCCESS;
      }
    }
  }
  
  *GpioServices = NULL;
  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: GetSmmGpioAccess end\n"));
  gMmst->MmFreePool (HandleBuffer);
  return EFI_NOT_FOUND;
}

/**
  This function returns all instances of GPIO services.
  It can be called with GpioServices parameter as NULL to get the count of GPIO service instances.

  @param[in] GpioServices       Pointer to array of GPIO services
  @param[in] GpioServicesCount  Pointer to count of GPIO services

  @retval EFI_SUCCESS           Function completed successfuly
          EFI_NOT_FOUND         No GPIO services instance has been found
          EFI_INVALID_PARAMETER GpioServicesCount or Hid is NULL
          EFI_OUT_OF_RESOURCES  Out of resources
**/
EFI_STATUS
SmmGpioV2GetAllServices (
  OUT GPIOV2_SERVICES  **GpioServices,
  OUT UINT32           *GpioServicesCount
  )
{
  EFI_STATUS       Status;
  GPIOV2_SERVICES  *GpioServicesPtr;
  UINTN            HandleCount;
  EFI_HANDLE       *HandleBuffer;
  UINTN            HandleIndex;
  UINTN            HandleBufferSize;

  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: %a start\n", __FUNCTION__));

  if (GpioServicesCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HandleBuffer     = NULL;
  HandleCount      = 0;
  HandleBufferSize = 0;

  Status = gMmst->MmLocateHandle (
                    ByProtocol,
                    &gGpioV2SmmProtocolGuid,
                    NULL,
                    &HandleBufferSize,
                    HandleBuffer
                    );
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    if (Status != EFI_INVALID_PARAMETER) {
      Status = EFI_NOT_FOUND;
    }
    *GpioServicesCount = 0;
    return Status;
  }

  HandleCount = HandleBufferSize / sizeof (EFI_HANDLE);
  DEBUG ((DEBUG_INFO, "GpioServicesCount: 0x%x\n", HandleCount));
  if (GpioServices == NULL) {
    *GpioServicesCount = (UINT32) HandleCount;
    return EFI_SUCCESS;
  }

  Status = gMmst->MmAllocatePool (EfiRuntimeServicesData, HandleBufferSize, (VOID **) &HandleBuffer);
  if (HandleBuffer == (EFI_HANDLE*) NULL) {
    *GpioServicesCount = 0;
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gMmst->MmLocateHandle (
                  ByProtocol,
                  &gGpioV2SmmProtocolGuid,
                  NULL,
                  &HandleBufferSize,
                  HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    gMmst->MmFreePool (HandleBuffer);
    *GpioServicesCount = 0;
    return Status;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gMmst->MmHandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2SmmProtocolGuid,
                    (VOID **)&GpioServicesPtr
                    );
    if (EFI_ERROR (Status)) {
      if (HandleIndex == 0) {
        *GpioServicesCount = 0;
        gMmst->MmFreePool (HandleBuffer);
        return EFI_NOT_FOUND;
      } else {
        break;
      }
    } else {
      GpioServices[HandleIndex] = GpioServicesPtr;
    }
  }
  *GpioServicesCount = (UINT32) HandleCount;

  gMmst->MmFreePool (HandleBuffer);

  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: %a end\n", __FUNCTION__));

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
              EFI_OUT_OF_RESOURCES  - Out of resources
**/
EFI_STATUS
SmmGpioV2GetPwmAccess (
  IN  CHAR8              *Hid,
  IN  UINT32             Uid,
  OUT GPIOV2_PWM         **Pwm
  )
{
  EFI_STATUS          Status;
  UINTN               HandleCount;
  EFI_HANDLE          *HandleBuffer;
  UINTN               HandleIndex;
  UINTN               HandleBufferSize;

  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: SmmGpioV2GetPwmAccess start\n"));

  if (Pwm == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HandleBuffer     = NULL;
  HandleCount      = 0;
  HandleBufferSize = 0;

  /*
    SmmLocateHandleBuffer is not a member of SmmServiveTable hence we need
    to work here with SmmLocateHandle and imitate SmmLocateHandleBuffer
    functionality in order to locate all SMM protocol instance for GPIOV2.
  */

  Status = gMmst->MmLocateHandle (
                  ByProtocol,
                  &gGpioV2SmmPwmProtocolGuid,
                  NULL,
                  &HandleBufferSize,
                  HandleBuffer
                  );
  if (EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL) {
    if (Status != EFI_INVALID_PARAMETER) {
      Status = EFI_NOT_FOUND;
    }
    return Status;
  }

  Status = gMmst->MmAllocatePool (EfiRuntimeServicesData, HandleBufferSize, (VOID**) &HandleBuffer);
  if (HandleBuffer == (EFI_HANDLE*) NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gMmst->MmLocateHandle (
                  ByProtocol,
                  &gGpioV2SmmPwmProtocolGuid,
                  NULL,
                  &HandleBufferSize,
                  HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    gMmst->MmFreePool (HandleBuffer);
    return Status;
  }

  HandleCount = HandleBufferSize / sizeof(EFI_HANDLE);

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gMmst->MmHandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gGpioV2SmmPwmProtocolGuid,
                    (VOID **)Pwm
                    );
    if (!EFI_ERROR (Status)) {
      if (CompareMem ((*Pwm)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
         (*Pwm)->Uid == Uid) {
        gMmst->MmFreePool (HandleBuffer);
        return EFI_SUCCESS;
      }
    }
  }

  *Pwm = NULL;
  DEBUG ((DEBUG_INFO, "[GPIOV2][SMM]: SmmGpioV2GetPwmAccess end\n"));
  gMmst->MmFreePool (HandleBuffer);
  return EFI_NOT_FOUND;
}