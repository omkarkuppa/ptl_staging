/** @file
  Header file for Gpio Services library for SMM Runtime Access.

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
#ifndef _GPIOV2_SMM_ACCESS_LIB_H_
#define _GPIOV2_SMM_ACCESS_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <GpioV2Pad.h>
#include <GpioV2Config.h>
#include <GpioV2Services.h>

/**
  Get Gpio Services instance for PCH

  @param[in]   Hid                  ACPI Hardware ID of the GPIO Controller, please refer to GpioV2PinsXXX.h (XXX - name of the SoC)
  @param[in]   Uid                  Unique Identifier for of GPIO particular type (please refer to GpioReadMeYYY.md (YYY- name of the Product)
  @param[out]  **GpioServices       Pointer to pointer for GpioServices structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - GpioServices is NULL
**/
EFI_STATUS
SmmGetGpioAccess (
  IN  CHAR8                   *Hid,
  IN  UINT32                  Uid,
  OUT GPIOV2_SERVICES         **GpioServices
  );

/**
  This function returns all instances of GPIO services.
  It can be called with GpioServices parameter as NULL to get the count of GPIO service instances.

  @param[in] GpioServices       Pointer to array of GPIO services
  @param[in] GpioServicesCount  Pointer to count of GPIO services

  @retval EFI_SUCCESS    Function completed successfuly
          EFI_NOT_FOUND  No GPIO services instance has been found
**/
EFI_STATUS
SmmGpioV2GetAllServices (
  OUT GPIOV2_SERVICES  **GpioServices,
  OUT UINT32           *GpioServicesCount
  );

#endif // _GPIOV2_SMM_ACCESS_LIB_H_
