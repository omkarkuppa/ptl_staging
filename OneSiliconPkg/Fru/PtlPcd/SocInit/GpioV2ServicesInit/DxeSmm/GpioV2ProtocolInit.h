/** @file
  Header file for Common Defines of GPIOV2 DXE and SMM

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

#ifndef _GPIOV2_PROTOCOL_INIT_H_
#define _GPIOV2_PROTOCOL_INIT_H_

#include <GpioV2ControllerInterface.h>
/**
  This function initializes GPIOV2_INTERFACE Structure.

  @param[in]  GpioInterface     Pointer to GPIOV2_INTERFACE.

  @retval     EFI_SUCCESS                The function completed successfully.
  @retval     EFI_INVALID_PARAMETER      Gpio Interface Pointer is NULL.
  @retval     EFI_OUT_OF_RESOURCES       Unabled to allocate memory.
  @retval     EFI_NOT_FOUND              Unable to Get P2SB access Pointer.
**/
EFI_STATUS
InternalGpioInterfaceConstructor (
  IN     GPIOV2_INTERFACE      *GpioInterface
  );

#endif // _GPIOV2_PROTOCOL_INIT_H_
