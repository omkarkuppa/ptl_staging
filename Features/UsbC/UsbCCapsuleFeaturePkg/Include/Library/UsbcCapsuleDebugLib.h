/** @file
  Header file of USBC Capsule Debug Protocol Library functions

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
#ifndef _USBC_CAPSULE_DEBUG_LIB_H_
#define _USBC_CAPSULE_DEBUG_LIB_H_

#include <Uefi.h>
#include <Usb4Debug/Usb4DebugProtocol.h>
#include <Usb4Debug/Usb4LogEvents.h>


/**
  Write Log data to the next available Log entry in Log buffer
  Protocol DebugLevel controls what level of Log data can be written to Log buffer

  @param[in] LogLevel  - Log level associated with Log data
  @param[in] EventCode - Event code of Log data
  @param[in] EvtArg0   - Argument 0 of Log data
  @param[in] EvtArg1   - Argument 1 of Log data

  @retval EFI_SUCCESS           - Write Log data to Log buffer successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_OUT_OF_RESOURCES  - Log buffer is full
**/
EFI_STATUS
EFIAPI
CapsuleLogWrite (
  IN UINT8                      LogLevel,
  IN UINT16                     EventCode,
  IN UINT32                     EvtArg0,
  IN UINT32                     EvtArg1
  );

/**
  Install USBC Capsule Debug Protocol.
**/
VOID
InstallCapsuleDebugProtocol (
  VOID
  );

#endif
