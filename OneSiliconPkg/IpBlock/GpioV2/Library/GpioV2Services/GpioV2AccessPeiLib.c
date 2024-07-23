/** @file
  This file contains code to get acces to GpioV2Controller in PEI phase

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
#include <Library/PeiServicesLib.h>
#include <Register/GpioV2ChipsetId.h>
#include <GpioV2Services.h>
#include <GpioV2Pwm.h>

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
  IN  CHAR8              *Hid,
  IN  UINT32             Uid,
  OUT GPIOV2_SERVICES    **GpioServices
  )
{
  EFI_STATUS          Status;
  UINT32              GpioControllerIndex;

  if (GpioServices == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (GpioControllerIndex = 0;; GpioControllerIndex++) {
    Status = PeiServicesLocatePpi (
      &gGpioV2PpiGuid,
      GpioControllerIndex,
      NULL,
      (VOID **) GpioServices
    );

    if (EFI_ERROR (Status)) {
      break;
    }

    if (CompareMem ((*GpioServices)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
       (*GpioServices)->Uid == Uid) {
      return EFI_SUCCESS;
    }
  }

  *GpioServices = NULL;

  return EFI_NOT_FOUND;
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
  UINT32           GpioServicesCnt;
  GPIOV2_SERVICES  *GpioServicesPtr;

  if (GpioServicesCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GpioServicesPtr = NULL;
  for (GpioServicesCnt = 0;; GpioServicesCnt++) {
    Status = PeiServicesLocatePpi (
      &gGpioV2PpiGuid,
      GpioServicesCnt,
      NULL,
      (VOID **) &GpioServicesPtr
      );
    if (EFI_ERROR (Status)) {
      if (GpioServicesCnt == 0) {
        *GpioServicesCount = 0;
        return EFI_NOT_FOUND;
      } else {
        break;
      }
    } else {
      if (GpioServices != NULL) {
        GpioServices[GpioServicesCnt] = GpioServicesPtr;
      }
    }
  }

  *GpioServicesCount = GpioServicesCnt;
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
  UINT32              GpioControllerIndex;

  if (Pwm == NULL || Hid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (GpioControllerIndex = 0;; GpioControllerIndex++) {
    Status = PeiServicesLocatePpi (
      &gGpioV2PwmPpiGuid,
      GpioControllerIndex,
      NULL,
      (VOID **)Pwm
    );

    if (EFI_ERROR (Status)) {
      break;
    }

    if (CompareMem ((*Pwm)->Hid, Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
       (*Pwm)->Uid == Uid) {
      return EFI_SUCCESS;
    }
  }

  *Pwm = NULL;

  return EFI_NOT_FOUND;
}