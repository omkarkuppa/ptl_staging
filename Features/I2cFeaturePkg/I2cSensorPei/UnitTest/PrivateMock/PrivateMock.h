/**@file
    Mock function of the I2C Sensor PEIM.
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef PRIVATE_MOCK_H
#define PRIVATE_MOCK_H

#include <Uefi.h>
#include <BootServicesMock.h>
#include <Protocol/I2cIo.h>
#include <PiPei.h>

/**
  This service enables a given PEIM to register an interface into the PEI Foundation.
  @param[in]  PpiList  A pointer to the list of interfaces that the caller shall install.
  @retval EFI_SUCCESS            - The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  - The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER  - Any of the PEI PPI descriptors in the list do not have the
                                   EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES   - There is no additional space in the PPI database.
**/
EFI_STATUS
EFIAPI
PeiServicesInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *PpiList
  );

#endif
