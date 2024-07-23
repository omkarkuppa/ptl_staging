/** @file
  PEI Itss policy library.

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
#ifndef _PEI_ITSS_POLICY_LIB_H_
#define _PEI_ITSS_POLICY_LIB_H_

#include <Uefi.h>
#include <InterruptConfig.h>

/**
  Add Interrupt ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
InterruptAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  );


/**
  Loads the Device Interrupt Configuration settings for the platform

  @param[out] NumOfDevIntConfig      Pointer to the length of DevIntConfig
  @param[out] DevIntConfig           Array which stores PCH device interrupt settings

  @retval None
**/
VOID
EFIAPI
LoadDeviceInterruptConfig (
  OUT UINT8                          *NumOfDevIntConfig,
  OUT PCH_DEVICE_INTERRUPT_CONFIG    DevIntConfig[]
  );

#endif // _PEI_ITSS_POLICY_LIB_H_
