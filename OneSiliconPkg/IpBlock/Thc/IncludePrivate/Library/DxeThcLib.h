/** @file
  Header for DXE Thc library

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
#ifndef _DXE_THC_LIB_H_
#define _DXE_THC_LIB_H_

#include <Base.h>
#include <Protocol/ThcResetProtocol.h>

/**
  This function controls output level of THC SPI reset pin.

  @param[in] This         Pointer to the THC_RESET_PROTOCOL instance.
  @param[in] InstanceId   SPI instance id
  @param[in] Value        Output value
                            0: OutputLow, 1: OutputHigh

  @retval     Status
**/
EFI_STATUS
EFIAPI
ThcResetSetOutputValue (
  IN THC_RESET_PROTOCOL  *This,
  IN UINT8               InstanceId,
  IN UINT32              Value
  );

#endif //_DXE_THC_LIB_H_
